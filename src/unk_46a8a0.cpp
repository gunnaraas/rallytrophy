// Drivetrain (0x46a8a0-0x46bb5f): engine, clutch, gearbox and differential of a Car.
// Map: analysis/range-46a8a0.md section 2. File A of four (0x46bb90 CarSnapshot, 0x46c2c0 Wheel and
// 0x46d720 SpringFollower follow in their own files).
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46a8a0.h"
#include <math.h>
#include <string.h>

// Keys of the car engine ini (0x4fdf30, 58 entries).
ConfigKey DrivetrainKeys[58] = {
    { "InertiaEngine", 2 },
    { "InertiaDriveShaft", 2 },
    { "EngineFriction", 2 },
    { "DifferentialBias", 2 },
    { "DifferentialLock", 2 },
    { "ClutchReleaseTime", 2 },
    { "GearShiftTime", 2 },
    { "ClutchEngageTime", 2 },
    { "NumGears", 1 },
    { "GearRRatio", 4 },
    { "GearRInertia", 2 },
    { "Gear1Ratio", 4 },
    { "Gear1Inertia", 2 },
    { "Gear2Ratio", 4 },
    { "Gear2Inertia", 2 },
    { "Gear3Ratio", 4 },
    { "Gear3Inertia", 2 },
    { "Gear4Ratio", 4 },
    { "Gear4Inertia", 2 },
    { "Gear5Ratio", 4 },
    { "Gear5Inertia", 2 },
    { "Gear6Ratio", 4 },
    { "Gear6Inertia", 2 },
    { "EndRatio", 2 },
    { "Gear1ShiftUp", 2 },
    { "Gear2ShiftUp", 2 },
    { "Gear3ShiftUp", 2 },
    { "Gear4ShiftUp", 2 },
    { "Gear5ShiftUp", 2 },
    { "Gear2ShiftDown", 2 },
    { "Gear3ShiftDown", 2 },
    { "Gear4ShiftDown", 2 },
    { "Gear5ShiftDown", 2 },
    { "Gear6ShiftDown", 2 },
    { "IdleRpm", 2 },
    { "TorqueMax", 2 },
    { "TorqueCurveParams", 1 },
    { "Torque00", 3 },
    { "Torque01", 3 },
    { "Torque02", 3 },
    { "Torque03", 3 },
    { "Torque04", 3 },
    { "Torque05", 3 },
    { "Torque06", 3 },
    { "Torque07", 3 },
    { "Torque08", 3 },
    { "Torque09", 3 },
    { "Torque10", 3 },
    { "Torque11", 3 },
    { "Torque12", 3 },
    { "Torque13", 3 },
    { "Torque14", 3 },
    { "Torque15", 3 },
    { "Torque16", 3 },
    { "Torque17", 3 },
    { "Torque18", 3 },
    { "Torque19", 3 },
    { "ExhaustPeakRpm", 2 },
};

// The writable key buffer of the Torque00..Torque19 loop (0x4fe100).
static char torqueKey[] = "Torque00";

// FUNCTION: RALLYTROPHY 0x46a8a0
Drivetrain::Drivetrain()
{
    car = 0;
    Reset();
}

// FUNCTION: RALLYTROPHY 0x46a900
Drivetrain::~Drivetrain()
{
}

inline void SetRatio(Vec3* dst, const ConfigVector3& v)
{
    dst->x = v.values[0];
    dst->y = v.values[1];
    dst->z = v.values[2];
}

// FUNCTION: RALLYTROPHY 0x46a920
void Drivetrain::LoadConfig(const char* path, unsigned int checksum)
{
    Config config;
    config.Define(DrivetrainKeys, 58);
    config.Load(path, true, true, checksum);
    inertiaEngine = config.GetFloat("InertiaEngine");
    inertiaDriveShaft = config.GetFloat("InertiaDriveShaft");
    differentialBias = config.GetFloat("DifferentialBias");
    differentialLock = config.GetFloat("DifferentialLock");
    clutchReleaseTime = config.GetFloat("ClutchReleaseTime");
    clutchEngageTime = config.GetFloat("ClutchEngageTime");
    gearShiftTime = config.GetFloat("GearShiftTime");
    numGears = config.GetInt("NumGears");
    SetRatio(&rawRatio[0], config.GetVector3("GearRRatio"));
    SetRatio(&rawRatio[1], config.GetVector3("Gear1Ratio"));
    SetRatio(&rawRatio[2], config.GetVector3("Gear1Ratio"));
    SetRatio(&rawRatio[3], config.GetVector3("Gear2Ratio"));
    SetRatio(&rawRatio[4], config.GetVector3("Gear3Ratio"));
    SetRatio(&rawRatio[5], config.GetVector3("Gear4Ratio"));
    SetRatio(&rawRatio[6], config.GetVector3("Gear5Ratio"));
    SetRatio(&rawRatio[7], config.GetVector3("Gear6Ratio"));
    gearInertia[0] = config.GetFloat("GearRInertia");
    gearInertia[1] = config.GetFloat("Gear1Inertia");
    gearInertia[2] = config.GetFloat("Gear1Inertia");
    gearInertia[3] = config.GetFloat("Gear2Inertia");
    gearInertia[4] = config.GetFloat("Gear3Inertia");
    gearInertia[5] = config.GetFloat("Gear4Inertia");
    gearInertia[6] = config.GetFloat("Gear5Inertia");
    gearInertia[7] = config.GetFloat("Gear6Inertia");
    shiftUp[0] = 0;
    shiftUp[1] = 0;
    shiftUp[2] = config.GetFloat("Gear1ShiftUp");
    shiftUp[3] = config.GetFloat("Gear2ShiftUp");
    shiftUp[4] = config.GetFloat("Gear3ShiftUp");
    shiftUp[5] = config.GetFloat("Gear4ShiftUp");
    shiftUp[6] = config.GetFloat("Gear5ShiftUp");
    shiftUp[7] = 9999999.0f;
    shiftDown[0] = 0;
    shiftDown[1] = 0;
    shiftDown[2] = 1.0f;
    shiftDown[3] = config.GetFloat("Gear2ShiftDown");
    shiftDown[4] = config.GetFloat("Gear3ShiftDown");
    shiftDown[5] = config.GetFloat("Gear4ShiftDown");
    shiftDown[6] = config.GetFloat("Gear5ShiftDown");
    shiftDown[7] = config.GetFloat("Gear6ShiftDown");
    endRatio = config.GetFloat("EndRatio");
    gearing = -1;
    idleRpm = config.GetFloat("IdleRpm");
    float powerScale = *(float*)((BYTE*)DAT_0056b0c8 + 0x13948c);
    torqueMax = config.GetFloat("TorqueMax") * powerScale;
    curvePoints = config.GetInt("TorqueCurveParams");
    engineFriction = config.GetFloat("EngineFriction");
    TorquePoint t;
    for (int i = 0; i < 20; i++) {
        torqueKey[6] = (char)(i / 10 + '0');
        torqueKey[7] = (char)(i % 10 + '0');
        const ConfigVector2& v = config.GetVector2(torqueKey);
        t.rpm = v.values[0];
        t.torque = v.values[1];
        curve[i].rpm = t.rpm;
        curve[i].torque = t.torque;
    }
    exhaustPeakRpm = config.GetFloat("ExhaustPeakRpm");
    SetGearing(2);
}

// FUNCTION: RALLYTROPHY 0x46adc0
void Drivetrain::Reset()
{
    throttleEff = 0;
    throttle = 0;
    clutch = 0;
    engineTorque = 0;
    engineSpeed = 0;
    idleRpm = 1200.0f;
    clutchReleaseTime = 0.1f;
    clutchEngageTime = 0.1f;
    gearShiftTime = 0.1f;
    gear = 0;
    targetGear = 0;
    reactionTorque = 0;
    engineFriction = 0.015f;
    shiftState = 0;
}

// FUNCTION: RALLYTROPHY 0x46ae30
void Drivetrain::ResetState()
{
    engineSpeed = idleRpm;
    engineTorque = 0;
    throttleEff = 0;
    throttle = 0;
    clutch = 0;
    targetGear = 0;
    gear = 0;
    shiftState = 0;
}

extern "C" float fun_004a7f30();
extern float DAT_004e13c4;
extern float DAT_004e1348;

// original: FUN_0046ae70
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x46ae70
float Drivetrain::TorqueAtRpm(float)
{
    __asm {
        mov eax, dword ptr fs:[0]
        push 0xffffffff
        push 0x4dbc28
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 0xc
        push esi
        mov esi, ecx
        mov edx, dword ptr [esi + 0x1ac]
        dec edx
        push edi
        lea edi, [edx - 1]
        xor ecx, ecx
        test edi, edi
        jle torque_at_rpm_first
        push ebx
        lea ebx, [esi + 0x114]
    torque_at_rpm_scan:
        fld dword ptr [ebx]
        fcomp dword ptr [esp + 0x28]
        fnstsw ax
        test ah, 1
        je torque_at_rpm_scan_done
        inc ecx
        add ebx, 8
        cmp ecx, edi
        jl torque_at_rpm_scan
    torque_at_rpm_scan_done:
        test ecx, ecx
        pop ebx
        jne torque_at_rpm_nonfirst
    torque_at_rpm_first:
        fld dword ptr [esp + 0x24]
        fdiv dword ptr [esi + 0x114]
        fld dword ptr [esi + 0x118]
        fsub dword ptr [esi + 0x110]
        fmulp st(1), st
        fadd dword ptr [esi + 0x110]
        jmp torque_at_rpm_adjust
    torque_at_rpm_nonfirst:
        cmp ecx, edi
        fld dword ptr [esp + 0x24]
        jl torque_at_rpm_interior
        fsub dword ptr [esi + edx*8 + 0x104]
        fld dword ptr [esi + edx*8 + 0x10c]
        fsub dword ptr [esi + edx*8 + 0x104]
        fdivp st(1), st
        fld dword ptr [esi + edx*8 + 0x110]
        fsub dword ptr [esi + edx*8 + 0x108]
        fmulp st(1), st
        fadd dword ptr [esi + edx*8 + 0x108]
        jmp torque_at_rpm_adjust
    torque_at_rpm_interior:
        fsub dword ptr [esi + ecx*8 + 0x10c]
        fld dword ptr [esi + ecx*8 + 0x114]
        fsub dword ptr [esi + ecx*8 + 0x10c]
        fdivp st(1), st
        fst dword ptr [esp + 8]
        fld st(0)
        fmul st, st(1)
        fst dword ptr [esp + 0xc]
        fmul st, st(1)
        fstp dword ptr [esp + 0x10]
        fstp st(0)
        mov eax, dword ptr [esi + ecx*8 + 0x120]
        mov edx, dword ptr [esi + ecx*8 + 0x118]
        push eax
        mov eax, dword ptr [esi + ecx*8 + 0x110]
        mov ecx, dword ptr [esi + ecx*8 + 0x108]
        push edx
        push eax
        push ecx
        lea ecx, [esp + 0x18]
        mov dword ptr [esp + 0x2c], 0
        call fun_004a7f30
    torque_at_rpm_adjust:
        mov edx, dword ptr [esi]
        fld dword ptr [edx + 0x1c750]
        mov ecx, dword ptr [esp + 0x14]
        fmul dword ptr [DAT_004e13c4]
        pop edi
        mov dword ptr fs:[0], ecx
        fsubr dword ptr [DAT_004e1348]
        fmulp st(1), st
        fmul dword ptr [esi + 0x108]
        pop esi
        add esp, 0x18
        ret 4
    }
}

// original: FUN_0046afb0

// FUNCTION: RALLYTROPHY 0x46afb0
float Drivetrain::GetDriveTorque()
{
    float t = ratio[gear + 1] * engineTorque * clutch * endRatio;
    return t * (1.0f - car->damage[2] * 0.2f);
}

// original: FUN_0046aff0

// FUNCTION: RALLYTROPHY 0x46aff0
float Drivetrain::GetReactionTorque()
{
    return reactionTorque;
}

// original: FUN_0046b000

// FUNCTION: RALLYTROPHY 0x46b000
int Drivetrain::GetEngagedGear()
{
    int g = 0;
    if (clutch > 0.5f)
        g = gear;
    return g;
}

// original: FUN_0046b020

// FUNCTION: RALLYTROPHY 0x46b020
// Evidence: ev_38a455eebb45bb71b016327f3427b840f47c2138b5826dc4d3e93599a7c8e104.
// Copy construction and dot-product source order preserve VC6 copy/EH scheduling.
int Drivetrain::SelectGear()
{
    CarBody* b = car->body;
    Vec3 v(*(const Vec3*)b->velocity);
    const Vec3* fwd = (const Vec3*)b->forward;
    float dot = v.x * fwd->x + v.y * fwd->y + v.z * fwd->z;
    if (*(unsigned*)&dot & 0x80000000)
        v -= *fwd * dot;
    float w = v.Length() * 60.0f / (car->wheel[0].unk264 * 6.2831855f);
    int g = targetGear;
    if (g <= 0)
        g = 1;
    float r = ratio[g + 1] * endRatio;
    r *= w;
    while (g > 1 && r <= shiftDown[g + 1]) {
        r = ratio[g];
        r *= endRatio;
        r *= w;
        if (r > shiftUp[g])
            break;
        g--;
    }
    r = ratio[g + 1] * endRatio;
    r *= w;
    while (g < numGears && r >= shiftUp[g + 1]) {
        r = ratio[g + 2];
        r *= endRatio;
        r *= w;
        if (r < shiftDown[g + 2])
            break;
        g++;
    }
    return g;
}

// original: FUN_0046b1a0

// FUNCTION: RALLYTROPHY 0x46b1a0
float Drivetrain::GetRpmFraction()
{
    float f = engineSpeed / exhaustPeakRpm;
    if (f > 1.0f)
        f = 1.0f;
    return f;
}

// original: FUN_0046b1d0

// FUNCTION: RALLYTROPHY 0x46b1d0
float Drivetrain::GetShiftProgress()
{
    float r = 0.0f;
    switch (shiftState) {
    case 1:
        if (shiftTimer >= clutchReleaseTime)
            r = 1.0f;
        else
            r = shiftTimer / clutchReleaseTime;
        break;
    case 2:
        r = 1.0f;
        break;
    case 3:
        if (shiftTimer >= clutchEngageTime)
            r = 0.0f;
        else
            r = 1.0f - shiftTimer / clutchEngageTime;
        break;
    }
    return r;
}

// original: FUN_0046b250

// FUNCTION: RALLYTROPHY 0x46b250
void Drivetrain::SetThrottle(float v)
{
    throttle = v;
    if (shiftState == 0)
        throttleEff = v;
}

// original: FUN_0046b280

// FUNCTION: RALLYTROPHY 0x46b280
void Drivetrain::ShiftBy(int delta)
{
    int top = numGears;
    if (car->shiftLimit >= 0.9f)
        top--;
    int g;
    switch (shiftState) {
    case 0:
        g = gear + delta;
        if (g > top || g < -1)
            return;
        shiftTimer = 0.0f;
        targetGear = g;
        shiftState = 1;
        return;
    case 1:
        g = gear;
        goto retarget;
    case 2:
        g = targetGear;
    retarget:
        g += delta;
        if (g > top || g < -1)
            return;
        targetGear = g;
        return;
    case 3:
        g = gear + delta;
        if (g > top || g < -1)
            return;
        targetGear = g;
        shiftState = 1;
        shiftTimer = (1.0f - shiftTimer / clutchEngageTime) * clutchReleaseTime;
        return;
    }
}

// original: FUN_0046b360

// FUNCTION: RALLYTROPHY 0x46b360
void Drivetrain::ShiftTo(int g)
{
    if (g < -1 || g > numGears || g == targetGear)
        return;
    if (g == numGears && car->shiftLimit >= 0.9f)
        return;
    targetGear = g;
    if (shiftState != 0) {
        if (shiftState == 3) {
            shiftState = 1;
            shiftTimer = (1.0f - shiftTimer / clutchEngageTime) * clutchReleaseTime;
        }
    } else {
        shiftTimer = 0.0f;
        shiftState = 1;
    }
}

// original: FUN_0046b3f0

// FUNCTION: RALLYTROPHY 0x46b3f0
void Drivetrain::ComputeEngineTorque()
{
    float x = engineSpeed;
    if (x < idleRpm)
        x = idleRpm;
    float f = -(x * engineFriction);
    engineTorque = (TorqueAtRpm(x) - f) * throttleEff + f;
    float r = ratio[gear + 1] * endRatio;
    float rr = r * r;
    float ee = endRatio * endRatio;
    float a = ee * inertiaDriveShaft + rr * gearInertia[gear + 1];
    float b = (1.0f - car->damage[2] * 0.2f) * clutch * inertiaEngine * rr;
    reactionTorque = a + b;
}

// original: FUN_0046ba60

// FUNCTION: RALLYTROPHY 0x46b4b0
// Splits the drive between the wheels of the driven axles by their spin speeds.
void Drivetrain::DistributeDifferential(int driveType)
{
    if (driveType == 3) {
        DriveCar* c = car;
        float a = (float)fabs(c->wheel[0].spin) + differentialBias;
        float b = (float)fabs(c->wheel[1].spin) + differentialBias;
        float inv = 1.0f / (b + a);
        c->wheel[0].driveShare = inv * a;
        c->wheel[1].driveShare = inv * b;
        a = (float)fabs(c->wheel[3].spin) + differentialBias;
        b = (float)fabs(c->wheel[2].spin) + differentialBias;
        inv = 1.0f / (b + a);
        c->wheel[3].driveShare = inv * a;
        c->wheel[2].driveShare = inv * b;
        return;
    }
    DriveWheel* p;
    DriveWheel* q;
    if (driveType == 2) {
        p = &car->wheel[0];
        q = &car->wheel[1];
    } else {
        p = &car->wheel[3];
        q = &car->wheel[2];
    }
    float a = (float)fabs(p->spin) + differentialBias;
    float b = (float)fabs(q->spin) + differentialBias;
    float inv = 1.0f / (b + a);
    p->driveShare = inv * a * 2;
    q->driveShare = inv * b * 2;
}

// FUNCTION: RALLYTROPHY 0x46b5b0
// Advances the shift state machine, integrates the engine against the driven wheels and limits
// their spin to what the differential allows.
void Drivetrain::Update(int driveType, float dt)
{
    if (shiftState != 0) {
        float t = dt + shiftTimer;
        shiftTimer = t;
        switch (shiftState) {
        case 1:
            if (t >= clutchReleaseTime) {
                clutch = 0;
                throttleEff = 0;
                shiftTimer = 0;
                shiftState = 2;
            } else {
                clutch = 1.0f - t / clutchReleaseTime;
                throttleEff = clutch * throttle;
                if (gear == 0)
                    clutch = 0;
            }
            break;
        case 2:
            if ((car->shiftLimit * 2.25f + 1.0f) * gearShiftTime <= t) {
                gear = targetGear;
                throttleEff = 0;
                clutch = 0;
                shiftTimer = 0;
                shiftState = 3;
            }
            break;
        case 3:
            if (t >= clutchEngageTime) {
                shiftState = 0;
                throttleEff = throttle;
                clutch = 1.0f;
                if (gear == 0)
                    clutch = 0;
            } else {
                clutch = t / clutchEngageTime;
                throttleEff = clutch * throttle;
                if (gear == 0)
                    clutch = 0;
            }
            break;
        }
    } else {
        throttleEff = throttle;
    }
    float* wa;
    float* wb;
    float sum;
    if (driveType == 3) {
        wb = &car->wheel[2].spin;
        wa = &car->wheel[3].spin;
        sum = car->wheel[1].spin;
        sum += car->wheel[0].spin;
        sum += *wb;
        sum += *wa;
        sum *= 0.5f;
    } else {
        DriveWheel* p;
        DriveWheel* q;
        if (driveType == 2) {
            p = &car->wheel[0];
            q = &car->wheel[1];
        } else {
            p = &car->wheel[3];
            q = &car->wheel[2];
        }
        sum = q->spin;
        wb = &q->spin;
        wa = &p->spin;
        sum += *wa;
    }
    if (clutch > 0.5f) {
        if (gear == -1) {
            if (sum > 0)
                sum = 0;
        } else if (gear > 0) {
            if (sum < 0)
                sum = 0;
        }
    }
    float r = IntegrateEngineSpeed(dt, sum * 0.5f) * 2;
    if (clutch > 0.5f) {
        if (gear == -1) {
            if (r > 0)
                r = 0;
        } else if (gear > 0) {
            if (r < 0)
                r = 0;
        }
    }
    float q = r / (2.0f - differentialLock);
    float q2 = (1.0f - differentialLock) * q;
    if ((float)fabs(*wa) > (float)fabs(q))
        *wa = q;
    else if ((float)fabs(*wa) < (float)fabs(q2))
        *wa = q2;
    if ((float)fabs(*wb) > (float)fabs(q))
        *wb = q;
    else if ((float)fabs(*wb) < (float)fabs(q2))
        *wb = q2;
    if (driveType == 3) {
        DriveCar* c = car;
        if ((float)fabs(c->wheel[0].spin) > (float)fabs(q))
            c->wheel[0].spin = q;
        else if ((float)fabs(c->wheel[0].spin) < (float)fabs(q2))
            c->wheel[0].spin = q2;
        if ((float)fabs(c->wheel[1].spin) > (float)fabs(q))
            c->wheel[1].spin = q;
        else if ((float)fabs(c->wheel[1].spin) < (float)fabs(q2))
            c->wheel[1].spin = q2;
    }
}

// FUNCTION: RALLYTROPHY 0x46b940
// Moves the engine speed toward the speed the wheels impose through the clutch; returns the wheel
// speed the engine speed corresponds to.
float Drivetrain::IntegrateEngineSpeed(float dt, float wheelSpeed)
{
    float result = wheelSpeed;
    float x = wheelSpeed * 9.549296f;
    x *= endRatio;
    x *= ratio[gear + 1];
    if (clutch < 1.0f) {
        float inertia;
        if (clutch < 0.001f) {
            inertia = inertiaEngine;
        } else {
            float g = endRatio;
            g *= ratio[gear + 1];
            float g2 = g * g;
            float e2 = endRatio * endRatio;
            inertia = (e2 * inertiaDriveShaft + g2 * gearInertia[gear + 1] + car->wheel[0].unk270 * 2) * clutch / g2 + inertiaEngine;
        }
        float drive = (1.0f - clutch) * engineTorque * 9.549296f;
        engineSpeed = drive * dt / inertia + (x - engineSpeed) * clutch + engineSpeed;
        if (*(unsigned int*)&engineSpeed & 0x80000000)
            engineSpeed = 0;
        if (gear != 0) {
            float d = endRatio;
            d *= ratio[gear + 1];
            result = engineSpeed / d * 0.10471975f;
        }
    } else {
        engineSpeed = x;
    }
    if (*(unsigned int*)&engineSpeed & 0x80000000)
        engineSpeed = 0;
    return result;
}

// Module end: static initialiser pieces of the STL header statics and the guard stub.

// FUNCTION: RALLYTROPHY 0x46ba60
float Drivetrain::GetSmoothedRpm()
{
    float lim = idleRpm * 1.5f;
    float r = engineSpeed;
    if (r < lim) {
        float t = r / lim;
        float tt = t * t;
        r = tt * (lim - idleRpm) + idleRpm;
    }
    return r;
}

// original: FUN_0046baa0

// FUNCTION: RALLYTROPHY 0x46baa0
void Drivetrain::SetGearing(int s)
{
    if (s == gearing)
        return;
    gearing = s;
    float a = 0.0f, b = 0.0f, c = 0.0f;
    switch (s) {
    case 0:
        a = 1.0f;
        break;
    case 1:
        a = 0.5f;
        c = 0.5f;
        break;
    case 2:
        c = 1.0f;
        break;
    case 3:
        b = 0.5f;
        c = 0.5f;
        break;
    case 4:
        b = 1.0f;
        break;
    }
    for (int i = 0; i < 8; i++)
        ratio[i] = (a * rawRatio[i].x + b * rawRatio[i].z + c) * rawRatio[i].y;
}

// FUNCTION: RALLYTROPHY 0x46bb60
// SYMBOL: _$E23
typedef void Generated_46bb60();

// FUNCTION: RALLYTROPHY 0x46bb80
// SYMBOL: _$E22
typedef void Generated_46bb80();
