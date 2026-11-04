// 0x41fc60: resolve accumulated contacts of a car body into linear/angular impulses. Types: include/unk_41fc60.h.
#include "unk_41fc60.h"

extern float DAT_004e134c, DAT_004e1348, DAT_004e1448;
Vec3 FUN_004203e0(const Vec3& a, const Vec3& b);   // cross product

#define F(p, o) (*(float*)((char*)(p) + (o)))
#define BITS(x) (*(int*)&(x))

// Hand-written x87 blocks (inline assembler in the original).
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

inline void CrossAsm(float* o, const float* a, const float* b)
{
    __asm {
        mov ecx, a
        mov edx, b
        mov eax, o
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx]
        fxch st(2)
        fsubp st(5), st
        fsubp st(3), st
        fsubp st(1), st
        fxch st(2)
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
}

// NONMATCHING: RALLYTROPHY 0x41fc60
// NONMATCHING: Xform (this+0x15c) and cross-product x87 blocks now inline asm like the original (shape matches); frame already matched; first loop/EH-state structure and slot allocation still differ (1719 of 1861 bytes, 1634 differing).
// Tried 2026-10-30 night: 17 variants (function-scope Vec3 for EH state 1, v write-back in loop 1, Vec3 copies of n/v/imp/pt, n passed as *(Vec3*)c->n, dot/decl/loop-form changes) only moved 393 to 387-402 insns; original keeps loop-1 accumulators on the FPU stack, copies n/v/imp/pt through Vec3 temps (EH states 5,7,0xa) and is 185 bytes larger, needs a whole-function Vec3-operator rewrite.
// Tried 2026-11-01 re-pass: fixed dot/pen/denominator/lp/ln/torque sums and the spd data (imp, not v) to the original's fadd order and fields, n passed as c->n; 10 more loop-1 v-copy/writeback variants (Vec3 copies, int-routed v1/v2, holder Vec3, volatile reads) stayed at 379-393 insns; still needs the whole-function Vec3 rewrite (built 1677 vs 1861 bytes).
void U40A850Body::FUN_0041fc60(void* stage, void* unused)
{
    unsigned cnt = *(unsigned*)((char*)this + 0x23c);
    if (cnt == 0) {
        return;
    }
    ImpStage* st = (ImpStage*)stage;
    Vec3 pos, neg;
    pos.x = 0.0f; pos.y = 0.0f; pos.z = 0.0f;
    neg.x = 0.0f; neg.y = DAT_004e134c; neg.z = DAT_004e134c;
    float thresh = (st == 0) ? 1000.0f : F(st, 0x1080);
    ImpContact* base = *(ImpContact**)((char*)this + 0x238);
    ImpContact* c = base;
    unsigned i;
    for (i = 0; i < cnt; i++, c++) {
        float vx = c->v[0], vy = c->v[1], vz = c->v[2];
        float dot = vy * c->n[1] + vz * c->n[2] + vx * c->n[0];
        if ((BITS(dot) & 0x80000000) == 0) {
            c->active = 0;
        } else {
            c->active = 1;
            float ix = c->n[0] * dot, iy = c->n[1] * dot, iz = c->n[2] * dot;
            c->imp[0] = ix; c->imp[1] = iy; c->imp[2] = iz;
            if ((BITS(ix) & 0x80000000) == 0) pos.x = pos.x + ix; else neg.x = neg.x + ix;
            if ((BITS(iy) & 0x80000000) == 0) pos.y = pos.y + iy; else neg.y = neg.y + iy;
            if ((BITS(iz) & 0x80000000) == 0) pos.z = pos.z + iz; else neg.z = neg.z + iz;
        }
    }
    if (BITS(pos.x) < 0x3a83126f) pos.x = 0.001f;
    if (BITS(pos.y) < 0x3a83126f) pos.y = 0.001f;
    if (BITS(pos.z) < 0x3a83126f) pos.z = 0.001f;
    if (DAT_004e1448 < neg.x) neg.x = -0.001f;
    if (DAT_004e1448 < neg.y) neg.y = DAT_004e1448;
    if (DAT_004e1448 < neg.z) neg.z = DAT_004e1448;
    neg.x = DAT_004e1348 / neg.x;
    neg.y = DAT_004e1348 / neg.y;
    neg.z = DAT_004e1348 / neg.z;
    pos.x = DAT_004e1348 / pos.x;
    pos.y = DAT_004e1348 / pos.y;
    pos.z = DAT_004e1348 / pos.z;

    c = base;
    for (unsigned k = 0; k < *(unsigned*)((char*)this + 0x23c); k++, c++) {
        if (c->active != 0) {
        Vec3 w(*(Vec3*)c->imp);
        float tx = w.x * (((BITS(w.x) & 0x80000000) != 0) ? neg.x : pos.x);
        float ty = w.y * (((BITS(w.y) & 0x80000000) != 0) ? neg.y : pos.y);
        float tz = w.z * (((BITS(w.z) & 0x80000000) != 0) ? neg.z : pos.z);
        w.x = tx * c->imp[0]; w.y = ty * c->imp[1]; w.z = tz * c->imp[2];
        float pen = w.y * c->n[1]; pen = w.x * c->n[0] + pen; pen = w.z * c->n[2] + pen; pen = -pen;
        if (pen > DAT_004e134c) {
        Vec3 r, rn, av;
        r.x = c->pt[0] - F(this, 0x224); r.y = c->pt[1] - F(this, 0x228); r.z = c->pt[2] - F(this, 0x22c);
        rn = FUN_004203e0(r, *(Vec3*)c->n);
        Xform(&av.x, &rn.x, (float*)((char*)this + 0x15c));
        CrossAsm(&rn.x, &av.x, &r.x);
        float inv = DAT_004e1348 / (rn.y * c->n[1] + rn.z * c->n[2] + rn.x * c->n[0] + F(this, 0x130));
        float jj = inv * pen + inv * pen;
        Vec3 j(c->n[0] * jj, c->n[1] * jj, c->n[2] * jj);
        F(this, 0x1fc) = j.x + F(this, 0x1fc);
        F(this, 0x200) = j.y + F(this, 0x200);
        F(this, 0x204) = j.z + F(this, 0x204);
        {
            Vec3 r2(c->pt[0] - F(this, 0x224), c->pt[1] - F(this, 0x228), c->pt[2] - F(this, 0x22c));
            Vec3 tq = FUN_004203e0(j, r2);
            F(this, 0x208) = tq.x + F(this, 0x208);
            F(this, 0x20c) = tq.y + F(this, 0x20c);
            F(this, 0x210) = tq.z + F(this, 0x210);
        }
        if (st != 0) {
            float spd = c->n[1] * c->imp[1]; spd = c->n[0] * c->imp[0] + spd; spd = c->n[2] * c->imp[2] + spd; spd = -spd;
            if (thresh < spd) {
                Vec3 d(*(Vec3*)c->pt), lp;
                d.x -= F(this, 0x14); d.y -= F(this, 0x18); d.z -= F(this, 0x1c);
                lp.x = d.z * F(this, 0x28) + d.y * F(this, 0x24) + d.x * F(this, 0x20);
                lp.y = d.z * F(this, 0x38); lp.y = d.y * F(this, 0x34) + lp.y; lp.y = d.x * F(this, 0x30) + lp.y;
                lp.z = d.z * F(this, 0x48); lp.z = d.y * F(this, 0x44) + lp.z; lp.z = d.x * F(this, 0x40) + lp.z;
                Vec3 ln;
                ln.x = F(this, 0x24) * c->n[1] + F(this, 0x28) * c->n[2] + c->n[0] * F(this, 0x20);
                ln.y = F(this, 0x34) * c->n[1]; ln.y = F(this, 0x38) * c->n[2] + ln.y; ln.y = c->n[0] * F(this, 0x30) + ln.y;
                ln.z = F(this, 0x44) * c->n[1]; ln.z = F(this, 0x48) * c->n[2] + ln.z; ln.z = c->n[0] * F(this, 0x40) + ln.z;
                st->AddContact(&lp.x, &ln.x, spd);
                if (F(this, 0x240) < spd * inv) {
                    F(this, 0x25c) = c->n[0];
                    F(this, 0x260) = c->n[1];
                    F(this, 0x264) = c->n[2];
                    F(this, 0x250) = j.z * F(this, 0x28) + j.y * F(this, 0x24) + j.x * F(this, 0x20);
                    float t254 = j.z * F(this, 0x38); t254 = j.y * F(this, 0x34) + t254; F(this, 0x254) = j.x * F(this, 0x30) + t254;
                    F(this, 0x24c) = lp.z;
                    F(this, 0x248) = lp.y;
                    F(this, 0x244) = lp.x;
                    float t258 = j.z * F(this, 0x48); t258 = j.y * F(this, 0x44) + t258; F(this, 0x258) = j.x * F(this, 0x40) + t258;
                    F(this, 0x240) = spd * inv;
                }
            }
        }
        }
        }
    }
    FUN_0041c010();
    *(int*)((char*)this + 0x23c) = 0;
}
