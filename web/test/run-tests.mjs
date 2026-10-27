#!/usr/bin/env node
// Plays every TestFiles script through the native build and the wasm build
// (Node target) and compares the transcripts (the game's log files):
//   native vs the Windows reference logs in TestDirectory/
//   wasm   vs native, which must be byte-identical (log and screen output)
//
// Usage: node web/test/run-tests.mjs [--native-only] [--case NAME]
// Build first: web/build-native.sh and web/build.sh
import { spawnSync } from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const here = path.dirname(fileURLToPath(import.meta.url));
const web = path.resolve(here, '..');
const repo = path.resolve(web, '..');
const args = process.argv.slice(2);
const nativeOnly = args.includes('--native-only');
const onlyCase = args.includes('--case') ? args[args.indexOf('--case') + 1] : null;

const { cases } = JSON.parse(fs.readFileSync(path.join(here, 'cases.json'), 'utf8'));
const known = JSON.parse(fs.readFileSync(path.join(here, 'known-differences.json'), 'utf8'));

const targets = {
  native: { cmd: path.join(web, 'build/native/adventure'), pre: [] },
  wasm: { cmd: process.execPath, pre: [path.join(here, 'wasm-driver.mjs')] },
};
const targetNames = nativeOnly ? ['native'] : ['native', 'wasm'];
for (const t of targetNames) {
  const file = t === 'native' ? targets[t].cmd : path.join(web, 'build/web/adventure.js');
  if (!fs.existsSync(file)) {
    console.error(`Missing ${path.relative(repo, file)}; run web/build-native.sh and web/build.sh first.`);
    process.exit(2);
  }
}

function freshDir(dir) {
  fs.rmSync(dir, { recursive: true, force: true });
  fs.mkdirSync(dir, { recursive: true });
  for (const f of fs.readdirSync(path.join(repo, 'AdventGameFiles')))
    if (/^ADVENT\d\.DAT$/.test(f)) fs.copyFileSync(path.join(repo, 'AdventGameFiles', f), path.join(dir, f));
  for (const f of fs.readdirSync(path.join(repo, 'TestFiles')))
    fs.copyFileSync(path.join(repo, 'TestFiles', f), path.join(dir, f));
}

function runCase(c, target) {
  const dir = path.join(web, 'build/test', target, c.name);
  freshDir(dir);
  let stdout = '';
  for (const run of c.runs) {
    const t = targets[target];
    let stdin = run.stdin;
    if (run.stdinScript) {
      // Type a script at the keyboard: drop its comment lines (only command
      // files skip those) and the CRs a console would not send.
      const lines = fs.readFileSync(path.join(repo, 'TestFiles', run.stdinScript), 'latin1')
        .replace(/\r/g, '').split('\n').filter((l) => l === '' || /^[A-Za-z]/.test(l));
      stdin = lines.join('\n') + '\n' + run.stdin;
    }
    const r = spawnSync(t.cmd, [...t.pre, ...run.args], {
      cwd: dir, input: stdin, argv0: 'AdventureCCaw1.exe',
      maxBuffer: 1 << 28, timeout: 600000, encoding: 'latin1',
    });
    if (r.error) throw r.error;
    stdout += r.stdout;
  }
  const logFile = path.join(dir, c.log);
  const log = fs.existsSync(logFile) ? fs.readFileSync(logFile) : Buffer.alloc(0);
  const saves = {};
  for (const f of fs.readdirSync(dir))
    if (/\.adv$/i.test(f) && !fs.existsSync(path.join(repo, 'TestFiles', f))) saves[f] = fs.readFileSync(path.join(dir, f));
  return { log, stdout: Buffer.from(stdout, 'latin1'), saves };
}

function firstDiff(a, b) {
  const al = a.toString('latin1').split('\n'), bl = b.toString('latin1').split('\n');
  for (let i = 0; i < Math.max(al.length, bl.length); i++)
    if (al[i] !== bl[i])
      return `line ${i + 1}:\n      got:      ${JSON.stringify(al[i])}\n      expected: ${JSON.stringify(bl[i])}`;
  return 'identical lines, different bytes';
}

let failures = 0;
for (const c of cases) {
  if (onlyCase && c.name !== onlyCase) continue;
  const results = {};
  for (const t of targetNames) results[t] = runCase(c, t);

  if (c.reference) {
    const ref = fs.readFileSync(path.join(repo, 'TestDirectory', c.reference));
    const same = results.native.log.equals(ref);
    const k = known[c.name];
    if (same) console.log(`PASS  ${c.name}: native log matches TestDirectory/${c.reference} byte-for-byte`);
    else if (k) console.log(`KNOWN ${c.name}: native log differs from reference (${k})\n      ${firstDiff(results.native.log, ref)}`);
    else { failures++; console.log(`FAIL  ${c.name}: native log differs from TestDirectory/${c.reference}\n      ${firstDiff(results.native.log, ref)}`); }
  } else {
    console.log(`INFO  ${c.name}: no Windows reference log; native/wasm parity only`);
  }

  if (results.wasm) {
    const logSame = results.wasm.log.equals(results.native.log);
    const outSame = results.wasm.stdout.equals(results.native.stdout);
    const names = [...new Set([...Object.keys(results.native.saves), ...Object.keys(results.wasm.saves)])].sort();
    const badSaves = names.filter((f) => !(results.native.saves[f] && results.wasm.saves[f] && results.native.saves[f].equals(results.wasm.saves[f])));
    if (badSaves.length) { failures++; console.log(`FAIL  ${c.name}: save files differ between native and wasm: ${badSaves.join(', ')}`); }
    const saveNote = names.length ? `, ${names.length} save file(s) identical` : '';
    if (logSame && outSame) console.log(`PASS  ${c.name}: wasm log and screen output match native byte-for-byte (${results.native.log.length} bytes${saveNote})`);
    else {
      failures++;
      if (!logSame) console.log(`FAIL  ${c.name}: wasm log differs from native\n      ${firstDiff(results.wasm.log, results.native.log)}`);
      if (!outSame) console.log(`FAIL  ${c.name}: wasm screen output differs from native\n      ${firstDiff(results.wasm.stdout, results.native.stdout)}`);
    }
  }
}
console.log(failures ? `\n${failures} failure(s)` : '\nAll checks passed');
process.exit(failures ? 1 : 0);
