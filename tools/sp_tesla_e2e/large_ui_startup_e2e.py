#!/usr/bin/env python3
"""Native large-UI construction/render proof with isolated Params; no CAN or device mutation."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import traceback

os.environ['BIG'] = '1'
os.environ['SUNNYPILOT_UI'] = '1'
os.environ.setdefault('SCALE', '0.5')


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--output', type=Path, required=True)
  args = parser.parse_args()
  from openpilot.common.prefix import OpenpilotPrefix
  result = {'scope': 'actual desktop large MainLayout, DeviceLayoutSP and Tesla settings; isolated Params; no vehicle/CAN',
            'passed': False, 'stages': []}
  with OpenpilotPrefix():
    from openpilot.system.ui.lib.application import gui_app
    from openpilot.selfdrive.ui.ui_state import ui_state
    from openpilot.selfdrive.ui.sunnypilot.layouts.settings.device import DeviceLayoutSP
    gui_app.init_window('Large UI startup E2E', fps=20)
    try:
      ui_state.started = False
      ui_state.params.put_bool('IsOffroad', True, block=True)
      device = DeviceLayoutSP()
      result['stages'].append('DeviceLayoutSP constructed')
      device._update_state()
      result['stages'].append('DeviceLayoutSP updated')
      from openpilot.selfdrive.ui.layouts.main import MainLayout
      from openpilot.selfdrive.ui.layouts.settings.settings import PanelType
      layout = MainLayout()
      result['stages'].append('MainLayout constructed')
      import pyray as rl

      renderer = gui_app.render()

      def render_page(name):
        for _ in range(3):
          next(renderer)
        result['stages'].append(name + ' rendered three frames')
        screenshot = args.output.resolve().with_name(args.output.stem + '-' + name + '.png')
        screenshot.parent.mkdir(parents=True, exist_ok=True)
        rl.take_screenshot(os.path.relpath(screenshot))
        result.setdefault('screenshots', {})[name] = {
          'path': str(screenshot), 'sha256': hashlib.sha256(screenshot.read_bytes()).hexdigest(),
        }

      render_page('home')
      layout.open_settings(PanelType.DEVICE)
      render_page('device')
      ui_state.params.put('CarPlatformBundle', {'brand': 'tesla', 'name': 'TESLA_MODEL_Y'}, block=True)
      layout.open_settings(PanelType.VEHICLE)
      render_page('tesla')
      from openpilot.selfdrive.ui.layouts.main import MainState
      from openpilot.selfdrive.ui.sunnypilot.layouts.settings.vehicle.brands.tesla import TeslaSettings
      vehicle = layout._layouts[MainState.SETTINGS]._panels[PanelType.VEHICLE].instance
      assert isinstance(vehicle._brand_settings, TeslaSettings)
      result['stages'].append('Vehicle panel uses actual TeslaSettings')
      result['passed'] = True
    except Exception:
      result['error'] = traceback.format_exc()
    finally:
      gui_app.close()
  source = Path(__file__).resolve().parents[2] / 'openpilot/selfdrive/ui/sunnypilot/layouts/settings/device.py'
  result['source_sha256'] = hashlib.sha256(source.read_bytes()).hexdigest()
  main_source = source.parents[3] / 'layouts/main.py'
  result['main_source_sha256'] = hashlib.sha256(main_source.read_bytes()).hexdigest()
  args.output.parent.mkdir(parents=True, exist_ok=True)
  args.output.write_text(json.dumps(result, indent=2) + '\n')
  print(json.dumps(result))
  raise SystemExit(0 if result['passed'] else 1)


if __name__ == '__main__':
  main()
