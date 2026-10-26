// WaterAnimator (0x46fa80-0x46fdbb, vtable 0x4e1fac) and the module end. Map: analysis/range-46d830.md section 7.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_476490.h"
#include "unk_46fa80.h"
#include <math.h>
#include <stdlib.h>

// Float to int with the x87 default rounding (inline assembler, as in the original).
inline int FloatToInt(float f)
{
    int i;
    __asm {
        fld f
        fistp i
    }
    return i;
}

// original: FUN_0046fa80
// FUNCTION: RALLYTROPHY 0x46fa80
WaterAnimator::WaterAnimator(DynamicMesh* m, WaterOwner* o, float r, Vec2* s, Vec2* v1, Vec2* v2)
{
    int n = m->vertexCount;
    mesh = m;
    owner = o;
    Vec2* copy = new Vec2[n];
    rate = r;
    scale.x = s->x;
    scale.y = s->y;
    velocity1.x = v1->x;
    velocity1.y = v1->y;
    saved = copy;
    velocity2.x = v2->x;
    velocity2.y = v2->y;
    for (int i = 0; i < n; i++) {
        float* p = &m->vertices[i].v;
        copy[i].x = p[-1];
        copy[i].y = p[0];
    }
}

// original: FUN_0046fb70
// FUNCTION: RALLYTROPHY 0x46fb70
// SYMBOL: ??_GWaterAnimator@@UAEPAXI@Z
typedef void Generated_46fb70();

// original: FUN_0046fb90
// FUNCTION: RALLYTROPHY 0x46fb90
WaterAnimator::~WaterAnimator()
{
    delete[] saved;
}

// original: FUN_0046fc10
// NONMATCHING: RALLYTROPHY 0x46fc10
// Same structure and EH states (24 bytes shorter than the original's 428: the original keeps `this` in edi and
// the count in eax, loads the clock time before the tick count, and reads the uv2 temporary's x straight from
// the call result). Not reproduced after ~25 variants.
void WaterAnimator::Update()
{
    StageClock* clock = (StageClock*)DAT_0056b0ac;
    stamp = clock->time;
    float t = (float)(DAT_0056b0d0 & 0xffff);
    int frame = FloatToInt(t * rate * 0.00048828125f);
    mesh->material->texture = owner->textures[frame & 31];
    Vec2 d1 = (velocity1 * (float)(DAT_0056b0d0 & 0xffff)) / 65536.0f;
    Vec2 d2 = (velocity2 * (float)(DAT_0056b0d0 & 0xffff)) / 65536.0f;
    EffectVertex* v = mesh->vertices;
    int n = mesh->vertexCount;
    if (n > 0) {
        Vec2* s = saved;
        float k1 = scale.x;
        float k2 = scale.y;
        do {
            Vec2 uv1;
            uv1 = (*s * k1) - d1;
            Vec2 uv2 = (*s * k2) - d2;
            uv2.x += 0.5f;
            uv2.y += 0.5f;
            v->u = uv1.x;
            v->v = uv1.y;
            v->u2 = uv2.x;
            v->v2 = uv2.y;
            s++;
            v++;
        } while (--n);
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x46fdc0
// SYMBOL: _$E24
typedef void Generated_46fdc0();

// FUNCTION: RALLYTROPHY 0x46fde0
// SYMBOL: _$E23
typedef void Generated_46fde0();
