# Colossal Cave Adventure in the browser

`dist/colossal-cave.html` is the whole game in one file: Al Whinery's
AdventureCCaw1 (the Windows version in `AdventGameFiles/`), compiled from the
unchanged C++ in `Source/` to WebAssembly. It plays like the Windows exe, with
the same text, parser, puzzles and scoring. It runs from a student's Downloads
folder in Microsoft Edge with no install, no server and no network connection,
and it never makes a network request.

## For the classroom

1. Upload `dist/colossal-cave.html` to Canvas as a file.
2. Students download it and double-click it. It opens in Edge.
3. They type commands at the `>` prompt. **Help** in the top-right corner explains the basics.

**Progress.** The game saves itself in the browser after every turn. When a
student reopens the file, they're offered **Continue your game** or **Start a new
game**. That copy lives in Edge on that laptop only. If students switch devices
or clear their browsing data, they need a save file:

- **Save** (or typing `save`) asks for a name and downloads
  `colossal-cave-<name>.adv`. Students can keep it in OneDrive.
- **Load** (or `restore <name>`) asks them to pick that file and returns to the saved game.
  Saves from the Windows exe load too.

**Transcript.** **Transcript** downloads `colossal-cave-transcript-<date>.txt`.
It's the game's own log, in the same format as the Windows exe's log, with
every command typed and every line the game printed. It builds up across class
periods until the student starts a new game, so it works as evidence of play.
(The game's `log` command turns logging off and on, exactly like the exe.)

### Check by hand on a school laptop

These can't be tested from here:

- **Touch.** Tapping the text should bring up the on-screen keyboard. The
  map's **+ / − / Done** buttons and the **Yes / No** buttons should work by
  touch.
- **Continue.** Edge saves the game for `file://` pages unless a school policy
  blocks site storage. To check: play a few turns, close Edge, reopen the file,
  and confirm you're offered **Continue your game**. If storage is blocked, the
  game still runs, but students need **Save** and **Load**.
- **Downloads.** Save and Transcript should drop files in Downloads, or ask
  where to put them, depending on your Edge settings.

### Things students may discover

These commands are part of the original game. The browser build keeps them,
because it changes no game logic:

- `tport <n>` / `tp <n>` teleports to location *n*. `tgrab <object>` / `tg <object>` grabs an object.
  These are the original author's test commands, and his test scripts use them.
- In the map display (`map map`, once you carry the map), pressing capital
  `M` moves you to the location shown. There's no button for it.
- `nodie`, `dwset 0`, `piset 0`, `friendly 1`, `hints 0` and `autocp` change
  the game's difficulty and behavior. `help help` lists every verb.

## Teacher settings

Near the top of `colossal-cave.html` is a block you can edit in Notepad:

```js
/* ===== TEACHER SETTINGS — edit these, save, re-upload to Canvas ===== */
const TEACHER_SETTINGS = {
  title: "Colossal Cave Adventure",   // shown in the tab and header
  startupCommands: [                  // run silently before the student's first turn, like the exe's -i option
    // "friendly 1",                  // show location numbers; dwarves and pirate less tedious
    // "hints 0",                     // turn hints off
    // "dwset 0",                     // no dwarves
    // "piset 0",                     // no pirate
  ],
  allowInputCommand: false,           // false = students can't run "input <file>" command scripts
  randomSeed: 511,                    // 511 matches the original game
  autosave: true,                     // true = save after every turn so "Continue your game" works next class
};
/* ===== END TEACHER SETTINGS ===== */
```

1. Right-click `colossal-cave.html` and choose **Open with → Notepad**.
2. Change a value. To turn a startup command on, delete the `//` in front of
   it, for example `"dwset 0",`. Keep the quotes and the comma.
3. Save, then double-click the file to try it before uploading.

What each setting does:

- **title**: the name in the browser tab and the header.
- **startupCommands**: game commands that run when a new game starts. They run
  after the "Would you like instructions?" question, because that's the game's
  first input. Students don't see them on screen, but they appear in the
  transcript. They don't run again when a student continues a game; the saved
  game already includes their effect.
- **allowInputCommand**: the game's `input <file>` command plays a file of
  commands, which could be a walkthrough. With `false`, `input` behaves as if
  the file doesn't exist. With `true`, students pick the file.
- **randomSeed**: the game's random numbers (dwarves, the pirate, some
  messages) always start from this number. With 511, the same commands give
  the same game every time, just like the exe. Change it if students are
  comparing notes too closely. Any whole number works.
- **autosave**: `false` turns off the per-turn save and **Continue your game**.

## Building

You only need this to change the code. The built file is already in `dist/`.

Requirements: Linux, macOS or WSL with bash, Node.js 18 or newer, Python 3,
curl and git.

```sh
web/build.sh
```

This writes `dist/colossal-cave.html`. The first time, it downloads the
Emscripten compiler (about 1.7 GB) into `web/.emsdk/`. If Emscripten is already
on your PATH, it's used instead. `build.sh` also copies the default teacher
settings from `web/frontend/teacher-settings.js` into the page, so edit that
file to change the defaults for every future build.

## Testing

```sh
(cd web && npm install && npx playwright install chromium)   # once
web/test.sh
```

`test.sh` runs three sets of checks:

1. **Native build vs Windows.** It builds the game with g++ and plays every
   script from `TestFiles/` with the same arguments as its `.bat` file. Each
   transcript is compared byte for byte with the Windows exe's log in
   `TestDirectory/`.
2. **Browser build vs native.** It plays the same scripts through the
   WebAssembly module the page uses (run under Node). The logs, the screen
   output and any save files must match the native build byte for byte.
3. **The page.** It opens `dist/colossal-cave.html` from `file://` in headless
   Chromium (Edge's engine) with networking blocked and checks:
   - playing, quitting, dying and reincarnating, and the map display;
   - Continue after closing the tab;
   - Save → New game → restore;
   - the transcript against the screen;
   - layout at 1366×768 and 1920×1080 at 100%, 125% and 150% scaling;
   - the teacher settings;
   - that no network request is made.

   Screenshots go to `web/build/screens/`.

### Results against the Windows reference logs

| Script (`.bat`) | Reference log | Native vs Windows | Browser vs native |
|---|---|---|---|
| `adventure1_win_cmd.txt` (`Test1_win.bat`) | `AdventureCCaw1_log_T1.Win.txt` | identical | identical |
| `adventure1_mem_cmd.txt` (`Test1_mem.bat`) | `AdventureCCaw1_log_T1.Mem.txt` | identical* | identical |
| `adventure1_p1_cmd.txt` + `_p2_` (`Test1_p1_p2.bat`) | `AdventureCCaw1_log_T1.p1_p2.txt` | identical | identical |
| `TestMovesDie_cmd.TXT` (`Test1_Die_NoDie.bat`) | `AdventureCCaw1_log_T1_Die.txt` | identical | identical |
| `TestMovesNoDie_cmd.TXT` (`Test1_Die_NoDie.bat`) | `AdventureCCaw1_log_T1_NoDie.txt` | identical | identical |
| `SingleWord_cmd.txt` (`SingleWord_Test.bat`) | `AdventureCCaw1_log_SingleWord_cmd.txt` | identical | identical |
| `DualWord_cmd.txt` (`DualWord_Test.bat`) | `AdventureCCaw1_log_DualWord_cmd.txt` | identical | identical |
| `adventure2_win_cmd.txt` (no `.bat`) | none | — | identical |
| `adventure1_win_cmd.txt` typed at the keyboard | none | — | identical |

\* `adventure1_mem_cmd.txt` ends with `input keyboard`. The Windows log shows the
tester then typed `blast` by hand, so the test types it too (`web/test/cases.json`).

Not compared:

- `AdventureCCaw1_log_Die.Mem.txt` and `AdventureCCaw1_log_NoDie.Mem.txt` have
  no `.bat`. Their turn counts (343 and 1236) don't match what the current
  `TestMoves*` scripts produce (427 and 1519), so they came from earlier
  versions of those scripts.
- `adventure.step.2.GM.txt` is a prose walkthrough, not a command script.

No script needed editing, and no mismatch was left over.

## How it works

The C++ in `Source/` is compiled unchanged. Everything new is in `web/`:

```
web/
  platform/        MSVC compatibility, force-included into every game file
    platform.h       stricmp, __max/__min, _MAX_PATH, MSVC rand()/srand(), file wrappers
    adv_platform.cpp Windows text-mode files, Windows-style file names, int main()
    include/         <conio.h>, <direct.h>, <crtdbg.h>, <STDIO.H>, and forwarders for
                     the headers Source/ includes with the wrong case
  io/              where the game meets the outside world
    adv_io_native.cpp  stdin/stdout, for the native test build
    adv_io_web.cpp     browser: waiting for input, autosave, file picker, downloads
    adv_pre.js         small Emscripten start-up script
  frontend/        index.template.html, app.js, styles.css, teacher-settings.js
  build.sh         Emscripten build + packaging into dist/colossal-cave.html
  build-native.sh  g++ build for the transcript tests
  package.mjs      inlines everything into one HTML file
  test.sh          runs every test
  test/            cases.json, run-tests.mjs, wasm-driver.mjs, browser-test.mjs
```

Key points:

- **Same random numbers.** The game seeds its random numbers with 511 and
  uses the C library's `rand()`. `platform.h` supplies the Microsoft
  version, so every build draws the same sequence as the Windows exe.
- **Same files.** The Windows exe writes its log in text mode (`\r\n` line
  endings) and accepts file names in any case. The platform layer does the
  same, which is what makes the logs match byte for byte. The game's own
  `puts` and `printf` replace the C library's (they also write the log), so
  the builds turn off compiler features that would bypass them
  (`-fno-builtin`, no `_FORTIFY_SOURCE`).
- **Waiting for input.** In the browser, the game's C++ pauses while it
  waits for the student (Emscripten's Asyncify). Every input passes through
  one function, `GetInputLine()`. A different wrapper is attached in each
  source file, so the page knows whether the game wants a command, a yes/no
  answer, a file name, or "Press Enter to Exit".
- **Autosave.** Before every turn, the game's own `SaveAdv` writes a save
  file with its output muted. The page keeps a copy in the browser's storage.
  Continue starts the game with the exe's own `-r <file>` option. The hook
  is a linker `--wrap` around `CGameState::PlayGameTurn`, so no source line
  changes.
- **The page itself.** The wasm is inlined as base64 and the six `ADVENT*.DAT`
  files are embedded inside it, so the page never fetches anything.

### Differences from the Windows exe

- **Typing `save` and answering the name prompt works.** The exe keeps the
  Enter key's newline in the name (`mygame\n.adv`), which Windows can't
  create, so this always failed there. The platform layer drops the newline.
  `save <name>` behaves the same in both versions.
- **Startup commands** run after the instructions question, not as a `-i`
  file (see Teacher settings above).
- **Randomness after Continue.** A continued game's random numbers restart
  from the seed, as they do when the exe restores a game.
- **Saves stay in the page.** `autocp` checkpoints are kept inside the page
  and aren't downloaded. They're lost when the tab closes; regular saves are
  downloaded.
- **The map's single-key prompt** waits for one key or a tap, as the exe's
  `getch()` does. Enter or Escape closes the map.

`.adv` save files have the same format in both versions. The samples in
`TestDirectory/` and `Source/cpblast1.adv` load in the browser build.
