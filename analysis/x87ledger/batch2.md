# x87 behaviour ledger, batch 2

One line per differing event of `tools/x87check.py --sym --list` (run before the fixes): `orig_addr | built_off | class | reason`. REAL-FIXED lines name the original and built instructions; "REAL (not fixed)" marks a difference that could not be reproduced.

## 0x417540 Stage::FUN_00417540 (src/unk_417540.cpp)

`0x417540 src/unk_417540.cpp: events 50/57, 55 differ, unreliable`

- 0x4176a0 | 0x162 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x4176ad | 0x16f | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x4176ba | 0x17c | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417767 | 0x24c | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417787 | 0x260 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x4177b4 | 0x279 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x4177eb | 0x2b7 | REAL-FIXED | DIR.Length() sum: original `fld [eax+8]; fld [eax+4]; fld [eax]; fld st0; fmul st,st1; fld st2; fmul st,st3; faddp; fld st3; fmul st,st4; faddp` = (x*x + y*y) + z*z, built `fld [eax]; fld [eax+4]; fld [eax+8]` then (z*z + y*y) + x*x; AVec::Length now chains s = a*a; s = b*b + s; s = c*c + s (built 0x283 fld [eax+8] / 0x292 fld [eax], same fadd order as original)
- 0x4177f2 | 0x2be | REAL-FIXED | DIR.Length() sum: original `fld [eax+8]; fld [eax+4]; fld [eax]; fld st0; fmul st,st1; fld st2; fmul st,st3; faddp; fld st3; fmul st,st4; faddp` = (x*x + y*y) + z*z, built `fld [eax]; fld [eax+4]; fld [eax+8]` then (z*z + y*y) + x*x; AVec::Length now chains s = a*a; s = b*b + s; s = c*c + s (built 0x283 fld [eax+8] / 0x292 fld [eax], same fadd order as original)
- 0x4177f8 | 0x2c4 | REAL-FIXED | DIR.Length() sum: original `fld [eax+8]; fld [eax+4]; fld [eax]; fld st0; fmul st,st1; fld st2; fmul st,st3; faddp; fld st3; fmul st,st4; faddp` = (x*x + y*y) + z*z, built `fld [eax]; fld [eax+4]; fld [eax+8]` then (z*z + y*y) + x*x; AVec::Length now chains s = a*a; s = b*b + s; s = c*c + s (built 0x283 fld [eax+8] / 0x292 fld [eax], same fadd order as original)
- 0x417834 | 0x2fa | REAL-FIXED | DIR.Length() sum: original `fld [eax+8]; fld [eax+4]; fld [eax]; fld st0; fmul st,st1; fld st2; fmul st,st3; faddp; fld st3; fmul st,st4; faddp` = (x*x + y*y) + z*z, built `fld [eax]; fld [eax+4]; fld [eax+8]` then (z*z + y*y) + x*x; AVec::Length now chains s = a*a; s = b*b + s; s = c*c + s (built 0x283 fld [eax+8] / 0x292 fld [eax], same fadd order as original)
- 0x417836 | 0x2fc | REAL-FIXED | DIR.Length() sum: original `fld [eax+8]; fld [eax+4]; fld [eax]; fld st0; fmul st,st1; fld st2; fmul st,st3; faddp; fld st3; fmul st,st4; faddp` = (x*x + y*y) + z*z, built `fld [eax]; fld [eax+4]; fld [eax+8]` then (z*z + y*y) + x*x; AVec::Length now chains s = a*a; s = b*b + s; s = c*c + s (built 0x283 fld [eax+8] / 0x292 fld [eax], same fadd order as original)
- 0x417839 | 0x2ff | REAL-FIXED | DIR.Length() sum: original `fld [eax+8]; fld [eax+4]; fld [eax]; fld st0; fmul st,st1; fld st2; fmul st,st3; faddp; fld st3; fmul st,st4; faddp` = (x*x + y*y) + z*z, built `fld [eax]; fld [eax+4]; fld [eax+8]` then (z*z + y*y) + x*x; AVec::Length now chains s = a*a; s = b*b + s; s = c*c + s (built 0x283 fld [eax+8] / 0x292 fld [eax], same fadd order as original)
- 0x4178ec | 0x3ac | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x4178f9 | 0x3bc | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417982 | 0x441 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417988 | 0x450 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417996 | 0x45e | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x4179a8 | - | NOISE | pairing shift: true counterpart is built 0x47b (identical cmp seg.len ? 0x139c + t*0.001*dt)
- 0x417a07 | 0x47b | NOISE | pairing shift: POS.x = r[3]-r[0]; true counterpart built 0x4c9 (orig base popped+0x1378 == built ebx0, same field)
- 0x417a16 | 0x4c9 | NOISE | pairing shift: POS.y sub; counterpart built 0x4d1
- 0x417a25 | 0x4d1 | NOISE | pairing shift: POS.z sub; counterpart built 0x4da
- 0x417a33 | 0x4da | NOISE | pairing shift: POS.x *= t/len; counterpart built 0x4e1, same tree
- 0x417a41 | 0x4e1 | NOISE | pairing shift: POS.y *= t/len; counterpart built 0x4e8
- 0x417a4d | 0x4e8 | NOISE | pairing shift: POS.z *= t/len; counterpart built 0x4ee
- 0x417a5b | 0x4ee | NOISE | pairing shift: POS.x += r[0]; counterpart built 0x4f5
- 0x417a6a | 0x4f5 | NOISE | pairing shift: POS.y += r[1]; counterpart built 0x4fd
- 0x417a79 | 0x4fd | NOISE | pairing shift: POS.z += r[2]; counterpart built 0x506
- 0x417b2f | 0x506 | NOISE | pairing shift: box lo.x = x-3; counterpart built 0x5ac
- 0x417b3b | 0x5ac | NOISE | pairing shift: box lo.y = y-(25+25); counterpart built 0x5be
- 0x417b41 | 0x5be | NOISE | pairing shift: box hi.x = 3+x; counterpart built 0x5c4
- 0x417b4a | 0x5c4 | NOISE | pairing shift: box lo.z = z-3; counterpart built 0x5cd
- 0x417b51 | 0x5cd | NOISE | pairing shift: box hi.y = 50+y; counterpart built 0x5d4
- 0x417b57 | 0x5d4 | NOISE | pairing shift: box hi.z = 3+z; counterpart built 0x5e1
- - | 0x5e1 | NOISE | pairing shift: counterpart of orig 0x417b57
- - | 0x70a | NOISE | built pushes F(0x13a0) for SetColour as a float load; original pushes it as an int copy (no event)
- 0x417c94 | 0x738 | NOISE | h1 < 1: original names the Cast() result R0 directly, built reads it back from its local (L0)
- 0x417cc7 | 0x767 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417cd5 | 0x76f | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417cde | 0x778 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417d0b | 0x7a2 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417d1a | 0x7a9 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417d29 | 0x7af | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417d68 | 0x801 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417d6a | 0x803 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417d6d | 0x806 | NOISE | same operands and association field by field (checked with a field-offset normaliser); trees differ only in address-expression form of the record pointer (add(M[0x13ac],..) vs M[0x13ac]+..) and L/R/phi naming from the linear walk
- 0x417d99 | 0x80b | NOISE | pairing: obj+0x18 = 0.5*(h..) has its counterpart at built 0x82e; built 0x80b is a float copy the original does as int mov
- 0x417e26 | 0x815 | NOISE | else-branch cross(ov, o2): original operands forwarded through the int copies of DIR/0x1384 into obj+0x40/obj+0x20, built reads obj+0x44/0x48 directly - same values (counterpart built 0x8ab, same products and order)
- 0x417e28 | 0x81f | NOISE | as 0x417e26 (counterpart built 0x8ad)
- 0x417e2b | 0x82e | NOISE | as 0x417e26 (counterpart built 0x8b0)
- - | 0x8ab | NOISE | counterpart of orig 0x417e26
- - | 0x8ad | NOISE | counterpart of orig 0x417e28
- - | 0x8b0 | NOISE | counterpart of orig 0x417e2b
- - | 0x8b5 | NOISE | built float copy obj.pos = POS; original copies with int mov (no event)
- - | 0x8be | NOISE | built float copy; original int mov
- - | 0x8c7 | NOISE | built float copy; original int mov

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

- 0x46cc7f | 0x4ce | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46cc9d | 0x4ee | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46ccb9 | 0x505 | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46cd0a | 0x554 | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46cd10 | 0x55a | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46cd2b | 0x575 | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46cd6d | 0x5af | NOISE | store order of independent int copies: original 0x300, 0x2f0, 0x304; built 0x304, 0x300, 0x2f0 (true pairs 0x46cd6d<->0x5bb, 0x46cd73<->0x5af)
- 0x46cd73 | 0x5bb | NOISE | see 0x46cd6d
- 0x46cd99 | 0x5e4 | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46cda2 | 0x5ed | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46cdd3 | 0x61a | REAL-FIXED | constant: original `fadd QWORD PTR ds:0x4e1f28` = 0.0099999997764825821 ((double)0.01f), built used 0.01; now `fabs(lonVel) + 0.0099999997764825821` (built fadd QWORD of that literal before 0x61a); remaining diff is slot naming
- 0x46ceb1 | 0x6f7 | NOISE | comb = sqrt(latSlip^2 + slipRatioN^2): both read spilled float slots (orig [ebp-0x14],[ebp-0x20]; built [ebp-0x14],[ebp-0x44]); tool forwards one side only
- 0x46ceb7 | 0x700 | NOISE | as 0x46ceb1 (int copy of the same fstp/fld value)
- 0x46cf14 | 0x761 | NOISE | as 0x46ceb1
- 0x46d060 | 0x896 | NOISE | unk314 = ((unk328*unk318)*load)*peakLon on both sides (`fld [esi+0x328]; fmul [esi+0x318]; fmul load; fmul peakLon-slot`); orig peakLon slot [ebp-0x64] unresolved, built [ebp-0x2c] forwarded
- 0x46d0eb | 0x91f | NOISE | fabs(slipSpeed0*G) < peakLon*9.81: same; original slots unresolved (L0), built forwarded
- - | 0x96f | NOISE | built push of slipSpeed0 ([ebp-0x3c]) for the scalar*vec call is resolved; original pushes [ebp-0x80] as an int (no event)
- 0x46d443 | 0xc5f | NOISE | equal after renaming L/phi slots only (field-offset normaliser EQ): lon/lat components come from branch-dependent slots the linear walk names differently
- 0x46d4be | 0xcc0 | NOISE | body->torque += cross(r, F): original adds via the returned pointer (`fld [edi+0x1f0]; fadd [eax]`), built via the out temp it passed (`fld [ebp-0xd8]; fadd [edi+0x1f0]`); true counterpart built 0xcec. Built 0xcc0 is the r.y temp store the tool takes for an arg
- 0x46d4cd | 0xcd2 | NOISE | as 0x46d4be; counterpart built 0xcfe (built 0xcd2 = r.z temp store)
- 0x46d4dc | 0xcec | NOISE | as 0x46d4be; counterpart built 0xd10
- - | 0xcfe | NOISE | counterpart of orig 0x46d4cd
- - | 0xd10 | NOISE | counterpart of orig 0x46d4dc
- 0x46d578 | 0xdac | REAL-FIXED | constants: original `fmul ds:0x4e1f1c` = 0.98100006580352783f (0x3f7b22d2) and `fadd ds:0x4e1f18` = 0.0098100006580352783f (0x3c20ba20), built 0.981f/0.00981f (1 ulp lower); now the exact literals

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
