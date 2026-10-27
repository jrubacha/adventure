#!/usr/bin/env bash
# Downloads a pinned Emscripten SDK into web/.emsdk (git-ignored).
# Uses raw.githubusercontent.com for the emsdk scripts and Google Storage for
# the compiler binaries, so it also works where github.com itself is blocked.
set -euo pipefail
EMSDK_VERSION="${EMSDK_VERSION:-6.0.13}"
HERE="$(cd "$(dirname "$0")" && pwd)"
DEST="$HERE/.emsdk"
RAW="https://raw.githubusercontent.com/emscripten-core/emsdk/main"

if [ -x "$DEST/upstream/emscripten/emcc" ]; then
  echo "emsdk already installed in $DEST"
  exit 0
fi
mkdir -p "$DEST"
for f in emsdk emsdk.py emsdk_env.sh emsdk_manifest.json \
         emscripten-releases-tags.json legacy-emscripten-tags.txt legacy-binaryen-tags.txt \
         llvm-tags-64bit.txt; do
  curl -fsSL "$RAW/$f" -o "$DEST/$f"
done
chmod +x "$DEST/emsdk"
cd "$DEST"
./emsdk install "$EMSDK_VERSION"
./emsdk activate "$EMSDK_VERSION"
echo "emsdk $EMSDK_VERSION installed in $DEST"
