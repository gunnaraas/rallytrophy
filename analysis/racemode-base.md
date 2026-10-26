<!-- Subagent analysis, 2026-10-22; read-only disassembly; guesses are marked. -->
# Race-mode base class module (0x4856e0 - 0x48f12b)

Source: raw disassembly of Patch 1.01 RallyTrophy.exe. All addresses are VAs. "Guess" marks inference. No REA used.

## 1. Base vtable = 0x4e2520
The base ctor stores it at 0x485c56 (`mov [esi],0x4e2520`). The base dtor stores it again at 0x485dc1.
Five vtables reference the slot 18-24 functions: 0x4e23ac (sibling), 0x4e2440 (NetRace), 0x4e2520 (base), 0x4e2690 and 0x4e2708. The last two are further derived classes in the next module:
- 0x4e2690: ctor 0x48f130 (calls base ctor, sets [0x7a8], [0x844], [0x848], writes global 0x501450 = this), dtor 0x48f180/0x48f160.
- 0x4e2708: ctor near 0x48f6c0 (stores at 0x48f6ca).

| Slot | Target | Size | Note |
|---|---|---|---|
| 0 | 0x485d80 | 30 | scalar deleting dtor: calls 0x485da0, then `operator delete` 0x4bd000 if bit 0 of arg |
| 1 | 0x4c19e3 | - | CRT `_purecall`, pure virtual. NetRace = Update 0x481940 |
| 2 | 0x48e660 | 1 | empty `ret`, shared across many vtables |
| 3 | 0x48e660 | 1 | empty |
| 4 | 0x4c19e3 | - | pure virtual. NetRace 0x481700 |
| 5 | 0x4c19e3 | - | pure virtual. NetRace 0x481860 |
| 6 | 0x48e660 | 1 | empty |
| 7 | 0x48e670 | 3 | `mov al,1; ret` (returns true) |
| 8 | 0x48e660 | 1 | empty |
| 9 | 0x48e660 | 1 | empty |
| 10 | 0x48e680 | 66 | float arg: for each player with car [p+0x28] and state [p+0x40]!=3, call car method 0x46bc90(arg) |
| 11 | 0x48e6d0 | 66 | same shape, calls 0x46be50(arg) |
| 12 | 0x4890b0 | 3 | `ret 4` (1 stack arg, no-op). NetRace = SendGrid 0x4825d0 |
| 13 | 0x48e630 | 28 | `bool f(Player* p)`: if p->car ([p+0x28]) calls car 0x4688f0(3), returns 1, else 0 |
| 14 | 0x48e650 | 11 | `int f(Player* p)` returns [p+0x1824]. Called by base methods as `call [vtbl+0x38]` ("is this player relevant/visible") |
| 15 | 0x48e660 | 1 | empty |
| 16 | 0x48e660 | 1 | empty |
| 17 | 0x48e660 | 1 | empty |
| 18 | 0x488aa0 | 461 | SEH frame. Per-player car setup (0x4693a0, 0x4695c0, 0x468c70) and a ghost distance fade (uses [0x794], [0x798]). Args: 2 (ret 8) |
| 19 | 0x488e50 | 154 | car draw pass: loops players, calls car 0x469630(4 args); also draws ghost [0x794]'s car. ret 0xC |
| 20 | 0x488ef0 | 115 | player loop plus ghost, car 0x4697c0 |
| 21 | 0x488f70 | 89 | player loop, car 0x469a00 |
| 22 | 0x488c70 | 467 | like slot 18 but skips one given car (3 args, ret 0xC) |
| 23 | 0x489030 | 121 | like slot 20 (car 0x4697c0, ghost) |
| 24 | 0x488fd0 | 95 | like slot 21 (car 0x469a00) |
| 25 | 0x4890c0 | 6 | `mov eax,1; ret` (true) |
| 26 | 0x4890c0 | 6 | same |
| 27 | 0x4890d0 | 8 | `mov [ecx+0x44],1; ret` |
| 28 | 0x4890b0 | 3 | `ret 4` |
| 29 | 0x4890b0 | 3 | `ret 4` |

Slots 18-24 are per-pass car rendering hooks (guess at the meaning).
Slots 30+ are not slots. The vtable data ends at slot 29; the next 4 dwords are float constants (300.0, 342.0, 180.0, 400.0).
Slot 1/4/5 are pure, so the base is abstract. Slots 2,3,6,8,9,15,16,17 are empty virtuals that derived classes override (NetRace overrides 2,3,7,12,13,14,15,17,25-29 among others).

## 2. Module range and function list

**Range: 0x4856e0 - 0x48f12b** (guess for the start, firm for the end).
- Start: 0x4856e0 is the first file-scope static initialiser pair of this TU (wrapper `call 0x4856f0; jmp 0x485710`). The previous module (Networking) ends with its guard/atexit pair at 0x4856b0-0x4856db. modules.csv currently ends Networking at 0x485700, so the new start sits slightly earlier than that boundary (0x4856e0-0x4856ef belongs to this TU).
- End: the last initialiser is 0x48f100 (guard byte 0x5826d5) with `push 0x402c90; call 0x4c0d01` at 0x48f120-0x48f12b. The next module starts at 0x48f130 (ctor of the vtable 0x4e2690 class).
- Evidence the statics belong to this TU: the Vec2/ARGB globals are used by functions at 0x487xxx-0x48dxxx. For example 0x565488 at 0x4878d1, 0x565458 at 0x488569, and 0x56543c/0x565444 at 0x489706 and 0x48c4fb.

### 2a. File-scope statics (start of module)
11 init/atexit pairs, `_$E` wrapper then init then trivial-dtor registration:

| Pair | Writes |
|---|---|
| 0x4856e0 (init 0x4856f0, dtor stub 0x485720) | 0x565488=720.0f, 0x56548c=10.0f (guess: Vec2) |
| 0x485730 | 0x565480=890.0f, 0x565484=10.0f |
| 0x485780 | 0x565478=512.0f, 0x56547c=0 |
| 0x4857d0 | 0x565470=512.0f, 0x565474=64.0f |
| 0x485820 | 0x565468=0, 0x56546c=0 |
| 0x485870 | 0x565460=512.0f, 0x565464=256.0f |
| 0x4858c0 | 0x565458=512.0f, 0x56545c=179.0f |
| 0x485910 | 0x565450=0, 0x565454=0 |
| 0x485960 | 0x565448=1024.0f, 0x56544c=768.0f |
| 0x4859b0 | 0x565444=0xffeaf1f7 (ARGB colour) |
| 0x485a00 | 0x56543c=0xff000000 (ARGB colour) |

- Guard byte 0x565440 bits 0/1 are set by the two dtor-wrapper bodies at 0x4859e0 and 0x485a30 (colour objects with empty dtors).
- A `0x401xxx`-style `ARGB`/`Vec2` class with trivial dtor is implied.
- 0x56541c is a format string/global used by 0x487b30.

### 2b. Functions

"Slot" means a vtable slot. "ext callers" are outside 0x4856e0-0x48f12b. All other callers are in-module.

**Comparators, qsort callbacks, `__cdecl(Player**, Player**)`. Referenced by `push imm`, no callers.**

| Addr | Size | Used by | Note |
|---|---|---|---|
| 0x485a50 | 124 | push in 0x48a070 (0x48a135) | compare by Player method 0x491f70, then name (inline strcmp of [p+0]) |
| 0x485ad0 | 127 | push in 0x48a920 (0x48a97b) | compare sum of Player arrays [p+0x1a28..] (count [p+0x1a50]), then name |
| 0x485b50 | 93 | push in 0x48b080 (0x48b0ed) | compare [p+0x1a54], then name |

**Ctor/dtor and construction helpers**

| Addr | Size | Kind / callers | Note |
|---|---|---|---|
| 0x485bb0 | 456 | ctor. ext callers: 0x4807d3 (sibling), 0x4813b2 (NetRace), 0x48f133, 0x48f6c3 | SEH. Calls 0x4a8580 (x2), 0x402700, 0x486240, 0x48ee60 |
| 0x485d80 | 30 | slot 0 | deleting dtor |
| 0x485da0 | 439 | dtor body. Callers: 0x485d83; ext: 0x480826, 0x4816e5, 0x48f19a, 0x48f74a (+ 3 data refs 0x4dcec3, 0x4dcf23, 0x4dd723) | see section 3 |
| 0x485f60 | 459 | direct, 13 ext callers (0x403f8b, 0x4335b3, 0x43361d, 0x433e48, 0x433ed2, 0x434d37, 0x44d9a9, 0x44dad3, 0x44dc5d, 0x44de28, 0x44e131, 0x44e192, 0x44e36a) | `AddPlayer(Player* p)` (guess): vector A insert via 0x4bc680, then if [p+0x38] nonzero grows/pushes into vector B. Contains an inlined vector grow |
| 0x486180 | 120 | ext 0x4925d7, 0x49322a | Creates Stage: `new(0x2534)`, ctor 0x40c9b0(this[0x28], this[0x2c], 2 args), stores [0x30], then 0x486130 |
| 0x486130 | 76 | in 0x4861e1 | for each player (state [p+0x40]!=3): p->0x491130(), car 0x464530(arg,0) |
| 0x486200 | 53 | ext 0x494bc5 | for each player with car: car 0x4654f0(0) |
| 0x486620 | 37 | ext 0x492a5e, 0x49359f | delete Stage [0x30] (dtor 0x40ebc0, free), null it |
| 0x486650 | 82 | ext 0x49274b, 0x493407 | stage 0x411320(arg), then each player's car 0x468c40(arg) |
| 0x486240 | 848 | in 0x485cb3 (ctor) | loads overlay images (see section 3). SEH, `new` via 0x4c1276 |
| 0x486590 | 140 | in 0x485e60 (dtor) | frees [0x58], [0x5c], [0x60] and the [0x328] image (dtor 0x4a86d0, 0x4a8bb0) |
| 0x48ee60 | 667 | in 0x485cc0 (ctor) | builds 7 UI element objects (0x14 bytes each, ctor 0x4390a0) and push_backs them into the [0x834] vector via 0x4bc680. Float coords e.g. (1047,..), (547,..) etc. |

**Replay, ghost and highscores (data-file I/O)**

| Addr | Size | Callers | Note |
|---|---|---|---|
| 0x4866b0 | 1323 | ext 0x492bec, 0x492d7a, 0x494b48 | session highscores (`data/highscores/sessionscores.hs`), formatted times, uses 0x486be0 |
| 0x486d20 | 602 | ext 0x492d81 | same file, other direction (load/save counterpart) |
| 0x48de10 | 992 | ext 0x48f908 | builds `data/replay/replay%d.ini` / `.rep` path, loads replay, creates ghost Player into [0x794] (guess) |
| 0x48e1f0 | 708 | in 0x486ba2 | replay path/file handling (save direction, guess) |
| 0x48dde0 | 43 | in 0x48de34. ext 0x48f73b, 0x48f8ea | delete ghost: Player dtor 0x490ee0, free, [0x794]=0 |

**HUD/text draw helpers** (non-virtual, all called from derived Draw functions; `this` in ecx)

| Addr | Size | Callers | Note |
|---|---|---|---|
| 0x486f80 | 242 | ext 0x49417e | uses [0x3c] time, font 0x4aa430 |
| 0x487080 | 340 | ext 0x494185 | draws using [0x24], [0x3c]. Ends with a switch jump table at 0x4871d4 (embedded, not code) |
| 0x487200 | 426 | ext 0x481123 (sibling), 0x481fa3 (NetRace), 0x48faa5 | draws via font vcalls [+0x8c]/[+0x108]; uses Vec2 statics |
| 0x4873b0 | 426 | in 0x48758b, 0x4886b9. ext 0x481138 | variant of 0x487200 (similar body) |
| 0x487560 | 572 | ext 0x481ffb, 0x48fb03 | `"%i."` position text and [0x7b4]/[0x7b8]/[0x82c] |
| 0x4877a0 | 370 | ext 0x48112a, 0x481faa, 0x48faac | `"%i%i:%i%i:%i%i"` time display, [0x3c], [0x30] |
| 0x487920 | 525 | ext 0x481fc1, 0x48fab3 | `"%i.%i"` (uses 0x487b30 sprintf-like helper) |
| 0x487b30 | 24 | in 0x4879d0 | tiny wrapper: `0x4c2431(0x56541c, arg, &local)` (CRT call) |
| 0x487b50 | 674 | ext 0x481131, 0x481fba | text draw using std::string (0x402700/0x402960/0x402980) |
| 0x487e00 | 749 | ext 0x48113f | text draw with time via 0x4821d0 (FormatTime in NetRace module) |
| 0x4880f0 | 812 | ext 0x481146 | `"%i/6"` lap/position |
| 0x488420 | 178 | ext 0x48114d, 0x481fc8 | split-time tracker: uses [0x7a0],[0x7a4],[0x7a8], Player [0x1a18]/[0x1888]; calls 0x4884e0. Exact body is transcribed from the PE because the source-form call differed in the final argument register (EDX vs ECX). |
| 0x4884e0 | 177 | in 0x4884c7 | draws split delta text with sign colour, via 0x4885a0 |
| 0x4885a0 | 236 | in 0x488503 | cdecl(int ms, char* out): "+/-" plus digits. Magic-constant division (0x45e7b273) |
| 0x486be0 | 320 | in 0x486a47, 0x486a97, 0x48a612, 0x48ad8f, 0x48bab1 | same format/divide pattern: cdecl(int, char*) time-to-string |
| 0x488690 | 484 | in 0x489165, 0x48917d, 0x4891a7 | `"%s: %i %%"` draws standings entries from [0x7b0], uses 0x4873b0 |
| 0x488880 | 538 | ext 0x465319, 0x49406c | virtual-slot-0x38 user (calls relevance predicate), font 0x4a9b00 |
| 0x4890e0 | 540 | ext 0x494293 | `"%s: %s"`, calls 0x488690 |
| 0x48e4c0 | 364 | in 0x48965a, 0x48bc9d, 0x48c7c0, 0x48d7a0 | draws sprite quad using local Vec2[4] (vec ctor iterator 0x4c11f4, ctor 0x490ad0, dtor 0x402c90) |
| 0x48e840 | 359 | in 0x48ed66 | same pattern (sprite quad) |
| 0x48e9b0 | 367 | in 0x48eddf | same |
| 0x48eb20 | 462 | in 0x48eda3 | same |
| 0x48ecf0 | 265 | in 0x48dc7b, 0x48dca4, 0x48dccd, 0x48dcf6 | wrapper that calls the three quad helpers above |
| 0x48ee00 | 94 | in 0x48dbb1 | clamp `*p += d` to 0..100 and play sound 0x49b730 (volume-slider style) |

**Large draw/screen functions** (one external caller each, in 0x4926xx-0x4948xx, i.e. the next-next module)

| Addr | Size | Ext caller | Hints |
|---|---|---|---|
| 0x489300 | 1636 | 0x494721 | |
| 0x4899a0 | 1713 | 0x4945f2 | has function-local static colour (atexit at 0x489a2e) |
| 0x48a070 | 2203 | 0x49464f | qsort players with 0x485a50, `" - "`, local static (atexit at 0x48a206) |
| 0x48a920 | 1839 | 0x494698 | qsort with 0x485ad0, `" - "`, statics (atexit at 0x48a9e1, 0x48aa18, 0x48aaa1) |
| 0x48b080 | 1422 | 0x4946f3 | qsort with 0x485b50, local static (atexit at 0x48b19d) |
| 0x48b620 | 1744 | 0x4942de | `"%i."`, uses [0x238],[0x23c],[0x254],[0x328] |
| 0x48bd10 | 2766 | 0x4945b0 | uses std::string [0x77c], [0x780], [0x784], [0x78c], [0x78d], [0x790], [0x3fc] |
| 0x48c7e0 | 1205 | 0x494857 | |
| 0x48cca0 | 723 | 0x49489e | |
| 0x48cf80 | 2145 | 0x4947f6 | std::string, |
| 0x48d7f0 | 1478 | in 0x4895f4 (inside 0x489300) | uses vector [0x834]/[0x838] and 0x48ecf0/0x48ee00 |

All of these use [0x60] (the in-game image) and [0x24c], and most use the global font object (0x56b0c8-based) for text. A shared Vec2 static, the colours 0x56543c / 0x565444 and 0x4ff0xx tables are referenced.

**Tiny functions**

| Addr | Size | Kind |
|---|---|---|
| 0x48e630 | 28 | slot 13 |
| 0x48e650 | 11 | slot 14 |
| 0x48e660 | 1 | empty `ret`, slots 2,3,6,8,9,15,16,17. Also the empty dtor of 0x14-byte UI elements (called at 0x485e93). ~44 ext callers, folded across the binary |
| 0x48e670 | 3 | slot 7 |
| 0x48e680 | 66 | slot 10. Ext callers 0x434806, 0x434b2c (plus data ref in vtable 0x4e2468, ...) |
| 0x48e6d0 | 66 | slot 11. Ext caller 0x43492e |
| 0x48e720 | 72 | direct, ext caller 0x49392b. Per player: if [p+0x28] and virt[0x38](p) and state!=2: car 0x4670c0 |
| 0x48e770 | 57 | direct, ext 0x494bec. Per player: car 0x4672c0 if state!=2 |
| 0x48e7b0 | 79 | direct, ext 0x494043. Like 0x48e720 with 1 arg, car 0x467440 |
| 0x48e800 | 24 | `__stdcall(a, b)`: returns table `[0x4ff068 + (a*10+b)*4]`. Ext callers 0x433731, 0x433ae5, 0x4347c6 |
| 0x48e820 | 20 | thiscall getter: `[0x4ff130 + ([this+0x28]*10+[this+0x2c])*4]`. Ext caller 0x468e3a |
| 0x4890b0 | 3 | slot 12, 28, 29 (`ret 4`) |
| 0x4890c0 | 6 | slot 25, 26 (`return 1`). Also used in ~28 other vtables |
| 0x4890d0 | 8 | slot 27 (`[this+0x44]=1`) |
| 0x48a060, 0x48a910, 0x48b050, 0x48b060, 0x48b070, 0x48b610 | 1 each | empty `ret` stubs: dtors for function-local static colour objects, `push`ed to atexit (0x4c0d01) at 0x489a10, 0x48a1fc, 0x48aa97, 0x48a9fa, 0x48a9c3, 0x48b193. They are not called |

Not code inside the range: jump tables at 0x4871d4 (after 0x487080), 0x489964 (after 0x489300), 0x48bcf0 (after 0x48b620), 0x48ddb6 (after 0x48d7f0). They look like function-sized gaps in functions.csv but are switch tables.

## 3. Layout 0x00 - 0x844 (offsets from the ctor 0x485bb0 and dtor 0x485da0)

| Offset | Size | Field (evidence) |
|---|---|---|
| 0x00 | 4 | vptr (0x4e2520) |
| 0x04 | 0x10 | `vector<Player*>` A. byte 0x04 (allocator, uninitialised stack copy), first 0x08, last 0x0c, end 0x10. All players. Iterated by nearly every method (state [p+0x40]==3 means skip, [p+0x28] = car) |
| 0x14 | 0x10 | vector B. byte 0x14, first 0x18, last 0x1c, end 0x20. Filled by AddPlayer 0x485f60 for players with [p+0x38]!=0 (guess) |
| 0x24 | 4 | local Player* (0) |
| 0x28 | 4 | int, ctor=1 (rally/championship index, guess). With 0x2c used as `[0x4ff130 + (a*10+b)*4]` by 0x48e820 and passed to Stage ctor 0x40c9b0, replay filename 0x48de10 |
| 0x2c | 4 | int, ctor=1 (stage index, guess) |
| 0x30 | 4 | Stage* (0x2534 bytes, ctor 0x40c9b0, dtor 0x40ebc0). Made by 0x486180, freed by 0x486620 and dtor |
| 0x34 | 4 | camera* = 0 |
| 0x38 | 4 | not touched by ctor |
| 0x3c | 4 | int time = 0xfffff448 (-3000). Reset 0x48f1b0 does the same |
| 0x40 | 4 | int = 0 (flag/state; checked as `[this+0x40]` in slot 18/22) |
| 0x44 | 4 | int = 0 (ctor), set to 1 by slot 27 and by Reset 0x48f1b0 |
| 0x48 | 4 | int = 0 (ctor), Reset sets 1. Tested in slot 18 ([0x48]!=0 selects flag arg of car 0x4693a0) |
| 0x4c, 0x50 | 4+4 | ints = 1 |
| 0x54 | 4 | int = 0 |
| 0x58 | 4 | Sprite* "OnScreenBars" (`data/global/overlay/onscreenbars.tga`, 0xd4-byte Sprite, ctor 0x4a85e0, image obj 0x68 bytes via 0x4a8030) |
| 0x5c | 4 | Sprite* "OnScreenBarsLeft" (`.../onscreenbarleft.tga`) |
| 0x60 | 4 | pointer to a 0x48-byte object, loaded from `data/extras/ImageInGame.tga` (0x4a8ae0; then 0x4a9280(5,6)). Used by every big draw function |
| 0x64 - 0x237 | 0x1d4 | never touched by the ctor/dtor, and no this-relative access found in the module (guess: unused or derived-only) |
| 0x238, 0x23c | 4+4 | 0x23c=-1 in ctor. 0x238/0x23c used in 0x48b620 |
| 0x240, 0x244, 0x248 | 4 each | 0 in ctor. 0x240/0x244 used in 0x489300, 0x48bd10, 0x48cf80 |
| 0x24c | 4 | not set by ctor. Read in the big draw functions and 0x48d7f0 |
| 0x250 | 4 | Sprite* mouse cursor (`data/menu/MouseCursor.tga`). Dtor: `[ [0x250] ]` is released via virtual dtor, then 0x4a86d0, free |
| 0x254 | 0xd4 | embedded Sprite (ctor 0x4a8580, dtor 0x4a86d0). Ctor then sets its fields: 0x26c=5, 0x270=6, 0x274=2, 0x278=2, 0x27c=1, 0x284=1, 0x294-0x29c=0, 0x2a0=128.0f, 0x2a4-0x2d0=0, 0x2ec=0xf |
| 0x328 | 0xd4 | embedded Sprite; member 0 gets an image from `data/extras/replaybuttons.tga` (0x68-byte object via 0x4a8030). Fields at 0x340-0x3c4 set by 0x486240 (5, 6, 2, 2, floats 1.0, 0xf) |
| 0x3fc | 0x380 | (to 0x77c) not constructed by the base. 0x3fc itself is referenced by 0x48bd10. Guess: more embedded objects (arrays) or reserved |
| 0x77c | 0x10 | std::string (VC6 layout: allocator byte 0x77c, ptr 0x780, len 0x784, res 0x788). Ctor `0x402700(0)`, dtor `0x402700(1)` (release) |
| 0x78c, 0x78d | 1+1 | bytes (flags), used in 0x48bd10 |
| 0x790 | 4 | int, used in 0x48bd10 |
| 0x794 | 4 | ghost Player* (= 0 in ctor; deleted by Player dtor 0x490ee0 + `operator delete`; created in 0x48de10) |
| 0x798 | 4 | int flag: "ghost active/visible" (set 0/1 by slots 18 and 22) |
| 0x79c | 4 | int (used in 0x48a070) |
| 0x7a0 | 4 | int, split delta (written 0x488420) |
| 0x7a4 | 4 | int, last split time stamp (0x488420) |
| 0x7a8 | 4 | int, split index. Not set by base ctor. Derived ctor 0x48f130 sets 0. |
| 0x7ac | 4 | int = -1 (ctor) |
| 0x7b0 | 0x78 | 6 standings entries x 0x14 bytes (0x7b0..0x827). 0x488690 reads [0x7b0..], 0x487560 [0x7b4]/[0x7b8] |
| 0x828, 0x82c, 0x830 | 4 each | ints. 0x82c read in 0x487560 |
| 0x834 | 0x10 | `vector<UIElement*>` (byte 0x834 = alloc, first 0x838, last 0x83c, end 0x840). Filled with 7 objects of 0x14 bytes by 0x48ee60 (called from ctor). Dtor deletes elements (trivial dtor 0x48e660), erases the range with 0x484380 (vector erase instance in NetRace module) and frees |
| 0x844 | - | derived class fields start here (0x4e2690 class uses 0x844 and 0x848) |

Dtor order (0x485da0): [0x794] ghost, [0x250] cursor sprite, vector A elements (0x490ee0 + free), [0x30] Stage (0x40ebc0 + free), 0x486590, [0x834] elements/erase/free, string [0x77c] release, sprites [0x328] and [0x254] (0x4a86d0), free vector B buffer, free vector A buffer.

## 4. Other classes, templates

- Only one vtable is stored inside the module: 0x4e2520, at 0x485c56 and 0x485dc1. No other class has its ctor here.
- Derived classes in neighbours: sibling 0x4e23ac (0x480800), NetRace 0x4e2440 (0x481390), 0x4e2690 (ctor 0x48f130, dtor 0x48f160/0x48f180, Reset-like 0x48f1b0, next one 0x48f260), 0x4e2708 (0x48f6xx). The classes in 0x4926xx-0x4948xx call the large draw functions and 0x4866b0 etc. directly.
- Classes used here but defined elsewhere: Sprite (0xd4 bytes, ctor 0x4a8580/0x4a85e0, dtor 0x4a86d0), Player (dtor 0x490ee0), Stage (0x2534 bytes, ctor 0x40c9b0, dtor 0x40ebc0), std::string (0x402700 ctor/dtor, 0x402960/0x402980), UI element (0x14 bytes, ctor 0x4390a0, trivial dtor 0x48e660), Vec2 (8 bytes, ctor 0x490ad0, trivial dtor 0x402c90).
- STL instances: none defined in the module. It calls `vector<T*>::insert(pos, n, val)` at 0x4bc680 (outside), `vector::erase` 0x484380 (in the NetRace module), `vector::size` 0x414460 and the EH `__vec_ctor` helper 0x4c11f4 and 0x4c1116. One vector grow is inlined in AddPlayer 0x485f60 (the second push_back).
- Every `new` goes through 0x4c1276 and every delete through 0x4bd000.

## 5. Suggested decompile order

1. One-byte and trivial: 0x48e660, 0x48e670, 0x48e650, 0x48e630, 0x4890b0, 0x4890c0, 0x4890d0, 0x48e800, 0x48e820, 0x487b30, six empty atexit stubs (0x48a060, 0x48a910, 0x48b050, 0x48b060, 0x48b070, 0x48b610), the deleting dtor 0x485d80.
2. Player-loop helpers, each ~50-80 bytes: 0x486130, 0x486200, 0x486620, 0x486650, 0x48e680, 0x48e6d0, 0x48e720, 0x48e770, 0x48e7b0, 0x48dde0, 0x48ee00, 0x486180, 0x486590.
3. Comparators 0x485a50, 0x485ad0, 0x485b50, and formatters 0x486be0, 0x4885a0.
4. Slots 20, 21, 23, 24 (0x488ef0, 0x488f70, 0x489030, 0x488fd0) and 19 (0x488e50), then 18 and 22 (0x488aa0, 0x488c70; near-identical, SEH).
5. Statics (11 pairs 0x4856e0-0x485a2b, plus 0x48f100/0x48f120) and ctor/dtor with their helpers: 0x486240, 0x48ee60, 0x485f60, 0x485bb0, 0x485da0.
6. The sprite-quad helpers 0x48e840, 0x48e9b0, 0x48eb20, 0x48ecf0, 0x48e4c0, and the 0x487200/0x4873b0 pair.
7. Text draw helpers: 0x487560, 0x4877a0, 0x487920, 0x4880f0, 0x488420, 0x4884e0, 0x488690, 0x4890e0, 0x487b50, 0x487e00, 0x486f80, 0x487080, 0x488880.
8. Replay and highscore I/O: 0x48e1f0, 0x48de10, 0x486d20, 0x4866b0.
9. The big draw functions last: 0x48cca0, 0x48c7e0, 0x48d7f0, 0x48b080, 0x4899a0, 0x489300, 0x48a920, 0x48a070, 0x48b620, 0x48cf80, 0x48bd10. These share statics and the qsort comparators, so port them after the comparators and statics exist.

Notes for the maintainer:
- Update modules.csv: Networking should end at 0x4856e0 (not 0x485700), and the new module spans 0x4856e0-0x48f130.
- 0x48e660 is an `ret`-only function used as the empty virtual and as the trivial dtor; do not create a separate C++ function per use (folded identical).
