// Car environment-map draw (0x459230-0x45988f, stub 0x459860). Map: analysis/range-4563e0.md section 4.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_464ab0.h"
#include "unk_459230.h"

void FUN_0048e660_plain();          // the folded empty function 0x48e660

// The draw context of the six-face render, a file-scope static with an empty destructor.
static DrawContext s_context;       // 0x508dc8

// Transforms *in by the 3x3 rows at *mat and adds *trans into *out (inline matrix multiply).
#define ENV_TRANSFORM() \
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

// Compiler-generated pieces of the DrawContext static (init-table wrapper, constructor thunk, atexit
// registration, destructor thunk). Symbols: objdump -t.

// FUNCTION: RALLYTROPHY 0x459230
// SYMBOL: _$E21
typedef void Generated_459230();

// FUNCTION: RALLYTROPHY 0x459240
// SYMBOL: _$E18
typedef void Generated_459240();

// FUNCTION: RALLYTROPHY 0x459250
// SYMBOL: _$E20
typedef void Generated_459250();

// FUNCTION: RALLYTROPHY 0x459260
// SYMBOL: _$E19
typedef void Generated_459260();

// NONMATCHING: RALLYTROPHY 0x459270
// Same size (1516 with the jump table), 206 differing bytes, all instruction scheduling: the original loads
// the p.x term from the body base before forming the matrix pointer (here it folds into mat+0x8c), spills the
// camera frame pointer before the three projection stores, and emits the identity constructor stores of the
// face matrix after the cross-product argument pushes (here before them).
void Car::Draw459270(ViewManager* view, DrawContext* context)
{
    EnvCar* car = (EnvCar*)this;
    if (!car->target) {
        return;
    }
    EnvDevice* device = (EnvDevice*)DAT_0056b0ac;
    int vx, vy, vw, vh;
    device->GetViewport(&vx, &vy, &vw, &vh);
    ((EnvDevice*)DAT_0056b0ac)->BeginFrame();
    MirrorView lv;
    EnvCamera* camera = (EnvCamera*)view;
    lv.fov = 1.5707964f;
    lv.nearPlane = 0.5f;
    lv.farPlane = 50.0f;
    EnvBlock* frameSrc = &camera->frame;
    *(EnvBlock*)lv.unk20 = *frameSrc;
    *(EnvBlock*)lv.m0 = camera->view;
    EnvBody* body = car->body;
    Vec3 p = body->center;
    p.x += body->unkac;
    const float* trans = body->pos;
    const float* mat = body->rows;
    p.y += mat[0x24];
    p.z += mat[0x25];
    const float* in = (const float*)&p;
    float* out = lv.pos;
    p.x *= 0.5f;
    p.y *= 0.5f;
    p.z *= 0.5f;
    ENV_TRANSFORM();
    s_context.ClearViews();
    FUN_0048e660_plain();
    Stage* stage = car->stage;
    stage->FUN_00411170((ViewManager*)&lv, &s_context);
    MirrorContext* ctx = (MirrorContext*)&s_context;
    MirrorStage* ms = (MirrorStage*)stage;
    ctx->unk188 = ms->unk1c4;
    ctx->unk18c = ms->unk1c8;
    ctx->unk190 = ms->unk1cc;
    ctx->FUN_004b8310(0);
    int oldTarget = ((EnvDevice*)DAT_0056b0ac)->GetRenderState(0xb);
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetRenderState(0xb, 1);
    int count = 1;
    unsigned start = 0;
    int frame = car->frame;
    if (frame > 0) {
        count = 2;
        if (frame & 1) {
            start = 1;
        }
    }
    car->frame = frame + 1;
    EnvTarget* target = car->target;
    if (target->cursor == 0) {
        target->cursor = 1;
        start = 0;
        count = 1;
    }
    for (unsigned face = start; face < 6; face += count) {
        EnvVec fwd, up;
        ((EnvDevice*)DAT_0056b0ac)->BeginFace(target, face, 1, 1);
        ((RaceDeviceSlots*)DAT_0056b0ac)->Slot24(2);
        switch (face) {
        case 0:
            fwd.x = 1.0f;
            fwd.y = 0.0f;
            fwd.z = 0.0f;
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            break;
        case 1:
            fwd.x = -1.0f;
            fwd.y = 0.0f;
            fwd.z = 0.0f;
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            break;
        case 2:
            fwd.x = 0.0f;
            fwd.y = 1.0f;
            fwd.z = 0.0f;
            up.x = 0.0f;
            up.y = 0.0f;
            up.z = -1.0f;
            break;
        case 3:
            fwd.x = 0.0f;
            fwd.y = -1.0f;
            fwd.z = 0.0f;
            up.x = 0.0f;
            up.y = 0.0f;
            up.z = 1.0f;
            break;
        case 4:
            fwd.x = 0.0f;
            fwd.y = 0.0f;
            fwd.z = 1.0f;
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            break;
        case 5:
            fwd.x = 0.0f;
            fwd.y = 0.0f;
            fwd.z = -1.0f;
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            break;
        }
        EnvMat m;
        EnvBuf b;
        Vec3 pad;
        Vec3 tmp;
        Vec3* right = FUN_004203e0(&tmp, up, fwd);
        m.m[0] = right->x;
        m.m[1] = right->y;
        m.m[2] = right->z;
        m.m[4] = up.x;
        m.m[5] = up.y;
        m.m[6] = up.z;
        m.m[8] = fwd.x;
        m.m[9] = fwd.y;
        m.m[10] = fwd.z;
        *(EnvBlock*)b.m = *frameSrc;
        b.m[12] = lv.pos[0];
        b.m[13] = lv.pos[1];
        b.m[14] = lv.pos[2];
        *(EnvBlock*)lv.unk20 = *(EnvBlock*)m.m;
        ((LightViewEnv*)lv.unk20)->Concat(*(EnvBlock*)b.m);
        *(EnvBlock*)lv.m0 = *(EnvBlock*)lv.unk20;
        ((RaceDeviceSlots*)DAT_0056b0ac)->SetMatrix(lv.m0, 1);
        ((RaceDeviceSlots*)DAT_0056b0ac)->SetClear(lv.fov, 1.0f, lv.nearPlane, lv.farPlane, 1);
        ((RaceDeviceSlots*)DAT_0056b0ac)->SetRenderState(0xd, 0);
        s_context.FUN_004b7c80((RaceDeviceSlots*)DAT_0056b0ac, (ViewManager*)&lv);
        if (((EnvDevice*)DAT_0056b0ac)->BeginScene()) {
            stage->FUN_00411350((RaceDeviceSlots*)DAT_0056b0ac, (ViewManager*)&lv);
            s_context.FUN_004b8160((RaceDeviceSlots*)DAT_0056b0ac, (ViewManager*)&lv);
            ((EnvDevice*)DAT_0056b0ac)->EndScene();
        }
    }
    ((EnvDevice*)DAT_0056b0ac)->EndFrame();
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetRenderState(0xb, oldTarget);
    ((MirrorDevice*)DAT_0056b0ac)->SetViewport(vx, vy, vw, vh);
    ((EnvContext*)context)->FUN_004b8340();
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x459860
// SYMBOL: _$E28
typedef void Generated_459860();

// FUNCTION: RALLYTROPHY 0x459880
// SYMBOL: _$E27
typedef void Generated_459880();
