#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
for model in 'comma mici' 'comma tizi' 'comma tici'; do
 for profile in '' standard c3 c3xl; do
  printf '%s\0' "$model" > "$tmp/model"
  printf '%s' "$profile" > "$tmp/profile"
  actual=$(SUNNYPILOT_HARDWARE_MODEL_FILE="$tmp/model" SUNNYPILOT_HARDWARE_PROFILE_FILE="$tmp/profile" SUNNYPILOT_HARDWARE_PROFILE='' bash -c 'source launch_env.sh; echo "${AGNOS_MANIFEST_FILE##*/}:$AGNOS_SKIP_UPDATE"')
  expected=agnos.json:0
  if [ "$model" = 'comma tici' ]; then
   case "$profile" in c3|c3xl) expected="agnos-$profile.json:0";; *) expected=agnos.json:1;; esac
  fi
  [ "$actual" = "$expected" ]; echo "$model/$profile: $actual"
 done
done
