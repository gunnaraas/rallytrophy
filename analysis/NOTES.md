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
