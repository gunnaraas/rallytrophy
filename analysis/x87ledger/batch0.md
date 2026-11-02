# x87 behaviour ledger, batch 0

Events from `x87check.py --sym --list` before any fix. Format: `orig_addr | built_off | REAL-FIXED / NOISE | reason`.

## 0x41d7d0 src/unk_41d7d0.cpp: events 76/58, 80 differ, unreliable

0x41e0d9 | 0x8c9 | NOISE | dot1 (dv . A->n, z,y,x) same association both sides; orig names v1/v2 by the fields (this/other+0x1cc..) because the int copies forward, built as locals - int vs float copy naming (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e108 | 0x8ee | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e117 | 0x909 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e123 | 0x924 | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e135 | 0x97b | REAL-FIXED | built side is a dot2 event (pre-fix x,y,z association), true counterpart is the orig dot2 of the same RECORD; fixed with DOT2_ZYX/DOT2_XZY (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e13b | 0x9a4 | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e14b | 0x9be | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e1af | 0x9dd | REAL-FIXED | dot2 association (shA-edge RECORD): orig (z*n2 + y*n1) + x*n0 [41e18b fld [ebp-0xac]; fmul [eax] / fld [ebp-0xb0]; fmul [eax-0x4]; faddp / fld [ebp-0xb4]; fmul [eax-0x8]; faddp], built was (x + y) + z [957 fld [ebp-0x198]; fmul [eax] / fld [ebp-0x194]; fmul [eax+0x4] / fld [ebp-0x190]; fmul [eax+0x8]]; now chained DOT2_ZYX, built 957 fld [ebp-0x190]; fmul [eax+0x8] / fld [ebp-0x194]; fmul [eax+0x4]; faddp / fld [ebp-0x198]; fmul [eax]; faddp (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e1f3 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e214 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e227 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e260 | - | NOISE | Edge() result compare against DAT_004e1348, identical; unpaired only by the event shift (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e355 | - | NOISE | dot1 (dv . A->n, z,y,x) same association both sides; orig names v1/v2 by the fields (this/other+0x1cc..) because the int copies forward, built as locals - int vs float copy naming (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e384 | - | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e393 | - | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e39f | - | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e3b1 | - | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e3b7 | - | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e3c7 | - | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e42b | - | REAL-FIXED | dot2 association (shA-edge RECORD): orig (z*n2 + y*n1) + x*n0 [41e18b fld [ebp-0xac]; fmul [eax] / fld [ebp-0xb0]; fmul [eax-0x4]; faddp / fld [ebp-0xb4]; fmul [eax-0x8]; faddp], built was (x + y) + z [957 fld [ebp-0x198]; fmul [eax] / fld [ebp-0x194]; fmul [eax+0x4] / fld [ebp-0x190]; fmul [eax+0x8]]; now chained DOT2_ZYX, built 957 fld [ebp-0x190]; fmul [eax+0x8] / fld [ebp-0x194]; fmul [eax+0x4]; faddp / fld [ebp-0x198]; fmul [eax]; faddp (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e46f | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e490 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e4a3 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e4dc | - | NOISE | Edge() result compare against DAT_004e1348, identical; unpaired only by the event shift (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e5d1 | - | NOISE | dot1 (dv . A->n, z,y,x) same association both sides; orig names v1/v2 by the fields (this/other+0x1cc..) because the int copies forward, built as locals - int vs float copy naming (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e600 | - | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e60f | - | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e61b | - | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e62d | - | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e633 | - | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e643 | - | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e6a7 | - | REAL-FIXED | dot2 association (shA-edge RECORD): orig (z*n2 + y*n1) + x*n0 [41e18b fld [ebp-0xac]; fmul [eax] / fld [ebp-0xb0]; fmul [eax-0x4]; faddp / fld [ebp-0xb4]; fmul [eax-0x8]; faddp], built was (x + y) + z [957 fld [ebp-0x198]; fmul [eax] / fld [ebp-0x194]; fmul [eax+0x4] / fld [ebp-0x190]; fmul [eax+0x8]]; now chained DOT2_ZYX, built 957 fld [ebp-0x190]; fmul [eax+0x8] / fld [ebp-0x194]; fmul [eax+0x4]; faddp / fld [ebp-0x198]; fmul [eax]; faddp (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e6eb | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e70c | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e71f | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e8b7 | 0xb43 | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e8c9 | 0xb5e | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e8d5 | 0xb79 | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e8ea | 0xbd0 | REAL-FIXED | built side is a dot2 event (pre-fix x,y,z association), true counterpart is the orig dot2 of the same RECORD; fixed with DOT2_ZYX/DOT2_XZY (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e8f0 | 0xbf9 | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e900 | 0xc13 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e964 | 0xc32 | REAL-FIXED | dot2 association (shB-edge RECORD): orig (x*n0 + z*n2) + y*n1 [41e928 fld [ebp-0x90]; fmul [ecx-0x8] / fld [ebp-0x88]; fmul [eax]; faddp / fld [ebp-0x8c]; fmul [eax-0x4]; faddp], built was (x + y) + z; now chained DOT2_XZY (x, then +z, then +y), built tree equals the original (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e9a7 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e9c8 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41e9db | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eb48 | 0xd98 | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eb54 | 0xdb3 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eb60 | 0xdce | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eb6f | 0xe25 | REAL-FIXED | built side is a dot2 event (pre-fix x,y,z association), true counterpart is the orig dot2 of the same RECORD; fixed with DOT2_ZYX/DOT2_XZY (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eb75 | 0xe4e | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eb85 | 0xe68 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41ebe3 | 0xe87 | REAL-FIXED | dot2 association (shB-edge RECORD): orig (x*n0 + z*n2) + y*n1 [41e928 fld [ebp-0x90]; fmul [ecx-0x8] / fld [ebp-0x88]; fmul [eax]; faddp / fld [ebp-0x8c]; fmul [eax-0x4]; faddp], built was (x + y) + z; now chained DOT2_XZY (x, then +z, then +y), built tree equals the original (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41ec23 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41ec41 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41ec54 | - | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41edc7 | 0x101c | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41edd6 | 0x1037 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41ede2 | 0x1052 | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41edf4 | 0x10a9 | REAL-FIXED | built side is a dot2 event (pre-fix x,y,z association), true counterpart is the orig dot2 of the same RECORD; fixed with DOT2_ZYX/DOT2_XZY (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41edfa | 0x10d2 | NOISE | dead intermediate store: orig writes c1->v = dv (41e108 mov [ebx+0x24],edx) then fmul [ebx+0x24]; built multiplies the float local dv (8d5 fld [ebp-0x138]; fmul [ebp-0x18]) - dv already rounded to float in both, same final value (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41ee0a | 0x10ec | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41ee74 | 0x110b | REAL-FIXED | dot2 association (shB-edge RECORD): orig (x*n0 + z*n2) + y*n1 [41e928 fld [ebp-0x90]; fmul [ecx-0x8] / fld [ebp-0x88]; fmul [eax]; faddp / fld [ebp-0x8c]; fmul [eax-0x4]; faddp], built was (x + y) + z; now chained DOT2_XZY (x, then +z, then +y), built tree equals the original (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eeb7 | 0x115c | NOISE | Edge() result compare against DAT_004e1348, identical; unpaired only by the event shift (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eed8 | 0x125c | NOISE | dot1 (dv . A->n, z,y,x) same association both sides; orig names v1/v2 by the fields (this/other+0x1cc..) because the int copies forward, built as locals - int vs float copy naming (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
0x41eeeb | 0x1281 | NOISE | c2->v = f2*nd: orig reads nd through the operator- result copied into dv (41e171..), built from the local; same product (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x129c | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x12b7 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x130e | REAL-FIXED | built side is a dot2 event (pre-fix x,y,z association), true counterpart is the orig dot2 of the same RECORD; fixed with DOT2_ZYX/DOT2_XZY (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x1337 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x1351 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x1370 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x13c1 | NOISE | Edge() result compare against DAT_004e1348, identical; unpaired only by the event shift (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x14c1 | NOISE | dot1 (dv . A->n, z,y,x) same association both sides; orig names v1/v2 by the fields (this/other+0x1cc..) because the int copies forward, built as locals - int vs float copy naming (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x14e6 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x1501 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x151c | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x1573 | REAL-FIXED | built side is a dot2 event (pre-fix x,y,z association), true counterpart is the orig dot2 of the same RECORD; fixed with DOT2_ZYX/DOT2_XZY (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x159c | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x15b6 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)
- | 0x15d5 | NOISE | c->v = f*dv product, same float operands and rounding (dv stored to float before the multiply both sides); operand naming differs (fields vs locals) (pairing mismatch inside the long replace block: the build emits 18 fewer events, see the dead-store lines)

## 0x470940 src/unk_46fdf0.cpp: events 39/39, 7 differ, unreliable

0x470aab | 0x16b | NOISE | same association x+(z+y) (chained x,z,y as source); orig int-copies camera->position into c (tool: local L), built float-copies (tool forwards the field) - int vs float copy
0x470ba9 | 0x269 | NOISE | same association; orig c fields int-copied (local L) vs built float copy forwarded to camera->position - int vs float copy
0x470cc0 | 0x39b | NOISE | same association; orig c fields int-copied (local L) vs built float copy forwarded to camera->position - int vs float copy
0x470de9 | 0x4a2 | NOISE | identical tree; orig reads half (lo+hi) through the operator+ call result (M[R0]), built from the local copy - call-result naming
0x470deb | 0x4a4 | NOISE | identical tree; orig reads half (lo+hi) through the operator+ call result (M[R0]), built from the local copy - call-result naming
0x470dee | 0x4a7 | NOISE | identical tree; orig reads half (lo+hi) through the operator+ call result (M[R0]), built from the local copy - call-result naming
0x4712ec | 0xa66 | NOISE | same values: orig 4712b6..4712ec d = c - proj into locals, sum z,y,x; built a30..a66 identical chain; r=G+M[..+0xc4] kept in [ebp+0x10] (orig) vs [ebp-0x50] (built) - spill naming; fcompp vs fcomp st(1) same compare

## 0x471ab0 src/unk_46fdf0.cpp: events 22/22, 7 differ, unreliable

0x471ca4 | 0x1f0 | NOISE | same fsub/fmul chain (orig 471c61..471ca4, built 1ad..1f0); tool phi/field naming from the ebp cursor in the linear walk
0x471d31 | 0x28d | NOISE | identical tree; vertex index: orig masks only the first (and eax,0xffff at 471cc4) and zero-extends the rest via xor+mov ax (tool: abs), built ands all three - same index
0x471d55 | 0x2b1 | NOISE | identical tree; vertex index: orig masks only the first (and eax,0xffff at 471cc4) and zero-extends the rest via xor+mov ax (tool: abs), built ands all three - same index
0x471fc5 | 0x506 | NOISE | identical tree; vertex index: orig masks only the first (and eax,0xffff at 471cc4) and zero-extends the rest via xor+mov ax (tool: abs), built ands all three - same index
0x471fe8 | 0x52e | NOISE | identical tree; vertex index: orig masks only the first (and eax,0xffff at 471cc4) and zero-extends the rest via xor+mov ax (tool: abs), built ands all three - same index
0x4720ef | 0x623 | NOISE | identical tree; vertex index: orig masks only the first (and eax,0xffff at 471cc4) and zero-extends the rest via xor+mov ax (tool: abs), built ands all three - same index
0x472112 | 0x646 | NOISE | identical tree; vertex index: orig masks only the first (and eax,0xffff at 471cc4) and zero-extends the rest via xor+mov ax (tool: abs), built ands all three - same index

## 0x4721b0 src/unk_46fdf0.cpp: events 25/25, 12 differ, unreliable

0x472510 | 0x360 | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x47252c | 0x37c | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x47254b | 0x39b | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x472560 | 0x3b0 | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x4725ae | 0x3fe | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x4725c8 | 0x418 | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x4725e9 | 0x439 | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x4725fe | 0x44e | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x47264c | 0x49c | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x472669 | 0x4b9 | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x472687 | 0x4d7 | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)
0x47269c | 0x4ec | NOISE | identical tree; same global table: orig lea eax,[eax*4+0x5096f8] (4724dc), built lea eax,[eax*4+reloc] (32c, unresolved relocation reads 0)

## 0x418b90 src/unk_418b90.cpp: events 20/20, 16 differ, unreliable

0x418d0b | 0x1ae | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418d44 | 0x1cf | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418d5b | 0x1e6 | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418d74 | 0x1ff | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418d8b | 0x216 | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418da4 | 0x22f | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418e58 | 0x2ea | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418e7e | 0x310 | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418e9c | 0x32e | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418eba | 0x34c | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418ed5 | 0x367 | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418eef | 0x381 | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418f07 | 0x399 | NOISE | identical tree; call result named R0 (orig) vs fs:0x0 (built, tool artefact after the SEH frame read)
0x418fd7 | 0x45e | NOISE | same arithmetic: orig 418f97..418fcb (den z,y,x; num y,z,x; d - num + r; / den), built 418..44e identical order; plane pointer from a stack slot (G) vs ebx (phi) - naming
0x418fec | 0x471 | NOISE | same t as 0x418fd7 (second compare); plane pointer naming only
0x4190cd | 0x546 | NOISE | same t as 0x418fd7 stored to the out arg; plane pointer naming only

## 0x42a330 src/unk_4295c0.cpp: events 25/25, 13 differ, unreliable

0x42ac5f | 0x92f | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42acae | 0x97e | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42aceb | 0x9bb | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42acee | 0x9be | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42acfb | 0x9cb | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42ad23 | 0x9f3 | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42ad28 | 0x9f8 | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42ad38 | 0xa08 | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42ad5d | 0xa2d | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42ad62 | 0xa32 | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42ad72 | 0xa42 | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42ada0 | 0xa70 | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant
0x42adb0 | 0xa80 | NOISE | 400 is MS2A8_COLW, a non-const static initialised to 400.0 (orig fld ds:0x4e15a4 = 400.0) and never written - same constant

## 0x4193a0 src/unk_418570.cpp: events 9/9, 9 differ, unreliable

0x4194f1 | 0x12a | NOISE | identical tree; orig walks hits with a pointer cursor ([esp+0x10], add eax,4), built indexes hits[i] (mov edx,[edi+ecx*4]) - same element
0x4194f1 | 0x12a | NOISE | identical tree; orig walks hits with a pointer cursor ([esp+0x10], add eax,4), built indexes hits[i] (mov edx,[edi+ecx*4]) - same element
0x419514 | 0x14d | NOISE | identical tree; orig walks hits with a pointer cursor ([esp+0x10], add eax,4), built indexes hits[i] (mov edx,[edi+ecx*4]) - same element
0x41952f | 0x168 | NOISE | identical tree; orig walks hits with a pointer cursor ([esp+0x10], add eax,4), built indexes hits[i] (mov edx,[edi+ecx*4]) - same element
0x41954d | 0x186 | NOISE | identical tree; orig walks hits with a pointer cursor ([esp+0x10], add eax,4), built indexes hits[i] (mov edx,[edi+ecx*4]) - same element
0x419567 | 0x1a0 | NOISE | identical tree; orig walks hits with a pointer cursor ([esp+0x10], add eax,4), built indexes hits[i] (mov edx,[edi+ecx*4]) - same element
0x419581 | 0x1ba | NOISE | identical tree; orig walks hits with a pointer cursor ([esp+0x10], add eax,4), built indexes hits[i] (mov edx,[edi+ecx*4]) - same element
0x419599 | 0x1d2 | NOISE | identical tree; orig walks hits with a pointer cursor ([esp+0x10], add eax,4), built indexes hits[i] (mov edx,[edi+ecx*4]) - same element
0x4196a2 | 0x2c7 | NOISE | both compare the TriClosest4b result with r*r: orig fcomp [esp+0x34] = rr stored at 4193c4, built fcomp [esp+0x74] = r slot overwritten with r*r at 222..22c - same value, slot naming

## 0x419770 src/unk_418570.cpp: events 12/12, 1 differ, unreliable

0x419bb5 | 0x402 | NOISE | both store s[0] to a_[0]: orig eax=[esp+0xa0] after the pops (419ba6), built [esp+0xb0] before them (3f3) - same arg; sum/normalise order identical

## 0x4b3800 src/unk_4b3050.cpp: events 30/26, 10 differ

0x4b39ba | - | NOISE | e>=0 compare on a tail the original duplicates (4b39ae) and the build tail-merges; the same compare runs on the merged path - branch layout
0x4b3d34 | 0x4ff | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted
0x4b3d42 | 0x50b | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted
0x4b3d4c | 0x520 | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted
0x4b3d6a | 0x52e | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted
0x4b3d77 | 0x537 | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted
0x4b3d85 | 0x541 | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted
0x4b3d93 | - | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted
0x4b3d9c | - | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted
0x4b3da6 | - | NOISE | END: orig keeps the running sum p in locals and stores out each step (4b3d29..4b3da6), built re-reads *out - same float value, out never aliases e1/e2/origin (all callers pass a local cp). The p-local form cleared x87check but grew insdiff 510->630 lines, reverted

## 0x411600 src/unk_411600.cpp: events 106/106, 6 differ, unreliable

0x4136c3 | 0x2091 | NOISE | identical tree; store through a moved pointer: orig this+0x49f4/0x4a1c, built cursor+0x24e4/0x250c - same field
0x4136d8 | 0x20ac | NOISE | identical tree; store through a moved pointer: orig this+0x49f4/0x4a1c, built cursor+0x24e4/0x250c - same field
0x4136fa | 0x20ce | NOISE | identical tree; store through a moved pointer: orig this+0x49f4/0x4a1c, built cursor+0x24e4/0x250c - same field
0x41370f | 0x20e3 | NOISE | identical tree; store through a moved pointer: orig this+0x49f4/0x4a1c, built cursor+0x24e4/0x250c - same field
0x413731 | 0x2105 | NOISE | identical tree; store through a moved pointer: orig this+0x49f4/0x4a1c, built cursor+0x24e4/0x250c - same field
0x413746 | 0x211a | NOISE | identical tree; store through a moved pointer: orig this+0x49f4/0x4a1c, built cursor+0x24e4/0x250c - same field

## 0x459910 src/unk_459890.cpp: events 53/51, 6 differ

0x45a2be | - | NOISE | orig float-copies the clamped value (45a2bb fld [ebp-0x14]; fstp [edi]), built int-copies (9b6 mov [edi],ecx) - int vs float copy
0x45a2c3 | - | NOISE | orig 45a2c0 fld [ebp-0x24]; fstp [edi+4], built 9b8 mov [edi+4],edx - int vs float copy
0x45a373 | 0xa66 | NOISE | identical tree; the body field M[M[this+0x24]+..] is named phi or field depending on branch layout - linear-walk artefact
0x45a38d | 0xa80 | NOISE | identical tree; the body field M[M[this+0x24]+..] is named phi or field depending on branch layout - linear-walk artefact
0x45a472 | 0xb68 | NOISE | identical tree; the body field M[M[this+0x24]+..] is named phi or field depending on branch layout - linear-walk artefact
0x45a48c | 0xb82 | NOISE | identical tree; the body field M[M[this+0x24]+..] is named phi or field depending on branch layout - linear-walk artefact

## 0x4267a0 src/unk_4266c0.cpp: events 9/9, 4 differ, unreliable

0x426ebb | 0x71b | NOISE | 142/54 are H1b_titleY/H1b_titleX, non-const statics holding the orig .rdata values and never written - same constant
0x426ed1 | 0x731 | NOISE | 142/54 are H1b_titleY/H1b_titleX, non-const statics holding the orig .rdata values and never written - same constant
0x426ef5 | 0x755 | NOISE | 142/54 are H1b_titleY/H1b_titleX, non-const statics holding the orig .rdata values and never written - same constant
0x426f05 | 0x765 | NOISE | 142/54 are H1b_titleY/H1b_titleX, non-const statics holding the orig .rdata values and never written - same constant

## 0x477690 src/unk_477370.cpp: events 8/8, 4 differ

0x47788f | 0x1ff | NOISE | identical tree; sin/cos table base: orig absolute 0x561ec0/0x561ec4, built relocation (reads 0/4) - same table
0x4778c4 | 0x234 | NOISE | identical tree; sin/cos table base: orig absolute 0x561ec0/0x561ec4, built relocation (reads 0/4) - same table
0x4778f8 | 0x268 | NOISE | identical tree; sin/cos table base: orig absolute 0x561ec0/0x561ec4, built relocation (reads 0/4) - same table
0x477921 | 0x291 | NOISE | identical tree; sin/cos table base: orig absolute 0x561ec0/0x561ec4, built relocation (reads 0/4) - same table

## 0x47b730 src/unk_47b4c0.cpp: events 5/4, 3 differ

0x47bc19 | 0x4e9 | NOISE | same association; orig reads p->position fields where the build int-copied pt (L), and 0.03/scale is the else-path 1.0 (orig) vs phi (built) - branch layout
0x47bc6d | 0x53d | NOISE | same association; orig reads p->position fields where the build int-copied pt (L), and 0.03/scale is the else-path 1.0 (orig) vs phi (built) - branch layout
0x47bf11 | - | NOISE | DrawQuad depth argument pushed as int (orig push [ebp-0x80] at 47bf0b, built push [ebp-0x60] at 7e9); same depth sum, the tool lost the built slot - push naming

## 0x4150b0 src/unk_414fe0.cpp: events 20/20, 2 differ

- | 0x1b2 | NOISE | identical tree (both keep the two exact-zero products); only its position in the event order differs - branch layout
0x415269 | - | NOISE | identical tree (both keep the two exact-zero products); only its position in the event order differs - branch layout

## 0x46f360 src/unk_46eec0.cpp: events 6/8, 2 differ, unreliable

- | 0x3dd | NOISE | same dot z,y,x (orig 46f728..46f746, built 3b9..3d7); the orig event sits after a jump the linear walk drops (unreliable)
- | 0x3e8 | NOISE | same dot z,y,x (orig 46f728..46f746, built 3b9..3d7); the orig event sits after a jump the linear walk drops (unreliable)

## 0x403ca0 src/unk_403270.cpp: events 1/0, 1 differ

0x403efc | - | NOISE | car->unk1a20 = rec.r.a2c: orig float copy (403ef8 fld [esp+0x70]; fstp [esi+0x1a20]), built int copy (25c mov) - int vs float copy, both float

## 0x415370 src/unk_415370.cpp: events 21/21, 1 differ

0x4156b7 | 0x347 | NOISE | same field: orig fmul [edi+0x7c] (415699), built fmul [esi+0x4] with esi = edi+0x78 cursor (327) - naming

## 0x4751b0 src/unk_474c50.cpp: events 4/4, 1 differ, unreliable

0x4756aa | 0x4fe | REAL-FIXED | constant value: orig unit = box * 1.52590219e-05 (0x37800080 at 0x4e2010, 4754e6 fmul ds:0x4e2010), built used 1/65536 = 1.52587891e-05 (0x37800000); kInvFixed and the unit.x/y/z literals now 1.0f/65535.0f, x87check 0 differ
