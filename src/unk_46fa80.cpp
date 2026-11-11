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
static inline void CopyV2(Vec2& d, const Vec2& o) { d.x = o.x; d.y = o.y; }
struct ClockI { BYTE pad[0x24]; int time; };
// FUNCTION: RALLYTROPHY 0x46fc10
// Evidence: ev_098aeda40f3866cc5d46de6aa9df644e5a1638f8a9cce76a6f260f691961b0eb; loop and helper calls are documented in
// analysis/range-46d830.md. What made it exact (2026-11-11):
// - the texture goes to the material's second slot (+4), not +0 (the earlier body wrote the wrong field);
// - the clock stamp is read as an int into a local (a float-typed copy is scheduled after the tick conversion);
// - the frame index is masked into its own int and the texture held in a local before the store;
// - `saved[i]` is indexed (pointer set up after the n > 0 guard, this in edi, esi pushed inside the guard) and v is
//   stepped in the for increment (edi stepped before esi);
// - uv2 is assigned field by field (CopyV2), so x + 0.5 is taken straight from the subtraction result.
void WaterAnimator::Update()
{
    int now = ((ClockI*)DAT_0056b0ac)->time;
    *(int*)&stamp = now;
    float t = (float)(DAT_0056b0d0 & 0xffff);
    int frame = FloatToInt(t * rate * 0.00048828125f) & 31;
    Texture* tex = owner->textures[frame];
    *(Texture**)mesh->material->unk04 = tex;
    Vec2 d1 = (velocity1 * (float)(DAT_0056b0d0 & 0xffff)) / 65536.0f;
    Vec2 d2 = (velocity2 * (float)(DAT_0056b0d0 & 0xffff)) / 65536.0f;
    EffectVertex* v = mesh->vertices;
    int n = mesh->vertexCount;
    for (int i = 0; i < n; i++, v++) {
        Vec2 uv1;
        Vec2 uv2;
        uv1 = (saved[i] * scale.x) - d1;
        CopyV2(uv2, (saved[i] * scale.y) - d2);
        uv2.x += 0.5f; uv2.y += 0.5f;
        v->u = uv1.x; v->v = uv1.y; v->u2 = uv2.x; v->v2 = uv2.y;
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x46fdc0
// SYMBOL: _$E24
typedef void Generated_46fdc0();

// FUNCTION: RALLYTROPHY 0x46fde0
// SYMBOL: _$E23
typedef void Generated_46fde0();
