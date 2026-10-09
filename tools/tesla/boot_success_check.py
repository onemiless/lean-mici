#!/usr/bin/env python3
"""Read current slot success using AGNOS's existing read-only libabctl getter."""
import ctypes
import ctypes.util
import json
import subprocess


def main():
  try:
    slot = subprocess.check_output(['abctl', '--boot_slot'], text=True, timeout=5).strip()
    if slot not in ('_a', '_b'):
      raise ValueError(f'Unrecognized boot slot: {slot!r}')
    library = ctypes.CDLL(ctypes.util.find_library('abctl') or 'libabctl.so.0')
    getter = library._Z25libabctl_getSuccessStatusj
    getter.argtypes = [ctypes.c_uint]
    getter.restype = ctypes.c_int
    result = getter(0 if slot == '_a' else 1)
    if result not in (0, 1):
      raise ValueError(f'libabctl could not read boot-success metadata: {result}')
    print(json.dumps({'pass': result == 1, 'slot': slot, 'successful': bool(result), 'method': 'libabctl_getSuccessStatus'}))
    return 0 if result == 1 else 1
  except (OSError, AttributeError, ValueError, subprocess.SubprocessError) as exc:
    print(json.dumps({'pass': False, 'status': 'unsupported_or_read_error', 'error': str(exc)}))
    return 2


if __name__ == '__main__':
  raise SystemExit(main())
