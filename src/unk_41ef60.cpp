// Car body contact response: turns the contact records into forces/torques on the body.
#include "unk_41ef60.h"

struct PV { float x, y, z; };
struct PVm { float x, y, z; PV operator*(float k) const; };   // 0x40b0c0, POD result read through eax
PV FUN_004203e0(const Vec3& a, const Vec3& b); // 0x4203e0 cross product
#include <math.h>
extern float DAT_004e1348;
extern float DAT_004e1448;
extern float DAT_004e134c;
extern float DAT_004e1444;
extern float DAT_004e1440;
extern float DAT_004e13dc;

#define NEG(f) (*(unsigned int*)&(f) & 0x80000000)
#define BF(o) (*(float*)((char*)this + (o)))
#define SF(o) (*(float*)((char*)stage + (o)))
// Stage per-surface tables indexed by the triangle's id.
#define SID(h) (*(short*)(*(char**)&(h)[2] + 0x10))

inline int FB(float f) { return *(int*)&f; }

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

// original: FUN_0041ef60
// NONMATCHING: RALLYTROPHY 0x41ef60
// Tried 2026-10-30 night: first loop rewritten with Vec3 v/c locals, direct PointVel pointer use, sign-branch order and negS compare flipped, 24 declaration-order permutations of posS/negS/limit/c (740->734 insns); loops 2-3 differ structurally (original has inlined Vec3 helper/normalize/operator* temps, 3319 vs our 3079 bytes), not reachable by local variants.
// Tried 2026-11-01 re-pass: re-derived every float sum association from the original x87 (pen, k denominator, dn, t2, p/q/F projections, work, first-loop d), fixed the depth test to read h[12..14] (n*d) not h[9..11]; e0..e2 now (a*s)*h[12] via a Vec3 copy; 740->712 insns, 3122 of 3319 bytes; still differs structurally (inlined Vec3 helper temps, per-axis select copies through int slots).
// NONMATCHING: Xform (this+0x15c) and cross-product x87 blocks now inline asm like the original; frame matches; remaining diffs are register/stack allocation and statement order (3107 of 3319 bytes, 2990 differing).
void RespBody::Respond(void* car, void* stage, int flag)
{
    if (*(int*)((char*)this + 0x23c) == 0) {
        return;
    }
    Vec3 posS;
    posS.x = posS.y = posS.z = 0.0f;
    Vec3 negS;
    negS.x = negS.y = negS.z = 0.0f;
    float limit;
    if (car == 0) {
        limit = 1000.0f;
    } else {
        limit = *(float*)((char*)car + 0x1080);
    }
    Vec3 c = *(Vec3*)((char*)this + 0x1cc);
    float* h = *(float**)((char*)this + 0x238);
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)this + 0x23c); i++, h += 20) {
        Vec3 v;
        Vec3 av;
        if (flag == 0) {
            v.x = h[9];
            *(int*)&v.y = *(int*)&h[10];
            *(int*)&v.z = *(int*)&h[11];
        } else {
            v = c;
            Vec3* w = &FUN_004204f0(*(Vec3*)&h[3]);
            v.x = c.x + w->x;
            float ty = c.y + w->y;
            *(int*)&v.y = *(int*)&ty;
            float tz = c.z + w->z;
            *(int*)&v.z = *(int*)&tz;
        }
        float d = v.x * h[6]; d = v.y * h[7] + d; d = v.z * h[8] + d;
        if (NEG(d) == 0) {
            *(int*)h = 0;
        } else {
            *(int*)h = 1;
            av.x = h[6] * d; av.y = h[7] * d; av.z = h[8] * d;
            h[10] = v.y;
            h[11] = v.z;
            h[9] = v.x;
            h[12] = av.x;
            h[13] = av.y;
            h[14] = av.z;
            if (NEG(av.x) != 0) negS.x = negS.x + av.x; else posS.x = posS.x + av.x;
            if (NEG(av.y) != 0) negS.y = negS.y + av.y; else posS.y = posS.y + av.y;
            if (NEG(av.z) != 0) negS.z = negS.z + av.z; else posS.z = posS.z + av.z;
        }
    }
    if (*(int*)&posS.x < FB(0.001f)) posS.x = 0.001f;
    if (*(int*)&posS.y < FB(0.001f)) posS.y = 0.001f;
    if (*(int*)&posS.z < FB(0.001f)) posS.z = 0.001f;
    if (negS.x > DAT_004e1448) negS.x = -0.001f;
    if (negS.y > DAT_004e1448) negS.y = -0.001f;
    if (negS.z > DAT_004e1448) negS.z = -0.001f;
    negS.x = DAT_004e1348 / negS.x;
    negS.y = DAT_004e1348 / negS.y;
    negS.z = DAT_004e1348 / negS.z;
    posS.x = DAT_004e1348 / posS.x;
    posS.y = DAT_004e1348 / posS.y;
    posS.z = DAT_004e1348 / posS.z;
    float invDt = DAT_004e1348 / BF(0x214);

    Vec3 fp, fn;                    // accumulated impulse per axis (positive / negative parts)
    fp.x = fp.y = fp.z = 0.0f;
    fn.x = fn.y = fn.z = 0.0f;
    h = *(float**)((char*)this + 0x238);
    for (i = 0; i < *(unsigned int*)((char*)this + 0x23c); i++, h += 20) {
        if (*(int*)h != 0) {
            Vec3 a = *(Vec3*)&h[12];
            a.x = a.x * (NEG(a.x) ? negS.x : posS.x);
            a.y = a.y * (NEG(a.y) ? negS.y : posS.y);
            a.z = a.z * (NEG(a.z) ? negS.z : posS.z);
            a.x = a.x * h[12];
            a.y = a.y * h[13];
            a.z = a.z * h[14];
            float pen = a.x * h[6]; pen = a.z * h[8] + pen; pen = -(a.y * h[7] + pen);
            if (pen > DAT_004e134c) {
                Vec3 r = *(Vec3*)&h[3];
                r -= *(Vec3*)((char*)this + 0x224);
                Vec3 rn;
                Vec3 bv;
                *(PV*)&rn = FUN_004203e0(r, *(Vec3*)&h[6]);
                Xform(&bv.x, &rn.x, (float*)((char*)this + 0x15c));
                CrossAsm(&rn.x, &bv.x, &r.x);
                float k = rn.x * h[6]; k = rn.z * h[8] + k; k = rn.y * h[7] + k + BF(0x130);
                k = DAT_004e1348 / k;
                float imp = k * pen;

                // tangential (friction) direction: velocity minus its normal component
                Vec3 vel = *(Vec3*)&h[9];
                const PV& nn = ((PVm*)&h[6])->operator*(h[11] * h[8] + h[10] * h[7] + h[9] * h[6]);
                vel.x = vel.x - nn.x;
                vel.y = vel.y - nn.y;
                vel.z = vel.z - nn.z;
                float t2 = vel.z * vel.z + vel.y * vel.y + vel.x * vel.x;
                if (t2 > DAT_004e1444) {
                    float tl = (float)sqrt(t2);
                    float inv = DAT_004e1348 / tl;
                    vel.x = inv * vel.x;
                    h[16] = vel.x;
                    vel.y = vel.y * inv;
                    h[17] = vel.y;
                    vel.z = inv * vel.z;
                    h[19] = tl;
                    h[18] = vel.z;
                    float grip = SF(0x42c + SID(h) * 4) * DAT_004e1440;
                    tl = tl * invDt;
                    float f = imp * invDt;
                    if (tl < grip) {
                        f = (tl / grip) * f;
                    }
                    h[15] = f;
                    float a0 = h[16], a1 = h[17], a2 = h[18];
                    float fx = a0 * f, fy = a1 * f;
                    if (NEG(a0) == 0) fp.x = fp.x + fx; else fn.x = fn.x - fx;
                    if (NEG(a1) == 0) fp.y = fp.y + fy; else fn.y = fn.y - fy;
                    if (NEG(a2) == 0) fp.z = fp.z + f * a2; else fn.z = fn.z - f * a2;
                } else {
                    *(int*)h = 0;
                }
                float bounce = SF(0x29c + SID(h) * 4) * imp;
                Vec3 F = *(Vec3*)&h[6];
                F.x = F.x * bounce;
                F.y = F.y * bounce;
                F.z = F.z * bounce;
                BF(0x1fc) = F.x + BF(0x1fc);
                BF(0x200) = F.y + BF(0x200);
                BF(0x204) = F.z + BF(0x204);
                {
                    Vec3 arm = *(Vec3*)&h[3];
                    arm -= *(Vec3*)((char*)this + 0x224);
                    const PV& tq = FUN_004203e0(F, arm);
                    BF(0x208) = BF(0x208) + tq.x;
                    BF(0x20c) = tq.y + BF(0x20c);
                    BF(0x210) = tq.z + BF(0x210);
                }
                if (car != 0) {
                    float depth = h[12] * h[6]; depth = h[14] * h[8] + depth; depth = -(h[13] * h[7] + depth);
                    if (limit < depth) {
                        Vec3 l = *(Vec3*)&h[3];
                        l -= *(Vec3*)((char*)this + 0x14);
                        Vec3 p, q;
                        p.x = l.z * BF(0x28); p.x = l.y * BF(0x24) + p.x; p.x = l.x * BF(0x20) + p.x;
                        p.y = l.z * BF(0x38); p.y = l.y * BF(0x34) + p.y; p.y = l.x * BF(0x30) + p.y;
                        p.z = l.z * BF(0x48); p.z = l.y * BF(0x44) + p.z; p.z = l.x * BF(0x40) + p.z;
                        q.x = BF(0x28) * h[8]; q.x = BF(0x24) * h[7] + q.x; q.x = h[6] * BF(0x20) + q.x;
                        q.y = BF(0x38) * h[8]; q.y = BF(0x34) * h[7] + q.y; q.y = h[6] * BF(0x30) + q.y;
                        q.z = BF(0x48) * h[8]; q.z = BF(0x44) * h[7] + q.z; q.z = h[6] * BF(0x40) + q.z;
                        ((Car*)car)->AddContact(&p, &q, depth);
                        if (BF(0x240) < depth * k) {
                            BF(0x25c) = h[6];
                            BF(0x260) = h[7];
                            BF(0x264) = h[8];
                            BF(0x250) = F.z * BF(0x28) + F.y * BF(0x24) + F.x * BF(0x20);
                            BF(0x254) = F.z * BF(0x38) + F.y * BF(0x34) + F.x * BF(0x30);
                            BF(0x24c) = p.z;
                            BF(0x248) = p.y;
                            BF(0x244) = p.x;
                            BF(0x258) = F.z * BF(0x48) + F.y * BF(0x44) + F.x * BF(0x40);
                            BF(0x240) = depth * k;
                        }
                    }
                }
            } else {
                *(int*)h = 0;
            }
        }
    }

    float cap = BF(0x12c) * DAT_004e1440;
    if (*(int*)&cap < *(int*)&fp.x) fp.x = cap / fp.x; else fp.x = 1.0f;
    if (*(int*)&cap < *(int*)&fp.y) fp.y = cap / fp.y; else fp.y = 1.0f;
    if (*(int*)&cap < *(int*)&fp.z) fp.z = cap / fp.z; else fp.z = 1.0f;
    if (*(int*)&cap < *(int*)&fn.x) fn.x = cap / fn.x; else fn.x = 1.0f;
    if (*(int*)&cap < *(int*)&fn.y) fn.y = cap / fn.y; else fn.y = 1.0f;
    if (*(int*)&cap < *(int*)&fn.z) fn.z = cap / fn.z; else fn.z = 1.0f;

    h = *(float**)((char*)this + 0x238);
    for (i = 0; i < *(unsigned int*)((char*)this + 0x23c); i++, h += 20) {
        if (*(int*)h != 0) {
            Vec3 unused3;
            Vec3 m;
            Vec3 d = *(Vec3*)&h[16];
            *(int*)&m.x = NEG(d.x) != 0 ? *(int*)&fn.x : *(int*)&fp.x;
            *(int*)&m.y = NEG(d.y) != 0 ? *(int*)&fn.y : *(int*)&fp.y;
            float m2 = (NEG(d.z) != 0) ? fn.z : fp.z;
            m.x = d.x * m.x;
            m.y = d.y * m.y;
            m.x = h[15] * m.x;
            m.y = h[15] * m.y;
            m.z = h[15] * (m2 * d.z);
            float w0 = d.x * m.x; w0 = w0 + d.z * m.z; w0 = w0 + d.y * m.y; float work = w0 * SF(0x42c + SID(h) * 4);
            if (*(int*)((char*)stage + 0x4cc + SID(h) * 4) != 0 && h[19] > DAT_004e13dc && work > BF(0x268)) {
                BF(0x268) = work;
                BF(0x26c) = h[3];
                BF(0x270) = h[4];
                BF(0x274) = h[5];
                BF(0x278) = h[6];
                BF(0x27c) = h[7];
                BF(0x280) = h[8];
            }
            Vec3 g = *(Vec3*)&h[16];
            g.x = g.x * work;
            g.y = g.y * work;
            g.z = g.z * work;
            Vec3 ng = -g;
            BF(0x1e4) = BF(0x1e4) + ng.x;
            BF(0x1e8) = ng.y + BF(0x1e8);
            BF(0x1ec) = ng.z + BF(0x1ec);
            Vec3 arm = *(Vec3*)&h[3];
            arm -= *(Vec3*)((char*)this + 0x224);
            const PV& tq = FUN_004203e0(ng, arm);
            BF(0x1f0) = tq.x + BF(0x1f0);
            BF(0x1f4) = tq.y + BF(0x1f4);
            BF(0x1f8) = tq.z + BF(0x1f8);
        }
    }
    FUN_0041c010();
    *(int*)((char*)this + 0x23c) = 0;
}
