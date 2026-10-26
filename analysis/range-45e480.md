<!-- Subagent analysis, 2026-10-23; read-only disassembly; guesses are marked. -->
# Range 0x45e480 - 0x464aaf: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py (nops stripped), refs.py for callers/abs refs, .rdata/.data reads with a PE section reader, and a decoder for the two SEH unwind tables (FuncInfo reached from the `push imm` after `push -1`; each unwind action decoded to its destructor call). "Guess" marks inference. Sizes from functions.csv; the three COMDAT-like helpers 0x464a70, 0x464aa0 (and the empty-dtor targets) are not in functions.csv, I measured them from the bytes.

## 0. Headline findings

1. **One source file, the Car constructor/destructor file**: `Car::Car` (0x45e480, 13705 B, the biggest function seen so far), `Car::~Car` (0x461a40, 2276 B), three ini/asset loaders (`LoadCarIni` 0x462350, `LoadTiresIni` 0x4634e0, `LoadTextures` 0x463940), the wiring helpers that Player/RaceMode call right after construction, then the file's `_$E` stub (0x4649c0) and the compiler-emitted late helpers 0x4649f0-0x464aaf.
2. **The file starts at 0x45e480**. The previous file's stub is at 0x45e450 (init-table entry; the next init-table entry is 0x4649c0, so this file has **no file-scope statics**: the init table goes 0x45e450 -> 0x4649c0 -> 0x464ab0 with nothing between). Boundary at the end: 0x4649c0 stub (0x30 bytes, guard bit 0x5826d5 + `atexit(0x402c90)`), then six "late" functions 0x4649f0-0x464aaf (see section 4); the next file (rear-view mirror) starts at 0x464ab0 (range-4649f0.md).
3. **Constructor signature** (thiscall, 7 stack args, `ret 0x1c`, SEH frame 0x1bc locals, EH handler 0x4db2a0, 124 unwind states): `Car(a, b, c, d, e, f, g)`. Only caller is **Player::CreateCar at 0x491269** (`new(0x1f30c)` at 0x491239 -> object size **0x1f30c**, matches the header). Arg meaning from the code (guess where marked):
   - a = `[ebp+0x8]`: shared cache block (Game+0x139404, global 0x56b0c8). If non-zero: `Car+0xfa4 = a`, `Car+0xfa0 = [0x56b0c8]+0x139424`, `Car+0xfa8 = 0` (not owned). If zero: the ctor `new`s a 0x3c-byte object (ctor 0x4ade40 -> `Car+0xfa0`) and a 0x20-byte object (ctor 0x4aee00 -> `Car+0xfa4`), sets `[Car+0xfa0] = 1` ... and `Car+0xfa8 = 1` (owned; the dtor deletes them only when `Car+0xfa8 != 0`: 0x4aee20/0x4ade80 dtors). Player passes 0 when `Player+0x40 == 2` (guess: remote/AI-without-physics player) and also sets e (ebx) = 0, esi = 4.
   - b = `[ebp+0xc]` car model number: builds `"data/cars/car_"` (0x4f6490) + `itoa(b,10)` (0x4c1902) + `"/"` (0x4f50bc) into a local std::string `dir`; checksum tables indexed by b: **0x4fb130** (car.ini), **0x4fb188** (engine.ini), **0x4fb1e0** (tires.ini). Also read as `Player+0x2c` by the caller.
   - c = `[ebp+0x10]` -> `Car+0x1ebc` (guess: car class/"set" index; indexes 12-byte rows of the table at **0x4fb0f8** with `c*12`: byte [row+4] = "no LOD/crash", [row+5], [row+6] "has interior" (guess), [row+7]; dword [row+0] = a count/kind used with `cmp eax,2` for the shadow quality in 0x4aeec0).
   - d = `[ebp+0x14]` -> `Car+0x1bce8` (guess: car kind 0 local / 1 remote / 2 replay: switch at 0x4609f1 chooses lamp/camera settings 0/1/2).
   - e = `[ebp+0x18]` -> `Car+0x1bcec`.
   - f = `[ebp+0x1c]` -> `Car+0x1ec0` (small int 0..3, guess: player slot; `cmp [ebx+0x1ec0],2` in 0x460b9a, 0x4610..).
   - g = `[ebp+0x20]` -> `Car+0x1bf00` **flags word**: bit0 (`test BYTE [ebx+0x1bf00],0x1`) = spectator/replay visual set (guess), bit1 = no reflection/extra object (`test ...,0x2` skips the 0x330-byte object at 0x460928), **bit2 = no sounds** (skips `InitSounds` 0x4666a0 at 0x45ef69 and `ReleaseSounds` 0x466fe0 in the dtor at 0x461e19).
   The ctor reuses dead argument slots (`[ebp+0x8]`, `[ebp+0x10]`, `[ebp+0x14]`, `[ebp+0x18]`, `[ebp+0x1c]`, `[ebp+0x20]`) as scratch locals after reading them; `[ebp+0x13]` is the high byte of arg c, read as the garbage "allocator byte" that VC6 stores in the first byte of `std::vector`/`std::string` objects (`mov al,[ebp+0x13]; mov [edi],al`). Treat these as artefacts; the matching source probably has the same effect from `std::vector<...>()` default construction.
4. **No vtables, no virtual functions on Car.** The Car class is non-polymorphic (the dtor is called directly and Player calls it via 0x490f1c). All virtual calls are calls *out*: `call [edx]` (slot 0 = scalar deleting dtor, `push 1`) on owned sub-objects, renderer slot `[0x56b0ac]->0x130` (capability query `bool Supports(int cap)`, caps 1,3,6,8,0xb,0xd,0x12).
5. **No STL template bodies in the range** (all `std::vector`/`std::string` helpers are out of line: `_Grow 0x402980`, `_Tidy 0x402700`, `_Eos 0x402960`, `0x4024f0`, `append 0x403ba0`, `operator+ 0x409130`, `copy 0x461a10` (in this range), `vector::reserve 0x4bcf30`, `insert(pos,n,val) 0x4bc680`, `0x403140` copy-ctor of a pointer slot, `0x424320/0x424350` vector insert helpers, `0x4bd00b` `_Xlen`/throw). Only `0x461a10` (a `memcpy`-style `char_traits<char>::copy`) is emitted inside the range.
6. **Config API**: the three loaders use the Config class (0x405580 ctor, 0x4076c0 `Define(keys, count)`, 0x406ce0 `Load(path, requireAll=1, verify=1, checksum)`, 0x407f60 `GetFloat`, 0x4085e0 `GetVec3`, 0x408390 `GetVec2`, 0x407d50 `GetString`, 0x4055a0 dtor) and on failure build `"Missing variables in file: " (0x4fac50) + path` and throw (`push 0x4e4328 ... call 0x4c13ca`), the same pattern as range-46a8a0.md / range-4649f0.md.

## 1. Car member layout revealed by the constructor (embedded objects in construction order)

The ctor's EH unwind table (handler 0x4db2a0, 124 states) lists the members that need destruction. States 0..0x4d are the embedded members, **in declaration order = construction order** (state N is entered right after member N is built, via `mov BYTE PTR [ebp-4], N`); 0x4e..0x7b are `new` temporaries (operator-new rollback `0x4bd000`) and stack temporaries (strings 0x430720, ModelFile 0x4afe20). The **destructor's table (handler 0x4db7c1) has the identical 0x4e members** in the same order (destroyed in reverse). "inl" = constructor fully inlined (zero-init stores), no call. Offsets are all from the unwind action `lea/add ecx,OFF`; sizes are gaps to the next listed member (a member without a destructor does not appear in the table, so gaps may hide trivial members; those are marked "+ trivial").

| State | Offset | Type / count | Ctor | Dtor | Notes |
|---|---|---|---|---|---|
| 0x00 | 0x028 | `Wheel[4]`, stride 0x330 | 0x46c2c0 (via vec-ctor 0x4c11f4, callback pair 0x46c420 dtor) | 0x46c420 | include/unk_46c2c0.h; ends 0xce8. Order fl, fr, rr, rl (see section 3) |
| 0x01 | 0x0ce8 | `Vec3Like[4]`, stride 0x34 (0x34-byte elements, empty ctor 0x490ad0 / dtor 0x402c90) | 0x490ad0 | 0x402c90 | per-wheel geometry block (guess) ; ends 0xdb8; `Car+0xcf0` etc. written in the wheel loop |
| 0x02 | 0x0db8 | `Drivetrain` | 0x46a8a0 | 0x46a900 | include/unk_46a8a0.h; `Car+0xdb8` back pointer = this (0x460239); loaded by `LoadConfig` 0x46a920; runs to ~0xfa0 |
| - | 0x0fa0 | `ModelCache*` | `new 0x3c` 0x4ade40 or Game+0x139424 | 0x4ade80 | trivial pointer, see arg a |
| - | 0x0fa4 | `SharedCache*` (0x20 bytes) | 0x4aee00 or arg a | 0x4aee20 | trivial pointer |
| - | 0x0fa8 | int "owns caches" | | | |
| 0x03 | 0x0fac | intrusive list (head,tail,..; 5 dwords, 0x14 B) | inl (5 zero stores 0xfac..0xfbc) | 0x49ad30 (`~List`) | list of the model's materials (guess): nodes `{next,prev,Material*}` (walk helpers 0x464990 `next`, 0x4649a0 `data`) |
| 0x04 | 0x0fc0 | same list type | inl | 0x49ad30 | |
| 0x05 | 0x0fd4 | same list type | inl | 0x49ad30 | filled by the ctor's final loop (0x461946..0x4619a5): per node a `new 0xd4` copy (0x4a8580 ctor, 0x4a87f0 assign) pushed with 0x4aecf0 |
| 0x06 | 0x0fe8 | `Vec3`-like with empty dtor (0x402c90) | inl/none | 0x402c90 | 0xfe8.. is the first car.ini-derived block (LoadCarIni writes 0xfe8..0x1430) |
| 0x07-0x12 | 0x1384, 0x1390, 0x139c, 0x13a8, 0x13b4, 0x13c0, 0x13cc, 0x13d8, 0x13e4, 0x13f0, 0x13fc, 0x1408 | 12 x `Vec3` (stride 0xc, empty ctor/dtor) | none (trivial) | 0x402c90 | control/input vectors; 0x1338-0x134c control values are zeroed by 0x464680 |
| 0x13 | 0x1430 | `std::vector<T>` (0x10 B: allocator byte, begin, end, cap) | inl, then `reserve(0x400)` 0x4bcf30 (0x45ed56) | 0x401bb0 | followed by `Car+0x1460 = 0x11` (inline, from the vector-adjacent int) ; 0x400 elements |
| 0x14 | 0x1464 | `std::vector<T>` | inl, `reserve(0x400)` 0x45ed66 | 0x401bb0 | followed by `Car+0x1494 = 0x11` |
| 0x15 | 0x1498 | `Vec3`+ (empty dtor) | none | 0x402c90 | |
| 0x16 | 0x14a8 | 0x40-byte object (matrix 4x4, empty dtor) | none | 0x402c90 | |
| 0x17 | 0x14e8 | 0x18-byte object | none | 0x402c90 | `Car+0x14fc` (Stage*, set by `AttachStage` 0x464530) lies in 0x14f0-0x1500 |
| 0x18 | 0x1500 | `Matrix4[6]`, stride 0x40 | 0x490ad0 | 0x402c90 | (guess: 4x4 matrix, empty ctor) |
| 0x19 | 0x1680 | `Matrix4[30]`, stride 0x40 | 0x490ad0 | 0x402c90 | tyre-dynamics block, 30 surfaces (see range-4649f0.md F4) |
| 0x1a | 0x1e04 | `Vec3[4]` | 0x490ad0 | 0x402c90 | |
| 0x1b | 0x1e54 | `Vec3[4]` | 0x490ad0 | 0x402c90 | |
| 0x1c | 0x1e84 | `Vec3[4]` | 0x490ad0 | 0x402c90 | `wheelSurface[4]` is at 0x1e44 (header) |
| 0x1d | 0x1ed0 | 0x60-byte object (empty dtor) | none | 0x402c90 | |
| 0x1e | 0x1f30 | `SpringFollower` | 0x46d720 | 0x402c90 | include/unk_46d720.h; initial `Car+0x1f30 = 250.0 (0x437a0000), +0x1f34 = 3.5 (0x40600000), +0x1f38 = 0.1 (0x3dcccccd), +0x1f44 = 0.5` are written after the ctor call |
| 0x1f | 0x1f4c | `SpringFollower` | 0x46d720 | 0x402c90 | same init values at 0x1f4c.. |
| 0x20 | 0x1f70 | `Matrix4` (hood) | none | 0x402c90 | copy of the "hood" part's matrix (`rep movs 0x10 dwords` from part+0x20) |
| 0x21 | 0x1fb0 | `Matrix4` (trunk) | none | 0x402c90 | copy of the "trunk" part's matrix |
| 0x22 | 0x2030 | `Vec3` | none | 0x402c90 | (zeroed at 0x46054c..) |
| 0x23 | 0x203c | `Vec3` | none | 0x402c90 | |
| 0x24 | 0x205c | `Vec3` | none | 0x402c90 | |
| 0x25 | 0x2068 | `Vec3`..0x20ac | none | 0x402c90 | |
| 0x26-0x2a | 0x20ac, 0x20b8, 0x20c4, 0x20d0, 0x20dc | 5 x `Vec3` | none | 0x402c90 | `Car+0x20dc` = `ext_pipe01` location (set with flag `Car+0x20e8 = 1`) |
| 0x2b | 0x20f0 | `std::vector<T>`-like (byte+3 ptrs, 0x10 B) | 0x405580 (shared tiny ctor: `byte; 3 zeroed`) | 0x4b9b20 (free `[this+4]`, zero ptrs) | list of mesh handles (0x4b9b50 pushes the body, hood and trunk meshes at 0x461725..0x46175c) |
| 0x2c | 0x2100 | emitter A | 0x4773d0 | 0x477450 | include/unk_477370.h; ends 0x222c (size 0x12c) |
| 0x2d | 0x222c | emitter B | 0x479170 | 0x479240 | include/unk_479170.h; ends 0x2358 (size 0x12c) |
| 0x2e | 0x2358 | emitter C | 0x47c460 | 0x47c4e0 | include/unk_47c460.h; ends 0x2488 (size 0x130); `0x47c4f0(&stage+0x1f8)` binds a stage vec3 at 0x4645dc |
| 0x2f | 0x2488 | emitter D | 0x47b4c0 | 0x47b550 | include/unk_47b4c0.h; ends 0x25e8 (size 0x160) |
| 0x30 | 0x25e8 | emitter E | 0x478080 | 0x4780e0 | include/unk_478020.h; ends 0x2748 (size 0x160) |
| 0x31 | 0x2748 | `SkidMarks[4]`, stride 0x6558 | 0x4741a0 (vec-ctor, dtor callback 0x474520) | 0x474520 | include/unk_4741a0.h; ends 0x2748+0x19560 = 0x1bca8; per-instance init `0x474590(&sm, [Car+0x1ec0])` at 0x460688 and `0x4745c0` in Reset |
| - | 0x1bca8..0x1bcc0 | six ints (zeroed in Reset 0x464800: 0x1bcac, 0x1bcb0, 0x1bcb4, 0x1bcb8, 0x1bcbc, 0x1bcc0) | | | |
| 0x32 | 0x1bcbc | 8-byte object with empty dtor | none | 0x402c90 | overlaps the ints above (Vec2) |
| 0x33 | 0x1bcc4 | `std::vector<Object*>` | inl (byte at 0x1bcc4, 3 ptrs at 0x1bcc8/cc/d0) | 0x401bb0 | filled by the ctor's inline `vector::insert` at 0x461350..0x461531; ctor `new`s via `0x4bc680` |
| 0x34 | 0x1bcd4 | `std::vector<Object*>` | inl (0x1bcd4; ptrs at 0x1bcd8/dc/e0) | 0x401bb0 | `SetXxx(int)` 0x464940 writes `obj[+4]` for each element |
| 0x35 | 0x1bd40 | `LightRec[3]`, stride 0x1c | 0x464a70 (vec-ctor) | 0x402c90 | front-left lights 1..3; record `{CarLight* light; byte; int mesh; int; int; int; int}` (0x464a70 zeroes it) |
| 0x36 | 0x1bd94 | `LightRec[3]` | 0x464a70 | 0x402c90 | front-right lights 1..3 |
| 0x37 | 0x1bde8 | `LightRec[2]` | 0x464a70 | 0x402c90 | rear left, rear right |
| 0x38 | 0x1be20 | `LightRec[2]` | 0x464a70 | 0x402c90 | brake left, brake right |
| 0x39 | 0x1be58 | `LightRec[2]` | 0x464a70 | 0x402c90 | reverse 1, reverse 2 (ends 0x1be90) |
| - | 0x1be90..0x1bea0 | ptrs: 0x1be90 `Object*` (deleted with `~Object` 0x4a86d0 + `delete` by the dtor), 0x1be94, 0x1be98, 0x1be9c, 0x1bea0 (`Object*` owned, virtual-deleted) | | | set by the material loop (headlight/rearlight/window meshes) |
| 0x3a | 0x1bed0 | `std::vector<T>` with non-trivial elements (0x34 B) | 0x401270 | 0x4012d0 | |
| 0x3b-0x42 | 0x1bf04, 0x1bf78, 0x1bfec, 0x1c060, 0x1c0d4, 0x1c148, 0x1c1b8, 0x1c228 | 8 x `SoundSource` (stride 0x74, last is 0x70 B up to 0x1c298) | 0x49b630 | 0x49b6e0 | sound.h; 8 separate members (not an array; each gets a state) ; ends ~0x1c298 |
| - | 0x1c298..0x1c2b0 | ints zeroed (0x1c298..0x1c2b0) | | | |
| 0x43 | 0x1c2b4 | `SoundLayer[4]`, stride 0x8c | 0x464a90 (callback) | 0x464aa0 | unk_4666a0.h; ends 0x1c4e4 |
| 0x44 | 0x1c4e4 | `SoundLayer[4]`, stride 0x8c | 0x464a90 | 0x464aa0 | ends 0x1c714 |
| 0x45 | 0x1c718 | `Vec3` | none | 0x402c90 | |
| 0x46 | 0x1c724 | `Vec3` | none | 0x402c90 | `0x1c730..0x1c748` ints zeroed |
| 0x47 | 0x1c75c | `CarSnapshot[3]`, stride 0x8c | 0x46bb90 (vec-ctor, dtor 0x46bbb0) | 0x46bbb0 | include/unk_46bb90.h; ends 0x1c900 |
| 0x48 | 0x1c914 | `Rec28[0x100]`, stride 0x28 | 0x490ad0 | 0x402c90 | 256 x 0x28-byte records (replay/state ring, guess); ends 0x1f114 |
| 0x49 | 0x1f160 | 0x90-byte aggregate (`float[4]` at +0, `8-byte[16]` at +0x10) | 0x40b4e0 | **0x4649f0** (in this range, section 4) | the only instance of that aggregate |
| 0x4a | 0x1f2d4 | `Vec3` | none | 0x402c90 | |
| 0x4b | 0x1f2e0 | `Vec4`/16-byte | none | 0x402c90 | |
| 0x4c | 0x1f2f0 | 16-byte | none | 0x402c90 | |
| 0x4d | 0x1f300 | 12-byte, ends at 0x1f30c = sizeof(Car) | none | 0x402c90 | |

(Order: members from 0x1f144.. are the replay/network counters zeroed in `Reset`; see 0x464800.)

Other fields the ctor/Reset touch that are *not* in the unwind table (plain data): `+0x04/+0x08` (`+0x8 = 2`), `+0x24` **CarBody\*** (new 0x284, ctor 0x41bab0 `(partsFromList, &[0x1034], &[0x1038], &[0x1044])`; dtor virtual slot 0 in the Car dtor), `+0x1f18` **ShadowCaster\*** (new 0x178, ctor 0x4ba5a0; dtor virtual), `+0x1f1c..0x1f28` four shadow-tyre `Object*` (new 0x12c, ctor 0x4a6910(partName)), `+0x1f2c` Object\*, `+0x1f68` hood-crash `Object*`, `+0x1f6c` trunk-crash `Object*`, `+0x1ff0 .. +0x2058` Object\* for wipers (0x1ff4), interior_lo (0x2010), interior_hi (0x2014), steering_wheel_lo (0x204c), steering_wheel_hi (0x2050), indicator_speed (0x2054, copy 0x205c), indicator_rpm (0x2058, copy 0x206c), `+0x2004`/`+0x2008` two more Object\*, `+0x2018,0x201c,0x2028,0x202c` four Object\*, `+0x20ec` **0x330-byte object** (ctor 0x4703c0, init 0x470610(device, row[+8]); created only if `!(flags&2)`, `Supports(0x12)` and `Car+0x1ec0 != 0`; guess: reflection/environment renderer), `+0x1bce4` Object\*, `+0x1bcec`/`+0x1bce8` ctor args, `+0x1bf00` flags, `+0x1bd30/+0x1bd34/+0x1bd38/+0x1bd3c` window-mesh state, `+0x1ec0`, `+0x1ebc`, `+0x1eb4 = 1`, `+0x1eb8 = 0`.

Everything in `0xfe8..0x1430` is filled by `LoadCarIni` (0x462350, 197 distinct field offsets from 0xfe8 to 0x1f14, per section 2) except the already-listed lights/tyre blocks. The `float` base set at 0x45ea81.. : many `1.0f` (`0x3f800000`) initial values at 0x1f70..0x1fec (hood/trunk/wiper animation) and `0.5f` at 0x1f44/0x1f60.

## 2. Functions (table)

| Addr | Size | Conv / frame | Role | Main callers |
|---|---|---|---|---|
| 0x45e480 | 13705 | thiscall, `ret 0x1c`, ebp frame 0x1bc locals, SEH (EH 0x4db2a0, 124 states), FPU (inline matrix/float ops) | **`Car::Car(a,b,c,d,e,f,g)`**; see section 3 | Player::CreateCar 0x491269 |
| 0x461a10 | 35 | cdecl, no frame | `memcpy`-style `char_traits<char>::copy(dst, src, n)` (rep movs dwords+bytes, returns dst); the only STL body in the range; inlined `string::append` calls it at 0x4616ce | 0x4616ce (ctor) |
| 0x461a40 | 2276 | fastcall (ecx=this), SEH (EH 0x4db7c1, 79 states), no FPU | **`Car::~Car`**: explicit body deletes owned sub-objects, ends with `ReleaseSounds` (if `!(flags&4)`), then compiler-generated member destruction in reverse (section 5) | Player dtor 0x490f1c |
| 0x462330 | 30 | thiscall, `ret 4` | scalar deleting dtor: `this->~T()` (0x4a86d0, direct), `if (flags&1) operator delete(this)`; type T = the Object-like class held by the three lists at 0xfac/0xfc0/0xfd4 | Car dtor 0x461d70, 0x461d9d, 0x461dca |
| 0x462350 | 4484 | thiscall `(const char* iniPath, unsigned checksum)`, `ret 8`, SEH (EH 0x4db829), FPU | **`Car::LoadCarIni`**: Config with 100 keys at 0x4fadd8 (`Define(0x4fadd8, 0x64)`), Load(path,1,1,checksum); throws on failure; fills Car+0xfe8..0x1f14. Keys: WheelDisplacement, WheelAligningTorqueLimits, Overlay{Meter,Speed,Gear,Rpm}Location, Overlay{Speed,Rpm}Size/Max, Interior{Rpm,Speed}Max, DriverType, InteriorLoc, BumperLoc, HoodLoc, DriverLoc, CoDriverLoc, FF{Friction,Centering}{NominalLoad,Offset}, Trunk/HoodAngle(+Max), Wiper*, Reflection, OpacityBias, Mass, MomentOfInertia, CenterOfMass, AeroDrag(+Loc), DownForceConst, CenterOfDownForce, Damping, DamageSpeedRange, DriveLight*, {Front,Rear,Brake,Reverse}Flare{Radius,Sharpness,AlphaMin}, Front/RearTraction, BrakeBalance/Torque, HandBrakeTorque, Steering*, TireTurnAngleIn/Out, TireCenteringSpeed, TireWidth/Radius/Mass, {Front,Rear}TireMomentOfInertia, SolidRearAxle, {Front,Rear}Suspension{Min,Max,Rest}Length/Const, RollBarStiffness, BumpDamp, ReboundDamp, {Front,Rear}Bumper{Length,Const}, {Front,Rear}CamberAngle/TravelUp/TravelDown | ctor 0x45ee90 |
| 0x4634e0 | 1115 | thiscall `(path, checksum)`, `ret 8`, SEH (EH 0x4db874) | **`Car::LoadTiresIni`**: 45 keys at 0x4fac70 (`Define(0x4fac70, 0x2d)`): Tire0..4 x {RollingResistance, InducedDragCoeff, PneumaticTrail, PneumaticOffset, ZStiffness, XStiffness, CStiffness, ZFriction, XFriction} (5 tyre sets x 9) into Car+0x1518.. (tyre parameter blocks); same error/throw path | ctor 0x45ef1a |
| 0x463940 | 2926 | thiscall `(const std::string& dir)`, `ret 4`, ebp frame, SEH (EH 0x4db940) | **`Car::LoadTextures`** (guess name): for each of `FlareFront.tga` (0x4fbec8), `FlareRear.tga`, `FlareBrake.tga` (+ `"flare brake"` 0x4fbe9c name at 0x404ba0), `FlareReverse.tga`, `LightsLit.tga`, `LightsBraking.tga`, `lights_damaged.tga`, `windows_damaged.tga`, `interior_windows_damaged.tga`, `OverlayMetersKmh.tga` (0x4fbe04) / `OverlayMetersMph.tga` (0x4fbdf0 + `Mph` text): builds `dir + name` (`operator+` 0x409130, `_Tidy` 0x402700), loads through `TextureCache::Get(name)` 0x4aef30 on `Car+0xfa4`, and `new 0x12c`/`0xd4` materials (0x4a8580) for the flare sprites; stores textures into the CarLight records / `Car+0x1bd..`/overlay fields | ctor 0x45ef33 |
| 0x4644b0 | 124 | thiscall `(float level)`, `ret 4`, FPU | `Car::SetLightIntensity(level)`: writes `light->[0x130] = level` for the 12 CarLight objects (groups 0x1bd40/0x1bd94 x3, 0x1bde8/0x1be20/0x1be58 x2 left/right pairs) | ctor 0x4618f6, 0x464530 (0x46456f) |
| 0x464530 | 323 | thiscall `(Stage* stage, int b)`, `ret 8`, FPU | **`Car::AttachStage` (byte-exact):** `Car+0x14fc = stage`; if `Car+0x1bce8 != 0` calls `0x464940(b==2 ? Car+0x1bce4 : stage+0x9e4)`; `SetLightIntensity(stage+0x1a0)`; points four SkidMarks to `stage+0x908`; copies stage fields into the shadow, emitters, wipers and car.ini lights. The target's final wiring block stores `stage+0x9e0` at `Car+0x247c/0x2224/0x2350/0x270c`, `stage+0x9dc` at `Car+0x25ac`, and passes the address `stage+0x1f8` to `0x47c4f0` | RaceMode/Stage start 0x48616d, 0x48e084 |
| 0x464680 | 63 | fastcall (ecx=this), no frame | `Car::ResetControls`: zero `Car+0x1338,0x133c,0x1340,0x1344,0x134c,0x1384,0x1388,0x138c,0x142c,0x1befc` (throttle/brake/steer inputs) | ctor 0x460923 |
| 0x4646c0 | 305 | fastcall (ecx=this), no SEH, no FPU | **`Car::AttachLights` (byte-exact):** owner is `body+0x124` mesh-list element 0; two three-iteration paired-light loops cover records at `Car+0x1bd40` and `+0x1bd94` (stride 0x1c), then a two-iteration loop covers d/c/e records at `+0x1be20`, `-0x38`, `+0x38`; each active light is attached to the body, its `light+0x14` mesh is looked up, the flag byte is cleared, and only the first two groups and d records write through their target pointers | ctor 0x4605ea |
| 0x464800 | 316 | fastcall (ecx=this), no SEH | **`Car::Reset`**: clears `Car+0x1bcac..0x1bcc0`, `SkidMarks::Reset` 0x4745c0 for the four marks, zero-fills `Car+0x1354` (8 dwords), `0x1374,0x1378`, `0x1f2b0..0x1f2d0`, `0x1f114`, `0x1f120/0x1f13c/0x1f144/0x1f148/0x1f2ac`, `0x1f24c..` (8x12-byte records), truncates the two vectors at `Car+0x1430` and `Car+0x1464` (the inline `erase(begin,end)` loops), sets the message `Car+0x1c908 = 0, Car+0x1c90c = -60000 (0xffff15a0)` | ctor 0x46192a, Player 0x4912f6 |
| 0x464940 | 53 | thiscall `(int v)`, `ret 4` | for each element `p` of the vector at `Car+0x1bcd4`: `p->[+4] = v` | 0x464530 (0x46455e) |
| 0x464980 | 13 | fastcall | `const char* std::string::c_str()` (returns `[this+4]` or the empty string 0x4e1344) | ctor 0x4616e4 |
| 0x464990 | 3 | fastcall | list node `Next()` (`return [ecx]`) | ctor 0x46170d, 0x461943, 0x46199a |
| 0x4649a0 | 4 | fastcall | list node `Data()` (`return [ecx+8]`) | ctor 0x461950 |
| 0x4649b0 | 14 | fastcall | `part->mesh(0)`: `0x4a6800(&part+0x124, 0)` (mesh-container `at(0)`) | ctor 0x461720, 0x46173d, 0x461754 |
| 0x4649c0 | 48 | `_$E` stub | guard bit 0x5826d5 + `atexit(0x402c90)` (init-table entry; file has no statics) | init table (.data off 0xefbb4) |
| 0x4649f0 | 94 | fastcall, SEH (EH 0x4db963) | implicit dtor of the 0x90-byte aggregate at `Car+0x1f160` (vec-dtor of `float[4]` at +0 and `8B[16]` at +0x10 with 0x402c90). No `call`/`jmp` from normal code: reached only as the `jmp 0x4649f0` of the unwind funclets for state 0x49 (ctor table at 0x4db063, dtor table at 0x4db768); the normal-path Car dtor inlines the same two `0x4c1116` vec-dtor calls at 0x461e3c-0x461e5b (lines `push 0x402c90; push 0x10; push 0x8`) | EH funclets only |
| 0x464a50 | 21 | fastcall | `Car::ResetMessage()` inline (`unk1c908 = 0; unk1c90c = -60000`) | ctor 0x4619a7 (the same two stores are inlined in Reset) |
| 0x464a70 | 32 | fastcall | `LightRec` ctor: `[0]=0; byte[4]=0; [8],[0xc],[0x10],[0x14],[0x18]=0` (0x1c-byte record) | vec-ctor callback of the five arrays at 0x45e6f7.. (abs refs) |
| 0x464a90 | 15 | fastcall | `SoundLayer::SoundLayer()` (`SoundSource` ctor 0x49b630 at `this+4`) | vec-ctor callback 0x45e873/0x45e894 |
| 0x464aa0 | 16 | fastcall | `SoundLayer::~SoundLayer()` (`jmp 0x49b6e0` on `this+4`) | vec-dtor callback; EH (0x4dafc9..) |

Not in functions.csv but used here (outside the range or folded): 0x464a70 and 0x464aa0 above (the csv lists 0x4649f0, 0x464a50, 0x464a90; 0x464a70/0x464aa0 appear to be folded into their neighbours' sizes by the analyzer; function boundaries from the bytes: 0x464a50 (21 B, ends 0x464a64 then padding), 0x464a70 (32 B), 0x464a90 (15 B), 0x464aa0 (16 B)).

## 3. The constructor 0x45e480 in detail

Frame: `push ebp; mov ebp,esp; push -1; push 0x4db2a0; ... sub esp,0x1bc; push ebx,esi,edi`; `ebx = ecx = this`; `[ebp-0xc8] = this` (saved for the EH funclets). `esi` is the constant 0 for most of the function. Local frame (ebp-relative; the compiler overlays disjoint locals, so read these as the roles seen):

| Local | Role |
|---|---|
| `[ebp-4]` | EH state (0..0x7b) |
| `[ebp-0x48]` | `std::string dir` ("data/cars/car_N/") ; destroyed by 0x430720 at the end (state 0x4d) |
| `[ebp-0x70]` | `char itoa[0x28]` buffer (`0x4c1902(b, buf, 10)`) |
| `[ebp-0x80]`, `[ebp-0x9c]`, `[ebp-0xac]` | `std::string` temps (file names: `car.ini`/`engine.ini`/`tires.ini` temp at -0x80; `model.bmf` at -0x9c; `collision.bmf` at -0xac); -0x80 area is later reused as `Part* lodTyre[4]` (-0x80, -0x7c, -0x78, -0x74) |
| `[ebp-0x24]..[ebp-0x18]` | `Part* tyre[4]` in wheel order fl, fr, rr, rl (from `tire_fl`, `tire_fr`, `tire_rr`, `tire_rl`); also reused as scratch string `[ebp-0x24]` in the tail |
| `[ebp-0x1c8]` | `ModelFile` #1 object (ctor 0x4af950 `(modelPath, dir, cache, &Car+0xfac, flag)` for `model.bmf`; dtor 0x4afe20); its part vector begin/end at `[ebp-0x19c]` / `[ebp-0x198]` (= object+0x2c / +0x30) |
| `[ebp-0x148]` | `ModelFile` #2 (collision.bmf, same ctor with flag 0); part vector at `[ebp-0x11c]` / `[ebp-0x118]` |
| `[ebp+0x20]` | current Part\* in the loops (scratch of arg g) |
| `[ebp+0x10]` | current part name `const char*` (scratch of arg c) |
| `[ebp+0xc]` | loop index i (scratch of arg b; b is already consumed) |
| `[ebp+0x18]` | `Part* body`; `[ebp+8]` = `Part* body_lod`, then later `&classRow = &0x4fb0f8[c]` (0x45feb6) |
| `[ebp-0x4c]` hood, `[ebp-0x14]` trunk, `[ebp-0x34]` body_crash/crash, `[ebp-0x30]` trunk_crash, `[ebp-0x10]` hood_crash | other Part\* picked in the loop |
| `[ebp-0xc4]` (0x14 B), `[ebp-0xb1]` | 5-dword copy of `0x4fbd70` (5 chars?) with `cl = [ebx+0x1030]+'0'` patched in (`add cl,0x30` -> a "NNNN" id string; passed to 0x455070 as a std::string-by-pointer) |
| `[ebp-0x84]..[ebp-0x88]` | window-material loop temporaries (mesh `esi=[node+8]`) |
| `[ebp-0x54]`, `[ebp-0x50]`, `[ebp+0x14]`, `[ebp+0x18]`, `[ebp+0x1c]`, `[ebp-0x58]..[ebp-0x68]` | window/lamp parameter set chosen by the kind switch at 0x4609f1 (`Car+0x1bce8` = 0/1/2) |

### 3.1 Phases (address ranges; for work groups)

| # | Range | Bytes | What |
|---|---|---|---|
| A | 0x45e480-0x45e8f2 | 1138 | EH prologue; construct embedded members states 0..0x47 in order (vec-ctor 0x4c11f4 for arrays, direct calls for Drivetrain 0x46a8a0, SpringFollower x2 0x46d720, emitters 0x4773d0/0x479170/0x47c460/0x47b4c0/0x478080, Config/vector 0x405580, 0x401270, SoundSource x8 0x49b630); inline list/vector ctors (zero stores) for 0xfac.. and 0x1430/0x1464/0x1bcc4/0x1bcd4 |
| B | 0x45e8f2-0x45e9c8 | 214 | Rec28[0x100] (0x1c914), the 0x90 aggregate (0x40b4e0), `Car+0x1ec0/0x1ebc/0x1bf00` from args, cache objects (arg a): either borrow or `new 0x3c`/`new 0x20` + inline init |
| C | 0x45e9c8-0x45ed31 | 873 | `0x4aeec0(row.count clamp)` on the shared cache; store args `+0x1bce8/ec`; zero-fills and default floats (1.0, 0.1, 0.5, 250.0, 3.5 ... 0x1f30-0x1fec etc.), `Car+0x8 = 2`, vectors `reserve(0x400)` x2 |
| D | 0x45ed31-0x45f061 | 816 | build `dir` string, call `LoadCarIni` (0x45ee90), `Drivetrain::LoadConfig` (0x45eed7), `LoadTiresIni` (0x45ef1a), `LoadTextures` (0x45ef33), `InitSounds` (0x45ef7c, if `!(flags&4)`), then two `ModelFile` constructions (0x45efc3 model.bmf, 0x45eff8 collision.bmf) and the zeroing of the part pointer locals |
| E | 0x45f03a-0x45fe9f | 3425 | **part loop** over `ModelFile#1` parts (`for (i=0; i<size; ++i)`): chain of `strcmp(name, "...")` (0x4ce290, `==0` branches) with the names listed in 3.2; each branch stores the part pointer or builds an Object/CarLight; the unwind states 0x58..0x77 are the `new`s inside. ~35 near-identical branches (lights: 12 x ~55 B) |
| F | 0x45fea4-0x4605df | 1851 | after the loop: pick LOD/crash parts from `row=0x4fb0f8[c]`; optionally `new 0x12c` Object for `interior` (0x2014), `0x1ff0`; copy tyre LOD pointers; build `CarBody` (`new 0x284`, 0x41bab0) -> `Car+0x24`; copy `dir`-based name `0x4fbd88` into `body+0x114` (std::string assign inline); **wheel loop** x4 (0x460028..0x4600aa: `part->mesh(0)`, copy part+0x14..0x1c (pivot) into wheel fields at +0xce8-stride (0x34) and `+0x44`, `+0x...+0x108`, call `Wheel::Init` 0x46c480); collision part (second `ModelFile` loop, finds `"collision"` -> `0x41c920(body, part)`); crash/hood/trunk link-up (`0x4b91c0`); the per-wheel float setup loop 0x460241..0x4604f1 (suspension geometry from car.ini fields: divides by `Car+0x10fc`, `Car+0x12ac`, etc., 4 iterations, stride 0x330); then `0x455070(&idStr)` (Car::SetupDynamics? guess), `0x455ec0`/`0x4560a0` if `row[6]`, `SetCockpit(0)` (0x4691e0) and `AttachLights` (0x4646c0) |
| G | 0x4605ef-0x460659 | 106 | virtual-delete (`call [vtbl]` with `push 1`) of every element of the two `ModelFile` part vectors ([ebp-0x17c]/[ebp-0x178] and [ebp-0xfc]/[ebp-0xf8], i.e. object+0x4c/+0x50 minus 0x1c8/0x148) |
| H | 0x460659-0x460bd2 | 1305 | attach emitters/shadow to the body (`0x4b2b30(body)` on Car+0x28.., 0x2748 SkidMarks x4 via 0x474590, emitters at 0x2358/0x2488/0x25e8/0x2100/0x222c, wiper/indicator Object\*), allocate the `ShadowCaster` (new 0x178, 0x4ba5a0), the optional 0x330-byte object (0x20ec), a 4x4 rotation (0x4b5760), the camera/lamp kind switch (0x4609f1..0x460b0a; renderer capability queries 1,3,6,8,0xb,0xd,0x12), `Car+0x1bd38/3c` window parameters, `Car+0x1bcf8->[+0x60]` |
| I | 0x460bd2-0x46170a | 4408 | **material loop**: `for (node = Car.list0xfac.head; node; node = node->Next())` over the model's materials (`mat = node->data`; `mat+0xa8` name `const char*`, `mat+0x9c` flags, `mat+0xb8`/`+0xbc`/`+0x98`/`+0x4c`/`+0x38`/`+0x87`): string compare chain against `"#E"` (prefix test 0x4c1c10 `strncmp`), `"#E1 rearlight_left"`, `"#E1 rearlight_right"`, `"#E1 headlight_left"`, `"#E1 headlight_right"`, `"#E1 window front"`, `"#E1 window back"`, `"#E1 window leftside"`, `"#E1 window rightside"`, `"windows"`; per match: create `Object` via 0x4a6ac0, store in `Car+0x1be90..0x1bea0`, `Car+0x1be48/4c` etc.; inline `std::string` ops (0x402aa0 size, 0x402700, 0x402ab0, 0x4bd00b), inline `vector<Object*>::insert` (0x4bc680, 0x403140, 0x424320/50) for `Car+0x1bcc4`; for each the `dirt.tga` (0x4fbc9c) name string is appended via 0x461a10 and looked up with `0x4aef30(c_str)` |
| J | 0x46170a-0x461a06 | 764 | `0x4649b0`/`0x4b9b50` push body/hood/trunk meshes into `Car+0x20f0`; if `Car+0x1bce8 != 1` and (`row[4]` or `DAT_0050145c`) release the textures (`0x4ae0e0(cache, ptr)` ~30 calls) for the unused parts; calls `SetLightIntensity(1.0)` (0x4644b0), `0x465e90(1)` (setup), `0x466500(2)`, `0x466520(2)`, `0x466590(2)`, `0x466610(2)` (the four setup setters from range-4649f0.md F4), `Reset` (0x464800), `0x48e660` (empty), `0x45a850`; then the final loop building the copy list at `Car+0xfd4` (0x4a8580, 0x4a87f0, 0x4aecf0); `0x464a50`; destroys the strings/ModelFiles (states 0x56..0x4d); `ret 0x1c` |

### 3.2 Model part names (`.bmf` object names; strings in 0x4e1c3c..0x4e1e6c, 0x4fbc9c..0x4fbddc)

Compared in this order inside the loop (phase E): `body`, `body_lod`, `hood`, `trunk`, `wipers`, `interior_lo`, `interior_hi`, `steering_wheel_lo`, `steering_wheel_hi`, `indicator_rpm`, `indicator_speed`, `tire_fl`, `tire_fr`, `tire_rr`, `tire_rl`, `tire_fl_lod`, `tire_fr_lod`, `tire_rr_lod`, `tire_rl_lod`, `shadow_tire_fl`, `shadow_tire_fr`, `shadow_tire_rl`, `shadow_tire_rr`, `light_front_left01..03`, `light_front_right01..03`, `light_rear_left`, `light_rear_right`, `light_brake_left`, `light_brake_right`, `light_reverse01`, `light_reverse02`, `ext_pipe01`; after the if-chain (outside it): `body_crash` or `crash` -> `[ebp-0x34]`, `trunk_crash` -> `[ebp-0x30]`, `hood_crash` -> `[ebp-0x10]`. Collision pass (phase F): `collision`. Material pass (phase I): `windows`, `#E1 window rightside/leftside/back/front`, `#E1 headlight_right/left`, `#E1 rearlight_right/left`, prefix `#E`.

Destinations: `hood`/`trunk` copy their 0x40-byte matrix into `Car+0x1f70` / `Car+0x1fb0`; `wipers` -> `Car+0x1ff4` (Object, `new 0x12c`, ctor 0x4a6910(part)); `interior_lo/hi` -> 0x2010/0x2014 (the hi one only when `row[6]` or `flags&1`); `steering_wheel_lo/hi` -> 0x204c/0x2050; `indicator_rpm` -> 0x2058 and copy 0x206c; `indicator_speed` -> 0x2054 and copy 0x205c; `shadow_tire_fl/fr/rr/rl` -> `Car+0x1f1c/20/24/28`; lights -> `LightRec` pointers `0x1bd40, 0x1bd5c, 0x1bd78 (front left 1-3), 0x1bd94, 0x1bdb0, 0x1bdcc (front right 1-3), 0x1bde8, 0x1be04 (rear L/R), 0x1be20, 0x1be3c (brake L/R), 0x1be58, 0x1be74 (reverse 1/2)` as `CarLight*` (`new 0x13c`, ctor 0x46e8e0, `CarLight::Init` 0x46ea00(Car+0x10ac) after copying part+0x14..0x1c and the 0x40-byte matrix part+0x20; sets `light+0x124 = Car+0x1bebc`, `light+0x134 = Car+0x10b4`, `light+0x128 = Car+0x10b0`); `ext_pipe01` -> `Car+0x20dc..0x20e4` (vec3 from part+0x14..) and `Car+0x20e8 = 1`.

### 3.3 Config/asset loader ordering (what the ctor calls, in order)

`LoadCarIni(dir+"car.ini", table0x4fb130[b])` -> `Drivetrain::LoadConfig(dir+"engine.ini", table0x4fb188[b])` -> `LoadTiresIni(dir+"tires.ini", table0x4fb1e0[b])` -> `LoadTextures(dir)` -> `InitSounds(dir)` (unk_4666a0.h, only `!(flags&4)`) -> models `model.bmf`, `collision.bmf`.

## 4. Compiler-generated / late functions of this file

- Stub 0x4649c0 (not a function in functions.csv's sense; 48 bytes).
- **Late COMDAT-style helpers** (emitted after the stub): 0x4649f0 (implicit `~Aggregate90`), 0x464a50 (inline setter `Car::ResetMessage`), 0x464a70 (LightRec default ctor), 0x464a90 / 0x464aa0 (SoundLayer ctor/dtor). 0x4649f0 has the vec-dtor pattern (`__vec_dtor`-style `0x4c1116` x2, `push 0x402c90`), SEH frame (EH 0x4db963, 2 states).
- Inside the front part: 0x461a10 (`char_traits::copy`), 0x462330 (scalar deleting dtor), 0x464980 (`string::c_str`), 0x464990/0x4649a0 (list `Next`/`Data`), 0x4649b0 (`Mesh::at(0)` wrapper).
- No 6- or 5-byte thunks like 0x480740/0x480770 in this range; no vtable; no `operator new[]`.

## 5. The destructor 0x461a40 in detail

`ecx = this` -> `esi`; SEH state starts at 0x4d (`mov [esp+0x1c], 0x4d`) and the unwind table is the same 0x4e-state table as the ctor (states 0..0x4d members, 0x4e = temporary when `0x4d`->?). Body (user code), in this order:
1. `delete Car+0x1f18` (virtual slot 0, `push 1`).
2. four `Object*` at 0x1f1c..0x1f28 (loop of 4).
3. `0x1be9c`, `0x1bea0`.
4. loop x3: for LightRec group stride 0x1c (bases 0x1bd94 and 0x1beb0): virtual-delete `[rec - 0x54]`, `[0x1beb0+i*4]...`: delete the `CarLight*` of each of the 12 light records plus the entries at 0x1be90+.. (three arrays x 3,3,2... then 2x2 loop for 0x1be20 base).
5. `~Object` + `delete` for `Car+0x1be90` (inline; 0x4a86d0 then 0x4bd000).
6. loop for `0x1be20` base x2 (3 records each).
7. `Car+0x2090, 0x2094, 0x2080, 0x2084, 0x2088, 0x208c`: `delete` after 0x401c30 (~`vector<T>`/array dtor), then virtual-delete of `0x1f2c, 0x1ff0, 0x2018, 0x201c, 0x2028, 0x202c, 0x2010, 0x2014, 0x204c, 0x2050, 0x2054, 0x2058, 0x1f68, 0x1f6c, 0x1ff4`, `0x1ff8` (`~T` 0x401c30 + delete), `0x2004`, `0x2008`, `0x24` (**CarBody**), `0x20ec`, `0x1bce4`.
8. The three lists 0xfac, 0xfc0, 0xfd4: pop every node, `scalar_deleting_dtor(node->data)` (0x462330, `push 1`), `list.remove(node)` (0x414490).
9. If `Car+0xfa8 != 0`: `~SharedCache` (0x4aee20) + delete `Car+0xfa4`; `~ModelCache` (0x4ade80) + delete `Car+0xfa0`.
10. `if (!(flags & 4)) ReleaseSounds()` (0x466fe0).
Then compiler-generated member destruction in reverse (section 1's table, states 0x4d..0): this tail of the function (0x461e29-0x462323, ~1210 B) is not written in source; it is the implicit member dtor sequence, and the funclets live in the EH table (0x4daad0..0x4db2a0 for the ctor, 0x4db2b0..0x4db7c1 for the dtor).

## 6. Suggested work groups

Do **not** split the ctor into separate functions: the whole function shares the EH state numbering, `ebx = this`, `esi = 0`. The recommended approach for byte-matching is to draft it as one function in several pieces and concatenate, with the members declared in the exact order of section 1 so the unwind table, the inline ctor sequences and the state numbers match automatically (header work first: declare the Car members from the table, then the ctor). The header (`include/unk_481390.h`) currently holds only the fields the earlier modules read; it must be widened to the full layout before drafting the ctor/dtor (WG0).

| Group | Range | Bytes | Task |
|---|---|---|---|
| WG0 | header only (no code bytes) | - | Car member declarations in construction order from section 1, wrappers for the 0x1c-byte `LightRec`, `SoundLayer` (0x8c), `Rec28` (0x28), `Vec3`/matrix helper types with `empty dtor` (0x402c90/0x490ad0), the three `List` members (dtor 0x49ad30), the two `vector` members, the 0x90 aggregate. Needed by all other groups. |
| WG1 | 0x461a40-0x462323 `~Car` + 0x462330 + 0x461a10 + 0x4649f0 (+0x464a50/0x464a70/0x464a90/0x464aa0) | 2276 + 30 + 35 + 94 + 21 + 32 + 15 + 16 = 2519 | destructor (user body = phases 1-10 of section 5), `memcpy`-copy helper, scalar deleting dtor, the late helpers. One agent; the dtor is mostly straight-line virtual deletes. |
| WG2 | 0x462350 `LoadCarIni` | 4484 | big, alone: 100 Config reads (GetFloat/GetVec3/GetVec2/GetString), throws on failure; copy the Config pattern from range-46a8a0.md. Key table 0x4fadd8 (100 `{name*, type}`). |
| WG3 | 0x4634e0 `LoadTiresIni` + 0x4644b0 + 0x464530 + 0x464680 + 0x4646c0 + 0x464800 + 0x464940 + 0x464980/90/a0/b0 | 1115 + 124 + 323 + 63 + 305 + 316 + 53 + 13 + 3 + 4 + 14 = 2333 | tire.ini loader (45 keys, 0x4fac70) and the small wiring helpers. |
| WG4 | 0x463940 `LoadTextures` | 2926 | texture/overlay/flare loader, string concat heavy; ebp frame, SEH. Alone. |
| WG5 | ctor phases A-D | 0x45e480-0x45f03a = 3002 B | prologue, the embedded-member construction (this is where the whole EH table is matched), arg-dependent cache setup, default values, path building, the loader calls, ModelFile construction. Needs WG0. Largest risk: inline vector/list ctor ordering and the `new 0x3c`/`0x20` branch. |
| WG6 | ctor phase E (part loop) | 0x45f03a-0x45fe9f = 3425 | the `strcmp` if-chain (~35 branches), local pointer variables; needs the `Part` struct (ModelFile part: name at +0x118, matrix at +0x20, pivot at +0x14, `mesh` at +0x124). Alone. |
| WG7 | ctor phases F+G+H | 0x45fea4-0x460bd2 = 3374 | after-loop assembly: CarBody, wheel loop, per-wheel float setup, effects attach, shadow/0x330-byte object, camera/lamp-kind switch. Heavy FPU. |
| WG8 | ctor phase I (material loop) | 0x460bd2-0x46170a = 4408 | material list walk, `#E1 ...` windows/lamps, inline `std::string` and `vector::insert` expansions. Alone (hardest part: many inline template expansions). |
| WG9 | ctor phase J + stub | 0x46170a-0x461a06 = 764 + stub 0x4649c0 (48) | tail calls, ~30 `ReleaseTexture` calls, final list copy, string/ModelFile destructors, `ret 0x1c`; plus the `_$E` stub and ordering of the file. |

If a smaller split is wanted for the ctor, the safe cut points are the phase boundaries above (the `[ebp-4]` EH state constants are `0x4d` at the end of A..D and `0x57` for E..J; state `0x57` is "all ModelFiles built"), and the shared-local table of section 3.

Total range: 26 KB, 20 listed functions (+ 0x464a70, 0x464aa0 folded + stub). Compiler-generated functions: 0x461a10, 0x462330, 0x4649f0, 0x464a70, 0x464a90, 0x464aa0 (plus inline setter 0x464a50); the ctor/dtor member sequences are implicit.

## 7. Open questions / guesses to verify

- Member names for 0x1384..0x1408 (12 Vec3) and 0x1498-0x14e8 blocks: unknown; LoadCarIni writes 0xfe8..0x1430 (197 offsets) and names them (the key order gives the field order).
- The identity of the class behind `0x4a6910/0x4a86d0/0x4a6800/0x4a6ac0` (Object, 0x12c bytes, mesh container at +0x124) is the same as in range-46a8a0.md; `CarLight` (0x13c bytes) is unk_46e880.h.
- Whether `0x4aeec0` (shared cache clamp) takes the row dword as a shadow-quality level (guess).
- `Car+0x1bce8` semantics (0 local, 1 remote, 2 replay) is a guess from the switch at 0x4609f1.
