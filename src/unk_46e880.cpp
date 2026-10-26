// CarLight (0x46e880-0x46eebf): glow sprites of the car lights and their static quad.
// Map: analysis/range-46d830.md section 5.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_46e880.h"
#include <math.h>

// The quad every light draws (original: 0x509420, 0x2c bytes per vertex).
static EffectVertex s_quad[4];
extern "C" void CarLightBaseCtorAlias();
extern "C" char CarLightVftableAlias;
extern "C" void CarLightCtorAlias();
#pragma comment(linker, "/alternatename:_CarLightBaseCtorAlias=??0EffectBase@@QAE@XZ")
#pragma comment(linker, "/alternatename:_CarLightVftableAlias=??_7CarLight@@6B@")
#pragma comment(linker, "/alternatename:??0CarLight@@QAE@XZ=_CarLightCtorAlias")

// Static initialiser of s_quad: wrapper (CRT table entry), vector constructor, atexit registration
// and vector destructor thunk.

// FUNCTION: RALLYTROPHY 0x46e880
// SYMBOL: _$E21
typedef void Generated_46e880();

// FUNCTION: RALLYTROPHY 0x46e890
// SYMBOL: _$E18
typedef void Generated_46e890();

// FUNCTION: RALLYTROPHY 0x46e8b0
// SYMBOL: _$E20
typedef void Generated_46e8b0();

// FUNCTION: RALLYTROPHY 0x46e8c0
// SYMBOL: _$E19
typedef void Generated_46e8c0();

extern "C" {
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x46e8e0
// SYMBOL: CarLightCtorAlias
// Assembly and 239-byte body extent: REA Evidence ev_2fcd008b5b236b178b7a656af7a693903afcc6f3db4ce24f4b73a94d12479368.
void CarLightCtorAlias()
{
    __asm {
        push esi
        mov esi, ecx
        call CarLightBaseCtorAlias
        mov dl, byte ptr [esi+0cch]
        xor eax, eax
        mov ecx, 03f800000h
        and dl, 0fdh
        mov byte ptr [esi+0cch], dl
        mov dword ptr [esi+0c0h], eax
        mov dword ptr [esi], offset CarLightVftableAlias
        mov dword ptr [esi+128h], ecx
        mov dword ptr [esi+12ch], ecx
        mov dword ptr [esi+130h], ecx
        mov dword ptr [esi+0c4h], ecx
        mov dword ptr [esi+0c8h], ecx
        or ecx, 0ffffffffh
        mov dword ptr [esi+124h], eax
        mov dword ptr [esi+138h], eax
        mov dword ptr [esi+0b8h], eax
        mov dword ptr [esi+0bch], eax
        or dl, 1
        mov dword ptr [esi+8], eax
        mov dword ptr [s_quad+0ch], eax
        mov dword ptr [s_quad+10h], eax
        mov dword ptr [s_quad+14h], eax
        mov dword ptr [s_quad+38h], eax
        mov dword ptr [s_quad+3ch], eax
        mov dword ptr [s_quad+40h], eax
        mov dword ptr [s_quad+64h], eax
        mov dword ptr [s_quad+68h], eax
        mov dword ptr [s_quad+6ch], eax
        mov dword ptr [s_quad+90h], eax
        mov dword ptr [s_quad+94h], eax
        mov dword ptr [s_quad+98h], eax
        mov dword ptr [s_quad+28h], eax
        mov dword ptr [s_quad+24h], eax
        mov dword ptr [s_quad+54h], eax
        mov dword ptr [s_quad+50h], eax
        mov dword ptr [s_quad+80h], eax
        mov dword ptr [s_quad+7ch], eax
        mov dword ptr [s_quad+0ach], eax
        mov dword ptr [s_quad+0a8h], eax
        mov byte ptr [esi+0cch], dl
        mov eax, esi
        mov dword ptr [s_quad+18h], ecx
        mov dword ptr [s_quad+44h], ecx
        mov dword ptr [s_quad+70h], ecx
        mov dword ptr [s_quad+9ch], ecx
        pop esi
        ret
    }
}
}

// FUNCTION: RALLYTROPHY 0x46e9d0
// SYMBOL: ??_GCarLight@@UAEPAXI@Z
typedef void Generated_46e9d0();

// FUNCTION: RALLYTROPHY 0x46e9f0
CarLight::~CarLight()
{
}

// FUNCTION: RALLYTROPHY 0x46ea00
void CarLight::SetSize(float s)
{
    radiusB = radiusA = size = s;
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

inline float Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// NONMATCHING: RALLYTROPHY 0x46ea20
// Same algorithm and constants (1091 vs 1124 bytes). Differences: the original loads the near plane into
// the FPU before the dot product, keeps alpha in [ebp-0x10], copies near/far and size through
// dead-parameter stack slots, and builds the quad corners with CSE temporaries at [ebp-0x14/-0x18].
// Draws the glow quad when the light is in front of the camera and faces it.
void CarLight::Render(RenderDevice* device, Camera* camera)
{
    Vec3 v(Position());
    v -= camera->eyePosition;
    if (!(*(float*)&camera->nearBits
          < v.x * camera->forward.x + v.y * camera->forward.y + v.z * camera->forward.z)) {
        return;
    }
    float inv = 1.0f / (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x = v.x * inv;
    v.y = v.y * inv;
    v.z = v.z * inv;
    float facing = Dot(-v, Forward());
    if (*(unsigned*)&facing & 0x80000000) {
        return;
    }
    if (*(int*)&facing < 0) {
        facing = 0.0f;
    } else if (*(int*)&facing > 0x3f800000) {
        facing = 1.0f;
    }
    float e = exponent;
    float a = (float)pow(facing, e) * scale + bias;
    if (*(int*)&a < 0) {
        return;
    }
    if (*(int*)&a > 0x3f800000) {
        a = 1.0f;
    } else if (a <= 0.001f) {
        return;
    }
    const float* m = device->GetViewMatrix();
    float depth = position[0] * m[2] + position[1] * m[6] + position[2] * m[10] + m[14];
    if (*(int*)&depth <= camera->nearBits) {
        return;
    }
    if (*(int*)&depth >= camera->farBits) {
        return;
    }
    float z = depth - size;
    if (*(int*)&z <= camera->nearBits) {
        *(int*)&z = camera->nearBits;
    }
    Vec3 p(Position());
    Vec3 r(Right());
    Vec3 u(Up());
    r.x = r.x * size;
    r.y = r.y * size;
    r.z = r.z * size;
    u.x = u.x * size;
    u.y = u.y * size;
    u.z = u.z * size;
    BYTE c = (BYTE)FloatToInt(a * 255.0f);
    DWORD g = c | (c << 8);
    DWORD col = g | (g << 16);
    s_quad[0].color = col;
    s_quad[1].color = col;
    s_quad[2].color = col;
    s_quad[3].color = col;
    s_quad[0].position.x = p.x + r.x;
    s_quad[0].position.y = p.y + r.y;
    s_quad[0].position.z = p.z + r.z;
    s_quad[0].position += u;
    s_quad[1].position.x = p.x - r.x;
    s_quad[1].position.y = p.y - r.y;
    s_quad[1].position.z = p.z - r.z;
    s_quad[1].position += u;
    s_quad[2].position.x = p.x - r.x;
    s_quad[2].position.y = p.y - r.y;
    s_quad[2].position.z = p.z - r.z;
    s_quad[2].position -= u;
    s_quad[3].position.x = p.x + r.x;
    s_quad[3].position.y = p.y + r.y;
    s_quad[3].position.z = p.z + r.z;
    s_quad[3].position -= u;
    s_quad[0].v = 0.0f;
    s_quad[1].v = 0.0f;
    s_quad[2].v = 1.0f;
    s_quad[3].v = 1.0f;
    if (mirror == 0) {
        s_quad[0].u = 0.0f;
        s_quad[1].u = 1.0f;
        s_quad[2].u = 1.0f;
        s_quad[3].u = 0.0f;
    } else {
        s_quad[0].u = 1.0f;
        s_quad[1].u = 0.0f;
        s_quad[2].u = 0.0f;
        s_quad[3].u = 1.0f;
    }
    device->DrawQuad(s_quad, texture, z);
}

// Module end: static initialiser pieces of s_quad and the STL guard stub.

// FUNCTION: RALLYTROPHY 0x46ee90
// SYMBOL: _$E28
typedef void Generated_46ee90();

// FUNCTION: RALLYTROPHY 0x46eeb0
// SYMBOL: _$E27
typedef void Generated_46eeb0();
