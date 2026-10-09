#!/usr/bin/env python3
"""Exercise fixture selection only; --check exits before Panda import or hardware access."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
script = root/'tools/bench/jungle_replay.sh'
assert '--check' in script.read_text(), 'Read-only routing interface must exist before executing this check'
results = []
with tempfile.TemporaryDirectory() as tmp:
  fixture = Path(tmp)/'synthetic-routing-only.xz'
  fixture.write_bytes(b'Routing check only: never transmit or deserialize this file')
  cases = [({}, 0), ({'CAR':'tesla'}, 1), ({'CAR':'unknown'}, 1),
           ({'CAR':'tesla', 'TESLA_CAN_FRAMES':str(fixture), 'TESLA_CAN_ROUTE':'synthetic-routing-only'}, 0)]
  for override, expected in cases:
    env = {k:v for k,v in os.environ.items() if k not in ('CAR','TESLA_CAN_FRAMES','TESLA_CAN_ROUTE')}; env.update(override)
    p = subprocess.run(['bash', str(script), '--check'], env=env, capture_output=True, text=True, timeout=10)
    assert p.returncode == expected, (override, p.returncode, p.stdout, p.stderr)
    if p.returncode == 0:
      data = json.loads(p.stdout)
      selected = fixture if override else root/'tools/bench/sienna_can_loop.xz'
      assert data['sha256'] == hashlib.sha256(selected.read_bytes()).hexdigest()
      assert data['car'] == override.get('CAR','toyota')
    results.append({'case': override.get('CAR','default Toyota'), 'exit':p.returncode, 'stdout':p.stdout, 'stderr':p.stderr})
print(json.dumps({'pass': True, 'scope':'read-only selection; no Jungle, CAN or bench acceptance', 'cases': results}, indent=2))
