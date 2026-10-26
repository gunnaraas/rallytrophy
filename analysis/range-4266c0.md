<!-- Codex draft, reviewed by Claude 2026-10-23. Vtables verified from the ctors: 0x4e1548 = {0x4266e0, Setup 0x426750, 0x43ff80, 0x4414c0}; 0x4e157c = {0x427a50, 0x427b10, 0x427b30, 0x4414c0}; 0x4e15b4 = {0x429740, 0x4298c0, 0x43ff80, 0x4414c0}; 0x4e1630 = {0x42c360, 0x42c400, 0x42c860, 0x4414c0}; 0x4e1688 = {0x42e9a0, 0x42ea30, 0x43ff80, Draw 0x4301b0}. Member layouts below are guesses: recover them from the ctors and unwind tables. -->
<!-- Function map, rawdis/caller evidence against RallyTrophy.exe Patch 1.01; semantic labels are hypotheses unless stated. -->
# Range 0x4266c0–0x4303df: menu screen modules

## Boundaries and evidence

The range contains five consecutive translation units, each ending in a 0x30-byte VC6 STL `_$E` guard/atexit stub. Boundaries align with embedded screen constructors called from `Game::Game` at 0x4303e0: 0x4266c0 at 0x430485; 0x427630 at 0x430415; 0x4295f0 at `MenuManager` ctor 0x43c602; 0x42c2b0 at 0x430445; 0x42e930 at 0x430465. Their Run/Setup wrappers are reached through their virtual tables. Proposed five-file split is supported.

| Module | Code range (end exclusive) | guard stub | class / evidence |
|---|---|---|---|
| 1 | 4266c0–427600 | 4275d0 | GameScreen2DD4; ctor stores vtable 4e1548; size 0x20 |
| 2 | 427600–4295c0 | 429590 | GameScreen11A4; ctor stores vtable 4e157c; size 0x1d4 |
| 3 | 4295c0–42c2b0 | 42c280 | MenuScreen2A8; embedded at MenuManager+0x2a8, size 0x4a8; vtable 4e15b4 |
| 4 | 42c2b0–42e930 | 42e900 | GameScreen1E9C; embedded at Game+0x1e9c, size 0x16c; vtable 4e1630 |
| 5 | 42e930–4303e0 | 4303b0 | GameScreen24FC; embedded at Game+0x24fc, size 0x4e8; vtable 4e1688 |

The virtual layout is the MenuScreen ABI: [0] deleting destructor, [1] Setup, [2] Release, [3] Slot3. Run methods are nonvirtual. The constructors show exact vtable addresses above. Not a global/static initializer range: stubs only. Check jump-table references in big methods as switch dispatches, not extra functions. One code body is absent from functions.csv: 0x426750–0x42676c (28 bytes, excluding alignment), a thiscall lazy-Setup guard that sets state/game and invokes 0x4270c0; it is an additional member, likely compiler helper/user-authored wrapper. Five `_$E` stubs are also omitted from functions.csv.

All classes inherit MenuScreen (0x20 base: state +4, selected +8, game +0xc, vector<UIRect*> +0x10). Constructors call base 0x4381c0. Unwind tables cover string/vector members constructed after base; padding-only declarations in the headers are incomplete. `_$E` stubs are compiler/runtime guards, not class methods. The scalar deleting destructors at 4266e0, 427a50, 429740, 42c360, 42c8c0, 42e9a0 are compiler-generated ABI wrappers; 427b30 is a tiny thunk/virtual helper. The big methods have SEH registration and FPU use where noted below.

## Module 1 — GameScreen2DD4

Object 0x20 bytes. Base fields +0..+0x1f; ctor clears +4/+8 and installs vtable 4e1548. Likely a compact front-end screen with no additional members. Run at 426770 delegates Setup and then performs state/input processing. Setup at 4267a0 is the large screen implementation. Strings/assets in its calls include text IDs and resource IDs; menu-content identification is tentative (likely mode/network selection from state 0x501458 and dispatched Game state). No direct INI filename evident in the short ctor.

## Module 2 — GameScreen11A4

Object 0x1d4 bytes; vtable 4e157c. Constructor builds several std::string/MenuLabel-like records beyond base (vector/string unwind calls). +0x20 onward is not padding: fields are labels/layout/state for the screen. Setup/Run use renderer, menu helpers and Game state. Role guess: career/event selection screen (large selectable list and progression state); verify by string table/text IDs before assigning a product name. No reliable INI filename established.

## Module 3 — MenuScreen2A8

Object 0x4a8 at MenuManager+0x2a8; vtable set by ctor (extract exact value from store at 4295f0). Includes substantial label/vector/string state. MenuManager forwarding thunk 43cb50 calls 42a210 with `this+0x2a8`; 42a210 itself dispatches to 42a330 and thus is a class method, despite CSV's stdcall classification. Main rendering/input methods 42a330 and 42aee0. Role guess: options sub-screen, probably gameplay/control settings based on state and resource IDs; distinguish from known SoundMenu at +0x750 and RaceOptionsMenu at +0x11c. Inspect its literal text IDs/INI reads before final naming. Setup and Release are vtable methods around 429760/42a2c0; 4293c0 is teardown/helper.

## Module 4 — GameScreen1E9C

Object 0x16c; vtable installed by 42c2b0. Base plus screen-owned labels/strings and state. Run wrapper 42c8c0, big Setup/render/input body 42c8e0, helper methods 42d710 and 42ddc0, teardown/release 42e760. Role guess: replay or player/vehicle configuration screen; class lives at Game+0x1e9c immediately before Slots at +0x2008, so likely intervening mode/player setup. Text IDs and assets should decide exact label; no filename asserted.

## Module 5 — GameScreen24FC

Object 0x4e8; vtable installed by 42e930. Large object with many labels/resource state; Run 42ec40, principal setup/render/input body 42ec70, helpers 42f360/42f5c0. Embedded after Slots at +0x2008 and before ReplayMenu at +0x29e4: likely track/stage/mode selection. String IDs and stage/car resource calls are the key evidence to resolve exact role. No INI filename confirmed.

## Function inventory

Calling convention and byte sizes are from functions.csv. EH/FPU notes from prologues/instruction bodies. Caller lists give known direct callers; virtual callers are through the listed screen vtables/Game dispatcher.

| Address | Size | Convention / frame | Role; main caller(s) |
|---|---:|---|---|
| 4266c0 | 26 | fastcall, leaf | ctor; base init, clear fields, vtable 4e1548; Game ctor |
| 4266e0 | 30 | thiscall | scalar deleting dtor wrapper; dtor + optional delete |
| 426700 | 79 | fastcall, SEH | dtor: Release then MenuScreen dtor |
| 426750 | 28 | thiscall | lazy Setup guard: set state/game, invoke 4270c0; called by Run 426770 |
| 426750 | 32 | thiscall | lazy Setup/state wrapper; calls 4270c0 |
| 426770 | 48 | thiscall | Run/frame wrapper; dispatches Setup and 4267a0 |
| 4267a0 | 2321 | fastcall, SEH, FPU | primary screen loop/render/input/state handling; Run |
| 4270c0 | 1286 | stdcall | setup/resource/text initialization; 426750 / virtual dispatch |
| 427630 | 1056 | fastcall, SEH | ctor; initializes labels/strings and installs vtable 4e157c; Game ctor |
| 427a50 | 30 | thiscall | scalar deleting dtor wrapper |
| 427a70 | 154 | fastcall, SEH | dtor/release and base cleanup |
| 427b30 | 16 | fastcall | small virtual thunk/helper |
| 427b40 | 204 | thiscall | Run/frame state/input wrapper; Game dispatcher |
| 427c10 | 2745 | fastcall, SEH, FPU | primary screen setup/render/input; Run/virtual dispatch |
| 4286d0 | 3303 | stdcall, SEH, FPU | large rendering/layout helper for screen 11A4; 427c10 |
| 4293c0 | 455 | thiscall, SEH | cleanup/release helper; MenuScreen2A8 dtor/Release |
| 4295c0 | 16 | thiscall | trivial base/slot helper |
| 4295d0 | 17 | thiscall | trivial base/slot helper |
| 4295f0 | 332 | fastcall, SEH | MenuScreen2A8 ctor, strings/labels; MenuManager ctor |
| 429740 | 30 | thiscall | scalar deleting dtor wrapper |
| 429760 | 340 | fastcall, SEH | MenuScreen2A8 dtor and owned-member cleanup |
| 42a210 | 165 | thiscall `ret 4` | **byte-exact** action/forwarded method; 43cb50, 42a330. Reads the render-capabilities dword, masks its low byte, extracts bit 2 and stores the result before setting render state 0xb. |
| 42a2c0 | 103 | thiscall | Release/setup teardown helper |
| 42a330 | 2921 | fastcall, SEH, FPU | MenuScreen2A8 main frame/render/input; MenuManager forwarding/vtable |
| 42aee0 | 5014 | fastcall, SEH, FPU | large screen layout/selection/render helper; 42a330 |
| 42c2b0 | 168 | fastcall, SEH | GameScreen1E9C ctor; Game ctor |
| 42c360 | 30 | thiscall | scalar deleting dtor wrapper |
| 42c380 | 128 | fastcall, SEH | dtor/release and base cleanup |
| 42c860 | 87 | fastcall | setup/state helper |
| 42c8c0 | 26 | thiscall | Run wrapper; Game dispatcher |
| 42c8e0 | 3597 | fastcall, SEH, FPU | principal Setup/frame/render/input body |
| 42d710 | 1704 | fastcall, SEH, FPU | screen-specific list/layout/render helper; 42c8e0 |
| 42ddc0 | 2464 | fastcall, SEH, FPU | screen-specific selection/action helper; 42c8e0 |
| 42e760 | 401 | thiscall, SEH | Release/owned resource cleanup |
| 42e930 | 103 | fastcall, SEH | GameScreen24FC ctor; Game ctor |
| 42e9a0 | 30 | thiscall | scalar deleting dtor wrapper |
| 42e9c0 | 102 | fastcall, SEH | dtor/release and base cleanup |
| 42ec40 | 33 | thiscall | Run wrapper; Game dispatcher |
| 42ec70 | 1768 | fastcall, SEH, FPU | main setup/frame/render/input body |
| 42f360 | 593 | fastcall, SEH, FPU | screen helper/layout/action |
| 42f5c0 | 3052 | thiscall, SEH, FPU | screen-specific selection/render/resource helper |

Literal IDs and constants are predominantly menu text/resource indices and screen dimensions derived from the global display dimensions (0x56b0ac); the 4267a0 body accesses the Game/MenuGame singleton at 0x56b0c8, TextRenderer at Game+0x1390c4, and mode/network globals 0x501458/0x501464. Exact string-to-screen attribution remains to be validated from the immediate operands of resource lookup calls; do not infer literal English menu labels from numeric IDs alone. No explicit `.ini` path can be confidently assigned from the inspected evidence.

## Suggested work groups (about 1.5–3 KB each; large routines standalone)

* Module 1: 4266c0–4270bf lifecycle/Run plus 426750 wrapper; separate 4270c0 setup (1.3 KB).
* Module 2: 427630–427b3f lifecycle; 427b40–427c0f Run/wrapper; 427c10 (2.7 KB) standalone; 4286d0 (3.3 KB) standalone.
* Module 3: 4293c0–4297bf cleanup/ctor/dtor; 42a210–42a2ff dispatch/Release; 42a330 (2.9 KB) standalone; 42aee0 (5.0 KB) standalone.
* Module 4: 42c2b0–42c8df lifecycle/short wrappers; 42c8e0 (3.6 KB) standalone; 42d710 (1.7 KB); 42ddc0 (2.5 KB); 42e760 cleanup.
* Module 5: 42e930–42ec6f lifecycle/Run; 42ec70 (1.8 KB); 42f360; 42f5c0 (3.1 KB) standalone.

Compiler-generated: scalar deleting destructor wrappers at 4266e0, 427a50, 429740, 42c360, 42e9a0; `_$E` guards at 4275d0, 429590, 42c280, 42e900, 4303b0. Tiny 427b30 and short constructors/destructor wrappers may be folded thunks. The vector/string construction and unwind helpers called from constructors are shared VC6 STL/runtime routines outside this range.
