# P4 local verification

- Failure modes and initial shell/C++ E2E probes committed before production edits (974acf52e).
- `bash tools/tesla/agnos_select_e2e.sh`: all 12 model/profile cases pass. Unknown tici profile skips updates.
- `PYTHONPATH=. python3 tools/tesla/check_agnos_manifests.py`: C3/C3XL chain references, corruption rejection, C4 byte preservation pass.
- `bash tools/tesla/profile_h_check.sh`: standard/c3/empty=0, c3xl=1 with one native binary.
- Imported compatible hardware checks: 32 passed. Removed imported assertions for superseded 19.7 system, compile-time profile, raw-tici inference and profile-only shell selection contracts; the full selection/manifest E2E probes replace these. No new unit tests were added.
- C3XL SPI discovery uses narrow 56a734f27e deviation from fixed source; parent runs tools/tesla/c3xl_panda_discovery_e2e.py --output PATH on detached device.
- Python bytecode and shell syntax checks pass. Native pandad and alert gate require full checkout dependencies; local SPI script attempt stops at missing libparams_c.dylib, before any device access.
- Panda F4/H7 firmware build succeeds after adapting DOS UART includes to lean pin. Local artifacts use in-progress opendbc safety: release must rebuild against final committed gitlink. See T4.2-size.json.
- Parent owns offline AGNOS image compatibility, device camera/offroad acceptance and any firmware operations. This worker performed no device access or firmware writes.

Re-run local commands from repository root. For pytest, PYTHONPATH must include the parent directory containing the panda package and the final opendbc checkout.
