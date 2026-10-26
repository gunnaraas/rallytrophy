<!-- Subagent analysis, 2026-10-23; read-only raw disassembly (rawdis.py/refs.py, Patch 1.01 exe); no REA. "Guess" marks inference. -->
# Range 0x46a8a0 - 0x46d82f: module map (four source files: Drivetrain, CarSnapshot save/restore, Wheel (tyre), SpringFollower)

Method: rawdis.py over the whole range, refs.py caller index, .rdata/.data reads from the PE (strings, float constants, the Config key table, jump tables). Sizes are from analysis/functions.csv; items the csv lacks are marked "(not in csv)". EH handler addresses (0x4dbbf6 ... 0x4dbdab) are the `push imm` after `push -1` in each SEH prologue. Every function was read in full except the body of 0x46c790 (3573 bytes), which was only surveyed (call list, field accesses, constants).

## 0. Headline findings

1. **Four files, as predicted by the guard stubs** (stub = `mov cl,[0x5826d5]; mov al,1; test; jne; or; mov; jmp +0x10; push 0x402c90; call atexit(0x4c0d01); pop; ret`, 0x30 bytes each, each referenced once from the init table at file offsets 0xefbe8..0xefbf8): previous stub 0x46a870 (end of the preceding file), then **0x46bb60, 0x46c290, 0x46d6f0, 0x46d800**. The four files:
   - **A = `Drivetrain`** (guess; engine + clutch + gearbox + differential), 0x46a8a0 - 0x46bb5f (code ends 0x46bb4b: 20 bytes of switch jump table at 0x46bb4c-0x46bb5f), stub 0x46bb60.
   - **B = `CarSnapshot`** (guess; a 0x8c-byte saved car state: setup bytes, damage floats, wheel angles, light flags, and three mesh-deformation buffers) plus the two `Car` member functions that save/restore it, 0x46bb90 - 0x46c28f, stub 0x46c290.
   - **C = `Wheel`** (guess; one wheel/tyre: EffectBase-derived object with a "tire" model child, spin integration, Pacejka tyre forces), 0x46c2c0 - 0x46d6ef, stub 0x46d6f0.
   - **D = `SpringFollower`** (guess; 0x1c-byte spring-damper that follows a target between limits), 0x46d720 - 0x46d7f8, stub 0x46d800.
2. **Not in the csv** (the csv folds them or skips them): 0x46bb90 (0x20 bytes, snapshot vec-ctor callback), 0x46bbb0 (5-byte `jmp 0x46bbc0`, snapshot vec-dtor callback), the five stubs, and the two jump tables. Total csv functions in range = 35 (36 with 0x46d830, which belongs to the next range).
3. **The Car object embeds all of these.** In the Car ctor 0x45e480: `Drivetrain` at **Car+0xdb8** (ctor call 0x45e4ec, config load 0x45eed7, dtor call 0x4622d7), `Wheel` array of 4 at **Car+0x28, stride 0x330** (vec-ctor), `CarSnapshot` array of **3 at Car+0x1c75c, stride 0x8c** (vec-ctor `0x4c11f4(this+0x1c75c, 0x8c, 3, 0x46bb90, 0x46bbb0)` at 0x45e8ae-0x45e8c8), two `SpringFollower`s at **Car+0x1f30 and Car+0x1f4c** (ctor 0x46d720 at 0x45e628/0x45e637, Reset 0x46d7f0 at 0x45cd9b/0x45cda6). So **the `CarWheel` struct in include/unk_481390.h (Car+0x2c0, stride 0x330) is the tail of the real Wheel object**: wheel base = Car+0x28+n*0x330, `CarWheel` starts at wheel+0x298 (`active` = wheel+0x298, `touching` = wheel+0x29c, `value` (+0x68) = wheel+0x300).
4. **All of the Drivetrain/Wheel functions are Car physics.** Callers are the Car physics step (0x45c1a2.. 0x45c3d0 region, 0x4590b9.. gearbox/input region), Car ctor/dtor/reset (0x45cd90, 0x45da2b..), the Car loader (0x460080) and sound code (0x469a65, 0x4543b5, 0x4677ce, 0x46a61f).
5. **Files A and C both load/hold configuration data**: A reads a 58-key `Config` table at 0x4fdf30 (car engine ini: see 2.3). C has no config.
6. **File C is polymorphic** (vtable 0x4e1ee8, EffectBase-derived like the neighbours, see range-46d830.md); A, B, D have no vtable. No file in the range has a file-scope static with a dynamic initialiser (no init-table pair at the start of any file); the only statics are the writable key buffer `"Torque00"` at 0x4fe100 (file A) and floats in .rdata.
7. **Static math helper in C**: `0x46d590` is a plain `cdecl` (no `this`) function placed *after* its only callers (both inside 0x46c790). It is the Pacejka "magic formula" `y = D*sin(C*atan(B*x - E*(B*x - atan(B*x))))`. VC6 emits functions in definition order, so in C.cpp it is defined after `Wheel::UpdateForces` (forward-declared static).
8. **Constants**: engine speeds are in rad/s with rpm conversion by 60/2pi = 9.5493 (0x4e1ec4 / 0x4e1ed8 = 9.549296); 2pi = 0x4e1ec8; 60.0 = 0x4e1ecc; 0.10471975 (rpm to rad/s) = 0x4e1ed4; gravity 9.81 = 0x4e1440; 1.0 = 0x4e1348, 0.0 = 0x4e134c, 0.5 = 0x4e138c, 0.001 = 0x4e1398, 0.2 = 0x4e13c4, 0.9 = 0x4e1b98, 2.0 = 0x4e1b44, 1.5 = 0x4e1530, 2pi (double) = 0x4e1f10, pi/2 = 0x4e1b28.

## 1. Module / file table

| # | Range | End marker | Content | Suggested file |
|---|---|---|---|---|
| A | 0x46a8a0 - 0x46bb5f (code to 0x46bb4b) | stub 0x46bb60 | `Drivetrain`: ctor/dtor, ini loader, engine torque, shifting, differential, per-step update | src/unk_46a8a0.cpp |
| B | 0x46bb90 - 0x46c28f | stub 0x46c290 | `CarSnapshot` ctor/dtor/Clear + `Car::SaveSnapshot(int)` + `Car::RestoreSnapshot(int)` | src/unk_46bb90.cpp |
| C | 0x46c2c0 - 0x46d6ef | stub 0x46d6f0 | `Wheel`: ctor, dtors, LoadModel, spin matrix, Reset, UpdateForces (big), Pacejka helper, IntegrateSpin | src/unk_46c2c0.cpp |
| D | 0x46d720 - 0x46d7ff | stub 0x46d800 | `SpringFollower`: ctor, Update(3 floats), Reset | src/unk_46d720.cpp |

Evidence for the splits: (1) the five guard stubs and their init-table entries (above); (2) each file starts 16-aligned with a fresh prologue; (3) distinct classes (A: drivetrain ini keys; B: snapshot struct; C: vtable 0x4e1ee8; D: 0x1c-byte follower) with disjoint call graphs; (4) 0x46bb90 follows the stub at 0x46bb60 with no padding other than the stub itself. The headers `include/unk_46d830.h` (CarRain, the next file) do not declare any of these types.

## 2. File A: `Drivetrain` (0x46a8a0 - 0x46bb5f)

### Identity and ownership
- Embedded in Car at **+0xdb8** (not allocated). Size: next Car field at +0xfac/+0xfb0/+0xfb4 is written by the Car ctor right after the ctor call (0x45e4f1..), so size <= 0x1f4; last field used is +0x1e4, so **0x1e8 is the likely size** (guess; vec of fields +0x5c Vec3[8] is the only member needing EH).
- `this+0x00` is the **Car\*** (set outside this file; the ctor zeroes it). Everything in the engine/gearbox reads Car state through it: Car+0x1c74c/+0x1c750/+0x1c754 (three floats: guess "damage" levels; engine torque is multiplied by `1 - 0.2*Car[0x1c750]`, drive torque by `1 - 0.2*Car[0x1c754]`), Car+0x1c758 (float; compared with 0.9 in the shift limiter), Car+0x24 (CarBody: velocity at +0x1cc/+0x1d0/+0x1d4, matrix rows at +0x40..), wheels (Car+0x28+n*0x330: spin speed at wheel+0x284, drive share at wheel+0x32c), Car+0x28c (float, wheel radius, used by 0x46b020).
- Non-polymorphic, no vtable. Constructor builds `Vec3 m_ratio[8]` at +0x5c with the usual Vec3 ctor/dtor callbacks (0x490ad0 / 0x402c90).

### Fields (offsets seen; "L" = set by the ini loader 0x46a920, key names are the original Config keys)
| Offset | Type | Meaning |
|---|---|---|
| 0x00 | Car\* | owner |
| 0x04 | float | L EngineFriction (Reset default 0.015 = 0x3c75c28f) |
| 0x08 | float | L DifferentialBias |
| 0x0c | float | L DifferentialLock |
| 0x10 | float | L InertiaEngine |
| 0x14 | float | L InertiaDriveShaft |
| 0x18 | int | L NumGears |
| 0x1c..0x38 | float[8] | L gear inertia indexed by gear+1: [0]=GearRInertia, [1]=Gear1Inertia (neutral reuses it), [2]=Gear1Inertia again, then Gear2..Gear6Inertia (loader reads the key "Gear1Inertia" twice, ditto Gear1Ratio) |
| 0x3c..0x58 | float[8] | computed gear ratios by gear+1 (code indexes `[this+gear*4+0x40]`, gear -1 = reverse, 0 = neutral); written only by SetGearing 0x46baa0 from the Vec3 table below |
| 0x5c..0xb8 | Vec3[8] | L raw gear ratio Vec3s (GearRRatio, Gear1Ratio twice, Gear2..Gear6Ratio); SetGearing mixes (x,y,z) into the effective ratio |
| 0xbc, 0xc0 | int | zeroed by the loader |
| 0xc4..0xd4 | float[5] | L Gear1ShiftUp .. Gear5ShiftUp |
| 0xd8 | float | 10000000.0 (0x4b18967f), shift-up threshold of the top gear |
| 0xdc, 0xe0 | int | 0 |
| 0xe4 | float | 1.0 |
| 0xe8..0xf8 | float[5] | L Gear2ShiftDown .. Gear6ShiftDown |
| 0xfc | float | L EndRatio (final drive) |
| 0x100 | int | gearing setting 0..4, init -1; loader ends with `SetGearing(2)` |
| 0x104 | float | L IdleRpm (Reset default 1200.0 = 0x44960000) |
| 0x108 | float | L TorqueMax * `[Game(0x56b0c8)+0x13948c]` (an int setting of the game object, probably an engine-power class; used as torque scale) |
| 0x10c..0x1ab | (float,float)[20] | L Torque00..Torque19: each key is a Vec2 (rpm, torque); stored at +0x10c+8i (rpm) and +0x110+8i (torque). Key string built by patching two digit characters into the writable buffer at 0x4fe100 (`"Torque00"`: byte 6 = `i/10+'0'`, byte 7 = `i%10+'0'`) |
| 0x1ac | int | L TorqueCurveParams (= number of curve points) |
| 0x1b0 | float | L ClutchReleaseTime (Reset default 0.1) |
| 0x1b4 | float | L ClutchEngageTime (default 0.1) |
| 0x1b8 | float | L GearShiftTime (default 0.1) |
| 0x1bc | float | L ExhaustPeakRpm (rpm normaliser) |
| 0x1c0 | float | throttle input (SetThrottle 0x46b250) |
| 0x1c4 | float | effective throttle (= 0x1c0 while no shift, ramped during a shift) |
| 0x1c8 | float | clutch engagement 0..1 (0 = open) |
| 0x1cc | float | net engine torque (result of 0x46b3f0) |
| 0x1d0 | float | engine speed (rad/s per the constants; rpm/9.549) |
| 0x1d4 | int | current gear (-1 = reverse, 0 = neutral) |
| 0x1d8 | float | reaction torque to the drive line (result of 0x46b3f0, read by 0x46aff0) |
| 0x1dc | int | target gear during a shift |
| 0x1e0 | int | shift state: 0 idle, 1 clutch releasing, 2 gear changing, 3 clutch engaging |
| 0x1e4 | float | shift timer |

### Config / ini facts (loader 0x46a920)
- Stack `Config` (0x405580 ctor, 0x4055a0 dtor), `Define(keys=0x4fdf30, count=0x3a)` (0x4076c0), `Load(path, requireAll=1, verifyChecksum=1, checksum)` (0x406ce0). Signature: `Drivetrain::LoadConfig(const char* iniPath, unsigned checksum)`, `ret 8`. Caller 0x45eed7 passes the checksum from the table `0x4fb188[carIndex]` = {0x2e5f, 0x5b9, 0x4ed, 0xaeeb}; path is the string from `0x409130(...)` or the empty string 0x4e1344 when null (the sibling load 0x462350 uses table 0x4fb130).
- Key table 0x4fdf30: 58 entries of {name ptr, type}: types 2 float, 1 int, 3 Vec2, 4 Vec3. Order: InertiaEngine, InertiaDriveShaft, EngineFriction, DifferentialBias, DifferentialLock, ClutchReleaseTime, GearShiftTime, ClutchEngageTime, NumGears(int), GearRRatio(v3), GearRInertia, Gear1Ratio(v3), Gear1Inertia, Gear2..Gear6 (Ratio v3, Inertia), EndRatio, Gear1..5ShiftUp, Gear2..6ShiftDown, IdleRpm, TorqueMax, TorqueCurveParams(int), Torque00..Torque19 (v2), ExhaustPeakRpm. Strings all at 0x4fe100..0x4fe434 (in .data).
- Getters used: GetFloat 0x407f60, GetInt 0x407d50, GetVector2 0x408390, GetVector3 0x4085e0.

### SetGearing (0x46baa0) detail
`SetGearing(int s)` returns at once if `s == [0x100]`; otherwise `[0x100] = s` and for each of the 8 gears `ratio[i] = (a*v.x + b*v.z + c) * v.y` where `v = m_ratio[i]` and (a,b,c) from the 5-entry jump table 0x46bb4c: s=0 (a=1,b=0,c=0), s=1 (0.5,0,0.5), s=2 (0,0,1.0), s=3 (0,0.5,0.5), s=4 (0,1.0,0). Guess: the Car setup (CarSetup, five values, see Car::FUN_00466500) selects a short/medium/long gearing preset; the Vec3 holds (short ratio, scale, long ratio).

### Functions
| Addr | Size | Convention | What | External callers |
|---|---|---|---|---|
| 0x46a8a0 | 91 | thiscall, no args, SEH (EH 0x4dbbf6), vec-ctor | **ctor**: `0x4c11f4(this+0x5c, 0xc, 8, 0x490ad0, 0x402c90)` (Vec3 x 8), `[this]=0`, then `call 0x46adc0` (Reset), returns this | Car ctor 0x45e4ec |
| 0x46a900 | 19 | thiscall, no SEH | **dtor**: `0x4c1116(this+0x5c, 0xc, 8, 0x402c90)` | Car dtor 0x4622d7 |
| 0x46a920 | 1184 | thiscall `ret 8`, SEH (EH 0x4dbc08), FPU | **LoadConfig(path, checksum)**: see above; ends with `SetGearing(2)` (`push 2; call 0x46baa0`) | Car ctor 0x45eed7 |
| 0x46adc0 | 97 | thiscall, no args | **Reset defaults**: zero state fields +0x1c0..0x1e0, +0x104 = 1200.0, +0x1b0/b4/b8 = 0.1, +0x4 = 0.015 | ctor 0x46a8e4 |
| 0x46ae30 | 57 | thiscall, no args | **ResetState**: `[0x1d0] = IdleRpm`, zero clutch/throttle/gear/shift state | Car 0x45cd90, 0x45da3f |
| 0x46ae70 | 307 | thiscall `ret 4`, SEH (EH 0x4dbc28), FPU | **`Drivetrain::TorqueAtRpm` (byte-exact):** scans for a curve segment; first segment is linear from zero, last segment extrapolates linearly, and interior segments call `0x4a7f30` with four torques and a stack `Vec3Slot` holding `t`, `t²`, `t³`. Result is multiplied by `1 - 0.2*Car[0x1c750]` and `TorqueMax` | 0x46b42f (ComputeEngineTorque) only |
| 0x46afb0 | 51 | thiscall, no args, float | **GetDriveTorque**: `ratio[gear] * [0x1cc] * [0x1c8] * EndRatio * (1 - 0.2*Car[0x1c754])` | Wheel::IntegrateSpin 0x46d63e |
| 0x46aff0 | 7 | thiscall, float | **GetReactionTorque**: `return [0x1d8]` | Wheel::IntegrateSpin 0x46d623 |
| 0x46b000 | 31 | thiscall, int | **GetEngagedGear**: `0` if clutch `[0x1c8] <= 0.5` else current gear `[0x1d4]` | 0x46a364 (car sound/gear display) |
| 0x46b020 | 376 | thiscall, no args, SEH (EH 0x4dbc48), FPU | **SelectGear**: from the car body's forward speed and `Car+0x28c` wheel radius computes a wheel rpm and walks the gear index down/up against the ShiftUp/ShiftDown tables (+0xc4.. / +0xe8..), returns the chosen gear (int) | Car 0x4590b9, 0x459152 |
| 0x46b1a0 | 34 | thiscall, float | **GetRpmFraction**: `min([0x1d0] / ExhaustPeakRpm, 1.0)` | 0x469a65 (engine sound) |
| 0x46b1d0 | 118 | thiscall, float | **GetShiftProgress**: by shift state returns the clutch blend (state 1: t/ReleaseTime clamp 1, state 2: 1.0, state 3: 1 - t/EngageTime, state 0: 0) | 0x4543b5 |
| 0x46b250 | 33 | thiscall `ret 4` | **SetThrottle(float)**: `[0x1c0]=v; if state==0 [0x1c4]=v` | Car 0x45c1a2 |
| 0x46b280 | 207 | thiscall `ret 4`, switch jump table 0x46b350 (4 entries: 0x46b2af, 0x46b2e7, 0x46b2ef, 0x46b30b) | **ShiftBy(int delta)**: state 0 starts a shift (target = gear+delta within [-1, NumGears(-1 if Car[0x1c758] < 0.9)]); state 1/2 retargets; state 3 restarts | Car 0x4591d6 |
| 0x46b360 | 136 | thiscall `ret 4` | **ShiftTo(int gear)**: absolute version of 0x46b280 (range-checks against NumGears, same state transitions) | Car 0x4590c1, 0x459135, 0x459172, 0x459193, 0x4591ea |
| 0x46b3f0 | 177 | thiscall, no args, FPU | **ComputeEngineTorque**: `x = max([0x1d0], IdleRpm)`, `[0x1cc] = (TorqueAtRpm(x) - EngineFriction*x) * [0x1c4]`-style net torque, and `[0x1d8]` reaction torque from gear inertia, ratio, EndRatio and clutch | Car 0x45c1a9 |
| 0x46b4b0 | 244 | thiscall `ret 4`, FPU | **DistributeDifferential(int driveType)**: type 3 = 4WD (all four wheel spin speeds `|w|+DifferentialBias`, sets wheel+0x32c drive shares of wheels 0..3 and a front/rear split), type 2 = front pair, else rear pair | Car 0x45c1b7 |
| 0x46b5b0 | 900 | thiscall `ret 8`, FPU | **Update(float dt, int driveType)**: advances the shift state machine (states 0..3 with clutch/throttle ramps), picks the wheel spin sum for the drive type, calls 0x46b940, then clamps wheel spin speeds (wheels +0x284 and, for 4WD, +0x2ac/+0x5dc) to the engine-driven speed | Car 0x45c3d0 |
| 0x46b940 | 287 | thiscall `ret 8`, FPU | **IntegrateEngineSpeed(float dt, float wheelLoad)**: `[0x1d0]` blends toward the speed implied by the wheels through the clutch (`[0x1c8]`), engine inertia and gear inertia, clamps to >= 0 | 0x46b7c4 (Update) |
| 0x46ba60 | 62 | thiscall, float | **GetSmoothedRpm**: `r = [0x1d0]`; below `IdleRpm*1.5` blends r toward IdleRpm with a squared ramp; used for sound/tach | 0x45413d, 0x4677ce, 0x46a61f |
| 0x46baa0 | 171 | thiscall `ret 4`, FPU, jump table 0x46bb4c (5 entries) | **SetGearing(int s)**: see above | ctor 0x46a920 (0x46ad96), Car 0x46650e |

Stub: 0x46bb60 (48 bytes) = `_$E` guard stub, init-table entry only.

Decompile groups: see section 6.

## 3. File B: `CarSnapshot` (0x46bb90 - 0x46c28f)

### Identity and ownership
- **CarSnapshot** (guess): a **0x8c-byte** record saved/restored by the Car. `Car+0x1c75c + slot*0x8c`, **3 slots** (vec-ctor count 3; slot 2 at 0x1c874 is the one loaded from a file/network message: `0x4a5e00(file, ptr, 0x8c)` raw read in 0x403f99-0x403fa8 and 0x44dc6d). The 3 slots are cleared with 0x46bbc0 at 0x403f9b / 0x44dc6d / 0x44e1b1 before reading.
- Used by the replay/network code and `Player` (0x48e6b3: loop over players calling `car->SaveSnapshot(mode)`, 0x48fd75, 0x491758 `SaveSnapshot(2)` then 0x491761 `SaveSnapshot(0)`).
- The Car header already declares `Car::FUN_0046bc90(int a)` and `Car::FUN_0046be50(int a)`; these are `SaveSnapshot(int slot)` and `RestoreSnapshot(int slot)` (names are guesses; arguments verified: slot index multiplied by 0x8c via `(slot*7*5*4)`).

### Layout (offsets into the 0x8c record; written by 0x46bc90)
| Offset | Meaning |
|---|---|
| 0x00..0x04 | five bytes = the CarSetup values (read by Car getters; written back by Car setters 0x465e90, 0x466500, 0x466520, 0x466590, 0x466610 in Restore) (the first five dwords of Car are the setup; only the low bytes are kept). Clear default = 2 for all five |
| 0x08, 0x0c, 0x10 | floats = Car[0x1c74c], [0x1c750], [0x1c754] (the three "damage" floats) |
| 0x14..0x20 | 4 dwords = Car[0x1c73c + 4k] |
| 0x24..0x30 | 4 floats = wheel k +0x2a4 (Car+0x2c8 + k*0x330 +4) (steer/angle) |
| 0x34..0x40 | 4 floats = wheel k +0x2a0 (Car+0x2c8 + k*0x330) |
| 0x44 | float = Car[0x1c758] |
| 0x64..0x67 | 4 bytes = Car[0x1bcfc], [0x1bd0c], [0x1bd1c], [0x1bd2c] (light/state flags, stride 0x10) |
| 0x68, 0x69 | bytes = `Car[0x1c730] != 0`, `Car[0x1c734] != 0` |
| 0x6a..0x6c | 3 bytes = flags of three light sprites at Car+0x1bd44.. (stride 0x1c, only if the sprite pointer at -4 is non-null) |
| 0x6d..0x6f | 3 bytes = second light set at Car+0x1bd44+0x50.. |
| 0x70, 0x71 | 2 bytes = flags at Car+0x1be24.. (stride 0x1c) |
| 0x72, 0x73 | 2 bytes = flags at Car+0x1be24+0x34.. |
| 0x74, 0x78, 0x7c | three ints = sizes of the three deformation buffers |
| 0x80, 0x84, 0x88 | three `new`'d buffers: mesh deformation data of the Car's mesh containers `[Car+0x24]+0x124`, `[Car+0x1f6c]+0x124`, `[Car+0x1f68]+0x124` (each obtained with `0x4a6800(container,0)` and filled by `0x4b99c0(mesh, &buf, &size)`); restored with `0x4b9a60(mesh, size, buf)`. Freed by `Clear` with `operator delete` 0x4bd000 |

The light flags are re-applied by Restore with the effect helpers `0x4b2b90` / `0x4b2b30` (EffectBase show/hide-style setters seen in range-46d830.md) and by writing the raw colour/intensity dwords of the sprite pairs (`[sprite+0x...]` picks `[x+4]` or `[x+8]` depending on the flag). Restore finishes by calling `0x45a7a0(0..3)` once per wheel (apply wheel state).

### Functions
| Addr | Size | Convention | What | External callers |
|---|---|---|---|---|
| 0x46bb90 | 32 (not in csv) | thiscall (vec-ctor callback), no SEH | **CarSnapshot ctor**: zero +0x80/+0x84/+0x88, `call 0x46bbc0`, return this | `push` at 0x45e8b5 (Car ctor vec-ctor) |
| 0x46bbb0 | 5 (not in csv) | `jmp 0x46bbc0` | **CarSnapshot dtor** (vec-dtor callback; just Clear) | `push` at 0x45e8b0, other EH tables |
| 0x46bbc0 | 204 | thiscall, no args | **Clear/Reset**: bytes 0..4 = 2, zero everything else (rep stos for +0x44..0x63, byte loops for +0x64..0x73), delete the three buffers and null them | 0x403f9b, 0x44dc6d, 0x44e1b1, ctor 0x46bba7, dtor jmp, 0x46bcaf |
| 0x46bc90 | 439 | `Car::` thiscall `ret 4` (int slot), FPU, no SEH | **Car::SaveSnapshot(slot)**: Clear the slot then copy everything in the table above | Player 0x48e6b3, 0x48fd75, 0x491758, 0x491761 |
| 0x46be50 | 1082 | `Car::` thiscall `ret 4` (int slot) | **Car::RestoreSnapshot(slot)**: calls the five setup setters, writes damage floats, wheel angles, flags, re-applies light sprite state and mesh deformation, then `0x45a7a0` x4 | Net/replay: 0x404023, 0x44dcf9, 0x44e2ba, 0x48e703, 0x48fdd5 |

Stub: 0x46c290 (48 bytes) = guard stub.

## 4. File C: `Wheel` (0x46c2c0 - 0x46d6ef)

### Identity and ownership
- **Wheel** (guess name; matches Car wheel array: stride 0x330, 4 wheels at Car+0x28; `new` not used, vec-ctor callback). Size **0x330**. Derived from the engine object base **EffectBase** (ctor 0x4b2830, dtor 0x4b29c0, 0x124 bytes, see unk_47c460.h): the ctor calls `0x4b2830` first and `[this+0]` is set to the wheel vtable **0x4e1ee8**.
- Vtable **0x4e1ee8** (9 slots, the next dword 0x3db2b8c3 = 0.0872665 is the float used by 0x46c550):

  | slot | address | meaning |
  |---|---|---|
  | 0 | 0x46c400 | scalar deleting dtor (`ret 4`) |
  | 1 | 0x4b2c10 | base |
  | 2 | 0x48e660 | shared empty `ret` |
  | 3 | 0x4b2d50 | base |
  | 4 | 0x4b2fc0 | base |
  | 5 | 0x4b3000 | base (Render) |
  | 6 | 0x4031e0 | (different from CarRain/WindowRain's 0x477080) |
  | 7 | 0x4b3010 | base SetFlag04 |
  | 8 | 0x4890c0 | `return 1` |

- Other users of the vtable address: the ctor (0x46c302) and dtor (0x46c43d) only.
- **Embedded sub-object at +0x134**: a polymorphic `Object` (same class family as EffectBase: ctor `0x4a6890`, dtor `0x4a6cb0`, vtable slot 2 called at 0x46c4a5, name `std::string` at its +0x114 = wheel+0x248 which is set to `"tire"`, mesh list `[obj+0x124]` = wheel+0x258 loaded with `0x4a66e0(this+0x258, arg, 0)`). Its matrix (obj+0x20 = wheel+0x154) is overwritten by 0x46c550 each frame. The ctor also assigns the same string `"tire"` (0x4fe444) to the wheel's own name std::string at +0x114 (inlined `string::assign`: `0x402980(this+0x114, len, 1)` then memcpy), and calls `0x4b2b30(this+0x134, this)` (base setter, same as the viewport's texture setter, here used as parent link).
- **`[this+0x130]` = Car\***: 0x46d5d0 does `lea edi,[[this+0x130]+0xdb8]` (= `Car::drivetrain`); 0x46c790 reads `[[this+0x130]+0x24]` = CarBody.

### Fields seen (wheel offsets; the CarWheel struct starts at +0x298)
| Offset | Type | Seen in | Meaning |
|---|---|---|---|
| 0x00 | vptr | ctor | 0x4e1ee8 |
| 0x08 | int | ctor | 0 |
| 0x14, 0x18, 0x1c | Vec3 | UpdateForces | wheel position relative to the body (EffectBase position) |
| 0x20, 0x40 | 3x3 rows | UpdateForces | EffectBase matrix rows (axle / forward) |
| 0x114 | std::string | ctor | name, `"tire"` |
| 0x130 | Car\* | 0x46d5d0, 0x46c790 | owner car |
| 0x134 | Object | ctor/dtor | "tire" model object (see above); its flags dword +0x08 (wheel+0x13c) = 0x7f in ctor |
| 0x154 | Matrix | 0x46c550 | matrix of the tire object, written by `0x4b5540(this+0x154, &mat, &mat2)` |
| 0x248, 0x258 | string, ptr | LoadModel | tire object's name and mesh container |
| 0x260 | float | ctor | 0.3 (0x3e99999a) |
| 0x264 | float | UpdateForces/IntegrateSpin | (copied to +0x268 at the start of UpdateForces) |
| 0x268, 0x26c, 0x270, 0x278, 0x27c, 0x280 | floats | IntegrateSpin/UpdateForces | tyre/wheel parameters: 0x270 = wheel inertia (divisor in IntegrateSpin), 0x268 = radius-like factor (multiplies brake torque), 0x280 = extra torque term. Not otherwise identified |
| 0x284 | float | everywhere | **spin angular velocity** (also read by Drivetrain: `wheel+0x284`) |
| 0x288 | float | UpdateForces | slip-related scratch |
| 0x28c | float | UpdateForces | (zeroed by Reset) |
| 0x290 | float | 0x46c550 | **spin angle**, `+= dt*[0x284]`, wrapped by fmod(2pi) via `0x4c1c9a` (QWORD 0x4e1f10) |
| 0x294 | int | UpdateForces | (surface/contact index store `[esi+0x294]=edx`) |
| 0x298 | int | ctor/Reset/UpdateForces | **CarWheel.active** (UpdateForces returns early to the zero path if 0) |
| 0x29c | int | IntegrateSpin | **CarWheel.touching**; IntegrateSpin only adds engine torque when it is non-zero |
| 0x2a0, 0x2a4 | float | Reset/0x46c550 | 0x2a4 = steer angle in units of 5 degrees (`* 0.0872665` = 0x4e1f0c at 0x46c649) |
| 0x2ac | float | UpdateForces | suspension compression/load (tested against 0.01 = 0x4e1edc) |
| 0x2b0 | int | Reset | surface index (`mov ebx,[esi+0x2b0]` at 0x46cb86, used with the surface table, see below) |
| 0x2b4, 0x2b8, 0x2bc | float | ctor/Reset | 0, 1.0, 0 (a direction/normal vector (0,1,0)) |
| 0x2c0, 0x2c4 | float | Reset | 0.005 (0x3ba3d70a), 2.0 |
| 0x2c8 | float | UpdateForces | |
| 0x2cc..0x2d4 | Vec3 | ctor/Reset | force accumulator (zeroed) |
| 0x2d8..0x2e0 | Vec3 | UpdateForces | force/torque outputs (`0x2e0` includes `+= load*0.0098*...` at the end) |
| 0x2e4 | float | UpdateForces | |
| 0x2ec, 0x2e8 | float | UpdateForces | tyre curve parameters (read) |
| 0x2f0, 0x2f4, 0x2f8 | float | UpdateForces | slip state; 0x2f8 clamped to [-10, 10] (consts 0x4e1c00, 0x4e1330) |
| 0x2fc..0x308 | float | UpdateForces | zeroed at the start of UpdateForces |
| 0x300 | float | Reset | **CarWheel.value** (wheel+0x300 = CarWheel+0x68) |
| 0x30c, 0x310, 0x314 | float | Reset/UpdateForces | |
| 0x318, 0x31c | float | ctor | 0.0004 (0x39d1b717) |
| 0x320, 0x324 | float | ctor | 1.5, 2.0 |
| 0x328 | float | ctor | 0 |
| 0x32c | float | Reset (1.0)/Drivetrain 0x46b4b0 | **drive torque share** written by the differential |

Globals: `0x509418`, `0x50941c` (two runtime floats multiplied in UpdateForces; look like per-session friction scales, located in .data uninitialised).

### Functions
| Addr | Size | Convention | What | External callers |
|---|---|---|---|---|
| 0x46c2c0 | 317 | thiscall, no args, SEH (EH 0x4dbcbc), vec-ctor callback | **Wheel ctor**: `0x4b2830`, `0x4a6890(this+0x134)`, set vptr 0x4e1ee8, `0x4b2b30(this+0x134,this)`, name string "tire", then field init, `call 0x46c6b0` (Reset), more zeroing | Car ctor vec-ctor (init table / file offset 0x5daa7) |
| 0x46c400 | 30 | thiscall `ret 4` | **scalar deleting dtor** (vtable slot 0) | vtable only |
| 0x46c420 | 85 | thiscall, SEH (EH 0x4dbce6) | **dtor**: vptr = 0x4e1ee8, `0x4a6cb0(this+0x134)`, `0x4b29c0` | 0x46c403, vec-dtor refs |
| 0x46c480 | 206 | thiscall `ret 4` | **LoadModel(const char\* name)**: `0x4a66e0(this+0x258, name, 0)`, vcall `[this+0x134]` slot 2, assigns `"tire"` (0x4fe454) to the std::string at +0x248 (inlined `string::assign`, uses 0x402700/0x402ab0 grow/unshare) | Car loader 0x460080 |
| 0x46c550 | 352 | thiscall `ret 4` (float dt), SEH (EH 0x4dbd0e), FPU, `esp` locals 0x90 | **UpdateSpinMatrix(dt)**: `[0x290] = fmod([0x290] + dt*[0x284], 2pi)`; builds identity matrices, `0x4b5760` rotation about Y (steer, `[0x2a4]*0.0872665`), `0x4b5670` rotation about X (spin angle) and multiplies into the tire object matrix at +0x154 (`0x4b5540`) | Car 0x45b4a8 (loop over 4 wheels) |
| 0x46c6b0 | 214 | thiscall, no args | **Reset**: zero ~30 state fields, +0x32c = +0x2b8 = 1.0, +0x2c0 = 0.005, +0x2c4 = 2.0 | ctor 0x46c393, Car 0x45ce7b, 0x45da2b |
| 0x46c790 | 3573 | thiscall, no args, SEH (EH 0x4dbdab), `ebp` frame (0xe4 locals), FPU heavy | **UpdateForces**: wheel contact and tyre forces. Transforms the local wheel position by the body matrix (inlined 3x3 mult at 0x46c843, 0x46c928, 0x46c98f), body point velocity (`0x4204f0`), slip ratio/angle with `atan` shape via **0x46d590 (Pacejka)**, friction circle/ellipse using the surface friction values and 0x509418/0x50941c, accumulates forces `0x4203e0` x3 (Vec3 helper), `0x420430` x2 (Vec3 * float), `0x414330` (Vec3 sub), `0x4b5850` (matrix op), writes the force outputs +0x2cc..+0x2e4, +0x30c..+0x314 | Car 0x45c1cf |
| 0x46d590 | 49 | **cdecl** (5 float args: x, B, C, D, E; result in st0) | **Pacejka magic formula** `D*sin(C*atan(B*x - E*(B*x - atan(B*x))))` (`fpatan`, `fsin`) | 0x46ce70, 0x46cf15 (inside UpdateForces) |
| 0x46d5d0 | 281 | thiscall `ret 4` (float dt), FPU | **IntegrateSpin(dt)**: net torque = `[0x2e0]+[0x2d4]` (tyre + other) * `[0x268]`... plus, if `[0x29c]`, the drivetrain terms `Drivetrain::GetReactionTorque*share*0.5` and `GetDriveTorque*share*0.5` (Car+0xdb8); `w += torque/inertia*dt`; friction/brake handling that snaps w to 0 when the torque magnitude is lower than the friction threshold | Car 0x45c3af |

`Wheel::IntegrateSpin` at `0x46d5d0` is byte-exact in `src/unk_46c2c0.cpp`. Plain PE disassembly observation: it computes the torque/inertia update with x87, stores the new spin while retaining the x87 value for the friction comparison, and either clamps to zero or branches on the sign bit. The sign path loads spin into EAX, executes `fxch st(1)`, tests `0x80000000`, then uses x87 add/subtract and stores the result. Focused matcher: 8/8 marked functions match in this file (3 existing NONMATCHING).

Stub: 0x46d6f0 (48 bytes) = guard stub.

## 5. File D: `SpringFollower` (0x46d720 - 0x46d7ff)

0x1c-byte object, embedded twice in Car (+0x1f30, +0x1f4c). Fields (from the three functions): +0x00 float k (spring gain), +0x04 float c (velocity gain), +0x08 float rest/lower reference, +0x0c float position, +0x10 float upper limit, +0x14 float lower limit, +0x18 float velocity. Consts: 0x4e1f34 = 0.375 and 0x4e1440 = 9.81 weight the two extra args, 0x4e1f30 = -0.5 is the velocity reflection factor on a limit hit. `Update(float dt, float a, float b)` (`ret 0xc`; stack args [esp+8],[esp+0xc],[esp+0x10]): acceleration = `-(pos-ref)*k - vel*c - a*0.375 - b*9.81`-style, `pos += vel*dt`, clamped and reflected at the limits (guess: body pitch/roll damper; the call sites at 0x4537ab and 0x45391d pass floats computed from Car+0x13f0 / +0x13a8 values and a frame dt). Field roles are guesses; the exact FPU order must be taken from the disassembly (0x46d740-0x46d7e8).

| Addr | Size | Convention | What | External callers |
|---|---|---|---|---|
| 0x46d720 | 25 | thiscall, no args | **ctor**: zero +0x00..+0x18 (7 dwords) | Car ctor 0x45e628, 0x45e637 |
| 0x46d740 | 171 | thiscall `ret 0xc` (float, float, float dt), FPU | **Update**: spring/damper step with reflection at the limits (+0x10/+0x14), velocity reflected by -0.5 on a limit hit | Car update 0x4537ab, 0x45391d |
| 0x46d7f0 | 9 | thiscall | **Reset**: `[0xc]=[0x18]=0` | Car reset 0x45cd9b, 0x45cda6 |

Stub: 0x46d800 (48 bytes) = guard stub (the range ends at 0x46d82f).

## 6. Suggested subagent work groups, and compiler-generated bits

### Work groups (sizes in bytes; all groups of a file go into the one .cpp, merge in address order)
| Group | Functions | Bytes | Notes |
|---|---|---|---|
| **A1 lifecycle+config** | 0x46a8a0, 0x46a900, 0x46a920, 0x46adc0, 0x46ae30 | 91+19+1184+97+57 = 1448 | do first: fixes the field list and the Config call order; 0x46a920 is long straight-line Config reads (`GetFloat/GetVector3` order matters, string literals are in .data) |
| **A2 engine/gearbox** | 0x46ae70, 0x46afb0, 0x46aff0, 0x46b000, 0x46b020, 0x46b1a0, 0x46b1d0, 0x46b250, 0x46b280, 0x46b360, 0x46b3f0, 0x46ba60, 0x46baa0 | 307+51+7+31+376+34+118+33+207+136+177+62+171 = 1710 | includes the two switch jump tables (0x46b350, 0x46bb4c) |
| **A3 differential/update** | 0x46b4b0, 0x46b5b0, 0x46b940 | 244+900+287 = 1431 | FPU heavy; 0x46b5b0 calls 0x46b940 (declare first) |
| **B1 CarSnapshot** | 0x46bb90, 0x46bbb0, 0x46bbc0, 0x46bc90, 0x46be50 | 32+5+204+439+1082 = 1762 | `Car::SaveSnapshot/RestoreSnapshot` are Car members (existing header declarations FUN_0046bc90/FUN_0046be50); needs `CarSnapshot` struct (0x8c) |
| **C1 wheel lifecycle + small** | 0x46c2c0, 0x46c400, 0x46c420, 0x46c480, 0x46c550, 0x46c6b0, 0x46d5d0 | 317+30+85+206+352+214+281 = 1485 | std::string inlined assign in 0x46c2c0 and 0x46c480 |
| **C2 UpdateForces (alone, hard)** | 0x46c790 (+ the Pacejka helper 0x46d590, 49) | 3573+49 | 0xe4 stack frame, FPU, SEH; match last; helper 0x46d590 is defined *after* it |
| **D1 SpringFollower** | 0x46d720, 0x46d740, 0x46d7f0 | 25+171+9 = 205 | tiny; combine with B1 or C1 in one session if wanted |

### Compiler-generated / non-source items
- **Scalar deleting dtor**: 0x46c400 (Wheel; calls 0x46c420, then `operator delete` 0x4bd000 if bit 0).
- **Vec-ctor / vec-dtor callbacks**: 0x46bb90 and 0x46bbb0 (CarSnapshot), 0x46c2c0/0x46c420 (Wheel ctor/dtor are used as callbacks by Car's `0x4c11f4`), 0x46d720 (SpringFollower ctor is called directly twice, no vec).
- **Thunk**: 0x46bbb0 (5-byte `jmp 0x46bbc0`; not in the csv).
- **Jump tables in .text**: 0x46b350-0x46b35f (4 dwords, switch in 0x46b280) and 0x46bb4c-0x46bb5f (5 dwords, switch in 0x46baa0). They come from `switch` statements, no source function.
- **Guard stubs** (not functions to write, they are emitted by the STL-using TU): 0x46bb60, 0x46c290, 0x46d6f0, 0x46d800 (plus the previous 0x46a870).
- **Inlined library code**: `std::string::assign(const char*)` in 0x46c2c0 and 0x46c480 (uses 0x402980 grow, 0x402700/0x402ab0, `rep movs` copies); `strlen` via `repnz scas` in both; Vec3 `ConfigVector3` copies in 0x46a920.
- External helpers (all out of range, guesses): 0x4b2830/0x4b29c0 EffectBase ctor/dtor, 0x4a6890/0x4a6cb0 Object ctor/dtor, 0x4a66e0 mesh-container load, 0x4a6800 mesh-container at(i), 0x4b99c0/0x4b9a60 mesh deformation save/restore, 0x4b2b30/0x4b2b90 EffectBase setters, 0x4b5540 matrix multiply, 0x4b5670/0x4b5760 rotation-matrix builders, 0x4a7f30 Catmull-Rom/Hermite evaluator, 0x40b0c0 Vec3 helper, 0x4203e0/0x4204f0/0x414330/0x420430 Vec3 helpers, 0x4c11f4/0x4c1116 EH vec-ctor/dtor iterators, 0x4c1c9a fmod, 0x490ad0 Vec3 ctor, 0x402c90 empty dtor, 0x405580/0x4055a0/0x4076c0/0x406ce0/0x407f60/0x407d50/0x408390/0x4085e0 Config API (see include/rallytrophy.h).
- **Suggested type work before decompiling**: add `Drivetrain` (0x1e8), `CarSnapshot` (0x8c), `Wheel : EffectBase` (0x330) and `SpringFollower` (0x1c) to a new header, and fix `Car` so that `drivetrain` is at 0xdb8, `wheel[4]` at 0x28 (replace the `CarWheel wheels[4]` at 0x2c0), `snapshot[3]` at 0x1c75c and `follower[2]` at 0x1f30. The existing code in other modules reads `wheels[n].active/touching/value`, so keep `CarWheel` as an accessor view at wheel+0x298 (`CarWheel* Wheel::tail()`), or adjust the offsets there.
