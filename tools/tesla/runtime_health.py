#!/usr/bin/env python3
"""Observe live offroad process stability; never starts processes or sends CAN."""
import argparse
import json
from pathlib import Path
import subprocess
import time

from openpilot.cereal import messaging
from openpilot.common.basedir import BASEDIR
from openpilot.common.params import Params
from openpilot.sunnypilot.hardware.profile import get_hardware_profile


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--seconds', type=float, default=30)
  parser.add_argument('--expected-head', required=True)
  parser.add_argument('--output', type=Path, required=True)
  args = parser.parse_args()
  assert 5 <= args.seconds <= 300
  services = ['deviceState', 'pandaStates', 'managerState']
  sm = messaging.SubMaster(services)
  counts = dict.fromkeys(services, 0)
  pids, violations, wanted = {}, set(), set()
  last_pandas = []
  deadline = time.monotonic() + args.seconds
  while time.monotonic() < deadline:
    sm.update(100)
    for service in services:
      counts[service] += int(sm.updated[service])
      if sm.updated[service] and not sm.valid[service]:
        violations.add(service + ' invalid')
    if sm.updated['deviceState'] and sm['deviceState'].started:
      violations.add('device started')
    if sm.updated['pandaStates']:
      last_pandas = [p.to_dict() for p in sm['pandaStates']]
      for panda in sm['pandaStates']:
        if panda.controlsAllowed or panda.ignitionLine or panda.ignitionCan:
          violations.add('Panda controls or ignition active')
        if panda.faultStatus != 'none' or panda.faults:
          violations.add('Panda fault')
    if sm.updated['managerState']:
      for process in sm['managerState'].processes:
        if process.shouldBeRunning:
          wanted.add(process.name)
          if not process.running or process.pid <= 0:
            violations.add(process.name + ' unexpectedly stopped')
          pids.setdefault(process.name, set()).add(process.pid)
          if len(pids[process.name]) > 1:
            violations.add(process.name + ' restarted')
  profile = get_hardware_profile().value
  required = {'ui', 'pandad', 'hardwared'} | ({'alert_output'} if profile == 'c3xl' else set())
  for name in required - wanted:
    violations.add(name + ' not managed as running')
  for service in services:
    if counts[service] < args.seconds / 2 or not sm.valid[service] or not sm.alive[service]:
      violations.add(service + ' missing, invalid or stale')
  if not last_pandas:
    violations.add('no Panda')
  if profile == 'c3xl' and wanted & {'micd', 'soundd', 'dmonitoringd'}:
    violations.add('unsupported C3XL audio or driver process')
  params = Params()
  if not params.get_bool('IsOffroad') or params.get_bool('IsEngaged'):
    violations.add('Params not offroad/unengaged')
  head = subprocess.check_output(['git', '-C', BASEDIR, 'rev-parse', 'HEAD'], text=True).strip()
  if head != args.expected_head:
    violations.add('unexpected active commit')
  result = {'scope': 'live offroad stability only; no onroad or driving acceptance',
            'passed': not violations, 'violations': sorted(violations), 'head': head,
            'active_path': str(Path(BASEDIR).resolve()), 'profile': profile, 'seconds': args.seconds,
            'counts': counts, 'process_pids': {k: sorted(v) for k, v in pids.items()}, 'last_pandas': last_pandas}
  args.output.parent.mkdir(parents=True, exist_ok=True)
  args.output.write_text(json.dumps(result, indent=2) + '\n')
  print(json.dumps({'passed': result['passed'], 'violations': result['violations'], 'output': str(args.output)}))
  return int(bool(violations))


if __name__ == '__main__':
  raise SystemExit(main())
