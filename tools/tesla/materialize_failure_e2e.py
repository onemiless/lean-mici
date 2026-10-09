#!/usr/bin/env python3
"""Execute production materialize_repo with real temporary files and failing shell IO."""
import json
import os
from pathlib import Path
import subprocess
import tempfile

root=Path(__file__).resolve().parents[2]
text=(root/'tools/release/device_release.sh').read_text()
fn=text[text.index('materialize_repo() {'):text.index('\nmaterialize_gitlinks()',text.index('materialize_repo() {'))]
rows=[]
with tempfile.TemporaryDirectory(prefix='materialize-e2e-') as tmp:
  for failure in ('none','remove','copy','checkout','pin'):
    case=Path(tmp)/failure;case.mkdir();src=case/'source';src.mkdir();name='e2e_'+case.parent.name+'_'+failure
    old=src/name;old.mkdir();(old/'old.txt').write_text('old')
    env=os.environ.copy();env.update(SRC=str(src),SRC_REF='fixed',NAME=name,FAILURE=failure,RECEIPT=str(case/'stamped'))
    harness=r'''
set -Eeuo pipefail
die() { echo "DIE: $*" >&2; exit 1; }
pin_intact() { return 1; }
write_pin() { [ "$FAILURE" != pin ] || return 1; touch "$RECEIPT"; }
git_fetch_retry() { return 0; }
git() {
 if [ "$1" = init ]; then command mkdir -p "$3"; return; fi
 case "$3" in
  rev-parse) echo verifiedsha;;
  remote) return 0;;
  checkout) [ "$FAILURE" != checkout ] || return 1; command mkdir -p "$2/package"; echo fixture > "$2/package/canary";;
  *) echo "unexpected git $*" >&2; return 1;;
 esac
}
rm() { if [ "$FAILURE" = remove ] && [ "${2:-}" = "$SRC/$NAME" ]; then return 1; fi; command rm "$@"; }
cp() { [ "$FAILURE" != copy ] || return 1; command cp "$@"; }
'''+fn+r'''
# Conditional call reproduces errexit suppression at a release-stage boundary.
materialize_repo "$NAME" local-fixture package/canary || exit 77
'''
    p=subprocess.run(['bash','-c',harness],env=env,capture_output=True,text=True)
    stamped=(case/'stamped').exists()
    assert (p.returncode==0 and stamped) if failure=='none' else (p.returncode!=0 and not stamped and '[ok]' not in p.stdout),(failure,p.returncode,stamped,p.stdout,p.stderr)
    rows.append({'case':failure,'exit':p.returncode,'stamped':stamped,'stdout':p.stdout,'stderr':p.stderr})
    # Remove only this replay's unique temporary clone.
    import shutil
    shutil.rmtree('/tmp/mat_'+name,ignore_errors=True)
print(json.dumps({'pass':True,'scope':'production shell function, temporary filesystem and injected IO failures; no device/network','cases':rows},indent=2))
