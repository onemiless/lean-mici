#!/usr/bin/env python3
"""Read-only process scheduling check; does not start processes or access GPIO."""
import json
import os
from unittest.mock import patch
from openpilot.system.manager import process_config

results = {}
with patch.object(process_config, "PC", False):
  for profile in ("standard", "c3", "c3xl"):
    with patch.dict(os.environ, {"SUNNYPILOT_HARDWARE_PROFILE": profile}):
      actual = process_config.managed_processes["alert_output"].should_run(False, None, None)
      assert actual == (profile == "c3xl")
      results[profile] = actual
print(json.dumps(results))
