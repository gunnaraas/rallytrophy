// 0x417540: per-player update of the roaming animal (picks a path record near the player, walks it, drops a hit sphere).
#include <math.h>
#include <stdlib.h>
#include "unk_417540.h"

extern float DAT_004e1330, DAT_004e1404, DAT_004e1348, DAT_004e1400, DAT_004e13dc;
extern float DAT_004e13f4, DAT_004e13f8, DAT_004e13fc, DAT_004e138c;

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

// cresult = ca x cb, a hand-written x87 block in the original.
inline void AnimCross(const float* ca, const float* cb, float* cresult)
{
    __asm mov ecx, ca
    __asm mov edx, cb
    __asm mov eax, cresult
    __asm fld dword ptr [ecx+4]
    __asm fmul dword ptr [edx+8]
    __asm fld dword ptr [ecx+8]
    __asm fmul dword ptr [edx]
    __asm fld dword ptr [ecx]
    __asm fmul dword ptr [edx+4]
    __asm fld dword ptr [ecx+8]
    __asm fmul dword ptr [edx+4]
    __asm fld dword ptr [ecx]
    __asm fmul dword ptr [edx+8]
    __asm fld dword ptr [ecx+4]
    __asm fmul dword ptr [edx]
    __asm fxch st(2)
    __asm fsubp st(5), st
    __asm fsubp st(3), st
    __asm fsubp st(1), st
    __asm fxch st(2)
    __asm fstp dword ptr [eax]
    __asm fstp dword ptr [eax+4]
    __asm fstp dword ptr [eax+8]
}

struct AVec {
    float x, y, z;
    void operator-=(const AVec& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    void operator+=(const AVec& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    void operator*=(float k)
    {
        x = k * x;
        y = k * y;
        z = k * z;
    }
    float Length() const
    {
        float a = x, b = y, c = z;
        return (float)sqrt(a * a + b * b + c * c);
    }
};

// Locals of the update are Vec3-like objects with an empty destructor (cleanup states 0-8 in the original) and
// the hit sphere is a 16-byte object of the same kind; the direction scale returns a plain AVec (no state).
struct AnimVec : AVec { ~AnimVec() {} AnimVec& operator=(const AVec& v) { x = v.x; y = v.y; z = v.z; return *this; } };
struct AnimSphere { float x, y, z, r; ~AnimSphere() {} };
struct AnimDir { AVec Scale(float k) const; };          // 0x40b0c0 (Vec3::operator*(float)), hidden POD result
struct AnimTrace {                                      // the hit collector at Stage + 0x13b8 (AnimalTrace layout)
    int alloc;
    void* first;                                        // 0x04
    void* last;                                         // 0x08
    void* end;                                          // 0x0c
    int pad10;
    AVec lo;                                            // 0x14
    AVec hi;                                            // 0x20
    AnimSphere* sphere;                                 // 0x2c
    int mode;                                           // 0x30
};

#define F(o, t) (*(t*)((BYTE*)this + (o)))
#define DIR (*(AVec*)((BYTE*)this + 0x1390))
#define POS (*(AVec*)((BYTE*)this + 0x1378))
#define REC(i) ((float*)(F(0x13ac, int) + (i) * 0x24))

// NONMATCHING: RALLYTROPHY 0x417540
// 2256 vs 2332 bytes; the nine calls and the cleanup states (0, 3, 5, 8 stored; nine Vec3-like locals in the
// original's order: dv, lo, hi, sphere, a, b, n1, n2, down) match, and the record lerp / box / sample points are
// vector operations as in the original. Left: register allocation (the original keeps the record pointer in edi
// and the trace in edi later) and the frame layout.
void Stage::FUN_00417540(Player* player, int time, int dt)
{
    int car = *(int*)((BYTE*)player + 0x28);
    unsigned int idx = 0xffffffff;
    int local30 = 0;
    float* r;
    float rate;
    float f4;
    float f5;

    if (F(0x136c, int) == -1) {
        unsigned int i = 0;
        if (F(0x13ec, int) == 2) {
            if (F(0x13f0, int) < 1) goto tail;
            if (F(0x13f0, int) <= F(0x13f4, int)) F(0x13f4, int) = 0;
            int k = F(0x13f4, int);
            int tk = F(0x13fc + k * 8, int);
            if (time + dt <= tk) goto tail;
            idx = F(0x13f8 + k * 8, unsigned int);
            if (REC(idx)[8] == 0) goto tail;
            local30 = (time - tk) + dt;
            F(0x13f4, int) = k + 1;
        } else {
            float px = *(float*)(*(int*)(car + 0x24) + 0x14);
            float pz = *(float*)(*(int*)(car + 0x24) + 0x1c);
            float r2 = F(0x240, float) * F(0x240, float);
            float best = r2 * DAT_004e1330;
            float dd = F(0x240, float) - F(0x244, float);
            float inner = dd * dd;
            int off = 0;
            for (; F(0x13ac, int) != 0 && i < (unsigned int)((F(0x13b0, int) - F(0x13ac, int)) / 0x24); i++) {
                float* p = (float*)(F(0x13ac, int) + off);
                if (*(int*)(F(0x13ac, int) + 0x20 + off) != 0) {
                    float z = p[2];
                    float dx = p[0] - px;
                    float d = dx * dx + (z - pz) * (z - pz);
                    if (d < r2 && inner < d && d < best) {
                        idx = i;
                        best = d;
                    }
                }
                off += 0x24;
            }
        }
        if (idx != 0xffffffff) {
            int mode = F(0x13ec, int);
            r = REC(idx);
            r[8] = 0.0f;
            if (mode != 2) {
                int lim = RoundF(F(0x248, float) * DAT_004e1404);
                if (lim < (rand() & 0x3ff)) goto tail;
            }
            F(0x1378, float) = r[0];
            F(0x1380, float) = r[2];
            F(0x1370, int) = local30;
            F(0x137c, float) = r[1];
            F(0x139c, int) = 0;
            F(0x13a0, int) = 0;
            F(0x1394, int) = 0;
            DIR = *(AVec*)&r[3];
            DIR -= *(AVec*)&r[0];
            F(0x136c, unsigned int) = idx;
            F(0x1374, int) = 1;
            float up[3];
            up[0] = 0.0f;
            up[1] = 1.0f;
            up[2] = 0.0f;
            DIR *= DAT_004e1348 / DIR.Length();
            AnimCross(up, &F(0x1390, float), &F(0x1384, float));
            if (F(0x13ec, int) == 1) {
                int n = F(0x13f0, int);
                F(0x13f8 + n * 8, unsigned int) = idx;
                F(0x13fc + n * 8, int) = time;
                F(0x13f0, int) = n + 1;
            }
        }
    }
tail:
    int cur = F(0x136c, int);
    if (-1 < cur && F(0x1374, int) != 0) {
        int b2 = *(int*)(car + 0x24);
        AnimVec dv;
        dv.x = *(float*)(b2 + 0x14) - F(0x1378, float);
        rate = F(0x234, float);
        dv.y = *(float*)(b2 + 0x18) - F(0x137c, float);
        dv.z = *(float*)(b2 + 0x1c) - F(0x1380, float);
        f4 = (float)sqrt(dv.y * dv.y + dv.z * dv.z + dv.x * dv.x);
        if (f4 < F(0x244, float) + F(0x244, float)) {
            if (F(0x244, float) < f4) {
                f4 = (f4 - F(0x244, float)) / F(0x244, float);
                rate = f4 * F(0x234, float) + (DAT_004e1348 - f4) * F(0x238, float);
            } else {
                rate = F(0x238, float);
            }
        }
        f5 = (float)dt * DAT_004e1400;
        r = REC(cur);
        F(0x1370, int) = F(0x1370, int) + dt;
        f4 = (f5 * F(0x23c, float) * rate) / F(0x234, float) + F(0x13a0, float);
        F(0x13a0, float) = f4;
        F(0x13a0, float) = (float)(f4 - floor(f4));
        f5 = f5 * rate + F(0x139c, float);
        f4 = r[6];
        F(0x139c, float) = f5;
        if (f4 < f5) {
            F(0x1374, int) = 0;
            F(0x136c, int) = -1;
            return;
        }
        f5 = f5 / r[6];
        POS = *(AVec*)&r[3];
        POS -= *(AVec*)&r[0];
        POS *= f5;
        POS += *(AVec*)&r[0];

        AnimVec lo;
        AnimVec hi;
        AnimSphere sph;                // x y z radius
        lo = POS;
        hi = POS;
        float rad = DAT_004e13f4 + DAT_004e13f4;
        lo.x -= DAT_004e13dc;
        lo.y -= rad;
        lo.z -= DAT_004e13dc;
        hi.x += DAT_004e13dc;
        hi.y += rad;
        hi.z += DAT_004e13dc;
        sph.x = POS.x;
        sph.y = POS.y;
        sph.z = POS.z;
        sph.r = rad;
        AnimTrace* trace = (AnimTrace*)((BYTE*)this + 0x13b8);
        ((AnimalTrace*)trace)->Erase(trace->first, trace->last);
        trace->mode = 0xf;
        trace->sphere = &sph;
        trace->lo.x = lo.x;
        trace->lo.y = lo.y;
        trace->lo.z = lo.z;
        trace->hi.x = hi.x;
        trace->hi.y = hi.y;
        trace->hi.z = hi.z;
        ((RaceScene*)this)->FUN_00411520((S6Collector*)trace, (S6Sphere*)&sph);

        int obj = *(int*)(F(0x13a4, int) + 0xc);
        AnimVec a;
        AnimVec b;
        a = POS;
        a.y += DAT_004e13f8;
        b = POS;
        b.y += DAT_004e13f8;
        AVec off1 = ((AnimDir*)&DIR)->Scale(*(float*)(obj + 0xb4));
        a.x = a.x + off1.x;
        a.y = a.y + off1.y;
        a.z = a.z + off1.z;
        AVec off2 = ((AnimDir*)&DIR)->Scale(*(float*)(obj + 0xa8));
        b.x = b.x + off2.x;
        b.y = b.y + off2.y;
        b.z = b.z + off2.z;
        AnimVec n1;
        AnimVec n2;
        AnimVec down;
        down.y = DAT_004e13f8 * DAT_004e13fc;
        down.x = 0.0f;
        down.z = 0.0f;
        float h1 = ((AnimalTrace*)trace)->Cast(0.1f, &a.x, &down.x, &n1.x, 0);
        float h2 = ((AnimalTrace*)trace)->Cast(0.1f, &b.x, &down.x, &n2.x, 0);
        ((AnimalPart*)F(0x13a4, int))->SetColour(F(0x13a0, float));

        float* ov = (float*)(obj + 0x40);
        float* o2 = (float*)(obj + 0x20);
        if (h1 < DAT_004e1348 && h2 < DAT_004e1348) {
            *(AVec*)ov = n1;
            *(AVec*)ov -= n2;
            *(AVec*)ov *= DAT_004e1348 / ((AVec*)ov)->Length();
            *(AVec*)o2 = *(AVec*)&F(0x1384, float);
            AnimCross(ov, o2, (float*)(obj + 0x30));
            *(float*)(obj + 0x14) = F(0x1378, float);
            *(float*)(obj + 0x18) = F(0x137c, float);
            *(float*)(obj + 0x1c) = F(0x1380, float);
            *(float*)(obj + 0x18) = (n2.y + n1.y) * DAT_004e138c;
            return;
        }
        *(AVec*)ov = DIR;
        *(AVec*)o2 = *(AVec*)&F(0x1384, float);
        AnimCross(ov, o2, (float*)(obj + 0x30));
        *(float*)(obj + 0x14) = F(0x1378, float);
        *(float*)(obj + 0x18) = F(0x137c, float);
        *(float*)(obj + 0x1c) = F(0x1380, float);
    }
}
