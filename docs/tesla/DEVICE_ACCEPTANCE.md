# C3XL delivery and acceptance

The implementation uses the two supplied October 9 plans, with actual repository and device evidence taking precedence over stale command examples. Work is isolated at `/Users/mile/work/tesla-port/lean-mici`; the original Desktop checkout was not used for edits.

## Repositories and source identities

- Main source: `onemiless/lean-mici`, branch `lean-tesla`; fixed upstream base `4802cb2fe8c993fd7852b1f78be5b8b9604dd5a5`.
- Runtime release: `lean-tesla-release-c3@cbf24c60604431e2f2e6df97aef49227c1aa8a5c`.
- Runtime Python/UI source: `d3120967e0032558a2aaf9655b00df90cb4aa19f`; native binaries built from the source content at `2c8d9089c20e8e674ae19f27e0f0c1746e2650bc`. The UI-only repair leaves native inputs unchanged.
- Correct C3 model recipe: source commit `1cda99308`, existing AR/OX preset `1928x1208`; the model was built on C3XL. C4 remains `1344x760` using the existing OS preset.
- opendbc: `522598445d134bea277789e01ea7d1809b3afc7c`; Panda: `2caa123f53bef8ba9c1561e74bde77616ee23124`.
- tinygrad/msgq/rednose retain their exact lean pins in `PORT_BASELINE.json`.

GitHub already had the account's fork in this upstream network, so the new named main repository preserves upstream Git ancestry as an independent repository. No protected upstream/dev-sp branch or force-push was used.

## Verified

- Latest combined controls, traffic, hardware and release suite: 573 passed. Upstream removed-brand failures are documented separately, not claimed fixed.
- Fixed dev-sp numerical reference: 18 combinations ×236 cycles =4,248; 220,896 floating-point scalars and 12,744 other scalars identical.
- Actual Toyota Prius route: 88,500 input messages; card/controlsd/plannerd/selfdrived each repeated twice before and after; normalized output hashes identical. Only upstream timing ignore fields were omitted.
- Safety/MISRA results and mutation checks are in the pinned opendbc repository. The shared opendbc seam count is 12.
- Main core seams:25 files, net+279; hardware:17 files, net+115. Shared files are conservatively counted in both categories. Generated schemas and integration/tooling are separate.
- AGNOS19.6.27 booted on `_b`; all six boot-chain partitions preserved; old `_a` system hash unchanged. Boot success was read through the verified libabctl getter, not the unsupported `abctl --get_success` option.
- Native release build passed, including four Panda artifacts, both legacy solvers, Params/schema checks and flat-tree gate. USB is statically linked from the declared vendor package; exported libusb symbols are present even though ldd has no separate USB dependency.
- Both final-release OX03C10 cameras:100 frames each, approximately20Hz, no frame-ID jumps; cabin absent as expected for C3XL.
- H7 application: one sector-1 write, full1MiB before/after proof, bootstub and other sectors preserved, running signature matches and faults=0. The first post-write read encountered a10ms SPI ACK timeout; an independent read-only pass with a100ms floor completed the proof without another erase/write. Reset occurred only after successful proof.
- Device aarch64 Experimental/TN golden replay passed.
- Native HOME/Device/Tesla pages rendered on-device. Logical-canvas screenshots are separate from physical-display orientation; the user confirmed the real display was normal and complete in landscape.
- A30-second live offroad window after the UI fix had no required process stop/PID change, no Panda faults/ignition/control activation. The final published release also passed a30-second window with updated enabled and all expected offroad processes stable.
- Isolated live camera/QCOM inference passed:122 valid model outputs,120 valid odometry outputs,163 frames from each OX camera, KGSL descriptor present. About20.2Hz over the captured window; first execution443.7ms, followed by121 samples averaging26.27ms with consecutive frame IDs. The first warm-up jump accounts for the overall observed frame-ID gap fraction. Inputs use demo CP, zero-rpy calibration, stopped vehicle and disabled controls; no card/controlsd/Panda process or CAN transmission is part of this inference test.

## Corrections required by actual execution

1. Actual camera topics are narrowRoadCameraState/cabinCameraState, not the old names in the task list.
2. Removed two dangling large-UI callbacks left by lean's training/setup removal; retained Regulatory.
3. Made release materialization fail before stamping an incomplete tree when removal/copy fails.
4. Preserved caller build-dependency paths; installed the already-declared libusb package in a separate `/data` build directory, keeping system read-only.
5. Selected release model camera dimensions from the existing device presets; the original helper was C4-only despite the SConscript being device-aware.
6. Added an explicit runtime stop before release source synchronization/build, retaining EXIT restoration. A build with live background processes became unresponsive and required a user power cycle; the independent build completed in211s with memory/temperature telemetry. The exact cause of the earlier stall is not proven.
7. Replaced a last-sample-only health check with a whole-window PID stability check; the old check could miss UI restart loops.

The device independently read back the GitHub release ref and it matches the active commit. Initial deployment settings were set to Official(0) and traffic-signal control disabled, as specified by the plan; the prior values are retained in the evidence and protected parameter backup. Other existing calibration and configuration were retained.

## Not qualified here

The strict old-versus-new kernel-error-category gate remains open: new boot/probe messages are recorded and reviewed in evidence/kernel-log-review.md, without claiming all are explained.

No actual Jungle is connected. The real Tesla CAN loop is provided with source hashes, frame order and ignition observations, but no bench transmission result is claimed. No physical C3/F4 or C4 test, real external big-model producer/fallback, acoustic confirmation, traffic-light road acceptance or vehicle control/road qualification is claimed. Post-acceptance P7 refactoring/schema deletion is deferred.

See `ROLLBACK.md` for preserved state and recovery order. Test reports distinguish source/build, isolated inference, installed runtime and physical/road acceptance.
