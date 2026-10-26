// Sky (0x4758b0-0x47648f): cloud/sky layers of a Stage (Stage+0x6ac, 0xf70 bytes).
// Map: analysis/range-4741a0.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_4758b0.h"
#include <math.h>

extern "C" void* fun_004aef30();

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

// A file-static 4x4 matrix (0x561b08) the constructor resets to identity.
static SkyMatrix s_skyMatrix;

// Static initialisers of the file-static matrix s_skyMatrix (0x561b08): wrapper, constructor thunk,
// atexit registration and destructor thunk.

// FUNCTION: RALLYTROPHY 0x4758b0
// SYMBOL: _$E21
typedef void Generated_4758b0();

// FUNCTION: RALLYTROPHY 0x4758c0
// SYMBOL: _$E18
typedef void Generated_4758c0();

// FUNCTION: RALLYTROPHY 0x4758d0
// SYMBOL: _$E20
typedef void Generated_4758d0();

// FUNCTION: RALLYTROPHY 0x4758e0
// SYMBOL: _$E19
typedef void Generated_4758e0();

// FUNCTION: RALLYTROPHY 0x4758f0
Sky::Sky()
{
    layerCount = 0;
    direction.x = direction.y = direction.z = 0.0f;
    hasBase = 0;
    visibility = 0.0f;
    s_skyMatrix.m[3] = s_skyMatrix.m[2] = s_skyMatrix.m[1] = 0.0f;
    s_skyMatrix.m[7] = s_skyMatrix.m[6] = s_skyMatrix.m[4] = 0.0f;
    s_skyMatrix.m[11] = s_skyMatrix.m[9] = s_skyMatrix.m[8] = 0.0f;
    s_skyMatrix.m[14] = s_skyMatrix.m[13] = s_skyMatrix.m[12] = 0.0f;
    scale = 0.1f;
    s_skyMatrix.m[15] = s_skyMatrix.m[10] = s_skyMatrix.m[5] = s_skyMatrix.m[0] = 1.0f;
}

// FUNCTION: RALLYTROPHY 0x4759f0
// SYMBOL: ??0SkyLayer@@QAE@XZ
// Compiler-generated: the layer constructor the EH vector iterator calls (Sprite ctor, returns this).
typedef void Generated_4759f0();

// FUNCTION: RALLYTROPHY 0x475a00
// SYMBOL: ??1SkyLayer@@QAE@XZ
// Compiler-generated: the layer destructor (jumps to ~Sprite).
typedef void Generated_475a00();

// FUNCTION: RALLYTROPHY 0x475a10
Sky::~Sky()
{
}

// FUNCTION: RALLYTROPHY 0x475a70
void Sky::SetBase(const char* path, TextureLoader* loader, float scale_)
{
    scale = scale_;
    base.texture = loader->Load(path, 0x840);
    base.unk60[0] = 0.0f;
    base.unk60[1] = 0.0f;
    base.unk60[2] = 0.0f;
    base.unk60[3] = 0.0f;
    base.unk40[0] = 0.0f;
    base.unk40[1] = 0.0f;
    base.unk40[2] = 0.0f;
    base.unk40[3] = 0.0f;
    base.unk9c = 2;
    base.unk70[0] = 0.0f;
    base.unk70[1] = 0.0f;
    base.unk70[2] = 0.0f;
    base.unk70[3] = 0.0f;
    base.unk18 = 5;
    base.unk1c = 2;
    base.unk98 = 0xf;
    base.unk20 = 2;
    base.unk24 = 1;
    hasBase = 1;
}

// original: FUN_00475b20
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x475b20
void Sky::AddLayer(const char* path, TextureLoader* loader, float p0, float p1, float p2, float p3, float p4)
{
    __asm {
        push ebx
        mov ebx, ecx
        mov eax, dword ptr [ebx + 0xe80]
        push esi
        lea ecx, [eax*8]
        sub ecx, eax
        push edi
        lea edx, [eax + ecx*4]
        mov eax, dword ptr [esp + 0x10]
        mov ecx, dword ptr [esp + 0x14]
        lea esi, [ebx + edx*8]
        push 0x840
        xor edi, edi
        push eax
        mov dword ptr [esi + 0x18], 5
        mov dword ptr [esi + 0x1c], 2
        mov dword ptr [esi + 0x14], 1
        mov dword ptr [esi + 0x10], edi
        mov dword ptr [esi + 0x9c], 2
        call fun_004aef30
        mov ecx, dword ptr [esp + 0x18]
        mov edx, dword ptr [esp + 0x1c]
        mov dword ptr [esi], eax
        mov eax, dword ptr [esp + 0x20]
        mov dword ptr [esi + 0x60], edi
        mov dword ptr [esi + 0x64], edi
        mov dword ptr [esi + 0x68], edi
        mov dword ptr [esi + 0x6c], edi
        mov dword ptr [esi + 0x40], edi
        mov dword ptr [esi + 0x44], edi
        mov dword ptr [esi + 0x48], edi
        mov dword ptr [esi + 0x4c], edi
        mov dword ptr [esi + 0x70], edi
        mov dword ptr [esi + 0x74], edi
        mov dword ptr [esi + 0x78], edi
        mov dword ptr [esi + 0x7c], edi
        mov dword ptr [esi + 0x98], 0xf
        mov dword ptr [esi + 0x20], 4
        mov dword ptr [esi + 0x24], 1
        mov dword ptr [esi + 0x18], 2
        mov dword ptr [esi + 0xd4], ecx
        mov ecx, dword ptr [esp + 0x24]
        mov dword ptr [esi + 0xd8], edx
        mov edx, dword ptr [esp + 0x28]
        mov dword ptr [esi + 0xdc], eax
        mov dword ptr [esi + 0xe0], ecx
        mov dword ptr [esi + 0xe4], edx
        mov eax, dword ptr [ebx + 0xe80]
        pop edi
        inc eax
        pop esi
        mov dword ptr [ebx + 0xe80], eax
        pop ebx
        ret 0x1c
    }
}

extern const float kHalfPi; // 0x4e1b28
extern int DAT_00501428;
extern int DAT_0050142c;

// FUNCTION: RALLYTROPHY 0x475c00
void Sky::Render(SkyDevice* device, Camera* camera)
{
    if (!hasBase) {
        return;
    }
    Vec4 clip;
    device->Slot44(1);
    Vec3 point = direction + camera->eyePosition;
    device->Project(&clip, point);
    if (clip.z <= 0.0f) {
        return;
    }
    device->ToScreen(&clip);
    int iw = device->width;
    int ih = device->height;
    float w = (float)iw;
    float h = (float)ih;
    float k = kHalfPi / (camera->fov) * w;
    float y = ((SkyDevice*)DAT_0056b0ac)->GetAspect() * w;
    int vx, vy, vw, vh;
    ((SkyDevice*)DAT_0056b0ac)->GetViewport(&vx, &vy, &vw, &vh);
    float ar = w / h;
    y = (float)vw / vh / ar * y;
    float hx = k * scale * 0.5f;
    float hy = y * scale * 0.5f;
    float left = clip.x - hx;
    float right = clip.x + hx;
    float top = clip.y - hy;
    float bottom = clip.y + hy;
    float maxX = w + 10.0f;
    float maxY = h + 10.0f;
    if (right < -10.0f || left > maxX || bottom < -10.0f || top > maxY) {
        return;
    }
    device->SetTexture(&base);
    Color color(0xffffffff);
    Vec2 quad[4];
    quad[0].x = left;
    quad[0].y = top;
    quad[1].x = right;
    quad[1].y = top;
    quad[2].x = right;
    quad[2].y = bottom;
    quad[3].x = left;
    quad[3].y = bottom;
    device->DrawRect(quad, color, 1.0f, 1.0f);
}

// NONMATCHING: RALLYTROPHY 0x475eb0
// 1437 vs 1441 bytes; call sequence, FPU algorithm and all 18 EH states match. Remaining: camera/this register swap (ebx/esi),
// the original keeps inv = 1/len in the dead device argument slot, a different scheduling of the argument pushes around the four
// vertex expressions, and about 110 of 441 instructions differ by order or stack slot (stack slots of P/A/B/vw/vh).
void Sky::Update(SkyDevice* device, Camera* camera, SkyWorld* world, float dt)
{
    int hasLens = camera->hasLens;
    float prev = visibility;
    visibility = 0.0f;
    Vec4 clip;
    if (layerCount == 0) {
        return;
    }
    device->Slot44(1);
    Vec3 point = direction + camera->eyePosition;
    device->Project(&clip, point);
    if (clip.z <= 0.0f) {
        return;
    }
    Vec3 n(clip.x, clip.y, clip.w);
    float len = n.Length();
    float inv = 1.0f / len;
    n.x = inv * n.x;
    n.y = n.y * inv;
    n.z = inv * n.z;
    device->ToScreen(&clip);
    int iw = device->width;
    int ih = device->height;
    float w = (float)iw;
    float h = (float)ih;
    Vec2 half;
    half.x = w * 0.5f;
    half.y = h * 0.5f;
    Vec2 corner;
    corner.x = clip.x - half.x;
    corner.y = clip.y - half.y;
    Vec3 q = camera->eyePosition;
    float a = (float)fabs(n.z);
    Vec3 r = q + direction;
    Vec3 d = r - q;
    float sc = 150.0f / (r - q).Length();
    r = q + sc * d;
    float hit = TraceRay(world, &q, &r);
    float t = 1.0f - hit;
    if (hasLens && t < prev) {
        float lim = prev - 8.0f * dt;
        if (t < lim) {
            t = lim;
        }
    }
    visibility = t;
    if (t <= 0.0f) {
        return;
    }
    float g = 1.0f - a;
    device->Slot38(0);
    float k = kHalfPi / (camera->fov) * w;
    float y = ((SkyDevice*)DAT_0056b0ac)->GetAspect() * k;
    int vx, vy, vw, vh;
    ((SkyDevice*)DAT_0056b0ac)->GetViewport(&vx, &vy, &vw, &vh);
    float ar = w / h;
    y = (float)vw / vh / ar * y;
    float phase = (clip.x / w + clip.y / h) * 3.1415927f;
    Vec2 quad[4];
    int last = layerCount - 1;
    if (!(DAT_00501428 && DAT_0050142c) && last > 1) {
        last = 1;
    }
    Color color(0xffffffff);
    Vec2 P, A, B;
    for (int i = last; i >= 0; i--) {
        SkyLayer* l = &layers[i];
        float v = (float)pow(a, l->param1) * t * 255.0f;
        int c = FloatToInt(v);
        color.a = color.b = color.g = color.r = (BYTE)c;
        if ((BYTE)c > 2) {
            float s = g * l->param2 + 1.0f;
            s = s * l->param0 * 0.5f;
            float ty = s * y;
            float tx = s * k;
            P.x = corner.x * l->param3 + half.x;
            P.y = corner.y * l->param3 + half.y;
            float ang = phase * l->param4;
            float cs = (float)cos(ang);
            float sn = (float)sin(ang);
            A.x = cs * tx;
            A.y = sn * ty;
            B.x = -(sn * tx);
            B.y = cs * ty;
            quad[0] = P - A - B;
            quad[1] = P + A - B;
            quad[2] = P + A + B;
            quad[3] = P - A + B;
            device->SetTexture(l);
            device->DrawRect(quad, color, 1.0f, 1.0f);
        }
    }
}

// Module end: the STL header statics guard and its atexit registration (the stub of this file).

// FUNCTION: RALLYTROPHY 0x476460
// SYMBOL: _$E28
typedef void Generated_476460();

// FUNCTION: RALLYTROPHY 0x476480
// SYMBOL: _$E27
typedef void Generated_476480();
