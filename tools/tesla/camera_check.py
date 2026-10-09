#!/usr/bin/env python3
"""Offline camera acceptance: manager must be stopped and vehicle detached."""
import json
import os
from pathlib import Path
import subprocess
import time

from openpilot.cereal import messaging
from openpilot.common.params import Params
from openpilot.sunnypilot.hardware.profile import has_driver_camera


def main():
  params = Params()
  if not params.get_bool("IsOffroad") or params.get_bool("IsEngaged"):
    raise RuntimeError("Camera check requires offroad and disengaged")
  # Reject an existing camera owner; never stop a user's running manager.
  for entry in Path("/proc").glob("[0-9]*/cmdline"):
    try:
      args = entry.read_bytes().split(b"\0")
    except OSError:
      continue
    if entry.parent.name != str(os.getpid()) and any(a.endswith(b"/manager.py") or a == b"openpilot.system.manager.manager" or a.endswith(b"/camerad") for a in args):
      raise RuntimeError("Stop manager/camerad before running camera check")
  root = Path(__file__).resolve().parents[2]
  streams = ["narrowRoadCameraState", "wideRoadCameraState", "cabinCameraState"]
  required = streams if has_driver_camera() else streams[:2]
  sockets = {name: messaging.sub_sock(name, timeout=100) for name in streams}
  frames = {name: [] for name in streams}
  env = os.environ.copy()
  if has_driver_camera():
    env.pop("DISABLE_DRIVER", None)
  else:
    env["DISABLE_DRIVER"] = "1"
  with open("/tmp/tesla-camera-check.log", "w") as log:
    proc = subprocess.Popen([str(root / "openpilot/system/camerad/camerad")], cwd=root, env=env, stdout=log, stderr=subprocess.STDOUT)
    try:
      deadline = time.monotonic() + 45
      while time.monotonic() < deadline and any(len(frames[n]) < 100 for n in required):
        if proc.poll() is not None:
          raise RuntimeError(f"camerad exited {proc.returncode}; see /tmp/tesla-camera-check.log")
        for name, sock in sockets.items():
          event = messaging.recv_one_or_none(sock)
          if event is not None:
            state = getattr(event, name)
            frames[name].append((state.frameId, state.timestampSof, str(state.sensor), time.monotonic()))
        time.sleep(0.001)
    finally:
      proc.terminate()
      try:
        proc.wait(timeout=5)
      except subprocess.TimeoutExpired:
        proc.kill()
        proc.wait()
  result = {}
  passed = True
  for name, values in frames.items():
    jumps = sum(b[0] != a[0] + 1 for a, b in zip(values, values[1:]))
    hz = (len(values) - 1) * 1e9 / (values[-1][1] - values[0][1]) if len(values) > 1 and values[-1][1] > values[0][1] else 0
    received_hz = (len(values) - 1) / (values[-1][3] - values[0][3]) if len(values) > 1 and values[-1][3] > values[0][3] else 0
    sensors = sorted({v[2] for v in values})
    ok = len(values) >= 100 and hz >= 18 and received_hz >= 18 and jumps == 0
    if name in ("narrowRoadCameraState", "wideRoadCameraState"):
      ok = ok and sensors == ["ox03c10"]
    if name not in required:
      ok = not values
    result[name] = {"frames": len(values), "sensors": sensors, "hz": hz, "received_hz": received_hz, "frame_jumps": jumps, "pass": ok}
    passed &= ok
  print(json.dumps({"pass": passed, "streams": result}, indent=2))
  return 0 if passed else 1


if __name__ == "__main__":
  raise SystemExit(main())
