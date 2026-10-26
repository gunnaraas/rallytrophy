// Car wheel/slide emitter CarEmitterB (0x479170-0x47b48f, vtable 0x4e2204). Map:
// analysis/range-478870.md section 2.3.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_479170.h"
#include <math.h>
#include <stdlib.h>

// Statics set by Spawn and read by the spawn cores.
static ParticleCar* s_car;   // 0x563f40
static ParticleBody* s_body; // 0x563f3c

struct SinCos { float cos, sin; };
extern SinCos DAT_00561ec0[1024]; // cos at +0, sin at +4
extern float DAT_00561bb8[];
extern float DAT_00561b98[];
extern float DAT_00561b78[];
extern const float DAT_004e2178[]; // quad width per sprite row 0x11..0x15: 0.0625, 0.0625, 0.125, 0.0625, 0.125
extern float DAT_00561cb8[];
extern float DAT_00561ca0[];

// Per sprite row 0x11..0x15: velocity damping base per second, fade-in fraction.
static const float DAMP_BASE[5] = { 0.5f, 0.5f, 0.35f, 0.5f, 0.3f }; // 0x4e2150
static const float FADE_AT[5] = { 0.1f, 0.1f, 0.1f, 0.1f, 0.25f };    // 0x4e2164

static int s_tablesReady;  // 0x563f44
static float s_fadeOut[5]; // 0x563f14: 1 / (1 - FADE_AT)
static float s_fadeIn[5];  // 0x563f28: 1 / FADE_AT

// FUNCTION: RALLYTROPHY 0x479170
CarEmitterB::CarEmitterB()
{
    SetFlag04(1);
    flags |= 2;
    if (!s_tablesReady) {
        s_tablesReady = 1;
        for (int i = 0; i < 5; i++) {
            s_fadeIn[i] = 1.0f / FADE_AT[i];
            s_fadeOut[i] = 1.0f / (1.0f - FADE_AT[i]);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x479220
// SYMBOL: ??_GCarEmitterB@@UAEPAXI@Z
// CarEmitterB scalar deleting destructor
typedef void Generated_479220();

// FUNCTION: RALLYTROPHY 0x479240
CarEmitterB::~CarEmitterB()
{
}

// NONMATCHING: RALLYTROPHY 0x479250
// 564 bytes, identical except that `xor ebx,ebx` (re-zeroing the register constant) sits before
// the final `fstp [age]` in the original and after it here (5 bytes differ; no source form moved it).
// Ages, moves, damps and fades the particles; keeps their bounding box.
void CarEmitterB::Update(float dt)
{
    float damp[5];
    for (int i = 0; i < 5; i++) {
        damp[i] = (float)pow(DAMP_BASE[i], dt);
    }
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
        if (FloatBelow(p->age, p->lifetime)) {
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
            int row = (p->frame >> 4) - 0x11;
            float d = damp[row];
            vel->x = d * vel->x;
            vel->y = d * vel->y;
            vel->z = d * vel->z;
            p->rotation = dt * p->spin + p->rotation;
            p->size = dt * p->gravity + p->size;
            float ratio = p->age / p->lifetime;
            float alpha;
            if (ratio < FADE_AT[row]) {
                alpha = s_fadeIn[row] * p->alphaScale * ratio;
            } else {
                alpha = (1.0f - ratio) * s_fadeOut[row] * p->alphaScale;
            }
            ((BYTE*)&p->color)[3] = (BYTE)FloatToInt(alpha * 255.0f);
            p->next = particles;
            particles = p;
            p->age = (dt + dt) + p->age;
        } else {
            p->next = DAT_0056ae90.head;
            DAT_0056ae90.head = p;
        }
        p = next;
    }
}

// FUNCTION: RALLYTROPHY 0x479490
// Emits particles of the given kind (0..4); returns the emission ratio of the core.
float CarEmitterB::Spawn(int type, float scale, float age, Car* car, CarBody* body)
{
    s_car = (ParticleCar*)car;
    s_body = (ParticleBody*)body;
    switch (type) {
    case 0:
        return SpawnCore1(scale, age);
    case 1:
        return SpawnCore2(scale, age);
    case 2:
        return SpawnCore3(scale, age);
    case 3:
        return SpawnCore4(scale, age);
    case 4:
        return SpawnCore5(scale, age);
    }
    return 0.0f;
}

// NONMATCHING: RALLYTROPHY 0x479530
// 1216 bytes, same size and layout; 7 bytes differ: the two `size * sin` products (second and third
// quad vectors) load the table operand first here (`fld [sin]; fmul [size]`), the original size first.
// Draws every visible live particle as a rotated camera-facing quad.
void CarEmitterB::Render(RenderDevice* device, Camera* camera)
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
    Vec3* up = &camera->up;
    const float* m = device->GetViewMatrix();
    int nearZ = camera->nearBits;
    int farZ = camera->farBits;
    for (Particle* p = particles; p; p = p->next) {
        if (((BYTE*)&p->color)[3] == 0) {
            continue;
        }
        float depth = ((m[10] * p->position.z) + (m[6] * p->position.y)) + (m[2] * p->position.x) + m[14];
        if (*(int*)&depth > nearZ && *(int*)&depth < farZ) {
            Vec3 a, b;
            int idx = FloatToInt(p->rotation) & 0x3ff;
            a = *right * (p->size * DAT_00561ec0[idx].cos);
            a += *up * (p->size * DAT_00561ec0[idx].sin);
            b = *right * -(p->size * DAT_00561ec0[idx].sin);
            b += *up * (p->size * DAT_00561ec0[idx].cos);
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
            if (row == 0x13 || row == 0x15) {
                v[0].u = v[3].u = DAT_00561ca0[0] + DAT_00561b78[col * 2];
                v[0].v = v[1].v = DAT_00561ca0[1] + DAT_00561b78[col * 2 + 1];
            } else {
                v[0].u = v[3].u = DAT_00561b98[col * 2] + DAT_00561cb8[row * 2];
                v[0].v = v[1].v = DAT_00561b98[col * 2 + 1] + DAT_00561cb8[row * 2 + 1];
            }
            
            v[1].u = v[2].u = v[3].u + DAT_004e2178[row - 0x11];
            v[2].v = v[3].v = v[1].v + DAT_004e2178[row - 0x11];
            device->DrawQuad(v, texture, depth);
        }
    }
}

// Core 1 thresholds (the guarded function statics below are 1 / (hi - lo)).
static const float C1_SLIP_LO = 1.0f, C1_SLIP_HI = 4.0f;      // 0x4e218c, 0x4e2190
static const float C1_SPEED_LO = 5.0f, C1_SPEED_HI = 40.0f;   // 0x4e2194, 0x4e2198
static const float C1_SLIP2_LO = 4.0f, C1_SLIP2_HI = 35.0f;   // 0x4e219c, 0x4e21a0

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

// NONMATCHING: RALLYTROPHY 0x4799f0
// 1376 vs 1382 bytes; same instruction mix. Open: the original keeps the float/int scratch slot
// at ebp-0x10 above the n1 home slot (ebp-0x14), here they come the other way round (no declaration
// order or block scope changes it); and a few schedule differences (pool head store, x/y velocity
// jitter adds with a CSE'd velocity.x, epilogue pop order).
// Spawn core 1: emits one particle with a probability from the slips and the body speed.
float CarEmitterB::SpawnCore1(float scale, float age)
{
    static float kSlip = 1.0f / (C1_SLIP_HI - C1_SLIP_LO);
    static float kSpeed = 1.0f / (C1_SPEED_HI - C1_SPEED_LO);
    static float kSlip2 = 1.0f / (C1_SLIP2_HI - C1_SLIP2_LO);
    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    float slip = (float)fabs(s_car->slipLong);
    if (slip < C1_SLIP_HI) {
        if (slip > C1_SLIP_LO) {
            n1 = FloatToInt((slip - C1_SLIP_LO) * kSlip * 256.0f);
        }
    } else {
        n1 = 256;
    }
    float speed = s_body->vel.Length();
    if (speed < C1_SPEED_HI) {
        if (speed > C1_SPEED_LO) {
            n2 = FloatToInt((speed - C1_SPEED_LO) * kSpeed * 256.0f);
        }
    } else {
        n2 = 256;
    }
    float lat = (float)fabs(s_car->slipLat);
    if (lat < C1_SLIP2_HI) {
        if (lat > C1_SLIP2_LO) {
            n3 = FloatToInt((lat - C1_SLIP2_LO) * kSlip2 * 256.0f);
        }
    } else {
        n3 = 256;
    }
    n3 += n2;
    int total = n3 + n1;
    if (total > 256) {
        total = 256;
    }
    float count = (float)total;
    float ratio = count * 0.003921568859f;
    if (!((rand() & 0xff) < total / 2)) {
        return ratio;
    }
    if (n1 > 256) {
        n1 = 256;
    }
    if (n3 > 256) {
        n3 = 256;
    }
    float alpha = ((float)n1 * 0.0017578124534f + (float)n3 * 0.0020703123882f) * 0.5f;
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    Particle* nextFree = p->next;
    DAT_0056ae90.head = nextFree;
    p->age = age;
    p->unk0 = 0;
    p->lifetime = 3.5f;
    Vec3 local(s_car->position);
    float jx = (rand() & 0xff) * 0.00039062500582f;
    if (s_car->position.x > 0.0f) {
        jx = -jx;
    }
    local.x += jx;
    const float* trans = (const float*)&s_body->pos;
    const float* matrix = s_body->m;
    const float* point = (const float*)&local;
    float* result = (float*)&p->position;
    TRANSFORM_POINT();
    Vec3 v = s_body->vel * 0.85f;
    v += s_body->PointVelocity(p->position) * 0.85f;
    Vec3* vel = &p->velocity;
    vel->x = v.x;
    vel->y = v.y;
    vel->z = v.z;
    *vel -= s_body->Forward() * s_car->slipLong * 0.15f;
    p->velocity.y *= 0.5f;
    float j1 = (float)((rand() & 0x3fff) - 0x1fff) * 9.1552734375e-5f;
    float j2 = (float)((rand() & 0x3fff) - 0x1fff) * 6.103515625e-5f;
    vel->x += j1;
    p->velocity.y += j2;
    p->size = 0.2f;
    p->velocity.z += (float)((rand() & 0x3fff) - 0x1fff) * 9.1552734375e-5f;
    p->gravity = (rand() & 0x3ff) * 9.765625145519e-5f + 0.5f;
    p->frame = (rand() & 3) | 0x110;
    p->color = 0xffffffff;
    p->alphaScale = alpha;
    p->rotation = (float)(rand() & 0x3ff);
    p->spin = (1.0f - (rand() & 0x3ff) * 0.001953125f) * count;
    p->position -= p->velocity * scale;
    p->next = particles;
    particles = p;
    p->size = p->size - scale * p->gravity;
    return ratio;
}

static const float kInv32767 = 1.0f / 32767.0f;

// Core 2 constants (.rdata 0x4e21a4..0x4e21b8, 0x4e222c..0x4e2240).
static const float C2_k256 = 256.0f, C2_lo0 = 1.0f, C2_hi0 = 4.0f, C2_lo1 = 5.0f, C2_hi1 = 40.0f, C2_lo2 = 4.0f, C2_hi2 = 35.0f;
static const float C_inv255 = 1.0f / 255.0f;                      // 0x4e200c
static const float C_inv512 = 1.0f / 512.0f;                      // 0x4e222c
static const float C_spinScale = 1.0f / 10240.0f;                 // 0x4e2230
static const float C_jit = 9.1552734375e-5f;                      // 0x4e2234
static const float C_off = 1.0f / 2560.0f;                        // 0x4e2238
static const float C2_mixB = 0.00207031239f, C2_mixA = 0.00175781245f; // 0x4e223c, 0x4e2240


// Core 3 constants (.rdata 0x4e21bc..0x4e21d0, 0x4e2244..0x4e2250).
static const float C3_lo0 = 1.0f, C3_hi0 = 4.0f, C3_lo1 = 5.0f, C3_hi1 = 30.0f, C3_lo2 = 1.0f, C3_hi2 = 5.0f;
static const float C3_grav = 0.000683593738f;                     // 0x4e2244
static const float C3_jit = 3.0517578125e-5f;                     // 0x4e2248
static const float C3_mixB = 0.00253906241f, C3_mixA = 0.00134765625f; // 0x4e224c, 0x4e2250

// NONMATCHING: RALLYTROPHY 0x479f60
// 1368 vs 1376 bytes. Frame, SEH states, statics, ramps, TRANSFORM_POINT block, the three Vec3 calls and the
// rand chain match. Open: n2/total land in esi/edi swapped (original total=esi, n2=edi), so the mix terms are
// emitted r-first; the y-velocity temp lives at [ebp-0x20] (original reuses [ebp-0x14]) and the x jitter is not
// spilled, so the j1/j2 order differs; pool-head store and final pops are scheduled differently.
// Spawn core 2 (tyre-slip dust): emission amount from slip, speed and lateral slip, one particle.
float CarEmitterB::SpawnCore2(float a, float b)
{
    static float inv0 = 1.0f / (C2_hi0 - C2_lo0);
    static float inv1 = 1.0f / (C2_hi1 - C2_lo1);
    static float inv2 = 1.0f / (C2_hi2 - C2_lo2);
    float v;
    int n0 = 0, n1 = 0, n2 = 0;
    v = (float)fabs(s_car->slipLong);
    if (v < C2_hi0) {
        if (v > C2_lo0) {
            n0 = FloatToInt((v - C2_lo0) * inv0 * C2_k256);
        }
    } else {
        n0 = 0x100;
    }
    v = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z);
    if (v < C2_hi1) {
        if (v > C2_lo1) {
            n1 = FloatToInt((v - C2_lo1) * inv1 * C2_k256);
        }
    } else {
        n1 = 0x100;
    }
    v = (float)fabs(s_car->slipLat);
    if (v < C2_hi2) {
        if (v > C2_lo2) {
            n2 = FloatToInt((v - C2_lo2) * inv2 * C2_k256);
        }
    } else {
        n2 = 0x100;
    }
    n2 += n1;
    int total = n2 + n0;
    if (total > 0x100) {
        total = 0x100;
    }
    float totalF;
    float ret = (totalF = (float)total) * C_inv255;
    if ((rand() & 0xff) >= total / 2) {
        return ret;
    }
    int r = n2;
    if (n0 > 0x100) {
        n0 = 0x100;
    }
    if (r > 0x100) {
        r = 0x100;
    }
    float mix = (float)n0 * C2_mixA + (float)r * C2_mixB;
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    DAT_0056ae90.head = p->next;
    p->age = b;
    p->unk0 = 0;
    p->lifetime = 3.5f;
    Vec3 local(s_car->position);
    float off = (float)(rand() & 0xff) * C_off;
    if (s_car->position.x > 0.0f) {
        off = -off;
    }
    local.x = local.x + off;
    const Vec3* trans = &s_body->pos;
    const float* matrix = s_body->m;
    const Vec3* point = &local;
    Vec3* result = &p->position;
    TRANSFORM_POINT();
    Vec3 vel = s_body->vel * 0.85f;
    vel += s_body->PointVelocity(p->position) * 0.85f;
    Vec3* pv = &p->velocity;
    pv->x = vel.x;
    pv->y = vel.y;
    pv->z = vel.z;
    float slip = s_car->slipLong;
    p->velocity -= s_body->Forward() * slip * 0.15f;
    float yv = p->velocity.y + 0.1f;
    p->velocity.y = yv;
    float j1 = (float)((rand() & 0x3fff) - 0x1fff) * C_jit;
    float j2 = (float)((rand() & 0x3fff) - 0x1fff) * 6.103515625e-5f;
    p->velocity.x = j1 + p->velocity.x;
    p->velocity.y = yv + j2;
    p->size = 0.2f;
    p->velocity.z = (float)((rand() & 0x3fff) - 0x1fff) * C_jit + p->velocity.z;
    p->gravity = (float)(rand() & 0x3ff) * C_spinScale + 0.3f;
    p->frame = (rand() & 3) | 0x120;
    p->color = 0xffffffff;
    p->alphaScale = mix;
    p->rotation = (float)(rand() & 0x3ff);
    p->spin = (1.0f - (float)(rand() & 0x3ff) * C_inv512) * totalF;
    p->position -= p->velocity * a;
    p->next = particles;
    particles = p;
    p->size = p->size - a * p->gravity;
    return ret;
}

// NONMATCHING: RALLYTROPHY 0x47a4c0
// 1332 vs 1330 bytes. Same open items as core 2 (esi/edi swap of total and n2, mix term order, pool-head
// store scheduling, final pops); the single y *= 0.1 and the two jitter terms match.
// Spawn core 3: like core 2 with longer slow particles (size 0.5, no y jitter).
float CarEmitterB::SpawnCore3(float a, float b)
{
    static float inv0 = 1.0f / (C3_hi0 - C3_lo0);
    static float inv1 = 1.0f / (C3_hi1 - C3_lo1);
    static float inv2 = 1.0f / (C3_hi2 - C3_lo2);
    float v;
    int n0 = 0, n1 = 0, n2 = 0;
    v = (float)fabs(s_car->slipLong);
    if (v < C3_hi0) {
        if (v > C3_lo0) {
            n0 = FloatToInt((v - C3_lo0) * inv0 * C2_k256);
        }
    } else {
        n0 = 0x100;
    }
    v = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z);
    if (v < C3_hi1) {
        if (v > C3_lo1) {
            n1 = FloatToInt((v - C3_lo1) * inv1 * C2_k256);
        }
    } else {
        n1 = 0x100;
    }
    v = (float)fabs(s_car->slipLat);
    if (v < C3_hi2) {
        if (v > C3_lo2) {
            n2 = FloatToInt((v - C3_lo2) * inv2 * C2_k256);
        }
    } else {
        n2 = 0x100;
    }
    n2 += n1;
    int total = n2 + n0;
    if (total > 0x100) {
        total = 0x100;
    }
    float totalF;
    float ret = (totalF = (float)total) * C_inv255;
    if ((rand() & 0xff) >= total / 2) {
        return ret;
    }
    int r = n2;
    if (n0 > 0x100) {
        n0 = 0x100;
    }
    if (r > 0x100) {
        r = 0x100;
    }
    float mix = (float)n0 * C3_mixA + (float)r * C3_mixB;
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    DAT_0056ae90.head = p->next;
    p->age = b;
    p->unk0 = 0;
    p->lifetime = 2.0f;
    Vec3 local(s_car->position);
    float off = (float)(rand() & 0xff) * C_off;
    if (s_car->position.x > 0.0f) {
        off = -off;
    }
    local.x = local.x + off;
    const Vec3* trans = &s_body->pos;
    const float* matrix = s_body->m;
    const Vec3* point = &local;
    Vec3* result = &p->position;
    TRANSFORM_POINT();
    Vec3 vel = s_body->vel * 0.85f;
    vel += s_body->PointVelocity(p->position) * 0.85f;
    Vec3* pv = &p->velocity;
    pv->x = vel.x;
    pv->y = vel.y;
    pv->z = vel.z;
    float slip = s_car->slipLong;
    p->velocity -= s_body->Forward() * slip * 0.15f;
    p->velocity.y = p->velocity.y * 0.1f;
    p->velocity.x = (float)((rand() & 0x3fff) - 0x1fff) * C3_jit + p->velocity.x;
    p->velocity.z = (float)((rand() & 0x3fff) - 0x1fff) * C3_jit + p->velocity.z;
    p->size = 0.5f;
    p->gravity = (float)(rand() & 0x3ff) * C3_grav + 1.0f;
    p->frame = (rand() & 3) | 0x130;
    p->color = 0xffffffff;
    p->alphaScale = mix;
    p->rotation = (float)(rand() & 0x3ff);
    p->spin = (1.0f - (float)(rand() & 0x3ff) * C_inv512) * totalF;
    p->position -= p->velocity * a;
    p->next = particles;
    particles = p;
    p->size = p->size - a * p->gravity;
    return ret;
}

// Cores 4 and 5 were drafted against the ParticleWheel view of the car state.
#define WCAR ((ParticleWheel*)s_car)

static const float F4_k256 = 256.0f, F4_lo0 = 1.0f, F4_hi0 = 5.0f, F4_lo1 = 5.0f, F4_hi1 = 30.0f, F4_lo2 = 2.0f, F4_hi2 = 8.0f;

// NONMATCHING: RALLYTROPHY 0x47aa00
// 1347 vs 1346 bytes; registers, locals and all x87 sequences match. Open: VC6 emits the two alpha
// fild terms in the opposite order (m2 term first) whatever the source order, and schedules the
// pool pop / age store with ecx and edx swapped.
// Spawn core 4: decides from wheel slip and body speed whether to emit; fills a particle.
float CarEmitterB::SpawnCore4(float a, float b)
{
    static float inv0 = 1.0f / (F4_hi0 - F4_lo0);
    static float inv1 = 1.0f / (F4_hi1 - F4_lo1);
    static float inv2 = 1.0f / (F4_hi2 - F4_lo2);
    int c0 = 0, c1 = 0, c2 = 0;
    float v = (float)fabs(WCAR->slipA);
    if (v < F4_hi0) {
        if (v > F4_lo0) {
            c0 = FloatToInt((v - F4_lo0) * inv0 * F4_k256);
        }
    } else {
        c0 = 256;
    }
    v = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z);
    if (v < F4_hi1) {
        if (v > F4_lo1) {
            c1 = FloatToInt((v - F4_lo1) * inv1 * F4_k256);
        }
    } else {
        c1 = 256;
    }
    v = (float)fabs(WCAR->slipB);
    if (v < F4_hi2) {
        if (v > F4_lo2) {
            c2 = FloatToInt((v - F4_lo2) * inv2 * F4_k256);
        }
    } else {
        c2 = 256;
    }
    int bc = c2 + c1;
    int total = bc + c0;
    if (total > 256) {
        total = 256;
    }
    float ratio = (float)total * (1.0f / 255.0f);
    if ((rand() & 0xff) >= total / 2) {
        return ratio;
    }
    int m2 = bc;
    if (c0 > 256) {
        c0 = 256;
    }
    if (bc > 256) {
        m2 = 256;
    }
    float alpha = m2 * 0.0025390624f + c0 * 0.0013476562f;
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    p->age = b;
    DAT_0056ae90.head = p->next;
    p->unk0 = 0;
    p->lifetime = 1.0f;
    Vec3 local = WCAR->pos;
    local.y -= WCAR->Radius();
    float side = (float)(rand() & 0xff) * 0.00039062501f;
    if (WCAR->pos.x > 0) {
        side = -side;
    }
    local.x += side;
    const Vec3* trans = &s_body->pos;
    const float* matrix = s_body->m;
    const Vec3* point = &local;
    Vec3* result = &p->position;
    TRANSFORM_POINT();
    Vec3 vel = s_body->vel * 0.85f;
    vel += s_body->PointVelocity(p->position) * 0.85f;
    Vec3* pv = &p->velocity;
    pv->x = vel.x;
    pv->y = vel.y;
    pv->z = vel.z;
    p->velocity -= s_body->Forward() * WCAR->SlipA() * 0.15f;
    p->velocity.y *= 0.05f;
    p->velocity.x += (float)((rand() & 0x3fff) - 0x1fff) * 3.0517578e-5f;
    p->velocity.z += (float)((rand() & 0x3fff) - 0x1fff) * 3.0517578e-5f;
    p->size = 0.3f;
    p->gravity = (float)(rand() & 0x3ff) * 0.00029296876f + 0.6f;
    p->frame = (rand() & 3) | 0x140;
    p->color = 0xffffffff;
    *(int*)&p->alphaScale = *(int*)&alpha;
    p->rotation = (float)(rand() & 0x3ff);
    p->spin = (1.0f - (float)(rand() & 0x3ff) * 0.001953125f) * (float)total;
    p->position -= p->velocity * a;
    p->next = particles;
    particles = p;
    p->size = p->size - a * p->gravity;
    return ratio;
}

static const float F5_k256 = 256.0f, F5_lo0 = 1.0f, F5_hi0 = 4.0f, F5_lo1 = 2.0f, F5_hi1 = 30.0f, F5_lo2 = 1.0f, F5_hi2 = 4.0f;

// NONMATCHING: RALLYTROPHY 0x47af50
// 1331 vs 1330 bytes; same two open scheduling differences as core 4 (alpha term order, ecx/edx
// swap around the pool pop) plus WCAR being re-read one instruction earlier before the side jitter.
// Spawn core 5: decides from wheel slip and body speed whether to emit; fills a particle.
float CarEmitterB::SpawnCore5(float a, float b)
{
    static float inv0 = 1.0f / (F5_hi0 - F5_lo0);
    static float inv1 = 1.0f / (F5_hi1 - F5_lo1);
    static float inv2 = 1.0f / (F5_hi2 - F5_lo2);
    int c0 = 0, c1 = 0, c2 = 0;
    float v = (float)fabs(WCAR->slipA);
    if (v < F5_hi0) {
        if (v > F5_lo0) {
            c0 = FloatToInt((v - F5_lo0) * inv0 * F5_k256);
        }
    } else {
        c0 = 256;
    }
    v = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z);
    if (v < F5_hi1) {
        if (v > F5_lo1) {
            c1 = FloatToInt((v - F5_lo1) * inv1 * F5_k256);
        }
    } else {
        c1 = 256;
    }
    v = (float)fabs(WCAR->slipB);
    if (v < F5_hi2) {
        if (v > F5_lo2) {
            c2 = FloatToInt((v - F5_lo2) * inv2 * F5_k256);
        }
    } else {
        c2 = 256;
    }
    int bc = c2 + c1;
    int total = bc + c0;
    if (total > 256) {
        total = 256;
    }
    float ratio = (float)total * (1.0f / 255.0f);
    if ((rand() & 0xff) >= total / 2) {
        return ratio;
    }
    int m2 = bc;
    if (c0 > 256) {
        c0 = 256;
    }
    if (bc > 256) {
        m2 = 256;
    }
    float alpha = m2 * 0.0025390624f + c0 * 0.0013476562f;
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    p->age = b;
    DAT_0056ae90.head = p->next;
    p->unk0 = 0;
    p->lifetime = 2.0f;
    Vec3 local = WCAR->pos;
    float side = (float)(rand() & 0xff) * 0.00039062501f;
    if (WCAR->pos.x > 0) {
        side = -side;
    }
    local.x += side;
    const Vec3* trans = &s_body->pos;
    const float* matrix = s_body->m;
    const Vec3* point = &local;
    Vec3* result = &p->position;
    TRANSFORM_POINT();
    Vec3 vel = s_body->vel * 0.85f;
    vel += s_body->PointVelocity(p->position) * 0.85f;
    Vec3* pv = &p->velocity;
    pv->x = vel.x;
    pv->y = vel.y;
    pv->z = vel.z;
    p->velocity -= s_body->Forward() * WCAR->SlipA() * 0.15f;
    p->velocity.y *= 0.1f;
    p->velocity.x += (float)((rand() & 0x3fff) - 0x1fff) * 3.0517578e-5f;
    p->velocity.z += (float)((rand() & 0x3fff) - 0x1fff) * 3.0517578e-5f;
    p->size = 0.5f;
    p->gravity = (float)(rand() & 0x3ff) * 0.00078125001f + 1.1f;
    p->frame = (rand() & 3) | 0x150;
    p->color = 0xffffffff;
    *(int*)&p->alphaScale = *(int*)&alpha;
    p->rotation = (float)(rand() & 0x3ff);
    p->spin = (1.0f - (float)(rand() & 0x3ff) * 0.001953125f) * (float)total;
    p->position -= p->velocity * a;
    p->next = particles;
    particles = p;
    p->size = p->size - a * p->gravity;
    return ratio;
}

// Module-end guard pair VC6 emits for files that include the STL headers:
// guard bit 0 of 0x5826d5, then atexit(0x402c90).

// FUNCTION: RALLYTROPHY 0x47b490
// SYMBOL: _$E28
typedef void Generated_47b490();

// FUNCTION: RALLYTROPHY 0x47b4b0
// SYMBOL: _$E27
typedef void Generated_47b4b0();
