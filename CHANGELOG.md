# Changelog

## v1.0 — 2026-10-27

Browser build of AdventureCCaw1: `dist/colossal-cave.html`, one self-contained
file that plays offline in Microsoft Edge from a local folder.

- The unchanged C++ from `Source/` is compiled to WebAssembly with Emscripten
  (Asyncify, single file, data files embedded). The page is 443 KB.
- MSVC compatibility layer in `web/platform/`: shims, the MSVC `rand()`,
  Windows text-mode files and Windows-style file names. Nothing in `Source/`
  changed.
- Native g++ reference build. All seven Windows reference logs in
  `TestDirectory/` match byte for byte. The browser build matches the native
  build byte for byte on every `TestFiles` script (logs, screen output and
  save files).
- Browser front end:
  - a terminal with a pinned input, command history and `\r`-aware rendering;
  - single-key map controls and yes/no buttons;
  - dark and light themes and adjustable text size, both remembered;
  - help and about panels.
- Saving:
  - autosave before every turn, with **Continue your game** on reopening;
  - save downloads as `colossal-cave-<name>.adv`;
  - restore and Load open a file picker;
  - transcript download in the Windows log format;
  - saves from the Windows exe load.
- Teacher settings block, editable in Notepad: title, silent startup commands,
  the `input` command, random seed, autosave.
- Tests: `web/test.sh` runs the transcript comparisons, then 57 checks of the
  page in headless Chromium from `file://` with networking blocked.
- Deviation from the exe: typing `save` and answering the name prompt now
  works. The exe kept the newline in the file name, so this always failed on
  Windows.
