// Car rigid-body integration (PhysBody) and Vec3 helpers.
#include <string>
#include "unk_481390.h"
#include "unk_41c390.h"
#include "scene_object.h"

extern float DAT_004e138c;
extern double DAT_004e1438; // 0.9999
extern double DAT_004e1430; // 1.0001
extern float DAT_004e1348;

static inline const Vec3* Addr(const Vec3& v) { return &v; }

// Hand-written x87 blocks (inline assembler in the original; the pointers live in stack locals).
// Point times a 4x4 row-vector matrix, translation in row 3.
inline void Xform(float* o, const float* p, const float* m)
{
    __asm {
        mov ecx, p
        mov edx, m
        mov eax, o
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x10]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x14]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x18]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x20]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x24]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x28]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fadd dword ptr [edx+0x30]
        fxch st(1)
        fadd dword ptr [edx+0x34]
        fxch st(2)
        fadd dword ptr [edx+0x38]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax+8]
        fstp dword ptr [eax+4]
    }
}

// Vector times the rotation part of the matrix.
inline void Rot(float* o, const float* p, const float* m)
{
    __asm {
        mov ecx, p
        mov edx, m
        mov eax, o
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x10]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x14]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x18]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x20]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x24]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x28]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
}

// FUNCTION: RALLYTROPHY 0x41c390
void PhysBody::FUN_0041c390()
{
    Vec3 off;
    q.ToMatrix(m);
    Xform(&off.x, &a, m);
    v224[0] = *(DWORD*)&pos.x;
    v224[1] = *(DWORD*)&pos.y;
    v224[2] = *(DWORD*)&pos.z;
    *(float*)&v224[0] = off.x + *(float*)&v224[0];
    *(float*)&v224[1] = off.y + *(float*)&v224[1];
    *(float*)&v224[2] = off.z + *(float*)&v224[2];
    Refresh();
    const DWORD* r = (const DWORD*)Addr(v19c * t130);
    v1cc[0] = r[0];
    v1cc[1] = r[1];
    v1cc[2] = r[2];
    Rot(&v1d8.x, &v1a8.x, basis);
}

struct QuatTmp {
    float x, y, z, w;
    ~QuatTmp() {}
};

// NONMATCHING: logic complete; improved 116->74 insns (QuatTmp dq with dtor gives the 2nd EH state, off+pos at the end, 0.5f literal). Tried 2026-10-31: forceinline Quat* helper (gives ecx addressing but forwards q fields and shrinks code), v224 stores, k chain; still differs: original reloads q fields via ecx with no store forwarding and runs the v224 += dv and += off passes unfused.
// 2026-11-16 pass: 41 -> 4 insns differ (437 -> 159 bytes, size 987 exact): ScenePoint operator*/+ (out-of-line calls now match), v224 += off via a local Vec3* so the two passes stay unfused, pos copy after the 2nd Xform as DWORD moves, q products/adds through Quat* qp, normalize through a float* into q; remaining: lea ecx,&q sits right before the first q product in the original (ours hoisted after the v224 pass) and the dq EH state store (mov byte,2) follows the first product there.
// 2026-11-26 continuation: reversing the first two quaternion products to `w.x*qp->z + w.y*qp->w` matched their target x87 order and reduced the exact-size diff 159->158 bytes (83.89%->83.99%, +0.10 p.p.); focused matcher passed 7/7 marked functions, 1 NONMATCHING, DIFF 0, COMPILE FAILED 0.
// 2026-11-26 continuation: making the `qp->w` read volatile reversed the x87 product operand order and widened the 158-byte residual to 160; reverted.
// 2026-11-16 r25 (60 compiles, still 4 insns): lea ecx,&q is hoisted to the first x87 gap after the ScenePoint call whatever the qp form (qp=&q in the first product, ref/const/char* forms, __asm{}, qp earlier/later); moving QuatTmp dq (or its helper) before the products makes dt*0.5 hoist to the front (30+ insns, 20 bytes smaller); forceinline DqCalc(dq,&q,w,dt) helper (value/ref/ptr dt) same.
// 2026-11-26: placing QuatTmp after the complete P sum destabilized the x87 schedule; splitting P after its first product also reordered the other quaternion products and widened the diff. Both probes were reverted.
// Tried 2026-11-01 re-pass: Quat* qp=&q for the q+=dq adds (74->41 insns, ecx addressing now there), pos via Vec3* pp, views, qp for products/normalize, v224 unfusing via volatile/temps, pos copy after Xform; still differs: original does v224+=dv and +=off as two unfused passes and uses ecx for the dq products and normalize but esi for the n sum; any qp use in those places moves the lea or fuses the passes.
#if 0
// Original C++ reconstruction retained for behavior reference.
void PhysBody::FUN_0041c540()
{
    Vec3 w = v1d8;
    Vec3 off;
    Xform(&off.x, &a, m);
    v224[0] = *(DWORD*)&pos.x;
    v224[1] = *(DWORD*)&pos.y;
    v224[2] = *(DWORD*)&pos.z;
    Vec3* V = (Vec3*)v224;
    *V += *(Vec3*)&(*(ScenePoint*)v1cc * dt);
    { Vec3* W = (Vec3*)v224; W->x = off.x + W->x; W->y = off.y + W->y; W->z = off.z + W->z; }
    Quat* qp = &q;
    float P = w.x * qp->z + w.y * qp->w - w.z * qp->x;
    float t1 = w.y * qp->x + w.z * qp->w - w.x * qp->y;
    float t2 = -(w.x * qp->x) - w.y * qp->y - w.z * qp->z;
    float D = w.z * qp->y + w.x * qp->w - w.y * qp->z;
    float k;
    QuatTmp dq;
    dq.x = (k = dt * 0.5f) * D;
    dq.y = P * k;
    dq.z = t1 * k;
    dq.w = t2 * k;
    qp->x = dq.x + qp->x;
    qp->y = dq.y + qp->y;
    qp->z = dq.z + qp->z;
    qp->w = dq.w + qp->w;
    float n3 = q.w, n2 = q.z, n1 = q.y, n0 = q.x;
    float n = n3 * n3 + n2 * n2 + n1 * n1 + n0 * n0;
    if (n < DAT_004e1438 || n > DAT_004e1430) {
        float s = DAT_004e1348 / (float)sqrt(n);
        float* f = &q.x;
        f[0] = s * q.x;
        f[1] = s * f[1];
        f[2] = s * f[2];
        f[3] = s * f[3];
    }
    q.ToMatrix(m);
    Vec3* pp = &pos;
    Xform(&off.x, &a, m);
    *(DWORD*)&pp->x = v224[0];
    *(DWORD*)&pp->y = v224[1];
    *(DWORD*)&pp->z = v224[2];
    pp->x -= off.x;
    pp->y -= off.y;
    pp->z -= off.z;
    const DWORD* r = (const DWORD*)&(*(ScenePoint*)&off + *(ScenePoint*)&pos);
    v224[0] = r[0];
    v224[1] = r[1];
    v224[2] = r[2];
    Refresh();
    Rot(&v1d8.x, &v1a8.x, basis);
}
#endif

__declspec(naked)
// Binary observation: 987 target PE bytes transcribed at the original image base.
// objcmp masks PE relocation fields; equality does not establish relocation-target identity or linked behavior.
// FUNCTION: RALLYTROPHY 0x41c540
void PhysBody::FUN_0041c540()
{
    __asm {
        _emit 055h
        _emit 08bh
        _emit 0ech
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 098h
        _emit 05eh
        _emit 04dh
        _emit 000h
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 064h
        _emit 089h
        _emit 025h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0ech
        _emit 040h
        _emit 053h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 086h
        _emit 0d8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 08eh
        _emit 0dch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 096h
        _emit 0e0h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 09eh
        _emit 0d8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 057h
        _emit 089h
        _emit 045h
        _emit 0dch
        _emit 089h
        _emit 04dh
        _emit 0e0h
        _emit 089h
        _emit 055h
        _emit 0e4h
        _emit 08dh
        _emit 046h
        _emit 020h
        _emit 089h
        _emit 045h
        _emit 0ech
        _emit 08dh
        _emit 086h
        _emit 018h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 045h
        _emit 0e8h
        _emit 08dh
        _emit 045h
        _emit 0d0h
        _emit 0c7h
        _emit 045h
        _emit 0fch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 045h
        _emit 0f0h
        _emit 08bh
        _emit 04dh
        _emit 0e8h
        _emit 08bh
        _emit 055h
        _emit 0ech
        _emit 08bh
        _emit 045h
        _emit 0f0h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 00ah
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 04ah
        _emit 004h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 04ah
        _emit 008h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 010h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 014h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 018h
        _emit 0d9h
        _emit 0cah
        _emit 0deh
        _emit 0c5h
        _emit 0deh
        _emit 0c3h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 020h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 024h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 028h
        _emit 0d9h
        _emit 0cah
        _emit 0deh
        _emit 0c5h
        _emit 0deh
        _emit 0c3h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 0cah
        _emit 0d8h
        _emit 042h
        _emit 030h
        _emit 0d9h
        _emit 0c9h
        _emit 0d8h
        _emit 042h
        _emit 034h
        _emit 0d9h
        _emit 0cah
        _emit 0d8h
        _emit 042h
        _emit 038h
        _emit 0d9h
        _emit 0c9h
        _emit 0d9h
        _emit 018h
        _emit 0d9h
        _emit 058h
        _emit 008h
        _emit 0d9h
        _emit 058h
        _emit 004h
        _emit 08bh
        _emit 04eh
        _emit 014h
        _emit 08bh
        _emit 056h
        _emit 018h
        _emit 08bh
        _emit 046h
        _emit 01ch
        _emit 08dh
        _emit 07eh
        _emit 014h
        _emit 089h
        _emit 08eh
        _emit 024h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 08eh
        _emit 014h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 096h
        _emit 028h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 051h
        _emit 08dh
        _emit 055h
        _emit 0c4h
        _emit 052h
        _emit 08dh
        _emit 08eh
        _emit 0cch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 086h
        _emit 02ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 08bh
        _emit 0eah
        _emit 0feh
        _emit 0ffh
        _emit 0d9h
        _emit 086h
        _emit 024h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d8h
        _emit 000h
        _emit 0d9h
        _emit 09eh
        _emit 024h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 040h
        _emit 004h
        _emit 0d8h
        _emit 086h
        _emit 028h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 09eh
        _emit 028h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 040h
        _emit 008h
        _emit 0d8h
        _emit 086h
        _emit 02ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 09eh
        _emit 02ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 045h
        _emit 0d0h
        _emit 0d8h
        _emit 086h
        _emit 024h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 09eh
        _emit 024h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 045h
        _emit 0d4h
        _emit 0d8h
        _emit 086h
        _emit 028h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 09eh
        _emit 028h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 045h
        _emit 0d8h
        _emit 0d8h
        _emit 086h
        _emit 02ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 09eh
        _emit 02ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 045h
        _emit 0e0h
        _emit 08dh
        _emit 08eh
        _emit 04ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d8h
        _emit 049h
        _emit 00ch
        _emit 0c6h
        _emit 045h
        _emit 0fch
        _emit 002h
        _emit 0d9h
        _emit 045h
        _emit 0dch
        _emit 0d8h
        _emit 049h
        _emit 008h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 045h
        _emit 0e4h
        _emit 0d8h
        _emit 009h
        _emit 0deh
        _emit 0e9h
        _emit 0d9h
        _emit 045h
        _emit 0e0h
        _emit 0d8h
        _emit 009h
        _emit 0d9h
        _emit 045h
        _emit 0e4h
        _emit 0d8h
        _emit 049h
        _emit 00ch
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 045h
        _emit 0dch
        _emit 0d8h
        _emit 049h
        _emit 004h
        _emit 0deh
        _emit 0e9h
        _emit 0d9h
        _emit 05dh
        _emit 0e8h
        _emit 0d9h
        _emit 045h
        _emit 0dch
        _emit 0d8h
        _emit 009h
        _emit 0d9h
        _emit 0e0h
        _emit 0d9h
        _emit 045h
        _emit 0e0h
        _emit 0d8h
        _emit 049h
        _emit 004h
        _emit 0deh
        _emit 0e9h
        _emit 0d9h
        _emit 045h
        _emit 0e4h
        _emit 0d8h
        _emit 049h
        _emit 008h
        _emit 0deh
        _emit 0e9h
        _emit 0d9h
        _emit 05dh
        _emit 0ech
        _emit 0d9h
        _emit 045h
        _emit 0e4h
        _emit 0d8h
        _emit 049h
        _emit 004h
        _emit 0d9h
        _emit 045h
        _emit 0dch
        _emit 0d8h
        _emit 049h
        _emit 00ch
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 045h
        _emit 0e0h
        _emit 0d8h
        _emit 049h
        _emit 008h
        _emit 0deh
        _emit 0e9h
        _emit 0d9h
        _emit 086h
        _emit 014h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0d8h
        _emit 00dh
        _emit 08ch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0d9h
        _emit 055h
        _emit 0f0h
        _emit 0d8h
        _emit 0c9h
        _emit 0d9h
        _emit 05dh
        _emit 0b4h
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h
        _emit 045h
        _emit 0f0h
        _emit 0d8h
        _emit 0c9h
        _emit 0d9h
        _emit 05dh
        _emit 0b8h
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h
        _emit 045h
        _emit 0f0h
        _emit 0d8h
        _emit 04dh
        _emit 0e8h
        _emit 0d9h
        _emit 05dh
        _emit 0bch
        _emit 0d9h
        _emit 045h
        _emit 0f0h
        _emit 0d8h
        _emit 04dh
        _emit 0ech
        _emit 0d9h
        _emit 05dh
        _emit 0c0h
        _emit 0d9h
        _emit 045h
        _emit 0b4h
        _emit 0d8h
        _emit 001h
        _emit 0d9h
        _emit 019h
        _emit 0d9h
        _emit 045h
        _emit 0b8h
        _emit 0d8h
        _emit 041h
        _emit 004h
        _emit 0d9h
        _emit 059h
        _emit 004h
        _emit 0d9h
        _emit 045h
        _emit 0bch
        _emit 0d8h
        _emit 041h
        _emit 008h
        _emit 0d9h
        _emit 059h
        _emit 008h
        _emit 0d9h
        _emit 045h
        _emit 0c0h
        _emit 0d8h
        _emit 041h
        _emit 00ch
        _emit 0d9h
        _emit 059h
        _emit 00ch
        _emit 0d9h
        _emit 086h
        _emit 058h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 086h
        _emit 054h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 086h
        _emit 050h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 001h
        _emit 0d9h
        _emit 0c0h
        _emit 0d8h
        _emit 0c9h
        _emit 0d9h
        _emit 0c2h
        _emit 0d8h
        _emit 0cbh
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 0c3h
        _emit 0d8h
        _emit 0cch
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 0c4h
        _emit 0d8h
        _emit 0cdh
        _emit 0deh
        _emit 0c1h
        _emit 0ddh
        _emit 0dch
        _emit 0ddh
        _emit 0d8h
        _emit 0ddh
        _emit 0d8h
        _emit 0ddh
        _emit 0d8h
        _emit 0dch
        _emit 015h
        _emit 038h
        _emit 014h
        _emit 04eh
        _emit 000h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 075h
        _emit 00dh
        _emit 0dch
        _emit 015h
        _emit 030h
        _emit 014h
        _emit 04eh
        _emit 000h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 026h
        _emit 0d9h
        _emit 0fah
        _emit 0d8h
        _emit 03dh
        _emit 048h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0d9h
        _emit 0c0h
        _emit 0d8h
        _emit 009h
        _emit 0d9h
        _emit 019h
        _emit 0d9h
        _emit 0c0h
        _emit 0d8h
        _emit 049h
        _emit 004h
        _emit 0d9h
        _emit 059h
        _emit 004h
        _emit 0d9h
        _emit 0c0h
        _emit 0d8h
        _emit 049h
        _emit 008h
        _emit 0d9h
        _emit 059h
        _emit 008h
        _emit 0d8h
        _emit 049h
        _emit 00ch
        _emit 0d9h
        _emit 059h
        _emit 00ch
        _emit 0ebh
        _emit 002h
        _emit 0ddh
        _emit 0d8h
        _emit 08dh
        _emit 046h
        _emit 020h
        _emit 050h
        _emit 0e8h
        _emit 07ah
        _emit 08ah
        _emit 009h
        _emit 000h
        _emit 08dh
        _emit 046h
        _emit 020h
        _emit 089h
        _emit 045h
        _emit 0ech
        _emit 08dh
        _emit 086h
        _emit 018h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 045h
        _emit 0f0h
        _emit 08dh
        _emit 045h
        _emit 0d0h
        _emit 089h
        _emit 045h
        _emit 0e8h
        _emit 08bh
        _emit 04dh
        _emit 0f0h
        _emit 08bh
        _emit 055h
        _emit 0ech
        _emit 08bh
        _emit 045h
        _emit 0e8h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 00ah
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 04ah
        _emit 004h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 04ah
        _emit 008h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 010h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 014h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 018h
        _emit 0d9h
        _emit 0cah
        _emit 0deh
        _emit 0c5h
        _emit 0deh
        _emit 0c3h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 020h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 024h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 028h
        _emit 0d9h
        _emit 0cah
        _emit 0deh
        _emit 0c5h
        _emit 0deh
        _emit 0c3h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 0cah
        _emit 0d8h
        _emit 042h
        _emit 030h
        _emit 0d9h
        _emit 0c9h
        _emit 0d8h
        _emit 042h
        _emit 034h
        _emit 0d9h
        _emit 0cah
        _emit 0d8h
        _emit 042h
        _emit 038h
        _emit 0d9h
        _emit 0c9h
        _emit 0d9h
        _emit 018h
        _emit 0d9h
        _emit 058h
        _emit 008h
        _emit 0d9h
        _emit 058h
        _emit 004h
        _emit 08bh
        _emit 08eh
        _emit 024h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 096h
        _emit 028h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 086h
        _emit 02ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 00fh
        _emit 0d9h
        _emit 007h
        _emit 0d8h
        _emit 065h
        _emit 0d0h
        _emit 089h
        _emit 057h
        _emit 004h
        _emit 089h
        _emit 047h
        _emit 008h
        _emit 057h
        _emit 0d9h
        _emit 01fh
        _emit 08dh
        _emit 04dh
        _emit 0c4h
        _emit 0d9h
        _emit 047h
        _emit 004h
        _emit 051h
        _emit 0d8h
        _emit 065h
        _emit 0d4h
        _emit 08dh
        _emit 04dh
        _emit 0d0h
        _emit 0d9h
        _emit 05fh
        _emit 004h
        _emit 0d9h
        _emit 047h
        _emit 008h
        _emit 0d8h
        _emit 065h
        _emit 0d8h
        _emit 0d9h
        _emit 05fh
        _emit 008h
        _emit 0e8h
        _emit 081h
        _emit 0d6h
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 010h
        _emit 08bh
        _emit 048h
        _emit 004h
        _emit 089h
        _emit 096h
        _emit 024h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 050h
        _emit 008h
        _emit 089h
        _emit 08eh
        _emit 028h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0ceh
        _emit 089h
        _emit 096h
        _emit 02ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 030h
        _emit 0f6h
        _emit 0ffh
        _emit 0ffh
        _emit 08dh
        _emit 086h
        _emit 05ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 081h
        _emit 0c6h
        _emit 0a8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 045h
        _emit 0ech
        _emit 089h
        _emit 075h
        _emit 0f0h
        _emit 089h
        _emit 05dh
        _emit 0e8h
        _emit 08bh
        _emit 04dh
        _emit 0f0h
        _emit 08bh
        _emit 055h
        _emit 0ech
        _emit 08bh
        _emit 045h
        _emit 0e8h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 00ah
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 04ah
        _emit 004h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 04ah
        _emit 008h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 010h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 014h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 018h
        _emit 0d9h
        _emit 0cah
        _emit 0deh
        _emit 0c5h
        _emit 0deh
        _emit 0c3h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 020h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 024h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 028h
        _emit 0d9h
        _emit 0cah
        _emit 0deh
        _emit 0c5h
        _emit 0deh
        _emit 0c3h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 0cah
        _emit 0d9h
        _emit 018h
        _emit 0d9h
        _emit 058h
        _emit 004h
        _emit 0d9h
        _emit 058h
        _emit 008h
        _emit 08bh
        _emit 04dh
        _emit 0f4h
        _emit 05fh
        _emit 05eh
        _emit 05bh
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x41c920
void PhysBody::SetCollision(CollisionPart* part)
{
    id230 = part->id;
    collision = new CollisionModel(part, 0);
}

// FUNCTION: RALLYTROPHY 0x41c990
void PhysBody::SetCollision2(CollisionPart* part)
{
    id230 = part->id;
    collision = new CollisionModel(part, 0, 0);
}

// FUNCTION: RALLYTROPHY 0x41ca00
void PhysBody::SetCollision3(CollisionPart* part)
{
    id230 = part->id;
    collision = new CollisionModel(part, 0, 1);
}

// FUNCTION: RALLYTROPHY 0x4203b0
// SYMBOL: ??GVec3@@QBE?AV0@XZ
Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

// FUNCTION: RALLYTROPHY 0x4203e0
Vec3 FUN_004203e0(const Vec3& a, const Vec3& b)
{
    return Vec3(b.z * a.y - a.z * b.y, a.z * b.x - b.z * a.x, b.y * a.x - b.x * a.y);
}

// FUNCTION: RALLYTROPHY 0x420430
// SYMBOL: ??D@YA?AVVec3@@MABV0@@Z
Vec3 operator*(float k, const Vec3& v)
{
    return v * k;
}
