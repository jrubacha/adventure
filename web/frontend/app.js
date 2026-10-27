// Colossal Cave Adventure: browser front end for the AdventureCCaw1 WebAssembly
// build. The game itself runs unchanged inside the wasm module; this file is
// its terminal, keyboard, file chooser and save storage (see web/io/adv_io_web.cpp
// for the other side of Module.advHost).
(function () {
  'use strict';

  const DEFAULTS = {
    title: 'Colossal Cave Adventure',
    startupCommands: [],
    allowInputCommand: false,
    randomSeed: 511,
    autosave: true,
  };
  const S = Object.assign({}, DEFAULTS,
    typeof TEACHER_SETTINGS === 'object' && TEACHER_SETTINGS ? TEACHER_SETTINGS : {});
  if (!Array.isArray(S.startupCommands)) S.startupCommands = [];

  // What the game is asking for (enum AdvPromptKind in web/io/adv_io.h).
  const KIND_MAIN = 0, KIND_QUESTION = 1, KIND_FILENAME = 2, KIND_EXIT = 3;
  const LOG_NAME = 'AdventureCCaw1_log.txt';     // the game's default log file
  const AUTOSAVE_NAME = 'autosave';              // restored with the exe's -r option

  // ---- storage: every access may throw (private mode, blocked storage) ----
  const PREFIX = 'colossal-cave:';
  const store = {
    get(k) { try { return localStorage.getItem(PREFIX + k); } catch (e) { return null; } },
    set(k, v) { try { localStorage.setItem(PREFIX + k, v); return true; } catch (e) { return false; } },
    del(k) { try { localStorage.removeItem(PREFIX + k); } catch (e) { /* ignore */ } },
  };

  const $ = (id) => document.getElementById(id);
  const screenEl = $('screen'), outEl = $('out'), partialEl = $('partial');
  const cmdEl = $('cmd'), promptEl = $('prompt'), formEl = $('inputrow');
  const jumpEl = $('jump'), keysEl = $('keys'), yesnoEl = $('yesno'), endedEl = $('ended');
  const buttons = {
    save: $('btn-save'), load: $('btn-load'), transcript: $('btn-transcript'), newGame: $('btn-new'),
    smaller: $('btn-smaller'), bigger: $('btn-bigger'), theme: $('btn-theme'),
    help: $('btn-help'), about: $('btn-about'),
  };

  // ---- byte helpers (the game's text is one byte per character) ----
  const toBytes = (s) => { const b = new Uint8Array(s.length); for (let i = 0; i < s.length; i++) b[i] = s.charCodeAt(i) & 0xff; return b; };
  const fromBytes = (b) => { let s = ''; for (let i = 0; i < b.length; i += 8192) s += String.fromCharCode.apply(null, b.subarray(i, i + 8192)); return s; };
  const toBase64 = (b) => btoa(fromBytes(b));
  const fromBase64 = (s) => toBytes(atob(s));

  function download(name, bytes, type) {
    const blob = new Blob([bytes], { type: type || 'application/octet-stream' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = name;
    document.body.appendChild(a);
    a.click();
    a.remove();
    setTimeout(() => URL.revokeObjectURL(url), 10000);
  }

  // ---- preferences: theme and text size ----
  const prefs = (() => { try { return JSON.parse(store.get('prefs') || '{}') || {}; } catch (e) { return {}; } })();
  function applyPrefs() {
    const size = Math.min(32, Math.max(14, prefs.fontSize || 18));
    document.documentElement.style.setProperty('--font-size', size + 'px');
    document.documentElement.dataset.theme = prefs.theme === 'light' ? 'light' : 'dark';
    buttons.theme.textContent = prefs.theme === 'light' ? 'Dark' : 'Light';
    buttons.smaller.disabled = size <= 14;
    buttons.bigger.disabled = size >= 32;
  }
  function savePrefs() { store.set('prefs', JSON.stringify(prefs)); applyPrefs(); }

  // ---- terminal ----
  // The game writes a console stream: "\n" ends a line, "\r" returns to the
  // start of the line (yes/no prompts overwrite themselves this way), and a
  // prompt is the unfinished last line. While the game waits for a typed line,
  // that unfinished line is shown as the input prompt instead of in the output.
  const term = {
    line: '', col: 0, hidden: false, pending: '',
    write(s) {
      for (let i = 0; i < s.length; i++) {
        const ch = s[i];
        if (ch === '\n') this.commit();
        else if (ch === '\r') this.col = 0;
        else {
          this.line = this.line.slice(0, this.col) + ch + this.line.slice(this.col + 1);
          this.col++;
        }
      }
      this.render();
    },
    commit() {
      if (!this.hidden) this.pending += this.line + '\n';
      this.line = '';
      this.col = 0;
    },
    render() {
      if (this.pending) {
        outEl.insertBefore(document.createTextNode(this.pending), partialEl);
        this.pending = '';
      }
      // A short prompt ("y or n : (y) : >") sits beside the input; a long
      // unfinished line stays in the output and only its ">" moves down.
      const waitingForLine = game && game.waiting && !this.hidden;
      let shown = this.hidden ? '' : this.line, prompt = '>';
      if (waitingForLine) {
        if (shown.length <= 48) { prompt = shown || '>'; shown = ''; }
        else if (shown.endsWith('>')) shown = shown.slice(0, -1);
      }
      partialEl.textContent = shown;
      promptEl.textContent = prompt;
      if (follow) scrollToBottom();
      updateJump();
    },
    echo(text) {               // what the player typed, after the prompt, as the log has it
      this.line = this.line + text;
      this.col = this.line.length;
      this.commit();
      this.render();
    },
    clear() {
      this.line = ''; this.col = 0; this.pending = ''; this.hidden = false;
      while (outEl.firstChild !== partialEl) outEl.removeChild(outEl.firstChild);
      partialEl.textContent = '';
    },
  };

  // Output follows the newest text until the student scrolls up to reread;
  // scrolling back down (or "Jump to newest", or typing) resumes following.
  let follow = true;
  function isAtBottom() { return screenEl.scrollHeight - screenEl.scrollTop - screenEl.clientHeight < 48; }
  function scrollToBottom() { screenEl.scrollTop = screenEl.scrollHeight; }
  function updateJump() { jumpEl.hidden = follow || isAtBottom(); }
  let lastScrollTop = 0;
  screenEl.addEventListener('scroll', () => {
    // Only the student's own scrolling (up) stops following.
    if (screenEl.scrollTop < lastScrollTop - 2 && !isAtBottom()) follow = false;
    else if (isAtBottom()) follow = true;
    lastScrollTop = screenEl.scrollTop;
    updateJump();
  }, { passive: true });
  jumpEl.addEventListener('click', () => { follow = true; scrollToBottom(); updateJump(); focusInput(); });
  // Text size changes, the map/yes-no buttons appearing and window resizes
  // all change the output's size; keep the newest text in view.
  if (typeof ResizeObserver === 'function') {
    const ro = new ResizeObserver(() => { if (follow) scrollToBottom(); updateJump(); });
    ro.observe(screenEl);
    ro.observe(outEl);
  }

  // ---- the running game ----
  let game = null;          // { mod, gen, waiting, waitingKey, transcript, ... }
  let generation = 0;

  function makeHost(state) {
    const live = () => game === state;
    return {
      write(s) { if (live()) term.write(s); },
      logged(s) { if (live()) state.transcript.push(s); },
      readLine(kind) {
        if (!live()) return new Promise(() => {});
        if (kind === KIND_MAIN) {
          state.savedThisTurn = false;
          if (!state.startupQueued) {
            state.startupQueued = true;
            state.queue = state.mode === 'new' ? S.startupCommands.map(String) : [];
          }
        }
        if (kind === KIND_EXIT && !state.savedThisTurn) clearAutosave();
        if (state.queue.length) {           // teacher's startup commands, run out of sight
          term.hidden = true;
          const line = state.queue.shift();
          term.echo(line);
          return Promise.resolve(line + '\n');
        }
        if (term.hidden) {
          term.hidden = false;
          term.render();
        }
        return new Promise((resolve) => {
          state.waiting = { kind, resolve };
          updateControls();
          term.render();
          focusInput();
        });
      },
      getch() {
        if (!live()) return new Promise(() => {});
        return new Promise((resolve) => {
          state.waitingKey = resolve;
          updateControls();
          term.render();
          focusInput();
        });
      },
      pickFile(path, what) {
        if (!live()) return new Promise(() => {});
        return pickFile(path, what).then((file) => {
          if (!file || !live()) return false;
          state.mod.FS.writeFile('/' + path, file);
          return true;
        });
      },
      fileWritten(path) {
        if (!live() || !/\.adv$/i.test(path) || path.indexOf('/') >= 0) return;   // AutoCP/ checkpoints stay inside
        state.savedThisTurn = true;
        const base = path.replace(/\.adv$/i, '');
        download('colossal-cave-' + base + '.adv', state.mod.FS.readFile('/' + path));
      },
      autosave(path) {
        if (!live()) return;
        const data = state.mod.FS.readFile('/' + path);
        store.set('autosave', JSON.stringify({ v: 1, savedAt: Date.now(), title: S.title, adv: toBase64(data) }));
        persistTranscript(state);
      },
      allowInput() { return !!S.allowInputCommand; },
      autosaveEnabled() { return !!S.autosave; },
    };
  }

  function persistTranscript(state) {
    if (!S.autosave) return;
    store.set('transcript', state.transcript.join(''));
  }
  function clearAutosave() { store.del('autosave'); }
  function savedGame() {
    if (!S.autosave) return null;
    try {
      const saved = JSON.parse(store.get('autosave') || 'null');
      return saved && saved.v === 1 && saved.adv ? saved : null;
    } catch (e) { return null; }
  }

  async function startGame(mode) {
    const saved = mode === 'continue' ? savedGame() : null;
    if (mode === 'continue' && !saved) mode = 'new';
    if (mode === 'new') { clearAutosave(); store.del('transcript'); }

    const state = {
      gen: ++generation, mode, mod: null, waiting: null, waitingKey: null, ended: false,
      transcript: [], queue: [], startupQueued: false, savedThisTurn: false,
    };
    if (saved) {
      const t = store.get('transcript');
      if (t) state.transcript.push(t);
    }
    game = state;
    term.clear();
    updateControls();

    const seed = Number.isFinite(Number(S.randomSeed)) ? String(Math.trunc(Number(S.randomSeed)) >>> 0) : '511';
    try {
      state.mod = await createAdventure({
        advHost: makeHost(state),
        onExit: () => { if (game === state) gameOver(); },
        onAbort: (what) => { if (game === state) showFatal(what); },
        print: (s) => console.log(s),
        printErr: (s) => console.warn(s),
        preRun: [(m) => {
          m.ENV.ADV_SEED = seed;
          if (saved) {
            m.FS.writeFile('/' + AUTOSAVE_NAME + '.adv', fromBase64(saved.adv));
            if (state.transcript.length) m.FS.writeFile('/' + LOG_NAME, toBytes(state.transcript.join('')));
          }
        }],
      });
    } catch (e) {
      showFatal(e);
      return;
    }
    if (game !== state) return;
    try {
      state.mod.callMain(saved ? ['-r', AUTOSAVE_NAME] : []);
    } catch (e) {
      if (!(e && e.name === 'ExitStatus')) showFatal(e);
    }
  }

  function gameOver() {
    game.ended = true;
    game.waiting = null;
    game.waitingKey = null;
    clearAutosave();
    updateControls();
    term.render();
    $('btn-again').focus();
  }

  function showFatal(what) {
    term.write('\n\n[The game stopped unexpectedly: ' + (what && what.message ? what.message : what) + ']\n');
    if (game) { game.ended = true; game.waiting = null; }
    updateControls();
  }

  // ---- input ----
  function submit(text) {
    const w = game && game.waiting;
    if (!w) return;
    game.waiting = null;
    text = String(text).replace(/[\r\n]/g, ' ').slice(0, 900);
    if (text.trim() && (!history.length || history[history.length - 1] !== text)) history.push(text);
    historyPos = history.length;
    follow = true;
    term.echo(text);
    scrollToBottom();
    updateControls();
    w.resolve(text + '\n');
  }

  function sendKey(code) {
    const k = game && game.waitingKey;
    if (!k) return;
    game.waitingKey = null;
    updateControls();
    k(code);
  }

  const history = [];
  let historyPos = 0;

  formEl.addEventListener('submit', (e) => {
    e.preventDefault();
    if (game && game.waiting) {
      const text = cmdEl.value;
      cmdEl.value = '';
      submit(text);
    }
  });

  cmdEl.addEventListener('keydown', (e) => {
    if (game && game.waitingKey) {          // map display: one key, no Enter
      if (e.key.length === 1 || e.key === 'Enter' || e.key === 'Escape') {
        e.preventDefault();
        sendKey(e.key === 'Enter' || e.key === 'Escape' ? 13 : e.key.charCodeAt(0));
      }
      return;
    }
    if (e.key === 'ArrowUp' && history.length) {
      e.preventDefault();
      historyPos = Math.max(0, historyPos - 1);
      cmdEl.value = history[historyPos];
      requestAnimationFrame(() => cmdEl.setSelectionRange(cmdEl.value.length, cmdEl.value.length));
    } else if (e.key === 'ArrowDown' && history.length) {
      e.preventDefault();
      historyPos = Math.min(history.length, historyPos + 1);
      cmdEl.value = historyPos < history.length ? history[historyPos] : '';
    }
  });
  // On-screen keyboards may insert text without keydown key values.
  cmdEl.addEventListener('input', () => {
    if (game && game.waitingKey && cmdEl.value) {
      const c = cmdEl.value.charCodeAt(cmdEl.value.length - 1);
      cmdEl.value = '';
      sendKey(c);
    }
  });

  keysEl.addEventListener('click', (e) => {
    const b = e.target.closest('button[data-key]');
    if (b) { sendKey(b.dataset.key.charCodeAt(0)); focusInput(); }
  });
  yesnoEl.addEventListener('click', (e) => {
    const b = e.target.closest('button[data-answer]');
    if (b) { submit(b.dataset.answer); focusInput(); }
  });

  function anyDialogOpen() { return !!document.querySelector('dialog[open]'); }
  function focusInput() {
    if (anyDialogOpen() || cmdEl.disabled) return;
    cmdEl.focus({ preventScroll: true });
  }
  // Clicking or tapping the output brings the keyboard back (unless the
  // student is selecting text to copy).
  screenEl.addEventListener('click', () => {
    const sel = window.getSelection && window.getSelection();
    if (sel && String(sel).length) return;
    focusInput();
  });

  function updateControls() {
    const g = game;
    const w = g && g.waiting;
    const atMain = !!(w && w.kind === KIND_MAIN);
    const ended = !!(g && g.ended);
    buttons.save.disabled = !atMain;
    buttons.load.disabled = !atMain;
    keysEl.hidden = !(g && g.waitingKey);
    yesnoEl.hidden = !(w && w.kind === KIND_QUESTION);
    endedEl.hidden = !ended;
    formEl.hidden = ended;
    cmdEl.disabled = !(w || (g && g.waitingKey));
    cmdEl.placeholder = g && g.waitingKey ? 'press a key' : '';
    if (!cmdEl.disabled) focusInput();
  }

  // ---- toolbar ----
  buttons.save.addEventListener('click', () => {
    if (game && game.waiting && game.waiting.kind === KIND_MAIN) submit('save');
    focusInput();
  });

  // Load picks the file first (browsers only open the chooser from a click),
  // puts it in the game's folder, then types the game's own restore command.
  const loadInput = document.createElement('input');
  loadInput.type = 'file';
  loadInput.accept = '.adv';
  loadInput.hidden = true;
  document.body.appendChild(loadInput);
  buttons.load.addEventListener('click', () => { loadInput.value = ''; loadInput.click(); });
  loadInput.addEventListener('change', async () => {
    const file = loadInput.files && loadInput.files[0];
    if (!file || !game || !game.waiting || game.waiting.kind !== KIND_MAIN) return;
    const bytes = new Uint8Array(await file.arrayBuffer());
    let name = file.name.toLowerCase().replace(/\.[^.]*$/, '').replace(/^colossal-cave-/, '')
      .replace(/[^a-z0-9_-]/g, '').slice(0, 40) || 'savedgame';
    game.mod.FS.writeFile('/' + name + '.adv', bytes);
    submit('restore ' + name);
  });

  function transcriptText() {
    if (game) return game.transcript.join('');
    return store.get('transcript') || '';
  }
  function downloadTranscript() {
    const d = new Date();
    const pad = (n) => String(n).padStart(2, '0');
    const stamp = d.getFullYear() + '-' + pad(d.getMonth() + 1) + '-' + pad(d.getDate());
    download('colossal-cave-transcript-' + stamp + '.txt', toBytes(transcriptText()), 'text/plain');
  }
  buttons.transcript.addEventListener('click', downloadTranscript);
  $('btn-transcript2').addEventListener('click', downloadTranscript);
  $('btn-again').addEventListener('click', () => startGame('new'));

  buttons.newGame.addEventListener('click', () => openDialog('dlg-new'));
  $('new-yes').addEventListener('click', () => { closeDialog('dlg-new'); startGame('new'); });
  $('new-transcript').addEventListener('click', downloadTranscript);
  $('new-cancel').addEventListener('click', () => closeDialog('dlg-new'));

  buttons.smaller.addEventListener('click', () => { prefs.fontSize = (prefs.fontSize || 18) - 2; savePrefs(); });
  buttons.bigger.addEventListener('click', () => { prefs.fontSize = (prefs.fontSize || 18) + 2; savePrefs(); });
  buttons.theme.addEventListener('click', () => { prefs.theme = prefs.theme === 'light' ? 'dark' : 'light'; savePrefs(); });
  buttons.help.addEventListener('click', () => openDialog('dlg-help'));
  buttons.about.addEventListener('click', () => openDialog('dlg-about'));

  // ---- dialogs ----
  function openDialog(id) {
    const d = $(id);
    if (!d.open) d.showModal();
  }
  function closeDialog(id) {
    const d = $(id);
    if (d.open) d.close();
  }
  document.querySelectorAll('dialog').forEach((d) => {
    d.addEventListener('close', () => setTimeout(focusInput, 0));
    d.querySelectorAll('[data-close]').forEach((b) => b.addEventListener('click', () => d.close()));
  });

  // The game asked to open a file the page doesn't have: a restore, or an
  // "input" script when the teacher allows it.
  let pickResolve = null;
  function pickFile(path, what) {
    const restoring = what === 'restore';
    $('pick-title').textContent = restoring ? 'Choose your saved game' : 'Choose a command file';
    $('pick-name').textContent = path;
    $('pick-hint').textContent = restoring
      ? 'Pick the .adv file you saved earlier (look in Downloads or OneDrive).'
      : 'Pick the text file of commands to run.';
    const input = $('pick-file');
    input.accept = restoring ? '.adv' : '.txt,text/plain';
    input.value = '';
    openDialog('dlg-pick');
    $('pick-choose').focus();
    return new Promise((resolve) => { pickResolve = resolve; });
  }
  function finishPick(result) {
    const r = pickResolve;
    pickResolve = null;
    closeDialog('dlg-pick');
    if (r) r(result);
  }
  $('pick-choose').addEventListener('click', () => $('pick-file').click());
  $('pick-file').addEventListener('change', async () => {
    const f = $('pick-file').files[0];
    finishPick(f ? new Uint8Array(await f.arrayBuffer()) : null);
  });
  $('pick-cancel').addEventListener('click', () => finishPick(null));
  $('dlg-pick').addEventListener('cancel', () => finishPick(null));

  // ---- start ----
  function init() {
    document.title = S.title;
    $('title').textContent = S.title;
    applyPrefs();
    if (typeof WebAssembly !== 'object' || typeof createAdventure !== 'function') {
      term.write('This browser cannot run the game. Please open this file in Microsoft Edge or Google Chrome.\n');
      cmdEl.disabled = true;
      return;
    }
    const saved = savedGame();
    if (!saved) { startGame('new'); return; }
    if (saved.savedAt) {
      const when = new Date(saved.savedAt);
      $('start-when').textContent = ', last played ' + when.toLocaleDateString(undefined, { weekday: 'long', month: 'short', day: 'numeric' })
        + ' at ' + when.toLocaleTimeString(undefined, { hour: 'numeric', minute: '2-digit' });
    }
    const dlg = $('dlg-start');
    let chosen = false;
    const choose = (mode) => { if (chosen) return; chosen = true; dlg.close(); startGame(mode); };
    $('start-continue').addEventListener('click', () => choose('continue'));
    $('start-new').addEventListener('click', () => choose('new'));
    dlg.addEventListener('cancel', (e) => e.preventDefault());
    dlg.showModal();
    $('start-continue').focus();
  }

  // Test hook (used by web/test/browser-test.mjs; harmless for players).
  window.__adventure = { state: () => game, settings: S, transcript: transcriptText };

  init();
})();
