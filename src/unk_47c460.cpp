// Car particle emitter CarParticles (0x47c460-0x47ef4f, vtable 0x4e237c): the
// wheel dust / smoke / gravel effects embedded in a Car. Map:
// analysis/range-47c460.md. The GameSpy CD-key SDK after 0x47ef80 is out of
// scope here.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include <math.h>
#include <stdlib.h>

// Statics set by Spawn and read by the spawn cores.
static ParticleWheel* s_car; // 0x563fdc
static ParticleBody* s_body; // 0x563fd8

// original: sin/cos table filled elsewhere; [i*2] = sin, [i*2+1] = cos
extern const float DAT_004e22c0; // pooled 1.0f (the damping base)
extern const float DAT_004e22b8;
extern const float DAT_004e22bc;
extern float DAT_00561ec0[1024 * 2];
extern float DAT_00561bb8[]; // uv tables
extern float DAT_00561b98[];
extern float DAT_00561cb8[];

// FUNCTION: RALLYTROPHY 0x47c460
CarParticles::CarParticles()
{
    SetFlag04(1);
    flags |= 2;
}

// FUNCTION: RALLYTROPHY 0x47c4c0
// SYMBOL: ??_GCarParticles@@UAEPAXI@Z
// CarParticles scalar deleting destructor
typedef void Generated_47c4c0();

// FUNCTION: RALLYTROPHY 0x47c4e0
CarParticles::~CarParticles()
{
}

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

// FUNCTION: RALLYTROPHY 0x47c4f0
// Sets the particle colour from an RGB triple in 0..1.
void CarParticles::SetColor(const Vec3* rgb)
{
    int r = FloatToInt(rgb->x * 255.0f);
    int g = FloatToInt(rgb->y * 255.0f);
    int b = FloatToInt(rgb->z * 255.0f);
    if (r < 0) {
        r = 0;
    } else if (r > 255) {
        r = 255;
    }
    if (g < 0) {
        g = 0;
    } else if (g > 255) {
        g = 255;
    }
    if (b < 0) {
        b = 0;
    } else if (b > 255) {
        b = 255;
    }
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;
}

// Compares two non-negative floats as integers (their bit patterns order the same way).
inline bool FloatBelow(const float& a, const float& b)
{
    return *(const int*)&a < *(const int*)&b;
}

// a = b + a, b * a per component (the operand order the original compiled).
inline void AddTo(Vec3& a, const Vec3& b)
{
    a.x = b.x + a.x;
    a.y = b.y + a.y;
    a.z = b.z + a.z;
}

inline void Scale(Vec3& a, float k)
{
    a.x = k * a.x;
    a.y = k * a.y;
    a.z = k * a.z;
}

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

// Cores A-C were drafted against these views of the two file statics.
#define PCAR ((ParticleCar*)s_car)
#define PBODY ((CarBody*)s_body)

// Body-relative vectors: right axis (0x20), forward axis (0x40), velocity (0x1cc).
#define BODY_RIGHT (*(Vec3*)PBODY->unk20)
#define BODY_FORWARD (*(Vec3*)PBODY->forward)
#define BODY_VELOCITY (*(Vec3*)PBODY->velocity)

// VC6 loads const float objects from memory instead of folding them, which
// the guarded function statics below (1 / (hi - lo)) rely on.
static const float RAND_SCALE = 1.0f / 32767.0f;       // 0x4e22b4
static const float SIZE_SMALL = 0.02f;                 // 0x4e22b8
static const float SIZE_LARGE = 0.05f;                 // 0x4e22bc
static const float SCALE_A = 256.0f;                   // 0x4e22c4
static const float SLIP_A_LO = 1.0f, SLIP_A_HI = 4.0f;       // 0x4e22c8
static const float SPEED_A_LO = 10.0f, SPEED_A_HI = 60.0f;   // 0x4e22d0
static const float SLIP2_A_LO = 4.0f, SLIP2_A_HI = 15.0f;    // 0x4e22d8
static const float SLIP_B_LO = 1.0f, SLIP_B_HI = 3.0f;       // 0x4e22f8
static const float SPEED_B_LO = 10.0f, SPEED_B_HI = 80.0f;   // 0x4e2300
static const float SLIP2_B_LO = 5.0f, SLIP2_B_HI = 15.0f;    // 0x4e2308
static const float SCALE_C = 256.0f;                         // 0x4e2328
static const float SLIP_C_LO = 1.0f, SLIP_C_HI = 4.0f;       // 0x4e232c
static const float SPEED_C_LO = 5.0f, SPEED_C_HI = 30.0f;    // 0x4e2334
static const float SLIP2_C_LO = 1.0f, SLIP2_C_HI = 4.0f;     // 0x4e233c
static const float SPEED_SCALE_C = 1.0f / 35.0f;             // 0x4e23a4

// Transforms the vector in ECX-load order by the 3x3 rows at ptr EDX and adds the vector at EBX.
#define BODY_TRANSFORM() \
    __asm mov ecx, in \
    __asm mov edx, mat \
    __asm mov eax, out \
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

static const float kInv32767 = 1.0f / 32767.0f;

static const float D_k256 = 256.0f, D_lo0 = 1.0f, D_hi0 = 5.0f, D_lo1 = 5.0f, D_hi1 = 30.0f, D_lo2 = 2.0f, D_hi2 = 8.0f;

// FUNCTION: RALLYTROPHY 0x47c5a0
// Ages, moves and recycles the particles; keeps their bounding box.
void CarParticles::Update(float dt)
{
    float damping = (float)pow(DAT_004e22c0, dt);
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
            p->age = dt + p->age;
            AddTo(p->position, p->velocity * dt);
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
            p->next = particles;
            particles = p;
            Scale(p->velocity, damping);
            p->velocity.y = dt * p->gravity + p->velocity.y;
            p->rotation = dt * p->spin + p->rotation;
        } else {
            p->next = DAT_0056ae90.head;
            DAT_0056ae90.head = p;
        }
        p = next;
    }
}

// FUNCTION: RALLYTROPHY 0x47c790
// Emits particles of the given kind (4 and 5 emit nothing).
void CarParticles::Spawn(int type, float a, float b, Car* car, CarBody* body)
{
    s_car = (ParticleWheel*)car;
    s_body = (ParticleBody*)body;
    switch (type) {
    case 0:
        Kind0(a, b);
        break;
    case 1:
        Kind1(a, b);
        break;
    case 2:
        Kind2(a, b);
        break;
    case 3:
        Kind3(a, b);
        break;
    case 6:
        Kind6(a, b);
        break;
    case 7:
        Kind7(a, b);
        break;
    case 8:
        Kind8(a, b);
        break;
    case 9:
        Kind9(a, b);
        break;
    case 10:
        Kind10(a, b);
        break;
    case 11:
        Kind11(a, b);
        break;
    }
}

// NONMATCHING: RALLYTROPHY 0x47c8a0
// 1199 vs 1202 bytes. Structure, frame layout and EH states match; remaining differences: the
// operand order of the commutative fmul/fadd pairs (the compiler loads the table/matrix operand first)
// and the u0/v0 FPU-stack spill after the frame/row branch.
// Draws every live particle as a camera-facing quad (4 vertices) through the renderer.
void CarParticles::Render(RenderDevice* device, Camera* camera)
{
    if (texture == 0 || particles == 0) {
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
        float depth = m[10] * p->position.z + m[6] * p->position.y + m[2] * p->position.x + m[14];
        if (*(int*)&depth > nearZ && *(int*)&depth < farZ) {
            Vec3 a, b;
            int idx = FloatToInt(p->rotation) & 0x3ff;
            a = *right * (DAT_00561ec0[idx * 2] * p->size);
            a += *up * (DAT_00561ec0[idx * 2 + 1] * p->size);
            b = *right * -(DAT_00561ec0[idx * 2 + 1] * p->size);
            b += *up * (DAT_00561ec0[idx * 2] * p->size);
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
            int col = p->frame & 0xf;
            int row = p->frame >> 4;
            float u0, v0, cell;
            if (row < 9) {
                u0 = DAT_00561bb8[col * 2];
                cell = 0.03125f;
                v0 = DAT_00561bb8[col * 2 + 1];
            } else {
                col = p->frame & 3;
                u0 = DAT_00561b98[col * 2];
                cell = 0.0625f;
                v0 = DAT_00561b98[col * 2 + 1];
            }
            v[0].u = v[3].u = u0 + DAT_00561cb8[row * 2];
            v[0].v = v[1].v = v0 + DAT_00561cb8[row * 2 + 1];
            v[1].u = v[2].u = v[3].u + cell;
            v[2].v = v[3].v = v[1].v + cell;
            device->DrawQuad(v, texture, depth);
        }
    }
}

// NONMATCHING: RALLYTROPHY 0x47cd60
// 1233 bytes, 52 differing bytes. Naming `randomValue` and `threshold`, then expressing the
// failure test as `threshold <= randomValue`, reproduces the target's FXCH/FCOMPP compare pair.
// Stack homes, compare operand/order flags, and the delayed `push edi` still differ; see the log.
// Spawn core A: emits a particle with probability from the slips and speed.
Particle* CarParticles::SpawnCoreA(float rate)
{
    static float kSlip = 1.0f / (SLIP_A_HI - SLIP_A_LO);
    static float kSpeed = 1.0f / (SPEED_A_HI - SPEED_A_LO);
    static float kSlip2 = 1.0f / (SLIP2_A_HI - SLIP2_A_LO);
    float chance = 0.0f;
    float slip = (float)fabs(PCAR->slipLong);
    if (slip < SLIP_A_HI) {
        if (slip > SLIP_A_LO) {
            chance = (slip - SLIP_A_LO) * SCALE_A * kSlip;
        }
        float speed = BODY_VELOCITY.Length();
        if (speed < SPEED_A_HI) {
            if (speed > SPEED_A_LO) {
                chance += (speed - SPEED_A_LO) * SCALE_A * kSpeed;
            }
            float lat = (float)fabs(PCAR->slipLat);
            if (lat < SLIP2_A_HI) {
                if (lat > SLIP2_A_LO) {
                    chance += (lat - SLIP2_A_LO) * SCALE_A * kSlip2;
                }
            } else {
                chance = SCALE_A;
            }
        } else {
            chance = SCALE_A;
        }
    } else {
        chance = SCALE_A;
    }
    int randomValue = rand() & 0xff;
    float threshold = chance * rate;
    if (threshold <= (float)randomValue) {
        return 0;
    }
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    DAT_0056ae90.head = p->next;
    Vec3 local;
    local.x = PCAR->position.x;
    local.y = PCAR->position.y;
    local.z = PCAR->position.z;
    float h = PCAR->unk264;
    local.y -= h;
    local.x += 0.05f - (rand() & 0xff) * 0.000390625f;
    const float* trans = PBODY->unk14;
    const float* mat = PBODY->unk20;
    const float* in = (const float*)&local;
    float* out = (float*)&p->position;
    BODY_TRANSFORM();
    Vec3 v = BODY_VELOCITY * ((rand() * RAND_SCALE) * 0.25f + 0.7f);
    v += PBODY->FUN_004204f0(p->position);
    p->velocity.x = v.x;
    p->velocity.y = v.y;
    p->velocity.z = v.z;
    float r1 = rand() * RAND_SCALE;
    float r2 = rand() * RAND_SCALE;
    rand();
    float lon = PCAR->slipLong;
    p->velocity -= BODY_FORWARD * lon * 0.2f * r1;
    float lat = PCAR->slipLat;
    AddTo(p->velocity, BODY_RIGHT * lat * 0.3f * r2);
    p->velocity.y += 2.5f;
    float j1 = (float)((rand() & 0x3fff) - 0x1fff) * 9.1552734375e-5f;
    float j2 = (float)((rand() & 0x3fff) - 0x1fff) * 9.1552734375e-5f;
    Vec3 jit;
    jit.x = j1;
    jit.y = j2;
    p->velocity.x += jit.x;
    p->velocity.y += jit.y;
    p->velocity.z += (float)((rand() & 0x3fff) - 0x1fff) * 9.1552734375e-5f;
    return p;
}

// FUNCTION: RALLYTROPHY 0x47d240
void CarParticles::Kind0(float scale, float age)
{
    Particle* p = SpawnCoreA(0.3f);
    if (p) {
        p->age = age;
        p->lifetime = 1.0f;
        p->size = 0.0175f;
        p->gravity = -9.0f;
        p->frame = rand() & 0xf;
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = 0.0f;
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// FUNCTION: RALLYTROPHY 0x47d2f0
void CarParticles::Kind1(float scale, float age)
{
    Particle* p = SpawnCoreA(0.3f);
    if (p) {
        p->age = age;
        p->lifetime = 1.0f;
        p->size = 0.0175f;
        p->gravity = -9.0f;
        p->frame = (rand() & 0xf) | 0x10;
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = 0.0f;
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// FUNCTION: RALLYTROPHY 0x47d3a0
void CarParticles::Kind2(float scale, float age)
{
    Particle* p = SpawnCoreA(0.3f);
    if (p) {
        p->age = age;
        p->lifetime = 1.0f;
        p->size = 0.0175f;
        p->gravity = -9.0f;
        p->frame = (rand() & 0xf) | 0x20;
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = 0.0f;
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// FUNCTION: RALLYTROPHY 0x47d450
void CarParticles::Kind3(float scale, float age)
{
    Particle* p = SpawnCoreA(0.8f);
    if (p) {
        p->age = age;
        p->lifetime = 1.0f;
        p->size = 0.0125f;
        p->gravity = -9.0f;
        p->frame = (rand() & 0xf) | 0x30;
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = 0.0f;
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// NONMATCHING: RALLYTROPHY 0x47d500
// Same size (1125); only 15 bytes differ: the original loads PCAR before the free-list head
// and stores the new head after the first copied coordinate; this build pops the list first
// and places `push edi` earlier. The `Vec3 jit` local is what makes VC6 keep the x jitter in
// a stack temp across the second rand() call like the original.
// Spawn core B: like core A with an integer 0..256 chance and no rate argument.
Particle* CarParticles::SpawnCoreB(float rate)
{
    static float kSlip = 1.0f / (SLIP_B_HI - SLIP_B_LO);
    static float kSpeed = 1.0f / (SPEED_B_HI - SPEED_B_LO);
    static float kSlip2 = 1.0f / (SLIP2_B_HI - SLIP2_B_LO);
    int chance = 0;
    float slip = (float)fabs(PCAR->slipLong);
    if (slip < SLIP_B_HI) {
        if (slip > SLIP_B_LO) {
            slip = (slip - SLIP_B_LO) * kSlip * 256.0f;
            int a;
            __asm {
                fld slip
                fistp a
            }
            chance = a;
        }
        float speed = BODY_VELOCITY.Length();
        if (speed < SPEED_B_HI) {
            if (speed > SPEED_B_LO) {
                float t = (speed - SPEED_B_LO) * kSpeed * 256.0f;
                int b;
                __asm {
                    fld t
                    fistp b
                }
                chance += b;
            }
            float lat = (float)fabs(PCAR->slipLat);
            if (lat < SLIP2_B_HI) {
                if (lat > SLIP2_B_LO) {
                    float t = (lat - SLIP2_B_LO) * kSlip2 * 256.0f;
                    int c;
                    __asm {
                        fld t
                        fistp c
                    }
                    chance += c;
                }
            } else {
                chance = 0x100;
            }
        } else {
            chance = 0x100;
        }
    } else {
        chance = 0x100;
    }
    if ((rand() & 0xff) >= chance) {
        return 0;
    }
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    DAT_0056ae90.head = p->next;
    Vec3 local;
    local.x = PCAR->position.x;
    local.y = PCAR->position.y;
    local.z = PCAR->position.z;
    float h = PCAR->unk264;
    local.y -= h;
    local.x += 0.05f - (rand() & 0xff) * 0.000390625f;
    const float* trans = PBODY->unk14;
    const float* mat = PBODY->unk20;
    const float* in = (const float*)&local;
    float* out = (float*)&p->position;
    BODY_TRANSFORM();
    Vec3 v = BODY_VELOCITY * 0.9f;
    v += PBODY->FUN_004204f0(p->position) * 0.9f;
    p->velocity.x = v.x;
    p->velocity.y = v.y;
    p->velocity.z = v.z;
    float lon = PCAR->slipLong;
    p->velocity -= BODY_FORWARD * lon * 0.1f;
    float lat = PCAR->slipLat;
    p->velocity += BODY_RIGHT * lat * 0.1f;
    p->velocity.y += 1.0f;
    float j1 = (float)((rand() & 0x3fff) - 0x1fff) * 9.1552734375e-5f;
    float j2 = (float)(rand() & 0x3fff) * 9.1552734375e-5f;
    Vec3 jit;
    jit.x = j1;
    jit.y = j2;
    p->velocity.x += jit.x;
    p->velocity.y += jit.y;
    p->velocity.z += (float)((rand() & 0x3fff) - 0x1fff) * 6.103515625e-5f;
    return p;
}

// FUNCTION: RALLYTROPHY 0x47d970
void CarParticles::Kind6(float scale, float age)
{
    Particle* p = SpawnCoreB(0.0f);
    if (p) {
        p->age = age;
        p->lifetime = 0.6f;
        p->frame = (rand() & 0xf) | 0x60;
        if ((p->frame & 0xf) >= 12) {
            p->gravity = -9.0f;
            p->size = DAT_004e22b8;
        } else {
            p->gravity = -4.0f;
            p->size = DAT_004e22bc;
        }
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = (float)((rand() & 0x3ff) - 0x200);
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// FUNCTION: RALLYTROPHY 0x47da50
void CarParticles::Kind7(float scale, float age)
{
    Particle* p = SpawnCoreB(0.0f);
    if (p) {
        p->age = age;
        p->lifetime = 0.6f;
        p->frame = (rand() & 0xf) | 0x70;
        if ((p->frame & 0xf) >= 12) {
            p->gravity = -9.0f;
            p->size = DAT_004e22b8;
        } else {
            p->gravity = -4.0f;
            p->size = DAT_004e22bc;
        }
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = (float)((rand() & 0x3ff) - 0x200);
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// FUNCTION: RALLYTROPHY 0x47db30
void CarParticles::Kind8(float scale, float age)
{
    Particle* p = SpawnCoreB(0.0f);
    if (p) {
        p->age = age;
        p->lifetime = 0.6f;
        p->frame = (rand() & 0xf) | 0x80;
        if ((p->frame & 0xf) >= 12) {
            p->gravity = -9.0f;
            p->size = DAT_004e22b8;
        } else {
            p->gravity = -4.0f;
            p->size = DAT_004e22bc;
        }
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = (float)((rand() & 0x3ff) - 0x200);
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// NONMATCHING: RALLYTROPHY 0x47dc10
// 1392 vs 1394 bytes; same three differences as core A (ebp-0x14/-0x10 slot swap of chance and
// the temp, missing fxch in the final compare, push edi position). The ternary condition over
// the surface kind reproduces the original je/je ladder, but its fall-through branch order
// (`jne set; jmp skip` vs the original `je skip; jmp set`) differs.
// Spawn core C: like core A with a speed-scaled upward kick and surface-dependent sideways push.
Particle* CarParticles::SpawnCoreC(float rate)
{
    static float kSlip = 1.0f / (SLIP_C_HI - SLIP_C_LO);
    static float kSpeed = 1.0f / (SPEED_C_HI - SPEED_C_LO);
    static float kSlip2 = 1.0f / (SLIP2_C_HI - SLIP2_C_LO);
    float chance = 0.0f;
    float slip = (float)fabs(PCAR->slipLong);
    if (slip < SLIP_C_HI) {
        if (slip > SLIP_C_LO) {
            chance = (slip - SLIP_C_LO) * SCALE_C * kSlip;
        }
        float speed = BODY_VELOCITY.Length();
        if (speed < SPEED_C_HI) {
            if (speed > SPEED_C_LO) {
                chance += (speed - SPEED_C_LO) * SCALE_C * kSpeed;
            }
            float lat = (float)fabs(PCAR->slipLat);
            if (lat < SLIP2_C_HI) {
                if (lat > SLIP2_C_LO) {
                    chance += (lat - SLIP2_C_LO) * SCALE_C * kSlip2;
                }
            } else {
                chance = SCALE_C;
            }
        } else {
            chance = SCALE_C;
        }
    } else {
        chance = SCALE_C;
    }
    if (!((float)(rand() & 0xff) < chance * rate)) {
        return 0;
    }
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    DAT_0056ae90.head = p->next;
    Vec3 local;
    local.x = PCAR->position.x;
    local.y = PCAR->position.y;
    local.z = PCAR->position.z;
    float h = PCAR->unk264;
    local.y -= h;
    local.x += 0.05f - (rand() & 0xff) * 0.000390625f;
    const float* trans = PBODY->unk14;
    const float* mat = PBODY->unk20;
    const float* in = (const float*)&local;
    float* out = (float*)&p->position;
    BODY_TRANSFORM();
    Vec3 v = BODY_VELOCITY * ((rand() * RAND_SCALE) * 0.25f + 0.7f);
    v += PBODY->FUN_004204f0(p->position);
    p->velocity.x = v.x;
    p->velocity.y = v.y;
    p->velocity.z = v.z;
    float r1 = rand() * RAND_SCALE;
    float r2 = rand() * RAND_SCALE;
    float r3 = rand() * RAND_SCALE;
    float push = BODY_VELOCITY.Length() * SPEED_SCALE_C;
    if (*(int*)&push < 0) {
        push = 0.0f;
    } else if (*(int*)&push > 0x3f800000) {
        push = 1.0f;
    }
    float lon = PCAR->slipLong;
    p->velocity -= BODY_FORWARD * lon * 0.15f * r1;
    float side = 0.0f;
    int kind = PCAR->surfaceKind;
    if (kind == 1 || kind == 2 ? PCAR->slipLat > 0.0f : PCAR->slipLat < 0.0f) {
        side = PCAR->slipLat * r2 * 0.15f;
    }
    p->velocity += BODY_RIGHT * side;
    p->velocity.y = ((float)fabs(PCAR->slipLat) * 0.2f + push * 4.0f) * r3 + p->velocity.y + 3.0f;
    float j1 = (float)((rand() & 0x3fff) - 0x1fff) * 3.0517578125e-5f;
    float j2 = (float)((rand() & 0x3fff) - 0x1fff) * 3.0517578125e-5f;
    Vec3 jit;
    jit.x = j1;
    jit.y = j2;
    p->velocity.x += jit.x;
    p->velocity.y += jit.y;
    p->velocity.z += (float)((rand() & 0x3fff) - 0x1fff) * 3.0517578125e-5f;
    return p;
}

// FUNCTION: RALLYTROPHY 0x47e190
void CarParticles::Kind9(float scale, float age)
{
    Particle* p = SpawnCoreC(0.5f);
    if (p) {
        p->age = age;
        p->lifetime = 3.0f;
        p->size = 0.25f;
        p->gravity = -13.0f;
        p->frame = (rand() & 3) | 0x90;
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = 0.0f;
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// NONMATCHING: RALLYTROPHY 0x47e240
// 1488 vs 1498 bytes. Prologue, guard statics, chance ramp, __asm transform and the AddTo tail match.
// Open: the emit test compiles without the original's fxch (test ah,0x41 instead of test ah,1), the
// second sqrt keeps x,y,z on the FPU stack differently, the first jitter term is not spilled to
// [ebp-0x1c], and p lands in edi where the original has esi.
// Spawn core D: decides from wheel slip and body speed whether to emit, takes a particle from the
// pool and sets position and velocity (the kind functions fill in the rest).
Particle* CarParticles::SpawnCoreD(float rate)
{
    static float inv0 = 1.0f / (D_hi0 - D_lo0);
    static float inv1 = 1.0f / (D_hi1 - D_lo1);
    static float inv2 = 1.0f / (D_hi2 - D_lo2);
    float v;
    float chance = 0;
    v = (float)fabs(s_car->slipA);
    if (v < D_hi0) {
        if (v > D_lo0) {
            chance = (v - D_lo0) * D_k256 * inv0;
        }
        v = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z);
        if (v < D_hi1) {
            if (v > D_lo1) {
                chance += (v - D_lo1) * D_k256 * inv1;
            }
            v = (float)fabs(s_car->slipB);
            if (v < D_hi2) {
                if (v > D_lo2) {
                    chance += (v - D_lo2) * D_k256 * inv2;
                }
            } else {
                chance = D_k256;
            }
        } else {
            chance = D_k256;
        }
    } else {
        chance = D_k256;
    }
    if ((float)(rand() & 0xff) >= chance * rate) {
        return 0;
    }
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    DAT_0056ae90.head = p->next;
    Vec3 local = s_car->pos;
    local.y -= s_car->Radius();
    local.x += 0.05f - (float)(rand() & 0xff) * 3.9062500582e-4f;
    const Vec3* trans = &s_body->pos;
    const float* matrix = s_body->m;
    const Vec3* point = &local;
    Vec3* result = &p->position;
    TRANSFORM_POINT();
    float r1 = (float)rand() * kInv32767;
    float r2 = (float)rand() * kInv32767;
    float r3 = (float)rand() * kInv32767;
    float r4 = (float)rand() * kInv32767;
    Vec3 vel0 = s_body->vel * (r1 * 0.25f + 0.7f);
    vel0 += s_body->PointVelocity(p->position);
    vel0.y *= 0.2f;
    p->velocity.x = vel0.x;
    p->velocity.y = vel0.y;
    p->velocity.z = vel0.z;
    float f = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z) * (1.0f / 35.0f);
    if (*(int*)&f < 0) {
        f = 0;
    } else if (*(int*)&f > 0x3f800000) {
        f = 1.0f;
    }
    p->velocity -= s_body->Forward() * s_car->SlipA() * 0.1f;
    p->velocity -= s_body->Forward() * s_car->SlipA() * 0.15f * r2;
    float lat = 0;
    if (s_car->mode == 1 || s_car->mode == 2) {
        if (s_car->slipB > 0) {
            lat = s_car->SlipB() * r3 * 0.15f;
        }
    } else if (s_car->slipB < 0) {
        lat = s_car->SlipB() * r3 * 0.15f;
    }
    AddTo(p->velocity, s_body->Right() * lat);
    p->velocity.y = ((float)fabs(s_car->slipB) * 0.05f + f * 1.5f) * r4 + p->velocity.y + 2.0f;
    r4 = (float)((rand() & 0x3fff) - 0x1fff) * (1.0f / 20480.0f);
    float jy = (float)((rand() & 0x3fff) - 0x1fff) * (1.0f / 20480.0f);
    p->velocity.x += r4;
    p->velocity.y += jy;
    p->velocity.z += (float)((rand() & 0x3fff) - 0x1fff) * (1.0f / 20480.0f);
    return p;
}


// FUNCTION: RALLYTROPHY 0x47e820
void CarParticles::Kind10(float scale, float age)
{
    Particle* p = SpawnCoreD(0.9f);
    if (p) {
        p->age = age;
        p->lifetime = 1.0f;
        p->size = 0.2f;
        p->gravity = -15.0f;
        p->frame = (rand() & 3) | 0xa0;
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = (float)((rand() & 0x3ff) - 0x200);
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

static const float E_k256 = 256.0f, E_lo0 = 1.0f, E_hi0 = 4.0f, E_lo1 = 2.0f, E_hi1 = 25.0f, E_lo2 = 1.0f, E_hi2 = 4.0f;


// NONMATCHING: RALLYTROPHY 0x47e8f0
// 1410 vs 1418 bytes. Same open items as core D (emit test without fxch, jitter spill) plus the
// AddTo operand order (original loads the temp first: fld [eax]; fadd [esi+0x1c]).
// Spawn core E (dust-like variant of D, no y damping and no second force term).
Particle* CarParticles::SpawnCoreE(float rate)
{
    static float inv0 = 1.0f / (E_hi0 - E_lo0);
    static float inv1 = 1.0f / (E_hi1 - E_lo1);
    static float inv2 = 1.0f / (E_hi2 - E_lo2);
    float v;
    float chance = 0;
    v = (float)fabs(s_car->slipA);
    if (v < E_hi0) {
        if (v > E_lo0) {
            chance = (v - E_lo0) * E_k256 * inv0;
        }
        v = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z);
        if (v < E_hi1) {
            if (v > E_lo1) {
                chance += (v - E_lo1) * E_k256 * inv1;
            }
            v = (float)fabs(s_car->slipB);
            if (v < E_hi2) {
                if (v > E_lo2) {
                    chance += (v - E_lo2) * E_k256 * inv2;
                }
            } else {
                chance = E_k256;
            }
        } else {
            chance = E_k256;
        }
    } else {
        chance = E_k256;
    }
    if (!((float)(rand() & 0xff) < chance * rate)) {
        return 0;
    }
    if (!DAT_0056ae90.head) {
        DAT_0056ae90.Grow();
    }
    Particle* p = DAT_0056ae90.head;
    DAT_0056ae90.head = p->next;
    Vec3 local = s_car->pos;
    local.y -= s_car->Radius();
    local.x += 0.05f - (float)(rand() & 0xff) * 3.9062500582e-4f;
    const Vec3* trans = &s_body->pos;
    const float* matrix = s_body->m;
    const Vec3* point = &local;
    Vec3* result = &p->position;
    TRANSFORM_POINT();
    Vec3 vel0 = s_body->vel * ((float)rand() * kInv32767 * 0.25f + 0.7f);
    vel0 += s_body->PointVelocity(p->position);
    p->velocity.x = vel0.x;
    p->velocity.y = vel0.y;
    p->velocity.z = vel0.z;
    float r2 = (float)rand() * kInv32767;
    float r3 = (float)rand() * kInv32767;
    float r4 = (float)rand() * kInv32767;
    float f = (float)sqrt(s_body->vel.x * s_body->vel.x + s_body->vel.y * s_body->vel.y + s_body->vel.z * s_body->vel.z) * (1.0f / 35.0f);
    if (*(int*)&f < 0) {
        f = 0;
    } else if (*(int*)&f > 0x3f800000) {
        f = 1.0f;
    }
    p->velocity -= s_body->Forward() * s_car->SlipA() * 0.15f * r2;
    float lat = 0;
    if (s_car->mode == 1 || s_car->mode == 2) {
        if (s_car->slipB > 0) {
            lat = s_car->SlipB() * r3 * 0.2f;
        }
        lat = lat + f;
    } else {
        if (s_car->slipB < 0) {
            lat = s_car->SlipB() * r3 * 0.2f;
        }
        lat = lat - f;
    }
    AddTo(p->velocity, s_body->Right() * lat);
    p->velocity.y = ((float)fabs(s_car->slipB) * 0.2f + f * 4.0f) * r4 + p->velocity.y + 3.0f;
    r4 = (float)((rand() & 0x3fff) - 0x1fff) * (1.0f / 32768.0f);
    float jy = (float)((rand() & 0x3fff) - 0x1fff) * (1.0f / 32768.0f);
    p->velocity.x += r4;
    p->velocity.y += jy;
    p->velocity.z += (float)((rand() & 0x3fff) - 0x1fff) * (1.0f / 32768.0f);
    return p;
}

// FUNCTION: RALLYTROPHY 0x47ee80
void CarParticles::Kind11(float scale, float age)
{
    Particle* p = SpawnCoreE(0.5f);
    if (p) {
        p->age = age;
        p->lifetime = 3.0f;
        p->size = 0.2f;
        p->gravity = -10.0f;
        p->frame = (rand() & 3) | 0xb0;
        p->color = color.value;
        p->rotation = (float)(rand() & 0x3ff);
        p->spin = (float)((rand() & 0xff) - 0x80);
        p->position -= p->velocity * scale;
        p->next = particles;
        particles = p;
    }
}

// FUNCTION: RALLYTROPHY 0x47ef50
// SYMBOL: _$E28
typedef void Generated_47ef50();

// FUNCTION: RALLYTROPHY 0x47ef70
// SYMBOL: _$E27
typedef void Generated_47ef70();
