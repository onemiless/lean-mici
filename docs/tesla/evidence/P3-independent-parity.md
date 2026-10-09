# Independent fixed dev-sp planner comparison

The reference is `/Users/mile/work/tesla-port/devsp-ref`, materialized with `GIT_LFS_SKIP_SMUDGE=1 git archive` from dev-sp `0f694538fa8f84190ce9751f106f879a62550f8a`. The archive has a local snapshot commit solely so runtime version helpers can inspect Git metadata; that local snapshot commit is not represented as the original source commit. No reference Python or C++ source was patched.

Dependencies use the reference's exact pins: opendbc `3b9bf1895eca4983b2ed7cb620f318d362129112`, panda `ac0f791f8fb2105073c390784f47591b0d313a23`, msgq `e7396e76dadbb49e374d4b664ff6bbb43a39bcb0`, rednose `28d4a7f69e80e1c3e0d24ca0733d7daeaeade3d0`, tinygrad `fe5d3169ba4f41d0947ad174925f413cbea9d056`. Python/acados/compiler environment is the same as the migrated checkout, so this comparison isolates source changes.

Every materialized tracked source blob in the selected main-tree archive (openpilot, tools, SConstruct, pyproject.toml, .gitignore) and all five dependency trees was independently checked using Git's blob hash formula against the fixed commit tree. **4,050 files checked, zero differences.** See `P3-devsp-provenance.json`. The missing route fixture was copied only after matching the hash recorded by the reference's own existing replay test.

Reference Params, upstream MPC and both legacy MPC extensions were built under the reference source root with SCons. `P3-devsp-build.log` preserves the build. The pinned reference E2E script was run unmodified. Its `replay_rows` function AST is identical to the migrated script's replay function.

The fixture contains only carState, selfdriveState, carControl, controlsState, radarState, modelV2, vehicleParameters and carParams. It contains no trafficRadarState or bigModelReply. Consequently the reference traffic slot @136 and migrated traffic slot @137 never reinterpret input data; no message filtering was needed.

**Result: 18 backend/profile/ACC-E2E combinations ×236 cycles =4,248 cycles matched.** The comparison checks aTarget, shouldStop, allowThrottle, source, speeds, accels and jerks. All **220,896 floating scalars match IEEE754 binary64 bits after JSON decoding**, and all **12,744 categorical/other scalars match exactly**. Zero differences. `P3-independent-parity.json` records artifact hashes, per-combination cycle counts and scope. The complete reference JSON is preserved as deterministic gzip `P3-devsp-fixed-18.json.gz` (~1.1MB); full migrated output remains `artifacts/P3-independent-parity.json` with its SHA256 recorded.

Reproduce in each source root, with that root's dependencies on PYTHONPATH:

```bash
export PATH=/Users/mile/Desktop/mo-op/.venv/bin:$PATH
export PYTHONPATH=$PWD:$PWD/opendbc_repo:$PWD/msgq_repo:$PWD/rednose_repo:$PWD/tinygrad_repo
scons -j8 openpilot/common/libparams_c.dylib \
  openpilot/selfdrive/controls/lib/longitudinal_mpc_lib/c_generated_code/acados_ocp_solver_pyx.so \
  openpilot/sunnypilot/selfdrive/controls/lib/longitudinal_backends/legacy_mpc
# Reference root:
python tools/sp_tesla_e2e/tesla_planner_chain_e2e.py --mode replay --output artifacts/devsp-fixed-18.json
# Migrated root; alternatively gunzip the tracked compressed reference to a temporary JSON:
python tools/sp_tesla_e2e/tesla_planner_chain_e2e.py --mode replay \
  --baseline ../devsp-ref/artifacts/devsp-fixed-18.json --output artifacts/P3-independent-parity.json
```

This establishes macOS arm64 solver/process replay parity for the specified route and tuning combinations. It does not establish device/aarch64 parity, bigmodeld transport/inference fallback, CAN behavior, or road acceptance.
