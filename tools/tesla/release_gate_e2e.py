#!/usr/bin/env python3
"""Exercise the actual release profile gate without device or CAN operations."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument("--output", type=Path, required=True)
  args = parser.parse_args()
  script = ROOT / "tools/release/device_release.sh"
  source = script.read_text()
  functions = [re.search(rf"^{name}\(\) \{{.*?^\}}", source, re.M | re.S)
               for name in ("die", "check_hardware_profile")]
  rows = []
  if not all(functions):
    rows.append({"case": "release gate exists", "pass": False})
  else:
    code = "\n".join(m.group(0) for m in functions) + "\ncheck_hardware_profile\n"
    with tempfile.TemporaryDirectory() as directory:
      profile_file = Path(directory) / "profile"
      for branch in ("lean-tesla-release-c3", "lean-tesla-release-c4", "lean-tesla-release"):
        for profile in (None, "", "standard", "c3", "c3xl", "invalid"):
          profile_file.unlink(missing_ok=True)
          if profile is not None:
            profile_file.write_text(profile + "\n")
          env = {**os.environ, "RELEASE_BRANCH": branch,
                 "SUNNYPILOT_HARDWARE_PROFILE_FILE": str(profile_file)}
          result = subprocess.run(["bash", "-c", code], env=env, text=True, capture_output=True)
          expected = profile in ("c3", "c3xl") if branch.endswith("-c3") else profile in (None, "", "standard")
          rows.append({"branch": branch, "profile": profile, "exit": result.returncode,
                       "pass": (result.returncode == 0) == expected, "stderr": result.stderr})
  result = {"source_sha256": hashlib.sha256(script.read_bytes()).hexdigest(), "cases": rows,
            "pass": all(row["pass"] for row in rows)}
  args.output.parent.mkdir(parents=True, exist_ok=True)
  args.output.write_text(json.dumps(result, indent=2) + "\n")
  print(json.dumps({"pass": result["pass"], "cases": len(rows)}))
  return 0 if result["pass"] else 1


if __name__ == "__main__":
  raise SystemExit(main())
