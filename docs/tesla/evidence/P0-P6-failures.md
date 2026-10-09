# Failure modes recorded before release implementation

- Missing DOS or primary/fallback solver artifacts yields an incomplete release; inspect artifact CLI and verify missing-file rejection.
- Native fingerprint omits hardware or legacy sources; assert both directories feed native input hash.
- C4 output sent to C3/C3XL or unknown profile; exercise every branch/profile combination with a temporary profile file before any release mutation.
- Materialization uses old fork or canary; inspect actual materialize arguments through shell integration.
- Solver imports fail on device; smoke must reject before starting CAN replay.
- Wrong replay route selected; Toyota remains default, invalid names fail, Tesla requires an explicitly available recorded fixture.
- Baseline core tests cannot import native dependencies on Mac; record dependency blockers separately from regression results.
- Full Toyota replay needs route fixture and compiled native processes; a hash of source code is not a replay baseline.
- Schema @136 and device bootchain invariants require separate controls/hardware validation.

No unit tests will be added after production edits. Existing release expected-artifact tests are updated first; release profile E2E is authored before implementation.
