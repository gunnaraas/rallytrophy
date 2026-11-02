# x87 behaviour ledger, batch 3

Events from `x87check.py --sym --list` before any fix. Format: `orig_addr | built_off | REAL-FIXED / NOISE | reason`.


## 0x49c130 src/unk_49b8e0.cpp: events 66/65, 7 differ, unreliable

0x49c550 | 0x415 | NOISE | divisor [esp+0x10] = (a+b)+c in orig (49c3ba fld [esp+0x2c]; fadd [esp+0x60]; fadd [esp+0x6c]; fstp [esp+0x10]) and built (285 fld [esp+0x14]; fadd [esp+0x24]; fadd [esp+0x4c]; fstp [esp+0x18]), same chain; built tree lost the slot across the __ftol call (call-forget artefact); fdiv [esp+0x10] / fdiv [esp+0x18] identical
0x49cb77 | 0xa63 | NOISE | same tree L*(L*((x+y)-1)); orig reads the scale from a frame copy (int copy of arg+0xc), built straight from arg+0xc - int vs float copy naming
0x49cb80 | 0xa6c | NOISE | square of the previous value, same shape; arg+0xc vs its frame copy (naming)
0x49cb87 | 0xa74 | NOISE | cube of the previous value, same shape; arg+0xc vs its frame copy (naming)
0x49ce8b | - | NOISE | orig copies unk7c->unk78 with fld [edi+0x7c]; fst [edi+0x78] (49ce88), built with mov ecx,[esi+0x7c]; mov [esi+0x78],ecx (d50/d66) - int vs float copy
0x49cefd | 0xe47 | NOISE | len = sqrt(x*x + z*z), two terms (orig 49ced7.. [esp+0x20]^2 + [esp+0x18]^2, built e21.. [esp+0x84]^2 + [esp+0x7c]^2); orig names the Vec3 call result R0, built the args - call-result naming
0x49cfc9 | 0xf2b | NOISE | dot (a*b) + ((0*c) + (d*e)) same association both sides incl. the stored-zero y term; leaves differ only by call-result vs arg naming

## 0x49d970 src/unk_49b8e0.cpp: events 25/24, 6 differ, unreliable

0x49dab6 | 0x137 | NOISE | same 3-term sqrt; orig re-walks this->unk14->unk28->unk24, built caches the pointer in a local (L0) - pointer naming
0x49db01 | 0x17f | NOISE | same 1 + G*(dot) tree; pointer cached in a local in built (naming)
0x49e235 | 0x8cc | NOISE | orig stores unk48 in each branch of if(unk114) (49e235 / 49e249: blend*nx+140 and unk80*nx+140), built merges the branches (8a8 fld st(0); fmul [esp+0xa0]; fadd [ebx]; fstp [esi+0x48]); same products/sums - branch layout
0x49e249 | 0x8dc | NOISE | the else-branch copy of the unk48 store (49e23a fld [esi+0x80]; fmul [esp+0x94]; fadd [ebx]); built has one merged store - branch layout
0x49e271 | 0x95b | NOISE | unk50 = unk148 + w*(nz) same both sides (49e252 fmul [esp+0x9c]; fadd [esi+0x148] vs 8cf fmul [esp+0xa8]; fadd [esi+0x148]); paired against built's unk34 because of the merged branch
0x49e31f | - | NOISE | unk34 = (e4*0 + e8*n1) + e0*n0 (49e2f4..49e31d vs 927..959) same association; orig re-reads unk48 from memory (phi) where built forwards the stored value - linear-walk/phi artefact

## 0x49e3d0 src/unk_49b8e0.cpp: events 66/64, 34 differ, unreliable

0x49e8ee | 0x514 | REAL-FIXED | unk2c = t*len0/total: tree identical (built lost the total slot), but the rounding point of t differed: orig 49e8da fdivp; 49e8dc fst [esp+0x18]; 49e8e0 fmul [esp+0x28] (unrounded t), built 4fc fdivp; 4fe fstp [esp+0x1c]; 502 fld [esp+0x1c]; 506 fmul (float-rounded t); fixed by assigning unk2c before sp.t (all three branches): built now 4fc fdivp; 4fe fst [esp+0x1c]; 502 fmul [esp+0x34]
- | 0x5f4 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (built branch-1 sp.t/t2/t3 at 5f4; orig 49ea2d, event 7) - L0*(L1*(x-1)) == L0*((x-1)*L1)
- | 0x603 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (t2^2, orig 49ea3b)
- | 0x608 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (t2^3, orig 49ea40)
- | 0x615 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (unk14c.x = Eval() call result, orig 49ea4d)
- | 0x633 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (unk14c.z = Eval() call result, orig 49ea6b)
0x49ea2d | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 5f4; step*((t2-1)*len) chained as the source comment says)
0x49ea3b | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 603)
0x49ea40 | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 608)
0x49ea4d | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 615)
0x49ea6b | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 633)
0x49eb9c | 0x7b6 | REAL-FIXED | unk2c = (t*len1 + len0)/total: same association; rounding point of t fixed as in 0x49e8ee (orig 49eb84 fdivp; fst [esp+0x1c]; fmul [esp+0x2c]; built now 790 fdivp; 792 fst [esp+0x10]; 796 fmul [esp+0x28], was fstp/fld)
- | 0x872 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (branch 2 t2, orig 49ecbb)
- | 0x881 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig 49ecca)
- | 0x886 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig 49eccf)
- | 0x894 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig 49ecdd)
- | 0x8b2 | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig 49ecfc)
0x49ecbb | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 872)
0x49ecca | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 881)
0x49eccf | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 886)
0x49ecdd | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 894)
0x49ecfc | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built 8b2)
0x49edfe | 0xa09 | REAL-FIXED | unk2c = ((t*len2 + len1) + len0)/total same association (49edf2 fadd [esp+0x2c]; fadd [esp+0x28] vs 9fc/a01); rounding point of t fixed (orig 49ede2 fdivp; fst [esp+0x1c]; fmul [esp+0x24]; built now 9dd fdivp; 9df fst [esp+0x14]; 9e3 fmul [esp+0x18], was 9e4 fstp/9e8 fld)
- | 0xada | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (branch 3 t2, orig 49ef38)
- | 0xaea | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig 49ef47)
- | 0xaef | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig 49ef4c)
- | 0xafd | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig 49ef5a)
0x49ef38 | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built ada)
0x49ef47 | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built aea)
0x49ef4c | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built aef)
0x49ef5a | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (orig of built afd)
0x49ef79 | - | NOISE | pairing shift from different branch layout: the same tree is emitted on the other side at a different position (counts equal per branch), see the neighbouring unpaired line (unk14c.z call result; built's copy is in the merged tail)
0x49f070 | - | NOISE | unk50 = unk148 + nz*width in the if(unk114) branch; orig duplicates the store per branch, built merges (same as 0x49d970 events 3-4) - branch layout
0x49f155 | 0xd2b | NOISE | unk34 dot same association ((e4*0 + e8*n1) + e0*n0); orig re-reads unk48/unk50 (phi), built forwards - phi artefact

## 0x49f1f0 src/unk_49b8e0.cpp: events 8/8, 1 differ

0x49f243 | 0x53 | REAL-FIXED | constant: orig 49f243 fcomp DWORD PTR ds:0x4e1348 (1.0), built 53 fcomp ds:__real 0.0 (`closeness < 0.0f`); now `closeness < DAT_004e1348`, built 53 fcomp DWORD PTR ds:?DAT_004e1348 - 0 differ

## 0x41ef60 src/unk_41ef60.cpp: events 50/47, 40 differ, unreliable

0x41f08c | 0x131 | NOISE | first-loop contact block: orig stores h[9..14] in the order 0x30,0x34,0x24,0x38 (41f0bb..41f0ed) and builds v.x on both if(flag) paths (41f03f / 41f069 fld [edi+0x4]), built stores 0x24 after 0x30..0x38 (14f..182); same values, dot ((x*h6 + y*h7) + z*h8) identical (41f075..41f087 vs 114..12c) - store order + phi of the flag==0 field read, pairing shifts by one
0x41f0d6 | 0x167 | NOISE | first-loop contact block: orig stores h[9..14] in the order 0x30,0x34,0x24,0x38 (41f0bb..41f0ed) and builds v.x on both if(flag) paths (41f03f / 41f069 fld [edi+0x4]), built stores 0x24 after 0x30..0x38 (14f..182); same values, dot ((x*h6 + y*h7) + z*h8) identical (41f075..41f087 vs 114..12c) - store order + phi of the flag==0 field read, pairing shifts by one
0x41f0df | 0x176 | NOISE | first-loop contact block: orig stores h[9..14] in the order 0x30,0x34,0x24,0x38 (41f0bb..41f0ed) and builds v.x on both if(flag) paths (41f03f / 41f069 fld [edi+0x4]), built stores 0x24 after 0x30..0x38 (14f..182); same values, dot ((x*h6 + y*h7) + z*h8) identical (41f075..41f087 vs 114..12c) - store order + phi of the flag==0 field read, pairing shifts by one
0x41f0e4 | 0x17f | NOISE | first-loop contact block: orig stores h[9..14] in the order 0x30,0x34,0x24,0x38 (41f0bb..41f0ed) and builds v.x on both if(flag) paths (41f03f / 41f069 fld [edi+0x4]), built stores 0x24 after 0x30..0x38 (14f..182); same values, dot ((x*h6 + y*h7) + z*h8) identical (41f075..41f087 vs 114..12c) - store order + phi of the flag==0 field read, pairing shifts by one
0x41f0e7 | 0x182 | NOISE | first-loop contact block: orig stores h[9..14] in the order 0x30,0x34,0x24,0x38 (41f0bb..41f0ed) and builds v.x on both if(flag) paths (41f03f / 41f069 fld [edi+0x4]), built stores 0x24 after 0x30..0x38 (14f..182); same values, dot ((x*h6 + y*h7) + z*h8) identical (41f075..41f087 vs 114..12c) - store order + phi of the flag==0 field read, pairing shifts by one
0x41f0ed | 0x18b | NOISE | first-loop contact block: orig stores h[9..14] in the order 0x30,0x34,0x24,0x38 (41f0bb..41f0ed) and builds v.x on both if(flag) paths (41f03f / 41f069 fld [edi+0x4]), built stores 0x24 after 0x30..0x38 (14f..182); same values, dot ((x*h6 + y*h7) + z*h8) identical (41f075..41f087 vs 114..12c) - store order + phi of the flag==0 field read, pairing shifts by one
0x41f106 | 0x1a3 | NOISE | first-loop contact block: orig stores h[9..14] in the order 0x30,0x34,0x24,0x38 (41f0bb..41f0ed) and builds v.x on both if(flag) paths (41f03f / 41f069 fld [edi+0x4]), built stores 0x24 after 0x30..0x38 (14f..182); same values, dot ((x*h6 + y*h7) + z*h8) identical (41f075..41f087 vs 114..12c) - store order + phi of the flag==0 field read, pairing shifts by one
0x41f11f | 0x1bb | NOISE | first-loop contact block: orig stores h[9..14] in the order 0x30,0x34,0x24,0x38 (41f0bb..41f0ed) and builds v.x on both if(flag) paths (41f03f / 41f069 fld [edi+0x4]), built stores 0x24 after 0x30..0x38 (14f..182); same values, dot ((x*h6 + y*h7) + z*h8) identical (41f075..41f087 vs 114..12c) - store order + phi of the flag==0 field read, pairing shifts by one
0x41f50c | 0x58b | NOISE | pairing shift: built's dn argument (585 fstp [ebp+0x10]; push edx, (x*h6 + (y*h7 + z*h8))) has its orig counterpart 41f4d6 fstp [esp] in the equal run; the t2 compare pairs with built line 10
0x41f53b | 0x5cf | NOISE | t2 compare: (tz^2 + ty^2) + tx^2 both (41f4f6..41f50a); orig names vel-nn via the operator* result R1, built via h[9] and locals - call-result naming; paired one off
0x41f546 | 0x608 | NOISE | h[16] = tx*inv same; naming (R1 vs locals), one-off pairing
0x41f554 | 0x613 | NOISE | h[17] = ty*inv same; naming, one-off pairing
0x41f557 | 0x61e | NOISE | h[19] = sqrt(t2) same; naming, one-off pairing
0x41f586 | 0x621 | NOISE | h[18] = tz*inv same; naming, one-off pairing
0x41f59b | 0x643 | NOISE | grip compare 9.81*SF(0x42c) vs tl*invDt same; one-off pairing
0x41f5c3 | 0x658 | NOISE | h[15] = (imp*invDt) * ((invDt*tl)/grip) both sides (orig L0*L1 spilled imp, built expands k); one-off pairing
0x41f601 | 0x65b | NOISE | NEG(h[16]) sign test of the same product; naming, one-off pairing
0x41f623 | 0x67e | NOISE | NEG(h[17]) sign test; naming, one-off pairing
0x41f6a2 | 0x69d | NOISE | BF(0x1fc) += h6*(SF(0x29c)*imp): same tree (orig names imp L0, built expands it); one-off pairing
0x41f6c2 | 0x6ed | NOISE | BF(0x200) += h7*(SF*imp); same; one-off pairing
0x41f6d6 | 0x6fe | NOISE | BF(0x204) += h8*(SF*imp); same; one-off pairing
0x41f71f | 0x70d | NOISE | BF(0x208) = BF + tq.x two terms; orig reads the FUN_004203e0 result R0, built a local copy - call-result naming
0x41f72e | 0x773 | NOISE | BF(0x20c) = tq.y + BF two terms; call-result naming
0x41f742 | 0x785 | NOISE | BF(0x210) = tq.z + BF two terms; call-result naming
- | 0x797 | NOISE | unpaired tail of the same 3-store group (shift by one)
0x41f866 | 0x8a8 | NOISE | BF(0x240) < depth*k: orig spills depth and k (L0*L1), built expands both; same product
0x41f909 | 0x94e | NOISE | BF(0x240) = depth*k; spilled vs expanded (naming)
0x41f979 | 0x9df | NOISE | orig unrolls the 6-way cap loop (41f979..41f9e5: int cmp [ebp-0x58],eax; fld [ebp+0x8]; fdiv [ebp-0x58]), built keeps the loop (9cc cmp ecx,[ebp+eax-0x130]; fdiv [ebp+eax-0x130]); same cap/acc[j] - loop shape
0x41f990 | - | NOISE | unrolled iteration 2 of the cap loop (int compare of cap), built loop body - loop shape
0x41f9a7 | - | NOISE | unrolled iteration 3 - loop shape
0x41f9be | - | NOISE | unrolled iteration 4 - loop shape
0x41f9d5 | - | NOISE | unrolled iteration 5 - loop shape
0x41fac6 | 0xaff | NOISE | BF(0x268) < work: same tree; differs only in the slot naming of sc[] (indexed vs fixed slots)
0x41fad6 | 0xb0b | NOISE | BF(0x268) = work; same as above
0x41fb73 | 0xb85 | NOISE | BF(0x1e4) = BF + ng.x two terms; orig reads operator- result R0, built a local - call-result naming
0x41fb96 | 0xb97 | NOISE | BF(0x1e8) two terms; call-result naming
0x41fbaa | 0xba9 | NOISE | BF(0x1ec) two terms; call-result naming
0x41fbfc | 0xc0d | NOISE | BF(0x1f0) = tq.x + BF two terms; call-result naming
0x41fc0b | 0xc1f | NOISE | BF(0x1f4) two terms; call-result naming
0x41fc1a | 0xc31 | NOISE | BF(0x1f8) two terms; call-result naming

## 0x40c9b0 src/unk_40c9b0.cpp: events 29/39, 17 differ, unreliable

- | 0xb54 | NOISE | built copies p+0x1e0 -> obj+0x12c via fld/fstp (b4e/b54), orig with mov eax,[ebx+0x1e0]; mov [ecx+0x12c],eax (40d4fc/40d51c) - int vs float copy
- | 0xb63 | NOISE | same, +0x1dc -> +0x128 (b5a/b63) - int vs float copy
- | 0xb72 | NOISE | same, +0x1e4 -> +0x130 (b69/b72) - int vs float copy
- | 0xd4c | REAL-FIXED | field read type: orig 40d6ea cmp DWORD PTR [edi+0x10c],esi (int != 0), built d46 fld [eax+0x10c]; d4c fcomp 0.0 (`FV(p,0x10c) != 0.0f`, differs for -0.0/NaN bit patterns); now `IV(p,0x10c) != 0`, built cmp DWORD PTR [..+0x10c],0 - event gone
- | 0xe9f | NOISE | G + G*int(x+1) arg (40d810 fmul ds:0x508494; fadd ds:0x508490 family); built `inc` vs orig `+1`, same value; pairing shift by one
0x40d8de | 0x1233 | NOISE | orig 40d8de G+G*int(x+1) paired with a built global copy; its true counterpart is built 0xe9f (line 5) - pairing
- | 0x123c | NOISE | built writes globals from fields with fld/fstp, orig with mov (int vs float copy)
- | 0x124c | NOISE | int vs float copy of a field to a global
- | 0x1393 | NOISE | int vs float copy of a field to a global
- | 0x139c | NOISE | int vs float copy of a field to a global
- | 0x13ac | NOISE | int vs float copy of a field to a global
0x40de37 | 0x149d | NOISE | unk170 = (float)n*cell + base: orig 40de1b fild [ebx+0x194]; fmul [ebx+0x188]; fadd [edi], built 1473 fld [ebx+0x188]; fimul [ebx+0x194]; fadd [eax] - same exact product, tool does not model fimul
0x40de48 | 0x14ae | NOISE | unk178 likewise: orig 40de39 fild; fmul [ebx+0x190]; fadd [ebx+0x16c], built 1494 fld [ebx+0x190]; fimul [ebp+0x10]; fadd [ebx+0x16c] - fimul not modelled
0x40df34 | 0x1577 | NOISE | grid cell x store (40df34 fst [eax] vs 156c fst [eax-0x8]); orig's row pointer came via a global-derived register (G), built a slot - pointer naming
0x40df41 | 0x1585 | NOISE | grid cell z store, same; pointer naming
0x40e353 | 0x199f | NOISE | G + G*int(x+1): inc vs +1, same value
0x40e4bb | 0x1b0b | NOISE | G + G*int(x+1): inc vs +1, same value

## 0x421fa0 src/unk_420490.cpp: events 37/35, 17 differ, unreliable

0x422658 | 0x73c | NOISE | orig restores text->scaleX with fld ds:0x56aea8; fstp [edi+0x1c] (42262c/422658), built with an int copy - int vs float copy; paired against a shifted arg
0x422662 | 0x742 | NOISE | scaleY likewise (42265b/422662) - int vs float copy
0x42273c | - | NOISE | y = 262*sy arg (constant 262 = __real in both); pairing shift
0x42273d | - | NOISE | 54*sx Print arg; pairing shift
- | 0x9d3 | NOISE | 350*sx DrawSelector arg (orig 422748 fld ds:0x4e1464 = 350.0); pairing shift
- | 0xa33 | NOISE | 54*sx arg; pairing shift
0x422a38 | 0xb3e | NOISE | y = 42*sy + y (orig 4229fd fld ds:0x4e1458=42; fmul [esp+0x28]; fadd [esp+0x18]); orig y slot read as phi 0 by the linear walk
0x422a39 | 0xb3f | NOISE | 250*sx arg (4e1460 = 250.0) paired with built's next 42*sy+y; pairing shift
- | 0xb40 | NOISE | 54*sx arg; pairing shift
0x422b84 | 0xc80 | NOISE | 424*sy arg (4e146c = 424.0) same; built sy slot is phi (layout)
0x422b94 | 0xc94 | NOISE | 576*sx arg (4e1468 = 576.0) same; phi naming
0x422c9b | 0xda8 | NOISE | 575*sx arg (4e1470 = 575.0) same; phi naming
0x422cdf | 0xe89 | NOISE | 798*sx (4e1474 = 798.0) paired with built y2 = 15*sy + y; pairing shift
0x422da0 | - | NOISE | (25+575)*sx: orig 422d67 fld ds:0x4e1480 (25.0); fadd ds:0x4e1470; fmul [esp+0x1c] = source (DAT_004e1480+575)*sx; tool tree mis-tracks the slot
0x422de4 | 0xec9 | NOISE | (25+798)*sx (422daa fld 4e1480; fadd 4e1474; fmul) same as source; paired with built y2 arg - pairing
0x422eb0 | - | NOISE | orig restores scaleX with fld ds:0x56aea8; fstp [edi+0x1c] (422eaa/422eb0), built int copy
0x422ebc | - | NOISE | scaleY restore (422eb3/422ebc), int vs float copy

## 0x43b1d0 src/unk_43a390.cpp: events 21/16, 13 differ, unreliable

0x43b341 | 0x177 | NOISE | scaleX = G+G same; orig re-walks this->unkc->+0x1390c4, built caches the renderer pointer - pointer naming
0x43b358 | 0x193 | NOISE | scaleY = G+G same; pointer naming
0x43b398 | - | NOISE | scaleX restore by fld/fstp in orig, int copy in built
0x43b3a3 | - | NOISE | scaleY restore, int vs float copy
- | 0x27f | NOISE | built pushes colB = 345*scale computed in place, orig pushes the precomputed colB slot as an int (push) - int vs float copy of an arg
0x43b485 | 0x2c3 | NOISE | x = colA - GetWidth(): orig colA spilled (L0), built recomputes 88*G (colA); same value
0x43b590 | 0x3d7 | NOISE | scaleX = 1.5*G same (4e1530 vs __real 1.5); orig names the renderer via a stale fp-register expression - pointer naming
0x43b59f | 0x3e6 | NOISE | scaleY = 1.5*G same; pointer naming
0x43b733 | 0x55f | NOISE | orig 43b733 `push ebx` (renderer) named with a stale value, built 55e push esi (rowY slot): register pushes of non-float args - naming; constants 345/210 verified in source and exe
0x43b80e | - | NOISE | scale restore int vs float copy
0x43b817 | - | NOISE | scale restore int vs float copy
0x43c50f | - | NOISE | scale restore int vs float copy
0x43c518 | - | NOISE | scale restore int vs float copy

## 0x419f30 src/unk_419f30.cpp: events 11/13, 10 differ

0x41a4d5 | 0x371 | NOISE | built copies a point with fld/fst (365/371), orig with int movs - int vs float copy; orig cmp at 41a4d5 pairs with built 544 (line 3)
0x41a568 | 0x389 | NOISE | cross product stores: identical code (41a53e..41a56d vs 5a8..5d7); pairing shift by one
0x41a56a | 0x544 | NOISE | cross y store vs built's fcomp [esi-0x20] (orig 41a4d5 counterpart) - pairing shift
0x41a56d | 0x5d2 | NOISE | cross z / x store, same code (41a568 / 5d2), pairing shift
0x41a5b5 | 0x5d4 | NOISE | normalize: both reload the stored fields (41a579.. fld [esi+0x18..0x20] / 5da.. fld [esi-0xc],[esi-0x10],[ebx]); built tree forwards the cross product because it aliases [esi-0x10] with [eax+0x4] - aliasing artefact, same instructions
0x41a5bd | 0x5d7 | NOISE | normalize y, same instructions (41a5ba / 60b); aliasing artefact
0x41a5c3 | 0x607 | NOISE | normalize z, same (41a5c0 / 611); aliasing artefact
0x41a5ec | 0x60e | NOISE | plane d = n.p same association (41a5c9..41a5ec vs 61a..63e); pairing shift
- | 0x614 | NOISE | built normalize z (tail of the shifted group)
- | 0x63e | NOISE | built plane d (tail of the shifted group)

## 0x42c8e0 src/unk_42c2b0.cpp: events 11/11, 7 differ, unreliable

0x42cf61 | 0x67c | NOISE | 136*scale: built reads static H4b_titleY (136.0, mirror of 0x4e15dc, value verified against the exe) - writable static vs .rdata
0x42cf73 | 0x692 | NOISE | 54*scale: H4b_titleX = 54.0 (0x4e15d8 verified) - writable static
0x42d05c | 0x77d | NOISE | 320*scale: H4b_toggleW = 320.0 (0x4e162c verified)
0x42d060 | 0x781 | NOISE | 250*L: H4b_toggleX = 250.0 (0x4e160c verified)
0x42d1e3 | 0x904 | NOISE | 270*scale: H4b_valueW = 270.0 (0x4e1628 verified)
0x42d428 | 0xb49 | NOISE | 270*scale: H4b_valueW (verified)
0x42d5b9 | 0xcda | NOISE | 270*scale: H4b_valueW (verified)

## 0x42d710 src/unk_42c2b0.cpp: events 1/1, 1 differ

0x42d79b | 0x8b | NOISE | 42 + (166 - 20): H4c_btn 42.0 (0x4e1640), H4c_base 166.0 (0x4e1610), H4c_trim 20.0 (0x4e1654), all verified against the exe - writable static

## 0x4013c0 src/unk_401000.cpp: events 7/7, 6 differ

0x40150c | 0x14c | NOISE | integer/register naming: orig keeps the arg in ebx after an early-return `pop ebx` path, the linear walk names it ebx0; built names arg+0x8 / ebx0 the other way round - same value (linear-walk artefact)
0x40153e | 0x174 | NOISE | integer/register naming: orig keeps the arg in ebx after an early-return `pop ebx` path, the linear walk names it ebx0; built names arg+0x8 / ebx0 the other way round - same value (linear-walk artefact)
0x401588 | 0x1b0 | NOISE | integer/register naming: orig keeps the arg in ebx after an early-return `pop ebx` path, the linear walk names it ebx0; built names arg+0x8 / ebx0 the other way round - same value (linear-walk artefact)
0x40159d | 0x1dd | NOISE | integer/register naming: orig keeps the arg in ebx after an early-return `pop ebx` path, the linear walk names it ebx0; built names arg+0x8 / ebx0 the other way round - same value (linear-walk artefact)
0x4015be | 0x1f9 | NOISE | integer/register naming: orig keeps the arg in ebx after an early-return `pop ebx` path, the linear walk names it ebx0; built names arg+0x8 / ebx0 the other way round - same value (linear-walk artefact)
0x4015cf | 0x20f | NOISE | integer/register naming: orig keeps the arg in ebx after an early-return `pop ebx` path, the linear walk names it ebx0; built names arg+0x8 / ebx0 the other way round - same value (linear-walk artefact)

## 0x43ffd0 src/unk_43f9e0.cpp: events 13/14, 6 differ, unreliable

0x4406e4 | 0x714 | NOISE | 400*sx: built reads static B2_COLW (400.0 = 0x4e1834, verified) - writable static
0x440712 | 0x743 | NOISE | 0*400: pushed-register naming of a non-float arg; B2_COLW static
0x440744 | 0x774 | NOISE | same as above
0x440754 | 0x784 | NOISE | orig 440754 push eax (int arg) named by a stale fp value; built 784 push ecx likewise - naming
0x440776 | 0x7a1 | NOISE | orig 440776 fstp [esp] = B2_COLW*sx (44075d fld 4e1834; fmul [esp+0x14]) vs built 7a6 same; pairing of the stale push events
- | 0x7a6 | NOISE | built extra stale-push event (7a1 push ecx) - naming

## 0x40f390 src/unk_40f390.cpp: events 7/7, 5 differ

0x40f556 | 0x1d9 | NOISE | same (0.5*sqrt(..) + (r + unk130))^2 compare; built names the cell pointer via a different index expression - pointer naming
0x40f575 | 0x1fd | NOISE | InsertA arg sqrt(...) - unkc4: same; index/pointer naming
0x40f776 | 0x444 | NOISE | same compare; built names the object via R0 (call-result register reuse), orig the full pointer chain - naming; code at 40f75a..40f776 = 425..444
0x40f79a | 0x466 | NOISE | InsertA arg (40f780 fsqrt; fsub [ecx+0xc4] vs 44e/45a) same; naming
0x40f946 | 0x5d2 | NOISE | radius r = slot + obj[0x28] two terms (40f861 fld [ebp-0x60]; fadd [ecx+0x28] vs 4e9 fld [edx+0x28]; fadd [ebp-0x64]); orig slot named G - commutative 2-term

## 0x473470 src/unk_472c90.cpp: events 6/6, 4 differ, unreliable

0x473919 | 0x4b5 | NOISE | same dot over the three dequantised components; orig names the vector arg+0x8+0x80, built a local pointer to it; second fist is a phi in orig (linear walk) - naming
- | 0xa06 | NOISE | built M[L0+4] = R0 + L1 store, orig's counterpart is line 3 (pairing shift)
0x473e9c | 0xa70 | NOISE | orig of built 0xa06 (same tree); paired with built's next compare - pairing
0x473eee | - | NOISE | orig compare (same tree as built 0xa70) - pairing shift

## 0x4766d0 src/unk_4766d0.cpp: events 8/8, 3 differ

- | 0x266 | NOISE | built 0x266 = orig 0x476953 tree (0.75 + 0.0625*int); shifted by one event
0x476953 | 0x294 | NOISE | orig 0x4c store paired with built 0x78 store; true counterpart built 0x266 - pairing
0x476961 | - | NOISE | orig 0x78 = 0.0624 + (...) store, same tree as built 0x294 - pairing

## 0x40b600 src/unk_40b560.cpp: events 2/2, 2 differ

0x40b6b7 | 0xb7 | REAL-FIXED | product grouping: orig 40b66a fild [ebp-0x14]; fmul ds:0x4e13b0 (0.002); fmul ds:0x4e13ac (255) = (age*0.002)*255, built 6a fild; fmul DAT_004e13ac; fmul DAT_004e13b0 (VC6 regrouped `age * b0 * ac`); now two statements, built 6a fild; 6d fmul ?DAT_004e13b0; 73 fmul ?DAT_004e13ac
0x40b71e | 0x11e | REAL-FIXED | same grouping in the fade-out branch: orig 40b69f fild; fmul 4e13b0; fmul 4e13ac, built 9f fild; a2 fmul DAT_004e13b0; a8 fmul DAT_004e13ac after the fix (was ac then b0)

## 0x458b30 src/unk_458700.cpp: events 25/24, 2 differ, unreliable

0x458e18 | 0x2e0 | REAL-FIXED | constant: orig 458e12 fmul ds:0x4e1bd8 = 0x3cea0ea1 (0.0285714287 = 1/35), built literal 0.02857143f = 0.0285714306; now 0.028571429f (0x3cea0ea1) - 0 differ
0x45901c | - | NOISE | orig copies this+0x4c with fld/fstp, built with an int mov - int vs float copy

## 0x4ac920 src/unk_4ab910.cpp: events 8/8, 2 differ, unreliable

0x4acc00 | 0x2dd | NOISE | float sum identical after masking memory operands (bilinear weights and order equal); only the neighbour index differs: orig clamps with a branch (phi 0), built with sbb/neg - integer codegen
0x4acc26 | 0x303 | NOISE | same as above for the second channel

## 0x46dd80 src/unk_46d830.cpp: events 2/2, 1 differ

0x46e0d1 | 0x355 | REAL-FIXED | constant: orig 0x4e1f60 = 0x386147ae (5.37109372e-05 = 0.055/1024), built kDropSizeStep 5.371e-5f = 5.37099986e-05; now 5.3710937e-5f - 0 differ

## After the fixes

`x87check --sym --quiet` on the batch: 0x49f1f0, 0x40b600, 0x46dd80 now 0 differ; 0x458b30 1 (int/float copy), 0x40c9b0 16 (field-type event gone). 0x49e3d0 stays at 34: the t rounding fix is invisible to the tree comparison. match.py counts did not change for any touched file.
