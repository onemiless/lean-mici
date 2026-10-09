#!/usr/bin/env python3
"""Replay the complete read-only boot-success CLI with controlled library/abctl IO."""
import contextlib
import ctypes
import ctypes.util
import io
import json
from pathlib import Path
import runpy
import subprocess
from unittest.mock import patch

script = Path(__file__).with_name('boot_success_check.py')
assert script.is_file(), 'Checker must exist before replay'
results = []
for slot, raw, expected in [('_a',1,0),('_b',1,0),('_a',0,1),('_a',-1,2),('help text',1,2),('_a',None,2)]:
  called = []
  class Getter:
    def __call__(self, index):
      called.append(index)
      return raw
  class Library:
    _Z25libabctl_getSuccessStatusj = Getter()
  def load(*args, **kwargs):
    if raw is None:
      raise OSError('library unavailable')
    return Library()
  with patch.object(subprocess,'check_output',lambda *a,**kw:slot+'\n'), patch.object(ctypes.util,'find_library',lambda n:'fixture-library'), patch.object(ctypes,'CDLL',load), contextlib.redirect_stdout(io.StringIO()) as output:
    try:
      runpy.run_path(str(script), run_name='__main__')
    except SystemExit as exc:
      code=exc.code
  assert code == expected, (slot,raw,code)
  payload=json.loads(output.getvalue())
  assert payload['pass'] == (expected == 0)
  if slot == 'help text':
    assert not called
  elif raw is not None:
    assert called == [0 if slot=='_a' else 1]
  results.append({'slot_input':slot,'getter_result':raw,'exit':code,'output':payload})
print(json.dumps({'pass':True,'scope':'synthetic CLI IO; getter implementation verified separately on device','cases':results},indent=2))
