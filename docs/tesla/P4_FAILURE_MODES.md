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

## Boot success readback correction, before implementation

The actual abctl does not implement --get_success and returns help with status 0. Never classify CLI exit status alone as boot success. The installed libabctl.so.0 exports _Z25libabctl_getSuccessStatusj (unsigned int slot); captured disassembly calls only libgpt_getPartitionEntry and returns the partition success attribute bit or -1 on read failure. Use this existing read-only getter instead of guessing a misc/GPT layout. Reject unknown boot-slot text before the getter; output distinct unsuccessful (1) versus unsupported/read failure (2). Missing library/symbol or results outside 0/1 cannot PASS. Full CLI replay cases are recorded first.

## Native dependency path preservation, before one-line fix

The AGNOS immutable root lacks the already-declared optional comma-deps-libusb package. Parent stages its pinned wheel in /data and supplies PYTHONPATH. rebuild_native currently discards that path, making libusb imports fail despite a valid caller dependency directory. Preserve caller PYTHONPATH after repository paths only for native SCons. E2E runs the real shell assignment with unset, empty and a spaced dependency directory, requiring successful import from the caller directory and no empty path element. Do not alter rootfs, install dependencies here, or change model-build paths.

## Materialization failure, before release helper fix

Real device removal of root-owned __pycache__ failed but an AND chain suppressed errexit and execution continued to write_pin/[ok]. Replay production function under a conditional caller with injected rm/cp/checkout/write_pin failures; any failed materialization must exit nonzero before a pin or success line. Guard each destructive/copy/setup step explicitly. write_pin's own mkdir/manifest pipeline must return failure before its SHA stamp, because checking its status also suppresses implicit errexit inside that function. No device permission changes here.

## Device model camera-shape fix, before implementation

C3XL actual OX03C10 streams are 1928x1208; release helper hardcoded the C4 1344x760 shape, producing modeld KeyError(1928,1208). Reuse exactly SConscript's existing _os_fisheye for mici and _ar_ox_fisheye otherwise. One queried shape must feed both cache fingerprint and compile CLI so old C4-shaped artifacts cannot hit the C3 cache. Validate actual presets for mici/tici/tizi and unchanged C4 fingerprint arguments before the production edit. No camerad/model runtime change.

## Release service isolation, before one-line stage fix

Source synchronization while manager processes import files and model GPU compilation while UI/hardwared run can race or compete for resources. A prior live compile required power-cycle recovery; its exact GPU/kernel cause is not established. After verifying the existing active-service precondition, stop comma before any source synchronization/build. Retain existing EXIT cleanup to start comma on success or build failure. Shell orchestration replay must prove prereqs -> stop -> sync -> build ordering and cleanup start on both outcomes; use no-op systemctl only.
