# P4 pre-change failure modes and validation

Recorded before production edits. Keep lean H7/C4 behavior, restore DOS/F4 only.

- DOS board declaration may introduce unsupported fan_stall_recovery: inspect all board initializers against lean pin.
- F4 flash/RAM overflow or signing incompatibility: build F4 and H7 artifacts, inspect ELF sections against linker script.
- Wrong USB/SPI internal Panda selected: runtime profile matrix, C3XL SPI regression, no device operations here.
- Runtime profile mismatch between Python/C++: compile C++ check and run same explicit profile values/file fallback.
- Boot-chain corruption/wrong board: immutable reference manifests, all six boot partitions field comparison, C4 byte equality, negative allowlist cases.
- Missing profile on tici: shell selection matrix must disable both startup and updater flashing.
- AGNOS boot/system compatibility: offline image inspection; device parent owns lsmod and mutation gates.
- Camera mismatch: bounded on-device checker must reject non-OX03C10 road sensor, dropped frames or low frame rate; no driver port.

Validation artifacts are written to main repository docs/tesla/evidence. Existing upstream tests are preserved, no post-code unit tests will be introduced. Hardware execution/firmware acceptance remain separate gates.

## Focused review before follow-up fixes

- Full profile text, whitespace or empty values could select different boards in Python/C++/shell. Run profile_parity_e2e.py across missing/empty/whitespace/invalid/multi-token values before modifying profile readers. Unknown nonempty values must fail native/Python profile parsing; tici shell refuses updates.
- Postflash health and camera tests require incompatible manager states. Separate normal collection from explicit --camera stage, report expected service state and never stop/start manager.
- Wide-road sensor may be wrong despite road camera passing; camera acceptance checks both road sensors and bounds acquisition to 45 seconds. Preserve >=100 frames, timestamp rate>=18 and contiguous frame IDs.
- Slot readback must reject unknown boot_slot; existing explicit _a/_b mapping is retained.
