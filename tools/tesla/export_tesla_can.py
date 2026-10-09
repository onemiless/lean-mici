#!/usr/bin/env python3
"""Export received Tesla CAN offline into the existing Jungle100Hz fixture format."""
import argparse
import hashlib
import json
import lzma
import pickle
import warnings
from collections import Counter
from pathlib import Path

import zstandard
from openpilot.tools.lib.logreader import LogReader

parser = argparse.ArgumentParser()
parser.add_argument('source', type=Path)
parser.add_argument('output', type=Path)
args = parser.parse_args()
raw = args.source.read_bytes()
decoder = zstandard.ZstdDecompressor().decompressobj()
uncompressed = decoder.decompress(raw)
assert decoder.eof and not decoder.unused_data, 'Incomplete or unexpected concatenated zstd input'
with warnings.catch_warnings():
  warnings.simplefilter('error')
  messages = list(LogReader.from_bytes(uncompressed))
platforms = sorted({(m.carParams.brand, m.carParams.carFingerprint) for m in messages if m.which() == 'carParams'})
assert platforms and all(brand == 'tesla' and platform.startswith('TESLA_') for brand, platform in platforms), platforms
can_events = [m for m in messages if m.which() == 'can']
assert can_events
start, end = int(can_events[0].logMonoTime), int(can_events[-1].logMonoTime)
assert end > start
batches = [[] for _ in range((end-start)//10_000_000+1)]
source_frames = []
source_bus = Counter()
physical_bus = Counter()
addresses = Counter()
last_time = start
power_states = Counter()
power_checksum_valid = Counter()
for event in can_events:
  stamp = int(event.logMonoTime)
  assert stamp >= last_time, 'Nonmonotonic CAN event timestamps'
  last_time = stamp
  for frame in event.can:
    addr, payload, bus = int(frame.address), bytes(frame.dat), int(frame.src)
    source_bus[bus] += 1
    if bus not in (0,1,2):
      continue
    assert 0 <= addr <= 0x1fffffff and 0 < len(payload) <= 8
    item = (addr,payload,bus)
    batches[(stamp-start)//10_000_000].append(item)
    source_frames.append(item)
    physical_bus[bus] += 1
    addresses[f'{bus}:0x{addr:x}'] += 1
    if addr == 0x221 and bus == 0 and len(payload) == 8:
      power_states[(payload[0] >> 5) & 3] += 1
      power_checksum_valid[((sum(payload[:7])+0x21+0x2) & 0xff) == payload[7]] += 1
assert source_frames
flatten = [frame for batch in batches for frame in batch]
assert flatten == source_frames, 'Export changed source frame sequence'
encoded = lzma.compress(pickle.dumps(batches, protocol=4), format=lzma.FORMAT_XZ, preset=6)
assert pickle.loads(lzma.decompress(encoded)) == batches
assert encoded == lzma.compress(pickle.dumps(batches, protocol=4), format=lzma.FORMAT_XZ, preset=6)
args.output.write_bytes(encoded)
assert pickle.loads(lzma.decompress(args.output.read_bytes())) == batches
ignition = Counter()
for message in messages:
  if message.which() == 'pandaStates':
    for panda in message.pandaStates:
      ignition[f'line={bool(panda.ignitionLine)},can={bool(panda.ignitionCan)}'] += 1
report = {
  'source_route':'2c912ca5de3b1ee9|0000025d--6eb6bcbca4--4',
  'source_url':'https://commadataci.blob.core.windows.net/openpilotci/2c912ca5de3b1ee9/0000025d--6eb6bcbca4/4/rlog.zst',
  'source_sha256':hashlib.sha256(raw).hexdigest(), 'source_bytes':len(raw),
  'source_messages':len(messages), 'platforms':platforms,
  'source_can_events':len(can_events), 'source_bus_frame_counts':dict(source_bus),
  'exported_bus_frame_counts':dict(physical_bus),'exported_frames':len(source_frames),
  'exported_unique_bus_addresses':len(addresses), 'address_counts':dict(sorted(addresses.items())),
  'start_log_mono_time':start,'end_log_mono_time':end,'source_span_seconds':(end-start)/1e9,
  'batch_period_seconds':0.01,'batches':len(batches),'empty_batches':sum(not b for b in batches),
  'frames_per_batch_min':min(map(len,batches)),'frames_per_batch_max':max(map(len,batches)),
  'fixture_sha256':hashlib.sha256(encoded).hexdigest(),'fixture_bytes':len(encoded),
  'format':'XZ(LZMA2) of pickle protocol4 list[list[tuple[int,bytes,int]]]',
  'panda_ignition_observations':dict(ignition),
  'bus0_0x221_vehicle_power_state_counts':dict(power_states),
  'bus0_0x221_checksum_valid_counts':{str(k):v for k,v in power_checksum_valid.items()},
  'validation':{'complete_zstd':True,'tesla_car_params':True,'source_frame_sequence_identical':True,'roundtrip_identical':True,'deterministic_bytes':True},
  'limits':['Only received CAN on physical buses0/1/2 exported; all other src values excluded with counts.',
            'Original received timestamps floor-quantized into10ms batches; empty batches retained.',
            'Finite recorded segment only; repeat boundary restarts original counters and payloads.',
            'No synthetic messages, checksum/counter edits, CAN transmission, or physical ignition action.',
            'Recorded ignition observations do not prove an ignition cycle or hardware acceptance.'],
}
args.output.with_suffix('.provenance.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k != 'address_counts'},indent=2))
