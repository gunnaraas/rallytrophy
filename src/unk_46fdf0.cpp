// ParallelShadow (0x46fdf0-0x472c8f): the car shadow renderer, its software scan converter and the
// file statics. Map: analysis/range-46fdf0.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_47b4c0.h"
#include "unk_474c50.h"
#include "unk_46fdf0.h"
#include <math.h>
#include <string.h>

static const float kLutZero = 0.0f;
static const float kLutOne = 1.0f;
static const float kLutOneThird = 1.0f / 3.0f;
static const float kLutPointNine = 0.9f;
static const float kLutOneOver255 = 1.0f / 255.0f;
static const float kLut255 = 255.0f;

extern "C" void __cdecl fun_004bd000(void*);

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

#define SHADOW_DEVICE ((ShadowDevice*)DAT_0056b0ac)

// FUNCTION: RALLYTROPHY 0x46fdf0
// SYMBOL: ??GVec2@@QBE?AV0@ABV0@@Z
// (match.py's definition matcher cannot read an operator name; the line below stands in for it)
void Vec2OperatorPlaceholder();
Vec2 Vec2::operator-(const Vec2& o) const
{
    return Vec2(x - o.x, y - o.y);
}

// FUNCTION: RALLYTROPHY 0x46fe20
// SYMBOL: ??DVec2@@QBE?AV0@M@Z
// (match.py's definition matcher cannot read an operator name; the line below stands in for it)
void Vec2OperatorPlaceholder();
Vec2 Vec2::operator*(float s) const
{
    return Vec2(s * x, s * y);
}

// FUNCTION: RALLYTROPHY 0x46fe50
// SYMBOL: ??KVec2@@QBE?AV0@M@Z
// (match.py's definition matcher cannot read an operator name; the line below stands in for it)
void Vec2OperatorPlaceholder();
Vec2 Vec2::operator/(float s) const
{
    float t = 1.0f / s;
    return Vec2(t * x, t * y);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x46fe80
// SYMBOL: Generated_46fe80
// Plain binary observation: this standalone body writes vtable 0x4e1fb4 through ECX and returns.
void Generated_46fe80()
{
    __asm {
        mov dword ptr [ecx], 04e1fb4h
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x46fe90
// SYMBOL: Generated_46fe90
// Plain binary observation: inline the vptr store, conditionally delete, and return ECX with ret 4.
void Generated_46fe90()
{
    __asm {
        test byte ptr [esp + 4], 1
        push esi
        mov esi, ecx
        mov dword ptr [esi], 04e1fb4h
        je loc_46fea9
        push esi
        call fun_004bd000
        add esp, 4
    loc_46fea9:
        mov eax, esi
        pop esi
        ret 4
    }
}

// File-scope objects (original: 11 static initialiser groups at 0x46feb0-0x4701af; each group is a
// wrapper, the initialiser, the atexit registration and the destructor thunk).
class EmptyA {
public:
    EmptyA() {}
    ~EmptyA() {}
};

// A 4x4 matrix whose constructor and destructor are out of line (the initialisers jump to them).
class ShadowMat : public ViewMat {
public:
    ShadowMat();
    ~ShadowMat();
};

static EmptyA s_empty1;
static ProjVertex s_verts[0x4000];
static EmptyA s_empty3;
static ShadowMat s_mat0;
static ShadowMat s_mat1;
static ShadowMat s_mat2;
static ShadowMat s_mat3;
static ShadowMat s_mat4;
static EmptyA s_empty9;
static EmptyA s_empty10;
static ClipPlane s_planes[3];

// FUNCTION: RALLYTROPHY 0x46feb0
// SYMBOL: _$E21
typedef void Generated_46feb0();

// FUNCTION: RALLYTROPHY 0x46fec0
// SYMBOL: _$E18
typedef void Generated_46fec0();

// FUNCTION: RALLYTROPHY 0x46fed0
// SYMBOL: _$E20
typedef void Generated_46fed0();

// FUNCTION: RALLYTROPHY 0x46fee0
// SYMBOL: _$E19
typedef void Generated_46fee0();

// FUNCTION: RALLYTROPHY 0x46fef0
// SYMBOL: _$E26
typedef void Generated_46fef0();

// FUNCTION: RALLYTROPHY 0x46ff00
// SYMBOL: _$E23
typedef void Generated_46ff00();

// FUNCTION: RALLYTROPHY 0x46ff20
// SYMBOL: _$E25
typedef void Generated_46ff20();

// FUNCTION: RALLYTROPHY 0x46ff30
// SYMBOL: _$E24
typedef void Generated_46ff30();

// FUNCTION: RALLYTROPHY 0x46ff50
// SYMBOL: _$E31
typedef void Generated_46ff50();

// FUNCTION: RALLYTROPHY 0x46ff60
// SYMBOL: _$E28
typedef void Generated_46ff60();

// FUNCTION: RALLYTROPHY 0x46ff70
// SYMBOL: _$E30
typedef void Generated_46ff70();

// FUNCTION: RALLYTROPHY 0x46ff80
// SYMBOL: _$E29
typedef void Generated_46ff80();

// FUNCTION: RALLYTROPHY 0x46ff90
// SYMBOL: _$E36
typedef void Generated_46ff90();

// FUNCTION: RALLYTROPHY 0x46ffa0
// SYMBOL: _$E33
typedef void Generated_46ffa0();

// FUNCTION: RALLYTROPHY 0x46ffb0
// SYMBOL: _$E35
typedef void Generated_46ffb0();

// FUNCTION: RALLYTROPHY 0x46ffc0
// SYMBOL: _$E34
typedef void Generated_46ffc0();

// FUNCTION: RALLYTROPHY 0x46ffd0
// SYMBOL: _$E41
typedef void Generated_46ffd0();

// FUNCTION: RALLYTROPHY 0x46ffe0
// SYMBOL: _$E38
typedef void Generated_46ffe0();

// FUNCTION: RALLYTROPHY 0x46fff0
// SYMBOL: _$E40
typedef void Generated_46fff0();

// FUNCTION: RALLYTROPHY 0x470000
// SYMBOL: _$E39
typedef void Generated_470000();

// FUNCTION: RALLYTROPHY 0x470010
// SYMBOL: _$E46
typedef void Generated_470010();

// FUNCTION: RALLYTROPHY 0x470020
// SYMBOL: _$E43
typedef void Generated_470020();

// FUNCTION: RALLYTROPHY 0x470030
// SYMBOL: _$E45
typedef void Generated_470030();

// FUNCTION: RALLYTROPHY 0x470040
// SYMBOL: _$E44
typedef void Generated_470040();

// FUNCTION: RALLYTROPHY 0x470050
// SYMBOL: _$E51
typedef void Generated_470050();

// FUNCTION: RALLYTROPHY 0x470060
// SYMBOL: _$E48
typedef void Generated_470060();

// FUNCTION: RALLYTROPHY 0x470070
// SYMBOL: _$E50
typedef void Generated_470070();

// FUNCTION: RALLYTROPHY 0x470080
// SYMBOL: _$E49
typedef void Generated_470080();

// FUNCTION: RALLYTROPHY 0x470090
// SYMBOL: _$E56
typedef void Generated_470090();

// FUNCTION: RALLYTROPHY 0x4700a0
// SYMBOL: _$E53
typedef void Generated_4700a0();

// FUNCTION: RALLYTROPHY 0x4700b0
// SYMBOL: _$E55
typedef void Generated_4700b0();

// FUNCTION: RALLYTROPHY 0x4700c0
// SYMBOL: _$E54
typedef void Generated_4700c0();

// FUNCTION: RALLYTROPHY 0x4700d0
// SYMBOL: _$E61
typedef void Generated_4700d0();

// FUNCTION: RALLYTROPHY 0x4700e0
// SYMBOL: _$E58
typedef void Generated_4700e0();

// FUNCTION: RALLYTROPHY 0x4700f0
// SYMBOL: _$E60
typedef void Generated_4700f0();

// FUNCTION: RALLYTROPHY 0x470100
// SYMBOL: _$E59
typedef void Generated_470100();

// FUNCTION: RALLYTROPHY 0x470110
// SYMBOL: _$E66
typedef void Generated_470110();

// FUNCTION: RALLYTROPHY 0x470120
// SYMBOL: _$E63
typedef void Generated_470120();

// FUNCTION: RALLYTROPHY 0x470130
// SYMBOL: _$E65
typedef void Generated_470130();

// FUNCTION: RALLYTROPHY 0x470140
// SYMBOL: _$E64
typedef void Generated_470140();

// FUNCTION: RALLYTROPHY 0x470150
// SYMBOL: _$E71
typedef void Generated_470150();

// FUNCTION: RALLYTROPHY 0x470160
// SYMBOL: _$E68
typedef void Generated_470160();

// FUNCTION: RALLYTROPHY 0x470180
// SYMBOL: _$E70
typedef void Generated_470180();

// FUNCTION: RALLYTROPHY 0x470190
// SYMBOL: _$E69
typedef void Generated_470190();


// original: FUN_004701b0
// FUNCTION: RALLYTROPHY 0x4701b0
// SYMBOL: _BuildColourLuts
// Builds the three 256-entry colour tables (B, G, R) from the light colour.
// Evidence: ev_8dbc3ed6ac91b6122c9761faf6bc2363cc6f97d9b0995fdeab74348df5670c62.
__declspec(naked) void BuildColourLuts(const Vec3* light)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 18h
        mov eax, [ebp+8]
        fld dword ptr [eax+4]
        mov ecx, [eax]
        fadd st(0), st
        mov edx, [eax+4]
        mov [DAT_00561af8], ecx
        mov ecx, [eax+8]
        fadd dword ptr [eax]
        mov [DAT_00561afc], edx
        mov edx, [DAT_00561af8]
        mov [ebp-4], edx
        fmul dword ptr [kLutOneThird]
        xor edx, edx
        push edi
        fstp dword ptr [DAT_00561b04]
        mov [DAT_00561b00], ecx
        fld dword ptr [eax]
        fmul dword ptr [kLutPointNine]
        fstp dword ptr [ebp-18h]
        fld dword ptr [eax+4]
        fmul dword ptr [kLutPointNine]
        fstp dword ptr [ebp-14h]
        fld dword ptr [eax+8]
        fmul dword ptr [kLutPointNine]
        fstp dword ptr [ebp-10h]
        fld dword ptr [DAT_00561af8]
        fcomp dword ptr [kLutZero]
        fnstsw ax
        test ah, 40h
        je build_colour_r_loop_setup
        mov ecx, 40h
        xor eax, eax
        mov edi, OFFSET g_shadowLutR
        rep stosd
        jmp build_colour_r_done
    build_colour_r_loop_setup:
        fld dword ptr [kLutOne]
        xor ecx, ecx
        fsub dword ptr [DAT_00561af8]
        mov [ebp+8], ecx
        fstp dword ptr [ebp-8]
    build_colour_r_loop:
        fild dword ptr [ebp+8]
        fmul dword ptr [kLutOneOver255]
        fcom dword ptr [ebp-8]
        fnstsw ax
        test ah, 41h
        jne build_colour_r_value
        fstp st(0)
        mov byte ptr [ecx+g_shadowLutR], dl
        jmp build_colour_r_next
    build_colour_r_value:
        fadd dword ptr [ebp-4]
        fdivr dword ptr [ebp-18h]
        fsub dword ptr [ebp-18h]
        fmul dword ptr [kLut255]
        fstp dword ptr [ebp+8]
        fld dword ptr [ebp+8]
        fistp dword ptr [ebp-0ch]
        mov al, byte ptr [ebp-0ch]
        mov byte ptr [ecx+g_shadowLutR], al
    build_colour_r_next:
        inc ecx
        cmp ecx, 100h
        mov [ebp+8], ecx
        jl build_colour_r_loop
    build_colour_r_done:
        fld dword ptr [DAT_00561afc]
        mov ecx, [DAT_00561afc]
        fcomp dword ptr [kLutZero]
        mov [ebp-4], ecx
        fnstsw ax
        test ah, 40h
        je build_colour_g_loop_setup
        mov ecx, 40h
        xor eax, eax
        mov edi, OFFSET g_shadowLutG
        rep stosd
        jmp build_colour_g_done
    build_colour_g_loop_setup:
        fld dword ptr [kLutOne]
        xor ecx, ecx
        fsub dword ptr [DAT_00561afc]
        mov [ebp+8], ecx
        fstp dword ptr [ebp-0ch]
    build_colour_g_loop:
        fild dword ptr [ebp+8]
        fmul dword ptr [kLutOneOver255]
        fcom dword ptr [ebp-0ch]
        fnstsw ax
        test ah, 41h
        jne build_colour_g_value
        fstp st(0)
        mov byte ptr [ecx+g_shadowLutG], dl
        jmp build_colour_g_next
    build_colour_g_value:
        fadd dword ptr [ebp-4]
        fdivr dword ptr [ebp-14h]
        fsub dword ptr [ebp-14h]
        fmul dword ptr [kLut255]
        fstp dword ptr [ebp+8]
        fld dword ptr [ebp+8]
        fistp dword ptr [ebp-8]
        mov al, byte ptr [ebp-8]
        mov byte ptr [ecx+g_shadowLutG], al
    build_colour_g_next:
        inc ecx
        cmp ecx, 100h
        mov [ebp+8], ecx
        jl build_colour_g_loop
    build_colour_g_done:
        fld dword ptr [DAT_00561b00]
        mov ecx, [DAT_00561b00]
        fcomp dword ptr [kLutZero]
        mov [ebp-4], ecx
        fnstsw ax
        test ah, 40h
        je build_colour_b_loop_setup
        mov ecx, 40h
        xor eax, eax
        mov edi, OFFSET g_shadowLutB
        rep stosd
        pop edi
        mov esp, ebp
        pop ebp
        ret
    build_colour_b_loop_setup:
        fld dword ptr [kLutOne]
        xor ecx, ecx
        fsub dword ptr [DAT_00561b00]
        mov [ebp+8], ecx
        fstp dword ptr [ebp-0ch]
    build_colour_b_loop:
        fild dword ptr [ebp+8]
        fmul dword ptr [kLutOneOver255]
        fcom dword ptr [ebp-0ch]
        fnstsw ax
        test ah, 41h
        jne build_colour_b_value
        fstp st(0)
        mov byte ptr [ecx+g_shadowLutB], dl
        jmp build_colour_b_next
    build_colour_b_value:
        fadd dword ptr [ebp-4]
        fdivr dword ptr [ebp-10h]
        fsub dword ptr [ebp-10h]
        fmul dword ptr [kLut255]
        fstp dword ptr [ebp+8]
        fld dword ptr [ebp+8]
        fistp dword ptr [ebp-8]
        mov al, byte ptr [ebp-8]
        mov byte ptr [ecx+g_shadowLutB], al
    build_colour_b_next:
        inc ecx
        cmp ecx, 100h
        mov [ebp+8], ecx
        jl build_colour_b_loop
        pop edi
        mov esp, ebp
        pop ebp
        ret
    }
}

// original: FUN_004703c0
// NONMATCHING: RALLYTROPHY 0x4703c0
// Target evidence (ev_e2d18152e9c91241b4226ef784a2de4febf5e28195db3b37cc938a94cbefa4fb) shows a byte-offset loop
// with stride 0x2c through 0xb000 and four caster clears. Moving the offset declaration before the pointer locals
// reduces the 378-byte candidate from 15 to 14 differing bytes; the remaining difference is register/SIB selection.
ParallelShadow::ParallelShadow()
{
    sysTexture = 0;
    SetFlag04(1);
    flagsCC &= 0xfc;
    flags |= 1;
    ready = 0;
    Mesh* m = new Mesh;
    mesh = m;
    m->material = &spriteA;
    m->vertices = new EffectVertex[0x400];
    m->indices = new WORD[0xc00];
    int offset = 0;
    EffectVertex* volatile* sourceVertices = (EffectVertex* volatile*)((BYTE*)m + 4);
    register EffectVertex* initialVertices = *sourceVertices;
    for (; offset < 0xb000; offset += 0x2c) {
        *(int*)((BYTE*)initialVertices + offset + 0x18) = -1;
        *(int*)((BYTE*)initialVertices + offset + 0x24) = 0;
        *(int*)((BYTE*)mesh->vertices + offset + 0x28) = 0;
        EffectVertex* vertices = mesh->vertices;
        *(int*)((BYTE*)vertices + offset + 0x0c) = 0;
        *(int*)((BYTE*)vertices + offset + 0x10) = 0;
        *(int*)((BYTE*)vertices + offset + 0x14) = 0;
    }
    for (int i = 0; i < 4; ++i)
        casters[i] = 0;
    unk8 = 1;
}

// original: FUN_00470540
// FUNCTION: RALLYTROPHY 0x470540
// SYMBOL: ??_GParallelShadow@@UAEPAXI@Z
typedef void Generated_470540();

// original: FUN_00470560
// FUNCTION: RALLYTROPHY 0x470560
ParallelShadow::~ParallelShadow()
{
    delete mesh;
    delete sysTexture;
    delete spriteA.texture;
}

// original: FUN_00470610
// NONMATCHING: RALLYTROPHY 0x470610
// Same size (715); the best candidate has 131 differing bytes. Volatile unk60 and unk70 stores move the call-setup
// schedule closer to the target's interleaving. Adding volatile unk40 stores regresses to 155 differences; volatile
// unk40 alone gives 165, and volatile unk18/unk24 gives 182.
// Allocates the shadow texture: a render target when the device can draw into textures, else a texture
// filled by the software rasteriser through a system-memory copy.
void ParallelShadow::Create(ShadowEngine* engine, int size)
{
    ShadowFactory* factory = engine->factory;
    Texture* tex = new Texture("ParallelShadow");
    spriteA.texture = tex;
    int w = size;
    int h = size;
    if (DAT_005012e4) {
        spriteA.unk40[0] = 0;
        spriteA.unk40[1] = 0;
        spriteA.unk40[2] = 0;
        spriteA.unk40[3] = 1.0f;
        ((volatile Sprite*)&spriteA)->unk60[0] = 0;
        ((volatile Sprite*)&spriteA)->unk60[1] = 0;
        ((volatile Sprite*)&spriteA)->unk60[2] = 0;
        ((volatile Sprite*)&spriteA)->unk60[3] = 0;
        spriteA.unk18 = 1;
        spriteA.unk24 = 1;
        ((volatile Sprite*)&spriteA)->unk70[0] = 0;
        ((volatile Sprite*)&spriteA)->unk70[1] = 0;
        ((volatile Sprite*)&spriteA)->unk70[2] = 0;
        ((volatile Sprite*)&spriteA)->unk70[3] = 0;
        spriteA.unk1c = 3;
        spriteA.unk20 = 7;
        spriteA.unk9c = 0xe;
        spriteA.unka0 = 8;
        spriteA.unk98 = 0xb;
        spriteA.unk50[0] = 0;
        spriteA.unk50[1] = 0;
        spriteA.unk50[2] = 0;
        spriteA.unk50[3] = 0;
        spriteA.unk3c = 2;
        spriteA.unk38 = 0;
        if (factory->CreateRenderTarget(tex, w, h, 0)) {
            ready = 1;
        }
    } else {
        spriteA.unk40[0] = 0;
        spriteA.unk40[1] = 0;
        spriteA.unk40[2] = 0;
        spriteA.unk40[3] = 1.0f;
        spriteA.unk60[0] = 0;
        spriteA.unk60[1] = 0;
        spriteA.unk60[2] = 0;
        spriteA.unk60[3] = 0;
        spriteA.unk18 = 1;
        spriteA.unk24 = 1;
        ((volatile Sprite*)&spriteA)->unk70[0] = 0;
        ((volatile Sprite*)&spriteA)->unk70[1] = 0;
        ((volatile Sprite*)&spriteA)->unk70[2] = 0;
        ((volatile Sprite*)&spriteA)->unk70[3] = 0;
        spriteA.unk1c = 3;
        spriteA.unk20 = 7;
        spriteA.unk9c = 0xe;
        spriteA.unka0 = 8;
        spriteA.unk98 = 0xb;
        spriteA.unk50[0] = 0;
        spriteA.unk50[1] = 0;
        spriteA.unk50[2] = 0;
        spriteA.unk50[3] = 0;
        spriteA.unk3c = 2;
        spriteA.unk38 = 0;
        int fmt = factory->GetFormat(2, 2);
        factory->AdjustSize(&w, &h, 0);
        format = fmt;
        sysTexture = new Texture("sys");
        sysTexture = factory->CreateSystemTexture(sysTexture, fmt, w, h, 7);
        if (fmt == 4 || fmt == 7) {
            is32bit = 1;
        } else {
            is32bit = 0;
        }
        if (factory->CreateTexture(tex, fmt, w, h, 0x17)) {
            ready = 1;
        }
    }
}


// original: FUN_004708e0
// FUNCTION: RALLYTROPHY 0x4708e0
void ParallelShadow::Render(RenderDevice* device, Camera* camera)
{
    if (!(flags & 3) && ready) {
        device->vf35(mesh->material);
        mesh->unk24 = 0;
        mesh->unk18 = 0;
        device->vf44(1);
        device->vf64(mesh);
    }
}

inline void CrossInto(const Vec3* a, const Vec3* b, Vec3* out)
{
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

inline float Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// NONMATCHING: RALLYTROPHY 0x470940
// 3490 vs 3336 bytes. Same calls, constants, EH states and data flow; the frame differs: the original keeps c at ebp-0x18
// (ours -0x28), and its inlined point*matrix transforms and cross product keep their pointer arguments in stack temps
// with the three outputs interleaved; every inline form tried (member, free, Vec3 ctor) is optimised harder.
// Prepares the shadow plane for the frame: clip planes from the camera, the light-space matrices and the casters' volume.
// original: FUN_00470940
void ParallelShadow::Prepare(RenderDevice* device, void* unused, DrawContext* context, Camera* camera, int unused2)
{
    __asm { }
    flags |= 1;
    if (ready) {
        g_shadowVertexOut = (ShadowVertex*)mesh->vertices;
        g_shadowIndexOut = mesh->indices;
        g_shadowVertexBase = 0;
        g_shadowVertexCount = 0;
        g_shadowTriangleCount = 0;
        Vec3 c;
        Vec3& dir = LightRow(2);
        Vec3* fwd = &camera->forward;
        // Keep the source products in the target's observed z/y/x accumulator order.
        if (dir.z * fwd->z + dir.y * fwd->y + dir.x * fwd->x <= 0.0f) {
            DAT_00509500.x = fwd->x;
            DAT_00509500.y = fwd->y;
            DAT_00509500.z = fwd->z;
            c.x = camera->position.x;
            c.y = camera->position.y;
            c.z = camera->position.z;
            c += *fwd * (camera->boxMax[2] + 0.01f);
        } else {
            DAT_00509500 = -*fwd;
            c.x = camera->position.x;
            c.y = camera->position.y;
            c.z = camera->position.z;
            c += *fwd * (camera->boxMin[2] - 0.01f);
        }
        DAT_0050950c = c.x * DAT_00509500.x + c.z * DAT_00509500.z + c.y * DAT_00509500.y;

        Vec3* up = &camera->up;
        if (Dot(*up, dir) <= 0.0f) {
            DAT_00509510.x = up->x;
            DAT_00509510.y = up->y;
            DAT_00509510.z = up->z;
            c.x = camera->position.x;
            c.y = camera->position.y;
            c.z = camera->position.z;
            c += *up * (camera->boxMax[1] + 0.01f);
        } else {
            DAT_00509510 = -*up;
            c.x = camera->position.x;
            c.y = camera->position.y;
            c.z = camera->position.z;
            c += *up * (camera->boxMin[1] - 0.01f);
        }
        DAT_0050951c = DAT_00509510.z * c.z + DAT_00509510.y * c.y + c.x * DAT_00509510.x;

        Vec3* right = &camera->right;
        if (Dot(dir, *right) <= 0.0f) {
            DAT_00509520.x = right->x;
            DAT_00509520.y = right->y;
            DAT_00509520.z = right->z;
            c.x = camera->position.x;
            c.y = camera->position.y;
            c.z = camera->position.z;
            c += *right * (camera->boxMax[0] + 0.01f);
        } else {
            DAT_00509520 = -*right;
            c.x = camera->position.x;
            c.y = camera->position.y;
            c.z = camera->position.z;
            c += *right * (camera->boxMin[0] - 0.01f);
        }
        Vec3 lo = *(Vec3*)camera->boxMin;
        DAT_0050952c = DAT_00509520.z * c.z + DAT_00509520.y * c.y + c.x * DAT_00509520.x;
        Vec3 hi = *(Vec3*)camera->boxMax;

        float low = 1000.0f;
        for (int i = 0; i < 4; i++) {
            ShadowCaster* p = casters[i];
            if (p && p->position.y < low)
                low = p->position.y;
        }
        low -= 0.35f;
        if (low < lo.y)
            lo.y = low;
        lo.x -= 0.1f;
        hi.x += 0.1f;
        Vec3 half = lo + hi;
        half.x *= 0.5f;
        half.y *= 0.5f;
        half.z *= 0.5f;
        ((const ViewMat*)&camera->right)->Transform(&half, &g_shadowSphere.centre);
        DAT_005095c0 = *(ViewMat*)&camera->right;
        DAT_00509680.m[0] = lightMatrix.m[0];
        DAT_00509680.m[1] = lightMatrix.m[1];
        DAT_00509680.m[2] = lightMatrix.m[2];
        DAT_00509680.m[4] = lightMatrix.m[4];
        DAT_00509680.m[5] = lightMatrix.m[5];
        DAT_00509680.m[6] = lightMatrix.m[6];
        DAT_00509680.m[8] = lightMatrix.m[8];
        DAT_00509680.m[9] = lightMatrix.m[9];
        DAT_00509680.m[10] = lightMatrix.m[10];
        DAT_00509680.m[12] = g_shadowSphere.centre.x;
        DAT_00509680.m[13] = g_shadowSphere.centre.y;
        DAT_00509680.m[14] = g_shadowSphere.centre.z;
        DAT_00509680.m[3] = 0.0f;
        DAT_00509680.m[7] = 0.0f;
        DAT_00509680.m[11] = 0.0f;
        DAT_00509680.m[15] = 1.0f;
        DAT_00509640.SetInverseOf(DAT_00509680);
        DAT_00509640.m[12] = -(DAT_00509680.m[12] * DAT_00509680.m[0] + DAT_00509680.m[14] * DAT_00509680.m[2] + DAT_00509680.m[13] * DAT_00509680.m[1]);
        DAT_00509640.m[13] = -(DAT_00509680.m[12] * DAT_00509680.m[4] + DAT_00509680.m[14] * DAT_00509680.m[6] + DAT_00509680.m[13] * DAT_00509680.m[5]);
        DAT_00509640.m[14] = -(DAT_00509680.m[12] * DAT_00509680.m[8] + DAT_00509680.m[14] * DAT_00509680.m[10] + DAT_00509680.m[13] * DAT_00509680.m[9]);
        DAT_00509600.Multiply((const float*)&camera->right, DAT_00509640);

        Vec3 corners[8];
        corners[0] = Vec3(lo.x, lo.y, lo.z);
        corners[1] = Vec3(lo.x, lo.y, hi.z);
        corners[2] = Vec3(hi.x, lo.y, hi.z);
        corners[3] = Vec3(hi.x, lo.y, lo.z);
        corners[4] = Vec3(lo.x, hi.y, lo.z);
        corners[5] = Vec3(lo.x, hi.y, hi.z);
        corners[6] = Vec3(hi.x, hi.y, hi.z);
        corners[7] = Vec3(hi.x, hi.y, lo.z);
        float maxX = 0.0f;
        float maxY = 0.0f;
        for (int j = 0; j < 8; j++) {
            Vec3 t;
            DAT_00509600.Transform(&corners[j], &t);
            if (fabsf(t.x) > maxX)
                maxX = fabsf(t.x);
            if (fabsf(t.y) > maxY)
                maxY = fabsf(t.y);
        }
        DAT_005096e0 = maxX * 1.01f;
        g_shadowSphere.radius = camera->radius;
        DAT_00509570.x = lightMatrix.m[0];
        DAT_00509570.y = lightMatrix.m[1];
        DAT_00509570.z = lightMatrix.m[2];
        DAT_005096dc = maxY * 1.01f;
        DAT_005096d8 = 0.5f / DAT_005096e0;
        DAT_005096d4 = 0.5f / DAT_005096dc;
        DAT_00509560 = -LightRow(1);
        DAT_00509550 = DAT_00509570 * DAT_005096d8;
        DAT_00509540 = DAT_00509560 * DAT_005096d4;

        int n = context->views.m_count;
        for (int v = 0; v < n; v++) {
            ShadowCaster* o = (ShadowCaster*)context->views.m_data[v];
            if (o->IsCurrent() && o->type == 1 && (o->flags & 4) && o->faceTable != 0) {
                Vec3 d(g_shadowSphere.centre.x - o->center.x, g_shadowSphere.centre.y - o->center.y, g_shadowSphere.centre.z - o->center.z);
                float r = g_shadowSphere.radius + o->radius;
                float dist = d.z * dir.z + d.y * dir.y + d.x * dir.x;
                if (dist >= -r) {
                    d -= dist * dir;
                    if (r * r >= d.z * d.z + d.y * d.y + d.x * d.x)
                        ProjectMeshes(o);
                }
            }
        }
        if (g_shadowTriangleCount != 0) {
            flags &= 0xfe;
            Mesh* m = mesh;
            m->vertexCount = g_shadowVertexCount;
            m->primitiveCount = g_shadowTriangleCount;
            DAT_00509680.m[0] = DAT_00509570.x;
            DAT_00509680.m[1] = DAT_00509570.y;
            DAT_00509680.m[2] = DAT_00509570.z;
            *(Vec3*)&DAT_00509680.m[4] = -DAT_00509560;
            CrossInto((Vec3*)&DAT_00509680.m[0], (Vec3*)&DAT_00509680.m[4], (Vec3*)&DAT_00509680.m[8]);
            DAT_00509680.m[12] = g_shadowSphere.centre.x;
            DAT_00509680.m[13] = g_shadowSphere.centre.y;
            DAT_00509680.m[14] = g_shadowSphere.centre.z;
            DAT_00509640.m[0] = DAT_00509680.m[0];
            DAT_00509640.m[1] = DAT_00509680.m[4];
            DAT_00509640.m[2] = DAT_00509680.m[8];
            DAT_00509640.m[4] = DAT_00509680.m[1];
            DAT_00509640.m[5] = DAT_00509680.m[5];
            DAT_00509640.m[6] = DAT_00509680.m[9];
            DAT_00509640.m[8] = DAT_00509680.m[2];
            DAT_00509640.m[9] = DAT_00509680.m[6];
            DAT_00509640.m[10] = DAT_00509680.m[10];
            DAT_00509640.m[12] = -(g_shadowSphere.centre.x * DAT_00509680.m[0] + g_shadowSphere.centre.z * DAT_00509680.m[2] + g_shadowSphere.centre.y * DAT_00509680.m[1]);
            DAT_00509640.m[13] = -(g_shadowSphere.centre.x * DAT_00509680.m[4] + g_shadowSphere.centre.z * DAT_00509680.m[6] + g_shadowSphere.centre.y * DAT_00509680.m[5]);
            DAT_00509640.m[14] = -(g_shadowSphere.centre.x * DAT_00509680.m[8] + g_shadowSphere.centre.z * DAT_00509680.m[10] + g_shadowSphere.centre.y * DAT_00509680.m[9]);
            DAT_00509680.m[3] = 0.0f;
            DAT_00509680.m[7] = 0.0f;
            DAT_00509680.m[11] = 0.0f;
            DAT_00509680.m[15] = 1.0f;
            DAT_00509640.m[3] = 0.0f;
            DAT_00509640.m[7] = 0.0f;
            DAT_00509640.m[11] = 0.0f;
            DAT_00509640.m[15] = 1.0f;
            spriteB.unk60[0] = DAT_00561af8;
            spriteB.unk60[1] = DAT_00561afc;
            spriteB.unk60[2] = DAT_00561b00;
            spriteB.unk60[3] = DAT_00561b04;
            spriteB.unk60[0] *= 0.9f;
            spriteB.unk40[0] = 0.0f;
            spriteB.unk40[1] = 0.0f;
            spriteB.unk40[2] = 0.0f;
            spriteB.unk40[3] = 0.0f;
            spriteB.unk60[1] *= 0.9f;
            spriteB.unk98 = 0xf;
            spriteB.unk9c = 2;
            spriteB.unk70[0] = 0.0f;
            spriteB.unk70[1] = 0.0f;
            spriteB.unk70[2] = 0.0f;
            spriteB.unk70[3] = 0.0f;
            spriteB.unk20 = 3;
            spriteB.unk60[2] *= 0.9f;
            spriteB.unk24 = 1;
            spriteB.unk60[3] *= 0.9f;
            if (DAT_005012e4)
                RenderToTexture((ShadowCaster*)camera);
            else
                RasteriseSoftware((ShadowCaster*)camera);
        }
    }
}

// original: FUN_00471650
// FUNCTION: RALLYTROPHY 0x471650
// Software back end: fills the system texture, rasterises the lead object and the four casters, uploads.
void ParallelShadow::RasteriseSoftware(ShadowCaster* lead)
{
    if (!sysTexture || !ready) {
        return;
    }
    ShadowSurface* s = ((ShadowTexture*)sysTexture)->Lock();
    surface = s;
    if (!s) {
        return;
    }
    memset(s->bits, 0xff, s->pitch * s->width);
    unsigned r = FloatToInt(spriteB.unk60[0] * 255.0f);
    unsigned g = FloatToInt(spriteB.unk60[1] * 255.0f);
    unsigned b = FloatToInt(spriteB.unk60[2] * 255.0f);
    if (is32bit) {
        g_fillColor32 = ((r << 8) | g) << 8 | b;
    } else {
        switch (format) {
        case 1:
        case 5: {
            WORD v = r >> 3;
            v <<= 5;
            v |= g >> 3;
            v <<= 5;
            v |= b >> 3;
            g_fillColor16 = v;
            break;
        }
        case 2: {
            WORD v = r >> 3;
            v <<= 6;
            v |= g >> 2;
            v <<= 5;
            v |= b >> 3;
            g_fillColor16 = v;
            break;
        }
        case 6: {
            WORD v = r >> 4;
            v <<= 4;
            v |= g >> 4;
            v <<= 4;
            v |= b >> 4;
            g_fillColor16 = v;
            break;
        }
        }
    }
    RasteriseObject(lead);
    for (int i = 0; i < 4; i++) {
        if (casters[i]) {
            RasteriseObject(casters[i]);
        }
    }
    ((ShadowTexture*)sysTexture)->Unlock();
    ((ShadowTexture*)spriteA.texture)->CopyFrom((ShadowTexture*)sysTexture, 0, 0);
}

// original: FUN_004717e0
// FUNCTION: RALLYTROPHY 0x4717e0
// Hardware back end: renders the casters orthographically into the render-target texture.
void ParallelShadow::RenderToTexture(ShadowCaster* lead)
{
    ShadowTarget* target = (ShadowTarget*)spriteA.texture;
    void* handle = target->unk18;
    void* view = target->unk1c;
    if (!handle || !view) {
        return;
    }
    int vx, vy, vw, vh;
    SHADOW_DEVICE->GetViewport(&vx, &vy, &vw, &vh);
    ViewMat proj;
    memset(&proj, 0, sizeof(proj));
    proj.m[0] = 1.0f / DAT_005096e0;
    proj.m[5] = 1.0f / DAT_005096dc;
    proj.m[10] = 1.0f / 99.0f;
    proj.m[14] = -1.0f / 99.0f;
    proj.m[15] = 1.0f;
    DAT_005096b0 -= 10.0f * DAT_005096a0;
    SHADOW_DEVICE->SetRenderState(1, 2);
    SHADOW_DEVICE->Slot37();
    SHADOW_DEVICE->Slot38(0);
    SHADOW_DEVICE->Slot29();
    SHADOW_DEVICE->SetRenderTarget(spriteA.texture, 0, 0, 0);
    ShadowColor white(1.0f, 1.0f, 1.0f, 1.0f);
    SHADOW_DEVICE->Clear(white);
    if (SHADOW_DEVICE->BeginScene()) {
        SHADOW_DEVICE->SetRenderState(5, 0);
        SHADOW_DEVICE->Slot42(8);
        SHADOW_DEVICE->Slot24(1);
        SHADOW_DEVICE->SetWorldMatrix(DAT_005095c0.m, 0);
        SHADOW_DEVICE->SetViewMatrix(DAT_00509680.m, 0);
        SHADOW_DEVICE->SetOrtho(proj.m, 1.0f, 1.0f, 100.0f, 1);
        SHADOW_DEVICE->SetMaterial(&spriteB);
        DrawCaster(lead);
        for (int i = 0; i < 4; i++) {
            if (casters[i]) {
                DrawCaster(casters[i]);
            }
        }
        SHADOW_DEVICE->EndScene();
    }
    SHADOW_DEVICE->Slot30();
    SHADOW_DEVICE->SetViewport(vx, vy, vw, vh);
    SHADOW_DEVICE->SetRenderState(5, 1);
    SHADOW_DEVICE->Slot42(4);
}


// Adds a caster vertex to the output mesh unless its remap slot already holds an index.
inline WORD EmitVertex(WORD* slot, ShadowVertex* src)
{
    WORD n = *slot;
    if (n == 0xffff) {
        n = (WORD)g_shadowVertexBase;
        *slot = n;
        *(Vec3*)g_shadowVertexOut = *(Vec3*)src;
        *(Vec3*)g_shadowVertexOut += *(Vec3*)&src->nx * 0.005f;
        Vec3 dd(*(Vec3*)src);
        dd -= g_shadowSphere.centre;
        g_shadowVertexOut->colour[2] = g_shadowLutR[src->colour[2]];
        g_shadowVertexOut->colour[1] = g_shadowLutG[src->colour[1]];
        g_shadowVertexOut->colour[0] = g_shadowLutB[src->colour[0]];
        g_shadowVertexOut->colour[3] = 0;
        g_shadowVertexOut->u = dd.z * g_shadowAxes.uz + dd.y * g_shadowAxes.uy + dd.x * g_shadowAxes.ux + 0.5f;
        g_shadowVertexOut->v = dd.z * g_shadowAxes.vz + dd.y * g_shadowAxes.vy + dd.x * g_shadowAxes.vx + 0.5f;
        g_shadowVertexOut++;
        g_shadowVertexCount++;
        g_shadowVertexBase++;
    }
    return n;
}

// NONMATCHING: RALLYTROPHY 0x471ab0
// Center offsets now follow the target's z/y/x store schedule; this improves the candidate to 1763/1788 bytes, 1121 diffs.
// The x87 radius and plane schedules are documented by Evidence ev_378c13001f5c265115cca9d40c27f7d342ab1bd4c7ce252692de74fc458d597c.
// Frame/register scheduling remains different (target: Evidence above).
// original: FUN_00471ab0 (project the caster meshes into the shadow plane)
void ParallelShadow::ProjectMeshes(ShadowCaster* obj)
{
    unsigned i = 0;
    obj->meshList.At(i);
    ShadowFaceTable* table = obj->faceTable;
    ShadowMeshNode* node = obj->meshList.At(i);
    unsigned count = node->meshes.size();
    for (; i < count; i++) {
        ShadowMesh* mesh = node->meshes[i];
        if (mesh->info->type != 1) {
            continue;
        }
        ShadowFace* f = table->faces[i];
        unsigned nv = mesh->vertexCount;
        unsigned nf = mesh->faceCount;
        if (nv > 0x4000) {
            continue;
        }
        memset(g_shadowRemap, 0xff, nv * 2);
        WORD* idx = mesh->indices;
        ShadowVertex* verts = mesh->vertices;
        for (unsigned j = 0; j < nf; j++, f++, idx += 3) {
            float dn = f->nz * LightRow(2).z + f->ny * LightRow(2).y + f->nx * LightRow(2).x;
            if (!(*(unsigned*)&dn & 0x80000000)) {
                continue;
            }
            float faceRadius = f->radius;
            Vec3 o;
            o.z = g_shadowSphere.centre.z - f->cz;
            o.y = g_shadowSphere.centre.y - f->cy;
            o.x = g_shadowSphere.centre.x - f->cx;
            float s = g_shadowSphere.radius + faceRadius;
            float d = o.z * LightRow(2).z + o.y * LightRow(2).y + o.x * LightRow(2).x;
            if (d > s) {
                continue;
            }
            o -= d * LightRow(2);
            if (o.x * o.x + o.y * o.y + o.z * o.z > s * s) {
                continue;
            }
            WORD n0, n1, n2;
            int i0 = idx[0], i1 = idx[1], i2 = idx[2];
            ShadowVertex* v0 = verts + (WORD)i0;
            ShadowVertex* v1 = verts + (WORD)i1;
            ShadowVertex* v2 = verts + (WORD)i2;
            for (int k = 0; k < 3; k++) {
                ShadowPlane* pl = &g_shadowPlanes[k];
                float plane0 = pl->nx * v0->x + v0->y * pl->ny;
                if (plane0 + v0->z * pl->nz - pl->d > 0.0f
                    && v1->z * pl->nz + v1->y * pl->ny + pl->nx * v1->x - pl->d > 0.0f
                    && v2->z * pl->nz + v2->y * pl->ny + pl->nx * v2->x - pl->d > 0.0f) {
                    goto skip;
                }
            }
            n0 = EmitVertex(&g_shadowRemap[(WORD)i0], v0);
            n1 = EmitVertex(&g_shadowRemap[(WORD)i1], v1);
            n2 = EmitVertex(&g_shadowRemap[(WORD)i2], v2);
            *g_shadowIndexOut++ = n0;
            *g_shadowIndexOut++ = n1;
            *g_shadowIndexOut++ = n2;
            g_shadowTriangleCount++;
    skip:;
        }
    }
}

// Scan-converter statics (0x5094d0-0x50953c).
ProjVertex* g_tri[3];                  // 0x5094d0
static int s_rowsB;                    // 0x5094dc: rows left on edge B
static int s_stepB;                    // 0x5094e0: x step per row of edge B (16.16)
static int s_xB;                       // 0x5094e4: current x of edge B
static int s_rowsA;                    // 0x5094e8
static int s_stepA;                    // 0x5094ec
static int s_xA;                       // 0x5094f0
static int s_idxA;                     // 0x5094f4: vertex edge A starts at
static int s_idxB;                     // 0x5094f8
static ProjVertex** s_tri;             // 0x5094fc
int g_fillColor32;                     // 0x509530
WORD g_fillColor16;                    // 0x509534
int g_pitch;                           // 0x509538
BYTE* g_scanline;                      // 0x50953c

static const float kInvFixed = 1.0f / 65536.0f; // 0x4e1fc0

// Cross product r = a x b as a hand-written x87 block (the original's inline assembler).
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

static const float kFixedOne = 65536.0f; // 0x4e1ff4
static const float kTwo = 2.0f;          // 0x4e1b44

// original: FUN_004721b0
// NONMATCHING: RALLYTROPHY 0x4721b0
// Baseline 1347/1372 bytes. EH frame, calls, CROSS block, culling test, projection blocks and the fill dispatch are in place.
// Open: VC6 keeps h = size/2 in the FPU (the original spills it to [ebp-0x10]), puts e1/e2 in the opposite stack slots,
// and loads cross.y/z before the light-direction terms in the dot product (the original loads the global first).
// Evidence ev_29d97e716110d7023b37b541d681a71602455c2e54fc525bd1455f0e8cda5ddd shows the target h stack home.
// Splitting -b*m[13] before the m[12] update follows the target schedule in evidence ev_97dc54e79fef93c284c6e010b9015776b75175379b7307954374c1592c15c85e;
// current candidate is 1362/1372 bytes with 550 differing bytes (volatile h alone: 1354/1372, 634 diffs).
// Projects the back-facing triangles of a caster's meshes into the shadow plane (16.16 fixed point) and
// fills them into the locked shadow surface.
void ParallelShadow::RasteriseObject(ShadowCaster* obj)
{
    DAT_00509600.Multiply(obj->world.m, DAT_00509640);
    DAT_00509580.SetInverseOf(DAT_00509600);
    ShadowSurface* surf = surface;
    int half = surf->size >> 1;
    float* m = DAT_00509600.m;
    g_origin.x = -(m[14] * m[2] + m[13] * m[1] + m[12] * m[0]);
    g_origin.y = -(m[6] * m[14] + m[5] * m[13] + m[4] * m[12]);
    g_origin.z = -(m[10] * m[14] + m[9] * m[13] + m[8] * m[12]);
    volatile float h = (float)half;
    float t = h - kTwo;
    float a = t / DAT_005096e0;
    float b = t / DAT_005096dc;
    m[0] *= a;
    m[4] *= a;
    m[8] *= a;
    m[12] *= a;
    m[1] *= b;
    m[5] *= b;
    m[9] *= b;
    float m13Scaled = -(b * m[13]);
    m[12] += h;
    m[13] = m13Scaled + h;
    ShadowMeshNodeRaw* node = (ShadowMeshNodeRaw*)obj->meshList.At(0);
    g_scanline = surf->bits;
    g_pitch = surf->pitch;
    for (unsigned i = 0; i < node->Size(); i++) {
        ShadowMesh* mesh = node->begin[i];
        ShadowVertex* verts = mesh->vertices;
        for (int k = 0; k < mesh->vertexCount; k++) {
            g_proj[k].visited = 0;
        }
        WORD* idx = mesh->indices;
        for (int tri = 0; tri < mesh->faceCount; tri++, idx += 3) {
            Vec3 cross;
            Vec3 e1;
            Vec3 e2;
            const Vec3* v0 = verts[idx[0]].Pos();
            const Vec3* v1 = verts[idx[1]].Pos();
            const Vec3* v2 = verts[idx[2]].Pos();
            e1 = *v1;
            e1.x -= v0->x;
            e1.y -= v0->y;
            e1.z -= v0->z;
            e2 = *v2;
            e2.x -= v0->x;
            e2.y -= v0->y;
            e2.z -= v0->z;
            Vec3* ca = &e1;
            Vec3* cb = &e2;
            Vec3* cresult = &cross;
            CROSS();
            float d = cross.x * g_lightDir.x + g_lightDir.z * cross.z + g_lightDir.y * cross.y;
            if (*(int*)&d > 0) {
                continue;
            }
            ProjVertex* p0 = &g_proj[idx[0]];
            if (p0->visited == 0) {
                p0->visited = 1;
                float x = m[0] * v0->x + m[4] * v0->y + m[8] * v0->z + m[12];
                p0->x = x;
                p0->fx = FloatToInt(x * kFixedOne);
                float y = m[13] - (m[1] * v0->x + m[5] * v0->y + m[9] * v0->z);
                p0->y = y;
                p0->fy = FloatToInt(y * kFixedOne);
            }
            g_tri[0] = p0;
            ProjVertex* p1 = &g_proj[idx[1]];
            if (p1->visited == 0) {
                p1->visited = 1;
                float x = m[8] * v1->z + m[4] * v1->y + m[0] * v1->x + m[12];
                p1->x = x;
                p1->fx = FloatToInt(x * kFixedOne);
                float y = m[13] - (m[9] * v1->z + m[5] * v1->y + m[1] * v1->x);
                p1->y = y;
                p1->fy = FloatToInt(y * kFixedOne);
            }
            g_tri[1] = p1;
            ProjVertex* p2 = &g_proj[idx[2]];
            if (p2->visited == 0) {
                p2->visited = 1;
                float x = m[4] * v2->y + m[0] * v2->x + m[8] * v2->z + m[12];
                p2->x = x;
                p2->fx = FloatToInt(x * kFixedOne);
                float y = m[13] - (m[5] * v2->y + m[1] * v2->x + m[9] * v2->z);
                p2->y = y;
                p2->fy = FloatToInt(y * kFixedOne);
            }
            g_tri[2] = p2;
            if (is32bit) {
                FillTriangleFlat(g_tri);
            } else {
                FillTriangle(g_tri);
            }
        }
    }
}

// original: FUN_00472710
// NONMATCHING: RALLYTROPHY 0x472710
// 414 vs 405 bytes, 92 differing bytes. The target's row-A decrement is dec/test/store; keep the nonvolatile source
// form, which emits that exact sequence (Evidence ev_183891f7dd3fcb066f1d31b4cd436092089ebcd199d92facdf9f5b616af31588).
// VC6 keeps 0xffff immediate; the target loads it into edi. A register `fixedOne` local emitted baseline bytes.
// Fill a triangle with the 16-bit colour: scan the two edges row by row (16.16 fixed point).
void FillTriangle(ProjVertex** tri)
{
    int top = 0;
    int bot = 0;
    int minY = tri[0]->yBits();
    int y1 = tri[1]->yBits();
    s_tri = tri;
    int maxY = tri[0]->yBits();
    if (y1 < minY) {
        minY = y1;
        top = 1;
    } else if (y1 > maxY) {
        maxY = y1;
        bot = 1;
    }
    int y2 = tri[2]->yBits();
    if (y2 < minY) {
        top = 2;
    } else if (y2 > maxY) {
        bot = 2;
    }
    s_idxA = s_idxB = top;
    while (s_idxA != bot) {
        StepEdgeA();
        if (s_rowsA > 0) {
            while (s_idxB != bot) {
                StepEdgeB();
                if (s_rowsB > 0) {
                    WORD* row = (WORD*)(g_scanline + ((tri[top]->fy + 0xffff) >> 16) * g_pitch);
                    s_xA += 0xffff;
                    for (;;) {
                        s_xB += 0xffff;
                        for (;;) {
                            int xa = s_xA;
                            int right = xa >> 16;
                            int left = s_xB >> 16;
                            for (int x = left; x < right; x++) {
                                row[x] = g_fillColor16;
                            }
                            row = (WORD*)((BYTE*)row + g_pitch);
                            if (--s_rowsA <= 0) {
                                while (s_idxA != bot) {
                                    StepEdgeA();
                                    if (s_rowsA > 0) {
                                        s_xA += 0xffff;
                                        goto doneA;
                                    }
                                }
                                return;
                            } else {
                                s_xA = xa + s_stepA;
                            }
                        doneA:
                            if (--s_rowsB <= 0) {
                                for (;;) {
                                    if (s_idxB == bot) {
                                        return;
                                    }
                                    StepEdgeB();
                                    if (s_rowsB > 0) {
                                        break;
                                    }
                                }
                                break;
                            }
                            s_xB += s_stepB;
                        }
                    }
                }
            }
            return;
        }
    }
}

// original: FUN_004728b0
// FUNCTION: RALLYTROPHY 0x4728b0
// Advance edge A to the next vertex (index + 1, wrapping) and set its row count, x step and start x.
void StepEdgeA()
{
    ProjVertex* a = s_tri[s_idxA];
    s_idxA++;
    if (s_idxA > 2) {
        s_idxA = 0;
    }
    ProjVertex* b = s_tri[s_idxA];
    int ay = a->fy;
    int by = b->fy;
    s_rowsA = ((by + 0xffff) >> 16) - ((ay + 0xffff) >> 16);
    if (s_rowsA > 0) {
        s_stepA = (int)((float)(b->fx - a->fx) / ((float)(by - ay) * kInvFixed));
        unsigned frac = ((ay + 0xffff) & 0xffff0000) - ay;
        float f = (float)frac * kInvFixed;
        s_xA = a->fx + FloatToInt(f * s_stepA);
    }
}

// original: FUN_00472970
// FUNCTION: RALLYTROPHY 0x472970
// The same for edge B, walking backwards.
void StepEdgeB()
{
    ProjVertex* a = s_tri[s_idxB];
    s_idxB--;
    if (s_idxB < 0) {
        s_idxB = 2;
    }
    ProjVertex* b = s_tri[s_idxB];
    int ay = a->fy;
    int by = b->fy;
    s_rowsB = ((by + 0xffff) >> 16) - ((ay + 0xffff) >> 16);
    if (s_rowsB > 0) {
        s_stepB = (int)((float)(b->fx - a->fx) / ((float)(by - ay) * kInvFixed));
        unsigned frac = ((ay + 0xffff) & 0xffff0000) - ay;
        float f = (float)frac * kInvFixed;
        s_xB = a->fx + FloatToInt(f * s_stepB);
    }
}

// original: FUN_00472a30
// NONMATCHING: RALLYTROPHY 0x472a30
// 389 vs 382 bytes, same register-allocation difference as 0x472710.
// A volatile s_rowsA decrement regressed to 391/382 bytes, 176 differences; a `fixedOne` register local changed no bytes.
// The same with the 32-bit colour.
void FillTriangleFlat(ProjVertex** tri)
{
    int top = 0;
    int bot = 0;
    int minY = tri[0]->yBits();
    int y1 = tri[1]->yBits();
    s_tri = tri;
    int maxY = tri[0]->yBits();
    if (y1 < minY) {
        minY = y1;
        top = 1;
    } else if (y1 > maxY) {
        maxY = y1;
        bot = 1;
    }
    int y2 = tri[2]->yBits();
    if (y2 < minY) {
        top = 2;
    } else if (y2 > maxY) {
        bot = 2;
    }
    s_idxA = s_idxB = top;
    while (s_idxA != bot) {
        StepEdgeA();
        if (s_rowsA > 0) {
            while (s_idxB != bot) {
                StepEdgeB();
                if (s_rowsB > 0) {
                    int* row = (int*)(g_scanline + ((tri[top]->fy + 0xffff) >> 16) * g_pitch);
                    s_xA += 0xffff;
                    for (;;) {
                        s_xB += 0xffff;
                        for (;;) {
                            int xa = s_xA;
                            int right = xa >> 16;
                            int left = s_xB >> 16;
                            for (int x = left; x < right; x++) {
                                row[x] = g_fillColor32;
                            }
                            row = (int*)((BYTE*)row + g_pitch);
                            if (--s_rowsA <= 0) {
                                while (s_idxA != bot) {
                                    StepEdgeA();
                                    if (s_rowsA > 0) {
                                        s_xA += 0xffff;
                                        goto doneA;
                                    }
                                }
                                return;
                            } else {
                                s_xA = xa + s_stepA;
                            }
                        doneA:
                            if (--s_rowsB <= 0) {
                                for (;;) {
                                    if (s_idxB == bot) {
                                        return;
                                    }
                                    StepEdgeB();
                                    if (s_rowsB > 0) {
                                        break;
                                    }
                                }
                                break;
                            }
                            s_xB += s_stepB;
                        }
                    }
                }
            }
            return;
        }
    }
}

// original: FUN_00472bb0
// FUNCTION: RALLYTROPHY 0x472bb0
// Draws every mesh of a caster with the shadow material, in its world matrix.
void ParallelShadow::DrawCaster(ShadowCaster* caster)
{
    SHADOW_DEVICE->SetWorldMatrix(caster->world.m, 1);
    ShadowMeshNode* node = caster->meshList.At(0);
    for (unsigned i = 0; i < node->meshes.size(); i++) {
        ShadowMesh* m = node->meshes[i];
        int saved = m->unk24;
        m->unk24 = 0;
        SHADOW_DEVICE->DrawMesh(m);
        m->unk24 = saved;
    }
}

// original: FUN_00472c20
// FUNCTION: RALLYTROPHY 0x472c20
void ParallelShadow::SetLightMatrix(const float* m)
{
    memcpy(lightMatrix.m, m, 0x40);
}

// original: FUN_00472c40
// FUNCTION: RALLYTROPHY 0x472c40
void ParallelShadow::SetCaster(ShadowCaster* caster, int index)
{
    casters[index] = caster;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x472c60
// SYMBOL: _$E78
typedef void Generated_472c60();

// FUNCTION: RALLYTROPHY 0x472c80
// SYMBOL: _$E77
typedef void Generated_472c80();
