// Emscripten --pre-js: runs inside the module factory before the game starts.
// The game finds its ADVENT*.DAT files next to argv[0]; an exe-style name
// with no folder makes that the current directory, where they are embedded.
Module.thisProgram = 'AdventureCCaw1.exe';

// C string -> JS string, one character per byte (Latin-1), so nothing the
// game writes is lost or re-encoded.
Module.advString = function (ptr) {
  let s = '';
  for (let i = ptr; HEAPU8[i]; i++) s += String.fromCharCode(HEAPU8[i]);
  return s;
};
