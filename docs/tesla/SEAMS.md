# Upstream seams

| 文件 | 阶段 | 增/删 | 作用 | dev-sp 来源 | 状态 |
|---|---|---|---|---|---|
| `tools/release/release_lib.py` | P6 | pending | DOS and legacy runtime artifacts and native inputs | task list | implemented |
| `tools/release/test_release_lib.py` | P6 | pending | expected artifacts written before implementation | task list | implemented |
| `tools/release/device_release.sh` | P6 | pending | fork canaries and hardware release gate | task list | implemented |
| `tools/bench/smoke_after_build.sh` | P6 | pending | device solver imports before CAN replay | task list | implemented |
| `openpilot/cereal/custom.capnp` | P2 | +131/-1 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/cereal/log.capnp` | P2 | +1/-1 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/cereal/services.py` | P2 | +1/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/common/params_keys.h` | P2 | +35/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/car/card.py` | P2 | +19/-6 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/selfdrived/selfdrived.py` | P2 | +16/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/sunnypilot/selfdrive/car/interfaces.py` | P2 | +5/-6 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/sunnypilot/selfdrive/controls/lib/speed_limit/__init__.py` | P2 | +13/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/sunnypilot/selfdrive/controls/lib/speed_limit/speed_limit_assist.py` | P2 | +10/-4 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/sunnypilot/selfdrive/controls/lib/speed_limit/speed_limit_resolver.py` | P2 | +8/-3 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/sunnypilot/selfdrive/selfdrived/events.py` | P2 | +7/-3 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/ui/sunnypilot/layouts/settings/vehicle/brands/factory.py` | P2 | +2/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/ui/sunnypilot/mici/layouts/settings.py` | P2 | +5/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/sunnypilot/SConscript` | P2 | +1/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/controls/controlsd.py` | P2 | +2/-2 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/controls/lib/longcontrol.py` | P2 | +8/-2 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/sunnypilot/selfdrive/controls/lib/longitudinal_planner.py` | P2 | +15/-8 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/controls/lib/longitudinal_mpc_lib/long_mpc.py` | P2 | +24/-5 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/controls/plannerd.py` | P2 | +12/-5 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/system/manager/manager.py` | P2 | +2/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/system/manager/process_config.py` | P2 | +1/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/ui/sunnypilot/onroad/hud_renderer.py` | P2 | +4/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `openpilot/selfdrive/ui/sunnypilot/mici/onroad/hud_renderer.py` | P2 | +4/-0 | Tesla control/planner/UI integration | 0f694538fa8f84190ce9751f106f879a62550f8a | implemented; local native/E2E verified |
| `SConstruct` | P4 | +1/-1 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `launch_chffrplus.sh` | P4 | +5/-1 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `launch_env.sh` | P4 | +17/-0 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/common/hardware/comma/agnos.py` | P4 | +12/-3 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/common/hardware/comma/hardware.h` | P4 | +6/-0 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/common/hardware/comma/hardware.py` | P4 | +2/-1 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/selfdrive/pandad/SConscript` | P4 | +2/-2 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/selfdrive/pandad/panda.cc` | P4 | +14/-3 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/selfdrive/pandad/panda.h` | P4 | +1/-1 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/selfdrive/pandad/panda_comms.h` | P4 | +43/-9 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/selfdrive/pandad/pandad.h` | P4 | +1/-0 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/selfdrive/pandad/pandad.py` | P4 | +20/-11 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/sunnypilot/selfdrive/selfdrived/events.py` | P4 | +9/-5 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/system/hardware/hardwared.py` | P4 | +8/-1 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/system/hardware/power_monitoring.py` | P4 | +2/-2 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/system/manager/process_config.py` | P4 | +4/-2 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |
| `openpilot/system/updated/updated.py` | P4 | +6/-3 | Runtime profile, safe boot selection or DOS USB | dev-sp 0f694538 + D2/D4/D5 | local E2E |

Shared core/hardware files are conservatively counted in both budgets using the full integrated diff. Generated schema code is excluded from source seam budgets.
