<!-- Subagent analysis, 2026-10-23; read-only disassembly; guesses are marked. -->
# Range 0x4741a0 - 0x47886f: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py, refs.py (callers, abs refs), my own per-function call/constant index over the whole range, .rdata reads (vtables, floats, strings), and a look at the callers (Car ctor/dtor/update 0x45e5f0-0x462098, 0x45b4c0, 0x469a20, Stage loader 0x413a00, Stage render 0x411380). "Guess" marks inference. EH handler addresses (0x4dc496 ... 0x4dc9fe) are the `push imm` after `push -1` in each SEH prologue. Virtual-address conversion used for abs refs: text off + 0x400a00, .rdata off + 0x401800, .data off + 0x402600.

## 0. Headline findings

1. **Eight source files, as briefed** (end-of-file guard stubs `mov cl,[0x5826d5]; mov al,1; ...; jmp; push 0x402c90; call atexit(0x4c0d01)` at 0x474c20, 0x475880, 0x476460, 0x476670, 0x4766a0, 0x477340, 0x477ff0, 0x478840 - confirmed, and the exe's static-init table at 0x4f223c.. lists them in this order, interleaved with the extra static initialisers of F3, F7 and F8: 0x474c20, 0x475880, **0x4758b0**, 0x476460, 0x476670, 0x4766a0, 0x477340, **0x477370, 0x4773a0**, 0x477ff0, **0x478020, 0x478050**, 0x478840, then 0x478bb0 ...). The eight files are **not one family**:
   - F1 **SkidMarks** (tyre marks, one object per wheel, 4 at Car+0x2748, 0x6558 bytes each), vtable 0x4e2014.
   - F2 **SnowFall** weather effect (Stage+0x6b0, 0x202c bytes), vtable 0x4e2038, on an abstract base (vtable 0x4e2000).
   - F3 **Sky** (cloud/sky layers, Stage+0x6ac, 0xf70 bytes), NOT polymorphic. Owns a file-static 0x40-byte object at 0x561b08 (identity matrix).
   - F4 **TexScroller** (animated texture coordinates of a mesh, 0x30 bytes), vtable 0x4e2050, base vtable 0x4e1fb4.
   - F5 **empty file** (the stub at 0x4766a0 only).
   - F6 **WindowRain** (rain drops on the windscreen, 0x240 bytes), vtable 0x4e206c.
   - F7 **Car effect at Car+0x2100** (an `Effect` subclass, 0x12c bytes, vtable 0x4e20bc): wheel debris/spray, only spawned at the highest effect detail. Guess `WheelSpray`.
   - F8 **Car effect at Car+0x25e8** (an `Effect` subclass, 0x160 bytes in the Car layout, vtable 0x4e20fc): exhaust smoke. Guess `ExhaustSmoke`.
2. Two different base classes: F1, F6 derive straight from the engine object base (ctor 0x4b2830, dtor 0x4b29c0; **9**-slot vtables, no Clear slot, no `Effect` fields); F7, F8 derive from `Effect` (ctor 0x478e80 / dtor 0x478f00, 10-slot vtable with Clear 0x4790e0, particles from the ParticlePool 0x56ae90, live list at +0x128, bbox at +0xa0). F7/F8 are siblings of CarEmitterB/CarEmitterC (range-478870.md) and CarParticles (range-47c460.md); the Car ctor builds them in the order 0x2100 (F7), 0x222c (B), 0x2358 (CarParticles), 0x2488 (C), 0x25e8 (F8), 0x2748 (skid marks x4).
3. **0x477080 is slot 6 of every one of these vtables** (F1 0x4e2014, F6 0x4e206c, F7, F8 and the B/C/CarParticles emitters): a 15-byte forwarding function `ret 0xc` that calls virtual slot 5 (Render) with two of its three arguments. It is an inline virtual of the shared effect base, emitted into F6's code stream (between 0x476fa0 and 0x4770a0) and shared by the others - the linker folded the identical copies. No hole in functions.csv hides anything else of importance.
4. **Holes in functions.csv inside the range** (not listed in the csv, all real code, give them to the owning module): 0x4758b0 (call/jmp, F3 static-init wrapper), 0x4758e0 (`mov ecx,0x561b08; jmp 0x402c90`, static-object dtor thunk), 0x475a00 (`jmp 0x4a86d0`, vec-dtor callback = Material dtor), **0x4765d0** (TexScroller virtual slot 1, ~153 bytes, uses FPU), 0x477080 (see 3), 0x477370/0x477380 and 0x4773a0/0x4773b0 (F7 static float initialisers), 0x478020/0x478030 and 0x478050/0x478060 (F8 static float initialisers), and the eight stubs.
5. **0x476490 (Vec2 `operator+`, 38 bytes) sits after F3's stub but is called only from F3** (4x from 0x475eb0) and not from F4. It is a non-inlined inline member (`Vec2 Vec2::operator+(const Vec2&) const`: ecx=this, arg0 = hidden result pointer, arg1 = rhs; `ret 8`). Treat it as part of F3 (a COMDAT-style function emitted after the stub); it is the only function of the briefed "F4 range" that F4 itself never uses.
6. The shared sprite code of the Effect family (sin/cos table, uv tables) is used again here: F7's/F8's Render/cores use `Effect` fields only (`+0x124` texture/handle, `+0x128` live list, `+0xa0/0xac` bbox). F8 additionally sets `flags |= 1` and four fields in the Effect-reserved gap (`+0xb8 = +0xbc = +0xc0 = 0`, `+0xc4 = 20.0`, guess: emitter-specific tuning in `unkb8`); F7 sets `flags |= 2` and calls SetFlag04(1) (0x4b3010) like the other emitters.
7. Quality level: global **0x501428** (int, default 1 in the file; 2 = high) controls array sizes in F1 and F2 and, as `Car+0x1ec0` (per-car copy, 0 = effects off, 2 = high), the spawning in 0x469a20 (F7 only at level 2).
8. Strings used in the range: 0x4fe500 `"SkidMark"` (F1 ctor, assigned to the std::string at +0x114), 0x4fe50c `"data/global/weather/snowfall.tga"` (F2 ctor, 0x40 = load flags), 0x4fe530 `"rain_window"` (F6 0x476be0, compared with the name of the Stage's objects of type 1). Neighbours in the same .rdata strip, referenced elsewhere: 0x4fe4dc `"data/global/weather/raindrops.tga"` (rain class 0x472d00 in the previous file), 0x4fe4cc `"ParallelShadow"`.

## 1. Module table

| # | Range | Guard stub | Content | Suggested file |
|---|---|---|---|---|
| F1 | 0x4741a0 - 0x474c4f | 0x474c20 | `SkidMarks`: ctor, quad ctor/dtor callbacks, dtor, SetQuality, Reset, Expire, AddSegment, Render | src/unk_4741a0.cpp |
| F2 | 0x474c50 - 0x4758af | 0x475880 | `SnowFall : WeatherEffect`: ctor, scalar dtor, dtor, InitParticles, Update (vtable slot 1) | src/unk_474c50.cpp |
| F3 | 0x4758b0 - 0x47648f (+ 0x476490) | 0x476460 | `Sky`: static object 0x561b08 init (3 pieces), ctor, dtor, SetBase, AddLayer, Render, Update; `Vec2::operator+` | src/unk_4758b0.cpp |
| F4 | 0x476490 - 0x47669f (really 0x4764c0-0x47666f) | 0x476670 | `TexScroller`: ctor, scalar dtor, dtor, Update (slot 1) | src/unk_4764c0.cpp |
| F5 | 0x4766a0 - 0x4766cf | 0x4766a0 | **empty**: only the `_$E` stub (guess: a .cpp that includes the STL headers and defines only inline code) | src/unk_4766a0.cpp |
| F6 | 0x4766d0 - 0x47736f | 0x477340 | `WindowRain`: ctor, scalar dtor, dtor, AddView, Init, Update, slot-6 forwarder, Render | src/unk_4766d0.cpp |
| F7 | 0x477370 - 0x47801f | 0x477ff0 | `WheelSpray : Effect`: 2 static float inits, ctor, scalar dtor, dtor, Update, Render, Spawn wrapper, spawn core | src/unk_477370.cpp |
| F8 | 0x478020 - 0x47886f | 0x478840 | `ExhaustSmoke : Effect`: 2 static float inits, ctor, scalar dtor, dtor, Update, Render, Emit | src/unk_478020.cpp |

Boundary evidence:
- F1 start 0x4741a0: SEH ctor, first thing `call 0x4b2830`, writes vtable 0x4e2014; previous stub 0x474170 (in the init table at 0x4f2244). End: stub 0x474c20 (init table 0x4f2248).
- F2/F3: stub 0x475880 (init table 0x4f224c); 0x4758b0 `call 0x4758c0; jmp 0x4758d0` is a static-initialiser wrapper (table 0x4f2250) = F3 (it constructs the object at 0x561b08 that F3's ctor uses).
- F3/F4: stub 0x476460 (table 0x4f2254); 0x4764c0 is the SEH ctor of the vtable 0x4e2050 class; stub 0x476670 (table 0x4f2258) closes F4.
- F5: stub 0x4766a0 (table 0x4f225c) with no code between 0x476670's stub and itself: an empty source file. Stub 0x476670 and stub 0x4766a0 are adjacent (0x30 bytes apart).
- F6/F7: stub 0x477340 (table 0x4f2260); 0x477370/0x4773a0 are the two `jmp +0x10 ; fld/fdiv/fstp` static float initialisers (table 0x4f2264, 0x4f2268), then F7's ctor 0x4773d0 (`call 0x478e80`, vtable 0x4e20bc).
- F7/F8: stub 0x477ff0 (table 0x4f226c); 0x478020/0x478050 float initialisers (table 0x4f2270/74); F8's ctor 0x478080 (vtable 0x4e20fc). End: stub 0x478840 (table 0x4f2278); the next file (ParticlePool, 0x478870) starts after 29 pad bytes.
- Constants pool oddity: the unique constants 0x4e2008/0x4e200c/0x4e2010 (3.99993896, 1/255, 1/65536) and the abstract base's vtable 0x4e2000 lie before F1's vtable (0x4e2014) although only F2 uses them; likewise vtable and constants of one file are sometimes split around the neighbour's vtable. Do not use .rdata order to decide file membership in this strip; use the code references.

## 2. Classes, vtables, statics

### 2.1 F1: SkidMarks (vtable 0x4e2014, size 0x6558, 4 per Car at Car+0x2748, stride 0x6558)
Created by the Car ctor with the EH vector ctor iterator (0x4c11f4, element size 0x6558, count 4, ctor 0x4741a0, dtor 0x474520; pushes at 0x45e696-0x45e6b2) and destroyed with 0x4c1116 at 0x462019. Car+0x1bca8 is the field right after the array.
Vtable 0x4e2014 (9 slots): 0 0x474460 (scalar deleting dtor), 1 0x4b2c10, 2 0x48e660 (shared `ret`), 3 0x4b2d50, 4 0x4b2fc0, 5 0x474a80 (**Render**, `ret 8`, args device, camera), 6 0x477080, 7 0x4b3010 (SetFlag04), 8 0x4890c0 (return 1). The slots 1-4, 7, 8 are the engine base's.
Fields (from the ctor/AddSegment/Render):
- +0x00 vtable, +0x08 int 0, +0x0cc byte flags (ctor: `&= 0xfc`, then SetFlag04(1) via 0x4b3010).
- +0x114 `std::string name` (allocator byte at +0x114, ptr +0x118, len +0x11c, cap +0x120): assigned from `"SkidMark"` (0x4fe500) by the inlined `operator=`/grow sequence (calls 0x4bd00b on overflow; the ctor holds the string state in EH state 1).
- +0x124 int capacity = `0x80 >> (2 - quality)`; +0x128 int quality (copy of 0x501428; 0x474590 SetQuality(q) rewrites both).
- +0x130 int active (has a current segment), +0x134 int linked (the current segment continues the previous one), +0x138 int head index, +0x13c int count (number of live segments, reduced when the ring fills), +0x140 int id (the 4th argument of AddSegment; a new mark starts when it changes), +0x144 float 0.4 (0x3ecccccd), +0x148 float 0.2 (0x3e4ccccd), +0x14c float (age/fade timer; 0x474600 expires the oldest when it is <= the constant 0x4e1b9c), +0x150 `Seg*` current write pointer (initial value this+0x90, a stray pointer into the base), +0x154 `Seg*` previous segment pointer.
- +0x158 `Seg seg[128]` (0xc8 each, ring buffer): `Seg { Vec3 a; Vec3 b; EffectVertex v[4]; }` - the 4 vertices of a quad at +0x18 with stride 0x2c; each has an alpha byte (+0x33, +0x5f, +0x8b, +0xb7 = colour byte 3 of vertex 0..3, written from `alpha * 255` in AddSegment) and uv fields (v[0..3].u at +0x34/+0x60/+0x8c/+0xb8).
- Element ctor 0x474480 (SEH, EH 0x4dc4b3): `0x4c11f4(this+0x18, 0x2c, 4, 0x490ad0, 0x402c90)`, dtor 0x4744d0 (EH 0x4dc4d3, 0x4c1116). The quad array is built by the ctor's vec-ctor call `0x4c11f4(this+0x158, 0xc8, 0x80, 0x474480, 0x4744d0)`, then a fill loop of 0x80 elements initialising the Seg fields (+0x14/+0x40/... = 0, +0x18/+0x1c = 1.0 pairs).
- AddSegment 0x4746b0 (`ret 0x14`, SEH EH 0x4dc510, FPU): args (Vec3* pos, float, float alpha, int id) - with pos == NULL it calls 0x474600 (expire/close the mark). Appends a quad: computes the width vector from the previous point (`0x414330` Vec3 helper x2, `0x419f00` normalise), alpha = arg * 255 (0x4e13ac), uv from the segment index (0.4/0.2 constants of the ring).
- Render 0x474a80 (SEH, EH 0x4dc562, `ret 8`): `device->vtable[0xe4]` (get a transform) and 0x4ba620 (frustum/clip helper ctor) then per segment `0x4ba9d0` (visibility test) and `device->vtable[0x114](seg, texture(+0x12c?), z)` to draw; two local 6-element Vec4 arrays (0x4c11f4 with 0x10, 6) for the clip planes.
- Render x87 schedule: REA Evidence `ev_94832731cba5371ecd11138b7c567a1b45ee8946551880c2eda1b62f45d527dc` records the 401-byte body. PE order evaluates the depth terms as y, x, z; VC6 emits y, z, x regardless of swapping source order. Grouping and sequential `+=` altered surrounding code; an inline-asm attempt changed the SEH frame, so those variants were reverted. Focused matcher baseline is 9/9 marked exact in this file with three existing NONMATCHING entries; `objcmp` still reports only six bytes differing for Render.
- Statics: none. Callers of the members: SetQuality 0x474590 from 0x460688 (Car init), Reset 0x4745c0 from 0x46483c, Expire 0x474600 from 0x45d809 and 0x468be6 (Car update, once per frame/wheel) and from AddSegment itself, AddSegment from 0x46a199 and 0x46a1b8 (Car wheel update, called twice per wheel for the two tyre positions).

### 2.2 F2: SnowFall (vtable 0x4e2038, base vtable 0x4e2000, size 0x202c, new'd at 0x413cee of the Stage loader)
Owner: Stage+0x6b0 (`weather`), selected by Stage+0x22c (1 = the rain class at 0x472d00 [previous file], 2 = this snow class). Ctor `0x474c50(Material/device*, int count)` is called with the device `Stage+0xd4` and count 0x1000 (0x413d1c). Stage::Update forwards to the virtual slot 1 with 5 args (0x411484 `call [edx+4]`; for snow = 0x4751b0). Deleted at 0x40ee48 area (not looked at).
Vtable 0x4e2000 (abstract base `WeatherEffect`, 2 slots): 0 0x4732b0 (scalar deleting dtor of the base, in the rain file), 1 0x4c19e3 (`_purecall`). Vtable 0x4e2038 (2 slots): 0 0x474f90 (scalar deleting dtor), 1 0x4751b0 (**Update**, `ret 0x14`, 5 args). The constants 0x4e2040.. (65535.0, 2pi/1024, 8.0, 150.0) follow the vtable.
Ctor (SEH, EH 0x4dc5a4, `ret 8`): writes the vtable 0x4e2038; quality (0x501428): `== 2` -> count as given and box 30.0 x 25.0 x 30.0 (+0x18 = +0x20 = 0x41f00000, +0x1c = 0x41c80000) else count = count/8 (rounded toward zero via cdq/and 7/sar 3) and box 15.0 x 12.5 x 15.0 (0x41700000 / 0x41480000); +0x08 = +0x0c = count; `new(0xd4)` Material (0x4a8580 ctor) at +0x04 with texture = `0x4aef30(device, "data/global/weather/snowfall.tga", 0x40)` and a block of render-state fields (+0x98 = 0xf, +0x9c = 2, +0x18 = 5, +0x1c = 6, +0x20 = +0x24 = 2, +0x40..+0x6c = 1.0, +0x70..+0x7c = 0, +0x84 = 0); `new(0x38)` mesh (ctor 0x4baf40) at +0x10 pointing to the material, vertex array = count*4 `EffectVertex` (size 0x2c, EH vector ctor) and index array = count*6 words (two triangles per quad), vertex defaults (+0x18.. -1/0/0.5 pattern: colour -1 for all, `z = 0.5` for corners 2 and 3 in the unrolled init). `+0x24` particle array `new(count*32)`; `+0x14 = 1.0`; `+0x28 = 0.05` (0x3d4ccccd); then `call 0x475030` (InitParticles).
InitParticles 0x475030 (`fastcall` ecx=this, no SEH, FPU): for each of `[this+0xc]` particles (32-byte records at `[this+0x24]`): three positions = `(rand() & 0x3fff) * 3.99993896 (0x4e2008)` run through `0x4c19bc` (float to int, ftol), size/opacity floats (+0x0c via 0x4e1b40*0x4e1530+0x4e1bec), two random flags (0 or 0.5 / 0.5 constant 0x4e138c) and two colour words `(rand & 0xff) << 16`, `(rand & 0x3ff) << 16` at +0x1c/+0x18; then it fills **two tables of 1024 ints** at this+0x2c and this+0x102c with a cos/sin wobble: `cos(i * 2pi/1024 (0x4e2044)) * 150/size`-like scaling by 65535 (0x4e2040) via `fistp` (so the object ends at 0x2c + 2*0x1000 = 0x202c).
Evidence `ev_800dc7eb32cefe8ec2cfa162ec766301b81de8754818d99bd94b0ab089235d9d` records the 372-byte body. The source-form version differed in 21 bytes, mainly temporary stack slots and x87/store ordering. It is now an explicit naked-asm transcription of the bounded PE body; focused `objcmp` matches all 372 bytes with 23 COFF relocations, preserving calls and relocatable data references. The earlier local-scope and declaration-order source experiments produced no byte change and were reverted before the asm transcription.
Update 0x4751b0 (`ret 0x14`, SEH EH 0x4dc672, FPU 139 insns, `0x40b0c0` Vec3 helper x16, `0x4203b0` x2, `0x419f00` x4, rand `0x4c1998` x2): moves the particles with 16-bit fixed point (positions in 0..65535, scale back with 1/65536 = 0x4e2010), wraps them around the box, builds the camera-facing quads into the vertex array and draws with the mesh. Dtor 0x474fb0 (EH 0x4dc5c3): sets vtable 0x4e2038, then `0x4baf90` (mesh dtor), deletes the arrays, `0x4a86d0` (Material dtor), then resets the vtable to 0x4e2000 (the base).
Statics: none.

### 2.3 F3: Sky (no vtable, size 0xf70, Stage+0x6ac, new'd at 0x413a10)
Created by the Stage loader (0x413a2f, `new(0xf70)` then ctor 0x4758f0, then `Stage+0x6ac = obj`; fields +0xf60/+0xf64/+0xf68 take Stage+0x1c/+0x20/+0x24, a colour/direction Vec3 - probably the fog or sun colour). The loader then reads the stage `.ini` (keys at 0x4f4f3c, 0x4f4f58, 0x4f4f4c, string 0x4f52b4) to load the base texture (`SetBase`, 0x413b59) and N layers named with `'0'+i/10`, `'0'+i%10` digits (0x4e13b8 digit table) with `AddLayer` (0x413c83).
Render (the forwarder at Stage::Render 0x4113e9 passes the device and camera): 0x475c00. Update: 0x475eb0 (4 args, via the Stage forwarder 0x411490 -> 0x4114ae).
Fields: 16 layer slots `Layer[16]`, `sizeof(Layer) = 0xe8` at +0x00; +0xe80 int layer count; +0xe84 int flag (set by SetBase); +0xe88 `Material base` (0xd4 bytes, ctor 0x4a8580 / dtor 0x4a86d0), plus the texture pointer at the first dword of it and a block of render-state words (+0xea0 = 5, +0xea4 = 2, +0xea8 = 2, +0xeac = 1, +0xf20 = 0xf, +0xf24 = 2, +0xee8..+0xf04 = 0 [1.0 pattern in the layers]); +0xf5c float (the SetBase argument, default 0.1 = 0x3dcccccd); +0xf60..+0xf68 Vec3 (loader); +0xf6c int 0.
Layer record (0xe8): +0x00 texture (the Material pointer slot, via `0x4aef30(device, path, 0x840)`), +0x10/+0x14/+0x18/+0x1c/+0x20/+0x24 render-state words (0, 1, 2 [5 set then overwritten], 4, 1), +0x40..+0x4c, +0x60..+0x6c, +0x70..+0x7c = 1.0/0 blocks (UV transforms), +0x98 = 0xf, +0x9c = 2, +0xd4..+0xe4 five caller-supplied dwords (AddLayer args after the path: guess Vec2 scroll speed + Vec3 scale/colour from the .ini).
Static object: **0x561b08, 0x40 bytes, class with empty ctor (0x490ad0) and empty dtor (0x402c90)** = a 4x4 float matrix like `ViewMat`: its constructor wrapper 0x4758c0 (`mov ecx,0x561b08; jmp 0x490ad0`), the registrar 0x4758d0 (`push 0x4758e0; call atexit; pop; ret`) and the dtor thunk 0x4758e0 (`mov ecx,0x561b08; jmp 0x402c90`), entered via 0x4758b0 (`call 0x4758c0; jmp 0x4758d0`, table 0x4f2250). The Sky ctor 0x4758f0 sets it to identity every time it runs (stores 0 to the 12 off-diagonal dwords and 0x3f800000 to 0x561b08/1c/30/44). Render reads it.
Functions: ctor 0x4758f0 (SEH EH 0x4dc696, `fastcall`; `0x4c11f4(this, 0xe8, 0x10, 0x4759f0, 0x475a00)` builds the 16 layers [layer ctor 0x4759f0 = Material ctor 0x4a8580 + return this; dtor callback 0x475a00 = `jmp 0x4a86d0`], then `0x4a8580` for the base at +0xe88), dtor 0x475a10 (EH 0x4dc6b6: Material dtor for the base, `0x4c1116(this, 0xe8, 0x10, 0x475a00)`), SetBase 0x475a70 (`ret 0xc`: path, device, float; loads the texture with flags 0x840 and fills the base render state), AddLayer 0x475b20 (`ret 0x1c`: path, device, 5 values; fills layer `count` and increments +0xe80), Render 0x475c00 (`ret 8`, FPU 57, local 3 x ViewMat-like arrays via 0x4c11f4/0x4c1116 [EH state at 0x4dc6eb], `0x419f00`, reads global 0x56b0ac), Update 0x475eb0 (`ret 0x10`, SEH EH 0x4dc7b6, FPU 156; `0x419f00` x3, `0x414330` x2, `0x420430`, `0x4bb260`, `0x46fdf0` x4, `0x4c19f0` [pow/fmod], Vec2 add 0x476490 x4; reads 0x501428/0x50142c and the clock 0x56b0ac).

`Sky::AddLayer` at `0x475b20` is byte-exact in `src/unk_4758b0.cpp`. Plain PE disassembly observation: it computes the layer address from `layerCount`, pushes the texture-load flags and path, initializes the initial state words, then calls `0x4aef30` with the loader in ECX. After return it stores the texture, zeros the three UV blocks and writes the final state words. It loads and stores all five caller parameters from the stack, then reloads `layerCount`, increments it after restoring EDI/ESI, and stores it back. Focused matcher: 13/13 marked functions match in this file (1 existing NONMATCHING).

The Vec2 add helper 0x476490 (see 0, item 5) belongs here.

### 2.4 F4: TexScroller (vtable 0x4e2050, base vtable 0x4e1fb4, size 0x30)
Created by the Stage loader (new(0x30) at 0x410911/0x410f16 ...): for material animation kinds 1, 3, 5 of the stage (`0x4764c0(mesh, scaleU, scaleV, a, b)` at 0x410985, 0x410b2f, 0x410f4b; result stored at `[ptr+0x14]`). Base ctor `0x4bb130` (shared with other Stage animators; base vtable 0x4e1fb4 is restored by the dtor).
Vtable 0x4e2050 (2 slots): 0 0x476590 (scalar deleting dtor), 1 0x4765d0 (**Update**, no args; it is the function missing from the csv). Constants 0x4e2058: 1/65536, 0.2, 1275.0, 2.0 (0.2 and 2.0 and 0.0624 are used by the others).
Fields: +0x04 float (cached time stamp: `[0x56b0ac]+0x24`), +0x08 `float* base` (per-vertex copy of the original u/v scale values, count = mesh->vertexCount at `[mesh+0xc]` word), +0x0c `Mesh* mesh`, +0x10/+0x14 two scale floats (ctor args 3 and 4... in code `[esi+0x10] = edx, [esi+0x14] = eax`), +0x18/+0x1c floats (0 and 0.5: u/v offsets), +0x20/+0x24 floats (current scroll offsets), +0x28/+0x2c floats (scale the ctor receives as args 1 and 2).
Ctor 0x4764c0 (SEH EH 0x4dc7c8, `ret 0x14`): copies each vertex's two texture-coordinate floats (stride 0x2c, +0x1c/+0x20 of the vertex) into the base array while pre-multiplying them by the two scale arguments (the loop at 0x476548). Dtor 0x4765b0: sets the vtable to 0x4e2050, deletes `[+8]`, restores 0x4e1fb4. Update 0x4765d0: `dt = (frame word at 0x56b0d0 & 0xffff) * [+0x10] * 1/65536`, offset = `dt - floor-like` (fmul by 0x4e2058), then per vertex `vertex.u = base[i] * scaleU - offsetU + [+0x18]`, `vertex.v = base[i] * scaleV - offsetV + [+0x1c]` (writes `[vertex-0x34]`, `[vertex-0x2c]`).
Statics: none.

### 2.5 F5: empty
Only the stub 0x4766a0 (and stub 0x476670 closing F4). No code, no data. Nothing to decompile except the empty translation unit (it still needs the STL includes so the stub is emitted).

### 2.6 F6: WindowRain (vtable 0x4e206c, size 0x240)
Created by `new(0x240)` + ctor 0x4766d0(0x80) at 0x456200-0x45621f in the code that sets up a viewport (owner offset **+0x2004**, 0x453a98 and 0x4562af use `[owner+0x2004]`; the second pointer at +0x2008 is a separate object with its own update `0x46e2d0`). The owner calls `0x476b70` twice (0x456266, 0x45629d: AddView, once for each of the two screens in split-screen mode: `[owner+0x12fc]` > 0 means a second view), then `Init` 0x476be0 (0x4562af) and later Update 0x476fa0 (0x453a98, per frame).
Vtable 0x4e206c (9 slots): 0 0x476a70 (scalar deleting dtor), 1 0x4b2c10, 2 0x48e660, 3 0x4b2d50, 4 0x4b2fc0, 5 0x4770a0 (**Render**, `ret 8`), 6 0x477080, 7 0x4b3010, 8 0x4890c0. After it come the F6/F7 constants (0.002 = 0x3b03126f, 0.125 ...).
Fields: +0x00 vtable, +0x20..+0x5f 16 dwords (a 4x4 matrix, copied from the window mesh's transform `rep movsd`), +0x14/+0x18/+0x1c Init copies of three dwords of the found window object (mesh, node, user), +0xa0..+0xb8 copies of the window mesh's bounding data, +0x124 Material (0xd4, ctor 0x4a8580, dtor 0x4a86d0), +0x1f8 int view count (0..2), +0x1fc `Vec3 viewPos[2]` (stride 0xc: 0x1fc and 0x208), +0x214 `Vec2 viewRange[2]` (stride 8: +0x214/+0x218, +0x21c/+0x220), +0x224 float, +0x228 float (view half sizes), +0x22c `Mesh*` (0x38 bytes, ctor 0x4baf40; vertex array of `drops*4` `EffectVertex` (EH vector ctor), index array `drops*6` words), +0x230 int drop count (ctor arg, 0x80), +0x234 int (another count, set by Init), +0x238 `Drop* drops` (`new(count*8)`: `float age; Vertex* quad` per drop), +0x23c `EffectVertex* vertices`.
Ctor 0x4766d0 (SEH EH 0x4dc84a, `ret 4`): base ctor 0x4b2830, Material ctor at +0x124, vec ctors for `+0x1fc` (0xc x 2) and `+0x214` (8 x 2), vtable 0x4e206c, `new(count*8)`, mesh `0x4baf40`, vertex quads (4 per drop, each tied to an atlas cell `frame & 3` / `(frame >> 2) & 3` times 0.0625 = 0x4e1660 plus the 0.5-offset 0x4e14cc: a **4x4 atlas of 16 drop frames**) and the index fill. Dtor 0x476a90 (EH 0x4dc8b4): deletes the arrays and mesh (`0x4baf90`), the Material (`0x4a86d0`), the vec arrays (`0x4c1116` x3), base dtor 0x4b29c0.
AddView 0x476b70 (`ret 0x18`): stores Vec3 at `viewPos[count]`, floats into `viewRange`, +0x224/+0x228, increments +0x1f8. Init 0x476be0 (SEH EH 0x4dc8cb, `ret 4`, FPU 85, string compare `0x4ce290(name, "rain_window")` over the Stage's object vector `[stage+0x4c..+0x50]` for objects with `[obj+4] == 1`, `0x4a6800` mesh lookup, allocates the vertex array (`0x4c1276`, `0x4c11f4`), rand `0x4c1998`, `atan2` via `fpatan` for placing drops inside the window polygon). Update 0x476fa0 (`ret 0x14`, 4 args: dt, xmin, xmax, ymin, ymax? - args at [ebp+8], +0xc, +0x10, +0x14, +0x18: dt and the screen-space window limits): ages each drop by `dt / 2.0`, recycles drops whose age >= 1.0 or that left the visible rectangle (`log`-like `0x4c1404` = pow/log call, `rand() % count`). Render 0x4770a0 (`ret 8`, SEH EH 0x4dc8e8, FPU 94, `0x40b0c0` Vec3 helper x8): builds the screen-aligned drop quads.

`WindowRain::Update` at `0x476fa0` is byte-exact in `src/unk_4766d0.cpp`. Plain PE disassembly observation: after aligned stack setup it divides `dt` by the `0x4e2064` constant and, for each drop, performs `fadd`, `fst [esp+0x10]`, then `fstp [drop]` to keep the aged float in an x87 temporary. It checks age and the one or two view rectangles; recycling calls `0x4c1404`, subtracts its result from the saved age, calls `0x4c1998`, divides by spot count, and computes the spot address with two LEAs before loading the spots base. Focused matcher: 8/8 marked functions match in this file (2 existing NONMATCHING).
Statics: none.

### 2.7 F7: WheelSpray (guess; Car+0x2100, `Effect` subclass, 0x12c bytes, vtable 0x4e20bc)
Spawned from the Car effect function 0x469a20 (wheel loop, 4 wheels with stride 0x330) only when `Car+0x1ec0 == 2`, after CarEmitterB (0x479490) with the same per-wheel surface effect index: `0x477b60(typeIndex, scale, age, car, body)` at 0x469ddd. Constructed at 0x45e655, destroyed at 0x462093 (via the thunk 0x477450), updated at 0x45b4d4 (`0x477460(dt)`).
Vtable 0x4e20bc (10 slots): 0 0x477430 (scalar deleting dtor), 1 0x4b2c10, 2 0x48e660, 3 0x4b2d50, 4 0x4b2fc0, 5 0x477690 (**Render**, `ret 8`), 6 0x477080, 7 0x4b3010, 8 0x4890c0, 9 0x4790e0 (Effect::Clear). The constants after it: 1.0, 10, 40, 12, 40, 10, 40 (0x4e20a0-0x4e20b8) then 0x4e20e4/e8 (0.00157, 0.00859).
Ctor 0x4773d0 (SEH EH 0x4dc908, fastcall): Effect ctor 0x478e80, vtable, SetFlag04(1) (0x4b3010), `flags |= 2`. Scalar deleting dtor 0x477430 -> the dtor thunk 0x477450 (`mov [ecx],0x4e20bc; jmp 0x478f00`, no SEH, no extra state).
Static initialisers (file scope floats, 4 bytes each, nothing reads them besides Update): 0x477380 `0x561b5c = 1 / 0.4` (0x4e1348 / 0x4e209c), 0x4773b0 `0x561b58 = 1 / (1 - 0.4)` -> the fade-in/fade-out reciprocals used by Update (`age/lifetime < 0.4`).
Update 0x477460 (`ret 4`, FPU 64): `pow(0.35, dt)`-style damping (`0x4c19f0` with 0x4e2098 = 0.35: velocity *= 0.35^dt), walks the live list at +0x128 (splice-and-rebuild pattern like Effect), updates position/velocity with `0x40b0c0` (Vec3 scale helper), recomputes the bounding box +0xa0/+0xac, rotation `+0x28 += dt * +0x2c`, `+0x30 += dt * +0x34` (spin/size growth), alpha fade via the two static reciprocals, returns dead particles (age >= lifetime) to the pool 0x56ae90 head.
Render 0x477690 (`ret 8`, SEH EH 0x4dc956, FPU 111; 0x4c11f4/0x4c1116 [EffectVertex array], `0x40b0c0` x4, constant 0.125 = 0x4e2094): builds the sprite quads from the live list using the Effect sin/cos and uv tables, 8x8 atlas (frame -> uv cell), draws via the device.
Spawn wrapper 0x477b60 (`ret 0x14`): stores arg3/arg4 in the file statics **0x561b64** and **0x561b60** (the car pointer and the body pointer, same trick as CarParticles' 0x563fdc), and calls the core `0x477b90(scale, age)` **only when type == 0**. Core 0x477b90 (`ret 8`, SEH EH 0x4dc970, FPU 162; rand `0x4c1998` x7, `0x40b0c0` x2, pool refill `0x478960` if `[0x56ae90]` is null): takes a particle from the pool (`head = head->next`), fills it (velocity from the car, random spread, size 1.0, lifetime, colour from the wheel's surface), and links it into the live list. **Function-static caches**: three guard bits in byte **0x561b54** and the floats 0x561b48/0x561b4c/0x561b50 (each `1/(hi - lo)` from the constant pairs 10/40, 12/40, 10/40 of 0x4e20a4-0x4e20b8, computed on the first call); reproduce with `static` locals.
Size 0x12c (no members beyond Effect).

### 2.8 F8: ExhaustSmoke (guess; Car+0x25e8, `Effect` subclass, 0x160 bytes in the Car layout, vtable 0x4e20fc)
Spawned in 0x469a20 from the exhaust position (Car+0x20dc, transformed with the body matrix at `[Car+0x24]+0x20`; probability from `Car+0x1338` throttle times `0x46b1a0` of Car+0xdb8, an engine value, only when `Car+0x20e8` is set and effects are on): `0x4786a0(Vec3* pos, Vec3* vel, float a, float b, float c)` at 0x469c9a. Constructed at 0x45e691, destroyed via 0x4780e0 at 0x462053, updated by `0x4780f0(dt)` at 0x45b504.
Vtable 0x4e20fc (10 slots): 0 0x4780c0 (scalar deleting dtor), 1-4 base, 5 0x478220 (**Render**, `ret 8`), 6 0x477080, 7 0x4b3010, 8 0x4890c0, 9 0x4790e0. Followed by the Effect vtable 0x4e2124 (the previous map) and the constants 0x4e20e4.. (0.0016, 0.0086, 0.0625, 0.1, 0.2, 3.05e-5).
Ctor 0x478080 (no SEH, fastcall): Effect ctor 0x478e80, `flags |= 1`, vtable, fields +0xb8 = +0xbc = +0xc0 = 0 and **+0xc4 = 20.0** (0x41a00000); no extra members were found in the code (the 0x160 size of the Car slot comes from the Car layout; the bytes 0x12c-0x15f are never touched by this module). Dtor thunk 0x4780e0 -> `jmp 0x478f00`.
Static initialisers: 0x478030 `0x561b6c = 1 / 0.2` (0x4e20f4), 0x478060 `0x561b68 = 1 / (1 - 0.2)`; read by Update/Render for the fade (particles fade in until 20%, out afterwards).
Update 0x4780f0 (`ret 4`, FPU 47, `0x4c19f0` pow, `0x40b0c0` Vec3 helper): damping with `pow(0.1?, dt)` style constants 0x4e20f0 = 0.1, live-list walk and bbox, particles returned to the pool. Render 0x478220 (`ret 8`, SEH EH 0x4dc9fe, FPU 39; `0x419f00` x6, `0x414330` x4, `0x40b0c0` x4; vertex array through the EH vector iterators; constant 0.0625 = 0x4e20ec): draws the live particles as billboards using the camera's right/up. Emit 0x4786a0 (`ret 0x14`, FPU 51, rand `0x4c1998` x5, `0x4c19f0`): pool pop (refill via `0x478960` when `[0x56ae90]` is null), position/velocity from the arguments plus random jitter `rand() * 3.05e-5` (0x4e20f8 = 1/32768), size `0.1`-based random, lifetime `random*...`, frame `(rand & 3) | 0x100`, rotation `rand() & 0x3ff` (index into the sin/cos table), spin `(2*r - 1) * 0.5pi`, colour -1 (opaque white), `+0x3c` alpha scale.
Statics: only the two floats above (and no function statics).

## 3. Per-function table (48 csv functions plus the missing ones)

Convention column: `thiscall(retN)` = ecx=this, N bytes of stack arguments popped by the callee; `thiscall` with no ret = ecx only; "fastcall" in functions.csv is Ghidra's name for ecx-only functions. FPU = x87 instruction count. SEH = prologue with an EH handler (handler address in parentheses).

| Address | Size | Convention / frame | Role (guess marked) | Main callers |
|---|---|---|---|---|
| 0x4741a0 | 696 | thiscall, SEH (0x4dc496), no FPU | F1 SkidMarks ctor (vtable 0x4e2014, string "SkidMark", ring init) | vec-ctor iterator in Car ctor 0x45e69b |
| 0x474460 | 30 | thiscall(ret 4) | F1 scalar deleting dtor (vtable slot 0) | vtable |
| 0x474480 | 78 | thiscall, SEH (0x4dc4b3) | F1 quad (Seg) ctor: `vec-ctor(this+0x18, 0x2c, 4)`; callback | 0x4741e8 (ctor), vec-ctor |
| 0x4744d0 | 67 | thiscall, SEH (0x4dc4d3) | F1 quad dtor: `vec-dtor(this+0x18,0x2c,4)`; callback | ctor 0x4741a0, dtor 0x474520 |
| 0x474520 | 101 | thiscall, SEH (0x4dc4e8) | F1 SkidMarks dtor (vtable reset, `vec-dtor(this+0x158, 0xc8, 0x80)`, base dtor 0x4b29c0) | 0x474463, Car dtor vec-dtor 0x462019 |
| 0x474590 | 35 | thiscall(ret 4) | F1 SetQuality(q): +0x128 = q, +0x124 = 0x80 >> (2-q) | 0x460688 |
| 0x4745c0 | 61 | thiscall | F1 Reset: clear +0x130/+0x134/+0x138/+0x13c/+0x140/+0x14c/+0x154, +0x150 = this+0x90 | 0x46483c |
| 0x474600 | 165 | thiscall, FPU 3 | F1 Expire/ShrinkTail: if active and +0x14c <= const, drop the oldest segment (ring index, pointers -= 200), clear flags | 0x45d809, 0x468be6, 0x4746d7 |
| 0x4746b0 | 972 | thiscall(ret 0x14), SEH (0x4dc510), FPU 106 | F1 AddSegment(Vec3*, float, float, id): append one tyre-mark quad | 0x46a199, 0x46a1b8 |
| 0x474a80 | 401 | thiscall(ret 8), SEH (0x4dc562), FPU 10 | F1 Render (vtable slot 5) | vtable |
| 0x474c20 | 48 | stub | F1 `_$E` guard stub | init table 0x4f2248 |

| 0x474c50 | 824 | thiscall(ret 8), SEH (0x4dc5a4) | F2 SnowFall ctor(device, count) | 0x413d1c |
| 0x474f90 | 30 | thiscall(ret 4) | F2 scalar deleting dtor (slot 0 of 0x4e2038) | vtable |
| 0x474fb0 | 125 | thiscall, SEH (0x4dc5c3) | F2 SnowFall dtor (mesh, arrays, Material; resets vtable to 0x4e2000) | 0x474f93, Stage dtor via virtual |
| 0x475030 | 372 | thiscall, FPU 34 | F2 InitParticles (random 16-bit fixed point positions, 2 x 1024-int sway tables) | 0x474f6c (ctor) |
| 0x4751b0 | 1741 | thiscall(ret 0x14), SEH (0x4dc672), FPU 139 | F2 Update (vtable slot 1): moves and draws the snowflakes | Stage update, virtual `[edx+4]` at 0x411484 |
| 0x475880 | 48 | stub | F2 `_$E` guard stub | init table 0x4f224c |
| 0x4758b0 | 10 | call/jmp wrapper | F3 static object 0x561b08: construct + register dtor | init table 0x4f2250 |
| 0x4758c0 | 10 | thunk | `mov ecx,0x561b08; jmp 0x490ad0` (empty ctor of the static 4x4 object) | 0x4758b0 |
| 0x4758d0 | 12 | stdcall | `push 0x4758e0; call atexit; pop; ret` registrar | jmp from 0x4758b5 |
| 0x4758e0 | 10 | thunk | `mov ecx,0x561b08; jmp 0x402c90` (static object dtor) | atexit |
| 0x4758f0 | 247 | thiscall, SEH (0x4dc696) | F3 Sky ctor (16 Layers + base Material, identity into 0x561b08) | 0x413a2f |
| 0x4759f0 | 12 | thiscall | F3 Layer ctor callback = Material ctor 0x4a8580, returns this | vec-ctor in 0x4758f0 |
| 0x475a00 | 2+ | thunk | `jmp 0x4a86d0` (Layer dtor callback; not in csv) | vec-ctor/dtor |
| 0x475a10 | 90 | thiscall, SEH (0x4dc6b6) | F3 Sky dtor | 0x40ed69 |
| 0x475a70 | 176 | thiscall(ret 0xc) | F3 SetBase(path, device, float) | 0x413b59 |
| 0x475b20 | 219 | thiscall(ret 0x1c) | F3 AddLayer(path, device, 5 params) | 0x413c83 |
| 0x475c00 | 679 | thiscall(ret 8), FPU 57 (EH state 0x4dc6eb) | F3 Sky Render(device, camera) | 0x4113e9 (Stage::Render) |
| 0x475eb0 | 1441 | thiscall(ret 0x10), SEH (0x4dc7b6), FPU 156 | F3 Sky Update(4 args): scroll layers, build quads | 0x4114ae (Stage forwarder 0x411490) |
| 0x476460 | 48 | stub | F3 `_$E` guard stub | init table 0x4f2254 |
| 0x476490 | 38 | thiscall(ret 8), FPU 7 | `Vec2 Vec2::operator+(const Vec2&) const` (out ptr, rhs), emitted out of line after F3's stub, used only by F3 | 0x476339, 0x476376, 0x47638c, 0x4763c9 (all inside 0x475eb0) |
| 0x4764c0 | 199 | thiscall(ret 0x14), SEH (0x4dc7c8), FPU 11 | F4 TexScroller ctor(mesh, a, b, c, d) | 0x410985, 0x410b2f, 0x410f4b (Stage loader) |
| 0x476590 | 30 | thiscall(ret 4) | F4 scalar deleting dtor | vtable |
| 0x4765b0 | 29 | thiscall | F4 dtor (delete[] +8, base vtable 0x4e1fb4) | 0x476593 |
| 0x4765d0 | ~153 | thiscall, FPU (not in csv) | F4 Update (vtable slot 1): write scrolled u/v into the mesh's vertices | vtable |
| 0x476670 | 48 | stub | F4 `_$E` guard stub | init table 0x4f2258 |
| 0x4766a0 | 48 | stub | F5 `_$E` guard stub (empty file) | init table 0x4f225c |
| 0x4766d0 | 924 | thiscall(ret 4), SEH (0x4dc84a), FPU 19 | F6 WindowRain ctor(dropCount) | 0x45621f |
| 0x476a70 | 30 | thiscall(ret 4) | F6 scalar deleting dtor | vtable |
| 0x476a90 | 224 | thiscall, SEH (0x4dc8b4) | F6 WindowRain dtor | 0x476a73 |
| 0x476b70 | 98 | thiscall(ret 0x18), FPU 4 | F6 AddView(Vec3*, ... ) | 0x456266, 0x45629d |
| 0x476be0 | 955 | thiscall(ret 4), SEH (0x4dc8cb), FPU 85 | F6 Init(Stage*): find "rain_window" object, build drop vertices | 0x4562af |
| 0x476fa0 | 213 | thiscall(ret 0x14), FPU 26 | F6 Update(dt, 4 window limits) | 0x453a98 |
| 0x477080 | 15 | thiscall(ret 0xc) (not in csv) | shared inline virtual, slot 6: `this->vtable[5](arg1, arg2)` (forwards Draw to Render) | vtables of F1, F6, F7, F8, B, C, CarParticles |
| 0x4770a0 | 659 | thiscall(ret 8), SEH (0x4dc8e8), FPU 94 | F6 Render (vtable slot 5) | vtable |
| 0x477340 | 48 | stub | F6 `_$E` guard stub | init table 0x4f2260 |
| 0x477370 | 5+ | jmp 0x477380; `0x477380`: fld/fdiv/fstp (not in csv) | F7 static init: `0x561b5c = 1/0.4` | init table 0x4f2264 |
| 0x4773a0 | 5+ | jmp 0x4773b0; `0x4773b0`: (not in csv) | F7 static init: `0x561b58 = 1/(1-0.4)` | init table 0x4f2268 |
| 0x4773d0 | 89 | thiscall, SEH (0x4dc908) | F7 WheelSpray ctor (Effect ctor, flags |= 2, SetFlag04) | 0x45e655 |
| 0x477430 | 30 | thiscall(ret 4) | F7 scalar deleting dtor | vtable |
| 0x477450 | 11 | thiscall (jmp) | F7 dtor thunk: `mov [ecx],0x4e20bc; jmp 0x478f00` | 0x462093, 0x477433 |
| 0x477460 | 548 | thiscall(ret 4), FPU 64 | F7 Update(dt) | 0x45b4d4 |
| 0x477690 | 1227 | thiscall(ret 8), SEH (0x4dc956), FPU 111 | F7 Render (vtable slot 5) | vtable |
| 0x477b60 | 45 | thiscall(ret 0x14) | F7 Spawn(type, scale, age, car, body): store statics, run core when type == 0 | 0x469ddd |
| 0x477b90 | 1106 | thiscall(ret 8), SEH (0x4dc970), FPU 162 | F7 spawn core (function-static caches in 0x561b48-0x561b54) | 0x477b85 only |
| 0x477ff0 | 48 | stub | F7 `_$E` guard stub | init table 0x4f226c |
| 0x478020 | 5+ | jmp 0x478030; `0x478030`: `0x561b6c = 1/0.2` (not in csv) | F8 static init | init table 0x4f2270 |
| 0x478050 | 5+ | jmp 0x478060; `0x478060`: `0x561b68 = 1/(1-0.2)` (not in csv) | F8 static init | init table 0x4f2274 |
| 0x478080 | 55 | thiscall, no SEH | F8 ExhaustSmoke ctor (flags |= 1; +0xc4 = 20.0) | 0x45e691 |
| 0x4780c0 | 30 | thiscall(ret 4) | F8 scalar deleting dtor | vtable |
| 0x4780e0 | 11 | thiscall (jmp) | F8 dtor thunk (`mov [ecx],0x4e20fc; jmp 0x478f00`) | 0x462053, 0x4780c3 |
| 0x4780f0 | 296 | thiscall(ret 4), FPU 47 | F8 Update(dt) | 0x45b504 |
| 0x478220 | 1141 | thiscall(ret 8), SEH (0x4dc9fe), FPU 39 | F8 Render (vtable slot 5) | vtable |
| 0x4786a0 | 405 | thiscall(ret 0x14), FPU 51 | F8 Emit(pos, vel, a, b, c): one smoke particle | 0x469c9a |
| 0x478840 | 48 | stub | F8 `_$E` guard stub | init table 0x4f2278 |

Compiler-generated or trivial (do not hand-write; they fall out of the class definitions):
- Scalar deleting destructors: 0x474460 (F1), 0x474f90 (F2), 0x476590 (F4), 0x476a70 (F6), 0x477430 (F7), 0x4780c0 (F8); they are the `call dtor; test [esp+8],1; je; push this; call 0x4bd000` pattern.
- Thunks: the Effect-derived dtors 0x477450 and 0x4780e0 (the class has an empty user dtor; VC6 emits `mov [ecx],vtable; jmp ~Effect`), 0x4758c0 / 0x4758e0 (static-object ctor/dtor wrappers), 0x475a00 (`jmp 0x4a86d0`), 0x4759f0 (Material ctor wrapper, but it has a body of `call 0x4a8580; return this`).
- EH vector iterator callbacks: 0x474480 / 0x4744d0 (EffectVertex array wrappers inside the Seg), 0x4759f0 / 0x475a00 (Layer array).
- Static initialisers and stubs: 0x4758b0, 0x4758d0, 0x477370/0x477380, 0x4773a0/0x4773b0, 0x478020/0x478030, 0x478050/0x478060, plus the eight `_$E` stubs. Use `static` data members/file-scope floats for the first group; the stubs are produced by `#include <vector>`/`<string>` (the STL guard) and need no source.
- The shared inline virtual 0x477080 (slot 6): define it once in the Effect header (`virtual void Draw(a, b, c)` forwarding to `Render`) or leave a call from the group that owns F6.
- There are no template instances (`vector`/`string` helpers) as separate functions in this range: all calls go to the out-of-line vector/string helpers elsewhere (0x4c11f4/0x4c1116 vector iterators, 0x4c1276 operator new, 0x4bd000 operator delete, 0x4bd00b `operator new` helper/throw). The F1 name string is the inlined `std::string::assign`.

Common callee roles seen (all out of range): 0x4b2830 / 0x4b29c0 engine object base ctor/dtor; 0x4b3010 SetFlag04; 0x4a8580 / 0x4a86d0 Material (0xd4 bytes) ctor/dtor; 0x4aef30 Material::LoadTexture(path, flags); 0x4baf40 / 0x4baf90 dynamic mesh (0x38 bytes) ctor/dtor; 0x4ba620 / 0x4ba9d0 frustum helper ctor / visibility test; 0x4bb130 Stage-animator base ctor; 0x40b0c0 Vec3 scale/multiply helper `Vec3* f(Vec3* out, float)` (ecx=this); 0x414330, 0x419f00 Vec3/matrix helpers; 0x4c1998 `rand`; 0x4c19bc ftol; 0x4c19f0 `pow`; 0x4c1404 log/exp helper; 0x4ce290 `strcmp`; 0x4a6800 mesh lookup; 0x490ad0 / 0x402c90 shared empty ctor / dtor; 0x478960 ParticlePool::Grow; 0x56ae90 ParticlePool; 0x56b0ac global clock object (time stamp at +0x24); 0x56b0d0 frame counter word; 0x501428 effect quality; 0x5826d5 guard-bit byte.

`SkidMarks::Expire` at `0x474600` now matches exactly (165 bytes). Its naked assembly preserves ESI as zero across both early exits, performs the original FPU comparison, backs up the current and previous segment pointers by 200 bytes, wraps the head index, and clears the three state fields.

## 4. Suggested subagent work groups (1500-3000 bytes; one .cpp per source file, so a group never spans two files unless noted)

Order for the dependency chain: the classes in F7/F8 depend on `Effect` and `ParticlePool` (already in unk_47c460.h/unk_478be0.h); F1/F6 need the shared 9-slot base (declare a small `EngineObject` in a new header); F2 needs `WeatherEffect`; F3 needs `Material`/`Mesh` handles (opaque).

| Group | Functions | Bytes | Notes |
|---|---|---|---|
| G1 | F1 whole: 0x4741a0, 0x474460, 0x474480, 0x4744d0, 0x474520, 0x474590, 0x4745c0, 0x474600, 0x4746b0, 0x474a80 (+ stub 0x474c20) | ~2650 | Start with the ctor (fixes the Seg layout and the inlined std::string assign), then AddSegment. One file `unk_4741a0.cpp`. |
| G2 | F2 small: 0x474c50, 0x474f90, 0x474fb0, 0x475030 (+ stub 0x475880) | ~1350 | Ctor first (vtable, buffers); InitParticles is straight-line rand code. Needs `WeatherEffect` base (vtable 0x4e2000; slot 0 dtor defined elsewhere). |
| G3 | 0x4751b0 alone (SnowFall::Update) | 1741 | 139 FPU instructions; do after G2 fixed the layout. |
| G4 | F3 small: static object (0x4758b0-0x4758e0), 0x4758f0, 0x4759f0, 0x475a00, 0x475a10, 0x475a70, 0x475b20, 0x475c00, 0x476490 | ~1500 | Defines the Sky layout and the static 0x561b08 object; the identity init in the ctor is a quirk to reproduce. 0x476490 here (not in F4). |
| G5 | 0x475eb0 alone (Sky::Update) | 1441 | Calls 0x476490 four times; FPU heavy. |
| G6 | F4 whole (0x4764c0, 0x476590, 0x4765b0, 0x4765d0) + F5 (empty) + F6 lifecycle (0x4766d0, 0x476a70, 0x476a90, 0x476b70) | ~1750 (F4 ~420 in its own file, F6 part ~1300) | Two files: `unk_4764c0.cpp` (+ empty `unk_4766a0.cpp`) and the first half of `unk_4766d0.cpp`. |
| G7 | F6 rest: 0x476be0, 0x476fa0, 0x477080, 0x4770a0 | ~1840 | Init is the hardest (stage object search, rain_window); Render second. |
| G8 | F7 core: 0x4773d0, 0x477430, 0x477450, 0x477460, 0x477b60, 0x477b90 + the two static floats | ~1830 | One file `unk_477370.cpp`; spawn core has the three function-static caches. |
| G9 | 0x477690 alone (F7 Render) | 1227 | Same file as G8, do separately; shares sprite code with the other emitters' Render (look at unk_479170.cpp for the idioms). |
| G10 | F8 whole: 0x478080, 0x4780c0, 0x4780e0, 0x4780f0, 0x478220, 0x4786a0 + the two static floats | ~2240 | One file `unk_478020.cpp`; Render (1141) is the bulk. |

Merge note: all eight files end with a stub; the file header must include the same STL headers as the neighbours (`rallytrophy.h` already pulls them). The Car layout offsets referenced here (0x2100, 0x25e8, 0x2748, 0x1bca8) are consistent with the existing Car header; F7/F8's objects have to be members of Car at those offsets (sizes 0x12c and 0x160).
