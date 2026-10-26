# AGENTS.md — decomp working rules

Project: byte-exact C++ reconstruction of Rally Trophy 1.01 (2001) from its
shipped binary, built with the original Visual C++ 6.0 SP5 toolchain. Read
`README.md`, `CONTRIBUTING.md`, `analysis/NOTES.md` (compiler flags, codegen
conventions) and `docs/architecture/overview.md` first. The active queue is
`analysis/PROGRESS.md`. When several local agents share one checkout, claim your
range in the local, untracked `analysis/COORDINATION.md` before editing.

The Rust behavior port lives separately in `../Rally-Trophy-Rust`; do not
edit it from here. Source and tooling are GPL-3.0 (`LICENSE.md`).

## Hard rules

- Never commit game files: ISO, executables, DLLs, assets, `extracted/`,
  `orig/`, or `toolchain/`. They are gitignored; keep them that way.
- Never commit or document copy-protection bypasses, patched executables, or
  key-check algorithms. Keep that kind of note local and untracked.
- Do not commit raw REA/Ghidra exports (`analysis/evidence-*.json`,
  `tool-results/`). They are gitignored and may contain local paths.
- Do not commit personal tool settings, local paths, or credentials.

## REA / Ghidra setup

- REA/Ghidra is optional for compiling and matching, and is used for new
  binary analysis. Configure it locally; do not commit personal tool settings.
- Set `GHIDRA_INSTALL_DIR` to a local Ghidra installation and make a JDK 21
  runtime available to the REA server.
- An REA session binds one binary at a time. Serialize REA-heavy work; never
  open another binary while analysis is in flight.
- ISO images are not openable by REA; extract them with
  `7z x "Rally Trophy.iso" -oextracted`.

## Analysis workflow

1. Subsystem findings go in `docs/analysis/*.md`; per-range maps and notes go
   in `analysis/`. Cite REA Evidence IDs for every
   non-trivial binary claim, or mark it as a plain observation or inference.
2. Large REA results belong under the session's `tool-results/`; parse them
   with `tools/rea_digest.py` or a short Python extraction. Never read or grep
   local `analysis/evidence-*.json` dumps directly.
3. Ghidra pseudocode is behavior reference, not original source. Do not copy
   it verbatim into C++.
4. Exclude the copy-protection stub (`.cms_t`). Miles, Bink, and GameSpy SDK
   behavior is third-party code and stays outside the first-party match queue.
5. Original function addresses are stable identifiers. Record them in source
   comments and the analysis ledger.

## Matching loop

- Make one small, evidence-backed source change at a time.
- Mark each function with `/* FUNCTION: RALLYTROPHY 0x<addr> */`. A function
  that is logically right but not byte-exact after a time-box gets
  `// NONMATCHING: RALLYTROPHY 0x<addr>` with a note on what differs and what
  was tried. Do not mark a function as matching unless `match.py` confirms it.
- Run `python3 -I tools/match.py --quiet <your source files>` while
  iterating; run the full `--quiet` matcher before committing.
- After a full pass, refresh generated progress with
  `python3 -I tools/progress.py`.
- Workspace edits by other workers are shared. Stage only your own verified
  files and preserve unrelated changes.
- Commit messages are short and imperative ("Match <function> bytecode").
