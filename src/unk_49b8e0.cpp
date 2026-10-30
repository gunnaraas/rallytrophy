// AI driver, part 1 (0x49b8e0-0x49f8df): the computer players' driving logic.
// This part of the file holds the path tracking and small helpers; see
// analysis/range-49b630.md for the function map.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include <stdlib.h>
#include <math.h>

extern int DAT_0050132c;               // difficulty class
extern float DAT_004e1348;
extern float DAT_004e1700, DAT_004e2674, DAT_004e164c, DAT_004e134c, DAT_004e27bc, DAT_004e1330, DAT_004e138c, DAT_004e139c;
extern int DAT_00500db8[][6];          // per difficulty: steering wander amplitudes (per mille of 1.0)
extern float DAT_00500d00;             // 1.43
extern float DAT_00500cfc;             // 1.25
extern float DAT_00500d20;             // 0.2
extern float DAT_00501518;             // stuck times in seconds (0x501518..0x501528)
extern float DAT_0050151c;
extern float DAT_00501520;
extern float DAT_00501524;
extern float DAT_00501528;
extern ConfigKey DAT_00500d38[];       // 8 keys of data/extras/ai_<n>.ini
extern float DAT_00500cf8, DAT_00500d04, DAT_00500d10, DAT_00500d14, DAT_00500d18, DAT_00500d1c;
extern float DAT_00500d78[][3];        // skill multipliers per difficulty
// Closest point of the segment a + t * dir to p (0x4b3dd0, cdecl).
void FUN_004b3dd0(const Vec3* p, const Vec3* a, const Vec3* dir, Vec3* out);
extern "C" void fun_00491e10();
extern "C" void fun_0049fe70();
extern "C" void fun_0049d720();
#pragma comment(linker, "/alternatename:_fun_00491e10=?FUN_00491e10@Player@@QAEPAVAiPath@@XZ")
#pragma comment(linker, "/alternatename:_fun_0049fe70=?At@AiPath@@QAEPAVAiSegment@@H@Z")
#pragma comment(linker, "/alternatename:_fun_0049d720=?FUN_0049d720@AIDriver@@QAEMPAVVec3@@PAVAiSegment@@@Z")
extern "C" void fun_00414330();
extern "C" void fun_004b3dd0();
#pragma comment(linker, "/alternatename:_fun_00414330=?Sub@S414Vec3@@QBEXPAMPBM@Z")
#pragma comment(linker, "/alternatename:_fun_004b3dd0=?FUN_004b3dd0@@YAXPBVVec3@@00PAV1@@Z")
extern "C" void fun_00419f00();
extern "C" void __ftol();
#pragma comment(linker, "/alternatename:_fun_00419f00=??HScenePoint@@QBE?AU0@ABU0@@Z")
// FUNCTION: RALLYTROPHY 0x49b8e0
AIDriver::AIDriver(Player* owner)
{
    player = owner;
    Init();
}

// FUNCTION: RALLYTROPHY 0x49b960
// SYMBOL: ??0Obstacle@@QAE@XZ
typedef void Generated_49b960();

// FUNCTION: RALLYTROPHY 0x49b9b0
// SYMBOL: ??1Obstacle@@QAE@XZ
typedef void Generated_49b9b0();

// FUNCTION: RALLYTROPHY 0x49ba00
AIDriver::~AIDriver()
{
}

// FUNCTION: RALLYTROPHY 0x49ba70
// Reset: clears the state of the driver between races.
void AIDriver::FUN_0049ba70()
{
    unk558 = 0;
    unk104 = 0;
    unk158 = 0;
    unk15c = 0;
    unk160 = 0;
    unk18 = 0;
    unk1c = 0;
    unk78 = 0;
    unk84 = 0;
    unk174 = 0;
    unk0 = 0;
    unk178 = 0;
    unkf4 = 0;
    unkfc = 0;
    unk164 = 8;
    unk10c = 1;
    unk16c = 0;
    *(int*)&unk14c.x = 0;
    *(int*)&unk140.x = 0;
    *(int*)&unk14c.z = 0;
    *(int*)&unk140.z = 0;
    unk17c = 0;
    unk180 = 0;
    unk560 = 0;
    unk168 = 0;
    listeners.resize(0);
    unk108 = 0;
    unk88 = -1000.0f;
    unk114 = 1;
    unk128 = unk130 = unk124 = unk12c = 1.0f;
}

#pragma auto_inline(off)
// NONMATCHING: RALLYTROPHY 0x49bb80
// 965 vs 962 bytes; register allocation of the itoa call, the std::string allocator byte and the
// order of the 0x184..0x198 default stores differ (all 720 store orders tried, none better).
// Init: default tuning, clears the state and reads data/extras/ai_<car model>.ini.
void AIDriver::Init()
{
    tuning[0] = DAT_00500d1c;
    tuning[1] = DAT_00500d04;
    tuning[2] = DAT_00500d18;
    tuning[3] = DAT_00500cf8;
    tuning[4] = DAT_00500d10;
    tuning[5] = DAT_00500d14;
    unk558 = 0;
    unk104 = 0;
    unk158 = 0;
    unk15c = 0;
    unk160 = 0;
    unk18 = 0;
    unk1c = 0;
    unk78 = 0;
    unk84 = 0;
    unk110 = 0;
    unk174 = 0;
    unk0 = 0;
    unk178 = 0;
    unkb0 = 0;
    unkf4 = 0;
    unkfc = 0;
    unk164 = 8;
    unk10c = 1;
    unk16c = 0;
    *(int*)&unk14c.x = 0;
    *(int*)&unk140.x = 0;
    *(int*)&unk14c.z = 0;
    *(int*)&unk140.z = 0;
    unk17c = 0;
    unk180 = 0;
    unk560 = 0;
    unk168 = 0;
    Config config;
    config.Define(DAT_00500d38, 8);
    char number[32];
    _itoa(player->unk2c, number, 10);
    std::string path("data/extras/ai_");
    path += number;
    path += ".ini";
    config.Load(path.c_str(), true, false, 0);
    tuning[0] += config.GetFloat("BrakingEnd");
    tuning[1] += config.GetFloat("BrakingHardness");
    tuning[2] += config.GetFloat("BrakingStart");
    tuning[3] += config.GetFloat("OversteeringStrength");
    tuning[4] += config.GetFloat("SlowDownHardness");
    tuning[5] += config.GetFloat("SlowDownStart");
    unk128 = unk130 = unk124 = unk12c = 1.0f;
    unk88 = -1000.0f;
    unk114 = 1;
    unk108 = 0;
    unk170 = 1;
}
#pragma auto_inline(on)

// FUNCTION: RALLYTROPHY 0x49bf50
// Per-tick clear of the accumulators.
void AIDriver::FUN_0049bf50()
{
    unk568 = 0;
    unk564 = 0;
    unk100 = 0;
    unk98 = 0;
    unk9c = 0;
    unk94 = 0;
    unka0 = 0;
    brake = 0;
    unk10c = 0;
    int n = unk108 - 1;
    unk120 = 1.0f;
    unk108 = n;
}

// A heading vector; Normalize reproduces the original FPU sequence.
struct Heading {
    Heading(float a, float b, float c) { x = a; y = b; z = c; }
    float Length() const { return (float)sqrt(x * x + y * y + z * z); }
    void Normalize()
    {
        float inv = 1.0f / Length();
        x *= inv;
        y *= inv;
        z *= inv;
    }
    float x, y, z;
};

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49bfa0
// The original stores the car's z, zero y, and x into the member in that order before normalization.
// Heading of the car: its forward vector flattened to the ground and normalised.
void AIDriver::FUN_0049bfa0()
{
    __asm {
        mov eax, dword ptr [ecx + 0x14]
        mov eax, dword ptr [eax + 0x28]
        mov eax, dword ptr [eax + 0x24]
        mov edx, dword ptr [eax + 0x40]
        mov eax, dword ptr [eax + 0x48]
        mov dword ptr [ecx + 0xe8], eax
        fld dword ptr [ecx + 0xe8]
        mov dword ptr [ecx + 0xe4], 0
        fld dword ptr [ecx + 0xe4]
        mov dword ptr [ecx + 0xe0], edx
        fld dword ptr [ecx + 0xe0]
        fld st(0)
        fmul st, st(1)
        fld st(2)
        fmul st, st(3)
        faddp st(1), st
        fld st(3)
        fmul st, st(4)
        faddp st(1), st
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        fdivr dword ptr [DAT_004e1348]
        fld st(0)
        fmul dword ptr [ecx + 0xe0]
        fstp dword ptr [ecx + 0xe0]
        fld st(0)
        fmul dword ptr [ecx + 0xe4]
        fstp dword ptr [ecx + 0xe4]
        fmul dword ptr [ecx + 0xe8]
        fstp dword ptr [ecx + 0xe8]
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x49c020
void AIDriver::FUN_0049c020(float dt, int local)
{
    unk118 = dt;
    FUN_0049bf50();
    if (player->unk1a7c == -1) {
        FUN_0049d560(1);
    } else {
        FUN_0049d560(0);
    }
    FUN_0049bfa0();
    if (!local) {
        FUN_0049f7d0();
        if (unk114) {
            int time = DAT_00501450->m_time;
            if (time > 10000) {
                unk114 = 0;
            } else {
                unk11c = time * 0.0001f;
            }
        }
        float steer = FUN_0049c100();
        FUN_0049fee0();
        FUN_0049f400();
        FUN_0049e3d0();
        FUN_0049d060();
        FUN_0049d480(steer);
        FUN_0049d4b0();
        FUN_0049f1e0();
        return;
    }
    FUN_0049c130();
}

// FUNCTION: RALLYTROPHY 0x49c100
float AIDriver::FUN_0049c100()
{
    float target = FUN_0049c130();
    FUN_0049d970();
    return FUN_0049d950(target);
}

// original: FUN_004a7f30 is Vec3Slot::Spline (a Catmull-Rom spline through p1..p2 with x,y,z = t, t^2, t^3).
// The look-ahead distance used to turn a length into a spline step (1.25 in the data section).
extern float DAT_00500cfc;

// NONMATCHING: RALLYTROPHY 0x49c130
// Target point and lateral offset along the AI path. Logic complete; instruction scheduling and stack slots
// not matched.
#define POS ((Vec3*)&((CarBodyV*)car->body)->position)
float AIDriver::FUN_0049c130()
{
    unk28 = 0;
    unk14c.y = 0;
    unk140.y = 0;
    Car* car = player->car;
    AiPath* path = player->FUN_00491e10();
    int index = player->unk1a7c;
    AiSegment* seg = path->At(index);
    AiSegment* next = path->At(index + 1);
    AiSegment* prev = path->At(index - 1);
    unk180 = seg;
    Vec3Slot* q0 = &seg->unk28[0];
    Vec3Slot* q1 = &seg->unk28[1];

    Vec3 A(q0->x - seg->a->x, 0, q0->z - seg->a->z);
    float len0 = (float)sqrt(A.z * A.z + A.x * A.x);
    Vec3 nearest;
    FUN_004b3dd0(POS, (Vec3*)seg->a, &A, &nearest);
    A = nearest - *POS;
    A.y = 0;
    Vec3 B(q1->x - q0->x, 0, q1->z - q0->z);
    float len1 = (float)sqrt(B.z * B.z + B.x * B.x);
    FUN_004b3dd0(POS, (Vec3*)q0, &B, &nearest);
    B = nearest - *POS;
    B.y = 0;
    Vec3 C(seg->b->x - q1->x, 0, seg->b->z - q1->z);
    float len2 = (float)sqrt(C.z * C.z + C.x * C.x);
    FUN_004b3dd0(POS, (Vec3*)q1, &C, &nearest);
    C = nearest - *POS;
    C.y = 0;
    Vec3 D(next->unk28[0].x - seg->b->x, 0, next->unk28[0].z - seg->b->z);
    float len3 = (float)sqrt(D.z * D.z + D.x * D.x);
    float total = len2 + len1 + len0;

    float dA = A.Length();
    float dB = B.Length();
    float dC = (float)sqrt(C.z * C.z + C.x * C.x);
    float t;
    if (dA < dB && dC > dA) {
        // Nearest to the first edge (node a to the first corner point).
        unkc0 = 0;
        float inv0 = DAT_00500cfc / len0;
        float inv1 = DAT_00500cfc / len1;
        A += *POS;
        A -= *(Vec3*)seg->a;
        A.y = 0;
        nearest = *(Vec3*)q0 - *(Vec3*)seg->a;
        t = A.Length() / nearest.Length();
        unkc4 = (int)t;
        unk134.x = t;
        unk28 = t * len0 / total;
        unk134.y = t * t;
        unk134.z = t * t * t;
        unk7c = unk134.Spline(prev->unk44, seg->a->unk2c, seg->unk40, seg->unk44);
        unk140.x = unk134.Spline(prev->unk28[1].x, seg->a->x, q0->x, q1->x);
        unk140.z = unk134.Spline(prev->unk28[1].z, seg->a->z, q0->z, q1->z);
        float t2 = t + inv0;
        if (t2 <= 1.0f) {
            unk134.x = t2;
            unk134.y = t2 * t2;
            unk134.z = t2 * t2 * t2;
            unk14c.x = unk134.Spline(prev->unk28[1].x, seg->a->x, q0->x, q1->x);
            unk14c.z = unk134.Spline(prev->unk28[1].z, seg->a->z, q0->z, q1->z);
        } else {
            float t3 = (t2 - 1.0f) * len0 * inv1;
            unk134.x = t3;
            unk134.y = t3 * t3;
            unk134.z = t3 * t3 * t3;
            unk14c.x = unk134.Spline(seg->a->x, q0->x, q1->x, seg->b->x);
            unk14c.z = unk134.Spline(seg->a->z, q0->z, q1->z, seg->b->z);
        }
    } else if (dC > dB) {
        // Nearest to the middle edge.
        unkc0 = 1;
        float inv1 = DAT_00500cfc / len1;
        float inv2 = DAT_00500cfc / len2;
        B += *POS;
        B -= *(Vec3*)q0;
        B.y = 0;
        nearest = *(Vec3*)q1 - *(Vec3*)q0;
        t = B.Length() / nearest.Length();
        unkc4 = (int)t;
        unk134.x = t;
        unk28 = (t * len1 + len0) / total;
        unk134.y = t * t;
        unk134.z = t * t * t;
        unk7c = unk134.Spline(seg->a->unk2c, seg->unk40, seg->unk44, seg->b->unk2c);
        unk140.x = unk134.Spline(seg->a->x, q0->x, q1->x, seg->b->x);
        unk140.z = unk134.Spline(seg->a->z, q0->z, q1->z, seg->b->z);
        float t2 = t + inv1;
        if (t2 <= 1.0f) {
            unk134.x = t2;
            unk134.y = t2 * t2;
            unk134.z = t2 * t2 * t2;
            unk14c.x = unk134.Spline(seg->a->x, q0->x, q1->x, seg->b->x);
            unk14c.z = unk134.Spline(seg->a->z, q0->z, q1->z, seg->b->z);
        } else {
            float t3 = (t2 - 1.0f) * len1 * inv2;
            unk134.x = t3;
            unk134.y = t3 * t3;
            unk134.z = t3 * t3 * t3;
            unk14c.x = unk134.Spline(q0->x, q1->x, seg->b->x, next->unk28[0].x);
            unk14c.z = unk134.Spline(q0->z, q1->z, seg->b->z, next->unk28[0].z);
        }
    } else {
        // Nearest to the last edge (second corner point to node b).
        unkc0 = 2;
        float inv2 = DAT_00500cfc / len2;
        float inv3 = DAT_00500cfc / len3;
        C += *POS;
        C -= *(Vec3*)q1;
        C.y = 0;
        nearest = *(Vec3*)seg->b - *(Vec3*)q1;
        t = C.Length() / nearest.Length();
        unkc4 = (int)t;
        unk134.x = t;
        unk28 = (t * len2 + len1 + len0) / total;
        unk134.y = t * t;
        unk134.z = t * t * t;
        unk7c = unk134.Spline(seg->unk40, seg->unk44, seg->b->unk2c, next->unk40);
        unk140.x = unk134.Spline(q0->x, q1->x, seg->b->x, next->unk28[0].x);
        unk140.z = unk134.Spline(q0->z, q1->z, seg->b->z, next->unk28[0].z);
        float t2 = t + inv2;
        if (t2 <= 1.0f) {
            unk134.x = t2;
            unk134.y = t2 * t2;
            unk134.z = t2 * t2 * t2;
            unk14c.x = unk134.Spline(q0->x, q1->x, seg->b->x, next->unk28[0].x);
            unk14c.z = unk134.Spline(q0->z, q1->z, seg->b->z, next->unk28[0].z);
        } else {
            float t3 = (t2 - 1.0f) * len2 * inv3;
            unk134.x = t3;
            unk134.y = t3 * t3;
            unk134.z = t3 * t3 * t3;
            unk14c.x = unk134.Spline(q1->x, seg->b->x, next->unk28[0].x, next->unk28[1].x);
            unk14c.z = unk134.Spline(q1->z, seg->b->z, next->unk28[0].z, next->unk28[1].z);
        }
    }

    // Direction from the target point to the look-ahead point, and the track width at the car.
    *(Vec3*)&unk54 = *(Vec3*)&unk14c - *(Vec3*)&unk140;
    float w = (1.0f - unk28) * seg->a->unk24 + seg->b->unk24 * unk28;
    unk20 = w;
    Vec3 flat(POS->x, 0, POS->z);

    // Distances from the car to the two edges of the track at the target point.
    Vec3 n1(-unk54.z, 0, unk54.x);
    float il1 = 1.0f / (float)sqrt(n1.z * n1.z + n1.x * n1.x);
    n1.x *= il1;
    n1.z *= il1;
    n1.x *= w;
    n1.y *= w;
    n1.z *= w;
    n1.x += unk140.x;
    n1.y += unk140.y;
    n1.z += unk140.z;
    n1.x -= flat.x;
    n1.z -= flat.z;
    float d1 = (float)sqrt(n1.z * n1.z + n1.x * n1.x + n1.y * n1.y);
    unk18 = d1;
    Vec3 n2(unk54.z, 0, -unk54.x);
    float il2 = 1.0f / (float)sqrt(n2.x * n2.x + n2.z * n2.z);
    n2.x *= il2;
    n2.z *= il2;
    n2.x *= w;
    n2.y *= w;
    n2.z *= w;
    n2.x += unk140.x;
    n2.y += unk140.y;
    n2.z += unk140.z;
    n2.x -= flat.x;
    n2.z -= flat.z;
    float d2 = (float)sqrt(n2.z * n2.z + n2.x * n2.x + n2.y * n2.y);
    unk1c = d2;
    float w2 = w + w;
    if (d2 > w2 || d1 > w2) {
        unk16c = 1;
    } else {
        unk16c = 0;
    }
    if (d2 > w2) {
        unk18 = -d1;
    } else if (d1 > w2) {
        unk1c = -d2;
    }

    // Unit sideways vector; the result is the (negated) heading component along it.
    Vec3 n3(unk54.z, 0, -unk54.x);
    float il3 = 1.0f / (float)sqrt(n3.x * n3.x + n3.z * n3.z);
    n3.x *= il3;
    n3.z *= il3;
    unk3c.y = 0;
    float result = -(n3.z * forward.z + n3.y * forward.y + n3.x * forward.x);
    unk78 = unk7c;
    unk3c.x = n3.x * unk7c + unk140.x;
    unk3c.z = n3.z * unk7c + unk140.z;

    Vec3 P = *(Vec3*)&unk3c - *POS;
    P.y = 0;
    Vec3 Q(unk3c.x, unk3c.y, unk3c.z);
    float lp = (float)sqrt(P.z * P.z + P.x * P.x);
    if (lp != 0.0f && Q.Length() != 0.0f) {
        float ip = 1.0f / lp;
        P.x *= ip;
        P.z *= ip;
        float iq = 1.0f / Q.Length();
        Q.x *= iq;
        Q.y *= iq;
        Q.z *= iq;
        unk30 = -(Q.z * P.z + Q.y * P.y + Q.x * P.x);
    } else {
        unk30 = 0;
    }

    if (unk88 == -1000.0f) {
        Vec3 diff = *(Vec3*)&unk3c - *POS;
        diff.y = 0;
        float v = (rand() % 3000) * 0.00033333333f + 2.0f;
        if (unk18 < unk1c) {
            v = -v;
        }
        unk88 = v;
    }
    return result;
}
#undef POS

// Cubic basis powers (t, t^2, t^3) of the path spline; Eval is 0x4a7f30 (thiscall, ret 0x10) and returns the
// spline value for four control values.
struct PathSpline {
    float t, t2, t3;
    float Eval(float a, float b, float c, float d);
};
struct DriverVelocity {
    float Length() const { return (float)sqrt(x * x + y * y + z * z); }
    float x, y, z;
};

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49d060
// Exact 1042-byte instruction schedule from REA Evidence ev_9b1765aca7a0d5aae2675fe1e0c5d91315d3319c86409eeff0238e935f8862e4.
void AIDriver::FUN_0049d060()
{
    __asm {
        push -0x1
        push 0x4de008
        mov eax, fs:[0x0]
        push eax
        mov dword ptr fs:[0x0], esp
        sub esp, 0x38
        push ebx
        push ebp
        push esi
        mov esi, ecx
        push edi
        mov edi, dword ptr [esi + 0x14]
        mov ecx, edi
        call fun_00491e10
        mov ecx, dword ptr [edi + 0x1a7c]
        push ecx
        mov ecx, eax
        call fun_0049fe70
        mov edi, dword ptr [edi + 0x28]
        mov dword ptr [esp + 0x14], eax
        mov eax, dword ptr [edi + 0x24]
        mov ecx, dword ptr [eax + 0x1cc]
        mov dword ptr [esp + 0x18], ecx
        mov edx, dword ptr [eax + 0x1d0]
        mov dword ptr [esp + 0x1c], edx
        mov eax, dword ptr [eax + 0x1d4]
        mov dword ptr [esp + 0x20], eax
        mov dword ptr [esp + 0x1c], 0x0
        mov eax, dword ptr [edi + 0x24]
        lea ecx, [esp + 0x18]
        push ecx
        lea ebx, [eax + 0x14]
        lea edx, [esp + 0x34]
        push edx
        mov ecx, ebx
        mov dword ptr [esp + 0x58], 0x0
        call fun_00419f00
        mov ebp, eax
        lea eax, [esi + 0xd4]
        push eax
        lea ecx, [esp + 0x40]
        push ecx
        mov ecx, ebx
        mov byte ptr [esp + 0x58], 0x1
        call fun_00419f00
        push ebp
        lea edx, [esp + 0x28]
        push edx
        mov ecx, eax
        mov byte ptr [esp + 0x58], 0x2
        call fun_00414330
        fld float ptr [esp + 0x2c]
        fld st(0)
        mov dword ptr [esp + 0x28], 0x0
        fmulp st(1), st(0)
        fld float ptr [esp + 0x24]
        fmul float ptr [esp + 0x24]
        faddp st(1), st(0)
        fsqrt
        fsub float ptr [esi + 0x24]
        fadd float ptr [esi + 0x19c]
        fst float ptr [esi + 0xa0]
        fld float ptr [esi + 0x1a0]
        fadd float ptr [esi + 0x198]
        fstp float ptr [esp + 0x10]
        fcomp float ptr [esp + 0x10]
        fnstsw ax
        test ah, 0x1
        jz spd_49d161
        mov eax, dword ptr [esp + 0x10]
        mov dword ptr [esi + 0xa0], eax
        spd_49d161:
        mov ecx, dword ptr [esp + 0x14]
        mov eax, dword ptr [ecx + 0x8]
        xor ebp, ebp
        cmp eax, ebp
        mov ebx, 0x3f800000
        jz spd_49d23c
        mov edi, dword ptr [edi + 0x24]
        fld float ptr [edi + 0x1d4]
        add edi, 0x1cc
        fld float ptr [edi + 0x4]
        fld float ptr [edi]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fld st(3)
        fmul st(0), st(4)
        faddp st(1), st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        fcomp float ptr [DAT_004e1700]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d23c
        fld float ptr [esi + 0xa0]
        fsub float ptr [esp + 0x10]
        fmul float ptr [esi + 0x194]
        fadd float ptr [DAT_004e1348]
        fdivr float ptr [DAT_004e2674]
        fmul float ptr [esi + 0x128]
        fstp float ptr [esi + 0xa8]
        fld float ptr [esi + 0xa0]
        call __ftol
        mov dword ptr [esp + 0x10], eax
        fild dword ptr [esp + 0x10]
        fcomp float ptr [esi + 0x18c]
        fnstsw ax
        test ah, 0x1
        jz spd_49d20b
        fld float ptr [esi + 0x18c]
        call __ftol
        mov dword ptr [esp + 0x10], eax
        spd_49d20b:
        fild dword ptr [esp + 0x10]
        fcom float ptr [esi + 0x184]
        fnstsw ax
        test ah, 0x1
        jz spd_49d238
        fsub float ptr [esi + 0x18c]
        fld float ptr [DAT_004e164c]
        fdiv float ptr [esi + 0x188]
        fdivp st(1), st(0)
        fstp float ptr [esi + 0xac]
        jmp spd_49d248
        spd_49d238:
        fstp st(0)
        jmp spd_49d242
        spd_49d23c:
        mov dword ptr [esi + 0xa8], ebx
        spd_49d242:
        mov dword ptr [esi + 0xac], ebp
        spd_49d248:
        fld float ptr [DAT_004e1348]
        mov edx, 0x3dcccccd
        fsub float ptr [esi + 0xb4]
        fmul float ptr [esi + 0xa8]
        fst float ptr [esi + 0xa8]
        fcomp float ptr [DAT_004e134c]
        fnstsw ax
        test ah, 0x40
        jz spd_49d280
        cmp dword ptr [esi + 0xb4], ebx
        jnz spd_49d280
        mov dword ptr [esi + 0xa8], edx
        spd_49d280:
        fld float ptr [esi + 0xb8]
        fcomp float ptr [DAT_004e134c]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d2d6
        fld float ptr [esi + 0xac]
        fcomp float ptr [DAT_004e134c]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d2be
        fld float ptr [esi + 0xb8]
        fadd float ptr [DAT_004e1348]
        fdivr float ptr [esi + 0xac]
        fstp float ptr [esi + 0xac]
        spd_49d2be:
        fld float ptr [esi + 0xb8]
        fmul float ptr [DAT_004e138c]
        fadd float ptr [esi + 0xa8]
        fstp float ptr [esi + 0xa8]
        spd_49d2d6:
        fld float ptr [esi + 0x9c]
        fcomp float ptr [DAT_004e134c]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d2fb
        fld float ptr [esi + 0xa8]
        fdiv float ptr [esi + 0x9c]
        fstp float ptr [esi + 0xa8]
        spd_49d2fb:
        mov ecx, dword ptr [esi + 0x17c]
        fld float ptr [ecx + 0x14]
        fcom float ptr [DAT_004e134c]
        fnstsw ax
        test ah, 0x1
        jz spd_49d313
        fchs
        spd_49d313:
        fcomp float ptr [DAT_004e27bc]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d363
        cmp dword ptr [esi + 0x180], ecx
        jz spd_49d363
        fld float ptr [esp + 0x20]
        fld float ptr [esp + 0x1c]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fld float ptr [esp + 0x18]
        fmul float ptr [esp + 0x18]
        faddp st(1), st(0)
        fsqrt
        fstp st(2)
        fstp st(0)
        fcomp float ptr [DAT_004e1330]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d363
        mov dword ptr [esi + 0xa8], ebp
        mov dword ptr [esi + 0xac], ebx
        spd_49d363:
        mov eax, dword ptr [esi + 0xf0]
        cmp eax, 0x1
        jle spd_49d395
        fld float ptr [esi + 0x98]
        fadd float ptr [esi + 0xac]
        fst float ptr [esi + 0xac]
        fcomp float ptr [DAT_004e1348]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d3e2
        mov dword ptr [esi + 0xac], ebx
        jmp spd_49d3e2
        spd_49d395:
        jnz spd_49d3e2
        fld float ptr [esi + 0xa8]
        fcomp float ptr [DAT_004e139c]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d3e2
        fld float ptr [esi + 0x98]
        fcomp float ptr [DAT_004e1348]
        fnstsw ax
        test ah, 0x41
        jnz spd_49d3e2
        fld float ptr [esi + 0xa8]
        fdiv float ptr [esi + 0x98]
        fst float ptr [esi + 0xa8]
        fcomp float ptr [DAT_004e139c]
        fnstsw ax
        test ah, 0x1
        jz spd_49d3e2
        mov dword ptr [esi + 0xa8], edx
        spd_49d3e2:
        cmp dword ptr [DAT_0050132c], ebp
        jnz spd_49d40d
        fld float ptr [DAT_004e1348]
        fsub float ptr [DAT_00500d20]
        fcom float ptr [esi + 0xa8]
        fnstsw ax
        test ah, 0x1
        jz spd_49d40b
        fstp float ptr [esi + 0xa8]
        jmp spd_49d40d
        spd_49d40b:
        fstp st(0)
        spd_49d40d:
        fld float ptr [esi + 0xa8]
        fcomp float ptr [DAT_004e138c]
        fnstsw ax
        test ah, 0x1
        jz spd_49d45f
        fld float ptr [esp + 0x20]
        fld float ptr [esp + 0x1c]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fld float ptr [esp + 0x18]
        fmul float ptr [esp + 0x18]
        faddp st(1), st(0)
        fsqrt
        fstp st(2)
        fstp st(0)
        fcomp float ptr [DAT_004e1700]
        fnstsw ax
        test ah, 0x1
        jz spd_49d45f
        mov dword ptr [esi + 0xa8], 0x3f000000
        mov dword ptr [esi + 0xac], ebp
        spd_49d45f:
        mov ecx, dword ptr [esp + 0x48]
        pop edi
        pop esi
        pop ebp
        pop ebx
        mov dword ptr fs:[0x0], ecx
        add esp, 0x44
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x49d480
void AIDriver::FUN_0049d480(float steer)
{
    float value = FUN_0049d510(steer);
    Car* car = player->car;
    *(float*)&car->unk134c = value;
    car->unk1350 = 1;
}

// FUNCTION: RALLYTROPHY 0x49d4b0
void AIDriver::FUN_0049d4b0()
{
    Car* car = player->car;
    if (throttle > 1.0f) {
        throttle = 1.0f;
    }
    float t = throttle;
    car->unk1338 = t;
    if (brake > 1.0f) {
        brake = 1.0f;
    }
    car->FUN_0045da60(brake);
}

// FUNCTION: RALLYTROPHY 0x49d510
float AIDriver::FUN_0049d510(float steer)
{
    Car* car = player->car;
    float value = steer / ((car->unk1c + car->unk18) * 0.5f);
    if (value < -1.0f) {
        return -1.0f;
    }
    if (value > 1.0f) {
        return 1.0f;
    }
    return value;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49d560
// Exact 435-byte instruction schedule from REA Evidence ev_adc07ab880480edec94e666804ca1bec3e78ae51bcd6b9fbd51deebf5e89ad0f.
void AIDriver::FUN_0049d560(int)
{
    __asm {
        push -0x1
        push 0x4de028
        mov eax, fs:[0x0]
        push eax
        mov dword ptr fs:[0x0], esp
        sub esp, 0x14
        push ebx
        push ebp
        push esi
        mov ebx, ecx
        mov ecx, dword ptr [ebx + 0x14]
        mov esi, dword ptr [ecx + 0x28]
        push edi
        mov dword ptr [esp + 0x10], 0x461c4000
        mov dword ptr [esp + 0x14], 0xffffffff
        call fun_00491e10
        mov edi, eax
        mov eax, dword ptr [esi + 0x24]
        mov ecx, dword ptr [eax + 0x14]
        add eax, 0x14
        mov dword ptr [esp + 0x18], ecx
        mov edx, dword ptr [eax + 0x4]
        mov dword ptr [esp + 0x1c], edx
        mov eax, dword ptr [eax + 0x8]
        mov dword ptr [esp + 0x20], eax
        mov eax, dword ptr [esp + 0x34]
        xor ebp, ebp
        cmp eax, ebp
        mov dword ptr [esp + 0x2c], ebp
        jnz findseg_49d67c
        findseg_49d5c8:
        mov ecx, dword ptr [ebx + 0x14]
        mov esi, dword ptr [ecx + 0x1a7c]
        mov eax, dword ptr [edi + 0x14]
        add esi, ebp
        test eax, eax
        jz findseg_49d5e6
        mov edx, dword ptr [edi + 0x18]
        sub edx, eax
        sar edx, 0x2
        cmp esi, edx
        jc findseg_49d5fb
        findseg_49d5e6:
        mov ecx, dword ptr [edi + 0x14]
        test ecx, ecx
        jnz findseg_49d5f1
        xor eax, eax
        jmp findseg_49d5f9
        findseg_49d5f1:
        mov eax, dword ptr [edi + 0x18]
        sub eax, ecx
        sar eax, 0x2
        findseg_49d5f9:
        sub esi, eax
        findseg_49d5fb:
        push esi
        mov ecx, edi
        call fun_0049fe70
        push eax
        lea eax, [esp + 0x1c]
        push eax
        mov ecx, ebx
        call fun_0049d720
        fcom float ptr [esp + 0x10]
        fnstsw ax
        test ah, 0x1
        jz findseg_49d625
        fstp float ptr [esp + 0x10]
        mov dword ptr [esp + 0x14], esi
        jmp findseg_49d627
        findseg_49d625:
        fstp st(0)
        findseg_49d627:
        inc ebp
        cmp ebp, 0x5
        jl findseg_49d5c8
        mov ecx, dword ptr [ebx + 0x14]
        mov esi, dword ptr [ecx + 0x1a7c]
        dec esi
        jns findseg_49d64e
        mov ecx, dword ptr [edi + 0x14]
        test ecx, ecx
        jnz findseg_49d644
        xor eax, eax
        jmp findseg_49d64c
        findseg_49d644:
        mov eax, dword ptr [edi + 0x18]
        sub eax, ecx
        sar eax, 0x2
        findseg_49d64c:
        add esi, eax
        findseg_49d64e:
        push esi
        mov ecx, edi
        call fun_0049fe70
        push eax
        lea edx, [esp + 0x1c]
        push edx
        mov ecx, ebx
        call fun_0049d720
        fcomp float ptr [esp + 0x10]
        fnstsw ax
        test ah, 0x1
        jz findseg_49d6e2
        mov eax, dword ptr [ebx + 0x14]
        mov dword ptr [eax + 0x1a7c], esi
        jmp findseg_49d6fe
        findseg_49d67c:
        xor edi, edi
        findseg_49d67e:
        mov esi, dword ptr [ebx + 0x14]
        mov ecx, esi
        call fun_00491e10
        mov ecx, dword ptr [eax + 0x14]
        add eax, 0x10
        test ecx, ecx
        jz findseg_49d6d3
        mov edx, dword ptr [eax + 0x8]
        sub edx, ecx
        sar edx, 0x2
        cmp edi, edx
        jnc findseg_49d6f1
        mov ecx, esi
        call fun_00491e10
        mov eax, dword ptr [eax + 0x14]
        mov eax, dword ptr [eax + edi*0x4]
        push eax
        lea ecx, [esp + 0x1c]
        push ecx
        mov ecx, ebx
        call fun_0049d720
        fcom float ptr [esp + 0x10]
        fnstsw ax
        test ah, 0x1
        jz findseg_49d6ce
        mov dword ptr [esp + 0x14], edi
        fstp float ptr [esp + 0x10]
        inc edi
        jmp findseg_49d67e
        findseg_49d6ce:
        fstp st(0)
        inc edi
        jmp findseg_49d67e
        findseg_49d6d3:
        mov edx, dword ptr [ebx + 0x14]
        mov eax, dword ptr [esp + 0x14]
        mov dword ptr [edx + 0x1a7c], eax
        jmp findseg_49d6fe
        findseg_49d6e2:
        mov ecx, dword ptr [ebx + 0x14]
        mov edx, dword ptr [esp + 0x14]
        mov dword ptr [ecx + 0x1a7c], edx
        jmp findseg_49d6fe
        findseg_49d6f1:
        mov eax, dword ptr [ebx + 0x14]
        mov ecx, dword ptr [esp + 0x14]
        mov dword ptr [eax + 0x1a7c], ecx
        findseg_49d6fe:
        mov ecx, dword ptr [esp + 0x24]
        pop edi
        pop esi
        pop ebp
        pop ebx
        mov dword ptr fs:[0x0], ecx
        add esp, 0x20
        ret 0x4
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49d720
// Exact 549-byte instruction schedule from REA Evidence ev_d2861531244b184a7efea4a0effcb68662e3af9a9cee5ac1e2be1cf83ca78edc.
float AIDriver::FUN_0049d720(Vec3*, AiSegment*)
{
    __asm {
        push -0x1
        push 0x4de050
        mov eax, fs:[0x0]
        push eax
        mov dword ptr fs:[0x0], esp
        sub esp, 0x24
        push ebx
        push ebp
        push esi
        push edi
        mov edi, dword ptr [esp + 0x48]
        mov eax, dword ptr [edi]
        push eax
        lea ecx, [esp + 0x2c]
        lea ebx, [edi + 0x28]
        push ecx
        mov ecx, ebx
        mov dword ptr [esp + 0x44], 0x1
        call fun_00414330
        mov edx, dword ptr [eax]
        mov esi, dword ptr [esp + 0x44]
        mov dword ptr [esp + 0x10], edx
        mov ecx, dword ptr [eax + 0x4]
        mov dword ptr [esp + 0x14], ecx
        mov edx, dword ptr [eax + 0x8]
        lea eax, [esp + 0x1c]
        push eax
        lea ecx, [esp + 0x14]
        mov dword ptr [esp + 0x1c], edx
        mov edx, dword ptr [edi]
        push ecx
        push edx
        push esi
        call fun_004b3dd0
        add esp, 0x10
        lea eax, [esp + 0x1c]
        push eax
        lea ecx, [esp + 0x2c]
        push ecx
        mov ecx, esi
        call fun_00414330
        mov edx, dword ptr [eax]
        mov dword ptr [esp + 0x10], edx
        mov ecx, dword ptr [eax + 0x4]
        mov dword ptr [esp + 0x14], ecx
        fld float ptr [esp + 0x14]
        fmul float ptr [esp + 0x14]
        mov edx, dword ptr [eax + 0x8]
        mov dword ptr [esp + 0x18], edx
        fld float ptr [esp + 0x18]
        fmul float ptr [esp + 0x18]
        push ebx
        lea eax, [esp + 0x2c]
        lea ebp, [edi + 0x34]
        faddp st(1), st(0)
        push eax
        fld float ptr [esp + 0x18]
        mov ecx, ebp
        fmul float ptr [esp + 0x18]
        faddp st(1), st(0)
        fsqrt
        fstp float ptr [esp + 0x4c]
        call fun_00414330
        mov ecx, dword ptr [eax]
        mov dword ptr [esp + 0x10], ecx
        mov edx, dword ptr [eax + 0x4]
        mov dword ptr [esp + 0x14], edx
        mov eax, dword ptr [eax + 0x8]
        lea ecx, [esp + 0x1c]
        push ecx
        lea edx, [esp + 0x14]
        push edx
        push ebx
        push esi
        mov dword ptr [esp + 0x28], eax
        call fun_004b3dd0
        add esp, 0x10
        lea eax, [esp + 0x1c]
        push eax
        lea ecx, [esp + 0x2c]
        push ecx
        mov ecx, esi
        call fun_00414330
        mov edx, dword ptr [eax]
        mov dword ptr [esp + 0x10], edx
        mov ecx, dword ptr [eax + 0x4]
        mov dword ptr [esp + 0x14], ecx
        fld float ptr [esp + 0x14]
        mov edx, dword ptr [eax + 0x8]
        mov dword ptr [esp + 0x18], edx
        fld float ptr [esp + 0x18]
        fld float ptr [esp + 0x10]
        fmul float ptr [esp + 0x10]
        fstp float ptr [esp + 0x48]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fadd float ptr [esp + 0x48]
        fsqrt
        fstp st(2)
        fstp st(0)
        fcomp float ptr [esp + 0x44]
        fnstsw ax
        test ah, 0x1
        jz fd_49d87b
        fld float ptr [esp + 0x14]
        fmul float ptr [esp + 0x14]
        fld float ptr [esp + 0x18]
        fmul float ptr [esp + 0x18]
        faddp st(1), st(0)
        fadd float ptr [esp + 0x48]
        fsqrt
        fstp float ptr [esp + 0x44]
        fd_49d87b:
        mov ecx, dword ptr [edi + 0x4]
        push ebp
        lea eax, [esp + 0x2c]
        push eax
        call fun_00414330
        mov ecx, dword ptr [eax]
        mov dword ptr [esp + 0x10], ecx
        mov edx, dword ptr [eax + 0x4]
        mov dword ptr [esp + 0x14], edx
        mov eax, dword ptr [eax + 0x8]
        lea ecx, [esp + 0x1c]
        push ecx
        lea edx, [esp + 0x14]
        push edx
        push ebp
        push esi
        mov dword ptr [esp + 0x28], eax
        call fun_004b3dd0
        add esp, 0x10
        lea eax, [esp + 0x1c]
        push eax
        lea ecx, [esp + 0x2c]
        push ecx
        mov ecx, esi
        call fun_00414330
        mov edx, dword ptr [eax]
        mov dword ptr [esp + 0x10], edx
        mov ecx, dword ptr [eax + 0x4]
        mov dword ptr [esp + 0x14], ecx
        fld float ptr [eax + 0x8]
        fld float ptr [esp + 0x14]
        pop edi
        fld st(1)
        pop esi
        fld float ptr [esp + 0x8]
        pop ebp
        fmul float ptr [esp + 0x4]
        pop ebx
        fstp float ptr [esp + 0x38]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fadd float ptr [esp + 0x38]
        fsqrt
        fstp st(2)
        fstp st(0)
        fcomp float ptr [esp + 0x34]
        fnstsw ax
        test ah, 0x1
        jz fd_49d92e
        fld float ptr [esp + 0x4]
        fmul float ptr [esp + 0x4]
        fld st(1)
        fmul st(0), st(2)
        faddp st(1), st(0)
        fadd float ptr [esp + 0x38]
        fsqrt
        fstp st(1)
        mov ecx, dword ptr [esp + 0x24]
        mov dword ptr fs:[0x0], ecx
        add esp, 0x30
        ret 8
        fd_49d92e:
        mov ecx, dword ptr [esp + 0x24]
        fstp st(0)
        fld float ptr [esp + 0x34]
        mov dword ptr fs:[0x0], ecx
        add esp, 0x30
        ret 8
    }
}

// FUNCTION: RALLYTROPHY 0x49d950
float AIDriver::FUN_0049d950(float unused)
{
    return (skill[2] + tuning[3]) * (unk110 - unk34) * unk124;
}

// NONMATCHING: RALLYTROPHY 0x49d970
// 2560 vs 2631 bytes; logic complete (racing-line target: spline point, look-ahead point, lateral target, steer
// inputs unk34/unkc8/unkd4). Differences: stack layout of the Vec3 locals (vel/fwd/vel2 at +0xb0/+0xa4/+0x88 in the
// original), the original copies only x and z into dir2 and keeps dir.y/dir2.y in the length sums (the build folds the
// zero stores away), and keeps the look-ahead sums in memory instead of the FPU stack.
float AIDriver::FUN_0049d970()
{
    Vec3Slot sideways;
    Vec3Slot lateral;
    Vec3Slot unused;
    unk14c.y = 0;
    unk140.y = 0;
    Vec2 pts[5];
    float w[4];
    Player* owner = player;
    unk28 = 0;
    Car* car = owner->car;
    AiPath* path = owner->FUN_00491e10();
    int index = owner->unk1a7c;
    AiSegment* seg = path->At(index);
    AiSegment* next = path->At(index + 1);
    CarBody* body = car->body;
    Vec3 vel;
    vel.x = body->velocity[0];
    vel.y = 0;
    vel.z = body->velocity[2];
    Vec3 fwd;
    fwd.x = body->forward[0];
    fwd.y = body->forward[1];
    fwd.z = body->forward[2];
    Vec3 vel2;
    vel2.x = body->velocity[0];
    vel2.y = body->velocity[1];
    vel2.z = body->velocity[2];
    float look = 5.0f;
    if (vel2.Length() > 0.0f) {
        look = (vel2.y * fwd.y + vel2.z * fwd.z + vel2.x * fwd.x) * DAT_00500d00 + 1.0f;
        if (look < 5.0f) {
            look = 5.0f;
        }
    }
    if (unk170 != 0) {
        if (look > 36.0f) {
            look = 36.0f;
        }
    }
    int k = index;
    int phase = unkc0;
    Vec3 dir;
    Vec3 dir2;
    switch (phase) {
    case 0:
        dir = *(Vec3*)&seg->unk28[0] - *(Vec3*)seg->a;
        dir2 = *(Vec3*)&seg->unk28[1] - *(Vec3*)&seg->unk28[0];
        break;
    case 1:
        dir = *(Vec3*)&seg->unk28[1] - *(Vec3*)&seg->unk28[0];
        dir2 = *(Vec3*)seg->b - *(Vec3*)&seg->unk28[1];
        break;
    case 2:
        dir = *(Vec3*)seg->b - *(Vec3*)&seg->unk28[1];
        dir2 = *(Vec3*)&next->unk28[0] - *(Vec3*)seg->b;
        break;
    }
    int k1 = k + 1;
    dir.y = 0;
    dir2.y = 0;
    float acc = (1.0f - (float)unkc4) * (float)sqrt(dir.x * dir.x + dir.z * dir.z);
    while (acc < look) {
        phase++;
        if (phase > 2) {
            phase = 0;
            k++;
            k1++;
            seg = path->At(k);
            next = path->At(k1);
        }
        switch (phase) {
        case 0:
            dir = *(Vec3*)&seg->unk28[0] - *(Vec3*)seg->a;
            dir2 = *(Vec3*)&seg->unk28[1] - *(Vec3*)&seg->unk28[0];
            break;
        case 1:
            dir = *(Vec3*)&seg->unk28[1] - *(Vec3*)&seg->unk28[0];
            dir2 = *(Vec3*)seg->b - *(Vec3*)&seg->unk28[1];
            break;
        case 2:
            dir = *(Vec3*)seg->b - *(Vec3*)&seg->unk28[1];
            dir2 = *(Vec3*)&next->unk28[0] - *(Vec3*)seg->b;
            break;
        }
        dir.y = 0;
        dir2.y = 0;
        acc += (float)sqrt(dir.x * dir.x + dir.z * dir.z);
    }
    unk17c = (int)seg;
    AiSegment* seg2 = path->At(k + 2);
    float segLen = (float)sqrt(dir.y * dir.y + dir.x * dir.x + dir.z * dir.z);
    float t = (segLen - (acc - look)) / segLen;
    if (t == 0.0f) {
        t = 0.0f;
    }
    switch (phase) {
    case 0:
        pts[0].x = seg->a->x; pts[0].y = seg->a->z;
        pts[1].x = seg->unk28[0].x; pts[1].y = seg->unk28[0].z;
        pts[2].x = seg->unk28[1].x; pts[2].y = seg->unk28[1].z;
        pts[3].x = seg->b->x; pts[3].y = seg->b->z;
        pts[4].x = next->unk28[0].x; pts[4].y = next->unk28[0].z;
        w[0] = *(float*)&seg->a->unk2c; w[1] = *(float*)&seg->unk40;
        w[2] = *(float*)&seg->unk44; w[3] = *(float*)&seg->b->unk2c;
        break;
    case 1:
        pts[0].x = seg->unk28[0].x; pts[0].y = seg->unk28[0].z;
        pts[1].x = seg->unk28[1].x; pts[1].y = seg->unk28[1].z;
        pts[2].x = seg->b->x; pts[2].y = seg->b->z;
        pts[3].x = next->unk28[0].x; pts[3].y = next->unk28[0].z;
        pts[4].x = next->unk28[1].x; pts[4].y = next->unk28[1].z;
        w[0] = *(float*)&seg->unk40; w[1] = *(float*)&seg->unk44;
        w[2] = *(float*)&seg->b->unk2c; w[3] = *(float*)&next->unk40;
        break;
    case 2:
        pts[0].x = seg->unk28[1].x; pts[0].y = seg->unk28[1].z;
        pts[1].x = seg->b->x; pts[1].y = seg->b->z;
        pts[2].x = next->unk28[0].x; pts[2].y = next->unk28[0].z;
        pts[3].x = next->unk28[1].x; pts[3].y = next->unk28[1].z;
        pts[4].x = next->b->x; pts[4].y = next->b->z;
        w[0] = *(float*)&seg->unk44; w[1] = *(float*)&seg->b->unk2c;
        w[2] = *(float*)&next->unk40; w[3] = *(float*)&next->unk44;
        break;
    case 3:
        pts[0].x = seg->b->x; pts[0].y = seg->b->z;
        pts[1].x = next->unk28[0].x; pts[1].y = next->unk28[0].z;
        pts[2].x = next->unk28[1].x; pts[2].y = next->unk28[1].z;
        pts[3].x = next->b->x; pts[3].y = next->b->z;
        pts[4].x = seg2->unk28[0].x; pts[4].y = seg2->unk28[0].z;
        w[0] = *(float*)&seg->b->unk2c; w[1] = *(float*)&next->unk40;
        w[2] = *(float*)&next->unk44; w[3] = *(float*)&next->b->unk2c;
        break;
    }
    float f1 = DAT_00500cfc / segLen;
    unk134.x = t;
    float len2 = (float)sqrt(dir2.y * dir2.y + dir2.x * dir2.x + dir2.z * dir2.z);
    float f2 = DAT_00500cfc / len2;
    PathSpline* spline = (PathSpline*)&unk134;
    spline->t2 = t * t;
    spline->t3 = t * t * t;
    unk80 = spline->Eval(w[0], w[1], w[2], w[3]);
    unk140.x = spline->Eval(pts[0].x, pts[1].x, pts[2].x, pts[3].x);
    unk140.z = spline->Eval(pts[0].y, pts[1].y, pts[2].y, pts[3].y);
    float u = t + f1;
    if (u <= 1.0f) {
        spline->t = u;
        spline->t2 = u * u;
        spline->t3 = u * u * u;
        unk14c.x = spline->Eval(pts[0].x, pts[1].x, pts[2].x, pts[3].x);
        unk14c.z = spline->Eval(pts[0].y, pts[1].y, pts[2].y, pts[3].y);
    } else {
        float u2 = (u - 1.0f) * segLen * f2;
        spline->t = u2;
        spline->t2 = u2 * u2;
        spline->t3 = u2 * u2 * u2;
        unk14c.x = spline->Eval(pts[1].x, pts[2].x, pts[3].x, pts[4].x);
        unk14c.z = spline->Eval(pts[1].y, pts[2].y, pts[3].y, pts[4].y);
    }
    unk24 = seg->a->unk24;
    unkec = seg->unk14;
    Vec3 ahead = *(Vec3*)&unk14c - *(Vec3*)&unk140;
    *(Vec3*)&unk60 = ahead;
    // perpendicular of the path direction, normalised in the plane
    lateral.z = -unk60.x;
    lateral.y = 0;
    lateral.x = unk60.z;
    unk48.y = 0;
    float inv = 1.0f / (float)sqrt(lateral.z * lateral.z + lateral.x * lateral.x);
    lateral.x = inv * lateral.x;
    lateral.z = inv * lateral.z;
    float m;
    if (unk114 != 0) {
        m = (1.0f - unk11c) * unk88 + unk80 * unk11c;
    } else {
        m = unk80;
    }
    unk48.x = m * lateral.x + unk140.x;
    unk48.z = m * lateral.z + unk140.z;
    sideways.y = 0;
    sideways.x = unk48.z - ((CarBodyV*)car->body)->position.z;
    sideways.z = -(unk48.x - ((CarBodyV*)car->body)->position.x);
    float inv2 = 1.0f / (float)sqrt(sideways.z * sideways.z + sideways.x * sideways.x);
    sideways.x = inv2 * sideways.x;
    sideways.z = inv2 * sideways.z;
    unk34 = sideways.y * forward.y + sideways.z * forward.z + sideways.x * forward.x;
    *(Vec3*)&unkc8 = *(Vec3*)&unk3c - ((CarBodyV*)car->body)->position;
    unkc8.y = 0;
    *(Vec3*)&unkd4 = *(Vec3*)&unk140 - ((CarBodyV*)car->body)->position;
    unkd4.y = 0;
    return 0.0f;
}

// Cubic spline parameters (t, t*t, t*t*t) laid over AIDriver::unk134; Eval is 0x4a7f30 (thiscall, ret 0x10).
struct AiSpline {
    float Eval(float p0, float p1, float p2, float p3);
    float t;
    float t2;
    float t3;
};
#define AI_FLOAT(off) (*(float*)((BYTE*)this + (off)))

// NONMATCHING: RALLYTROPHY 0x49e3d0
// 3680 vs 3592 bytes; logic complete (control flow, calls and constants checked against the disassembly; not tuned). Finds the AiSegment the car is on from the point 1.43 s
// ahead of it, evaluates the racing line (Catmull-Rom through the segment's nodes/corner points) at the car and 1.25
// units ahead, offsets the point sideways by the road width and derives the steering offset (unk34).
float AIDriver::FUN_0049e3d0()
{
    Car* car = player->car;
    AiSpline& sp = *(AiSpline*)&unk134;
    unk28 = 0;
    AiPath* path = player->FUN_00491e10();
    int index = player->unk1a7c;
    AiSegment* cur = path->At(index);
    AiSegment* next = path->At(index + 1);
    AiSegment* prev = path->At(index - 1);

    // Where the car will be in 1.43 seconds (horizontal speed capped at 15).
    Vec3 vel;
    vel.x = car->body->velocity[0];
    vel.y = car->body->velocity[1];
    vel.z = car->body->velocity[2];
    vel.y = 0;
    float speed = (float)sqrt(vel.z * vel.z + vel.x * vel.x);
    if (speed > 15.0f) {
        float k = 15.0f / speed;
        vel.x = k * vel.x;
        vel.z = k * vel.z;
    }
    Vec3 point;
    point = *(Vec3*)car->body->unk14 + vel * 1.43f;
    point.y = 0;

    // Advance while the next segment is closer than the current one.
    int i = 0;
    float dCur;
    float dNext;
    do {
        dCur = FUN_0049d720(&point, cur);
        dNext = FUN_0049d720(&point, next);
        if (dCur > dNext) {
            prev = cur;
            cur = next;
            next = path->At(player->unk1a7c + i + 2);
        }
        i++;
    } while (dCur > dNext);

    AiNode* a = cur->a;
    AiNode* b = cur->b;
    Vec3* u0 = (Vec3*)&cur->unk28[0];
    Vec3* u1 = (Vec3*)&cur->unk28[1];

    // The segment is three pieces: a -> u0, u0 -> u1, u1 -> b; find the nearest point on each.
    Vec3 nearest;
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
    v0.x = u0->x - a->x;
    v0.y = 0;
    v0.z = u0->z - a->z;
    float len0 = (float)sqrt(v0.z * v0.z + v0.x * v0.x);
    FUN_004b3dd0(&point, (Vec3*)a, &v0, &nearest);
    v0 = nearest - point;
    v0.y = 0;
    v1.x = u1->x - u0->x;
    v1.y = 0;
    v1.z = u1->z - u0->z;
    float len1 = (float)sqrt(v1.z * v1.z + v1.x * v1.x);
    FUN_004b3dd0(&point, u0, &v1, &nearest);
    v1 = nearest - point;
    v1.y = 0;
    v2.x = ((Vec3*)b)->x - u1->x;
    v2.y = 0;
    v2.z = ((Vec3*)b)->z - u1->z;
    float len2 = (float)sqrt(v2.z * v2.z + v2.x * v2.x);
    FUN_004b3dd0(&point, u1, &v2, &nearest);
    v2 = nearest - point;
    v2.y = 0;

    // Gap between this segment's end and the next segment's first corner.
    Vec3 gap;
    gap.x = ((Vec3*)&next->unk28[0])->x - ((Vec3*)b)->x;
    gap.y = 0;
    gap.z = ((Vec3*)&next->unk28[0])->z - ((Vec3*)b)->z;
    float lenGap = (float)sqrt(gap.z * gap.z + gap.x * gap.x);
    float total = len2 + len1 + len0;

    float dist0 = v0.Length();
    float dist1 = v1.Length();
    float dist2 = v2.Length();
    if (dist0 < dist1 && dist0 < dist2) {
        // Nearest to the first piece.
        float step0 = 1.25f / len0;
        float step1 = 1.25f / len1;
        v0 = v0 + point;
        v0 -= *(Vec3*)a;
        v0.y = 0;
        Vec3 dir = *u0 - *(Vec3*)a;
        float t = v0.Length() / dir.Length();
        sp.t = t;
        AI_FLOAT(0x2c) = t * len0 / total;
        sp.t2 = t * t;
        sp.t3 = sp.t2 * t;
        AI_FLOAT(0x80) = sp.Eval(prev->unk44, a->unk2c, cur->unk40, cur->unk44);
        unk140.x = sp.Eval(prev->unk28[1].x, ((Vec3*)a)->x, u0->x, u1->x);
        unk140.z = sp.Eval(prev->unk28[1].z, ((Vec3*)a)->z, u0->z, u1->z);
        float t2 = t + step0;
        if (t2 > 1.0f) {
            t2 = (t2 - 1.0f) * len0 * step1;
            sp.t = t2;
            sp.t2 = t2 * t2;
            sp.t3 = sp.t2 * t2;
            unk14c.x = sp.Eval(((Vec3*)a)->x, u0->x, u1->x, ((Vec3*)b)->x);
            unk14c.z = sp.Eval(((Vec3*)a)->z, u0->z, u1->z, ((Vec3*)b)->z);
        } else {
            sp.t = t2;
            sp.t2 = t2 * t2;
            sp.t3 = sp.t2 * t2;
            unk14c.x = sp.Eval(prev->unk28[1].x, ((Vec3*)a)->x, u0->x, u1->x);
            unk14c.z = sp.Eval(prev->unk28[1].z, ((Vec3*)a)->z, u0->z, u1->z);
        }
    } else if (dist2 > dist1) {
        // Nearest to the middle piece.
        float step1 = 1.25f / len1;
        float step2 = 1.25f / len2;
        v1 = v1 + point;
        v1 -= *u0;
        v1.y = 0;
        Vec3 dir = *u1 - *u0;
        float t = v1.Length() / dir.Length();
        sp.t = t;
        AI_FLOAT(0x2c) = (t * len1 + len0) / total;
        sp.t2 = t * t;
        sp.t3 = sp.t2 * t;
        AI_FLOAT(0x80) = sp.Eval(a->unk2c, cur->unk40, cur->unk44, b->unk2c);
        unk140.x = sp.Eval(((Vec3*)a)->x, u0->x, u1->x, ((Vec3*)b)->x);
        unk140.z = sp.Eval(((Vec3*)a)->z, u0->z, u1->z, ((Vec3*)b)->z);
        float t2 = t + step1;
        if (t2 > 1.0f) {
            t2 = (t2 - 1.0f) * len1 * step2;
            sp.t = t2;
            sp.t2 = t2 * t2;
            sp.t3 = sp.t2 * t2;
            unk14c.x = sp.Eval(u0->x, u1->x, ((Vec3*)b)->x, ((Vec3*)&next->unk28[0])->x);
            unk14c.z = sp.Eval(u0->z, u1->z, ((Vec3*)b)->z, ((Vec3*)&next->unk28[0])->z);
        } else {
            sp.t = t2;
            sp.t2 = t2 * t2;
            sp.t3 = sp.t2 * t2;
            unk14c.x = sp.Eval(((Vec3*)a)->x, u0->x, u1->x, ((Vec3*)b)->x);
            unk14c.z = sp.Eval(((Vec3*)a)->z, u0->z, u1->z, ((Vec3*)b)->z);
        }
    } else {
        // Nearest to the last piece.
        float step2 = 1.25f / len2;
        float stepGap = 1.25f / lenGap;
        v2 = v2 + point;
        v2 -= *u1;
        v2.y = 0;
        Vec3 dir = *(Vec3*)b - *u1;
        float t = v2.Length() / dir.Length();
        sp.t = t;
        AI_FLOAT(0x2c) = (t * len2 + len1 + len0) / total;
        sp.t2 = t * t;
        sp.t3 = sp.t2 * t;
        AI_FLOAT(0x80) = sp.Eval(cur->unk40, cur->unk44, b->unk2c, next->unk40);
        unk140.x = sp.Eval(u0->x, u1->x, ((Vec3*)b)->x, ((Vec3*)&next->unk28[0])->x);
        unk140.z = sp.Eval(u0->z, u1->z, ((Vec3*)b)->z, ((Vec3*)&next->unk28[0])->z);
        float t2 = t + step2;
        if (t2 > 1.0f) {
            t2 = (t2 - 1.0f) * len2 * stepGap;
            sp.t = t2;
            sp.t2 = t2 * t2;
            sp.t3 = sp.t2 * t2;
            unk14c.x = sp.Eval(u1->x, ((Vec3*)b)->x, ((Vec3*)&next->unk28[0])->x, next->unk28[1].x);
            unk14c.z = sp.Eval(u1->z, ((Vec3*)b)->z, ((Vec3*)&next->unk28[0])->z, next->unk28[1].z);
        } else {
            sp.t = t2;
            sp.t2 = t2 * t2;
            sp.t3 = sp.t2 * t2;
            unk14c.x = sp.Eval(u0->x, u1->x, ((Vec3*)b)->x, ((Vec3*)&next->unk28[0])->x);
            unk14c.z = sp.Eval(u0->z, u1->z, ((Vec3*)b)->z, ((Vec3*)&next->unk28[0])->z);
        }
        unk14c.y = 0;
    }

    // Blend the node values and take the line direction.
    AI_FLOAT(0x24) = (1.0f - AI_FLOAT(0x2c)) * a->unk24 + b->unk24 * AI_FLOAT(0x2c);
    *(int*)((BYTE*)this + 0xec) = cur->unk14;
    Vec3 ahead;
    ahead = *(Vec3*)&unk14c - *(Vec3*)&unk140;
    unk60.x = ahead.x;
    unk60.y = ahead.y;
    unk60.z = ahead.z;
    unk48.y = 0;
    Vec3 side;
    side.x = unk60.z;
    side.z = -unk60.x;
    side.y = 0;
    float inv = 1.0f / (float)sqrt(side.x * side.x + side.z * side.z);
    side.x = side.x * inv;
    side.z = side.z * inv;
    if (unk114) {
        float width = (1.0f - unk11c) * unk88 + AI_FLOAT(0x80) * unk11c;
        unk48.x = side.x * width + unk140.x;
        unk48.z = side.z * width + unk140.z;
    } else {
        unk48.x = AI_FLOAT(0x80) * side.x + unk140.x;
        unk48.z = AI_FLOAT(0x80) * side.z + unk140.z;
    }

    // Steering offset: the target's side of the car's heading.
    Vec3 toTarget;
    toTarget.x = unk48.x;
    toTarget.y = unk48.y;
    toTarget.z = unk48.z;
    Vec3* pos = (Vec3*)player->car->body->unk14;
    toTarget.x = toTarget.x - pos->x;
    toTarget.y = toTarget.y - pos->y;
    toTarget.z = toTarget.z - pos->z;
    toTarget.y = 0;
    Vec3 perp;
    perp.x = toTarget.z;
    perp.y = 0;
    perp.z = -toTarget.x;
    float inv2 = 1.0f / (float)sqrt(perp.x * perp.x + perp.z * perp.z);
    perp.x = perp.x * inv2;
    perp.z = perp.z * inv2;
    unk34 = perp.y * forward.y + perp.z * forward.z + perp.x * forward.x;

    Vec3 rel;
    rel = *(Vec3*)&unk3c - *pos;
    unkc8.x = rel.x;
    unkc8.y = rel.y;
    unkc8.z = rel.z;
    unkc8.y = 0;
    rel = *(Vec3*)&unk140 - *pos;
    unkd4.x = rel.x;
    unkd4.y = rel.y;
    unkd4.z = rel.z;
    unkd4.y = 0;
    return 0.0f;
}

// FUNCTION: RALLYTROPHY 0x49f1e0
void AIDriver::FUN_0049f1e0()
{
    FUN_0049f560(unk118);
}

struct AiVelocity {
    float Length() const { return (float)sqrt(x * x + y * y + z * z); }
    float x, y, z;
};

// NONMATCHING: RALLYTROPHY 0x49f1f0
// 485 vs 522 bytes, 354 differing bytes; logic complete. The original copies the two velocities into locals in a mixed fld/integer
// pattern (x1 stays in the FPU, x1*x1 and x2*x2 are reused for the squared speeds) that plain struct copies and a
// by-value helper (not inlined, EH frame) did not reproduce.
int AIDriver::FUN_0049f1f0(Player* other)
{
    int result = 0;
    Car* theirs = other->car;
    Vec3 offset = ((CarBodyV*)theirs->body)->position - ((CarBodyV*)player->car->body)->position;
    offset.y = 0;
    float distance = (float)sqrt(offset.z * offset.z + offset.x * offset.x);
    float closeness = 15.0f - distance;
    if (closeness < 0.0f) {
        closeness = 1.0f;
    }
    AiVelocity a = *(AiVelocity*)player->car->body->velocity;
    AiVelocity b = *(AiVelocity*)theirs->body->velocity;
    float speed1 = a.Length();
    float speed2 = b.Length();
    float ratio = speed1 / speed2;
    float q1 = a.x * a.x + a.y * a.y + a.z * a.z;
    float q2 = b.x * b.x + b.y * b.y + b.z * b.z;
    if (q1 * 2.5f > q2 || other->local && distance < 12.0f && q1 * 1.5f > q2) {
        result = 1;
    }
    float factor = 0.0f;
    if (speed1 > 5.0f || (speed1 > 2.0f && unk114 == 1) || other->local) {
        if (ratio >= 1.0f) {
            factor = (ratio * closeness + 1.0f) * 2.0f;
        }
    }
    if (result) {
        factor *= 1.25f;
    }
    unk9c = factor;
    return result;
}

// FUNCTION: RALLYTROPHY 0x49f400
void AIDriver::FUN_0049f400()
{
    float sum = 0.0f;
    float min = 1.0f;
    float max = -1.0f;
    int count = 0;
    Car* car = player->car;
    for (int i = 0; i < 4; i++) {
        CarWheel* wheel = &car->wheels[i];
        if (wheel->touching && wheel->active) {
            float v = car->wheels[i].value;
            count++;
            sum += v;
            if (v > max) {
                max = v;
            }
            if (v < min) {
                min = v;
            }
        }
    }
    if (count == 0) {
        unkb4 = 0;
        return;
    }
    if (max > 0.2f) {
        unkb4 = max - 0.2f;
    } else {
        unkb4 = 0;
    }
    if (unkb4 < 0.0f) {
        unkb4 = 0;
    } else if (unkb4 > 1.0f) {
        unkb4 = 1.0f;
    }
    if (min < -0.2f && !(min < -1.0f)) {
        unkb8 = -(min + 0.2f);
    } else {
        unkb8 = 0;
    }
    if (unkb8 < 0.0f) {
        unkb8 = 0;
    } else if (unkb8 > 1.0f) {
        unkb8 = 1.0f;
    }
}

// FUNCTION: RALLYTROPHY 0x49f560
void AIDriver::FUN_0049f560(float dt)
{
    int active = 0;
    int inRange = 0;
    int surfaced = 0;
    int rough = 0;
    if (!(unk160 < 0.0f)) {
        unk160 -= dt;
    }
    if (unk160 < 0.0f) {
        Car* car = player->car;
        for (int i = 0; i < 4; i++) {
            if (car->wheels[i].active) {
                active++;
                int surface = car->wheelSurface[i];
                if (surface < 5) {
                    inRange++;
                }
                int grip = car->sub->surface[surface];
                if (grip) {
                    surfaced++;
                    if (grip > 1) {
                        rough++;
                    }
                }
            }
        }
        if (surfaced > 0) {
            unk158 = 4;
            unk15c += dt;
        } else if (active == 0 && car->unk142c == 0) {
            unk158 = 1;
            unk15c += dt;
        } else {
            float* v = car->body->velocity;
            if (v[0] * v[0] + v[1] * v[1] + v[2] * v[2] < 1.0f) {
                if (car->unk142c != 0) {
                    unk158 = 2;
                    unk15c += dt;
                } else if (inRange < 4) {
                    unk158 = 3;
                    unk15c += dt;
                }
            } else {
                unk158 = 0;
                unk15c = 0;
                return;
            }
        }
        if (unk158 == 1 && DAT_00501518 * 1000.0f < unk15c
            || unk158 == 2 && DAT_0050151c * 1000.0f < unk15c
            || unk158 == 3 && DAT_00501520 * 1000.0f < unk15c
            || (float)unk158 == 4000.0f && (unk15c > DAT_00501524 || rough != 0 && unk15c > DAT_00501528)) {
            DAT_00501450->Slot13(player);
            unk160 = 1000.0f;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x49f790
// SetSkill: copies the three skill multipliers of a difficulty.
void AIDriver::FUN_0049f790(int stage)
{
    skill[0] = DAT_00500d78[stage][0];
    skill[1] = DAT_00500d78[stage][1];
    skill[2] = DAT_00500d78[stage][2];
}

// FUNCTION: RALLYTROPHY 0x49f7d0
void AIDriver::FUN_0049f7d0()
{
    int difficulty = DAT_0050132c;
    if (unk124 == unk12c) {
        int n = DAT_00500db8[difficulty][0];
        unk12c = (rand() % (2 * n) - n) * 0.01f + 1.0f;
    } else if (unk12c > unk124) {
        unk124 += 0.0001f;
    } else {
        unk124 -= 0.0001f;
    }
    if (unk128 == unk130) {
        int n = DAT_00500db8[difficulty][2];
        unk130 = (rand() % (2 * n) - n) * 0.01f + 1.0f;
    } else if (unk130 > unk128) {
        unk128 += 0.0001f;
    } else {
        unk128 -= 0.0001f;
    }
}

// FUNCTION: RALLYTROPHY 0x49f8e0
// SYMBOL: _$E23
typedef void Generated_49f8e0();

// FUNCTION: RALLYTROPHY 0x49f900
// SYMBOL: _$E22
typedef void Generated_49f900();
