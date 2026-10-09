#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
python3 - <<'PY'
import subprocess
from pathlib import Path
base = '4802cb2fe8c993fd7852b1f78be5b8b9604dd5a5'
totals = {'core': [0, 0], 'hardware': [0, 0]}
for line in Path('docs/tesla/SEAMS.md').read_text().splitlines():
  cells = [v.strip().strip('`') for v in line.split('|')]
  if len(cells) < 7 or cells[2] not in ('P2', 'P3', 'P4'): continue
  path, phase = cells[1:3]
  if subprocess.run(['git', 'cat-file', '-e', f'{base}:{path}'], stderr=subprocess.DEVNULL).returncode: continue
  diff = subprocess.check_output(['git', 'diff', '--numstat', base, '--', path], text=True).split()
  if diff and diff[0].isdigit():
    counts = totals['hardware' if phase == 'P4' else 'core']; counts[0] += 1; counts[1] += int(diff[0])-int(diff[1])
print(totals)
assert totals['core'][0] <= 26 and totals['core'][1] <= 400 and totals['hardware'][0] <= 24 and totals['hardware'][1] <= 180, totals
PY
