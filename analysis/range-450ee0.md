<!-- Subagent analysis, 2026-10-23; read-only disassembly; guesses are marked. -->
# Range 0x450ee0 - 0x4563df: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py over the whole range, refs.py for callers/abs refs, a per-function summariser (call targets, .rdata strings, globals, `[reg+off]` histograms) and .rdata reads with the PE section reader. "Guess" marks inference. EH handler addresses (0x4da249 ... 0x4da6a2) are the `push imm` after `push -1` in each SEH prologue. functions.csv is missing one function (0x451040, see section 0, point 3) and mislabels a few conventions (noted per function).

## 0. Headline findings

1. **The range is four files, and only the last one is Car code.** The first three are front-end/Game code that happen to sit before the car-camera file:
   - F1 0x450ee0-0x45237f: the **SoundMenu** screen (options screen "Engine / SoundEffects / Speech / Music / MenuMusic / Comments / Back"; layout from `data/extras/soundmenu.ini`).
   - F2 0x452380-0x45252f: **bonus unlock checks** (`Game::` member-like functions called by the race session at the end of a race; they set bits in the unlocked-cars masks).
   - F3 0x452530-0x452b1f: two **front-end video helpers** of the Game (`PlayVictoryVideo`, `ShowCountryVideo(n)`), using the `BinkVideo` class (rallytrophy.h) and `OverlayImage`.
   - F4 0x452b20-0x4563df: **Car "visual" methods and model loaders** (`Car::UpdateLampPoints`, `Car::UpdateSprings`, `Car::UpdateVisuals(k)`, driver/wiper/rain model loaders). Same `Car` class as `CarObject` in include/unk_45e480.h / `Car` in unk_481390.h.
   File boundaries: the `_$E` guard stubs (`mov cl,[0x5826d5]; mov al,1; test al,cl; jne; or cl,al; mov [0x5826d5],cl; jmp +; push 0x402c90; call atexit; pop ecx; ret`) at 0x450eb0 (previous file), **0x452350, 0x452500, 0x452af0, 0x4563b0** (each 0x30 bytes, init-table entries at file offsets 0xefb70..0xefb80, one per file; abs refs found by refs.py). The next file (Car camera ViewManager) starts at 0x4563e0.
2. **No file-scope statics anywhere in the range** (init table has exactly the five stubs and nothing in between). The only statics are **function-local statics inside two Car functions** (section 5): Vec3 constants with `atexit` registration and a guard byte (0x508c90 bits 1/2/4 in 0x453940, 0x508c6c in 0x454d10). Their dtor thunks are 1-byte `ret`s (0x454cf0, 0x454d00, 0x455060) that are *not* in functions.csv.
3. **Missing from functions.csv: 0x451040** (1008 bytes, vtable slot 1 of SoundMenu, `sub esp,0xc; push ebx; mov ebx,ecx; ...`). functions.csv folds nothing there (0x450f90 has size 175 and ends at 0x45103e); the 18-function count becomes **19 functions + 4 stubs + 3 `ret` thunks**.
4. **SoundMenu is a MenuScreen subclass, not a stand-alone object.** It is embedded in the menu manager (vtable 0x4e17c8) at `+0x750` (size 0x18c; next embedded screens at +0x8dc, +0xa48; ctor call 0x43c612, dtor call 0x43c6db, Run call 0x43cbdc). Its vtable is 0x4e1a7c. The base class `MenuScreen` (ctor 0x4381c0, dtor 0x438210, base vtable 0x4e16dc, 17 derived screens) is outside the range; I used its layout facts only where this range reads them.
5. **The Car methods here are called from Car::Update / PostUpdate (range-459890.md) and the Car ctor (range-45e480.md)**: 0x452b20 from 0x45b9ee (Update), 0x452eb0 from 0x45ba40 (Update, after the physics step 0x45bc20), 0x453940 from 0x45b538 (PostUpdate), and the three loaders 0x455070 / 0x455ec0 / 0x4560a0 from the Car ctor at 0x4605ba / 0x4605cf / 0x4605da (already declared in unk_45e480.h as `FUN_00455070(const char*)`, `FUN_00455ec0(const std::string&)`, `FUN_004560a0(const std::string&)`). They are non-virtual; the Car class has no vtable.

Module table:

| # | Range | Guard stub | Content | Suggested file |
|---|---|---|---|---|
| F1 | 0x450ee0 - 0x45237f | 0x452350 | `SoundMenu` (MenuScreen subclass, 0x18c bytes): ctor, dtors, Setup (slot 1), Run, per-frame Frame, soundmenu.ini layout loader | src/unk_450ee0.cpp |
| F2 | 0x452380 - 0x45252f | 0x452500 | bonus-unlock helpers (3 functions, ~345 bytes) | src/unk_452380.cpp |
| F3 | 0x452530 - 0x452b1f | 0x452af0 | `PlayVictoryVideo`, `ShowCountryVideo` (Bink + overlay) | src/unk_452530.cpp |
| F4 | 0x452b20 - 0x4563df | 0x4563b0 | Car visuals + model loaders (8 functions + 2 statics blocks) | src/unk_452b20.cpp |

## 1. F1: SoundMenu, 0x450ee0 - 0x45237f

### Class and vtable
- Base `MenuScreen` facts seen from this range: `+0x00` vtable, `+0x04` int state (set to 1 by Setup), `+0x08` int selected item (0..6), `+0x0c` `Game*` (stored by Setup from its argument), `+0x10` `std::vector<Button*>`-like (alloc byte at +0x10, begin `+0x14`, end `+0x18`, cap `+0x1c`; element = pointer to a clickable rect, read via `0x4242f0` (`vector::operator[]`), `0x4243c0` (mouse position, `MouseInput::GetPosition`, global 0x56b0c0), `0x4390d0` (point-in-rect test)).
- `SoundMenu` adds from +0x20: **ten 0x24-byte text slots** (std::string at slot+0 with ptr at +4/len at +8; floats at +0x18/+0x1c, an int at +0x20): `[0]` +0x20 (text id 2), `[1]` +0x44 (id 1), `[2]` +0x68 (id 0x5b), `[3..9]` +0x8c, +0xb0, +0xd4, +0xf8, +0x11c, +0x140, +0x164 (ids 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x19b, 0x18). The first two are built by vec-ctor `0x4c11f4(this+0x20, 0x24, 2, 0x430700, 0x430720)`, the last seven by a second `0x4c11f4(this+0x8c, 0x24, 7, ...)`; the +0x68 slot is initialised inline (alloc byte at +0x68, zero +0x6c/+0x70/+0x74), exactly the std::string layout used elsewhere. Slot class ctor 0x430700, dtor 0x430720 (guess name `MenuLabel`).
- `+0x80/+0x84` floats (= width*32/1024 and height*16/768 style scales: `[0x56b0ac]->+4 * 0.0009765625` (0x4e1358) and `->+8 * 0.0013020833` (0x4e1354) times 32.0 (0x4e14c0) and 16.0 (0x4e14bc)); `+0x88` int -1; `+0xa4 + i*0x24` (i = 0..6): 7 rect records initialised by a loop in Setup (float x, float y, int -1 at +8); `+0x180` float = 610.0 (0x4e1a78) x yscale; `+0x188` int/ptr (second argument of Run; non-null -> `call 0x4831c0` on it: per-frame callback object, guess).
- **Vtable 0x4e1a7c**: slot 0 = 0x450f70 (scalar deleting dtor), slot 1 = 0x451040 (**Setup(Game\*)**: builds texts and rects, then calls the layout loader 0x451c10), slot 2 = 0x43ff80 (shared: `if (this[+4]) { this[+4]=0; this[+0xc]=0; jmp 0x438d00 }` = MenuScreen::Release, ICF'd across screens), slot 3 = 0x4414c0 (`ret 4`, shared ICF'd stub, out of range).
- Plain objcmp observation for Setup: its target interval is 1008 bytes, with 10 NOP bytes after `ret 4`; the built function body is 998 bytes. Comparing those bodies leaves 21 differing bytes in the baseline. The differing instructions are x87 operand order and loop FPU stack scheduling. Tested alternatives (reversing multiplication operands, changing scale/local declaration order, and explicit volatile constant reads) were worse or unchanged.
- Object size 0x18c (embedded, never `new`ed: the scalar deleting dtor 0x450f70 is only reachable through the vtable).
- Run (0x451430) is the same code as the neighbouring screens' Run (e.g. 0x43ffa0); signature `int Run(Game* game, void* cb) ret 8`: `[+0x188] = cb; vtbl[1](game); do r = Frame(); while (r != -1); return r`.

### Strings / globals / constants
- `data/extras/soundmenu.ini` (0x4fa778); 18 layout keys read with `Config::GetVec2` (0x408390): `EngineText_UpperLeft/LowerRight` (0x4fa760/0x4fa748), `SoundEffectsText_*` (0x4fa72c/0x4fa710), `SpeechText_*` (0x4fa6f8/0x4fa6e0), `MusicText_*` (0x4fa6cc/0x4fa6b4), `MenuMusicText_*` (0x4fa69c/0x4fa680), `CommentsText_*` (0x4fa668/0x4fa650), `BackText_*` (0x4f62dc/0x4f62c8, shared with other menus), `EngineArrow1_*` (0x4fa638/0x4fa620), `EngineArrow2_*` (0x4fa608/0x4fa5f0). Key table passed to `Config::Define` is at 0x4fa330 (count 0x22 = 34 keys, so 16 more keys than the 18 read here; the other screens' keys live in the same table).
- Settings globals: 0x501438 (Engine volume 0..100, default 80), 0x50143c (SoundEffects 80), 0x501440 (Speech 80), 0x501444 (Music 40), 0x501448 (MenuMusic 80), 0x50147c (Comments on/off, default 1). Item index -> global and handler: items 0..4 `MenuScreen::0x438be0(&global, dir*10)` (slider step 10, `dir` = -1/0/+1 from the key states below), item 5 `0x438ca0(&0x50147c, dir)` (toggle); jump table at 0x451bdc (6 dwords: 0x45188f, 0x4518a3, 0x4518b7, 0x4518cb, 0x4518df, 0x4518f3).
- Input: key-state bytes 0x57ba35 (Enter), 0x57ba43 (Esc), 0x57ba48, 0x57ba4d (Left), 0x57ba4e (Up), 0x57ba4f (Right), 0x57ba50 (Down) (guess of key names from the usage: up/down move selection, left/right adjust), mouse buttons 0x57ba20/0x57ba24, mouse position 0x57ba00/0x57ba04, `57b9f4` (guess: window-closed/quit flag), `ClearKeys` 0x4b7390 on global 0x56b0bc.
- Click sound: `SoundSource::Play` (0x49b730) on `Game+0x139224` (move) and `Game+0x139294` (select) and `Game+0x139304` (back/exit); Game base `[0x56b0c8]`.
- Renderer (global 0x56b0ac) virtual calls: slot 0x84 (state set, args (5,0)), 0xa8 (arg 8), 0x6c (BeginScene, returns bool), 0x98, 0x70 (EndScene), 0x34 (Present). Font `0x4a97a0` (`Font::Print(device, x, y, text, flags, sx, sy)`, unk_481390.h) and `0x4aa430` (`GetWidth`). Localised text lookup `0x405380(id)` (returns `const char*`); ids used: 0x5b..0x60, 0x19b, 0x18, 2, 1, 0x3d.

### Functions

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x450ee0 | 138 | ctor, thiscall, SEH (EH 0x4da249) | `SoundMenu::SoundMenu()`: base ctor `0x4381c0`, vec-ctor 2 x 0x24 at +0x20, init +0x68 string slot, vec-ctor 7 x 0x24 at +0x8c, store vtable 0x4e1a7c, return this. (csv says __fastcall; plain thiscall.) | 0x43c612 (menu manager ctor) |
| 0x450f70 | 30 | scalar deleting dtor (compiler-generated) | `~SoundMenu(); if (flags & 1) delete this` (`0x4bd000`) | vtable slot 0 only (abs ref at file off 0xe027c) |
| 0x450f90 | 175 | dtor, SEH (EH 0x4da2a1) | restores vtable, `0x43ff80` (Release), vec-dtor 7 x 0x24 at +0x8c (`0x4c1116`), inline `~string` of the +0x68 slot (refcount at ptr[-1]), vec-dtor 2 x 0x24 at +0x20, then base dtor `0x438210` | 0x450f73, 0x43c6db |
| 0x451040 | 1008 | **virtual slot 1 `Setup(Game*)`**, no SEH, FPU 30, thiscall `ret 4` | not in functions.csv. `if (this[+4]) return;` computes the two screen scales from `[0x56b0ac]+4/+8`, then ten x { `0x405380(id)`; `strlen`; inline `std::string::assign` (`0x402980` grow + rep movs + `0x402960` eos) } into the ten text slots, sets +0x80/+0x84/+0x88, initialises the 7 rect records at +0xa4 (stride 0x24) in a loop, `this[+4] = 1`, `this[+0xc] = game`, `[+0x180] = 610.0 * scale`, then `call 0x451c10` (layout loader). (`ret 4` at 0x451423: one stack arg, Game*.) | vtable slot 1; called by Run (0x451430) |
| 0x451430 | 41 | thiscall `ret 8`, no frame | `Run(Game*, cb)`: store cb at +0x188, `vtbl[1](game)`, then `do { r = Frame(); } while (r != -1)`; returns r. Same shape as 0x43ffa0 (ICF did not merge them because of the different Frame callee). | 0x43cbdc |
| 0x451460 | 1915 | **Frame()**, thiscall, SEH (EH 0x4da318), 0x74 locals, FPU 44 | one iteration of the options menu loop: renderer state calls, `ClearKeys`, `FUN_004b69f0` message pump (exit when it returns 0), play the "hover" sound when the selection changes, keyboard (up/down wrap 0..6, left/right via jump table -> 0x438be0 / 0x438ca0), mouse hit-test over the 7 rects (`0x4243c0` point, `0x4242f0` rect, `0x4390d0` test, mouse-driven selection and click handling with `0x57ba00/04`), Escape/Back -> exit; draw: `0x432a10`, `BeginScene`, `0x432a60`, `Font::Print` of the labels (`0x4a97a0` x2 here), `0x435de0`, `0x4329e0`, `0x438700`, `0x438d80`, `0x435b60`, `0x438f40`, `EndScene`/`Present`; `Game::0x432c00` per-frame update at the loop head; returns `-1` when leaving (after a "back" click sound) else the selection. The 6-dword jump table at 0x451bdc follows the function (code boundary: function ends at 0x451bdb). | 0x451449 (Run) |
| 0x451c10 | 1848 | thiscall (this in ecx, csv says __stdcall), SEH | **LoadLayout**: `Config cfg` (ctor 0x405580), `Define(key table 0x4fa330, 0x22)` (`0x4076c0`), `std::string path = "data/extras/soundmenu.ini"` (inline `append`), `Load(path, ...)` (`0x406ce0`), then 18 x `GetVec2` (`0x408390`) stored into the ten slots/rects through `0x4c1276`-new'd temporaries and vector `0x4bc680` inserts, `0x424300` / `0x4390a0` / `0x403140` helpers (each pair UpperLeft/LowerRight becomes one `Button` rect pushed onto the base vector at +0x14); no throw path in this function; `~string`, `~Config` (0x4055a0). | 0x45141a (Setup) |
| 0x452350 | 48 | `_$E` stub | module end | init table |

Decompile: the 18-key GetVec2 block of 0x451c10 is mechanical (same 9-instruction block repeated), 0x451460 is the hard one.

## 2. F2: bonus unlock helpers, 0x452380 - 0x45252f

Three functions on one tiny data set: bit masks `0x5014cc` (normal) and `0x5014d0` (when race option `[0x501458] != 0`, the same global that selects the draw routine in unk_481390.h), one-shot byte flags `0x5014c8` and `0x5014c9`, and the difficulty class `0x50132c` (0 / 1 / 2 as in unk_481390.h `DAT_0050132c`). Called with `ecx = [0x56b0c8]` (Game) from the race session end-of-race code, so they are `Game::` members that never touch `this` (compiler kept `ecx` unused; treat as `thiscall` on Game, `ret 8`, or as plain `__stdcall`; the caller loads ecx in both cases).

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x452380 | 130 | thiscall/stdcall `ret 8` `(int position, Player* p)` returns bit flags | `r = 0`; by difficulty: 0 -> position == 1, 1 -> position <= 2, 2 -> position <= 3: `if (MarkCarUnlocked(p->+0x2c)) r = 1` (`Player+0x2c` = car model, range-45e480.md); then `if (!byte[0x5014c9] && difficulty == 1) { r |= 2; byte[0x5014c9] = 1 }`; returns r. | 0x4943d5 (race session, when `[ebx+0x1a]`) |
| 0x452410 | 82 | **cdecl** `(int model)` returns 0/1 | `bit = (1 << ((model - 1) & 0xfffe)) << 1`; mask = `[0x501458] ? &0x5014d0 : &0x5014cc`; if already set return 0, else set and return 1. | 0x4523b2, 0x4523cf, 0x45249f, 0x4524b8 |
| 0x452470 | 133 | thiscall/stdcall `ret 8` `(int position, Player* p)` | same as 0x452380 but with thresholds 0 -> position == 1, 1 -> position <= 3, 2 -> position <= 6 and the one-shot flag is `byte[0x5014c8]` guarded by `[p+0x1a5c] <= 6` (Player+0x1a5c: finishing position, guess) | 0x4944b2 |
| 0x452500 | 48 | `_$E` stub | module end | init table |

(The `5826d5` reference counted inside 0x452470 by the summariser is the 0x452500 stub that follows it.) Decompile: one worker, 3 functions + stub; the two big functions differ only in the thresholds and flag; take care to give the exact same compare chain order (`sub eax,esi; je; dec eax; je; dec eax; jne` = `switch (difficulty)` with cases 0, 1, 2).

## 3. F3: front-end video helpers, 0x452530 - 0x452b1f

Both use `BinkVideo` (0xac bytes; rallytrophy.h: ctor 0x4097a0 `(name, RenderDevice*, flags)`, `Update()` 0x40a4a0 (-1 done, 1 new frame), `Render(device)` 0x40a0f0, dtor 0x40a060), the renderer (global 0x56b0ac, slots 0x84/0xa8/0x6c/0x70/0x34 as in F1), the message pump `FUN_004b69f0`, `ClearKeys` (0x4b7390) and the key/mouse globals 0x57ba20/24/35/43/48.

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x452530 | 423 | cdecl, no args, SEH (EH 0x4da40b), csv says __stdcall | **PlayVictoryVideo()**: `new BinkVideo("data/video/victory.bik" (0x4fa7f4), renderer, 1)` (`new 0xac`, ctor 0x4097a0), renderer state calls (2,1), (5,0), (13,0), (8), then pump loop: any key/click (`0x57ba43`, `0x57ba35`, `0x57ba48`, `0x57ba20`) sets a skip flag; `Update()` == 1 -> `Render` + `EndScene` + `Present`; Update returns other than -1/1 -> `0x4a18d0(500)` (frame limiter, microseconds); ends when Update == -1 or skipped; `~BinkVideo` (0x40a060) + delete; `ClearKeys`, clears 0x57ba20/0x57ba24. | 0x433b60, 0x4342a2 (Game flow, 0x433xxx) |
| 0x4526e0 | 1040 | thiscall `ret 4` `(char n)`, SEH (EH 0x4da436), FPU 38 | **ShowCountryVideo(char n)**: copies the 24-byte template `"data/video/country1.bik"` (0x4fa82c, 6 dwords) to the stack and patches the digit with `n + '0'` (index 17); `new BinkVideo(path, renderer, 0)`; `new OverlayImage("data/menu/ImageCountryVideo.tga" (0x4fa80c), renderer, 1, 1)` (0x4a8ae0, then `SetGrid(5, 6)` 0x4a9280); positions the two menu fonts `[Game+0x1390c4]` and `[Game+0x1390c0]` (`+0x1c/+0x20` x/y scale, `+0x120` colour 0xffffffff / 0xff000000, `+0x130` flag, `+0x134/+0x138` size); loads captions with `0x405380(0xb4 + n)` and `0x405380(0xb9 + n)` and `0x405380(0x117)` (the continue prompt); loop like 0x452530 but each frame draws the overlay (`OverlayImage::Draw` 0x4a92c0) and `Font::Print` (0x4a97a0 x3) with `Font::GetWidth` (0x4aa430) to centre the text; cleanup `~OverlayImage` (0x4a8bb0), `~BinkVideo`, `ClearKeys`. | 0x4337f7, 0x43405d (Game flow) |
| 0x452af0 | 48 | `_$E` stub | module end | init table |

Decompile: one worker, 2 functions; 0x4526e0 first for the font/overlay field usage (Game+0x1390c0 / 0x1390c4 are two `Font` objects, `Game+0x1390c4` the larger one).

## 4. F4: Car visuals and model loaders, 0x452b20 - 0x4563df

All are `Car` members (`this` = Car). Offsets are from the Car layout of unk_45e480.h; new facts from this range are listed after the table.

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x452b20 | 897 | thiscall (no stack args), `ebp` frame, SEH (EH 0x4da450), FPU 166 | **Car::UpdateLampPoints()** (guess name): four times: local point `p` (for the first: Vec3 (0, 0, body+0xb4); then (`Car+0x20d0..0x20d8` + `Car+0x1018..0x1020`), (`Car+0x203c..0x2044` + `Car+0x1024..0x102c`) and one more) is rotated by the body 3x3 at `[Car+0x24]+0x20` (fully inlined 9-multiply matrix product, `[ebp-0x1c/-0x18/-0x14/-0x10]` pointers), added to the body position `body+0x14`, then `0x4204f0(body, &out, &in)` (world-to-body velocity of a point) gives a Vec3 written to `Car+0x13a8`, `+0x13b4`, `+0x13c0`, `+0x13cc` (these are the 12 control vectors listed in range-45e480.md). Ends the Update prologue before the physics step. | 0x45b9ee (Car::Update) |
| 0x452eb0 | 2693 | thiscall `ret 4` `(float dt)`, `ebp` frame 0x50 locals, SEH (EH 0x4da4b8), FPU ~559 | **Car::UpdateSprings(dt)** (guess): reads body velocity (`body+0x1cc..0x1d4`), speed^2 (thresholds 0x4e1ae8 / 0x4e13a4), uses the same local-to-world products to get four body-frame acceleration vectors, scales them (`0x40b0c0` Vec3*float with 0.5 (0x4e138c) and the constant at 0x4e1ae0), stores into `Car+0x13d8 .. 0x1410` (stride 0xc, 5 more vectors) and `Car+0x139c..0x13a4`; feeds the two `SpringFollower::Update(a, b, dt)` calls (`0x46d740`) at `Car+0x1f4c` (0x4537ab) and `Car+0x1f30` (0x45391d) with the gain 0x4e1398; also reads `Car+0x12bc/0x12c8/0x12d4/0x12e0` (4 Vec3-like), `Car+0x1c730/0x1c734` (two ints, recorder/state). Also calls `0x414330` x4 (Vec3 transform/sub). | 0x45ba40 (Car::Update) |
| 0x453940 | 5036 | thiscall `ret 4` `(float k)` with `ebp` frame, 0x11c locals, SEH (EH 0x4da534), FPU ~821 | **Car::UpdateVisuals(k)** (guess; k = 1/dt or dt, caller in PostUpdate): (a) wiper animation when `Car+0x1ffc` and object `Car+0x1ff8` exist: phase `Car+0x2000 += k * Car+0x1300`, wrap via `fmod` (0x4c1404), wiper angle interpolated between `Car+0x130c/0x1310` and `0x1314/0x1318` (two angle ranges), `0x4021b0` (set angle on the 0x20-byte wiper object), `0x476fa0` on the rain emitter `Car+0x2004`, `0x46e2d0` on object `Car+0x2008`; (b) hood/trunk flaps when `Car+0x1f6c`: spring-follower driven rotation (`Car+0x1f3c`, `0x1f44`, `0x12f8`), builds rotation matrices with `0x4b5760` / `0x4b55e0` and concatenates with `0x4b5540` (matrix multiply) into `Car+0x1f70` (hood) / `Car+0x1fb0` (trunk); (c) function-statics init (section 5); (d) interior/driver animation: reads `Car+0x2020/0x2024`, `Car+0x2048..0x207c`, `0x2098..0x20cc` (driver, co-driver, arms, transmission lever), `Car+0xfe8..0xff0`, `Car+0xdb8` (Drivetrain, gear/rpm), `Car+0x1340` (handbrake) and `Car+0x134c` (steering) and `rand` (0x4c1998) / `pow` (0x4c19f0) for engine shake; lamp objects through `CarLight::Show` (0x4b2b90) and `AttachTo` (0x4b2b30), `0x46b1d0`, `0x46ba60`; (e) tail: `call 0x454d10` (see below) then `ret 4` at 0x454ce9. Callees of note: `0x414330` x2. Reads `Car+0x1bcac`, `Car+0x1bcc8/0x1bccc`, `Car+0x1befc` (flags/state from the Car flags word `Car+0x1bf00` region, see range-45e480.md). | 0x45b538 (Car::PostUpdate) |
| 0x454cf0 | 1 | `ret` (compiler-generated, ICF'd shared empty dtor) | atexit callback for the function static at 0x508c70 | atexit arg at 0x453ccf |
| 0x454d00 | 1 | `ret` (compiler-generated) | atexit callback for the function static at 0x508c80 | atexit arg at 0x453c94 |
| 0x454d10 | 843 | **fastcall** (ecx = Car, no args, no SEH), `ebp` frame, FPU 185 | **Car::UpdateInteriorTilt()** (guess): reads Vec3 `Car+0x1408..0x1410` (last of the 12 control vectors), normalises when `|v|^2 > 0x4e1b0c`, clamps components against `0x4e164c`, combines with `Car+0x1024..0x102c` (car.ini vector) and writes the Vec3 results at `Car+0x2028..0x2044`; uses the function static Vec3 (0,0,1) at 0x508c60 (guard 0x508c6c) as the reference axis; 1 `atexit` registration (`push 0x455060`, `call 0x4c0d01`) | 0x454b7b (end of 0x453940) |
| 0x455060 | 1 | `ret` | atexit callback for the static 0x508c60 | atexit arg at 0x454f84 |
| 0x455070 | 3651 | thiscall `ret 4` `(const char* driverDir)`, SEH (EH 0x4da62f), 0x1a0 locals, **no FPU** | **Car::LoadDriver(dir)** (existing decl `FUN_00455070(const char*)`): clears the six driver Vec3 blocks `Car+0x20c4..0x20d8` and `Car+0x2030..0x2044`, builds `std::string path = dir + "driver.bmf"` (inline `append` 0x403ba0 / `0x402980` / `0x402960`), loads a model container (`ModelFile` ctor `0x4af950(path-or-empty-string 0x4e1344, dir, cache = Car+0xfa4, list = Car+0xfac)` ) then walks its meshes: for each mesh compare `mesh->name (+0x118)` with `strcmp` (0x4ce290) against **driver_body / driver_head / co_driver_body / co_driver_head** (0x4e1aa0, 0x4e1aac, 0x4e1ab8, 0x4e1ac8); each hit `new 0x12c` (Object, ctor `0x4a6910`) and stores it in the Car; repeats for **co_driver.bmf** (0x4fa8b4), **arm_left_lo.bmf** (0x4fa8a4), **arm_left_hi.bmf** (0x4fa894), **arm_right_lo.bmf** (0x4fa880), **arm_right_hi.bmf** (0x4fa86c), **transmission_lo.bmf** (0x4fa858), **transmission_hi.bmf** (0x4fa844) with `0x4a6050` (file exists test, `0x4e1344` = empty string constant when the std::string is empty), `0x4c1276` (new), `0x401c10` / `0x401cf0` (mesh container ctor / load), `0x4afe20` (ModelFile dtor), `0x4bd00b` (`_Xlen`/throw helper) and an inline `~string` (refcount at ptr[-1], delete via `0x4bd000`). Five `new`/`delete` pairs and `0x402aa0/0x402ab0` helpers appear once each. | 0x4605ba (Car::Car) |
| 0x455ec0 | 468 | thiscall `ret 4` `(const std::string& dir)`, SEH (EH 0x4da664), 0x94 locals | **Car::LoadWipers(dir)** (existing decl `FUN_00455ec0`): `path = dir + "wipers.bmf"` (0x4fa8d0), `0x4a6050` exists check (return when missing), `ModelFile(path, dir, Car+0xfa4, Car+0xfac)` (`0x4af950`), `new 0x20` (ctor `0x401c10`, wiper mesh object) stored at `Car+0x1ff8`, `0x401cf0` load, for each loaded child `vtbl[0](1)`, `Object::AttachTo(Car+0x24)` (`0x4b2b30`, sets bit 1 in `[obj+0xc]+0xc`), `~ModelFile` | 0x4605cf |
| 0x4560a0 | 769 | thiscall `ret 4` `(const std::string& dir)`, SEH (EH 0x4da6a2), 0xa0 locals | **Car::LoadRain(dir)** (existing decl `FUN_004560a0`): `path = dir + "rain_drop.bmf"` (0x4fa8dc), exists check, `ModelFile(...)`, `new 0x240` particle emitter (ctor `0x4766d0(0x80)`, stored at `Car+0x2004`; its `0x476b70(a, pos, ...)` is called twice with the wiper/rain geometry `Car+0x1304..0x1310`, `Car+0x131c`, `Car+0x1328` when `Car+0x12fc > 0`), `0x476be0(modelFile)`, `AttachTo(body)` (`0x4b2b30`), `new 0x300` object (ctor `0x46d830(0x20)` stored at `Car+0x2008`) with `0x46dd80(modelFile)`, `0x4b2b30`, `0x46e1a0(body, Car+0x1f6c, Car+0x1f68)`; frees the ModelFile children | 0x4605da |

| 0x4563b0 | 48 | `_$E` stub | module end | init table |

Evidence `ev_d90cf4ae7ba2746e90ef75f080c42f4c19004b4fd020a62c3ac70f31c58ab0a0` records the 769-byte LoadRain body. Source-shape attempts to alter the inline append destination expression and expose `dir.c_str()` through a local pointer did not change codegen; after reverting, `objcmp` still reports 11 differing bytes in two register-scheduling clusters (9 in the append copy and 2 before the `ModelFile` call). Exact matching remains open.

Evidence `ev_f9908a4dcee49a085119f23bda24b43ed7b34008c95dafec5ea12b1258c0e250` records the 3,651-byte LoadDriver body. The five hand-expanded string append sites each differ only in the commutative base/index order of two LEAs (10 bytes total); source changes swapping `m + len` to `len + m`, `ptr + len` to `len + ptr`, and introducing a temporary destination pointer produce identical code. Baseline restored; focused matcher still reports the function as NONMATCHING with all EH states intact.

New Car fields seen in F4 (offsets in Car; see unk_45e480.h for the rest):
- `+0x12f8` float (flap/wiper gain), `+0x12fc` int (rain-drop count), `+0x1300` float (wiper speed), `+0x1304..0x1310` rain/wiper geometry floats, `+0x130c/+0x1310` and `+0x1314/+0x1318` float pairs (wiper angle ranges), `+0x131c`, `+0x1328` Vec3-like blocks (rain emitter positions).
- `+0x1ff8` wiper object (0x20 bytes, `Object` 0x401c10), `+0x1ffc` int (wiper present flag), `+0x2000` float wiper phase 0..0.5, `+0x2004` rain emitter (0x240 bytes), `+0x2008` rain/overlay object (0x300 bytes, ctor 0x46d830), `+0x2014/+0x2018/+0x201c/+0x2020/+0x2024` interior objects/flags, `+0x1f68/+0x1f6c` hood/trunk part pointers, `+0x1f70/+0x1fb0` hood/trunk matrices (already in the layout).
- `+0x20c4..0x20d8`, `+0x2030..0x2044`, `+0x2048..0x207c`, `+0x2098..0x20cc`: driver/co-driver/arms/transmission bookkeeping (Vec3 blocks and Object pointers), zeroed by `LoadDriver`.
- `+0x1c730/+0x1c734`: two ints read by `UpdateSprings` (state of the CarRecorder/replay at the end of the object; the Car is 0x1f30c bytes).

## 5. Function-local statics

| Function | Static | Address | Init | atexit |
|---|---|---|---|---|
| 0x453940 | `static float` capturing `Car+0xfe8` at first call | 0x508c8c | bit 1 of guard 0x508c90, `mov ecx,[ebx+0xfe8]` | none |
| 0x453940 | `static Vec3 up = (0, 1, 0)` | 0x508c80 | bit 2 | `atexit(0x454d00)` |
| 0x453940 | `static Vec3 forward = (0, 0, 1)` | 0x508c70 | bit 4 | `atexit(0x454cf0)` |
| 0x454d10 | `static Vec3 axis = (0, 0, 1)` | 0x508c60 | guard 0x508c6c | `atexit(0x455060)` |

(Init of the statics sits in the middle of 0x453940 at 0x453c72-0x453cff, i.e. after the first matrix products, not at function entry; the match needs the statics declared inside the function bodies at that point.)

## 6. Compiler-generated / out-of-line template items

- 0x450f70: scalar deleting destructor of SoundMenu (compiler-generated, vtable slot 0).
- 0x454cf0, 0x454d00, 0x455060: 1-byte `ret` dtor callbacks for the Vec3 function statics (never called by name; not in functions.csv).
- Stubs 0x452350, 0x452500, 0x452af0, 0x4563b0.
- No STL template instance bodies are defined in this range. `std::string` appends/assigns are inlined and call the shared out-of-line `_Grow` 0x402980 / `_Eos` 0x402960 / `_Tidy` 0x402700 / `append` 0x403ba0 / `0x4024f0` / `0x4bd00b` (`_Xlen`) instances. No 5- or 6-byte jump thunks.
- Inlined helpers: the 3x3 matrix times vector product in 0x452b20 and 0x452eb0 is hand-inlined (`vec3 * mat3x3 + translation`, 9 multiplies); matches the `Matrix3::Transform` seen in other Car functions.
- Shared bare `ret`/`jmp` ICF targets referenced: slot 2 `0x43ff80` (MenuScreen::Release), slot 3 `0x4414c0` (`ret 4`).

## 7. Suggested decompile work groups (merge in address order)

Sizes are bytes of code; "hard" means FPU-heavy or large control flow.

| Group | Functions | Bytes | Notes |
|---|---|---|---|
| **A: SoundMenu lifecycle** | 0x450ee0, 0x450f70, 0x450f90, 0x451040, 0x451430 | 138 + 30 + 175 + 1008 + 41 = 1392 | declare `class SoundMenu : MenuScreen` with 10 text slots; 0x451040 first (it fixes the slot layout), 0x450f70 is a compiler-generated scalar deleting dtor (do not write it by hand) |
| **B: SoundMenu::Frame** | 0x451460 | 1915 | hard; alone; needs the jump table (6 cases) and the rect-hit loop |
| **C: soundmenu.ini loader** | 0x451c10 | 1848 | mechanical (18 identical GetVec2 blocks) but long; alone |
| **D: unlock helpers** | 0x452380, 0x452410, 0x452470 | 345 | tiny; one worker; the `switch` chains must keep compare order; file-end stub 0x452500 |
| **E: videos** | 0x452530, 0x4526e0 | 1463 | one worker; BinkVideo/OverlayImage/Font declarations exist (rallytrophy.h, unk_481390.h) |
| **F: Car lamp points + interior tilt** | 0x452b20, 0x454d10 | 897 + 843 = 1740 | FPU, inlined matrix products; 0x454d10 has a function static, 0x452b20 starts the Car visuals file (declare before 0x453940) |
| **G: Car springs** | 0x452eb0 | 2693 | hard FPU, `ebp` frame with temporaries (EH state byte 0/2); alone |
| **H: Car::UpdateVisuals** | 0x453940 | 5036 | very hard (821 FPU insns, three function statics); alone, ideally split by the four sections (a)-(d) after one pass to name the Car fields; calls 0x454d10 last |
| **I: LoadDriver** | 0x455070 | 3651 | no FPU, std::string-heavy, big SEH state table; alone |
| **J: wiper and rain loaders** | 0x455ec0, 0x4560a0 | 468 + 769 = 1237 | same pattern (`dir + "x.bmf"`, ModelFile, new Object, AttachTo); one worker; do 0x455ec0 first |

Order of merge into files: F1 = A, B, C (+ stub 0x452350); F2 = D (+ 0x452500); F3 = E (+ 0x452af0); F4 = F, G, H, I, J interleaved by address (0x452b20, 0x452eb0, 0x453940, 0x454d10, 0x455070, 0x455ec0, 0x4560a0, stub 0x4563b0). Because function-local statics need the guard bytes placed by the compiler, F4 should compile in one translation unit.

## 8. Corrections for project docs and cross-links
- COORDINATION.md row "Code before the car camera 0x450ee0-0x4563df": it is **SoundMenu + unlock helpers + front-end videos + Car visuals/loaders**, not Car-only. modules.csv: add four modules at 0x450ee0, 0x452380, 0x452530, 0x452b20 (and 0x451040 as a function).
- unk_481390.h already declares the three Car methods (`FUN_00452b20()`, `FUN_00452eb0(float dt)`, `FUN_00453940(float k)`); `FUN_00454d10` is a new fastcall helper (no args).
- unk_45e480.h already declares `FUN_00455070(const char*)`, `FUN_00455ec0(const std::string&)`, `FUN_004560a0(const std::string&)`; their real names are LoadDriver / LoadWipers / LoadRain.
- `Player+0x1a5c` is read by 0x452470 (finishing position, guess); `Game+0x139224/0x139294/0x139304` are three SoundSource slots (move/select/back clicks) in the Game (stride 0x70, see range-49b630.md section 1).
- The shared global 0x56b0c8 is the Game object (also called with `ecx = [0x56b0c8]` for the unlock helpers).
