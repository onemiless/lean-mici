#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
python3 - <<'PY' | tee "/data/agnos_postflash_$(date +%s).json"
import json, subprocess
commands = {
  'version': ['cat', '/VERSION'], 'kernel': ['uname', '-r'], 'slot': ['abctl', '--boot_slot'],
  'slot_success': ['sudo', 'abctl', '--get_success'], 'kernel_errors': ['dmesg', '-l', 'err,crit'],
  'device_nodes': ['ls', '/dev/kgsl-3d0', '/dev/spidev0.0', '/dev/ion'], 'usb': ['lsusb'],
  'network': ['ip', '-br', 'a'], 'comma_service': ['systemctl', 'is-active', 'comma'],
  'profile': ['cat', '/data/hardware_profile'], 'cameras': ['python3', 'tools/tesla/camera_check.py'],
}
failed = False
for name, cmd in commands.items():
  try:
    p = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
    print(json.dumps({'check': name, 'returncode': p.returncode, 'stdout': p.stdout, 'stderr': p.stderr}), flush=True)
    failed |= p.returncode != 0
  except (OSError, subprocess.TimeoutExpired) as exc:
    print(json.dumps({'check': name, 'error': str(exc)}), flush=True)
    failed = True
raise SystemExit(1 if failed else 0)
PY
