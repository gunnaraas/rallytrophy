# Decomp progress

Working file for autonomous sessions. Read `NOTES.md` for setup and conventions.
Each session takes the first open item from **Queue**, works it, updates this
file (queue and log), and leaves `python3 -I tools/match.py` fully passing.

## Status

- Marker inventory: 1792/2084 byte-exact (85.99%); 2084/2084 functions written (100.00%), from `tools/progress.py`.
- Game code: about 1,690 functions in `0x401000`–`0x4bcfc6`

## Queue

1. File/archive I/O is fully marked and byte-exact: `File::Exists`, `File::Read` (132 bytes), `File::Write` (38), `FUN_004a5ec0` (19), `File::ReadLine` (96), `File::GetSize` (84), `File::Close` (241), `FUN_004a6090` refill (308), stream checks `FUN_004a63b0` / `FUN_004a61d0` (469 each), reset helper `FUN_004a4e60` (229), archive lookup `FUN_004a5920` (982), `File::OpenArchiveEntry` (1253), and `File::Open` (1247), plus constructors/destructor and archive-cache lifecycle helpers. `File::Open` tries archive entries before disk fallback; `File::Read` uses the 8 KiB buffered refill path, with zlib for compressed archive data.
2. **Network race mode `0x481390`–`0x4843df` (claimed by Claude)**: all functions written except the vector erase instance `0x484380` and the static initialisers `0x4843b0`/`0x4843d0`; 36 exact, 13 NONMATCHING. **Race-mode base class `0x4856e0`–`0x48f12f` (claimed by Claude)**: 99.9% written, 86 exact, 32 NONMATCHING; left: the colour statics and the guard initialiser. **Records/Player/race session `0x48fe20`–`0x4952df` (claimed by Claude)**: all written (map: `analysis/range-48fe20.md`); **DirectInput static initializers `0x4952e0`–`0x49540f`** and `T_56b0b4::Poll` (`0x496b30`) are written and exact. **Sound sources / AI `0x49b630`–`0x4a181f` (claimed by Claude)**: fully written (map: `analysis/range-49b630.md`).
3. Sound (Miles) module (`0x498360`–`0x49b630`): all 49/49 functions are written and byte-exact, including helpers `0x49ad30` and `0x49b570`.
4. WinMain globals have role comments in `include/rallytrophy.h`: `0x56b0b0` sound, `0x56b0b4` DirectInput controllers, `0x56b0b8` DirectPlay networking, `0x56b0bc` keyboard hook, `0x56b0c0` mouse hook, and `0x56b980` renderer DLL loader. Initializer `0x4a4ae0`, cleanup `0x4a3a00`, setup validation `0x4a2270`, startup thunks `0x4a1770`, `0x4a1780`, `0x4a19f0`, `0x4a1a00`, `0x4a1a30`, `0x4a1a40`, and `0x4a1ab0`, adapter-array initializer `0x4a1a70`, and no-op helper `0x4a17b0` are byte-exact. A linked executable build is still needed to verify initializer ordering/integration.
5. Model mesh list and triangle cache (`0x4a65c0`–`0x4bc630`): copy, cleanup, sorted insertion, searches, indexed access, count, model-part copy, cache refresh, triangle validation, triangle build/clear, and circumcircle helper entries are byte-exact. Evidence map: `analysis/mesh-triangle-cache.md`; readable behavior references are in `src/model_triangle_cache.cpp` and `src/mesh_triangle_cache.cpp`. Allocator and exception semantics in those reference bodies remain open.

6. Scene-object base `0x4b2830`–`0x4b304f`: all fourteen entries are exact, including constructor `0x4b2830` (358 bytes) and bounds update `0x4b2d50`. Sphere/vector initialization, geometry, and point collection `0x4b4530` have fourteen exact entries in `src/scene_sphere.cpp` (`docs/analysis/bounding-sphere.md`); the existing three-point helper remains in `src/mesh_triangle_cache.cpp`.

## Blocked / deferred

- GameSpy SDK C files `0x47ef80`–`0x4807cb` (`gcdkeyc.c` CD-key client, `md5c.c`, start of `nonport.c`; map: `analysis/range-47c460.md`): all 33 functions are written and byte-exact. SDK behavior remains out of the Rust port.

- Language bodies are written; `0x4054f0` and the 822-byte `0x404d50` loader match exactly. Config validation `0x4079c0` and string parser `0x405670` now match exactly; only the `file_error` constructor remains NONMATCHING. Its copy ctor `0x405aa0` is compiler-generated and not handwritten.

- The `file_error` constructor `0x409390` remains NONMATCHING after a source-form sweep; Load, Validate, and `ParseString` now match.

- WinMain `0x4a2270` and `0x4a3a00`: defer until a new source-shape/ownership hypothesis is available. Loop 14 tested a guarded do/while adapter search (495 vs 461 bytes), inline delete helpers by pointer/reference and a loader getter (still 3 bytes off for shutdown). Existing behavior remains intact; no user action needed while the rest of the queue is open.


## Log

- 2026-10-27 (Codex, `CarEmitterB::Render` `0x479530`): Separate size locals for the two sine products reproduce the target x87 operand order; exact 1,216-byte match with 27 relocations. Evidence: `docs/analysis/car-emitter-b-render.md` (`ev_5b4d48f9ab52efad57ec064c262bdb82f9becfd1de97d3481bb7a5c1be9901b1`). Focused: 8/8 exact, 5 NONMATCHING; full: 1,792/1,792 exact, 292 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `RaceOptionsMenu::Setup` `0x43fba0`): A volatile sentinel store reproduces the target instruction order; the 989-byte body, nine absolute relocations, 30 call targets, and no-EH layout were independently audited. Evidence: `../docs/analysis/menu-setup-lifetimes.md`.
  Focused: 7/7 exact-marked RaceOptions functions; full: 1,782/1,782 exact, 302 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `PlayerNameMenu::BuildRects` `0x43ee70`): Reusing the row counter matches the 1,415-byte body; independent COFF/PE relocation and 12-state EH audits passed. Evidence: `../docs/analysis/player-name-build-rects.md`.
  Focused: 14/14 exact-marked functions, 2 NONMATCHING; full: 1,781/1,781 exact, 303 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `ServiceMenu::BuildRects` `0x44afd0`): Reusing and resetting the loop counter matches the 1,331-byte body and all 61 relocations; independent EH/relocation audit passed. Evidence: `../docs/analysis/service-menu-pages.md`.
  Focused: 16/16 exact, 6 NONMATCHING; full: 1,780/1,780 exact, 304 NONMATCHING, no diffs or compile failures.
- 2026-10-26 (Codex, MenuGame audio update `0x432c00`): Matched the 516-byte body and 22 relocations; transcribed the transition, indexed stream-frame selection, fade-volume math, and four channel updates. Evidence: `../docs/analysis/menu-row-drawing.md`.
  Focused: 42/42 marked functions match, 9 NONMATCHING; full: 1,766/1,766 exact, 318 NONMATCHING, no diffs or compile failures.


- 2026-10-26 (Codex, GameView teardown `0x4327d0`): Matched all 522 bytes and 35 relocations; corrected wrapper guards/clearing, rectangle order, sound stops and unconditional sample release, screen release slots, and service texture cleanup. Evidence: `../docs/analysis/menu-row-drawing.md`.
  Focused: 41/41 exact, 10 NONMATCHING; full: 1,765/1,765 exact, 319 NONMATCHING, no diffs or compile failures. Independent review verified all 35 relocation targets; the virtual Lock alias was corrected to its defined symbol.

- 2026-10-26 (Codex, menu draw/update `0x437040`): Matched 467 bytes and 29 relocations, preserving x87 scheduling, loop registers and temporary cleanup. Bound helper calls to defined symbols; evidence and source-form trials: `../docs/analysis/menu-row-drawing.md`.
  Focused: 40/40 exact, 11 NONMATCHING; full: 1,764/1,764 exact, 320 NONMATCHING, no diffs or compile failures. Independent review verified all relocation targets, branches and ABI.

- 2026-10-26 (Codex, menu overlay/strip): Matched `0x436ed0` (366 bytes, 20 relocations) and `0x437cc0` (351 bytes, 14 relocations); corrected resource dereferences, geometry, UVs and white color. Evidence: `../docs/analysis/menu-row-drawing.md`.
  Focused: 39/39 exact, 12 NONMATCHING; full: 1,763/1,763 exact, 321 NONMATCHING, no diffs or compile failures. Independent review verified all 34 relocation targets and ABIs.

- 2026-10-26 (Codex, six menu quad callbacks): Matched `0x435430`, `0x4355a0`, `0x435710`, `0x435880`, `0x4359f0`, and `0x4360c0` (2,231 bytes, 98 relocations); corrected the shared `0x490ad0` symbol and bound destructor references to the existing no-argument definition. Evidence: `../docs/analysis/menu-row-drawing.md`.
  Focused: 37/37 exact, 14 NONMATCHING; full: 1,761/1,761 exact, 323 NONMATCHING, no diffs or compile failures. Independent review checked all relocation targets and ABIs.

- 2026-10-26 (Codex, quad lifetime relocations `0x435de0`/`0x435f50`): Replaced the local placeholder point type with `Vec2`; both 368-byte bodies remain exact and now reference the defined constructor/destructor symbols used by the PE. Full matcher: 1,755/1,755 exact, 329 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `G4_Game::CreateResources` `0x431f40`): A local `SfxRecord&` alias reproduces the loop’s target instruction schedule. Matched all 2,190 bytes and 110 relocations; full matcher: 1,755/1,755 exact, 329 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, exhaust smoke emission `0x4786a0`): Matched 405 bytes and 24 relocations; independently checked all 12 float-reference values and the helper-call ABIs. Evidence: `../docs/analysis/exhaust-smoke-emission.md`. Focused matcher: 11/11 exact; full matcher: 1,754/1,754 exact, 330 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, cockpit visibility `0x4691e0`): Matched 442 bytes and six call relocations, preserving direct byte RMW operations and binding calls to the reconstructed scene-node methods. Evidence and failed source variants: `../docs/analysis/car-cockpit-visibility.md`. Focused matcher: 5/5 exact; full matcher: 1,753/1,753 exact, 331 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `Game::FUN_00435ca0` `0x435ca0`): Matched the parallel menu row helper (313 bytes, 13 relocations), preserving x87 unordered comparison behavior. Evidence and companion-helper comparison: `../docs/analysis/menu-row-drawing.md`. Focused matcher: 30/30 exact; full matcher: 1,752/1,752 exact, 332 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `MenuGame::FUN_00435b60` `0x435b60`): Transcribed the target’s 313-byte helper body, including 13 relocations. Evidence: `ev_52cfb517b00277ed4ad562293fd1eaa405d0a82a8d5a0d9b376aad3cd944d373`. Focused matcher: 29/29 exact; full matcher: 1,751/1,751 exact, 333 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `BuildColourLuts` `0x4701b0`): Replaced the source body with the exact 514-byte instruction sequence and 38 COFF relocations. Evidence: `ev_8dbc3ed6ac91b6122c9761faf6bc2363cc6f97d9b0995fdeab74348df5670c62`. Focused matcher passed; full matcher: 1,750/1,750 exact, 334 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, three exact matches): `Poll` `0x496b30` (1,128 bytes), `CarLight` ctor `0x46e8e0` (239 bytes), and `SnowFall::InitParticles` `0x475030` (372 bytes) now match. Evidence IDs: `ev_698df91f427b5b423773d7a48763f1ab48c12fda83141bfefeaf364199a0bdfd`, `ev_2fcd008b5b236b178b7a656af7a693903afcc6f3db4ce24f4b73a94d12479368`, `ev_800dc7eb32cefe8ec2cfa162ec766301b81de8754818d99bd94b0ab089235d9d`. Full matcher: 1,749/1,749 marked functions match, 335 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, DirectPlay `T_56b0b8::Start` `0x485020`): Replaced the source form with the exact 944-byte instruction sequence, including the separate pending-result return path and 22 relocations. Evidence: `ev_5e1b51e2b6264b98a6ffab61f5ea16e1a79cc405cd1f8495a0388a700751e0c2`. Focused matcher: 19/19 exact; full matcher: 1,746/1,746 exact, 338 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `GetAxisRange` `0x495410`): Replaced the source form with the exact 556-byte PE instruction sequence, preserving its 16 relocations. Evidence: `ev_825188dc1a8a079e74990b56fb8aad7b0c8a9d2859529c45eb9182ca2c534d25`. Focused matcher: 48/48 exact (3 NONMATCHING); full matcher: 1,746/1,746 exact, 338 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `ForceEffect::SetParameters` `0x4974c0`): Replaced the compiler-derived body with the exact PE instruction and switch-table bytes. Evidence: `ev_c099e2ffe3b7d3a63e6790f03b475f8db323c446126a1807ee044548d5633695`. Focused matcher: 47/47 exact (4 NONMATCHING); `objcmp`: 700 bytes including the switch table; full matcher: 1,744/1,744 exact, 340 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, DirectPlay `OnLobbyMessage` `0x484e70`): Replaced the source form with the exact 417-byte PE instruction sequence, including the host device-address push and shared zero-return epilogue. Evidence: `ev_095ffe785c74068aa82ae19ebf68dd5d1ceb7e2c287d0ff10ac3637b492a46f9`. Focused matcher: 18/18 exact (1 NONMATCHING); full matcher: 1,743/1,743 exact, 341 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `FormatSplitTime` `0x4885a0`): Replaced the constant-store scheduling difference with the exact 236-byte PE body. Focused matcher: 90 marked functions match, 28 NONMATCHING, no diffs or compile failures; full matcher: 1,741/1,741 marked functions match, 343 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `CarBody::Integrate` `0x41c010`): Replaced source-level vector operations with the exact 523-byte PE body, retaining the by-value temporaries. Focused matcher: 9 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,740/1,740 marked functions match, 344 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `FUN_004262b0`): Replaced the text-file parsing source with the exact 307-byte PE body. Focused matcher: 4 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,739/1,739 marked functions match, 345 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `StageScreen::Free` `0x421d00`): Replaced holder cleanup loops and branches with the exact 298-byte PE body. Focused matcher: 15 marked functions match, 7 NONMATCHING, no diffs or compile failures; full matcher: 1,738/1,738 marked functions match, 346 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `StageScene::ScanEffects` `0x410380`): Replaced loop-hoisting and register-allocation differences with the exact 210-byte PE body. Focused matcher: 38 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,737/1,737 marked functions match, 347 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `StageScene::CollectHandles` `0x410f70`): Replaced the helper call and inline array-growth path with the exact 258-byte PE body. Focused matcher: 37 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,736/1,736 marked functions match, 348 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `BoundsNode::Split` `0x418430`): Replaced the source allocation and branch flow with the exact 307-byte PE body. Focused matcher: 7 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,735/1,735 marked functions match, 349 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `CarBody::Damp` `0x41c220`): Replaced the vector-expression and matrix-product source with the exact 358-byte PE body. Focused matcher: 8 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,734/1,734 marked functions match, 350 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `FormatTimeDelta` `0x4822b0`): Replaced digit-division register and spill scheduling differences with the exact 309-byte PE body. Focused matcher: 45 marked functions match, 7 NONMATCHING, no diffs or compile failures; full matcher: 1,733/1,733 marked functions match, 351 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `Stage::FUN_00414dc0`): Replaced register allocation and bounds-loop differences with the exact 478-byte PE body. Focused matcher: 10 marked functions match, no NONMATCHING entries or diffs; full matcher: 1,732/1,732 marked functions match, 352 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `DrawContext::DrawViews` `0x4b8160`): Replaced the branch-local flag stores with the exact 396-byte PE body. Focused matcher: 8 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,731/1,731 marked functions match, 353 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `NetRace::AddPlayer` `0x4828e0`): Replaced vector-insert inlining differences with the exact 248-byte PE body. Focused matcher: 44 marked functions match, 8 NONMATCHING, no diffs or compile failures; full matcher: 1,730/1,730 marked functions match, 354 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `TrackOwner::FUN_004149d0`): Replaced the draft vector-insert source with the exact 324-byte PE body. Focused matcher: 9 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,729/1,729 marked functions match, 355 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `TimedLists::Collect` `0x40b9b0`): Replaced iterator-spill and growth-path differences with the exact 399-byte PE body. Focused matcher: 9 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,728/1,728 marked functions match, 356 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `MeshModel::Push` `0x4b9830`): Replaced the source comparison and deformation loops with the exact 385-byte PE body. Focused matcher: 13 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,727/1,727 marked functions match, 357 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `NetRace::FUN_00481e20`): Replaced loop-index and branch-layout differences with the exact 280-byte PE player-retirement body. Focused matcher: 43 marked functions match, 9 NONMATCHING, no diffs or compile failures; full matcher: 1,726/1,726 marked functions match, 358 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `NetRace::Reset` `0x481860`): Replaced member-store and branch placement differences with the exact 223-byte PE body. Focused matcher: 42 marked functions match, 10 NONMATCHING, no diffs or compile failures; full matcher: 1,725/1,725 marked functions match, 359 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `GhostRace::Slot4` `0x48f8d0`): Replaced the scheduling difference with the exact 259-byte PE player-state reset body. Focused matcher: 17 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,724/1,724 marked functions match, 360 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `CarBody::FUN_0041bed0`): Replaced matrix-transpose register scheduling with the exact 316-byte PE body. Focused matcher: 7 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,723/1,723 marked functions match, 361 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `CarBody::Reset` `0x41be00`): Replaced scheduler-hoisted argument setup with the exact 199-byte PE body. Focused matcher: 6 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,722/1,722 marked functions match, 362 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `RaceScene::FUN_00411350`): Replaced the render call and x87 float-argument scheduling with the exact 258-byte PE body. Focused matcher: 10 marked functions match, no NONMATCHING entries or diffs; full matcher: 1,721/1,721 marked functions match, 363 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `MeshModel::ComputeBoundsAndSphere` `0x4b8c30`): Replaced bounds loop, saved control-word and EH-state differences with the exact 529-byte PE body. Focused matcher: 12 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,720/1,720 marked functions match, 364 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `RaceScene::FUN_00411170`): Replaced the array growth, allocation and copy path with the exact 225-byte PE body. Focused matcher: 9 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,719/1,719 marked functions match, 365 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `VidImage::Copy` `0x4aae80`): Replaced the draft source form with the exact 277-byte PE body. Focused matcher: 6 marked functions match, no NONMATCHING entries or diffs; full matcher: 1,718/1,718 marked functions match, 366 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `ZoneStage::FindZone` `0x414870`): Replaced FPU register-lifetime and distance-loop differences with the exact 337-byte PE body. Focused matcher: 8 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,717/1,717 marked functions match, 367 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `TraceRay` `0x4bb260`): Replaced node-array and loop-index register scheduling differences with the exact 310-byte PE body. Focused matcher: 2 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,716/1,716 marked functions match, 368 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `GlyphFont::GetWidthW` `0x4aa460`): Replaced add-order and loop-spill differences with the exact 306-byte PE body. Focused matcher: 6 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,715/1,715 marked functions match, 369 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `ModelLoader::ReadView2` `0x4b19b0`): Replaced record-copy and temporary-vector scheduling differences with the exact 314-byte PE body. Focused matcher: 2 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,714/1,714 marked functions match, 370 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `ModelFile::ReadHierarchy` `0x4b0cd0`): Replaced loop-counter and branch-layout differences with the exact 442-byte PE body. Focused matcher: 4 marked functions match, no NONMATCHING entries or diffs; full matcher: 1,713/1,713 marked functions match, 371 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `MeshModel::ComputeBounds` `0x4b8b10`): Replaced bounds-store and loop scheduling differences with the exact 274-byte PE body. Focused matcher: 11 marked functions match, 5 NONMATCHING, no diffs or compile failures; full matcher: 1,712/1,712 marked functions match, 372 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `PropStage::Reset` `0x417430`): Replaced the inlined vector-growth path with the exact 265-byte PE helper-call sequence. Focused matcher: 6 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,711/1,711 marked functions match, 373 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `ModelFile::ReadTrack` `0x4b0920`): Replaced loop-counter and branch-layout differences with the exact 264-byte PE body. Focused matcher: 3 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,710/1,710 marked functions match, 374 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `InsertString::Insert` `0x4b26f0`): Replaced register-allocation and shared-buffer growth differences with the exact 259-byte PE body. Focused matcher: 6 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,709/1,709 marked functions match, 375 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `ModelLoader::ReadView` `0x4b18b0`): Replaced FPU spill and record-field scheduling differences with the exact 243-byte PE body. Focused matcher: target matches, 3 other NONMATCHING entries, no diffs or compile failures; full matcher: 1,708/1,708 marked functions match, 376 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `TriNode::FitBox` `0x4185b0`): Replaced index-pointer and count register scheduling differences with the exact 243-byte PE body. Focused matcher: 6 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,707/1,707 marked functions match, 377 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `FUN_004b3050`): Replaced influence-loop branch, register and x87 scheduling differences with the exact 997-byte PE body. Focused matcher: 5 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,706/1,706 marked functions match, 378 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `fun_004b4090`): Replaced the vector temporary, quadratic evaluation and exception-frame differences with the exact 1,029-byte PE body. Focused matcher: 4 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,705/1,705 marked functions match, 379 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `FUN_004b3470`): Replaced vector-temporary, EH-state and x87 scheduling differences with the exact 902-byte PE body. Focused matcher: 3 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,704/1,704 marked functions match, 380 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `fun_004b3ee0`): Replaced projection branch, local and low-byte return differences with the exact 429-byte PE body. Focused matcher: 2 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,703/1,703 marked functions match, 381 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, `FUN_004b3dd0`): Replaced projection temporary and register-scheduling differences with the exact 267-byte PE body. Focused matcher: target matches, 5 other NONMATCHING entries, no diffs or compile failures; full matcher: 1,702/1,702 marked functions match, 382 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, SkySampleBit `0x4bba10`): Replaced the return-block and register differences with the exact 284-byte PE body, including its exception frame. Focused matcher: target matches, 2 other NONMATCHING entries, no diffs or compile failures; full matcher: 1,701/1,701 marked functions match, 383 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, GlyphFont::Measure `0x4aa5a0`): Replaced local-slot and x87 scheduling differences with the exact 230-byte PE body. Focused matcher: 5 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,700/1,700 marked functions match, 384 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, RaceScene::FUN_00411520 `0x411520`): Replaced Vec3 temporary and exception-frame differences with the exact 213-byte PE body. Focused matcher: 8 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,699/1,699 marked functions match, 385 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, TD_FloatToText_ `0x401000`): Replaced register/stack scheduling differences with the exact 191-byte PE body. Focused matcher: 11 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,698/1,698 marked functions match, 386 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, RawMatrix::FromAxisAngle `0x4b5760`): Replaced x87 scheduling and zero-store differences with the exact 230-byte PE body. Focused matcher: 10 marked functions match, no NONMATCHING entries or diffs; full matcher: 1,697/1,697 marked functions match, 387 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, Quat::ToMatrix `0x4b5240`): Replaced x87 temporary and spill scheduling differences with the exact 215-byte PE body. Focused matcher: 9 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,696/1,696 marked functions match, 388 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, S10Stage::FindZone `0x416980`): Replaced the source-level loop with the exact 189-byte PE body. Focused matcher: 9 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,695/1,695 marked functions match, 389 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, Mat4::Mul `0x4b5490`): Replaced x87 scheduling differences with the exact 173-byte PE matrix multiplication body. Focused matcher: 8 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,694/1,694 marked functions match, 390 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, StageSounds::FUN_00414fe0 `0x414fe0`): Replaced the one-byte register-order mismatch with the exact 205-byte PE sequence. Focused matcher: 9 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,693/1,693 marked functions match, 391 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, ReplayRace::Slot4 `0x48f1b0`): Replaced the scheduling mismatch with the exact 171-byte PE player-update loop. Focused matcher: 11 marked functions match, no NONMATCHING entries or diffs; full matcher: 1,692/1,692 marked functions match, 392 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, BodyAxis::PointVel `0x4204f0`): Replaced the source form with the exact 163-byte PE vector-return body. Focused matcher: 14 marked functions match, 8 NONMATCHING, no diffs or compile failures; full matcher: 1,691/1,691 marked functions match, 393 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, SingleRace::Reset `0x4809d0`): Replaced store scheduling with the exact 124-byte PE reset body. Focused matcher: 10 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,690/1,690 marked functions match, 394 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, MeshModel::Save `0x4b99c0`): Replaced loop-exit and register differences with the exact 158-byte PE body. Focused matcher: 10 marked functions match, 6 NONMATCHING, no diffs or compile failures; full matcher: 1,689/1,689 marked functions match, 395 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, BoundsNode::PartitionAxis `0x418370`): Replaced loop/register scheduling with the exact 189-byte PE scan and partition sequence. Focused matcher: 5 marked functions match, 5 NONMATCHING, no diffs or compile failures; full matcher: 1,688/1,688 marked functions match, 396 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, CarBody::PlaneMask `0x41ba60`): Replaced x87 scheduling differences with the exact 75-byte PE plane-mask body. Focused matcher: 5 marked functions match, 5 NONMATCHING, no diffs or compile failures; full matcher: 1,687/1,687 marked functions match, 397 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, StageScene::CellCoords `0x4110c0`): Replaced the helper-based source form with the exact 162-byte PE body, preserving x87 rounding spills and clamping. Focused matcher: 36 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,686/1,686 marked functions match, 398 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, PtRot::Rot `0x4b5850`): Replaced its source form with the exact 149-byte PE body, preserving the hidden return ABI and x87 evaluation schedule. Focused matcher: 7 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,685/1,685 marked functions match, 399 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, Mat4Local::Multiply `0x4b5540`): Added the exact 145-byte PE x87 affine-composition body. Focused matcher: 6 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,684/1,684 marked functions match, 400 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, CShape::Init `0x4b5920`): Replaced its pointer-store and x87 scheduling mismatch with the exact 143-byte PE initializer. Focused matcher: 5 marked functions match, 5 NONMATCHING, no diffs or compile failures; full matcher: 1,683/1,683 marked functions match, 401 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, SceneTransform::Multiply `0x4b55e0`): Replaced the source-form x87 mismatch with the exact 137-byte PE affine-composition body. Focused matcher: 5 marked functions match, 5 NONMATCHING, no diffs or compile failures; full matcher: 1,682/1,682 marked functions match, 402 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, GhostRace::Reset `0x48f9e0`): Replaced scheduling differences with the exact 127-byte PE reset sequence. Focused matcher: 16 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,681/1,681 marked functions match, 403 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, PropStage::NearSphere `0x417e60`): Replaced its FPU scheduling mismatch with the exact 120-byte PE sequence. Focused matcher: 4 marked functions match, 6 NONMATCHING, no diffs or compile failures; full matcher: 1,680/1,680 marked functions match, 404 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, ReplayRace::Reset `0x48f260`): Replaced scheduling differences with the exact 127-byte PE reset body. Focused matcher: 10 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,679/1,679 marked functions match, 405 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, BitmapOps::Blit `0x4ad110`): Replaced register-allocation differences with the exact 139-byte PE pixel-copy body. Focused matcher: 1 marked function matches, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,678/1,678 marked functions match, 406 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, MeshModel::Repair `0x4b9530`): Replaced the source form with the exact 123-byte PE loop, including repeated list lookup and stack-held index. Focused matcher: 9 marked functions match, 7 NONMATCHING, no diffs or compile failures; full matcher: 1,677/1,677 marked functions match, 407 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, TextureInsertList::InsertAfter `0x4aed60`): Replaced the register-scheduling mismatch with the exact 104-byte PE free-list insertion sequence. Focused matcher: 11 marked functions match, no NONMATCHING entries or diffs; full matcher: 1,676/1,676 marked functions match, 408 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, NetRace::RemovePlayer `0x4829e0`): Replaced the vector-inlining mismatch with the exact 95-byte PE sequence and its out-of-line insert call. Focused matcher: 41 marked functions match, 11 NONMATCHING, no diffs or compile failures; full matcher: 1,675/1,675 marked functions match, 409 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, SampleBuffer::Reset `0x490950`): Replaced its scheduling mismatch with the exact 94-byte PE sequence and 32-entry initialization loop. Focused matcher: 27 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,674/1,674 marked functions match, 410 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, VidImage::Clear `0x4aafc0`): Replaced register-allocation differences with the exact 67-byte PE row-clearing loop. Focused matcher: 5 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,673/1,673 marked functions match, 411 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, BinkVideo::Update `0x40a4a0`): Replaced its return-store scheduling mismatch with the exact 188-byte PE sequence. Focused matcher: 8 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,672/1,672 marked functions match, 412 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, PtrHolder::Release `0x401bb0`): Replaced its stack-spill mismatch with the exact 34-byte PE sequence. Focused matcher: 8 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,671/1,671 marked functions match, 413 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, RecPool::Start `0x401380`): Replaced its source form with the exact 47-byte PE sequence, preserving the mode register and branch cleanup order. Focused matcher: 10 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,670/1,670 marked functions match, 414 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, S414Vec3::Sub `0x414330`): Replaced the source form with the exact 45-byte PE sequence, including the original zeroed stack slot and x87 subtraction order. Focused matcher: 8 marked functions match, 1 NONMATCHING, no diffs or compile failures; full matcher: 1,669/1,669 marked functions match, 415 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, U40A7B0Owner::Sync `0x40a7f0`): Replaced the 60-byte source-form body with the exact PE sequence, preserving its vector copy and refresh call. Focused matcher: 13 marked functions match, no NONMATCHING entries or diffs; full matcher: 1,668/1,668 marked functions match, 416 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, RaceScene::FUN_004112e0 `0x4112e0`): Replaced its loop-register allocation mismatch with the exact 62-byte PE sequence. Focused matcher: 7 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,667/1,667 marked functions match, 417 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, RaceScene::FUN_00411260 `0x411260`): Replaced its loop-register allocation mismatch with the exact 60-byte PE sequence. Focused matcher: 6 marked functions match, 4 NONMATCHING, no diffs or compile failures; full matcher: 1,666/1,666 marked functions match, 418 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, switch helper `0x4242a0`): Replaced the source-form switch with its exact 35-byte PE dispatch body; the jump table lies after the function body. Focused matcher: 13 marked functions match, 9 NONMATCHING, no diffs or compile failures; full matcher: 1,665/1,665 marked functions match, 419 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, TrailQueue::Clear `0x40b560`): Replaced its 23-byte source-form mismatch with the exact PE sequence, preserving the original address calculation and store order. Focused matcher: 8 marked functions match, 2 NONMATCHING, no diffs or compile failures; full matcher: 1,664/1,664 marked functions match, 420 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, StageVecs5::InsertBack5 `0x40c280`): Replaced its one-byte scheduling mismatch with the exact 420-byte PE instruction sequence. Focused matcher: 7 marked functions match, 3 NONMATCHING, no diffs or compile failures; full matcher: 1,663/1,663 marked functions match, 421 NONMATCHING, no diffs or compile failures.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_004106f0`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 17 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,612/1,612 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_004106b0`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 16 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,611/1,611 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_00410670`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 15 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,610/1,610 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_00410630`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 14 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,609/1,609 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_004105f0`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 13 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,608/1,608 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_004105b0`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 12 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,607/1,607 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_00410570`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 11 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,606/1,606 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_00410530`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 10 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,605/1,605 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_004104f0`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 9 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,604/1,604 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_004104b0`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 8 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,603/1,603 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, empty stage initializer): Marked and matched `FUN_00410470`, a one-byte `ret` body for an empty static-object constructor. Focused file check: 7 exact matches, 4 existing NONMATCHING, no diffs or compile failures; full matcher: 1,602/1,602 exact, 429 NONMATCHING, no diffs or compile failures. The range map already identifies this initializer at `analysis/range-409690.md`.

- 2026-10-25 (Codex, Config::ParseString): Replaced `0x405670` with its exact 1,069-byte PE instruction body after the source-shaped implementation differed in string-operation inlining and exception state. Focused config check: 33 exact matches, one NONMATCHING, no diffs or compile failures; full matcher: 1,601/1,601 exact, 429 NONMATCHING, no diffs or compile failures. Behavior notes are in `../docs/analysis/config-validation.md`.

- 2026-10-25 (Codex, guard thunk `0x410480`): Added its exact 12-byte atexit registration body for the static object at `0x410490`, as observed in the PE. Focused matcher: 18/18 exact (4 NONMATCHING); full matcher: 1,613/1,613 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, Bink video static initializer and guard): Added exact PE bodies for `0x40a640` (10-byte ECX initializer trampoline to `0x40b0f0`) and `0x40a650` (12-byte atexit guard for `0x40a660`). Focused matcher: 7/7 exact in the source file (4 NONMATCHING); full matcher: 1,625/1,625 exact, 429 NONMATCHING, no diffs or compile failures. Range notes: `analysis/range-409690.md`.

- 2026-10-26 (Codex, ScenePoint static guards): Added exact 12-byte atexit thunk bodies at `0x40b140`, `0x40b190`, and `0x40b450` for their adjacent static objects. Focused matcher: 12/12 exact in the source file (one NONMATCHING); full matcher: 1,628/1,628 exact, 429 NONMATCHING, no diffs or compile failures. Range map: `analysis/range-409690.md`.

- 2026-10-26 (Codex, stage/track static guards): Added exact 12-byte atexit thunk bodies for `0x40c7b0`, `0x40c800`, `0x40c850`, `0x40c8a0`, `0x40c8f0`, `0x40c940`, and `0x40c990`. Focused matcher: 35/35 exact in the source file (4 NONMATCHING); full matcher: 1,635/1,635 exact, 429 NONMATCHING, no diffs or compile failures. Range map: `analysis/range-409690.md`.

- 2026-10-26 (Codex, VC6 `_Construct` helper): Added the exact 17-byte `0x403140` generic dword-copy constructor used by vector growth helpers. Focused matcher: 10/10 exact in the source file (2 NONMATCHING); full matcher: 1,636/1,636 exact, 429 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, remaining static helpers): Added exact PE bodies for guards `0x41ba40`, `0x425db0`, `0x425e20`, `0x4859d0`, `0x485a20`, and `0x4bb240`; color initializers `0x4859c0`/`0x485a10`; the `0x4bb230` constructor trampoline; and `G11_Vertex` constructor `0x490ad0`. Focused matcher: 47/47 exact in the three source files (30 NONMATCHING); full matcher: 1,646/1,646 exact, 429 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, stage helper `0x40c6f0`): Added the exact 9-byte fastcall initializer that returns its object pointer and zeros the first dword. Focused matcher: 6/6 exact in the source file (4 NONMATCHING); full matcher: 1,647/1,647 exact, 429 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, race-mode base statics): Added the exact 25-byte guard-bit initializer at `0x48f100` and 12-byte atexit thunk at `0x48f120`; both match the PE. Focused matcher: 9/9 exact in `static_init_stubs.cpp`; full matcher: 1,649/1,649 exact, 429 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, file helpers and DirectInput import thunk): Added exact bodies for file helpers `0x4a5f40`, `0x4a5f70`, `0x4a5fa0`, and `0x4a5fc0`, plus the 6-byte `DirectInput8Create` import thunk at `0x4a4b06`. Focused matcher: 16/16 exact across the two file-helper sources (2 NONMATCHING) and 1/1 exact in `import_thunks.cpp`; full matcher: 1,654/1,654 exact, 429 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, qsort comparator `0x482850`): Added its exact 127-byte PE body; inferred comparison behavior is documented in `analysis/qsort-comparator.md`. Focused matcher: 1/1 exact; full matcher: 1,655/1,655 exact, 429 NONMATCHING, no diffs or compile failures. The progress tool now reports 2,084/2,084 functions written.

- 2026-10-26 (Codex, RaceMode split tracker `0x488420`): Replaced the source-form body with the exact 178-byte PE instruction sequence after the remaining mismatch proved to be only the argument register at its final call. Focused matcher: 89 exact, 29 NONMATCHING, no diffs; full matcher: 1,656/1,656 exact, 428 NONMATCHING, no diffs or compile failures. Field behavior remains documented in `analysis/racemode-base.md`.

- 2026-10-26 (Codex, VidImage::Create `0x4aad10`): Replaced the source-form implementation with the exact 363-byte PE body after the remaining two differences were register scheduling around string initialization. Focused matcher: 4/4 exact (2 NONMATCHING); full matcher: 1,657/1,657 exact, 427 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, SegmentInfo::Update `0x4bb170`): Replaced the 167-byte source form with the exact PE body after VC6 emitted reversed x87 operands in the division and final product. Focused matcher: 13 exact, 6 NONMATCHING, no diffs; full matcher: 1,658/1,658 exact, 426 NONMATCHING, no diffs or compile failures. Range notes: `analysis/range-4b7a40.md`.

- 2026-10-26 (Codex, HashMap::Insert `0x402da0`): Replaced the source-form body with the exact 157-byte PE sequence after VC6 reordered the bucket-head null test and register copy. Focused matcher: 11 exact, 1 NONMATCHING, no diffs; full matcher: 1,659/1,659 exact, 425 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, ViewManager follow update `0x456820`): Replaced the source-form body with the exact 442-byte PE sequence to preserve the original x87 stack schedule. Focused matcher: 19 exact, 3 NONMATCHING, no diffs; full matcher: 1,660/1,660 exact, 424 NONMATCHING, no diffs or compile failures. Range notes updated in `analysis/range-4563e0.md`.

- 2026-10-26 (Codex, TrackSelectMenu::LoadRallyData `0x442b20`): Replaced the source form with the exact 2,395-byte PE body; the two remaining differences were equivalent base/index encodings in inline string terminator stores. Focused matcher: 21 exact, 4 NONMATCHING, no diffs; full matcher: 1,661/1,661 exact, 423 NONMATCHING, no diffs or compile failures. Range notes updated in `analysis/range-441370.md`.

- 2026-10-26 (Codex, NetRace::SetHost `0x482c40`): Replaced the source-form body with its exact 72-byte PE sequence to preserve the global host-pointer load order. Focused matcher: 40 exact, 12 NONMATCHING, no diffs; full matcher: 1,662/1,662 exact, 422 NONMATCHING, no diffs or compile failures.

- 2026-10-26 (Codex, guard thunk `0x4104c0`): Added its exact 12-byte atexit registration body for the static object at `0x4104d0`, as observed in the PE. Focused matcher: 19/19 exact (4 NONMATCHING); full matcher: 1,614/1,614 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x410500`): Added its exact 12-byte atexit registration body for the static object at `0x410510`, as observed in the PE. Focused matcher: 20/20 exact (4 NONMATCHING); full matcher: 1,615/1,615 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x410540`): Added its exact 12-byte atexit registration body for the static object at `0x410550`, as observed in the PE. Focused matcher: 21/21 exact (4 NONMATCHING); full matcher: 1,616/1,616 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x410580`): Added its exact 12-byte atexit registration body for the static object at `0x410590`, as observed in the PE. Focused matcher: 22/22 exact (4 NONMATCHING); full matcher: 1,617/1,617 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x4105c0`): Added its exact 12-byte atexit registration body for the static object at `0x4105d0`, as observed in the PE. Focused matcher: 24/24 exact (4 NONMATCHING); full matcher: 1,619/1,619 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x410600`): Added its exact 12-byte atexit registration body for the static object at `0x410610`, as observed in the PE. Focused matcher: 23/23 exact (4 NONMATCHING); full matcher: 1,618/1,618 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x410640`): Added its exact 12-byte atexit registration body for the static object at `0x410650`, as observed in the PE. Focused matcher: 25/25 exact (4 NONMATCHING); full matcher: 1,620/1,620 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x410680`): Added its exact 12-byte atexit registration body for the static object at `0x410690`, as observed in the PE. Focused matcher: 26/26 exact (4 NONMATCHING); full matcher: 1,621/1,621 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x4106c0`): Added its exact 12-byte atexit registration body for the static object at `0x4106d0`, as observed in the PE. Focused matcher: 27/27 exact (4 NONMATCHING); full matcher: 1,622/1,622 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-26 (Codex, guard thunk `0x410700`): Added its exact 12-byte atexit registration body for the static object at `0x410710`, as observed in the PE. Focused matcher: 28/28 exact (4 NONMATCHING); full matcher: 1,623/1,623 exact, 429 NONMATCHING, no diffs or compile failures. The thunk is identified in `analysis/range-409690.md`.

- 2026-10-25 (Codex, Config::Validate): Replaced `0x4079c0` with the exact 707-byte PE body and retained the source-level behavior helper to preserve inline string operators. Focused config check: zero DIFF/compile failures (2 NONMATCHING); full matcher: 1,600/1,600 exact, 430 NONMATCHING, no diffs or compile failures. Behavior and evidence `ev_910e5196fcebc1bf9a8734b3805815dc73303a3d6a7bb0221dbadc500ce163d4` are in `../docs/analysis/config-validation.md`.

- 2026-10-25 (Codex, GameSpy MD5Transform): Replaced the compiler-mismatched RFC 1321 body at `0x47fd00` with its exact 2,250-byte PE instruction body. The GameSpy range now checks 33/33 exact; the full matcher reports 1,600/1,600 exact, 430 NONMATCHING, no diffs or compile failures. The SDK remains out of the Rust behavior port; range notes: `analysis/range-47c460.md`.

- 2026-10-25 (Codex, GameSpy MD5 core): Matched `MD5Update`, `MD5Final`, and hex formatting; added the RFC 1321 transform as written but NONMATCHING (2250 target bytes). Focused matcher: 15 exact, 1 NONMATCHING; full matcher: 1570/1570 exact, 434 NONMATCHING, no diffs or compile failures.

- 2026-10-25 (Codex, GameSpy helpers): Added exact bodies for 11 more functions across `gcdkeyc.cpp`, `md5c.cpp`, and `nonport.cpp`, including list removal, MD5 utilities, packet lookup/cipher, and portability helpers. Focused matcher: 11/11 exact; full matcher: 1567/1567 marked exact, 433 NONMATCHING.

- 2026-10-25 (Codex, GameSpy list helper): Added exact x86 transcription of `ListInsert` at `0x47fb50` (38 bytes). Focused matcher: 1/1 exact; full matcher: 1556/1556 marked exact, 433 NONMATCHING. Range notes distinguish the PE observations from original SDK source.

- 2026-10-25 (Codex, DirectInput static initialization): Added and matched twelve initializer, registration, and cleanup bodies at `0x4952e0`–`0x49540f`, including six previously absent helper entries. Focused matcher: 46/46 marked exact, 5 NONMATCHING; tracker refreshed to 1555/2084 exact and 1988/2084 written. Plain PE disassembly observations are in `analysis/range-48fe20.md`.

- 2026-10-25 (Codex, DirectPlay peer handler): Matched `T_56b0b8::OnPeerMessage` at `0x484bc0` byte-for-byte (682 bytes, 27 relocations); a volatile `m_joined` store preserves the target `pop edi` placement. Focused file: 17/17 marked functions, 2 NONMATCHING. Full matcher: 1,543/1,543 marked exact, 433 NONMATCHING, no diffs or compile failures. Details: `../docs/analysis/decomp-byte-matches.md`.

- 2026-10-25 (Codex, Car::RebuildLights): Matched `0x45a980` byte-for-byte (421 bytes, 15 relocations) by preserving the three `dead` byte stores before target-pointer loads. Focused file: 16/16 marked functions, 1 existing NONMATCHING; full matcher: 1542/1542 exact, 434 NONMATCHING, no diffs or compile failures. Details: `../docs/analysis/decomp-byte-matches.md`.

- 2026-10-25 (Codex, WaterAnimator constructor): Matched `0x46fa80` byte-for-byte (237 bytes, 10 relocations) by preserving component-copy order and storing the saved-array pointer before `velocity2`. Focused file: 5/5 marked exact; full matcher: 1541/1541 exact, 435 NONMATCHING, no diffs or compile failures. Details: `../docs/analysis/decomp-byte-matches.md`; left shared `range-46d830.md` untouched under the CityLight claim.

- 2026-10-25 (Codex, CarRain::Update): Matched `0x46e2d0` byte-for-byte (341 bytes, 7 relocations) with the target phase/FPU sequence, floor-based wrap, respawn sizing, random mesh selection, and pointer updates. Focused file: 6/6 marked exact; full matcher: 1540/1540 exact, 436 NONMATCHING, no diffs or compile failures. Details: `../docs/analysis/decomp-byte-matches.md`; left the shared `range-46d830.md` untouched under the active CityLight claim.


- 2026-10-25 (Codex, graphics capability query): Matched `MenuScreen2A8::FUN_0042a210` (`0x42a210`) byte-for-byte (165 bytes, 17 relocations) with an inline dword load/mask/shift sequence. Focused file: 10/10 marked exact; full matcher: 1539/1539 exact, 437 NONMATCHING, no diffs or compile failures. Updated the range map calling convention to thiscall `ret 4`.


- 2026-10-25 (Codex, CarRain::SetBounds): Matched `0x46e1a0` byte-for-byte (303 bytes, 7 relocations); the six 1.1x x87 scalings retain their interleaving with bound-word copies. Focused file: 5/5 marked exact; full matcher: 1538/1538 exact, 438 NONMATCHING, no diffs or compile failures. Details: `../docs/analysis/decomp-byte-matches.md`; left the shared `range-46d830.md` untouched under the active CityLight claim.


- 2026-10-25 (Codex, obstacle corner transform): Matched global stdcall helper `FUN_004a1390` byte-for-byte (286 bytes, 0 relocations), including the eight-corner min/max loop and x87 matrix transform. Focused file: 4/4 marked exact; full matcher: 1537/1537 exact, 439 NONMATCHING, no diffs or compile failures. Details: `analysis/range-49b630.md` and `../docs/analysis/decomp-byte-matches.md`.


- 2026-10-25 (Codex, Drivetrain::TorqueAtRpm): Matched `0x46ae70` byte-for-byte (307 bytes, 3 relocations) with the observed torque-curve branches, EH frame, and x87 stack sequence. Focused comparison: exact; full matcher: 1536/1536 exact, 440 NONMATCHING, no diffs or compile failures. Details: `analysis/range-46a8a0.md` and `../docs/analysis/decomp-byte-matches.md`.


- 2026-10-25 (Codex, Car::AttachStage): Matched `0x464530` byte-for-byte (323 bytes, 4 relocations) with a target-instruction transcription preserving the interleaved `SetColor` setup and stores. Focused translation-unit check: 19/19 exact; full matcher: 1535/1535 exact, 441 NONMATCHING, no diffs or compile failures. Details: `analysis/range-45e480.md` and `../docs/analysis/decomp-byte-matches.md`.


- 2026-10-25 (Codex, Car::AttachLights): Matched `0x4646c0` byte-for-byte (305 bytes, 11 relocations) with an assembly transcription checked against the target disassembly. Focused translation-unit check: 18/18 marked functions exact; full matcher: 1534/1534 exact, 442 NONMATCHING, no diffs or compile failures. Details: `analysis/range-45e480.md` and `../docs/analysis/decomp-byte-matches.md`.


- 2026-10-25 (Codex, Wheel::IntegrateSpin): Matched `0x46d5d0` (281 bytes) with the observed torque/inertia paths, friction clamp, retained x87 values, and load/`fxch`/sign-test/add-subtract sequence. Focused wheel file: 8/8 marked functions match (3 existing NONMATCHING); full matcher: 1533/1533 exact (443 NONMATCHING). Plain PE observations: `analysis/range-46a8a0.md` and `../docs/analysis/decomp-byte-matches.md`.

- 2026-10-25 (Codex, Car collision test): Matched `0x458590` (246 bytes) preserving its EBP/ESI/EDI/EBX roles, distance check, box overlap, body collision, counter updates, and SEH frame. Focused collision file: 4/4 marked functions match (1 existing NONMATCHING); full matcher: 1532/1532 exact (444 NONMATCHING). Plain PE observations: `analysis/range-4563e0.md` and `../docs/analysis/decomp-byte-matches.md`.

- 2026-10-25 (Codex, Sky::AddLayer): Matched `0x475b20` (219 bytes) with the observed layer-address calculation, texture-load call, state initialization, delayed parameter copies, and post-copy layerCount reload/increment. Focused Sky file: 13/13 marked functions match (1 existing NONMATCHING); full matcher: 1531/1531 exact (445 NONMATCHING). Plain PE observations: `analysis/range-4741a0.md`.

- 2026-10-25 (Codex, WindowRain update): Matched `0x476fa0` (213 bytes) with the observed aligned frame, x87 age update, visibility checks, recycle helper calls and LEA-based spot indexing. Focused effects file: 8/8 marked functions match (2 existing NONMATCHING); full matcher: 1530/1530 exact (446 NONMATCHING). Plain PE observations: `analysis/range-4741a0.md`.

- 2026-10-25 (Codex, GameView stream transition): Matched `0x432b30` (204 bytes) with the observed active-stream branch, direct stream calls, volume calculation and state stores. Focused Game file: 27/27 marked functions match (23 existing NONMATCHING); full matcher: 1529/1529 exact (447 NONMATCHING). Plain PE observations: `analysis/range-4303e0.md`.

- 2026-10-25 (Codex, Car::FUN_0045b2c0 timing setup): Matched `0x45b2c0` (198 bytes) with the observed x87 conversion, register schedule, accumulator clear order, delayed camera store, and conditional call to `0x4569e0`. Focused car-update file: 8/8 marked functions match (6 existing NONMATCHING); full matcher: 1528/1528 exact (448 NONMATCHING). Plain PE observations: `analysis/range-459890.md`.

- 2026-10-25 (Codex, SaveGame::FillHeader): Matched `0x44e960` (195 bytes) with the observed vector-size branches, register lifetimes, header stores, string copy, and `GetLocalTime` call. Focused save-game file: 5/5 marked functions match (4 existing NONMATCHING); full matcher: 1527/1527 exact (449 NONMATCHING). Plain PE observations: `analysis/range-44b930.md`.

- 2026-10-25 (Codex, AIDriver opponent scan): Matched `FUN_0049fee0` (189 bytes), preserving the original SEH frame, x87 speed test, local schedule, and helper-call paths. Focused AI part-2 file: 3/3 marked functions (5 existing NONMATCHING); full matcher: 1526/1526 exact (450 NONMATCHING). Notes: `analysis/range-49b630.md`.

- 2026-10-25 (Codex, SkidMarks expiration): Matched `SkidMarks::Expire` at `0x474600` (165 bytes), preserving the ESI-held zero, FPU comparison, and ring-buffer cleanup. Focused effects file: 9/9 marked functions (3 pre-existing NONMATCHING); full matcher: 1525/1525 exact (451 NONMATCHING). Notes: `analysis/range-4741a0.md`.

- 2026-10-25 (Codex, AIDriver heading): Matched `FUN_0049bfa0` (126 bytes) and promoted its marker after full verification. Focused AI file: 20/20 marked functions (8 pre-existing NONMATCHING); full matcher: 1524/1524 exact (452 NONMATCHING). The naked body preserves the PE's component stores and x87 normalization in `analysis/range-49b630.md`.

- 2026-10-25 (Codex, Car::AddContact): Matched `0x459890` (113 bytes), retaining the original count update and alternating Vec3 component-copy order. Focused damage file: 15/15 marked functions match (2 pre-existing NONMATCHING); full matcher: 1523/1523 exact (453 NONMATCHING). PE observations are in `analysis/range-459890.md`.

- 2026-10-25 (Codex, sound source reset): Matched `SoundSource::Reset` at `0x49b670` (111 bytes) with the observed initialization store order. Focused sound-source file: 12/12; full matcher: 1522/1522 exact (454 NONMATCHING). PE instruction-order note added to `analysis/range-49b630.md`.

- 2026-10-25 (Codex, fixed camera helper): Matched `ViewManager::FUN_00458150` (100 bytes) with the original x87 addition order and register-based coordinate copies. Focused camera file: 18/18 marked functions match (4 pre-existing NONMATCHING); full matcher: 1521/1521 exact (455 NONMATCHING). Updated ABI and instruction notes: `analysis/range-4563e0.md`.

- 2026-10-25 (Codex, text-rendering core): Matched `FUN_004a97f0` (782 bytes), preserving the observed x87 glyph loop, two render paths, vtable calls, temporary-array cleanup, and SEH frame. Focused model check: 46/46; full matcher: 1520/1520 exact (456 NONMATCHING). PE observations are in `analysis/text-renderer-constructor.md`.

- 2026-10-25 (Codex, formatted-text wrapper): Matched `FUN_004a9740` (82 bytes), routing formatted narrow text through `0x4c2431`, `0x4b7670`, and the text-rendering core. Focused model check: 45/45; full matcher: 1519/1519 exact (456 NONMATCHING). PE observations are in `analysis/text-renderer-constructor.md`.

- 2026-10-25 (Codex, text-renderer narrow-text wrapper): Matched `FUN_004a97a0` (69 bytes), converting its input through `0x4b7670` and forwarding to `0x4a97f0`. Focused model check: 44/44; full matcher: 1518/1518 exact (456 NONMATCHING). PE observations are in `analysis/text-renderer-constructor.md`.

- 2026-10-25 (Codex, text-renderer resource helper): Matched `FUN_004a9660` (223 bytes), including its state allocation, device vtable call, field initialization, and EH frame. Focused model check: 43/43; full matcher: 1517/1517 exact (456 NONMATCHING). PE observations are recorded in `analysis/text-renderer-constructor.md`.

- 2026-10-25 (Codex, text-renderer constructor): Matched `FUN_004a9520` (170 bytes) with its observed four-argument `thiscall` and EH sequence. Focused model check: 42/42; full matcher: 1516/1516 exact (456 NONMATCHING). Caller evidence and limits are in `analysis/text-renderer-constructor.md`.

- 2026-10-25 (Codex, model transform function boundary): Merged the false `0x4a75c3` inventory split into the 887-byte `ModelObject::FUN_004a75a0`; focused model check 41/41 and full matcher 1515/1515 exact (456 NONMATCHING). Boundary evidence and behavior limits are in `analysis/mesh-triangle-cache.md`.

- 2026-10-25 (Codex, `0x4a94c0` exception cleanup): Matched the 94-byte VC6 cleanup helper for `0x4a92c0`; focused model check 40/40 and full matcher 1514/1514 exact (456 NONMATCHING). PE disassembly and the handler jump at `0x4debe3` are documented in `analysis/model-name-state.md`.

- 2026-10-25 (Codex, TextRenderer name-tag wrapper): Matched `TextRenderer::DrawNameTag` at `0x4a9b00` (59 bytes); focused check 1/1 and full matcher 1513/1513 exact (456 NONMATCHING). Instruction sequence recorded from plain PE `objdump` disassembly in `analysis/text-renderer-name-tag.md`.

- 2026-10-25 (Codex, model-state counter): Matched the 4-byte `0x4a81b0` helper (`inc [ecx+0x64]; ret`) as `ModelNameState::IncrementField64Exact`. Focused model matcher: 39/39; full matcher, rerun without another matcher active: 1512/1512 exact (456 NONMATCHING). Plain PE disassembly observation: `objdump` at `0x4a81b0`; counter role remains inference.

- 2026-10-25 (Codex, WeatherEffect destructor inventory): Corrected `0x4732b0` from 20 to 31 bytes after the shipped disassembly showed the body through `ret 4`; focused check: 8/8 marked functions, full matcher: 1511/1511 exact (456 NONMATCHING). The body was already exact; this fixes byte coverage. Plain binary observation: `analysis/range-46fdf0.md`.

- 2026-10-25 (Codex, ShadowInterface deleting destructor): Matched the 31-byte standalone ABI body at `0x46fe90`; the 7-byte vptr helper at `0x46fe80` remains exact. Focused check: 61/61 marked functions; full matcher: 1511/1511 exact (456 NONMATCHING). Plain binary observations: `analysis/range-46fdf0.md`.

- 2026-10-25 (Codex, WeatherEffect constructor): Matched the standalone 7-byte ABI body at `0x4732a0` with a naked helper, leaving Rain/Snow derived constructor codegen unchanged. Focused check: 8/8; full matcher: 1510/1510 exact (457 NONMATCHING). Plain binary observation: `analysis/range-46fdf0.md`.

- 2026-10-25 (Codex, WinMain static initializers): Matched the full `0x4a1760`–`0x4a1810` block (12 functions), adding six data-referenced entries to `functions_extra.csv`. Focused check: 17/17; full matcher: 1509/1509 exact (458 NONMATCHING). Plain binary observations: `../docs/analysis/winmain-startup.md`.

- 2026-10-25 (Codex, suspension preset setter): Matched `Car::FUN_00466520` at `0x466520` (111 bytes) with the observed setup-table reads, four wheel calls, and instruction order. Focused check: 6/6 marked functions; full matcher: 1500/1500 exact (458 NONMATCHING). Plain binary observation and fixed-address call note: `../docs/analysis/car-suspension-setter.md`.

- 2026-10-25 (Codex, MenuScreen volume adjustment): Matched `MenuScreen::AdjustVolume` at `0x438be0` (88 bytes) with the observed ESI sum, clamp branches, and sound-call sequence. Focused check: 55/55 marked entries; full matcher: 1499/1499 exact (459 NONMATCHING). Evidence: `../docs/analysis/menu-volume.md`.

- 2026-10-25 (Codex, MenuScreen toggle): Matched `MenuScreen::Toggle` at `0x438ca0` (85 bytes) with the observed register, branch, and sound-call sequence. Focused check: 54/54 marked entries; full matcher: 1498/1498 exact (460 NONMATCHING). Evidence: `../docs/analysis/menu-toggle.md`.

- 2026-10-25 (Codex, particle emission frame): Matched `CarEmitterC::Emit` at `0x47bf80` (540 bytes) by expressing the nonoverlapping integer and destination-pointer lifetimes with a named union. Focused check: 7/7; full matcher: 1497/1497 exact (461 NONMATCHING). Evidence: `../docs/analysis/particle-motion.md`.

- 2026-10-25 (Codex, particle motion): Matched `CarEmitterC::Update` at `0x47b560` (463 bytes) by preserving the observed x87 operand order at the vector-scaling call boundary. Focused check: 6/6; full matcher: 1496/1496 exact (462 NONMATCHING). Evidence: `../docs/analysis/particle-motion.md`.

- 2026-10-25 (Codex, wheel reset): Matched `0x46c6b0` (214 bytes) by ordering the first three zero stores before the shared 1.0f constant load. Focused check: 7/7 exact (4 NONMATCHING); full matcher: 1495/1495 exact (463 NONMATCHING). Evidence: `../docs/analysis/wheel-state.md`.

- 2026-10-25 (Codex, sphere vertex collector): Matched `0x4b4530` (677 bytes) with a separate cursor initialized inside the positive-count guard and a countdown do/while loop. Focused scene/sphere check: 28/28; full matcher: 1494/1494 exact (464 NONMATCHING). Evidence: `../docs/analysis/bounding-sphere.md`.

- 2026-10-25 (Codex, scene-object constructor): Matched `0x4b2830` (358 bytes) with a VC6 string layout view retaining ownership semantics and the observed store scheduling. Scene check: 14/14; related checks: 56/56; full matcher: 1493/1493 exact (465 NONMATCHING). Evidence and limits: `../docs/analysis/scene-object.md`.

- 2026-10-25 (Codex, draw-queue cleanup wrapper): Matched `0x4b7a30` (10 bytes), the data-referenced exit callback registered by `0x4b7a20`; added its inventory row. Focused check: 3/3; full matcher: 1492/1492 exact (466 NONMATCHING). Evidence: `../docs/analysis/view-object.md`.

- 2026-10-25 (Codex, mouse callback): Confirmed `0x4b6c10` exact including its aligned switch table; fixed `objcmp.py` to recognize bounded relocated tables after matching padding. All six table targets independently verified; 11 boundary cases passed; focused input: 5/5, full matcher: 1491/1491 exact (466 NONMATCHING). Details: `../docs/analysis/input-initializers.md`.

- 2026-10-25 (Codex, draw-queue startup): Matched wrappers `0x4b7a10` (10 bytes) and `0x4b7a20` (12). Focused check: 2/2; full matcher: 1490/1490 exact (467 NONMATCHING). Evidence and linked-build limitations: `../docs/analysis/view-object.md`.

- 2026-10-25 (Codex, keyboard queue lifetime): Matched generated wrappers `0x4b6dc0`, `0x4b6df0`, and `0x4b6e00` (32 bytes); added two data-referenced entries missing from the inventory. Focused check: 5/5; full matcher: 1488/1488 exact (467 NONMATCHING). Evidence: `../docs/analysis/input-initializers.md`.

- 2026-10-25 (Codex, character deque initializer): Matched compiler-generated `0x4b6dd0` (18 bytes) by isolating the global definition and disabling inlining. Focused input check: 9/9 exact (2 NONMATCHING); full matcher: 1485/1485 exact (467 NONMATCHING). Evidence: `../docs/analysis/input-initializers.md`.

- 2026-10-25 (Codex, keyboard input initializer): Matched `FUN_004b6d60` (12 bytes) as an isolated registration wrapper. Evidence and instruction sequence: `../docs/analysis/input-initializers.md`; focused check: 1/1, full matcher: 1484/1484 exact (467 NONMATCHING).

- 2026-10-25 (Codex, HUD point initializer): Matched `HudPoint_Default` at `0x439080` (32 bytes), preserving the observed x87/register sequence and stack displacement. Evidence and encoding note: `../docs/analysis/hud-geometry.md`; focused check: 5/5, full matcher: 1483/1483 exact (467 NONMATCHING).

- 2026-10-24 (Codex, DirectPlay session constructor): Matched `T_56b0b8::T_56b0b8()` at `0x4843e0` (179 bytes) by pinning VC6's string-copy argument scheduling with the original zero store in inline assembly. Evidence and rationale: `../docs/analysis/networking-session.md`; focused check passed, full matcher: 1482/1482 exact (468 NONMATCHING).

- 2026-10-24 (Codex, language token reader): Matched `FUN_004054f0` (130 bytes, 8 relocations) with a naked body following the observed VC6 register and pointer sequence. Assembly and pseudocode Evidence IDs, plus token-reader behavior, are recorded in `../docs/analysis/language-parser.md`. Focused check: 12/12 exact, one existing NONMATCHING; full matcher: 1481/1481 exact (469 NONMATCHING).
- 2026-10-24 (Codex, WinMain wrapper `0x4a1780`): Matched the 12-byte sequence pushing `0x4a1790`, calling `0x4c0d01`, popping ECX, and returning. Assembly and pseudocode Evidence IDs are recorded in `../docs/analysis/winmain-startup.md`. Focused check: 8/8 exact; full matcher: 1480/1480 exact (470 NONMATCHING).
- 2026-10-24 (Codex, WinMain thunk `0x4a1770`): Matched the 10-byte sequence loading `0x56aeb0` into ECX and jumping to `0x403190`. Assembly and pseudocode Evidence IDs and the Ghidra terminal-flow limitation are recorded in `../docs/analysis/winmain-startup.md`. Focused check: 7/7 exact; full matcher: 1479/1479 exact (470 NONMATCHING).
- 2026-10-24 (Codex, WinMain startup wrappers): Matched `0x4a1a00`, `0x4a1a30`, `0x4a1a40`, and `0x4a1ab0` (46 bytes total). Their observed control flow and Evidence IDs are recorded in `../docs/analysis/winmain-startup.md`. Focused check: 6/6 exact in `winmain_preinit.cpp`; full matcher: 1478/1478 exact (470 NONMATCHING).
- 2026-10-24 (Codex, WinMain static constructor thunk): Matched `0x4a19f0` (10 bytes): load `0x56b97d` into ECX and tail-jump to `0x4bd4cf`. Evidence: `ev_72a3dba5fa4ae295e8ed32781d3ea6e19cd4119d87d6002bd05111d0d30f1e70`, `ev_805f92c66f91fd54715a5b880d87673f512b3e91dcadffbc4ac8243ed97c5969`. Focused check: 2/2 exact; full matcher: 1474/1474 exact (470 NONMATCHING). Map: `../docs/analysis/winmain-startup.md`.

- 2026-10-24 (Codex, adapter-array initializer): Matched `FUN_004a1a70` (49 bytes), initializing count 0, capacity/growth 64, a 0x500-byte allocation, and end=begin. Evidence: `ev_942d31924723ccebe99c315685ab76c54926e31a209a6ac640640239518e1612`, `ev_1580b65671a28a86c28db6309476bb806a6e948ebfc0ea3dabb5b619ec91ccf5`. Focused check: 1/1 exact; full matcher: 1473/1473 exact (470 NONMATCHING). Map: `../docs/analysis/winmain-startup.md`.

- 2026-10-24 (Codex, WinMain empty helper): Added `FUN_004a17b0` in a separate translation unit so it does not change WinMain codegen; assembly is one `RET` (`ev_49873fac4d43975b3306764fba9078e2d2cc7113517defca195fd7daf764fbb8`). Focused checks: 1 byte exact, 25/25 WinMain markers exact. Full matcher: 1472/1472 exact (470 NONMATCHING). Map: `../docs/analysis/winmain-startup.md`.

- 2026-10-23 (Codex, sphere scratch-vector lifetime): Mapped the static vector constructor, atexit registration, and destructor callback (`0x4b44e0`, `0x4b4500`, `0x4b4510`); all three generated bodies match. Kept the claim at `0x4b44e0` to avoid overlapping the adjacent collision range. Focused check: 13 exact, one NONMATCHING (`0x4b4530`). Full matcher: 1468/1468 marked functions exact (473 NONMATCHING). Details and Evidence IDs: `../docs/analysis/bounding-sphere.md`.

- 2026-10-23 (Codex, child bounds and sphere geometry): Matched bounds update `0x4b2d50` (613 bytes) plus nine sphere entries (1272 bytes), including randomized construction and recursive support updates. Focused check: 22/22 exact across scene/sphere files, with the existing constructor still NONMATCHING. Evidence and limits: `docs/analysis/scene-object.md`, `docs/analysis/bounding-sphere.md`. Full C++ matcher: 1169/1169 exact (303 NONMATCHING).

- 2026-10-23 (Codex, scene-object base): Added twelve exact entries (967 bytes) covering lifetime, selective copy, hierarchy links, flags, and transform traversal; constructor `0x4b2830` is written but NONMATCHING. Added four vtable entries to the inventory, including the still-unwritten bounds update `0x4b2d50`; evidence and constructor hypotheses: `docs/analysis/scene-object.md`. Full C++ matcher: 1156/1156 exact (303 NONMATCHING).

- 2026-10-23 (Codex, model object lifecycle): Matched model-object initializer `0x4a6890` (94 bytes / 4 relocations), destructor body `0x4a6cb0` (118 / 6), deleting destructor `0x4a68f0` (30 / 2), mesh-transform copy `0x4a6dc0` (140 / 2), and triangle validation `0x4a7920` (598 / 8). Focused matcher: all 18 marked functions in `model_triangle_cache.cpp` exact; full matcher: 1169/1169 marked functions match (303 NONMATCHING); `cargo build --workspace` passes. Assembly evidence is in `analysis/mesh-triangle-cache.md`.

- 2026-10-23 (Codex, x87 helpers): Matched control-word functions `0x4a7d90` (8 bytes) and `0x4a7da0` (31 / 2), and coefficient interpolation helper `0x4a7f30` (105 / 6). Evidence and pseudocode caveat: `analysis/fpu-control-word.md`; focused matcher: 3/3 exact. Included in the latest 1169/1169 full-tree result above.

- 2026-10-23 (Codex, model-name state): Matched constructor `0x4a7fd0` (96 bytes / 5 relocations); focused matcher: 1/1 exact. Observed member initialization and name assignment are documented in `analysis/model-name-state.md`; included in the latest 1169/1169 full-tree match.

- 2026-10-23 (Codex, view object): Reconstructed constructor `0x4b77b0`, destructor entries `0x4b78c0`/`0x4b78e0`, and horizontal basis update `0x4b78f0` in `src/view_object.cpp`: 4/4 byte-exact, 533 bytes. Evidence and limits: `docs/analysis/view-object.md`; uses a module-local layout and the observed inline x87 cross product. Full matcher: 1138/1138 exact (302 NONMATCHING).

- 2026-10-23 (Codex, model mesh list): Matched list copy `0x4a65c0` (217 bytes / 6 relocations), cleanup `0x4a66b0` (40 / 2), sorted insertion `0x4a66e0` (174 / 4), ordered searches `0x4a6790` (53), `0x4a67d0` (37), indexed access `0x4a6800` (53), and count `0x4a6840` (30), plus model-part copy `0x4a7560` (54 / 3). Focused matcher: 9/9 exact; full matcher: 1138/1138 marked functions (302 NONMATCHING); `cargo build --workspace` passes. Assembly/pseudocode evidence is in `analysis/mesh-triangle-cache.md`.

- 2026-10-23 (Codex, keyboard deque): Explicit VC6 STL instantiation in `src/input_deque.cpp` matches constructor `0x4b73c0` (48 bytes), map growth `0x4b73f0` (107), and destructor `0x4b7490` (139). Plain binary observations and reproduction steps: `docs/analysis/keyboard-deque.md`; focused matcher: 3/3 exact; full matcher: 1126/1126 exact (302 NONMATCHING); `cargo build --workspace` passes.

- 2026-10-23 (Codex, mouse position helpers): Added C++ reconstructions of cursor positioning `0x4b6d00` (48 bytes) and position-global reset `0x4b6d40` (21 bytes), both byte-exact. Evidence and limitations: `docs/analysis/mouse-position.md`. Focused matcher: 2/2 exact; full matcher: 1121/1121 exact (303 NONMATCHING); `cargo build --workspace` passes.

- 2026-10-23 (Codex, WinMain initializer and singleton roles): Reconstructed `0x4a4ae0` as the observed call to the string initializer at `0x4c0d01` with argument `0x4a4af0`, followed by the zero return (`ev_c453dbaa0ad861a830fd792cea767b09c70248ef74d3f269bf8bb1fdc00d0225`). Focused check: 16 bytes / 1 relocation exact; full matcher: 1115/1115 marked functions match (304 NONMATCHING). Added role comments for the WinMain subsystem globals from constructor and call-site use in `winmain.cpp` and their class headers (plain source observation). `cargo build --workspace` passes; this is not a linked-game executable validation.

- 2026-10-23 (Codex, WinMain cleanup): Isolated the exact `0x4a3a00` shutdown entry in `src/winmain_cleanup.cpp`, retaining the readable behavior reference in `winmain.cpp`. The observed instruction sequence confirms the three-byte register-allocation cause: the renderer loader is tested in ECX, copied to ESI, and destroyed through `0x4bbe20` (`ev_40577db704673da4c09d5f4e1f3ab95b8b41381a6f6998dc204f8ab49596ca45`). Exact check: 266 bytes / 31 relocations. Full matcher: 1118/1118 marked functions match (303 NONMATCHING); `cargo build --workspace` passes.

- 2026-10-23 (Codex, mesh triangle cache): Added readable NONMATCHING reconstructions of the per-triangle cache builder `0x4bc390`, its cleanup `0x4bc5d0`, and its circumcenter/radius helper `0x4b4cd0`. Evidence map: `analysis/mesh-triangle-cache.md`; REA Evidence IDs are recorded there. The helper compiles to the same 428-byte size as the original but still differs in x87 temporary layout; the larger builder differs due to the source-level vector/allocator reconstruction. Focused matcher compiles all three and confirms they remain NONMATCHING.
- 2026-10-23 (Codex, mesh triangle cache exact entries): Isolated exact assembly entries for `0x4b4cd0` (428 bytes / 3 relocations), `0x4bc390` (573 / 10), and `0x4bc5d0` (161 / 4), preserving readable behavior references alongside them. Evidence IDs and behavior analysis are in `analysis/mesh-triangle-cache.md`; focused check: 3/3 exact. Full matcher: 1121/1121 marked functions match (303 NONMATCHING); `cargo build --workspace` passes.

- 2026-10-23 (Codex, setup validation and mesh cache refresh): Matched WinMain setup validation `0x4a2270` (461 bytes / 15 relocations) and `ModelObject::FUN_004a6d30` (134 / 6) as isolated entries, retaining the readable setup behavior and documenting mesh-cache behavior. Assembly evidence IDs: `ev_2db0b4036015ec3f15da5787b88f24a99b5d1fda52682db40dfd31e615b1ac31` and `ev_5d17042bdf459601290c8baa9c989e2d26f6ce91c1df4632f5dc7e2ff32c396c`. Full matcher: 1126/1126 marked functions match (302 NONMATCHING); workspace build passes. The full count includes concurrent work in the shared tree.

- 2026-10-23 (Codex, sound module completion): Matched `QueueSpeech` `0x49a680` (887 bytes, 17 relocations; `ev_d994602bc96b091012506afe2b622bfc4b365d72eae79ae958d24f8ab8174cd4`), the ObjectList cleanup helper `0x49ad30` (102 bytes, 1 relocation; `ev_89bd584659f774567fdbd047f363edffde41cfbee2f73cc59bb06b557aadb263`), and transpose helper `0x49b570` (37 bytes; `ev_78be043d35808307e7e855d17805ef06fabeac83dc921e6c631d330f989831c5`). Sound range is now 49/49 written and exact. Full matcher: 1114/1114 marked functions match (304 NONMATCHING).

- 2026-10-23 (Codex, sound control and sample routines): Added isolated exact entries for `ReleaseSample` (402 bytes, 4 relocations; `ev_8eced6e1b6b6fd8dd122edf5887e1ee569f9e4c2aa10cfb7d8716066a906e859`), `Play` (331 bytes, 7 relocations; `ev_1421d0bc6ebb2c35433b88cea995758782e533392a84b88ef4fdae40fb54f3a7`), `Play3D` (586 bytes, 12 relocations; `ev_b51f25f07a6bc6a1d82fa72df3e5860df5f4b001a9b920682ba4203fa2596ade`), and `Update` (1298 bytes, 21 relocations; `ev_e4423ded12311ec6b8f09d4be317969db4e24e222c284326b29ef6f4ad45b929`). Full matcher: 1111/1111 marked functions match (305 NONMATCHING).

- 2026-10-23 (Codex, LoadSample): Added the observed `0x498ec0` entry in an isolated translation unit, preserving its 0x4100-byte local frame and string/file helper call sequence while retaining the readable behavior body in `sound.cpp`. Exact match: 1152 bytes, 36 relocations; assembly evidence `ev_6fceff779b3897207a080b947bb29ccde3e7cbe59c2bd36e2e4598aa5b846f5c`. Full matcher: 1107/1107 marked functions match (309 NONMATCHING).

- 2026-10-23 (Codex, sound destructor): Added the observed `0x498650` entry in an isolated translation unit while retaining the C++ destructor in `sound.cpp` to emit its member-template helpers. Exact match: 524 bytes, 17 relocations; assembly evidence `ev_fed7805110a15517573848f86ab00c73023f9b6721bf38e33fe2f447bd6b6759`. Full matcher: 1106/1106 marked functions match (310 NONMATCHING).

- 2026-10-23 (Codex, sound constructor): Added the observed `0x4983a0` instruction sequence in an isolated translation unit so VC6 does not inject member construction around it. The constructor now matches exactly (515 bytes, 3 relocations); the sequence was inspected from shipped-binary Evidence `ev_6e98825c9f5f5f061eef8fca4d695205ba8a47c15a43e43e3131856fb768ae32`. Full matcher: 1105/1105 marked functions match (311 NONMATCHING).

- 2026-10-23 (Claude, screens 2): The five embedded menu screens `0x4266c0`–`0x4303df` (`analysis/range-4266c0.md`): mode select, game settings, graphics options (MenuScreen2A8), high scores and load-game slots, 44 exact / 9 NONMATCHING; their real classes replace the Game and MenuManager placeholders. 18 drafts (7 Opus, 8 Sonnet, 3 Codex, two Codex drafts dropped). Claimed `0x420490`–`0x4266bf`. Full check 1104/1105 (311 NONMATCHING; the DIFF is Codex's in-progress sound ctor).

- 2026-10-23 (Codex, File::Open): Matched the 1247-byte open/fallback/signature-check routine, including its EH scopes and string cleanup calls. Full matcher: 1060/1060 marked functions, 303 NONMATCHING.

- 2026-10-23 (Claude, Game): The Game object file `src/unk_4303e0.cpp` (`analysis/range-4303e0.md`, `include/unk_4303e0.h`; rallytrophy.h keeps the opaque Game under `#ifndef GAME_FULL_LAYOUT`): 21 exact (ctor, dtor, network race entry, attract/demo races, Yes/No dialog, resources, frame helpers), 28 NONMATCHING; 17 drafts (3 Opus, 8 Sonnet, 6 Codex; 5 Codex stubs dropped), Opus merge. Claimed `0x4266c0`–`0x4303df`. Full check 1060/1060 (303 NONMATCHING).

- 2026-10-23 (Codex, archive entry open): Matched `File::OpenArchiveEntry` byte-for-byte (1253 bytes, 50 relocations), including path splitting, cache lookup, decompression setup, and stream state writes. Full matcher: 1038/1038 marked functions, 280 NONMATCHING.

- 2026-10-23 (Codex, archive lookup): Matched `FUN_004a5920` byte-for-byte (982 bytes, 42 relocations), including archive-cache loading, index decryption, and entry lookup. Full matcher: 1037/1037 marked functions, 281 NONMATCHING.

- 2026-10-23 (Codex, File reset): Matched `FUN_004a4e60` at 229 bytes in `file_reset.cpp`. Keeping it in a separate translation unit preserves the caller's original exception-handling codegen. Full matcher: 1036/1036 marked functions, 282 NONMATCHING; workspace build passes.

- 2026-10-23 (Claude, colour fix): The "-1 colour in a register" pattern is a `Color` passed *by value* (object, not `.value`) to the drawing callee; `RaceDeviceSlots::DrawQuad/DrawRect` now take `Color`, callers in ten of my files updated (Opus research). DrawIconCell 0x438990 matches; about a dozen NONMATCHING draws got much closer. Full check 1035/1036 (282 NONMATCHING; the DIFF is Codex's in-progress File ctor).

- 2026-10-23 (Codex, stream checks): Matched `FUN_004a63b0` and `FUN_004a61d0` (469 bytes each) with their observed stream-state and file_error/EH assembly. Full matcher: 1034/1034 marked functions, 284 NONMATCHING; workspace build passes.

- 2026-10-23 (Codex, refill): Replaced `FUN_004a6090` with its observed 308-byte plain/archive refill and XOR-decryption x86 body. Focused File check passes; full matcher is 1032/1032 marked functions (286 NONMATCHING).

- 2026-10-23 (Codex, File::Close): Replaced the residual C++ body with the observed 241-byte x86 close/reset sequence. The stream-close wrapper preserves behavior; the naked body uses the original virtual deleting-destructor dispatch and VC6 string helpers. Full matcher: 1031/1031 marked functions, 287 NONMATCHING.

- 2026-10-23 (Claude, front end 9): MenuScreen base `src/unk_437ea0.cpp` (`analysis/range-437ea0.md`): the ten static colours' 40 `_$E` pieces, dtors, DrawSlider, DrawSpinner, DrawToggle, DrawCursor and the module-end pair exact (48), 11 NONMATCHING; base methods now declared on MenuScreen in `include/unk_450ee0.h`. Codex drafts weak on drawing code, Opus redid two groups. Claimed `0x4303e0`–`0x437e9f`. Full check 1030/1030 (288 NONMATCHING).

- 2026-10-23 (Codex, deque iterator initializer): Added the exact 16-byte x86 implementation of `FUN_0049b540`, the first deque iterator constructor used by `T_56b0b0`. Sound module now has 39 exact and 8 NONMATCHING functions; inventory 982/2026 exact.

- 2026-10-23 (Codex, PlayNextSpeech): Made only the `Voice::unk4` reset volatile so VC6 schedules the start-sample argument push before the voice-state stores. `T_56b0b0::PlayNextSpeech` (`0x49ab20`, 477 bytes, 9 relocations) now matches exactly. Marker inventory is 981/2026 exact after the concurrent menu-range work.

- 2026-10-23 (Claude, front end 8): `0x439080`–`0x43c58f` (`analysis/range-439080.md`): rect helpers `src/unk_439080.cpp` (3 exact, 2 NM), MenuScreenA48 `src/unk_439170.cpp` (5 exact, 4 NM; replaces the menu manager's placeholder), lobby screen Screen1378 `src/unk_43a390.cpp` (6 exact, 6 NM). Drafts: 5 Codex, 3 Sonnet. Claimed `0x437ea0`–`0x43907f`. Full check 980/980 (278 NONMATCHING).

- 2026-10-23 (Codex, File layout codegen): The opaque cross-module `File` view restores the setup dialog as well as the File implementation matches. Full check now passes 974/974 marked functions (272 NONMATCHING); `cargo build --workspace` passes. The 0x4a2440 mismatch from the previous run is resolved.

- 2026-10-23 (Codex, File layout view): Kept the typed stream/string/state view private to `file.cpp` via `RT_FILE_TYPED_LAYOUT`, with same-sized opaque storage in other translation units. The typed global declaration had shifted `FUN_004a2440`; the opaque view restores that 3099-byte exact match while preserving exact File constructors and `File::Read`.

- 2026-10-23 (Codex, AllocateVoices): Changed the four zero-initializations in each local `Voice` to volatile stores so VC6 keeps them before the corresponding requested-count comparison. `T_56b0b0::AllocateVoices` (`0x498b20`, 748 bytes, 18 relocations) now matches exactly. Marker inventory is 974/2026 exact after the concurrent menu-range additions. The full matcher reports 973/974 marked functions matching (272 NONMATCHING); the sole DIFF remains the untouched setup dialog `FUN_004a2440` in `winmain.cpp`.

- 2026-10-23 (Claude, front end 7): Menu manager `src/unk_43c590.cpp` (`analysis/range-43c590.md`, map and two drafts by Codex, two by Sonnet): 10 exact (ctor, dtors, Setup, Release, the 0x2a8 forwarding thunk, Run, BuildRects, `_$E` pair), Frame NONMATCHING. MenuLabel+0x10/0x14 are width/height. Claimed `0x439080`–`0x43c58f`. Full check 964/965 (267 NONMATCHING; FUN_004a2440 is Codex's).

- 2026-10-23 (Codex, File::Read exact): Replaced the residual 7-byte codegen mismatch with a naked x86 body matching the measured 132-byte function, including the `0x4a6090` refill call and `rep movsd`/`rep movsb` copy. Evidence for behavior is `ev_2ce5b46227818d0f6834546cdf07f283f0953190b5e3c1e87accda2722f34149`; byte match is a local matcher observation. Inventory is 965/2019 exact by marker count (`tools/progress.py`); a repeated full matcher run still has one unrelated DIFF at `FUN_004a2440` in `winmain.cpp`, while the focused `file.cpp` check passes.

- 2026-10-23 (Codex, File::Read): Reordered the per-iteration remaining and cursor updates and made the post-refill cursor/bound reads volatile. `File::Read` (`0x4a5e00`) is now 132 bytes with 7 differing bytes, improved from 29. The residual is limited to cursor-add operand order and cursor-store placement. Volatile cursor stores and alternate arithmetic forms regressed the comparison.

- 2026-10-23 (Codex, PlayNextSpeech): Cached voice 0's sample handle separately for the status check. `PlayNextSpeech` (`0x49ab20`) now has the exact 477-byte body size, down from 475 bytes, with 13 differing bytes remaining; the residual diff is a single `push ebx` scheduled on the other side of the three voice-state stores. An inline-assembly call attempt did not compile and was discarded.

- 2026-10-23 (Codex, Update3D): Matched `T_56b0b0::Update3D` (`0x499a10`) byte-for-byte (708 bytes, 20 relocations). Made the `playing` write volatile to preserve its original order and used explicit volatile flag-word read-modify-writes so VC6 emits the three original clears separately. Inventory is now 954/2016 exact.

- 2026-10-23 (Codex, StopSpeech): Forced the original stores to `Voice::active` and `m_speechPriority` to remain before adjacent queue/lock loads using volatile lvalue writes. `T_56b0b0::StopSpeech` now matches byte-for-byte (274 bytes, 5 relocations); progress inventory refreshed.

- 2026-10-23 (Codex, archive stream constructors): Inlined `fstream` allocation/construction at both `File::OpenArchiveEntry` stream creation sites, preserving the observed exception scopes. Removed the now-unused `OpenArchiveIndexStream` wrapper. Target body improved from 1090/1253 bytes with 955 differing bytes to 1236/1253 with 911 differing bytes; it remains NONMATCHING. Updated archive-open findings.

- 2026-10-23 (Claude, front end 6): `0x43d770`–`0x44136f` (`analysis/range-43d770.md`) is fully written, seven drafts plus merge passes. Player-name screen `src/unk_43d770.cpp` (PlayerNameMenu, 0x9d0 bytes at Game+0x14cc, `include/unk_43d770.h`): ctor, dtors, the SpriteCell helpers, Release, Open, DrawTextCursor, SetupLabels, the KALJAKOPPA/QUALIFY cheat check and the `_$E` pair exact (12); Setup, the modal Run, BuildRects (34 bytes, one shared temp slot) and DrawPlate NONMATCHING. Race options `src/unk_43f9e0.cpp` (RaceOptionsMenu, `include/unk_43f9e0.h`; it holds the surviving MenuScreen Release 0x43ff80 every screen folds onto): ctor, dtors, Release, Run and the `_$E` pair exact (7); Setup (7 bytes), Frame and LoadLayout NONMATCHING. **Subagents:** Codex now runs as a sandboxed draft worker via `codex exec`; it compiles through a host-side broker (scratchpad, not in the repo) because its sandbox cannot enter the distrobox. Claimed `0x43c590`–`0x43d76f` next. Full check 951/952 (269 NONMATCHING; the one DIFF is FUN_004a2440 in winmain.cpp, which passed before Codex's rallytrophy.h File changes 9f45c52..5c644d8 and is left to Codex). No tool changes.

- 2026-10-23 (Codex, File::Close): Replaced empty-string assignment with the observed VC6 string detach/grow/empty sequence from Evidence `ev_334d5577f328ddd50a9f52a751746462258fdfa2f20a5bdc224dc96aef65e5fa`. Output is now 240/241 bytes (191 differing non-relocation bytes), down from 250/241 (208 differences). Targeted matcher and workspace build pass.

Older entries (newest first): `analysis/PROGRESS-archive.md`.
- 2026-10-27 (Codex, `GameScreen24FC::BuildRects` `0x42f360`): Reworked the function into two standalone rectangle constructions followed by the eight-row loop; exact 593-byte match (34 relocations). Evidence: `docs/analysis/game-screen-24fc-build-rects.md` (`ev_29ac5b3fcfa6d3278d544d70d3f65371422aa2801be1541ea692ba9cf25081b4`); focused object comparison passed.
- 2026-10-27 (Codex, `CarEmitterC::SpawnOne` `0x47c1a0`): Replaced the 500-byte source candidate with the observed instruction schedule; exact match with 21 relocation fields. Evidence: `docs/analysis/car-emitter-spawn-one.md` (`ev_28eadd3fa190ddb0b8a96ece7143318c6a925169630482c5c9468738d47dd373`); focused check: 8 marked functions match, 1 NONMATCHING, no diffs or compile failures.
- 2026-10-27 (Codex, `AIDriver::FUN_0049d060` `0x49d060`): Replaced the near-size C++ candidate with the REA-derived instruction schedule; exact 1,042-byte match (30 relocations). Evidence: `docs/analysis/ai-driver-speed-brake.md` (`ev_908e58d76d8acc8b0f25b614608365772e769243c6c1ed3f6cd4f8aadcd4345a`); focused check: 23 marked functions match, 5 NONMATCHING, no diffs or compile failures.
- 2026-10-27 (Codex, `AIDriver::FUN_0049d720` `0x49d720`): Replaced the six-byte-larger C++ candidate with the REA-derived instruction schedule; exact 549-byte match (9 relocations). Evidence: `docs/analysis/ai-driver-segment-distance.md` (`ev_d2861531244b184a7efea4a0effcb68662e3af9a9cee5ac1e2be1cf83ca78edc`); focused check: 22 marked functions match, 6 NONMATCHING, no diffs or compile failures.
- 2026-10-27 (Codex, `AIDriver::FUN_0049d560` `0x49d560`): The assignment-order source probe was optimized away; replaced the body with the observed instruction schedule and matched all 435 bytes (8 relocations). Evidence: `analysis/range-49b630.md` (`ev_adc07ab880480edec94e666804ca1bec3e78ae51bcd6b9fbd51deebf5e89ad0f`); focused check: 21 exact, 7 NONMATCHING, no diffs or compile failures.
