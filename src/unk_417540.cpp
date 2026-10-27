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

#define F(o, t) (*(t*)((BYTE*)this + (o)))
#define DIR (*(AVec*)((BYTE*)this + 0x1390))
#define REC(i) ((float*)(F(0x13ac, int) + (i) * 0x24))

// NONMATCHING: RALLYTROPHY 0x417540
// 2234 vs 2332 bytes. The three cross products are the inline asm helper and dir/ov use vector ops like the
// original; still short elsewhere (the record lerp and sphere setup) and the frame is 4 bytes larger.
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
        float dv[3];
        dv[0] = *(float*)(b2 + 0x14) - F(0x1378, float);
        rate = F(0x234, float);
        dv[1] = *(float*)(b2 + 0x18) - F(0x137c, float);
        dv[2] = *(float*)(b2 + 0x1c) - F(0x1380, float);
        f4 = (float)sqrt(dv[1] * dv[1] + dv[2] * dv[2] + dv[0] * dv[0]);
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
        f4 = r[4];
        float f6 = r[5];
        F(0x1378, float) = r[3];
        F(0x137c, float) = f4;
        F(0x1380, float) = f6;
        F(0x1378, float) = F(0x1378, float) - r[0];
        F(0x137c, float) = F(0x137c, float) - r[1];
        F(0x1380, float) = F(0x1380, float) - r[2];
        F(0x1378, float) = f5 * F(0x1378, float);
        F(0x137c, float) = f5 * F(0x137c, float);
        F(0x1380, float) = f5 * F(0x1380, float);
        F(0x1378, float) = F(0x1378, float) + r[0];
        F(0x137c, float) = F(0x137c, float) + r[1];
        F(0x1380, float) = F(0x1380, float) + r[2];

        float sph[4];                  // x y z radius
        float lo[3];
        float hi[3];
        sph[0] = F(0x1378, float);
        lo[0] = sph[0] - DAT_004e13dc;
        sph[1] = F(0x137c, float);
        sph[2] = F(0x1380, float);
        sph[3] = DAT_004e13f4 + DAT_004e13f4;
        lo[1] = sph[1] - sph[3];
        lo[2] = sph[2] - DAT_004e13dc;
        hi[0] = sph[0] + DAT_004e13dc;
        hi[1] = sph[3] + sph[1];
        hi[2] = sph[2] + DAT_004e13dc;
        AnimalTrace* trace = (AnimalTrace*)((BYTE*)this + 0x13b8);
        trace->Erase(F(0x13bc, void*), F(0x13c0, void*));
        F(0x13e8, int) = 0xf;
        F(0x13e4, float*) = sph;
        F(0x13cc, float) = lo[0];
        F(0x13d0, float) = lo[1];
        F(0x13d8, float) = hi[0];
        F(0x13d4, float) = lo[2];
        F(0x13dc, float) = hi[1];
        F(0x13e0, float) = hi[2];
        ((RaceScene*)this)->FUN_00411520((S6Collector*)trace, (S6Sphere*)sph);

        int obj = *(int*)(F(0x13a4, int) + 0xc);
        float a[3];
        float b[3];
        a[0] = F(0x1378, float);
        a[1] = F(0x137c, float) + DAT_004e13f8;
        a[2] = F(0x1380, float);
        b[1] = F(0x137c, float) + DAT_004e13f8;
        b[0] = a[0];
        b[2] = a[2];
        Vec3 off1 = (*(Vec3*)&F(0x1390, Vec3)) * *(float*)(obj + 0xb4);
        a[0] = a[0] + off1.x;
        a[1] = a[1] + off1.y;
        a[2] = a[2] + off1.z;
        Vec3 off2 = (*(Vec3*)&F(0x1390, Vec3)) * *(float*)(obj + 0xa8);
        b[0] = b[0] + off2.x;
        b[1] = b[1] + off2.y;
        b[2] = b[2] + off2.z;
        float down[3];
        down[1] = DAT_004e13f8 * DAT_004e13fc;
        down[0] = 0.0f;
        down[2] = 0.0f;
        float n1[3];
        float n2[3];
        float h1 = trace->Cast(0.1f, a, down, n1, 0);
        float h2 = trace->Cast(0.1f, b, down, n2, 0);
        ((AnimalPart*)F(0x13a4, int))->SetColour(F(0x13a0, float));

        float* ov = (float*)(obj + 0x40);
        float* o2 = (float*)(obj + 0x20);
        if (h1 < DAT_004e1348 && h2 < DAT_004e1348) {
            *(AVec*)ov = *(AVec*)n1;
            *(AVec*)ov -= *(AVec*)n2;
            *(AVec*)ov *= DAT_004e1348 / ((AVec*)ov)->Length();
            *(AVec*)o2 = *(AVec*)&F(0x1384, float);
            AnimCross(ov, o2, (float*)(obj + 0x30));
            *(float*)(obj + 0x14) = F(0x1378, float);
            *(float*)(obj + 0x18) = F(0x137c, float);
            *(float*)(obj + 0x1c) = F(0x1380, float);
            *(float*)(obj + 0x18) = (n2[1] + n1[1]) * DAT_004e138c;
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
