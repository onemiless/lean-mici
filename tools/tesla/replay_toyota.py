#!/usr/bin/env python3
import argparse
import hashlib
import json
import multiprocessing
import subprocess
import traceback
from collections import Counter
from pathlib import Path

from openpilot.common.timeout import Timeout
from openpilot.tools.lib.logreader import LogReader, save_log
from openpilot.selfdrive.test.process_replay.process_replay import CONFIGS, replay_process, check_most_messages_valid
from openpilot.selfdrive.test.process_replay.compare_logs import remove_ignored_fields

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('input', type=Path)
  parser.add_argument('output', type=Path)
  parser.add_argument('--process', required=True, choices=['card','controlsd','plannerd','selfdrived'])
  args = parser.parse_args()
  args.output.mkdir(parents=True, exist_ok=True)
  source = subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()
  messages = list(LogReader(str(args.input)))
  assert messages
  report = {'source':source, 'input_sha256':hashlib.sha256(args.input.read_bytes()).hexdigest(), 'input_messages':len(messages), 'processes':{}}
  for name in [args.process]:
    cfg = next(c for c in CONFIGS if c.proc_name == name)
    result = {'ignore':cfg.ignore, 'runs':[]}
    report['processes'][name] = result
    for run in (1,2):
      capture = {}
      try:
        with Timeout(90, error_msg=f"startup timeout for {name}"):
          output = replay_process(cfg, messages, captured_output_store=capture, disable_progress=True)
        save_log(str(args.output/f'{name}-{run}.rlog.zst'),output)
        normalized = [remove_ignored_fields(m,cfg.ignore).to_dict() for m in output]
        data = json.dumps(normalized,sort_keys=True,separators=(',',':'),default=lambda b: {'bytes_hex':b.hex()}).encode()
        (args.output/f'{name}-{run}.normalized.json').write_bytes(data)
        counts = dict(Counter(m.which() for m in output))
        result['runs'].append({'sha256':hashlib.sha256(data).hexdigest(), 'counts':counts, 'valid':check_most_messages_valid(output), 'expected_services_present':set(cfg.subs)==set(counts)})
      except Exception:
        result['runs'].append({'error':traceback.format_exc()})
      finally:
        (args.output/f'{name}-{run}.capture.json').write_text(json.dumps(capture,indent=2))
        (args.output/'report.json').write_text(json.dumps(report,indent=2))
      if 'error' in result['runs'][-1]:
        break
    result['deterministic'] = len(result['runs']) == 2 and result['runs'][0].get('sha256') == result['runs'][1].get('sha256')
    result['passed'] = result['deterministic'] and all(r.get('valid') and r.get('expected_services_present') for r in result['runs'])
    (args.output/'report.json').write_text(json.dumps(report,indent=2))
    print(name, json.dumps(result), flush=True)
  raise SystemExit(0 if all(r['passed'] for r in report['processes'].values()) else 1)


if __name__ == "__main__":
  multiprocessing.set_start_method("fork")
  main()
