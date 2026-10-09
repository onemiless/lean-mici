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

## Focused follow-up review

- `profile_parity_e2e.py`: 12 subprocess cases execute production Python/C++/launch_env paths, including empty files, whitespace, invalid and multiline input. All agree; invalid values fail Python/C++ and disable tici updates.
- `hardware_checks_replay.py`: synthetic CLI IO reproduces postflash service conflict before the fix; health now expects active service, explicit `agnos_postflash_check.sh --camera` expects inactive and invokes camera check. It never starts/stops service. Both road sensors require OX03C10, >=100 contiguous frames, sensor and received rates >=18Hz. Wrong wide sensor, gaps, slow timestamps, slow delivery and missing wide frames all reject, missing-stream acquisition ends at 45 seconds.
- Production updater body executed with real launch_env.sh and forbidden flash/slot stubs: missing, empty, standard and invalid tici profiles return before flash/slot lookup. No updater production change needed.
- agnos_slot_check.py retains explicit _a/_b whitelist before any verify_partition calls; unknown output raises.
- Final Panda four-artifact rebuild uses clean opendbc 522598445d134bea277789e01ea7d1809b3afc7c. T4.2-final-size.json supersedes the initial dirty-tree build evidence.
