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
void U40A850Body::FUN_0041fc60(void* stage, void* unused)
{
    int cnt = *(int*)((char*)this + 0x23c);
    if (cnt == 0) {
        return;
    }
    ImpStage* st = (ImpStage*)stage;
    float negB = DAT_004e134c, negC = DAT_004e134c, negA = 0.0f;
    float posA = 0.0f, posB = 0.0f, posC = 0.0f;
    float thresh = (st == 0) ? 1000.0f : F(st, 0x1080);
    ImpContact* base = *(ImpContact**)((char*)this + 0x238);
    ImpContact* c = base;
    int i;
    for (i = cnt; i != 0; i--, c++) {
        float nx = c->n[0], ny = c->n[1], nz = c->n[2];
        float vx = c->v[0], vy = c->v[1], vz = c->v[2];
        float dot = vy * nx + vz * nz + vx * ny;
        if ((BITS(dot) & 0x80000000) == 0) {
            c->active = 0;
        } else {
            c->active = 1;
            float ix = nx * dot, iy = ny * dot, iz = nz * dot;
            c->imp[0] = ix; c->imp[1] = iy; c->imp[2] = iz;
            if ((BITS(ix) & 0x80000000) == 0) posA = posA + ix; else negA = negA + ix;
            if ((BITS(iy) & 0x80000000) == 0) posB = posB + iy; else negB = negB + iy;
            if ((BITS(iz) & 0x80000000) == 0) posC = posC + iz; else negC = negC + iz;
        }
    }
    if (BITS(posA) < 0x3a83126f) posA = 0.001f;
    if (BITS(posB) < 0x3a83126f) posB = 0.001f;
    if (BITS(posC) < 0x3a83126f) posC = 0.001f;
    if (DAT_004e1448 < negA) negA = -0.001f;
    if (DAT_004e1448 < negB) negB = DAT_004e1448;
    if (DAT_004e1448 < negC) negC = DAT_004e1448;
    negA = DAT_004e1348 / negA;
    negB = DAT_004e1348 / negB;
    negC = DAT_004e1348 / negC;
    posA = DAT_004e1348 / posA;
    posB = DAT_004e1348 / posB;
    posC = DAT_004e1348 / posC;

    c = base;
    for (unsigned k = 0; k < *(unsigned*)((char*)this + 0x23c); k++, c++) {
        if (c->active == 0) continue;
        float sx = ((BITS(c->imp[0]) & 0x80000000) != 0) ? negA : posA;
        float sy = ((BITS(c->imp[1]) & 0x80000000) != 0) ? negB : posB;
        float sz = ((BITS(c->imp[2]) & 0x80000000) != 0) ? negC : posC;
        float wx = c->imp[0] * sx * c->imp[0];
        float wy = c->imp[1] * sy * c->imp[1];
        float wz = c->imp[2] * sz * c->imp[2];
        float pen = -(wz * c->n[2] + wx * c->n[0] + wy * c->n[1]);
        if (!(DAT_004e134c < pen)) continue;
        Vec3 r(c->pt[0] - F(this, 0x224), c->pt[1] - F(this, 0x228), c->pt[2] - F(this, 0x22c));
        Vec3 n(c->n[0], c->n[1], c->n[2]);
        Vec3 rn = FUN_004203e0(r, n);
        float av[3];
        Xform(av, &rn.x, (float*)((char*)this + 0x15c));
        CrossAsm(&rn.x, av, &r.x);
        float cx = rn.x, cy = rn.y, cz = rn.z;
        float inv = DAT_004e1348 / (cx * c->n[0] + cz * c->n[2] + cy * c->n[1] + F(this, 0x130));
        float j = inv * pen + inv * pen;
        float jx = c->n[0] * j, jy = c->n[1] * j, jz = c->n[2] * j;
        F(this, 0x1fc) = jx + F(this, 0x1fc);
        F(this, 0x200) = jy + F(this, 0x200);
        F(this, 0x204) = jz + F(this, 0x204);
        Vec3 r2(c->pt[0] - F(this, 0x224), c->pt[1] - F(this, 0x228), c->pt[2] - F(this, 0x22c));
        Vec3 imp(jx, jy, jz);
        Vec3 tq = FUN_004203e0(imp, r2);
        F(this, 0x208) = tq.x + F(this, 0x208);
        F(this, 0x20c) = tq.y + F(this, 0x20c);
        F(this, 0x210) = tq.z + F(this, 0x210);
        if (st != 0) {
            float spd = -(c->n[2] * c->v[2] + c->n[0] * c->v[0] + c->n[1] * c->v[1]);
            if (thresh < spd) {
                float dx = c->pt[0] - F(this, 0x14);
                float dy = c->pt[1] - F(this, 0x18);
                float dz = c->pt[2] - F(this, 0x1c);
                float lp[3], ln[3];
                lp[0] = dx * F(this, 0x20) + dy * F(this, 0x24) + dz * F(this, 0x28);
                lp[1] = dx * F(this, 0x30) + dy * F(this, 0x34) + dz * F(this, 0x38);
                lp[2] = dx * F(this, 0x40) + dy * F(this, 0x44) + dz * F(this, 0x48);
                ln[0] = c->n[0] * F(this, 0x20) + F(this, 0x28) * c->n[2] + F(this, 0x24) * c->n[1];
                ln[1] = F(this, 0x30) * c->n[0] + F(this, 0x38) * c->n[2] + F(this, 0x34) * c->n[1];
                ln[2] = c->n[0] * F(this, 0x40) + F(this, 0x48) * c->n[2] + F(this, 0x44) * c->n[1];
                st->AddContact(lp, ln, spd);
                if (F(this, 0x240) < spd * inv) {
                    F(this, 0x25c) = c->n[0];
                    F(this, 0x260) = c->n[1];
                    F(this, 0x264) = c->n[2];
                    F(this, 0x250) = jx * F(this, 0x20) + jy * F(this, 0x24) + jz * F(this, 0x28);
                    F(this, 0x254) = jx * F(this, 0x30) + jy * F(this, 0x34) + jz * F(this, 0x38);
                    F(this, 0x24c) = lp[2];
                    F(this, 0x248) = lp[1];
                    F(this, 0x244) = lp[0];
                    F(this, 0x258) = jx * F(this, 0x40) + jy * F(this, 0x44) + jz * F(this, 0x48);
                    F(this, 0x240) = spd * inv;
                }
            }
        }
    }
    FUN_0041c010();
    *(int*)((char*)this + 0x23c) = 0;
}
