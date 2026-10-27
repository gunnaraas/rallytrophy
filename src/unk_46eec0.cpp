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

// NONMATCHING: RALLYTROPHY 0x46eec0
// The original copies the child matrix with 16 strictly sequential ecx load/store pairs and keeps three
// overwritten translation stores. Volatile destination stores preserve those three, but the ctor remains
// NONMATCHING (979/1044 bytes, 746 differing); the corner helper also uses a different register schedule.
CityLight::CityLight(LightSource* source, TextureLoader* loader, Sprite* glowSprite, Sprite* flareSprite)
{
    unk8 = 0;
    Name() = source->Name();
    glow = glowSprite;
    flare = flareSprite;
    matrix = source->matrix;
    matrix.translation.x = source->position.x;
    matrix.translation.y = source->position.y;
    matrix.translation.z = source->position.z;
    kind = 5;
    LightSource* child = source->child;
    if (child) {
        ViewMat tmp;
        tmp.m[0] = child->matrix.m[0];
        tmp.m[1] = child->matrix.m[1];
        tmp.m[2] = child->matrix.m[2];
        tmp.m[3] = child->matrix.m[3];
        tmp.m[4] = child->matrix.m[4];
        tmp.m[5] = child->matrix.m[5];
        tmp.m[6] = child->matrix.m[6];
        tmp.m[7] = child->matrix.m[7];
        tmp.m[8] = child->matrix.m[8];
        tmp.m[9] = child->matrix.m[9];
        tmp.m[10] = child->matrix.m[10];
        tmp.m[11] = child->matrix.m[11];
        *(volatile float*)&tmp.m[12] = child->matrix.translation.x;
        *(volatile float*)&tmp.m[13] = child->matrix.translation.y;
        *(volatile float*)&tmp.m[14] = child->matrix.translation.z;
        tmp.m[15] = child->matrix.w;
        tmp.m[12] = child->position.x;
        tmp.m[13] = child->position.y;
        tmp.m[14] = child->position.z;
        ((LightView*)&matrix)->Concat(tmp);
    }
    position14 = matrix.translation;
    flags &= 0xfd;
    flags |= 1;
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
    c[0] = up * -0.75f;
    c[3] = up * -0.75f;
    c[1] = up * 0.75f;
    c[2] = up * 0.75f;
    c[0] += right * 1.5f;
    c[1] += right * 1.5f;
    c[2] -= right * 1.5f;
    c[3] -= right * 1.5f;
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
    float ms = -s;
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
// 1749 vs 1767 bytes, frame 0x10c vs 0x104. Same structure (glow quad, flare quad drawn twice with the uv square
// turned one way then the other); the compiler schedules the operand loads of the sum-of-squares, depth and
// normalisation chains differently, and keeps more locals live than the original (which reuses [ebp-0x1c]).
void CityLight::Render(RenderDevice* device, Camera* camera)
{
    Vec3 d = *(Vec3*)&position;
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
    float len = (float)sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
    float inv = 1.0f / len;
    d.x = inv * d.x;
    d.y = d.y * inv;
    d.z = inv * d.z;
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
    float ang = (d.z * camera->right.z + d.y * camera->right.y + d.x * camera->right.x) * 0.7f
              + (d.z * camera->up.z + d.y * camera->up.y + d.x * camera->up.x) * 0.7f;
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
