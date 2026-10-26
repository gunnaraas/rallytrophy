<!-- Subagent analysis (Codex, reviewed by Claude), 2026-10-23; rawdis.py / refs.py, Rally Trophy Patch 1.01. Guesses marked. -->
# Range 0x43c590 - 0x43d76f: module map

Method: disassembly and whole-image references. This range is **one source module**, the menu-manager class, bracketed by its `_$E` stubs at 0x43c560 and 0x43d740. The next TU begins at 0x43d770. The manager constructor directly constructs all embedded screens, and the trailing function is its layout/rect builder. Both guard stubs call the same STL atexit helper (0x402c90), consistent with one module including the menu STL headers.

## 1. Module and class

### Menu manager (tentative name `MenuManager`), 0x43c590 - 0x43d73f

Object size **0xcf8**, embedded in Game at **Game+0x2df4** (manager ctor call at Game ctor 0x430495; manager dtor at 0x4308ff; dispatch thunk at 0x430b59). Vtable **0x4e17c8** has four entries:

| Slot | Address | Role |
|---|---|---|
| 0 | 0x43c660 | scalar deleting destructor |
| 1 | 0x43c770 | Setup/initialise the manager and its root menu labels/layout (absent from functions.csv) |
| 2 | 0x43cae0 | Close/release active screen and clear manager state |
| 3 | 0x4414c0 | shared empty `Slot3(int)` thunk (outside this module, shared with menu screens) |

The words after the four function entries in .rdata are float constants, not further virtual slots. The ctor sets base `MenuScreen` at +0, constructs six `MenuLabel`s at +0x44 (stride 0x24), then embedded screens: `RaceOptionsMenu` +0x11c (size 0x18c, vtable 0x4e183c), screen +0x2a8 (size 0x4a8), `SoundMenu` +0x750 (size 0x18c), screens +0x8dc and +0xa48, and stores manager vtable last. Last field is network/session pointer at +0xcf4 (read/written by dispatch). `Game+0x2df4 + 0x11c = Game+0x2f10` is the RaceOptionsMenu address.

Base `MenuScreen` fields +0..+0x1f are described in [unk_450ee0.h](../include/unk_450ee0.h); manager uses state +4, selected +8, Game* +0xc, and `vector<UIRect*>` +0x10. Manager fields observed: byte +0x20; three zeroed dwords +0x24/+0x28/+0x2c (likely std::string internals; setup at 0x43c770 assigns text); labels +0x44..+0x11b; embedded objects listed above; +0xcf4 NetRace*. Constructor argument is retained in base-related setup (inspect parameter at stack +4); constructor writes vptr 0x4e17c8 at end. Manager setup reads screen dimensions from 0x56b0ac, scales by 1/1024 and 1/768 constants (0x4e1358/0x4e1354), uses text renderer at Game+0x1390c4, and creates six menu rectangles.

The child classes use existing `MenuScreen`, `MenuGame`/GameView, `MenuLabel`, `UIRect`, and Sprite/TextRenderer types. Their object layouts and vtables are documented in [range-43d770.md](range-43d770.md), [unk_43f9e0.h](../include/unk_43f9e0.h), and [range-450ee0.md](range-450ee0.md). The relevant current Game members are its text renderer at +0x1390c4 and menu sounds at +0x139224/+0x139294/+0x139304. Screen dispatch calls the embedded screens’ `Run(game, net)` methods. The large manager frame uses `Sprite`/font rendering (0x4a97a0), video methods on Game, `UIRect::Contains` 0x4390d0, and base cursor drawing 0x438f40.

Strings/constants: layout path is made from 0x4f8efc (module-specific base/path fragment) and screen dimensions; 0x43d1b0 creates six `UIRect`s using key strings 0x4f8ed4/ee8, 0x4f8ea4/ebc, 0x4f8e68/e88, 0x4f8e38/e50, 0x4f8e00/e1c, and shared Back keys 0x4f62c8/62dc. Config filename/key prefix and full strings should be read from .rdata when implementing. Common constants include 1/1024, 1/768, 0.5, and screen-scale data at 0x56b0ac. UI colours at 0x508bfc, 0x508c04, 0x508c0c.

### Missing analysis entries / tables

- **0x43c770**, size 0x370 (ends at 0x43cae0), is a real function referenced by vtable slot 1 but missing from functions.csv. It is the manager's setup path: guard on state +4; initialise root labels and geometry/layout, including inline `std::string` assignment and FPU screen scaling.
- **0x43cb50**, 19 bytes, is in functions.csv; this is a small this-adjusting forwarding thunk to 0x42a210 with `this += 0x2a8`, used by Game and another menu caller.
- **0x43c560-0x43c58f** and **0x43d740-0x43d76f** are 48-byte compiler `_$E` guard/atexit stubs, omitted as functions. Guards/table references are at 0xefb4c and 0xefb50.
- **Jump table** at 0x43cc2c: six dispatch destinations (default/error, RaceOptionsMenu, screen +0x2a8, SoundMenu, screen +0x8dc, screen +0xa48). This is data within the code section and is not a function.
- No local static constructors/destructors or vector/string template bodies were found. `0x4414c0` is a shared empty virtual function outside the range; no additional local vtable slots. The adjacent next-file data entries 0x43db20, 0x43dc70, 0x43de10 belong to PlayerNameMenu and are not this module.

## 2. Function table

Frame/exception notes: `SEH` means an FS:[0] registration frame with the listed handler immediate; `FPU` indicates x87 arithmetic. Calls listed are principal direct callers from whole-image references.

| Address | Size | Convention / frame | Role | Main callers |
|---|---:|---|---|---|
| 0x43c590 | 201 | thiscall ctor, SEH handler 0x4d8801, stack locals 8 | `MenuManager` ctor: base, six MenuLabels, five embedded screens; sets vtable 0x4e17c8 | Game ctor 0x430495 |
| 0x43c660 | 30 | thiscall, `ret 4`, no SEH | compiler scalar deleting dtor; calls complete dtor then conditionally delete | vtable slot 0 |
| 0x43c680 | 238 | thiscall dtor, SEH handler 0x4d88d8 | complete destructor; calls child screen dtors in reverse, destroys labels/string, base dtor | deleting dtor 0x43c663; Game dtor 0x4308ff |
| **0x43c770** | **880** | thiscall, FPU, no SEH, stack locals 0xc | manager `Setup` / root menu initialisation, string and label assignments plus geometry/layout | vtable slot 1 (vtable ref in .rdata) |
| 0x43cae0 | 108 | thiscall, no SEH | Close/release: if active, virtual slot 2 on five embedded screens; clears state/game and item rects | Game dtor 0x4308ff, manager Run 0x43c6ab |
| 0x43cb50 | 19 | thiscall forwarding thunk, `ret 4` | adjust `this` by +0x2a8 and forward one-arg call to 0x42a210 | 0x430b59, 0x4345ae |
| 0x43cb70 | 186 | thiscall, `ret 8`, no SEH | `Run(game, net)`: save net at +0xcf4, virtual Setup, repeatedly call Frame and dispatch selected index via jump table | 0x434dd9, 0x43a9af, 0x43ac37 |
| 0x43cc50 | 1362 | thiscall, SEH handler 0x4d88d8, x87, locals 0x64 | manager `Frame`: message/update loop, input and hit tests, draw labels/cursor/video, sound and modal exit | 0x43cb89 |
| 0x43d1b0 | 1415 | thiscall-shaped member, SEH handler 0x4d8972, stdcall metadata, locals 0x3c | `BuildRects`/layout reader: Config load, six UIRect allocations and pointer-vector inserts; same shape as PlayerNameMenu::BuildRects at 0x43ee70 | 0x43cacf |

All eight CSV entries plus omitted 0x43c770 belong to this single module. 0x43c590 and 0x43c680 have SEH cleanup states corresponding to child construction/destruction. `0x43cc50` has an inline six-way switch table at 0x43cc2c; no other switch table is evident here.

## 3. Suggested implementation work groups

This is one source file, so groups are analysis/decompilation partitions rather than separate modules:

| Group | Functions | Bytes | Notes |
|---|---|---:|---|
| M1 lifecycle and setup | 0x43c590, 0x43c660, 0x43c680, 0x43c770 | 1349 | Include the omitted vtable function; constructor/destructor EH states matter for exact VC6 output. |
| M2 dispatch and close | 0x43cae0, 0x43cb50, 0x43cb70 | 313 | Small helpers; include six-case jump table at 0x43cc2c. |
| M3 frame loop | 0x43cc50 | 1362 | Big function alone; preserve x87 order, event-loop branches and renderer virtual-call order. |
| M4 layout rect builder | 0x43d1b0 | 1415 | Big function alone; compare directly with PlayerNameMenu::BuildRects 0x43ee70. |

Compiler-generated: 0x43c660 scalar deleting dtor; 0x43cb50 forwarding thunk; both boundary `_$E` stubs. Inline string and vector operations occur in setup/layout functions, but no out-of-line template instances are emitted in this range. The `UIRect` and MenuLabel constructors/destructors are external helpers.
