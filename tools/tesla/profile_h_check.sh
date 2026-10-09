#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
bin=$(mktemp); trap 'rm -f "$bin"' EXIT
c++ -std=c++17 -I. tools/tesla/profile_h_check.cc -o "$bin"
for profile in '' standard c3 c3xl; do
  actual=$(SUNNYPILOT_HARDWARE_PROFILE="$profile" SUNNYPILOT_HARDWARE_PROFILE_FILE=/nonexistent "$bin")
  expected=0; [ "$profile" != c3xl ] || expected=1
  [ "$actual" = "$expected" ]; echo "$profile: $actual"
done
