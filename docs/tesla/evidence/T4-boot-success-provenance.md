# Read-only boot-success proof

- Device: comma@172.16.1.202, read-only SSH authorized by parent; no service or boot-state mutation.
- Actual CLI --get_success is unsupported and emits help with exit 0; removed that false-positive check.
- `/lib/aarch64-linux-gnu/libabctl.so.0` SHA256: `6516e1da564d22729ad3c4fad05edfaa9d810308d76cfe4f51b9eabf43e18843`.
- `nm -D -C` confirms `libabctl_getSuccessStatus(unsigned int)` / `_Z25libabctl_getSuccessStatusj`. Captured disassembly (T4-abctl-getter-disassembly.txt) calls libgpt_getPartitionEntry, extracts bit54 and returns 0/1, or returns -1 on read failure. No write/set call in getter. No raw metadata layout is reimplemented.
- Inspected official commaai/agnos-builder source tree; it invokes `abctl --set_success`, but does not contain libabctl source. The read-only method is therefore grounded in the installed ABI and its inspected implementation, with explicit unsupported/error on unavailable ABI.
- Full CLI simulated cases: slot A/B success, success flag zero, getter read failure, help text instead of slot, and missing library. All produce expected pass/exit status. Source script was absent and replay failed before implementation.
- Executed checker source on device via `ssh ... 'sudo python3 -' < tools/tesla/boot_success_check.py`, without copying a file: `_a`, successful=true, method=libabctl_getSuccessStatus (T4-boot-success-device.json).
