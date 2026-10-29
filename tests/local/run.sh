#!/bin/bash
# Checks for this fork's local changes. Each one compiles real yabai sources
# against stubs and posts no events, so it is safe on a desktop in use.
# The suite under tests/src drives the live WindowServer: build it, never run it here.
set -euo pipefail

root=$(cd "$(dirname "$0")/../.." && pwd)
work=$(mktemp -d "${TMPDIR:-/tmp}/yabai-local-tests.XXXXXX")
trap 'rm -rf "$work"' EXIT

python3 -I "$root/tests/local/check_query_json.py" "$root"

xcrun clang -std=c11 -g -fsanitize=undefined -I "$root/src" "$root/tests/local/test_mouse_press.c" \
    -framework ApplicationServices -o "$work/test-mouse-press"
"$work/test-mouse-press"
