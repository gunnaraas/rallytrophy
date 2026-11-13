# RallyTrophy.exe 1.01: analysis notes

Target: `orig/RallyTrophy.exe`, the official Patch 1.01 build
(sha256 `2d5b2632a6467edb31eb43e8decf237d47e11d62e9b6f37bf8d528e4ad0d83b7`, image base `0x400000`).

## Toolchain (from the Rich header)

| Objects | Tool | Build | Meaning |
|---|---|---|---|
| 157 C++, 4 C | cl (Utc12) | 12.00.8966 | **VC6 SP5**: the game code |
| 1 C | cl (Utc12 PP) | 12.00.8943 | VC6 SP4 Processor Pack: one object |
| 153 C, 20 C++ | cl | 12.00.8047 | VC6 static libraries (CRT) |
| 1 C, 14 C++ | cl | 12.00.8168 | VC6 RTM: a prebuilt third-party or SDK lib |
| 35 | MASM | 6.13.7299 | CRT assembly |
| 16 | implib | 7.00.9210 | Import libraries from a newer SDK |
| 1 / 3 | VS2002 C++ / linker | 8830 / 8755 | SDK static libs (probably DirectX 8 SDK) |
| - | link | 6.00.8447 | **VC6 SP5 linker** |
| 1 | cvtres | 5.00.1735 | Resources |

Build IDs looked up in [richprint's comp_id.txt](https://github.com/dishather/richprint/blob/master/comp_id.txt).
To match, build with **Visual C++ 6.0 SP5** (`cl` 12.00.8966, `link` 6.00.8447).

## SecuROM

- The PE entry point (`0x591fdc`) is SecuROM's loader in `.cms_t`/`.cms_d`, which has its own statically linked CRT. It isn't game code; ignore it.
- `0x4c0000`–`0x4c4fff` (20 KB) of `.text` is encrypted. It contains the real entry point `0x4c2652` (`WinMainCRTStartup`) and the surrounding VC6 CRT functions (all unaligned). It's library code, so it isn't decompiled.
- Every import call in game code (`call [__imp_X]`) was redirected to one dispatcher slot, `0x59b2f8`. [`api_callsites.csv`](api_callsites.csv) maps all 304 sites to their real API; `tools/securom_api_map.py` generated it from the official exe plus a locally prepared reference exe with the same code and a resolved import table.
- Matching compares the pre-SecuROM code. Import-call operands are relocations, so they're masked anyway.

## Analysis database


Analyse a locally prepared reference exe with the same code at the same addresses but a resolved import table, so Ghidra types every API call. Analysing the official exe produces broken stack tracking around the ~300 dispatcher calls. **Match against the official exe.** The reference exe is not part of this repository.

[`functions.csv`](functions.csv) is the full Ghidra function inventory (entry, size, alignment, name source), exported by [`../tools/ghidra/ExportFunctions.java`](../tools/ghidra/ExportFunctions.java).

## Build and matching environment

- **Toolchain** (`toolchain/`): VC6 Professional (1999 MSDN disc) with the SP5 English files laid on top, all names lowercased. The SP5 code generator is hidden on the disc as `enu/os/system/msvcep.dll` and installs as `c2.dll` 12.0.8966. Versions: cl 12.00.8804 (driver), c1 8867, c1xx 8964, **c2 8966**, link 6.00.8447. Compiled objects carry `@comp.id` 0x000A2306 (C) and 0x000B2306 (C++), the same as the game's Rich header.
- The VS6 disc stores some headers under 8.3 names that its installer renames; the toolchain now has the long names too (`algorithm`, `exception`, `functional`, `stdexcept`, `streambuf`, `strstream`, plus a few COM/SDK files), taken from `SETUP/VS98PRO.INF`.
- **Flags: `/O2 /Oa /G6`** (C++ files also `/GX`). `/G6` fixed scheduling (`0x4a1ad0`); `/Oa` (assume no aliasing) was found by sweeping flags over the NONMATCHING set: it fixed three of them without breaking any existing match. Symptoms of a missing `/Oa`: same size, but a stack slot/register difference and reloads of values the original keeps cached. `RT_EXTRA_FLAGS="/Ox" python3 -I tools/match.py` runs such a sweep.
- `tools/vc6.sh <tool> args...` runs cl/link/dumpbin under Wine in distrobox `rt-vc6` (Wine prefix `~/.local/share/wineprefixes/rt-vc6`). It works from the host or inside the container.
- **Match loop:** `python3 -I tools/match.py [files]` compiles everything under `src/` and checks each `/* FUNCTION: RALLYTROPHY 0x<addr> */`-marked function against `orig/RallyTrophy.exe` with `tools/objcmp.py`. Linker-filled fields are masked, and a disassembly diff is printed on mismatch.
- **NONMATCHING:** mark a function `// NONMATCHING: RALLYTROPHY 0x<addr>` (with a note on what differs and what was tried) when it's logically right but not byte-exact after a time-box. match.py compiles and reports it without failing.
- **`tools/try_variants.py SOURCE 0xADDR variant.cpp...`** swaps alternative definitions of one function in turn (each variant file holds a full definition with its marker) and reports MATCH/DIFF for each, without touching the source.
- reccmp 0.1.7 can be installed in an optional local virtual environment for whole-exe comparison once there's a linked build, using the reference exe above as its "original" so import calls resolve to names. The environment is not tracked.

## Code layout (`.text`)

| Range | Contents |
|---|---|
| `0x401000`–`0x4bcfc6` | **Game code**: about 1,690 functions, 714 KB, all 16-byte aligned (exception: `0x4a4b06` is the DirectInput8Create import thunk) |
| `0x4bd000`–end | VC6 static libraries: CRT, C++ standard library, old `iostream.h` library, EH helpers, math. Starts with `operator delete` (`0x4bd000`). Linked from LIBC/LIBCP/LIBCI; not decompiled |

RTTI is **off** (`/GR-`). Type descriptors exist only for thrown types: `exception`, `file_error`, `assertion_error` and the standard exceptions. The release build has no `__FILE__` strings, and none of the shipped binaries has a PDB path.

`Device/bbdx8.dll` (the D3D8 renderer plugin) has the same VC6 SP5 compiler plus VC6 RTM (8168) objects, like the game. The 15 RTM objects are probably a shared Bugbear base library.

## Landmarks

| Address | What |
|---|---|
| `0x4c2652` | `WinMainCRTStartup` (encrypted in the official build) |
| `0x4a1b40` | `WinMain` |
| `0x4a3390` | Display-mode / renderer init: creates the "Rally Trophy" window through the device-plugin interface |
| `0x4a1770`–`0x4a1ab0` | Global-object initialisers in WinMain's module, including the display-mode list at `0x56b968` (64 × 20-byte entries) |
| `0x4a1820`, `0x4a4170`–`0x4a43b0` | Start-up registration check and its dialog |
| `0x4a4440`/`0x4a45c0` | Settings file load/save (`Settings` at `0x56b8c8`, 0x98 bytes, magic 0x20011015) |
| `0x4a4660`/`0x4a4870` | Progress file load/save (`Progress` at `0x56b700`, 0x1c8 bytes, magic 0x20011217); snapshot of option blocks `0x501300`–`0x5014ef` |
| `0x4a1b40` | `WinMain`: settings → progress → key dialog → setup dialog (`0x4a1fe0`) → display init (`0x4a3390`) → game flow (`0x4a3ca0`), all inside nested try/catch |
| `0x4a3390` | Display init: window (device slot 0x2c, retried without flag bit 0), render states 5-9, 1/1024 and 1/768 scale factors, 24 shaders `data/shaders/*.sha` into slots 1-24 via `0x4a3390`→`0x4a3930`, device feature probes, then creates input/sound/game subsystems |
| `0x4a3ca0` | Game flow: JoWooD, Bugbear and intro videos (`data/video/*.bik`), loading screen, `Game` object (0x139894 bytes), then saves `data/options.cfg` |
| `0x4a1fe0` | Setup: loads `device/bbdx8.dll` (`T_56b980`), lists adapters into `Array<AdapterEntry>` at `0x56b968`, runs the setup dialog (resource 0x65, proc `0x4a2440`) |
| `0x404c80`–`0x40555f` | Language and text table (`src/language.cpp`): 20 languages, 419 texts, a ';'-statement / '#'-comment text format parser |
| `0x4bbdf0`–`0x4bc1db` | Renderer plugin loader `T_56b980`: LoadLibrary("device/bbdx8.dll"), `BeGetDevices()` export returns the plugin's `List<RenderDevice*>` |
| `0x57ba28` | Key states (256 bytes, indexed by virtual-key code); cleared by `T_56b0bc::ClearKeys()` (`0x4b7390`) |
| `0x4b7520` | `Timer` class (ctor/dtor/Update/Reset/GetCycles, matched) |
| `0x59b2f8` | SecuROM API dispatcher slot (see `api_callsites.csv`) |

### Library functions (VC6 CRT, called from game code)

| Address | Function |
|---|---|
| `0x4bd000` | `operator delete` |
| `0x4c1276` | `operator new` |
| `0x4c0d01` | `atexit` |
| `0x4c198b` | `srand` |
| `0x4c2414` | `wcslen` |
| `0x4ce290` | `_stricmp` |

### Decomp conventions learned so far

- An aligned switch table can follow Ghidra's body size after padding; compare its relocated entries too (mouse callback `0x4b6c10`). `objcmp.py` requires matching short padding, corresponding table relocations, and a known next-function boundary; verify local table targets independently.
- Source files are C++ (`.cpp`, `/GX`). Unknown modules are named `src/unk_<first address>.cpp` until we know what they are; functions inside stay in address order.
- Each global object with a constructor produces a 10-byte initialiser (`mov ecx, offset obj; jmp ctor`) and a 12-byte `atexit` registration of its destructor thunk. These are compiler-generated, so declaring the globals in the right order is enough to reproduce them.
- **Inline accessors:** Bugbear's classes use small inline getters (`GetWidth()`, `GetArg(i)`...). They change register allocation: a getter's result lands in a register (e.g. then spilled to one shared temp before `fild`), and a field read through a getter may be reloaded into a different register. When only registers/temps differ around member reads, try a getter. Likewise a computed call argument may have been a named temporary (`int value = x ? 12 : 0; f(9, value);`).
- **Store order = source order** (with `/Oa`): when a function's only difference is the order of stores into a struct/locals, write the assignments in the order the original performs the stores (see `0x4a1fe0`).
- Declaration order affects VC6 register allocation: in `0x4a49b0`, a local declared early but assigned after other locals was needed for an exact match. Try that when only registers differ.
- `delete p` on a class with a non-virtual destructor compiles to `mov esi,p; test; je; mov ecx,esi; call dtor; push esi; call operator delete`.
- Unknown classes are named after the global that holds them (`T_56b0bc`) until their purpose is known; vtables are declared with placeholder slots `vfNN` (slot N at `+4*N`).
- Message handlers compiled to `cmp`/`jne` chains were `if`/`else if`; a `switch` with consecutive cases compiles to `sub`/`dec` chains instead.
- Block layout: for `if (!x) { A } else { B }`, VC6 emits B inline and moves A out of line after the epilogue. A function whose failure path sits after the `ret` likely used this form (see `0x4a4660`).
- Functions Ghidra didn't find (only referenced as data, e.g. dialog procedures) go in `analysis/functions_extra.csv` (entry,size,note) so match.py knows their size.
- **Identical-code folding:** the linker merged identical functions, so e.g. `0x402c90` is every empty function at once (the release-build debug logger `FUN_00402c90(fmt, ...)`, trivial constructors/destructors passed to the vector iterators). Calls to it are calls to some empty function, not necessarily the same one in source.
- **C++ exceptions:** an `ebp` frame with `mov [ebp-0x10], esp` means a `try` block. The catch clauses, their order and types can be read from the FuncInfo the handler thunk loads (`mov eax, FuncInfo; jmp __CxxFrameHandler`): magic 0x19930520, unwind map, try map → HandlerType {adjectives, TypeDescriptor, catch object offset, handler}. Type names: `.PAD` = `char*`, `.H` = `int`. The catch handlers follow the function body; objcmp compares them too. `0x4c13ca(0, 0)` is `throw;`.
- **`switch` case order:** case bodies are emitted in source order (not numeric). Recover the order from the case blocks' stores and write the cases in that order. A `switch` mapping with `case 0 / case 1 / default` shows up as `sub eax,0; je ...; dec eax; je ...`.
- Dialog notifications: check the actual code value (e.g. 9 = `CBN_SELENDOK`, not `CBN_SELCHANGE` = 1); Ghidra doesn't name them.
- A function that walks two lists with one counter (Ghidra shows the same variable/parameter slot for both loops) reused one `i` in the source.
- A `switch` with a few non-consecutive cases (e.g. -1 and 1) compiles to a `cmp`/`je` chain with the default case inline.
- When a variable is set to a constant on one path, VC6 drops later tests of it on that path (e.g. `DAT_0057ba20` right after zeroing it). Don't add an `else` for that.
- The game has its own growable array template (`Array<T>` in rallytrophy.h: data, end, count, capacity, grow; inline `Add` reallocates with `operator new` + `memcpy` + `operator delete`). Spot it by the `count == capacity` check followed by `new((capacity + grow) * size)`.
- **`std::string` is used** (VC6 STL, reference counted: a count byte at `_Ptr[-1]`, 0/0xFF meaning unshared/frozen). Inline `assign`/`append` show up as "len > npos-2 → `_Xlen` (`0x4bd00b`)", then `_Grow` (`0x402980`) / `_Tidy` (`0x402700`) / `_Copy` (`0x402ab0`). Write plain `std::string` operations (`=`, `+=`, `c_str()`); `c_str()` is the "ptr ? ptr : \"\"" pattern.
- **Thrown types:** `push offset ThrowInfo; ...; call __CxxThrowException@8`. ThrowInfo = {attributes, destructor, forwardCompat, CatchableTypeArray}; each CatchableType = {properties, TypeDescriptor (name at +8), mdisp, pdisp, vdisp, size, copy ctor}. The first type is the thrown class (e.g. `std::runtime_error` in Config::GetInt).
- `std::vector::resize(0)` shows up as a size check, a zero stored to a stack slot (the default `T()` argument), then an inline erase/copy loop.
- **Inlining budget:** VC6 `/O2` inlines STL members (e.g. `std::string::compare`) in some functions but not others. For Config accessors, direct vector indexing reproduces the original out-of-line comparison; caching begin() changes inlining and register allocation. See loop 15.
- `.ini` files are read through `Config` (rallytrophy.h): Define(key table, entry count), Load, GetInt/GetString, Close.
- The game also has its own linked list `List<T>` (rallytrophy.h): nodes {next, prev, data}, list {head, tail, free, count, freeCount}; removed nodes go to a free list that the inline destructor deletes. Spot inline `RemoveHead`/`Remove` by "node->next = free; node->prev = 0; free = node; freeCount++; count--". `AddTail` (`0x4aecf0`) and `RemoveNode` (`0x414490`) are out-of-line instances shared by all pointer types (identical-code folding).
- **Keep original bugs.** A matching decomp reproduces them; mark them with a comment (e.g. `T_56b980::FUN_004bbff0` frees the wrong module).
- Inline `strcmp` (byte-pair loop ending in `sbb eax,eax; sbb eax,-1`) and `memcmp` (`repz cmpsd`) come from `/O2` intrinsics: write plain `strcmp`/`memcmp` calls.
- Windows `min`/`max` macros show up as duplicated comparisons (see `Timer::Timer`).
- A WinMain-module file starts with guarded initialisers for header statics (guard byte `0x5826d5`), so `0x4a1670` is the start of winmain.cpp.
- VC6 may store a local into a dead parameter's stack slot (e.g. a checksum local in `0x4a4170`); that's normal codegen, not a hint about the source.
- Loops whose result is only assigned at the exits came from `for (;;) { ... result = X; break; }`.
- An `rdtsc` in inline assembly is written with `_emit 0x0f` / `_emit 0x31` (VC6 SP5's inline assembler doesn't know the mnemonic).
- `0x4b73c0`–`0x4b751f` are VC6 STL `std::deque` template internals (4 KB blocks). They're skipped for now.

Distilled from the per-loop history; "(A: X)" means `NOTES-archive.md`, section X.

- Test header experiments in a scratch copy of the repository; concurrent full runs can race on `build/obj` (rerun) (A: Sound system, iteration 2).
- Shared-header edits can change unrelated files (setup dialog `0x4a2440`): keep module classes in their own header, run the full check after header edits (A: Sound system, iteration 2; Game controllers T_56b0b4).
- Renderer vtable calls: use a module-local class with only the needed slots, not a wider RenderDevice (A: Game controllers, iteration 8).
- A class too entangled to widen can be redeclared under another name with `// SYMBOL:` (CarObject); per-file view structs cast from `this` keep parallel edits mergeable (A: Derived race modes).
- Merge drafts by splitting on the FUNCTION/NONMATCHING markers, then re-read the `_$E` numbers (A: Networking T_56b0b8; Derived race modes).
- match.py masks relocated .rdata constants: a float literal one ulp off still MATCHes; check bit patterns (1/768 = `0.0013020834f`) (A: Derived race modes).
- `/FAs` shows frame slots; compile many variants as members of throwaway derived classes in one TU (A: Derived race modes).
- Store order is a first guess: `0x4989c0` matched only via a 720-order try_variants search (A: Sound system T_56b0b0).
- **Generated code.** Mark at file end: `// FUNCTION:` + `// SYMBOL: <COFF name>` + `typedef void Generated_<addr>();`; find names with `objdump -t` (A: Marking compiler-generated functions).
- Module-end stubs (guard bit 0 of `0x5826d5`, `atexit(0x402c90)`, 25 + 12 bytes) are the STL `_$E<n>` pair, one per .cpp; only TUs whose includes instantiate STL statics emit it (A: Derived race modes).
- `_$E` numbers depend on the whole TU (static guards, reference-bound temporaries use counters); module-end guard = higher number (A: Derived race modes).
- File-scope objects with inline ctor + empty dtor give four `_$E` COMDATs; static data member arrays give guarded dtor iterators (A: Networking T_56b0b8; Derived race modes).
- Vtable-only functions are often missing from functions.csv: dump vtables (A: Networking T_56b0b8). A template instance inside a range may belong elsewhere (A: Marking compiler-generated functions).
- **EH states.** Members/locals with a non-trivial (even empty) dtor get unwind states; the ctor's unwind table recovers layouts (Vec3 vs float[3]) (A: Sound system T_56b0b0; Derived race modes).
- By-value class params with a dtor take EH state 0 in the callee; with an out-of-line copy ctor and no dtor, none (A: Networking T_56b0b8; Derived race modes).
- A state store vanishes when an earlier-defined callee can't throw: leave callees undefined, keep source order = address order (A: Derived race modes).
- An EH frame in a trivial ctor can be `resize(0)`'s default-argument temp; `v.resize(0)`, not `clear()`, empties vectors (A: Game controllers, iteration 2; Derived race modes).
- Read FuncInfo (handler's `mov eax, imm32`) for EH locals and offsets (A: Derived race modes).
- **Inline budget / STL.** Early std::string/vector sites stay calls, later ones expand: pin sites with declared-only wrappers or hand expansion using `char_traits<char>::copy` (A: Derived race modes).
- Out-of-line STL calls in inlined code: derived wrapper defined under `#pragma auto_inline(off)` (A: Networking T_56b0b8).
- `push_back` vs `insert(end(), x)` via a local `vector&`, and element ctors, change inlining; a dummy inline call shifts the cutoff (A: Sound system T_56b0b0; Derived race modes).
- `std::string s("lit"); s += x` keeps `_Grow`/`_Eos` out of line; `= "lit"` inlines `_Eos`; `a = b` calls `assign(src,0,npos)` 0x4024f0 (A: Derived race modes).
- Allocator-byte read `mov al,[esp+X]` + out-of-line `_Tidy`: layout twin with `Ctor(const allocator& a = allocator()) : alloc(a) { Tidy(false); }` (A: Derived race modes).
- `resize(n)` / `+= c` are erase 0x408de0 / append(count, c) 0x408d10; string arrays copied via 0x408a80 need a layout twin (A: Derived race modes).
- **Floats.** `static const float` objects aren't folded (literals are); `literal * scale` order needs a non-const `static float`; `mov reg,[.rdata]` member stores come from an extern const (A: Derived race modes).
- fld/fstp float copies pass through inline float parameters (ctors, `SetScale`, `PassFloat`) (A: Networking T_56b0b8; Derived race modes).
- `(float)fabs(x)` per use; `(float)cos/sin` inline; sign tests may be integer (`*(const int*)&a < ...`) (A: Game controllers T_56b0b4; Derived race modes).
- VC6 re-associates float products: split into locals or `r *= e` statements; `x / 0.25f` becomes a multiply unless k is static const (A: Derived race modes).
- Inline float args go right to left onto x87; accumulator declaration order sets the x87 layout; `x*x+y*y+z*z` is emitted z, y, x (A: Networking T_56b0b8; Derived race modes).
- **Frames.** /O2 slots follow weighted reference counts; aggregates go below scalars; sibling-block locals overlap unless declared at function scope (A: Networking T_56b0b8; Derived race modes).
- The most-used constant takes a callee-saved register; open: shared fild temp slot (A: Networking T_56b0b8; Derived race modes).
- **-1 colour in a register** (`or edi,-1`, slot store, `push edi`, 4-byte larger frame): the callee takes `Color` *by value* and the caller passes the `Color` object (`f(..., color, ...)`, not `color.value`). Renderer slots 0x108/0x10c: call through `RaceDeviceSlots` (`DrawQuad((Vec2*)pos, (Vec2*)uv, color, ...)`); declaring `Color` in rallytrophy.h breaks 0x4a2440 (any extra type there does) (MenuScreen 0x438990).
- Cached `Player* p` vs repeated `m_players[i]->`, named pointer locals and `#define V ((View*)this)` change register choice (A: Networking T_56b0b8; Derived race modes).
- **Statements.** `a = b = v` stores the right target first; `G = m = v` stores the member first (A: Networking T_56b0b8; Derived race modes).
- `if (x != K)` vs the inverted test swaps blocks; read jump tables for shared case bodies (an empty case turns a table into a chain) (A: Game controllers, iterations 2-3).
- Constant ternaries compile branchless; the original often used if/else (A: Networking T_56b0b8; Derived race modes).
- Loops: `for (done = 0; done < dt;) { ...; done += step; }` keeps the shape; increment order matters; NULL-terminated arrays use an index (A: Derived race modes; Game controllers, iteration 4).
- `int r = 0; ...; return r` keeps 0 in a register; `test eax,eax` callers mean int, not bool; `bool b = (x == y)` gives `sete al; test al,al` (A: Derived race modes).
- Condition chains must read the member, not a local copy (A: Derived race modes). A call computed inside an argument list runs last: name it (A: Networking T_56b0b8).
- `x--` is `dec [m]`; flags-word load/or/store needs `*(volatile unsigned*)&f |= N`; `BYTE = int` narrows loads (A: Derived race modes).
- `memset` of small structs gives stores, larger `rep stos`; `= {0}` differs; 44-byte struct copies are `rep movsd`; /Oa elides copy-in/out temps (A: Game controllers, iteration 3; Networking T_56b0b8; Derived race modes).
- `delete p` adds its own null check; a shared empty dtor call needs an undefined out-of-line dtor (A: Networking T_56b0b8; Derived race modes).
- Mouse warps use an argument temporary with inline ctor and empty dtor (A: Derived race modes).
- **Types.** Empty-dtor classes (Color, Vec3) add EH states; use POD where the original has none; a user copy ctor on a small POD enables NRV (A: Derived race modes).
- `ObjectList<T>` (sound.h): out-of-line RemoveNode 0x414490, inlined AddTail (A: Sound system T_56b0b0). Bitfield clears: separate statements, not `&=` masks (A: Sound system, iteration 2).
- A thiscall that never reads `this` stays a member (A: Derived race modes).
- **Misc.** Any `__asm` forces an ebp frame and memory homes; `rdtsc` needs `_emit`; `far`/`small`/`big` are reserved or macros (A: Sound system, iteration 2; Derived race modes).
- **Inline asm helpers.** `inline` functions whose body is `__asm` are still inlined; their pointer arguments land in ebp slots that the asm reloads (`mov ecx,[ebp-x]`). That is the shape of the x87 transform/cross blocks (0x4150b0, 0x41b270, 0x41c540): write them as inline functions, not macros with block locals. Act on vector fields directly (`rec->dir -= ...`), not through a pointer local, or VC6 forwards the stored components instead of reloading them like the original.
- **Missing asm helpers.** If the original has an ebp frame (`push ebp; mov ebp,esp`) and the build does not, look for an inlined asm helper first: `fld [ebp+x]; fistp [ebp+y]` is `FloatToInt` (VC6 emits `__ftol` calls for casts, never `fistp`), and `fld [ecx]; fmul [edx]; fld [ecx]; fmul [edx+4]` is the Xform/Rot block. `tools/asm_helpers.py` lists NONMATCHING functions whose original has more of these patterns than the build (14 on 2026-10-30, 37 KB). Example: `FColor::Set` in `src/unk_420490.cpp` uses `(BYTE)v`; the asm form gives `DrawBook` 0x4239c0 the original frame and takes it from 278 to 219 differing bytes (A: Claude, 2026-10-30).
- **Float bit-compares.** `fstp [x]; mov reg,[x]; cmp`/`test` in the original is a compare of float bit patterns (`*(int*)&d2 <= *(int*)&rr`, `*(int*)&dot <= 0`), not an `(int)` cast; casts compile to `__ftol` calls. `tools/structural.py` groups NONMATCHING functions by frame, EH-state and call-count differences (A: Claude, 2026-10-30).
- **Judge structural fixes by instructions.** `try_variants.py` byte counts are positional, so a fix that changes size early can score worse; use its `insns differ` (aligned) count instead (A: Claude, 2026-10-30).
- **Double constants.** A `QWORD PTR` FPU operand in the original means a double literal or `extern double` (0.9999, 1.0001, 0.0001, `+ 0.01`, `1.0 / x`); a DWORD operand on a float constant can be a float global (`DAT_004e13e0` = 1e6) rather than a literal. `structural.py` tags these as `double` (A: Claude, 2026-10-30).
- **EH frame from a vector temporary.** An original EH frame with one state around an out-of-line vector op (0x414330 `operator-`, 0x419f00 `operator+`) comes from a 3-float type with an empty destructor that is live across the call; see `FrVec` in `src/unk_4ba440.cpp` (A: Claude, 2026-10-30).
- **Agent wave 2026-11-04 (Claude orchestrating Sonnet agents, one scratch workspace each via `tools/mkws.sh`).** Leads came from `tools/ehmap.py` (original unwind map) and `tools/calls.py` (call-sequence diff), not from byte counts. Findings: (1) the triage `eh` column is a false positive for non-EH functions that store constants to `[ebp-4]` (0x4578c0, 0x490050 have no EH frame; check ehmap first). (2) A funclet that calls a class destructor which itself destroys two 2x8-byte empty-dtor arrays (FUN_004a94c0) means ONE struct with inline ctor/dtor holding both arrays plus scalar members; its states are 0 (first array), 1 (whole object), 2 (second array dying). The dead tint/alpha stores in PrintWrapped are that struct's members, set in its constructor (`QuadPoints`, 0x4aa690: 529->371). (3) `calls.py` showing `??HScenePoint` where the build has `??HVec3` means the sum is a ScenePoint (0x41d7d0). (4) EH state numbers follow declaration order, the state *store* follows construction time: a local declared inside the block where it is first used gets its store right before that use; hoisted locals get one combined store before the loop (0x41d7d0 tmp/hit). (5) `v -= a * k` consumes the out-of-line `operator*` result directly; `Vec3 t = a * k; v.x -= t.x` adds a slot, a copy and a state (0x45c700). (6) `throw std::logic_error("lit")` whose original shows `_Grow` + `_Eos` + `exception("")` 0x4c12db + out-of-line `assign` is a `CStr` temp plus an `exception`-derived class with an out-of-line-assign string member (0x419f30 `ColLogicError`); int arrays zeroed with `rep stos` dword + `and ecx,3` byte tail are `memset(p, 0, n*4)`. (7) VC6 sinks state stores of consecutive empty-dtor locals to the next throwing call or scope join; a mid-function `mov [state],N` with no call nearby means a throw point or loop pop-target the source lacks (0x4193a0). (8) An unused empty-dtor local costs no frame space or code but adds an EH state (placeholder only; the original's object was used, find its role). (9) A `memcpy` whose byte count ends in `*2` always gives `shr 2 / rep movsd / and 3 / rep movsb` here; the original's `shr 1 / rep movsd / adc / rep movsw` (0x4aa690) is some other construct, unresolved after 11 spellings. (10) `T* e = member = new T[n];` with `int i = 0;` declared first makes VC6 use one register for the `new` result phi and the stored zero (0x420db0 prologue exact); `member = new ...; e = member;` does not. (11) Register-only near misses (0x4b8640, 0x4b6800, 0x491c10, 0x41b270) resisted another 30 cycles of declaration/loop-shape variants: do not assign them again without a new idea.
- **Agent wave 2 (2026-11-04, Claude orchestrating; targets from `tools/dossier.py`).** Nine agents on 16 functions chosen by unwind-map/call-sequence leads; 13 merged, 0 exact. Patterns confirmed across many functions: (1) **Out-of-line helpers return POD.** `Sub` 0x414330, `Vec3*RotMatrix` 0x4b5850 and the `*`/`+` operators return a plain struct read through eax; model them as a file-local POD (`struct PV {float x,y,z;}` + `/alternatename` to the real symbol, or `SubPod* Sub(SubPod*, ...)`) and declare a destructible `Vec3` only where the unwind map has an entry (AIDriver 0x49c130/0x49ffa0/0x4a0420, 0x4b3800). (2) **State-store placement.** `Vec3 h = a + b;` is NRV: `h` becomes the hidden-return temp and its store follows the call; the original's "store before the call" means `Vec3 h; h = a + b;` (0x470940). Consecutive empty-dtor locals share one store; declare one *separately* (or as the function's first statement) to get its own early store (0x4ad1d0, 0x49c130). A copy constructor (`Vec3 d(*(Vec3*)p)`) moves the store to the declaration; default-construct-then-assign defers it (0x41fc60). Nested ifs instead of `continue` give the shared "reset to state N" loop tail (0x41fc60). (3) **Arrays with a destructor** show as `push dtor; push count; push size` funclets: `Tri3 ta, tb` with empty ctor/dtor plus an out-of-line `Init()` (0x41ca70), `Vec3 corners[8]` (0x470940), `Vec3 a[3], b[3]` with an original that destroys them on every early `return` = a flat chain of `if (...) return 0;` tests, not nested ifs (0x4ad1d0). (4) **A function-local struct with inline ctor/dtor gets out-of-line ctor/dtor calls**; put empty-dtor vector types at file scope (0x4b9d30). (5) **Component-wise stores** `arr[i].x = src.x` give integer register copies; `arr[i] = Vec3(x,y,z)` gives fld/fstp pairs through the float parameters (about 180 bytes over 24 stores, 0x470940). (6) **String twins per local:** early locals destroy via out-of-line `_Tidy(true)` (`~S(){Tidy(true);}`), only the last local gets the inline refcount expansion (`~S(){Free(true);}`): one destructor-less base with two derived twins (0x4137c0); a "missing last state" can be a copied 16-byte empty-dtor value (`FlareRect rc = AT(FlareRect, off)`). (7) **Frame slots:** an array in its own `{ }` overlaps an earlier sibling block's arrays, one at function scope does not. A block-local array shares its pool slot with statement temporaries (e.g. the `resize(0)` default-argument temp) and sorts *below* them; a function-scope array is allocated in the parent frame so the temporaries sort *above* it. 0x495a60 MATCHED by moving `char number[32]` from the loop to function scope: try the real array at function scope before inventing pads or unused locals. (8) **Placeholders that stay:** state 7 of 0x418b90 has its funclet slot at ebp-0xc (the EH registration node), so an empty-dtor placeholder is the only way to get that state; 0x41ca70 keeps one `Vec3 dead` sharing a slot with `ex`. (9) An unwind entry whose address lies inside another object is a block-scoped local overlapping a sibling block's local (0x45c700 dir.y). (10) After the EH map, frame size and call sequence all equal the original, what is left is register roles (`this` in ebx/edi vs esi/ecx) and slot order; `volatile` on a loop pointer reproduced a spill once (0x418b90) but adds reloads. No source-shape idea moved those in 20+ cycles per function; the next lever is a tool that compares slot layouts/weighted reference counts, not more variants.
- **Agent wave 3 (2026-11-04; targets = frame-size mismatches with small inst delta + remaining unwind leads).** Two MATCHES by *frame-slot* reasoning: (a) 0x495a60: `char number[32]` moved from loop scope to function scope so the resize temp sorts above it; (b) 0x487b50: `char buffer[4]` (the original overflows it for 4+ digits; keep the bug), `text` pointer declared before `lap`/`laps` (edx/ecx load order), `sx = scale; sy = scale; sx *= A; sy *= B;` (`fld st(0); fmul m32` instead of `fld m32; fmul st,st(1)`). **Slot-map method:** translate each original `[esp+X]` operand to a frame-base offset by subtracting the pushes in flight (saved registers plus pending argument pushes); locals that land *inside* an aggregate's natural slot mean the original aggregate is smaller than natural (or shares a slot). `cl /FAs` on the build names the built slots. Other wave-3 findings: a `Vec` copy from a call result that the original stores straight to `[this+off]` via ecx/edx/eax is a scalar-int copy through `const int*`; a Vec4 copy that goes through `lea ecx; [ecx]..[ecx+0xc]` is a real struct copy (test per site, 0x411600). A global float read as `fld rdata; fmul [member]` is a literal in the original (`fld [member]; fmul rdata`): check the bit pattern against the exe before replacing (0x411600 1/255, 1000.0). Inline `Set4(float*, a,b,c,d)` reproduces right-to-left x87 argument evaluation with in-order stores. A loop-local `T v = f();` gets no EH state when nothing can throw after it; `T v; v = f();` does. `char* e = t.p; t.n = n; e[n] = 0;` gives `[ecx+eax]` where `t.p[n] = 0` gives `[eax+ecx]`. A constant-count copy loop over a stack temp becomes `rep movs` unless the source is read through a `volatile` pointer (0x44deb0). A named `int* fa = &info->firstArcade` with `*fa`/`fa[-1]` anchors the hoisted element pointer at a non-lowest field; `d = x + 1; s - d + e` keeps `inc; sub` where `s - (x + 1) + e` folds to `dec` (0x433bc0). Original frames where message/text buffers share esp offsets are one function-scope buffer/union, not sibling locals; a toy test showed VC6 does not overlap sibling-block *arrays* in that function, while 0x4137c0's `num[32]` in its own block did overlap the key arrays: treat sibling overlap as something to measure per function (0x4831c0, not merged: only a fitted 0x304 pad reached the frame size). Identical `delete[]; return 0;` exits always cross-jump once the epilogue has 4 pops (0x44deb0's three duplicated epilogues unresolved). `#define private public` around `<string>` reaches `_Grow`/`_Tidy` for a twin with out-of-line `_Eos` (0x411600; mangling of std::string members changes in that TU: replace with a cleaner twin when possible).
- **Per-file flag sweep (2026-11-01).** Full match.py passes with RT_EXTRA_FLAGS = /G5, /Oy-, /Op, /Ob2, /Ob0, /Ox, /O1, /Ow, /GB, /Oi-, /GF and /G5 /Oy-: no flag set gains a single match in any file; every flag that changes codegen loses 155-1077 existing matches (/Ox and /GF are no-ops). The 22 files with no exact function (so unconstrained by matches) improve by at most 0.5-4% of their differing bytes under any flag set, which is shuffle noise rather than convergence. The compiler build is pinned by the Rich header (c2 12.00.8966, VC6 SP5). The remaining NONMATCHING bytes are source-shape differences, not a toolchain or flag mismatch.
- **Checked cluster hypotheses (2026-10-30, A: Claude).** `structural.py` clusters after the fixes above: calls 45, frame 53, eh-states 19. calls: most deltas are naming artifacts (library `new`/`delete`, `Print`=0x4a97a0); std::string `_Tidy`/`_Grow` deltas run both ways and an `inline_depth` 0-255 sweep over 12 of them helps only TrackSelectMenu::Run (depth 1). frame: direction splits 28/25 with deltas 4-472 and no correlation with `volatile` hacks. eh-states: 10 orig-more vs 9 build-more; removing V3D's destructor helped ShadowUpdater by one state only. Permuter wins on EmitEffects (142->64) and RecordTable 0x490310 (70->52) reorder dependent statements and change behaviour; rejected.
- Ghidra pseudocode may run past a `throw`: take boundaries from the inventory (A: Loop 13; Loop 15).
- Separate a declaration from its assignment for store-order mismatches (A: Loop 13); member-initializer vs body assignment changes vptr store order (A: Loop 14).
- An empty user-declared derived dtor stores the derived vptr first; if the target skips that store, drop the dtor so VC6 generates it (e.g. `NamedDerived` 0x409720).
- EH-state mismatches: an early state-0 store before a base/member ctor means a destructible (often empty-dtor) member precedes it; hand-rolled vector structs add per-member unwind states where real `std::vector` members add none (`StageLoader` 0x40c520/0x40c5e0, `SceneXform` 0x40a680).
- Config accessors: unsigned index, `size()` bound, direct `m_entries[i]` (A: Loop 15).

- A this-adjusting forwarder (`add ecx,N; call f; ret 4`) is a plain member calling the embedded object's method; no asm needed (MenuManager 0x43cb50).

- **Triage first.** `python3 -I tools/triage.py --md analysis/nonmatching-triage.md` (after a full match.py run) classifies every NONMATCHING function: cause of the first difference, byte counts, an instruction-multiset delta (0 = pure register/scheduling permutation; leave those for last) and `ret N` / cleanup-state sequence mismatches. `tools/try_variants.py` compiles variant files in parallel and ranks them by differing bytes (`-k` keeps `build/try/<n>/` for objcmp).
- Check `ret N` of every NONMATCHING function against the original (the triage notes column): `CullViews` 0x4b7c80, `ColBody::Collide` 0x41ca70 and `FUN_0041fc60` all take a trailing argument the body never reads, and 0x417ee0 is `vector<StageProp>::insert(pos, n, x)` emitted out of line (three arguments), not a wrapper.
- A cleanup-state number that jumps ahead before a call (DrawContext 0x4b7a40: arrays give states 0-3, the next store is 7) counts destructible members whose constructors emit no code: add that many empty-destructor members (two ScenePoints and a float wrapper) in the layout gap.
- VC6 canonicalises commutative operands: swapping `a*b`, `a+b` or `n = len + m` in the source never changes `fld`/`fmul`, `imul` or `lea [base+index]` operand order (ExhaustSmoke 0x478220, FUN_004ac730, Car::LoadDriver 0x455070). Those are register-class differences; do not spend variants on them.
- A dead zero store to a stack slot right before an inline `size()` test and an out-of-line erase (0x484380) is `v.resize(0)`'s default argument: write a `Resize(n, const T& x = 0)` twin with inline `size()` and the erase as a call (NetRace::~NetRace 0x4815a0); where the original inlines the erase but calls `_Destroy` (0x403130), the twin's erase is written out with `Destroy()` declared under `auto_inline(off)`.
- **Layout from the unwind map.** A big class's destructor matches once its members are real types: the constructor's/destructor's FuncInfo unwind map (handler `mov eax, FuncInfo`; map entries `lea ecx,[this+off]; jmp dtor` or `push dtor; push count; push size; ... call __ehvec_dtor`) lists every destructible member with offset, size and destructor; states chained to an earlier parent are an inlined member constructor/destructor (Stage 0x40ebc0: 42 members, `include/stage.h`). Keep the partial declaration other modules use under `#ifndef <CLASS>_FULL_LAYOUT`.
- Member twins used in stage.h: `~T()` declared only keeps a vector destructor a call (0x401bb0); an inline `Destroy(first, last)` call reproduces `_Destroy` out of line (0x403130); a node with an inline empty destructor is freed with `operator delete` and its own null check, a node freed through `push 1; call ??_G` needs an explicit `if (n) n->Delete(1)` (VC6 never emits that call for plain `delete`); `v.resize`-style element loops with `push 0; call ??_G` likewise need an explicit `p->Destroy(0)` loop; `std::string` members destroyed by `_Tidy(true)` alone are a layout twin whose dtor calls a declared `Tidy(bool)`.
- `for (k = 0; k < count; k++)` over an array becomes a down-counting copy of `count` (`mov ebp,eax`) after the pointer setup; a named `Sample* sample = p->sample` local before a call picks eax where the inline expression picks edx (Stage 0x40ebc0).
- `std::string::npos` passed to an out-of-line call is loaded from its static (`mov ecx,[0x4e1340]`); write `std::string::npos`, not `(unsigned)-1`, in string twins (Stage ctor).
- **Compare call sequences first** on a big function: list the original's `call` targets and the built object's call relocations side by side (scratch `calls.py` idea); the first misaligned index points at the structural difference (missing inline, extra expansion, moved block) far faster than the byte diff.
- A `static` helper is not inlined under /O2 (/Ob1): mark file-local helpers `static inline` when the original has their body expanded (`NameOf` in the Stage ctor).
- Identical statement tails in two branches are cross-jumped by VC6 (the `_itoa` + append in the Stage ctor label block): write the tail in both branches when the original shares one copy.
- The same string type takes several twins in one TU as the inline budget changes: early locals = out-of-line assign/append/dtor, later locals = inline `_Tidy` ctor with appends expanded around `_Xlen`/`_Grow` calls, the member = inline `_Tidy(true)` dtor (Stage ctor `StPath` / `StPathLate` vs `StString`).
- Error paths in loaders: `FUN_00402c90(fmt, name)` (the empty debug logger, two pushes) precedes a string-like local, `_Freeze` calls at 0x414750, a call to 0x414550, and `throw std::logic_error` (= `exception("")` 0x4c12db + assign 0x4024f0 + `__CxxThrowException`). The 505-byte helper at 0x414550 performs range replacement by pointer differences and string growth/copy operations; the caller pushes a format literal and an object field as the source pair, whose meaning remains unresolved. Do not describe this as printf formatting until that mismatch is explained (Stage ctor).
- The last string flavour (Stage ctor `ai`): assign(x, 0, npos) expanded with the `this == &x` erase calls (0x408de0) and the refcount-sharing branch, `_Grow` expanded around `_Copy` 0x402ab0 / `_Tidy` 0x402700 / `_Eos` 0x402960, `c_str()` through an out-of-line `_Nullstr` (0x414480).
- A string local whose cleanup state is stored *after* its first assign call was built by a `(const char*)` constructor of one class (`_Tidy(); assign(s, strlen(s))`); a derived twin (`BmfString : ModelRecordString`) with the same body adds a base-subobject state instead (BmfReader ctor).
- Drafts "written from pseudocode" can be logically wrong where Ghidra lost structure: PropStage::Load's loop had read-and-insert where the original reads two points, looks up the midpoint's checkpoint and stores index/flag/length. Walk the original's call list against the source before tuning registers.
- Model-file temporaries are constructed with the real five-argument constructor (path, dir, texture cache, material list, 0) and destroyed implicitly; give the opaque type declared-only ctor/dtor rather than wrapper calls (PropStage::Load).
- Two `File` locals of sibling blocks overlap in VC6 unless one is at function scope; a second big local with its own slot after an `if (!Exists) return;` reproduces the original's two-slot frame (LoadSoundFiles: 0x812c = 2 × 0x4078 + scalars). A path string that is reassigned (`assign(name, 0, npos)`) for the next file is one local, not two.

- A standalone out-of-line copy of a class's inline method (UIRect ctor 0x4390a0) is defined on a file-local layout twin with `// SYMBOL:`; don't make the shared inline definition out of line.

- Several default-constructed `Color` locals declared back to back, values assigned afterwards, merge their EH state writes into one dword store (DrawSpinner 0x438810).
- Pass a POD twin as the hidden result of a call whose return value is discarded when the original has no EH state for it (HudPoint has an empty dtor).

- A class whose full layout clashes with an opaque declaration in rallytrophy.h: wrap the opaque one in `#ifndef <MODULE>_FULL_LAYOUT` and `#define` it at the top of the module's .cpp (Game; adding new types to rallytrophy.h breaks 0x4a2440).
- Out-of-line empty dtors folded to 0x48e660 need a declared-but-undefined `~T()` (RecordTable, RaceSession).

- Layout loaders whose first N push_backs stay calls: a vector twin with an undefined `insert(pos, const T&)` (0x4293c0) for those N and a real `push_back` for the expanded tail (GameScreen11A4 0x4286d0); better than inline-call ballast.
- `template class std::vector<T*>;` forces an otherwise unused member instance into the TU (warning C4660).
- The inline budget is spent from the end of a function backwards: inline calls added late push early STL sites out of line (MenuScreen2A8::Setup 0x4298c0).

History (Config loops 13-17, sound, controllers, networking, race modes, screens): `analysis/NOTES-archive.md`, verbatim.

There are no Direct3D imports. Rendering goes through a device plugin (`Device/bbdx8.dll`) that the game calls through a vtable (`DAT_0056b0ac`).

## Emission order and "already compiled means non-throwing" (2026-11-10)

Measured on `config.cpp` and small test files; this is what made `file_error::file_error` (0x409390) match.

- **VC6 treats a callee as non-throwing once a body for it has been compiled earlier in the same object**, and then
  drops the EH state store in front of the call. A body compiled later has no effect. It holds for extern, static
  and inline definitions alike (bisected by moving an empty function body through config.cpp).
- **`/YX` (automatic precompiled header) is now part of the C++ flags.** It changes no bytes in any matched function
  (full run 1880/1880 with one fresh PCH per source file), but it changes WHEN header inline functions get their
  out-of-line copy: without a PCH the copy is compiled right after the first function that calls it; with one it is
  deferred to the end of the object, after all normal functions. The original layout shows the deferred form: the
  out-of-line copies sit in a block at the end of each file's address range (config: 0x408a60-0x40968f).
  `tools/match.py` passes `/Fp<source>.pch` and deletes it afterwards, because a PCH made from another file's
  headers is silently ignored and the file then compiles in the non-deferred way.
- **The release-build logger `FUN_00402c90` is an inline empty function in the header**, not an extern. Every object
  emits its own copy late (all fold to 0x402c90), so normal functions still see it as may-throw (they keep their
  state stores, as the original does) while header inline copies compiled after it see it as non-throwing.
- Order inside the late block, as far as measured: first the instances that this file's own code references
  directly (e.g. `string(const allocator&)`, the copy constructor, members whose address is taken), then
  non-template header inline functions in header definition order, then template instances first reached through
  inlined code (`_Tidy`, `assign`, `operator+` ...). The original has template instances ahead of
  `file_error`'s constructor, so its order is not fully reproduced; config.cpp forces `_Tidy` forward with a direct
  reference (`g_tidy`, needs `#define private public`). What the original source did instead is unknown.
- Three further copies of non-template inline functions (`runtime_error`/`logic_error` members, implicit copy
  constructors) are still emitted immediately in our build but late in the original.
- `FUN_004a2440` (setup dialog, winmain.cpp) flips between two register allocations when the number of symbols
  declared in rallytrophy.h changes by one (any declaration or typedef; comments do not count). Its match is tied to
  that count; four `language.cpp`-private externs were moved out of the header to keep it matching with the inline
  logger. Expect it to flip again when rallytrophy.h gains or loses a declaration.
- Where to look for this effect elsewhere: a late (header inline or template) function whose build has an EH state
  store in front of a call that the original lacks.

## Stack slot sharing between blocks (2026-11-11)

Measured with small test files (VC6 /O2 /Oa /G6 /GX), frame size read from `sub esp`:

- Locals of blocks whose lifetimes do not overlap can share stack space. Two sibling blocks with `char a[0x200]`
  and `char b[0x2e8]` give a 0x2e8 frame. The same holds through loops, nested ifs, plain structs and inlined
  functions.
- A local reuses a dead slot only if it is at least half the slot's size. 372 and 376 bytes share a 744-byte
  slot; 256 or 192 bytes do not (the frame grows by their size). Two 256-byte arrays in one block next to a
  744-byte block give 744 + 512; the same 512 bytes as one struct give 744.
- Small locals of equal size in sibling blocks share with each other (two `char[256]` blocks: 256).
- A block that holds an object with a destructor does not share (0x300 instead of 0x200 in the first test).

Use: compare the first `sub esp` with the original (insdiff). If ours is larger, look for buffers that should be
one object, or objects with destructors inside a block; if smaller, look for locals declared once at function
scope that the original has per block, or a buffer that is too small. Read the buffer addresses in the original
(`lea reg,[esp+N]`) to see which locals sit in the same slot.

## Member path vs local alias after `new` (2026-11-11)

Measured on the effect constructors (`ParallelShadow` 0x4703c0, `SnowFall` 0x474c50, `RainEffect` 0x472d00):

- With `/Oa`, VC6 forwards a value stored through a local pointer (`m->vertices = new ...; v = m->vertices;` gives
  `mov ecx,ebp`). Stored or read through the member (`mesh->vertices`) it reloads the field after the next
  `operator new`, as the original does (`mov ecx,[edi+4]`), and `add esp,4` sinks below the store and the reload.
  The earlier `*(T* volatile*)&m->field` reloads imitate that but pin the `add esp,4` and the load one slot off.
  Which accesses go through the member differs per function; a sweep over m/member per statement group finds it
  (typically: material and array stores through the local, counts and the two array reads through the member).
- `if (n > 0) { cursors...; int j = n; while (j--) { ... } }` puts the cursor setup after the guard and reloads the
  count before `this` at the loop exit; `do { } while (--j)` and `for` forms reload them in the other order.
- A store that sits between a member's field init and that member's EH state store belongs to the constructor body
  of a struct wrapping those fields (`TrackQuery` at 0x48 of `RainEffect`); neither a body statement nor a
  mem-initializer of the outer class lands there.
- Loop counter forms (measured on 0x4ae4b0 and 0x48fb10): inside `if (n > 0) { ... }`, `for (; n; n--)` copies the
  count with `mov reg,reg`, `while (n--)` with `lea reg,[reg]`. A loop driven by a one-based value with the
  zero-based index written as `rank - 1` (`for (rank = 1; rank - 1 < count; rank++) a[rank - 1]`) makes the index a
  derived induction variable: its `xor` comes after the guard and its increment after the pointer step. A separate
  `n` counter gives the hoisted `xor` and the swapped increments instead.
- Typed copies and indexing (measured on `WaterAnimator::Update` 0x46fc10): a 4-byte value copied through a float
  lvalue is scheduled later than the same copy through an int local (`int now = clock->time; stamp = now;` keeps the
  load at the top and reuses eax). Implicit struct assignment copies as a block (`mov` pairs, not forwarded); explicit
  member copies (`d.x = o.x; d.y = o.y;`) let VC6 take a following `x += k` straight from the source (`fld [eax];
  fadd; fst`). Indexing `saved[i]` instead of walking a pointer declared before the loop moves the pointer setup
  after the guard and changes which register holds `this`. Check field offsets against the original first: this
  function had been writing the wrong field (+0 instead of +4).
- Check the numbers first (`tools/numdiff.py`, 2026-11-11): an instruction that differs from the original only in a
  displacement or immediate usually means the source is wrong, not the codegen. Found this way: the wrong texture
  slot in 0x46fc10, the wrong body field in 0x452eb0 (0x88 for 0xa8) and `sizeof(ShadowObject)` 0x17c for 0x178.
  Empty-destructor members used to reproduce EH states must sit at the offsets the unwind funclets name
  (`lea ecx,[this+N]`), not be appended as padding.
- `std::vector::operator[]` used twice on the same element loads it twice (0x45dac0); a raw pointer array is CSE'd.
- A ternary over a bool-returning inline keeps the byte-wide test: `Odd(c) ? A+1 : A` gives `and al,1 / neg al /
  sbb eax,eax / neg eax / add eax,A`; arithmetic forms fold to `and eax,1`.
- `memset(p, 0, 12)` emits a local `xor reg,reg` + three stores and keeps other literal zeros from being pooled
  into a callee-saved zero register.

## Hand-written STL instances: use the real template (2026-11-11)

Some NONMATCHING functions are out-of-line instances of VC6 library templates that the source re-implemented by
hand (`StMsg::Format` = `std::string::replace(iterator, iterator, const char*, const char*)`, `S10Vec::Insert` =
`std::vector<T>::insert(iterator, size_type, const T&)`). Instantiating the real template gives the original bytes.

- Force the out-of-line instance by taking the member's address through a typed pointer-to-member
  (`typedef void (V::*Ins)(T*, unsigned, const T&); Ins g = &V::insert;`); a direct call is inlined. Mark it with
  `// FUNCTION` + `// SYMBOL: <mangled name>` + a `typedef void Inst_xxx();` like the other instances.
- The element type decides the bytes. Read it off the original's `_Construct` and `operator=` helpers:
  which fields are copied one by one and which as a block; an out-of-line destructor gives the per-element call to
  the empty function. A float field is re-read inside a fill loop where an int field is hoisted out of it.
- How often VC6 inlines `operator=` inside `insert` depends on the number of copy statements (9 single copies: two
  out-of-line calls of three; 6-7: one; two sub-struct copies: none).
- Real `std::string` in `SaveReplay` compiles to the same bytes as the `H6Str` emulation, so those emulations are
  faithful; the cloned-epilogue difference there is not caused by them. Thirty flag sets did not remove it either.
- Locals read by inline asm (measured on 0x4578c0, 2026-11-11): stack slots follow declaration order within a block
  (first declared at the highest address), and a local that is assigned twice sorts apart from the single-assigned
  ones. Two asm blocks that each need their own pointers want two inner `{ }` blocks with their own declarations,
  not one set of pointers reassigned between them. Names and split/unsplit declarations do not matter.
- Term order of a float sum: a product held in a named temporary (`float fz = a * b; x = fz + c * d + e;`) is the
  first term loaded; reordering or parenthesising the written sum does not change the emitted order.


## Stack slots of plain locals follow reference counts (2026-11-11)

Found with standalone compiles and add/remove-one-reference probes on 0x427c10 and 0x43cc50:

- VC6 orders the stack slots of non-register locals by **reference count, highest count at the lowest offset**;
  references inside a loop weigh more than ones outside; ties keep declaration order. Declaration order, names,
  types and initialiser form do not move a slot otherwise (30+ such variants on 0x427c10 changed nothing).
- The count is taken after constant folding (reads of a known constant member do not count) but before tail
  merging. So when two locals sit in swapped slots with otherwise identical code, the original has one more (or one
  fewer) *source* reference that folds to the same instructions. The natural case: `if (a || b) { v = 1; }` was
  written as two ifs with the same body, which VC6 merges back (0x427c10: clicked/sy slots, 12 -> 3 bytes).
- Probe to confirm before searching: replace one read of the lower variable by another variable; if the slots flip
  to the original's, hunt for the missing/extra reference.
- 8-byte objects with destructors obey the same rule. In 0x43cc50 the per-item (1, 1) scale is an empty-ctor
  object assigned in both arms of `if (selected) {...} else {...}`; the arms merge, the count doubles, and the stores
  land where the original has them (25 -> 1 insn). Look for an if/else or `||` whose arms could have repeated a store.
- Slot sharing follows source variables: a temporary shares the slot of a variable whose lifetime it does not
  overlap. If a temporary sits one slot off, try merging or splitting variables around it. 0x44b510: the rally
  title's x and the item list's x are ONE variable (`float x = ...; Print(x); ... x = k;`); with a separate `rx`
  the title temporaries shared y's slot instead.
- A variable whose first value is used once and then reassigned under an `if` is one stored variable with a high
  count. If the original has it above another local, the first value was its own variable:
  `float y0 = ...; Print(y0); float y = y2;` (0x48cca0, y/y2 slots).

## Menu frame title block: inline colour setter (2026-11-11)

Five menu `Frame` functions (0x4267a0, 0x427c10, 0x424960, 0x42a330, 0x43cc50) were stuck on the title block
(`fld scale; <load>; fadd st,st`). The title colour is set through an inline setter that takes a `Color` object:
`text->SetColorObj(Color(DAT_00508bfc))` (TextRenderer, `void SetColorObj(const Color& c) { color = c.value; }`).
A plain `text->color = DAT` schedules the load correctly but hands ebp to -1 instead of 0 later in the function;
a volatile read keeps ebp but pins the load after `fadd`. Try this form in any remaining screen with a title block.

## Hand-written member stores vs inline member functions (2026-11-12)

When a run of member stores next to a call schedules differently from the original no matter how the statements are
ordered, the original probably called small inline members. 0x4b7c80: `solids.Clear(); normals.Clear();
specials.Clear();` with `void Clear() { m_end = m_data; m_count = 0; }` matched after ~700 orders of the six
hand-written stores had not. Same family: `SetColorObj(Color(x))` for the menu title colour.

## Frame too large by 4-8 bytes: constructed temporaries (2026-11-12)

0x48d7f0: when the only differences are stack offsets and the frame is a few bytes too large, an object the
original built as a temporary is a named local here. `SetPosition(&Pt(x * sx, y * sy))` with an inline two-argument
constructor stores straight into the temporary, sets the EH state after both stores, and lets the temporary share
the slots of floats that die in its arguments. A named `HudPoint position; position.x = ...` does not share.
Do not add such a constructor to the shared HudPoint class: it changes SnowFall::SnowFall; use a local subclass.

## Wave 2026-11-12 (inline-member lever, six Sonnet agents)

- Constant folded into a computed offset (`[eax+ebp+0x39a8]` vs ours `lea` with the constant): the original indexed a
  real struct array, `((Rally*)(base + 0x39a4))[r].a[s]`, not `off = r*1512 + s*16 + 0x39a4` (0x448f80 matched).
- Quad helpers: `SetQuad(uv, ...)` called before the position stores, colour assigned last (0x48e4c0, 0x48eb20).
- Reusing an existing float local as the FPU temporary (`x = ...; pos[0].x = pos[3].x = x;`) fixed an `fld st(1)` schedule (0x449860 229 -> 63).
- The inline-setter lever did nothing for register-swap-only near misses (0x44c580, 0x43a6e0, 0x450be0, 0x436250).
- slotmap.py survey over all 172 NONMATCHING functions (2026-11-12): in the 17 where the frames anchor at >= 90%,
  no local sits in a different slot. Slot flags appear only where anchoring is weak, i.e. where the code itself
  differs (e.g. 0x49d970 is 91 bytes short). Slot order is not the main remaining cause; fix structure
  (calls.py, size) first and use slotmap.py once a function is the right size.
- TrackSelectMenu::Run 0x443790: the original copies name/description of the by-value RallyInfo inline (_Tidy +
  assign); ours calls the copy ctor out of line. An inline/__forceinline subclass copy ctor does not inline.
- Sizegap wave (2026-11-12, six Sonnet agents, 25 functions with 1-4 hunks + six RaceMode menus): 0 matches, gains on
  0x443790 (387 -> 95, two-arm colour/scale before `std::string buf`), 0x422fc0 (62 -> 3, insert grow arm expanded with
  end stored before last), 0x4721b0 (34 -> 30, light-direction y/z read into locals first), 0x479f60 (9 -> 3, volatile
  j2 write like j1). Recurring blocker in 4 functions (0x443790, 0x44d6a0, 0x403ca0, 0x48bd10): where VC6 stops inlining
  std::string helpers in a function (inline vs call to _Tidy / copy ctor / resize). `#pragma inline_depth` 1-4 and
  `auto_inline(off)` change nothing; `inline_depth(0)` disables all inlining. Needs its own investigation.
- 0x46c2c0 Wheel::Wheel: the original reloads `this` into ecx and discards it between Reset() and the normal stores,
  hinting at an inline call with a discarded result there.
- slotmap.py gives 0 aligned hits on 0x480ce0 (bug to fix); sizegap's "original size" is wrong for some variants of 0x489300.

## VC6 inline budget, measured (2026-11-12)

Standalone probes (`build/inl`, project flags /O2 /Oa /G6 /GX) with K `{ std::string s(p); use(s.c_str()); }` scopes
followed by M calls to an inline function:
- Every function has a fixed budget of inline EXPANSIONS (call sites), not bytes: a one-line inline and one with a
  loop cost the same. Pragmas do not move it (inline_depth 1-4, auto_inline(off): no change).
- The budget is spent from the LAST statement backwards. With K=4, M=0 the last two string scopes expand their
  destructor inline (delete) and all four expand the ctor's assign (_Grow); each extra trailing inline call
  (M=1..8) turns one more step, walking backwards, into a call: dtor -> _Tidy(true) call, ctor -> assign call.
  K=2 inlines everything; from K=4 the earliest sites are calls.
- Consequence for a mismatch at site S: if the original CALLS a helper at S that we expand, our code after S spends
  fewer expansions than the original's (look for something the original inlined later that we call or hand-model);
  if the original EXPANDS at S and we call, we spend more after S. Hand-modelled twins (FB2_Path, TSE_Str16,
  TSE_Str::Grow) bypass the budget entirely: their inline/call choice is whatever we wrote, so for them fix the
  twin at that site instead of searching for budget.
- 0x44d6a0: every call after the fail exit is identical (calls.py); the fail-exit difference is FB2_Path's hand-
  inlined destructor, i.e. a twin modelling choice, not budget.
- Limit found the same day: in 0x44d6a0, an FB2_Path destructor written as `~FB2_Path() { Free(true); }` (inline
  member, so the budget could choose) stays inline at BOTH exits even with 16 extra value-returning inline calls
  added before the final return (empty inline calls are not counted at all). So the standalone rule does not by
  itself explain why the original calls _Tidy(true) at the fail exit; something else (exit kind, EH state, or the
  order the compiler visits exits) decides there. Unresolved.
- Issue pass 2026-11-12: #3's one-hunk functions (0x4ae4b0 `lea ebx,[ecx]` vs `mov`, 0x4b0180 load+test vs
  `cmp [esi],0`, 0x4b0e90 and 0x4b13c0 deferred stores) are all scheduling/encoding choices with 100-220 recorded
  variants each; a small sizegap hunk count does not mean an easy function. 0x42d710's out-of-line size() in the last
  push_back is unaffected by `#pragma inline_depth` 8..255. 0x43df70: 52 -> 48 via SetScale for the final restore.
- sizegap.py limits: it disassembles switch jump tables after `ret` as code (bogus top hunk on 0x43df70) and can
  misalign a hunk onto a matching EH-state store (0x4b0e90); confirm a hunk with insdiff before acting on it.

## Corpus lookup and a dead lever (2026-11-13)

- `tools/idiom.py` indexes the 1500 byte-exact functions (original instruction -> source line, from `/FAcs`
  listings) so a NONMATCHING hunk can be looked up instead of guessed: e.g. `lea R,[R+N]; mov R2,R` (the copy
  0x461a40 lacks) comes from `SpeechItem item = queue.front();` and from `for (int j = 0; j < 7; j++)` loops.
- Corpus survey over all 169 NONMATCHING functions (shape-aligned hunks): 5721 hunks, 2595 with an exact-text hit in
  the corpus; the big "size" functions get the most leads (Stage::Stage 119 of 262 hunks, StageScreen::Layout 98/164).
- **Runs of four identical stores are an inline helper** (`static inline void Quad(float* v, float f)`): that is what
  hoists the next allocation's `push <size>` above the quad stores (0x486240 LoadOverlays matched, from the corpus hit
  on G4_Quad in unk_4303e0.cpp). Swept over the other six functions with such runs: no change, so it only matters next
  to a `new`.
- **Stale `volatile` workarounds**: a `*(volatile T*)&x = e` or `T volatile x` added in an earlier pass can be what
  blocks the function now (0x469a20: plain `float kk = ...` 17 -> 6 bytes). Swept over all 57 NONMATCHING functions
  that carry one (one site at a time and all at once): closer in 8 (applied), same or worse elsewhere.
- Local NAMES do not matter either: 12 random renamings of 0x436250's locals gave identical bytes.
- Neutral reference-count rewrites swept mechanically (`if (A || B)` -> `if (A) {} else if (B) {}` with the same
  body; `v = e;` moved into both arms of a following if/else) over the 38 functions that have such sites: no match,
  noise-level gains only in two 5-10 KB functions. These forms matter only where the slot order is the last difference.
- Declaration count is NOT a general lever: forcing 1..32 extra global typedefs into unk_4303e0, unk_43a390,
  unk_452b20 and unk_490950 changed no byte in any of their 19 NONMATCHING functions. The winmain flip was a
  one-off.

## Big-function rounds (2026-11-13)

- **Chained assignment = load once, copy registers.** `a.x = b.x = c.x = d.x = lo.x;` reproduces the original's
  `mov ecx,eax; mov edx,eax` fan-out where per-field stores reload (0x470940 corner block now exact).
- **A named temporary re-addressed with `lea` is a sign the original passed the call result straight through**
  (`dst.Assign(Plus(a, b))` instead of `T tmp = Plus(a, b); dst.Assign(tmp);`): 0x4418d0, seven 7-byte hunks.
- **Register roles follow use weight, so lower the weight of the wrong winner**: reading `m_game->text` in two blocks
  instead of the `text` local fixed 0x421fa0's ebx/edi swap; testing `info == rally` instead of `r == 0` fixed
  0x4418d0's. Adding uses to the intended winner rarely works; removing uses from the wrong one does.
- **`fcompp` vs `fcomp; fstp st(0)`**: write the compare as one expression with the sum on the left
  (`if (d.z*d.z + d.y*d.y + d.x*d.x <= r*r)`), not via a named d2.
- **Literal constants as named file statics** (`static const float K262 = 262.0f;`, used `K262 * sy`) give the
  original's `fld [const]; fmul [local]` order and stop folding of `25 + 575` (0x421fa0).
- Unexplained: in 0x411600 the score moves with the number of unused `__forceinline` helpers defined in the file
  (0, 2 -> 124 insns; 1, 3 -> 127; 4 -> 121). Same family as the winmain declaration-count flip; not used.
- FillCells 0x40f390: the inline-asm RoundF costs register allocation (plain casts free esi/edi/ebx but call
  __ftol, which the original lacks). `/QIfist` on the cast form does not reproduce the original either (2128 bytes
  vs 2101 without); how the original rounds inline without asm demoting registers is open.
- x87 operand order (toy compiles, 0x470940): VC6 canonicalises commutative fmul/fadd operands; local-struct x
  global always emits `fld global; fmul local`, whatever the source order, helper or alias. A one-expression sum is
  emitted z,y,x; a statement chain (`t = c.x*D.x; t += c.z*D.z; t += c.y*D.y`) keeps source order. Where the original
  is local-first, the local term is probably a different kind of node (e.g. produced by an inline `+=` through a member
  path); unresolved.
- Unrolled loops: where the original has N copies of a small search with no loop counter, write N copies (a macro is
  fine); `while (1) { if (a) break; if (b) break; e++; }` stays top-tested where `for (; cond;)` gets rotated
  (0x419f30, 490 -> 362 insns with the e[] fixup also unrolled).
- **`/Oa` caches a plain pointer field across calls**: a `m->name` load is CSE'd through a chain of strcmp calls.
  Reading it through a string-like sub-object (`struct { const char* ptr; int len; ...; c_str() { return ptr ? ptr : ""; } }`)
  reloads it per use as the original does (0x45e480, ~300 insns of the glow/window chain).
- **Right-to-left setters**: `fld b; fld a; fstp d0; fstp d1` is an inline `Set2f(d, a, b)` call (arguments evaluated
  right to left), not two stores (0x45e480).
- **Identity matrices**: a struct without a constructor, stores written inline right after the declaration, puts the
  EH state store before the identity stores as in the original (0x493650 IdentityMatrix, 0x473470 RainMatrix).
- **Stale volatiles keep paying**: removing `volatile BYTE* hit` aliasing in 0x45cc30 was its biggest single gain.
- **A named local/helper can renumber `_$E` static initialisers** (0x45bc20: `Vec3 f = vel * -down;` or a helper broke
  two matched `_$E` functions in unk_45ad10.cpp). Always rerun match.py on the whole file, not only the target.
- **Vector-style `size()` stops VC6 hoisting a base pointer** (`first == 0 ? 0 : last - first` as an inline member):
  0x417540 got the original's register map. Arrays instead of scalar locals give the original's real frame slots
  (0x4ad1d0 r[9]/a[9]).
