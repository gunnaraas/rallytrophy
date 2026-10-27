<!-- Subagent analysis, 2026-10-23; read-only disassembly; guesses are marked. -->
# Range 0x43d770 - 0x44136f: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py over the whole range, refs.py for callers and absolute refs (vtable slots show up as `abs@off`; file offset = VA - 0x400000 + 0 for .rdata here, e.g. vtable 0x4e17fc = file offset 0xdfffc), .rdata/.data strings and floats read with a PE section reader, the init-table scan (file offsets 0xefb30-0xefb6c), text ids resolved against `language_english.TXT` (id = 0-based line number, same as range-441370.md). "Guess" marks inference. EH handler addresses are the `push imm` after `push -1` in each SEH prologue.

## 0. Headline findings

1. **Two files, two MenuScreen subclasses**, split exactly where the prompt said. Boundaries are the `_$E` guard stubs, all three confirmed in the CRT init table (file offsets 0xefb50 -> 0x43d740, 0xefb54 -> 0x43f9b0, 0xefb58 -> 0x441340, 0xefb5c -> 0x441370 = next file's static initialiser):
   - **File A, 0x43d770 - 0x43f9af (+ stub 0x43f9b0-0x43f9df)**: the *player-name entry* screen. Guess name `PlayerNameMenu`. Embedded in Game at **+0x14cc**, size **0x9d0**, vtable **0x4e17fc**.
   - **File B, 0x43f9e0 - 0x44133f (+ stub 0x441340-0x44136f)**: the *Race Options* screen (text 0x46 "RACE OPTIONS": Units / Ghost Car / Wildlife / Pace Notes / Back). Guess name `RaceOptionsMenu`. Embedded in the **menu manager** (the vtable-0x4e17c8 object at Game+0x2df4, ctor 0x43c590, size 0xcf8) at manager **+0x11c** (so Game+0x2f10), size **0x18c**, vtable **0x4e183c**. Same family as SoundMenu (0x450ee0, manager +0x750, size 0x18c, `NetRace* net` at +0x188, `Run(game, net)`, `LoadLayout`).
   - Neither file has file-scope statics (no initialiser pair at either start; file A starts with a SEH constructor, file B too). Each file ends with its own `_$E` stub.
2. **Hint check, 0x43ff80 = MenuScreen::Release (confirmed)** and it lives in file B. Body: `if (this[+4]) { this[+4] = 0; this[+0xc] = 0; jmp 0x438d00 }` (0x438d00 = the MenuScreen method that clears the item-rect vector). It is ICF-folded: 9 direct calls (0x4244fb, 0x42672b, 0x42978b, 0x42e9eb, 0x43927b, 0x43faeb (own dtor), 0x44b9eb, 0x44eb1b, 0x450fbc) and 9 vtable slot-2 entries (file offsets 0xdfd1c, 0xdfd50, 0xdfdbc, 0xdfe90, 0xdff2c, 0xe0044 (= our vtable), 0xe01cc, 0xe0254, 0xe0284). The surviving copy sits in file B, so `RaceOptionsMenu::Release` must be defined as an out-of-line member with exactly this body in src for file B (same pattern as SoundMenu::Release in include/unk_450ee0.h). File A has its *own* Release (0x43de10, it frees the linename sprite), unlike TrackSelectMenu which also has its own.
3. **Shared empty slot 3** `0x4414c0` (`ret 4`, 4 bytes) is the vtable slot 3 of both screens; it is defined in the *next* TU (range-441370.md, section 0 item 3). Not part of this range.
4. **functions.csv misses three functions and two stubs** in this range (it lists 19 entries; the range has 22 functions + 2 stubs):
   - **0x43db40** (28 bytes) `SpriteCell` constructor helper (vec-ctor callback, `0x4c11f4(this+4, 8, 2, 0x490ad0, 0x402c90)`; gap after 0x43db20 ends at 0x43db3e).
   - **0x43db60** (19 bytes) `SpriteCell` destructor helper (vec-dtor callback `0x4c1116(this+4, 8, 2, 0x402c90)`).
   - **0x43fba0** (989 bytes, `ret 4`) **RaceOptionsMenu::Setup** = vtable slot 1 (abs ref at file offset 0xe0040). Real code in the 1006-byte gap after 0x43fac0 (functions.csv has the gap 0x43fb92-0x43ff80). This is the biggest hole; it ends with `call 0x440890; ret 4` at 0x43ff71-0x43ff7a (which is why csv attributes the callee 0x440890 to nothing).
   - **0x43f9b0** (48 bytes) file A's `_$E` stub (`mov cl,[0x5826d5]; ... push 0x402c90; call 0x4c0d01`), init-table entry 0xefb54.
   - **0x441340** (48 bytes) file B's `_$E` stub, init-table entry 0xefb58.
   All other inter-function bytes are `nop` padding (0x90) or two jump-table areas (next item).
5. **Two jump tables sit in .text** (not in any function):
   - **0x43ee50-0x43ee67** (6 dwords: 0x43e7c3, 0x43e7e8, 0x43e80d, 0x43e82e, 0x43e850, 0x43e872), used by `jmp [eax*4+0x43ee50]` at 0x43e7bc inside 0x43df70 (switch on the selected row 0..5 for the left/right icon change; default 0x43e899). Followed by 0x90 padding up to 0x43ee70.
   - **0x440870-0x44088f** (two 4-dword tables back to back): table 1 at 0x440870 (0x44051c, 0x440524, 0x44052c, 0x440534: toggle `Units`, `GhostCar`, `Wildlife`, `PaceNotes` via `MenuScreen::Toggle(&global, delta)` 0x438ca0 on globals 0x501300 / 0x501304 / 0x50131c / 0x501478), used at 0x440515 inside 0x43ffd0; table 2 at 0x440880 (0x4406c5, 0x4406f4, 0x440725, 0x440757: draw the four toggle rows with `DrawToggle`), used at 0x4406be (`jmp [ebp*4+0x440880]`). Both inside 0x43ffd0; a straight linear sweep desynchronises after 0x44086d, so always disassemble 0x440890 on its own.
6. **Not a data-heavy range**: the only data tables are the key table `0x4f8f58` (26 `{char* key, int type=3}` pairs, 0x1a = SetBase count) and the player-name/icon globals (section 1).
7. **The "linename" sprite is not text**: `data/menu/linename.tga` (0x4f8f28) is a `Sprite` (0xd4 bytes, ctor 0x4a8580, texture `Texture(path, 0x841, 0)` ctor 0x4a8030 + `Upload` 0x4a8350) used as a 512x256 plate behind the name rows; the 64 `SpriteCell` records (0x1c each) are a 4-column x 16-row UV grid of 64x32-pixel cells taken from `Game+0x1390f8` (a pointer read from Game, guess: the shared HUD/icon texture).

## 1. File A: PlayerNameMenu, 0x43d770 - 0x43f9af (guard stub 0x43f9b0)

Used by the single-player/network front end: `Open(game, net)` (0x43de50) is called from 0x4333b2 (in 0x433220 -> `if (Open == -1) ...`, flag `[0x501464] != 3`), 0x433cd6 (in 0x433bc0) and 0x434641 (in 0x434300, right after `0x4b7730(0x501338, [0x56b0b8] + 0x238, 0x1e)` copies the profile name into the first default name slot when `0x4853e0() == 0`... guess from the code shape). Constructor caller: Game ctor 0x430435 (`lea ecx,[esi+0x14cc]`, EH state 2); destructor caller: Game dtor 0x43095f.

### Layout (offsets in PlayerNameMenu; "S" = seen; names are guesses)
Base `MenuScreen` (0x20 bytes, ctor 0x4381c0, dtor 0x438210, include/unk_450ee0.h): +0 vtable 0x4e17fc, +4 state (Setup returns early when nonzero), +8 selected row (0..7), +0xc `Game*`, +0x10 `std::vector<UIRect*>`.
- **+0x20 `MenuLabel labels[8]`** (stride 0x24; vec-ctor `0x4c11f4(this+0x20, 0x24, 8, 0x430700, 0x430720)`, EH state 0): [0..5] = the six captions `"<text 0x94 'Player'>" + " " + itoa(i+1) + ":"` (ctor builds them with `GetText` 0x405380, the string `" "` 0x4f2e40, `itoa` 0x4c1902 radix 10 and `":"` 0x4f8f24), label x = 54.0 (0x4e17ec) * scaleX, y = (262.0 + 42.0 * i) * scaleY; [6] (+0xf8) = text 0x17 "Continue" (assigned in 0x43f570), [7] (+0x11c) = text 0x18 "Back". Text-id table: dwords 0x17, 0x18 at 0x4f8f1c / 0x4f8f20.
- **+0x140 `MenuLabel positions[6]`** (EH state 1): only x/y (+0x18/+0x1c) are written by the ctor: x = (381.0 + 58.0) * scaleX (0x4e17f0 + 0x4e14c4), y = (262.0 + 42.0 * i - 8.0) * scaleY (guess: the anchor of each name field; text is drawn from the string array below).
- **+0x218 `int icon[6]`** (ctor copies them from the global `int[6]` at 0x5013ec; Run copies them back on exit), **+0x230 `int editing[6]`** (Run sets all to 1 before the loop; `rep stos` of 6 dwords).
- **+0x248 `SpriteCell cells[64]`** (vec-ctor `0x4c11f4(this+0x248, 0x1c, 0x40, 0x43db40, 0x43db60)`, EH state 2; 64 * 0x1c = 0x700, ends at 0x948). `SpriteCell` (0x1c): +0 texture pointer, +4 `HudPoint uv0`, +0xc `HudPoint uv1` (two `HudPoint` objects built by 0x490ad0 in the helper), +0x14 float width = 64.0 (0x42800000), +0x18 float height = 32.0 (0x42000000). Setup fills cell n with column c = n % 4, row r = n / 4: uv0 = (c * 0.25, r * 0.0625), uv1 = ((c+1) * 0.25, (r+1) * 0.0625).
- **+0x948 int `rowCount`** (= global `[0x501334]`, set by 0x43f570), **+0x94c `void* iconTexture`** (= `[game + 0x1390f8]`, set by Setup), **+0x950 `Sprite* plate`** (`new(0xd4)` Sprite, ctor 0x4a8580, texture `data/menu/linename.tga`, sprite fields +0x40..+0x4c and +0x60..+0x6c = 1.0f, +0x98 = 0xf, +0x9c = 2, +0x18 = 5, +0x1c = 6, +0x24 = 2), **+0x954 DWORD `blinkTick`** (`call ds:0x4e11d8`; guess: the `GetTickCount` import, it returns a millisecond counter), **+0x958 int `blinkOn`** (cursor visible 0x50 = 80 ms after tick, off after 0x78 = 120 ms).
- **+0x95c `std::string scratch`** (alloc byte +0x95c, ptr +0x960, len +0x964, cap +0x968, all zero in the ctor; guess: temp for the typed name).
- **+0x96c `std::string name[6]`** (stride 0x10; vec-ctor `0x4c11f4(this+0x96c, 0x10, 6, 0x404cd0, 0x430720)`, EH state 4; ctor then assigns "Player 1".."Player 6" from the global table 0x501338 with stride 0x1e). Names are limited to 15 characters.
- **+0x9cc `NetRace* net`** (stored by Open from its second argument; `0x4831c0` (net service call) in Run when non-null).
- Object size **0x9d0** (next Game member +0x1e9c, ctor 0x42c2b0).
- `[ebx+0x120]` / `+0x1c` / `+0x20` in Run are the Game text renderer (`TextRenderer* [game+0x1390c4]`, colour at +0x120, scale at +0x1c/+0x20), as in the other screens.

### Vtable 0x4e17fc (read from .rdata)
| Slot | Address | Role |
|---|---|---|
| 0 | 0x43db20 | scalar deleting dtor (`call 0x43db80`; `delete this` via 0x4bd000 when bit 0 of the flag arg) |
| 1 | 0x43dc70 | `Setup(Game*)`, `ret 4` |
| 2 | 0x43de10 | `Release()` (own, not the shared 0x43ff80) |
| 3 | 0x4414c0 | shared empty `Slot3(int)`, `ret 4` (defined in the next TU) |
No other slots; the .rdata floats start at 0x4e180c (the screen's layout constants).

### Strings, constants, globals
- Strings: `data/menu/linename.tga` (0x4f8f28), `" "` (0x4f2e40), `":"` (0x4f8f24), `""` (0x5070b0, empty, used to reset a blank name), cheat names `KALJAKOPPA` (0x4f8f48) and `QUALIFY` (0x4f8f40), default names `Player 1` ... `Player 6` at 0x501338 + 0x1e*n (30-byte slots; dword `[0x501334]` just before it = number of rows, initial value 1).
- Text ids: 0x94 "Player" (per caption), 0x39 "Enter your name" (screen title), 0x17 "Continue", 0x18 "Back". Colours: `[0x508bfc]` title, `[0x508c04]` normal, `[0x508c0c]` selected (as in the sibling screens).
- Float constants (all `.rdata`): 381.0 (0x4e17f0), 58.0 (0x4e14c4), 262.0 (0x4e17d8), 42.0 (0x4e17dc), 8.0 (0x4e1644), 54.0 (0x4e17ec), 54.0/142.0 (0x4e17e0/0x4e17e4: title x/y), 610.0 (0x4e17e8), 762.0 (0x4e17f4), 64.0/1.0 (0x4e17f8 / 0x4e1348: icon index range 0..63), 490.0 (0x4e180c: name width limit), 915.0 (0x4e1814), 96.0 (0x4e1810), 12.0 (0x4e1818), 24.0 (0x4e158c), 512.0 / 256.0 (0x4e14e8 / 0x4e14e0: plate size), 0.5 (0x4e138c), 1.5 (0x4e1530), 10.0 (0x4e1330), 420.0 (0x4e16a0), 200.0 (0x4e1648), 128.0 (0x4e1524), 4.0 (0x4e164c), 32.0/16.0 (0x4e14c0/0x4e14bc), 0.25/0.0625 (0x4e13a4 / 0x4e1660), scale factors 1/1024 (0x4e1358) and 1/768 (0x4e1354); `[0x56aea8]` / `[0x56aeac]` = X/Y screen scale.
- Cheat codes typed into a name (0x43f8b0): `KALJAKOPPA` (unlock everything: sets `[0x501498] = [0x50149c] = 5`, `[0x5014cc] = [0x5014d0] = -1`, bytes `[0x5014c8] = [0x5014c9] = 1`, loops `[0x5014a0..] = 6` / `[0x5014b4..] = 2`, `[0x5014a0] = 8`; returns 1) and `QUALIFY` (toggles `[0x501480]`; returns 0). Both strcmp loops are inlined. Guess: career-progress unlock tables and a "qualify" cheat flag.
- Input/system globals as in the other screens: renderer `0x56b0ac` (vcalls 0x84, 0xa8, 0x6c, 0x70, 0x34, 0x24, 0x98, 0x8c, 0x108), input `0x56b0bc` (`ClearKeys` 0x4b7390), mouse `0x56b0c0`, `0x57ba00/04` mouse xy, `0x57ba20/24` buttons, `0x57ba35` Enter, `0x57ba43` Esc, `0x57ba4d/4e/4f/50` Left/Up/Right/Down, `0x57b9f4` window closed. Click sounds on `Game+0x139224` (hover), `+0x139294` (select), `+0x139304` (back), `SoundSource::Play` 0x49b730.

### Function table (file A)
Frames: "SEH" = FS:[0] frame with the EH handler address; "FPU n" = number of x87 instructions.
| Addr | Size | Convention / frame | Role (guess) | Main callers |
|---|---|---|---|---|
| 0x43d770 | 936 | thiscall (ecx), SEH (EH 0x4d89f4), FPU 19, `sub esp,0x2c` | **Ctor**: `MenuScreen::MenuScreen` (0x4381c0), the four vec-ctors (labels, positions, cells, names), the 6-iteration init loop (captions + positions + icon copy), vtable store `0x4e17fc` | Game ctor 0x430435 |
| 0x43db20 | 30 | thiscall, `ret 4`, no SEH | **Scalar deleting dtor** (slot 0), compiler-generated | vtable only |
| 0x43db40 | 28 | thiscall, no SEH; **not in csv** | `SpriteCell` ctor helper (vec-ctor callback: 2 `HudPoint`s at +4) | abs ref in ctor 0x43d7d8 |
| 0x43db60 | 19 | thiscall, no SEH; **not in csv** | `SpriteCell` dtor helper (vec-dtor callback) | abs refs in ctor/dtor/EH tables |
| 0x43db80 | 239 | thiscall, SEH (EH 0x4d8a74) | **Dtor**: sets vtable, calls Release 0x43de10 (direct, 0x43dbac), destroys names (inlined string release for +0x960), cells, positions, labels with `0x4c1116`, then `MenuScreen::~MenuScreen` 0x438210 | 0x43db23 (deleting dtor), Game dtor 0x43095f |
| 0x43dc70 | 416 | thiscall, `ret 4`, SEH (EH 0x4d8a96), FPU 12 | **Setup(Game*)** (slot 1): guard on state, store game, read `game+0x1390f8`, fill 64 `SpriteCell`s, `new(0xd4)` Sprite + `new(0x68)` Texture `linename.tga` (0x841), `Upload` with the renderer, set sprite fields | vtable slot 1; Open 0x43de6c (virtual call) |
| 0x43de10 | 62 | thiscall, no SEH | **Release()** (slot 2): if state, clear state and game, virtual-delete the texture ([sprite+0] `call [vtable+0]` with 1), `~Sprite` 0x4a86d0, `delete` | vtable slot 2, dtor 0x43dbac |
| 0x43de50 | 288 | thiscall, `ret 8`, no SEH | **Open(Game*, NetRace*)**: store net at +0x9cc, virtual Setup, copy the six default names from 0x501338 into `name[]` (inlined `string::assign`), `if ([0x501458]) [0x501334] = 1`, `SetupLabels` 0x43f570, clamp selection (`if sel < 6 && sel >= rowCount sel = 0`), tail-call into `Run` 0x43df70 | 0x4333b2, 0x433cd6, 0x434641 (Game flow) |
| 0x43df70 | 3806 | thiscall, SEH (EH 0x4d8b0c), FPU 110, `sub esp,0x94` | **Run()**: the whole modal loop. Renderer state, `BuildRects` 0x43ee70, `rep stos` of the 6 flags, message pump `0x4b69f0`, `Game::Update` 0x432c00, optional `NetRace` service 0x4831c0, mouse hit tests (`MenuScreen::HitTest` 0x438f90 with the rect vector, `UIRect::Contains` 0x4390d0), keyboard (text edit: backspace 8, printable 0x20/0x21-0x7e/0xc0-0xff via `string::erase/insert` 0x408de0/0x408d10, Up/Down wrap with selection 0..7 skipping rows >= rowCount, Left/Right change icon via jump table 0x43ee50 -> `MenuScreen::AdjustValue(int*, delta, lo=0, hi=63)` 0x438c40), cheat check 0x43f8b0 on Enter, draw: title (text 0x39) with `Font::Print` 0x4a97a0, plate 0x43f740, per-row caption / name / icon cell (`MenuScreen` cell draw 0x438990 with `&cells[icon]`), blinking cursor 0x43f400, mouse cursor 0x438f40, `EndScene`/`Present`; on exit copies names/icons back to 0x501338 / 0x5013ec and returns the selection or -1 (plays the back sound) | Open 0x43df63 only |
| 0x43ee70 | 1415 | thiscall, SEH (EH 0x4d8b8f), FPU 61 | **BuildRects()**: builds the `vector<UIRect*>` at +0x10: 6 row rects (x 54..762-ish, y from 262 + 42*i; rows >= rowCount get `enabled = 0`), the Continue and Back rects (x 610 / 142, rows after the last), then (guess from the loop at 0x43f0fc) 2 arrow rects per row; each via `new(0x14)` (0x4c1276) + `UIRect(HudPoint, HudPoint)` 0x4390a0 + `vector::insert` 0x4bc680 (`0x424300/0x424320/0x424350` vector helpers) | Run 0x43e041 |
| 0x43f400 | 357 | thiscall, `ret 0x10`, **ebp frame**, SEH (EH 0x4d8bce), FPU 10 | **DrawTextCursor(Renderer*, float x, float y, DWORD colour)**: two 4-element `HudPoint` arrays (quad corners and UVs; UV constants 0x3e800000 = 0.25, 0x3e980000 = 0.296875, 0x3e940000 = 0.2891, 0x3ec00000 = 0.375), texture from `Game::0x432a00`, renderer vcalls 0x8c (SetTexture) and 0x108 (DrawQuad) | Run 0x43ec25 |
| 0x43f570 | 461 | thiscall, no SEH, FPU 14 | **SetupLabels()**: `rowCount = [0x501334]`; assigns `GetText(0x17)` to label[6] and `GetText(0x18)` to label[7] (two inlined `string::assign`); sets label[6] x = `[+0x110] = 54.0 * sx` (0x4e17ec) and `[+0x114] = (rowCount * 42.0 + 262.0) * sy`, label[7] `[+0x134] = 54.0 * sx`, `[+0x138] = 610.0 * sy` (0x4e17e8); stores `blinkTick = GetTickCount()` | Open 0x43df45 |
| 0x43f740 | 368 | thiscall, `ret 8`, **ebp frame**, SEH (EH 0x4d8c0e), FPU 18 | **DrawPlate(float x, float y)**: draws `plate` (`[this+0x950]`) as a 512 x 256 quad at (x, y) * scale (two 4-element `HudPoint` arrays, vcalls 0x8c / 0x108) | Run 0x43e957 (called with x = 0x43d88000 = 433.0, y = 0x43720000 = 242.0 as pushed) |
| 0x43f8b0 | 241 | thiscall (ecx set, **`this` unused**), `ret 4`, no SEH, no FPU | **CheckCheatName(const char* name)**: inlined `strcmp` against `KALJAKOPPA` / `QUALIFY` (see above); returns 1 for the unlock code, 0 otherwise | Run 0x43e1e5 |
| 0x43f9b0 | 48 | `_$E` stub | guard bit 0x5826d5 + `atexit(0x402c90)` (STL init) | init table 0xefb54 |

Ghidra evidence `ev_d378b16bc7d7bf3b1b773af280befd0195b1c1203350bf8066a6ea214b9c2e17` confirms the 368-byte target and the scheduling around the two renderer calls. Early `DAT_0056b0ac` / `plate` locals, including a trial that moved `Color(-1)` closer to `DrawQuad`, worsened code generation (373–375 bytes and shifted EH/local frame); the variants were reverted. Keep the current compiler-generated EBP SEH frame until a lifetime change preserves it.

## 2. File B: RaceOptionsMenu, 0x43f9e0 - 0x44133f (guard stub 0x441340)

Constructor caller: menu manager ctor 0x43c590 at 0x43c5f2 (`lea ecx,[esi+0x11c]; call 0x43f9e0`, EH state 2); destructor caller: manager dtor 0x43c680 at 0x43c6fb (call 0x43fac0, state 2 -> direct call, not through the deleting dtor); Run caller: manager dispatch method 0x43cb70 (186 bytes, jump table 0x43cc2c, case at 0x43cb9b: `Run(game = [this+0xc], net = [this+0xcf4])`). Neighbours in the manager: +0x2a8 (ctor 0x4295f0, size 0x4a8), SoundMenu +0x750, +0x8dc (0x424410), +0xa48 (0x439170). In Game: **Game+0x2df4 + 0x11c = Game+0x2f10**.

### Layout (offsets in RaceOptionsMenu; size 0x18c)
- Base `MenuScreen` +0..+0x1f (vtable 0x4e183c, state +4, selected +8 (ctor 0), game +0xc, rect vector +0x10).
- **+0x20 `std::string title`** (alloc byte from the stack garbage as in the other screens, ptr/len/cap zeroed; Setup assigns text 0x46 "RACE OPTIONS"); +0x30/+0x34 untouched (padding); **+0x38 float textScaleX = screenW/1024 * 32.0**, **+0x3c float textScaleY = screenH/768 * 16.0** (0x4e1358 * 0x4e14c0, 0x4e1354 * 0x4e14bc on `[0x56b0ac]+4/+8`); **+0x40 int = -1**.
- **+0x44 `MenuLabel items[5]`** (stride 0x24, vec-ctor count 5, EH state 1): [0] +0x44 text 0x48 "Units", [1] +0x68 text 0x49 "Ghost Car", [2] +0x8c text 0x4c "Wildlife", [3] +0xb0 text **0x19a** (past the end of the 410-line english file; guess "Pace Notes" via a different string table), [4] +0xd4 text 0x18 "Back".
- **+0xf8 `MenuLabel units[2]`** (count 2, EH state 2): +0xf8 = text 0x1f "Metric", +0x11c = text 0x20 "Imperial".
- **+0x140 `MenuLabel onOff[2]`** (count 2, EH state 3): +0x140 = text 2 "Disabled", +0x164 = text 1 "Enabled".
- **+0x188 `NetRace* net`** (set by Run). Size 0x18c = 0x188 + 4.
- Settings it edits are four int globals (`Toggle` 0x438ca0, 0/1): **0x501300 Units** (0 metric, 1 imperial), **0x501304 Ghost Car**, **0x50131c Wildlife**, **0x501478 Pace Notes** (initial values in the exe image: 0, 0, 1, 1).
- Layout is data-driven: `LoadLayout` reads `data/extras/racemenu.ini` (0x4f9298) through `Config`: ctor 0x405580, `SetBase(0x4f8f58, 0x1a)` 0x4076c0, `Load(path, 1, 0, 0)` 0x406ce0, 26 `GetVector2(key)` 0x408390 calls, dtor 0x4055a0. Keys (26 entries at 0x4f8f58): `UnitsText_UpperLeft/LowerRight`, `GhostCarText_*`, `WildlifeText_*`, `PaceNotesText_*`, `BackText_*` (0x4f62dc / 0x4f62c8, shared with the track screen), and for each of Units, GhostCar, Wildlife, PaceNotes: `<Name>Arrow1_UpperLeft/LowerRight` and `<Name>Arrow2_UpperLeft/LowerRight` (strings 0x4f9028-0x4f9284). It creates **13 UIRects** (5 text rects + 8 arrow rects; the 13th is inserted by the inlined `vector::insert` with `0x403130/0x403140`, the other 12 by `0x4bc680`) in the order Units, GhostCar, Wildlife, PaceNotes, Back, UnitsArrow1/2, GhostCarArrow1/2, WildlifeArrow1/2, PaceNotesArrow1/2.

### Vtable 0x4e183c
| Slot | Address | Role |
|---|---|---|
| 0 | 0x43faa0 | scalar deleting dtor (`call 0x43fac0`) |
| 1 | 0x43fba0 | `Setup(Game*)`, `ret 4` (**not in functions.csv**) |
| 2 | 0x43ff80 | `Release()` (shared MenuScreen::Release, defined here) |
| 3 | 0x4414c0 | shared empty `Slot3(int)` |
After the four slots: the screen's float constants at 0x4e184c.. (`0x43830000` = 262.0 ... same style as the other screens).

### Constants / strings
- `data/extras/racemenu.ini`, the 26 key strings, 262.0 (0x4e181c), 42.0 (0x4e1820), 54.0 (0x4e1824), 610.0 (0x4e1838), 520.0 (0x4e1828), 54.0/142.0 (0x4e182c / 0x4e1830: title position), 400.0 (0x4e1834: column of the value labels), 1.5 (0x4e1530), 0.5 (0x4e138c), 32.0/16.0 (0x4e14c0/0x4e14bc), scale 1/1024 and 1/768 (0x4e1358/0x4e1354). Colours: `[0x508bfc]` title, `[0x508c04]` normal, `[0x508c0c]` selected.
- Text ids: 0x46, 0x48, 0x49, 0x4c, 0x19a, 0x18, 2, 1, 0x1f, 0x20 (the three value pairs); **0x19a is outside the 410-line english table** (flag for the resolver: the id is probably a later entry of the real table than the extraction gives, or the label is empty at runtime).

### Function table (file B)
| Addr | Size | Convention / frame | Role (guess) | Main callers |
|---|---|---|---|---|
| 0x43f9e0 | 177 | thiscall, SEH (EH 0x4d8c61), no FPU, `sub esp,8` | **Ctor**: `MenuScreen::MenuScreen`, title string init, vec-ctors for the three label arrays (counts 5, 2, 2 with 0x430700 / 0x430720), `selected = 0`, vtable store `0x4e183c` | manager ctor 0x43c590 (0x43c5f2) |
| 0x43faa0 | 30 | thiscall, `ret 4`, no SEH | **Scalar deleting dtor** (slot 0), compiler-generated | vtable only |
| 0x43fac0 | 210 | thiscall, SEH (EH 0x4d8cc9) | **Dtor**: vtable store, `Release` 0x43ff80 (direct call at 0x43faeb), vec-dtors of the three arrays, inlined release of the title string, `~MenuScreen` 0x438210 | 0x43faa3, manager dtor 0x43c680 (0x43c6fb) |
| 0x43fba0 | 989 | thiscall, `ret 4`, **no SEH**, FPU 30, `sub esp,0xc`; **not in csv** | **Setup(Game*)** (slot 1): guard on state, compute `textScaleX/Y` (+0x38/+0x3c) from `[0x56b0ac]`, assign the ten texts (0x46 title, items, value labels) with inlined `string::assign` (`0x402980` grow + `rep movs` + `0x402960` set length), label positions, then `call 0x440890` (LoadLayout) as the last step | vtable slot 1 (abs ref file offset 0xe0040) |
| 0x43ff80 | 21 | thiscall, no frame | **MenuScreen::Release** (see section 0.2); ICF root of 9+9 users | slot 2 of nine vtables; seven other direct calls |
| 0x43ffa0 | 41 | thiscall, `ret 8` | **Run(Game*, NetRace*)**: `[this+0x188] = net; vtbl[1](game); do r = Frame(); while (r != -1); return r != 0`-style loop (`or eax,eax` at the end) | manager dispatch 0x43cb70 (0x43cbac) |
| 0x43ffd0 | 2205 | thiscall, SEH (EH 0x4d8d78), FPU 52, `sub esp,0x74` | **Frame()**: renderer state (0x84 = (5,0), 0xa8 = 8), `ClearKeys`, message pump 0x4b69f0, `Game::Update` 0x432c00, `NetRace` service 0x4831c0 when non-null, mouse hit tests over the rect vector (rects 0..4 items, 5+ arrows via `HitTest`/`Contains`), hover/select/back sounds, Up/Down wrap over 0..4, Left/Right/Enter -> jump table 0x440870 -> `MenuScreen::Toggle(&global, delta)` 0x438ca0, mouse warp 0x4b6d00 on key moves; draw pass: `Game::UpdateVideo` 0x432a10, BeginScene (vcall 0x6c), `DrawVideo` 0x432a60, title (text colour `[0x508bfc]`, `Font::Print` 0x4a97a0), four `DrawToggle` rows (0x438d80, via table 0x440880) + Back, cursor 0x438f40, `EndScene`/`Present`, `Game::0x435de0` (blit helper), `0x4329e0` (scale pair); loops internally until Back/Esc/window close, then plays the back sound (`Game+0x139304`) and returns -1, otherwise returns `[this+8]` | Run 0x43ffb9 |
| 0x440890 | 2722 | thiscall, SEH (EH 0x4d8e97), **no FPU**, `sub esp,0x38` | **LoadLayout()**: the racemenu.ini reader described above; 13 `new(0x14)` + `UIRect` ctor 0x4390a0 + `vector::insert` (0x4bc680 x11, inline path with 0x403130/0x403140 for the last one), `Config` dtor at the end | Setup 0x43ff71 |
| 0x441340 | 48 | `_$E` stub | guard bit 0x5826d5 + `atexit(0x402c90)` | init table 0xefb58 |

## 3. Types used by the two screens (already declared)
- `MenuScreen`, `MenuLabel`, `UIRect`, `MenuGame` (include/unk_450ee0.h): base (ctor 0x4381c0, dtor 0x438210), `AdjustVolume` 0x438be0, `Toggle` 0x438ca0, `DrawSlider` 0x438700, `DrawToggle` 0x438d80, `DrawCursor` 0x438f40. Used here besides: `HitTest` 0x438f90, `AdjustValue(int*,delta,lo,hi)` 0x438c40 (include/unk_445e80.h), item-rect clear 0x438d00, cell draw 0x438990 (undeclared; args seen: `(MenuLabel* label?/text renderer, float x, float y, int index, int selected, SpriteCell*, int, int)` pushed as ebx(text), x, y, flag, icon, 0, ftol(63.0-1.0)=63, cell) - leave as `FUN_00438990`.
- `HudPoint`, `Sprite` (0xd4, ctor 0x4a8580, dtor 0x4a86d0), `Texture` (0x68, ctor 0x4a8030, `Upload` 0x4a8350), `TextRenderer`, `GameSound` (`FUN_0049b730`), `NetRace` (include/unk_481390.h), `Config` (include/rallytrophy.h: `GetVector2` 0x408390, `Load` 0x406ce0).
- New types needed: `SpriteCell` (0x1c: `Texture*/void* tex; HudPoint uv0, uv1; float w, h;`, implicit ctor 0x43db40 and dtor 0x43db60), `PlayerNameMenu`, `RaceOptionsMenu`. The Game view should name the members `Game+0x14cc` and (manager) `+0x11c`.
- `include/unk_481390.h` (`GameView`/`MenuGame`) offsets worth recording: `Game+0x1390c4` TextRenderer*, `+0x1390bc` second renderer pointer used for scale writes, `+0x1390f8` icon texture pointer, `+0x139224/+0x139294/+0x139304` hover/select/back `GameSound`.

## 4. Corrections / notes for the project docs
- The csv misses 0x43db40, 0x43db60 and 0x43fba0 (add to `functions_extra.csv`, together with the two stubs 0x43f9b0 and 0x441340). Also the earlier stub 0x43d740 (previous TU) is not in this range.
- The hint "0x438xxx functions are MenuScreen methods" holds: all of 0x438c40, 0x438ca0, 0x438d00, 0x438d80, 0x438990, 0x438f40, 0x438f90 are base-class methods taken from here.
- The Game flow name for 0x43c590 (vtable 0x4e17c8, size 0xcf8, members: strings at +0x44.., RaceOptions +0x11c, +0x2a8, SoundMenu +0x750, +0x8dc, +0xa48) is not defined in this range; it is the previous TU and the owner of RaceOptionsMenu.
- `PlayerNameMenu` also holds the in-name cheat codes `KALJAKOPPA` / `QUALIFY`; the unlock globals 0x501498-0x5014d0 are consumed elsewhere (not mapped here).

## 5. Suggested worker groups (sizes in bytes)
Write two source files: `src/unk_43d770.cpp` (+ `include/unk_43d770.h`, file A) and `src/unk_43f9e0.cpp` (+ `include/unk_43f9e0.h`, file B). Each file needs the `_$E` stub pair at its end (compiler-generated, produced by including the STL headers; same pattern as the other modules). Bring the layout facts above into the header first (one agent), then:

| Group | Functions | Bytes | Notes |
|---|---|---|---|
| A1 | 0x43d770 ctor, 0x43db20 (generated), 0x43db40 + 0x43db60 (SpriteCell, generated), 0x43db80 dtor, 0x43de10 Release, 0x43de50 Open | 936 + 30 + 28 + 19 + 239 + 62 + 288 = 1602 | EH frame states 0..5 in the ctor/dtor; `SpriteCell` header must exist first |
| A2 | 0x43dc70 Setup, 0x43f570 SetupLabels, 0x43f8b0 CheckCheatName, 0x43f400 DrawTextCursor, 0x43f740 DrawPlate | 416 + 461 + 241 + 357 + 368 = 1843 | the two ebp-frame draw helpers use `and esp,-8` (8-byte stack alignment because of the `HudPoint` arrays) |
| A3 | 0x43ee70 BuildRects | 1415 | alone; same shape as the previous TU's 1415-byte rect builder at 0x43d1b0 (can borrow its decomp) |
| A4 | 0x43df70 Run | 3806 | alone; big x87/EH function, jump table 0x43ee50 (6 cases); expect NONMATCHING parts |
| B1 | 0x43f9e0 ctor, 0x43faa0 (generated), 0x43fac0 dtor, 0x43ff80 Release, 0x43ffa0 Run, 0x43fba0 Setup | 177 + 30 + 210 + 21 + 41 + 989 = 1468 | the shared Release must be this class's own out-of-line member so ICF folds the others onto it |
| B2 | 0x43ffd0 Frame | 2205 | alone; jump tables 0x440870 / 0x440880 (4 cases each, `switch` on selected item) |
| B3 | 0x440890 LoadLayout | 2722 | alone; 26-key table at 0x4f8f58, 13 UIRects; mostly mechanical call sequence |

Compiler-generated / template-instance functions: **0x43db20, 0x43faa0** (scalar deleting dtors), **0x43db40, 0x43db60** (implicit `SpriteCell` ctor/dtor helpers used as vec-ctor/vec-dtor callbacks), **0x43f9b0, 0x441340** (`_$E` stubs), and inlined `std::string` code inside the ctor/dtor/Setup/Run bodies (no out-of-line vector/string instance is defined in this range; all helper calls go to the shared instances 0x402700 / 0x402960 / 0x402980 / 0x402ab0 / 0x4024f0 / 0x408d10 / 0x408de0 / 0x409500, 0x4bc680 / 0x403130 / 0x403140 / 0x424300 / 0x424320 / 0x424350, 0x4c11f4 / 0x4c1116, 0x4c1276 `operator new`, 0x4bd000 `operator delete`, 0x4bd00b length-error throw, 0x4c19bc `_ftol`, 0x4c1902 `itoa`).

Suggested order: header work (SpriteCell, PlayerNameMenu, RaceOptionsMenu) -> A1 and B1 (lifecycle, small) -> A2/A3 -> B3 -> B2 -> A4 last.
