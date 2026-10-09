# Live camera/model E2E failure modes, before helper implementation

- Wrong host/profile, engaged/onroad global state, or a pre-existing manager/camera/model/control process: reject before spawning.
- Params/IPC prefix not isolated: reject an inherited prefix; use OpenpilotPrefix, keep global safety reads outside it and throughout the run.
- Camera absent, wrong sensor, unsynchronized frames, zero calibration transform, missing demo CP, non-QCOM execution, model load/runtime failure: require actual camera messages, calibrated synthetic zero-rpy input, live model output and modeld's KGSL device descriptor proof; preserve child logs.
- Nonfinite predictions, invalid events, insufficient valid model/odometry samples, duplicate frame IDs or expired deadline: fail with counts/artifact, never call a partial stream a pass.
- Exception/interrupt/timeout: terminate only owned process groups, escalate to SIGKILL with bounded waits before prefix cleanup, report cleanup failure.

Scope: parent owns device access. Helper starts only camerad and modeld --demo; synthetic stopped carState, disabled carControl, zero-rpy calibration and demo CarParams are explicit. No manager/card/controlsd/Panda/CAN sender is launched. Live camera/QCOM inference is not calibration accuracy or road/control acceptance.
