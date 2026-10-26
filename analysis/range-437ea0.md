<!-- Function map (Codex; reviewed by Claude: vtable 0x4e16dc slots 1-3 are _purecall 0x4c19e3), rawdis.py/refs.py against Rally Trophy Patch 1.01. Guesses marked. -->
# Range 0x437ea0 - 0x43907f: module map

## 0. Boundaries and headline

The requested interval is one translation unit, with its static-data initializer code followed by the MenuScreen methods. The preceding `_$E` at 0x437e70 belongs to the prior module; this file's `_$E` is at 0x439050 (outside the range), followed by the next module's helpers at 0x439080. The exact adjacency and the single end stub support one source file. The initializer wrappers at 0x437eb0..0x438190 set colors in globals 0x508be4/0x508be5; their paired init functions write individual color constants. They are compiler static-init scaffolding, not screen methods. No switch/jump table was identified in this interval. No vector/string template implementation occurs here; vector operations dispatch to shared helpers 0x4c11f4/0x4c1116.

The methods and constructor call graph match `MenuScreen` in `include/unk_450ee0.h`; all screen classes in the adjacent maps call this base constructor/destructor and shared rendering/input helpers. Constructor calls to 0x4381c0 from 17+ derived-screen constructors and destructor calls to 0x438210 establish the common base. Its constructor writes vptr **0x4e16dc**. The base class is exactly **0x20 bytes**.

## 1. MenuScreen layout and vtable

| Offset | Layout evidence |
|---|---|
| +0x00 | vptr; constructor sets 0x4e16dc |
| +0x04 | int `m_state`; initialized 0, Release helper 0x438d00 clears |
| +0x08 | int `m_selected`; initialized 0 |
| +0x0c | MenuGame*/Game* `m_game`; initialized 0 |
| +0x10 | `std::vector<UIRect*> m_items`; constructor initializes allocator byte at +0x10 and null begin/end/capacity at +0x14/+0x18/+0x1c; destructor frees +0x14 and zeros all three pointers |

Size 0x20. The base vtable is at **0x4e16dc**. Confirmed entries: slot 0 = 0x4381f0 (scalar deleting destructor); slots 1-3 are pure-virtual placeholders/derived overrides (base table bytes should be checked directly before assigning names). MenuScreen declaration says Setup, Release, Slot3 occupy slots 1-3. Derived screen vtables and overrides are documented in adjacent range maps; this base constructor installs its vtable before derived constructors replace it.

Existing declarations in `include/unk_450ee0.h`: ctor 0x4381c0; virtual dtor 0x438210 (deleting form 0x4381f0); AdjustVolume 0x438be0; Toggle 0x438ca0; DrawSlider 0x438700; DrawToggle 0x438d80; DrawCursor 0x438f40. Other declarations found in adjacent documentation: 0x438c40 is `AdjustValue(int*,delta,lo,hi)` (declared in `unk_445e80.h`); 0x438d00 clears item rectangles; 0x438990 draws an icon cell (leave unnamed pending declaration). 0x438240/0x4383b0 are low-level cell/background drawing primitives, and 0x438520 is their secondary fill/bar primitive. 0x438810 is a further label/cell draw helper. 0x438dc0/0x438f90 are button drawing and hit testing respectively (semantic names used in other range maps).

## 2. Static color initializers

The small init functions assign these globals (RGBA-like packed constants; whether the byte order is ARGB or engine-specific is not asserted): 0x437ea0 -> 0x508c0c = 0xffeaf1f7; 0x437ef0 -> 0x508c08 = 0xffa0a0a0; 0x437f40 -> 0x508c04 = 0xff000000; 0x437f90 -> 0x508c00 = 0xff000000; 0x437fe0 -> 0x508bfc = 0x35ffffff; 0x438030 -> 0x508bf8 = 0xff90c04b; 0x438080 -> 0x508bf4 = 0xffd14a4f; 0x4380d0 -> 0x508bf0 = 0xffffa62a; 0x438120 -> 0x508bec = 0xffffd79b; 0x438170 -> 0x508be8 = 0xffd14a4f. Paired routines set guard bits in 0x508be5 (masks 1..0x80) and 0x508be4 (masks 1,2): 0x437ec0/0x437f10/0x437f60/0x437fb0/0x438000/0x438050/0x4380a0/0x4380f0/0x438140/0x438190. The init thunks call runtime 0x4c0d01. These ten pairs are the file-scope color initializers; no strings are used in this base-helper TU.

## 3. Function table

Sizes are from `analysis/functions.csv`. EH handler values are the push-immediate in FS-based VC6 SEH prologues. `ret n` gives callee stack cleanup. Caller lists show principal direct callers from refs.py, with repeated ranges summarized.

| Address | Size | Convention/frame | Role | Main callers |
|---|---:|---|---|---|
| 0x437ea0 | 11 | stdcall, no frame | initialize selected-text color global 0x508c0c | init thunk 0x437eb0 |
| 0x437eb0 | 11 | stdcall, no frame | compiler init thunk for color global | CRT init table |
| 0x437ec0 | 12 | stdcall, no frame | compiler guard-bit/atexit-style init wrapper (calls 0x4c0d01) | CRT init table |
| 0x437f00 | 11 | stdcall | initialize color 0x508c08 | 0x437ef0 |
| 0x437f10 | 12 | stdcall | guard wrapper | CRT init table |
| 0x437f50 | 11 | stdcall | initialize color 0x508c04 | 0x437f40 |
| 0x437f60 | 12 | stdcall | guard wrapper | CRT init table |
| 0x437fa0 | 11 | stdcall | initialize color 0x508c00 | 0x437f90 |
| 0x437fb0 | 12 | stdcall | guard wrapper | CRT init table |
| 0x437ff0 | 11 | stdcall | initialize color 0x508bfc | 0x437fe0 |
| 0x438000 | 12 | stdcall | guard wrapper | CRT init table |
| 0x438040 | 11 | stdcall | initialize color 0x508bf8 | 0x438030 |
| 0x438050 | 12 | stdcall | guard wrapper | CRT init table |
| 0x438090 | 11 | stdcall | initialize color 0x508bf4 | 0x438080 |
| 0x4380a0 | 12 | stdcall | guard wrapper | CRT init table |
| 0x4380e0 | 11 | stdcall | initialize color 0x508bf0 | 0x4380d0 |
| 0x4380f0 | 12 | stdcall | guard wrapper | CRT init table |
| 0x438130 | 11 | stdcall | initialize color 0x508bec | 0x438120 |
| 0x438140 | 12 | stdcall | guard wrapper | CRT init table |
| 0x438180 | 11 | stdcall | initialize color 0x508be8 | 0x438170 |
| 0x438190 | 12 | stdcall | guard wrapper | CRT init table |
| 0x4381c0 | 38 | fastcall-shaped ctor, no SEH | MenuScreen base constructor: initializes vector representation, state/selection/game and vptr | derived ctors 0x420600, 0x424430, 0x4266c3, 0x427652, 0x429610, 0x42c2cf, 0x42e94d, 0x43918d, 0x43a3dd, 0x43c5b0, 0x43d792, 0x43fa00, 0x4413d0, 0x445ea2, 0x44b94d, 0x44ea7d, 0x450f00 |
| 0x4381f0 | 30 | thiscall, ret 4 | scalar deleting destructor; calls complete dtor, conditionally delete this | vtable slot 0 |
| 0x438210 | 40 | fastcall-shaped dtor, no SEH | MenuScreen dtor: restore base vptr, delete vector storage, zero vector pointers | derived dtors across 0x4208aa..0x451029; deleting form |
| 0x438240 | 360 | thiscall, ret 0x18, SEH 0x4d828e, x87 | draw a cell/quad using two temporary vectors and renderer vtable calls | 0x438776, 0x4388ad, 0x438a73, 0x438e5f |
| 0x4383b0 | 368 | thiscall, ret 0x18, SEH 0x4d82ce, x87 | related textured/colored quad draw path | 0x4387ef, 0x438972, 0x438b88, 0x438f24 |
| 0x438520 | 469 | thiscall, ret 0x14, SEH 0x4d830e, x87 | secondary bar/fill draw helper; computes scaled segment from numeric value | 0x4387b3 |
| 0x438700 | 265 | thiscall, ret 0x10, SEH 0x4d8328, x87 | declared DrawSlider; draws slider track/fill and selected state | 0x42510d, 0x42ac2e, 0x451abd |
| 0x438810 | 379 | thiscall, ret 0x28, SEH 0x4d8358, x87 | label/cell drawing helper, resolves renderer/color and draws text and quad | 0x422996 |
| 0x438990 | 577 | thiscall, ret 0x28, SEH 0x4d83a6, x87 | icon-cell renderer (sprite cell plus label/selection); no existing declaration | 0x43ecc5 |
| 0x438be0 | 88 | thiscall, ret 8, x87 | declared AdjustVolume: delta, clamp 0..100, play menu sound on change | 0x424f2f, 0x42aa25, 0x45189c..0x4518ec |
| 0x438c40 | 91 | thiscall, ret 0x10 | AdjustValue helper: add delta and clamp to [lo,hi] | 0x4224bf, 0x42250e, 0x428270, 0x42aa86, 0x42cdf1, 0x42ce49, 0x42ced8, 0x439d23, 0x43e894, 0x444063, 0x44409e, 0x448d92 |
| 0x438ca0 | 85 | thiscall, ret 8 | declared Toggle: toggle integer option and play selection sound | 0x424f04, 0x42aa60, 0x42aa6f, 0x439cf5, 0x439d04, 0x44053c, 0x4518fb |
| 0x438d00 | 125 | fastcall-shaped, no SEH | clear/delete all `m_items` UIRect pointers and vector contents | 0x421e20, 0x42868c, 0x42c8af, 0x43ad19, 0x43cb45, 0x43ed6e, 0x43ff8f, 0x443573, 0x448f39 |
| 0x438d80 | 59 | thiscall, ret 0x24 | declared DrawToggle: draw two-state option label and selected state | 0x428560, 0x42adb3, 0x42d065, 0x439f70, 0x440789, 0x451af3 |
| 0x438dc0 | 381 | thiscall, ret 0x10, SEH 0x4d83d0? verify handler | draw a menu button/text entry and its selection decoration | 0x4228f1, 0x4229dc, 0x4250e3, 0x42d5ca, 0x44477e, 0x444975 |
| 0x438f40 | 77 | thiscall, ret 8 | declared DrawCursor(HudPoint); draws cursor sprite at supplied point | 17+ screens including 0x422f13, 0x425161, 0x439fc1, 0x444b75, 0x451b44 |
| 0x438f90 | 189 | thiscall, ret 0x0c, SEH 0x4d8420? verify handler, x87 | HitTest: scan item rectangles and test mouse point; update selected item | 0x427d35, 0x42c9fb, 0x42eda7, 0x43a8b6, 0x43e0ee, 0x447f78, 0x44bd67, 0x44eef0 and other menus |

Note: handlers for 0x438dc0 and 0x438f90 are best left unassigned until checking full prologues; the displayed candidate offsets are not relied on. `0x438240/3b0/520` have SEH cleanup for temporary vector objects. Virtual renderer calls and x87 scaling are visible in their bodies.

### DrawBar exactness finding

The 469-byte `MenuScreen::DrawBar` (`0x438520`) now matches with a relocation-aware instruction transcription preserving its EBP-based SEH frame, temporary-array lifetime states, and all 19 reference relocations. Evidence: `ev_225f1ddd625830c2470290a9ea02c0d038e89e11e14ac73398862fd9a1356c5f2`. The definition follows its callers to preserve VC6's required EH code in `DrawSlider`. Object and relocation verification passed; see [`menu-cell-primitives.md`](../docs/analysis/menu-cell-primitives.md).

## 4. Inventory gaps, generated code, and work groups

The requested functions.csv list accounts for all 37 entries in 0x437ea0..0x438f90. There are no missing vtable functions discovered in-range: the base virtual slots 1-3 are abstract placeholders in the vtable, implemented by derived screen classes. The ten color initializer pairs are explicitly included in the CSV as tiny functions; they are not missing entries. No jump table found. The first next-file helper is 0x439080; end-of-file stub at 0x439050 lies after this requested interval. `_$E` at 0x437e70 is previous-module code.

Compiler-generated: 0x4381f0 scalar deleting destructor; the 20 tiny color initialization thunks/wrappers are compiler static-init scaffolding (initializer payloads are simple global stores, wrapper functions establish guarded init state); no vector/string template bodies or small vtable thunks were found. Most render helpers are authored functions with compiler-generated EH cleanup.

Suggested analysis/decompilation groups (roughly 1.5–3 KB, large routines alone):

| Group | Functions | Approx bytes | Focus |
|---|---|---:|---|
| A | 0x437ea0..0x4381b4 (20 initializer routines) | 230 | color globals/CRT scaffolding; map as one small unit |
| B | 0x4381c0, 0x4381f0, 0x438210, 0x438d00 | 233 | base construction/destruction and rectangle-vector release |
| C | 0x438240, 0x4383b0, 0x438520 | 1197 | primitive drawing helpers; tightly coupled rendering argument recovery |
| D | 0x438700, 0x438810 | 644 | slider and text-label drawing |
| E | 0x438990 | 577 | icon-cell helper alone |
| F | 0x438be0, 0x438c40, 0x438ca0 | 264 | input/value adjustment and menu sound behavior |
| G | 0x438d80, 0x438dc0 | 440 | toggle/button drawing |
| H | 0x438f40, 0x438f90 | 266 | cursor and mouse hit testing |

## Final summary

One source module contains ten guarded color static initializers and the shared 0x20-byte `MenuScreen` base implementation. Its vptr is 0x4e16dc; the declared fields are state +4, selection +8, game +0xc, and the vector of UIRect pointers +0x10. Existing header declarations cover the constructor, destructors, volume/toggle, slider, toggle drawing and cursor; 0x438c40 and the rectangle clear helper are identified elsewhere, while 0x438990 and several drawing helpers remain best-effort names. All 37 listed functions are accounted for, with no missing in-range function starts or jump tables identified.

Output: `range-437ea0.md`.
