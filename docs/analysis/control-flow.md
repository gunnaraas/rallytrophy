# RallyTrophy.exe — Control-Flow Spine (REA/Ghidra analysis)

Target: `extracted/Program Files/JoWooD/Rally Trophy/RallyTrophy.exe`
(SHA-256 1fd65f9dfc2249f5649aa6db02b98ae783c7ad166d4bf6db8bcbaabca74afb44, PE32 x86).
All addresses are image-base 0x400000. Roles not backed by a direct string/vtable
observation are marked INFERENCE.

## Import indirection (read this first)

The copy protection (.cms_t/.cms_d sections) hides every API call:

- There are **no direct references to the IAT** (searched: 0-byte matches for any
  `6c 56 6c xx` / `f8 a2 69 xx` pointer immediate in .text/.cms_t/.cms_d).
- `.text` calls imports **through pointer slots in .cms_d** (0x699000–0x6c4fff),
  e.g. `call [0x0069a2f8]` = GetModuleFileNameA (confirmed by the
  `(0, buf, 0x400)` call pattern in FUN_00480420 and its outgoing READ ref to
  0x69a2f8). The slots are filled at runtime by the .cms_t wrapper; in the file
  they hold placeholder values, so Ghidra's decompiler renders many calls as
  `(*DAT_0069a2f8)(...)` or `(*(code *)0xeb580)(...)` with different *actual*
  slots per call site. Known slots:
  - 0x0069a2f8 → GetModuleFileNameA (DAT_00667420 buffer)
  - `(*(code *)0xeb580)(&msg,0,0,0,1)` in FUN_004b2480 → PeekMessageA(PM_REMOVE)
  - 0xeb5a4 → TranslateMessage, 0xeb590 → DispatchMessageA (IAT ground truth:
    PeekMessageA=0x6c56c0, DispatchMessageA=0x6c56c4, TranslateMessage=0x6c56c8)
- Consequence: `find_xrefs_to_name` on USER32 names only gives IAT slots, never
  call sites. USER32::CreateWindowExA / RegisterClassA / ShowWindow are **not
  imported at all** — the window is created by the renderer DLL (see below).

## WinMain

**FUN_0049aea0** (0x49aea0–0x49b6db, ~2.1 KB) is the WinMain-equivalent entry of
game code. Evidence: classic `WinMain(hinst, prev, lpCmdLine, show)` signature —
param_1 used as hInstance (DialogBox-style calls), param_3 strcmp'd against
`"setup"`/`"-setup"` (→ setup-mode flag DAT_0066df00), returns -1 on fatal init
error; MSVC6 SEH prologue; gets module path via GetModuleFileNameA and strips to
the exe directory (DAT_0066d870). It has **no resolved caller** — it is reached
from the obfuscated CRT main FUN_0068a6d0 in .cms_t via an indirect call
(INFERENCE, consistent with the known entry→CRT fact).

What it does, in order:
1. FUN_004bbe50/FUN_004bc548 — SEH/CRT init scaffolding.
2. FUN_004bb922(0x28)+FUN_004b7690 → **DAT_0066d638** (CD-key/registry object;
   destroyed at exit by FUN_004b76c0+FUN_004b8200 in FUN_0049b8f7).
3. FUN_00470cf0 = printf-style logger (called with "load settings", "config",
   "init translator", "game exit ok." strings) — INFERENCE: log/trace.
4. Loads `data/device.cfg` (0x98-byte struct **DAT_0066de30**, magic 0x20011015,
   default 640x480x16bpp; width/height/bpp at +0x68/+0x6c/+0x70, "lang" at
   +0x80) and `data/options.cfg` (0x1bc-byte struct **DAT_0066dc70**, magic
   0x20010918, copied to 0x4fb9d8 statics).
5. CD-key check loop (FUN_0049e030, "Invalid CD Key" message).
6. FUN_004bb922(0x28)+FUN_004b7840 → **DAT_0066dee8**; FUN_004bb922(0x2ac)+
   FUN_00491740 → **DAT_0066d624** (music/sound manager — see globals).
7. **LoadLibrary("device/bbdx8.dll")** via FUN_004b79e0 (string xref @0x49b36b).
8. Device enumeration: FUN_00447f30 → device list (**DAT_0066ded0**, 0x14-byte
   entries, selected index **DAT_0066deec**); per device, vtable **+0x0c** →
   display-mode list; matches cfg width/height/bpp; FUN_0049e2c0 adds candidate.
9. Unless setup mode: two dialog boxes (resource ids 0x6a/0x6b via .cms_d slots,
   dlgproc FUN_0049bbf0) — the "select video device" UI.
10. On success: FUN_004b7a70 → **FUN_0049cb10** (renderer init) →
    **FUN_0049dc00** (intro videos + main menu) → cleanup **FUN_0049b8f7**
    ("game exit ok." + destroy DAT_0066d638).
    Setup mode instead writes default device.cfg and exits.

## Main loop(s)

The game has three nested "loops"; the per-frame pump is the same everywhere.

### 1. Message pump / per-frame kernel — FUN_004b2480 (0x4b2480)

```
int FUN_004b2480(int waitMode) {
  if (DAT_004fcc00 == 0) return 0;              // g_running
  while ((r = PeekMessageA(&msg_0067b758,0,0,0,PM_REMOVE)) == 1 || waitMode) {
    if (r == 0) { if (waitMode && !obj->0x28) otherImport(); }   // idle wait
    else {
      if (msg.message == WM_QUIT) { DAT_0067b774 = 1; DAT_004fcc00 = 0; return 0; }
      TranslateMessage(&msg); DispatchMessageA(&msg);
    }
  }
  return DAT_004fcc00;
}
```
Globals: DAT_0067b758 = MSG, **DAT_004fcc00 = g_running**, **DAT_0067b774 = g_quit**.

### 2. Menu loop — FUN_00430f10 (0x430f10), driven by FUN_0042eb10 (0x42eb10)

FUN_0042eb10 = "main menu" (log string `"exit_mainmenu_loop"`; loads
`data/settings/game.ini` — NumberOfCars/NumberOfTracks/TorqueMultiplier/
ResetTime*/CPUGaps*; highscores; menu music `data/music/menu.mp3` via
FUN_004938c0). Its `while (!g_quit)` body dispatches on FUN_00430f10() → 0..5:
race/championship/options/etc.

FUN_00430f10 (6-item main menu, selected index at `this+0x12158`):
```
do {
  FUN_00430d00();                    // INFERENCE: input poll (sets DAT_0067b7xx)
  if (rendererDevice[10]) {          // device ready
    if (menuIndexChanged) { ... FUN_00422810(idx)/FUN_00422870 ... FUN_0049ad60() }
    if (keys ok) {                   // DAT_0067b7b5 / 0067b7a0
      FUN_00435030();                // INFERENCE: draw menu frame
      FUN_004b2dc0();                // clear per-frame input edges (see below)
    }
  }
} while (FUN_004b2480(0));           // message pump = loop heartbeat
```
Input globals: DAT_0067b7a0/0x1cfa3-style flags, DAT_0067b7b5, DAT_0067b7c3,
DAT_0067b7c8, DAT_0067b7ce, DAT_0067b7d0. **FUN_004b2dc0** zeroes 0x40 dwords at
DAT_0067b7a8 = per-frame input edge-state reset.

### 3. In-race game loop — FUN_0048d470 (0x48d470), per-driver wrapper FUN_0048c7f0

FUN_0048c7f0 = "run race event": loading screen + progress bar (FUN_004036d0),
`"load_track"`, `"setup_shadows"`, `"upload_textures"` log points, creates
camera (`"Car camera"`) and 0x194/0x230-byte helper objects, music tracks
0x20–0x43, then loops over drivers calling **FUN_0048d470(stage)** which returns
-2 (abort) / >0 (finished) / -1 (retry).

FUN_0048d470 = the real frame loop (log points `"enter_gameloop"`,
`"Exit_gameloop"`, `"Call mpTrack->audiostart"`, `"clear_events"`):

```
setup: renderer +0x7c(1,2) +0xc4(...) +0x9c(fog) +0xa0(4) +0x54(1.0f); FUN_004b2dc0();
FUN_004146f0();                     // event manager arm
iVar7 = FUN_004b2480(0);            // pump
do {
  if (pumpDead || aborted) { teardown; FUN_00414730(); return result; }
  FUN_00430d00();                   // input
  if (!device[10]) { FUN_004928b0(0); FUN_00452650(); FUN_004932a0(); }  // sound
  FUN_004a32f0();                   // INFERENCE: begin frame
  switch (subState) {               // pause/results dialogs, cases 1..10:
    case 1: FUN_00485030(); ... case 10: FUN_00487ac0(); }
  default:                          // normal frame
    if (screenshotKey DAT_0067b823) { save "ScreenShots/rally%i.tga" via +0x40/+0x44 }
    renderer +0x68();               // begin scene  (also in bink path FUN_00409540)
    FUN_0040f740(piVar1, driver);   // INFERENCE: draw cars
    FUN_004b3620(dev, piVar1);      // INFERENCE: terrain pass
    FUN_0045eb40(piVar1);           // INFERENCE: static objects pass
    renderer +0xa8(1) +0x78(3);
    FUN_0040fa20 / FUN_004b3b00 / FUN_004845a0 / FUN_0040fb20 / FUN_004b3c40 /
    FUN_0040fb50                    // world render passes (profiled 0,1,5,6,8)
    FUN_0045ee10 / FUN_00464370 / FUN_004649c0;   // INFERENCE: HUD
    renderer +0x90(0); +0x2c();     // end scene / present
  FUN_004147a0(dt);                 // INFERENCE: sim step (dt = frames*timeslice)
  DAT_0066d644 += frameTime;        // race clock
  FUN_0048e950();                   // frame limiter (FUN_004bc552 = timeGetTime,
                                    // ~0x5c/92 ms cap)
  music/sfx: FUN_004897d0/93a60/93870/93890/932a0;
  iVar7 = FUN_004b2480(local_48);   // pump = loop end
} while (true);
```
FUN_00403040/FUN_00403060 pairs = profiler marks (sections 0,1,2,5,6,7,8).
FUN_00409540 = Bink video per-frame updater (uses `this+0x9c` Bink handle,
`+0xa4` backbuffer, warns `"video lagging"`), used by the intro-movie loop in
FUN_0049dc00 together with FUN_004b2480/FUN_004b2dc0.

## Renderer interface (bbdx8.dll, loaded at FUN_0049aea0+0x4de)

Two related objects:
- **Device object** `DAT_0066d620` = entry picked from device list
  `DAT_0066ded0[idx*0x14]`; its vtable is observed at these offsets:
  | off | role | evidence |
  |-----|------|----------|
  | +0x0c | get display-mode list | FUN_0049aea0 loops modes ({w,h,bpp} at +4/+8/+0xc of each mode) |
  | +0x10 | app/window init (create window) | FUN_00480420 passes {0, L"Rally Trophy", hinst, exeDir, L"Rally Trophy (c) 2001 Bugbear Entertainment Ltd.", ...} |
  | +0x14 | device caps (flags at +4 of returned struct; bit3 → DAT_004fb9b8) | FUN_0049cb10 |
  | +0x24 | create display/device (params: name, hicon, icon 0x7f00, device desc, ...) → DAT_0066d640 | FUN_0049cb10 |
  | +0x28 | release/shutdown display (on create failure) | FUN_0049cb10 |
  | +0x2c | end scene / present | FUN_0048d470, FUN_00409540 |
  | +0x40 / +0x44 | get render target / release it (screenshot) | FUN_0048d470 saves TGA |
  | +0x50 | clear(0,0,0,0) | FUN_0048c7f0 teardown |
  | +0x54 | set ambient/alpha float | FUN_0048d470 |
  | +0x58 | begin scene variant (arg 1/2) | FUN_0048d470 |
  | +0x68 | begin scene (with clear) | FUN_0048d470, FUN_00409540 |
  | +0x78 | set texture (stage 3) | FUN_0048d470 passes |
  | +0x7c | set render state (state, value); (5,0)/(5,1),(6..9,cfg),(1,2),(0xd,0) | everywhere |
  | +0x90 | set depth state (0) | FUN_0049cb10/FUN_0048d470 |
  | +0x9c | set fog params | FUN_0048d470 |
  | +0xa0 | set texture-stage state (8)/(4) | FUN_0049cb10/FUN_0048d470 |
  | +0xa8 | set texture (1) | FUN_0048d470 |
  | +0xb8 | set transform (matrix, 1) | FUN_0048d470 |
  | +0xc4 | set camera/viewport (floats, 1) | FUN_0048d470 |
  | +0x120 | create shader from memory (name, data, idx 1..0xf) | FUN_0049cb10 loads data/shaders/*.sha |
  | +0x124 | shader validation (idx) → bool | FUN_0049cb10 |
- **"App" object** at `g_game+4` (filled by FUN_00480420's `this+4`, vtable
  called at +4 (begin, FUN_00431320) and +8 (end)); +0x10 on it = create
  window (see above). INFERENCE: bbdx8 "application" interface vs "device"
  interface.

## Top-level subsystem globals (.data 0x4ec000+, all observed)

| address | size | role | evidence |
|---|---|---|---|
| DAT_0066d63c | huge (≥0x125e8) | **g_game** singleton (this of menu/race methods) | offsets +0x5628 NumberOfCars, +0x53d8 NumberOfTracks, +0x125e8 TorqueMultiplier, +0x12158 menu idx, +0x12248 renderer ctx, +0x2d70 stage data |
| DAT_0066d620 | ptr | **g_rendererDevice** (bbdx8 device obj) | vtable calls throughout |
| DAT_0066d640 | ptr | **g_display** (created by dev+0x24) | FUN_0049cb10 → FUN_0048ee70/FUN_00452430 |
| DAT_0066d624 | 0x2ac | **g_music/sound** (FUN_00491740) | menu.mp3, SFX idx 0x20–0x43, FUN_00493a60/93870/93890 |
| DAT_0066d638 | 0x28? | CD-key/config object (FUN_004b7690/004b76c0) | created early in FUN_0049aea0, freed at exit |
| DAT_0066dee8 | 0x28 | config object #2 (FUN_004b7840) | FUN_0049aea0 |
| DAT_0066d628 | 0x188 | display-bound resource mgr (FUN_0048ee70(display,1)) — INFERENCE texture mgr | FUN_0049cb10 |
| DAT_0066d62c | 0x4a0 | big helper obj (FUN_0047fd10) — INFERENCE font/UI mgr | FUN_0049cb10 |
| DAT_0066d630/34 | 1? | tiny singletons (FUN_004b2840 / FUN_004b2540) | FUN_0049cb10 |
| DAT_0066de30 | 0x98 | device.cfg (magic 0x20011015) | FUN_0049aea0 |
| DAT_0066dc70 | 0x1bc | options.cfg (magic 0x20010918) | FUN_0049aea0 |
| DAT_0066ded0/ec | arr+idx | device list / selected device | FUN_0049aea0/0049cb10 |
| DAT_0066d61c, DAT_0066def4 | ptrs | hInstance / hIcon | FUN_0049aea0 |
| DAT_0066d870, DAT_00666c20 | str | exe directory | GetModuleFileNameA + strip |
| DAT_0066df00 | flag | setup mode ("-setup") | FUN_0049aea0 |
| DAT_0067b758 | MSG | message pump struct | FUN_004b2480 |
| DAT_004fcc00, DAT_0067b774 | flags | g_running, g_quit | FUN_004b2480 |
| DAT_0067b7a0–d0 | bytes | input state (keys/edges) | FUN_00430f10/0048d470 |
| DAT_0066d644 | int | race clock accumulation | FUN_0048d470 |
| DAT_004fbb30/34/38/3c/40, DAT_004fba00/04/0c | ints | menu/race mode state | FUN_0042eb10/00431320/0048d470 |

## Control-flow spine

```
0x6908c7 entry (.cms_t, XOR-decode wrapper)
  └─ FUN_0068a6d0  CRT main (.cms_t, obfuscated)            [known fact]
       └─ FUN_0049aea0  "WinMain": cfg, CD-key, device pick  (0x49aea0)
            ├─ FUN_0049e030  CD-key validate
            ├─ FUN_004b79e0  LoadLibrary("device/bbdx8.dll") (str 0x4fbe48)
            ├─ FUN_0049cb10  renderer/shader init            (0x49cb10)
            │    ├─ dev+0x24 create display → DAT_0066d640
            │    ├─ loads 15 shaders via dev+0x120 (data/shaders/*.sha)
            │    ├─ creates DAT_0066d628/62c/630/634
            │    └─ FUN_00480420  window create via app-iface+0x10 (0x480420)
            │         (GetModuleFileNameA → exe dir; title str 0x4f89bc)
            ├─ FUN_0049dc00  intro videos + loading screen  (0x49dc00)
            │    └─ FUN_00408740 "jowood.bik"/"intro.bik" player loop
            │         (FUN_004b2480 pump + FUN_00409540 bink frame)
            └─ FUN_0042eb10  MAIN MENU                      (0x42eb10)
                 └─ while(!g_quit) FUN_00430f10 menu loop    (0x430f10)
                      └─ case: FUN_00431320 championship flow (0x431320)
                           states: car select FUN_00424da0 →
                           driver FUN_00426150 → track FUN_0042cd90/
                           FUN_0043b000 → grid FUN_00420330 →
                           race FUN_0043ffa0 (per stage)
                           └─ FUN_00443800 stage init; iface+4/+8 begin/end;
                                per driver FUN_004442d0
                                └─ FUN_0048c7f0 race event    (0x48c7f0)
                                     └─ FUN_0048d470 GAME LOOP (0x48d470)
                                          └─ FUN_004b2480 pump every frame
            (exit) FUN_0049b8f7 cleanup "game exit ok."      (0x49b8f7)
```

## Next steps

1. Confirm the CRT→WinMain edge dynamically or by deobfuscating FUN_0068a6d0
   (static call graph shows no caller of FUN_0049aea0).
2. Map the .cms_d slot table (each slot ↔ import) by decompiling the .cms_t
   init/copy loops; then re-label all `(*DAT_0069a2f8)` decompiler noise.
3. Decompile FUN_0048d470 fully (saved dossier) and name the 10 sub-state
   dialogs (results/pause/replay) + the world-render passes FUN_004b3620 etc.
4. Identify DirectInput poll: FUN_00430d00 (menu/race call it every frame);
   DINPUT8 GetDeviceState should appear there through a .cms_d slot.
5. FUN_00431320's sibling FUN_00431ca0 (menu case 1) = single-race flow;
   diff the two to extract the common "session" structure.
6. g_game object map: DAT_0066d63c layout (0x12000+ bytes) — offsets already
   partially identified (0x53d8, 0x5510, 0x5618, 0x5628, 0x12158, 0x12248,
   0x1236c-0x1237c, 0x12540, 0x125e8).
