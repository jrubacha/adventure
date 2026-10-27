# Shared by build-native.sh and build.sh: game sources and per-file flags.
# Sourced, not run.
WEB_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd "$WEB_DIR/.." && pwd)"
SRC_DIR="$REPO_DIR/Source"

GAME_SOURCES="AdvMessageData.Cpp Adventure.cpp CaveArray.Cpp DeathEndScore.Cpp
  Describe.Cpp DoTrPrObject.Cpp DwarvesAndPirate.Cpp English.Cpp GameState.cpp
  GameStateInitialize.cpp GameStateSaveRestore.cpp Hints.Cpp ItVerb.Cpp
  LongIntArray.Cpp MoveRequests.Cpp PlayGameTurn.Cpp RandomPercent.Cpp
  ShortIntArray.Cpp TrVerb.Cpp TravelArray.Cpp TravelCave.Cpp Vocabulary.Cpp"

# Flags for every game file: release build (no _DEBUG), MSVC shims
# force-included, "void main" renamed, and no builtin rewriting of
# printf/puts and no glibc fortify wrappers (__printf_chk): the game supplies
# its own puts and printf, and every call must reach them.
GAME_CFLAGS="-x c++ -std=gnu++03 -O2 -w -fno-builtin -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -DNDEBUG
  -I$WEB_DIR/platform/include -include $WEB_DIR/platform/platform.h
  -Dmain=adv_main"

PLATFORM_CFLAGS="-std=gnu++11 -O2 -Wall -fno-builtin -I$SRC_DIR -I$WEB_DIR/platform/include"

# Tell the io layer which prompt each file's GetInputLine calls are for.
# English.Cpp keeps the real name: its only call is the command prompt.
prompt_flags() {
  case "$1" in
    AdvMessageData.Cpp)      echo "-DGetInputLine=adv_GetInputLine_question" ;;
    GameStateSaveRestore.cpp) echo "-DGetInputLine=adv_GetInputLine_filename" ;;
    Adventure.cpp)           echo "-DGetInputLine=adv_GetInputLine_exit" ;;
    *)                       echo "" ;;
  esac
}
