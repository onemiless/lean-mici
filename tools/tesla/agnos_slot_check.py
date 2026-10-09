#!/usr/bin/env python3
"""Read-only full verification of the active slot; never switches slots."""
import json
import subprocess
import sys
from openpilot.common.hardware.comma.agnos import slot_number_to_suffix, verify_partition

raw_slot = subprocess.check_output(["abctl", "--boot_slot"], text=True).strip()
if raw_slot not in ("_a", "_b"):
  raise RuntimeError(f"Unrecognized boot slot: {raw_slot!r}")
slot = {"_a": 0, "_b": 1}[raw_slot]
results = [{"partition": p["name"], "slot": slot_number_to_suffix(slot), "match": verify_partition(slot, p, force_full_check=True)}
           for p in json.load(open(sys.argv[1]))]
print(json.dumps(results))
sys.exit(0 if all(r["match"] for r in results) else 1)
