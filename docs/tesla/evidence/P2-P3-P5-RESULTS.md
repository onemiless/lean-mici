# P2/P3/P5 local verification

Source: lean `4802cb2fe8c993fd7852b1f78be5b8b9604dd5a5`, Tesla source `0f694538fa8f84190ce9751f106f879a62550f8a`. Production commits: `b45711d1f`, `3211d66f4`; E2E additions were recorded in `55e9855a4` before production edits. Final synthetic model boundary correction: `4b3b98f55`.

## Verified locally (macOS arm64)

- Native SCons build succeeded: common Params library, cereal C++, upstream MPC, both frozen legacy solver extensions. See `P3-build.txt`.
- Cereal regeneration repeated byte-identically; bigModelReply @136 preserved, trafficRadarState @137 supported. See `P2-schema.txt`.
- Restored traffic/planner tests: **452 passed** including pinned platform Experimental and TN numerical golden replay (236 cycles each).
- Actual control-chain, ambient safety, traffic override E2E passed; each has JSON and a repeatable CLI.
- Planner p1 27 cases, cold-start, non-Tesla zero legacy import/session bypass, session-transition, enabled DEC/Vision SCC/Map SCC/SLA modes all passed.
- Feature replay: three backends ×236 cycles finite; Official default output matches a child process running the pinned lean MPC class bit-for-bit. The reference class is installed in the child launcher, not a parent-only patch lost by multiprocessing spawn.
- All 18 backend/profile/ACC-E2E replay combinations ran, 236 cycles each. Full local artifact is `artifacts/P3-18-combinations.json`; tracked summary records its SHA256. No claim of independent dev-sp parity for all18 combinations: only the separately recorded platform goldens and Official comparison are verified.
- Synthetic modelV2 big→small, zero frame pairs and sequence reset: two legacy consumers passed trajectory-jerk continuity and stop-state assertions. This exercises consumer messages, not bigmodeld inference or transport. The legacy MPC has jerk cost, not a hard jerk constraint; the check uses emitted trajectory jerk ×DT_MDL and does not establish a universal bound across arbitrary producer transitions.
- Settings native constructors/callbacks and isolated persisted Params passed. BMS display/passive-subscription checks14/14 passed. Three inherited checks for home-panel behavior/alert localization are explicitly excluded as out of scope. Consumption rendering needed3.5s of synthetic data to pass the50m warmup threshold and500ms display refresh; no BMS production behavior changed.

## Reproduction

From checkout root, activate the repository Python environment and set `PYTHONPATH=$PWD:$PWD/opendbc_repo:$PWD/msgq_repo:$PWD/rednose_repo:$PWD/tinygrad_repo`.

```
python -m pytest -q openpilot/sunnypilot/selfdrive/traffic_control openpilot/sunnypilot/selfdrive/controls/tests
python tools/sp_tesla_e2e/tesla_control_chain_e2e.py --output artifacts/control.json
python tools/sp_tesla_e2e/ambient_safety.py --output artifacts/ambient.json
python tools/sp_tesla_e2e/traffic_override_e2e.py --output artifacts/traffic.json
python tools/sp_tesla_e2e/tesla_settings_e2e.py --output artifacts/settings.json
python tools/sp_tesla_e2e/tesla_display_e2e.py --output artifacts/display.json
# Repeat for p1, cold-start, non-tesla, session-transition, features, bigmodel-fallback, replay:
python tools/sp_tesla_e2e/tesla_planner_chain_e2e.py --mode p1 --output artifacts/planner.json
# Only on Linux/aarch64 with locally built solvers:
python tools/sp_tesla_e2e/tesla_planner_chain_e2e.py --mode device --output artifacts/device.json
```

The route fixture was absent from the fixed dev-sp Git tree despite its README. Recovered the original local fixture only after its SHA256 matched the existing assertion `36e4a6e774d33839b2b9f78d9f90d14b132020fb1cb88511d1096c737b0747f4`; force-added it so clean checkouts work.

## Remaining boundaries

Device/aarch64 numerical verification, physical display screenshots/HUD overlap, true bigmodeld producer fallback, full18-combination independent dev-sp comparison, Toyota reference replay, final release integration and road/control acceptance remain parent/integration work. No device, firmware, CAN transmission, remote branches, or dirty original files were modified by this workstream.
