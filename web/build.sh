#!/usr/bin/env bash
# Builds the browser version:
#   1. compiles Source/ + the platform layer to WebAssembly with Emscripten
#      (web/build/web/adventure.js, wasm and game data inlined), and
#   2. packages it with the front end into dist/colossal-cave.html.
# The wasm is inlined as base64: raw binary encoding would put NUL bytes
# inside a <script> element, which HTML parsers replace.
# Uses emcc from PATH, or installs a pinned emsdk into web/.emsdk first.
set -euo pipefail
source "$(dirname "$0")/common.sh"

if ! command -v em++ >/dev/null 2>&1; then
  [ -x "$WEB_DIR/.emsdk/upstream/emscripten/em++" ] || "$WEB_DIR/install-emsdk.sh"
  source "$WEB_DIR/.emsdk/emsdk_env.sh" >/dev/null 2>&1
fi

OUT="$WEB_DIR/build/web"
mkdir -p "$OUT/obj"

objs=()
for f in $GAME_SOURCES; do
  o="$OUT/obj/${f%.*}.o"
  em++ $GAME_CFLAGS $(prompt_flags "$f") -c "$SRC_DIR/$f" -o "$o"
  objs+=("$o")
done
o="$OUT/obj/adv_platform.o"; em++ $PLATFORM_CFLAGS -c "$WEB_DIR/platform/adv_platform.cpp" -o "$o"; objs+=("$o")
# The web io layer uses the game's headers (CGameState::SaveAdv for autosave).
o="$OUT/obj/adv_io_web.o"
em++ ${GAME_CFLAGS/-Dmain=adv_main/} -I"$SRC_DIR" -c "$WEB_DIR/io/adv_io_web.cpp" -o "$o"; objs+=("$o")

embeds=()
for n in 0 1 2 3 4 5; do
  embeds+=(--embed-file "$REPO_DIR/AdventGameFiles/ADVENT$n.DAT@/ADVENT$n.DAT")
done

em++ -O2 -o "$OUT/adventure.js" "${objs[@]}" "${embeds[@]}" \
  --pre-js "$WEB_DIR/io/adv_pre.js" \
  -Wl,--wrap=_ZN10CGameState12PlayGameTurnEv \
  -sASYNCIFY=1 -sASYNCIFY_STACK_SIZE=65536 \
  -sMODULARIZE=1 -sEXPORT_NAME=createAdventure \
  -sSINGLE_FILE=1 -sSINGLE_FILE_BINARY_ENCODE=0 -sENVIRONMENT=web,node \
  -sINVOKE_RUN=0 -sEXIT_RUNTIME=1 -sALLOW_MEMORY_GROWTH=1 -sFORCE_FILESYSTEM=1 \
  -sEXPORTED_RUNTIME_METHODS=FS,ENV,callMain
echo "Built $OUT/adventure.js ($(wc -c < "$OUT/adventure.js") bytes)"

if [ -f "$WEB_DIR/package.mjs" ]; then
  node "$WEB_DIR/package.mjs"
fi
