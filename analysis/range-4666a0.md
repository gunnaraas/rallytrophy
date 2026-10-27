<!-- Subagent analysis, 2026-10-23; read-only disassembly; guesses are marked. -->
# Range 0x4666a0 - 0x46a89f: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py plus an annotating wrapper (string/float immediates), refs.py for callers, .rdata reads. "Guess" marks inference. EH handler addresses are the `push imm` after `push -1` in each SEH prologue.

## 0. Headline findings

1. **The whole range is member functions of one class, `Car`** (the 0x1f3xx-byte object that owns the drivetrain at +0xdb8 and the 4 wheels at +0x2c0, stride 0x330). Every function except the three float helpers (0x468410/0x4684b0/0x468500) and 0x46a790 takes `this` = Car* in ecx (thiscall/fastcall). Callers are the Car physics/update (0x45b4xx-0x45bcxx), Car ctor/dtor (0x45ef7c, 0x461e24), camera/view code (0x456678, 0x4605e3, 0x4886xx-0x489xxx) and RaceMode base vtable thunks (0x48e630-0x48e830).
2. **Four source files**, each closed by the `_$E` guard stub (guard bit 0x5826d5 + `atexit(0x402c90)`); the previous stub is at 0x466670, so file starts are 0x4666a0, 0x468720, 0x468c40, 0x469630. The stubs are not in functions.csv.
3. **No vtables, no virtual functions defined here.** No function in the range is referenced as data except the four stubs (init-table entries at file offsets 0xefbdc/e0/e4/e8). The only indirect calls are *out*: RaceMode global 0x501450 slot 13 (`[vtbl+0x34]`, 0x468720), renderer global 0x56b0ac slots 0x8c/0x108/0x10c (0x46a200), a render object `[obj+0x14]` (0x469a00).
4. **No out-of-line STL/template bodies and no scalar deleting dtors in the range.** The only template code is an inlined `vector<Car*>::push_back` (grow path via `operator new` 0x4c1276 / `operator delete` 0x4bd000, copy with `rep movs`) at the end of 0x468c70, plus the vector reads in 0x469e20. VC6 `vector` vec-ctor/dtor helpers 0x4c11f4 / 0x4c1116 (`__vec_ctor`/`__vec_dtor`, callbacks 0x490ad0 = empty Vec2 ctor, 0x402c90 = empty dtor) appear in 0x46a200 for two local `Vec2[4]` arrays.
5. **No file-scope statics** in any of the four files (no initialiser pair at the start of a file, stubs only).
6. No thunk functions in the range. (The small 6-/5-byte thunks near 0x480740/0x480770 are not in this range.)
7. The module boundaries match what a plain "Car" split would give: **sound** (0x4666a0), **race state / respawn / checkpoint** (0x468720), **model visibility & cockpit/damage views** (0x468c40), **render + effects + gauges** (0x469630). Treat them as four .cpp files sharing the Car header (the Car class lives in include/unk_481390.h and unk_46a8a0.h etc.; new members are listed per module below).

| # | Range | Guard stub | Content | Suggested file |
|---|---|---|---|---|
| F1 | 0x4666a0 - 0x46871f | 0x4686f0 | Car sound setup (Sounds.ini), teardown, per-frame engine/exhaust/crash sound update, surface-change sound | src/unk_4666a0.cpp |
| F2 | 0x468720 - 0x468c3f | 0x468c10 | Car stuck detection/respawn, checkpoint/lap tracking, wheel-trail flag set/clear | src/unk_468720.cpp |
| F3 | 0x468c40 - 0x46962f | 0x469600 | Car model part visibility, view switches (cockpit/damage), alpha | src/unk_468c40.cpp |
| F4 | 0x469630 - 0x46a89f | 0x46a870 | Car render matrices, lighting, dust/spray particle emission, wheel weights, tyre-track update, dashboard/gauge drawing | src/unk_469630.cpp |

Next file after the range starts at 0x46a8a0 (drivetrain, see range-46a8a0.md).

## 1. Car members seen in this range (offsets in Car; "S" = seen; new vs. unk_481390.h/unk_46a8a0.h)

Known/earlier: +0x24 CarBody* (pos +0x14, matrix +0x20..+0x5c, +0x60..+0x9c second matrix, velocity +0x1cc/+0x1d0/+0x1d4); +0x2c0 wheels[4] stride 0x330 (flag word at +0x34 per wheel, bit 2 set/cleared by 0x4691e0; wheel +0x280 within 0x330 stride = slip/force value read in 0x46a000); +0xdb8 Drivetrain (GetRpmFraction 0x46b1a0, GetSmoothedRpm 0x46ba60, 0x46b000 = max gear, see range-46a8a0.md); +0x1338 float throttle; +0x142c int; +0x14fc Stage*; +0x1becc Player*.

Seen here (all guesses for names):
- **Stage tables** indexed by the per-wheel surface id `Car+0x1e44[i]` (int[4]): Stage+0x33c (per-surface counters), +0x51c (dust count), +0x56c (dust/spray effect handle), +0x5bc (spray effect), +0x60c (track-effect id), +0x69c (float scale). Stage+0x130c = checkpoint count/spacing, Stage+0xa0c (stride 0x48) = checkpoint array, Stage+0xa04/+0xa08 = count/array of the Checkpoint list (accessor 0x414fb0, 0x414dc0 finds nearest), Stage+0x24cc used by 0x416980 (surface lookup by position).
- **Model part pointers** (each points at an object with a flag byte at +0xc, bit 0 = hidden): +0xfa0, +0xfa4, +0xfa8 (render lists, 0x468c40), +0xfac/+0xfd4 (two lists of nodes; node has next at +0, data at +8; the data has material-like fields +0x18..+0x9c; used by 0x4693a0/0x4695c0/0x4697c0), +0x1bd30, +0x1bcfc/+0x1bce4/+0x1bce8/+0x1bcc8..+0x1bccc (vector of sub-objects, each with a Vec4 at +0x88), +0x1f18, +0x1f68, +0x1f6c, +0x1ff0, +0x1ff4, +0x1ff8, +0x1ffc, +0x2004, +0x2008, +0x200c, +0x2010, +0x2014, +0x2018, +0x201c, +0x2020, +0x2024, +0x2028, +0x202c, +0x2048, +0x204c, +0x2050, +0x2054, +0x2058, +0x2074..+0x207c (current-view body objects, set from +0x2080..+0x2094 pairs), +0x2098/+0x209c (floats), +0x20ec (render object with vtable; slot 5 at +0x14 called in 0x469a00), +0x1bde8.. (arrays of stride 0x1c, 3x and 2x), +0x1bd94, +0x1beb0, +0x1be9c, +0x1bea0.
- **View/state**: +0x1eb4 int (inside/cockpit view flag), +0x1eb8 int (damage/"model B" flag), +0x1ebc int (index into 12-byte table at 0x4fb0fe, byte [idx*12]), +0x1ec0 int (graphics detail level 0..2), +0x1ec8/+0x1ecc/+0x1ed0..+0x1f0c: dashboard rectangle/needle parameters (floats at +0x1ed0/+0x1ed4 position scale, +0x1ed8..+0x1eec offsets, +0x1ef0..+0x1f08 angles/limits; ints +0x1ec8/+0x1ecc = texture ids chosen by global 0x501300).
- **Wheel-effect objects**: `WheelTrail[4]` at +0x2748, stride 0x6558 (a 0x6558-byte object, class of range-4741a0.md: `0x474600` = start/reset, `0x4746b0` = update). Effect emitters at +0x2100 (0x477b60), +0x222c (0x479490), +0x2358 (0x47c790), +0x2488 (0x47bf80/0x47c1a0), +0x25e8 (0x4786a0) (classes of unk_47c460.h and neighbours: dust, spray, sparks, smoke, debris).
- **Sound block**: see section 2.
- **Race state**: +0x1f114 int, +0x1f118 int *stuck state 0..4* (0 = ok, 1 = no wheel contact and car not "grounded", 2, 3, 4 = progressively different reasons), +0x1f11c float *state timer* (ms), +0x1f120 int *last checkpoint index*, +0x1f144 int *checkpoint progress counter* (crossings so far), +0x1f148 int *race time ms*, +0x1f158 byte *tyre-track enable*, +0x1f15c Surface* (last surface sound object), +0x1f160 (a sound/ message queue object: 0x40b580 push(id,time), 0x40b600 flush(time)), +0x1f208 Surface*[16] ring, +0x1f248 ring head, +0x1f24c crash sound slots (8 x 12-byte {id, aux, timeout}), +0x1f2ac slot head, +0x1f2b0..+0x1f2b8 last-time stamps, +0x1f2bc..+0x1f2d0 pending sound event flags (damage events), +0x1f2d4/+0x1f2e0/+0x1f2f0/+0x1f300 Vec3 positions of crash events, +0x1f2ec/+0x1f2fc ints (volume scalars).
- +0x1bcac float[4] wheel weight/normalised surface mix, +0x1bef4/+0x1befc ints (Car state: 2 = disabled for the random rattle sounds).

## 2. F1: Car sounds, 0x4666a0 - 0x46871f (9 fns + stub)

The sound block is a **Sounds.ini** loader and per-frame updater. Strings (all .rdata around 0x4fdbxx-0x4fdfxx): file `Sounds.ini` (0x4fdf20) opened via a File object (0x405580 ctor, 0x4055a0 dtor, 0x4076c0 open with mode 0x19, 0x409130 section lookup `GetSection(name)`; 0x408170 string key `Get(const char*)`; 0x4085e0 `GetVec3`; 0x407f60 `GetFloat`; 0x408390 `GetVec2`; 0x402700 / 0x4024f0 / 0x406ce0 are CString-like (VC6 `std::string` assign/free) helpers; the `0x4bd000` free at the end is the string buffer release).

Keys read (the fact is in the push/store order): `EngineAttenuation`, `ExhaustAttenuation` (Vec2, applied to the 4 layers' SetRange as two pairs: layers 0.. use engine pair, the exhaust pair for the other bank), `EngineSound1/2/3` + `ExhaustSound1/2/3` (sample file names -> `Sample* = LoadSample(name)` (0x498ec0) and `SoundSource::SetSample` (0x49b790)), `EngineSound1Range/2Range/3Range`, `ExhaustSound1Range/2Range/3Range` (Vec3 ranges stored at +0x1c3b4, +0x1c440, +0x1c4cc and +0x1c5e4, +0x1c670, +0x1c6fc), `EngineLoc`, `ExhaustLoc` (Vec3 at +0x1c718 / +0x1c724), `EngineSoundFadeRange`, `EngineSoundVolume`, `EngineSoundVolumeAcc`, `ExhaustSoundFadeRange`, `ExhaustSoundVolume`, `ExhaustSoundVolumeAcc` (floats copied into each layer), `EngineThrottleRpm` (float +0x1c714). The section name is built from a string index (`[this+0x1becc]` Player/model name). Crash sample paths (hard-coded): `data/soundfx/window_smash1.wav`, `window_smash2.wav`, `light_smash.wav`, `hard_crash1.wav`, `hard_crash2.wav`, `soft_crash1.wav`, `soft_crash2.wav` -> +0x1c298..+0x1c2b0 (7 Sample*).

Layer records: 3 engine layers at Car+0x1c340 + k*0x8c (k=0..2) and 3 exhaust layers at Car+0x1c570 + k*0x8c; each record = {Sample\* at +0x00, SoundSource at +0x04 (0x6c bytes), Range Vec3 at +0x74, fade float +0x80, volume/acc floats near +0x84/+0x88}. The 4-iteration init loop at the top of 0x4666a0 uses stride 0x8c over the same block. Other SoundSources initialised with SetSample(engine sample) + SetVolume(0) + SetRange(0.1, 1.0): +0x1bf04, +0x1bf78, +0x1bfec, +0x1c060, +0x1c0d4 (five 0x74-stride sources, the `Car` ctor already built them: 8 sources stride 0x74), then three crash sources (+0x1c148, +0x1c1b8, +0x1c228; stride 0x70) with range (5.0, 300.0) = 0x40a00000/0x43960000.

Globals: 0x56b0b0 sound system (Miles wrapper, methods 0x498ec0 LoadSample, 0x499340 ReleaseSample, 0x499ce0 AddSource, 0x499ec0 RemoveSource, 0x49a680 `QueueSpeech/PlayOneShot(id, prio, done*)`), 0x50147c (sound-effects enabled), 0x501438/0x50143c (SFX and music volume, ints 0..127), 0x501308/0x50146c (used by helper 0x464ea0).

Constants: 0.01 (0x4e15c8), 0.00390625 (0x4e1e98), 0.63662 double (2/pi, 0x4e1ea0), 1.0 (0x4e1348), 0.0 (0x4e134c), 0x51eb851f magic divide-by-100 (volume scaling `x*vol/100`), 80.0 (crash trigger distance, `Stage::0x417e60(pos,80)`), 5000/10000 ms cool-downs (0x1388/0x2710).

| Addr | Size | Conv / frame | Role | Callers |
|---|---|---|---|---|
| 0x4666a0 | 2361 | thiscall `ret 4`, SEH (EH 0x4dbab8), no FPU math beyond float loads; big | **Car::InitSounds(arg)**: parses Sounds.ini (above), sets up the 3+3 engine/exhaust layers and the crash sources | Car ctor/init 0x45ef7c |
| 0x466fe0 | 221 | fastcall, no frame | **Car::ReleaseSounds**: calls 0x4672c0 (stop), then `ReleaseSample` for 3+3 layer samples and 7 crash samples | Car dtor 0x461e24 |
| 0x4670c0 | 504 | fastcall (ecx=Car), no frame | **Car::StartSounds**: clears crash slots, `SetVolume(0)` + `AddSource` + `Play` for all engine/exhaust layers; for "own car" (`Player+0x38`) also starts the five rotating/wheel sources | RaceMode base vtable thunk 0x48e75b |
| 0x4672c0 | 377 | fastcall, no frame | **Car::StopSounds**: clears crash slots, `Stop()` + `RemoveSource` for all layers and for the own-car extras | 0x466fe3, RaceMode thunk 0x48e79d |
| 0x467440 | 4039 | thiscall (`ret` plain, ebp frame), SEH (EH 0x4dbaf0), FPU heavy | **Car::UpdateSounds**: crash/rattle one-shots (random sample via rand 0x4c1998), per-layer pitch/volume from rpm, throttle and distance curves (0x468410/0x4684b0/0x468500), rpm smoothing 0x46ba60; sets SoundSource position (0x49b880), volume (0x49b7b0), pitch (0x49b820 via `_ftol` 0x4c19bc), range (0x49b840) | RaceMode thunk 0x48e7f0 |
| 0x468410 | 157 | cdecl `float f(float x, Range3* r, float fade, int lowSide, int highSide)`, FPU | **Range fade curve**: 1.0 inside [r.lo, r.hi]; below lo (if lowSide) or above hi (if highSide) falls linearly to 0 over `fade`; returns float in st0 | 0x4677fb, 0x467816, 0x467830, 0x4678d7, 0x4678f2, 0x46790d |
| 0x4684b0 | 74 | cdecl `float f(float x, Curve* c)`, FPU | piecewise-linear 2-point blend: x<=c+0x14 -> c+0x1c; x>=c+0x18 -> c+0x20; between lerps (c+0x1c -> c+0x20) | 0x467dfc, 0x467f59, 0x4680a1, 0x4681e9, 0x468339 |
| 0x468500 | 74 | cdecl `float f(float x, Curve* c)`, FPU | same, using c+0x04/+0x08 limits and c+0x0c/+0x10 values (second curve in the same record) | 0x467e15, 0x467f72, 0x4680ba, 0x468202, 0x468352 |
| 0x468550 | 406 | fastcall, SEH (EH 0x4dbb08), FPU | **Car::UpdateSurfaceSound**: `Stage::0x416980(body pos)` finds the surface zone under the car; if changed and within radius^2 < 100 and |dot| < 0.5 pushes it in the 16-slot ring (+0x1f208/+0x1f248), plays its up-to-4 sound ids via `0x49a680` (id 14/15 get no done pointer); then expires queued ring entries through `0x40b580` | Car update 0x45bc04 |
| 0x4686f0 | 48 | `_$E` stub | guard bit + atexit(0x402c90) | init table (file offset 0xefbdc) |

Decompile: 0x4666a0 alone (long, mostly call sequences; File/string helpers need the exact header classes, SEH states 0..0xa); 0x467440 alone (4 KB, several near-identical blocks; pure `NONMATCHING` risk in FPU); group the rest.

## 3. F2: Car race state / respawn / checkpoint, 0x468720 - 0x468c3f (6 fns + stub)

All Car members; stuck state machine, respawn and trail flag. RaceMode global 0x501450 (virtual method slot 13, `[vtbl+0x34]`, takes Player\*: "respawn/reset this player", see racemode-base.md slot 13 which itself calls 0x4688f0(3)).

| Addr | Size | Conv / frame | Role | Callers |
|---|---|---|---|---|
| 0x468720 | 463 | thiscall `ret 4` (float dt), no frame, FPU | **Car::UpdateStuck(dt)**: counts wheels with ground contact (wheel[+0]!=0), wheels with surface id <5 / <15 and Stage+0x33c surface counters; sets `+0x1f118` state 1/2/3/4 from speed^2 < 1.0 and conditions, accumulates `dt` into +0x1f11c; compares with thresholds in globals 0x501518 (5.0), 0x50151c (1.0), 0x501520 (2.0), 0x501524 (2.0), 0x501528 (0.1); if exceeded calls `RaceMode(0x501450)->vslot13(player)` (reset car to track) | Car update 0x45bc0c |
| 0x4688f0 | 135 | thiscall `ret 4` (int delta), no frame | **Car::RespawnAtCheckpoint(delta)**: checkpoint index = [this+0x1f120] +/- delta (clamped at 0); fetch Checkpoint via `Stage::0x414fb0(idx)` or (when null) `Stage::0x414dc0(pos, delta)`; calls `Car::FUN_0045cc30(start, world)` (reposition; same function as range-46a8a0 notes), then if `[this+0x20]` (physics/helper) `0x4566e0`, and `++[this+0x1f2bc]` (respawn counter) | RaceMode vtable slot 13 (thunk 0x48e63f with delta 3) |
| 0x468980 | 35 | thiscall `ret 4` (int idx), no frame | **Car::RespawnAtIndex(idx)**: `Checkpoint = Stage::0x414fb0(idx)`; `FUN_0045cc30(cp+0x30, cp)` | 0x481196, 0x481f88 (race modes) |
| 0x4689b0 | 532 | thiscall `ret 4` (int ms), no frame, FPU | **Car::UpdateCheckpoints(ms)**: next checkpoint = `Stage+0xa0c + (idx % [Stage+0x130c]) * 0x48`; plane/radius test (dist^2 < [cp+0x44] and signed distances on cp normals +0x30,+0x24 within [0, cp+0x3c] and < 5.0 (0x4e1700)); on pass: `++[this+0x1f144]`, records lap/sector times into Player arrays (+0x1884 times, +0x19f0 sector deltas, +0x1a18 last index, +0x1a1c sector count, +0x1a20 max speed from `sqrt([this+0x1f13c])*3.6` (km/h), +0x1a24/+0x1a50 final marks, +0x30 finished flag) | Car update 0x45ba70 |
| 0x468bd0 | 39 | fastcall, no frame | **Car::EnableTyreTracks**: `[this+0x1f158]=1`; for the 4 WheelTrail objects at +0x2748 (stride 0x6558) call `0x474600` (reset) | Player 0x491880 |
| 0x468c00 | 15 | fastcall, no frame | **Car::DisableTyreTracks**: `[+0x1f158]=0; [+0x1f114]=0` | Player 0x491920 |
| 0x468c10 | 48 | `_$E` stub | | init table (0xefbe0) |

Decompile: one worker for the five real functions; 0x4689b0 is the only FPU-heavy one. Use the Checkpoint struct from unk_481390.h (`struct Checkpoint`, 0x48 bytes at Stage+0xa0c, pos +0x00..+0x08/+0x14.., normals at +0x24 and +0x30, extents +0x3c, +0x44).

## 4. F3: model visibility and views, 0x468c40 - 0x46962f (5 fns + stub)

| Addr | Size | Conv / frame | Role | Callers |
|---|---|---|---|---|
| 0x468c40 | 47 | thiscall `ret 4` (arg), no frame | **Car::SetModelTexture/SetLod(arg)**: if `[+0xfa8]`: `list(+0xfa4)->0x4af180(arg)` (upload textures of each node, `0x4a8350` per node unless already resident, flag +0xc bit 0), then `obj(+0xfa0)->0x4ae1f0(arg)` (rebuild/reload) | 0x486694, 0x48e0eb |
| 0x468c70 | 1392 | thiscall `ret 4` (`vector<Car*>&` out), no frame, no FPU | **Car::BuildVisibleParts(list)**: sets/clears hide bit 0 (flag byte at +0xc) on ~40 model parts depending on `+0x1eb8` (damaged/second model), `+0x1eb4` (cockpit view), `+0x1ebc` (table 0x4fb0fe indexed by *12), global 0x501454/0x501458/0x501464/0x50132c, `RaceMode(0x501450)::0x48e820` (returns "show"); at the end `list.push_back(this->body)` (inline vector push_back with `operator new` 0x4c1276 growth) | 0x488b14, 0x488b2c, 0x488c53, 0x488cea, 0x488d02, 0x488e29 |
| 0x4691e0 | 442 | thiscall `ret 4` (int inside), no frame | **Car::SetCockpitView(int)** (guess): switches the active meshes (+0x200c, +0x2048 from the pair +0x2010/+0x2014 and +0x204c/+0x2050), calls `0x4b2b30(body)` / `0x4b2b90()` (attach/detach mesh to body), toggles bit 0 in the flag byte of 5 view-dependent bodies (+0x210c, +0x2238, +0x2364, +0x2494, +0x25f4), copies +0x2080/0x2088/0x2090 or +0x2084/0x208c/0x2094 vectors into +0x2074..+0x207c, and sets/clears bit 1 in each wheel flag word (+0x34, stride 0x330) | 0x456678, 0x4567ce, 0x456bbe, 0x4605e3 |
| 0x4693a0 | 543 | thiscall `ret 4` (int damaged), no frame | **Car::SetDamagedModel(int)** (guess): when changing `+0x1eb8` iterates the two node lists (+0xfac / +0xfd4) and either resets each node's material to defaults (floats 1.0 at +0x40..+0x48, ints at +0x18..+0x9c: blend modes 5/6, 3/4, etc.; depends on bit 0x40 of +0x9d) or copies the corresponding state from list 2 to list 1 (clean vs. damaged mesh); sets `[+0x1bd30]+0x38` = 0x11 / 0 if `[+0x1bcfc]` | 0x488afc, 0x488b20, 0x488cd2, 0x488cf6, 0x48e0f4, 0x491304, 0x491367 |
| 0x4695c0 | 53 | thiscall `ret 4` (float a), no frame, FPU | **Car::SetBodyAlpha(float)**: if `+0x1eb8`: `a*0.75` stored to `node->data+0x4c` for every node of list +0xfac | 0x488b08, 0x488c3d, 0x488cde, 0x488e13 |
| 0x469600 | 48 | `_$E` stub | | init table (0xefbe4) |

Evidence `ev_06648552681e86e21ea85e48e812fce3e34a5089ef0aa3b5ed596e67b326cea5` records the 1,392-byte `BuildVisibleParts` body. The focused matcher on `src/unk_468c40.cpp` reports 4/4 marked functions (3 existing NONMATCHING); `objcmp` reports 1,390 built bytes and 928 differing bytes outside relocations, with most disassembly shifts downstream of the two-byte size gap. Replacing the saved old-buffer temporary with `list->data` at the later delete did not change codegen. An explicit `goto` layout around the `v1c738` test also compiled identically. Both trials were reverted; EH is absent in this function and unaffected.

Decompile: group (all small except 0x468c70 whose long cascades of `if (p) p->flag |= 1` must be matched by order; build with a local helper only if it does not change codegen).

## 5. F4: render, lighting, effects, gauges, 0x469630 - 0x46a89f (11 fns + stub)

| Addr | Size | Conv / frame | Role | Callers |
|---|---|---|---|---|
| 0x469630 | 393 | thiscall `ret 0x10` (4 args), no frame | **Car::UpdateRenderTransforms(dt?, a, b, c)** (guess): `body->0x4b3030()` (is-shadow-body test), accumulates 4 x copies of the wheel matrices (Car+0x44, stride 0x330, 16 dwords) into the render objects +0x1f1c[4]; positions the 3D render object +0x20ec (`0x472c40`, `0x472c20`), copies body pos+matrix to +0x1ff0 and calls renderer `0x470940(renderer 0x56b0ac, ...)`; clamps counters +0x1420/+0x1424 down by arg1 to a minimum of 0x28; calls `0x459270(this, ...)` if +0x1bce4 | 0x488eb9, 0x488edd |
| 0x4697c0 | 517 | thiscall `ret 8`, SEH (EH 0x4dbb30), FPU | **Car::UpdateLighting(...)** (guess): fetches ambient from the Stage (Stage+0x1e8..+0x1f4 min/max or default 0..1), `0x45dac0` (sun/speed factor), clamps to [0,1], lerps, writes RGB (ambient +0x40..+0x48, +0x70..+0x78) into every node of list +0xfac, calls 0x4699d0, applies `0x4021b0` (set material colour) on +0x2074/+0x2078/+0x207c objects with +0x2098/+0x209c | 0x488f3a, 0x488f57, 0x489080, 0x48909d |
| 0x4699d0 | 46 | thiscall `ret 8`, no frame | **Car::UpdateShadow**: if `[+0x1bce8]==1` call `0x4b9d30(&this->+0x20f0, body, arg, &body+0xb8, body[+0xc4])` (shadow/projection update) | 0x469940 |
| 0x469a00 | 30 | thiscall `ret 8`, no frame | thin forwarder: `[+0x20ec]->vslot5(renderer 0x56b0ac, arg)` (draw the car's render object) | 0x488fba, 0x489020 |
| 0x469a20 | 1009 | thiscall `ret 0xc`, SEH (EH 0x4dbb68), FPU | **Car::EmitEffects(a, b, c)** (guess): dust/spray/exhaust particle emission: when `+0x20e8`: exhaust smoke from `rpm*throttle` (rand 0x4c1998 * 3.0517e-5 scaling), world point = body.matrix * +0x20dc + pos, velocity from body velocity and `0x4204f0` (world->body); emits to emitter `+0x25e8` (0x4786a0); when detail `+0x1ec0 > 1` and `body[+0x268] > 0` emits to emitter `+0x2488` (0x47bf80, else 0x47c1a0 x2); for each of the 4 wheels with `wheel+0x298` set, uses Stage tables [+0x5bc]/[+0x60c] to emit to emitters `+0x2358` (0x47c790), `+0x222c` (0x479490), and (detail 2) `+0x2100` (0x477b60) | Car update 0x45bafd |
| 0x469e20 | 115 | fastcall, no frame | **Car::ClearPartPositions**: zero the 4 floats at +0x1bcac; for every sub-object of vector `[+0x1bcc8..+0x1bccc]` write a zero Vec4 to sub+0x88 | 0x480a35, 0x481906, 0x48f2bd, 0x48fa20 |
| 0x469ea0 | 348 | thiscall `ret 4`, no frame, FPU | **Car::ComputeWheelSurfaceMix(float)**: for each wheel with contact and Stage+0x51c[surface] > 0 builds `w = clamp(speed/6 + |slip|... , 0, 1) * Stage+0x69c[surface-1] * arg`, adds to `+0x1bcac[surface-1]` and subtracts from the other three, then normalises the four weights to sum 1 (so the sound/tyre-track code can blend surface types) | Car update 0x45baab |
| 0x46a000 | 511 | fastcall, ebp frame, SEH (EH 0x4dbb88), FPU | **Car::UpdateTyreTracks**: for each of the 4 wheels: if wheel contact (`wheel+0x284`) and Stage surface entry `[stage+surface*4+0x56c]`: world position of the contact patch (body matrix * wheel offset + pos), minus 0.015 above ground, slip scale `(wheel+0x280 - 2.5) * 0.1` clamped to 1.0, calls `WheelTrail[i]->0x4746b0(...)` (track stamp); else calls it with null/zero (end track) | Car update 0x45b529, 0x45bb45 |
| 0x46a200 | 1424 | fastcall, `and esp,-8` frame, SEH (EH 0x4dbbce), FPU heavy | **Car::DrawGauges** (guess: dashboard speed and rpm needles; 2 gauges) | Player 0x494166 |
| 0x46a790 | 188 | **cdecl** `void f(float* out8, float angle, float cx?, float cy?, float w, float h)` (pushes 6 args: out, 0.7854 = pi/4, scale, 2 floats), FPU (fcos, fsin) | **RotateQuad**: writes 4 rotated 2D corner pairs (8 floats) of an axis-aligned quad rotated by angle about its centre: used for the needles | 0x46a548, 0x46a685 |
| 0x46a850 | 19 | fastcall, no frame | **Car::FlushMessageQueue**: `[this+0x1f160 queue]->0x40b600([this+0x1f148] time)` | 0x49416d |
| 0x46a870 | 48 | `_$E` stub | | init table (0xefbe8) |

0x46a200 details (so the worker does not rediscover them): `[0x501310]` = HUD/gauges enabled; `0x501300` selects `[+0x1ecc]` vs `[+0x1ec8]` as texture/colour id; reads `[0x56b0c8]+0x1390fc/0x139100/0x139104` (screen layout ints from the Game object, three values), renderer 0x56b0ac slots 0x8c (SetTexture) and 0x108 / 0x10c (DrawQuad with 4 Vec2 corners + 4 Vec2 uvs, args colour/1.0/1.0 alpha pairs); two local `Vec2[4]` arrays built/destroyed with the VC6 vector helpers `0x4c11f4`/`0x4c1116` (SEH states 0..2) -> the function therefore needs the unwinding table with 3 entries; background quad uses UVs 0..1 (0.5/1.0 constants), the needles use 0.7854 rad (pi/4) offset and speed `|v|*3.6/[+0x1f0c]` and rpm `GetSmoothedRpm/[+0x1f08]` fractions clamped to [0,1]; angles from `[+0x1efc]*0.5`, `[+0x1f00]-[+0x1f04]`, `[+0x1ef0]*0.5`, `[+0x1ef4]-[+0x1ef8]`.

## 6. Constants and globals used (quick reference)

.rdata floats: 0x4e1348 = 1.0, 0x4e134c = 0.0, 0x4e138c = 0.5, 0x4e13a4 = 0.25, 0x4e13d4 = 100.0, 0x4e13dc = 3.0, 0x4e15c8 = 0.01, 0x4e1b20 = 3.6, 0x4e1b98 = 0.9, 0x4e1e7c = pi, 0x4e1e98 = 1/256, 0x4e1ea8 = 1/32768 (rand scaling), 0x4e1eac = 1.8, 0x4e1eb0 = 1/60, 0x4e1eb4 = 1/6, 0x4e1eb8 = 2.5, 0x4e1ebc = 0.015, 0x4e1700 = 5.0, 0x4e14cc = 0.75, 0x4e14e0 = 256.0, 0x4e14bc = 16.0, 0x4e14c0 = 32.0, 0x4e16ec = 0.125, 0x4e1358/0x4e1354 = 1/1024 and 1/768 (screen scale), 0x4e139c = 0.1, 0x4e1ea0 = double 2/pi.
.data/.bss globals: 0x56b0b0 sound system, 0x56b0ac renderer, 0x56b0c8 game object, 0x501450 RaceMode\*, 0x501454/0x501458/0x501464/0x50132c race flags (online/replay/mode/difficulty), 0x501300/0x501310/0x501438/0x50143c/0x50147c/0x501468 options (0x501468 = checkpoint laps multiplier), 0x501518..0x501528 stuck-timer thresholds (floats 5.0, 1.0, 2.0, 2.0, 0.1), 0x4fb0fe (table, stride 12 bytes).

## 7. Call-graph pointers

- Car::Update (0x45b4xx-0x45bc1f, outside the range): calls in order 0x46a000 (tyre tracks, 0x45b529), 0x4689b0 (0x45ba70), 0x469ea0 (0x45baab), 0x469a20 (0x45bafd), 0x46a000 again (0x45bb45), 0x468550 (0x45bc04), 0x468720 (0x45bc0c).
- RaceMode base vtable (0x48e630-0x48e830, see racemode-base.md): slot 13 = 0x48e630 -> 0x4688f0(3); sound slots call 0x4670c0 (0x48e75b), 0x4672c0 (0x48e79d), 0x467440 (0x48e7f0).
- Player forwarders: 0x491880 -> 0x468bd0, 0x491920 -> 0x468c00, 0x494166/0x49416d -> 0x46a200/0x46a850, 0x491304/0x491367 -> 0x4693a0.

## 8. Suggested subagent work groups (roughly 1500-3000 bytes; big functions alone)

| Group | Functions | Bytes | Notes |
|---|---|---|---|
| S1 | 0x4666a0 (alone), then 0x466fe0 | 2582 | Sounds.ini loader (SEH 0x4dbab8, ~11 unwind states, many `std::string` temporaries via 0x4076c0/0x402700 helpers: reuse the File/string classes already used by other loaders such as Car::FUN_00466500 / stage loaders). 0x466fe0 is trivial but belongs to the same file. |
| S2 | 0x467440 (alone) | 4039 | UpdateSounds: FPU heavy, ebp frame, SEH 0x4dbaf0, helper float curves 0x468410/0x4684b0/0x468500 must exist first (S3). Expect NONMATCHING ledger for 4-5 repeated blocks. |
| S3 | 0x4670c0, 0x4672c0, 0x468410, 0x4684b0, 0x468500, 0x468550 | 1592 | Start/Stop sounds loops, the 3 cdecl float helpers (file-scope statics/`static` free functions), and UpdateSurfaceSound (SEH 0x4dbb08). Do this together with S1/S2 because the helpers are used by 0x467440 and share the header. |
| S4 | 0x468720, 0x4688f0, 0x468980, 0x4689b0, 0x468bd0, 0x468c00 | 1219 | Entire file F2 (stuck/respawn/checkpoint). |
| S5 | 0x468c40, 0x468c70, 0x4691e0, 0x4693a0, 0x4695c0 | 2477 | Entire file F3 (visibility/view). 0x468c70 is the long flag cascade; the rest are small. |
| S6 | 0x469630, 0x4697c0, 0x4699d0, 0x469a00, 0x469e20, 0x469ea0 | 1449 | Start of F4: transforms, lighting, shadow, forwarder, clears, wheel-surface mix. |
| S7 | 0x469a20 (alone), 0x46a000 | 1520 | EmitEffects (SEH 0x4dbb68, ~3 states) and UpdateTyreTracks (SEH 0x4dbb88). Needs the effect emitter class signatures from unk_47c460.h / range-47c460.md and the WheelTrail class of range-4741a0.md. |
| S8 | 0x46a200, 0x46a790, 0x46a850 | 1631 | DrawGauges with `__vec_ctor/dtor` locals, RotateQuad (cdecl, `fcos/fsin`), message flush. |

Order: S1 + S3 (Sounds + helpers) first; S5 and S4 are independent; S7 depends on the emitter/WheelTrail headers.

## 9. Compiler-generated / non-source functions

- `_$E` guard stubs: 0x4686f0, 0x468c10, 0x469600, 0x46a870 (mov cl,[0x5826d5]; ... jmp; push 0x402c90; call atexit 0x4c0d01). They are not in functions.csv; the four files end at these stubs (each stub is followed by 4 `nop`, then the next file begins 16-byte aligned).
- Inline template code (not separate functions): `vector<Car*>::push_back` grow path at the end of 0x468c70; vector size/index loop in 0x469e20; `__vec_ctor`/`__vec_dtor` calls (0x4c11f4/0x4c1116) in 0x46a200.
- No scalar deleting destructors, no thunks, no vtables, no `catch` blocks (SEH only, no try/catch EH state beyond unwinding).
- Trailing int3/nop padding: functions are 16-byte aligned (align16 column in functions.csv is 1 for all).

## 10. Corrections / notes for project docs

- range-46a8a0.md and unk_481390.h call this class `Car`; the Car methods in this range should be declared in the same header: UpdateSounds, InitSounds, StartSounds, StopSounds, ReleaseSounds, UpdateSurfaceSound, UpdateStuck, RespawnAtCheckpoint, RespawnAtIndex, UpdateCheckpoints, EnableTyreTracks, DisableTyreTracks, SetCockpitView, SetDamagedModel, SetBodyAlpha, BuildVisibleParts, UpdateRenderTransforms, UpdateLighting, UpdateShadow, EmitEffects, ComputeWheelSurfaceMix, UpdateTyreTracks, DrawGauges.
- modules.csv: add four modules (names above); the stubs at 0x4686f0/0x468c10/0x469600/0x46a870 are the module ends.
- RaceMode base vtable thunks (0x48e630-0x48e830) are already documented as calling the Car members at 0x4688f0/0x4670c0/0x4672c0/0x467440.
