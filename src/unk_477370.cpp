// Car wheel spray effect WheelSpray (0x477370-0x47801f, vtable 0x4e20bc),
// embedded in a Car at +0x2100. Map: analysis/range-4741a0.md section 2.7.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_477370.h"
#include <math.h>
#include <stdlib.h>

// Statics set by Spawn and read by the spawn core.
static ParticleCar* s_car;   // 0x561b64
static ParticleBody* s_body; // 0x561b60

extern float DAT_00561ec0[]; // (cos, sin) pairs
extern float DAT_00561b78[];
extern float DAT_00561c38[];

static const float kAtlasStep = 0.125f;  // 0x4e2094
static const float kDampBase = 0.35f;    // 0x4e2098
static const float kFadeAt = 0.4f;       // 0x4e209c

static float s_fadeOut = 1.0f / (1.0f - kFadeAt); // 0x561b58
static float s_fadeIn = 1.0f / kFadeAt;           // 0x561b5c

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

// FUNCTION: RALLYTROPHY 0x477370
// SYMBOL: _$E22
typedef void Generated_477370();

// FUNCTION: RALLYTROPHY 0x477380
// SYMBOL: _$E21
typedef void Generated_477380();

// FUNCTION: RALLYTROPHY 0x4773a0
// SYMBOL: _$E19
typedef void Generated_4773a0();

// FUNCTION: RALLYTROPHY 0x4773b0
// SYMBOL: _$E18
typedef void Generated_4773b0();

// FUNCTION: RALLYTROPHY 0x4773d0
WheelSpray::WheelSpray()
{
    SetFlag04(1);
    flags |= 2;
}

// FUNCTION: RALLYTROPHY 0x477430
// SYMBOL: ??_GWheelSpray@@UAEPAXI@Z
typedef void Generated_477430();

// FUNCTION: RALLYTROPHY 0x477450
WheelSpray::~WheelSpray()
{
}

// FUNCTION: RALLYTROPHY 0x477460
// Ages, moves, damps and fades the particles; keeps their bounding box.
void WheelSpray::Update(float dt)
{
    float damp = (float)pow(kDampBase, dt);
    Particle* p = particles;
    particles = 0;
    if (p != 0) {
        boxMin[0] = p->position.x;
        boxMin[1] = p->position.y;
        boxMin[2] = p->position.z;
        boxMax[0] = p->position.x;
        boxMax[1] = p->position.y;
        boxMax[2] = p->position.z;
    } else {
        boxMin[0] = boxMin[1] = boxMin[2] = 0;
        boxMax[0] = boxMax[1] = boxMax[2] = 0;
    }
    while (p != 0) {
        Particle* next = p->next;
        if (*(int*)&p->age < *(int*)&p->lifetime) {
            Vec3* vel = &p->velocity;
            p->position += *vel * dt;
            if (p->position.x > boxMax[0]) {
                boxMax[0] = p->position.x;
            } else if (p->position.x < boxMin[0]) {
                boxMin[0] = p->position.x;
            }
            if (p->position.y > boxMax[1]) {
                boxMax[1] = p->position.y;
            } else if (p->position.y < boxMin[1]) {
                boxMin[1] = p->position.y;
            }
            if (p->position.z > boxMax[2]) {
                boxMax[2] = p->position.z;
            } else if (p->position.z < boxMin[2]) {
                boxMin[2] = p->position.z;
            }
            vel->x = damp * vel->x;
            vel->y = damp * vel->y;
            vel->z = damp * vel->z;
            p->rotation = dt * p->spin + p->rotation;
            p->size = dt * p->gravity + p->size;
            float ratio = p->age / p->lifetime;
            float alpha;
            if (ratio < kFadeAt) {
                alpha = s_fadeIn * p->alphaScale * ratio;
            } else {
                alpha = (1.0f - ratio) * p->alphaScale * s_fadeOut;
            }
            ((BYTE*)&p->color)[3] = (BYTE)FloatToInt(alpha * 255.0f);
            p->age = dt + p->age;
            p->next = particles;
            particles = p;
        } else {
            p->next = DAT_0056ae90.head;
            DAT_0056ae90.head = p;
        }
        p = next;
    }
}

// NONMATCHING: RALLYTROPHY 0x477690
// 1227 bytes, same size and layout; named atlas accumulators reproduce the target's constant-first x87 adds.
// Four bytes differ: the depth argument load is scheduled before the first add instead of after it.
// Evidence: ev_6e2034557ea54db46de9b42321c77c445cd8efd3f0dbae384e3a4b07d011fb93.
// Draws every visible live particle as a rotated camera-facing quad.
void WheelSpray::Render(RenderDevice* device, Camera* camera)
{
    if (particles == 0 || texture == 0) {
        return;
    }
    EffectVertex v[4];
    Vec3* right = &camera->right;
    v[0].normal.x = 0;
    v[0].normal.y = 0;
    v[0].normal.z = 0;
    v[1].normal.x = 0;
    v[1].normal.y = 0;
    v[1].normal.z = 0;
    v[2].normal.x = 0;
    v[2].normal.y = 0;
    v[2].normal.z = 0;
    v[3].normal.x = 0;
    v[3].normal.y = 0;
    v[3].normal.z = 0;
    v[0].u2 = v[0].v2 = 0;
    v[1].u2 = v[1].v2 = 0;
    v[2].u2 = v[2].v2 = 0;
    v[3].u2 = v[3].v2 = 0;
    v[0].u = v[0].v = v[1].v = v[3].u = 0;
    v[1].u = v[2].u = v[2].v = v[3].v = 1.0f;
    Vec3* up = &camera->up;
    const float* m = device->GetViewMatrix();
    int nearZ = camera->nearBits;
    int farZ = camera->farBits;
    for (Particle* p = particles; p; p = p->next) {
        float depth = p->position.y * m[6] + p->position.z * m[10];
        depth += p->position.x * m[2];
        depth += m[14];
        if (*(int*)&depth > nearZ && *(int*)&depth < farZ) {
            Vec3 a, b;
            int idx = FloatToInt(p->rotation) & 0x3ff;
            a = *right * (p->size * DAT_00561ec0[idx * 2]);
            a += *up * (p->size * DAT_00561ec0[idx * 2 + 1]);
            b = *right * -(p->size * DAT_00561ec0[idx * 2 + 1]);
            b += *up * (p->size * DAT_00561ec0[idx * 2]);
            Vec3 c, d;
            c.x = a.x + b.x;
            c.y = a.y + b.y;
            c.z = a.z + b.z;
            d.x = a.x - b.x;
            d.y = a.y - b.y;
            d.z = a.z - b.z;
            v[0].position.x = p->position.x;
            v[0].position.y = p->position.y;
            v[0].position.z = p->position.z;
            v[0].position -= d;
            v[1].position.x = p->position.x;
            v[1].position.y = p->position.y;
            v[1].position.z = p->position.z;
            v[1].position += c;
            v[2].position.x = p->position.x;
            v[2].position.y = p->position.y;
            v[2].position.z = p->position.z;
            v[2].position += d;
            v[3].position.x = p->position.x;
            v[3].position.y = p->position.y;
            v[3].position.z = p->position.z;
            v[3].position -= c;
            v[0].color = p->color;
            v[1].color = p->color;
            v[2].color = p->color;
            v[3].color = p->color;
            int col = p->frame & 3;
            int row = p->frame >> 4;
            v[0].u = v[3].u = DAT_00561b78[col * 2] + DAT_00561c38[row * 2];
            v[0].v = v[1].v = DAT_00561b78[col * 2 + 1] + DAT_00561c38[row * 2 + 1];
            float atlasU = kAtlasStep;
            atlasU += v[3].u;
            v[1].u = v[2].u = atlasU;
            float atlasV = kAtlasStep;
            atlasV += v[1].v;
            v[2].v = v[3].v = atlasV;
            device->DrawQuad(v, texture, depth);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x477b60
// Emits spray for kind 0 only.
void WheelSpray::Spawn(int type, float scale, float age, Car* car, CarBody* body)
{
    s_car = (ParticleCar*)car;
    s_body = (ParticleBody*)body;
    if (type == 0) {
        SpawnCore(scale, age);
    }
}

// Spawn core thresholds (the guarded function statics of SpawnCore are 1 / (hi - lo)):
// longitudinal slip, body speed and lateral slip each ramp from lo to hi.
static const float kMax = 1.0f;                        // 0x4e20a0
static const float kLongLo = 10.0f, kLongHi = 40.0f;   // 0x4e20a4, 0x4e20a8
static const float kSpeedLo = 12.0f, kSpeedHi = 40.0f; // 0x4e20ac, 0x4e20b0
static const float kLatLo = 10.0f, kLatHi = 40.0f;     // 0x4e20b4, 0x4e20b8

// local point -> world: point * rotation (rows at matrix) + trans, a hand-written x87 block in the original
#define TRANSFORM_POINT() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
    __asm mov ebx, trans \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx] \
    __asm fxch st(1) \
    __asm fadd dword ptr [ebx+4] \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx+8] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// FUNCTION: RALLYTROPHY 0x477b90
// Emits one particle with a probability from the body speed and the two slips.
void WheelSpray::SpawnCore(float scale, float age)
{
    static float kInvLong = 1.0f / (kLongHi - kLongLo);
    static float kInvSpeed = 1.0f / (kSpeedHi - kSpeedLo);
    static float kInvLat = 1.0f / (kLatHi - kLatLo);
    float c = 0.0f;
    float b = 0.0f;
    float a;
    float t = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z);
    if (t > kSpeedLo) {
        if (t > kSpeedHi) {
            a = kMax;
        } else {
            a = (t - kSpeedLo) * kMax * kInvSpeed;
        }
        t = (float)fabs(s_car->slipLat);
        if (t > kLatLo) {
            if (t > kLatHi) {
                b = kMax;
            } else {
                b = (t - kLatLo) * kMax * kInvLat;
            }
        }
        t = (float)fabs(s_car->slipLong);
        if (t > kLongLo) {
            if (t > kLongHi) {
                c = kMax;
            } else {
                c = (t - kLongLo) * kMax * kInvLong;
            }
        }
        c += a;
        c += b;
        t = c;
        if (t > kMax) {
            t = kMax;
        }
        float r = (float)(rand() & 0xff) * 0.00390625f;
        if (r < t * 0.5f) {
            if (!DAT_0056ae90.head) {
                DAT_0056ae90.Grow();
            }
            Particle* p = DAT_0056ae90.head;
            DAT_0056ae90.head = p->next;
            Vec3 world;
            const Vec3* trans = &s_body->pos;
            const float* matrix = s_body->m;
            const Vec3* point = &s_car->position;
            Vec3* result = &world;
            TRANSFORM_POINT();
            Vec3 vel = s_body->vel * 0.75f;
            vel.y = 0.15f;
            vel.x += (float)((rand() & 0xff) - 0x80) * a * 0.00859375f;
            vel.z += (float)((rand() & 0xff) - 0x80) * a * 0.00859375f;
            Vec3* pv = &p->velocity;
            p->position.x = world.x;
            p->position.y = world.y;
            p->position.z = world.z;
            pv->x = vel.x;
            pv->y = vel.y;
            pv->z = vel.z;
            p->lifetime = t * 3.0f + 2.0f;
            p->age = age;
            p->size = 0.2f;
            p->gravity = (float)(rand() & 0xff) * 0.0015686275f + t * 0.7f + 0.9f;
            p->frame = (rand() & 3) | 0xc0;
            p->color = 0xffffffff;
            p->alphaScale = t * 0.2f + 0.8f;
            p->rotation = (float)(rand() & 0x3ff);
            p->spin = (float)((rand() & 0x1ff) - 0x100) * (a * 0.9f + 0.1f);
            p->position -= p->velocity * scale;
            p->next = particles;
            particles = p;
            p->size = p->size - scale * p->gravity;
        }
    }
}

// Module-end guard pair VC6 emits for files that include the STL headers:
// guard bit 0 of 0x5826d5, then atexit(0x402c90).

// FUNCTION: RALLYTROPHY 0x477ff0
// SYMBOL: _$E30
typedef void Generated_477ff0();

// FUNCTION: RALLYTROPHY 0x478010
// SYMBOL: _$E29
typedef void Generated_478010();
