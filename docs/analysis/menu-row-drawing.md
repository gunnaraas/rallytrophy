# Menu row drawing helpers

## Binary observations

`Game::FUN_00435b60` (`0x435b60`) and `Game::FUN_00435ca0`
(`0x435ca0`) are parallel 313-byte helpers. Both receive the object in ECX
and six stack arguments, and return with `ret 0x18`.

The observed drawing sequence is:

- Compute X as `DAT_0056aea8 * 48.0`.
- Compute the initial Y as `(a * 42.0 + origin) * DAT_0056aeac`.
- For a positive `b`, draw `b` rows and advance Y by
  `DAT_0056aeac * 42.0` after each row. A nonzero `c` selects callback
  `0x4355a0`; zero selects the helper-specific alternate callback.
- If `d` is nonzero, draw the footer at X and
  `DAT_0056aeac * height`. The observed x87 C3 test selects height 604.0
  when `f` equals -1.0 or the comparison is unordered; otherwise it uses `f`.
  An unordered result includes NaN, subject to the floating-point trap mode.
  A nonzero `e` selects
  callback `0x435710`; zero selects `0x4355a0`.

| Helper | Origin | Alternate row callback |
|---|---:|---|
| `0x435b60` | 214.0 | `0x435880` |
| `0x435ca0` | 218.0 | `0x4359f0` |

Evidence: `ev_52cfb517b00277ed4ad562293fd1eaa405d0a82a8d5a0d9b376aad3cd944d373`
for `0x435b60`, and
`ev_15926055542cc424ed13afe7291bc24c71d1f742cc8f5fb6a36521e0ff4f373c`
for `0x435ca0`. Both dossiers identify the official executable SHA-256
`2d5b2632a6467edb31eb43e8decf237d47e11d62e9b6f37bf8d528e4ad0d83b7`.
Ghidra pseudocode supplies a behavior reference; the ECX/stack convention
and argument order above follow the decoded assembly and the source signature.
Drawing roles such as "row" and "footer" are analyst interpretations.

The callback at `0x4355a0` builds UV points before position points, using the
four-`Vec2` array constructor/destructor iterators. Its position extent is
`DAT_0056aea8 * 512.0` by `DAT_0056aeac * 32.0`; its UV corners span 0 to 1,
and it selects the resource at `Game+0x1390dc` before drawing a white quad at
unit scale. These observations come from REA Evidence
`ev_a1c65cf3ecc285a7c4440bdcdc86d66c025a10c292ff652fe6e1eaff54d573c9` and
the target instruction bytes. The exact source transcription now matches all
368 bytes and 16 relocations, including the white color temporary at
`[esp+0x10]` and the height spill/reload at `[esp+8]`.

Plain observation from the target PE: callback `0x4355a0` has an SEH frame with
handler `0x4d7f5e`; the three sibling callbacks have corresponding handlers
`0x4d7f9e`, `0x4d7fde`, and `0x4d801e`. The shared empty constructor at
`0x490ad0` is named `FUN_00490ad0` in the function inventory and now has that
source symbol, returning the incoming `this` pointer as in the three-byte body.

Plain observation from the target PE instructions: the exact point helpers
`0x435de0` and `0x435f50` pass constructor `0x490ad0` and destructor
`0x402c90` to the EH vector initializer. Their source arrays now use `Vec2`,
whose constructor and destructor resolve to those defined functions; the
previous local `GameQuadPoint` declarations had no definitions. Both helpers
remain byte-exact at 368 bytes with 22 relocations.

The three sibling callbacks now match independently at 368 bytes and 16
relocations each. REA instruction evidence (import-resolved 1.01 reference, SHA-256
`cc1718e21a83b7422cfd2b304a9fe43e4f6a6e7550389d9f20bf30530f19ec8d`):

| Callback | Extent | Resource at Game+ | SEH handler | Evidence |
|---|---|---|---|---|
| `0x435710` | 256 × 32 | `0x1390e0` | `0x4d7f9e` | `ev_5ee50593a4f3a6d823dc194396a39d64dcd330794caa2ce4639d487b89ee63fc` |
| `0x435880` | 512 × 32 | `0x1390e4` | `0x4d7fde` | `ev_57bcd7701ba00558f26c5d030e540137f1af419f3b9fec7c84dcd74eb81faa76` |
| `0x4359f0` | 1024 × 16 | `0x1390e8` | `0x4d801e` | `ev_d5e2d4ee8e251bd8897e103208a52fb53a81b50cead0247370d35bac7f7297ec` |

Plain observation from the official PE: the width/height constants in that
table decode from `0x4e14e0`/`0x4e14c0`, `0x4e14e8`/`0x4e14c0`, and
`0x4e1350`/`0x4e14bc`, respectively. All 94 instructions per sibling were
compared against `0x4355a0`; only the handler, constants and resource offset
differ. Relocation operands were reviewed separately from the masked matcher.

`0x435430` now matches all 364 bytes and 14 relocations. It constructs both
point arrays through the same lifetime helpers, reads dimensions from
`Game+0x139478`/`+0x13947c`, and selects `Game+0x1390d8`. Its atlas UVs are
`(.01,.01), (.99,.01), (.99,.99), (.01,.99)`; the former source had the wrong
lower-right Y coordinate. Evidence:
`ev_8ab4bdc7104b83f38e3e1d2eaeccd5ea26663e045ca107b7a3c54bc6691ecddf`.

`0x4360c0` now matches all 395 bytes and 20 relocations. It derives atlas
column `flag & 3` and signed row `flag >> 2`, using scales 0.25 and 0.0625.
It selects `Game+0x1390f8` and returns with `ret 0x14`. The transcription
preserves the target's 0x50-byte local allocation, integer-to-float spills,
x87 order and array cleanup. Evidence:
`ev_00ca0dff9d71556e7fc3876515ed6a31cd43c3a0308b01c025ee5a8a7204626b`.
The import-resolved reference observations above were checked against official PE
instructions; the matcher uses the official executable.

Each new callback declares the empty destructor as `void FUN_00402c90()`
locally. This selects the existing no-argument definition rather than the
unrelated varargs declaration in the shared header. The shared constructor
is defined as `FUN_00490ad0`; CRT vector helpers still need linked integration.
Retained REA reports are under `tool-results/menu-quads-2026-10-26/`.

## Overlay and strip callbacks

`0x436ed0` now matches 366 bytes and 20 relocations. It draws the clockwise
rectangle `(256*sx, 320*sy)`–`(768*sx, 448*sy)` with full 0–1 UVs and white
color, where `sx`/`sy` are `DAT_0056aea8`/`DAT_0056aeac`. It dereferences the
resource pointer at `Game+0x1390f4`, binds it after computing the corners,
and returns without stack arguments. SEH handler: `0x4d817e`. Evidence:
`ev_08af09e751500907395e46423823b89995d90bb2394b49bfdd2ff2efe6c880dc`.

`0x437cc0` now matches 351 bytes and 14 relocations. Its rectangle begins at
`(x,y)`, has width `size` and height `32*sy`, and uses clockwise UV corners
`(.2,0), (.3,0), (.3,1), (.2,1)`. It dereferences `Game+0x1390dc`, draws in
white, and returns with `ret 0xc`. SEH handler: `0x4d824e`. Evidence:
`ev_aca88f14baf6ae27d33f01841b05eba2808c7351027894b480be047222d00600`.

Both evidence records use the import-resolved reference identified above; plain
observation from official PE disassembly confirms these instructions and
literal values. The earlier source passed resource slot addresses, used
zigzag corners, and assigned incorrect dimensions. In `0x437cc0`, the old
color literal `0x3e4ccccd` was actually the target's U=.2 UV value. The
transcriptions preserve array lifetimes and x87 order. Independent object
review checked all 34 relocation targets, resource dereferences and ABIs.
Retained reports share `tool-results/menu-quads-2026-10-26/`.

## Menu draw/update `0x437040`

The 467-byte function draws the video background, title at `(54*sx,142*sy)`,
seven selectable labels, row decoration and cursor. It skips Present only
when the stack argument equals 1. ECX carries Game; the body returns with
`ret 4` and uses SEH handler `0x4d81a8`. Evidence:
`ev_50354542e16d87a1067e5410135194efbb6ba96e62c7ef1d4e26592b3475ff3f`
(import-resolved reference identified above), checked against official PE instructions.

Plain source/object comparison: adding a unit-scale point temporary reproduced
the target's 0x14-byte local allocation and 467-byte size, reducing differences
from 178 to 43 bytes. Moving it inside the loop reduced the difference to 26;
separate X/Y assignments reduced it to 25. Constructor initialization, float
identity helpers, and alternate scale setters did not improve the best result.
The target cleans each iteration back to EH state 0 and restores font scales
through x87 loads/stores. A missing point lifetime is an inference from these
stack slots and states, not a recovered source declaration.

The instruction transcription now matches all 467 bytes with 29 relocations.
Six explicit linker aliases bind member calls to their defined MenuGame/Game
symbols. Text calls bind `FUN_004a97a0`, cursor-position calls bind the defined
`GetMousePosition`, and cursor drawing binds the defined two-float Game method.
These replace earlier header declarations with different C++ symbols but
compatible machine layouts. Independent review verified all 29 relocation
targets, five branch destinations, ABI and alias directives. Retained evidence:
`tool-results/menu-quads-2026-10-26/437040.json`.

## GameView teardown `0x4327d0`

The 522-byte function disposes resource wrappers in target order at
`Game+0x1390d4`, `+0x1390f8`, `+0x1390f4`, and `+0x139488`; deletes the two
rectangles at `+0x139470` and `+0x139474`; then disposes wrappers at
`+0x139104`, `+0x1390fc`, and `+0x139100`. It clears only `+0x1390d4`.
Each wrapper cleanup releases its inner object with flag 1, calls its base
cleanup at `0x4a86d0`, then deletes the wrapper. Evidence:
`ev_3e893bab32639325a528c10cd1e020da2023509e04c027ab3bb0bb393bb65aa2`.

Next it stops the four sound sources at offsets `+0x139224`, `+0x139294`,
`+0x139304`, and `+0x139374`; releases each stored sample; calls virtual
slot `+8` on screen objects at `+0x3aec` and `+0x59e4`; then calls
`ServiceMenu::UnloadTextures` on `Game+4`. These are Ghidra instruction and
plain PE observations. The source transcription now matches all 522 bytes and
35 relocations. Linker aliases bind calls to the current definitions for the
base cleanup, delete operator, sound Stop and ReleaseSample, virtual
`RaceMode::Lock`, `ServiceMenu::UnloadTextures`, and the caller-facing Game
method. Independent review verified all 35 relocation targets and the aliases
against current object definitions. The base cleanup alias resolves to
`ModelResourceState::DestroyStringExact`, whose source body carries the exact
`0x4a86d0` marker.

## Reconstruction

The source retains the observed x87 operations, stack argument reuse,
callback routing, and early return paths in naked instruction transcriptions.
The callback aliases refer to the reconstructed method symbols. Independent
local disassembly comparison finds only the two changes in the table after
normalizing internal branch destinations between the two helpers.

Focused verification for `src/unk_4303e0.cpp`: 41/41 marked functions exact,
10 NONMATCHING, no differences or compile failures. The `0x435ca0`
body matches all 313 bytes with 13 object relocations accounted for. This is
object-level byte comparison; linked image integration remains separate work.

Full matcher after this change: 1,765/1,765 marked functions exact,
319 NONMATCHING, no differences or compile failures.

## MenuGame audio update `0x432c00`

The 516-byte `MenuGame::Update` body is a naked instruction transcription and
matches all 22 relocations. The binary uses a 500 ms transition state, takes
the stream frame from the per-track frame table, indexes stream names with a
four-entry stride per track, and advances that frame only when the sound
manager is present. It computes the selected stream volume and applies the
configured stage volume to the four sound sources at `+0x139224`, `+0x139294`,
`+0x139304`, and `+0x139374`. These instruction-level observations are from
Ghidra evidence `ev_125a5508d19db5bf429f45b7d07aa3fa7438a3c43c406e2f1a05861f9523c640`.
The `SoundSource::SetVolume` calls use a linker alias to the existing method
definition. Focused matcher: 42/42 marked functions match, with 9 other
functions still NONMATCHING. Full matcher after this change: 1,766/1,766
marked functions exact, 318 NONMATCHING, no differences or compile failures.
