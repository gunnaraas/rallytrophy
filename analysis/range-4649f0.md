<!-- Subagent analysis, 2026-10-23; read-only disassembly; guesses are marked. -->
# Range 0x4649f0 - 0x46669f: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py (nops stripped), refs.py for callers/abs refs, .rdata/.data reads with a PE section reader, the init table in .data (file offset 0xefba8; entries 0x45da90, 0x45de60, 0x45e450, 0x4649c0, **0x464ab0, 0x464b00, 0x464b50, 0x464b90, 0x464bd0, 0x4653c0, 0x4654c0, 0x465e60, [0x466670]**, then the stubs of the 0x4666a0 range). "Guess" marks inference. EH handler addresses are the `push imm` after `push -1` in each SEH prologue.

## 0. Headline findings

1. **Every function in the range is a member of `Car`** (or a helper of its two networking/setup files) except the static-initialiser machinery and the three non-member helpers 0x464ea0 (reads globals), 0x465d50 and 0x465e40 (cdecl clamp/scale helpers). Four stubs (0x4653c0, 0x4654c0, 0x465e60, 0x466670) match the lead's four-file split.
2. **Boundary correction (important for matching).** The first six functions, 0x4649f0, 0x464a50, 0x464a70, 0x464a90, 0x464aa0 (and the two empty `ret` dtors 0x464af0 / 0x464b40), are **compiler/inline functions used by the Car constructor/destructor file (0x45e480-0x4649bf)**, placed after that file's `_$E` stub (0x4649c0) in the usual "late COMDAT" position:
   - 0x464a70 is an element ctor used as vec-ctor callback in the Car ctor (abs refs from 0x45e6f3, 0x45e775, 0x45e793, 0x45e7b1, 0x45e7d9: array(s) of 0x1c-byte records at Car+0x1bd40 etc., `push 0x1c` at 0x45e731).
   - 0x464a90 / 0x464aa0 are the implicit ctor/dtor of the 0x8c-byte `SoundLayer` wrapper (SoundSource at +4, ctor call `0x49b630`, dtor `jmp 0x49b6e0`); used as vec-ctor/dtor callbacks at 0x45e86e/0x45e873 (`push 0x464aa0; push 0x464a90; push 4; push 0x8c; lea eax,[ebx+0x1c2b4]`), in the Car dtor (0x461e98...) and in 0x4dafca/0x4db6e1 (EH unwind tables).
   - 0x464a50 is `Car::` inline (`unk1c908=0; unk1c90c=-60000`) called once from the Car file (0x4619a7).
   - 0x4649f0 has **no callers** (no call, jmp or absolute reference anywhere). It is an unused implicit dtor of a 0x90-byte aggregate (`float`-sized x4 at +0, 8-byte x16 at +0x10, both through `0x4c1116` with the empty dtor 0x402c90).
   So the first file that really starts in the range begins at **0x464ab0** (first static initialiser). The ctor/dtor 0x4653f0 / 0x465460 are likewise implicit COMDAT members (SkidFrustum) positioned after file 1's stub. If you keep the lead's four-way split, treat 0x4649f0-0x464aa0 as "belongs to the Car ctor/dtor file; compile there, not here".
3. **Four source files** (stub = last 0x30 bytes of the file; guard bit 0x5826d5 + `atexit(0x402c90)`), by the table above:

| # | Range | Stub | Content | Suggested file |
|---|---|---|---|---|
| F1 | 0x464ab0 - 0x4653bf (+COMDATs 0x4653f0, 0x465460) | 0x4653c0 | **rear-view mirror**: 5 file-scope statics (two Vec2 consts, DrawContext, ViewManager, SkidFrustum), mirror camera setup 0x464c10, mirror visibility test 0x464ea0, mirror scene render 0x464ef0 | src/unk_464ab0.cpp |
| F2 | 0x4654c0 only (0x30-byte stub) | 0x4654c0 | **empty module** (stub only; same pattern as 0x4a16a0..0x4a1730 in range-49b630.md). The SkidFrustum ctor/dtor at 0x4653f0/0x465460 sit in front of it and belong to F1 (lead's alternative view: F2 = those two functions + stub; bytes are identical either way). | nothing (or fold into F1) |
| F3 | 0x4654f0 - 0x465e5f | 0x465e60 | **replay/ghost recorder glue and network state packing**: `SetReplayMode`, replay file load/save, `GetState` / `SetState` (CarState, 0x57 bytes), `GetState2` / `SetState2` (CarState2, 0x14 bytes), byte clamp helpers | src/unk_4654f0.cpp |
| F4 | 0x465e90 - 0x46669f | 0x466670 | **Car setup**: `LoadTyreDynamics(set)` (data/global/dynamics/tire<N>...dynamics.ini, 30 surfaces x 9 keys) and the four tiny setup setters (gearing, suspension, balance, steering scale) | src/unk_465e90.cpp |

   F3 and F4 both start immediately after a stub (0x4654c0 resp. 0x465e60). Next file after the range starts at 0x4666a0 (Car sounds).
4. **No vtables, no virtual functions, no scalar deleting dtors.** The only data refs to functions in the range are the init-table entries (stubs and static-initialiser wrappers), the vec-ctor/dtor callbacks, and the jump table at 0x4665f8. Indirect calls are only calls *out* (renderer 0x56b0ac slots, RaceMode 0x501450 slots 22/23/24).
5. **No STL template bodies** are defined in the range. F4 uses `std::string` (VC6 layout, inlined `assign`/`append`) via out-of-line `_Grow 0x402980`, `_Eos 0x402960`, `_Tidy 0x402700`, `append 0x403ba0`, `operator+ 0x409130`, `0x4024f0` (see unk_4666a0.h `TmpString`/`NameString`).
6. **File-scope statics: five, all in F1** (see section 1). They are the only statics; F3/F4 have none (but F4 uses the global scratch buffer 0x5093f8 as an `itoa` target, guess: `static char s_buf[]` in F4 or a global in this region).
7. Jump table: `0x466590` uses a 5-entry table at 0x4665f8 (dwords 0x4665a3, 0x4665af, 0x4665c7, 0x4665d3, 0x4665eb) right after the function; rawdis prints it as garbage instructions. `0x466610` builds a 5-float local table (`{0.75, 0.875, 1.0, 1.125, 1.25}` as immediates on the stack, indexed `[esp+arg*4]`).

## 1. F1: rear-view mirror, 0x464ab0 - 0x4653bf (+0x4653f0, 0x465460)

### 1.1 Statics (init order = init-table order = declaration order; addresses in .data tail / bss)

| Init wrapper (`call X; jmp Y`) | Ctor X / atexit Y | Object | Class | Notes |
|---|---|---|---|---|
| 0x464ab0 | 0x464ac0 / 0x464ae0 (registers empty dtor 0x464af0) | 0x5093f0 (8 bytes) | 2-float struct with user ctor+empty dtor ("Vec2" with ctor, guess `Vec2(352.0f, 8.0f)`) | ctor stores 352.0 (0x43b00000), 8.0 (0x41000000) |
| 0x464b00 | 0x464b10 / 0x464b30 (empty dtor 0x464b40) | 0x5093e8 (8 bytes) | same shape, `(320.0f, 80.0f)` | 0x43a00000, 0x42a00000 |
| 0x464b50 | 0x464b60 `mov ecx,0x509250; jmp 0x4b7a40` / 0x464b70 (registers 0x464b80 -> `jmp 0x4b7bf0`) | 0x509250 | **DrawContext** (ctor 0x4b7a40, 0x194 bytes, see unk_481390.h) | ends 0x5093e4 |
| 0x464b90 | 0x464ba0 `jmp 0x4b77b0` / 0x464bb0 (0x464bc0 -> `jmp 0x4b78e0`) | 0x509080 | **ViewManager** (ctor 0x4b77b0, dtor 0x4b78e0, 0x1d0 bytes) | ends 0x509250 |
| 0x464bd0 | 0x464be0 `jmp 0x4653f0` / 0x464bf0 (0x464c00 -> `jmp 0x465460`) | 0x508f68 | **SkidFrustum** (0x118 bytes; header unk_4741a0.h; planesA[6], planesB[6] at 0x00/0x60, view ptr at 0xc0) | ends 0x509080. A second static of the same type is initialised in the 0x4b7xxx file (0x4b7c50: `mov ecx,0x57bc88; jmp 0x4653f0`), which is why the ctor/dtor bodies exist only once. |

(The two Vec2 values are the **mirror rectangle in 1024x768 units**: x=352, y=8, w=320, h=80 -> centred at the top. 0x464ef0 multiplies them by the screen scale globals 0x56aea8 / 0x56aeac.) Static order in the source (guess): Vec2 A, Vec2 B, DrawContext, ViewManager, SkidFrustum.

Addresses 0x5090a0-0x509118, 0x5091a4-0x5091ac, 0x5093c4-0x5093e0 are **fields of those statics** (ViewManager +0x14 position, +0x20.. matrix, +0x60.. second matrix, +0x124/+0x128/+0x12c near/far/fov-like params; DrawContext +0x174..+0x190 copied from the Stage). Do not declare them as separate globals.

### 1.2 SkidFrustum ctor/dtor (implicit, compiler generated)

- 0x4653f0 (98 B, SEH EH 0x4db9c3): `__vec_ctor(this, 0x10, 6, 0x490ad0, 0x402c90)` then the same for `this+0x60`. Plane element = 16 bytes (4 floats; `SkidPlane` in unk_4741a0.h), empty element ctor/dtor 0x490ad0/0x402c90.
- 0x465460 (94 B, SEH EH 0x4db9e3): `__vec_dtor(this+0x60, 0x10, 6, 0x402c90)` then `this`.
Callers: only the two static-init wrappers (0x464be5/0x464c05) and the other file's static (0x4b7c55/0x4b7c75).

### 1.3 Mirror functions (Car members; Car+0x24 = CarBody*, Car+0x14fc = Stage*)

Common structure of 0x464c10 and the head of 0x464ef0 (the second is a superset): sets the mirror projection constants in the ViewManager static (0x5091ac = pi/2 (0x3fc90fdb), 0x5091a4 = 1.0, 0x5091a8 = 100.0), builds the camera from the **car body**: position = body->0xb8..0xc0 / 0xb4 rotated through the body matrix (body+0x60, 3x3 + translation at +0x30..+0x38) after adding `[body+0xb0]*0.85 (0x4e14dc)` and `[body+0xb4...]*0.15 (0x4e1e88)` to the y offset, then copies two matrices (body+0x20.. and +0x60..) and a cross product (`0x4203b0` Vec3 helper) into the ViewManager statics. Renderer calls (device = global 0x56b0ac): slot 0x84 (SetRenderState(id, val)), 0xc0 (SetViewMatrix(ptr, 1)), 0xcc (SetProjection(fov, 1.0, near, far, 1)), 0x38/0x3c (SetViewport / GetViewport 4 values), 0x60, 0x80.

Functions:
- **0x464c10 `Car::PrepareMirror(ViewManager* view)`** (thiscall, `ret 4`, SEH EH 0x4db978, `ebp` frame, FPU): fills the mirror camera statics, programs the device with the mirror view/projection, `SkidFrustum::Init(device)` (0x4ba620) on 0x508f68, then **restores** the caller's view: slot 0xc0 with `view+0x60` and slot 0xcc with `view+0x12c, 1.0, view+0x124, view+0x128`. Caller: 0x493fa4 (the per-frame race loop body 0x493650, the main-view sibling `0x4b7c80(ctx...)` is called at 0x493f9b).
- **0x464ea0 `bool MirrorVisible(Object* o)`** (stdcall-style `ret 4`, no frame): `if (DAT_00501308 && DAT_0050146c && !(o->flags[+0xc] & 3)) return SkidFrustum(0x508f68).Test(o+0xa0 (sphere), o+0x60)` (0x4ba830). Caller: 0x469655 (Car render 0x469630). DAT_00501308 (mirror enabled option, guess), DAT_0050146c (header unk_481390.h calls it replay speed; the use here suggests it is the mirror/view flag too - guess, check).
- **0x464ef0 `Car::RenderMirror(float dt)`** (thiscall, `ret 4`, SEH EH 0x4db998, `ebp` frame, FPU, 1230 B). Skipped if DAT_00501308 == 0. Same camera build as 0x464c10, then: renderer slot 0x84(14,1); `0x48e660()` (shared empty ret); `Stage::FUN_00411170(&ViewManager, &DrawContext)` (0x411170); copies Stage fields into the DrawContext statics (0x5093c4..0x5093e0 from stage+0x1a8..0x1cc; stage+0x1a4 only if DAT_00501434); RaceMode (global 0x501450) virtual **slot 22 (+0x58)**(ctx, view, car); `DrawContext::0x4b8310(0)`; converts the Vec2 constants to an integer viewport (`fistp`, x = 0x56aea8*0x5093f0, y = 0x56aeac*0x5093f4, w = 0x5093e8*0x56aea8, h = 0x5093ec*0x56aeac); renderer 0x3c GetViewport (save), 0x38 SetViewport(mirror), slot 0x60(2), 0x84(1,1), 0xc0, 0xcc; `DrawContext::0x4b7c80(device, view)`; RaceMode slot 23 (+0x5c); `Stage::0x411350(device, view)`; `DrawContext::0x4b8160(device, view)`; RaceMode slot 24 (+0x60); renderer 0x84(1,1); `RaceMode::0x488880(&view)`; 0x80(3); `Stage::0x411460(device, view, ctx, 0)`; 0x80(3); then restores states 0x84(0,14), 0x84(2,1), SetViewport(saved), 0x84(0,13). Caller 0x49415f. (This is the draw order of the main view with a different viewport and mirrored camera; names guess.)

Plain PE/object observations (`objcmp`) for `0x464ef0`: reordering the
independent Stage-to-context stores to read `stage+0x1c8` and `stage+0x1cc`
before `stage+0x1a8` removed the 10-byte register-order hunk (15→13 differing
bytes). A body-pointer alias and an explicit `body + 0x60` float pointer kept
the remaining mismatch. The exact source shape evaluates the eye-height term
through `car->body->m0[17]` before declaring `matrix = car->body->m0`; this
reproduces the target's in-place `add eax,0x60` schedule. `objcmp` confirms the
complete 1,230-byte function and all 108 relocations match. Focused file check:
27 exact-marked, no `NONMATCHING`, DIFF or compile failures after the shared
camera setup was matched below.

For `0x464c10`, the same scheduling fix is to evaluate the eye-height term via
`body->m0[17]` before declaring `matrix = body->m0`. This matches the target's
in-place `add eax,0x60` and subsequent pointer store. `objcmp` confirms the
complete 645-byte function and all 47 relocations match.

## 2. F3: replay glue and network state packing, 0x4654f0 - 0x465e5f

Embedded object at **Car+0x1bed0**: a replay/ghost **recorder** (methods 0x401380 `Start(mode)` (mode 1 calls `0x401880` and clears +0x24, stores mode in recorder+0x28 = Car+0x1bef8), 0x4013b0 `Stop()` (`[this+0x28]=0`), 0x4013c0 per-frame `Update(Car*)` (reads `car+0x1becc` Player*), 0x401920 `Save(name)` (uses 0x4a4d40 File), 0x401a40 `Load(name)`). The recorder in turn calls `Car::GetState` (0x401412) and `Car::SetState` (0x40147e). `Car+0x1befc` is the Car-side mode word (0 = off, 1 = record, 2 = play/ghost; same word `state` in unk_4666a0.h "2 = no random sounds").

State packing: `GetState` / `SetState` (CarState, 0x57 bytes, unaligned dword stores inside) are used by the network (callers 0x483a5b, 0x491888 etc.) and by the recorder. Offsets in CarState (from the stores in 0x465570 / loads in 0x4658a0):
- +0x00 int time (written by caller; 0x4658a0 only reads fields from +4).
- +0x04 byte throttle (Car+0x1338 * 255, clamp 0..255); +0x05 byte Car+0x133c (brake, guess); +0x06 byte Car+0x1340 (clutch/handbrake?); +0x07 byte Car+0xf80 (guess); +0x08 byte (Car+0x134c * 127 (0x4e1c10) + 128, clamp 0..255; steering); +0x09 byte Car+0xf8c (gear int); +0x0a word (int)Car+0xf88 (rpm?); +0x0c word: Car+0x1354[8] ring buffer (read from index Car+0x1374 & 7): `(sum(newest 4) - sum(oldest 4)) * 0.25 * 16383` (0x4e13a4, 0x4e1e94) (steering rate, guess);
- +0x0e..+0x1a four dwords from CarBody +0x14c, +0x150, +0x154, +0x158; +0x1e,+0x22,+0x26 body +0x14,+0x18,+0x1c (position); +0x2a..+0x3e six dwords from body +0x19c..+0x1b0 (velocity / angular velocity, guess); +0x42..+0x45 four bytes `(w[i].f110 - w[i].f0)/Car[0xd08+i*0x34] * 255` (suspension compression, w[i] = per-wheel record at Car+0x40+i*0x330); +0x46,+0x48,+0x4a,+0x4c shorts = `w[i].f26c * 255` clamped to +-32767; +0x4e,+0x50,+0x52,+0x54 shorts = `Car[0xd10+i*0x34] * 255` clamped; +0x56 byte = `Car+0x209c * 255` if `Car+0x20a4` else 0 (damage/mud level, guess).
- `SetState` (0x4658a0) does the inverse (byte * 1/255 via 0x4e1e8c; clamps/signs for steering via 0x4e1e90 = 1/127), then calls `Car::0x45da60(brake)` (Car::SetBrake) and `0x41c390`, `Car::0x4581f0`, re-places the four wheels on the ground: for each wheel `0x419770(Car+0x1464 (collision/terrain query), t, ...)`; on a hit copies position, surface id (`stage+0x24c+i*4`), friction scale (`stage+0x47c+..`), normal (`stage+0x38c+..` 8-byte pairs) into the wheel records (+0x294..+0x30c, stride 0x330). Last: `Car+0x20a0 = byte[+0x56] * 1/255`.
- `GetState2` (0x465c40, CarState2 0x14 bytes) packs 4 x (Car+0x2c8+i*0x330, Car+0x1c73c+i*4, Car+0x2cc+i*0x330) as clamped bytes (`0x465d50(v, 255.0, 0, 255)`) to out+4+i, +8+i, +0xc+i, then Car+0x1c750 -> +0x10, +0x1c74c -> +0x11, +0x1c754 -> +0x12, +0x1c758 -> +0x13 (wear / damage values, cf. unk_46a8a0.h `damage[3]`). `SetState2` (0x465d80, arg is `CarSetup*` in the header but really a CarState2*) is the inverse via `0x465e40(byte)` = `(float)(b & 0xff) * (1/255)`; callers 0x491944 (SetState2), 0x483b02 (GetState2).

Helpers: **0x465d50** `static unsigned char ClampScale(float v, float scale, int lo, int hi)` (cdecl, 16 bytes of args, `v*scale` -> fistp -> returns `lo` if below, `hi`... returns low byte; called 7x from 0x465c40); **0x465e40** `static float ByteToUnit(int b)` (cdecl, `(b & 0xff) * 0.003921569`, 7 call sites in 0x465d80).

## 3. F4: Car setup, 0x465e90 - 0x46669f

Car setup = `CarSetup` (5 ints at Car+0x00..0x10, unk_481390.h), written by the five setters called together from the menu/replay/network code (callers 0x433739, 0x433aed, 0x4347ce (car selection screens), 0x447d91..0x447dbd, 0x461908.. (Car ctor), 0x46be7a.. (RestoreSnapshot, see range-4666a0.md), 0x48e091.., 0x4809a2, 0x4916fe..0x49173e):

| Setter | Arg stored at | Effect |
|---|---|---|
| 0x465e90 `value[0]` | Car+0x00 | loads tyre dynamics set N from `data/global/dynamics/tire<N>...dynamics.ini` (see 3.1) |
| 0x466500 `value[1]` | Car+0x04 | `Drivetrain::SetGearing(n)` on Car+0xdb8 (0x46baa0, "gearing presets") |
| 0x466520 `value[2]` | Car+0x08 | suspension preset: `Car[0xd00]=Car[0xd34]=Car[0x1138+n*4]`, `Car[0xd68]=Car[0xd9c]=Car[0x11fc+n*4]` (per-wheel spring rates, stride 0x34 records at Car+0xd00), then `Car::ApplyWheel(0..3)` (0x45a7a0) |
| 0x466590 `value[3]` | Car+0x0c | 5-way switch (jump table 0x4665f8) setting `Car+0x14` (float): n=0 `[1108]`, 1 `([110c]+[1108])*0.5`, 2 `[110c]`, 3 `([1110]+[110c])*0.5`, 4 `[1110]` (guess: brake balance / differential lock) |
| 0x466610 `value[4]` | Car+0x10 | `Car+0x18 = [10ec]*s[n]`, `Car+0x1c = [10f0]*s[n]`, `s = {0.75, 0.875, 1.0, 1.125, 1.25}` (guess: steering ratio / lock; Car+0x18 is "steering range" in the header) |

### 3.1 LoadTyreDynamics (0x465e90, 1644 B, `Car::` thiscall `ret 4`, SEH EH 0x4dba4f, 0x13c-byte frame, FPU)

`void Car::FUN_00465e90(int set)`: `this->setup.value[0] = set`. Builds the file name with `std::string`: `"data/global/dynamics/tire"` (0x4fdb80) + `itoa(set, 0x5093f8, 10)` (0x4c1902; 0x5093f8 is a global scratch buffer) + `"dynamics.ini"` (0x4fdb70) (no separator is visible in the strings; guess the data uses names like `tire1dynamics.ini` - verify against the shipped data dir). Opens a `Config` (0x405580 ctor, 0x4055a0 dtor) with `Config::Define(keys=0x4fbed8, count=0x10e)` (0x4076c0), `Config::Load(path, requireAll=1, verifyChecksum=1, checksum[set])` (0x406ce0) where the checksum table is on the stack: `{0x660b, 0xe153, 0x8cb3, 0xb053, 0x080e}` for sets 0..4 (set is 0..4). On failure builds `"Missing variables in file: " (0x4fac50) + name` (operator+ 0x409130, 0x4024f0/0x4c12db string moves) and throws (`push 0x4e4328; ... call 0x4c13ca`, throw machinery, exception type vtable 0x4e2c78/0x5070b0). 0x4fbed8 is a key table of 270 (30 x 9) `{name*, type}` pairs, key names `Dyn<NN><Suffix>` with NN = 00..29.

Then for NN = 0..29 (the two digits are patched into 9 stack copies of the key templates at 0x4fdac4..0x4fdb58: `Dyn00XFriction, Dyn00ZFriction, Dyn00CStiffness, Dyn00XStiffness, Dyn00ZStiffness, Dyn00PneumaticOffset, Dyn00PneumaticTrail, Dyn00InducedDragCoeff, Dyn00RollingResistance`; digits via `0x66666667` division and `+0x30`), the record at **Car+0x1680 + NN*0x40** (30 records, 0x780 bytes, ends 0x1e00) is filled and **multiplied component-wise by a scale record at `Car + (set+0x54)*0x40` = Car+0x1500+set*0x40**: +0 RollingResistance (GetFloat 0x407f60), +4 InducedDragCoeff, +8 PneumaticTrail, +0xc PneumaticOffset, +0x10 ZStiffness (GetVector3 0x4085e0), +0x1c XStiffness (Vector3), +0x28 CStiffness (GetVector2 0x408390), +0x30 ZFriction (Vector2), +0x38 XFriction (Vector2). These are the per-surface tyre force constants (NN = surface id used with `Car+0x1e44 wheelSurface[4]`, guess) that the wheel code of 0x46c2c0 reads. Ends by destroying the strings and `Config` (0x4055a0) (EH state 0 -> destroy std::string temporaries, ref-count decrement via `[ptr-1]`, `0x4bd000` delete).

## 4. Function table

Columns: addr, size (bytes up to the next function; csv size where it exists), convention / frame, role, main callers. "fastcall" = `this` in ecx with no stack args (VC6 thiscall). SEH = `push -1; push handler; fs:0` prologue. "inline" = compiler-generated or inline member. Stubs and static-init wrappers are not in functions.csv.

| Addr | Size | Conv / frame | Role | Main callers |
|---|---|---|---|---|
| 0x4649f0 | 94 | fastcall, SEH (EH 0x4db963), vec-dtor x2 | **inline/implicit dtor** of a 0x90-byte aggregate (4 x 4-byte + 16 x 8-byte elements, `0x4c1116`); no callers | none |
| 0x464a50 | 21 | fastcall leaf | `Car::` inline: `unk1c908 = 0; unk1c90c = 0xffff15a0 (-60000)` (clear on-screen message) | 0x4619a7 |
| 0x464a70 | 25 | fastcall leaf | **ctor of the 0x1c-byte record** (dword +0, byte +4, dwords +8..+0x18 zeroed); vec-ctor callback | Car ctor 0x45e6f3, 0x45e775, 0x45e793, 0x45e7b1, 0x45e7d9 |
| 0x464a90 | 15 | fastcall | **SoundLayer wrapper ctor** (implicit): `call 0x49b630(this+4)`, returns this | Car ctor (vec-ctor callback 0x45e874, 0x45e895) |
| 0x464aa0 | 8 | fastcall | **SoundLayer wrapper dtor**: `add ecx,4; jmp 0x49b6e0` | Car ctor/dtor, EH tables (0x45e86f, 0x45e890, 0x461499...) |
| 0x464ab0 | 10 | static-init wrapper | `call 0x464ac0; jmp 0x464ae0` (Vec2 A) | init table |
| 0x464ac0 | 21 | leaf | ctor Vec2 A: 352.0, 8.0 -> 0x5093f0/f4 | 0x464ab0 |
| 0x464ae0 | 12 | stdcall | atexit(0x464af0) via 0x4c0d01 | 0x464ab0 |
| 0x464af0 | 1 | `ret` | empty dtor | atexit |
| 0x464b00 | 10 | static-init wrapper | Vec2 B | init table |
| 0x464b10 | 21 | leaf | ctor Vec2 B: 320.0, 80.0 -> 0x5093e8/ec | 0x464b00 |
| 0x464b30 | 12 | stdcall | atexit(0x464b40) | 0x464b00 |
| 0x464b40 | 1 | `ret` | empty dtor | atexit |
| 0x464b50 | 10 | static-init wrapper | DrawContext @0x509250 | init table |
| 0x464b60 | 10 | thunk | `mov ecx,0x509250; jmp 0x4b7a40` (ctor) | 0x464b50 |
| 0x464b70 | 12 | stdcall | atexit(0x464b80) | 0x464b50 |
| 0x464b80 | 10 | thunk | `mov ecx,0x509250; jmp 0x4b7bf0` (dtor) | atexit |
| 0x464b90 | 10 | static-init wrapper | ViewManager @0x509080 | init table |
| 0x464ba0 | 10 | thunk | `mov ecx,0x509080; jmp 0x4b77b0` (ctor) | 0x464b90 |
| 0x464bb0 | 12 | stdcall | atexit(0x464bc0) | 0x464b90 |
| 0x464bc0 | 10 | thunk | `mov ecx,0x509080; jmp 0x4b78e0` (dtor) | atexit |
| 0x464bd0 | 10 | static-init wrapper | SkidFrustum @0x508f68 | init table |
| 0x464be0 | 10 | thunk | `mov ecx,0x508f68; jmp 0x4653f0` | 0x464bd0 |
| 0x464bf0 | 12 | stdcall | atexit(0x464c00) | 0x464bd0 |
| 0x464c00 | 10 | thunk | `mov ecx,0x508f68; jmp 0x465460` | atexit |
| 0x464c10 | 645 | thiscall `ret 4`, SEH (EH 0x4db978), ebp frame, FPU | `Car::PrepareMirror(ViewManager* view)`: mirror camera + frustum, restores view | 0x493fa4 |
| 0x464ea0 | 66 | `ret 4`, no frame (esi only) | `MirrorVisible(Object*)` via SkidFrustum::Test (0x4ba830) on 0x508f68 | 0x469655 |
| 0x464ef0 | 1230 | thiscall `ret 4`, SEH (EH 0x4db998), ebp frame, FPU, fistp | `Car::RenderMirror(float dt)`: full mirror scene render | 0x49415f |
| 0x4653c0 | 44 | stub | `_$E` guard stub (bit 0x5826d5, `atexit(0x402c90)`) ends F1 | init table |
| 0x4653f0 | 98 | fastcall, SEH (EH 0x4db9c3) | **SkidFrustum ctor** (implicit, 2 x `__vec_ctor(0x10, 6, 0x490ad0, 0x402c90)`) | 0x464be5, 0x4b7c55 |
| 0x465460 | 94 | fastcall, SEH (EH 0x4db9e3) | **SkidFrustum dtor** (implicit, 2 x `__vec_dtor`) | 0x464c05, 0x4b7c75 |
| 0x4654c0 | 44 | stub | `_$E` stub of the empty F2 module | init table |
| 0x4654f0 | 54 | thiscall `ret 4` | `Car::SetReplayMode(int mode)` (header: FUN_004654f0): if `[this+0x1befc]` stop recorder (0x4013b0); store; if mode start (0x401380) | 0x4808e8, 0x480a2e, 0x4817db, 0x4818ff, 0x486229, 0x48f22a, 0x48f2b6, 0x48f948, 0x48f99d, 0x48fa19 |
| 0x465530 | 19 | thiscall `ret 4` | `Car::LoadReplay(const char* name)` -> `recorder(this+0x1bed0).Load` (0x401a40) | 0x48e0dd, 0x49297d |
| 0x465550 | 19 | thiscall `ret 4` | `Car::SaveReplay(const char* name)` -> 0x401920 | 0x48e3ff |
| 0x465570 | 804 | thiscall `ret 4`, ebp frame, FPU | `Car::GetState(CarState*)`: pack control/body/wheel state into 0x57 bytes | 0x401412, 0x483a5b |
| 0x4658a0 | 926 | thiscall `ret 4`, SEH (EH 0x4dba00), ebp frame, FPU | `Car::SetState(CarState*)` (header: apply a network sample): unpack, re-ground the wheels (0x419770), apply controls | 0x40147e, 0x491888 |
| 0x465c40 | 261 | thiscall `ret 4`, no frame | `Car::GetState2(CarState2*)`: pack wear/damage to 0x14 bytes | 0x483b02 |
| 0x465d50 | 39 | **cdecl** helper | `static BYTE ClampScale(float v, float scale, int lo, int hi)` | 0x465c76..0x465d32 |
| 0x465d80 | 182 | thiscall `ret 4`, no frame | `Car::SetState2(CarState2*)`: inverse via `ByteToUnit` | 0x491944 |
| 0x465e40 | 24 | **cdecl** helper (returns in st0) | `static float ByteToUnit(int b)` | 0x465da9..0x465e20 |
| 0x465e60 | 44 | stub | `_$E` stub, ends F3 | init table |
| 0x465e90 | 1644 | thiscall `ret 4`, SEH (EH 0x4dba4f), 0x13c frame, std::string, Config | `Car::LoadTyreDynamics(int set)` | 0x433739, 0x433aed, 0x4347ce, 0x447d91, 0x4618ff, 0x46be7a, 0x4809a2, 0x48e091, 0x4916fe |
| 0x466500 | 22 | thiscall `ret 4` | `Car::SetGearingPreset(int)`: store value[1], `Drivetrain::SetGearing` | 0x447d9c, 0x461908, 0x46be87, 0x48e09f, 0x49170e |
| 0x466520 | 111 | thiscall `ret 4` | `Car::SetSuspensionPreset(int)`: value[2], spring tables, `ApplyWheel(0..3)` | 0x447da7, 0x461911, 0x46be94, 0x48e0ad, 0x49171e |
| 0x466590 | 103 (+20 jump table) | thiscall `ret 4`, jump table 0x4665f8 | `Car::SetBalancePreset(int)`: value[3], 5-way float interpolation into Car+0x14 | 0x447db2, 0x46191a, 0x46bea1, 0x48e0ba, 0x49172e |
| 0x466610 | 82 | thiscall `ret 4`, local float table | `Car::SetSteeringPreset(int)`: value[4], `Car+0x18/0x1c = base * {0.75..1.25}[n]` | 0x447dbd, 0x461923, 0x46beae, 0x48e0c8, 0x49173e |
| 0x466670 | 44 | stub | `_$E` stub, ends F4 | init table |

Compiler-generated / thunks to treat specially (not "real" source functions): 0x4649f0, 0x464a90, 0x464aa0, 0x4653f0, 0x465460, all of 0x464ab0-0x464c00 (static-init wrappers, ctor thunks `mov ecx,imm; jmp X`, atexit registrars (12 bytes: `push dtor; call 0x4c0d01; pop ecx; ret`), the two empty `ret` dtors), and the four stubs. The 6-/5-byte thunks near 0x480740/0x480770 are not in this range.

## 5. Suggested subagent work groups

(Header work first: add `Car` members listed in the table to unk_481390.h; define the mirror/state view structs in the same style as CarSoundView. `DrawContext`, `ViewManager`, `SkidFrustum` and the recorder at Car+0x1bed0 are needed as types. The file-local statics and all stubs come out of the same .cpp.)

| Group | Functions | Bytes | Notes |
|---|---|---|---|
| A (statics + COMDAT helpers + stubs) | 0x4649f0-0x464aa0 (belongs to the Car ctor file, check there first), 0x464ab0-0x464c00 (declare the 5 statics: `Vec2 A(352,8); Vec2 B(320,80); DrawContext; ViewManager; SkidFrustum`), 0x4653f0, 0x465460, stubs 0x4653c0, 0x4654c0 | ~800 | Matching the static-init order and the empty dtor thunks is the work. SkidFrustum class already in unk_4741a0.h (implicit ctor/dtor); Vec2 statics need a class with user ctor + empty dtor. |
| B (mirror) | 0x464c10, 0x464ea0, 0x464ef0 | 1941 | Large FPU sequences with inlined 3x4 matrix * vector multiplies (same inline as in other Car files: `fld/fmul` 6-6-3 pattern with `fxch`) and ViewManager statics accessed by absolute address. Do 0x464c10 first (it is the first half of 0x464ef0). |
| C (state pack/unpack) | 0x465570, 0x4658a0 | 1730 | Matched pair; define `CarState` fields (offsets in section 2) once. `Car+0x1354` 8-float ring buffer read; unaligned dword stores into the packet (packed struct, `#pragma pack(1)`). `0x4658a0` has an EH frame (the temporary Vec3s in the wheel loop). |
| D (small members + setters) | 0x4654f0, 0x465530, 0x465550, 0x465c40, 0x465d50, 0x465d80, 0x465e40, stub 0x465e60, 0x466500, 0x466520, 0x466590, 0x466610, stub 0x466670 | ~1270 | 0x465d50/0x465e40 are file-static cdecl helpers (not inlined at their 7 call sites each). 0x466590 needs a dense `switch` that produces the jump table; 0x466610 the local float array. |
| E (tyre dynamics loader) | 0x465e90 | 1644 | Alone. std::string temporaries with VC6 inline `_Tidy`, 9 key-template stack copies, 30-iteration loop with 7 Config getter calls per iteration, throw path. Model it on `Drivetrain::LoadConfig` (0x46a920, range-46a8a0.md) and `InitSounds` (0x4666a0): same `Config` + checksum + `TmpString` machinery. |

Order of difficulty: D (easy), A, C, B, E.

## 6. Open questions (guesses to verify before relying on them)

- Whether 0x4653f0/0x465460 and the first six functions of the range are file members or trailing COMDATs of the preceding file (see headline 2). Doesn't change the bytes, only where the matching harness must compile them.
- Exact path string: `"...tire" + itoa + "dynamics.ini"` without a separator (check the shipped `data/global/dynamics/` folder for names).
- Roles of the five Car setup values beyond "gearing" and "suspension springs" (balance/steering are guesses from the target fields Car+0x14/0x18/0x1c).
- DAT_00501308 (mirror option) and DAT_0050146c (used both here and as "replay speed" in the header) - same variable? cross-check at 0x92ecf.. users.
- Which class owns the 0x90-byte aggregate destroyed by 0x4649f0 (no caller; possibly dead code or reached only via an EH unwind table entry).
