<!-- Subagent analysis, 2026-10-23; read-only disassembly (rawdis.py/refs.py, Patch 1.01 exe); no REA. "Guess" marks inference. -->
# Range 0x46d830 - 0x46fdef: module map (five source files: viewport body rain, an empty file, car light glow sprites, city light flares, water animator)

Method: rawdis.py over the whole range, caller index with refs.py, .rdata strings/floats/vtables read from the PE. Sizes are from analysis/functions.csv; items the csv lacks are marked "(not in csv)". EH handler addresses (0x4dbe67 ... 0x4dc110) are the `push imm` after `push -1` in each SEH prologue. The finished neighbour is range-46fdf0.md (it already names `StageAnimator` vtable 0x4e1fb4, whose ctor `0x4bb130` is called from file E here).

## 0. Headline findings

1. **Five files, matching the guard stubs given in the brief** (stub = `mov cl,[0x5826d5]; mov al,1; test; jne; or; mov; jmp +0x10; push 0x402c90; call atexit(0x4c0d01); pop; ret`, 0x30 bytes each): stubs at 0x46e820, 0x46e850, 0x46ee90, 0x46fa50, 0x46fdc0 (previous stub 0x46d800). File boundaries: A 0x46d830-0x46e84f, B' 0x46e850-0x46e87f (stub only, no code), B 0x46e880-0x46eebf, C 0x46eec0-0x46fa7f, D 0x46fa80-0x46fdef (the last 0x30 bytes 0x46fdc0-0x46fdef are the stub; 0x46fdf0 starts the ParallelShadow file).
2. **Four classes, all derived from the engine object base `EffectBase`** (ctor 0x4b2830, dtor 0x4b29c0, 9-slot vtable, `include/unk_47c460.h`) except the last, which derives from `StageAnimator` (vtable 0x4e1fb4, include/unk_476490.h). Every effect vtable here overrides only slot 0 (scalar deleting dtor) and slot 5 (`Render(RenderDevice*, Camera*)`, `ret 8`); slots 1-4, 6-8 are the shared base entries `0x4b2c10, 0x48e660, 0x4b2d50, 0x4b2fc0, 0x477080, 0x4b3010, 0x4890c0` (A, B, C vtables are 9 slots; slot 6 is 0x477080 here, as in WindowRain, not 0x4031e0).
3. **File A = `CarRain`** (guess; "rain streaks on the car body panels", object owned at **Viewport+0x2008**, `new(0x300)`, ctor arg 0x20 = 32 drops). Its update `0x46e2d0` is called from the viewport's per-frame code (0x453aa8) right after `WindowRain::Update 0x476fa0` (Viewport+0x2004), see range-4741a0.md. It looks up three meshes by name in the car model (`"rain_body"` 0x4fe478, `"rain_hood"` 0x4fe46c, `"rain_trunk"` 0x4fe460), copies their vertex positions, and each frame draws up to 32 billboards that spawn on random body-panel vertices, fade over 1/0.25 s and respawn.
4. **File B = `CarLight`** (guess; glow sprite for headlights, tail, brake and reverse lights). 12 per car, created in the car model loader `0x45e480` (the 13705-byte Car loader, see `range-47c460.md`/NOTES) for the nodes `light_front_left01..03`, `light_front_right01..03`, `light_rear_left`, `light_rear_right`, `light_brake_left`, `light_brake_right`, `light_reverse01`, `light_reverse02` (strings 0x4e1d88..0x4e1e5c), stored at Car+0x1bd40 + 0x1c*i. Size 0x13c. The file also has one static: `Vertex s_quad[4]` (0x2c bytes each) at 0x509420.
5. **File C = `CityLight`** (guess; street-lamp lens flare in city stages): a sprite effect with a glow quad (`data/global/flares/cityglow.tga`, 0x4fe4a8) and a flare quad (`data/global/flares/cityflare.tga`, 0x4fe484). Created in the Stage's object loader (0x40d6f6: `new(0x16c)`, ctor 0x46eec0(obj, &Stage+0xd4, &Stage+0x6b4, &Stage+0x788)) for object kind 2 with a child pointer at obj+0x10c; the object is pushed into the Stage's effect vector at Stage+0x68 (`0x4bc680` insert, 0x40d74b and 0x40da06).
6. **File D = `WaterAnimator`** (guess; the sibling of `TexScroller`, 0x30 bytes, `new(0x30)` at 0x410cbe, ctor 0x46fa80 from the Stage loader at 0x410d2f, result stored at `[record+0x14]`). It derives `StageAnimator` (base ctor 0x4bb130 sets vtable 0x4e1fb4 and `stamp = -1`), cycles the mesh material's texture through a 32-entry table at `owner+0x884` and animates two sets of texture coordinates.
7. **The Vec2 helper calls in D use the out-of-line inlines at 0x46fdf0 (`operator-`), 0x46fe20 (`operator*(float)`), 0x46fe50 (`operator/(float)`)**, which are *defined in the next file* (range-46fdf0.md section 2.1). D therefore only calls them; they are not part of this range.
8. **Static initialisation**: only file B has a real static (`s_quad`, init-table entry at 0x46e880: `call 0x46e890; jmp 0x46e8b0`). Files A, C, D have none. File B' (0x46e850) is a TU that emits only the STL guard stub (an empty .cpp that includes the STL headers; same case as the empty modules M7-M10 in range-49b630.md).
9. **No class here holds a Miles/Bink/DirectX call directly**; the render slots go through the device vtable (`RenderDevice`, include/rallytrophy.h): `[0xb0](1)`, `[0xe4]()` returns the current view matrix (4x4 floats; `call [vt+0xe4]` result indexed as `[+0x8,+0x18,+0x28,+0x38]` = third column + translation row used as a depth/planar test), `[0x104] DrawMesh(DynamicMesh*, flags)`, `[0x114] DrawSprite(const EffectVertex* quad[4], texture, camera)` (guess for the third arg).

## 1. Module / file table

| # | Range | End marker | Content | Suggested file |
|---|---|---|---|---|
| A | 0x46d830 - 0x46e84f | stub 0x46e820 (0x46e820-0x46e84f) | `CarRain` (ctor, dtors, Init, SetBounds, Update, Render) | src/unk_46d830.cpp |
| B' | 0x46e850 - 0x46e87f | stub 0x46e850 | empty TU, only the `_$E` stub | no file (see 0.8; keep in the list so the stub is counted) |
| B | 0x46e880 - 0x46eebf (code ends 0x46ee81; stub 0x46ee90-0x46eebf) | stub 0x46ee90 | static `s_quad[4]` init/atexit pair, `CarLight` (ctor, dtors, SetSize, Render) | src/unk_46e880.cpp |
| C | 0x46eec0 - 0x46fa7f (code ends 0x46fa0f + padding; stub 0x46fa50-0x46fa7f) | stub 0x46fa50 | `CityLight` (ctor, dtors, Render) | src/unk_46eec0.cpp |
| D | 0x46fa80 - 0x46fdef (code ends 0x46fdbb; stub 0x46fdc0-0x46fdef) | stub 0x46fdc0 | `WaterAnimator` (ctor, dtors, Update) | src/unk_46fa80.cpp |

Evidence for the splits: (1) the five 0x30-byte stubs above (plus the previous one at 0x46d800 from the brief); (2) each file's first function is 16-aligned and starts with a fresh ctor/prologue (0x46d830 SEH ctor, 0x46e880 static-init pair, 0x46eec0 SEH ctor, 0x46fa80 SEH ctor); (3) file-private data: B alone refers to .bss 0x509420-0x5094cf; (4) the vtables (.rdata 0x4e1f3c, 0x4e1f64, 0x4e1f88, 0x4e1fac) are in file order and each is referenced only by code in its own file plus callers' `new`s. The stubs and the 0x46e8c0 callback are not in functions.csv.

## 2. Shared engine-object facts (EffectBase layout as used here)

From the ctors/Render code; the full base is outside this range (0x4b2830), see unk_47c460.h (`EffectBase`, `Effect`). Offsets in the 0x124-byte base:

- +0x00 vptr; +0x04 int kind/id (C sets 5; base ctor presets it); +0x08 int (zeroed by A? no: B ctor `[esi+8]=0`, C ctor `[ebp+8]=0`); +0x0c byte flags (bit 0x04 = SetFlag04); +0x14/+0x18/+0x1c Vec3 position (C copies from the object record); +0x20 4x4 matrix (0x40 bytes, copied with `rep movsd 0x10`); +0x50/+0x54/+0x58 Vec3 (C copies the record's +0x14..+0x1c, then zeroes them); +0x60..+0x7c "world" matrix rows (the camera object's right/up/forward at +0x60/+0x70/+0x80 are read in the Render code: camera +0x60 right, +0x70 up, +0x80 forward, +0x90 position, +0x124 near distance, +0x128 far distance); +0xa0..+0xb4 bounding box (6 floats, boxMin/boxMax in Effect); +0xb8,+0xbc,+0xc0 ints (zeroed by ctors); +0xc4,+0xc8 floats (B sets both to `size`; C sets 3.0); +0xcc byte flags (bit 0 set and bit 1 cleared by A's base, B and C ctors via and 0xfd / or 1); +0x114 `std::string name` (0x10 bytes: +0x118 = char*; A compares it with the rain mesh names; C copies it with `std::string::assign` 0x4024f0(src, 0, npos 0x4e1340)); +0x124 texture/mesh handle.
- `Camera` (render arg 2): position (+0x90, +0x94, +0x98 in these functions is the *effect's* own world position; the camera's is at +0x90 of the camera arg = `[edi+0x90]` in B/C); near/far = camera +0x124/+0x128.

External helpers called (not in range, names are guesses): 0x4b2830 EffectBase ctor, 0x4b29c0 EffectBase dtor, 0x4bd000 `operator delete`, 0x4c1276 `operator new`, 0x4c11f4 / 0x4c1116 EH vector ctor/dtor iterator (the usual `__vec_ctor`/`__vec_dtor`: args (ptr, elemsize, count, ctor, dtor) / (ptr, elemsize, count, dtor)), 0x490ad0 empty ctor, 0x402c90 empty dtor, 0x4baf40 / 0x4baf90 `DynamicMesh` ctor/dtor (0x38 bytes), 0x4a6800 (lookup of a mesh by name on a model: `ecx = &obj+0x124`, arg = 0; returns a `Mesh*` with vertex count at +0x40 and Vec3 positions at +0x48), 0x4ce290 `strcmp`, 0x4b55e0 `Matrix::Multiply(this, const Matrix*)`, 0x40b0c0 `Vec3 operator*(float)` (ecx = vec, args (out*, scalar), returns out), 0x4203b0 returns a `Vec3*` (the current light/sun direction; many callers 0x41cf97 ...), 0x4c1998 `rand`, 0x4c1404 `floor` (double in, st0 result), 0x4c19f0 `pow`-style helper taking (x, y) on the FPU stack, 0x4aef30 `LoadTexture(path, flags 0x40)` (returns a handle), 0x4024f0 `std::string::assign(const string&, pos, n)`, 0x4bb130 `StageAnimator::StageAnimator`.

## 3. File A: `CarRain` (0x46d830 - 0x46e84f)

### Identity and ownership
- `new(0x300)` + ctor `0x46d830(0x20)` at 0x4562dc-0x4562e0 (viewport setup, `ebp` = Viewport); stored at **Viewport+0x2008** (0x4562f9). Then `Init 0x46dd80(carModelObj)` (0x4562ff), `0x4b2b30(texture)` on the effect (0x456307, base setter, argument edi = the ParticleTexture also given to WindowRain 0x456304-ish), `SetBounds 0x46e1a0(src, [Viewport+0x1f6c], [Viewport+0x1f68])` (0x45631d), and per frame `Update 0x46e2d0(dt)` (0x453aa8, after WindowRain::Update at 0x453a98; the `[ebx+0x2008]` null test at 0x453a9d). Scalar deleting dtor 0x46dc50 is slot 0 of the vtable (file offset 0xe073c is its table slot in the object init table); real dtor body 0x46dc70 (called at 0x46dc53).
- Size **0x300**. Vtable **0x4e1f3c** (9 slots, the next dword 0x4e1f60 is the float constant 5.371e-5 = 0.055/1024):

  | slot | address | meaning |
  |---|---|---|
  | 0 | 0x46dc50 | scalar deleting dtor (`ret 4`) |
  | 1 | 0x4b2c10 | base |
  | 2 | 0x48e660 | shared empty `ret` |
  | 3 | 0x4b2d50 | base |
  | 4 | 0x4b2fc0 | base |
  | 5 | **0x46e430** | `Render(RenderDevice* device, Camera* camera)` (ret 8, SEH frame, ebp-based, ~109 FPU instructions) |
  | 6 | 0x477080 | base (the same entry as WindowRain) |
  | 7 | 0x4b3010 | base SetFlag04 |
  | 8 | 0x4890c0 | `return 1` |

### Fields (offsets into the 0x300-byte object; 0x00-0x123 is the EffectBase)
| Offset | Type | Seen in | Meaning |
|---|---|---|---|
| 0x124 | ptr | ctor zero, Init (mesh lookup arg) | texture (set by the viewport via 0x4b2b30; Render passes it to DrawMesh) |
| 0x128 | DynamicMesh* | ctor (`0x4baf40`), dtor (`0x4baf90` + delete), Render | the drop quad mesh: vertex array `[mesh+4]` (4 x 0x2c bytes per drop, the array has a count prefix and is built with the EH vector ctor 0x4c11f4), index array `[mesh+8]` (6 ushorts per drop: i*4+{0,1,2,0,2,3}), `[mesh+0xc]` = 4n (ushort), `[mesh+0xe]` = 2n (ushort), `[mesh+0x18]` = 0, `[mesh+0x24]` = 0; the ctor sets for each quad vertex normal 0, color -1 and the 4 uv corners (0.25,0.5), (0.5,0.5), (0.5,1.0), (0.25,1.0)) in four patterns using 0x3e800000=0.25, 0x3f000000=0.5, 0x3f800000=1.0 |
| 0x12c | int | ctor (n), Init, Update, Render | drop count n (32) |
| 0x130 | Drop* | ctor (`new(0x18*n+4)`, EH vec ctor), dtor (vec dtor), Init, Update, Render | array of n drops, 0x18 bytes each: +0 float phase 0..1 (init i/n), +4 float size (0.015 + (rand&0x3ff)*5.371e-5), +8 Vec3 local position (a random vertex of one of the three body meshes), +0x14 Matrix* (which of the three body matrices to transform by: &this+0x150 / +0x190 / +0x1d0) |
| 0x134 | Vec3* (counted) | Init, dtor | body mesh vertex positions (copy of mesh+0x48, count in +0x140) |
| 0x138 | Vec3* (counted) | Init, dtor | trunk mesh vertex positions (count +0x144) |
| 0x13c | Vec3* (counted) | Init, dtor | hood mesh vertex positions (count +0x148) |
| 0x140 | int | Init | body vertex count |
| 0x144 | int | Init | trunk vertex count |
| 0x148 | int | Init | hood vertex count |
| 0x14c | int | Init | total vertex count (body+trunk+hood); random vertex = `rand() % total`, then picks the array by range |
| 0x150, 0x190, 0x1d0 | Matrix x3 (0x40) | Render | the live matrices used to place drops from the body, hood and trunk arrays (Render fills them each frame from `[this+0x2f4/0x2f8/0x2fc]+0x60` or from the saved matrices) |
| 0x210, 0x250, 0x290 | Matrix x3 (0x40) | Init, ctor, Render | saved static matrices of the `rain_body`, `rain_hood`, `rain_trunk` meshes (copied from mesh object +0x20). The ctor writes identity (1.0 on the diagonal) into 0x250 and 0x290 only; 0x210 is not initialised by the ctor (guess: it is only used when the body part is absent, never read in Render) |
| 0x25c-0x2c8 | ints | ctor | zeroed; they are the off-diagonal parts of those matrices (the ctor zeros 0x254..0x2cc item by item); not separate fields |
| 0x2d0, 0x2d4, 0x2d8 | Vec3 | Init | body panel translation `(+0x14,+0x18,+0x1c of the mesh object)` |
| 0x2dc, 0x2e0, 0x2e4 | Vec3 | Init | hood panel translation |
| 0x2e8, 0x2ec, 0x2f0 | Vec3 | Init | trunk panel translation |
| 0x2f4 | obj* | SetBounds, Render | source of the body matrix (`[obj+0x60]`, 64 bytes copied to 0x150): `src` argument of SetBounds |
| 0x2f8 | obj* | SetBounds (third arg), Render | optional trunk/hood object: if non-null its `+0x60` matrix becomes matrix 0x190; if null Render builds 0x190 from the saved matrix 0x250 and translation 0x2dc.. multiplied by 0x150 (`0x4b55e0`) |
| 0x2fc | obj* | SetBounds (second arg), Render | same for matrix 0x1d0 (saved 0x290, translation 0x2e8..) |

(The pairing of 0x2f8/0x2fc with hood/trunk is by arg order in SetBounds: `[+0x2fc]=arg2`, `[+0x2f8]=arg3`; the Render uses 0x2f8 for the matrix at 0x190 and 0x2fc for 0x1d0. Whether those are the hood and the trunk follows from the 0x250=hood/0x290=trunk saved matrices.)

### Algorithms
- **Ctor 0x46d830(n)** (thiscall, SEH, `ret 4`): EffectBase ctor, vptr, identity diagonals of matrices 0x250/0x290, zeroes ~60 dwords, `[this+0x12c]=n`. Allocates (1) the drop array `0x18*n+4` with the vector ctor (empty ctor 0x490ad0), (2) `new(0x38)` DynamicMesh (`0x4baf40`) -> `[this+0x128]`, (3) the mesh vertex array `176*n+4` (4 vertices x 0x2c per drop; vector ctor 0x4c11f4), (4) the index array `12*n` bytes, sets `[mesh+0xe]=n*2`, `[mesh+0xc]=n*4`, fills 6 indices per drop (`i*4+0,1,2,0,2,3`) and the four fixed uv/colour patterns per quad (loop unrolled by the compiler across the 4 vertices), finally `[this+8]=0`.
- **Dtor 0x46dc70** (`__fastcall`-style, SEH EH 0x4dbf06): vptr reset, `0x4baf90` + delete on the DynamicMesh, vector dtor (`0x4c1116`) + delete on the drop array (`ptr-4` has the count), and on the three vertex arrays (element 0xc, count prefix), then the base dtor 0x4b29c0.
- **Init 0x46dd80(const Container* c)** (thiscall, SEH, `ret 4`): walks `c->items` (the vector at `c+0x4c..+0x50`, `std::vector<Obj*>` begin/end), for each item with `item+4 == 1` (mesh object kind) takes `item+0x118` as name (empty-string constant 0x4e1344 if null) and `strcmp`s it with "rain_body", "rain_hood", "rain_trunk"; on a match it calls `0x4a6800` on the item's `+0x124` for the mesh, copies the item's 64-byte matrix at `+0x20` and position `+0x14..+0x1c` into this+0x210/0x250/0x290 / 0x2d0/0x2dc/0x2e8. Then allocates counted `Vec3` arrays for the matched meshes, copies the vertices with `rep movsd`, sums the counts into +0x14c, and for each drop i: phase = i/n, size = rand&0x3ff*5.371e-5 + 0.015, picks `rand() % total` and uses it to choose the array and vertex (setting `Matrix*` +0x14 to this+0x150/0x190/0x1d0).
- **SetBounds 0x46e1a0(src, a, b)** (`thiscall, ret 0xc`, no frame, FPU): copies the 0xa0-0xcc bytes of `src` (the bounding box and flags, with the six box floats +0xa0..+0xb4 scaled by 1.1 = 0x4e13e4) into this; stores `[this+0x2f4]=src`, `[this+0x2fc]=a`, `[this+0x2f8]=b`; `[this+0xc4]` = `[this+0xc4]*1.1` before it is overwritten by `[src+0xc4]` (compiler artefact of the in-place multiply; the code multiplies this->0xc4 first, then replaces it).
- **Update 0x46e2d0(dt)** (`thiscall, ret 4`, ebp frame, no SEH): `d = dt/0.25` (0x4e1f38); for each drop `phase += d`; if `phase >= 1.0`: `phase -= floor(phase)`, new size `0.015 + (rand&0x3ff)*5.371e-5`, new random vertex/matrix as in Init.
- **Render 0x46e430(device, camera)** (vtable slot 5): returns if `[this+0x124]` is 0; builds the three live matrices (see fields), sets `[mesh vertex]=...`, then for each drop i: `s = (phase*0.8 + 0.2) * size` (0.8 = 0x4e1390, 0.2 = 0x4e13c4), `half = s*0.5`, alpha byte = `(1.0 - phase)*255` stored as the vertex colour byte, world pos = `matrix * localPos` (inline 3x3+translation), then four corners at `pos +/- right*s +/- up*half` using the camera's right (+0x60) and up (+0x70) via `Vec3::operator*` 0x40b0c0; writes position, colour -1 patched with the alpha, 0x2c-byte vertices. Finally `device->vf[0xb0](1)` and `device->DrawMesh(mesh, 1)` (`[0x104]`).

## 4. File B': 0x46e850 - 0x46e87f
Only the `_$E` guard stub at 0x46e850 (0x30 bytes: `mov cl,[0x5826d5]` ... `push 0x402c90; call 0x4c0d01`). No code, no data. (File A's stub 0x46e820 is the same pattern, 0x46e820-0x46e84f.)

## 5. File B: static quad + `CarLight` (0x46e880 - 0x46eebf)

### Static data
- `Vertex s_quad[4]` at **0x509420**, element 0x2c bytes (`EffectVertex` layout: pos +0, normal +0xc, color +0x18, u +0x1c, v +0x20, u2 +0x24, v2 +0x28), 0xb0 bytes total (end 0x5094d0). Init-table entry **0x46e880** (`call 0x46e890; jmp 0x46e8b0`): 0x46e890 `__vec_ctor(&s_quad, 0x2c, 4, ctor 0x490ad0, dtor 0x402c90)`; 0x46e8b0 pushes the atexit callback 0x46e8c0 and calls `atexit` 0x4c0d01; 0x46e8c0 `__vec_dtor(&s_quad, 0x2c, 4, 0x402c90)`. (0x46e8c0 is a 20-byte callback, not in csv.)
- The `CarLight` ctor (0x46e8e0) *also* initialises most of the quad's fixed fields through absolute stores each time it runs: `0x50942c,30,34` (normal 0), `0x509444,48` (+0x24/+0x28 of the vertex? no: it is the second-vertex normal = zero), `0x509458/5c/60`, `0x509470/74`, `0x509484/88/8c`, `0x50949c/a0`, `0x5094b0/b4/b8`, `0x5094c8/cc` (u2 / v2 or normal zeroes) and `0x509438, 0x509464, 0x509490, 0x5094bc` = `-1` (the four vertex colours). The data are inline-expanded static stores in the ctor, i.e. in the source they are `s_quad[i].normal = 0; s_quad[i].color = -1;` statements in the CarLight ctor (guess; or a static initialiser block placed there by the compiler). The complete 239-byte entry/body and its instruction sequence are observed in REA Evidence `ev_2fcd008b5b236b178b7a656af7a693903afcc6f3db4ce24f4b73a94d12479368`; the focused matcher now confirms byte-exact output for `CarLight::CarLight`.

### Class `CarLight` (guess), size **0x13c**, EffectBase-derived, vtable **0x4e1f64**
| slot | address | meaning |
|---|---|---|
| 0 | 0x46e9d0 | scalar deleting dtor (`ret 4`; calls 0x46e9f0, delete if flags&1) |
| 1 | 0x4b2c10 | base |
| 2 | 0x48e660 | `ret` |
| 3 | 0x4b2d50 | base |
| 4 | 0x4b2fc0 | base |
| 5 | **0x46ea20** | `Render(device, camera)` (ret 8, SEH EH 0x4dbf90, 171 FPU instructions) |
| 6 | 0x477080 | base |
| 7 | 0x4b3010 | base |
| 8 | 0x4890c0 | `return 1` |

Fields: +0x124 texture handle (`Car+0x1bebc/0x1bec0/0x1bec4/0x1bec8`: front/rear/brake/reverse light texture), +0x128 float falloff exponent (`Car+0x10b0/0x10bc/0x10c8/0x10d4`), +0x12c float size (default 1.0; SetSize), +0x130 float intensity scale (default 1.0), +0x134 float bias (`Car+0x10b4/0x10c0/0x10cc/0x10d8`), +0x138 int mirror flag (1 for the right-hand lights `front_right*`, `rear_right`, `brake_right`, `reverse02`; 0 otherwise). Base fields set by the ctor: `[+0xcc] = (b & 0xfd) | 1`, `[+0xc0]=0`, `[+0xb8]=[+0xbc]=0`, `[+0xc4]=[+0xc8]=1.0`, `[+8]=0`.

Car-side facts (the 12 sites in the Car loader 0x45e480, 0x45f5a0-0x45fe40, each of the form `strcmp(name, "light_...")`; `new(0x13c)`; ctor 0x46e8e0; copy of the mesh object's matrix `rep movs 0x10` from +0x20 and of its +0x14..+0x1c; `[this+0x124]=Car tex`; `SetSize(Car+size)`; `[this+0x134]=bias; [this+0x128]=exponent`; `[Car+off]=this`):

| node name (string) | Car slot | size field | falloff | bias | tex |
|---|---|---|---|---|---|
| light_front_left01 (0x4e1d88) | +0x1bd40 | +0x10ac | +0x10b0 | +0x10b4 | +0x1bebc |
| light_front_left02 (0x4e1d9c) | +0x1bd5c | same | same | same | same |
| light_front_left03 (0x4e1db0) | +0x1bd78 | same | same | same | same |
| light_front_right01..03 (0x4e1dc4, 0x4e1dd8, 0x4e1dec) | +0x1bd94, +0x1bdb0, +0x1bdcc | same | same | same | same (flag +0x138 = 1) |
| light_rear_left (0x4e1e00) | +0x1bde8 | +0x10b8 | +0x10bc | +0x10c0 | +0x1bec0 |
| light_rear_right (0x4e1e10) | +0x1be04 | same | same | same | same (flag 1) |
| light_brake_left (0x4e1e24) | +0x1be20 | +0x10c4 | +0x10c8 | +0x10cc | +0x1bec4 |
| light_brake_right (0x4e1e38) | +0x1be3c | same | same | same | same (flag 1) |
| light_reverse01 (0x4e1e4c) | +0x1be58 | +0x10d0 | +0x10d4 | +0x10d8 | +0x1bec8 |
| light_reverse02 (0x4e1e5c) | +0x1be74 | same | same | same | same (flag 1) |

Algorithms:
- **Ctor 0x46e8e0** (`__fastcall`, no SEH, no frame, no args): EffectBase ctor 0x4b2830; `[this]=0x4e1f64`; the field defaults above; the static quad fixed fields above.
- **0x46e9d0** scalar deleting dtor -> **0x46e9f0** (11 bytes: `[ecx]=0x4e1f64; jmp 0x4b29c0`, the real `~CarLight`, which has no members to destroy).
- **SetSize 0x46ea00(float s)** (`thiscall, ret 4`): `[+0x12c] = [+0xc4] = [+0xc8] = s` (also the effect's own size fields used by culling).
- **Render 0x46ea20(device, camera)** (slot 5): `v = this->worldPos(+0x90) - camera->position(+0x90)`; returns unless `dot(v, camera->forward(+0x80..0x88)) > camera->near (+0x124)`; normalises v; `facing = dot(this->forward(+0x80..), *0x4203b0())` clamped to 0..1 (the light direction vs the lamp's axis; sign test on the float bits); `alpha = pow(facing, [+0x128]) * [+0x130] + [+0x134]`, clamped to 1.0, rejected below 0.001 (0x4e1398); gets the device matrix with `[device vt+0xe4]()` and tests the lamp's depth along the view axis against near/far (`camera+0x124/+0x128`); billboard half-size `[+0x12c]`; fills the **static quad** s_quad with the camera right/up vectors scaled by the size (four corners, uv (0,0)..(1,1) with the mirror flag +0x138 selecting the u orientation), writes the grey colour `c | c<<8 | c<<16` with `c = (int)(alpha*255)` into all four vertex colours at 0x509438/0x509464/0x509490/0x5094bc, and calls `device->DrawSprite(s_quad, [+0x124] tex, camera)` (`[vt+0x114]`, args (0x509420, texture, camera)).

## 6. File C: `CityLight` (0x46eec0 - 0x46fa7f)

### Identity and ownership
- `new(0x16c)` + ctor `0x46eec0(obj, &Stage+0xd4, &Stage+0x6b4, &Stage+0x788)` (thiscall, `ret 0x10`) at 0x40d6f6-0x40d726 and again at 0x40d9e7 (two call sites in the Stage object loader: kind 2 with `[obj+0x10c] != 0`). The result is inserted with `std::vector::insert` 0x4bc680 into the Stage's effect vector at **Stage+0x68** (0x40d74b, 0x40da06) and also given `0x4b2b90`.
- Vtable **0x4e1f88**, 9 slots, same shape: slot 0 0x46f2e0 (scalar deleting dtor, calls 0x46f300), slot 5 **0x46f360** (Render, ret 8, 270 FPU instructions, SEH EH 0x4dc046, ebp frame, local `EffectVertex quad[4]` built with the EH vector ctor at ebp-0x104), slots 1-4,6-8 base entries. Real dtor 0x46f300 (SEH EH 0x4dbff8): vptr, `__vec_dtor(this+0x13c, 12, 4)` for the four Vec3 corners, base dtor 0x4b29c0.

### Fields (0x16c bytes)
- +0x04 = 5 (kind). +0x08 = 0. +0x114 `std::string` copy of the object name (assign from `obj+0x114`). +0x124 = glow texture slot pointer (arg 3 = &Stage+0x6b4, loaded once: if `*slot == 0`, `*slot = LoadTexture("data/global/flares/cityglow.tga", 0x40)` 0x4fe4a8); +0x128 = flare texture slot pointer (arg 4 = &Stage+0x788, `"data/global/flares/cityflare.tga"` 0x4fe484). These slots are only loaded when arg 2 (the Stage's texture loader at +0xd4) is non-null. +0x12c float 1.0 (flare exponent), +0x130 float 1.5, +0x134 float 1.0 (scale), +0x138 float 0 (bias). +0x13c, +0x148, +0x154, +0x160: four `Vec3` (the glow quad's corner offsets: `+/-0.75 * right`, `+/-0.75 * up`, then adjusted by `+/-1.5 * (right+up)` in the ctor; the ctor calls `Vec3::operator*(0xbf400000 = -0.75 / 0x3f400000 = 0.75 / 0x3fc00000 = 1.5)` on the object's axis vectors read from `obj+0x20..0x38`).
- Copies from the object record: `obj+0x20` 64-byte matrix -> this+0x20, `obj+0x14/0x18/0x1c` -> this+0x50/0x54/0x58 -> then moved to +0x14/+0x18/+0x1c and the +0x50..+0x58 cleared; if `obj+0x10c` (a child with its own matrix at +0x20..+0x5f) exists the matrix is multiplied in with 0x4b55e0 (the 16-dword copy to `esp+0x38` then `Matrix::Multiply`).
- Base flags: `[+0xcc] = ((b & 0xfd) | 1)` (written twice), `[+0xb8..+0xc0]=0`, `[+0xc4]=[+0xc8]=3.0 (0x40400000)`.

### Render 0x46f360 (slot 5)
Same culling front-end as `CarLight::Render` (this position vs camera, near/far); then draws **two** textured quads: (1) the glow (`[+0x124]`) through `device->DrawSprite(quad, glowTex, camera)` (0x46f713) using the local vertex array whose corners are `pos +/- (glowCorner[i])`, transformed by the camera right/up (and the stored `+0x13c...` offsets), colour white; (2) the flare (`[+0x128]`) with `pow(facing, [+0x12c]) * [+0x134] + [+0x138]` modulating the grey colour (`*255`), constants 0.7 (0x4e1bec) and pi (0x4e1e7c) giving a rotating/scaled quad (two more `DrawSprite` calls at 0x46f966 and 0x46f9eb with the 0.5 (0x4e138c) uv offsets), and finally `0x4c1116` destroys the local vertex array. It skips drawing entirely if the lamp is behind the camera or `alpha < 0.001`.

## 7. File D: `WaterAnimator` (0x46fa80 - 0x46fdef)

### Identity
- `new(0x30)` at 0x410cbe (`push 0x30`) in the Stage loader; ctor `0x46fa80(mesh, owner, float rate, Vec2* v1, Vec2* v2, Vec2* v3)` (thiscall, `ret 0x18`, SEH EH 0x4dc084) called at 0x410d2f with args `(DynamicMesh* [esp+0x5c], Stage* ebx, [0x5083e0] (float), &0x5083f8, &0x5083f0, &0x5083e8)`; result stored at `[record+0x14]`. Derived from `StageAnimator` (include/unk_476490.h: ctor 0x4bb130, `stamp` at +4); vtable **0x4e1fac**:

  | slot | address | meaning |
  |---|---|---|
  | 0 | 0x46fb70 | scalar deleting dtor (`ret 4`; calls 0x46fb90, delete if flags&1) |
  | 1 | **0x46fc10** | `Update()` (no args, plain `ret`, SEH EH 0x4dc110, ebp frame) - the pure virtual slot of the base (0x4e1fb4 slot 1 = `_purecall` 0x4c19e3) |

  Base vtable 0x4e1fb4 follows at 0x4e1fb4 (slot 0 0x46fe90 in the next file, slot 1 `_purecall` 0x4c19e3). Following floats: 0x4e1fbc = 1/2048 (0.00048828125), 0x4e1fc0 = 1/65536 (0x37800000), 0x4e1fc4 = 1/3.
- Real dtor 0x46fb90 (SEH EH 0x4dc0b9): sets vptr to 0x4e1fac, `__vec_dtor(this+0x24 array, 8, count prefix)` + delete on the saved-UV array, then resets the vptr to the base vtable 0x4e1fb4.

### Fields (0x30 bytes)
| Offset | Type | Meaning |
|---|---|---|
| 0x00 | vptr | 0x4e1fac |
| 0x04 | float | last Update time stamp (`[0x56b0ac]+0x24`, the global clock) |
| 0x08 | float | `rate` (ctor arg 3) |
| 0x0c, 0x10 | float x2 | `*v1` (Vec2 copy, first scale `[+0x0c]` is applied to the saved u,v of layer 1; `[+0x10]` to layer 2) |
| 0x14, 0x18 | Vec2 | `*v2`: scroll velocity of layer 1 |
| 0x1c, 0x20 | Vec2 | `*v3`: scroll velocity of layer 2 |
| 0x24 | Vec2* (counted) | copy of each vertex's original `(u, v)` (`mesh->vertices[i].u/v` at vertex+0x1c/+0x20), count `mesh->vertexCount` (ushort at `mesh+0xc`), `new(8*n+4)` with the vector ctor (element 8 bytes, empty ctor 0x490ad0 / dtor 0x402c90) |
| 0x28 | DynamicMesh* | the animated mesh (ctor arg 1) |
| 0x2c | Stage* | owner (ctor arg 2); `owner + 0x884 + i*4` is a table of 32 texture handles |

### Update 0x46fc10
Binary evidence: REA dossier `ev_098aeda40f3866cc5d46de6aa9df644e5a1638f8a9cce76a6f260f691961b0eb` (Ghidra 12.1.3, shipped executable digest `cc1718e21a83b7422cfd2b304a9fe43e4f6a6e7550389d9f20bf30530f19ec8d`). The observed body is 428 bytes (`0x46fc10-0x46fdbb` inclusive); assembly confirms the clock-time store, tick-based texture selection, two Vec2 helper chains, and a vertex loop using a 16-bit count, 8-byte saved-UV stride, and 0x2c-byte vertex stride. The pseudocode is decompiler output and is used as a behavior reference, not as original source.

Matcher probes on 2026-10-27 leave the tracked candidate unchanged: baseline 424 bytes / 290 differing bytes; direct initialization of `uv1` only, 409 / 291; a volatile clock read, 420 / 278; a 16-bit local count, 431 / 229; both latter changes, 427 / 268. None matches, and the volatile form is diagnostic only. Focused tracked-file result remains 5/5 marked functions exact, one `NONMATCHING`, with no diffs or compile failures.

`stamp = clock+0x24`; `t = clock16 = (int)([0x56b0d0] & 0xffff)`; frame `f = (int)(t * [+8] * 1/2048) & 31`; `mesh->material->texture ([[+0x28]]+4) = *(owner+0x884 + f*4)`. Then `d1 = Vec2(+0x14) * t / 65536.0` and `d2 = Vec2(+0x1c) * t / 65536.0` (out-of-line Vec2 helpers 0x46fe20 / 0x46fe50 from the next file; 0x47800000 = 65536.0f), and for each vertex `i` (loop count `ushort [mesh+0xc]`, stride 0x2c, pointers advanced by 8 in the saved array): `uv1 = saved[i] * [+0x0c] - d1`, `uv2 = saved[i] * [+0x10] - d2`, then `vertex.u = uv1.x`, `vertex.v = uv1.y`, `vertex.u2 = uv2.x + 0.5`, `vertex.v2 = uv2.y + 0.5` (0.5 = 0x4e138c). (The (uv2 + 0.5) offsets, the second texture layer.)

## 8. Table of all functions

Sizes: from functions.csv unless "(not in csv)". "frame" lists: SEH (EH handler address), ebp (frame pointer), FPU (x87 instructions). Callers: direct callers from the scan (`refs.py`), or "vtable" (table slot).

| Address | Size | Convention / frame | Role (guess marked) | Main callers |
|---|---|---|---|---|
| 0x46d830 | 1048 | thiscall `ret 4`, SEH 0x4dbe67, no ebp frame | `CarRain::CarRain(int n)` ctor: base ctor, matrix identities, drop array, DynamicMesh, quad indices/uv | 0x4562e0 (Viewport setup) |
| 0x46dc50 | 30 | thiscall `ret 4` | `CarRain` scalar deleting dtor (compiler-generated, vtable slot 0) | vtable 0x4e1f3c slot 0 |
| 0x46dc70 | 258 | fastcall-like (ecx = this), SEH 0x4dbf06 | `CarRain::~CarRain()` real dtor | 0x46dc53 (scalar deleting dtor) |
| 0x46dd80 | 1050 | thiscall `ret 4`, SEH 0x4dbf31, FPU 9 | `CarRain::Init(Container* model)`: find rain_* meshes, copy vertices/matrices, seed the drops | 0x4562ff |
| 0x46e1a0 | 303 | thiscall `ret 0xc`, no frame, FPU | `CarRain::SetBounds(src, a, b)`: copy flags/box (x1.1) + body/panel object pointers | 0x45631d |
| 0x46e2d0 | 341 | thiscall `ret 4`, ebp frame, FPU 18, no SEH | `CarRain::Update(float dt)`: advance phases, respawn drops | 0x453aa8 (Viewport update) |
| 0x46e430 | 997 | thiscall `ret 8`, SEH 0x4dbf60, ebp, FPU 109 | `CarRain::Render(device, camera)`: build 4 corners per drop, draw the DynamicMesh | vtable 0x4e1f3c slot 5 (file offset 0xe0750) |
| 0x46e820 | 48 | stub, no frame (not in csv) | STL guard `_$E` stub of file A | init table |
| 0x46e850 | 48 | stub (not in csv) | STL guard `_$E` stub of the empty file B' | init table |
| 0x46e880 | 12 | init-table entry (not in csv) | `call 0x46e890; jmp 0x46e8b0` for the static `s_quad` | init table |
| 0x46e890 | 25 | stdcall-like, no frame | `__vec_ctor(&s_quad, 0x2c, 4, 0x490ad0, 0x402c90)` | 0x46e880 |
| 0x46e8b0 | 12 | stdcall-like | `atexit(0x46e8c0)` registrar | 0x46e885 (jmp) |
| 0x46e8c0 | 20 | callback (not in csv) | `__vec_dtor(&s_quad, 0x2c, 4, 0x402c90)` | atexit |
| 0x46e8e0 | 239 | fastcall (ecx=this), no frame, no SEH | `CarLight::CarLight()` ctor (inline field init and static-quad init) | 12 sites 0x45f5d0 ... 0x45fcce in the Car loader 0x45e480 |
| 0x46e9d0 | 30 | thiscall `ret 4` | `CarLight` scalar deleting dtor (compiler-generated) | vtable 0x4e1f64 slot 0 |
| 0x46e9f0 | 11 | fastcall-like, no frame | `CarLight::~CarLight()`: vptr store + tail `jmp 0x4b29c0` | 0x46e9d3 |
| 0x46ea00 | 27 | thiscall `ret 4` | `CarLight::SetSize(float)`: `+0x12c = +0xc4 = +0xc8` | 12 sites 0x45f61b ... 0x45fdc7 |
| 0x46ea20 | 1124 | thiscall `ret 8`, SEH 0x4dbf90, ebp, FPU 171 | `CarLight::Render(device, camera)`: cull, alpha, fill `s_quad`, DrawSprite | vtable 0x4e1f64 slot 5 |
| 0x46ee90 | 48 | stub (not in csv) | STL guard `_$E` stub of file B | init table |
| 0x46eec0 | 1044 | thiscall `ret 0x10`, SEH 0x4dbfd8, no ebp frame, FPU 36 | `CityLight::CityLight(obj, loader, glowSlot, flareSlot)` ctor | 0x40d726, 0x40d9e7 (Stage object loader) |
| 0x46f2e0 | 30 | thiscall `ret 4` | `CityLight` scalar deleting dtor (compiler-generated) | vtable 0x4e1f88 slot 0 |
| 0x46f300 | 95 | fastcall-like, SEH 0x4dbff8 | `CityLight::~CityLight()`: vptr, `__vec_dtor` of the corners, base dtor | 0x46f2e3 |
| 0x46f360 | 1767 | thiscall `ret 8`, SEH 0x4dc046, ebp, FPU 270 | `CityLight::Render(device, camera)`: glow + flare sprites | vtable 0x4e1f88 slot 5 |
| 0x46fa50 | 48 | stub (not in csv) | STL guard `_$E` stub of file C | init table |
| 0x46fa80 | 237 | thiscall `ret 0x18`, SEH 0x4dc084 | `WaterAnimator::WaterAnimator(mesh, owner, rate, v1, v2, v3)` ctor | 0x410d2f (Stage loader) |
| 0x46fb70 | 30 | thiscall `ret 4` | `WaterAnimator` scalar deleting dtor (compiler-generated) | vtable 0x4e1fac slot 0 |
| 0x46fb90 | 125 | fastcall-like, SEH 0x4dc0b9 | `WaterAnimator::~WaterAnimator()`: delete the saved UV array, reset vptr to the base vtable | 0x46fb73 |
| 0x46fc10 | 428 | thiscall (plain `ret`), SEH 0x4dc110, ebp, FPU 17 | `WaterAnimator::Update()` (virtual, slot 1): texture-frame cycling + two uv layers | vtable 0x4e1fac slot 1 (file offset 0xe07b0) |
| 0x46fdc0 | 48 | stub (not in csv) | STL guard `_$E` stub of file D | init table |

Total code in csv: 22 functions, 9251 bytes (A 4027, B 1468, C 2936, D 820) plus six non-csv items (five stubs 5 x 0x30, 0x46e880 12 bytes, 0x46e8c0 20 bytes).

## 9. Suggested work groups for decompilation

Header first (one pass by whoever does group G1): a new `include/unk_46d830.h` holding `CarRain`, `CarLight`, `CityLight`, `WaterAnimator`, using the existing `EffectBase`/`Effect` (unk_47c460.h), `StageAnimator`/`DynamicMesh` (unk_476490.h) and `EffectVertex`, `Camera`. The four ctors are called from outside this range (Car loader 0x45e480, Stage loader, Viewport setup), so any caller code that is already decompiled can show the right signature.

| Group | Functions | Bytes | Notes |
|---|---|---|---|
| G1 | `CarRain` ctor 0x46d830, scalar dtor 0x46dc50, dtor 0x46dc70, SetBounds 0x46e1a0, Update 0x46e2d0 + the header | 1048+30+258+303+341 = 1980 | the ctor is long but linear; the dtor needs three vector deletes with element size 0xc and one with 0x18; Update uses `floor` and `rand`; keep the 1.1 scale / in-place multiply order in SetBounds |
| G2 | `CarRain::Init` 0x46dd80 + `Render` 0x46e430 | 1050+997 = 2047 | Init: string compares with the three mesh names, `rep movsd` copies, counted Vec3 arrays; Render: 109 FPU instructions, inline matrix times vec3, four `Vec3::operator*` calls per corner |
| G3 | file B in full: statics 0x46e880/0x46e890/0x46e8b0/0x46e8c0, `CarLight` ctor 0x46e8e0, 0x46e9d0, 0x46e9f0, SetSize 0x46ea00, Render 0x46ea20 | 12+25+12+20+239+30+11+27+1124 = 1500 | Render is the only heavy one (171 FPU instructions, pow, sign tests via float bit patterns); the ctor writes 20+ static dwords |
| G4 | `CityLight::Render` 0x46f360 alone | 1767 | 270 FPU instructions, local `EffectVertex[4]` with EH, three `DrawSprite` calls |
| G5 | `CityLight` ctor 0x46eec0, scalar dtor 0x46f2e0, dtor 0x46f300 + file D in full (0x46fa80, 0x46fb70, 0x46fb90, 0x46fc10) | 1044+30+95+237+30+125+428 = 1989 | two files (src/unk_46eec0.cpp and src/unk_46fa80.cpp); the ctors have several `0x40b0c0` Vec3 temporaries; D relies on the out-of-line Vec2 inlines in the next file's header |
| - | stubs 0x46e820, 0x46e850, 0x46ee90, 0x46fa50, 0x46fdc0 | 5 x 0x30 | compiler-generated `_$E` guard stubs, do nothing to decompile |

### Compiler-generated items (do not hand-write; they come from the class definitions)
- Scalar deleting destructors (vtable slot 0, `test flags,1; je; push this; call operator delete`): **0x46dc50, 0x46e9d0, 0x46f2e0, 0x46fb70**.
- The vtables themselves (0x4e1f3c, 0x4e1f64, 0x4e1f88, 0x4e1fac).
- The five `_$E` guard stubs; the static-initialiser pair 0x46e880/0x46e890/0x46e8b0/0x46e8c0 for `s_quad` (a file-scope `EffectVertex s_quad[4];`).
- EH vector constructor/destructor iterator calls (0x4c11f4 / 0x4c1116) come from `new T[n]` / `delete[]` with non-trivial (empty user-declared) ctors/dtors: `Drop`, `Vec3`, `EffectVertex`, `Vec2` elements all need empty user-declared ctor/dtor to produce the calls to `0x490ad0`/`0x402c90`.
- The tiny `0x46e9f0` (`mov [ecx],vptr; jmp base_dtor`) is the real `~CarLight` with an empty body; it is only separate because the scalar deleting dtor calls it, not inlined.

### Unknowns / things to verify in a port
- `Render` third argument of `DrawSprite` ([0x114]) and the semantics of `[device+0xb0](1)`; both are shared with the other effect classes.
- Whether `CarRain` (Viewport+0x2008) draws on the car body of the *viewed* car or on a windshield-like plane; the name is from the three `rain_*` meshes it searches in the car model (strings 0x4fe460-0x4fe478).
- The exact role of the object kind at `+0x04` (C sets 5) and of the `0x4203b0` Vec3 (light/sun direction, many other callers).

### Object matching note: `CarRain::Render` (`0x46e430`)

Plain PE/object comparison showed the original loads `hoodObject` after the
`liveBody` matrix copy; the original source form allowed VC6 to hoist the load
before that copy. Reading `hoodObject` once through a volatile lvalue at its
existing branch preserves the target ordering and reduces the diff from 43 to
16 bytes at 997 bytes. The remaining differences are register assignment for
the `liveBody` destination and `Multiply` arguments (`eax`/`edx`); the
transform, vertex fill and EH state sequence match. This candidate remains
`NONMATCHING`.
Register hints on `body` and `hood`, and passing `&liveBody` directly to the
hood fallback `Multiply`, left this 16-byte diff unchanged.
Declaring `hood` before the first copy and assigning it in the branch
condition also left the diff unchanged.
Replacing the volatile local with an empty inline-assembly block before the
direct `hoodObject` test allowed VC6 to hoist the load again (43 differences);
that variant was reverted.
