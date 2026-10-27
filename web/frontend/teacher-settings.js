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
