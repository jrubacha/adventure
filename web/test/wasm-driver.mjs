#!/usr/bin/env node
// Runs the browser build's WebAssembly module (web/build/web/adventure.js)
// under Node with the same interface as the native binary: game arguments
// on the command line, keyboard input on stdin, files in the current folder.
// Files in the current folder are copied into the module's virtual file
// system before the game starts and copied back when it exits, so the test
// runner can read the log. Autosave is on, to prove it leaves no trace.
import fs from 'node:fs';
import path from 'node:path';
import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';

const here = path.dirname(fileURLToPath(import.meta.url));
const require = createRequire(import.meta.url);
const createAdventure = require(path.join(here, '../build/web/adventure.js'));

let input = fs.readFileSync(0).toString('latin1');
const cwd = process.cwd();
const isData = (f) => /^ADVENT\d\.DAT$/i.test(f);
const before = new Map();
let autosaves = 0, lastAutosave = 0;

const host = {
  write: (s) => fs.writeSync(1, Buffer.from(s, 'latin1')),
  logged: () => {},
  readLine: async () => {
    if (!input.length) return null;
    const nl = input.indexOf('\n');
    const line = nl < 0 ? input : input.slice(0, nl + 1);
    input = input.slice(line.length);
    return line;
  },
  getch: async () => {
    if (!input.length) return '\n'.charCodeAt(0);
    const c = input.charCodeAt(0);
    input = input.slice(1);
    return c;
  },
  pickFile: async () => false,
  fileWritten: () => {},
  autosave: (p) => { autosaves++; lastAutosave = mod.FS.readFile('/' + p).length; },
  allowInput: () => true,
  autosaveEnabled: () => true,
};

let mod;
function copyBack() {
  for (const name of mod.FS.readdir('/')) {
    if (name === '.' || name === '..' || isData(name)) continue;
    const st = mod.FS.stat('/' + name);
    if (!mod.FS.isFile(st.mode)) continue;
    const data = mod.FS.readFile('/' + name);
    const old = before.get(name);
    if (!old || !Buffer.from(old).equals(Buffer.from(data)))
      fs.writeFileSync(path.join(cwd, name), data);
  }
}

mod = await createAdventure({
  advHost: host,
  onExit: () => {
    copyBack();
    if (process.env.ADV_DRIVER_STATS) console.error(`autosaves: ${autosaves}, last ${lastAutosave} bytes`);
  },
  preRun: [(m) => {
    for (const name of fs.readdirSync(cwd)) {
      const full = path.join(cwd, name);
      if (isData(name) || !fs.statSync(full).isFile()) continue;
      const data = fs.readFileSync(full);
      m.FS.writeFile('/' + name, data);
      before.set(name, data);
    }
  }],
});
mod.callMain(process.argv.slice(2));
