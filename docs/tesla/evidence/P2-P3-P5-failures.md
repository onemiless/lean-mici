# Failure modes recorded before production changes

- P2: cereal slot collision with bigModelReply, stale C++ schemas, wrong Tesla ownership/event suppression, inconsistent CP_SP flags and Params, unavailable settings, Toyota behavior drift.
- P3: eager legacy imports on Toyota, wrong session latch/termination, planner feature exceptions/nonfinite output, model fallback acceleration discontinuities or stop chatter, aarch64 solver drift, traffic stale-green release and health veto failure.
- P5: BMS sends CAN, retains a previous car data, or subscribes when hidden/non-Tesla; top-level panel replacement.

Verification: restored fixed dev-sp E2E scripts before production changes; new planner features/bigmodel-fallback/device modes will be written before production edits. Artifacts must state numerical, synthetic, UI and actual device boundaries. No new unit tests after implementation.
