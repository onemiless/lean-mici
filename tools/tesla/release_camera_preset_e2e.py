#!/usr/bin/env python3
"""Execute release camera query against actual camera presets and device-type matrix."""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import re
import subprocess
import sys
import types
from unittest.mock import patch

# Load native numpy dependencies once before restoring the hardware-module override.
import openpilot.common.transformations.camera

root=Path(__file__).resolve().parents[2]
script=(root/'tools/release/device_release.sh').read_text()
assert 'DRIVE_CAMERA_RESOLUTION=' in script, 'Release model build still hardcodes C4 resolution'
query=script.split("<<'CAMERA_PY'\n",1)[1].split('\nCAMERA_PY',1)[0]
args_line=next(line.strip() for line in script.splitlines() if line.strip().startswith('DRIVE_ARGS='))
assert '--camera-resolutions "$DRIVE_CAMERA_RESOLUTION"' in script
rows=[]
for device,expected in [('mici','1344x760'),('tici','1928x1208'),('tizi','1928x1208')]:
 module=types.ModuleType('openpilot.common.hardware');module.HARDWARE=types.SimpleNamespace(get_device_type=lambda:device)
 with patch.dict(sys.modules,{'openpilot.common.hardware':module}),contextlib.redirect_stdout(io.StringIO()) as output:
  exec(compile(query,'release-camera-query','exec'),{})
 actual=output.getvalue().strip();assert actual==expected
 args=subprocess.check_output(['bash','-c','DRIVE_CAMERA_RESOLUTION="$1"; '+args_line+'; printf "%s" "$DRIVE_ARGS"','camera-check',actual],text=True)
 assert f'--camera-resolutions {expected}' in args
 rows.append({'device':device,'camera':actual,'fingerprint_args':args,'args_sha256':hashlib.sha256(args.encode()).hexdigest()})
assert rows[0]['args_sha256']!=rows[1]['args_sha256']==rows[2]['args_sha256']
assert rows[0]['fingerprint_args']=='--model-size 512x256 --camera-resolutions 1344x760 --frame-skip 4'
print(json.dumps({'pass':True,'scope':'release query + actual camera presets + shell fingerprint arguments; no model build','cases':rows},indent=2))
