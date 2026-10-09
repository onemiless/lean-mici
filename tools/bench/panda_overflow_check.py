#!/usr/bin/env python3
"""Read-only live Panda buffer counters across a bounded observation window."""
import json
import sys
import time
from openpilot.cereal import messaging


def snapshot(sock):
  event = messaging.recv_one(sock)
  if event is None or not event.valid or not event.pandaStates:
    raise RuntimeError("Missing or invalid pandaStates sample")
  return [{"type": str(p.pandaType), "rx": p.rxBufferOverflow, "tx": p.txBufferOverflow, "uptime": p.uptime} for p in event.pandaStates]


def main():
  duration = float(sys.argv[1])
  if not 0 < duration <= 3600:
    raise ValueError("Observation must be in (0, 3600] seconds")
  sock = messaging.sub_sock("pandaStates", timeout=5000, conflate=True)
  before = snapshot(sock)
  end = time.monotonic() + duration
  after = before
  while time.monotonic() < end:
    after = snapshot(sock)
    if len(before) != len(after) or any(a["type"] != b["type"] or a["rx"] != b["rx"] or a["tx"] != b["tx"] or b["uptime"] < a["uptime"] for a, b in zip(before, after)):
      print(json.dumps({"pass": False, "before": before, "after": after, "reason": "Panda changed, reset or buffer counter increased"}))
      return 1
  print(json.dumps({"pass": True, "before": before, "after": after, "watch_seconds": duration}))
  return 0


if __name__ == "__main__":
  raise SystemExit(main())
