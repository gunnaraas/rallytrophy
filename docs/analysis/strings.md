# RallyTrophy.exe — Defined-String Inventory & Subsystem Buckets

- Target: `extracted/Program Files/JoWooD/Rally Trophy/RallyTrophy.exe` (PE32 x86)
- SHA-256: `1fd65f9dfc2249f5649aa6db02b98ae783c7ad166d4bf6db8bcbaabca74afb44`
- Source: REA MCP `list_strings` (Ghidra 12.1.3, defined-string Data only), evidence `ev_43fe8f64…4793`
- Date: 2026-10-22. Total defined strings: **3280**.

Coverage caveat: Ghidra reports only *defined* string Data. Packed/undefined bytes
(and any non-ASCII/non-terminated text) can hide more strings; the real inventory is
likely larger. A few entries carry a spurious first byte from Ghidra's start
alignment (`<data/settings/camera.txt`, `Ddata/highscores/highscores.hs`, `.ATCH2` (=BATCH2),
`U. Aureli`, `0File`); read them minus the first character.

## Bucket summary

| Bucket | Count | Address ranges (main) | Examples |
|---|---|---|---|
| config_vehicle_physics_keys | 457 | 0x4ee768, 0x4ef724, 0x4f0204-0x4f0210, 0x4f1270-0x4f128c, 0x4f25c0, 0x4f4590-0x4f45a8, 0x4f5054-0x4f5a9c, 0x4f67a8-0x4f7bb8, 0x4f820c-0x4f8534 | DriveLightIntensity ; Brake |
| dx_hresult_name_table | 350 | 0x4db6f8-0x4dd6a0 | DMUS_S_STRING_TRUNCATED ; DMUS_S_LAST_TOOL |
| config_stage_track_keys | 324 | 0x4ec670-0x4ec964, 0x4ed834-0x4eda10, 0x4ee67c-0x4eeba8, 0x4ef99c, 0x4f05f8-0x4f0c10, 0x4f11c0-0x4f1360, 0x4f1b30-0x4f1b90, 0x4f2550-0x4f2638, 0x4f3068-0x4f3f6c, 0x4f463c-0x4f4b20 | LanguageInitials20 ; LanguageName20 |
| winapi_import_names | 315 | 0x4de2ac, 0x4ead52-0x4eb5e4, 0x6bb6e4-0x6bba40, 0x6c3e56-0x6c41d0, 0x6c5d9e-0x6c662e | MessageBoxA ; DirectInput8Create |
| config_surface_material_keys | 255 | 0x4eda24-0x4ee670 | T9Sound ; T8Sound |
| config_audio_keys | 138 | 0x4ef648-0x4ef990, 0x4f1abc-0x4f25ac, 0x4f44c8-0x4f4574, 0x4f7cc8-0x4f7e8c | Event1File ; Event1Volume |
| miles_bink_imports | 118 | 0x4ea70e-0x4ead30, 0x6c575a-0x6c5d72 | _BinkGetSummary@8 ; _BinkOpen@8 |
| ui_menu_element_ids | 108 | 0x4effd0-0x4f05d4, 0x4f0a30-0x4f11a0, 0x4f2f28-0x4f3498, 0x4f42b8-0x4f44b0 | TransmissionSelectionArrow2_LowerRight ; TransmissionSelectionArrow2_UpperLeft |
| debug_trace_msgs | 93 | 0x4eebb0-0x4eeed4, 0x4efde4-0x4efe64, 0x4f074c, 0x4f137c, 0x4f270c-0x4f27cc, 0x4f2bd8-0x4f2bec, 0x4f30f4, 0x4f45dc-0x4f4618, 0x4f4b4c-0x4f4b7c, 0x4f5ab4-0x4f5f28 | aidata ok ; init environment |
| config_keys_other | 88 | 0x4ec4dc, 0x4ecbac-0x4ecbb8, 0x4ee6ec, 0x4eed74-0x4ef00c, 0x4ef794-0x4ef9bc, 0x4f00f0-0x4f0224, 0x4f0b68-0x4f0b80, 0x4f13d4, 0x4f25f4-0x4f2e20, 0x4f3ad8 | frame ; AviRight |
| msvcrt_runtime | 78 | 0x4dd910-0x4de2f8, 0x4fbd74-0x4fbda8, 0x6bedbc-0x6bf248 | string too long ; invalid string position |
| config_ai_cpu_keys | 76 | 0x4ed818-0x4ed988, 0x4f251c-0x4f2540, 0x4f9a98-0x4f9d3c | CPUFTCSplit0 ; CPUSplit0 |
| audio_speech_pacenotes | 68 | 0x4faaa8-0x4fb2a0 | data/speech/windscreen_eng.wav ; data/speech/sweet_eng.wav |
| debug_printf_formats | 63 | 0x4ecbd4, 0x4eebbc-0x4eede4, 0x4efd74-0x4efe7c, 0x4f02c8, 0x4f07dc, 0x4f2aac-0x4f2cc8, 0x4f3af0, 0x4f3f98-0x4f3fc0, 0x4f4b6c, 0x4f8544-0x4f8990 | %s: preloaded %i bytes ; load aidata: %s |
| game_asset_paths | 59 | 0x4ed7f0, 0x4eeddd-0x4ef040, 0x4f026c-0x4f0290, 0x4f0788, 0x4f2758-0x4f2a38, 0x4f4724-0x4f47bc, 0x4f5af0-0x4f5ef8, 0x4f7be0, 0x4f8600-0x4f8698, 0x4f96b4-0x4f9730 | data/global/effects/water_loop1000.tga ; ky.bmf |
| net_dplay_session | 55 | 0x4f2ad0-0x4f2b7c, 0x4f897c, 0x4f927c-0x4f9798, 0x4fc048 | disconnect ok ; disconnect |
| ui_menu_assets | 54 | 0x4ec4e4-0x4ec510, 0x4f0238-0x4f02d4, 0x4f076c-0x4f0c44, 0x4f139c-0x4f13b8, 0x4f27ec-0x4f3110, 0x4f3a28-0x4f3b08, 0x4f3f7c-0x4f4150, 0x4f465c-0x4f46ec, 0x4f8944, 0x4f9698 | data/menu/loadpattern.tga ; data/menu/LoadingT |
| ui_stage_names | 54 | 0x4f8eb0-0x4f9238 | Switzerland Arcade 2 ; Switzerland Arcade 1 |
| vehicle_object_node_names | 43 | 0x4daa4a-0x4dae24 | CDdriver_body ; driver_head |
| fs_paths_game | 41 | 0x4ec974-0x4ec998, 0x4eedf8-0x4eeff4, 0x4efb68-0x4efe10, 0x4f02ac-0x4f02b8, 0x4f07c0, 0x4f1407, 0x4f2740-0x4f2ba8, 0x4f3a40-0x4f3ae0, 0x4f3fac-0x4f4190, 0x4f5bc4-0x4f5db0 | data/language/language.ini ; data/language/language_ |
| zlib_inflate_msgs | 33 | 0x4ec9cc, 0x4fbdc8, 0x4fc8ac, 0x50020c-0x5003c4, 0x5014e8-0x501634 | file error: %s %s ; file error |
| pacenote_display_names | 33 | 0x4ef9fc-0x4efd28 | VALTAISA TURSAS ; MERIROSVON PAPUKAIJA PUHUU RIVOJA |
| msvc_rtti_typeinfo | 32 | 0x4ec488-0x4ec4a0, 0x4ec9b8, 0x4fbcf8-0x4fbd48, 0x4fcd48-0x4fd198 | .?AVexception@@ ; .?AVruntime_error@std@@ |
| dev_sample_strings | 30 | 0x4ef9cc-0x4efb5c | SAMPLE 32 ; SAMPLE 31 |
| dll_names | 29 | 0x4de2b8, 0x4ea7e0, 0x4ead46-0x4eb0bc, 0x4fbe48, 0x6bb688-0x6bb694, 0x6c41e8, 0x6c574c-0x6c582c, 0x6c5d90-0x6c5dfe, 0x6c63e8-0x6c663a | user32.dll ; binkw32.dll |
| audio_debug_msgs | 27 | 0x4f2a84, 0x4f7f8c-0x4f801c, 0x4fa114, 0x4fb2bc-0x4fb3c4 | set sound track ; audioinit |
| av_files | 26 | 0x4f07a8, 0x4f1c58-0x4f1e08, 0x4f27dc, 0x4f46d4-0x4f470c, 0x4fc3f4-0x4fc434 | data/video/credits.bik ; data/music/tr5_a.mp3 |
| ui_option_value_labels | 24 | 0x4ecb3c-0x4ecb7c, 0x4fbf60-0x4fc040 | argb 8888 ; argb 4444 |
| renderer_shader_files | 24 | 0x4fc05c-0x4fc368 | data/shaders/Lake.sha ; data/shaders/DirtConstStandard.sha |
| tokens_misc | 23 | 0x4de370, 0x4f1ab4, 0x4f45c0, 0x4f5ecc, 0x4f9e0c-0x4fa188, 0x4fba10-0x4fbaa6, 0x4fc3c8-0x4fc3e4, 0x4fc888-0x4fcba8 |  inflate 1.1.3 Copyright 1995-1998 Mark Adle ; 0File |
| game_error_msgs | 23 | 0x4f86bc, 0x4fbd60-0x4fbe74, 0x4fc388, 0x4fc898-0x4fccc4 | Cannot find rain_window object ; unknown exception |
| parser_error_msgs | 22 | 0x4ec4bc-0x4ec4cc, 0x4ec9e4-0x4ecc30, 0x4ef028, 0x4f41a0, 0x4f4b30 | Invalid frame ' ; error in name |
| input_ff_debug_msgs | 16 | 0x4fa7a0-0x4fa93c | input: forced enumeration ; add device: ref |
| names_players_credits | 14 | 0x4f1b9c-0x4f1c4c, 0x4f2e78 | U. Aureli ; H. Koppinen |
| net_gamespy_protocol | 14 | 0x4f86f8-0x4f8828, 0x4fb9ac, 0x4fbe64 | CD Key or challenge too long ; key.gamespy.com |
| audio_soundfx_files | 13 | 0x4efdb0, 0x4f297c-0x4f29f0, 0x4f7ea0-0x4f7f60 | data/soundfx/ ; data/soundfx/clickillegal.wav |
| bmf_format_chunk_tags | 13 | 0x4fcaa8-0x4fcc38 | TRACK ; HIERARCHY |
| registry_paths_cli | 11 | 0x4fbce4-0x4fbf40, 0x4fcd24 | HDPath ; config |
| car_damage_marker_names | 8 | 0x4f5b04-0x4f5ba0 | #E1 window rightside ; #E1 window leftside |
| credits_title | 8 | 0x4f89a0-0x4f8a40, 0x4fc3a4-0x4fc45c, 0x4fc954 | Rally Trophy ; Rally Trophy (c) 2001 Bugbear Entertainment  |
| net_rtnet_protocol | 6 | 0x4f8964, 0x4f9e68-0x4f9ee0 | -RTNET_HCMD_SYNC: %i ; -RTNET_PCMD_PROGRESS %i: %i |
| net_cli_switches | 5 | 0x4f924c-0x4f9270 | +game ; +hostname |
| video_bink_debug | 3 | 0x4ecbec-0x4ecc18 | bink::highestio: %i ; bink::totaliomem: %i |
| digit_font_strings | 2 | 0x4da3b0, 0x4f3df0 | 0123456789 ; 0123456789 |
| other | 2 | 0x4f9df0, 0x4ffa68 | player::ResetChampionship() ;  	-] |
| ghidra_artifact_empty | 2 | 0xffdff1ac, 0xffdffc00 |  ;  |

Counts are classifier output (regex + explicit sets over the full 3280);
buckets are heuristic and a handful of borderline strings may sit in the wrong one.


## Address neighborhood map (data pages)

Game strings live in `.data`/`.rdata` pages 0x4d0000-0x5016ff; a second, mostly
third-party string area sits at 0x6bb000-0x6c66ff.

| Range | Content |
|---|---|
| 0x4da3b0-0x4dae24 | digit font string; car 3D node names (driver_body, tire_fl, light_*, shadow_*) |
| 0x4dab07 | `<data/settings/camera.txt` (leading byte artifact) |
| 0x4db6f8-0x4dd6a0 | DX HRESULT→name table (DMUS_/DSERR/D3DERR/DXFILEERR/DPNERR/DVERR/DIERR) |
| 0x4dd910-0x4de370 | MSVC CRT messages, `1#QNAN/INF/IND/SNAN`, zlib banner |
| 0x4ea70e-0x4eb5e4 | delay-load import names: `_Bink*`, `_AIL_*` (Miles), Win32 API, DLL names |
| 0x4ec488-0x4ecbb8 | RTTI names incl. custom `file_error`; parser errors; video format labels |
| 0x4ec670-0x4ec998 | language.ini key block: `LanguageInitials00-20`, `LanguageName00-20`, `NumLanguages` |
| 0x4ed7f0-0x4eda10 | track effects (waterfall/lake/river .ini), CPUSplit/CPUFTCSplit (frame-split AI) |
| 0x4eda24-0x4ee670 | surface material keys S0-S4, T0-T9, R0-R4 (+`*Sound` variants) |
| 0x4ee67c-0x4ef040 | world rendering keys (Fog/Sun/Shadow/SkidMark/Animal...), track build trace, effects paths |
| 0x4ef648-0x4ef990 | engine/skid sound keys (Acc/Brake/Slide/Ground/Roll Attenuation-Volume-Pitch) |
| 0x4ef9cc-0x4efe64 | `SAMPLE n` dev strings, pacenote display names, audio load trace |
| 0x4effd0-0x4f13b8 | control/gamesettings/graphics menu element ids + menu .ini/.tga paths |
| 0x4f1ab4-0x4f2510 | Sfx00-31 audio keys, Demo1-3 track/car keys, AI driver names |
| 0x4f2550-0x4f3110 | mainmenu/optionsmenu/racemenu text keys, `game.ini`, click wavs, `3dsounds.ini`, `ai1.ini` |
| 0x4f3068-0x4f4190 | rallymenu StageN* keys, repair/tune times, savegame paths |
| 0x4f463c-0x4f4b20 | soundmenu keys, driver/co-driver .bmf names, Camera1-9 keys |
| 0x4f5054-0x4f5a9c | car.ini physics: suspension, bumpers, steering, brakes, flares, aero, wipers, HUD overlay |
| 0x4f5af0-0x4f5f28 | car data files: `tires.ini`, `engine.ini`, `car.ini`, `collision.bmf`, `model.bmf`, damage markers |
| 0x4f67a8-0x4f7bb8 | tires dynamics.ini: `Dyn00`-`Dyn29` friction/stiffness/rolling-resistance sets |
| 0x4f7cc8-0x4f8534 | EngineSound/ExhaustSound keys, gearbox Gear1-6 + R ratios/inertias, clutch/differential |
| 0x4f8544-0x4f8a40 | skidmarks/weather assets, GameSpy strings, title/copyright, `RallyTrophy.exe` |
| 0x4f8eb0-0x4f9238 | stage list: `<Country> Stage/Arcade N`, `Rally/Arcade <Country>`, championships |
| 0x4f924c-0x4f97f4 | `+`-switches, DirectPlay connect/host strings, HUD overlay .tga, time format `%i%i:%i%i:%i%i` |
| 0x4f9a80-0x4f9e0c | `cpu01.ini`, `TrackNRate1-8`, `TrackN(Errors|Fatals)`, `CarModel*`, RTNET_PCMD debug |
| 0x4fa0c0-0x4fa1e0 | `video.ini`, `VideoOutFps`, CountDown, gameloop trace, `ScreenShots/rally_%i.tga` |
| 0x4fa7a0-0x4fa93c | DirectInput / force-feedback init trace |
| 0x4faaa8-0x4fb3c4 | 68 co-driver speech wavs; Miles init/load trace (`Miles Fast 2D`) |
| 0x4fb4e0-0x4fb55c | AI driver difficulty keys (SlowDown/Oversteering/Braking hardness, DriverName/Nationality) |
| 0x4fb9ac-0x4fbf40 | GameSpy key area: `arllytrophy`, `HDPath`, exception texts, `device.cfg`, `-setup`, **registry key**, URLs |
| 0x4fbf60-0x4fc434 | option labels (texture/audio), 24 `.sha` shaders, `Creating connection`, bink videos |
| 0x4fc888-0x4fd198 | BMF loader errors + chunk tags, `commandline:`/`cmd: %s`, `BeGetDevices`, STL RTTI |
| 0x50020c-0x501634 | zlib 1.1.3 tables/messages (second copy) |
| 0x6bb688-0x6bba40 | ASPI/SCSI + mutex/file API names — INFERENCE: CD-copy-protection (SafeDisc/SecuROM-style) check |
| 0x6bedbc-0x6bf248 | MSVC CRT runtime (second copy) |
| 0x6c3e56-0x6c663a | second import-name block (KERNEL32, binkw32, mss32, dinput8, winmm, user32, wsock32...) |

## Notable individual strings

Subsystem/module identifiers, original data formats, and names worth pinning down:

| Address | String | Note |
|---|---|---|
| 0x4fbef0 | `Software\Bugbear\Rally Trophy` | registry key; xref 0x49aee9 → `FUN_0049aea0` |
| 0x4fbce4 | `HDPath` | registry value next to it (install dir) |
| 0x4fbe8c | `data/options.cfg` | options file; xrefs 0x49b16e/0x49b188 → `FUN_0049aea0`, 0x49dff1 → `FUN_0049dc00` |
| 0x4fbebc | `data/device.cfg` | renderer/audio device selection |
| 0x4fbe48 | `device/bbdx8.dll` | renderer DLL loaded by relative path; xref 0x49b36b → `FUN_0049aea0` ("bb" = Bugbear, "dx8" = Direct3D 8) |
| 0x4fbee0 | `-setup` | command-line switch; xref 0x49af78 → `FUN_0049aea0` |
| 0x4f924c-0x4f9270 | `+game`, `+hostname`, `+host`, `+name`, `+connect` | multiplayer command-line switches |
| 0x4fcd0c | `commandline:` / 0x4fcd1c `cmd: %s` | cmdline echo (log) |
| 0x4fcd24 | `BeGetDevices` | INFERENCE: engine device-enumeration entry ("Be" = Bugbear engine prefix) |
| 0x4ec9b8 | `.?AVfile_error@@` | custom C++ exception `file_error` (RTTI) |
| 0x4fbd18 | `.?AVassertion_error@@` | custom C++ exception `assertion_error` |
| 0x4f9df0 | `player::ResetChampionship()` | log string containing original class::method name |
| 0x4fccc4 | `Model::SetDeformationData() failed because vertex counts differ` | original class/method: `Model::SetDeformationData` |
| 0x4fa1e0 | `Call mpTrack->audiostart` | log naming a member variable `mpTrack` (m-prefixed members, see `mCurSelection` 0x4f9838) |
| 0x5d.. n/a | `TRACK/HIERARCHY/MATERIAL/MATERIALLIST/MODEL/CAMERA/CAMERA2/LIGHT/BATCH/POLYLIST/VERTEXLIST` (0x4fcaa8-0x4fcb68) | `.bmf` model container chunk tags (Bugbear Mesh Format — INFERENCE) |
| 0x4f5d6c/0x4f5d7c | `collision.bmf`, `model.bmf` | per-car model + collision mesh files |
| 0x4f5d98-0x4f5db0 | `tires.ini`, `engine.ini`, `car.ini` | per-car config trio (under `data/cars/car_N/`) |
| 0x4f7bd0/0x4f7be0 | `dynamics.ini`, `data/global/dynamics/tire` | shared tire dynamics table |
| 0x4f7f80 | `Sounds.ini` | per-car sound events |
| 0x4f29f0 | `data/soundfx/3dsounds.ini` | 3D sound defs |
| 0x4f2ac0 | `data/AI/ai1.ini` | AI driver definitions |
| 0x4f9a80 | `data/settings/cpu01.ini` | CPU driver rates/errors per track |
| 0x4f2740 | `data/settings/game.ini` | global game settings |
| 0x4fa0cc | `data/settings/video.ini` | video settings |
| 0x4f1407 | `Ddata/highscores/highscores.hs` | high-score file `.hs` (leading byte artifact) |
| 0x4f4180/0x4f4190 | `savegame/rally`, `savegame/arcade` | save paths |
| 0x4f9858 | `data/replay/replay` (+ 0x4fa144 `data/replay/demomode`, 0x4f2ba8 `data/replay/demo.ini`) | replay/demo system |
| 0x4f8964 | `-RTNET_HCMD_SYNC: %i` | host command protocol (HCMD) |
| 0x4f9e68-0x4f9ee0 | `-RTNET_PCMD_PROGRESS/SETUP/CHECKPOINT/DAMAGEFRAME/KEYFRAME` | player command protocol (PCMD) |
| 0x4f86f8 | `CD Key or challenge too long` | GameSpy CD-key auth |
| 0x4f8718 | `key.gamespy.com` | GameSpy key server |
| 0x4f876c/0x4f8788 | `\disc\\pid\%d\cd\%s\ip\%d`, `\auth\\pid\%d\ch\%s\resp\%s\ip\%d\skey\%d` | GameSpy `\disc\`/`\auth\` protocol strings |
| 0x4f8818/0x4f8828 | `\uon\\skey\%d`, `\uoff\\skey\%d` | GameSpy user on/off |
| 0x4fb9ac | `arllytrophy` | INFERENCE: GameSpy game identifier (typo of "rallytrophy") |
| 0x4fb2bc | `Miles Fast 2D` | Miles Sound System driver label |
| 0x4ecbec-0x4ecc18 | `bink::highestio: %i`, `bink::totaliomem: %i`, `bink::soundskips: %i` | Bink IO/telemetry counters |
| 0x4fa1a4 | `ScreenShots/rally_%i.tga` | screenshot output (debug key) |
| 0x4f2afc/0x4f2b64 | `bugijahti: champ loop`, `bugijahti: get modes` | Finnish dev-speak ("bug hunt") debug leftovers |
| 0x4f2e78 | `KALJAKOPPA` | Finnish "beer den" — test string in optionsmenu data |
| 0x4ef9fc/0x4efafc | `VALTAISA TURSAS`, `MERIROSVON PAPUKAIJA PUHUU RIVOJA` | Finnish dev jokes among SAMPLE/pacenote strings |
| 0x4f97f8 | `Waiting For Players...` | MP lobby UI text |
| 0x4f976c | `%i%i:%i%i:%i%i` | HUD stage-time format (MM:SS:CC) |
| 0x4fc450 | `%I64x` | 64-bit hex format (CD key/steam-style id handling) |

## Leads for next analysis

1. **`FUN_0049aea0` (0x49aea0-0x49b6db)** is the main init: it references the registry
   key (0x4fbef0), `data/options.cfg` (×2), `device/bbdx8.dll` (0x49b36b), `-setup`
   (0x49af78), plus "set default options"/"load options"/"init translator"/"load icon".
   Decompile it first: it is the front door to config loading, renderer DLL loading,
   and the setup-mode switch. `FUN_0049dc00` (0x49dc00-0x49e027) is the options
   save/load counterpart.
2. **Config-file parser**: parser errors at 0x4ec9e4-0x4ecb7c ("missing Vector3 value",
   "unknown variable '", "storage file not found") mark the generic key-value storage
   class used by every `.ini`. Xref `0x4ecac0`/`0x4eca7c` to find the parser and the
   variable-dispatch table; the `S0`-`T9`-`R4`, `Dyn00`-`Dyn29`, `StageN*` key blocks
   (0x4eda24-0x4ee670, 0x4f67a8-0x4f7bb8, 0x4f3518-0x4f39a0) are the consumers.
3. **Car physics**: xref any key in 0x4f5054-0x4f5a9c (e.g. `FrontSuspensionRestLength`
   0x4f5208) to locate the car loader ("create car"/"load settings" at 0x4f5db8/0x4f5e08)
   and the physics init reading `car.ini`/`engine.ini`/`tires.ini`/`dynamics.ini`.
4. **Renderer interface**: `device/bbdx8.dll` + `data/device.cfg` + shader list
   0x4fc05c-0x4fc368 (`*.sha`) → find `LoadLibrary` call in `FUN_0049aea0` and the
   device vtable; "BeGetDevices" (0x4fcd24) is likely the engine's device enumerator.
5. **Game loop**: trace strings "enter gameloop"/"Exit gameloop" (0x4fa1c0/0x4fa194),
   "Call mpTrack->audiostart" (0x4fa1e0) → main loop body; "update dt: %i" (0x4fc424)
   for frame timing.
6. **Multiplayer**: RTNET HCMD/PCMD tags (0x4f8964, 0x4f9e68-0x4f9ee0) → host/player
   command handlers; GameSpy strings 0x4f86f8-0x4f8828 → CD-key validation routine;
   DirectPlay error strings 0x4f927c-0x4f94b4 → session setup UI.
7. **Audio**: `Miles Fast 2D` + `_AIL_open_digital_driver` xrefs → audio subsystem
   init; `data/soundfx/3dsounds.ini` → 3D sound loader; Sfx00-31 keys (0x4f1abc-
   0x4f2510) → per-event sample table.
8. **BMF model loader**: chunk tags 0x4fcaa8-0x4fcb68 + loader errors 0x4fca84-
   0x4fcba8 → model import code (car and track meshes).
9. **Copy protection**: ASPI/SCSI imports at 0x6bb9bc-0x6bba40 and CD-check strings
   ("Invalid CD Key" at 0x4fbe64/0x4f87b4) — check whether the GameSpy "Invalid CD Key"
   dialog doubles as the disc check; xref `0x4fbe64`.
10. **Menu system**: every `data/menu/*.ini` has a matching `Loading <X>menu resources`
    trace string; xref the trace to find each menu class, and the `*Text_UpperLeft`/
    `LowerRight` id blocks to find the menu layout loader.
