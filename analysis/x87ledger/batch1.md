# x87 behaviour ledger, batch 1

Generated from `python3 tools/x87check.py --sym --list SRC:ADDR` after `tools/match.py` on each file
(0x45c700 re-listed after its fix). One line per event: `orig_addr | built_off | verdict | reason`.

## src/unk_4563e0.cpp 0x456b10

`0x456b10 src/unk_4563e0.cpp: events 71/64, 50 differ`

- 0x456d68 | - | NOISE | fov clamp copy: orig `fld [ebp+0x8]; fstp [ecx+0x12c]`, built `mov eax,[ebp+0x8]; mov [ecx+0x12c],eax` (int vs float copy of the same float)
- 0x456f53 | 0x440 | NOISE | only the frame offset of the same local differs (orig 0x58/0x40 vs built 0x44/0x38 from the &L0 base); same fld/fmul/fstp shape
- 0x456f5e | 0x44b | NOISE | only the frame offset of the same local differs (orig 0x58/0x40 vs built 0x44/0x38 from the &L0 base); same fld/fmul/fstp shape
- 0x456f69 | 0x456 | NOISE | only the frame offset of the same local differs (orig 0x58/0x40 vs built 0x44/0x38 from the &L0 base); same fld/fmul/fstp shape
- 0x456f77 | 0x464 | NOISE | only the frame offset of the same local differs (orig 0x58/0x40 vs built 0x44/0x38 from the &L0 base); same fld/fmul/fstp shape
- 0x456f92 | 0x47b | NOISE | only the frame offset of the same local differs (orig 0x58/0x40 vs built 0x44/0x38 from the &L0 base); same fld/fmul/fstp shape
- 0x456f9d | 0x486 | NOISE | only the frame offset of the same local differs (orig 0x58/0x40 vs built 0x44/0x38 from the &L0 base); same fld/fmul/fstp shape
- 0x456fa8 | 0x491 | NOISE | only the frame offset of the same local differs (orig 0x58/0x40 vs built 0x44/0x38 from the &L0 base); same fld/fmul/fstp shape
- 0x456fb6 | 0x49f | NOISE | only the frame offset of the same local differs (orig 0x58/0x40 vs built 0x44/0x38 from the &L0 base); same fld/fmul/fstp shape
- 0x457441 | 0x91c | NOISE | G3_TRANSFORM inline asm, byte-identical (orig 0x457441 = built 0x91c `fstp [eax]; fstp [eax+8]; fstp [eax+4]`); the walker names the trans pointer R0/phi in orig and G in built
- 0x457443 | 0x91e | NOISE | G3_TRANSFORM inline asm, byte-identical (orig 0x457441 = built 0x91c `fstp [eax]; fstp [eax+8]; fstp [eax+4]`); the walker names the trans pointer R0/phi in orig and G in built
- 0x457446 | 0x921 | NOISE | G3_TRANSFORM inline asm, byte-identical (orig 0x457441 = built 0x91c `fstp [eax]; fstp [eax+8]; fstp [eax+4]`); the walker names the trans pointer R0/phi in orig and G in built
- 0x4574fb | 0x9d5 | NOISE | G3_TRANSFORM inline asm, byte-identical (orig 0x457441 = built 0x91c `fstp [eax]; fstp [eax+8]; fstp [eax+4]`); the walker names the trans pointer R0/phi in orig and G in built
- 0x4574fd | 0x9d7 | NOISE | G3_TRANSFORM inline asm, byte-identical (orig 0x457441 = built 0x91c `fstp [eax]; fstp [eax+8]; fstp [eax+4]`); the walker names the trans pointer R0/phi in orig and G in built
- 0x457500 | 0x9da | NOISE | G3_TRANSFORM inline asm, byte-identical (orig 0x457441 = built 0x91c `fstp [eax]; fstp [eax+8]; fstp [eax+4]`); the walker names the trans pointer R0/phi in orig and G in built
- 0x457512 | 0xa13 | NOISE | orig copies target 0x1f0..0x1f8 into the forward row as floats (`fld [ecx+0x1f0]; fst [ecx+0x40]`); built copies it as ints, so its next float store (`fst [esi]` at 0xa13) is already dx=row-camPos. Same values; the pairing is off by one event
- 0x457527 | 0xa24 | NOISE | orig copies target 0x1f0..0x1f8 into the forward row as floats (`fld [ecx+0x1f0]; fst [ecx+0x40]`); built copies it as ints, so its next float store (`fst [esi]` at 0xa13) is already dx=row-camPos. Same values; the pairing is off by one event
- 0x45753f | 0xa39 | NOISE | orig copies target 0x1f0..0x1f8 into the forward row as floats (`fld [ecx+0x1f0]; fst [ecx+0x40]`); built copies it as ints, so its next float store (`fst [esi]` at 0xa13) is already dx=row-camPos. Same values; the pairing is off by one event
- 0x45754a | 0xa57 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x45755b | 0xa5f | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x45756d | 0xa68 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x45758e | 0xa7f | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x457596 | 0xace | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x45759f | 0xad0 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x4575b6 | 0xad3 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x4575fb | 0xb00 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x4575fd | 0xb07 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x457600 | 0xb0d | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x45762d | 0xb15 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x457634 | 0xb61 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x45763a | 0xb63 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x457642 | 0xb66 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x457688 | 0xbb6 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x45768a | 0xbb8 | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x45768d | 0xbbb | NOISE | normalise/dot block is the same op for op (orig 0x457548.. vs built 0xa18..: fsub, fstp [ebp+8], fstp/fld dz, dz*dz+dy*dy+dx*dx, fsqrt, fdivr 1.0, fmulp, fst row, then the dot with [ecx+8]/[ecx+4]/[ecx]). The trees differ only because orig forwards the target fields and built reads the int-copied row (phi(slot)); the pairing is shifted inside this block
- 0x4576d4 | 0xbfd | NOISE | cross product block (`fld [ecx+4]; fmul [edx+8]` .. `fsubp`) is identical. The walker pairs it with other built stores: orig keeps the view-8 path after the ret and built puts it before, so the true counterparts are the built cross products in the same block
- 0x4576d6 | 0xbff | NOISE | cross product block (`fld [ecx+4]; fmul [edx+8]` .. `fsubp`) is identical. The walker pairs it with other built stores: orig keeps the view-8 path after the ret and built puts it before, so the true counterparts are the built cross products in the same block
- 0x4576d9 | 0xc02 | NOISE | cross product block (`fld [ecx+4]; fmul [edx+8]` .. `fsubp`) is identical. The walker pairs it with other built stores: orig keeps the view-8 path after the ret and built puts it before, so the true counterparts are the built cross products in the same block
- 0x45770f | 0xcab | NOISE | cross product block (`fld [ecx+4]; fmul [edx+8]` .. `fsubp`) is identical. The walker pairs it with other built stores: orig keeps the view-8 path after the ret and built puts it before, so the true counterparts are the built cross products in the same block
- 0x457711 | 0xcb8 | NOISE | cross product block (`fld [ecx+4]; fmul [edx+8]` .. `fsubp`) is identical. The walker pairs it with other built stores: orig keeps the view-8 path after the ret and built puts it before, so the true counterparts are the built cross products in the same block
- 0x457714 | 0xcc1 | NOISE | cross product block (`fld [ecx+4]; fmul [edx+8]` .. `fsubp`) is identical. The walker pairs it with other built stores: orig keeps the view-8 path after the ret and built puts it before, so the true counterparts are the built cross products in the same block
- 0x45774a | - | NOISE | view 8: orig copies target into row +0x40 with fld/fst, built with int moves (same bits)
- 0x457765 | - | NOISE | view 8: orig copies target into row +0x40 with fld/fst, built with int moves (same bits)
- 0x45777a | - | NOISE | view 8: orig copies target into row +0x40 with fld/fst, built with int moves (same bits)
- 0x4577c7 | - | NOISE | view-8 normalise: the true counterparts are built 0xcab/0xcb8/0xcc1 (`fmul st,st(1); fstp [esi]`, `fld [ebp-0x18]; fmul [ebp+8]`, `fmul [ebp-0x1c]`). They have the same rounding (dx extended, dy/dz float) and the same 1/sqrt(dz*dz+dy*dy+dx*dx) order as orig 0x4577a6..0x4577dd; the walker paired them with the cross product
- 0x4577d4 | - | NOISE | view-8 normalise: the true counterparts are built 0xcab/0xcb8/0xcc1 (`fmul st,st(1); fstp [esi]`, `fld [ebp-0x18]; fmul [ebp+8]`, `fmul [ebp-0x1c]`). They have the same rounding (dx extended, dy/dz float) and the same 1/sqrt(dz*dz+dy*dy+dx*dx) order as orig 0x4577a6..0x4577dd; the walker paired them with the cross product
- 0x4577dd | - | NOISE | view-8 normalise: the true counterparts are built 0xcab/0xcb8/0xcc1 (`fmul st,st(1); fstp [esi]`, `fld [ebp-0x18]; fmul [ebp+8]`, `fmul [ebp-0x1c]`). They have the same rounding (dx extended, dy/dz float) and the same 1/sqrt(dz*dz+dy*dy+dx*dx) order as orig 0x4577a6..0x4577dd; the walker paired them with the cross product
- 0x45788a | 0xd7d | NOISE | same expression; the row base pointer is named this+0x30 in orig and phi(slot)+0x10 in built
- 0x45788c | 0xd7f | NOISE | same expression; the row base pointer is named this+0x30 in orig and phi(slot)+0x10 in built
- 0x45788f | 0xd82 | NOISE | same expression; the row base pointer is named this+0x30 in orig and phi(slot)+0x10 in built

## src/unk_4563e0.cpp 0x4578c0

`0x4578c0 src/unk_4563e0.cpp: events 13/13, 10 differ`

- 0x4579ff | 0x13f | NOISE | table at 0x508ca8: orig shows the absolute 0x508cXX, built the relocation-relative offset (0x508cbc-0x508ca8=0x14 etc.); the trees are equal after rebasing
- 0x457a01 | 0x141 | NOISE | table at 0x508ca8: orig shows the absolute 0x508cXX, built the relocation-relative offset (0x508cbc-0x508ca8=0x14 etc.); the trees are equal after rebasing
- 0x457a04 | 0x144 | NOISE | table at 0x508ca8: orig shows the absolute 0x508cXX, built the relocation-relative offset (0x508cbc-0x508ca8=0x14 etc.); the trees are equal after rebasing
- 0x457a26 | 0x165 | NOISE | table at 0x508ca8: orig shows the absolute 0x508cXX, built the relocation-relative offset (0x508cbc-0x508ca8=0x14 etc.); the trees are equal after rebasing
- 0x457a9f | 0x1da | NOISE | after rebasing 0x508ca8, the two products of the inner sum are only listed in swapped order. Instructions identical (orig 0x457a89 `fld; fmul [eax+0x508ca8]; fld st1; fmul [eax+0x508cb0]; faddp; fadd [ecx+0x14]` = built 0x1cb..0x1d7)
- 0x457aaa | 0x1e2 | NOISE | table at 0x508ca8: orig shows the absolute 0x508cXX, built the relocation-relative offset (0x508cbc-0x508ca8=0x14 etc.); the trees are equal after rebasing
- 0x457ac4 | 0x1f5 | NOISE | after rebasing 0x508ca8, the two products of the inner sum are only listed in swapped order. Instructions identical (orig 0x457a89 `fld; fmul [eax+0x508ca8]; fld st1; fmul [eax+0x508cb0]; faddp; fadd [ecx+0x14]` = built 0x1cb..0x1d7)
- 0x457ade | 0x20b | NOISE | after rebasing 0x508ca8, the two products of the inner sum are only listed in swapped order. Instructions identical (orig 0x457a89 `fld; fmul [eax+0x508ca8]; fld st1; fmul [eax+0x508cb0]; faddp; fadd [ecx+0x14]` = built 0x1cb..0x1d7)
- 0x457ae9 | 0x213 | NOISE | table at 0x508ca8: orig shows the absolute 0x508cXX, built the relocation-relative offset (0x508cbc-0x508ca8=0x14 etc.); the trees are equal after rebasing
- 0x457b03 | 0x227 | NOISE | after rebasing 0x508ca8, the two products of the inner sum are only listed in swapped order. Instructions identical (orig 0x457a89 `fld; fmul [eax+0x508ca8]; fld st1; fmul [eax+0x508cb0]; faddp; fadd [ecx+0x14]` = built 0x1cb..0x1d7)

## src/unk_4856e0.cpp 0x489300

`0x489300 src/unk_4856e0.cpp: events 10/9, 1 differ, unreliable`

- 0x4898d4 | - | NOISE | orig pushes the spilled x arg in its own tail (`fstp [esp+0x2c]` / `mov edx,[esp+0x2c]` / `push edx`). Built cross-jumps that Print tail (`jmp 0x590`, same `push edx` at 0x5a7), so the walker sees it on another path: branch-layout artefact

## src/unk_4856e0.cpp 0x48a070

`0x48a070 src/unk_4856e0.cpp: events 15/18, 5 differ, unreliable`

- - | 0x401 | NOISE | orig has the same `push esi` of xRank (sx*184 loaded into esi; it shows as an unchanged context line in insdiff). Orig loads esi from [esp+0x2c], which the walker does not resolve; built loads it from a slot it does. Unpaired by naming only
- - | 0x43a | NOISE | orig has the same `push esi` of xRank (sx*184 loaded into esi; it shows as an unchanged context line in insdiff). Orig loads esi from [esp+0x2c], which the walker does not resolve; built loads it from a slot it does. Unpaired by naming only
- - | 0x46e | NOISE | orig has the same `push esi` of xRank (sx*184 loaded into esi; it shows as an unchanged context line in insdiff). Orig loads esi from [esp+0x2c], which the walker does not resolve; built loads it from a slot it does. Unpaired by naming only
- 0x48a680 | 0x614 | NOISE | orig `fsubr [esp+0x64]` vs built `fsubr [esp+0x70]`: the same x local at a different frame offset; GetWidth result named R1/R0
- 0x48a6ef | 0x682 | NOISE | same as 0x48a680 (second centred Print)

## src/unk_4856e0.cpp 0x48a920

`0x48a920 src/unk_4856e0.cpp: events 15/15, 2 differ, unreliable`

- 0x48ae45 | 0x4f8 | NOISE | x - GetWidth: same fsubr; the minuend local is at another frame offset (named L0 vs arg+0x8)
- 0x48afc6 | 0x673 | NOISE | orig 0x48afb7 `fld [esp+0x20]; fmul 710` = built 0x664 `fld [esp+0x20]; fmul 710` (sy*710); the built walker forwarded an earlier slot store, orig did not

## src/unk_4856e0.cpp 0x48b080

`0x48b080 src/unk_4856e0.cpp: events 24/20, 18 differ, unreliable`

- 0x48b224 | 0x1a7 | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b24e | 0x1d2 | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b25b | 0x1df | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b297 | 0x216 | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b2cf | 0x24f | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b309 | 0x35a | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b30a | - | NOISE | unpaired by drift: the matching built push is on another line of this block (same Print arg sequence)
- 0x48b3e8 | - | NOISE | unpaired by drift: the matching built push is on another line of this block (same Print arg sequence)
- 0x48b435 | 0x3a9 | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b45c | 0x3ce | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b45d | 0x3ef | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b47d | 0x419 | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b4a7 | 0x424 | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b4b2 | 0x44b | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b4d9 | - | NOISE | unpaired by drift: the matching built push is on another line of this block (same Print arg sequence)
- 0x48b57f | 0x4ed | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b58c | 0x515 | NOISE | all x87 ops and constants line up: 0.85, 0.85, 76/184, 510, 794, 116, 234, 6, 28, 194; room=(xCar-xName)-10; scale=room/GetWidth clamped to 0.85; y+sy*6 and y+=sy*28 hoisted in both. Orig keeps sx/sy/xRank in several int-copied slots and spills this to [esp+0x28]; the walker mislabels those slots as `this`/L0, so the pairing drifts
- 0x48b5aa | - | NOISE | unpaired by drift: the matching built push is on another line of this block (same Print arg sequence)

## src/unk_4856e0.cpp 0x48bd10

`0x48bd10 src/unk_4856e0.cpp: events 30/31, 1 differ, unreliable`

- - | 0x901 | NOISE | `cmp ebx,eax`: eax is an int. The walker carried a float slot value from the previous block, which ends in jmp: linear-walk artefact. Orig has the same cmp

## src/unk_4856e0.cpp 0x48cca0

`0x48cca0 src/unk_4856e0.cpp: events 12/13, 3 differ, unreliable`

- 0x48cd8a | - | NOISE | push of the by-value ColorArg temp: the slot is garbage and is overwritten by `mov [eax],0xffffffff` (orig 0x48cd97 / built 0xf8). Orig's slot happens to hold the dead 90*sy, built's another dead local
- - | 0x15c | NOISE | push y in the variant Print: same sequence as orig 0x48cdfb `push ecx`. y comes from [esp+0x1c] in orig vs [esp+0x20] in built (the y/y2 slots are swapped, as the source comment notes); the walker resolved the built slot to a stale value
- - | 0x1cf | NOISE | same as 0x15c for the 0x167 Print (orig 0x48ce60 push edx)

## src/unk_41ca70.cpp 0x41ca70

`0x41ca70 src/unk_41ca70.cpp: events 23/23, 17 differ, unreliable`

- 0x41cbf9 | 0x17a | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41cc11 | 0x18f | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41cc29 | 0x1b6 | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41cc3e | 0x1c6 | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41cc56 | 0x1df | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41cc6e | 0x1f8 | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41cc86 | 0x211 | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41cc9e | 0x22a | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41ccb6 | 0x243 | NOISE | same fields: orig reaches this and [this+0x234] through registers the walker names fs:0x0/esi0 after the early-return block (esi=[ecx+0x234] at 0x41ca90); built names them this/M[this+0x234]
- 0x41cd0d | 0x292 | NOISE | face index from the 16-bit word [ecx+0x12]: orig zero-extends with `xor edx,edx; mov dx,..` (walker: 0), built with `mov dx,..; and edx,0xffff` on a stale edx that the walker took from a float. Int idiom only; same fcomp
- 0x41cd1b | 0x2af | NOISE | face index from the 16-bit word [ecx+0x12]: orig zero-extends with `xor edx,edx; mov dx,..` (walker: 0), built with `mov dx,..; and edx,0xffff` on a stale edx that the walker took from a float. Int idiom only; same fcomp
- 0x41cd44 | 0x2cc | NOISE | face index from the 16-bit word [ecx+0x12]: orig zero-extends with `xor edx,edx; mov dx,..` (walker: 0), built with `mov dx,..; and edx,0xffff` on a stale edx that the walker took from a float. Int idiom only; same fcomp
- 0x41cd52 | 0x2e6 | NOISE | face index from the 16-bit word [ecx+0x12]: orig zero-extends with `xor edx,edx; mov dx,..` (walker: 0), built with `mov dx,..; and edx,0xffff` on a stale edx that the walker took from a float. Int idiom only; same fcomp
- 0x41cd7c | 0x2ff | NOISE | face index from the 16-bit word [ecx+0x12]: orig zero-extends with `xor edx,edx; mov dx,..` (walker: 0), built with `mov dx,..; and edx,0xffff` on a stale edx that the walker took from a float. Int idiom only; same fcomp
- 0x41cd86 | 0x316 | NOISE | face index from the 16-bit word [ecx+0x12]: orig zero-extends with `xor edx,edx; mov dx,..` (walker: 0), built with `mov dx,..; and edx,0xffff` on a stale edx that the walker took from a float. Int idiom only; same fcomp
- 0x41d3fe | 0x8e8 | NOISE | sphere test: the (a-b)^2 sums and (rA+rB)^2 use the same op order (fld/fsub x3, `fld st0; fmul` .. faddp, fstp/mov/cmp bits) as built 0x8b0..0x8e8. Built caches the second sphere pointer in a local (L1) where orig re-reads M[M[M[this+0x234]]+0x18]
- 0x41d3fe | 0x8e8 | NOISE | sphere test: the (a-b)^2 sums and (rA+rB)^2 use the same op order (fld/fsub x3, `fld st0; fmul` .. faddp, fstp/mov/cmp bits) as built 0x8b0..0x8e8. Built caches the second sphere pointer in a local (L1) where orig re-reads M[M[M[this+0x234]]+0x18]

## src/unk_465e90.cpp 0x465e90

`0x465e90 src/unk_465e90.cpp: events 16/16, 16 differ`

- 0x46636a | 0x4da | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x466381 | 0x4f1 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x466398 | 0x508 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x4663af | 0x51f | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x4663d4 | 0x549 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x4663e2 | 0x552 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x4663ea | 0x55a | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x466408 | 0x578 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x46641a | 0x58a | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x466423 | 0x593 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x466441 | 0x5b1 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x46644e | 0x5be | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x46646c | 0x5dc | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x466479 | 0x5e9 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x466499 | 0x609 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code
- 0x4664a2 | 0x612 | NOISE | the instructions at every event are identical (`call 0x407f60; fmul [edi+k]; fstp [esi+k]`). The row base differs only in the int index register (orig `lea edi,[ebp+0x54]` vs built `lea edi,[ebx+0x54]`), whose source the walker resolves from unrelated code

## src/unk_41fc60.cpp 0x41fc60

`0x41fc60 src/unk_41fc60.cpp: events 25/24, 11 differ, unreliable`

- 0x41fd74 | - | NOISE | orig pops the leftover dot copy with `fstp [eax+0x4]`; built copies the same field value as an int
- 0x41fe44 | 0x188 | NOISE (24-bit precision control) | rounding point. Orig keeps the loop-1 accumulators (all but negA) on the x87 stack across the whole contact loop (0x41fcec fld x3; `fadd [ebp-0x14]` with fxch st(3)/st(1)/st(4)/st(2)); only negA is in memory. Built keeps three of them in memory (0x105 `fadd [ebp-0x2c]; fstp [ebp-0x2c]` etc.), so each partial sum is rounded to float. Here orig does `fcom 0x4e1448` on the extended st0, built `fcomp [ebp-0x34]` on the stored float. Fixing it needs the whole-function FPU-allocation rewrite described in the source comment; local edits cannot reach it. Reclassified by the orchestrator: FUN_004a7d30 sets x87 precision control to 24 bits (analysis/fpu-control.md), so a sum held on the FPU stack rounds exactly like one stored to a float slot; the values are identical outside overflow/underflow.
- 0x41fe5e | 0x1a0 | NOISE (24-bit precision control) | the same accumulator rounding point as 0x41fe44 (orig `fld st(1); fcomp` on the extended negC; built `fld K; fcomp st(1)` after the per-iteration float stores)
- 0x4200f3 | 0x3e7 | NOISE | impulse j and F(0x1fc..0x204) += c->n*j use the same association. Orig reads the Xform/Cross results through the returned pointer (R2), built through its local copy (L): call-result naming
- 0x420102 | 0x3f6 | NOISE | impulse j and F(0x1fc..0x204) += c->n*j use the same association. Orig reads the Xform/Cross results through the returned pointer (R2), built through its local copy (L): call-result naming
- 0x420123 | 0x404 | NOISE | impulse j and F(0x1fc..0x204) += c->n*j use the same association. Orig reads the Xform/Cross results through the returned pointer (R2), built through its local copy (L): call-result naming
- 0x420183 | 0x467 | NOISE | tq + F(0x208..): orig reads the FUN_004203e0 result through the returned pointer [R0], built through the local tq; commutative add of the same fields
- 0x420192 | 0x476 | NOISE | tq + F(0x208..): orig reads the FUN_004203e0 result through the returned pointer [R0], built through the local tq; commutative add of the same fields
- 0x4201a1 | 0x485 | NOISE | tq + F(0x208..): orig reads the FUN_004203e0 result through the returned pointer [R0], built through the local tq; commutative add of the same fields
- 0x4202b2 | 0x592 | NOISE | spd*inv: on the built side the AddContact call makes the walker forget slots near the lea'd lp/ln arrays (L0*L1), while orig forwards the expression. Both are `fld spd; fmul inv` from stack slots
- 0x42034c | 0x635 | NOISE | spd*inv: on the built side the AddContact call makes the walker forget slots near the lea'd lp/ln arrays (L0*L1), while orig forwards the expression. Both are `fld spd; fmul inv` from stack slots

## src/unk_45ad10.cpp 0x45bc20

`0x45bc20 src/unk_45ad10.cpp: events 27/26, 7 differ, unreliable`

- 0x45bc77 | - | NOISE | DAT_00509418 = V->rate: orig fld/fstp, built int mov (W5_PassFloat copy)
- 0x45c0d7 | 0x4ab | NOISE | downforce d0 uses rot[0..2]: orig reads all three through rotp (eax=[ebp-0x5c]), built through [ecx]/[esi+0x24]/[esi+0x28]. rotp == b->rot, so the addresses and the association are the same
- 0x45c0f0 | 0x4c0 | NOISE | torque += FUN_004203e0(..): orig reads the result through the returned eax ([eax],[eax+4],[eax+8]), built through the local slot [ebp-0x18..]; same value
- 0x45c10a | 0x4cf | NOISE | torque += FUN_004203e0(..): orig reads the result through the returned eax ([eax],[eax+4],[eax+8]), built through the local slot [ebp-0x18..]; same value
- 0x45c119 | 0x4de | NOISE | torque += FUN_004203e0(..): orig reads the result through the returned eax ([eax],[eax+4],[eax+8]), built through the local slot [ebp-0x18..]; same value
- - | 0x709 | NOISE | same event as orig 0x45c371 (delta*0.05 added to centre); unpaired only because of block order
- 0x45c371 | - | NOISE | counterpart of built 0x709 (identical tree)

## src/unk_45ad10.cpp 0x45c700

`0x45c700 src/unk_45ad10.cpp: events 16/16, 1 differ`

- 0x45c8e7 | 0x1e7 | REAL-FIXED | rounding point: d = dist in the dist<rideTop arm. Orig keeps the unstored product hFrac*t on the FPU (`0x45c89a fld st(1)` for comp; `0x45c8bf fcom [ebp-0x10]`, then st0 goes straight into `0x45c8e4 fsub [ebp-0x28]`). Built reloaded the stored float (`0x1bb fld [ebp-0x20]; fcomp [ebp-0x10]` .. `0x1c8 fld [ebp-0x20]`). Fixed with `d = hFrac * t;` in that arm: built now `0x191 fld st(0); fst [ebp-0x44]; fld st(0)` .. `0x1bf fcomp [ebp-0x10]` keeps the extended product into `0x1e4 fsub [ebp-0x24]` (match.py 656 -> 641 differing bytes). The event line remains only because the walker phis the two arms differently

## src/unk_45ad10.cpp 0x45cc30

`0x45cc30 src/unk_45ad10.cpp: events 41/41, 3 differ`

- 0x45d73c | 0xb10 | NOISE | radius (0.45*f10f8) is in the dead param slot [ebp+0x8] in orig (the walker forwards it) and in [ebp-0x38] in built (not forwarded across the call); same `fld st1; fadd radius; fsub [ebp-0x8c]; faddp` order
- 0x45d780 | 0xb5a | NOISE | orig subtracts its int copy of v->rideHeight ([ebp-0x50], copied by mov from [ebx+0x10fc] at 0x45d61f before Cast); built reads [ebx+0x10fc] directly. Same field, and Cast does not write it. Both keep the extended dist (fcom [ebp+0xc]/[ebp+0x8])
- 0x45d905 | 0xcd5 | NOISE | 0.01 + radius: radius slot naming, as in 0x45d73c

## src/unk_4ba440.cpp 0x4ba620

`0x4ba620 src/unk_4ba440.cpp: events 12/12, 10 differ`

- 0x4ba6d9 | 0xc0 | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba6db | 0xc2 | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba6de | 0xc5 | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba743 | 0x11e | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba745 | 0x120 | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba748 | 0x123 | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba75f | 0x13a | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba764 | 0x13f | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba783 | 0x15e | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded
- 0x4ba7a1 | 0x17c | NOISE | orig addresses planes/tplanes through this (edi) and keeps them in registers; built goes through the copied pointers L0/L1 (P/T locals). The Xform/Rot inline asm and the q dot sum are byte-identical (no x87 hunk in insdiff), so the trees differ only in how the T[0..2] stores are named and forwarded

## src/unk_4137c0.cpp 0x4137c0

`0x4137c0 src/unk_4137c0.cpp: events 9/3, 6 differ`

- 0x41413e | - | NOISE | orig copies the colour floats with fld [esp+0x28..0x30]/fstp [edi|eax+0x60..0x68]; built copies them as ints (mov [edi+0x60],edx ...): same bits
- 0x414155 | - | NOISE | orig copies the colour floats with fld [esp+0x28..0x30]/fstp [edi|eax+0x60..0x68]; built copies them as ints (mov [edi+0x60],edx ...): same bits
- 0x414166 | - | NOISE | orig copies the colour floats with fld [esp+0x28..0x30]/fstp [edi|eax+0x60..0x68]; built copies them as ints (mov [edi+0x60],edx ...): same bits
- 0x4141af | - | NOISE | orig copies the colour floats with fld [esp+0x28..0x30]/fstp [edi|eax+0x60..0x68]; built copies them as ints (mov [edi+0x60],edx ...): same bits
- 0x4141c6 | - | NOISE | orig copies the colour floats with fld [esp+0x28..0x30]/fstp [edi|eax+0x60..0x68]; built copies them as ints (mov [edi+0x60],edx ...): same bits
- 0x4141de | - | NOISE | orig copies the colour floats with fld [esp+0x28..0x30]/fstp [edi|eax+0x60..0x68]; built copies them as ints (mov [edi+0x60],edx ...): same bits

## src/unk_4741a0.cpp 0x4746b0

`0x4746b0 src/unk_4741a0.cpp: events 12/12, 6 differ`

- - | 0x151 | NOISE | pairing shift: built 0x151 = orig 0x474817, built 0x1af = orig 0x474861, built 0x1be = orig 0x474870, built 0x357 = orig 0x474a15 (identical trees, one event earlier or later in the walk)
- 0x474817 | 0x1af | NOISE | pairing shift: built 0x151 = orig 0x474817, built 0x1af = orig 0x474861, built 0x1be = orig 0x474870, built 0x357 = orig 0x474a15 (identical trees, one event earlier or later in the walk)
- 0x474861 | 0x1be | NOISE | pairing shift: built 0x151 = orig 0x474817, built 0x1af = orig 0x474861, built 0x1be = orig 0x474870, built 0x357 = orig 0x474a15 (identical trees, one event earlier or later in the walk)
- 0x474870 | - | NOISE | pairing shift: built 0x151 = orig 0x474817, built 0x1af = orig 0x474861, built 0x1be = orig 0x474870, built 0x357 = orig 0x474a15 (identical trees, one event earlier or later in the walk)
- - | 0x357 | NOISE | pairing shift: built 0x151 = orig 0x474817, built 0x1af = orig 0x474861, built 0x1be = orig 0x474870, built 0x357 = orig 0x474a15 (identical trees, one event earlier or later in the walk)
- 0x474a15 | - | NOISE | pairing shift: built 0x151 = orig 0x474817, built 0x1af = orig 0x474861, built 0x1be = orig 0x474870, built 0x357 = orig 0x474a15 (identical trees, one event earlier or later in the walk)

## src/unk_45e480.cpp 0x45e480

`0x45e480 src/unk_45e480.cpp: events 74/72, 2 differ, unreliable`

- 0x4606e4 | - | NOISE | orig copies the vector 0x109c..0x10a4 into +0x128..0x130 with fld/fstp, built with int moves (same bits)
- 0x4606f6 | - | NOISE | orig copies the vector 0x109c..0x10a4 into +0x128..0x130 with fld/fstp, built with int moves (same bits)

## src/unk_45e480.cpp 0x462350

`0x462350 src/unk_45e480.cpp: events 119/119, 2 differ`

- - | 0xfe5 | NOISE | the identical G=cos(0.0174532924*R0) event is listed at different positions (block order)
- 0x46334f | - | NOISE | the identical G=cos(0.0174532924*R0) event is listed at different positions (block order)

## src/unk_478020.cpp 0x478220

`0x478220 src/unk_478020.cpp: events 7/7, 4 differ`

- 0x4783d1 | 0x1b1 | NOISE | sin/cos table at 0x561ec0: orig shows absolute 0x561ec0/0x561ec4, built relocation-relative +0/+4; same trees
- 0x4783fe | 0x1de | NOISE | sin/cos table at 0x561ec0: orig shows absolute 0x561ec0/0x561ec4, built relocation-relative +0/+4; same trees
- 0x47843b | 0x21b | NOISE | sin/cos table at 0x561ec0: orig shows absolute 0x561ec0/0x561ec4, built relocation-relative +0/+4; same trees
- 0x478468 | 0x248 | NOISE | sin/cos table at 0x561ec0: orig shows absolute 0x561ec0/0x561ec4, built relocation-relative +0/+4; same trees

## src/unk_49fee0.cpp 0x49ffa0

`0x49ffa0 src/unk_49fee0.cpp: events 6/6, 1 differ`

- 0x4a02db | 0x363 | NOISE | instructions identical at both addresses (`fld [esp+0x40]; fmul [esi+0x1d0]` ... `fstp [esp+0x2c]; fcomp 0x4e134c`). The walker forwards the built local vector from int stores of 0 (`xor eax; mov [ebp+..],eax`) and leaves orig's as L0..L2

## src/unk_49fee0.cpp 0x4a0420

`0x4a0420 src/unk_49fee0.cpp: events 24/24, 2 differ, unreliable`

- 0x4a095a | 0x4fd | NOISE | `fcom [esp+0x2c]` on st0 at a loop-exit join reached by jmp: the orig walker has the reloaded value L0, the built walker carried the 1/sqrt from the fall-through. Linear-walk phi artefact; same instruction
- 0x4a0bf0 | 0x79b | NOISE | `fcom [esp+0x2c]` on st0 at a loop-exit join reached by jmp: the orig walker has the reloaded value L0, the built walker carried the 1/sqrt from the fall-through. Linear-walk phi artefact; same instruction

## src/unk_43c590.cpp 0x43cc50

`0x43cc50 src/unk_43c590.cpp: events 6/6, 2 differ, unreliable`

- 0x43d017 | 0x3c8 | NOISE | title x/y: orig loads 142/54 from .rdata 0x4e17c0/0x4e17bc; built loads them from the writable statics M3_titleY/M3_titleX (= 142/54), shown as G. Same value and same fmul [0x56aeac]/[0x56aea8]
- 0x43d029 | 0x3da | NOISE | title x/y: orig loads 142/54 from .rdata 0x4e17c0/0x4e17bc; built loads them from the writable statics M3_titleY/M3_titleX (= 142/54), shown as G. Same value and same fmul [0x56aeac]/[0x56aea8]

## src/unk_46fa80.cpp 0x46fc10

`0x46fc10 src/unk_46fa80.cpp: events 4/4, 2 differ`

- - | 0x198 | NOISE | the identical event is listed unpaired on both sides (block order)
- 0x46fd9e | - | NOISE | the identical event is listed unpaired on both sides (block order)

## src/unk_40a850.cpp 0x40a850

`0x40a850 src/unk_40a850.cpp: events 21/21, 1 differ, unreliable`

- 0x40aec6 | 0x63c | NOISE | same field M[this+0x4c]; for this product built reads this from a frame slot (L0), orig from the register

## src/unk_44b930.cpp 0x44cfb0

`0x44cfb0 src/unk_44b930.cpp: events 10/9, 1 differ, unreliable`

- 0x44d136 | - | NOISE | orig stores x=485*scale to [esp+0x14] and y into the int slot it reuses after 3 pushes; built uses [esp+0x1c]/[esp+0x18]. Both push y then x before `mov ecx,esi; call 0x4a97a0` (orig 0x44d170..0x44d180, built 0x1a8..0x1c5); the walker loses the orig x slot

## src/unk_4758b0.cpp 0x475eb0

`0x475eb0 src/unk_4758b0.cpp: events 7/7, 1 differ, unreliable`

- 0x47610e | 0x25e | NOISE | orig `fld [0x4e2048]` (8.0); built `fld kEight`, which is `extern const float kEight` at 0x4e2048. The unresolved extern shows as G; same constant

