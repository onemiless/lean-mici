#!/usr/bin/env python3
"""Bounded C3XL live camera/model demo, isolated IPC/Params; no vehicle control."""
import argparse
import json
import math
import os
from pathlib import Path
import platform
import signal
import subprocess
import sys
import time
import traceback

ROOT = Path(__file__).resolve().parents[2]
GLOBAL_PARAMS = Path('/data/params/d')
BLOCKED = {'manager', 'camerad', 'modeld', 'bigmodeld', 'card', 'controlsd', 'pandad'}


def safe_global_state():
  state = {key: (GLOBAL_PARAMS / key).read_text().strip() for key in ('IsOffroad', 'IsEngaged')}
  assert state == {'IsOffroad': '1', 'IsEngaged': '0'}, state
  return state


def competing_processes():
  conflicts = []
  for proc in Path('/proc').glob('[0-9]*'):
    if int(proc.name) == os.getpid():
      continue
    try:
      tokens = [proc.joinpath('comm').read_text().strip(),
                *proc.joinpath('cmdline').read_bytes().decode(errors='replace').split('\0')]
    except (FileNotFoundError, ProcessLookupError):
      continue
    names = {Path(token).name.removesuffix('.py').split('.')[-1] for token in tokens}
    if names & BLOCKED:
      conflicts.append({'pid': int(proc.name), 'names': sorted(names & BLOCKED)})
  return conflicts


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument('--output', type=Path, required=True)
  parser.add_argument('--timeout', type=int, default=90, choices=range(60, 91))
  args = parser.parse_args()
  args.output = args.output.resolve()
  args.output.parent.mkdir(parents=True, exist_ok=True)
  report = {'passed': False, 'scope': 'live cameras/model inference only; synthetic demo CP, stopped car, disabled control, zero-rpy calibrated input',
            'target_valid_samples': 120, 'deadline_s': args.timeout, 'children': {}, 'cleanup': {}}
  children, logs = {}, []
  signal.signal(signal.SIGTERM, lambda *_: (_ for _ in ()).throw(KeyboardInterrupt()))
  try:
    assert platform.system() == 'Linux' and platform.machine() == 'aarch64', 'requires Linux aarch64 device'
    assert not os.environ.get('OPENPILOT_PREFIX'), 'refuse inherited prefix; safety checks must use global state'
    assert Path('/data/hardware_profile').read_text().strip() == 'c3xl', 'requires actual C3XL profile'
    report['global_before'] = safe_global_state()
    conflicts = competing_processes()
    assert not conflicts, f'processes already running: {conflicts}'
    from openpilot.common.prefix import OpenpilotPrefix
    from openpilot.common.params import Params
    from openpilot.common.hardware import HARDWARE
    from openpilot.cereal import messaging
    device_type = HARDWARE.get_device_type()
    assert device_type in ('tici', 'tizi'), f'unexpected C3XL device type {device_type}'
    report['device_type'] = device_type
    with OpenpilotPrefix() as prefix:
      try:
        report['ipc_prefix'] = prefix.prefix
        params = Params()
        params.put_bool('BigmodelToggle', False, block=True)
        params.put_bool('IsOffroad', True, block=True)
        params.put_bool('IsEngaged', False, block=True)
        services = ['deviceState', 'extrinsicsCalibration', 'carState', 'carControl', 'lateralDelay']
        pm = messaging.PubMaster(services)
        received = ['modelV2', 'cameraOdometry', 'narrowRoadCameraState', 'wideRoadCameraState']
        sockets = {name: messaging.sub_sock(name, timeout=0) for name in received}
        counts = {name: 0 for name in received}
        valid = dict(counts)
        samples, sensors, qcom_fds = [], {}, set()
        report.update(counts=counts, valid_counts=valid, sensors=sensors, samples=samples)
        pythonpath = os.pathsep.join(str(ROOT / p) for p in ('', 'opendbc_repo', 'msgq_repo', 'rednose_repo', 'tinygrad_repo'))
        env = {**os.environ, 'SUNNYPILOT_HARDWARE_PROFILE': 'c3xl', 'DISABLE_DRIVER': '1', 'DEVICE': 'QCOM',
               'PYTHONPATH': pythonpath + os.pathsep + os.environ.get('PYTHONPATH', '')}
        commands = {'camerad': [str(ROOT / 'openpilot/system/camerad/camerad')],
                    'modeld': [sys.executable, '-m', 'openpilot.selfdrive.modeld.modeld', '--demo']}
        for name, command in commands.items():
          path = args.output.with_name(args.output.stem + '-' + name + '.log')
          stream = path.open('w')
          logs.append(stream)
          children[name] = subprocess.Popen(command, cwd=ROOT / 'openpilot/system/camerad' if name == 'camerad' else ROOT,
                                            env=env, stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
          report['children'][name] = {'pid': children[name].pid, 'command': command, 'log': str(path)}
        start = last_safety = time.monotonic()
        deadline = start + args.timeout
        while time.monotonic() < deadline:
          now = time.monotonic()
          if now - last_safety >= 1:
            safe_global_state()
            last_safety = now
          for name, child in children.items():
            assert child.poll() is None, f'{name} exited {child.returncode}; inspect child log'
          for fd in Path(f'/proc/{children["modeld"].pid}/fd').iterdir():
            try:
              target = os.readlink(fd)
              if target.startswith('/dev/kgsl'):
                qcom_fds.add(target)
            except FileNotFoundError:
              pass
          for name in services:
            msg = messaging.new_message(name, valid=True)
            value = getattr(msg, name)
            if name == 'deviceState':
              value.deviceType, value.started = device_type, False
            elif name == 'extrinsicsCalibration':
              value.calStatus, value.rpyCalib, value.wideFromDeviceEuler = 'calibrated', [0., 0., 0.], [0., 0., 0.]
              value.calPerc, value.validBlocks = 100, 20
            elif name == 'carState':
              value.vEgo, value.standstill = 0., True
            elif name == 'carControl':
              value.enabled, value.latActive, value.longActive = False, False, False
            elif name == 'lateralDelay':
              value.lateralDelay = .2
            pm.send(name, msg)
          for name, sock in sockets.items():
            for msg in messaging.drain_sock(sock):
              counts[name] += 1
              valid[name] += int(msg.valid)
              value = getattr(msg, name)
              if name.endswith('CameraState'):
                sensors[name] = str(value.sensor)
                assert str(value.sensor) == 'ox03c10', f'{name} sensor {value.sensor}'
              elif name == 'cameraOdometry':
                for field in ('trans', 'rot', 'transStd', 'rotStd'):
                  values = list(getattr(value, field))
                  assert len(values) == 3 and all(math.isfinite(v) for v in values), f'invalid odometry {field}'
              elif name == 'modelV2':
                for field in ('position', 'velocity'):
                  vector = getattr(value, field)
                  for axis in ('x', 'y', 'z', 't'):
                    values = list(getattr(vector, axis))
                    assert values and all(math.isfinite(v) for v in values), f'invalid {field}.{axis}'
                assert all(math.isfinite(v) for v in (value.action.desiredAcceleration, value.action.desiredCurvature,
                                                       value.modelExecutionTime, value.frameDropPerc)), 'nonfinite model values'
                assert not value.big, 'unexpected remote big model'
                samples.append({'received_s': now - start, 'valid': bool(msg.valid), 'frame_id': value.frameId,
                                'execution_ms': value.modelExecutionTime * 1000, 'frame_drop_percent': value.frameDropPerc})
          if valid['modelV2'] >= 120 and valid['cameraOdometry'] >= 120 and all(valid[s] >= 120 for s in sensors) and len(sensors) == 2:
            break
          time.sleep(.02)
        report['elapsed_s'] = time.monotonic() - start
        report['qcom_device_descriptors'] = sorted(qcom_fds)
        assert valid['modelV2'] >= 120 and valid['cameraOdometry'] >= 120, 'insufficient valid model/odometry messages before deadline'
        assert len(sensors) == 2 and all(valid[s] >= 120 for s in sensors), 'insufficient dual camera frames'
        assert qcom_fds, 'no live modeld KGSL device descriptor proof'
        frames = [s['frame_id'] for s in samples]
        assert all(b > a for a, b in zip(frames, frames[1:])), 'duplicate/reset model frame IDs'
        span = samples[-1]['received_s'] - samples[0]['received_s']
        assert span > 0, 'no measurable model sampling interval'
        report['model_hz'] = (len(samples) - 1) / span
        report['mean_execution_ms'] = sum(s['execution_ms'] for s in samples) / len(samples)
        report['observed_frame_drop_ratio'] = 1 - len(frames) / (frames[-1] - frames[0] + 1)
        report['mean_reported_frame_drop_percent'] = sum(s['frame_drop_percent'] for s in samples) / len(samples)
        report['global_after'] = safe_global_state()
        report['passed'] = True
      finally:
        for name, child in children.items():
          if child.poll() is None:
            try:
              os.killpg(child.pid, signal.SIGTERM)
            except ProcessLookupError:
              pass
        for name, child in children.items():
          try:
            child.wait(timeout=5)
          except subprocess.TimeoutExpired:
            try:
              os.killpg(child.pid, signal.SIGKILL)
            except ProcessLookupError:
              pass
            try:
              child.wait(timeout=5)
            except subprocess.TimeoutExpired:
              report['passed'] = False
          report['cleanup'][name] = {'pid': child.pid, 'exit_code': child.returncode, 'stopped': child.poll() is not None}
  except BaseException:
    report['passed'] = False
    report['error'] = traceback.format_exc()
  finally:
    for stream in logs:
      stream.close()
    args.output.write_text(json.dumps(report, indent=2) + '\n')
  print(json.dumps({'passed': report['passed'], 'artifact': str(args.output)}))
  return 0 if report['passed'] else 1


if __name__ == '__main__':
  raise SystemExit(main())
