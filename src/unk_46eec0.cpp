// CityLight (0x46eec0-0x46fa7f): street-lamp glow and lens flare sprites of the city stages.
// Map: analysis/range-46d830.md section 6.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_47b4c0.h"
#include "unk_474c50.h"
#include "unk_46eec0.h"
#include <math.h>

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

__forceinline void CopyV(Vec3& d, const Vec3& s)
{
    int x = *(const int*)&s.x, y = *(const int*)&s.y, z = *(const int*)&s.z;
    *(int*)&d.x = x;
    *(int*)&d.y = y;
    *(int*)&d.z = z;
}

struct TmpLV : public ViewMat {
    TmpLV(LightSource* c) {
        m[0] = c->matrix.m[0];
        m[1] = c->matrix.m[1];
        m[2] = c->matrix.m[2];
        m[3] = c->matrix.m[3];
        m[4] = c->matrix.m[4];
        m[5] = c->matrix.m[5];
        m[6] = c->matrix.m[6];
        m[7] = c->matrix.m[7];
        m[8] = c->matrix.m[8];
        m[9] = c->matrix.m[9];
        m[10] = c->matrix.m[10];
        m[11] = c->matrix.m[11];
        *(volatile float*)&m[12] = c->matrix.translation.x;
        *(volatile float*)&m[13] = c->matrix.translation.y;
        *(volatile float*)&m[14] = c->matrix.translation.z;
        *(volatile float*)&m[15] = c->matrix.w;
        m[12] = c->position.x;
        *(volatile float*)&m[13] = c->position.y;
    }
};
// NONMATCHING: RALLYTROPHY 0x46eec0
// The original copies the child matrix with 16 strictly sequential ecx load/store pairs and keeps three
// overwritten translation stores. Volatile destination stores preserve those three, and scalar translation
// copies avoid three indirect stores; current candidate is 975/1044 bytes with 630 differing bytes. The
// corner helper still uses a different register schedule.
// Tried 2026-10-31: CopyV int copy for corner stores plus this->corner[k] for corners 1-3 (c only for corner 0) took 101->~19 insns; kind/flags barriers and load permutations done; still differs in EH state store placement (early vs just before Concat), flags/position14 register choice, and x87 operand order of the first corner add.
// Tried 2026-10-31 re-pass: local ViewMat-derived TmpLV with inline copy ctor (EH state 2 now lands before Concat as in the original) and tmp.m[14] stored after it took 19->14 insns; sweeps of flags/position14 load order, glow/flare/matrix store order, volatile tmp stores, and AddRC/AddCR x87 operand helpers did not move the remaining diffs (add esi,0x20 slot, edx/eax roles for position14/flags, first-component fld order of corners 0 and 1).
CityLight::CityLight(LightSource* source, TextureLoader* loader, Sprite* glowSprite, Sprite* flareSprite)
{
    unk8 = 0;
    Name() = source->Name();
    glow = glowSprite;
    flare = flareSprite;
    matrix = source->matrix;
    matrix.translation.x = source->position.x;
    int ty = *(int*)&source->position.y;
    *(volatile int*)&kind = 5;
    *(int*)&matrix.translation.y = ty;
    matrix.translation.z = source->position.z;
    LightSource* child = source->child;
    if (child) {
        TmpLV tmp(child);
        tmp.m[14] = child->position.z;
        ((LightView*)&matrix)->Concat(tmp);
    }
    int px = *(int*)&matrix.translation.x;
    int pz = *(int*)&matrix.translation.z;
    int py = *(int*)&matrix.translation.y;
    BYTE f = flags;
    *(int*)&position14.x = px;
    *(int*)&position14.z = pz;
    *(int*)&position14.y = py;
    f &= 0xfd;
    BYTE g = f | 1;
    *(volatile BYTE*)&flags = f;
    *(volatile BYTE*)&flags = g;
    matrix.translation.x = matrix.translation.y = matrix.translation.z = 0;
    unkb8 = 0;
    unkbc = 0;
    unkc0 = 0;
    unkc4 = 3.0f;
    unkc8 = 3.0f;
    if (loader) {
        if (glow && glow->texture == 0) {
            glow->texture = loader->Load("data/global/flares/cityglow.tga", 0x40);
        }
        if (flare && flare->texture == 0) {
            flare->texture = loader->Load("data/global/flares/cityflare.tga", 0x40);
        }
    }
    exponent = 1.0f;
    scale = 1.0f;
    size = 1.5f;
    bias = 0;
    Vec3 up = *(Vec3*)&matrix.m[4];
    Vec3 right = *(Vec3*)&matrix.m[0];
    Vec3* c = corner;
    CopyV(c[0], up * -0.75f);
    CopyV(corner[3], up * -0.75f);
    CopyV(corner[1], up * 0.75f);
    CopyV(corner[2], up * 0.75f);
    c[0] += right * 1.5f;
    corner[1] += right * 1.5f;
    corner[2] -= right * 1.5f;
    corner[3] -= right * 1.5f;
}

// FUNCTION: RALLYTROPHY 0x46f2e0
// SYMBOL: ??_GCityLight@@UAEPAXI@Z
typedef void Generated_46f2e0();

// FUNCTION: RALLYTROPHY 0x46f300
CityLight::~CityLight()
{
}

inline void ScaleVec(Vec3& v, float k)
{
    v.x = k * v.x;
    v.y = v.y * k;
    v.z = v.z * k;
}

inline float Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Rotated texture coordinates of the flare quad: the unit square turned by (c, s) about its centre.
inline void FlareUV(EffectVertex* v, float c, float s)
{
    volatile float ms = -s;
    v[0].u = 0.5f - c - s;
    v[0].v = 0.5f - ms - c;
    v[1].u = c + 0.5f - s;
    v[1].v = ms + 0.5f - c;
    v[2].u = s + c + 0.5f;
    v[2].v = ms + c + 0.5f;
    v[3].u = s + (0.5f - c);
    v[3].v = c + (0.5f - ms);
}

// NONMATCHING: RALLYTROPHY 0x46f360
// 1777 vs 1767 bytes. The eye-relative vector follows the target's component load/subtract/store order; separately
// named right/up flare-angle contributions preserve their scale-before-add sequence. Volatile flare intermediates
// remain at different stack slots, and projection, normalization, and UV code still differ.
// Tried 2026-10-30 night: shared float temp/bit-compare for near plane, pos*m operand order, forceinline ScaleVec for normalise and R/U, Vec3 d*inv, double inv, quad normal x,y,z order, local declaration order (204 insns, only the inline inv multiply changed); still differs: original keeps inv on the FPU stack (fld y; fmul st,st(1)), frame is 4 bytes larger (extra 8-byte slot before depth), depth/near compare via [ebp+0xc] float copy, return-path layout.
void CityLight::Render(RenderDevice* device, Camera* camera)
{
    Vec3 d;
    d.x = position.x;
    d.y = position.y;
    d.z = position.z;
    d -= camera->eyePosition;
    float nearF = *(float*)&camera->nearBits;
    if (d.x * camera->forward.x + d.y * camera->forward.y + d.z * camera->forward.z <= nearF) {
        return;
    }
    const float* m = device->GetViewMatrix();
    float depth = m[10] * position.z + m[6] * position.y + m[2] * position.x + m[14] - size;
    int nearZ = camera->nearBits;
    if (*(int*)&depth <= nearZ) {
        *(int*)&depth = nearZ;
    }
    float inv = 1.0f / (float)sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
    d.x = inv * d.x;
    d.y = d.y * inv;
    d.z = d.z * inv;
    EffectVertex quad[4];
    Vec3 c = *(Vec3*)&position;
    c -= d * 1.0f;
    for (int i = 0; i < 4; i++) {
        quad[i].normal.x = axis.x;
        quad[i].normal.z = axis.z;
        quad[i].normal.y = axis.y;
        quad[i].v2 = quad[i].u2 = 0;
        quad[i].position.x = c.x;
        quad[i].position.y = c.y;
        quad[i].position.z = c.z;
        quad[i].color = -1;
    }
    quad[0].u = quad[3].u = quad[0].v = quad[1].v = 0;
    quad[1].u = quad[2].u = quad[2].v = quad[3].v = 1.0f;
    Vec3 R = camera->right;
    Vec3 U = camera->up;
    ScaleVec(R, size);
    ScaleVec(U, size);
    quad[0].position -= R;
    quad[0].position += U;
    quad[1].position += R;
    quad[1].position += U;
    quad[2].position += R;
    quad[2].position -= U;
    quad[3].position -= R;
    quad[3].position -= U;
    device->DrawQuad(quad, glow, depth);
    float f = Dot(*(Vec3*)&axis, -d);
    if (*(unsigned*)&f & 0x80000000) {
        return;
    }
    if (*(int*)&f < 0) {
        f = 0;
    } else if (*(int*)&f > 0x3f800000) {
        f = 1.0f;
    }
    float e = exponent;
    float a = (float)pow(f, e) * scale + bias;
    if (*(int*)&a < 0) {
        return;
    }
    if (*(int*)&a > 0x3f800000) {
        a = 1.0f;
    } else if (a <= 0.001f) {
        return;
    }
    c = *(Vec3*)&position;
    int v = FloatToInt(a * 255.0f) & 0xff;
    DWORD col = (v << 8) | v;
    col = col | (col << 16);
    for (int j = 0; j < 4; j++) {
        quad[j].position = c;
        quad[j].position += corner[j];
    }
    quad[0].color = col;
    quad[1].color = col;
    quad[2].color = col;
    quad[3].color = col;
    float rightAngle = (d.z * camera->right.z + d.y * camera->right.y + d.x * camera->right.x) * 0.7f;
    float upAngle = (d.z * camera->up.z + d.y * camera->up.y + d.x * camera->up.x) * 0.7f;
    float ang = rightAngle + upAngle;
    ang = ang * 3.1415927f;
    float cs = (float)cos(ang) * 0.5f;
    float sn = (float)sin(ang) * 0.5f;
    FlareUV(quad, cs, sn);
    device->DrawQuad(quad, flare, depth);
    FlareUV(quad, cs, -sn);
    device->DrawQuad(quad, flare, depth);
}

// Module end: the STL header statics guard and its atexit registration (the stub of this file).

// FUNCTION: RALLYTROPHY 0x46fa50
// SYMBOL: _$E23
typedef void Generated_46fa50();

// FUNCTION: RALLYTROPHY 0x46fa70
// SYMBOL: _$E22
typedef void Generated_46fa70();
