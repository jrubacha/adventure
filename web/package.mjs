#!/usr/bin/env node
// Inlines the Emscripten module, front end and teacher settings into one
// self-contained page: dist/colossal-cave.html. Run by build.sh.
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const web = path.dirname(fileURLToPath(import.meta.url));
const repo = path.resolve(web, '..');
const read = (p) => fs.readFileSync(path.join(web, p), 'utf8');
const MAX_BYTES = 1.5 * 1024 * 1024;

// "</script" inside inlined code would end the <script> element early.
const scriptSafe = (js) => js.replace(/<\/(script)/gi, '<\\/$1').replace(/<!--/g, '<\\!--');

const version = 'v' + read('VERSION').trim();

const parts = {
  TEACHER_SETTINGS: read('frontend/teacher-settings.js').trim(),
  VERSION: version,
  STYLES: read('frontend/styles.css'),
  ENGINE: scriptSafe(read('build/web/adventure.js')),
  APP: scriptSafe(read('frontend/app.js')),
};

let html = read('frontend/index.template.html');
html = html.replace(/\{\{([A-Z_]+)\}\}/g, (m, key) => (key in parts ? parts[key] : m));

const out = path.join(repo, 'dist/colossal-cave.html');
fs.mkdirSync(path.dirname(out), { recursive: true });
fs.writeFileSync(out, html);
const size = fs.statSync(out).size;
console.log(`Wrote dist/colossal-cave.html (${(size / 1024).toFixed(0)} KB)`);
if (size > MAX_BYTES) {
  console.error(`dist/colossal-cave.html is larger than the 1.5 MB target`);
  process.exit(1);
}
