#!/usr/bin/env python3
"""Deterministic CLI replay with synthetic command/camera IO; no device access."""
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import subprocess
import sys
import types
from unittest.mock import patch

root = Path(__file__).resolve().parents[2]
results = []
# Replay the postflash CLI's Python command runner in both explicitly different service states.
script = (root/'tools/tesla/agnos_postflash_check.sh').read_text().split("<<'PY'", 1)[1].split('\nPY\n', 1)[0]
script = script[script.index('\n') + 1:]
for stage, service in [('health', 'active'), ('camera', 'inactive')]:
  calls = []
  def command(cmd, **kwargs):
    calls.append(cmd)
    inactive = cmd[0] == 'systemctl' and service == 'inactive'
    return types.SimpleNamespace(returncode=3 if inactive else 0, stdout=service+'\n' if cmd[0]=='systemctl' else 'ok\n', stderr='')
  with patch.object(subprocess, 'run', command), patch.object(sys, 'argv', ['-', stage]), contextlib.redirect_stdout(io.StringIO()):
    try:
      exec(compile(script, 'postflash-cli', 'exec'), {})
    except SystemExit as exc:
      code = exc.code
  assert code == 0, (stage, code)
  camera_called = any('tools/tesla/camera_check.py' in call for call in calls)
  assert camera_called == (stage == 'camera'), (stage, calls)
  results.append({'stage': stage, 'service': service, 'exit': code, 'camera_called': camera_called})

# Replay the complete camera CLI with finite frame streams and a virtual clock.
params = types.ModuleType('openpilot.common.params')
params.Params = lambda: types.SimpleNamespace(get_bool=lambda k: k == 'IsOffroad')
messaging = types.ModuleType('openpilot.cereal.messaging')
cereal = types.ModuleType('openpilot.cereal'); cereal.messaging = messaging
profile = types.ModuleType('openpilot.sunnypilot.hardware.profile'); profile.has_driver_camera = lambda: False
mods = {'openpilot.common.params': params, 'openpilot.cereal': cereal, 'openpilot.cereal.messaging': messaging, 'openpilot.sunnypilot.hardware.profile': profile}
with patch.dict(sys.modules, mods):
  spec = importlib.util.spec_from_file_location('camera_check_replay', root/'tools/tesla/camera_check.py')
  camera = importlib.util.module_from_spec(spec); spec.loader.exec_module(camera)
  for case, expected in [('valid', 0), ('wrong_wide', 1), ('gap', 1), ('slow', 1), ('slow_delivery', 1), ('missing_wide', 1)]:
    now = [0.0]; counters = {};
    def subscribe(name, **kwargs):
      assert name in ('narrowRoadCameraState', 'wideRoadCameraState', 'cabinCameraState'), name
      return name
    messaging.sub_sock = subscribe
    def receive(name):
      if name == 'cabinCameraState' or (case == 'missing_wide' and name == 'wideRoadCameraState'):
        return None
      n = counters.get(name, 0); counters[name] = n + 1
      if n >= 100:
        return None
      state = types.SimpleNamespace(frameId=n + int(case=='gap' and n>=50), timestampSof=(n+1)*(100000000 if case=='slow' else 50000000), sensor='ar0231' if case=='wrong_wide' and name=='wideRoadCameraState' else 'ox03c10')
      return types.SimpleNamespace(**{name: state})
    messaging.recv_one_or_none = receive
    proc = types.SimpleNamespace(poll=lambda: None, terminate=lambda: None, wait=lambda **kw: 0, kill=lambda: None)
    with patch.object(Path, 'glob', lambda *a: []), patch.object(camera.subprocess, 'Popen', lambda *a, **kw: proc), patch.object(camera.time, 'monotonic', lambda: now[0]), patch.object(camera.time, 'sleep', lambda seconds: now.__setitem__(0, now[0]+(0.1 if case=='slow_delivery' else 0.05))), contextlib.redirect_stdout(io.StringIO()) as out:
      code = camera.main()
    assert code == expected and now[0] <= 45.1, (case, code, now)
    results.append({'camera_case': case, 'exit': code, 'virtual_seconds': now[0], 'result': json.loads(out.getvalue())})
print(json.dumps({'pass': True, 'scope': 'synthetic CLI IO only, no hardware acceptance', 'results': results}, indent=2))
