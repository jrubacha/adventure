#!/usr/bin/env bash
# Runs every check: builds the native reference version, plays all TestFiles
# scripts through it and through the wasm build, then tests
# dist/colossal-cave.html in headless Chromium. Run web/build.sh first.
set -euo pipefail
WEB_DIR="$(cd "$(dirname "$0")" && pwd)"
"$WEB_DIR/build-native.sh"
echo
echo "== Transcripts: native vs Windows reference logs, wasm vs native"
node "$WEB_DIR/test/run-tests.mjs"
echo
echo "== Browser: dist/colossal-cave.html from file://, networking blocked"
node "$WEB_DIR/test/browser-test.mjs"
