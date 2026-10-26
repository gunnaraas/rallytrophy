// Vec2::operator+ and TexScroller (0x476490-0x47669f, vtable 0x4e2050). Map: analysis/range-4741a0.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_476490.h"
#include <math.h>
#include <stdlib.h>

// FUNCTION: RALLYTROPHY 0x476490
// SYMBOL: ??HVec2@@QBE?AV0@ABV0@@Z
Vec2 Vec2::operator+(const Vec2& o) const
{
    Vec2 r;
    r.x = o.x + x;
    r.y = o.y + y;
    return r;
}

// FUNCTION: RALLYTROPHY 0x4764c0
TexScroller::TexScroller(DynamicMesh* m, float su, float sv, float spu, float spv)
{
    scaleU = su;
    scaleV = sv;
    int n = m->vertexCount;
    speedU = spu;
    speedV = spv;
    mesh = m;
    base = (float*)operator new(n * 4);
    offsetU = 0.0f;
    offsetV = 0.5f;
    if (n > 0) {
        float* b = base;
        float* p = &m->vertices->v;
        do {
            float a = p[-1];
            float c = p[0];
            *b++ = c;
            p[-1] = a * su;
            p[0] = c;
            p[1] = a * sv;
            p[2] = c;
            p += 11;
        } while (--n);
    }
}

// FUNCTION: RALLYTROPHY 0x476590
// SYMBOL: ??_GTexScroller@@UAEPAXI@Z
typedef void Generated_476590();

// FUNCTION: RALLYTROPHY 0x4765b0
TexScroller::~TexScroller()
{
    delete[] base;
}

// FUNCTION: RALLYTROPHY 0x4765d0
void TexScroller::Update()
{
    StageClock* clock = (StageClock*)DAT_0056b0ac;
    if (clock != 0) {
        stamp = clock->time;
        float t = (float)(DAT_0056b0d0 & 0xffff);
        float du = t * speedU * 1.52587890625e-05f;
        scrollU = du;
        float dv = t * speedV * 1.52587890625e-05f;
        scrollV = dv;
        int n = mesh->vertexCount;
        EffectVertex* v = mesh->vertices;
        if (n > 0) {
            float* b = base;
            for (int i = 0; i < n; i++, v++) {
                v->v = scaleU * b[i] - du + offsetU;
                v->v2 = scaleV * b[i] - dv + offsetV;
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x476670
// SYMBOL: _$E23
typedef void Generated_476670();

// FUNCTION: RALLYTROPHY 0x476690
// SYMBOL: _$E22
typedef void Generated_476690();
