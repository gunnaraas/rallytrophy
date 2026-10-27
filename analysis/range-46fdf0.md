<!-- Subagent analysis, 2026-10-23; read-only disassembly (rawdis.py/refs.py, Patch 1.01 exe); no REA. "Guess" marks inference. -->
# Range 0x46fdf0 - 0x47419f: module map (two source files: ParallelShadow + Vec2 inlines, and the Rain weather effect)

Method: rawdis.py over the whole range, caller index with refs.py, .rdata strings/floats/vtables read from the PE. Sizes are from analysis/functions.csv; functions the csv lacks are marked "(not in csv)". EH handler addresses (0x4dc168 ... 0x4dc45c) are the `push imm` after `push -1` in each SEH prologue.

## 0. Headline findings

1. **The big class of file A is the `ParallelShadow` (car shadow) renderer, not a camera/plane class.** Its Create (0x470610) builds a 0x68-byte Texture from the string 0x4fe4cc `"ParallelShadow"`, and a second one from 0x4fe4c8 `"sys"` (system-memory texture). The class draws the car (up to 4 casters) projected along the light direction into a small texture. Two back ends: (a) **hardware render-to-texture** (0x4717e0, device vtable calls: SetRenderTarget slot 0x7c, viewport slot 0x3c/0x38, SetRenderState slot 0x84, SetWorldMatrix 0xb8, SetProjection 0xc0/0xc8, mesh draw 0x100) and (b) a **software rasteriser** (0x471650 lock texture, fill, 0x4721b0 project and rasterise caster triangles, 0x472710/0x4728b0/0x472970/0x472a30 scan-converter with 16.16 fixed point, write 8/16 bit pixels). The switch between them is `[0x5012e4]` (guess: nonzero = render-to-texture capable) read in Create (0x470610: branch at 0x470668) and in 0x470940.
2. **The class is owned by the Stage at +0x20ec** (`new(0x330)` at 0x46094d, ctor 0x4703c0 called from 0x460967, Create 0x470610 from 0x46098a); the per-frame entry is 0x470940, called from 0x469787 with ecx = Stage+0x20ec (and `push 0x470940`-style code ref at file offset 0x12ebd3 i.e. also taken as an address elsewhere; guess: a callback table). 0x469730 calls 0x472c20 (SetLightMatrix) with `[[Stage+0x160]]+0x60` (the lead car's body matrix). 0x4696f6 calls 0x472c40 (SetCaster) in a loop over 4 cars (`add ebp,0x330` = per-slot Player size).
3. **The second file (0x472c90-0x47419f) is the Rain weather effect**, sibling of the SnowFall (0x474c50, range-4741a0.md section 2.2). Stage ctor 0x413d53: `case Stage+0x22c == 1: new(0x7c); Rain(loader = &Stage+0xd4, 0x400, 0x200)`; `case 2: new(0x202c); SnowFall(...)`. Both derive from the abstract `WeatherEffect` (vtable 0x4e2000: slot 0 = scalar deleting dtor 0x4732b0, slot 1 = _purecall 0x4c19e3). Rain vtable 0x4e1ff8: slot 0 0x4732d0 (scalar deleting dtor), slot 1 0x473470 (**Update(device, camera, a, b, dt)**, `ret 0x14`). Texture string 0x4fe4dc `"data/global/weather/raindrops.tga"` (load flags 0x40, like the snowfall's).
4. **0x472c90 is `ViewMat::SetInverseOf(const ViewMat&)`** (this = ecx, src on the stack, `ret 4`): transposes the 3x3 part into this and copies the translation row and the last column dwords unchanged (it does NOT negate the translation, so it is a 'transpose-3x3 + copy', exactly as documented in include/unk_47b4c0.h). It is the only function of file B that is not rain code (see 2.4). Callers in file A: 0x470eba (ParallelShadow::Update, with ecx=0x509640 static, arg 0x509680) and 0x4721f3 (rasteriser, ecx=0x509580, arg 0x509600); elsewhere 0x47b97a (CarEmitterC), 0x4b9d68/0x4b9dd3/0x4b9e51/0x4ba183/0x4bb417.
5. **File A starts with out-of-line Vec2 inline instances** (0x46fdf0 `operator-`, 0x46fe20 `operator*(float)`, 0x46fe50 `operator/(float)`) which are used by the previous file (callers 0x46fc91..0x46fd64) and by the sky (0x4762fc, 0x476312, 0x47634f, 0x4763b3). They are COMDAT-like instances emitted into this TU's code (the TU starts after the stub at 0x46fdc0); none of the ParallelShadow code calls them (guess: the TU includes the Vec2 header and one of its inlines is referenced through the linker's first-definition-wins rule).
6. **Eleven static initialisers precede the code** (the exe's init table at file offset 0xefc14..0xefc3c, entries `call ctor ; jmp registrar-of-atexit`). Four are empty-ctor/empty-dtor statics with no storage touched (0x46feb0, 0x46ff50, 0x4700d0, 0x470110; ctor = `ret`, atexit callback = `ret`; guess: statics of a class with a non-trivial but empty ctor, e.g. an ABC or a `Vec2`-like with a user-declared ctor in an unused TU). The others construct: a 16384-element array (0x14 bytes each) at 0x5096f8; five 4x4 matrices (0x40 bytes) at 0x509680, 0x509640, 0x509600, 0x5095c0, 0x509580 (ctor 0x490ad0 = `mov eax,ecx; ret`, dtor 0x402c90 = same shape: the shared empty ctor/dtor); an array of 3 elements of 0x10 bytes at 0x509500.
7. **STL guard stubs**: 0x472c60 (ends file A, init table 0x4efc40) and 0x474170 (ends file B, table 0x4efc44): `mov cl,[0x5826d5]; mov al,1; test; jne; or cl,al; mov [0x5826d5],cl; jmp +0x10; push 0x402c90; call atexit(0x4c0d01); pop; ret`. 0x472c60 spans 0x472c60-0x472c8f with its body at 0x472c80; 0x474170 spans to 0x47419f with its body at 0x474190. Both are not in functions.csv.

## 1. Module / file table

| # | Range | End marker | Content | Suggested file |
|---|---|---|---|---|
| A | 0x46fdf0 - 0x472c8f | stub 0x472c60 (+body 0x472c80) | Vec2 inlines, base-class stubs, 11 static initialisers, colour LUT builder 0x4701b0, `ParallelShadow` (ctor, dtor, Create, Render slot, Update, HW and SW back ends, rasteriser), tiny setters | src/unk_46fdf0.cpp |
| B | 0x472c90 - 0x47419f | stub 0x474170 (+body 0x474190) | `ViewMat::SetInverseOf` (guess: header inline emitted here), `WeatherEffect` base stubs, `RainEffect` (ctor, dtor, InitDrops, Update) | src/unk_472c90.cpp |

Evidence for the split: the two guard stubs (0x472c60, 0x474170) are 0x30 bytes each and follow the pattern given in the brief; the next stub after 0x474170 is 0x474c20 (the SnowFall file, range-4741a0.md). The previous stub is 0x46fdc0.

Constants shared: 0x4e1348 = 1.0, 0x4e134c = 0.0, 0x4e138c = 0.5, 0x4e1b98 = 0.9, 0x4e13a8 = 1/255, 0x4e13ac = 255.0, 0x4e13a0 = 1000.0, 0x4e139c = 0.1, 0x4e18f8 = 0.01, 0x4e1b44 = 2.0, 0x4e1b9c = 0.05, 0x4e1654 = 20.0 (used by the rain Update), 0x4e1358 = 1/1024.

## 2. File A: Vec2 helpers, statics and `ParallelShadow`

### 2.1 Vec2 inlines (0x46fdf0, 0x46fe20, 0x46fe50)

| Address | Size | Role | Body |
|---|---|---|---|
| 0x46fdf0 | 36 | `Vec2 Vec2::operator-(const Vec2& o) const` (thiscall, hidden result pointer in [esp+8] / `ret 8`) | `r.x = x - o.x; r.y = y - o.y` ; has the usual `mov [esp+0],0` dead temp |
| 0x46fe20 | 35 | `Vec2 Vec2::operator*(float s) const` (`ret 8`) | `r.x = s*x; r.y = s*y` |
| 0x46fe50 | 39 | `Vec2 Vec2::operator/(float s) const` (`ret 8`) | `t = 1.0f / s` (the constant 0x4e1348), `r.x = t*x; r.y = t*y` |

### 2.2 Classes and vtables

* **Abstract base at vtable 0x4e1fb4** (2 slots: 0 = 0x46fe90 scalar deleting dtor, 1 = 0x4c19e3 `_purecall`). The two standalone bodies are 0x46fe80 (7 bytes, `mov [ecx],0x4e1fb4; ret`, the inlined-looking ctor; no callers) and 0x46fe90 (31 bytes, `ret 4`, sets the vtable and calls `operator delete(this)` if `flags & 1`; referenced only from the vtable). Both are represented by naked ABI helpers in `src/unk_46fdf0.cpp`; the source class keeps its destructor inline so the generated vptr store remains in the scalar-destructor path. Six more code references to the vtable (0x46fde3, 0x46fdfd... in the previous file, 0x4765c5 in the sky file, 0x4b5132): it is a shared interface (guess: the Stage-owned `Weather`/`Environment` interface or a `Light`/`Shadow` interface). Not used by anything in this range.
* **ParallelShadow**: size **0x330** (`new(0x330)` at 0x46094d), derives from the engine object base (`EffectBase`, ctor 0x4b2830, dtor 0x4b29c0, 9-slot vtable, include/unk_47c460.h). Vtable **0x4e1fc8** (9 slots, then floats from 0x4e1fec):

  | slot | address | meaning |
  |---|---|---|
  | 0 | 0x470540 | scalar deleting dtor (`ret 4`; calls 0x470560, `operator delete` if flags&1) |
  | 1 | 0x4b2c10 | base |
  | 2 | 0x48e660 | shared empty `ret` |
  | 3 | 0x4b2d50 | base |
  | 4 | 0x4b2fc0 | base |
  | 5 | **0x4708e0** | `Render(RenderDevice* device, Camera* camera)` override (`ret 8`, 96-byte span, not in csv) |
  | 6 | 0x4031e0 | base |
  | 7 | 0x4b3010 | SetFlag04 |
  | 8 | 0x4890c0 | `return 1` |

  .rdata after the vtable: 0x4e1fec = 1.01f and 0x4e1ff0 = 0.35f belong to 0x470940 (clip/expand and a depth bias), 0x4e1ff4 = 65536.0f (rasteriser fixed-point scale), 0x4e1fc0 = 1/65536, 0x4e1fc4 = 1/3 (0x4701b0). Dump of 0x4e1fec.. is `[1.01, 0.35, 65536.0]`.

* Fields seen (offsets into the 0x330-byte object; 0x00-0x123 is the engine object base):

  | Offset | Size | Seen in | Meaning |
  |---|---|---|---|
  | 0x00 | 4 | ctor | vptr (0x4e1fc8) |
  | 0x08 | 4 | ctor | set to 1 at the end of the ctor (`[+8] = 1`) |
  | 0x0c | 1 | ctor, 0x4708e0, 0x470940 | flags: ctor does `|= 1`; 0x4708e0 returns when `(flags & 3)`; 0x470940 sets bit 0 |
  | 0xcc | 1 | ctor | `&= 0xfc` (engine-base flags byte) |
  | 0x124 | 4*4 | ctor (zeroed 0x124..0x130), 0x470940, 0x471650, 0x472c40 | **caster table: 4 object pointers** (0x472c40 `SetCaster(obj, idx)`: `this[0x124 + idx*4] = obj`) |
  | 0x134 | 0xd4 | ctor (Sprite ctor 0x4a8580 at ecx=this+0x134), dtor, Create | **embedded Sprite A**; its first field (`Texture* texture`) is the 0x68-byte texture named "ParallelShadow" created in Create; the dtor deletes the texture through its vtable slot 0, then calls the Sprite dtor 0x4a86d0 |
  | 0x20c | 4 | ctor (zeroed), Create, dtor | **lock/surface object** (the 0x68-byte "sys" texture, result of device call [0x20]), deleted in the dtor |
  | 0x208 | 4 | 0x471650 | surface description pointer (from 0x4a84c0: has [+4] width, [+8] height, [+0x18] pixel pointer) |
  | 0x210 | 0x40 | 0x472c20, 0x470940 | **light/world matrix (4x4)** copied in by SetLightMatrix; rows at 0x210, 0x220, 0x230 are right/up/forward (0x230,0x234,0x238 = direction used in 0x470940's dot products) |
  | 0x250 | 4 | ctor, dtor, 0x4708e0, 0x470940 | `Mesh*` (0x38 bytes, ctor 0x4baf40, dtor 0x4baf90): one quad of 4 vertices x 0x2c bytes (0x400 vertices array: ctor allocates `new(0xb004)` = 0x400 * 0x2c + 4, vertex ctor 0x490ad0), indices `new(0x1800)`; vertex init loop sets `+0x18 = -1`, `+0x24/0x28/0xc..0x14 = 0` |
  | 0x254 | 4 | Create, 0x471650 | **texture format** (1..6; 4 and 7 set +0x32c) |
  | 0x258 | 0xd4 | ctor (Sprite ctor), 0x4717e0 | **embedded Sprite B** (selected for the final draw via device vtable 0x8c) |
  | 0x2b8..0x2c0 | 3*4 | 0x471650 | shadow colour RGB, floats 0..1 (x255 then packed) |
  | 0x32c | 1 | Create, 0x471650 | `true` when the software path writes 32-bit pixels (formats 4 and 7) |
  | 0x32d | 1 | ctor (cleared), Create (set when Create succeeds), 0x4708e0, 0x470940, 0x471650 | **ready** flag |

  (0x230..0x238 appear in 0x470940 as the light direction multiplying camera axes at camera+0x70/+0x80/+0x60; 0x214/0x218 and 0x220..0x228 are copied into the static matrices at 0x509680..0x509698.)

### 2.3 Static data of file A (storage, with writers)

| Address | Size | Type (guess) | Role |
|---|---|---|---|
| 0x509500 | 3 x 0x10 | array of 3 empty-ctor objects | clip planes / frustum edge vectors (normal xyz at +0, dot at +0xc), written in 0x470940 (0x509500..0x50952c) |
| 0x5094d0..0x5094d8 | 12 | Vec3 | rasteriser: shadow-plane origin? read in 0x4721b0 |
| 0x5094dc..0x5094fc | 0x24 | scan-converter state | 0x5094dc/0x5094e8 rows left on the left/right edge, 0x5094e0/0x5094ec per-row x step, 0x5094e4/0x5094f0 current x (16.16), 0x5094f4/0x5094f8 current edge ids, 0x5094fc triangle pointer (written by 0x472710) |
| 0x509530 | 4 | DWORD | 32-bit fill colour |
| 0x509534 | 2 | WORD | 16-bit fill colour (packed by the switch in 0x471650) |
| 0x509538 | 4 | int | pitch (bytes) |
| 0x50953c | 4 | BYTE* | current scanline pointer |
| 0x509540..0x509578 | 0x3c | Vec3 x several | shadow plane origin and axes (written by 0x470940, read by 0x471ab0) |
| 0x509580 | 0x40 | ViewMat | static matrix (SetInverseOf target in 0x4721b0) |
| 0x5095a0..0x5095b8 | 0x1c | floats | transform terms used by 0x4721b0 |
| 0x5095c0, 0x509600, 0x509640, 0x509680 | 0x40 each | ViewMat | static matrices |
| 0x5096a0..0x5096bc | | floats | shadow origin/extents; 0x5096c0..0x5096d0 bbox, 0x5096dc/0x5096e0 reciprocal of width/height, 0x5096e4..0x5096f0 centre |
| 0x5096f8 | 0x4000 x 0x14 = 0x50000 | array of 0x14-byte elements | **projected vertices** (vec ctor/dtor from 0x46fef0/0x46ff20) |
| 0x5596f8 | 0x4000 x 2 | WORD array | vertex index remap/visited table, filled with 0xffff by 0x471ab0 |
| 0x5617f8, 0x5618f8, 0x5619f8 | 0x100 each | BYTE[256] | colour LUTs (B, G, R) built by 0x4701b0 from the Stage vector +0x1c4; (not written by ParallelShadow; the writers/readers are in the car/player code: 0x4925ed, 0x493240) |
| 0x561af8..0x561b04 | 4 floats | floats | scratch copies of the Vec3 argument of 0x4701b0 and its inputs |

(0x5617f8.. LUTs and 0x561af8 sit in the same .data strip as the sky file's matrix at 0x561b08.)

### 2.4 Function table, file A

| Address | Size | Conv / frame | Role | Main callers |
|---|---|---|---|---|
| 0x46fdf0 | 36 | thiscall, `ret 8`, FPU | `Vec2::operator-` | 0x46fd33, 0x46fd64, sky 0x4762fc/0x476312/0x47634f/0x4763b3 |
| 0x46fe20 | 35 | thiscall, `ret 8`, FPU | `Vec2::operator*(float)` | 0x46fc91, 0x46fccc, 0x46fd20, 0x46fd51 |
| 0x46fe50 | 39 | thiscall, `ret 8`, FPU | `Vec2::operator/(float)` | 0x46fca8, 0x46fce0 |
| 0x46fe80 | 7 | thiscall | abstract base ctor (not in csv) | none |
| 0x46fe90 | 32 | thiscall, `ret 4` | abstract base scalar deleting dtor (not in csv) | vtable 0x4e1fb4 |
| 0x46feb0 | 16 | cdecl, stub | static init #1: `call 0x46fec0; jmp 0x46fed0` (not in csv) | init table 0xefc14 |
| 0x46fec0 | 1 | ret | empty ctor #1 | 0x46feb0 |
| 0x46fed0 | 12 | `push 0x46fee0; call atexit; pop; ret` | atexit registrar #1 | 0x46feb0 |
| 0x46fee0 | 1 | `ret` | empty dtor #1 (not in csv) | atexit |
| 0x46fef0 | 16 | stub | static init #2: array 0x5096f8 (`call 0x46ff00; jmp 0x46ff20`) (not in csv) | table 0xefc18 |
| 0x46ff00 | 28 | `__vec_ctor(0x5096f8, 0x14, 0x4000, 0x490ad0, 0x402c90)` via 0x4c11f4 | array ctor | 0x46fef0 |
| 0x46ff20 | 12 | registrar | | 0x46fef0 |
| 0x46ff30 | 32 | `__vec_dtor(0x5096f8, 0x14, 0x4000, 0x402c90)` via 0x4c1116 | array dtor (atexit target; not in csv) | atexit |
| 0x46ff50 | 16 | stub | static init #3 (empty, `call 0x46ff60; jmp 0x46ff70`) | table 0xefc1c |
| 0x46ff60 / 0x46ff70 / 0x46ff80 | 1 / 12 / 1 | empty ctor / registrar / empty dtor | | |
| 0x46ff90 | 16 | stub | static init #4: ViewMat at 0x509680 (`mov ecx,0x509680; jmp 0x490ad0`) | table 0xefc20 |
| 0x46ffa0 | 10 | ctor thunk | | 0x46ff90 |
| 0x46ffb0 | 12 | registrar | | 0x46ff90 |
| 0x46ffc0 | 16 | dtor thunk (`mov ecx,0x509680; jmp 0x402c90`) (not in csv) | | atexit |
| 0x46ffd0 .. 0x470090 | 4 x (16 stub + 10 + 12 + 16 thunk) | | statics #5-#8: 0x509640 (0x46ffd0/e0/f0/0x470000), 0x509600 (0x470010/20/30/40), 0x5095c0 (0x470050/60/70/80), 0x509580 (0x470090/a0/b0/c0) | tables 0xefc24..0xefc30 |
| 0x4700d0 / 0x4700e0 / 0x4700f0 / 0x470100 | 16 / 1 / 12 / 1 | empty static #9 | | table 0xefc34 |
| 0x470110 / 0x470120 / 0x470130 / 0x470140 | 16 / 1 / 12 / 1 | empty static #10 | | table 0xefc38 |
| 0x470150 | 16 | stub | static init #11: array of 3 x 0x10 at 0x509500 (`call 0x470160; jmp 0x470180`) | table 0xefc3c |
| 0x470160 | 25 | `__vec_ctor(0x509500, 0x10, 3, 0x490ad0, 0x402c90)` | | 0x470150 |
| 0x470180 | 12 | registrar | | 0x470150 |
| 0x470190 | 32 | `__vec_dtor(0x509500, 0x10, 3, 0x402c90)` (not in csv) | | atexit |
| 0x4701b0 | 514 | cdecl, `push ebp` frame, FPU, 1 arg (`const Vec3*`) | **BuildColourLuts(const Vec3* light)**: copies x,y,z, derives 2y+x scale (x=r, y=g, z=b?), builds three 256-entry BYTE tables at 0x5619f8/0x5618f8/0x5617f8 (zero table if the component is <= 0, else a reciprocal ramp `((1-c)... )*255`), constants 1.0, 0.9, 1/255, 255.0 | 0x4925ed, 0x493240 (stage loader code at 0x49xxxx) |
Evidence `ev_8dbc3ed6ac91b6122c9761faf6bc2363cc6f97d9b0995fdeab74348df5670c62` records the body. After source-shape attempts left 12 frame-slot bytes unmatched, `BuildColourLuts` was replaced with a bounded naked-asm transcription. The focused matcher and `objcmp` confirm an exact 514-byte body; 38 COFF relocations preserve calls and symbolic references, with the PE absolute-address fixups masked by `objcmp`.
| 0x4703c0 | 378 | fastcall thiscall, SEH EH 0x4dc168 | **ParallelShadow::ParallelShadow()**: 0x4b2830 base, Sprite ctors at +0x134 and +0x258 (0x4a8580), vtable 0x4e1fc8, SetFlag04 (0x4b3010), flags, `new(0x38)` Mesh at +0x250 with the 0x400-vertex array (0x2c each, vec ctor 0x4c11f4) and 0x1800 index bytes, init loop, zero 0x124..0x130 | Stage ctor 0x460967 |
Evidence `ev_e2d18152e9c91241b4226ef784a2de4febf5e28195db3b37cc938a94cbefa4fb` confirms the vertex initialization offset loop (`0` through `<0xb000`, stride `0x2c`) and four caster clears. The current source expresses that loop and builds a 378-byte candidate, but 15 bytes still differ in stack-cleanup order, register selection, and equivalent SIB address ordering; this remains NONMATCHING. A second assembly capture (`ev_a4849866ee6245fe6292dece536ae0256b174e90c489d4e17c935cac35de6719`) isolates the remaining loop instructions. Reversing pointer-plus-offset expressions, naming the `-1` color, and using explicit or volatile index allocation did not change the mismatch. Naming the `WORD[0xc00]` allocation result before assigning `m->indices` also left codegen unchanged. Re-reading `m->vertices` in the loop increased the difference to 50 bytes; using the ordinary field access for the cached pointer changed register allocation and increased it to 168 bytes. A typed `i`-indexed loop grew the body to 389 bytes with 113 differing bytes; both probes were reverted.
| 0x470540 | 30 | thiscall, `ret 4` | scalar deleting dtor (vtable slot 0) | vtable |
| 0x470560 | 168 | SEH EH 0x4dc1b2 | **~ParallelShadow()**: deletes Mesh +0x250, the "sys" texture +0x20c, the shadow texture +0x134 (via vtable slot 0), then Sprite dtors (0x4a86d0) at +0x258 and +0x134, base dtor 0x4b29c0 | 0x470543 |
| 0x470610 | 715 | thiscall, `ret 8`, SEH EH 0x4dc1d6 | **Create(Engine* eng, int size)**: allocates texture 0x68 "ParallelShadow" (0x4a7fd0), reads `[0x5012e4]`; fills the Sprite A fields (+0x14c.. 3, 7, 0xe, 8, 0xb, scales 1.0f), then either asks the device for a render-target texture (vtable 0x18, 0x1c) or creates a system-memory texture ("sys" 0x4fe4c8) and queries the format (0x20, 0x1c); sets +0x254/+0x20c/+0x32c/+0x32d | Stage ctor 0x46098a |
| 0x4708e0 | 48 | thiscall, `ret 8` | **vtable slot 5 Render(device, camera)**: if `(flags&3)==0 && ready`: select material (device 0x8c with Mesh+0x250.material), set `mesh+0x24 = 0`, `mesh+0x18 = 0`, device call 0xb0(1), then device 0x100(mesh) (draw) (not in csv) | vtable |
| 0x470940 | 3336 | thiscall, `ret 0x14`, SEH EH 0x4dc21e, `push ebp` frame, FPU heavy | **ParallelShadow::Update/Prepare(engine, ?, ?, Camera* cam, int)**: sets flags|=1; if `ready`, projects the camera frustum corners and the car into the shadow plane: builds clip planes into 0x509500..0x50952c and 0x509540..0x509578, the matrices 0x509640/0x509680/0x5095c0/0x509600 (via 0x4b5540 matrix multiply, 0x472c90 inverse, `rep movs` of 16 dwords), the 8-corner bbox (vec ctor 8 x 0xc), 0x5096dc/0x5096e0 reciprocals, then calls 0x471ab0 (extent), then 0x4717e0 (HW) or 0x471650 (SW). Uses 0x40b0c0 (Vec3*float), 0x4203b0, 0x419f00 (cross/clamp), 0x420430 (singleton getter) | 0x469787 (Stage render), also code ref at 0x470940 push |
| 0x471650 | 376 | thiscall, `ret 4`, `push ebp` frame, FPU | **RasteriseSoftware(Camera* cam)**: surface = 0x4a84c0; fills the surface with 0xff; packs the shadow colour (+0x2b8..0x2c0) according to `[+0x254]` (switch 1..6, jump table 0x4717c8: 565 / 555 / 444-style); runs 0x4721b0 for the lead object and the 4 casters (+0x124[4]); unlock 0x4a8500, upload 0x4a8490 | 0x471618 (0x470940), jump table at 0x4717c8 follows (6 dwords: 0x47171d, 0x471739, 0x471778, 0x471778, 0x47171d, 0x471755) |
| 0x4717e0 | 710 | thiscall, `ret 4`, SEH EH 0x4dc240 | **RenderToTexture(Camera* cam)**: HW path; device (global 0x56b0ac): GetViewport (0x3c), set orthographic projection from 0x5096dc/0x5096e0 and 0x5096b0.. extents, SetRenderTarget (0x7c), clear (0x58), BeginScene (0x6c), set state (0x84/0xa8/0x60), matrices (0xb8, 0xc0), material (0x8c with Sprite B +0x258), draw casters via 0x472bb0 (lead then 4 slots), EndScene (0x70), restore viewport (0x38) | 0x47160b (0x470940) |
| 0x471ab0 | 1788 | thiscall, `ret 4`, SEH EH 0x4dc25b | **ProjectMeshes(Object* obj)**: walks the caster object's mesh list (`obj+0x124` vector, 0x4a6800 = list at()), for meshes of type 1 (`[mesh+0x1c] == 1`) clears the 0x4000-WORD remap table (0x5596f8) and transforms all vertices into the shadow plane into 0x5096f8 (0x14 bytes per vertex), building the bounding box 0x5096c0..0x5096d0 and centre 0x5096e4..0x5096f0 | 0x4712f8 (0x470940) |
| 0x4721b0 | 1372 | thiscall, `ret 4`, SEH EH 0x4dc288 | **RasteriseObject(Object* obj)**: matrix product 0x509600 = obj*0x509640 (0x4b5540), inverse into 0x509580 (0x472c90), derives transform terms 0x5095b0..0x5095b8, per mesh/triangle projection to 16.16 fixed point (scale 65536.0 at 0x4e1ff4) and `0x472710` per triangle, skipping back faces | 0x47177e, 0x471797 (0x471650) |
| 0x472710 | 405 | cdecl, no frame, 1 arg (triangle `[3]` of projected vertex pointers) | **FillTriangle(ProjVertex** tri)**: orders the three vertices by y (index of top/middle/bottom), sets the two edge steppers (0x4728b0, 0x472970), loops over the scanlines (rows counters 0x5094dc/0x5094e8), writes fixed-point left/right x | 0x4726bd (0x472a30) |
| 0x4728b0 | 186 | cdecl, void, global state | **StepLeftEdge**: next edge on the left side; fixed-point reciprocal (`/ 65536`), `_ftol` 0x4c19bc | 0x472771, 0x47283d, 0x472a91, 0x472b46 |
| 0x472970 | 186 | cdecl, void | **StepRightEdge**: same for the right side | 0x472794, 0x47287a, 0x472ab4, 0x472b83 |
| 0x472a30 | 382 | cdecl | **FillTriangleFlat(ProjVertex** tri)**: flat-fill variant used by the shadow, loops scanlines with `rep stos` of the fill value 0x509530 (32-bit) and per-row pointer 0x50953c += pitch 0x509538 | 0x4726b6 (0x472710) |
| 0x472bb0 | 107 | thiscall, `ret 4` | **DrawObjectMeshes(Object* obj)**: device->SetWorldMatrix(obj+0x60, 1); iterates the mesh list of obj+0x124 (0x4a6800 at(0)), for each mesh saves `[mesh+0x24]`, zeroes it, device->vtable 0x100 (DrawMesh), restores | 0x471a17, 0x471a30 (0x4717e0) |
| 0x472c20 | 24 | thiscall, `ret 4` | **SetLightMatrix(const float m[16])**: `memcpy(this+0x210, m, 0x40)` | 0x469730 |
| 0x472c40 | 18 | thiscall, `ret 8` | **SetCaster(Object* obj, int idx)**: `this[0x124 + idx*4] = obj` | 0x4696f6 |
| 0x472c60 | 32 | stub | **end of file A**: STL guard stub (guard byte 0x5826d5; body at 0x472c80) | init table 0xefc40 |

Note about 0x4721b0 / 0x472710 etc.: they take a pointer to a Car/body object whose `+0x60` is a 4x4 world matrix (the same layout as `CarBody::m` in unk_47c460.h: right/up/forward rows at 0x20/0x30/0x40 is the body; at 0x60 is the Object's render matrix) and `+0x124` is a vector of mesh holders (`[+0x34]`..`[+0x38]` begin/end of the mesh pointer array, `[mesh]->[+0x1c]==1` selects shadow-casting meshes). Treat these as guesses that need a look at the earlier Car mesh classes.

## 3. File B: `ViewMat::SetInverseOf` and the `RainEffect`

### 3.1 Classes and vtables

* `WeatherEffect` (abstract base, vtable **0x4e2000**: slot 0 = 0x4732b0 scalar deleting dtor (sets vptr 0x4e2000 then `operator delete` if flags&1), slot 1 = 0x4c19e3 `_purecall`). Out-of-line pieces in the range: 0x4732a0 (`mov [ecx],0x4e2000; ret`, base ctor, no callers) and 0x4732b0. The standalone constructor body is represented as a naked ABI helper in `src/unk_472c90.cpp` so the derived constructors retain their inline codegen; it matches all 7 bytes. The scalar deleting destructor is 31 bytes through `ret 4` (the earlier 20-byte inventory size was short). Other users of the vtable: SnowFall's code (0x474xxx), 0x4f500f, 0x4fe7fb are data references; code refs 0x728a2, 0x728ba, 0x729bd, 0x7461d (file offsets in this strip). Already declared in include/unk_474c50.h.
* `RainEffect` (my name; guess), size **0x7c** (`new(0x7c)` at 0x413d23 in the Stage ctor), vtable **0x4e1ff8**: slot 0 = 0x4732d0 (scalar deleting dtor, `ret 4`: calls 0x4732f0 then delete if flags&1), slot 1 = **0x473470** (`Update(RenderDevice* device, Camera* camera, int a, int b, float dt)`, `ret 0x14`; same signature as `WeatherEffect::Update` in unk_474c50.h).
* Constructor **0x472d00** (SEH EH 0x4dc300, `ret 0xc`): `RainEffect(TextureLoader* loader, int dropCount, int splashCount)`. Stage ctor calls it with `ecx = new(0x7c)`, loader = `&Stage+0xd4`, dropCount = 0x400, splashCount = 0x200 (pushes `0x200, 0x400, lea ecx,[ebp+0xd4]` at 0x413d40..0x413d51).
* Fields:

  | Offset | Size | Written by | Meaning |
  |---|---|---|---|
  | 0x00 | 4 | ctor | vptr 0x4e1ff8 (0x4e2000 during base-class destruction) |
  | 0x04 | 4 | ctor | `Sprite*` (new(0xd4), ctor 0x4a8580), `sprite->texture = loader->Load("data/global/weather/raindrops.tga", 0x40)`; fields +0x98 = 0xd, +0x9c = 2, +0x18 = 5, +0x1c = 6, +0x24 = 4, +0x84 = 0xf, floats +0x60..+0x6c = 1.0, +0x70..+0x7c = 0 (blend/alpha-state words, colour 1.0) |
  | 0x08 | 4 | ctor | `Mesh*` for the drops (new(0x38), ctor 0x4baf40): material = sprite, `vertices = new(dropCount*4 * 0x2c + 4)` with count header and vec ctor of 0x2c-byte vertices, `indices = new(dropCount*12)` (6 WORD per quad: 0,1,2,0,2,3 pattern, `ecx*4+{0,1,2,0,2,3}`), `vertexCount = 4*[+0x10]`, `primitiveCount = 2*[+0x10]`, `+0x24 = 0`; initial vertex data: colour `-1`, normal 0, UV pairs from (0,0.5)/(1.0,0.5)/(1.0,0)/(0,0)-style (constants 0x3f000000, 0x3f800000) |
  | 0x0c | 4 | ctor | `Mesh*` for the splashes (same layout, `[+0x18]` = splash count, vertex UVs in quarter steps 0.25/0.5/1.0) |
  | 0x10 | 4 | ctor | drop count (arg 2) |
  | 0x14 | 4 | ctor | copy of drop count |
  | 0x18 | 4 | ctor | splash count (arg 3) |
  | 0x1c | 4 | ctor | copy of splash count |
  | 0x20 | 4 | ctor, 0x4733d0 | `float[3*dropCount]` positions (`new(count*12)`), randomised by 0x4733d0 as 16-bit fixed point |
  | 0x24 | 4 | ctor, 0x4733d0 | `RainSplash[splashCount]` (`new(count*0x34+4)` with a count header, vec ctor 0x4c11f4 with ctor 0x490ad0, 0x34 bytes each, dtor via 0x4c1116); field 0 of each is `-(rand() & 0xff)` (delay) |
  | 0x28 | 4 | ctor | 1.0 |
  | 0x2c | 12 | ctor | Vec3 (0, 30.0, 0) = rain streak/wind vector (guess), used with 0x40b0c0 (Vec3*0.05f) in Update |
  | 0x38 | 4 | ctor | 40.0 |
  | 0x3c, 0x40, 0x44 | 12 | ctor | 0.03, 0.025, 0.085 (guess: tuning scales) |
  | 0x48 | 1 | ctor | low byte of drop count (!) |
  | 0x4c, 0x50, 0x54 | 12 | ctor | `std::vector<T*>`-like (begin, end, cap): ctor reserves 0x200 pointers (`new(0x800)`, copy loop is the inlined `vector::reserve`) |
  | 0x78 | 4 | ctor | 0x11 (guess: effect kind / render pass) |

* Strings: 0x4fe4dc `data/global/weather/raindrops.tga`. Float constants: 0x4e2008 = 3.99993896 (rand&0x3fff -> 0..65535), 0x4e200c = 1/255, 0x4e2010 = 1.5259e-05 (1/65535) used by Update; 0x4e1358 = 1/1024; global enable flag `[0x501428]` (=1 default, tested at Update entry; also used elsewhere, e.g. 0x42a052).

### 3.2 Function table, file B

| Address | Size | Conv / frame | Role | Main callers |
|---|---|---|---|---|
| 0x472c90 | 101 | thiscall, `ret 4`, no frame | **ViewMat::SetInverseOf(const ViewMat& src)**: `this[0]=src[0]; this[1]=src[4]; this[2]=src[8]; this[3]=src[3]; this[4]=src[1]; this[5]=src[5]; this[6]=src[9]; this[7]=src[7]; this[8]=src[2]; this[9]=src[6]; this[10]=src[10]; this[11..15]=src[11..15]` (3x3 transpose; row 3 unchanged) | 0x470eba, 0x4721f3, 0x47b97a, 0x4b9d68, 0x4b9dd3, 0x4b9e51, 0x4ba183, 0x4bb417 |
| 0x472d00 | 1434 | thiscall (fastcall-seen), `ret 0xc`, SEH EH 0x4dc300, `push -1` frame | **RainEffect::RainEffect(TextureLoader*, int drops, int splashes)** | 0x413d53 (Stage ctor, case weather 1) |
| 0x4732a0 | 7 | thiscall | `WeatherEffect::WeatherEffect` (vptr 0x4e2000; extra inventory entry) | none |
| 0x4732b0 | 31 | thiscall, `ret 4` | `WeatherEffect` scalar deleting dtor (extra inventory entry) | vtable 0x4e2000 (also SnowFall) |
| 0x4732d0 | 30 | thiscall, `ret 4` | **RainEffect scalar deleting dtor** (calls 0x4732f0, delete if flags&1) | vtable 0x4e1ff8 |
| 0x4732f0 | 221 | thiscall, SEH EH 0x4dc32e | **RainEffect::~RainEffect**: vptr 0x4e1ff8; destroys the Sprite (+4: dtor 0x4a86d0 + delete), delete[] +0x20, vector dtor of the RainSplash array (+0x24, 0x34 bytes, vec dtor 0x4c1116, count header at -4), Mesh +8 (0x4baf90 + delete), Mesh +0xc, `delete +0x4c`, zero 0x4c..0x54, vptr 0x4e2000 | 0x4732d3 |
| 0x4733d0 | 154 | thiscall (fastcall-seen), no frame | **RainEffect::InitDrops()**: for each drop (`+0x10`) three `ftol(float(rand() & 0x3fff) * 3.99994)` into `+0x20[i*3..]`; for each splash (`+0x18`) `*(int*)(+0x24 + i*0x34) = -(rand() & 0xff)` | 0x47320d (ctor) |
| 0x473470 | 3324 | thiscall, `ret 0x14`, SEH EH 0x4dc45c, `push ebp` frame (0x1c8 locals), FPU heavy | **RainEffect::Update(device, camera, a, b, dt)**: returns early if `[0x501428]==0`; computes the camera-relative wind vector (camera+0x14 position, camera+0x70/+0x80 axes cross product, camera+0x134/+0x138..+0x140 attached object, +0x1c4 speed), advances the drops (16-bit wraparound positions in the weather box), updates the splash timers, builds quads into both Meshes (vertex 0x2c bytes: pos, normal, colour at +0x18, uv at +0x1c/+0x20), uses 0x40b0c0 (Vec3*f), 0x411520, 0x414330, 0x419f00, 0x484380 (vector erase), `rand` 0x4c1998, `_ftol` 0x4c19bc, and device vtable calls 0x84 (state), 0x8c (material), 0xa8, 0xb0, 0xb8 (world matrix), 0xc0, 0xcc, 0x100 (DrawMesh) | vtable 0x4e1ff8 slot 1 (also code refs 0x11ab73 and 0x1302d3 in file-offset terms, i.e. `push` of the address) |
| 0x474170 | 48 | stub | **end of file B**: STL guard stub (body at 0x474190) | init table 0xefc44 |

(File B's functions: 0x472c90, 0x472d00, 0x4732d0, 0x4732f0, 0x4733d0, 0x473470 = the 6 csv entries; 0x4732a0 and 0x4732b0 are the two extra WeatherEffect functions and 0x474170 the stub.)

## 4. Compiler-generated pieces and thunks

* Scalar deleting dtors: 0x46fe90 (abstract base 0x4e1fb4), 0x470540 (ParallelShadow), 0x4732b0 (WeatherEffect), 0x4732d0 (RainEffect). Base ctors written as stand-alone functions: 0x46fe80, 0x4732a0.
* Static-initialiser machinery (all `_$E`-style wrappers, 7 bytes of padding grouped in 16-byte slots): `call ctor ; jmp registrar` (16-byte stubs at 0x46feb0, 0x46fef0, 0x46ff50, 0x46ff90, 0x46ffd0, 0x470010, 0x470050, 0x470090, 0x4700d0, 0x470110, 0x470150), registrars `push dtorthunk ; call atexit(0x4c0d01) ; pop ; ret` (12 bytes: 0x46fed0, 0x46ff20, 0x46ff70, 0x46ffb0, 0x46fff0, 0x470030, 0x470070, 0x4700b0, 0x4700f0, 0x470130, 0x470180), ctor thunks (0x46ffa0, 0x46ffe0, 0x470020, 0x470060, 0x4700a0: `mov ecx,static ; jmp 0x490ad0`), dtor thunks (0x46ffc0, 0x470000, 0x470040, 0x470080, 0x4700c0: `mov ecx,static ; jmp 0x402c90`), array ctor/dtor wrappers (0x46ff00/0x46ff30 for 0x5096f8, 0x470160/0x470190 for 0x509500). **None of these has user source**; they come from the five `ViewMat` statics, two vec2 arrays, and four empty statics being declared in the headers the TU includes. Source order guess (mirrors the init order): empty static, `static ProjVertex proj[0x4000]` (0x5096f8), empty static, `static ViewMat mat680, mat640, mat600, mat5c0, mat580`, two empty statics, `static Vec3? planes[3]` (0x509500).
* STL guard stubs 0x472c60 and 0x474170 (see headline 7).
* Vtable-adjacent data: the 6-entry jump table at **0x4717c8** (inside code after 0x471650): `[0x47171d, 0x471739, 0x471778, 0x471778, 0x47171d, 0x471755]` for the texture-format switch of 0x471650 (`switch (fmt-1)` with 6 cases, default falls to 0x471778). Treat the bytes 0x4717c8..0x4717df as data, not code (objdump decodes them as garbage instructions).
* No thunks like the 6- and 5-byte ones at 0x480740/0x480770 exist in this range; the only small "thunks" are the stub/registrar triples above.

## 5. Suggested work groups

Per-function bytes use csv sizes. Do the matching-order-sensitive pieces (statics, base-class stubs) last because their emission order is fixed by declaration order in the file.

| Group | Functions | Bytes | Notes |
|---|---|---|---|
| A1 | 0x4701b0 (LUT builder), 0x4703c0 (ctor), 0x470540, 0x470560 (dtor pair), 0x470610 (Create) | 514 + 378 + 30 + 168 + 715 = 1805 | all small; Create has two branches on `[0x5012e4]` and an SEH region 0..4; the ctor mixes two Sprite ctors, a Mesh and 0x2c-byte vertex vec-ctor |
| A2 | 0x4708e0 (Render slot) + 0x470940 (Update/Prepare) | 48 + 3336 = 3384 | the big one; give it alone (ideally with 2 half-sessions); FPU heavy, many static-matrix writes; 0x472c90 and 0x4b5540 calls |
| A3 | 0x471650 (SW rasterise), 0x4717e0 (HW render), 0x472bb0, 0x472c20, 0x472c40 | 376 + 710 + 107 + 24 + 18 = 1235 | the jump table at 0x4717c8 follows 0x471650 |
| A4 | 0x471ab0 (project meshes) | 1788 | alone; vertex remap loop with `rep stos`, bbox, SEH EH 0x4dc25b |
| A5 | 0x4721b0 (rasterise object), 0x472710 (triangle), 0x4728b0, 0x472970 (edge steppers), 0x472a30 (flat fill) | 1372 + 405 + 186 + 186 + 382 = 2531 | scan-converter file statics (0x5094dc..0x50953c) and fixed-point; do 0x4728b0/0x472970 first |
| A6 | Vec2 inlines (0x46fdf0, 0x46fe20, 0x46fe50), abstract base (0x46fe80, 0x46fe90), the 11 static initialisers + array/matrix thunks, stub 0x472c60 | 110 + 39 + about 1000 of stubs and thunks | finish last; declare statics in the init order above (and the empty ones) |
| B1 | 0x472c90 (SetInverseOf), 0x472d00 (rain ctor), 0x4733d0 (InitDrops), 0x4732a0/0x4732b0 (WeatherEffect pieces), 0x4732d0, 0x4732f0 (dtors) | 101 + 1434 + 154 + 7 + 31 + 30 + 221 = about 1978 | ctor has long unrolled-looking vertex init loops (two meshes) and the inlined `vector::reserve` |
| B2 | 0x473470 (rain Update) | 3324 | alone; FPU heavy, SEH EH 0x4dc45c |
| B3 | stub 0x474170 | 48 | trivial, last |

Signatures to add to headers (guesses from the above): `class ParallelShadow : public EffectBase { ParallelShadow(); ~ParallelShadow(); void Create(Engine* eng, int size); void SetLightMatrix(const float* m); void SetCaster(Object* obj, int idx); void Update(...); ... }` with the 0x330-byte layout of section 2.2; `class RainEffect : public WeatherEffect { RainEffect(TextureLoader*, int, int); ~RainEffect(); void InitDrops(); virtual void Update(RenderDevice*, Camera*, int, int, float); }` with the 0x7c layout of 3.1; the abstract base 0x4e1fb4 needs a name from its users (previous file). The `ViewMat` class in include/unk_47b4c0.h already declares `SetInverseOf` at 0x472c90 (same file B goes there).
