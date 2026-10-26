<!-- Subagent analysis, 2026-10-22; read-only disassembly; guesses are marked. -->
# Range 0x48fe20 - 0x49540f: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py/refs.py output plus .rdata reads. "Guess" marks inference.

## 0. Module boundaries (important correction)

Every module ends with a guard-byte stub (`mov cl,[0x5826d5]; or ...; push 0x402c90; call 0x4c0d01` = atexit(0x402c90)), 0x30 bytes before the next module's first function. The stubs are CRT init-table entries (file offsets 0xefcfc.. in order). Statics of a module are emitted at its start and its guard stub at its end (same layout as racemode-base.md: statics 0x4856e0-0x485a2b, guard 0x48f100).

Guard stubs in the range: 0x48fdf0 (end of the previous module, 0x48f130-0x48fe1f), **0x490920, 0x491f80, 0x4952b0**, then init entries 0x4952e0, 0x495350, 0x4953b0 and later 0x498330.

So the range holds THREE modules, then the start of the controller module:

| Module | Range | Guard stub (end) | Content |
|---|---|---|---|
| A | 0x48fe20 - 0x49094f | 0x490920 | RecordTable (highscores) |
| B | 0x490950 - 0x491faf | 0x491f80 | Player |
| C | 0x491fb0 - 0x4952df | 0x4952b0 | in-race game loop / session class (embedded in Game) |
| D (not this job) | 0x4952e0 - 0x49833x | 0x498330 | controller module; it starts at **0x4952e0**, not 0x495410. 0x4952e0-0x4953ef are D's static initialisers (see section 4). modules.csv start 0x495410 should become 0x4952e0 |

None of A/B/C defines a vtable or an STL instance. No class here stores a vtable in a ctor (all non-polymorphic), no static/global objects (only the guard stubs).

Shared helpers living in this range: 0x490ad0 (3 bytes, `mov eax,ecx; ret`) is a trivial ctor ICF-folded across the binary (about 270 refs: vec-ctor callbacks `push 0x490ad0`, jmps). It is defined in B (Player's 0x5c-byte sample struct ctor) but is "Vec2()/empty ctor" for everybody. 0x402c90 is the matching empty dtor/log function. Do not reimplement per class; treat as the one shared empty ctor.

## 1. Module A: RecordTable, 0x48fe20 - 0x49094f (10 fns incl. stub)

Class RecordTable. Embedded in `Game` (global ptr 0x56b0c8, class `Game` 0x139894 bytes) at **Game+0x5c30**: Game ctor 0x4303e0 does `lea ecx,[esi+0x5c30]; call 0x48fe20` at 0x4304c5 (also 0x4306db). Next embedded member is the module-C object at Game+0x139080. Size = 0x133441 (flag byte at +0x133440, gap to next member is alignment). No vtable.

Record (0x60 bytes, 96), from ctor 0x48fe40 and the insert functions (offsets: facts; names: guesses):
- +0x00 char name[0x1e] (default string at 0x5070b0, 0x18 dwords zeroed first, copy by strcpy)
- +0x1e byte (translated through byte table 0x4ff354+idx*4 in the loader, class/car group? guess)
- +0x1f byte (index/rank-ish; ctor stores `bl` = (k==0)?1:k, signed in insert: `movsx [rec+0x1f]`)
- +0x20..0x2f 16 bytes: filled by `GetLocalTime` (IAT 0x4e10b0 called at 0x48fe50) = SYSTEMTIME copy (as 4 dwords), date stamp
- +0x30..0x57: 10 dwords default 0x8ca0*n (36000..360000)
- +0x54 total/main time (compare key of 0x490050, default 360000, same slot as 10th dword)
- +0x58 second time (compare key of 0x4901b0/0x490310; default 360000 if group<3 else 3,600,000 (0x36ee80))
- +0x5c int (default 0)

Table indexing (facts): record address = this + ((((X*23 + a4)*10 + a3)*0x60), X = (g ? g*11+2 : 0) + h (+3 in 0x48ffd0/0x490050/0x4901b0, +0 in 0x490010/0x490310). Ctor loops g in 0..4, h in 0..(g==0 ? 12 : 10), k in 0..22, e in 0..9, i.e. 10-entry ranking lists. "Insert" shifts entries down 0x60 bytes (rep movs 0x18 dwords) when new value < existing; sets dirty byte [+0x133440]=1; returns 1 if it became rank 0.

File format (0x490470 load, 0x490820 save): magic dword; current = [0x4ff350]=0x20020122. Older versions: 0x4ff34c=0x20020111 (load sets dirty only, i.e. discard), 0x4ff348=0x20011129 (record loop, 0x60 copies with byte remap via 0x4ff354 table), 0x4ff344=0x20010823 (oldest: 0x68-byte records converted: 0x68 read, 0x28-byte pieces). Current format: whole 0x133440 block read/written after a stream cipher call (0x401180 decrypt / 0x4010f0 encrypt, key 0x4ff3d4, 0x4cd10 dwords). Uses file-stream class 0x4a4d40 (ctor), 0x4a4f50 (open: mode 0x81 read / 0x82 write), 0x4a5e00 (read), 0x4a5e90 (write), 0x4a5d00 (close), 0x4a4d90 (dtor); 0x4a6050 = file-exists (data archive aware). Both functions have SEH frames and big stack buffers (0x4170/0x407c, chkstk 0x4c1290). Save is skipped unless dirty.

| Addr | Size | Kind | What | External callers |
|---|---|---|---|---|
| 0x48fe20 | 19 | ctor (fastcall) | clears dirty byte, calls 0x48fe40 | 0x4304c5, 0x4306db |
| 0x48fe40 | 386 | direct (init) | fill all records with defaults (GetLocalTime) | 0x48fe2a only |
| 0x48ffd0 | 56 | thiscall(4 args, ret 0x10) | `Record* At(g,h,e,k)` with +3 | 0x42e4a7, 0x442eab, 0x4869e4, 0x487e6f, 0x48fb45 |
| 0x490010 | 54 | thiscall(4 args) | same without +3 | 0x42e46d |
| 0x490050 | 341 | thiscall(g, Record*, h) ret 0xc | insert by +0x54, 2 tables (X and X'=g*23+[rec+0x1f]) | 0x486855, 0x48687e, 0x486b65, 0x486b8c |
| 0x4901b0 | 341 | thiscall ret 0xc | insert by +0x58 | 0x4867ed, 0x486812, 0x486973, 0x4869a1 |
| 0x490310 | 339 | thiscall ret 0xc | insert by +0x58, no +3 | 0x486e52, 0x486e74, 0x486f23, 0x486f44 |
| 0x490470 | 929 | thiscall(filename) ret 4 | Load highscores (version dispatch) | 0x430b29, 0x43457e |
| 0x490820 | 250 | thiscall(filename) ret 4 | Save if dirty (e.g. data/highscores/sessionscores.hs, string 0xfccd4; "highscores.hs" is at 0xf4fef) | 0x4309bb, 0x430c8b, 0x4349d6, 0x486bd0, 0x486f6f |
| 0x490920 | ~48 | CRT init | guard stub, module end | init table |

Decompile: one file, ~10 fns, all independent of other modules except file-stream class and cipher. Group A1 = all 10.

## 2. Module B: Player, 0x490950 - 0x491faf (class `Player`, 0x1a88 bytes, no vtable)

Instances are `new`ed (0x1a88) and stored in the race mode's `vector<Player*>` (RaceMode+0x04 vector A). No vtable store anywhere; it has a CRITICAL_SECTION at +0x17c8 (Init IAT 0x4e10c8, Delete 0x4e10cc, Enter 0x4e10d0, Leave 0x4e10d4). Network messages (see 0x491490) arrive on another thread, hence the lock around the sample buffers.

Constructor family (all call common init 0x490fe0; array ctors via EH helper 0x4c11f4 with elements of 0x5c bytes, ctor 0x490ad0, dtor 0x402c90):
- 0x4909b0 ctor(int a, int remote) ret 8: [0x3c]=a; remote ? state=2 and [0x34]=[0x38]=0 : state=0. (callers: 0x403e9b, 0x433566, 0x433dfc, 0x434cdd, 0x44d8cc, 0x44db63, 0x44e0ad, 0x48e051)
- 0x490ae0 ctor(const char* name, a, b, c) ret 0x10: name copied via 0x4b7730(this,name,0x1e); [0x3c]=a,[0x34]=b,[0x38]=c; state=0 (caller 0x482930)
- 0x490b90 ctor(arg) ret 4: state=1, [0x34]=[0x38]=0 (callers 0x403ec1, 0x433e73, 0x44da1e)
- 0x490c30 ctor(int index) ret 8: state=3 (guess: simulated/AI opponent without physical car). Reads `data/settings/cpu%02d.ini` (digits patched into string at 0x4ff5c9/0x4ff5ca) via Config class 0x405580..: keys Name -> [0x00], Nationality -> [0x20], CarModel{Novice,Intermediate,Expert} / CarModelFTC{...} depending on globals 0x501460 and 0x50132c -> [0x2c]; allocates 0xc8 bytes -> [0x1a84] and fills floats: Track1..5Errors at +0..+0x24 (stride 4; 10 floats = Track1..5Errors then Track1..5Fatals), then Track{1..5}Rate{1..8} at +0x28 + t*0x20 + r*4. (callers 0x433604, 0x44dd78, 0x44e17e, 0x44e329)
- state values at [0x40]: 0 local human (guess), 1 (?), 2 remote/network, 3 AI. Race-mode base code skips state 3 for car work.

Layout facts (offset: meaning, S=seen in these fns):
- 0x00 char name[]  (strcpy'd default "Player 1" from 0x4ff908; SetName 0x490fb0)
- 0x20 nationality (value from getString; int/ptr)
- 0x24 int =1; 0x28 Car* (0x1f30c bytes, ctor 0x45e480, dtor 0x461a40; car back-pointer written at car+0x1becc); 0x2c car model/index
- 0x30 int flag (set 1 when result data received), 0x34/0x38 ints (ctor args), 0x3c int id, 0x40 state, 0x44 int (finish/penalty time; -60000 = 0xffff15a0 = none), 0x48 int flag (set by 0x491110), 0x4c/0x50/0x54 ints (checkpoint-info copies)
- 0x58 int
- **+0x5c SampleBuffer sub-struct** (init 0x490950, fastcall on this+0x5c; implicit dtor 0x490a60, unreferenced - inlined at its users): +0x00 count, +0x04 latest time (init -5000), +0x08 array A[32] of 0x5c-byte samples (first dword = timestamp, init -3000), +0xb88 counters/times (-5000), +0xba4 array B[32] samples, +0x173c/0x1738/0x1724 = -6000, +0x1768 byte. In Player coordinates: A at +0x64, B at +0xc00 (count +0xbfc), copy B->A is 0x2e0 dwords.
- 0xbe4/0xbe8: 5 dwords copied from 0x1780 (best/last packet)
- 0x1740..0x1764 floats written by 0x491c10 (interpolation deltas/targets: pairs at +0x1740/44, 0x1748/4c, 0x1750/54, 0x1758/5c, 0x1760/64)
- 0x1780 5 dwords (packet type 1 payload), 0x17e0/0x17e4 (type 3), 0x17e2-0x17e7 bytes (car appearance, applied through car methods 0x465e90/0x466500/0x466520/0x466590/0x466610), 0x17e8 byte pending flag, 0x17ec (type 4 byte)
- 0x17f0 ring of 8 dwords (frame deltas), 0x1810 ring index, 0x1814 max, 0x1818 min (init 10000), 0x181c avg, 0x1820
- 0x1824 int =1 ("visible/active", returned by RaceMode vtable slot 14 at 0x48e650), 0x1828 =1, 0x182c, 0x1838 byte
- 0x17c8 CRITICAL_SECTION (24 bytes)
- 0x1884 int (count of messages), 0x1888 int[~91] cumulative split times (init i*60000 up to 0x535020 limit), 0x19f0 int[9] deltas, 0x1a14, 0x1a18 current checkpoint/progress counter (used by standings), 0x1a1c count, 0x1a20 float, 0x1a24 int[10] per-stage results (final value 0x1a24[idx], idx = [0x1a50]), 0x1a28 int[10], 0x1a50 stage idx, 0x1a54/0x1a60/0x1a64/0x1a68 save/restore pairs (0x491430/0x491450 swap), 0x1a58, 0x1a5c
- 0x1a80 ptr to 0x574-byte helper object (ctor 0x49b8e0(this), dtor 0x49ba00; created only in 0x491130 when 0x501458 && !0x501454) - the module starting at 0x49b8e0
- 0x1a84 float[50] AI table (0xc8 bytes)

Functions (all direct calls, none virtual):

| Addr | Size | cc / sig | What | External callers |
|---|---|---|---|---|
| 0x490950 | 94 | fastcall (this+0x5c) | init SampleBuffer sentinels | 0x49106d, 0x4912b8 |
| 0x4909b0 | 163 | ctor ret 8 | see above | list above |
| 0x490a60 | 100 | fastcall | SampleBuffer dtor (EH frame), 0 refs | - |
| 0x490ad0 | 3 | shared empty ctor | ICF'd, ~270 refs | many |
| 0x490ae0 | 176 | ctor ret 0x10 | see above | 0x482930 |
| 0x490b90 | 150 | ctor ret 4 | state 1 | 0x403ec1, 0x433e73, 0x44da1e |
| 0x490c30 | 678 | ctor ret 8 | AI player from cpu ini | 0x433604, 0x44dd78, 0x44e17e, 0x44e329 |
| 0x490ee0 | 194 | dtor fastcall | DeleteCS, delete car/0x1a84/0x1a80, vec dtors | 0x434d9a, 0x4815fc, 0x4832d1, 0x485ddd, 0x485e36, 0x48ddf0 |
| 0x490fb0 | 42 | thiscall(const char*) ret 4 | SetName (strcpy) | 0x403eec, 0x43357c, 0x433e16, 0x433eb4, 0x43a7ea, 0x44d8e6, 0x44da38, 0x44db7d, 0x44dd92, 0x44e0ca, 0x44f145, 0x482de0 |
| 0x490fe0 | 303 | fastcall | common init (InitCS, zero, "Player 1", 0x4913e0/0x491410, split table) | only the 4 ctors |
| 0x491110 | 31 | thiscall(a,b) ret 8 | if a: [0x48]=a,[0x44]=b else [0x44]=-60000 (SetFinish/Penalty) | 0x4014a8, 0x4015e3, 0x480cb1, 0x48118e, 0x48180c, 0x481bb9, 0x481dde, 0x481e75, 0x481f21, 0x481f80, 0x48f5d2, 0x49186f, 0x4918a7, 0x494ba4 |
| 0x491130 | 354 | fastcall, SEH | CreateCar: allocs 0x574 helper and Car (0x1f30c), picks params from globals 0x501420-0x50142c and tables 0x4ff5dc/0x4ff5f0, 0x56b0c8+0x1393e4/0x139404 | 0x403f81, 0x43359c, 0x433e3e, 0x43400f, 0x4346e7, 0x43478f, 0x44dacb, 0x44dc55, 0x44e125, 0x48615c, 0x48e075 |
| 0x4912a0 | 165 | fastcall | ResetForRace: lock, clear rings, car reset 0x464800/0x4693a0/0x45da60, unlock | 0x4808fa, 0x4817e2, 0x48f231, 0x48f94f, 0x48f9a4 |
| 0x491350 | 29 | fastcall | clear finish, car 0x4693a0(0) | 0x480a26, 0x4818f3, 0x48f2ad, 0x48fa11 |
| 0x491370 | 110 | fastcall | ResetResults (lock, zero 0x1884/0x19f0..., rebuild split table) | 0x48090a, 0x4817ef, 0x48f960, 0x491400, 0x492b1d, 0x492c81 |
| 0x4913e0 | 39 | fastcall | clear 0x1a28[10], 0x1a50, 0x1a5c, then 0x491370 | 0x4336cd, 0x433a80, 0x434003, 0x434202, 0x434783, 0x4910be |
| 0x491410 | 27 | fastcall | zero 0x1a54/60/64/68 | 0x4910c5 |
| 0x491430 | 25 | fastcall | [1a60]=[1a64]; [1a54]=[1a68] | 0x433a8f, 0x4341f6 |
| 0x491450 | 25 | fastcall | inverse of 0x491430 | 0x4336dc, 0x43402a |
| 0x491470 | 14 | fastcall | Lock (EnterCriticalSection this+0x17c8) | 0x43a7bf, 0x482dc0, 0x482eb6, 0x4912a5, 0x491374, 0x4914cf, 0x491510, 0x4915b6, 0x491633, 0x4916a6 |
| 0x491480 | 14 | fastcall | Unlock | 0x43a7f9, 0x482def, 0x482ecd, 0x4912ec, 0x4913d6, 0x4914fd, 0x4915a3, 0x4915d6, 0x491694, 0x491769 |
| 0x491490 | 366 | thiscall(buf, n) ret 8 | OnPacket: switch on byte [p+1] 0..4 (jump table 0x491600; 5 entries): 0 -> sample (0x491620), returns 0x5e; 1 -> 5 dwords at 0x1780 + [0x1820]=word, returns 0x18; 2 -> checkpoint/split packet (time table, [0x1a24+idx*4]), returns 0xb; 3 -> car appearance 8 bytes ([0x17e0],[0x17e4], flag 0x17e8), returns 8; 4 -> [0x17ec]=byte, returns 3 | 0x482cbd |
| 0x491620 | 127 | thiscall(sample*) ret 4 | PushSample: ring of 32 by timestamp | 0x4914bc |
| 0x4916a0 | 688 | thiscall(now) ret 4 | Apply network snapshot: B->A copy, appearance, picks sample, calls car 0x45b2c0/0x45b900/0x468bd0/0x4658a0/0x468c00/0x465d80 and 0x491950 per substep; stats ring | 0x481b13 |
| 0x491950 | 696 | thiscall(t) ret 4 | Interpolate two samples into car state (via 0x491c10, car 0x45da60) | 0x481c8e, 0x491901 |
| 0x491c10 | 473 | **cdecl** (Player*, sA*, sB*, int copy) | unpack u8 fields with scale consts 1/255 (0x4e2780), 1/127 (0x4e2784) into [0x1740..0x1764] | 0x491a05, 0x491a5d |
| 0x491df0 | 24 | thiscall(a,b) ret 8 | forwards to [0x1a80]->0x49c020(a,b) | 0x480bb0 |
| 0x491e10 | 16 | fastcall | returns [[[this+0x28]+0x14fc]+0x1368] | 0x49c175, 0x49d083, 0x49d594, 0x49d683, 0x49d6a0, 0x49d9e2, 0x49e40f |
| 0x491e20 | 331 | thiscall(stage*) ret 4 | **SimulateStageTimes**: builds [0x1888+i*4] for i<10 from AI table [0x1a84] plus rand (0x4c1998 = rand; masks 0x3ff/0x3fff/0xfff, scales 0.001, 163.84), stores [0x1a24+stage*4] | 0x48fa8f |
| 0x491f70 | 14 | fastcall | GetResultTime: [this+[0x1a50]*4+0x1a24] (also qsort comparator helper) | 0x485a61, 0x485a6a, 0x485a7e, 0x485a87, 0x488131, 0x488172, 0x48a53a, 0x48a5eb, 0x48a60a, 0x48a621, 0x48b9eb |
| 0x491f80 | ~48 | CRT init | guard stub, module end | init table |

Notes: Player has no vtable; no Player virtuals. Constants used: 0x4e133c=5000.0, 0x4e1348=1.0, 0x4e134c=0.0, 0x4e15c8=0.01, 0x4e2780..0x4e2794 listed above (0.003921569, 0.007874016, 0.0078125, 6.1039e-05, 0.00097752, 163.84).

Decompile groups:
- B1 ctors/dtor/init (11): 0x490950, 0x4909b0, 0x490a60, 0x490ae0, 0x490b90, 0x490c30, 0x490ee0, 0x490fb0, 0x490fe0, 0x491110 (+ skip 0x490ad0 trivial)
- B2 car/results/lock helpers (14): 0x491130, 0x4912a0, 0x491350, 0x491370, 0x4913e0, 0x491410, 0x491430, 0x491450, 0x491470, 0x491480, 0x491df0, 0x491e10, 0x491f70, 0x491f80
- B3 net sampling + AI simulation (6, FPU heavy; do after B1/B2): 0x491490, 0x491620, 0x4916a0, 0x491950, 0x491c10, 0x491e20

## 3. Module C: race session / game-loop class, 0x491fb0 - 0x4952df (name = guess: `RaceGame`/`GameSession`)

Embedded in `Game` at **Game+0x139080** (ctor call 0x4304d5, in Game ctor 0x4303e0; the next member's array-ctor starts at +0x139110). Known size >= 0x38 (extends probably to 0x90). No vtable, no STL members (reads RaceMode's `vector<Player*>` inline: [mode+8]=first,[mode+0xc]=last).

Fields seen (guess names): +0x00 Sprite wrapper ptr (CountDown overlay: Sprite 0xd4 via ctor 0x4a85e0 holding image object 0x68 via ctor 0x4a8030 "Data/Global/Overlay/CountDown.tga", name "CountDown"), +0x04 RaceMode* (calls its vtable slots 4,5,6,15-17,18-21,25,29: offsets 0x10,0x14,0x18,0x3c-0x44,0x48-0x54,0x64,0x74), +0x08 Stage* ([mode+0x30]), +0x0c 0x194-byte object (ctor 0x4b7a40), +0x10 0x254-byte view/camera manager (ctor 0x456490; methods 0x456780(view), 0x456660, 0x4566e0, 0x458150; string "Car camera"), +0x14 int (followed-player index / frame counter), +0x19/+0x1a bytes (args), +0x1b byte flag, +0x1c Car* of followed player, +0x20 byte, +0x24 int (frame cap, `VideoOutFps` from data/settings/video.ini, default 25), +0x28 object* (deleted with 0x4aafa0 in 0x492060), +0x2c byte =1, +0x30 int (arg), +0x34 0x7c-byte object (ctor 0x458700(0x56b0cc), init 0x4588d0).
Globals written: 0x501470 (demo/attract flag, set in 0x492e40), 0x501328 (selected camera 0..5/-1), 0x565508/0x5654fc/0x565500 (loop state), viewport rect 0x508d94..0x508dbc, record arrays 0x50149c/0x501498/0x5014b0[]. Renderer is the object at 0x56b0ac (47 vtable calls: slots 0x34,0x48,0x4c,0x5c,0x60,0x6c,0x70,0x80,0x84,0x98,0xa4,0xa8,0xb0,0xc0,0xcc,0x10).

| Addr | Size | Sig | What | Callers |
|---|---|---|---|---|
| 0x491fb0 | 169 | fastcall ctor, SEH | calls 0x457b10(this) (config/video loader, 1599 bytes, outside), sets defaults, reads VideoOutFps | 0x4304d5 |
| 0x492060 | 44 | fastcall | Shutdown: destroy [0x28] (0x4aafa0 + delete), then 0x492190 | 0x433974, 0x43419d, 0x43493e, 0x434b53, 0x434d63 |
| 0x492090 | 256 | fastcall, SEH | lazily create CountDown sprite (0xd4 + 0x68 objects, set defaults) | 0x49273d, 0x4933f9, 0x4950d1 |
| 0x492190 | 51 | fastcall | destroy that sprite | 0x492084, 0x4950ba |
| 0x4921d0 | 3171 | thiscall(RaceMode*, int, byte, byte) ret 0x10 (args inferred) | RunRace: builds loading text (" - ", strings 0x405380 ids 0x1a/0x2a/0x1b/0x136/0x138), creates Stage-side objects 0x254 and 0x194, "Car camera", handles `data/replay/demomode`+".rep", then calls 0x493650 in loop | 0x43395b, 0x434194, 0x434914, 0x434d5c |
| 0x492e40 | 2050 | thiscall(RaceMode*) ret 4 | variant of the above for the alternative Game entry 0x434300 (sets 0x501470=1, [0x30]=0); no demomode string | 0x434b4c |
| 0x493650 | 5605 | thiscall(int) ret 4, SEH | per-frame race loop body: sets view rects, input, drive RaceMode virtuals, renderer begin/end, screenshots "ScreenShots/rally_%i.tga", countdown draw | 0x492986, 0x492bbf, 0x492c0c, 0x492d26, 0x492dad, 0x4934e1 (internal only) |
| 0x494c80 | 119 | fastcall | RecordStageProgress: updates max-progress arrays 0x501498/0x50149c/0x5014b0[] | 0x494a0b |
| 0x494d00 | 498 | thiscall(a,b*) ret 8 | camera key handling: key table 0x57ba58-0x57ba61 -> view select (0x456780) and 0x501328; advances [this+0x14] | 0x493c93 |
| 0x494f00 | 247 | thiscall(int) ret 4 | compute viewport rect globals 0x508d94-0x508dbc from camera manager (0x458150/0x456660/0x4566e0) | 0x4937c0, 0x494ee6, 0x49506c |
| 0x495000 | 119 | fastcall | pick the best eligible Player ([p+0x38]==0,[p+0x1824]!=0,[p+0x30]==0, min [p+0x1a18]) -> [this+0x14], then 0x494f00(0) | 0x494b74 |
| 0x495080 | 549 | thiscall(Player/mode*) ret 4, SEH | draw countdown overlay (CountDown sprite quads; two local Vec2[4] arrays via 0x4c11f4; renderer slots 0x8c, 0x108) | 0x4941da |
| 0x4952b0 | ~48 | CRT init | guard stub, module end | init table |

Decompile order for C: first 0x491fb0, 0x492060, 0x492090, 0x492190 (small, define the class), then 0x495000, 0x494f00, 0x494c80, 0x495080, 0x494d00, then the big three 0x4921d0, 0x492e40, 0x493650 (share most logic; 0x492e40 diffs first, then 0x4921d0). 0x493650 is a single 5.6 KB function; it may be split by worker into regions (0x493650-0x493b60 setup/loop head, 0x493b60-0x494260 per-frame render passes, 0x494260-0x494c35 tail/input).

Groups:
- C1 (9): 0x491fb0, 0x492060, 0x492090, 0x492190, 0x494c80, 0x494f00, 0x495000, 0x495080, 0x4952b0
- C2 (3, medium): 0x494d00, 0x4921d0, 0x492e40
- C3 (1, large): 0x493650

## 4. Controller module D start (for the owner of unk_495640.cpp)

Static initialisers at 0x4952e0-0x49540f belong to D (their data is used by DirectInput code from 0x495410 onward):
- 0x4952e0: `call 0x4952f0; jmp 0x495310`. 0x4952f0 (29 B) ctor of a static vector at **0x569e48** (allocator byte 0x569e48, first 0x569e4c, last 0x569e50, end 0x569e54, all zeroed). 0x495310 registers atexit(0x495320). 0x495320 (dtor: operator delete(first), zero the three pointers).
- 0x495350: `call 0x495360; jmp 0x495380`. 0x495360 = `__vec_ctor(0x569e30, 8, 3, 0x490ad0, 0x402c90)`: static `Vec2[3]` at 0x569e30 (= `DAT_00569e30[axis].min/.scale` in unk_495640.cpp). 0x495380 = atexit dtor 0x495390 (`__vec_dtor`).
- 0x4953b0: same for `Vec2[3]` at 0x569e18 (0x4953c0 vec ctor, 0x4953e0 atexit, 0x4953f0 dtor).
- guard byte 0x569e5b used at 0x495410. unk_495640.cpp already references these.
Plain PE disassembly observations: the 0x4952e0, 0x495350 and 0x4953b0 call/jump pairs are init-table wrappers; 0x495320 frees and clears the three vector pointers; 0x495390 and 0x4953f0 invoke the EH vector destructor iterator. These twelve code bodies (including all six entries already present in `functions.csv`) are now represented in `src/unk_495640.cpp`; the six omitted callback/wrapper entries are added to `functions_extra.csv`. Focused matcher: 46/46 marked functions match, with five existing NONMATCHING functions and no DIFF or compile failures.

The DirectInput source range begins at 0x4952e0, as recorded in `modules.csv`; the preceding records/player/race-session range ends at 0x4952df.

## 5. STL / library instances
None defined in 0x48fe20-0x4952df (no vector/string globals, no templates instantiated). Uses: Config class (0x405580 ctor, 0x4076c0 load, 0x406ce0 section, 0x407d50 getString, 0x407f60 getFloat, 0x408170, 0x405600/0x4055a0 dtor), string (0x402700, 0x402960, 0x402980, 0x403270...), `vector<Player*>` read inline from RaceMode, EH vec helpers 0x4c11f4/0x4c1116, `operator new` 0x4c1276, delete 0x4bd000.

## 6. Suggested order and workers
1. A (RecordTable, 10 fns) - independent, clean.
2. B1 then B2 (Player), then B3. B1/B2 can run in parallel with A.
3. C1, C2, C3 (C2/C3 depend on Player header and RaceMode vtable layout from racemode-base.md).
Header work needed first: `struct Record` (0x60), `class RecordTable`, `class Player` (0x1a88) and its `SampleBuffer` (0x5c elements), `class RaceSession`. Update `Game` members at +0x5c30 and +0x139080 when Game is decompiled.
