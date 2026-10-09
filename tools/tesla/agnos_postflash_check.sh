#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
stage="${1:-health}"
case "$stage" in health|--camera) ;; *) echo "Usage: $0 [health|--camera]" >&2; exit 2 ;; esac
python3 - "$stage" <<'PY' | tee "/data/agnos_postflash_$(date +%s).json"
import json, subprocess, sys
stage = sys.argv[1].removeprefix("--")
expected_service = "inactive" if stage == "camera" else "active"
commands = {
  'version': ['cat', '/VERSION'], 'kernel': ['uname', '-r'], 'slot': ['abctl', '--boot_slot'],
  'slot_success': ['sudo', 'abctl', '--get_success'], 'kernel_errors': ['dmesg', '-l', 'err,crit'],
  'device_nodes': ['ls', '/dev/kgsl-3d0', '/dev/spidev0.0', '/dev/ion'], 'usb': ['lsusb'],
  'network': ['ip', '-br', 'a'], 'comma_service': ['systemctl', 'is-active', 'comma'],
  'profile': ['cat', '/data/hardware_profile'],
}
if stage == "camera":
  commands["cameras"] = ["python3", "tools/tesla/camera_check.py"]
failed = False
for name, cmd in commands.items():
  try:
    p = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
    passed = p.stdout.strip() == expected_service if name == 'comma_service' else p.returncode == 0
    print(json.dumps({'check': name, 'stage': stage, 'expected_service': expected_service if name == 'comma_service' else None, 'pass': passed, 'returncode': p.returncode, 'stdout': p.stdout, 'stderr': p.stderr}), flush=True)
    failed |= not passed
    if name == 'comma_service' and not passed and stage == 'camera':
      break
  except (OSError, subprocess.TimeoutExpired) as exc:
    print(json.dumps({'check': name, 'error': str(exc)}), flush=True)
    failed = True
raise SystemExit(1 if failed else 0)
PY
