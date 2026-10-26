# Reconstruction architecture

This document is the bridge between what the analysis proves about the
original and how the Rust workspace reimplements it. It is updated as
`docs/analysis/` grows.

## What the original looks like (observed)

From REA/Ghidra analysis of `RallyTrophy.exe` (Evidence:
`ev_f084873c…` binary overview, `ev_06684edb…` entry dossier, plus
`docs/analysis/control-flow.md`):

| Fact | Value |
|---|---|
| Container | PE32, x86, Windows GUI subsystem |
| Sections | `.text` 888 KiB game code; `.rdata`; `.data`; `.cms_t`/`.cms_d` hold the MSVC CRT **plus a copy-protection stub**; entry point is inside `.cms_t` |
| Procedures | 5448 (symbols stripped; MSVC 6-era C++) |
| Imports | user32, kernel32, gdi32, ole32, shell32, advapi32, winmm, **dinput8**, **mss32** (Miles Sound), **binkw32** (video), **wsock32**, avifil32 |
| Graphics | **not** imported directly; `device/bbdx8.dll` is `LoadLibrary`d (string at `0x4fbe48`) and it imports `d3d8.dll` |
| Registry | `Software\Bugbear\Rally Trophy` (`0x4fbef0`) |
| Netcode | GameSpy (`key.gamespy.com`, `0x4f8718`) |
| Protection | XOR-decode stub + indirect calls in `.cms_t` (Safedisc-like); `.cms_t` also hosts the CRT |

Control-flow spine (all addresses in `docs/analysis/control-flow.md`):
WinMain `FUN_0049aea0` → intro videos → main menu `FUN_0042eb10` /
menu loop `FUN_00430f10` → single-race `FUN_00431ca0` or championship
`FUN_00431320` → per-stage `FUN_0043ffa0` → race event `FUN_0048c7f0` →
**game loop `FUN_0048d470`** (10-case sub-state machine, per-frame renderer
vtable calls + sim step `FUN_004147a0(dt)` + frame limiter). Key singletons:
`g_game`=`0x66d63c`, `g_rendererDevice`=`0x66d620`, `g_display`=`0x66d640`,
`g_music`=`0x66d624`. The renderer's function-table interface is mapped
offsets +0x0c…+0x124 (see `rt-render` docs).

Important analysis nuance: **every API call goes through runtime-filled
pointer slots in `.cms_d`**, so Ghidra pseudocode shows `(*DAT_0069a2f8)(...)`
noise; slot↔import mapping is a next analysis step.

## Mapping original subsystems → crates

| Original subsystem (evidence) | Rust crate | Porting strategy |
|---|---|---|
| Vehicle physics, `Data/Global/dynamics.bzf` | `rt-physics` | Port function-by-function from Ghidra pseudocode; validate against replays |
| Track/terrain + surfaces | `rt-physics` + `rt-assets` | `.bzf` spec from `docs/analysis/data-formats.md` |
| D3D8 device wrapper `bbdx8.dll` | `rt-render` | Keep the discovered interface table as a trait; implement over wgpu |
| Miles Sound + `3dsounds.ini` | `rt-audio` | symphonia decode, cpal output, documented ini schema |
| Bink intro/menu video | out of scope initially | Re-encode to mp4/mkv for the reconstruction |
| DirectInput 8 + wheel FF | `rt-input` | gilrs/winit + TOML device maps |
| Game states, rally rules | `rt-game` | Port the main-loop dispatch once mapped |
| Menus/HUD/fonts (`fonts.bzf`, `overlay.bzf`) | `rt-ui` | Data-driven pages mirroring original layout |
| GameSpy multiplayer | `rt-game` (later) | Modern re-implementation (server list configurable) |
| Copy protection `.cms_t` stub | — | **Not ported.** It is DRM, not game behavior. |
| Miles/Bink/GameSpy SDK code | — | **Not ported.** Replaced with open equivalents. |

## Principles

1. **Evidence-first.** Every ported function carries a comment with its
   original address range. Every data struct cites byte offsets in
   `docs/analysis/data-formats.md`. Analysis claims cite REA Evidence IDs.
2. **Behavior over bytes.** The goal is a game that behaves like the
   original and loads its data — not a transliteration of x86.
3. **Moddable by design.** Anything the original read from `Data/` becomes a
   documented, overridable asset (see `rt-mod`).
4. **Clean room for code, dirty hands for data.** Rust code is original
   work; asset *decoders* are written against observed file bytes only.
5. **No DRM.** The protection stub is analysis noise, not a porting target.

## Milestones

1. **Foundation** (done): workspace, docs, analysis tooling.
2. **Asset pipeline**: `.bzf` parser (spec → `rt-assets`), texture/mesh
   extraction, font decode.
3. **Static viewer**: load a stage + car, orbit camera (`rt-render`).
4. **Driving**: vehicle physics port, input, HUD (`rt-physics`, `rt-input`,
   `rt-ui`).
5. **Rally loop**: stage timing, surfaces/weather, audio, replays.
6. **Championship + menus**, then multiplayer.

## Open questions (tracked in `docs/analysis/`)

- Exact fixed-timestep rate and physics ordering (sim step `FUN_004147a0(dt)`
  needs a full decompile; frame cap measured at ~92 ms in the limiter).
- bbdx8.dll **internals** (a REA pass over the DLL itself; the exe-side
  interface table is already mapped — see `rt-render`).
- ~~`.cms_d` slot↔import mapping~~ **Solved** (2026-10-22): the protection is
  SecuROM-redirected imports; pairing the retail exe with the official 1.01
  patch exe (`orig/RallyTrophy.exe`, from the ISO's own `Crack/Patch 1.01/`
  zip) at identical code offsets recovers the API behind 304 call sites —
  see `analysis/api_callsites.csv` and `tools/securom_api_map.py`.
- Names of the 10 race-loop sub-state cases (decompile `FUN_0048d470` fully).
