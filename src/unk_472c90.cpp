// ViewMat::SetInverseOf and the Rain weather effect (0x472c90-0x47419f). Map:
// analysis/range-46fdf0.md section 3.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_47b4c0.h"
#include "unk_474c50.h"
#include "unk_472c90.h"
#include "unk_479170.h"
#include <math.h>
#include <stdlib.h>

// FUNCTION: RALLYTROPHY 0x472c90
void ViewMat::SetInverseOf(const ViewMat& src)
{
    m[0] = src.m[0];
    m[1] = src.m[4];
    m[2] = src.m[8];
    m[3] = src.m[3];
    m[4] = src.m[1];
    m[5] = src.m[5];
    m[6] = src.m[9];
    m[7] = src.m[7];
    m[8] = src.m[2];
    m[9] = src.m[6];
    m[10] = src.m[10];
    m[11] = src.m[11];
    m[12] = src.m[12];
    m[13] = src.m[13];
    m[14] = src.m[14];
    m[15] = src.m[15];
}

// Writing the normal through a Vec3 pointer gives the original single induction pointer per vertex field.
static inline void ClearVec(Vec3* p)
{
    p->x = 0;
    p->y = 0;
    p->z = 0;
}

// original: FUN_00472d00
// NONMATCHING: RALLYTROPHY 0x472d00
// 1425 vs 1434 bytes; every call, EH state and store is present. Open: the original does not forward the
// new[] results (m->vertices / m->indices are re-read from the mesh after the index loop; ours keeps them in
// a stack temp), keeps the index loop counter in memory, and stores queryKind (0x11) right after the vector init.
RainEffect::RainEffect(TextureLoader* loader, int dropCount_, int splashCount_) : queryKind(0x11)
{
    dropCount = dropCount_;
    dropDrawCount = dropCount_;
    splashCount = splashCount_;
    splashDrawCount = splashCount_;
    material = new Sprite;
    material->texture = loader->Load("data/global/weather/raindrops.tga", 0x40);
    material->unk98 = 0xd;
    material->unk9c = 2;
    material->unk18 = 5;
    material->unk1c = 6;
    material->unk60[0] = 1.0f;
    material->unk60[1] = 1.0f;
    material->unk60[2] = 1.0f;
    material->unk60[3] = 1.0f;
    material->unk70[0] = 0.0f;
    material->unk70[1] = 0.0f;
    material->unk70[2] = 0.0f;
    material->unk70[3] = 0.0f;
    material->unk24 = 4;
    material->unk84 = 0xf;
    int i;
    Mesh* m = new Mesh;
    dropMesh = m;
    m->material = material;
    m->vertices = new EffectVertex[dropCount_ * 4];
    m->indices = new WORD[dropCount_ * 6];
    m->unk24 = 0;
    WORD w = (WORD)dropCount;
    m->primitiveCount = w * 2;
    m->vertexCount = w * 4;
    WORD* idx = m->indices;
    for (i = 0; i < dropCount_; i++) {
        int b = i << 2;
        *idx++ = b;
        *idx++ = b + 1;
        *idx++ = b + 2;
        *idx++ = b;
        *idx++ = b + 2;
        *idx++ = b + 3;
    }
    EffectVertex* v = m->vertices;
    for (i = 0; i < dropCount_; i++) {
        v->color = -1;
        ClearVec(&v->normal);
        v->u = 0;
        v->v = 0.5f;
        v++;
        v->color = -1;
        ClearVec(&v->normal);
        v->u = 1.0f;
        v->v = 0.5f;
        v++;
        v->color = -1;
        ClearVec(&v->normal);
        v->u = 1.0f;
        v->v = 0;
        v++;
        v->color = -1;
        ClearVec(&v->normal);
        v->u = 0;
        v->v = 0;
        v++;
    }
    m = new Mesh;
    m->material = material;
    splashMesh = m;
    m->vertices = new EffectVertex[splashCount_ * 4];
    m->indices = new WORD[splashCount_ * 6];
    m->unk24 = 0;
    w = (WORD)splashCount;
    m->primitiveCount = w * 2;
    m->vertexCount = w * 4;
    idx = m->indices;
    for (i = 0; i < splashCount_; i++) {
        int b = i * 4;
        *idx++ = b;
        *idx++ = b + 1;
        *idx++ = b + 2;
        *idx++ = b;
        *idx++ = b + 2;
        *idx++ = b + 3;
    }
    v = m->vertices;
    for (i = 0; i < splashCount_; i++) {
        v->color = -1;
        ClearVec(&v->normal);
        v->u = 0;
        v->v = 1.0f;
        v++;
        v->color = -1;
        ClearVec(&v->normal);
        v->u = 0.25f;
        v->v = 1.0f;
        v++;
        v->color = -1;
        ClearVec(&v->normal);
        v->u = 0.25f;
        v->v = 0.5f;
        v++;
        v->color = -1;
        ClearVec(&v->normal);
        v->u = 0;
        v->v = 0.5f;
        v++;
    }
    drops = new int[dropCount_ * 3];
    splashes = new RainSplash[splashCount_];
    velocity.x = 0.0f;
    velocity.y = 30.0f;
    velocity.z = 0.0f;
    threshold = 1.0f;
    boxSize = 40.0f;
    streakWidth = 0.03f;
    splashSizeOld = 0.025f;
    splashSizeNew = 0.085f;
    InitDrops();
    tris.reserve(0x200);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4732a0
// SYMBOL: Generated_4732a0
// Plain binary observation: write WeatherEffect's vtable pointer through ECX, then return.
// Keep this standalone ABI body separate so derived constructors retain their original inline codegen.
void Generated_4732a0()
{
    __asm {
        mov dword ptr [ecx], 04e2000h
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x4732b0
// SYMBOL: ??_GWeatherEffect@@UAEPAXI@Z
typedef void Generated_4732b0();

// FUNCTION: RALLYTROPHY 0x4732d0
// SYMBOL: ??_GRainEffect@@UAEPAXI@Z
typedef void Generated_4732d0();

// original: FUN_004732f0
// FUNCTION: RALLYTROPHY 0x4732f0
RainEffect::~RainEffect()
{
    delete material;
    operator delete(drops);
    delete[] splashes;
    delete dropMesh;
    delete splashMesh;
}

static const float kFixedScale = 3.9999390f; // 0x4e2008: 16383 * this = 65535

// original: FUN_004733d0
// FUNCTION: RALLYTROPHY 0x4733d0
void RainEffect::InitDrops()
{
    for (int i = 0; i < dropCount; i++) {
        int* p = &drops[i * 3];
        p[0] = (int)((float)(rand() & 0x3fff) * kFixedScale);
        p[1] = (int)((float)(rand() & 0x3fff) * kFixedScale);
        p[2] = (int)((float)(rand() & 0x3fff) * kFixedScale);
    }
    for (int j = 0; j < splashCount; j++) {
        splashes[j].delay = -(rand() & 0xff);
    }
}

extern int DAT_00501428; // graphics quality: 0 = effects off

// Component-wise copy (the original's three load/store pairs).
inline void Set(Vec3& a, const Vec3& b)
{
    a.x = b.x;
    a.y = b.y;
    a.z = b.z;
}

#pragma auto_inline(off)
TriVector::iterator TriVector::Erase(iterator first, iterator last) { return erase(first, last); }
#pragma auto_inline(on)

#define CROSS() \
    __asm mov ecx, ca \
    __asm mov edx, cb \
    __asm mov eax, cresult \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx] \
    __asm fxch st(2) \
    __asm fsubp st(5), st \
    __asm fsubp st(3), st \
    __asm fsubp st(1), st \
    __asm fxch st(2) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+4] \
    __asm fstp dword ptr [eax+8]

// original: FUN_00473470
// NONMATCHING: RALLYTROPHY 0x473470
// 3318 vs 3324 bytes; same calls, EH states and FPU code. Open: register assignment (original this=ebx saved to
// [ebp-0x124], vertex/device=edi; here this=edi) and the lo/hi/sphere setup and tri-pick temporaries of the splash part.
void RainEffect::Update(RenderDevice* device, Camera* camera, int a, int b, float dt)
{
    if (DAT_00501428 == 0) {
        return;
    }
    int step = FloatToInt(dt * 1000.0f);
    WorldMatrix world;
    world.m[12] = camera->position.x;
    world.m[13] = camera->position.y;
    world.m[14] = camera->position.z;
    Vec3 wind;
    if (camera->hasLens) {
        wind.x = world.m[12] - camera->lensPos[0];
        wind.y = world.m[13] - camera->lensPos[1];
        wind.z = world.m[14] - camera->lensPos[2];
        float scale = camera->lensScale;
        float k = 0.05f / scale;
        wind.x *= k;
        wind.y = wind.y * k;
        wind.z = wind.z * k;
    } else {
        wind.x = 0;
        wind.y = 0;
        wind.z = 0;
    }
    wind += velocity * 0.05f;
    Vec3 qa, qc, qb, qd;
    Vec3 right;
    qa.x = wind.x;
    qa.y = wind.y;
    qa.z = wind.z;
    qb.x = wind.x;
    qb.y = wind.y;
    qb.z = wind.z;
    qc.x = qc.y = qc.z = 0;
    qd.x = qd.y = qd.z = 0;
    Vec3* fwd = &camera->forward;
    Vec3* ca = &camera->up;
    Vec3* cb = fwd;
    Vec3* cresult = &right;
    CROSS();
    float w = streakWidth;
    qa -= (right * w) * 0.5f;
    qb += (right * w) * 0.5f;
    qc += (right * w) * 0.5f;
    qd -= (right * w) * 0.5f;
    float unit = boxSize * 1.5259022e-5f;
    int fall = FloatToInt(dt * velocity.y / unit);
    int offX = FloatToInt(world.m[12] / unit);
    int offY = FloatToInt(world.m[13] / unit);
    int offZ = FloatToInt(world.m[14] / unit);
    int* p = drops;
    EffectVertex* v = dropMesh->vertices;
    int drawn = 0;
    for (int i = 0; i < dropDrawCount; i++, p += 3) {
        float half = boxSize * 0.5f;
        p[1] -= fall;
        if (p[1] < 0) {
            p[1] &= 0xffff;
            p[0] = (int)((float)(rand() & 0x3fff) * 3.9999390f);
            p[2] = (int)((float)(rand() & 0x3fff) * 3.9999390f);
        }
        Vec3 rel;
        rel.x = (float)((p[0] - offX) & 0xffff) * unit;
        rel.y = (float)((p[1] - offY) & 0xffff) * unit;
        rel.z = (float)((p[2] - offZ) & 0xffff) * unit;
        rel.x -= half;
        rel.y -= half;
        rel.z -= half;
        if (rel.z * fwd->z + rel.y * fwd->y + rel.x * fwd->x > threshold) {
            Set(v->position, rel + qa);
            v++;
            Set(v->position, rel + qb);
            v++;
            Set(v->position, rel + qc);
            v++;
            Set(v->position, rel + qd);
            v++;
            drawn++;
        }
    }
    dropMesh->primitiveCount = drawn * 2;
    dropMesh->vertexCount = drawn * 4;
    device->vf46(world.m, 1);
    device->vf48((const float*)&camera->right, 1);
    device->vf51(camera->fov, 1.0f, *(float*)&camera->nearBits, *(float*)&camera->farBits, 1);
    device->SetRenderState(5, 0);
    device->vf42(4);
    device->vf35(material);
    device->vf64(dropMesh);

    // splashes
    RainSplash* s = splashes;
    EffectVertex* sv = splashMesh->vertices;
    QuerySphere sphere;
    sphere.center.x = camera->position.x;
    sphere.center.y = camera->position.y;
    sphere.center.z = camera->position.z;
    sphere.radius = 20.0f;
    Vec3 lo, hi;
    Set(lo, camera->position);
    Set(hi, camera->position);
    lo.x -= 20.0f;
    lo.y -= 20.0f;
    lo.z -= 20.0f;
    hi.x += 20.0f;
    hi.y += 20.0f;
    hi.z += 20.0f;
    Tris()->Clear();
    queryKind = 0xf;
    querySphere = &sphere;
    queryMin.x = lo.x;
    queryMin.y = lo.y;
    queryMin.z = lo.z;
    queryMax.x = hi.x;
    queryMax.y = hi.y;
    queryMax.z = hi.z;
    float eyeDot = camera->eyePosition.z * fwd->z + camera->eyePosition.y * fwd->y + camera->eyePosition.x * fwd->x;
    bool searched = false;
    bool found = false;
    int shown = 0;
    for (int j = 0; j < splashDrawCount; j++, s++) {
        s->delay -= step;
        if (s->delay < 0) {
            if (!searched) {
                searched = true;
                ((TrackGrid*)b)->CollectNear(Tris(), &sphere);
                if (tris.size()) {
                    found = true;
                }
            }
            if (!found) {
                continue;
            }
            s->delay &= 0xff;
            CollTri* tri = tris[rand() % tris.size()];
            Vec3 e = tri->v1->pos;
            e -= tri->v0->pos;
            float inv = 1.0f / (float)sqrt(e.z * e.z + e.y * e.y + e.x * e.x);
            e.x *= inv;
            e.y *= inv;
            e.z *= inv;
            Vec3 axis;
            ca = &tri->normal;
            cb = &e;
            cresult = &axis;
            CROSS();
            float u = (float)(rand() & 0x3ff) * (1.0f / 1024.0f);
            float f1 = 1.0f - u;
            float f2 = (float)(rand() & 0x3ff) * f1 * (1.0f / 1024.0f);
            float f3 = f1 - f2;
            Vec3 pos;
            Set(pos, tri->v0->pos * u + tri->v1->pos * f2 + tri->v2->pos * f3);
            pos += tri->normal * 0.02f;
            Set(s->pos, pos);
            Set(s->axisA, axis);
            Set(s->axisB, e);
            Set(s->normal, tri->normal);
        }
        if (s->pos.z * fwd->z + s->pos.y * fwd->y + s->pos.x * fwd->x >= eyeDot) {
            Vec3 A = s->axisA;
            Vec3 B = s->axisB;
            float t = (float)s->delay * 0.003921569f;
            float size = t * splashSizeOld + (1.0f - t) * splashSizeNew;
            A.x *= size;
            A.y *= size;
            A.z *= size;
            B.x *= size;
            B.y *= size;
            B.z *= size;
            Set(sv[0].position, (s->pos - A) + B);
            Set(sv[1].position, (s->pos + A) + B);
            Set(sv[2].position, (s->pos + A) - B);
            Set(sv[3].position, (s->pos - A) - B);
            Set(sv[0].normal, s->normal);
            Set(sv[1].normal, s->normal);
            Set(sv[2].normal, s->normal);
            Set(sv[3].normal, s->normal);
            BYTE alpha = (BYTE)s->delay;
            ((BYTE*)&sv[0].color)[3] = alpha;
            ((BYTE*)&sv[1].color)[3] = alpha;
            ((BYTE*)&sv[2].color)[3] = alpha;
            ((BYTE*)&sv[3].color)[3] = alpha;
            sv += 4;
            shown++;
        }
    }
    splashMesh->primitiveCount = shown * 2;
    splashMesh->vertexCount = shown * 4;
    device->vf44(1);
    device->vf64(splashMesh);
}

// FUNCTION: RALLYTROPHY 0x474170
// SYMBOL: _$E26
typedef void Generated_474170();

// FUNCTION: RALLYTROPHY 0x474190
// SYMBOL: _$E25
typedef void Generated_474190();
