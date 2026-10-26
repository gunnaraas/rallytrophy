<!-- Subagent analysis, 2026-10-23; read-only raw disassembly of the Patch 1.01 exe (no REA); guesses are marked. -->
# Range 0x4563e0 - 0x45988f: module map (ViewManager/camera, Car collision, CarInput, Car env-map draw)

Method: rawdis.py of the whole range, my own call/absolute-reference summary per function, refs.py for callers, .rdata reads (strings, floats, vtable, init table). "Guess" marks inference. EH handler addresses (0x4da6fe ...) are the `push imm` after `push -1` in each SEH prologue. The `abs@off` hits of refs.py are file offsets; I converted them to VAs (.rdata VA = file offset - 0xdf800 + 0x4e1000).

## 0. Headline findings

1. **The range is four source files (four `_$E` stubs), not one class.** Init table at 0x4f1380 (dwords): 0x4563b0 (previous file's stub), **0x4563e0, 0x456450, 0x4581c0**, **0x4586d0**, **0x459200**, **0x459230, 0x459860**. So: file 1 has two static initialisers plus its stub; files 2 and 3 are stub-only; file 4 has one static initialiser plus its stub.
2. **File 1 = `ViewManager`, the chase/replay camera of a Car** (ctor 0x456490, vtable 0x4e1b74, size **0x254**, base class `MirrorView` (0x1d0 bytes, ctor 0x4b77b0 / dtor 0x4b78e0, include/unk_464ab0.h)). A Car owns a pointer to it at **Car+0x20** (Car `SetState` stores it, 0x45b348; Car::Update calls `0x456820(dt)` at 0x45ba95, 0x456b10 at 0x45b4c8, 0x4569e0 at 0x45b370). The RaceSession creates it (`new`, ctor calls at 0x4924e5 and 0x493141) and calls the loader 0x457b10 once (0x491fd1, RaceSession ctor).
3. **The nine camera presets live in a static table of 9 x 0x20 bytes at 0x508ca8**, filled from `data/settings/camera.txt` by 0x457b10 (keys `Camera<N>LookAt`, `Camera<N>Position`, `Camera<N>Stiffness`, `Camera<N>Fov`, N = 1..9, plus the error string `Missing variables in file: `). Camera index = `ViewManager+0x1d4` (0..8; 8 = the "random/auto" slot, 7 and 6 are special).
4. **File 2 (0x4581f0-0x4586ff) is three `Car` methods** about collision broadphase: `Car::FUN_004581f0` (compute the oriented-box AABB and register it with the Stage), `Car::FUN_00458590(Car* other)` (car-vs-car overlap test, returns 0) and `Car::FUN_00458690` (apply pending collisions). They are not camera code.
5. **File 3 (0x458700-0x4591f4) is `CarInput`** (alias `Controls` in include/unk_495640.h): ctor 0x458700, dtor 0x458820, set/reset helpers, force-feedback effect creation and update (`CarInput::Update(Car*, int dtMs, int)` 0x458b30, 1393 bytes), and gear-shift handling 0x4590d0 / 0x4591c0. In the middle of this file there is one `Car` method, **0x4590b0** (`Car::FUN_004590b0`, "replay hook": `drive.ShiftTo(drive.SelectGear())`, i.e. re-apply the gear).
6. **File 4 (0x459230-0x45988f) is `Car::Draw459270(view, drawArg)`**, the 6-face environment/cube-map render of the Car (jump table at 0x459844, 6 entries), with the file-scope static **`DrawContext` at 0x508dc8** (ctor `0x4b7a40`, atexit dtor `0x4b7bf0`).
7. **No STL instance bodies are emitted in the range.** `0x498300` / `0x403130` / `0x4bd000` / `0x4c1276` are out-of-line shared helpers (ptr-copy, erase, delete, new); the ViewManager ctor has an inlined `vector::reserve(0x400)` (see below).

Module table (end = address before the next module; each stub is 0x30 bytes with 16-byte alignment, not in functions.csv):

| # | Range | Stub / init entries | Content | Suggested file |
|---|---|---|---|---|
| M1 | 0x4563e0 - 0x4581ef | inits 0x4563e0 (camera table) + 0x456450 (empty object); stub **0x4581c0** | `ViewManager` (camera), camera.txt loader, `0x458150` static helper | src/unk_4563e0.cpp |
| M2 | 0x4581f0 - 0x4586ff | stub **0x4586d0** only | `Car` collision members 0x4581f0, 0x458590, 0x458690 | src/unk_4581f0.cpp |
| M3 | 0x458700 - 0x45922f | stub **0x459200** only | `CarInput` (all of it) + `Car::FUN_004590b0` | src/unk_458700.cpp |
| M4 | 0x459230 - 0x45988f | init **0x459230** (static DrawContext 0x508dc8) + stub **0x459860** | `Car::Draw459270` | src/unk_459230.cpp |

Stubs in M1..M4 all use the guard byte 0x5826d5 and `atexit(0x402c90)` (shared empty destructor), same as every other `_$E` stub.

## 1. M1: ViewManager and camera table, 0x4563e0 - 0x4581ef

### 1.1 File-scope statics (the two initialisers)

- **0x4563e0** (init, no SEH): `call 0x4563f0; jmp 0x456410`.
  - 0x4563f0 (25 bytes, stdcall): `0x4c11f4(0x508ca8, size 0x20, count 9, ctor 0x490ad0, dtor 0x402c90)`, i.e. `static CameraPreset g_cameras[9];` (0x490ad0 is `mov eax,ecx; ret`, an empty ctor; 0x402c90 the shared empty dtor).
  - 0x456410 (12 bytes): `atexit(0x456420)`.
  - 0x456420 (40 bytes, atexit callback, not in functions.csv): guarded by bit 0 of byte **0x508ca4** (the array's own guard byte): `0x4c1116(0x508ca8, 0x20, 9, 0x402c90)` (vector destructor iterator).
- **0x456450** (init): `call 0x456460; jmp 0x456470`: 0x456460 is a bare `ret` (1 byte); 0x456470 does `atexit(0x456480)` and 0x456480 is a bare `ret`. This is a static object (guess: an empty class instance, or a static whose constructor/destructor are empty) with no data address; nothing to decompile except to keep three empty functions.
- 0x508c98 / 0x508c9c / 0x508ca0: three floats (0.0, -0.1 (0xbdcccccd), 10.0), written by the ViewManager ctor at 0x4565c9..0x4565dd (every construction, not a guarded static). The loader 0x457b10 later overwrites them with the Camera7 vector (0x458113..0x458133: copy of 0x508d68..0x508d70).

### 1.2 CameraPreset (0x20 bytes, 9 entries from 0x508ca8; stride `shl 5`)

Order of the keys read by 0x457b10 (the first key's 3 floats go to +0x00, the second's to +0x0c): for N = 1..9, entry N-1:

| Offset | Source key (string address) | Type | Use seen |
|---|---|---|---|
| +0x00 | `Camera<N>LookAt` (0x4fac40 for N=1, then -0x40 stride: 0x4fac00, 0x4fabc0, 0x4fab80, 0x4fab40, 0x4fab00, 0x4faac0, 0x4faa80, 0x4faa40) | Vec3 via 0x4085e0 | offset added to the car position in 0x4578c0 (`[eax+0x508ca8..b0]`) |
| +0x0c | `Camera<N>Position` (0x4fac30, 0x4fabf0, 0x4fabb0, 0x4fab70, 0x4fab30, 0x4faaf0, 0x4faab0, 0x4faa70, 0x4faa30) | Vec3 via 0x4085e0 | second offset (`[eax+0x508cb4..bc]`) |
| +0x18 | `Camera<N>Stiffness` (0x4fac1c, 0x4fabdc, 0x4fab9c, 0x4fab5c, 0x4fab1c, 0x4faadc, 0x4faa9c, 0x4faa5c, 0x4faa1c) | float via 0x407f60 | `+0x508cc0`: read in 0x456820 as follow stiffness |
| +0x1c | `Camera<N>Fov` (0x4fac10, ... , 0x4faa10) | float via 0x407f60, multiplied by **0.0174532925 (0x4e1b54, deg to rad)** | `+0x508cc4`: copied to `ViewManager+0x12c` (projection FOV) in 0x456b10 |

(Which of the two vectors is "eye" and which is "target" is not certain; the loader fills +0x00 from `...LookAt` and +0x0c from `...Position`, 0x4578c0 treats them as offsets. Treat the names as a guess.)

Config reading uses the same `Config` helpers as the AI loader: ctor 0x405580, open 0x4076c0(path, 0x24), 0x406ce0(file, 1, 0, 0) with file name 0x4e1b58 `data/settings/camera.txt`, Vec3 reader 0x4085e0 (ptr to key string), float reader 0x407f60, dtor 0x4055a0. When the open fails (`eax == 0` branch at 0x457b5f) it builds the string `Missing variables in file: ` (0x4fac50) + file name and throws via `0x4c13ca` with the exception object vtable 0x4e2c78 / type 0x4e4328 (std::string-style inlined appends with 0x402700 / 0x402980 / 0x402960, `0x4c12db` copy-construct, `0x4024f0` append): same pattern as other loaders.

### 1.3 `ViewManager` (size 0x254, vtable 0x4e1b74)

Vtable (7 dwords, from .rdata): [0] **0x456600** scalar deleting dtor (this module), [1] 0x4b2c10, [2] 0x48e660 (shared empty function), [3] 0x4b2d50, [4] 0x4b2fc0, [5] 0x4b3000, [6] 0x4031e0 (guess: `_purecall`). Slots 1-6 are inherited from `MirrorView`; MirrorView's ctor (0x4b77b0) installs its own vtable first and the ViewManager ctor overwrites it at 0x4564f0.

Fields seen (offset: use). Offsets below 0x1cc belong to MirrorView (include/unk_464ab0.h, unk_481390.h); `0x14..0x1c` position, `0x20..0x58` frame, `0x60..0x9c` matrix, `0x114` `CameraName`, `0x124/0x128/0x12c` projection (near, far, FOV):
- +0x12c: FOV (written from `preset.fov` in 0x456b10; also smoothed with a 0.282743364 = 16.2 degree floor / 1.5707963 (pi/2) ceiling clamp, consts 0x4e1ba8 / 0x4e1b28).
- +0x130: float/int (copied from the zone's field `[zone+0x130]`; tested non-zero in 0x456b10).
- +0x134: int flag, set 0 or 1 in 0x456b10 / 0x4569e0 (smoothing reset flag; `[+0x1e0]` toggles it).
- +0x138, +0x13c, +0x140: saved copy of the car position (Vec3 from MirrorView +0x14..+0x1c, stored by 0x456b10 at 0x456b43).
- +0x144..+0x183: 16 dwords = copy of the MirrorView matrix at +0x20..+0x5f (`rep movs ecx=0x10` at 0x456b65) (previous-frame matrix).
- +0x1c4: dt (int ms or float; stored from the stack arg of 0x456b10).
- **+0x1cc: `Car*` target** (set by 0x456660/0x456780). **+0x1d0: `Stage*`** (second argument of 0x456660; used as `ecx` of 0x414870, a zone lookup).
- **+0x1d4: view index 0..8** (shl 5 into the preset table; 6 = rigid mounted camera, 8 = random/auto, 7 = fallback when not local).
- +0x1d8: counter (zone confirmations; resets to 0, switches when > 5).
- **+0x1dc: byte `local`** (ctor sets 1; "followed player is local", include/unk_481390.h).
- +0x1e0: int flag (ctor 1; cleared by 0x456b10).
- +0x1e4: int, nonzero = automatic zone-driven camera switching enabled (0x4569e0 and 0x456820 test it); ctor 0.
- +0x1e8: current zone pointer (result of 0x414870), +0x1ec previous/candidate zone.
- +0x1f0: Vec3 eye output; +0x1fc: Vec3 look-at output (0x4578c0 writes both through its two pointer args: `&this+0x1f0`, `&this+0x1fc`).
- +0x208: Vec3 smoothed eye, +0x214: Vec3 smoothed look-at (0x4566e0 initialises them from the first eye/look-at plus the car position).
- +0x220: byte (allocator slot; set from `[esp+0xf]` in the ctor, std::vector artefact), then **+0x224 first, +0x228 last, +0x22c end**: an inlined `std::vector<int-or-pointer>::reserve(0x400)` (capacity bytes 0x1000; elements copied with `0x403140`, freed with `0x4bd000`, allocated with `0x4c1276(0x1000)`); freed in the dtor 0x456620.
- +0x250: int = **0x11 (17)** (ctor; the same limit the `CarTrace` buffers at Car+0x1464 store at +0x30).
- Total 0x254 (header already has `BYTE unk1dd[0x254 - 0x1dd]`).

Floats / constants in .rdata used by the update (0x456b10): 0x4e1b00 0.02, 0x4e1b04 0.024, 0x4e1b28 pi/2, 0x4e1b48 0.1, 0x4e1b4c 0.15, 0x4e1b50 0.3, 0x4e1b98 0.9, 0x4e1b9c 0.05, 0x4e1ba0 0.027, 0x4e1ba4 0.999, 0x4e1ba8 0.2827 (0x3e90c3bd), 0x4e1348 1.0, 0x4e1530 1.5, 0x4e138c 0.5, 0x4e1390 0.8, 0x4e139c 0.1, 0x4e13a4 0.25, 0x4e13c4 0.2, 0x4e13dc 3.0, 0x4e14cc 0.75, 0x4e1398 0.001. Statics read: 0x508d74, 0x508d94..0x508d9c, 0x508db4..0x508dbc (preset entries 6, 7, 8 vectors).

### 1.4 Functions

| Addr | Size | Conv / frame | Role (guess marked) | Main callers |
|---|---|---|---|---|
| 0x4563e0 | 10 | init | static init: `call 0x4563f0; jmp 0x456410` | init table |
| 0x4563f0 | 25 | stdcall | vector ctor iterator for `g_cameras[9]` (0x20 stride) | 0x4563e0 |
| 0x456410 | 12 | stdcall | `atexit(0x456420)` | jmp from 0x4563e5 |
| 0x456420 | 40 | atexit cb (not in csv) | guarded vec dtor iterator for `g_cameras` (guard 0x508ca4) | atexit |
| 0x456450 | 10 | init | `call 0x456460; jmp 0x456470` | init table |
| 0x456460 | 1 | stdcall | empty function (`ret`) | 0x456450 |
| 0x456470 | 12 | stdcall | `atexit(0x456480)` | jmp |
| 0x456480 | 1 | atexit cb | empty `ret` | atexit |
| 0x456490 | 355 | fastcall, SEH (EH 0x4da6fe) | **ViewManager::ViewManager()**: MirrorView ctor, vtable 0x4e1b74, field init, inlined reserve(0x400), statics 0x508c98.. | RaceSession 0x4924e5, 0x493141 |
| 0x456600 | 30 | thiscall `ret 4` | **scalar deleting dtor** (vtable slot 0): `~ViewManager(); if (flags&1) delete this` | vtable only |
| 0x456620 | 59 | fastcall | **~ViewManager()**: reset vtable, free the vector buffer (0x4bd000), zero first/last/end, call MirrorView dtor 0x4b78e0 | 0x456600 |
| 0x456660 | 121 | thiscall `ret 8` | **SetTarget(Car*, Stage*)**: if the old target differs, `oldCar->SetCockpit(0)` (0x4691e0); set +0x1cc/+0x1d0; if not local and view==8 then view=7 | RaceSession 0x494fe4 |
| 0x4566e0 | 145 | fastcall | **Reset/PlaceCamera()**: `0x4578c0(&eye@+0x1f0, &look@+0x1fc)` and initialise +0x208/+0x214 from the car position (two `Vec3 -` 0x414330), then `0x456b10(0)` | 0x456780, 0x4569e0, many RaceSession sites (0x4937d5, 0x493a9f, 0x493c5f, 0x494b87, 0x494feb), 0x46895f |
| 0x456780 | 157 | thiscall `ret 4` | **SetView(int view)**: `car->SetCockpit(...)` when needed (0x4691e0), store +0x1d4 (view 0..8), clear +0x1e4..+0x1ec, `0x4566e0` | RaceSession (0x4937ce, 0x493951, 0x493cd8, 0x494b80, 0x494d40..0x494e2f: camera keys) |
| 0x456820 | 442 | fastcall `ret 4`, SEH (EH 0x4da730), FPU | **Update1(float dt)** (guess): the cheap per-frame follow: eye/look-at from `0x4578c0`, stiffness `preset.stiffness` with `pow(…, dt)` (0x4c19f0), blend with `1.0 - x`, accumulate onto +0x208/+0x214 via `Vec3 operator*` (0x40b0c0), `operator+` (0x419f00), writes +0x1f0/+0x1fc. The byte-exact body preserves the original x87 stack scheduling in the final smooth-look update. | Car::Update **0x45ba95** only (Car calls it with `push dt`) |
| 0x4569e0 | 304 | fastcall, SEH (EH 0x4da748) | **ScanZones()**: only when +0x1e4: find the camera zone at the car position (`0x414870(stage)`), confirm it for 5+ frames (+0x1d8), then pick a view: `rand()&0xff > 0x80` -> 8, else `rand() % 9`; local-only rule (`!local && view==8` -> 7); copy `zone+0x130`; `0x4566e0` | Car SetState 0x45b370 |
| 0x456b10 | 3489 | thiscall `ret 4`, SEH (EH 0x4da7be), FPU 647 insns | **Update2(arg)** (guess: the full camera step): saves the previous matrix (+0x144), computes the Car-relative eye/look-at from the preset (rotating by body matrix at Car body +0x20/+0xb4/+0xb8 etc.), smooths with preset stiffness and the clamps above, stage-geometry occlusion with `0x484380` (vector erase), `0x411520` (CollectNear tris), `0x418b90`, `0x4203b0`, `0x4b55e0` (ViewMat::Concat), and writes MirrorView position/matrix (+0x14, +0x20..) and FOV (+0x12c) | Car::Update 0x45b4c8, 0x456764 (SetView path) |
| 0x4578c0 | 588 | thiscall `ret 8`, FPU, no frame beyond ebp | **GetCameraOffsets(Vec3* eye, Vec3* look)** (guess): view==6: both = preset transformed by the full car matrix (+0x20 rows) plus car pos (+0x14); other views: yaw-only (velocity heading from body +0x40/+0x48, or `±` +0x30/+0x38 when speed below 0.001 (0x4e1398)) rotation of the preset offsets; `Car == NULL` returns (0,0,100.0 (0x42c80000)) and (0,0,0) | 0x456700 (0x4566e0), 0x456873 (0x456820) |
| 0x457b10 | 1599 | stdcall, SEH (EH 0x4da7f8) | **LoadCameraSettings()**: reads `data/settings/camera.txt` into the 9 presets (section 1.2), error path "Missing variables in file: ", copies Camera7 vec to 0x508c98 | RaceSession ctor 0x491fd1 |
| 0x458150 | 100 | thiscall, ignores ECX (Vec3* arg, `ret 4`) | **SetFixedCamera(const Vec3* p)**: sets preset 6 (Camera7) fields: +0x00 = p + (0x508c98..a0), +0x0c = p, +0x18 = p (copies for 0x508d68..0x508d7c). Original loads `p.x` onto x87 before adding the x offset, copies y/z through EDX/ECX, then applies y/z offsets. | RaceSession 0x4936e4, 0x494fd8 |
| 0x4581c0 | 48 | `_$E` stub | guard bit 0x5826d5 + `atexit(0x402c90)` | init table |

Compiler-generated: 0x456600 (scalar deleting dtor), 0x4563f0/0x456410/0x456420/0x456460/0x456470/0x456480 (static array init/dtor plumbing), 0x4581c0 stub.

Suggested decompile groups (M1):
- **G1 (~1.6 KB)**: statics (0x4563e0 group, 0x456450 group) + 0x456490, 0x456600, 0x456620, 0x456660, 0x4566e0, 0x456780, 0x4569e0 (small SEH functions; first fix the class layout; `0x456490` needs the `vector::reserve` inline reproduced). Then 0x456820 (442 bytes, FPU) as its own item at the end of G1.
- **G2 (3489 bytes, alone)**: 0x456b10 (hardest; FPU-heavy; depends on layout of preset table and Car body fields).
- **G3 (~2.3 KB)**: 0x4578c0 (588), 0x457b10 (1599; strings, Config helpers, inline std::string), 0x458150 (100).

## 2. M2: Car collision members, 0x4581f0 - 0x4586ff

Class `Car` (CarObject, include/unk_45e480.h / unk_481390.h). All three are thiscall on the Car and live in the Car TU family. They use Car fields: `[+0x24]` body (CarBody*), `[body+0x234]` box data pointer, `[body+0x14..0x1c]` position, `[body+0x20..0x58]` matrix, **Car+0x1464 `CarTrace vec1464`** (vector of 0x34-byte records; limit 0x11 at +0x1494), **Car+0x14f4** int collision counter, **Car+0x14f8** int flag, **Car+0x14fc Stage\***.

| Addr | Size | Conv / frame | Role (guess marked) | Main callers |
|---|---|---|---|---|
| 0x4581f0 | 923 | fastcall, SEH (EH 0x4da85b), FPU 164 insns | **Car::FUN_004581f0 = RegisterBounds()**: reads the body's box (`[body+0x234]`: 6 floats, min/max), enlarges it by 1.0 (0x4e1348) / 1.5 (0x4e1530, y), transforms the 8 box corners by the body matrix and position (3x3 + translation), builds a world AABB (sign-bit tests for min/max), pads it by 0.5 (0x4e138c) and pushes 2 corner vectors (`0x498300` ptr-copy + `0x403140` erase on `Car+0x1464`), sets `[Car+0x1494] = 0x11`, then calls `Stage::0x4114c0(&vec1464, &aabb)` on `Car+0x14fc` | Car code 0x45c209, 0x45d9c8 (SetState), 0x465a67 |
| 0x458590 | 246 | thiscall `ret 4`, SEH (EH 0x4da878), FPU 18 | **Car::FUN_00458590(Car* other)** = CollideWith: squared distance of the two body positions < **1000.0 (0x4e13a0)**; then box overlap `0x4ad1d0(boxA+4, boxB+4, &matA, &matB, ...)` and body collision `CarBody::0x41d7d0(this body, other body, 1)`; on a hit increments `Car+0x14f4` on both cars; returns 0 | RaceMode loops 0x480c19, 0x481d1b, 0x48f45e |
| 0x458690 | 62 | fastcall | **Car::FUN_00458690 = ApplyCollisions()**: if `Car+0x14f4 > 0`: `Car+0x14f8 = 1`, `body->0x41fc60(this, Car+0x14fc stage)`, `body->0x41c010()`, `Car+0x14f4 = 0` | 0x480c52, 0x481d60, 0x48f497, 0x48f83d |
| 0x4586d0 | 48 | `_$E` stub | module end | init table |

`Car::FUN_00458590` at `0x458590` is byte-exact in `src/unk_4581f0.cpp`. Plain PE disassembly observation: the function keeps `this` in EBP, the own and other body pointers in ESI/EDI, and the own box pointer in EBX; it subtracts the two body positions and checks squared distance against `0x4e13a0`, calls `0x4ad1d0` for box overlap, then `0x41d7d0` for body collision. A successful collision increments the hit counters at `+0x14f4` on both cars. Focused matcher: 4/4 marked functions match in this file (1 existing NONMATCHING).

Decompile group: **one group (~1.2 KB)**, order 0x458590, 0x458690, 0x4581f0 (the last is the FPU-heavy one with 8 conditional min/max updates per axis; `test eax,0x80000000` against the float sign bit is how VC6 compiled an inlined `if (x < 0)`-by-sign-bit trick; reproduce with explicit sign-bit tests on `*(unsigned*)&x`).

## 3. M3: CarInput + Car::FUN_004590b0, 0x458700 - 0x45922f

Class **`CarInput`** (= `Controls` in include/unk_495640.h; fed by `T_56b0b4` (the DirectInput controller manager, global 0x56b0b4)). Created by RaceSession (`new CarInput(DAT_0056b0cc, &pad[4])`; src/unk_491fb0.cpp) at 0x4927c7 / 0x49347c; destroyed at 0x492a4b / 0x49358c. **Size 0x7c**, non-polymorphic, no SEH.

Fields (offset: use), all seen in this range:
- +0x00 int = 1 after construction (constructed flag).
- +0x04 steering mode: **1 = analog** (set by SetSteering 0x458950), 0 = digital (set by 0x4589b0 / 0x4589d0). +0x08 throttle mode (1 analog via 0x458970, 0 digital via 0x4589f0). +0x0c brake mode (1 analog via 0x458990, 0 digital via 0x458a10).
- +0x10 brake digital pressed; +0x14 throttle digital pressed; +0x18 steer-left digital; +0x1c steer-right digital; +0x20 handbrake digital (0x458a30); +0x24 second button (0x458a40; header: used by `Poll`).
- +0x28, +0x2c one-shot request flags (set to 1 by 0x458a50 and 0x458a60; header calls +0x2c `cameraButton`; +0x28 is the shift-up/other one-shot, guess); +0x30 `resetButton` (0x458ab0); all three cleared by 0x458940.
- +0x34 int = -10 (0xfffffff6) "no gear request" sentinel (set in 0x458ad0 and 0x458b30); +0x38 `step` (followed-player step, += arg in 0x458ac0; 0 after reset).
- +0x3c float 1.0 (ctor); +0x40 throttle value 0..1, +0x44 brake value 0..1, +0x48 handbrake value 0..1 (all smoothed from the digital flags in Update with rates 4.0/10.0 (throttle up/down), 5.0/5.0 (brake), 2.0/5.0 (handbrake), consts 0x4e1bac..0x4e1bc0, times dt seconds), +0x4c steering value (analog).
- +0x50 int (ctor 0, reset 0). +0x54, +0x58 floats: gear-shift repeat timers (decremented by dt in 0x4590d0; set to 1.4 (0x3fb33333) after an up/down shift). +0x5c int (force-feedback timer A), +0x60 int (timer B, init **-50 = 0xffffffce**), +0x64 pointer to a helper object destroyed in the dtor with `0x4959a0` + delete (guess: the `T_56b0b4`-owned device object, or the object created by 0x497010; the dtor frees it only if non-null), +0x68 float "FFB gain/decay" 0..1.
- +0x6c, +0x70, +0x74, +0x78: `ForceEffect*` (4 effects created in the ctor via `T_56b0b4::CreateEffect(0x400, type, 1)` with types 4, 8, 0x40, 0x40: stored +0x70 (type 8? the order is: first call type 4 -> +0x70, second type 8 -> +0x6c, then type 0x40 -> +0x74, type 0x40 -> +0x78)). Destroyed with `0x497d10` (ForceEffect::Stop) in the dtor.

Force-feedback setup in the ctor (only if global 0x56b0b4 != 0 and `[0x501408]` (FFB enabled) != 0): `0x497010` (Refresh), `0x497a90(0x400, [0x50140c] * 25)` (force gain; `x*5*5*4` via lea chain), `0x497b20(10000 - [0x501410]*70)` (saturation), `0x497ba0([0x501414]*25)` (dead zone), then the four `CreateEffect` calls.

Statics / globals touched: 0x56b0b4 (T_56b0b4*), 0x501408 FFB enable, 0x50140c FFB gain (default 0x32 = 50), 0x501410 (0x32), 0x501414 (5), and three UI toggles flipped by 0x458a70 (0x501308 = **mirror enabled**, `x = !x`), 0x458a90 (0x50130c, `^= 1`) and 0x458aa0 (0x501310 = **HUD gauges enabled**, `^= 1`).

| Addr | Size | Conv / frame | Role | Main callers |
|---|---|---|---|---|
| 0x458700 | 286 | fastcall `ret 8` (ignores both args except as the ctor signature), no SEH | **CarInput::CarInput(HWND, void*)**: field init (see layout), FFB setup, finally `Reset()` 0x458880 | 0x4927c7, 0x49347c |
| 0x458820 | 91 | fastcall | **~CarInput()**: `ForceEffect::Stop` (0x497d10) on +0x70/+0x6c/+0x74/+0x78, `T_56b0b4::Unacquire` (0x497060), delete +0x64 via 0x4959a0 | 0x492a4b, 0x49358c |
| 0x458880 | 80 | fastcall | **Reset()**: `StopEffects()` (0x4588d0) + `ClearOneShots()` (0x458940), then zero +0x4,+8,+0xc,+0x10,+0x14,+0x18,+0x1c,+0x20,+0x24,+0x38,+0x40,+0x44,+0x48,+0x4c,+0x50,+0x54,+0x58,+0x5c and +0x60 = -50 | ctor 0x458812 (tail), none external |
| 0x4588d0 | 98 | fastcall | **StopEffects()**: `ForceEffect::SetParameters(0,0,0)` (0x4974c0) on the four effects, zero +0x68/+0x5c, +0x60 = -50 | 0x458883, RaceSession (0x4927dd, 0x493492, 0x493a0c, 0x493b07, 0x494be4) |
| 0x458940 | 12 | fastcall | **ClearOneShots()**: zero +0x28, +0x2c, +0x30 | 0x45888a, RaceSession (0x493959, 0x493d7d, 0x4942a7, 0x494312, 0x494738, 0x494815) |
| 0x458950 | 17 | thiscall `ret 4` | **SetSteering(float)**: `[+4]=1; [+0x4c]=v` | T_56b0b4::Poll 0x496cfc |
| 0x458970 | 17 | thiscall `ret 4` | **SetThrottle(float)**: `[+8]=1; [+0x40]=v` | 0x496e06, 0x496e50 |
| 0x458990 | 17 | thiscall `ret 4` | **SetBrake(float)**: `[+0xc]=1; [+0x44]=v` | 0x496e26, 0x496e30 |
| 0x4589b0 | 17 | thiscall `ret 4` | steer-left digital: `[+4]=0; [+0x18]=v` | 0x496e5d, 0x497003 |
| 0x4589d0 | 17 | thiscall `ret 4` | steer-right digital: `[+4]=0; [+0x1c]=v` | 0x496e6a, 0x496ffb |
| 0x4589f0 | 17 | thiscall `ret 4` | throttle digital: `[+8]=0; [+0x14]=v` | 0x496e77 |
| 0x458a10 | 17 | thiscall `ret 4` | brake digital: `[+0xc]=0; [+0x10]=v` | 0x496e84 |
| 0x458a30 | 10 | thiscall `ret 4` | `[+0x20]=v` (handbrake digital) | 0x496e8e |
| 0x458a40 | 10 | thiscall `ret 4` | `[+0x24]=v` | 0x496e98 |
| 0x458a50 | 8 | fastcall | `[+0x28]=1` (one-shot) | 0x496ebf |
| 0x458a60 | 8 | fastcall | `[+0x2c]=1` (one-shot, camera button per header) | 0x496eb2 |
| 0x458a70 | 19 | stdcall | `[0x501308] = ![0x501308]` (mirror toggle; 19 bytes because of the `sete`) | 0x496ecc |
| 0x458a90 | 8 | stdcall | `[0x50130c] ^= 1` | 0x496ef7 |
| 0x458aa0 | 8 | stdcall | `[0x501310] ^= 1` (HUD gauges toggle) | 0x496f04 |
| 0x458ab0 | 8 | fastcall | `[+0x30]=1` (reset button) | 0x496ea5 |
| 0x458ac0 | 15 | thiscall `ret 4` | `[+0x38] += step` | 0x496edb, 0x496eea |
| 0x458ad0 | 89 | fastcall | **Reacquire()**: `[+0x38]=0; [+0x34]=-10`; if `0x56b0b4 && T_56b0b4::Poll(this, 1)` then `ForceEffect::Start` (0x497d40) on the four effects | RaceSession 0x493db5 |
| 0x458b30 | 1393 | thiscall `ret 0xc`, FPU 172, no SEH | **CarInput::Update(Car* car, int dtMs, int flag)** (header name `FUN_00458b30`): decays +0x68; clamps; computes FFB torque/force from Car wheel/spring forces (Car+0x338, +0x668, +0x998, +0xcc8, +0x2cc, +0x5fc, +0xd00..0xd4c: spring/damper values per wheel) and the body speed (`car->body+0x1cc` velocity), drives the four effects with `ForceEffect::SetParameters` (0x4974c0) only when the timers +0x5c/+0x60 expire (period 100 ms via `% 0x64`); `T_56b0b4::Poll(this,1)` (0x496b30) re-start effects on reacquire; smooths digital inputs into analog values (+0x40/+0x44/+0x48 clamped 0..1); writes the Car outputs: `Car+0x134c = steer` and `Car+0x1350 = 1` (analog) or `Car+0x1344 = ±1/0` and `Car+0x1350 = 0` (digital), `Car+0x1338 = throttle`, `Car::0x45da60(brake)` (SetBrake), `Car+0x1340 = handbrake`; if `flag == 0` then handles gears: `+0 != 0` -> 0x4590d0 else 0x4591c0 (both tail-called through `ret 0xc`) | RaceSession 0x493d98 |
| 0x4590b0 | 24 | fastcall | **Car::FUN_004590b0 (replay hook)**: `esi = ecx+0xdb8` (Drivetrain); `ShiftTo(SelectGear())` = `0x46b360(0x46b020())` | Car::Update 0x45b91e (when `Player+0x40 == 1`) |
| 0x4590d0 | 237 | thiscall `ret 8`, FPU 28 | **CarInput::FUN_004590d0(Car*, float dt)** = gear shift step for the *human* path (guess): subtract dt from the two repeat timers +0x54/+0x58; if `car` set and body speed² (car->body+0x1cc) < 100.0 (0x4e13d4) then shift to gear -1 (`0x46b360(-1)`, reverse/neutral) and clear timers; otherwise compare `drive.SelectGear()` (0x46b020) with `[Car+0xf8c]` and do `ShiftTo` up/down when the matching timer is positive; timers set to 1.4; negatives clamped to 0 (sign-bit test `& 0x80000000`) | CarInput::Update 0x45907e |
| 0x4591c0 | 52 | thiscall `ret 8` | **CarInput::FUN_004591c0(Car*, int)** = gear shift for the *request* path: `if [+0x38] != 0 drive.ShiftBy([+0x38])` (0x46b280); `if [+0x34] != -10 drive.ShiftTo([+0x34])` (0x46b360) | CarInput::Update 0x459093 |
| 0x459200 | 48 | `_$E` stub | module end | init table |

Unclear: which of +0x28/+0x2c is "shift up"/"shift down" vs. camera; header include/unk_481390.h already names +0x2c `cameraButton`, +0x30 `resetButton`. The gear request path uses +0x34 (target gear, -10 = none) and +0x38 (relative step), not the one-shot flags.

Compiler-generated here: nothing besides the stub; all 0x458950-0x458ab0 members are normal one-line members (not templated; the 6- and 5-byte thunk pattern of 0x480740/0x480770 does not occur).

Decompile groups (M3):
- **G4 (~1.1 KB, easy)**: 0x458700, 0x458820, 0x458880, 0x4588d0, 0x458940, 0x458950..0x458ac0 (17 tiny members), 0x458ad0, 0x4590b0.
- **G5 (1.7 KB)**: 0x458b30 (1393, FPU) + 0x4590d0 (237) + 0x4591c0 (52). Do 0x458b30 as its own work item; 0x4590d0 and 0x4591c0 are quick. Check the exact `fild` / `fistp` rounding, `0x4c19bc` (`_ftol`) and the integer divisions by 100 (`idiv` at 0x458bcb, 0x458be8).

## 4. M4: Car::Draw459270 (environment/cube-map render), 0x459230 - 0x45988f

- **Static**: `DrawContext` at **0x508dc8** (0x194 bytes, see `DrawContext` in include/unk_481390.h). Init 0x459230: `call 0x459240; jmp 0x459250`. 0x459240 (10 bytes, stdcall): `mov ecx,0x508dc8; jmp 0x4b7a40` (DrawContext ctor). 0x459250 (12 bytes, stdcall): `atexit(0x459260)`. 0x459260 (not in csv, 10 bytes): `mov ecx,0x508dc8; jmp 0x4b7bf0` (DrawContext dtor). Fields of this static read/written here: 0x508eec (copied to 0x508ef0), 0x508ef4 = 0, 0x508f50/54/58 (= stage +0x1c4/+0x1c8/+0x1cc: fog colour; these are `unk188..unk190` of the DrawContext).
- **0x459270** (1492 bytes, thiscall `ret 8`, SEH EH 0x4da8c9, FPU 52, frame 0x2a0): `Car::Draw459270(void* view, int arg)` (existing decl `Car::Draw459270(int a, void* view)` in include/unk_469630.h has the two parameters in this order; check against the stack: first stack arg (ebp+8) is dereferenced as a view with matrices at +0x20 and +0x60 (MirrorView layout), the second (ebp+0xc) is passed as `ecx` to `DrawContext::0x4b8340`, so the arguments are (MirrorView\* camera, DrawContext-ish `this` or int)). Skips the whole function when `Car+0x1bce4` (render target / PolyObject, `obj1bce4`) is null. Flow: save the device viewport via renderer vtable (`[0x56b0ac]` slot 0x3c), `slot 0x74` (begin scene); construct a **local MirrorView** (0x4b77b0) and copy the camera's two 16-dword matrices into it; local FOV = 1.5707963 (pi/2, 0x3fc90fdb), near 0.5, far 50.0; compute the car's reflection position (`Car body +0xa0..+0xac` position plus `+0x90/0x94` offsets, times 0.5, transformed by the body matrix); `0x411170(stage, &localView, 0x508dc8)` (Stage setup, name `FUN_00411170(view, context)`), copy fog values, `0x4b8310(0)`; save/set render target 0xb via vtable slots 0x88/0x84; **frame counter `Car+0x1428`**: incremented each call; when > 0 renders 2 faces on even frames (else 1) – `n = 2 or 1` – and the face start index cycles with `Car+0x1bce4->+0x50` (a cursor, init 1; if 0 it is set to 1 and the face to 0); **6 faces** (`cmp ecx,6`): a jump table `jmp [edi*4+0x459844]` selects the direction pairs (+x, -x, +y, -y, +z, -z: forward and up vectors from constants ±1.0 = 0x3f800000/0xbf800000) at 0x459547..0x4595ee; each face: begin (vtable 0x7c with `Car+0x1bce4`, face, 1, 1), clear (slot 0x60, 2), build the camera frame via `Vec3 cross 0x4203e0`, fill the local MirrorView matrix, `ViewMat::Concat` (0x4b55e0), `device->SetTransform(view)` (slot 0xc0), `SetProjection(fov, near, far, aspect)` (slot 0xcc, args 1.0, far, near, fov, 1), render slot 0x84(0xd, 0), `DrawContext::0x4b7c80(device, localView)`, and if the device `slot 0x6c` returns true: `Stage::0x411350(device, localView)`, `DrawContext::0x4b8160(device, localView)`, `slot 0x70` (end face); at the end `slot 0x78`, restore target 0xb, restore viewport (slot 0x38), `DrawContext(0x4b8340)`; destroy the local MirrorView (0x4b78e0). The jump table at **0x459844** (6 dwords = code addresses 0x459547, 0x459550, 0x45956a, 0x459596, 0x4595c2, 0x4595d9) sits inside the function range as data, which is why raw disassembly shows garbage after 0x459844 (that is data, then the stub at 0x459860).
- Main caller: **Car::UpdateRenderTransforms 0x469630 (0x4697ad)** (range-4666a0.md).

| Addr | Size | Conv / frame | Role | Main callers |
|---|---|---|---|---|
| 0x459230 | 10 | init | static-init: `call 0x459240; jmp 0x459250` | init table |
| 0x459240 | 10 | stdcall | `DrawContext::DrawContext()` on 0x508dc8 | 0x459230 |
| 0x459250 | 12 | stdcall | `atexit(0x459260)` | jmp from 0x459235 |
| 0x459260 | 10 | atexit cb (not in csv) | `DrawContext::~DrawContext()` on 0x508dc8 | atexit |
| 0x459270 | 1492 | thiscall `ret 8`, SEH (EH 0x4da8c9), FPU 52, switch(6) | **Car::Draw459270** (cube-map faces) | 0x4697ad |
| 0x459860 | 48 | `_$E` stub | module end (the next function in functions.csv, 0x459890, is the first of range-459890.md) | init table |

Decompile group: **G6 (1492, alone)**: 0x459270 plus the 3 plumbing functions (need the vtable slot names of the renderer class in `RenderDevice`/`DAT_0056b0ac` and include/unk_469630.h; the switch needs a dense `switch (face)` with six cases so VC6 emits the `cmp 5 / ja default / jmp [table]` form).

## 5. Summary table of every function (functions.csv + non-csv helpers)

| Addr | Size | Module | Short role |
|---|---|---|---|
| 0x4563e0 | 10 | M1 | static init (camera table) |
| 0x4563f0 | 25 | M1 | vec-ctor of g_cameras[9] |
| 0x456410 | 12 | M1 | atexit(0x456420) |
| 0x456420 | 40 | M1 | atexit dtor of g_cameras (non-csv) |
| 0x456450 | 10 | M1 | static init (empty object) |
| 0x456460 | 1 | M1 | empty |
| 0x456470 | 12 | M1 | atexit(0x456480) |
| 0x456480 | 1 | M1 | empty (non-csv) |
| 0x456490 | 355 | M1 | ViewManager ctor |
| 0x456600 | 30 | M1 | scalar deleting dtor (vtable[0]) |
| 0x456620 | 59 | M1 | ~ViewManager |
| 0x456660 | 121 | M1 | SetTarget(Car*, Stage*) |
| 0x4566e0 | 145 | M1 | PlaceCamera/Reset |
| 0x456780 | 157 | M1 | SetView(int) |
| 0x456820 | 442 | M1 | Update (follow, dt) |
| 0x4569e0 | 304 | M1 | zone scan / auto view switch |
| 0x456b10 | 3489 | M1 | full camera update |
| 0x4578c0 | 588 | M1 | GetCameraOffsets |
| 0x457b10 | 1599 | M1 | LoadCameraSettings |
| 0x458150 | 100 | M1 | SetFixedCamera(Vec3*) |
| 0x4581c0 | 48 | M1 | `_$E` stub |
| 0x4581f0 | 923 | M2 | Car::RegisterBounds |
| 0x458590 | 246 | M2 | Car::CollideWith(Car*) |
| 0x458690 | 62 | M2 | Car::ApplyCollisions |
| 0x4586d0 | 48 | M2 | `_$E` stub |
| 0x458700 | 286 | M3 | CarInput ctor |
| 0x458820 | 91 | M3 | ~CarInput |
| 0x458880 | 80 | M3 | Reset |
| 0x4588d0 | 98 | M3 | StopEffects |
| 0x458940 | 12 | M3 | ClearOneShots |
| 0x458950 .. 0x458a10 | 17 each (7) | M3 | analog/digital setters |
| 0x458a30, 0x458a40 | 10 each | M3 | setters |
| 0x458a50, 0x458a60 | 8 each | M3 | one-shot flags |
| 0x458a70, 0x458a90, 0x458aa0 | 19, 8, 8 | M3 | option toggles (mirror/?/HUD) |
| 0x458ab0 | 8 | M3 | reset one-shot |
| 0x458ac0 | 15 | M3 | step += |
| 0x458ad0 | 89 | M3 | Reacquire |
| 0x458b30 | 1393 | M3 | CarInput::Update |
| 0x4590b0 | 24 | M3 | Car::FUN_004590b0 (gear resync) |
| 0x4590d0 | 237 | M3 | shift step |
| 0x4591c0 | 52 | M3 | shift request |
| 0x459200 | 48 | M3 | `_$E` stub |
| 0x459230 | 10 | M4 | static init (DrawContext) |
| 0x459240 | 10 | M4 | DrawContext ctor thunk |
| 0x459250 | 12 | M4 | atexit register |
| 0x459260 | 10 | M4 | DrawContext dtor thunk (non-csv) |
| 0x459270 | 1492 | M4 | Car::Draw459270 |
| 0x459860 | 48 | M4 | `_$E` stub |

48 functions.csv entries: M1 = 16 (4563f0, 456410, 456460, 456470, 456490, 456600, 456620, 456660, 4566e0, 456780, 456820, 4569e0, 456b10, 4578c0, 457b10, 458150), M2 = 3, M3 = 26, M4 = 3 (0x459240, 0x459250, 0x459270); total 48.

Non-csv items to keep in the TU: the stubs 0x4581c0, 0x4586d0, 0x459200, 0x459860, the init entries 0x4563e0, 0x456450, 0x459230, atexit callbacks 0x456420, 0x456480, 0x459260.

## 6. Work groups (1.5-3 KB each, big functions alone)

| Group | Functions | Bytes | Notes |
|---|---|---|---|
| G1 | statics 0x4563e0/0x456450 + 0x456490, 0x456600, 0x456620, 0x456660, 0x4566e0, 0x456780, 0x4569e0 | ~1.3K | ViewManager layout first; ctor has inlined vector::reserve; two SEH tables |
| G1b | 0x456820 | 442 | FPU; needs `pow` (0x4c19f0) |
| G2 | 0x456b10 | 3489 | alone; hardest; depends on G1/G3 layout |
| G3 | 0x4578c0, 0x457b10, 0x458150 | 2287 | loader uses Config helpers + inline std::string/throw; rdata strings above |
| G4 | 0x4581f0, 0x458590, 0x458690 | 1231 | Car members; sign-bit trick, AABB build |
| G5 | 0x458700, 0x458820, 0x458880, 0x4588d0, 0x458940, 0x458950-0x458ac0, 0x458ad0, 0x4590b0 | ~1.1K | CarInput small functions; very easy |
| G6 | 0x458b30 (+ 0x4590d0, 0x4591c0 as quick follow-ups) | 1393 (+289) | FPU; FFB computations |
| G7 | 0x459270 + plumbing 0x459230..0x459260 | ~1.5K | switch(6) jump table; renderer vtable calls |

Compiler-generated / trivial: scalar deleting dtor 0x456600; vector ctor/dtor iterator plumbing 0x4563f0/0x456410/0x456420; empty functions 0x456460/0x456480; the four `_$E` stubs; the DrawContext thunks 0x459240/0x459260 (`mov ecx,imm; jmp`, produced by a global object with a non-inline ctor/dtor). No 5- or 6-byte vtable thunks (like 0x480740/0x480770) occur in this range.

## 7. Dependencies and hints for matching

- `Car` members used: `body` +0x24 (CarBody; position +0x14, matrix +0x20, velocity +0x1cc, box pointer +0x234), view pointer +0x20, `drivetrain` +0xdb8 (`SelectGear` 0x46b020, `ShiftBy` 0x46b280, `ShiftTo` 0x46b360), inputs +0x1338..+0x1350, `vec1464` +0x1464, collision counters +0x14f4/+0x14f8, stage +0x14fc, frame counter +0x1428, env target +0x1bce4, cursor `[+0x1bce4]->+0x50`.
- Existing declarations: `ViewManager` (include/unk_481390.h:1462; missing the base class `MirrorView`, a `local` byte at +0x1dc is already there), `CarInput` (1541; needs the real layout above), `DrawContext` (1511), `Controls` (unk_495640.h), `Car::FUN_004581f0/00458590/00458690/004590b0`, `Car::Draw459270` (unk_469630.h:134).
- The camera table is a file-scope array of 9 non-POD elements (empty ctor/dtor). Reproduce with a struct that has an empty user ctor and dtor (e.g. members of a class like `Vec3` whose ctor is the empty `0x490ad0`), otherwise the `0x4c11f4` iterator and the stub at 0x456420 will not appear.
- Use of `T_56b0b4` methods listed in include/unk_495640.h (`Poll`, `Refresh`, `Unacquire`, `SetForceGain`, `SetSaturation`, `SetDeadZone`, `CreateEffect`), `ForceEffect` (`Stop`, `Start`, `SetParameters`).
