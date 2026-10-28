<!-- Codex draft, reviewed by Claude 2026-10-23: embedded-screen offsets agree with the screen maps (ServiceMenu +4, lobby +0x1378, PlayerNameMenu +0x14cc, MenuManager +0x2df4, TrackSelectMenu +0x3aec). -->
# Range 0x4303e0-0x437e9f: Game module map

Evidence is from Patch 1.01 raw disassembly, `analysis/functions.csv`, embedded-class constructors/destructors, refs, and the existing `GameView`/`MenuGame` views. Function-purpose labels not already present in the headers are role guesses. No C++ is proposed here.

## Module boundary and completeness

This range is **one source module, likely the Game implementation TU**. The previous module ends at the 0x4303b0 `_$E` guard stub; this module's first function is Game ctor 0x4303e0. At 0x437e70 another 0x30-byte `_$E` guard stub follows the final function 0x437e20. The constructor initializes the singleton pointer `DAT_0056b0c8`; Game is created at 0x4a3cd6, and its large lifecycle/dispatch routines are called together from within this range. The guard pattern and the continuous cluster of Game member routines support a single TU. There are no intervening `_$E` stubs / static-initializer pairs in the range.

`functions.csv` has all 48 executable function starts in the requested interval. No additional callable function start is evident in the gaps. The apparent data islands are in-function switch tables: 0x430ccc (seven front-end states), 0x433ba4 and 0x4342e4 (race setup/advance state dispatch). 0x4303b0 and 0x437e70 are compiler guard stubs, outside the CSV function list. There are no Game-class vtable stores or vtable entries: **Game itself is nonpolymorphic**. It owns polymorphic screen objects whose vtables are set by constructors in neighboring modules; those vtables are outside this range and should be taken from the individual screen maps (including `MenuScreen` at 0x4e16dc in range-437ea0).

## Class and layout: Game at DAT_0056b0c8

The constructor starts with subobjects at offsets below; unwind-state values in its SEH frame corroborate reverse destruction. The size is at least 0x139894 (last constructed 0x20-byte array runs from +0x139490 through +0x13988f); exact allocation size is not established by this range. `DAT_0056b0c8` is published during construction at 0x430627 and cleared during destruction at 0x4307b9.

| Offset | Type / inferred subobject | Ctor | Evidence / notes |
|---:|---|---:|---|
| +0x0004 | ServiceMenu-like screen, 0x11a0 bytes | 0x445e80 | Dtor 0x4464f0; constructor unwind state 0 |
| +0x11a4 | screen class | 0x427630 | dtor 0x427a70; `Game` call sites treat it through vtable |
| +0x1378 | Screen1378 / lobby screen | 0x43a3c0 | dtor 0x43a440 |
| +0x14cc | PlayerNameMenu | 0x43d770 | dtor 0x43db80 |
| +0x1e9c | screen | 0x42c2b0 | dtor 0x42c380 |
| +0x2008 | screen | 0x44ea60 | dtor 0x44eaf0 |
| +0x24fc | screen | 0x42e930 | dtor 0x42e9c0 |
| +0x29e4 | screen | 0x44b930 | dtor 0x44b9c0 |
| +0x2dd4 | Net/session screen or manager | 0x4266c0 | dtor 0x426700 |
| +0x2df4 | MenuManager | 0x43c590 | dtor 0x43c680; active-screen manager, virtual slots +4/+8 used |
| +0x3aec | TrackSelectMenu | 0x4413b0 | dtor 0x441760 |
| +0x59e4 | StageTable / championship data | 0x4205e0 | dtor 0x420730 |
| +0x5c30 | RecordTable | 0x48fe20 | dtor 0x48e660; loader wrapper 0x4309b0 |
| +0x139080 | UI/input helper object | 0x491fb0 | destroyed via 0x48e660 |
| +0x139110 | `MenuLabel[7]` (0x24 each) | vector ctor 0x4c11f4 with 0x430700 / 0x430720 | title/menu labels; text rendering setup writes dimensions/positions |
| +0x139224 | `SoundSource[4]` (0x70 each) | 0x49b630 at +0,+0x70,+0xe0,+0x150 | ctor callsites 0x430504..534; GameSound view describes four channels as move/select/back/limit |
| +0x1393e4, +0x139404 | 0x20-byte UI/audio-related objects | 0x4aee00 | dtor 0x4aee20 |
| +0x139424 | 0x3c-byte object | 0x4ade40 | dtor 0x4ade80 |
| +0x139460 | current menu selection / cursor state | scalar init | initial byte 1; label vector at +0x139490 |
| +0x139490 | 32 string-bearing records (0x20 stride) | vector ctor 0x4c11f4 with 0x430700 / 0x430720 | constructor initializes the string prefix of each record; destructor releases it |
| +0x139074 | optional remote `RecordTable*` | conditional new 0x133444, ctor 0x48fe20 | only when `DAT_00501471` |
| +0x2dcc, +0x2dd0 | 0x614-byte / 0x70-byte allocated UI/render resources | dynamic allocation in ctor | destroyed through 0x48e660 then operator delete |
| +0x1390b8..+0x1390c8 | five `TextRenderer*` | allocated/configured by 0x430cf0 | ctor/configurator 0x4a9520 with five resource names; dtor/release by 0x430eb0 |
| +0x1390d4 and +0x1390dc..+0x13910c | UI/render/audio resource pointers | initialized by 0x431f40 | many created in 0x431f40; individually released in 0x431c50 |
| +0x139214..+0x139220, +0x139478..+0x13948c | scalars / floats | direct initialization and config loads | timing, selected-stage, and UI scale values; see globals below |

Other confirmed field usage: +0x139078 is the current front-end selection (0..6); +0x13907c guards screen setup/teardown; +0x139108 and +0x13910c are allocated subsystems; +0x139460 anchors the screen-label/cursor state; +0x139474 is a screen rectangle; +0x139484 initialized to 1. The body at 0x430f60 builds UI resources and labels; 0x431c50 releases them.

### Globals, strings, and constants

- Singleton pointer: `DAT_0056b0c8` (Game). Rendering/device globals used throughout: `DAT_0056b0ac`, `DAT_0056b0b0`, `DAT_0056b0b8`, `DAT_0056b0bc`, `DAT_0056b0c0`.
- Front-end / race mode state: `DAT_00501450` RaceMode pointer, `DAT_00501454`, `DAT_00501458`, `DAT_0050145c`, `DAT_00501460`, `DAT_00501464`, `DAT_00501468`, `DAT_00501471..473`; record/round arrays at 0x50149c, 0x5014a0, 0x5014b0.
- Menu flags and input state include 0x57b9f4, 0x57ba20/24/35/43/4e/50 and 0x508bdc, 0x508f60. The constructor/config paths load menu timing/scale values to +0x13948c and globals 0x50150c..0x501528.
- Visible resource/config strings include the text renderer resource names at 0x4f89b4, 0x4f8998, 0x4f897c, 0x4f8960, 0x4f6940; menu labels/layout keys clustered at 0x4f7ca8..0x4f7cd0; record table name 0x4f75f0; setup/config labels 0x4f7688/0x4f8948 and 0x4f87xx. Exact readable text should be recovered from `.rdata` when naming individual UI controls.
- Float constants include display dimension conversion factors 0x4e1354/0x4e1358; several positions/scales are computed from RenderDevice dimensions. Race setup uses track/championship arrays and globals at 0x50132c/0x501334.

## Function map

Convention column follows `functions.csv`; fastcall functions with a `this` pointer receive it in ECX (EDX is typically the unused second fastcall register). `SEH` means the VC6 `push -1; push handler; fs:[0]` exception frame is visible; handler address is shown. FPU marks observed x87 work. Caller list gives principal in-range or external callers; internal callsites are omitted when obvious from role.

| Address | Size | Convention / frame | Role guess and main callers |
|---:|---:|---|---|
| 0x4303e0 | 798 | fastcall; SEH 0x4d7950 | **Game ctor**: constructs embedded menu/screens, labels, four SoundSources, resource fields; sets `DAT_0056b0c8`; optionally creates remote records. Caller 0x4a3cd6. |
| 0x430700 | 22 | fastcall; leaf | MenuLabel string/vector element ctor: stores allocator byte and zeroes string storage fields. Game ctor 0x4304f4/0x4305a1; resource-label builders 0x43231e etc. |
| 0x430720 | 61 | fastcall; leaf | MenuLabel element dtor: decrement shared string refcount or free; zeroes string fields. Game dtor and vector-dtor callbacks across 0x40eace, 0x4616ff, many menu label arrays. |
| 0x430760 | 581 | fastcall; SEH 0x4d7acf | **Game dtor**: tears down dynamic resources, labels, sounds, then embedded screens in reverse construction order; clears singleton. Called 0x4a3e08. |
| 0x4309b0 | 17 | stdcall; leaf wrapper | RecordTable load wrapper: forwards `this+0x5c30` and table name 0x4f75f0 to 0x490820. Called in game setup 0x430b29 and 0x43457e. |
| 0x4309d0 | 761 | fastcall; SEH 0x4d7ae8, FPU | **Front-end main loop / startup dispatch**: load config, initialize render/menu resources and modes, dispatches on the seven-state table at 0x430ccc; calls menu/race flows and tears down. Caller 0x4a3dfd. |
| 0x430cf0 | 436 | fastcall; SEH 0x4d7b37 | Create five TextRenderer resources (0x150 each) from named assets, attach to fields +0x1390b8..c8. Called from 0x430b30 and 0x434585. |
| 0x430eb0 | 171 | fastcall; leaf | Release the five TextRenderers and clear their pointers. Called 0x430789, 0x430ca0, 0x4349eb. |
| 0x430f60 | 3300 | fastcall; SEH 0x4d7c08, FPU | **Build Game UI/resource state**: initialize renderer/UI objects, load menu layout/config, populate label arrays, dimensions and sound sample settings. Called during startup/return to menu (0x430b3e, 0x430c99, 0x4339ac, 0x434593, etc.). |

| 0x431c50 | 741 | fastcall; leaf | **Release Game UI/resource state**: virtual-release active embedded screens, delete transient UI/render resources, destroy per-screen objects and clear pointers/vectors. Called 0x430790/0x430c99 and race transitions 0x433933/0x4348d8. |
| 0x431f40 | 2190 | fastcall; SEH 0x4d7cf8 | **Create/configure rendering and UI resources**: allocates effect/UI objects, sets graphics properties, stores resources under +0x1390d4 onward. Called during setup 0x430b37 and 0x43458c. |
| 0x4327d0 | 522 | fastcall; leaf | Exact GameView teardown: disposes wrappers/rectangles, stops and releases four sound sources, releases two screens, unloads service-menu textures. |
| 0x4329e0 | 23 | fastcall; leaf | `MenuGame::GetScale` (known header name): returns pointer to scale pair at +0x1390cc, set to (1,1). Callers include menu/screen modules. |
| 0x432a00 | 7 | fastcall; leaf | `GameView::FUN_00432a00` (known header): return field +0x1390d4. |
| 0x432a10 | 45 | fastcall; leaf | `MenuGame::UpdateVideo` (known header): advance background video state. |
| 0x432a40 | 22 | fastcall; leaf | `MenuGame::FUN_00432a40` (known header): draw current background video frame when loaded. |
| 0x432a60 | 63 | thiscall; leaf | `MenuGame::DrawVideo(int)` (known header): conditional video drawing path. |
| 0x432aa0 | 89 | fastcall; leaf | Common Game frame/render setup helper used before menu state dispatch; called by 0x430b60 and 0x4345b5. |
| 0x432b00 | 35 | fastcall; leaf | Common UI/frame finish or state-reset helper; called by front-end loop 0x430c43/0x430c7f and race setup 0x433b59. |
| 0x432b30 | 204 | thiscall; leaf | `GameView::FUN_00432b30(int,int)` (known header): frame/presentation transition with two parameters. Called from 0x430c53 and race loops 0x4339e4/0x433b69. |
| 0x432c00 | 516 | fastcall; leaf | Exact `MenuGame::Update` (known header): advances the 500 ms stream transition, updates the per-track stream frame when the audio manager exists, computes stream fade volume, then updates four sound channels; called by 0x432ee2 and many screen methods (see header). |
| 0x432e10 | 1031 | fastcall; SEH 0x4d7d40, FPU | **Front-end interaction loop**: pumps input/device state, calls `Update`, hit-tests seven menu labels, changes selection, plays move/select sounds, draws menu selection and waits/polls until exit/transition. Called by 0x430b73. |
| 0x433220 | 2435 | fastcall; SEH 0x4d7d77 | **Race launch / create players and start race**: routes selected menu action (track, stage, name, career), allocates/configures Player objects, uses stage/car selections and track tables, initializes RaceMode/global fields, and iterates player/car setup. Called by front-end state dispatch 0x430bab. |
| 0x433bc0 | 1826 | fastcall; SEH 0x4d7db7 | **Advance/continue race or championship setup**: resolves selected career/championship/track state, initializes player entries and track selection, updates progress/round globals. Called by 0x430c00. |
| 0x434300 | 1871 | fastcall; SEH (handler in prologue), FPU | **Network/multiplayer race setup**: reads network session state, obtains player/config selections, initializes online players/stages and selected race parameters, then enters resource setup. Called by 0x4a3cfa. |
| 0x434a50 | 335 | fastcall; leaf | **Return from network/setup flow**: clears network/menu flags, refreshes Game state and resumes front-end setup. Called by 0x430c2e. |
| 0x434ba0 | 557 | cdecl; SEH likely | **Network/session dialog or lobby service**: invoked by front-end when network global 0x508bdc is set; services that session and returns to dispatch. Called by 0x430c67. |
| 0x434dd0 | 18 | fastcall; leaf | Small Game transition/reset helper. Called by front-end state case 0x430c73. |
| 0x434df0 | 16 | fastcall; leaf | Small Game transition/reset helper (paired with 0x434dd0); called by front-end state case 0x430c37. |
| 0x434e00 | 1572 | stdcall; SEH (handler in prologue) | **Race continuation/result transition**: called from 0x431bfa inside the larger setup/resource path; processes result/next-state and stage/player progression. Likely a callback/helper with explicit Game argument due stdcall. |
| 0x435430 | 364 | thiscall; SEH 0x4d7f26, FPU | `MenuGame::FUN_00435430(HudPoint)` (known header): draw cursor at a point. Called by Game drawing routines 0x4367e4/0x436e1a/0x4371dc and MenuScreen helper 0x438f77. |
| 0x4355a0 | 368 | stdcall; SEH 0x4d7f5e | UI draw callback variant 1; exact naked transcription in source. Registered/invoked through function pointer table outside this range. |
| 0x435710 | 368 | stdcall; SEH 0x4d7f9e | UI draw callback variant 2; same shape as 0x4355a0. |
| 0x435880 | 368 | stdcall; SEH 0x4d7fde | UI draw callback variant 3; same shape as 0x4355a0. |
| 0x4359f0 | 368 | stdcall; SEH 0x4d801e | UI draw callback variant 4; same shape as 0x4355a0. |
| 0x435b60 | 313 | stdcall; frame not SEH | `MenuGame::FUN_00435b60` (known header): GameView drawing helper taking rect/id parameters; invoked by menu screen drawing paths. |
| 0x435ca0 | 313 | stdcall; frame not SEH | Parallel drawing helper to 0x435b60 (different resource/coordinate variant); address is used by neighboring menu components. |

`0x435b60` uses width `DAT_0056aea8 * 48.0`, starts its repeated row at `((float)a * 42.0 + 214.0) * DAT_0056aeac`, then advances by `DAT_0056aeac * 42.0`. Callback `0x435880` is selected when `c == 0`, otherwise `0x4355a0`. If `d != 0`, the optional footer calls `0x4355a0` when `e == 0` and `0x435710` when `e != 0`; `f == -1.0` selects a 604.0 height. These are observations from Ghidra assembly and pseudocode for Evidence `ev_52cfb517b00277ed4ad562293fd1eaa405d0a82a8d5a0d9b376aad3cd944d373`; pseudocode is behavioral reference, not source. The 313-byte naked instruction transcription now matches all 13 object relocations.
| 0x435de0 | 368 | stdcall; frame not SEH | `MenuGame::FUN_00435de0(float,float)` known header; Game drawing/layout helper. |
| 0x435f50 | 368 | stdcall; frame not SEH | `MenuGame::FUN_00435f50(float,float)` known header; paired drawing/layout helper. |
| 0x4360c0 | 395 | thiscall; SEH 0x4d80de, FPU | `GameView::FUN_004360c0(float,float,float,float,int)` (known header): draws car icon in UI. |
| 0x436250 | 1605 | thiscall; SEH/FPU likely | **Apply race result / update record UI data**: operates on result/car/stage values and menu results state. External callers 0x44f0dd and 0x44f575. |
| 0x4368a0 | 1569 | thiscall; SEH/FPU likely | **Game UI interaction/result helper**: tests/updates selection or confirmation state, called by front-end input loop and menu/online screens (0x432f84/0x432fd7, 0x43a8d0, 0x448204/0x4485a9). |
| 0x436ed0 | 366 | ECX object; SEH 0x4d817e | Exact overlay draw callback: resource +0x1390f4, scaled rectangle (256,320)–(768,448), white color, full UVs; no stack arguments. |
| 0x437040 | 467 | thiscall; SEH 0x4d81a8 | Exact menu draw/update: video, title, seven labels, row decoration and cursor; skips Present when argument is 1. |
| 0x437220 | 1346 | fastcall; SEH (handler in prologue) | `GameView::FUN_00437220(int)` (known header): confirmation dialog; returns -1 while the dialog remains open. Callers 0x48961a and 0x48d75f. |
| 0x437770 | 1352 | fastcall; SEH (handler in prologue) | **Second modal prompt / confirmation flow**: dialog loop parallel to 0x437220, returns result to 0x48c75c. |
| 0x437cc0 | 351 | ECX object; SEH 0x4d824e | Exact strip draw callback: resource +0x1390dc, width argument and scaled height 32, U=.2–.3, white color; ret 0xc. |
| 0x437e20 | 66 | stdcall; leaf | Small final callback/thunk used by adjacent UI/resource machinery; no direct callsite recovered. |

REA Evidence `ev_e1f0bf3ec6bbfbed57e68d3ec7ce422d318338ab1a10657e4c4e28a40d87e309` confirms `0x437770` spans 1,352 bytes and keeps the answer in ESI, row Y in EBX, and both string-cleanup paths before the shared epilogue at `0x437ca3`. The current source's result-local/forward-goto form reduces the matcher diff from 102 to 83 bytes but grows the body to 1,358 bytes; VC6 emits the second cleanup after `ret`, so this remains NONMATCHING.

`GameView::FUN_00432b30` at `0x432b30` is byte-exact in `src/unk_4303e0.cpp`. Plain PE disassembly observation: EDI holds `this`; the active-stream branch requires the current ID at `+0x139214` to differ from -1 and the second stack argument to be nonzero, then records the timer, sets transition state 1, and stores the first argument at `+0x139218`. Otherwise it closes the stream, indexes the 16-byte stream-name table by the first argument, opens and starts that stream, computes `(global*127)/100`, stores the selected ID and clears transition state, substitutes the alternate volume for IDs 0, 2, or 3, calls SetStreamVolume, and saves the volume. Focused matcher: 27/27 marked functions match in this file (23 existing NONMATCHING).

Evidence `ev_06558af5896aec6fcef4ea896093b00a26135b492d3fd6efa75dd992ed90834d` records the 3,300-byte target. Focused matcher baseline is 28/28 marked functions compiled/matched (23 existing NONMATCHING); `objcmp` reports 3,301 built bytes and 580 differing bytes outside relocations. In particular, target loads `this` from `[esp+0x28]` into EBP at 0x43133d, while VC6 uses EBX; the one-byte size excess is accompanied by register-schedule changes through the body. Moving the four line-sprite style stores before or interleaved with their texture uploads worsened the diff (827 bytes / 3,340 bytes) and was reverted. No narrow source-order fix found; EH layout stayed intact.

## Vtables, compiler output, and missing symbols

- No `Game` vtable. The ctor begins by calling member constructors and never writes a vptr at offset 0. Individual screen constructors own their vtables; Game calls virtual slot +4 (Setup) and +8 (Release) on MenuManager/screens in 0x431c50/0x4323xx.
- No vtable is defined in this TU. No range function is a scalar-deleting destructor or obvious this-adjusting thunk. `0x430700/0x430720` are string element ctor/dtor callbacks passed to the VC6 vector helpers, and are compiler-support-shaped but hand-emitted members. The `_$E` stubs at 0x4303b0/0x437e70 are compiler guard functions. Vector construction/destruction (`0x4c11f4/0x4c1116`) and string allocation/release (`0x402aa0`, `0x402700`, `0x4bd000`) are out of range.
- The three switch tables are the only executable-range gaps with code-like bytes. They are not omitted functions. The functions.csv list is complete for callable starts in the requested range.

## Suggested work groups

Group by coherent source work; all probably belong in one eventual Game TU. Large routines should be handled individually.

| Group | Functions | Approx bytes | Focus |
|---|---|---:|---|
| G1 lifecycle/menu labels | 0x4303e0, 0x430700, 0x430720, 0x430760, 0x4309b0 | 898 | Game layout, unwind order, labels, remote records |
| G2 startup/config/asset lifecycle | 0x4309d0, 0x430cf0, 0x430eb0 | 1,368 | Front-end state switch, renderer creation/release |
| G3 UI/resource builders | 0x430f60 alone | 3,300 | Large label/resource setup; one worker |
| G4 UI resource release/create | 0x431c50, 0x431f40 | 2,931 | Resource unwind/create pairing; one worker |
| G5 frame and video helpers | 0x4327d0, 0x4329e0, 0x432a00, 0x432a10, 0x432a40, 0x432a60, 0x432aa0, 0x432b00, 0x432b30, 0x432c00 | 1,526 | Match MenuGame/GameView names and render frame semantics |
| G6 front-end interaction | 0x432e10 | 1,031 | Input, selection and transition loop; coordinate with G7 |
| G7 race start | 0x433220 | 2,435 | Player construction, setup route, mode state; one worker |
| G8 championship continuation | 0x433bc0 | 1,826 | Continue/next-stage state and player fields |
| G9 multiplayer setup | 0x434300, 0x434a50, 0x434ba0, 0x434dd0, 0x434df0 | 2,797 | Network-facing state; 0x434300 alone if needed |
| G10 race continuation/result | 0x434e00 | 1,572 | Result/next-stage transition; one worker |
| G11 UI callback cluster A | 0x435430, 0x4355a0, 0x435710, 0x435880, 0x4359f0, 0x435b60, 0x435ca0 | 2,462 | Compare repeated callback bodies and GameView helpers |
| G12 UI callback cluster B | 0x435de0, 0x435f50, 0x4360c0 | 1,131 | Compare the paired layout helpers and icon drawing |
| G13 result helper | 0x436250 | 1,605 | One worker; trace external result callers |
| G14 menu interaction helper | 0x4368a0 | 1,569 | One worker; trace external menu callers |
| G15 modal confirmation | 0x437220 | 1,346 | One worker; modal loop |
| G16 secondary modal confirmation | 0x437770 | 1,352 | One worker; parallel modal loop |
| G17 tail callbacks | 0x436ed0, 0x437040, 0x437cc0, 0x437e20 | 1,250 | Small table-driven UI/resource helpers |


For `G17_437cc0` (`0x437cc0`), evidence `ev_ffa5ae54c7b2a5043751751123119198e41d1828b59971966932852c498ab50a` shows the 351-byte target. Moving the `vf35` material call after the temporary arrays and vertex/UV stores increased the compiled body from 348 to 357 bytes and raised differing bytes from 235 to 244; the source order was restored. The call-order trial alone does not explain the broad schedule mismatch.
