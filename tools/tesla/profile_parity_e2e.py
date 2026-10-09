#!/usr/bin/env python3
"""Run production Python/C++/shell profile paths with isolated files, no hardware."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

root = Path(__file__).resolve().parents[2]
cases = [(None, None, 'standard'), ('', 'c3xl\n', 'c3xl'), (' c3xl \n', 'c3', 'c3xl'),
         (None, '\t c3xl \r\n', 'c3xl'), (None, '', 'standard'), (None, ' \t\n', 'standard'),
         ('standard', 'c3xl', 'standard'), ('c3', 'c3xl', 'c3'),
         ('bogus', 'c3xl', 'invalid'), (None, 'c3xl extra', 'invalid'), (None, 'c3xl\nc3', 'invalid')]
results = []
with tempfile.TemporaryDirectory() as tmp:
  tmp = Path(tmp)
  (tmp/'probe.cc').write_text('#include "openpilot/sunnypilot/hardware/profile.h"\n#include <iostream>\nint main(){try{std::cout << sunnypilot::hardware::is_c3xl();}catch(const std::exception&){return 2;}}\n')
  subprocess.run(['c++', '-std=c++17', '-I'+str(root), str(tmp/'probe.cc'), '-o', str(tmp/'probe')], check=True)
  (tmp/'model').write_bytes(b'comma tici\0')
  for override, file_value, expected in cases:
    profile = tmp/'profile'
    profile.unlink(missing_ok=True)
    if file_value is not None:
      profile.write_text(file_value)
    env = os.environ.copy()
    env.pop('SUNNYPILOT_HARDWARE_PROFILE', None)
    if override is not None:
      env['SUNNYPILOT_HARDWARE_PROFILE'] = override
    env.update(PYTHONPATH=str(root), SUNNYPILOT_HARDWARE_PROFILE_FILE=str(profile), SUNNYPILOT_HARDWARE_MODEL_FILE=str(tmp/'model'))
    py = subprocess.run([sys.executable, '-c', 'from openpilot.sunnypilot.hardware.profile import get_hardware_profile; print(get_hardware_profile().value)'], env=env, capture_output=True, text=True)
    cpp = subprocess.run([str(tmp/'probe')], env=env, capture_output=True, text=True)
    shell = subprocess.check_output(['bash', '-c', 'source launch_env.sh; printf "%s:%s" "${AGNOS_MANIFEST_FILE##*/}" "$AGNOS_SKIP_UPDATE"'], cwd=root, env=env, text=True)
    if expected == 'invalid':
      assert py.returncode != 0 and cpp.returncode == 2, (override, file_value, py.stdout, cpp.stdout)
    else:
      assert py.returncode == cpp.returncode == 0 and py.stdout.strip() == expected and cpp.stdout == str(int(expected == 'c3xl')), (override, file_value, py.stdout, cpp.stdout)
    expected_shell = f'agnos-{expected}.json:0' if expected in ('c3','c3xl') else 'agnos.json:1'
    assert shell == expected_shell, (override, file_value, shell, expected_shell)
    results.append({'env': override, 'file': file_value, 'profile': expected, 'shell': shell})
print(json.dumps({'pass': True, 'cases': results}, indent=2))
