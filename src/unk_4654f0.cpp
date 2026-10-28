// Car replay and network state glue (0x4654f0-0x465e5f, stub 0x465e60). Map: analysis/range-4649f0.md section 2.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_4654f0.h"

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

// original: FUN_004654f0
// FUNCTION: RALLYTROPHY 0x4654f0
void Car::FUN_004654f0(int mode)
{
    CarReplayView* v = (CarReplayView*)this;
    if (v->mode)
        v->recorder.Stop();
    v->mode = mode;
    if (mode)
        v->recorder.Start(mode);
}

// FUNCTION: RALLYTROPHY 0x465530
void Car::FUN_00465530(const char* name)
{
    ((CarReplayView*)this)->recorder.Load(name);
}

// FUNCTION: RALLYTROPHY 0x465550
void Car::FUN_00465550(const char* name)
{
    ((CarReplayView*)this)->recorder.Save(name);
}

// Scales a float into a byte range (see ClampByte).
inline int ClampByte(float f)
{
    int i = FloatToInt(f * 255.0f);
    if (i < 0)
        i = 0;
    else if (i > 255)
        i = 255;
    return i;
}

inline int ClampShort(float f)
{
    int i = FloatToInt(f * 255.0f);
    if (i > 0x7fff)
        i = 0x7fff;
    else if (i < -0x7fff)
        i = -0x7fff;
    return i;
}

#define V ((CarStateView*)this)

// FUNCTION: RALLYTROPHY 0x465570
void Car::GetState(CarState* packet)
{
    CarStateData* state = (CarStateData*)packet;
        int i;
    state->throttle = ClampByte(V->throttle);
    state->brake = ClampByte(V->brake);
    state->clutch = ClampByte(V->clutch);
    state->handbrake = ClampByte(V->handbrake);
    {
        int s = FloatToInt(V->steer * 127.0f) + 128;
        if (s < 0)
            s = 0;
        else if (s > 255)
            s = 255;
        state->steer = s;
    }
    float ring[8];
    int head = V->steerHead;
    for (i = 8; i; i--)
        ring[8 - i] = V->steerRing[head++ & 7];
    float newest = ring[4] + ring[5] + ring[6] + ring[7];
    float oldest = ring[0] + ring[1] + ring[2] + ring[3];
    state->steerRate = FloatToInt((newest * 0.25f - oldest * 0.25f) * 16383.0f);
    int g = V->gear;
    state->gear = g;
    state->rpm = FloatToInt(V->rpm);
    StateBody* b = V->body;
    state->body[0] = b->unk14c;
    state->body[1] = b->unk150;
    state->body[2] = b->unk154;
    state->body[3] = b->unk158;
    state->body[4] = *(int*)&b->position[0];
    state->body[5] = *(int*)&b->position[1];
    state->body[6] = *(int*)&b->position[2];
    state->body[7] = b->vel[0];
    state->body[8] = b->vel[1];
    state->body[9] = b->vel[2];
    state->body[10] = b->vel[3];
    state->body[11] = b->vel[4];
    state->body[12] = b->vel[5];
    for (i = 0; i < 4; i++) {
        state->compression[i] = ClampByte((V->wheel[i].topY - V->wheel[i].pos[1]) / V->spring[i].length);
        state->spin[i] = ClampShort(V->wheel[i].spin);
        state->load[i] = ClampShort(V->spring[i].load);
    }
    if (V->damageSet)
        state->damage = ClampByte(V->damage);
    else
        state->damage = 0;
}

// Transforms *point by the rotation *matrix (rows 0x10 apart) and adds *trans into *result.
#define TRANSFORM_ADD() \
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

inline float ByteToUnitF(int b)
{
    return (float)b * 0.003921569f;
}

// original: FUN_004658a0 (declared with PlayerSample*, the same packet seen from the player side)
// NONMATCHING: RALLYTROPHY 0x4658a0
// Semantics complete; 917 vs 926 bytes. Multiplying the spring length by the parenthesized normalized compression
// byte matches the target's x87 multiply order. Open: scheduling of body copies (the original loads Car+0x24 late)
// and the hit-array induction pointer (original: Car+0x1e0c + n*12).
void Car::FUN_004658a0(PlayerSample* packet)
{
    CarStateData* state = (CarStateData*)packet;
    V->throttle = ByteToUnitF(state->throttle);
    FUN_0045da60(ByteToUnitF(state->brake));
    V->clutch = ByteToUnitF(state->clutch);
    V->handbrake = ByteToUnitF(state->handbrake);
    V->steerSet = 1;
    V->steer = (float)(state->steer - 128) * 0.007874016f;
    V->gear = (signed char)state->gear;
    V->rpm = (float)state->rpm;
    StateBody* b = V->body;
    b->unk14c = state->body[0];
    b->unk150 = state->body[1];
    b->unk154 = state->body[2];
    b->unk158 = state->body[3];
    *(int*)&b->position[0] = state->body[4];
    *(int*)&b->position[1] = state->body[5];
    *(int*)&b->position[2] = state->body[6];
    b->vel[0] = state->body[7];
    b->vel[1] = state->body[8];
    b->vel[2] = state->body[9];
    b->vel[3] = state->body[10];
    b->vel[4] = state->body[11];
    b->vel[5] = state->body[12];
    b->FUN_0041c390();
    int i;
    for (i = 0; i < 4; i++) {
        float d = V->spring[i].length;
        V->wheel[i].pos[1] = V->wheel[i].topY - d * ((float)state->compression[i] * 0.003921569f);
        V->wheel[i].spin = (float)state->spin[i] * 0.003921569f;
        V->spring[i].load = (float)state->load[i] * 0.003921569f;
        V->spring[i].travel = V->wheel[i].topY - V->wheel[i].pos[1];
    }
    FUN_004581f0();
    float t = V->rideA * 0.45f;
    for (i = 0; i < 4; i++) {
        StateWheel* w = &V->wheel[i];
        Vec3 local;
        *(int*)&local.x = *(int*)&w->pos[0];
        *(int*)&local.z = *(int*)&w->pos[2];
        float wheelY = *(volatile float*)&w->pos[1];
        local.y = wheelY - (V->rideB - t);
        Vec3 world;
        {
            const float* trans = V->body->position;
            const float* matrix = V->body->matrix;
            const float* point = (const float*)&local;
            float* result = (float*)&world;
            TRANSFORM_ADD();
        }
        if (V->query.FUN_00419770(t + 0.01f, (float*)&world, V->hitNormal[i], V->hitDepth[i], V->hitPoint[i], &V->surface[i])) {
            int idx = V->surface[i];
            int tyre = V->stage->tyre[idx];
            w->normal[0] = V->hitNormal[i][0];
            w->normal[1] = V->hitNormal[i][1];
            w->normal[2] = V->hitNormal[i][2];
            w->surface = V->tyre[tyre];
            w->friction = V->stage->friction[idx];
            w->params[0] = V->stage->normal[idx][0];
            w->params[1] = V->stage->normal[idx][1];
        } else {
            w->active = 0;
        }
    }
    V->damageOut = (float)state->damage * 0.003921569f;
}

static unsigned char ClampScale(float v, float scale, int lo, int hi);
static float ByteToUnit(int b);

// FUNCTION: RALLYTROPHY 0x465c40
void Car::GetState2(CarState2* packet)
{
    CarWearData* state = (CarWearData*)packet;
    CarStateView* v = (CarStateView*)this;
    for (int i = 0; i < 4; i++) {
        state->a[i] = ClampScale(v->wheel[i].GetWearA(), 255.0f, 0, 255);
        state->b[i] = ClampScale(v->wear[i], 255.0f, 0, 255);
        state->c[i] = ClampScale(v->wheel[i].GetWearC(), 255.0f, 0, 255);
    }
    state->damage[0] = ClampScale(v->wearEngine[1], 255.0f, 0, 255);
    state->damage[1] = ClampScale(v->wearEngine[0], 255.0f, 0, 255);
    state->damage[2] = ClampScale(v->wearEngine[2], 255.0f, 0, 255);
    state->damage[3] = ClampScale(v->wearEngine[3], 255.0f, 0, 255);
}

// FUNCTION: RALLYTROPHY 0x465d50
static unsigned char ClampScale(float v, float scale, int lo, int hi)
{
    v = v * scale;
    int i = FloatToInt(v);
    if (i < lo)
        return lo;
    if (i > hi)
        return hi;
    return i;
}

// original: FUN_00465d80 (declared with CarSetup*, really the wear packet)
// FUNCTION: RALLYTROPHY 0x465d80
void Car::FUN_00465d80(CarSetup* setup)
{
    CarWearData* state = (CarWearData*)setup;
    CarStateView* v = (CarStateView*)this;
    for (int i = 0; i < 4; i++) {
        v->wheel[i].SetWearA(ByteToUnit(state->a[i]));
        v->wear[i] = ByteToUnit(state->b[i]);
        v->wheel[i].SetWearC(ByteToUnit(state->c[i]));
    }
    v->wearEngine[1] = ByteToUnit(state->damage[0]);
    v->wearEngine[0] = ByteToUnit(state->damage[1]);
    v->wearEngine[2] = ByteToUnit(state->damage[2]);
    v->wearEngine[3] = ByteToUnit(state->damage[3]);
}

// FUNCTION: RALLYTROPHY 0x465e40
static float ByteToUnit(int b)
{
    return (float)(b & 0xff) * 0.003921569f;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x465e60
// SYMBOL: _$E23
typedef void Generated_465e60();

// FUNCTION: RALLYTROPHY 0x465e80
// SYMBOL: _$E22
typedef void Generated_465e80();
