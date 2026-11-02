# x87 behaviour ledger, batch 2

One line per differing event of `tools/x87check.py --sym --list` (run before the fixes): `orig_addr | built_off | class | reason`. REAL-FIXED lines name the original and built instructions; "REAL (not fixed)" marks a difference that could not be reproduced.

## 0x417540 Stage::FUN_00417540 (src/unk_417540.cpp)

`0x417540 src/unk_417540.cpp: events 50/57, 55 differ, unreliable`

- 0x4176a0 | 0x162 | NOISE | d2=(rec.x-car.x)^2+(rec.z-car.z)^2: orig 0x41767b fld [eax] (rec+0)..fsub [ebp-0x18] (car+0x14, 0x417609), 0x417686 fld rec+8..fsub [ebp-0x10] (car+0x1c, 0x41760e), 0x41768c-0x417694 sq+sq; built 0x140 fld [ecx] (rec+8)..fsub [ebp-0xb4] (car+0x1c, 0xd3/0xdd), 0x148 fld [ecx-0x8] (rec+0)..fsub [ebp-0x10] (car+0x14, 0xc8/0xda), 0x14e-0x156: same fields, a two-term sum (commutative only); compare: orig 0x4176a0 fcomp [ebp-0x7c] = 0x240*0x240 (0x417607-0x417617), built 0x162 fcomp [ebp-0x78] = 0x240*0x240 (0xd6-0xe3), both test ah,0x1 je skip
- 0x4176ad | 0x16f | NOISE | d2 as 0x4176a0; orig 0x4176ad fcomp [ebp-0x1c] = (0x240-0x244)^2 (0x417628-0x417638), built 0x16f fcomp [ebp-0x14] = (0x240-0x244)^2 (0xf1-0x101), both test ah,0x41 jne skip
- 0x4176ba | 0x17c | NOISE | d2 as 0x4176a0; orig 0x4176ba fcomp st(1) = 0x240^2*[0x4e1330]=10 (0x41761f-0x417622), built 0x17c fcomp st(1) = DAT_004e1330*[ebp-0x78] (0xe8-0xee, reloc DAT_004e1330); same best-so-far st(1), same test ah,0x1
- 0x417767 | 0x24c | NOISE | DIR = r[3..5]-r[0..2]: orig int copies r+0xc/0x10/0x14 to esi+0x1390.. (0x417740-0x417769) then fld [eax+0x0]..fsub [edi]..fstp (0x417767); built int copies (0x213-0x237) then fld [eax+0x0]..fsub [edi]..fstp (0x24c); eax=esi+0x1390, edi=record on both: same field 0x1390, same subtrahend
- 0x417787 | 0x260 | NOISE | DIR = r[3..5]-r[0..2]: orig int copies r+0xc/0x10/0x14 to esi+0x1390.. (0x417740-0x417769) then fld [eax+0x4]..fsub [edi+0x4]..fstp (0x417787); built int copies (0x213-0x237) then fld [eax+0x4]..fsub [edi+0x4]..fstp (0x260); eax=esi+0x1390, edi=record on both: same field 0x1394, same subtrahend; orig 0x41777e zeroes 0x1394 between its fld 0x41776c and fstp 0x417787, built zeroes it at 0x1f8 before the copy: overwritten on both, same final value
- 0x4177b4 | 0x279 | NOISE | DIR = r[3..5]-r[0..2]: orig int copies r+0xc/0x10/0x14 to esi+0x1390.. (0x417740-0x417769) then fld [eax+0x8]..fsub [edi+0x8]..fstp (0x4177b4); built int copies (0x213-0x237) then fld [eax+0x8]..fsub [edi+0x8]..fstp (0x279); eax=esi+0x1390, edi=record on both: same field 0x1398, same subtrahend
- 0x4177eb | 0x2b7 | REAL-FIXED | was (z*z+y*y)+x*x; now DIR.Normalize: orig 0x4177be fld [eax+8]; fld [eax+4]; fld [eax]; 0x4177c9-0x4177d7 = (x*x+y*y)+z*z, fsqrt, fdivr [0x4e1348]=1.0; built 0x283 fld [eax+8]; 0x28d fld [eax+4]; 0x293 fld [eax]; 0x295-0x2a3 same (x*x+y*y)+z*z, 0x2ab fsqrt, 0x2ad fdivr DAT_004e1348 (fsqrt after the pops instead of before: same value); orig 0x4177e9 fmul [eax]..0x4177eb fstp [eax], built 0x2b5 fmul [eax]..0x2b7 fstp [eax]
- 0x4177f2 | 0x2be | REAL-FIXED | as 0x4177eb (verified); orig 0x4177ef fmul [eax+0x4]..0x4177f2 fstp, built 0x2bb fmul [eax+0x4]..0x2be fstp
- 0x4177f8 | 0x2c4 | REAL-FIXED | as 0x4177eb (verified); orig 0x4177f5 fmul [eax+0x8]..0x4177f8 fstp, built 0x2c1 fmul [eax+0x8]..0x2c4 fstp
- 0x417834 | 0x2fa | REAL-FIXED | Length sum feeding it fixed as 0x4177eb; RIGHT = cross(up, DIR): orig ecx=[ebp-0x2c]=&{0,1,0} at [ebp-0x18] (0x4177a6-0x4177b7), edx=[ebp-0x1c]=esi+0x1390, eax=[ebp+8]=esi+0x1384, 12-insn cross 0x41780a-0x417832; built ecx=[ebp-0x10]=&{0,1,0} at [ebp-0x20] (0x272-0x286), edx=[ebp-0x14]=esi+0x1390, eax=[ebp+8]=esi+0x1384, identical cross 0x2d0-0x2f8 (same operand offsets and fsubp order); the 0/1 tree terms are those constants; orig 0x417834 fstp [eax], built 0x2fa fstp [eax]
- 0x417836 | 0x2fc | REAL-FIXED | as 0x417834 (verified); orig 0x417836 fstp [eax+0x4], built 0x2fc fstp [eax+0x4]
- 0x417839 | 0x2ff | REAL-FIXED | as 0x417834 (verified); orig 0x417839 fstp [eax+0x8], built 0x2ff fstp [eax+0x8]
- 0x4178ec | 0x3ac | NOISE | dist = sqrt((dx^2+dz^2)+dy^2), d=car+0x14/0x18/0x1c minus esi+0x1378/0x137c/0x1380: orig 0x417889-0x4178df (dx [ebp-0x44], dy [ebp-0x40], dz [ebp-0x3c]); built 0x34f-0x39f (fsubr [eax+0x14..0x1c], dx [ebp-0x60], dy [ebp-0x5c], dz [ebp-0x58]), same (x,z) then y order; orig 0x4178e4 fld [esi+0x244]; fadd st0; 0x4178ec fcomp [ebp+8]=dist; built 0x3a4/0x3aa/0x3ac identical, both test ah,0x41 jne
- 0x4178f9 | 0x3bc | NOISE | dist as 0x4178ec; orig 0x4178f6 fld dist; fcomp [esi+0x244]; test ah,0x41 je interp, built 0x3b6 fld [esi+0x244]; 0x3bc fcomp dist; test ah,0x1 je speed=0x238: operands swapped with the inverse test, same branch (dist > 0x244 -> interp); interp orig 0x417911-0x417936 (1-t)*0x238 + t*0x234, t=(d-0x244)/0x244, built 0x3c6-0x3eb same
- 0x417982 | 0x441 | NOISE | dt = int(arg+0xc)*0.001: orig 0x41793b fild [ebp+0x10]; 0x41794d fmul [0x4e1400]=0.001, built 0x3fd fild; 0x40c fmul DAT_004e1400; v = 0x13a0 + ((dt*0x23c)*speed)/0x234: orig 0x417961 fld dt; fmul [esi+0x23c]; fmul [ebp+0xc]; fdiv [esi+0x234]; fadd [esi+0x13a0], built 0x426 fld [esi+0x23c]; fmul dt; fmul [ebp+0xc]; fdiv [esi+0x234]; fadd [esi+0x13a0]: same fields and grouping; orig 0x417982 fst [esi+0x13a0], built 0x441 fst [esi+0x13a0]; phi is the walk merging the speed branches
- 0x417988 | 0x450 | NOISE | floor arg: orig 0x417988 fstp QWORD [esp] of the same st, built 0x450 fstp QWORD [esp] (built first saves st via 0x447 fld [ebp+0x10]/0x44d fstp QWORD [ebp-0x74]); same value v as 0x417982
- 0x417996 | 0x45e | NOISE | v - floor(v): orig 0x417990 fsubr DWORD [ebp+0x10] (v as float), built 0x458 fsubr QWORD [ebp-0x74] (double of that same float v, 0x447-0x44d); call _floor both; orig 0x417996 / built 0x45e fstp [esi+0x13a0]
- 0x4179a8 | - | NOISE | t = dt*speed + 0x139c vs seg len: orig 0x41799c fld [ebp+8]; fmul [ebp+0xc]; fadd [esi+0x139c]; 0x4179a8 fcom [edi+0x18]; test ah,0x41 jne continue; built 0x464-0x46a same chain, 0x470 fld [ebx+0x18]; 0x47b fcomp st(1); test ah,0x1 je continue: swapped operands + inverse test, same branch; edi/ebx = 0x13ac + idx*0x24 (orig 0x417953/0x41795e, built 0x403/0x412); counterpart is built 0x47b (pairing shift)
- 0x417a07 | 0x47b | NOISE | pairing shift, POS.x = r[3]-r[0]: orig (edi=record, esi+0x1378=POS) 0x4179ed int copy r+0xc..; 0x4179ff fld [esi+0x1378]; fsub [edi]; 0x417a07 fstp; built (ebx=record, edi=esi+0x1378) 0x4b3-0x4c2 int copy; 0x4c5 fld [edi]; fsub [ebx]; 0x4c9 fstp; same fields/order, true counterpart built 0x4c9
- 0x417a16 | 0x4c9 | NOISE | pairing shift, POS.y sub: orig (edi=record, esi+0x1378=POS) 0x417a0d fld [esi+0x137c]; fsub [edi+0x4]; 0x417a16 fstp; built (ebx=record, edi=esi+0x1378) 0x4cb fld [edi+0x4]; fsub [ebx+0x4]; 0x4d1 fstp; same fields/order, true counterpart built 0x4d1
- 0x417a25 | 0x4d1 | NOISE | pairing shift, POS.z sub: orig (edi=record, esi+0x1378=POS) 0x417a1c fld [esi+0x1380]; fsub [edi+0x8]; 0x417a25 fstp; built (ebx=record, edi=esi+0x1378) 0x4d4 fld [edi+0x8]; fsub [ebx+0x8]; 0x4da fstp; same fields/order, true counterpart built 0x4da
- 0x417a33 | 0x4da | NOISE | pairing shift, POS.x *= t/len: orig (edi=record, esi+0x1378=POS) 0x4179e1 fdiv [edi+0x18] (st=t/len); 0x417a2b fld st0; fmul [esi+0x1378]; 0x417a33 fstp; built (ebx=record, edi=esi+0x1378) 0x4ad fdiv [ebx+0x18]; 0x4dd fld st0; fmul [edi]; 0x4e1 fstp; same fields/order, true counterpart built 0x4e1
- 0x417a41 | 0x4e1 | NOISE | pairing shift, POS.y *= t/len: orig (edi=record, esi+0x1378=POS) 0x417a39 fld st0; fmul [esi+0x137c]; 0x417a41 fstp; built (ebx=record, edi=esi+0x1378) 0x4e3 fld st0; fmul [edi+0x4]; 0x4e8 fstp; same fields/order, true counterpart built 0x4e8
- 0x417a4d | 0x4e8 | NOISE | pairing shift, POS.z *= t/len: orig (edi=record, esi+0x1378=POS) 0x417a47 fmul [esi+0x1380]; 0x417a4d fstp; built (ebx=record, edi=esi+0x1378) 0x4eb fmul [edi+0x8]; 0x4ee fstp; same fields/order, true counterpart built 0x4ee
- 0x417a5b | 0x4ee | NOISE | pairing shift, POS.x += r[0]: orig (edi=record, esi+0x1378=POS) 0x417a53 fld [esi+0x1378]; fadd [edi]; 0x417a5b fstp; built (ebx=record, edi=esi+0x1378) 0x4f1 fld [ebx]; fadd [edi]; 0x4f5 fstp (operands swapped, one add); same fields/order, true counterpart built 0x4f5
- 0x417a6a | 0x4f5 | NOISE | pairing shift, POS.y += r[1]: orig (edi=record, esi+0x1378=POS) 0x417a61 fld [esi+0x137c]; fadd [edi+0x4]; 0x417a6a fstp; built (ebx=record, edi=esi+0x1378) 0x4f7 fld [edi+0x4]; fadd [ebx+0x4]; 0x4fd fstp; same fields/order, true counterpart built 0x4fd
- 0x417a79 | 0x4fd | NOISE | pairing shift, POS.z += r[2]: orig (edi=record, esi+0x1378=POS) 0x417a70 fld [esi+0x1380]; fadd [edi+0x8]; 0x417a79 fstp; built (ebx=record, edi=esi+0x1378) 0x500 fld [edi+0x8]; fadd [ebx+0x8]; 0x506 fstp; same fields/order, true counterpart built 0x506
- 0x417b2f | 0x506 | NOISE | pairing shift, box+0x14 lo.x: 0x417b2f mov [edi+0x14],[ebp-0x6c]; 0x5ac mov [ebx+0x14],[ebp-0x54]; true counterpart built 0x5ac. box = POS -/+ (3, 2*25, 3): orig 0x417a7f fld x; fsub [0x4e13dc]=3 ->[ebp-0x6c]; 0x417aa3 fld [0x4e13f4]=25; fadd st0; y-2c ->[ebp-0x68]; z-3 ->[ebp-0x64]; x+3 ->[ebp-0x54]; 2c+y ->[ebp-0x50]; z+3 ->[ebp-0x4c]; built 0x509-0x598 same with DAT_004e13dc/DAT_004e13f4 (relocs 0x51f/0x50d/0x55d/0x576/0x594): lo.x [ebp-0x54], lo.y [ebp-0x50], lo.z [ebp-0x4c], hi.x [ebp-0x48], hi.y [ebp-0x44] (y+2c), hi.z [ebp-0x40]; stored by int mov to box (orig edi=esi+0x13b8, built ebx=esi+0x13b8)
- 0x417b3b | 0x5ac | NOISE | pairing shift, box+0x18 lo.y: 0x417b3b mov [edi+0x18],[ebp-0x68]; 0x5be mov [ebx+0x18],[ebp-0x50]; true counterpart built 0x5be. box = POS -/+ (3, 2*25, 3): orig 0x417a7f fld x; fsub [0x4e13dc]=3 ->[ebp-0x6c]; 0x417aa3 fld [0x4e13f4]=25; fadd st0; y-2c ->[ebp-0x68]; z-3 ->[ebp-0x64]; x+3 ->[ebp-0x54]; 2c+y ->[ebp-0x50]; z+3 ->[ebp-0x4c]; built 0x509-0x598 same with DAT_004e13dc/DAT_004e13f4 (relocs 0x51f/0x50d/0x55d/0x576/0x594): lo.x [ebp-0x54], lo.y [ebp-0x50], lo.z [ebp-0x4c], hi.x [ebp-0x48], hi.y [ebp-0x44] (y+2c), hi.z [ebp-0x40]; stored by int mov to box (orig edi=esi+0x13b8, built ebx=esi+0x13b8)
- 0x417b41 | 0x5be | NOISE | pairing shift, box+0x20 hi.x: 0x417b41 mov [edi+0x20],[ebp-0x54]; 0x5c4 mov [ebx+0x20],[ebp-0x48]; true counterpart built 0x5c4. box = POS -/+ (3, 2*25, 3): orig 0x417a7f fld x; fsub [0x4e13dc]=3 ->[ebp-0x6c]; 0x417aa3 fld [0x4e13f4]=25; fadd st0; y-2c ->[ebp-0x68]; z-3 ->[ebp-0x64]; x+3 ->[ebp-0x54]; 2c+y ->[ebp-0x50]; z+3 ->[ebp-0x4c]; built 0x509-0x598 same with DAT_004e13dc/DAT_004e13f4 (relocs 0x51f/0x50d/0x55d/0x576/0x594): lo.x [ebp-0x54], lo.y [ebp-0x50], lo.z [ebp-0x4c], hi.x [ebp-0x48], hi.y [ebp-0x44] (y+2c), hi.z [ebp-0x40]; stored by int mov to box (orig edi=esi+0x13b8, built ebx=esi+0x13b8)
- 0x417b4a | 0x5c4 | NOISE | pairing shift, box+0x1c lo.z: 0x417b4a mov [edi+0x1c],[ebp-0x64]; 0x5cd mov [ebx+0x1c],[ebp-0x4c]; true counterpart built 0x5cd. box = POS -/+ (3, 2*25, 3): orig 0x417a7f fld x; fsub [0x4e13dc]=3 ->[ebp-0x6c]; 0x417aa3 fld [0x4e13f4]=25; fadd st0; y-2c ->[ebp-0x68]; z-3 ->[ebp-0x64]; x+3 ->[ebp-0x54]; 2c+y ->[ebp-0x50]; z+3 ->[ebp-0x4c]; built 0x509-0x598 same with DAT_004e13dc/DAT_004e13f4 (relocs 0x51f/0x50d/0x55d/0x576/0x594): lo.x [ebp-0x54], lo.y [ebp-0x50], lo.z [ebp-0x4c], hi.x [ebp-0x48], hi.y [ebp-0x44] (y+2c), hi.z [ebp-0x40]; stored by int mov to box (orig edi=esi+0x13b8, built ebx=esi+0x13b8)
- 0x417b51 | 0x5cd | NOISE | pairing shift, box+0x24 hi.y: 0x417b51 mov [edi+0x24],[ebp-0x50]; 0x5d4 mov [ebx+0x24],[ebp-0x44]; true counterpart built 0x5d4. box = POS -/+ (3, 2*25, 3): orig 0x417a7f fld x; fsub [0x4e13dc]=3 ->[ebp-0x6c]; 0x417aa3 fld [0x4e13f4]=25; fadd st0; y-2c ->[ebp-0x68]; z-3 ->[ebp-0x64]; x+3 ->[ebp-0x54]; 2c+y ->[ebp-0x50]; z+3 ->[ebp-0x4c]; built 0x509-0x598 same with DAT_004e13dc/DAT_004e13f4 (relocs 0x51f/0x50d/0x55d/0x576/0x594): lo.x [ebp-0x54], lo.y [ebp-0x50], lo.z [ebp-0x4c], hi.x [ebp-0x48], hi.y [ebp-0x44] (y+2c), hi.z [ebp-0x40]; stored by int mov to box (orig edi=esi+0x13b8, built ebx=esi+0x13b8)
- 0x417b57 | 0x5d4 | NOISE | pairing shift, box+0x28 hi.z: 0x417b57 mov [edi+0x28],[ebp-0x4c]; 0x5e1 mov [ebx+0x28],[ebp-0x40]; true counterpart built 0x5e1. box = POS -/+ (3, 2*25, 3): orig 0x417a7f fld x; fsub [0x4e13dc]=3 ->[ebp-0x6c]; 0x417aa3 fld [0x4e13f4]=25; fadd st0; y-2c ->[ebp-0x68]; z-3 ->[ebp-0x64]; x+3 ->[ebp-0x54]; 2c+y ->[ebp-0x50]; z+3 ->[ebp-0x4c]; built 0x509-0x598 same with DAT_004e13dc/DAT_004e13f4 (relocs 0x51f/0x50d/0x55d/0x576/0x594): lo.x [ebp-0x54], lo.y [ebp-0x50], lo.z [ebp-0x4c], hi.x [ebp-0x48], hi.y [ebp-0x44] (y+2c), hi.z [ebp-0x40]; stored by int mov to box (orig edi=esi+0x13b8, built ebx=esi+0x13b8)
- - | 0x5e1 | NOISE | pairing shift: built 0x5e1 mov [ebx+0x28],[ebp-0x40] (hi.z = 3+z) is the counterpart of orig 0x417b57 mov [edi+0x28],[ebp-0x4c]; see 0x417b2f
- - | 0x70a | NOISE | SetColour(0x13a0) arg: orig 0x417c6b mov eax,[esi+0x13a0]; push eax (int copy, no event), built 0x6fd fld [esi+0x13a0]; 0x70a fstp [esp]: same 4 bytes; callee 0x4021b0 both
- 0x417c94 | 0x738 | NOISE | h2 < 1: orig 0x417c68 fstp [ebp+0xc] (2nd Cast result, out [ebp-0x18]); 0x417c91 fld [ebp+0xc]; 0x417c94 fcomp [0x4e1348]=1.0; built 0x6fa fstp [ebp-0x70] (2nd Cast, out [ebp-0x84]); 0x735 fld [ebp-0x70]; 0x738 fcomp DAT_004e1348; h1 checked first on both (orig 0x417c80, built 0x715); only R0/L0 naming differs
- 0x417cc7 | 0x767 | NOISE | obj+0x40 = hit1-hit2: hit1 = 1st Cast out (orig [ebp-0x78..-0x70], built [ebp-0x20..-0x18]), hit2 = 2nd Cast out (orig [ebp-0x18..-0x10], built [ebp-0x84..-0x7c]); both copy hit1 into obj+0x40 by int mov (orig 0x417ca5-0x417cb8, built 0x749-0x759); obj = [[esi+0x13a4]+0xc] (orig ebx, built ebx); orig 0x417cad fld [eax]; fsub [ebp-0x18]; 0x417cc7 fstp [eax], built 0x75c fld [ecx]; fsub [ebp-0x84]; 0x767 fstp [ecx] (eax/ecx = obj+0x40)
- 0x417cd5 | 0x76f | NOISE | obj+0x44: orig 0x417ccc fld [eax+0x4]; fsub [ebp-0x14]; 0x417cd5 fstp, built 0x769 fld [ecx+0x4]; fsub [ebp-0x80]; 0x76f fstp; hit1.y - hit2.y on both (see 0x417cc7)
- 0x417cde | 0x778 | NOISE | obj+0x48: orig 0x417cd8 fld [eax+0x8]; fsub [ebp-0x10]; 0x417cde fstp, built 0x772 fld [ecx+0x8]; fsub [ebp-0x7c]; 0x778 fst (z kept in st for the normalise); hit1.z - hit2.z on both
- 0x417d0b | 0x7a2 | NOISE | normalise obj+0x40: orig 0x417ce1 fld z; fld y; fld x; 0x417ce9-0x417cf7 (x*x+y*y)+z*z; fsqrt; fdivr [0x4e1348]; built z still in st from 0x778 fst, 0x77b fld y; 0x77e fld x; 0x780-0x78e same (x*x+y*y)+z*z; 0x790 fsqrt; 0x798 fdivr DAT_004e1348; orig 0x417d07 fld st0; fmul [eax]; 0x417d0b fstp, built 0x79e fld st0; fmul [ecx]; 0x7a2 fstp
- 0x417d1a | 0x7a9 | NOISE | as 0x417d0b; orig 0x417d15 fld st0; fmul [eax+0x4]; 0x417d1a fstp, built 0x7a4 fld st0; fmul [ecx+0x4]; 0x7a9 fstp
- 0x417d29 | 0x7af | NOISE | as 0x417d0b; orig 0x417d26 fmul [eax+0x8]; 0x417d29 fstp, built 0x7ac fmul [ecx+0x8]; 0x7af fstp
- 0x417d68 | 0x801 | NOISE | obj+0x30 = cross(obj+0x40, obj+0x20), obj+0x20 = int copy of esi+0x1384.. (orig 0x417d0d-0x417d2f, built 0x7b2-0x7c8): orig ecx=[ebp+8]=obj+0x40, edx=[ebp+0xc]=obj+0x20, eax=[ebp+0x10]=obj+0x30, cross 0x417d3e-0x417d66; built ecx=[ebp+0xc]=obj+0x40, edx=[ebp+0x10]=obj+0x20, eax=[ebp+8]=obj+0x30, identical 12-insn cross 0x7d7-0x7ff; orig 0x417d68 / built 0x801 fstp [eax]
- 0x417d6a | 0x803 | NOISE | as 0x417d68; orig 0x417d6a / built 0x803 fstp [eax+0x4]
- 0x417d6d | 0x806 | NOISE | as 0x417d68; orig 0x417d6d / built 0x806 fstp [eax+0x8]
- 0x417d99 | 0x80b | NOISE | pairing: obj+0x18 = 0.5*(hit2.y+hit1.y): orig 0x417d8b fld [ebp-0x14]; fadd [ebp-0x74]; fmul [0x4e138c]=0.5; 0x417d99 fstp [ebx+0x18]; counterpart built 0x822 fld [ebp-0x1c]; fadd [ebp-0x80]; fmul DAT_004e138c; 0x82e fstp [ebx+0x18] (one add, commutative); built 0x80b is the obj+0x14 = POS.x float copy (0x809 fld [edi]) that orig does as int mov 0x417d70/0x417d82
- 0x417e26 | 0x815 | NOISE | else branch: orig 0x417dad-0x417dea int copies esi+0x1390..->obj+0x40, esi+0x1384..->obj+0x20, cross 0x417dfc-0x417e24 with ecx=obj+0x40, edx=obj+0x20, 0x417e26 fstp [eax=obj+0x30]; counterpart built 0x842-0x86f same copies, cross 0x881-0x8a9 ecx=obj+0x40, edx=obj+0x20, 0x8ab fstp [eax=obj+0x30]: same products and order; built 0x815 is the obj+0x18 = POS.y float copy (orig int mov 0x417d85)
- 0x417e28 | 0x81f | NOISE | as 0x417e26: orig 0x417e28 fstp [eax+0x4] <-> built 0x8ad; built 0x81f is the obj+0x1c = POS.z float copy (0x818 fld [esi+0x1380]; orig int mov 0x417d88)
- 0x417e2b | 0x82e | NOISE | as 0x417e26: orig 0x417e2b fstp [eax+0x8] <-> built 0x8b0; built 0x82e is the counterpart of orig 0x417d99
- - | 0x8ab | NOISE | counterpart of orig 0x417e26 (built 0x8ab fstp [eax=obj+0x30] after cross 0x881-0x8a9 = orig 0x417dfc-0x417e24)
- - | 0x8ad | NOISE | counterpart of orig 0x417e28 (built 0x8ad fstp [eax+0x4])
- - | 0x8b0 | NOISE | counterpart of orig 0x417e2b (built 0x8b0 fstp [eax+0x8])
- - | 0x8b5 | NOISE | obj+0x14 = POS.x: built 0x8b3 fld [edi=esi+0x1378]; 0x8b5 fstp [ebx+0x14], orig 0x417e2e/0x417e40 mov via ecx (int copy, no event): same field, exact copy
- - | 0x8be | NOISE | obj+0x18 = POS.y: built 0x8b8 fld [esi+0x137c]; 0x8be fstp [ebx+0x18], orig 0x417e34/0x417e43 int mov
- - | 0x8c7 | NOISE | obj+0x1c = POS.z: built 0x8c1 fld [esi+0x1380]; 0x8c7 fstp [ebx+0x1c], orig 0x417e3a/0x417e46 int mov

## 0x41a890 FUN_0041a890 (src/unk_41a890.cpp)

`0x41a890 src/unk_41a890.cpp: events 49/49, 31 differ`

- 0x41a9fb | 0x169 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa05 | 0x173 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa0b | 0x17d | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa0e | 0x180 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa14 | 0x186 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa17 | 0x189 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa1a | 0x18c | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa1d | 0x18f | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa23 | 0x195 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa26 | 0x198 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa29 | 0x19b | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa2c | 0x19e | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa32 | 0x1a4 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa35 | 0x1a7 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa38 | 0x1aa | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa3b | 0x1ad | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa41 | 0x1b3 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa44 | 0x1b6 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa47 | 0x1b9 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa4a | 0x1bc | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa51 | 0x1c4 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa59 | 0x1c7 | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa5c | 0x1ca | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41aa5f | 0x1cd | NOISE | identical fld/fst chain (`fld [eax]; fst [ebx+0x74]; fst [ebx+0x44]...`); the tool resolves the original ebx as a writable global (G) and the built one as a call result R0 - base naming only
- 0x41b0b6 | 0x837 | NOISE | X87Cross(n, l28, l20): same 12-instruction cross; original reads a/b through the returned pointers of 0x414330 ([ebp-0x24], [ebp-0x1c]), built through the out temps [ebp-0x30]/[ebp-0x3c]; a = v1-v0, b = v2-v0 on both sides, dst n
- 0x41b0b8 | 0x839 | NOISE | as 0x41b0b6
- 0x41b0bb | 0x83c | NOISE | as 0x41b0b6
- 0x41b101 | 0x878 | NOISE | normalise: same sum (z*z + (x*x + y*y)) and fields; address form abs+M*1+0x18 vs M+0*1+0x18 only
- 0x41b10b | 0x882 | NOISE | as 0x41b101
- 0x41b113 | 0x88a | NOISE | as 0x41b101
- 0x41b13a | 0x8b9 | NOISE | plane d chain: same order and fields; address form only

## 0x46c790 Wheel::UpdateForces (src/unk_46c2c0.cpp)

`0x46c790 src/unk_46c2c0.cpp: events 47/50, 24 differ, unreliable`

- 0x46cc7f | 0x4ce | NOISE | slots: v = orig/built [ebp-0xa4,-0xa0,-0x9c]; lonDir orig [ebp-0x40,-0x3c,-0x38] (0x46c9fd-0x46ca80) = built [ebp-0x38,-0x34,-0x30] (0x261-0x2d8); latDir orig [ebp-0x30,-0x2c,-0x28] (0x46caa1-0x46cb16) = built [ebp-0x28,-0x24,-0x20] (0x2f7-0x36c); lon = (vx*lonx + vz*lonz) + vy*lony: orig 0x46cc37-0x46cc67 ->[ebp-0x10], built 0x486-0x4b6 ->[ebp-0x10], same order (lat likewise orig 0x46cc15-0x46cc34 ->[ebp-0x48], built 0x464-0x483 ->[ebp-0x54]); slip = 0x268*0x284 - lon: orig 0x46cc6a fld [esi+0x284]; fmul [esi+0x268]; fst [ebp-0x14]; fsub [ebp-0x10]; fst [ebp-0x80]; 0x46cc7f fstp [esi+0x288], built 0x4b9 fld 0x268; fmul 0x284; fst [ebp-0x14]; fsub [ebp-0x10]; fst [ebp-0x3c]; 0x4ce fstp [esi+0x288]
- 0x46cc9d | 0x4ee | NOISE | c = 2*(|lon|*G*0x2ec): orig 0x46cc85 fld lon; fabs; fld st0; fmul [0x50941c]; fmul [esi+0x2ec]; fadd st0; fst [ebp-0x50]; 0x46cc9d fcomp [0x4e1348]=1.0, built 0x4d4 fld lon; fabs; fld st0 x2; fmul DAT_0050941c; fmul [esi+0x2ec]; fadd st0; fst [ebp-0x58]; 0x4ee fcomp __real 1.0; both test ah,0x1 jne; lon as 0x46cc7f
- 0x46ccb9 | 0x505 | NOISE | c>=1 path: orig 0x46ccaa fld [ebp-0x80] (slip); fld lon; fabs; fdivp; fst [ebp-0x18]; fchs; 0x46ccb9 fstp [esi+0x2f8], built 0x4fb fld [ebp-0x3c] (slip); fdiv st,st(1) (|lon| kept from 0x4d9); fst [ebp-0x18]; fchs; 0x505 fstp [esi+0x2f8]: same slip/|lon|
- 0x46cd0a | 0x554 | NOISE | c<1 path: 0x2f8 *= -0.7 on sign change (orig 0x46ccda fmul [0x4e1c34]=bf333333, built 0x526 __real -0.7); 0x2f8 += G*0x2ec*((lon - slipProd) - |lon|*0x2f8): orig 0x46cce6-0x46cd04 fld lon; fsub [ebp-0x14]; |lon| fmul 0x2f8; fsubp; fmul [0x50941c]; fmul 0x2ec; fadd 0x2f8, built 0x532-0x54e fld lon; fsub [ebp-0x14]; fld st1 (|lon|); fmul 0x2f8; fsubp; fmul DAT_0050941c; fmul 0x2ec; fadd 0x2f8: same; orig 0x46cd0a fcom [0x4e1330]=10.0, built 0x554 fcom __real 10.0, both test ah,0x41 jne
- 0x46cd10 | 0x55a | NOISE | as 0x46cd0a: orig 0x46cd10 fst [esi+0x2f8], built 0x55a fst [esi+0x2f8] of the same chain; clamp to 0x41200000 orig 0x46cd1f / built 0x569
- 0x46cd2b | 0x575 | NOISE | as 0x46cd0a: orig 0x46cd2b fcomp [0x4e1c00]=-10.0 (c1200000), built 0x575 fcomp __real -10.0; both test ah,0x1 je; clamp 0xc1200000 orig 0x46cd38 / built 0x582
- 0x46cd6d | 0x5af | NOISE | store order of independent int copies: orig 0x46cd64 mov [esi+0x300],[ebp-0x18]; 0x46cd6d mov [esi+0x2f0],[ebp-0x10] (lon); 0x46cd73 mov [esi+0x304],[ebp-0x80] (slip); built 0x5af mov [esi+0x304],[ebp-0x3c] (slip); 0x5b5 [esi+0x300],[ebp-0x18]; 0x5bb [esi+0x2f0],[ebp-0x10]; true pairs 0x46cd6d<->0x5bb, 0x46cd73<->0x5af
- 0x46cd73 | 0x5bb | NOISE | see 0x46cd6d (orig 0x46cd73 mov [esi+0x304],edx=[ebp-0x80] <-> built 0x5af mov [esi+0x304],eax=[ebp-0x3c], both = slip from 0x46cc7c / 0x4cb)
- 0x46cd99 | 0x5e4 | NOISE | 0x2f4 += G*0x2e8*(lat - |lon|*0x2f4): orig st=|lon| (kept since 0x46cc88), 0x46cd7e fmul [esi+0x2f4]; fsubr [ebp-0x48] (lat); fmul [esi+0x2e8]; fmul [0x50941c]; fadd [esi+0x2f4]; 0x46cd99 fst, built 0x5c6 fmul [esi+0x2f4]; fsubr [ebp-0x54] (lat); fmul [esi+0x2e8]; fmul DAT_0050941c; fadd [esi+0x2f4]; fst [ebp-0x14]; 0x5e4 fstp [esi+0x2f4]: same fields and order; lat/lon slots as 0x46cc7f
- 0x46cda2 | 0x5ed | NOISE | c < 1: orig 0x46cd9f fld [ebp-0x50]; 0x46cda2 fcomp [0x4e1348]=1.0, built 0x5ea fld [ebp-0x58]; 0x5ed fcomp __real 1.0; c = same chain as 0x46cc9d
- 0x46cdd3 | 0x61a | REAL-FIXED | constant: orig 0x46cdb7 fadd QWORD [0x4e1f28] = 0.0099999997764825821 ((double)0.01f), built had 0.01; now built 0x5ff fadd QWORD __real@8@3ff8a3d70a0000000000 (same value). Blend verified: orig 0x46cdaf-0x46cdd3 q = lat/(|lon|+k); (1-c)*q + newv*c, built 0x5fa-0x61a same terms and one final add (orig newv from st after fst, built from [ebp-0x14] fst of it); orig 0x46cdd3 / built 0x61a fstp [esi+0x2f4]
- 0x46ceb1 | 0x6f7 | NOISE | comb = sqrt(a*a + b*b): orig 0x46ce98 fld [ebp-0x14]; fmul same; fld [ebp-0x20]; fmul same; faddp; fsqrt; fstp [ebp-0x10], built 0x6e1 [ebp-0x14]^2 + [ebp-0x44]^2 same; [ebp-0x14] = latSlip on both (orig 0x46ce43/0x46ce4d, built 0x68c/0x696), orig [ebp-0x20] (0x46cd7b) = built [ebp-0x44] (0x5c3) = ([ebp-0x18]*b(w))/(0x2ac*(b30+b34*w)); orig 0x46ceb1 fcomp [0x4e1ec8]=2pi (40c90fdb), built 0x6f7 fcomp __real 2pi (4001c90fdb)
- 0x46ceb7 | 0x700 | NOISE | as 0x46ceb1: orig 0x46ceae mov edx,[ebp-0x10]; 0x46ceb7 mov [esi+0x294],edx, built 0x6fd mov edx,[ebp-0x10]; 0x700 mov [esi+0x294],edx: int copy of the same comb
- 0x46cf14 | 0x761 | NOISE | as 0x46ceb1: Pacejka arg comb: orig 0x46cefd mov eax,[ebp-0x10]; 0x46cf14 push eax, built 0x74a mov eax,[ebp-0x10]; 0x761 push eax; other pushes identical immediates; call 0x46d590 / ?Pacejka
- 0x46d060 | 0x896 | NOISE | 0x314 = ((0x328*0x318)*0x2ac)*peak: orig 0x46d049 fld 0x2ac; fld [esi+0x328]; fmul [esi+0x318]; fmul st,st(1); fmul [ebp-0x64]; 0x46d060 fstp, built 0x881 fld 0x328; fmul 0x318; fmul 0x2ac; fmul [ebp-0x2c]; 0x896 fstp; orig [ebp-0x64] (0x46cbcd) = built [ebp-0x2c] (0x423) = w*[ebx+0x34]+[ebx+0x30]
- 0x46d0eb | 0x91f | NOISE | |slip*G| < peak*9.81: orig 0x46d0d1 fld [ebp-0x80] (slip); fmul [0x509418]; fld [ebp-0x64]; fmul [0x4e1440]=9.81; fstp [ebp-0x24]; fabs; 0x46d0eb fcomp [ebp-0x24], built 0x905 fld [ebp-0x2c]; fmul __real 9.81 (40029cf5c3); fstp [ebp-0x40]; 0x914 fld [ebp-0x3c] (slip); fmul DAT_00509418; fabs; 0x91f fcomp [ebp-0x40]; same test ah,0x1
- - | 0x96f | NOISE | scalar*vec arg slip: built 0x968 mov edx,[ebp-0x3c]; 0x96f push edx, orig 0x46d134 mov edx,[ebp-0x80]; 0x46d13b push edx: both int pushes of slip (0x46cc7c / 0x4cb), with &lonDir (orig lea [ebp-0x40], built lea [ebp-0x38])
- 0x46d443 | 0xc5f | NOISE | F = latDir*Flat + lonDir*Flon per component: orig 0x46d3ed fld [ebp-0x30]; fmul [ebp-0x18]; fld [ebp-0x40]; fmul [ebp-0x10]; faddp ->[ebp-0x8c] (y 0x46d401, z 0x46d415), built 0xc17 fld [ebp-0x18]; fmul [ebp-0x28]; fld [ebp-0x10]; fmul [ebp-0x38]; faddp ->[ebp-0x8c] (0xc2b, 0xc3f): same factors (slot map as 0x46cc7f); body+0x1e4 += F.x: orig 0x46d429 fld [ebp-0x8c]; fadd [edi+0x1e4]; 0x46d443 fstp, built 0xc53 fld [ebp-0x8c]; fadd [edi+0x1e4]; 0xc5f fstp
- 0x46d4be | 0xcc0 | NOISE | torque += cross(F, r), r = [ebp-0x98..]-body+0x224..: orig r at [ebp-0xcc] (0x46d478-0x46d4a8), call 0x4203e0(out [ebp-0xf0], &F [ebp-0x8c], &r), adds via the returned eax; built r at [ebp-0xc0] (0xc89-0xcd2), call FUN_004203e0(out [ebp-0xd8], &F, &r), adds from out; orig 0x46d4b3 fld [edi+0x1f0]; fadd [eax]; 0x46d4be fstp <-> built 0xcdd fld [ebp-0xd8]; fadd [edi+0x1f0]; 0xcec fstp; built 0xcc0 is the r.y store (= orig 0x46d496 fstp [ebp-0xc8])
- 0x46d4cd | 0xcd2 | NOISE | as 0x46d4be: orig 0x46d4c4 fld [eax+0x4]; fadd [edi+0x1f4]; 0x46d4cd fstp <-> built 0xcf2 fld [ebp-0xd4]; fadd [edi+0x1f4]; 0xcfe fstp; built 0xcd2 is the r.z store (= orig 0x46d4a8)
- 0x46d4dc | 0xcec | NOISE | as 0x46d4be: orig 0x46d4d3 fld [eax+0x8]; fadd [edi+0x1f8]; 0x46d4dc fstp <-> built 0xd04 fld [ebp-0xd0]; fadd [edi+0x1f8]; 0xd10 fstp
- - | 0xcfe | NOISE | counterpart of orig 0x46d4cd (built 0xcf2 fld [ebp-0xd4]; fadd [edi+0x1f4]; 0xcfe fstp)
- - | 0xd10 | NOISE | counterpart of orig 0x46d4dc (built 0xd04 fld [ebp-0xd0]; fadd [edi+0x1f8]; 0xd10 fstp)
- 0x46d578 | 0xdac | REAL-FIXED | constants: original `fmul ds:0x4e1f1c` = 0.98100006580352783f (0x3f7b22d2) and `fadd ds:0x4e1f18` = 0.0098100006580352783f (0x3c20ba20), built had 0.981f/0.00981f (1 ulp lower); now built 0xd98 fmul __real 3ffefb22d2 and 0xd9e fadd __real 3ff8a0ba20 (same bits). Chain verified: orig 0x46d541 fld 0x284; fmul 0x278; fmul 0x264; fld 0x2a4; fmul k1; fadd k2; fmul st,st(1); fadd [esi+0x2e0]; 0x46d578 fstp, built 0xd75-0xdac same order (fmulp); this event no longer differs

## 0x4bb3a0 TraceNodeSweep (src/unk_4bb3a0.cpp)

`0x4bb3a0 src/unk_4bb3a0.cpp: events 18/18, 16 differ, unreliable`

- 0x4bb5a3 | 0x207 | REAL-FIXED | sphere d: original `fld [ebx+0xc0]; fmul [ecx+8]; fld [ebx+0xbc]; fmul [ecx+4]; faddp; fld [ebx+0xb8]; fmul [ecx]; faddp` = (c2*s11 + c1*s10) + c0*s9, built (c1*s10 + c0*s9) + c2*s11 (VC6 re-associated the one-expression sum); chained d = c2*s11; d = c1*s10 + d; d = c0*s9 + d
- 0x4bb5ba | 0x21a | REAL-FIXED | same d as 0x4bb5a3
- 0x4bb5de | 0x23e | REAL-FIXED | same d as 0x4bb5a3
- 0x4bb651 | 0x2a3 | REAL-FIXED | df^2: original `fld [ebp-0x68]; fmul; fld [ebp-0x60]; fmul; faddp; fld [ebp-0x64]; fmul; faddp` = (x^2+z^2)+y^2, built (z^2+y^2)+x^2; chained dq
- 0x4bb6fb | 0x342 | REAL-FIXED | tri facing d: original (p[-4]*s11 + p[-5]*s10) + s9*tv0 (`fld [edi-0x10]; fmul [esi+0x2c]; fld [edi-0x14]; fmul [esi+0x28]; faddp; fld [esi+0x24]; fmul [edx]`), built (p[-5]*s10 + tv0*s9) + p[-4]*s11; chained
- 0x4bb72b | 0x36f | REAL-FIXED | tri d: original (s11*p0 + p[-1]*s10) + s9*p[-2], built (p[-1]*s10 + p[-2]*s9) + p0*s11; chained
- 0x4bb73f | 0x383 | REAL-FIXED | same d as 0x4bb72b
- 0x4bb763 | 0x3a7 | REAL-FIXED | same d as 0x4bb72b
- 0x4bb7c7 | 0x408 | REAL-FIXED | ef^2: original (x^2+z^2)+y^2 ([ebp-0x50],[ebp-0x48],[ebp-0x4c]), built (z^2+y^2)+x^2; chained eq
- 0x4bb80d | 0x44b | REAL-FIXED | t2: original (s8*p[-4] + s7*p[-5]) + s6*tv0 (`fld [esi+0x20]; fmul [edi-0x10]; fld [esi+0x1c]; fmul [edi-0x14]; faddp; fld [esi+0x18]; fmul [ecx]`), built (s6*tv0 + s8*p[-4]) + s7*p[-5]; chained t2 (built now 0x43b fld [esi+0x20]; fmul [edi-0x10] ...). Residual: tv naming
- 0x4bb836 | 0x474 | NOISE | naming only after the fixes: original reads tv[] through a spilled pointer (M[L0]), built through ebx
- 0x4bb859 | 0x497 | NOISE | naming only after the fixes: original reads tv[] through a spilled pointer (M[L0]), built through ebx
- 0x4bb86d | 0x4a8 | NOISE | naming only after the fixes: original reads tv[] through a spilled pointer (M[L0]), built through ebx
- 0x4bb894 | 0x4cf | NOISE | naming only after the fixes: original reads tv[] through a spilled pointer (M[L0]), built through ebx
- 0x4bb93f | 0x580 | NOISE | naming only after the fixes: original reads tv[] through a spilled pointer (M[L0]), built through ebx
- 0x4bb96d | 0x5b1 | NOISE | naming only after the fixes: original reads tv[] through a spilled pointer (M[L0]), built through ebx

## 0x449860 TuneMenu draw FUN_00449860 (src/unk_445e80.cpp)

`0x449860 src/unk_445e80.cpp: events 14/13, 3 differ, unreliable`

- 0x449fb4 | - | NOISE | y = 580*G pushed in the 5-value loop; built pushes the same y (0x744 push ecx) but its slot is a loop phi the tool cannot resolve
- 0x449fb5 | - | NOISE | the `push ecx` that reserves the x slot (overwritten by fstp [esp] = x - half); built 0x745 same
- - | 0x8ae | NOISE | 604*G: original computes it before the FUN_00405380 call into a frame slot (`fstp [esp+0x28]`) and pushes it as int; built computes after the call (`fstp [esp]`); same value

## 0x44a420 FUN_0044a420 (src/unk_445e80.cpp)

`0x44a420 src/unk_445e80.cpp: events 16/17, 5 differ, unreliable`

- - | 0x69f | NOISE | 804*G: original spills it before the string call (0x44aa91 fstp [esp+0x20]) and pushes as int; built fstp [esp] after the call
- - | 0x6d4 | NOISE | 604*G: same spill-before-call vs fstp [esp] pattern (orig 0x44aae8)
- - | 0x754 | NOISE | 200*G: same pattern (orig 0x44ab09.. spilled)
- 0x44ac4e | - | NOISE | scale reset text->scaleX = G: original fld/fstp, built int mov copy
- 0x44ac5d | - | NOISE | scaleY: float vs int copy

## 0x44b510 ServiceMenu::Slot3 (src/unk_445e80.cpp)

`0x44b510 src/unk_445e80.cpp: events 16/16, 3 differ, unreliable`

- 0x44b5b9 | 0xa9 | REAL-FIXED | SMI_TITLEX/TITLEY/RALLYY at 0x4f9f84/88/90 are writable .data in the original (`fld ds:0x4f9f88; fmul ds:0x56aeac`), built folded them as const literals; now plain static float (event identical, match unchanged)
- 0x44b5cb | 0xbb | REAL-FIXED | SMI_TITLEX/TITLEY/RALLYY at 0x4f9f84/88/90 are writable .data in the original (`fld ds:0x4f9f88; fmul ds:0x56aeac`), built folded them as const literals; now plain static float (event identical, match unchanged)
- 0x44b670 | 0x163 | REAL-FIXED | SMI_TITLEX/TITLEY/RALLYY at 0x4f9f84/88/90 are writable .data in the original (`fld ds:0x4f9f88; fmul ds:0x56aeac`), built folded them as const literals; now plain static float (event identical, match unchanged)

## 0x452eb0 Car::FUN_00452eb0 (src/unk_452b20.cpp)

`0x452eb0 src/unk_452b20.cpp: events 66/66, 1 differ`

- 0x452f1c | 0x6c | REAL-FIXED | constant: original `fcomp DWORD PTR ds:0x4e1ae8` = 0.0100000007 (next float above 0.01f), built 0.01f; now extern DAT_004e1ae8 (built `fcomp ds:DAT_004e1ae8`)

## 0x453940 Car::FUN_00453940 (src/unk_452b20.cpp)

`0x453940 src/unk_452b20.cpp: events 74/74, 9 differ, unreliable`

- 0x4547c9 | 0xea5 | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction
- 0x4547cf | 0xeaa | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction
- 0x4547d7 | 0xeb3 | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction
- 0x454815 | 0xeee | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction
- 0x454817 | 0xef0 | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction
- 0x45481a | 0xef3 | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction
- 0x454868 | 0xf41 | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction
- 0x45486a | 0xf43 | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction
- 0x45486d | 0xf46 | NOISE | call-result naming: original 0x414330 writes the vector into [ebp-0x30] via its out pointer and is read as L*, built copies *eax into the same locals (R0); normalise sum (x^2+z^2)+y^2 and the cross products are identical instruction for instruction

## 0x4398e0 FUN_004398e0 (src/unk_439170.cpp)

`0x4398e0 src/unk_439170.cpp: events 14/14, 8 differ, unreliable`

- 0x439dac | 0x4cd | REAL-FIXED | B2_titleX/titleY/COLW live in .rdata (0x4e1714..) in the original (read-only constant), built had them as writable statics (G); now static const - still `fld const; fmul scale`, match unchanged
- 0x439dbe | 0x4df | REAL-FIXED | B2_titleX/titleY/COLW live in .rdata (0x4e1714..) in the original (read-only constant), built had them as writable statics (G); now static const - still `fld const; fmul scale`, match unchanged
- 0x439ecb | 0x5ec | REAL-FIXED | B2_titleX/titleY/COLW live in .rdata (0x4e1714..) in the original (read-only constant), built had them as writable statics (G); now static const - still `fld const; fmul scale`, match unchanged
- 0x439ef9 | 0x61a | REAL-FIXED | B2_titleX/titleY/COLW live in .rdata (0x4e1714..) in the original (read-only constant), built had them as writable statics (G); now static const - still `fld const; fmul scale`, match unchanged
- 0x439f2b | 0x64c | REAL-FIXED | B2_titleX/titleY/COLW live in .rdata (0x4e1714..) in the original (read-only constant), built had them as writable statics (G); now static const - still `fld const; fmul scale`, match unchanged
- 0x439f3b | 0x65c | REAL-FIXED | B2_titleX/titleY/COLW live in .rdata (0x4e1714..) in the original (read-only constant), built had them as writable statics (G); now static const - still `fld const; fmul scale`, match unchanged
- 0x439f5d | 0x67e | REAL-FIXED | B2_titleX/titleY/COLW live in .rdata (0x4e1714..) in the original (read-only constant), built had them as writable statics (G); now static const - still `fld const; fmul scale`, match unchanged
- 0x439f61 | 0x682 | REAL-FIXED | B2_titleX/titleY/COLW live in .rdata (0x4e1714..) in the original (read-only constant), built had them as writable statics (G); now static const - still `fld const; fmul scale`, match unchanged

## 0x427c10 FUN_00427c10 (src/unk_427600.cpp)

`0x427c10 src/unk_427600.cpp: events 20/20, 6 differ, unreliable`

- 0x4284e5 | 0x8d7 | REAL-FIXED | H2b_firstY/step/backY/toggleW/listW at 0x4e155c..74 are .rdata in the original (`fld ds:0x4e1570; fmul [esp+0x14]` = 64), built had writable statics (G); now static const, match unchanged
- 0x428526 | 0x917 | REAL-FIXED | H2b_firstY/step/backY/toggleW/listW at 0x4e155c..74 are .rdata in the original (`fld ds:0x4e1570; fmul [esp+0x14]` = 64), built had writable statics (G); now static const, match unchanged
- 0x428550 | 0x942 | REAL-FIXED | H2b_firstY/step/backY/toggleW/listW at 0x4e155c..74 are .rdata in the original (`fld ds:0x4e1570; fmul [esp+0x14]` = 64), built had writable statics (G); now static const, match unchanged
- 0x4285e2 | 0x9d4 | REAL-FIXED | H2b_firstY/step/backY/toggleW/listW at 0x4e155c..74 are .rdata in the original (`fld ds:0x4e1570; fmul [esp+0x14]` = 64), built had writable statics (G); now static const, match unchanged
- 0x4285f8 | 0x9ea | REAL-FIXED | H2b_firstY/step/backY/toggleW/listW at 0x4e155c..74 are .rdata in the original (`fld ds:0x4e1570; fmul [esp+0x14]` = 64), built had writable statics (G); now static const, match unchanged
- 0x428608 | 0x9fa | REAL-FIXED | H2b_firstY/step/backY/toggleW/listW at 0x4e155c..74 are .rdata in the original (`fld ds:0x4e1570; fmul [esp+0x14]` = 64), built had writable statics (G); now static const, match unchanged

## 0x4021b0 FUN_004021b0 (src/unk_4018f0.cpp)

`0x4021b0 src/unk_4018f0.cpp: events 18/16, 5 differ, unreliable`

- 0x402279 | - | NOISE | high dword of the double arg: original `fstp QWORD [ebp-0x20]` overlaps the int slot [ebp-0x1c] written by fistp, tool reads it as fist(R0); the arg is the double phi*int(M[this+4]) (orig 0x40227a = built 0xca)
- 0x4022a6 | - | NOISE | same overlapping-slot high dword (counterpart built 0xf7)
- 0x4024b4 | 0x2fd | NOISE | only difference is the sqrt table base: original M[..*4+0x573bc0], built the unresolved DAT_00573bc0 relocation (same symbol); products identical
- 0x4024c3 | 0x30d | NOISE | only difference is the sqrt table base: original M[..*4+0x573bc0], built the unresolved DAT_00573bc0 relocation (same symbol); products identical
- 0x4024cc | 0x317 | NOISE | only difference is the sqrt table base: original M[..*4+0x573bc0], built the unresolved DAT_00573bc0 relocation (same symbol); products identical

## 0x4921d0 FUN_004921d0 (src/unk_491fb0.cpp)

`0x4921d0 src/unk_491fb0.cpp: events 2/1, 2 differ, unreliable`

- 0x4925b4 | 0x522 | NOISE | view->farPlane = DAT_005012f8: original fld/fstp float copy, built `mov [esi+0x128],edx` int copy (built 0x3f4); paired by the tool with built 0x522 which is the true counterpart of orig 0x4926f2
- 0x4926f2 | - | NOISE | identical `fild [esp+..]; fdiv [esp+0x18]; fstp [esp]` (built 0x522); trees differ only from the unreliable esp walk (frame 0x74 vs 0x64)

## 0x493650 FUN_00493650 (src/unk_491fb0.cpp)

`0x493650 src/unk_491fb0.cpp: events 7/4, 3 differ, unreliable`

- 0x49409d | - | NOISE | dtSec = total*0.001f stored to a float slot and pushed three times: original slot [ebp-0x30] is resolved, built slot [ebp-0x40] also holds an int temp earlier so the walk loses it; built pushes at 0xa3c/0xac8/0xb01 are the same args
- 0x494129 | - | NOISE | dtSec = total*0.001f stored to a float slot and pushed three times: original slot [ebp-0x30] is resolved, built slot [ebp-0x40] also holds an int temp earlier so the walk loses it; built pushes at 0xa3c/0xac8/0xb01 are the same args
- 0x49415c | - | NOISE | dtSec = total*0.001f stored to a float slot and pushed three times: original slot [ebp-0x30] is resolved, built slot [ebp-0x40] also holds an int temp earlier so the walk loses it; built pushes at 0xa3c/0xac8/0xb01 are the same args

## 0x443790 TrackSelectMenu::Run (src/unk_441370.cpp)

`0x443790 src/unk_441370.cpp: events 18/18, 3 differ, unreliable`

- 0x4445c2 | 0xdff | NOISE | list row y/x: same `fld K; fmul sx/sy` and push sequence; loop-carried slots (orig [esp+0x14]/[esp+0x34], built [esp+0x14]/[esp+0x38]) resolved on opposite sides by the linear walk (phi)
- 0x4445c3 | 0xe00 | NOISE | list row y/x: same `fld K; fmul sx/sy` and push sequence; loop-carried slots (orig [esp+0x14]/[esp+0x34], built [esp+0x14]/[esp+0x38]) resolved on opposite sides by the linear walk (phi)
- 0x444ae9 | 0x131f | NOISE | 576*sx: orig `fmul [esp+0x5c]` after 6 pushes = slot 0x44 = sx, built the same; phi vs L0 naming

## 0x467440 FUN_00467440 (src/unk_4666a0.cpp)

`0x467440 src/unk_4666a0.cpp: events 33/31, 3 differ, unreliable`

- 0x467d31 | - | NOISE | surface sum store fstp [ebp-0x18] right after a push: the unreliable esp walk takes it for an arg slot; built stores the same sums to [ebp-0x1c]/[ebp-0x14] (same fdivr 1/n scaling)
- 0x467d53 | - | NOISE | as 0x467d31
- 0x4680a0 | 0xc5d | REAL-FIXED | constant: original `fmul QWORD PTR ds:0x4e1ea0` = 0.6366197546520227, built 0.63662; now that literal (double 0x3fe45f306446f9b4)

## 0x4b95b0 FUN_004b95b0 (src/unk_4b8b10.cpp)

`0x4b95b0 src/unk_4b8b10.cpp: events 11/11, 3 differ`

- 0x4b9741 | 0x18b | NOISE | only the sqrt-table relocation differs (orig +0x573bc0 vs unresolved DAT_00573bc0); sums and products identical
- 0x4b974b | 0x195 | NOISE | only the sqrt-table relocation differs (orig +0x573bc0 vs unresolved DAT_00573bc0); sums and products identical
- 0x4b9755 | 0x19f | NOISE | only the sqrt-table relocation differs (orig +0x573bc0 vs unresolved DAT_00573bc0); sums and products identical

## 0x4799f0 CarEmitterB::SpawnCore1 (src/unk_479170.cpp)

`0x4799f0 src/unk_479170.cpp: events 30/30, 1 differ`

- 0x479e56 | 0x446 | REAL-FIXED (a) + NOISE (b, 24-bit precision control) | two parts. (a) field read FIXED: original `fld [ebp-0x20]; fadd DWORD PTR [ebx]` re-reads velocity.x, built added the value cached by the `*vel -=` fst ([ebp-0x20]); now `j1 + *(volatile float*)&vel->x` -> built 0x440 `fadd DWORD PTR [ebx]`. (b) rounding point NOT fixed: original spills j1 to float (`fstp [ebp-0x20]`) across the 2nd rand() and adds after it; VC6 sinks the 2nd rand() below the x store here, so j1 stays 80-bit (volatile local / volatile cast reads tried, forwarded). Only matters if the FPU is not in 24-bit precision

## 0x479f60 CarEmitterB::SpawnCore2 (src/unk_479170.cpp)

`0x479f60 src/unk_479170.cpp: events 30/30, 1 differ`

- 0x47a3c0 | 0x442 | NOISE (24-bit precision control) | rounding point NOT fixed: original `fstp [ebp-0x20]` (j1 to float) then 2nd rand(), `fld [ebp-0x20]; fadd [ebx]`; built `fst [ebp+0xc]; fadd [ebx]` keeps j1 80-bit because VC6 sinks the 2nd rand() below the x store (same cause as SpawnCore1); field read already matches. Reclassified by the orchestrator: FUN_004a7d30 sets 24-bit precision (analysis/fpu-control.md), so j1 held on the stack equals j1 stored to a float.

## 0x40fcc0 FUN_0040fcc0 (src/unk_40fcc0.cpp)

`0x40fcc0 src/unk_40fcc0.cpp: events 3/3, 1 differ, unreliable`

- 0x40ff8c | 0x2d0 | NOISE | identical `lea esi,[ecx+1]; fild; fidiv; fmul G; fadd G; fstp [esp]`; loop counter slot [ebp-0x10] vs [ebp-0x18] named phi vs L0

## 0x4658a0 FUN_004658a0 (src/unk_4654f0.cpp)

`0x4658a0 src/unk_4654f0.cpp: events 13/12, 1 differ`

- 0x465bcb | - | NOISE | wheel friction = stage->friction[idx]: original fld/fstp float copy, built `mov edx,[ecx+eax*4+0x47c]; mov [edi+0x30c],edx` int copy (bit-identical)

## Summary

| addr | function | events | REAL-FIXED | REAL not fixed | NOISE |
|---|---|---|---|---|---|
| 0x417540 | Stage::FUN_00417540 (src/unk_417540.cpp) | 55 | 6 | 0 | 49 |
| 0x41a890 | FUN_0041a890 (src/unk_41a890.cpp) | 31 | 0 | 0 | 31 |
| 0x46c790 | Wheel::UpdateForces (src/unk_46c2c0.cpp) | 24 | 2 | 0 | 22 |
| 0x4bb3a0 | TraceNodeSweep (src/unk_4bb3a0.cpp) | 16 | 10 | 0 | 6 |
| 0x449860 | TuneMenu draw FUN_00449860 (src/unk_445e80.cpp) | 3 | 0 | 0 | 3 |
| 0x44a420 | FUN_0044a420 (src/unk_445e80.cpp) | 5 | 0 | 0 | 5 |
| 0x44b510 | ServiceMenu::Slot3 (src/unk_445e80.cpp) | 3 | 3 | 0 | 0 |
| 0x452eb0 | Car::FUN_00452eb0 (src/unk_452b20.cpp) | 1 | 1 | 0 | 0 |
| 0x453940 | Car::FUN_00453940 (src/unk_452b20.cpp) | 9 | 0 | 0 | 9 |
| 0x4398e0 | FUN_004398e0 (src/unk_439170.cpp) | 8 | 8 | 0 | 0 |
| 0x427c10 | FUN_00427c10 (src/unk_427600.cpp) | 6 | 6 | 0 | 0 |
| 0x4021b0 | FUN_004021b0 (src/unk_4018f0.cpp) | 5 | 0 | 0 | 5 |
| 0x4921d0 | FUN_004921d0 (src/unk_491fb0.cpp) | 2 | 0 | 0 | 2 |
| 0x493650 | FUN_00493650 (src/unk_491fb0.cpp) | 3 | 0 | 0 | 3 |
| 0x443790 | TrackSelectMenu::Run (src/unk_441370.cpp) | 3 | 0 | 0 | 3 |
| 0x467440 | FUN_00467440 (src/unk_4666a0.cpp) | 3 | 1 | 0 | 2 |
| 0x4b95b0 | FUN_004b95b0 (src/unk_4b8b10.cpp) | 3 | 0 | 0 | 3 |
| 0x4799f0 | CarEmitterB::SpawnCore1 (src/unk_479170.cpp) | 1 | 0 | 1 | 0 |
| 0x479f60 | CarEmitterB::SpawnCore2 (src/unk_479170.cpp) | 1 | 0 | 1 | 0 |
| 0x40fcc0 | FUN_0040fcc0 (src/unk_40fcc0.cpp) | 1 | 0 | 0 | 1 |
| 0x4658a0 | FUN_004658a0 (src/unk_4654f0.cpp) | 1 | 0 | 0 | 1 |
