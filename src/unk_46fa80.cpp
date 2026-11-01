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
// Evidence: ev_098aeda40f3866cc5d46de6aa9df644e5a1638f8a9cce76a6f260f691961b0eb. The 428-byte target's loop and helper calls are documented in analysis/range-46d830.md.
// A WORD-sized loop count lowers the candidate from 424/290 to 431/229 bytes/differences, matching the target's 16-bit
// count load. The remaining mismatch includes a larger target frame, `this` in edi, and uv2.x read from the subtraction.
// Tried 2026-10-31 re-pass: ~25 more (const-ref temp, split/hoisted uv decls, inline scale.x/y in loop 52->50 applied, int-bit stores, while loop, store orders); still 50 insns: this=esi not edi (original pushes esi only inside the n>0 guard), uv2.x taken from the call-result pointer via fld/fst not an int copy.
// Tried 2026-10-30 night: ~35 variants; assigned (not constructed) uv1/uv2 locals with int count and for-loop gave 65->52 insns; const-ref/pointer to the subtraction temp, if/do-while, ushort count, int-copy stores were no better; still differs in this=edi vs esi allocation, frame 0x48, and uv2 stored via fst instead of an eax-pointer reload.
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
    Vec2* s = saved;
    for (int i = 0; i < n; i++) {
        Vec2 uv1;
        Vec2 uv2;
        uv1 = (*s * scale.x) - d1;
        uv2 = (*s * scale.y) - d2;
        uv2.x += 0.5f; uv2.y += 0.5f;
        v->u = uv1.x; v->v = uv1.y; v->u2 = uv2.x; v->v2 = uv2.y;
        s++;
        v++;
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x46fdc0
// SYMBOL: _$E24
typedef void Generated_46fdc0();

// FUNCTION: RALLYTROPHY 0x46fde0
// SYMBOL: _$E23
typedef void Generated_46fde0();
