<!-- Subagent analysis, 2026-10-22; read-only disassembly; guesses are marked. -->
# Range 0x49b630 - 0x4a181f: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py plus my own caller/abs-ref index over .text and the whole file, .rdata/.data reads. "Guess" marks inference. EH handler addresses (0x4ddd76 ... 0x4de3c2) are the `push imm` after `push -1` in each SEH prologue.

## 0. Headline findings

1. **"PlayerSound" is not a sound class. It is an AI driver.** The 0x574-byte class with ctor 0x49b8e0 / dtor 0x49ba00 loads `data/extras/ai_<n>.ini` (string 0x500e88 + itoa([[this+0x14]+0x2c], base 10) + ".ini" at 0x4f5250) with keys BrakingEnd, BrakingHardness, BrakingStart, OversteeringStrength, SlowDownHardness, SlowDownStart (0x500e4c/3c/58/24/00/14). It steers/throttles the Car via Car+0x134c/0x1350/0x1338 and tracks an AI path. I call it `AIDriver` (guess). There is no Miles code in it. It is owned by Player+0x1a80.
2. **No vtables anywhere in the range.** No ctor stores an .rdata pointer, and a byte scan of the whole exe finds no dword equal to any function start in the range except `push imm` code references (0x49b960, 0x49b9b0 as vec-ctor/dtor callbacks, 0x4a1790, 0x4a17d0, 0x4a1810 as atexit callbacks). All calls are direct. The only virtual call is a call *out*: `call [edx+0x34]` on the RaceMode global 0x501450 (slot 13) in 0x49f560.
3. **No STL instance bodies are defined in the range.** All `vector` helper calls go to out-of-line instances elsewhere (0x498300 ptr-copy, 0x403130 and 0x484380 erase, 0x4bc680 insert(pos,n,val), 0x4c11f4/0x4c1116 vec ctor/dtor iterators). Each module still has the `_$E` stub, so each includes the STL headers.
4. **The range holds 10 modules** (guard stubs at 0x49b8b0, 0x49f8e0, 0x49feb0, 0x4a14b0, 0x4a1500, 0x4a1670, 0x4a16a0, 0x4a16d0, 0x4a1700, 0x4a1730), then the *static initialisers of the WinMain module* at 0x4a1760-0x4a181f. Stubs and the 12-byte thunk are not in functions.csv (stubs are folded into the previous function's size in some tools; address them separately).
5. **No file-scope statics** in any of these ten modules (none has an initialiser pair at its start). The first statics seen are WinMain's at 0x4a1760.
6. The AIDriver class is split over **two** TUs (0x49b8e0 block and 0x49fee0 block) with the AI path classes in between. Treat it as one header, two .cpp files.

Module table (end = address before the next module starts; sizes of stubs 0x30):

| # | Range | Guard stub | Content | Suggested file |
|---|---|---|---|---|
| M1 | 0x49b630 - 0x49b8df | 0x49b8b0 | `SoundSource` member functions (class is declared in sound.h/sound.cpp; this is a separate TU from the Miles module that ends with the stub at 0x49b5c0) | src/unk_49b630.cpp |
| M2 | 0x49b8e0 - 0x49f90f | 0x49f8e0 | `AIDriver` part 1 (ctor, dtor, Update, steering, speed, segment tracking, stuck logic) | src/unk_49b8e0.cpp |
| M3 | 0x49f910 - 0x49fedf | 0x49feb0 | `AiPath` (guess; the "road" owned by Stage, loaded from `<stage>.ai`) | src/unk_49f910.cpp |
| M4 | 0x49fee0 - 0x4a14df | 0x4a14b0 | `AIDriver` part 2 (opponent/obstacle detection and avoidance) | src/unk_49fee0.cpp |
| M5 | 0x4a14e0 - 0x4a152f | 0x4a1500 | `AiNode` ctor only (0x30-byte path node) | src/unk_4a14e0.cpp |
| M6 | 0x4a1530 - 0x4a169f | 0x4a1670 | `AiSegment` ctor and dtor (0x98 bytes) | src/unk_4a1530.cpp |
| M7-M10 | 0x4a16a0, 0x4a16d0, 0x4a1700, 0x4a1730 (0x30 each) | one each | **empty modules**: only the `_$E` stub (guess: .cpp files that include the STL headers but emit no code, e.g. all-inline classes) | nothing to decompile |
| - | 0x4a1760 - 0x4a181f | none | start of the WinMain module: three static initialisers (see section 8) | belongs to winmain.cpp |

## 1. M1: SoundSource, 0x49b630 - 0x49b8df (9 fns + stub)

The class `SoundSource` already exists in the sound.cpp/sound.h work (NOTES.md: "SoundSource+0x5c is a bitfield word"). Global 0x56b0b0 is the sound system (T_56b0b0). Object size 0x6c (guess: ctor writes up to +0x68). Embedded: 4x at Game+0x139224 (stride 0x70, Game ctor 0x430504..0x430534), 8x in Car (stride 0x74, Car ctor 0x45e800..0x45e869), as `this+4` of a wrapper (0x464a90 ctor / 0x464aa0 dtor), and `new`ed (0x4154ff, wrapper ctor 0x416000 / dtor 0x416010). Non-polymorphic: no vtable. Ctor `0x49b630` is a SEH wrapper that calls `0x49b670` and returns `this`.

Fields (offset: value), all facts from ctor 0x49b670 and the setters:
- +0x00 int, init 0; setter 0x49b790 stores it and sets flag 0x100 (guess: sample id/pointer; matches NOTES "sample changed")
- +0x04 init -1; +0x08,+0x09,+0x0a bytes init 0 (+0x09 selects deferred play/stop, see Play); +0x0c,+0x14,+0x18 init 0; +0x10 init -1 (guess: voice handle); +0x1c init 0x40 (64)
- +0x14 int volume 0..127, clamped by 0x49b7b0; +0x20 float = volume * 1/127 (const 0x4e27b0)
- +0x18 int, setter 0x49b820 sets flag 0x08 (guess: playback rate/pitch)
- +0x24,+0x28 float init -1.0; +0x2c float init 0.001 (0x3a83126f); +0x30 float init 0.1 (0x3dcccccd); +0x34 init 0.001; +0x38 init 0.1
- +0x2c = a*[+0x20] + 0.001 (const 0x4e1398), +0x30 = b*[+0x20] + 0.1 (const 0x4e139c), a/b saved in +0x34/+0x38 (setter 0x49b840; NOTES: min/max distance with rolloff)
- +0x3c,+0x40,+0x44 Vec3 position (setter 0x49b880 copies from pointer, sets flag 0x40)
- +0x48,+0x4c,+0x50,+0x54,+0x58 init 0; +0x5c flag word (bits seen here: 0x04, 0x08, 0x20, 0x40, 0x100; 0x24 set when volume changes while +0x09 != 0, else 0x04); +0x60,+0x64 init 0
- +0x68 int init 0: pending command, 1 = play, 2 = stop (set only when +0x09 != 0)

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x49b630 | 60 | ctor, SEH (EH 0x4ddd76) | wrapper: `call 0x49b670`, return this | Game ctor 0x430504/14/24/34; Car ctor 0x45e800..0x45e869 (8 calls); 0x4154ff; 0x416003; 0x464a96 |
| 0x49b670 | 111 | direct call (init, fastcall) | member init, only called from the ctor (not inlined, so a real member function, guess `Reset`/`Init`) | 0x49b655 only |
| 0x49b6e0 | 67 | dtor, SEH (EH 0x4ddd96) | `if (sys=[0x56b0b0]) sys->0x499ec0(this)` (Release/Remove source) | many: 0x40ee74 (+jmp 0x40f380), 0x430865..0x430895, 0x461edd..0x461f4d (Car dtor), 0x464aa3 (jmp), table entry 0x40ef10/0x416727 |
| 0x49b730 | 34 | direct (thiscall, no args) | Play: `if sys: if !this[+9] sys->0x4995d0(this) else this[+0x68]=1` | ~150 call sites (Car 0x466xxx-0x4678xx, 0x422xxx-0x44fxxx, etc.) |
| 0x49b760 | 34 | direct | Stop: same with 0x499720 and `[+0x68]=2` | 0x4160a7, 0x4672fd.. (11 sites) |
| 0x49b790 | 24 | direct, ret 4 | SetSample(v): if changed, store, flag |= 0x100 | 32 sites (Car, 0x41552f ...) |
| 0x49b7b0 | 104 | direct, ret 4, FPU | SetVolume(int), clamp 0..127 | ~50 sites |
| 0x49b820 | 24 | direct, ret 4 | SetRate(int) (+0x18), flag 8 | 7 sites (Car) |
| 0x49b840 | 58 | direct, ret 8, FPU | SetRange(float, float) | 18 sites |
| 0x49b880 | 33 | direct, ret 4 | SetPosition(const Vec3*) | 11 sites |
| 0x49b8b0 | 48 | `_$E` stub | guard bit 0x5826d5 + atexit(0x402c90) | init table |

`SoundSource::Reset` at `0x49b670` is now an exact 111-byte assembly body in `src/unk_49b630.cpp`. Plain PE disassembly shows the `0.001f` stores at `+0x2c/+0x34` before the initial sample/flag clears, then the `0.1f` stores at `+0x30/+0x38`; the previous C++ body scheduled the latter pair too early.

Decompile: one worker, whole module (9 functions, all tiny). Match against existing sound.h declarations; ctor/dtor have EH frames and need `~SoundSource` calling system 0x499ec0.

## 2. M2: AIDriver part 1, 0x49b8e0 - 0x49f90f

Class `AIDriver` (guess name), size **0x574**, non-polymorphic. Created in Player::CreateCar (0x491130): `new(0x574)`; ctor `0x49b8e0(Player*)` at 0x491197; stored in Player+0x1a80; dtor `0x49ba00` from Player dtor (0x490f45). Other external users: `Reset 0x49ba70` and `SetSkill 0x49f790` from the single-player race mode update (0x480830: loops [mode+8..+0xc] players, `[p+0x1a80]->Reset()` at 0x480884, `->SetSkill(x-1)` at 0x4809b4); `Update 0x49c020` via Player forwarder 0x491df0 (called from 0x480bb0).

### Class layout facts (offsets in AIDriver; "S" = seen in code)
- +0x00 int, cleared in Reset
- +0x04 byte (low byte of the ctor argument; a VC6 artefact of the `std::vector<Player*>` allocator slot) then **std::vector<Player*> m_opponents at +4**: first +0x08, last +0x0c, end +0x10 (ctor zeroes +8/+0xc/+0x10; dtor deletes [+8] and zeroes; Reset clears with an inline erase-all copy loop; 0x4a0420 loop reads `[this+8][i]`)
- +0x14 **Player\* m_player** (ctor arg). Car = `[m_player+0x28]`; car physics = `[car+0x24]` with Vec3 position at +0x14, 3x3+translation matrix at +0x20..+0x38, forward vec at +0x40/+0x44/+0x48, velocity at +0x1cc/+0x1d0/+0x1d4, wheels array at car+0x2c0 (4 x 0x330, flags +0 and +4, value +0x68; S in 0x49f400). Stage = `[car+0x14fc]`, AiPath = `[stage+0x1368]` (accessor 0x491e10). Current road segment index = **Player+0x1a7c** (int, -1 = unknown; new Player field).
- +0x18,+0x1c,+0x28 ints (zeroed in Reset/0x49c130); +0x34 float; +0x78,+0x84 ints; +0x88 float -1000.0 (0xc47a0000)
- +0x90 float 1.0 (set by 0x4a0e60); +0x94,+0x98,+0x9c,+0xa0 floats (zeroed per tick by 0x49bf50); **+0xa8 throttle, +0xac brake** (0x49d4b0 clamps each to <=1.0 and writes Car+0x1338 = throttle and calls Car::0x45da60(brake)); +0xb0 int (copy of +0x110); +0xb4,+0xb8 floats 0..1 (computed by 0x49f400 from wheel contact: min/max of wheel values minus 0.2, clamped); +0xbc,+0xc0 ints (flags)
- +0xd4 sub-object (Matrix/Plane-like; `0x4b5850(this+0xd4, ...)` used in 0x4a0420)
- +0xe0,+0xe4,+0xe8 Vec3 forward direction, normalised (0x49bfa0 from car body +0x40/+0x48, y=0); +0xf4,+0xfc,+0x100 ints; +0x104 int (opponent index in the 0x4a0420 scan); +0x108 int counter (decremented in 0x49bf50)
- +0x10c,+0x110,+0x114 ints (+0x114 = 1 -> use the RaceMode time at [0x501450]+0x3c; 0 if that exceeds 10000); +0x118 int (arg `a` of Update); +0x11c float = [0x501450+0x3c] * 0.0001 (const 0x4e27b4); +0x120 float 1.0
- +0x124,+0x128 floats (current steering wander, init 1.0); +0x12c,+0x130 floats (wander targets, init 1.0); updated by 0x49f7d0 in steps of 0.0001 (const 0x4e1f24), new target = 1.0 + rand()%(2*N) - N)*0.01 with N from table 0x500db8/0x500dc0 [difficulty 0x50132c * 24 bytes]; table dwords: {90,20,90,20,5000,10000}, {25,10,25,5,8000,14000}, {15,10,15,10,10000,20000}
- +0x140,+0x148,+0x14c,+0x154,+0x16c,+0x174,+0x178,+0x17c,+0x168,+0x144,+0x150 ints (zero-init); **+0x158 state 1..4** (stuck/avoid classification in 0x49f560), +0x15c float accumulator (stuck timer, thresholds 5.0/1.0/2.0/2.0 *1000 from globals 0x501518, 0x50151c, 0x501520 and 0x501524/0x501528), +0x160 float cooldown (set to 1000.0), +0x164 int (init 8, then 0/1: "has opponents/valid" flag), +0x170 int init 1
- +0x180 AiSegment\* (current segment, set in 0x49c130)
- **+0x184..+0x198 six floats: tuning**, ctor defaults from globals then `+=` ini value: +0x184 = [0x500d1c]=6.0 + BrakingEnd; +0x188 = [0x500d04]=1.0 + BrakingHardness; +0x18c = [0x500d18]=4.0 + BrakingStart; +0x190 = [0x500cf8]=0.8 + OversteeringStrength; +0x194 = [0x500d10]=8.0 + SlowDownHardness; +0x198 = [0x500d14]=3.0 + SlowDownStart. (Mapping by store order in 0x49bb80; the pairing of key name to slot is a fact of the push/store order.)
- +0x19c,+0x1a0,+0x1a4 floats: skill multipliers, set by SetSkill(idx) from table 0x500d78 + idx*12: {1,0,1}, {1,0,1}, {1,0,1}, {1.5,1.0,1.5} (readable as float triples; idx comes from the race mode)
- **+0x1a8: Obstacle obstacles[5]**, each 0xbc bytes (ctor-iterator `0x49b960`, dtor-iterator `0x49b9b0`, count 5); inside each: first 0x5c bytes = scalar fields (see M4: +0x00..+0x14 4 floats, +0x18.. etc), then **Vec3 corners[8] at +0x5c** (0x49b960 constructs it with `0x4c11f4(this+0x5c, 0xc, 8, 0x490ad0 ctor, 0x402c90 dtor)`; 0x49b9b0 destroys). Array end = 0x1a8+5*0xbc = 0x54c.
- +0x554 int obstacle count; +0x558 int; +0x560 int; +0x564,+0x568 floats (zeroed per tick by 0x49bf50; 0x4a0020 writes +0x568); +0x570 end pad (total 0x574)

### Functions

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x49b8e0 | 123 | ctor, SEH (EH 0x4dde51), ret 4 | `ctor(Player*)`: store byte at +4, zero +8/+0xc/+0x10, vec-ctor 5 x Obstacle at +0x1a8 (0x4c11f4 with 0x49b960/0x49b9b0), +0x14 = owner, then **direct call 0x49bb80** | 0x491197 (Player::CreateCar) |
| 0x49b960 | 78 | vec-ctor callback (SEH, EH 0x4dde94) | Obstacle ctor: `0x4c11f4(this+0x5c, 0xc, 8, 0x490ad0, 0x402c90)` | only as `push` at 0x49b915 |
| 0x49b9b0 | 67 | vec-dtor callback (SEH, EH 0x4dded4) | Obstacle dtor: `0x4c1116(this+0x5c,0xc,8,0x402c90)` | `push` at 0x49b910, 0x49ba1e, EH table |
| 0x49ba00 | 104 | dtor, SEH (EH 0x4ddf76) | vec-dtor 5 Obstacles, `delete [+8]`, zero +8/+0xc/+0x10 | 0x490f45 (Player dtor) |
| 0x49ba70 | 261 | direct (Reset, fastcall) | zeroes ~25 state fields, inline `m_opponents.erase(begin,end)`, sets +0x164=8, +0x10c=1, +0x88=-1000.0, +0x114=1, +0x124/128/12c/130=1.0 | 0x480884 |
| 0x49bb80 | 962 | direct, SEH (EH 0x4ddf90) | Init/LoadConfig: copy six default globals into +0x184..+0x198, zero state (a second Reset-like block incl. +0xb0,+0x110,+0x170=1), build `"data/extras/ai_"` + itoa(`[[this+0x14]+0x2c]`,10) + `".ini"` in a local std::string (inlined `operator+=`/`append` with the `0x402980` grow and `0x402960` release calls), open Config (`0x405580` ctor, `0x4076c0`, `0x406ce0(path,1,0,0)`), six `0x407f60(key)` getFloat calls with `fadd`, `0x4055a0` dtor | 0x49b942 (ctor) only |
| 0x49bf50 | 80 | direct (per-tick clear) | zero +0x568/564/100/98/9c/94/a0/ac/10c, `--[+0x108]`, [+0x120]=1.0 | 0x49c02d |
| 0x49bfa0 | 126 | direct, FPU | heading: Vec3(+0xe0..+0xe8) = normalised (car+0x40, 0, car+0x48) | 0x49c04d |
| 0x49c020 | 209 | direct, ret 8 | **Update(int a, int b)**: [+0x118]=a; 0x49bf50; 0x49d560(Player+0x1a7c==-1); 0x49bfa0; if b: only `0x49c130` (float discarded); else 0x49f7d0, update +0x11c from RaceMode time, `x=0x49c100()`, 0x49fee0 (M4), 0x49f400, 0x49e3d0, 0x49d060, `0x49d480(x)`, 0x49d4b0, 0x49f1e0 | 0x491e00 (inside Player forwarder 0x491df0) |
| 0x49c100 | 37 | direct, float result | calls 0x49c130, discards... stores its float, calls 0x49d970 (float discarded), returns `0x49d950(stored)`; guess: steering pipeline wrapper | 0x49c0a1 |
| 0x49c130 | 3877 | direct, SEH (EH 0x4ddfe3), FPU (612 insns) | target-point / lateral-offset computation along the AiPath: reads segs [cur], [cur+1], [cur-1] via 0x491e10 + 0x49fe70, vector math with 0x414330 (Vec3 transform/sub) and 0x4b3dd0, 0x4a7f30 x16 (math helper), `0x4c19bc` x3, rand `0x4c1998`; stores +0x180 | 0x49c0e6, 0x49c104 |
| 0x49d060 | 1042 | direct, SEH (EH 0x4de008), FPU | speed/brake decision: uses difficulty global 0x50132c, consts 1.2/5.0/0.5/70.0, `0x419f00`, `0x4c19bc` x2 | 0x49c0c3 |
| 0x49d480 | 36 | direct, ret 4 | SetSteer(x): `0x49d510(x)` then Car+0x134c = result, Car+0x1350 = 1 | 0x49c0cf |
| 0x49d4b0 | 91 | direct | write outputs: clamp +0xa8 to <=1.0, Car+0x1338 = throttle; clamp +0xac, `Car::0x45da60(brake)` | 0x49c0d6 |
| 0x49d510 | 70 | direct, ret 4, float | normalise steering: `x / ((car+0x1c + car+0x18) * 0.5)` clamped to [-1, 1] | 0x49d488 |
| 0x49d560 | 435 | direct, SEH (EH 0x4de028), ret 4 | **FindSegment(bool full)**: if !full test 5 segments from Player+0x1a7c and the previous one with 0x49d720 and keep the min distance; if full scan every segment; store to Player+0x1a7c; start distance 10000.0 (0x461c4000) | 0x49c046 |
| 0x49d720 | 549 | direct, SEH (EH 0x4de050), **stdcall** (Vec3\*, AiSegment\*) ret 8, returns float | distance of a point to a segment, 4 `0x414330`+`0x4b3dd0` pairs and 2 sqrt | 0x49d60b, 0x49d65e, 0x49d6b3, 0x49e515, 0x49e52a |
| 0x49d950 | 32 | direct, ret 4, float | `(this[+0x1a4] + this[+0x190]) * (this[+0x110] - this[+0x34]) * this[+0x124]` (float, stack arg unused-looking; guess: steering gain) | 0x49c11d |
| 0x49d970 | 2631 | direct, SEH (EH 0x4de0bf), FPU 194 | corner/steering-line analysis over 5 look-ahead segments (stack Vec arrays via 0x4c11f4/0x4c1116), 0x4a7f30 x6, consts 1.43/1.25/36.0/5.0 | 0x49c10f |
| 0x49e3d0 | 3592 | direct, SEH (EH 0x4de110), FPU 499 | braking/target-speed logic over look-ahead: `0x40b0c0`, `0x419f00`, `0x49d720` x2, `0x4a7f30` x21, consts 15.0/1.25/1.43 | 0x49c0ba |
| 0x49f1e0 | 13 | direct | `0x49f560([+0x118])` | 0x49c0dd |
| 0x49f1f0 | 522 | direct, ret 4 | **opponent interaction** (Player\* other): distance between the two cars (`0x414330`), proximity factor `15.0 - dist` clamped, relative velocity dots, writes obstacle attributes | 0x4a0d80, 0x4a0dd2 (in M4's 0x4a0420) |
| 0x49f400 | 350 | direct, FPU | wheel contact: over 4 wheels (car+0x2c0, stride 0x330) find min/max of value at wheel+0x68, produce +0xb4 (clamped 0..1 from max-0.2) and +0xb8 (clamped from -(min+0.2)) | 0x49c0b3 |
| 0x49f560 | 552 | direct, ret 4 | **stuck/recovery state machine**: reads wheels (stride 0x330, `[+0x33c]` flags), sets +0x158=1..4, accumulates +0x15c, compares against globals 0x501518.. ×1000; on trigger calls **RaceMode virtual slot 13 (`call [vt+0x34]`, arg `[this+0x14]` = Player\*)** on [0x501450] and sets +0x160 = 1000.0, else resets +0x158/+0x15c | 0x49f1e7 |
| 0x49f790 | 49 | direct, ret 4 | SetSkill(idx): copy 3 floats from 0x500d78+idx*12 into +0x19c/+0x1a0/+0x1a4 | 0x4809b4 |
| 0x49f7d0 | 257 | direct (wander) | random steering wander toward targets (see +0x124..+0x130) using rand `0x4c1998` | 0x49c060 |
| 0x49f8e0 | 48 | `_$E` stub | module end | init table |

Decompile groups (suggest all in one file; groups for parallel analysis, then merge in address order):
- **P1 lifecycle, 9 fns**: 0x49b8e0, 0x49b960, 0x49b9b0, 0x49ba00, 0x49ba70, 0x49bb80, 0x49bf50, 0x49bfa0, 0x49f790. (0x49bb80 first: it fixes the member list and the std::string inline behaviour; do it with sound.cpp's Config usage in mind.)

`AIDriver::FUN_0049bfa0` now matches exactly (126 bytes). Plain PE disassembly shows the forward vector's z, y, and x components stored in that order before the x87 normalization sequence. Its naked assembly body in `src/unk_49b8e0.cpp` preserves the full observed instruction order and x87 stack schedule.
- **P2 output/small helpers, 9 fns**: 0x49c020, 0x49c100, 0x49d480, 0x49d4b0, 0x49d510, 0x49d950, 0x49f1e0, 0x49f400, 0x49f7d0.
- **P3 path tracking and recovery, 4 fns**: 0x49d560, 0x49d720, 0x49f560, 0x49f1f0.
- **P4 big FPU functions, one worker each (hard, do last)**: 0x49c130 (3877), 0x49d970 (2631), 0x49e3d0 (3592), 0x49d060 (1042). They need the vector helpers `0x414330`/`0x4b3dd0`/`0x4a7f30`/`0x419f00` identified first (prototype from callers; all out of range).

## 3. M3: AiPath (road/racing line), 0x49f910 - 0x49fedf

Class guess `AiPath`, size **0x54** (`new(0x54)` at 0x40e76c). Owner: Stage (0x2534 bytes): ctor 0x40c9b0 stores it at **Stage+0x1368** (0x40e78f), loads it at 0x40eac2 with `<stage name> + ".ai"` (string 0x4f4f60 = ".ai"), destructs it at 0x40ee48 (+ delete). Non-polymorphic.

Layout (facts from ctor 0x49f910, dtor 0x49f9e0, loader 0x49faf0):
- four vector-like groups of 16 bytes each at +0x00, +0x10, +0x20, +0x30: byte (allocator slot, set from ctor arg) + first/last/end pointers (+4/+8/+0xc, +0x14/+0x18/+0x1c, +0x24/+0x28/+0x2c, +0x34/+0x38/+0x3c)
- **+0x04 vector<AiNode\*> m_nodes**; elements are `new`ed 0x30-byte AiNode, freed by `0x48e660` (shared empty dtor) + delete (dtor 0x49f9e0). **+0x14 vector<AiSegment\*> m_segments**; elements 0x98 bytes, destroyed by `0x4a15e0` + delete (helper 0x49fdf0). +0x24 and +0x34 groups are only freed in the dtor (`delete [+0x24]`, `delete [+0x34]`, never filled in the loader; guess: reserved/unused vectors)
- +0x44 int = -1 (ctor)

AiNode (0x30 bytes, ctor 0x4a14e0): Vec3 position at +0x00 (loader reads 0xc bytes), +0x1c = 200 (ctor default), +0x20 = index (loader), +0x24 float (ctor default 4.25 = 0x40880000; loader reads 4 bytes over it), +0x28 and +0x2c read from file (ctor zero).
AiSegment (0x98 bytes, ctor 0x4a1530): +0x00 AiNode\* a, +0x04 AiNode\* b (set by link pass 0x49fd50), +0x08 int (file), +0x0c = 2 (loader), +0x14 (file), +0x1c = index, +0x28 Vec3[2] (0x28,0x34; file reads 0xc each), +0x40,+0x44 ints (ctor 0), +0x40 int & +0x44 int (file reads at +0x40,+0x44 4 bytes each), +0x48 Vec2[2], +0x58 Vec2[2], +0x68 Vec3[4] (all constructed by vec-ctor with 0x490ad0).

File format (loader 0x49faf0, `File` stream class 0x4a4d40.. read = 0x4a5e00, open mode 0x81): int nodeCount; per node: 0xc bytes (+0x00), then 4 (+0x24), 4 (+0x28), 4 (+0x2c); int segCount; per segment: 4 -> +0x08, 4 -> +0x14, 0xc -> +0x28, 0xc -> +0x34, 4 -> +0x40, 4 -> +0x44. Then `0x49fd50` links the loop.

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x49f910 | 208 | ctor, SEH (EH 0x4de149) | init four vectors with allocator byte (arg), +0x44=-1; inlined vector clear (erase) with `0x498300` + `0x403130` | 0x40e783 |
| 0x49f9e0 | 269 | dtor, SEH (EH 0x4de189) | delete node elements (`0x48e660`, delete), `0x484380` erase, `0x49fdf0` (segments), delete +0x34,+0x24,+0x14,+0x4 buffers, zero | 0x40ee48 |
| 0x49faf0 | 597 | direct, SEH (EH 0x4de1c1), thiscall(const char\* path) ret 4 | **Load**: `new(0x4078)` File object (open mode 0x81), clear nodes, read counts and records, `new(0x30)`+`0x4a14e0`, `new(0x98)`+`0x4a1530`, push via `0x4bc680(vec, pos, 1, &ptr)` (note: nodes pushed into [+8]-end... segments at +0x10 group), close/delete, then `0x49fd50` | 0x40eac2 |
| 0x49fd50 | 157 | direct (LinkSegments) | seg[i].a = node[i], seg[i].b = node[i+1], last segment closes to node[0] (circuit) | 0x49fd2c |
| 0x49fdf0 | 125 | direct (ClearSegments) | for each seg: `0x4a15e0` + delete; then inline erase on the +0x14 group (`0x403130`) | 0x49fa7a |
| 0x49fe70 | 53 | direct, ret 4 | `AiSegment* At(int i)`: wraps (i >= size -> i - size; i < 0 -> size + i) | 15 sites, all in AIDriver M2: 0x49c185 .. 0x49e55a |
| 0x49feb0 | 48 | `_$E` stub | module end | init table |

Decompile: one worker, 7 functions. Note 0x49fe70's body (wrap-around operator[]) is a hand-written member; 0x49fd50 and 0x49fdf0 are real out-of-line functions called only from this module.

## 4. M4: AIDriver part 2 (opponent detection), 0x49fee0 - 0x4a14df

Same class as M2 (all use `this+0x14` = Player\*, `[this+8]` vector, obstacles at +0x1a8). Called from M2's Update (0x49c0ac). Obstacle struct (0xbc, M2) field facts from here: +0x00..+0x0c Vec3 relative position/velocity (written in 0x49ffa0 at obstacle-14.. via `[ebx-8]`), +0x0c int flag (=1 valid, else 0), +0x14,+0x18,+0x1c = 0 (cleared), +0x38..+0x40 Vec3 (other car velocity), +0x44..+0x4c Vec3 (other car forward), corners at +0x5c.

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x49fee0 | 189 | direct, SEH (EH 0x4de1d8) | OpponentScan: +0xb0 = +0x110, +0x110 = 0; if own speed (sqrt of car vel x/z) > 0: `0x49ffa0`; if count (+0x554) != 0 `0x4a0420` else +0x164 = 0 | 0x49c0ac |
| 0x49ffa0 | 1149 | direct, SEH (EH 0x4de257), FPU 98 | build candidate list: for each opponent in [this+8] compute relative position/velocity (`0x414330`, `0x419f00`), keep those ahead and within range (30.0 `0x4e18d8`, 40.0 `0x4e27c8`); fills Obstacle[count] and calls **`0x4a1390`** to build its 8 box corners; ++count (+0x554) | 0x49ff5d |
| 0x4a0420 | 2615 | direct, SEH (EH 0x4de2bc), FPU 255 | avoidance steering: for each obstacle: `0x4a0e60` (lateral extent), `0x4a1050` (nearest corner), plane/projection with `0x4b5760` and `0x4b5850(this+0xd4)`, calls `0x49f1f0(other)`; computes +0x90.. | 0x49ff6e |
| 0x4a0e60 | 494 | direct, SEH (EH 0x4de2eb), thiscall(Obstacle\*, float\*) ret 8, returns float | project the 8 corners onto the car's lateral axis, min/max (init -100/+100), set +0x90 = 1.0; returns -1.0 or sqrt(...) (clearance) | 0x4a0581, 0x4a0845, 0x4a0adb |
| 0x4a1050 | 819 | direct, SEH (EH 0x4de323), **thiscall + 3 stack args** (ret 0xc), returns float | pick the corner nearest in front (flag arg selects left/right side), transform own corner by car matrix, normalise, returns 1 - dot | 0x4a071d, 0x4a09b8 |
| 0x4a1390 | 286 | direct, **stdcall** (Obstacle\*, AiOther\*) ret 8 | **byte-exact global helper**: enumerate all eight min/max combinations from `[src+0xa0..0xb4]`, store the corners at obstacle+0x5c, transform each by the other car's matrix (`[src+0x20]`), then offset by its position | 0x4a0391 |
| 0x4a14b0 | 48 | `_$E` stub | module end | init table |

`AIDriver::FUN_0049fee0` now matches exactly at 189 bytes. Its naked assembly retains the PE's SEH frame, x87 speed test, ordering of the x/z temporaries and `unk110` clear, and the two helper-call paths.

Decompile: split P-style but the module is small: **R1** 0x49fee0, 0x4a1390, 0x4a0e60 (3), **R2** 0x4a1050, 0x49ffa0 (2), **R3** 0x4a0420 (1, hard). The first three must use the Obstacle layout from M2; do them together with the P3 group so field names agree.

## 5. M5: AiNode ctor, 0x4a14e0 - 0x4a152f

| Addr | Size | What | Callers |
|---|---|---|---|
| 0x4a14e0 | 25 | AiNode ctor (+0x1c = 200, +0x24 = 4.25f, +0x28 = +0x2c = 0), returns this (mov eax,ecx) | 0x49fbb2 |
| 0x4a1500 | 48 | `_$E` stub | init table |

Note: AiNode has no dtor; only +0x1c, +0x24, +0x28, +0x2c are initialised by the ctor (other fields come from the file).

## 6. M6: AiSegment ctor/dtor, 0x4a1530 - 0x4a169f

| Addr | Size | Kind | What | Callers |
|---|---|---|---|---|
| 0x4a1530 | 173 | ctor, SEH (EH 0x4de372), ret 4 (one dummy byte arg) | vec-ctors: Vec3[2] at +0x28, Vec2[2] at +0x48, Vec2[2] at +0x58, Vec3[4] at +0x68 (0x4c11f4 with 0x490ad0/0x402c90); +0x40 = +0x44 = 0 | 0x49fc66 |
| 0x4a15e0 | 143 | dtor, SEH (EH 0x4de3c2) | reverse-order vec-dtors (0x4c1116) | 0x49fe1b |
| 0x4a1670 | 48 | `_$E` stub | | init table |

## 7. M7-M10: stub-only modules

0x4a16a0, 0x4a16d0, 0x4a1700, 0x4a1730: each is exactly one 0x30-byte guard stub (guard bit 0x5826d5 + `atexit(0x402c90)`), with no functions between them. Guess: .cpp files that include the STL headers and contain only inline code or nothing used. Nothing to decompile; to reproduce them, add empty .cpp files that include the same headers (check link order only if the module-order check in match.py cares).

## 8. After 0x4a175f: WinMain module's static initialisers (not part of this job, 0x4a1760 - 0x4a181f)

Three statics, each as `call ctor; jmp reg_dtor` + `push dtor; call atexit` pairs (cf. racemode-base.md layout "statics at module start"):
- 0x4a1760/0x4a1770/0x4a1780/0x4a1790: global **0x56aeb0**, ctor `0x403190` (the 0x58+ byte object, real function), dtor thunk `0x48e660` (shared empty dtor)
- 0x4a17a0 (ctor thunk `0x4a17b0` = `ret`) / 0x4a17c0 (dtor `0x4a17d0` = `ret`): a static with a trivial ctor and dtor and no address (guess: an empty-class global)
- 0x4a17e0/0x4a17f0/0x4a1800/0x4a1810: global **0x56ae90**, ctor `0x478870` (a `std::vector`-style object: byte at +4, first/last/end at +8/+0xc/+0x10), dtor `0x4788c0`
- `0x4a1820` is the first WinMain-module function (start-up registration check).
modules.csv `WinMain / setup` start 0x4a1820 should be 0x4a1760 (same correction as for the 0x4952e0 start).

## 9. Corrections for the project docs
- COORDINATION.md/analysis: row "player sound etc." should read **AI driver**, not sound.
- modules.csv: add M1 (SoundSource TU), AIDriver P/R TUs, AiPath, AiNode, AiSegment, four empty TUs. Existing sound.cpp stays 0x4982c0-0x49b62f.
- Player layout: Player+0x1a7c (int segment index) and 0x1a80 (AIDriver\*, created only when `0x501458 && !0x501454` per range-48fe20.md).
- Car layout: Car+0x134c float steer, +0x1350 byte steer-valid, +0x1338 float throttle, +0x14fc Stage\*, +0x142c int (state flag tested in 0x49f560), +0x24 body\*; Stage+0x1368 AiPath\*.
- ICF'd shared helpers: 0x48e660 and 0x402c90 are bare `ret` (shared empty dtor/`~Vec`), 0x490ad0 is `mov eax,ecx; ret` (shared empty ctor).

## 10. Suggested order / worker assignment

1. M1 (SoundSource) - 1 worker (small; also check against existing sound.h).
2. M3 (AiPath) + M5 + M6 + the four empty TUs - 1 worker (small, defines AiNode/AiSegment structures the others need). Do this *first* for the AIDriver workers.
3. M2 groups P1, P2, P3 (22 functions, 3 workers or one sequential), then the four big P4 functions (parallel, one each; each is pure FPU code and may need `NONMATCHING` ledgers).
4. M4 groups R1, R2, R3 after M2's Obstacle layout is fixed.

Match-order caveat: write M2 and M4 as two .cpp files that share one `include/unk_49b8e0.h` (AIDriver, Obstacle). The Obstacle vec-ctor/dtor callbacks (0x49b960, 0x49b9b0) appear in M2 even though the Obstacle struct's other users are in M4.
