#!/usr/bin/env bash
# Native reference build (g++ or clang++) used to check the platform layer
# against the Windows reference transcripts. Output: web/build/native/adventure
set -euo pipefail
source "$(dirname "$0")/common.sh"
CXX="${CXX:-g++}"
OUT="$WEB_DIR/build/native"
mkdir -p "$OUT/obj"

objs=()
for f in $GAME_SOURCES; do
  o="$OUT/obj/${f%.*}.o"
  $CXX $GAME_CFLAGS $(prompt_flags "$f") -c "$SRC_DIR/$f" -o "$o"
  objs+=("$o")
done
for f in "$WEB_DIR/platform/adv_platform.cpp" "$WEB_DIR/io/adv_io_native.cpp"; do
  o="$OUT/obj/$(basename "${f%.*}").o"
  $CXX $PLATFORM_CFLAGS -c "$f" -o "$o"
  objs+=("$o")
done
$CXX -o "$OUT/adventure" "${objs[@]}"
echo "Built $OUT/adventure"
