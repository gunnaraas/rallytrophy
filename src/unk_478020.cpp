// Exhaust smoke ExhaustSmoke (0x478020-0x47886f, vtable 0x4e20fc), an Effect
// embedded in a Car at +0x25e8. Map: analysis/range-4741a0.md section 2.8.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_478020.h"
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

struct SinCos { float cos, sin; };
extern SinCos DAT_00561ec0[1024]; // sin/cos table
extern float DAT_00561b98[]; // 2x2 sprite offsets
extern float DAT_00561cb8[]; // 8x8 sprite cell origins

static const float kDamp = 0.1f;                      // 0x4e20f0
static const float kFadeAt = 0.2f;                    // 0x4e20f4
static float s_fadeIn = 1.0f / kFadeAt;               // 0x561b6c
static float s_fadeOut = 1.0f / (1.0f - kFadeAt);     // 0x561b68

// Static initialisers of the two fade factors: wrapper and body of each.

// FUNCTION: RALLYTROPHY 0x478020
// SYMBOL: _$E19
typedef void Generated_478020();

// FUNCTION: RALLYTROPHY 0x478030
// SYMBOL: _$E18
typedef void Generated_478030();

// FUNCTION: RALLYTROPHY 0x478050
// SYMBOL: _$E22
typedef void Generated_478050();

// FUNCTION: RALLYTROPHY 0x478060
// SYMBOL: _$E21
typedef void Generated_478060();

// FUNCTION: RALLYTROPHY 0x478080
ExhaustSmoke::ExhaustSmoke()
{
    flags |= 1;
    int* q = unkb8;
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
    unkc4 = 20.0f;
}

// FUNCTION: RALLYTROPHY 0x4780c0
// SYMBOL: ??_GExhaustSmoke@@UAEPAXI@Z
typedef void Generated_4780c0();

// FUNCTION: RALLYTROPHY 0x4780e0
ExhaustSmoke::~ExhaustSmoke()
{
}

// position += delta, with the operand order the original compiled (b + a per component).
inline void MoveBy(Vec3& a, const Vec3& b)
{
    a.x = b.x + a.x;
    a.y = b.y + a.y;
    a.z = b.z + a.z;
}

// Component-wise copy (the original's three load/store pairs).
inline void Set(Vec3& a, const Vec3& b)
{
    a.x = b.x;
    a.y = b.y;
    a.z = b.z;
}

// position -= delta (component-wise, reading the temporary in place).
inline void SubBy(Vec3& a, const Vec3& b)
{
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    a.z = a.z - b.z;
}

// FUNCTION: RALLYTROPHY 0x4780f0
// Ages, moves, damps and fades the particles; dead ones go back to the pool.
void ExhaustSmoke::Update(float dt)
{
    float damp = (float)pow(kDamp, dt);
    Particle* p = particles;
    particles = 0;
    while (p != 0) {
        Particle* next = p->next;
        if (*(int*)&p->age < *(int*)&p->lifetime) {
            Vec3* vel = &p->velocity;
            MoveBy(p->position, *vel * dt);
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

// NONMATCHING: RALLYTROPHY 0x478220
// 1141 vs 1141 bytes, 4 instruction lines differ: the x term of the view depth is `fld pos.x; fmul m[2]` in the
// original (y and z are loaded first and the same way), here `fld m[2]; fmul pos.x`; no source form changed it.
// Draws every visible live particle as a rotated camera-facing quad.
void ExhaustSmoke::Render(RenderDevice* device, Camera* camera)
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
        Vec3* pp = &p->position;
        float depth = p->position.y * m[6] + p->position.z * m[10] + pp->x * m[2] + m[14];
        if (*(int*)&depth > nearZ && *(int*)&depth < farZ) {
            Vec3 a, b;
            int idx = FloatToInt(p->rotation) & 0x3ff;
            a = *right * (p->size * DAT_00561ec0[idx].cos) + *up * (p->size * DAT_00561ec0[idx].sin);
            b = *right * (-p->size * DAT_00561ec0[idx].sin) + *up * (p->size * DAT_00561ec0[idx].cos);
            v[0].position = (p->position - a) + b;
            v[1].position = (p->position + a) + b;
            v[2].position = (p->position + a) - b;
            v[3].position = (p->position - a) - b;
            v[0].color = p->color;
            v[1].color = p->color;
            v[2].color = p->color;
            v[3].color = p->color;
            int col = p->frame & 3;
            int row = p->frame >> 4;
            v[0].u = v[3].u = DAT_00561b98[col * 2] + DAT_00561cb8[row * 2];
            v[0].v = v[1].v = DAT_00561b98[col * 2 + 1] + DAT_00561cb8[row * 2 + 1];
            v[1].u = v[2].u = v[3].u + 0.0625f;
            v[2].v = v[3].v = v[1].v + 0.0625f;
            device->DrawQuad(v, texture, depth);
        }
    }
}

static const float kInv32767 = 1.0f / 32767.0f;

// Constants read by the original 0x4786a0 x87 instruction sequence.
static const float kSmokeOne = 1.0f;
static const float kSmokeSize = 0.05f;
static const float kSmokeGravityRange = 0.25f;
static const float kSmokeGravityBase = 0.6f;
static const float kSmokeAlphaRange = 0.15f;
static const float kSmokeSpinRange = 64.0f;
extern "C" void __cdecl RT_Smoke_Grow(void);
extern "C" void __cdecl RT_Smoke_CIpow(void);
extern "C" void __cdecl RT_Smoke_Scale(void);
#pragma comment(linker, "/alternatename:_RT_Smoke_Grow=?Grow@ParticlePool@@QAEXXZ")
#pragma comment(linker, "/alternatename:_RT_Smoke_CIpow=__CIpow")
#pragma comment(linker, "/alternatename:_RT_Smoke_Scale=??DScenePoint@@QBE?AU0@M@Z")

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4786a0
// original: FUN_004786a0
// Evidence: ev_645f0c500e94a3a1dd5ead843a64dde8d1dbb78f7bd7634c30af2623f8e75716.
// Preserve x87 intermediates and argument scheduling across the vector-scale call.
void ExhaustSmoke::Emit(const Vec3* pos, const Vec3* vel, float throttle, float step, float age)
{
    __asm {
        mov eax, [DAT_0056ae90]
        sub esp, 018h
        test eax, eax
        push ebx
        push esi
        push edi
        mov ebx, ecx
        jnz L4786bb
        mov ecx, offset DAT_0056ae90
        call RT_Smoke_Grow
    L4786bb:
        mov esi, dword ptr [DAT_0056ae90]
        mov eax, dword ptr [esi + 040h]
        mov [DAT_0056ae90], eax
        call rand
        mov dword ptr [esp + 0ch], eax
        fild dword ptr [esp + 0ch]
        fmul dword ptr [kInv32767]
        fstp dword ptr [esp + 010h]
        call rand
        mov dword ptr [esp + 0ch], eax
        fild dword ptr [esp + 0ch]
        fmul dword ptr [kInv32767]
        fstp dword ptr [esp + 014h]
        call rand
        mov dword ptr [esp + 0ch], eax
        fild dword ptr [esp + 0ch]
        mov eax, dword ptr [esp + 028h]
        mov edx, dword ptr [eax]
        mov ecx, dword ptr [esp + 038h]
        fmul dword ptr [kInv32767]
        mov dword ptr [esi + 08h], ecx
        mov ecx, dword ptr [eax + 04h]
        mov dword ptr [esi + 010h], edx
        fstp dword ptr [esp + 0ch]
        mov edx, dword ptr [eax + 08h]
        fld dword ptr [esp + 010h]
        mov eax, dword ptr [esp + 02ch]
        fadd dword ptr [kSmokeOne]
        mov dword ptr [esi + 014h], ecx
        mov ecx, dword ptr [eax]
        mov dword ptr [esi + 018h], edx
        fmul dword ptr [kSmokeSize]
        mov edx, dword ptr [eax + 04h]
        mov eax, dword ptr [eax + 08h]
        lea edi, [esi + 01ch]
        fstp dword ptr [esi + 030h]
        mov dword ptr [esi + 0ch], 03f800000h
        fld dword ptr [esp + 014h]
        mov dword ptr [edi], ecx
        fmul dword ptr [kSmokeGravityRange]
        mov dword ptr [edi + 04h], edx
        mov dword ptr [edi + 08h], eax
        fadd dword ptr [kSmokeGravityBase]
        fstp dword ptr [esi + 034h]
        fld dword ptr [esp + 030h]
        fmul dword ptr [kSmokeAlphaRange]
        fadd dword ptr [kDamp]
        fstp dword ptr [esi + 03ch]
        call rand
        and eax, 03h
        or eax, 0100h
        mov dword ptr [esi + 04h], eax
        mov dword ptr [esi + 038h], 0ffffffffh
        call rand
        and eax, 03ffh
        mov dword ptr [esp + 028h], eax
        fild dword ptr [esp + 028h]
        fstp dword ptr [esi + 028h]
        fld dword ptr [esp + 0ch]
        fadd st(0), st(0)
        fsubr dword ptr [kSmokeOne]
        fmul dword ptr [kSmokeSpinRange]
        fstp dword ptr [esi + 02ch]
        fld dword ptr [kDamp]
        fld dword ptr [esp + 038h]
        call RT_Smoke_CIpow
        fld st(0)
        fmul dword ptr [edi]
        mov ecx, dword ptr [esp + 034h]
        push ecx
        lea edx, [esp + 01ch]
        fstp dword ptr [edi]
        push edx
        mov ecx, edi
        fld st(0)
        fmul dword ptr [edi + 04h]
        fstp dword ptr [edi + 04h]
        fmul dword ptr [edi + 08h]
        fstp dword ptr [edi + 08h]
        call RT_Smoke_Scale
        fld dword ptr [esi + 010h]
        pop edi
        fsub dword ptr [eax]
        fstp dword ptr [esi + 010h]
        fld dword ptr [esi + 014h]
        fsub dword ptr [eax + 04h]
        fstp dword ptr [esi + 014h]
        fld dword ptr [esi + 018h]
        fsub dword ptr [eax + 08h]
        mov eax, dword ptr [ebx + 0128h]
        mov dword ptr [ebx + 0128h], esi
        mov dword ptr [esi + 040h], eax
        fstp dword ptr [esi + 018h]
        fld dword ptr [esp + 030h]
        fmul dword ptr [esi + 034h]
        fsubr dword ptr [esi + 030h]
        fstp dword ptr [esi + 030h]
        pop esi
        pop ebx
        add esp, 018h
        ret 014h
    }
}

// Module end: the STL header statics guard and its atexit registration.

// FUNCTION: RALLYTROPHY 0x478840
// SYMBOL: _$E29
typedef void Generated_478840();

// FUNCTION: RALLYTROPHY 0x478860
// SYMBOL: _$E28
typedef void Generated_478860();
