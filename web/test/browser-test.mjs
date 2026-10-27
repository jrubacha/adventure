#!/usr/bin/env node
// Acceptance checks for dist/colossal-cave.html in headless Chromium (the
// engine Edge uses), opened from file:// with all networking blocked.
//
// Usage: node web/test/browser-test.mjs [--screens DIR]
// Uses the Playwright that is installed globally or in node_modules.
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { createRequire } from 'node:module';
import { fileURLToPath, pathToFileURL } from 'node:url';

const here = path.dirname(fileURLToPath(import.meta.url));
const repo = path.resolve(here, '../..');
const require = createRequire(import.meta.url);
const { chromium } = require('playwright');

const args = process.argv.slice(2);
const screens = args.includes('--screens') ? path.resolve(args[args.indexOf('--screens') + 1]) : path.join(repo, 'web/build/screens');
fs.mkdirSync(screens, { recursive: true });
const tmp = fs.mkdtempSync(path.join(os.tmpdir(), 'cave-'));

const page_ = path.join(repo, 'dist/colossal-cave.html');
const pageUrl = pathToFileURL(page_).href;

let failures = 0;
const results = [];
function check(name, ok, detail) {
  results.push({ name, ok });
  if (!ok) failures++;
  console.log(`${ok ? 'PASS' : 'FAIL'}  ${name}${detail ? ' (' + detail + ')' : ''}`);
}

const offRequests = [];
const pageErrors = [];
const browser = await chromium.launch();

async function newContext(opts = {}) {
  const ctx = await browser.newContext({ viewport: { width: 1366, height: 768 }, acceptDownloads: true, ...opts });
  await ctx.setOffline(true);
  ctx.on('request', (r) => { if (!/^(file|blob|data):/.test(r.url())) offRequests.push(r.url()); });
  await ctx.route('**/*', (r) => (/^(file|blob|data):/.test(r.request().url()) ? r.continue() : r.abort()));
  return ctx;
}

async function open(ctx, url = pageUrl) {
  const p = await ctx.newPage();
  p.on('pageerror', (e) => pageErrors.push(String(e)));
  p.on('console', (m) => { if (m.type() === 'error') pageErrors.push(m.text()); });
  await p.goto(url);
  return p;
}

const ready = (p) => p.waitForFunction(() => {
  const s = window.__adventure && window.__adventure.state();
  return s && (s.waiting || s.waitingKey || s.ended);
});
const kind = (p) => p.evaluate(() => { const s = window.__adventure.state(); return s.waiting ? s.waiting.kind : s.waitingKey ? 'key' : s.ended ? 'ended' : null; });
const outText = (p) => p.evaluate(() => document.getElementById('out').textContent);

async function type(p, cmd) {
  await ready(p);
  await p.fill('#cmd', cmd);
  await p.press('#cmd', 'Enter');
  await ready(p);
}
async function play(p, cmds) { for (const c of cmds) await type(p, c); }
async function since(p, marker, fn) {   // output produced by fn()
  const before = (await outText(p)).length;
  await fn();
  return (await outText(p)).slice(before);
}

// Mimic the console: CRLF -> LF, "\r" returns to the start of the line.
function consoleText(s) {
  return s.replace(/\r\n/g, '\n').split('\n').map((line) => {
    let out = '', col = 0;
    for (const ch of line) {
      if (ch === '\r') { col = 0; continue; }
      out = out.slice(0, col) + ch + out.slice(col + 1);
      col++;
    }
    return out;
  }).join('\n');
}

// 1. One file, small, no network ---------------------------------------
{
  const size = fs.statSync(page_).size;
  check('dist/colossal-cave.html is one file under 1.5 MB', size < 1.5 * 1024 * 1024, `${(size / 1024).toFixed(0)} KB`);
}

// 2. Basic play, yes/no buttons, transcript ------------------------------
{
  const ctx = await newContext();
  const p = await open(ctx);
  await ready(p);
  check('game starts at the instructions question', (await kind(p)) === 1 && (await outText(p)).includes('Welcome to ADVENTURE!'));
  check('opening credits are intact', /Willie Crowther[\s\S]*Don Woods[\s\S]*J\. R\. Jaeger[\s\S]*Jerry D\. Pohl[\s\S]*Al Whinery[\s\S]*John W\. Kennedy/.test(await outText(p)));
  check('yes/no buttons shown at a question', await p.isVisible('#yesno'));
  await p.click('#yesno button[data-answer="n"]');
  await ready(p);
  check('No button answers the question', (await kind(p)) === 0 && (await outText(p)).includes('end of a road'));
  check('Save/Load enabled at the command prompt', !(await p.isDisabled('#btn-save')) && !(await p.isDisabled('#btn-load')));
  check('input has spellcheck, autocorrect and autocapitalize off', await p.evaluate(() => {
    const i = document.getElementById('cmd');
    return i.spellcheck === false && i.getAttribute('autocorrect') === 'off' && i.getAttribute('autocapitalize') === 'off';
  }));
  await play(p, ['in', 'take lamp', 'take keys', 'xyzzy']);
  // history
  await p.press('#cmd', 'ArrowUp');
  check('Up arrow recalls the last command', (await p.inputValue('#cmd')) === 'xyzzy');
  await p.fill('#cmd', '');
  // click in output returns focus to the input
  await p.evaluate(() => document.getElementById('btn-help').focus());
  await p.click('#screen', { position: { x: 400, y: 200 } });
  check('clicking the output focuses the input', await p.evaluate(() => document.activeElement.id === 'cmd'));
  await play(p, ['quit']);
  check('quit asks a yes/no question', (await kind(p)) === 1);
  check('Save disabled at a question', await p.isDisabled('#btn-save'));
  await type(p, 'y');
  check('end-of-game score shown', /Score:\s+\d+/.test(await outText(p)) && (await kind(p)) === 3);
  await p.press('#cmd', 'Enter');
  await p.waitForFunction(() => window.__adventure.state().ended);
  check('after "Press Enter to Exit" the game ends with a New game button', await p.isVisible('#btn-again'));

  const dl = p.waitForEvent('download');
  await p.click('#btn-transcript2');
  const d = await dl;
  const tfile = path.join(tmp, d.suggestedFilename());
  await d.saveAs(tfile);
  const transcript = fs.readFileSync(tfile, 'latin1');
  check('transcript file is named colossal-cave-transcript-<date>.txt', /^colossal-cave-transcript-\d{4}-\d{2}-\d{2}\.txt$/.test(d.suggestedFilename()), d.suggestedFilename());
  check('transcript uses Windows log line endings (CRLF)', transcript.includes('\r\n') && !/[^\r]\n/.test(transcript));
  const screen = await outText(p);
  check('transcript matches the screen, including commands', consoleText(transcript) === screen,
    consoleText(transcript) === screen ? '' : `transcript ${consoleText(transcript).length} chars vs screen ${screen.length}`);
  await p.screenshot({ path: path.join(screens, 'ended.png') });
  await ctx.close();
}

// 3. Continue your game after closing the tab ------------------------------
{
  const cmds = ['n', 'in', 'take lamp', 'take keys', 'out', 's', 's'];
  const probe = ['look', 'inventory', 'score'];

  const refCtx = await newContext();
  const ref = await open(refCtx);
  await play(ref, cmds);
  const expected = await since(ref, '', () => play(ref, probe));
  await refCtx.close();

  const ctx = await newContext();
  let p = await open(ctx);
  await play(p, cmds);
  await p.close();                                  // close the tab
  p = await open(ctx);                              // reopen the file
  await p.waitForSelector('#dlg-start[open]');
  check('reopening offers "Continue your game"', await p.isVisible('#start-continue'));
  await p.screenshot({ path: path.join(screens, 'continue-dialog.png') });
  await p.click('#start-continue');
  await ready(p);
  const got = await since(p, '', () => play(p, probe));
  const norm = (s) => s.replace(/Turns:\s+\d+/, 'Turns: N');
  check('continued game has the same location, inventory and score', norm(got) === norm(expected));
  const turns = (s) => (s.match(/Turns:\s+(\d+)/) || [])[1];
  check('continued game has the same turn count', turns(got) === turns(expected), `${turns(got)} vs ${turns(expected)}`);
  const transcript = await p.evaluate(() => window.__adventure.transcript());
  check('transcript carries over to the continued game', transcript.includes('>take keys') && transcript.includes('>inventory'));
  await ctx.close();
}

// 4. Save to file, New game, restore from that file ---------------------
{
  const ctx = await newContext();
  const p = await open(ctx);
  await play(p, ['n', 'in', 'take lamp', 'take bottle', 'out']);
  const before = await since(p, '', () => play(p, ['inventory', 'look']));
  await p.click('#btn-save');
  await ready(p);
  check('Save types the game\'s save command (filename prompt)', (await kind(p)) === 2 && (await p.textContent('#prompt')).includes('name the saved game'));
  const dl = p.waitForEvent('download');
  await type(p, 'mygame');
  const d = await dl;
  check('save downloads colossal-cave-<name>.adv', d.suggestedFilename() === 'colossal-cave-mygame.adv', d.suggestedFilename());
  const saveFile = path.join(tmp, d.suggestedFilename());
  await d.saveAs(saveFile);

  await p.click('#btn-new');
  await p.waitForSelector('#dlg-new[open]');
  check('New game asks for confirmation', await p.isVisible('#new-yes'));
  await p.click('#new-yes');
  await ready(p);
  await type(p, 'n');
  check('New game starts over', (await outText(p)).includes('end of a road') && !(await outText(p)).includes('take bottle'));

  // restore <name>: the file isn't in the game's folder, so the picker dialog opens
  await p.fill('#cmd', 'restore mygame');
  await p.press('#cmd', 'Enter');
  await p.waitForSelector('#dlg-pick[open]');
  check('restore opens a file picker for a save the page does not have', await p.isVisible('#pick-choose'));
  await p.screenshot({ path: path.join(screens, 'restore-picker.png') });
  const chooser = p.waitForEvent('filechooser');
  await p.click('#pick-choose');
  await (await chooser).setFiles(saveFile);
  await ready(p);
  const after = await since(p, '', () => play(p, ['inventory', 'look']));
  check('restore from the downloaded file returns to the saved state', after === before);

  // Load button
  await play(p, ['in', 'drop lamp']);
  const chooser2 = p.waitForEvent('filechooser');
  await p.click('#btn-load');
  await (await chooser2).setFiles(saveFile);
  await p.waitForFunction(() => /drop lamp[\s\S]*>restore mygame\n/.test(document.getElementById('out').textContent));
  await ready(p);
  const after2 = await since(p, '', () => play(p, ['inventory', 'look']));
  check('Load button restores the chosen file', after2 === before);

  // Cancel in the picker: the game reports the missing file itself
  await p.fill('#cmd', 'restore nosuchgame');
  await p.press('#cmd', 'Enter');
  await p.waitForSelector('#dlg-pick[open]');
  await p.click('#pick-cancel');
  await ready(p);
  check('cancelling the picker gives the game\'s own error', (await outText(p)).includes("Sorry, I can't open the file...nosuchgame.adv"));
  await ctx.close();
}

// 5. Map display (single key), dying and reincarnating ------------------
{
  const ctx = await newContext();
  const p = await open(ctx);
  await play(p, ['n', 'forest', 'ascend', 'forest', 'climb tree', 'get map']);
  await type(p, 'map map');      // "map" alone asks "map what?"
  check('map waits for a single key with on-screen buttons', (await kind(p)) === 'key' && await p.isVisible('#keys'));
  await p.screenshot({ path: path.join(screens, 'map.png') });
  check('output follows the newest text (no "Jump to newest" button)', await p.isHidden('#jump') &&
    await p.evaluate(() => { const e = document.getElementById('screen'); return e.scrollHeight - e.scrollTop - e.clientHeight < 48; }));
  await p.mouse.move(600, 300);
  await p.mouse.wheel(0, -600);
  await p.waitForSelector('#jump:not([hidden])');
  const scrollAfterWheel = await p.evaluate(() => document.getElementById('screen').scrollTop);
  await p.click('#keys button[data-key="+"]');
  await ready(p);
  check('scrolling up to reread stops auto-scroll and shows "Jump to newest"',
    await p.isVisible('#jump') && (await p.evaluate(() => document.getElementById('screen').scrollTop)) === scrollAfterWheel);
  await p.click('#jump');
  check('"Jump to newest" returns to the bottom', await p.isHidden('#jump'));
  let t = await since(p, '', async () => { await p.click('#keys button[data-key="+"]'); await ready(p); });
  check('"+ Next place" shows the next location', /Press \+ for Forward/.test(t) && (await kind(p)) === 'key');
  t = await since(p, '', async () => { await p.press('#cmd', '-'); await ready(p); });
  check('pressing "-" on the keyboard works without Enter', /Press \+ for Forward/.test(t) && (await kind(p)) === 'key');
  t = await since(p, '', async () => { await p.click('#keys button:has-text("Done")'); await ready(p); });
  t = (await outText(p)) + (await p.textContent('#prompt'));
  check('Done closes the map', t.endsWith('Cave map closed.>') && (await kind(p)) === 0);

  await play(p, ['tp 017', 'jump']);
  check('dying asks about reincarnation', (await outText(p)).includes('reincarnate you?') && (await kind(p)) === 1);
  await p.click('#yesno button[data-answer="y"]');
  await ready(p);
  check('reincarnation works', (await outText(p)).includes('POOF') && (await kind(p)) === 0);
  await ctx.close();
}

// 6. Layout at common laptop sizes and Windows display scaling -------------
{
  const sizes = [[1366, 768], [1920, 1080]];
  const scales = [1, 1.25, 1.5];
  for (const [w, h] of sizes) for (const s of scales) {
    // Edge's own tabs and address bar take about 80 px of height.
    const vw = Math.round(w / s), vh = Math.round(h / s) - 80;
    const ctx = await newContext({ viewport: { width: vw, height: vh }, deviceScaleFactor: s });
    const p = await open(ctx);
    await play(p, ['n', 'help help']);
    const m = await p.evaluate(() => ({
      doc: document.documentElement.scrollWidth - document.documentElement.clientWidth,
      screen: document.getElementById('screen').scrollWidth - document.getElementById('screen').clientWidth,
      bar: document.querySelector('.bar').scrollWidth - document.querySelector('.bar').clientWidth,
      inputVisible: (() => { const r = document.getElementById('cmd').getBoundingClientRect(); return r.bottom <= innerHeight && r.width > 60; })(),
    }));
    check(`layout ${w}x${h} at ${s * 100}%: no horizontal scroll, input visible`,
      m.doc <= 0 && m.screen <= 0 && m.bar <= 0 && m.inputVisible, JSON.stringify(m));
    await p.screenshot({ path: path.join(screens, `layout-${w}x${h}-${s * 100}.png`) });
    if (w === 1366 && s === 1.5) {
      for (let i = 0; i < 7; i++) await p.click('#btn-bigger');
      const big = await p.evaluate(() => document.documentElement.scrollWidth - document.documentElement.clientWidth);
      check('largest text size still has no horizontal scroll', big <= 0);
      await p.screenshot({ path: path.join(screens, 'layout-largest-text.png') });
    }
    await ctx.close();
  }
}

// 7. Theme and text size persist ------------------------------------------
{
  const ctx = await newContext();
  let p = await open(ctx);
  await ready(p);
  await p.click('#btn-theme');
  await p.click('#btn-bigger');
  await p.screenshot({ path: path.join(screens, 'light-theme.png') });
  await p.close();
  p = await open(ctx);
  const st = await p.evaluate(() => ({ theme: document.documentElement.dataset.theme, size: getComputedStyle(document.documentElement).getPropertyValue('--font-size').trim() }));
  check('theme and text size persist between sessions', st.theme === 'light' && st.size === '20px', JSON.stringify(st));
  await p.click('#btn-help');
  await p.screenshot({ path: path.join(screens, 'help.png') });
  const fits = await p.evaluate(() => { const d = document.getElementById('dlg-help'); return d.scrollHeight <= d.clientHeight + 1; });
  check('Help fits on one screen at 1366x768', fits);
  await ctx.close();
}

// 8. Runs without storage --------------------------------------------------
{
  const ctx = await newContext();
  await ctx.addInitScript(() => {
    Object.defineProperty(window, 'localStorage', { get() { throw new Error('storage blocked'); } });
  });
  const p = await open(ctx);
  await play(p, ['n', 'in', 'take lamp']);
  check('game runs normally when storage is unavailable', (await outText(p)).includes('>take lamp\nOk.'));
  await ctx.close();
}

// 9. Teacher settings edited in the built file ------------------------------
{
  let html = fs.readFileSync(page_, 'utf8');
  html = html.replace('title: "Colossal Cave Adventure",', 'title: "Period 3 Cave",')
    .replace('// "friendly 1",', '"friendly 1",')
    .replace('// "hints 0",', '"hints 0",');
  const edited = path.join(tmp, 'edited.html');
  fs.writeFileSync(edited, html);
  const ctx = await newContext();
  const p = await open(ctx, pathToFileURL(edited).href);
  await play(p, ['n', 'in']);
  check('title setting shows in the tab and header', (await p.title()) === 'Period 3 Cave' && (await p.textContent('#title')) === 'Period 3 Cave');
  const screen = await outText(p);
  const transcript = await p.evaluate(() => window.__adventure.transcript());
  check('startup commands take effect (friendly shows location numbers)', /Location \d+:/.test(screen));
  check('startup commands are hidden on screen but kept in the transcript',
    !screen.includes('friendly 1') && transcript.includes('>friendly 1') && transcript.includes('>hints 0'));
  await p.fill('#cmd', 'input cheats.txt');
  await p.press('#cmd', 'Enter');
  await ready(p);
  check('allowInputCommand false: "input <file>" opens nothing', !(await p.isVisible('#dlg-pick')) && (await kind(p)) === 0);
  await ctx.close();

  const html2 = fs.readFileSync(page_, 'utf8').replace('allowInputCommand: false,', 'allowInputCommand: true,');
  const edited2 = path.join(tmp, 'edited2.html');
  fs.writeFileSync(edited2, html2);
  const script = path.join(tmp, 'cmds.txt');
  fs.writeFileSync(script, 'in\r\ntake lamp\r\n');
  const ctx2 = await newContext();
  const p2 = await open(ctx2, pathToFileURL(edited2).href);
  await play(p2, ['n']);
  await p2.fill('#cmd', 'input cmds.txt');
  await p2.press('#cmd', 'Enter');
  await p2.waitForSelector('#dlg-pick[open]');
  const ch = p2.waitForEvent('filechooser');
  await p2.click('#pick-choose');
  await (await ch).setFiles(script);
  await p2.waitForFunction(() => document.getElementById('out').textContent.includes('>take lamp'));
  await ready(p2);
  check('allowInputCommand true: "input <file>" runs the chosen script', (await outText(p2)).includes('>take lamp\nOk.'));
  await ctx2.close();
}

await browser.close();

check('no network requests were made', offRequests.length === 0, offRequests.slice(0, 3).join(', '));
check('no script errors', pageErrors.length === 0, pageErrors.slice(0, 3).join(' | '));
console.log(`\n${results.length - failures}/${results.length} checks passed. Screenshots: ${path.relative(process.cwd(), screens)}`);
process.exit(failures ? 1 : 0);
