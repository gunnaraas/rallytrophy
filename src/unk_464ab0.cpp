// Rear-view mirror (0x464ab0-0x4653ef, stub 0x4653c0; SkidFrustum ctor/dtor 0x4653f0/0x465460).
// Map: analysis/range-4649f0.md section 1.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_4741a0.h"
#include "unk_464ab0.h"

// The mirror rectangle in 1024x768 units (x, y, width, height), file-scope statics with an empty
// destructor. original: 0x464ab0..0x464b40
static Vec2 s_mirrorPos(352.0f, 8.0f);
static Vec2 s_mirrorSize(320.0f, 80.0f);
static DrawContext s_context;       // 0x509250
static MirrorView s_view;           // 0x509080
static SkidFrustum s_frustum;       // 0x508f68

void FUN_0048e660_plain();          // the folded empty function 0x48e660

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

// Passes a float through an inline function's parameter (the copy then goes through the FPU).
inline float PassFloat(float v)
{
    return v;
}

// Programs the projection (field of view, aspect 1, near, far) through the renderer's float slot.
inline void SetProjection(RaceDeviceSlots* device, float fov, float nearPlane, float farPlane)
{
    device->SetClear(fov, 1.0f, nearPlane, farPlane, 1);
}

// Transforms *point by *matrix (row vectors, translation at +0x30) into *result.
#define MIRROR_TRANSFORM() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, offset s_view + 0x14 \
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
    __asm fadd dword ptr [edx+48] \
    __asm fxch st(1) \
    __asm fadd dword ptr [edx+52] \
    __asm fxch st(2) \
    __asm fadd dword ptr [edx+56] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// NONMATCHING: RALLYTROPHY 0x464c10
// Same size (645), 13 differing bytes: the original computes the matrix pointer (body + 0x60) in place
// (`add eax,0x60` and `mov [ebp-0x10],eax`) where this build uses `lea ecx,[eax+0x60]`; the rest is identical.
// Pointer arithmetic from the body start kept the same code; mutating body inside the expression emitted `add eax`
// but evaluated the matrix term first (19 bytes); splitting the eye-height product before advancing body produced
// 647 bytes / 266 differences, and a volatile pointer expanded the diff to 286. Plain observations.
void Car::FUN_00464c10(ViewManager* view)
{
    MirrorCar* car = (MirrorCar*)this;
    s_view.fov = 1.5707964f;
    s_view.nearPlane = 1.0f;
    s_view.farPlane = 100.0f;
    MirrorBody* body = car->body;
    Vec3 eye = body->unkb8;
    eye.z = body->unkb4;
    const float* matrix = body->m0;
    eye.y = body->unkb0 * 0.85f + matrix[17] * 0.15f;
    const float* point = (const float*)&eye;
    MIRROR_TRANSFORM();
    MirrorBody* b = car->body;
    s_view.unk20[0] = b->row0[0];
    s_view.unk20[1] = b->row0[1];
    s_view.unk20[2] = b->row0[2];
    s_view.unk30[0] = b->row1[0];
    s_view.unk30[1] = b->row1[1];
    s_view.unk30[2] = b->row1[2];
    *(Vec3*)s_view.unk40 = -b->row2;
    s_view.m0[0] = b->m0[0];
    s_view.m0[1] = b->m0[1];
    s_view.m0[2] = b->m0[2];
    s_view.m1[0] = b->m1[0];
    s_view.m1[1] = b->m1[1];
    s_view.m1[2] = b->m1[2];
    *(Vec3*)s_view.m2 = -b->m2;
    s_view.m3[0] = s_view.pos[0];
    s_view.m3[1] = s_view.pos[1];
    s_view.m3[2] = s_view.pos[2];
    RaceDeviceSlots* device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->SetMatrix(s_view.m0, 1);
    SetProjection((RaceDeviceSlots*)DAT_0056b0ac, s_view.fov, s_view.nearPlane, s_view.farPlane);
    ((SkidFrustum*)&s_frustum)->Init(DAT_0056b0ac);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->SetMatrix(view->m0, 1);
    SetProjection((RaceDeviceSlots*)DAT_0056b0ac, view->fov, view->nearPlane, view->farPlane);
}

// FUNCTION: RALLYTROPHY 0x464ea0
int MirrorCar::CanSee(MirrorObject* object)
{
    int visible = 0;
    if (DAT_00501308 && DAT_0050146c && !(object->flags & 3)) {
        if (((MirrorFrustum*)&s_frustum)->TestSphere(object->sphere, object->matrix)) {
            visible = 1;
        }
    }
    return visible;
}

// FUNCTION: RALLYTROPHY 0x464ef0
// Reusing the member expression for eye height before declaring the matrix pointer reproduces the original's
// in-place `add eax,0x60` schedule. Exact 1230-byte match, 108 relocations.
void Car::FUN_00464ef0(float dt)
{
    MirrorCar* car = (MirrorCar*)this;
    if (!DAT_00501308) {
        return;
    }
    s_view.fov = 1.5707964f;
    s_view.nearPlane = 1.0f;
    s_view.farPlane = 100.0f;
    Vec3 eye = car->body->unkb8;
    eye.z = car->body->unkb4;
    eye.y = car->body->unkb0 * 0.85f + car->body->m0[17] * 0.15f;
    const float* matrix = car->body->m0;
    const float* point = (const float*)&eye;
    float* result = s_view.pos;
    MIRROR_TRANSFORM();
    MirrorBody* b = car->body;
    s_view.unk20[0] = b->row0[0];
    s_view.unk20[1] = b->row0[1];
    s_view.unk20[2] = b->row0[2];
    s_view.unk30[0] = b->row1[0];
    s_view.unk30[1] = b->row1[1];
    s_view.unk30[2] = b->row1[2];
    *(Vec3*)s_view.unk40 = -b->row2;
    s_view.m0[0] = b->m0[0];
    s_view.m0[1] = b->m0[1];
    s_view.m0[2] = b->m0[2];
    s_view.m1[0] = b->m1[0];
    s_view.m1[1] = b->m1[1];
    s_view.m1[2] = b->m1[2];
    *(Vec3*)s_view.m2 = -b->m2;
    s_view.m3[0] = s_view.pos[0];
    s_view.m3[1] = s_view.pos[1];
    s_view.m3[2] = s_view.pos[2];
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetRenderState(0xe, 1);
    s_context.ClearViews();
    FUN_0048e660_plain();
    MirrorStage* stage = (MirrorStage*)car->stage;
    car->stage->FUN_00411170((ViewManager*)&s_view, &s_context);
    MirrorContext* ctx = (MirrorContext*)&s_context;
    ctx->unk188 = stage->unk1c4;
    ctx->unk188 = stage->unk1c4;
    ctx->unk180 = PassFloat(stage->unk1b4);
    ctx->unk18c = stage->unk1c8;
    ctx->unk190 = stage->unk1cc;
    ctx->unk174 = stage->unk1a8;
    ctx->unk178 = stage->unk1ac;
    ctx->unk17c = stage->unk1b0;
    ctx->unk184 = DAT_00501434 ? stage->unk1a4 : 0;
    DAT_00501450->DrawSetupExcept((int)&s_context, (ViewPoint*)&s_view, (Car*)this);
    ctx->FUN_004b8310(0);
    int vx, vy, vw, vh;
    int x = FloatToInt(DAT_0056aea8 * s_mirrorPos.x);
    int y = FloatToInt(DAT_0056aeac * s_mirrorPos.y);
    int w = FloatToInt(s_mirrorSize.x * DAT_0056aea8);
    int h = FloatToInt(s_mirrorSize.y * DAT_0056aeac);
    ((MirrorDevice*)DAT_0056b0ac)->GetViewport(&vx, &vy, &vw, &vh);
    ((MirrorDevice*)DAT_0056b0ac)->SetViewport(x, y, w, h);
    RaceDeviceSlots* device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->Slot24(2);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->SetRenderState(1, 1);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->SetMatrix(s_view.m0, 1);
    SetProjection((RaceDeviceSlots*)DAT_0056b0ac, s_view.fov, s_view.nearPlane, s_view.farPlane);
    s_context.FUN_004b7c80((RaceDeviceSlots*)DAT_0056b0ac, (ViewManager*)&s_view);
    DAT_00501450->DrawShadowsExcept((int)&s_context, (int)&s_view, (Car*)this);
    car->stage->FUN_00411350((RaceDeviceSlots*)DAT_0056b0ac, (ViewManager*)&s_view);
    s_context.FUN_004b8160((RaceDeviceSlots*)DAT_0056b0ac, (ViewManager*)&s_view);
    DAT_00501450->DrawLightsExcept((int)&s_context, (int)&s_view, (Car*)this);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->SetRenderState(1, 1);
    DAT_00501450->FUN_00488880((ViewPoint*)&s_view);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->Slot32(3);
    car->stage->FUN_00411460((RaceDeviceSlots*)DAT_0056b0ac, (ViewManager*)&s_view, &s_context, 0.0f);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->Slot32(3);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->SetRenderState(0xe, 0);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->SetRenderState(1, 2);
    ((MirrorDevice*)DAT_0056b0ac)->SetViewport(vx, vy, vw, vh);
    device = (RaceDeviceSlots*)DAT_0056b0ac;
    device->SetRenderState(0xd, 0);
}

// Compiler-generated pieces of the file-scope statics (four per static: init-table wrapper, constructor,
// atexit registration, destructor thunk) and of the SkidFrustum member functions. Symbols: objdump -t.

// FUNCTION: RALLYTROPHY 0x464ab0
// SYMBOL: _$E21
typedef void Generated_464ab0();

// FUNCTION: RALLYTROPHY 0x464ac0
// SYMBOL: _$E18
typedef void Generated_464ac0();

// FUNCTION: RALLYTROPHY 0x464ae0
// SYMBOL: _$E20
typedef void Generated_464ae0();

// FUNCTION: RALLYTROPHY 0x464af0
// SYMBOL: _$E19
typedef void Generated_464af0();

// FUNCTION: RALLYTROPHY 0x464b00
// SYMBOL: _$E26
typedef void Generated_464b00();

// FUNCTION: RALLYTROPHY 0x464b10
// SYMBOL: _$E23
typedef void Generated_464b10();

// FUNCTION: RALLYTROPHY 0x464b30
// SYMBOL: _$E25
typedef void Generated_464b30();

// FUNCTION: RALLYTROPHY 0x464b40
// SYMBOL: _$E24
typedef void Generated_464b40();

// FUNCTION: RALLYTROPHY 0x464b50
// SYMBOL: _$E31
typedef void Generated_464b50();

// FUNCTION: RALLYTROPHY 0x464b60
// SYMBOL: _$E28
typedef void Generated_464b60();

// FUNCTION: RALLYTROPHY 0x464b70
// SYMBOL: _$E30
typedef void Generated_464b70();

// FUNCTION: RALLYTROPHY 0x464b80
// SYMBOL: _$E29
typedef void Generated_464b80();

// FUNCTION: RALLYTROPHY 0x464b90
// SYMBOL: _$E36
typedef void Generated_464b90();

// FUNCTION: RALLYTROPHY 0x464ba0
// SYMBOL: _$E33
typedef void Generated_464ba0();

// FUNCTION: RALLYTROPHY 0x464bb0
// SYMBOL: _$E35
typedef void Generated_464bb0();

// FUNCTION: RALLYTROPHY 0x464bc0
// SYMBOL: _$E34
typedef void Generated_464bc0();

// FUNCTION: RALLYTROPHY 0x464bd0
// SYMBOL: _$E41
typedef void Generated_464bd0();

// FUNCTION: RALLYTROPHY 0x464be0
// SYMBOL: _$E38
typedef void Generated_464be0();

// FUNCTION: RALLYTROPHY 0x464bf0
// SYMBOL: _$E40
typedef void Generated_464bf0();

// FUNCTION: RALLYTROPHY 0x464c00
// SYMBOL: _$E39
typedef void Generated_464c00();

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x4653c0
// SYMBOL: _$E48
typedef void Generated_4653c0();

// FUNCTION: RALLYTROPHY 0x4653e0
// SYMBOL: _$E47
typedef void Generated_4653e0();

// The implicit SkidFrustum constructor and destructor (two vector iterators over the 6-plane arrays); the
// second SkidFrustum static of the 0x4b7xxx file uses the same bodies.

// FUNCTION: RALLYTROPHY 0x4653f0
// SYMBOL: ??0SkidFrustum@@QAE@XZ
typedef void Generated_4653f0();

// FUNCTION: RALLYTROPHY 0x465460
// SYMBOL: ??1SkidFrustum@@QAE@XZ
typedef void Generated_465460();
