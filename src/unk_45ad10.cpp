// Car update and physics step (0x45ad10-0x45dabf, module end pair 0x45da90/0x45dab0). Map: analysis/range-459890.md section 2.
// Merged from the work groups W3 (steering geometry, timing, SetBrake), W4 (PostUpdate, Update), W5 (PhysicsStep),
// W6 (suspension, wheel contact, brakes) and W7 (Reset / PlaceOnTrack). File-local helpers keep their group prefix.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_477370.h"
#include "unk_478020.h"
#include "unk_479170.h"
#include "unk_47b4c0.h"
#include "unk_46a8a0.h"
#include "unk_46c2c0.h"
#include <math.h>
#include <string.h>
#include <stddef.h>

struct StagePointCopy {
    float x, y, z;
    StagePointCopy(const Vec3& source)
    {
        *(int*)&y = *(const int*)&source.y;
        x = source.x;
        z = source.z;
    }
    ~StagePointCopy() {}
};

// ---- W3 helpers ----
extern float DAT_004e1c28;
extern "C" void fun_004569e0();
// 1 / g; nothing reads it (dead global). The compiler makes two functions of the dynamic initialiser: the init-table
// entry (a jmp, 0x45ad10) and its body (0x45ad20).
static const float W3_kGravity = 9.81f;
static float DAT_00508f5c = 1.0f / W3_kGravity;

// FUNCTION: RALLYTROPHY 0x45ad10
// SYMBOL: _$E19
// Init-table entry of DAT_00508f5c (jmp to the body).
typedef void Generated_45ad10();

// FUNCTION: RALLYTROPHY 0x45ad20
// SYMBOL: _$E18
// Dynamic initialiser body: DAT_00508f5c = 1.0f / 9.81f.
typedef void Generated_45ad20();

// A plain vector (the original's temporaries have no destructor).
struct W3_Vec {
    float x, y, z;
};
W3_Vec* FUN_004203e0(W3_Vec* result, const W3_Vec& a, const W3_Vec& b); // cross product a x b (cdecl; returns its first argument)

// One wheel of the car as UpdateSteering sees it (stride 0x330 from Car+0x28): the orientation rows at +0x20.
struct W3_Wheel {
    BYTE unk0[0x14];
    float position[3];                 // 0x14: relative to the body
    float row0[4];                     // 0x20
    float row1[4];                     // 0x30
    float row2[4];                     // 0x40
    BYTE unk50[0x2a4 - 0x50];
    float damage;                      // 0x2a4: steering (toe) damage
    BYTE unk2a8[0x330 - 0x2a8];
};

struct W3_Spring {
    BYTE unk0[0x24];
    float value;                       // 0x24
    BYTE unk28[0x34 - 0x28];
};

struct W3_Body {
    BYTE unk0[0x1cc];
    float velocity[3];                 // 0x1cc
    BYTE unk1d8[0x1e4 - 0x1d8];
    float force[3];                    // 0x1e4
    float torque[3];                   // 0x1f0
    float extra[3];                    // 0x1fc
    float extra2[3];                   // 0x208
};

struct W3_CarView {
    BYTE unk0[8];
    int tuning;                        // 0x08: index into the per-car tables
    BYTE unkc[0x18 - 0xc];
    float lockLeft;                    // 0x18
    float lockRight;                   // 0x1c
    void* camera;                      // 0x20
    W3_Body* body;                     // 0x24
    W3_Wheel wheel[4];                 // 0x28
    W3_Spring spring[4];               // 0xce8
    BYTE unkdb8[0x10dc - 0xdb8];
    float steerRate;                   // 0x10dc
    float steerSpeed;                  // 0x10e0
    BYTE unk10e4[0x10f4 - 0x10e4];
    float centering;                   // 0x10f4
    BYTE unk10f8[0x1120 - 0x10f8];
    float toeBase;                     // 0x1120
    float toeIn;                       // 0x1124
    float toeOut;                      // 0x1128
    BYTE unk112c[0x1160 - 0x112c];
    float frontRef[5];                 // 0x1160
    float frontIn[5];                  // 0x1174
    float frontOut[5];                 // 0x1188
    BYTE unk119c[0x11e0 - 0x119c];
    int fixedRear;                     // 0x11e0
    float camberBase;                  // 0x11e4
    float camberIn;                    // 0x11e8
    float camberOut;                   // 0x11ec
    BYTE unk11f0[0x1224 - 0x11f0];
    float rearRef[5];                  // 0x1224
    float rearIn[5];                   // 0x1238
    float rearOut[5];                  // 0x124c
    BYTE unk1260[0x1334 - 0x1260];
    int forceBrake;                    // 0x1334: nonzero = brake input forced to full
    BYTE unk1338[0x133c - 0x1338];
    float brake;                       // 0x133c
    BYTE unk1340[0x1344 - 0x1340];
    float steerInput;                  // 0x1344
    float steerPeak;                   // 0x1348
    float steer;                       // 0x134c
    BYTE steerLocked;                  // 0x1350
    BYTE unk1351[0x1390 - 0x1351];
    float lastVelocity[3];             // 0x1390
    BYTE unk139c[0x14f8 - 0x139c];
    int unk14f8;                       // 0x14f8
    BYTE unk14fc[0x1bca8 - 0x14fc];
    int unk1bca8;                      // 0x1bca8
    BYTE unk1bcac[0x1c910 - 0x1bcac];
    int unk1c910;                      // 0x1c910
    BYTE unk1c914[0x1f148 - 0x1c914];
    int clock;                         // 0x1f148
    int dtMs;                          // 0x1f14c
    float dtSec;                       // 0x1f150
    int prevClock;                     // 0x1f154

    // Front toe: the base angle plus the deflection of a front spring from its reference height.
    float Toe(float base, float spring)
    {
        float ref = frontRef[tuning];
        if (spring < ref) {
            return (ref - spring) * frontIn[tuning] * toeIn + base;
        }
        return (spring - ref) * frontOut[tuning] * toeOut + base;
    }
    // Rear camber, same shape with the rear tables.
    float Camber(float base, float spring)
    {
        volatile float ref = rearRef[tuning];
        if (spring < ref) {
            return (ref - spring) * rearIn[tuning] * camberIn + base;
        }
        return (spring - ref) * rearOut[tuning] * camberOut + base;
    }
};

// The camera object handed to BeginTiming.
struct W3_Camera {
    void Attach();                     // 0x4569e0
};

// Cross product *cresult = *ca x *cb as a hand-written x87 block (the original's inline assembler; the pointers live in
// stack locals).
#define W3_CROSS() \
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


// ---- W4 helpers ----
#include <math.h>

// A plain vector whose operator- is the out-of-line 0x414330 (no destructor, so no EH frame).
struct W4_Vec3 {
    W4_Vec3() {}
    W4_Vec3(const W4_Vec3& o) { x = o.x; y = o.y; z = o.z; }
    void SetBits(const W4_Vec3& o)
    {
        *(int*)&x = *(const int*)&o.x;
        *(int*)&y = *(const int*)&o.y;
        *(int*)&z = *(const int*)&o.z;
    }
    void Set(const W4_Vec3& o)
    {
        x = o.x;
        y = o.y;
        z = o.z;
    }
    void operator*=(float k)
    {
        x *= k;
        y *= k;
        z *= k;
    }
    void operator-=(const W4_Vec3& o)
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
    }
    float x, y, z;
    float LengthSquared() const { return x * x + y * y + z * z; }
    W4_Vec3 operator-(const W4_Vec3& o) const; // 0x414330
};

// The camera object at Car+0x20.
class W4_Camera {
public:
    void Update(float dt);             // 0x456820
    void Tick(float dt);               // 0x456b10
};

// The recorder at Car+0x1bed0.
class W4_Recorder {
public:
    void Push(Car* car, int clock);    // 0x4013c0
};

// The car body (Car+0x24) as these two functions see it.
struct W4_Body {
    BYTE unk0[0x14];
    W4_Vec3 pos;                       // 0x14
    float m[3][4];                     // 0x20: rotation rows
    BYTE unk50[0x1cc - 0x50];
    W4_Vec3 velocity;                  // 0x1cc
    BYTE unk1d8[0x240 - 0x1d8];
    int unk240;                        // 0x240
    BYTE unk244[0x268 - 0x244];
    int unk268;                        // 0x268
};

// The shadow caster at Car+0x1f18.
struct W4_Shadow {
    BYTE unk0[0x14];
    W4_Vec3 pos;                      // 0x14
};

// The wheel (stride 0x330) as the post-update sees it.
struct W4_Wheel {
    BYTE unk0[0x330];
    void UpdateSpinMatrix(float dt);   // 0x46c550
};

struct W4_Player {
    BYTE unk0[0x38];
    int local;                         // 0x38
    int unk3c;
    int state;                         // 0x40
};

struct W4_CarView {
    BYTE unk0[0x20];
    W4_Camera* camera;                 // 0x20
    W4_Body* body;                     // 0x24
    W4_Wheel wheel[4];                 // 0x28
    BYTE unkce8[0x1090 - 0xce8];
    W4_Vec3 shadowOffset;              // 0x1090
    BYTE unk109c[0x134c - 0x109c];
    float steer;                       // 0x134c
    BYTE unk1350[4];
    float steerHist[8];                // 0x1354
    int steerIndex;                    // 0x1374
    int steerCount;                    // 0x1378
    float dtSec;                       // 0x137c
    float invDt;                       // 0x1380
    W4_Vec3 accel;                     // 0x1384
    W4_Vec3 prevVel;                   // 0x1390
    W4_Vec3 vel;                       // 0x139c
    BYTE unk13a8[0x1414 - 0x13a8];
    float timer1414;                   // 0x1414
    float timer1418;                   // 0x1418
    float timer141c;                   // 0x141c
    BYTE unk1420[0x14fc - 0x1420];
    Stage* stage;                      // 0x14fc
    BYTE unk1500[0x1eb8 - 0x1500];
    int damaged;                       // 0x1eb8
    BYTE unk1ebc[4];
    int slot;                          // 0x1ec0
    BYTE unk1ec4[0x1f18 - 0x1ec4];
    W4_Shadow* shadow;                 // 0x1f18
    BYTE unk1f1c[0x1bca8 - 0x1f1c];
    int unk1bca8;                      // 0x1bca8
    BYTE unk1bcac[0x1becc - 0x1bcac];
    W4_Player* player;                 // 0x1becc
    W4_Recorder recorder;              // 0x1bed0
    BYTE unk1bed1[0x1befc - 0x1bed1];
    int replayMode;                    // 0x1befc
    BYTE unk1bf00[0x1c74c - 0x1bf00];
    float damageBody;                  // 0x1c74c
    float damageZone2;                 // 0x1c750
    float damageZone3;                 // 0x1c754
    float damageZone1;                 // 0x1c758
    BYTE unk1c75c[0x1c900 - 0x1c75c];
    BYTE latch[8];                     // 0x1c900
    int messageId;                     // 0x1c908
    int messageTime;                   // 0x1c90c
    BYTE unk1c910[0x1f114 - 0x1c910];
    int contactCount;                  // 0x1f114
    BYTE unk1f118[0x1f120 - 0x1f118];
    int stageSlice;                    // 0x1f120
    int ring[4];                       // 0x1f124
    int ringIndex;                     // 0x1f134
    int ringMonotonic;                 // 0x1f138
    float maxSpeed2;                   // 0x1f13c
    float stageDistance;               // 0x1f140
    BYTE unk1f144[4];
    int clock;                         // 0x1f148
    int dtMs;                          // 0x1f14c
    float dtClock;                     // 0x1f150
    int prevClock;                     // 0x1f154
    BYTE noVisual;                     // 0x1f158
};

extern int DAT_0050145c;

// ---- W5 helpers ----
#include <stddef.h>

// The two scale factors the tyre code reads (src/unk_46c2c0.cpp): PhysicsStep is their only writer.
float DAT_00509418;                    // 0x509418: Car+0x1380
float DAT_0050941c;                    // 0x50941c: Car+0x137c

// x87 transform of the vector *point by the 3x3 part of *matrix (row vectors) plus *trans, written to *result. The
// original is an inline function with an assembler body: its argument temporaries live in stack slots.
inline void W5_Xform(const float* point, const float* matrix, float* result, const float* trans)
{
    __asm {
        mov ecx, point
        mov edx, matrix
        mov eax, result
        mov ebx, trans
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+16]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+20]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+24]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+32]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+36]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+40]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fadd dword ptr [ebx]
        fxch st(1)
        fadd dword ptr [ebx+4]
        fxch st(2)
        fadd dword ptr [ebx+8]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax+8]
        fstp dword ptr [eax+4]
    }
}

inline float W5_PassFloat(float v)
{
    return v;
}

struct W5_Mat {
    float m[16];
};

// The car body (Car+0x24) as PhysicsStep sees it; the methods are the rigid body integrator.
struct W5_Body {
    BYTE unk0[0x14];
    float position[3];                 // 0x14
    float rot[16];                     // 0x20: rows 0x10 apart
    BYTE unk60[0x12c - 0x60];
    float mass;                        // 0x12c
    BYTE unk130[0x14c - 0x130];
    int in14c[4];                      // 0x14c
    BYTE unk15c[0x1cc - 0x15c];
    float velocity[3];                 // 0x1cc
    BYTE unk1d8[0x1e4 - 0x1d8];
    float force[3];                    // 0x1e4
    float torque[3];                   // 0x1f0
    BYTE unk1fc[0x214 - 0x1fc];
    float dt;                          // 0x214
    BYTE unk218[0x224 - 0x218];
    float centre[3];                   // 0x224
    void FUN_0041c010();               // 0x41c010: integrate / clear forces
    void FUN_0041c540();               // 0x41c540
    void FUN_0041c390();               // 0x41c390
    void FUN_0041c220(float a, float b); // 0x41c220
    int FUN_0041ca70(void* query, int flag); // 0x41ca70: collide with the stage
    void FUN_0041ef60(void* car, void* stage, int flag); // 0x41ef60
};

// The Car as PhysicsStep sees it (view; the real class is Car in unk_481390.h).
struct W5_Car {
    BYTE unk0[0x24];
    W5_Body* body;                     // 0x24
    Wheel wheel[4];                    // 0x28
    BYTE unkce8[0xdb8 - 0xce8];
    Drivetrain drive;                  // 0xdb8
    BYTE unkfa0[0x1050 - 0xdb8 - 0x1e8];
    Vec3 aero;                         // 0x1050: per body axis drag coefficients
    Vec3 downPoint;                    // 0x105c: application point of the downforce
    float stepA;                       // 0x1068
    float stepB;                       // 0x106c
    Vec3 dragPoint;                    // 0x1070: application point of the drag
    float dragScale;                   // 0x107c
    BYTE unk1080[0x1338 - 0x1080];
    float throttle;                    // 0x1338
    BYTE unk133c[0x137c - 0x133c];
    float dt;                          // 0x137c
    float rate;                        // 0x1380
    BYTE unk1384[0x142c - 0x1384];
    int airborne;                      // 0x142c: result of the last collision query
    BYTE unk1430[0x1464 - 0x1430];
    BYTE query[0x1498 - 0x1464];       // 0x1464: ground query object
    int saved[4];                      // 0x1498: copy of body+0x14c
    W5_Mat savedMat;                   // 0x14a8: copy of the body matrix
    float savedPos[3];                 // 0x14e8: copy of the body position
    int unk14f4;                       // 0x14f4
    BYTE unk14f8[4];
    void* stage;                       // 0x14fc
    BYTE unk1500[0x1ec4 - 0x1500];
    int driveType;                     // 0x1ec4
    BYTE unk1ec8[0x1c74c - 0x1ec8];
    float damage;                      // 0x1c74c: body damage (scales the drag)
};



// ---- W6 helpers ----
// x87 transforms of a vector by the body matrix, as in the original (hand-written assembler, operands in
// stack locals): same macros as the Wheel file.
#define W6_XFORM_ADD() \
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
#define W6_XFORM() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
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
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+4] \
    __asm fstp dword ptr [eax+8]
#define W6_XFORM_ROW() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
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

// One suspension record (Car+0xce8 + i*0x34).
struct W6_Spring {
    float anchor[3];                   // 0x00
    float k;                           // 0x0c
    float dampBump;                    // 0x10
    float dampRebound;                 // 0x14
    float rest;                        // 0x18
    float minLen;                      // 0x1c
    float rideOffset;                  // 0x20
    float compress;                    // 0x24
    float vel;                         // 0x28
    float limit;                       // 0x2c
    float endK;                        // 0x30
    // Spring force of the record (shared by both suspension passes).
    float Force()
    {
        float f = (rest - compress) * k;
        f += ((*(unsigned int*)&vel & 0x80000000) ? dampRebound : dampBump) * vel;
        if (*(int*)&compress < *(int*)&limit) {
            f += (limit - compress) * endK;
        }
        return f;
    }
};

// The wheel as the contact code sees it (Wheel is 0x330 bytes).
struct W6_Wheel {
    BYTE unk0[0x18];
    float prevLen;                     // 0x18 (previous ride length)
    BYTE unk1c[0x30 - 0x1c];
    float up[3];                       // 0x30
    BYTE unk3c[0x124 - 0x3c];
    float pos[3];                      // 0x124
    BYTE unk130[0x260 - 0x130];
    float compress;                    // 0x260
    BYTE unk264[0x280 - 0x264];
    float brakeTorque;                 // 0x280
    BYTE unk284[0x298 - 0x284];
    int contact;                       // 0x298
    BYTE unk29c[0x2a0 - 0x29c];
    float brakeDamage;                 // 0x2a0
    BYTE unk2a4[0x2ac - 0x2a4];
    float load;                        // 0x2ac
    void* surface;                     // 0x2b0
    float normal[3];                   // 0x2b4
    int params[2];                     // 0x2c0
    BYTE unk2c8[0x328 - 0x2c8];
    float grip;                        // 0x328
    BYTE unk32c[4];
};

// The body at Car+0x24 (the part the suspension reads; the rest is WheelBody).
struct W6_Body : public WheelBody {
    BYTE unk22c[0x230 - 0x22c];
    float depth;                       // 0x230
};

// The Stage at Car+0x14fc: per surface id tables.
struct W6_Stage {
    BYTE unk0[0x24c];
    int surfaceType[80];               // 0x24c
    int pair[30][2];                   // 0x38c
    float grip[30];                    // 0x47c
};

// The terrain query at Car+0x1464 (object of the stage collision code; only the two entry points the wheels use).
struct W6_Query {
    float Trace(float radius, const void* origin, const void* dir, int a, int b); // 0x418b90 (thiscall)
    int Surface(float radius, const void* point, void* normal, void* depth, void* hit, int* surface); // 0x419be0
};

struct W6_Car {
    int setup[3];                      // 0x00 (setup[2] = 0x08)
    BYTE unkc[8];
    float brakeBalance;                // 0x14
    BYTE unk18[0x24 - 0x18];
    W6_Body* body;                     // 0x24
    W6_Wheel wheel[4];                 // 0x28 (stride 0x330)
    W6_Spring spring[4];               // 0xce8
    BYTE unkdb8[0x10f8 - 0xdb8];
    float wheelRadius;                 // 0x10f8
    float rideHeight;                  // 0x10fc
    float loadScale;                   // 0x1100
    float invMass;                     // 0x1104
    BYTE unk1108[0x1114 - 0x1108];
    float brakeFront;                  // 0x1114
    float brakeRear;                   // 0x1118
    BYTE unk111c[0x119c - 0x111c];
    float rateFront[5];                // 0x119c
    BYTE unk11b0[0x1260 - 0x11b0];
    float rateRear[5];                 // 0x1260
    BYTE unk1274[0x133c - 0x1274];
    float brakeFrontIn;                // 0x133c
    float brakeRearIn;                 // 0x1340
    BYTE unk1344[0x137c - 0x1344];
    float dt;                          // 0x137c
    float invDt;                       // 0x1380
    BYTE unk1384[0x1464 - 0x1384];
    W6_Query query;                    // 0x1464 (empty class, 1 byte)
    BYTE unk1465[0x14fc - 0x1465];
    W6_Stage* stage;                   // 0x14fc
    BYTE unk1500[0x1680 - 0x1500];
    BYTE tyreMat[30][0x40];            // 0x1680
    BYTE unk1e00[4];
    float hitNormal[4][3];             // 0x1e04
    float lastLoad[4];                 // 0x1e34
    int wheelSurface[4];               // 0x1e44
    float hitPoint[4][3];              // 0x1e54
    float hitDepth[4][3];              // 0x1e84
};
typedef char W6_AssertWheel[offsetof(W6_Car, wheel) == 0x28 ? 1 : -1];
typedef char W6_AssertSpring[offsetof(W6_Car, spring) == 0xce8 ? 1 : -1];
typedef char W6_AssertStage[offsetof(W6_Car, stage) == 0x14fc ? 1 : -1];
typedef char W6_AssertQuery[offsetof(W6_Car, query) == 0x1464 ? 1 : -1];
typedef char W6_AssertHitDepth[offsetof(W6_Car, hitDepth) == 0x1e84 ? 1 : -1];
typedef char W6_AssertWheelSize[sizeof(W6_Wheel) == 0x330 ? 1 : -1];
typedef char W6_AssertSpringSize[sizeof(W6_Spring) == 0x34 ? 1 : -1];

static inline float W6_PassFloat(float v)
{
    return v;
}

// dst += v written as v + dst (the original's operand order).
static inline void W6_AddTo(float* dst, const Vec3& v)
{
    dst[0] = v.x + dst[0];
    dst[1] = v.y + dst[1];
    dst[2] = v.z + dst[2];
}

static inline float W6_Sub(float a, float b)
{
    return a - b;
}


// ---- W7 helpers ----
// ---- group W7: file-local views of the Car and the objects Reset touches ----

// An embedded particle emitter (WheelSpray, CarEmitterB, CarParticles, CarEmitterC, ExhaustSmoke); only its
// virtual slot 9 (Reset) is called.
class W7_Emitter {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void Reset();              // slot 9
};
template <int N>
class W7_EmitterN : public W7_Emitter {
    BYTE pad[N - 4];
};

class W7_Wheel {
public:
    void Reset();                      // 0x46c6b0
    BYTE unk0[0x14];
    float posX, posY, posZ;            // 0x14
    BYTE unk20[0x124 - 0x20];
    float localX, localY, localZ;      // 0x124
    BYTE unk130[0x260 - 0x130];
    float distance;                    // 0x260
    BYTE unk264[0x27c - 0x264];
    int z27c, z280, z284, z288, z28c;  // 0x27c: cleared
    BYTE unk290[0x298 - 0x290];
    int contact;                       // 0x298
    BYTE unk29c[0x2b0 - 0x29c];
    BYTE* surface;                     // 0x2b0
    float normalX, normalY, normalZ;   // 0x2b4
    float grip0, grip1;                // 0x2c0
    BYTE unk2c8[0x328 - 0x2c8];
    float f328;                        // 0x328
    BYTE unk32c[0x330 - 0x32c];
};

struct W7_Spring {                     // 0x34 bytes at Car+0xce8
    BYTE unk0[0x1c];
    float top;                         // 0x1c
    float rest;                        // 0x20
    float lower;                       // 0x24
    float lowerRate;                   // 0x28
    BYTE unk2c[0x34 - 0x2c];
};

class W7_Drive {
public:
    void ResetState();                 // 0x46ae30
};
class W7_Follower {
public:
    void Reset();                      // 0x46d7f0
    BYTE data[0x1c];
};
class W7_Skid {
public:
    void Expire();                     // 0x474600
    BYTE data[0x6558];
};
class W7_Queue {
public:
    void Clear();                      // 0x40b560
    BYTE data[0xa8];
};

struct W7_Body {
    BYTE unk0[0x14];
    Vec3 pos;                          // 0x14
    Vec3 right;                        // 0x20: rotation rows 0x20 / 0x30 / 0x40, translation row 0x50
    float w0;
    Vec3 up;                           // 0x30
    float w1;
    Vec3 fwd;                          // 0x40
    float w2;
    float trans[4];                    // 0x50
    BYTE unk60[0xc4 - 0x60];
    float margin;                      // 0xc4
    BYTE unkc8[0x230 - 0xc8];
    float f230;                        // 0x230
    void ClearForces();                // 0x41be00
};

struct W7_Sphere {
    Vec3 center;
    float radius;
};

// The terrain query object at Car+0x1430.
class W7_Query {
public:
    void Erase(void* first, void* last);                                        // 0x484380
    float Cast(float r, Vec3* from, Vec3* dir, int a, int b);                   // 0x418b90
    int Probe(float r, Vec3* at, Vec3* normal, Vec3* p, Vec3* q, int* surface); // 0x419be0
    int unk0;
    void* first;                       // 0x04
    void* last;                        // 0x08
    BYTE unkc[0x14 - 0xc];
    float minX, minY, minZ;            // 0x14
    float maxX, maxY, maxZ;            // 0x20
    BYTE unk2c[4];
    int mode;                          // 0x30
};

struct W7_Pair {
    float a, b;
};

inline float W7_PassFloat(float v)
{
    return v;
}

struct W7_V3 {
    float x, y, z;
};

struct W7_Stage {
    BYTE unk0[0x24c];
    int surfaceType[1];                // 0x24c
    BYTE unk250[0x38c - 0x250];
    W7_Pair pair[1];                   // 0x38c
    BYTE unk394[0x47c - 0x394];
    float value[1];                    // 0x47c
    void Collect(W7_Query* query, W7_Sphere* sphere); // 0x4114c0
};

// Transforms *point by the rotation *matrix (rows 0x10 apart), adds *trans and stores the result in *result.
#define W7_TRANSFORM_ADD() \
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
// The same with the translation row of the matrix (matrix+0x30) instead of *trans.
#define W7_TRANSFORM() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
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
    __asm fadd dword ptr [edx+0x30] \
    __asm fxch st(1) \
    __asm fadd dword ptr [edx+0x34] \
    __asm fxch st(2) \
    __asm fadd dword ptr [edx+0x38] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// out = a x b of the three-float vectors (inline assembler in the original: the operands are loaded from the stack).
#define W7_CROSS() \
    __asm mov ecx, a \
    __asm mov edx, b \
    __asm mov eax, dst \
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

inline void W7_Normalize(float* p)
{
    float inv = 1.0f / (float)sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
    p[0] *= inv;
    p[1] *= inv;
    p[2] *= inv;
}

// Non-folded constants (loaded from memory in the original: .rdata 0x4e1c18 / 0x4e1c1c / 0x4e1c20).
static const float W7_kTyreScale = 0.45f;
static const float W7_kOne = 1.0f;
static const float W7_kRay = 15.0f;

struct W7_Block {                      // 0x40 bytes: Car+0x1680 is an array of tyre matrices
    BYTE data[0x40];
};

struct W7_Car {
    BYTE unk0[8];
    int setup2;                        // 0x08
    BYTE unkc[0x24 - 0xc];
    W7_Body* body;                     // 0x24
    W7_Wheel wheel[4];                 // 0x28
    W7_Spring spring[4];               // 0xce8
    W7_Drive drivetrain;               // 0xdb8
    BYTE unkdb8[0x10f8 - 0xdb8 - sizeof(W7_Drive)];
    float tyreRadius;                  // 0x10f8
    float rideHeight;                  // 0x10fc
    BYTE unk1100[0x1160 - 0x1100];
    float frontOffset[49];             // 0x1160
    float rearOffset[1];               // 0x1224
    BYTE unk1228[0x1334 - 0x1228];
    int z1334, z1338, z133c, z1340, z1344, z1348, z134c; // 0x1334
    BYTE unk1350[4];
    int z1354[8];                      // 0x1354
    int z1374, z1378;                  // 0x1374
    float step;                        // 0x137c
    float stepMax;                     // 0x1380
    float v1384[3];                    // 0x1384
    BYTE unk1390[0x13d8 - 0x1390];
    float v13d8[3], v13e4[3], v13f0[3], v13fc[3], v1408[3]; // 0x13d8
    float k1414[3];                    // 0x1414
    int z1420, z1424, z1428, z142c;    // 0x1420
    W7_Query query;                    // 0x1430
    BYTE unk1464[0x14f4 - 0x1464];
    int z14f4;                         // 0x14f4
    BYTE unk14f8[4];
    W7_Stage* stage;                   // 0x14fc
    BYTE unk1500[0x1e04 - 0x1500];
    W7_V3 hitNormal[4];                // 0x1e04
    int z1e34[4];                      // 0x1e34
    int wheelSurface[4];               // 0x1e44
    Vec3 hitA[4];                      // 0x1e54
    Vec3 hitB[4];                      // 0x1e84
    BYTE unk1eb4[0x1f30 - 0x1eb4];
    W7_Follower follower0;             // 0x1f30
    W7_Follower follower1;             // 0x1f4c
    BYTE unk1f68[0x2098 - 0x1f68];
    float mix0, mix1;                  // 0x2098
    int z20a0, z20a4;                  // 0x20a0
    int z20a8;                         // 0x20a8
    float z20ac[3];                    // 0x20ac
    float z20b8[3];                    // 0x20b8
    BYTE unk20c4[0x2100 - 0x20c4];
    W7_EmitterN<0x12c> emitter0;       // 0x2100
    W7_EmitterN<0x12c> emitter1;       // 0x222c
    W7_EmitterN<0x130> emitter2;       // 0x2358
    W7_EmitterN<0x160> emitter3;       // 0x2488
    W7_EmitterN<0x160> emitter4;       // 0x25e8
    W7_Skid skid[4];                   // 0x2748
    BYTE unk1bca8[0x1c910 - 0x1bca8];
    int z1c910;                        // 0x1c910
    BYTE unk1c914[0x1f114 - 0x1c914];
    int z1f114, z1f118, z1f11c;        // 0x1f114
    BYTE unk1f120[4];
    int z1f124[4];                     // 0x1f124
    int z1f134, z1f138;                // 0x1f134
    BYTE unk1f13c[0x1f15c - 0x1f13c];
    int z1f15c;                        // 0x1f15c
    W7_Queue queue;                    // 0x1f160
    int ring[16];                      // 0x1f208
    int z1f248;                        // 0x1f248
};


// ---- functions in address order ----
// NONMATCHING: RALLYTROPHY 0x45ad40
// Best tested form: 1402/1404 bytes, 696 differing bytes. Making Camber's rear reference height volatile improves
// the FPU/stack schedule; volatile Toe reference/base/output values, a volatile one-time reference store, and factoring
// the Toe base after its branch are worse. Other differences remain in the toe
// blocks, wheel-matrix stack slots, and the target's hoisted 1.0f register. Evidence: ev_44efd80de74fa57abbe051afd7c89aabb5cbbfca52bc9ff6c566f2d37ba828ec.
void Car::UpdateSteering(float dt)
{
    W3_CarView* v = (W3_CarView*)this;
    if (!v->steerLocked) {
        float a = (float)fabs(v->steer);
        if (a > v->steerPeak) {
            v->steerPeak = a;
        }
        if ((float)fabs(v->steerInput) < 0.1f) {
            v->steerPeak = 0;
            v->steer = (float)pow(1.0f - v->centering, dt) * v->steer;
        } else {
            float rate = (1.0f - v->steerPeak) * v->steerRate + v->steerSpeed * v->steerPeak;
            if (v->steerInput < 0.0f) {
                rate = -rate;
            }
            float s = rate * dt + v->steer;
            v->steer = s;
            if (s > 1.0f) {
                v->steer = 1.0f;
            } else if (s < -1.0f) {
                v->steer = -1.0f;
            }
        }
    }

    float x = v->Toe(v->toeBase, v->spring[0].value);
    float y = v->Toe(v->toeBase, v->spring[1].value);

    float steer = v->steer;
    float dl = v->wheel[0].damage;
    float dr = v->wheel[1].damage;
    float l0 = (1.0f - dl * 0.5f) * steer - dl * 0.25f;
    float l1 = (1.0f - dr * 0.5f) * steer + dr * 0.25f;
    float a0, a1;
    if (*(unsigned int*)&steer & 0x80000000) {
        a0 = l0 * v->lockRight;
        a1 = l1 * v->lockLeft;
    } else {
        a0 = l0 * v->lockLeft;
        a1 = l1 * v->lockRight;
    }

    W3_Wheel* w = &v->wheel[0];
    float ca0 = (float)cos(a0);
    float sa0 = (float)sin(a0);
    w->row2[0] = sa0;
    w->row2[1] = 0;
    w->row2[2] = ca0;
    float sx = (float)sin(x);
    w->row1[1] = (float)cos(x);
    w->row1[0] = sx * ca0;
    w->row1[2] = -(sx * sa0);
    {
        float* ca = w->row1;
        float* cb = w->row2;
        float* cresult = w->row0;
        W3_CROSS();
    }

    w = &v->wheel[1];
    float ca1 = (float)cos(a1);
    float sa1 = (float)sin(a1);
    w->row2[0] = sa1;
    w->row2[1] = 0;
    w->row2[2] = ca1;
    float sy = (float)sin(-y);
    w->row1[1] = (float)cos(-y);
    w->row1[0] = sy * ca1;
    w->row1[2] = -(sy * sa1);
    {
        float* ca = w->row1;
        float* cb = w->row2;
        float* cresult = w->row0;
        W3_CROSS();
    }

    if (v->fixedRear) {
        float* row2a = v->wheel[2].row0;
        float* row3a = v->wheel[3].row0;
        float ang = (float)atan((v->wheel[2].position[1] - v->wheel[3].position[1]) /
                                (v->wheel[2].position[0] - v->wheel[3].position[0]));
        float p = ang - v->camberBase;
        row2a[0] = (float)cos(p);
        v->wheel[2].row0[1] = (float)sin(p);
        float q = ang + v->camberBase;
        row3a[0] = (float)cos(q);
        v->wheel[3].row0[1] = (float)sin(q);
        W3_Vec tmp;
        W3_Vec* r = FUN_004203e0(&tmp, *(W3_Vec*)v->wheel[2].row2, *(W3_Vec*)row2a);
        v->wheel[2].row1[0] = r->x;
        v->wheel[2].row1[1] = r->y;
        v->wheel[2].row1[2] = r->z;
        r = FUN_004203e0(&tmp, *(W3_Vec*)v->wheel[3].row2, *(W3_Vec*)row3a);
        v->wheel[3].row1[0] = r->x;
        v->wheel[3].row1[1] = r->y;
        v->wheel[3].row1[2] = r->z;
    } else {
        float x2 = v->Camber(v->camberBase, v->spring[3].value);
        float y2 = v->Camber(v->camberBase, v->spring[2].value);
        W3_Wheel* w3 = &v->wheel[3];
        w3->row2[0] = 0;
        w3->row2[1] = 0;
        w3->row2[2] = 1.0f;
        w3->row1[2] = 0;
        w3->row1[1] = (float)cos(x2);
        w3->row1[0] = (float)sin(x2);
        {
            float* ca = w3->row1;
            float* cb = w3->row2;
            float* cresult = w3->row0;
            W3_CROSS();
        }
        W3_Wheel* w2 = &v->wheel[2];
        w2->row1[1] = (float)cos(-y2);
        w2->row2[1] = 0;
        w2->row2[2] = 1.0f;
        w2->row1[2] = 0;
        w2->row2[0] = 0;
        w2->row1[0] = (float)sin(-y2);
        {
            float* ca = w2->row1;
            float* cb = w2->row2;
            float* cresult = w2->row0;
            W3_CROSS();
        }
    }
}

// original: FUN_0045b2c0
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x45b2c0
void Car::FUN_0045b2c0(void* camera, int time, int dt)
{
    __asm {
        mov eax, dword ptr [esp + 8]
        fild dword ptr [esp + 0xc]
        mov edx, dword ptr [esp + 0xc]
        push esi
        fmul dword ptr [DAT_004e1c28]
        mov esi, ecx
        mov ecx, dword ptr [esp + 8]
        mov dword ptr [esi + 0x1f148], eax
        mov dword ptr [esi + 0x1f14c], edx
        fstp dword ptr [esi + 0x1f150]
        mov dword ptr [esi + 0x1f154], eax
        mov eax, dword ptr [esi + 0x24]
        mov edx, dword ptr [eax + 0x1cc]
        mov dword ptr [esi + 0x1390], edx
        mov edx, dword ptr [eax + 0x1d0]
        push edi
        xor edi, edi
        cmp ecx, edi
        mov dword ptr [esi + 0x1394], edx
        mov edx, dword ptr [eax + 0x1d4]
        mov dword ptr [eax + 0x1ec], edi
        mov dword ptr [eax + 0x1e8], edi
        mov dword ptr [eax + 0x1e4], edi
        mov dword ptr [eax + 0x1f8], edi
        mov dword ptr [eax + 0x1f4], edi
        mov dword ptr [eax + 0x1f0], edi
        mov dword ptr [eax + 0x204], edi
        mov dword ptr [eax + 0x200], edi
        mov dword ptr [eax + 0x1fc], edi
        mov dword ptr [esi + 0x20], ecx
        mov dword ptr [esi + 0x1398], edx
        mov dword ptr [eax + 0x210], edi
        mov dword ptr [eax + 0x20c], edi
        mov dword ptr [eax + 0x208], edi
        mov dword ptr [esi + 0x1bca8], edi
        je car_begin_timing_no_camera
        call fun_004569e0
    car_begin_timing_no_camera:
        mov dword ptr [esi + 0x1c910], edi
        mov dword ptr [esi + 0x14f8], edi
        pop edi
        pop esi
        ret 0xc
    }
}

// FUNCTION: RALLYTROPHY 0x45b390
// StagePointCopy preserves the target's integer y-copy before the x87 x-load. The explicit z
// delta store reproduces its stack spill. Evidence: `ev_482eda42dbfbd7e19e133ac34ae46d58a20274d24bb34531c85f16e68882584a`.
void Car::FUN_0045b390()
{
    W4_CarView* v = (W4_CarView*)this;
    if (v->shadow) {
        W4_Vec3 t(v->shadowOffset);
        float y = t.y;
        W4_Body* b = v->body;
        t.x = t.x + y * b->m[0][1];
        t.y = y * b->m[1][1];
        t.z = t.z + y * b->m[2][1];
        v->shadow->pos.Set(t);
    }
    float dtSec = v->dtMs * 0.001f;
    v->accel.Set(v->body->velocity);
    v->accel -= v->prevVel;
    v->accel *= 1.0f / dtSec;
    for (int i = 0; i < 4; i++)
        v->wheel[i].UpdateSpinMatrix(dtSec);
    if (!v->noVisual) {
        if (v->camera)
            v->camera->Tick(dtSec);
        ((WheelSpray*)((BYTE*)this + 0x2100))->Update(dtSec);
        ((CarEmitterB*)((BYTE*)this + 0x222c))->Update(dtSec);
        ((CarParticles*)((BYTE*)this + 0x2358))->Update(dtSec);
        ((CarEmitterC*)((BYTE*)this + 0x2488))->Update(dtSec);
        ((ExhaustSmoke*)((BYTE*)this + 0x25e8))->Update(dtSec);
        if (v->unk1bca8 == 0) {
            v->timer141c = 0.02f;
            if (v->damaged == 0)
                UpdateTyreTracks();
        }
    }
    UpdateLights();
    FUN_00453940(dtSec);
    Stage* stage = v->stage;
    W4_Vec3* position = &v->body->pos;
    Checkpoint* cp = stage->FUN_00414dc0((Vec3*)position, 0);
    if (cp) {
        {
            StagePointCopy d(cp->position);
            d.x -= position->x;
            d.y -= position->y;
            float dz = d.z - position->z;
            *(volatile float*)&d.z = dz;
            if (d.x * d.x + d.y * d.y + dz * dz < 100.0f) {
                v->stageSlice = cp->index;
                v->stageDistance = *(float*)((BYTE*)stage + 0xa00) - cp->distance;
            }
        }
        if (v->player->local && v->replayMode != 2) {
            int id = cp->index;
            int n = v->ringIndex;
            if (id != v->ring[n]) {
                n = (n + 1) & 3;
                v->ringIndex = n;
                v->ring[n] = id;
            }
            int idx = v->ringIndex;
            int cur = v->ring[idx];
            n = idx - 1;
            if (n < 0)
                n = 3;
            v->ringMonotonic = 1;
            for (int i = 0; i < 3; i++) {
                int prev = v->ring[n];
                if (prev <= cur) {
                    v->ringMonotonic = 0;
                    break;
                }
                n--;
                cur = prev;
                if (n < 0)
                    n = 3;
            }
            if (DAT_0050145c) {
                float limit = 0.8f;
                if (DAT_00501454 == 0 && DAT_00501464 != 3) {
                    switch (DAT_0050132c) {
                    case 0:
                        limit = 0.9f;
                        break;
                    case 1:
                        limit = 0.8f;
                        break;
                    case 2:
                        limit = 0.7f;
                        break;
                    }
                }
                if (!v->latch[7]) {
                    if (!v->latch[0] && v->damageBody > limit) {
                        v->latch[0] = 1;
                        v->messageId = 1;
                        v->messageTime = v->clock;
                    }
                    if (!v->latch[1] && v->damageZone1 > limit) {
                        v->latch[1] = 1;
                        v->messageId = 7;
                        v->messageTime = v->clock;
                    }
                    if (!v->latch[2] && v->damageZone2 > limit) {
                        v->latch[2] = 1;
                        v->messageId = 2;
                        v->messageTime = v->clock;
                    }
                    if (!v->latch[3] && v->damageZone3 > limit) {
                        v->latch[3] = 1;
                        v->messageId = 3;
                        v->messageTime = v->clock;
                    }
                    if (!v->latch[4] && GetBrakeDamage() > limit) {
                        v->latch[4] = 1;
                        v->messageId = 5;
                        v->messageTime = v->clock;
                    }
                    if (!v->latch[5] && GetWheelDamage() > limit) {
                        v->latch[5] = 1;
                        v->messageId = 4;
                        v->messageTime = v->clock;
                    }
                    if (!v->latch[6] && GetSteeringDamage() > limit) {
                        v->latch[6] = 1;
                        v->messageId = 6;
                        v->messageTime = v->clock;
                    }
                    if (!v->latch[7] && !DAT_00501454 && !DAT_00501458 && !DAT_00501464
                        && DAT_00501450->m_localPlayers.size() == 1) {
                        int n = 0;
                        if (v->latch[0])
                            n++;
                        if (v->latch[1])
                            n++;
                        if (v->latch[2])
                            n++;
                        if (v->latch[3])
                            n++;
                        if (v->latch[4])
                            n++;
                        if (v->latch[5])
                            n++;
                        if (v->latch[6])
                            n++;
                        if (n > 3) {
                            v->latch[7] = 1;
                            v->messageId = 8;
                            v->messageTime = v->clock;
                        }
                    }
                }
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x45b900
void Car::FUN_0045b900(int dt)
{
    W4_CarView* v = (W4_CarView*)this;
    W4_Body* body = v->body;
    if (v->player->state == 1)
        FUN_004590b0();
    v->dtSec = dt * 0.001f;
    v->invDt = 1000.0f / dt;
    float dtClock = (v->clock - v->prevClock) * 0.001f;
    float dtSec = dt * 0.001f;
    if (v->replayMode)
        v->recorder.Push(this, v->clock);
    v->steerCount -= dt;
    if (v->steerCount <= 0) {
        v->steerCount = 16;
        v->steerHist[v->steerIndex] = v->steer;
        v->steerIndex = (v->steerIndex + 1) & 7;
    }
    W4_Vec3* vel = &v->vel;
    vel->Set(v->body->velocity);
    FUN_00452b20();
    PhysicsStep(dtSec);
    v->vel.SetBits(v->body->velocity - v->vel);
    v->vel *= v->invDt;
    FUN_00452eb0(dtSec);
    if (v->replayMode != 2) {
        int s = v->player->state;
        if (s == 1 || (s == 0 && v->player->local))
            FUN_004689b0(dt);
    }
    v->clock += dt;
    UpdateSteering(dtSec);
    if (v->camera)
        v->camera->Update(dtSec);
    if (!v->noVisual) {
        FUN_00469ea0(dtSec);
        v->timer1418 -= dtSec;
        if (v->timer1418 <= 0) {
            v->timer1418 += 0.02f;
            if (v->slot)
                EmitEffects(0.02f, dtClock, v->dtClock - dtClock);
            body->unk268 = 0;
        }
        v->timer141c -= dtSec;
        if (v->timer141c <= 0) {
            v->timer141c += 0.02f;
            if (v->damaged == 0)
                UpdateTyreTracks();
            v->unk1bca8 = 1;
        }
        v->timer1414 -= dtSec;
        if (v->timer1414 <= 0) {
            v->timer1414 += 0.02f;
            if (v->player->state != 2)
                ProcessContacts();
            v->contactCount = 0;
            body->unk240 = 0;
        }
    }
    if (v->player->local && v->replayMode != 2) {
        W4_Vec3* vel = &v->body->velocity;
        float speed2 = vel->x * vel->x + vel->y * vel->y + vel->z * vel->z;
        if (v->maxSpeed2 < speed2)
            v->maxSpeed2 = speed2;
        if (v->ringMonotonic == 0)
            UpdateSurfaceSound();
        FUN_00468720(dtSec);
    }
}


#define V ((W5_Car*)this)

// NONMATCHING: RALLYTROPHY 0x45bc20
// original: FUN_0045bc20
// Best tested source form: 1946/2002 bytes, 1529 differing bytes outside relocations. A volatile rotp pointer
// improves the target's frame-home shape; direct centre updates reduce the differing bytes further. Volatile posp,
// loop-counter, and radius-local variants are worse. Reordering speed2 to y/z/x emits the target's x/z/y square
// sequence without changing the total mismatch count; hoisting wp2 to match the persistent Vec3 lifetime helps.
// Remaining differences include EH state numbers (target: 3 persistent Vec3 locals), transform argument slots,
// the radius-sum FPU stack, and post-collision centre-of-mass update order. Evidence: ev_c92c59602c67fd58248c61d5b8be6532014a74b970e4fb2582dd1e42d5e74d32.
// One physics step of dt seconds: gravity, aerodynamic drag and downforce, suspension, brakes, drivetrain, tyre
// forces, rigid body integration and the collision with the stage (restoring the pose when the body hit).
void Car::PhysicsStep(float dt)
{
    W5_Body* b = V->body;
    Vec3 gravity(0.0f, -(b->mass * 9.81f), 0.0f);
    DAT_00509418 = W5_PassFloat(V->rate);
    DAT_0050941c = V->dt;
    V->saved[0] = b->in14c[0];
    V->saved[1] = b->in14c[1];
    V->saved[2] = b->in14c[2];
    V->saved[3] = b->in14c[3];
    float* volatile rotp = b->rot;
    Vec3* posp = (Vec3*)b->position;
    V->savedMat = *(W5_Mat*)rotp;
    V->savedPos[0] = b->position[0];
    V->savedPos[1] = b->position[1];
    V->savedPos[2] = b->position[2];
    b->dt = dt;
    Vec3& force = *(Vec3*)b->force;
    Vec3& torque = *(Vec3*)b->torque;
    force += gravity;
    int i;
    {
        int n = 0;
        for (i = 0; i < 4; i++) {
            if (V->wheel[i].active == 0) {
                n++;
            }
        }
        if (n == 4) {
            force += gravity * 0.5f;
        }
    }
    Vec3 vel = *(Vec3*)b->velocity;
    Vec3 wp2;
    float speed2 = vel.y * vel.y + vel.z * vel.z + vel.x * vel.x;
    float e;
    {
        float s = (V->wheel[0].radius + V->wheel[1].radius + V->wheel[2].radius + V->wheel[3].radius) * 0.625f;
        s = s * s;
        e = (float)exp(s * s * -3.0f);
    }
    {
        Vec3 wp;
        W5_Xform((const float*)&V->dragPoint, rotp, (float*)&wp, (const float*)posp);
        Vec3 drag = *(Vec3*)&b->rot[4];
        float k = -(e * V->dragScale * speed2);
        drag.x = drag.x * k;
        drag.y = drag.y * k;
        drag.z = drag.z * k;
        force += drag;
        Vec3 arm(wp.x - b->centre[0], wp.y - b->centre[1], wp.z - b->centre[2]);
        torque += FUN_004203e0(drag, arm);
    }
    if (speed2 > 1.0f) {
        float inv = 1.0f / (float)sqrt(vel.x * vel.x + vel.z * vel.z + vel.y * vel.y);
        vel.x = vel.x * inv;
        vel.y = vel.y * inv;
        vel.z = vel.z * inv;
        float down;
        {
            float d0 = vel.z * b->rot[2] + vel.y * b->rot[1] + vel.x * b->rot[0];
            float d1 = vel.z * b->rot[6] + vel.y * b->rot[5] + vel.x * b->rot[4];
            float d2 = vel.z * b->rot[10] + vel.y * b->rot[9] + vel.x * b->rot[8];
            float sc = V->damage * 0.4f + 1.0f;
            down = (d2 * V->aero.z * d2 + d1 * V->aero.y * d1 + d0 * V->aero.x * d0) * sc * speed2;
        }
        W5_Xform((const float*)&V->downPoint, rotp, (float*)&wp2, (const float*)posp);
        const Vec3& f = vel * -down;
        force += f;
        Vec3 arm2(wp2.x - b->centre[0], wp2.y - b->centre[1], wp2.z - b->centre[2]);
        torque += FUN_004203e0(f, arm2);
    }
    ApplySuspensionForces();
    b->FUN_0041c010();
    SetupBrakes();
    V->drive.SetThrottle(V->throttle);
    V->drive.ComputeEngineTorque();
    V->drive.DistributeDifferential(V->driveType);
    for (i = 0; i < 4; i++) {
        V->wheel[i].UpdateForces();
    }
    b->FUN_0041c010();
    int wasAirborne = V->airborne == 0;
    b->FUN_0041c540();
    FUN_004581f0();
    V->airborne = b->FUN_0041ca70(V->query, 0);
    if (V->airborne) {
        Vec3 delta = *(Vec3*)V->savedPos;
        delta.x -= posp->x;
        delta.y -= posp->y;
        delta.z -= posp->z;
        b->FUN_0041ef60(this, V->stage, 1);
        b->in14c[0] = V->saved[0];
        b->in14c[1] = V->saved[1];
        b->in14c[2] = V->saved[2];
        b->in14c[3] = V->saved[3];
        *posp = *(Vec3*)V->savedPos;
        *(W5_Mat*)rotp = V->savedMat;
        b->FUN_0041c390();
        b->FUN_0041c540();
        b->FUN_0041c220(V->stepA, V->stepB);
        if (wasAirborne) {
            delta.x *= 0.05f;
            delta.y *= 0.05f;
            delta.z *= 0.05f;
            *posp += delta;
            b->centre[0] += delta.x;
            b->centre[1] += delta.y;
            b->centre[2] += delta.z;
        }
    } else {
        b->FUN_0041c220(V->stepA, V->stepB);
    }
    UpdateWheelContact();
    for (i = 0; i < 4; i++) {
        V->wheel[i].IntegrateSpin(dt);
    }
    V->drive.Update(V->driveType, dt);
    V->unk14f4 = 0;
}

#undef V

// NONMATCHING: RALLYTROPHY 0x45c400
// original: FUN_0045c400. Same logic, 739 vs 756 bytes. Open: the original loads the compress pair with
// non-popping fsub (fld A, fld B, fxch, fsub st,st(1)) and keeps four separate pointer temporaries; the
// Vec3 local slots (arm/dead) and the torque add operand order (fld tmp; fadd torque) are not reproduced.
// Anti-roll store order and aggregate initialization each worsened the count by one; a local torque result did too.
// Per wheel: spring force + anti-roll term, applied to the body force and (arm x force) torque.
void Car::ApplySuspensionForces()
{
    W6_Car* c = (W6_Car*)this;
    W6_Body* body = c->body;
    float front = W6_Sub(c->spring[1].compress, c->spring[0].compress) * c->rateFront[c->setup[2]];
    float rear = W6_Sub(c->spring[2].compress, c->spring[3].compress) * c->rateRear[c->setup[2]];
    float arb[4];
    arb[3] = rear;
    arb[0] = front;
    arb[1] = -front;
    arb[2] = -rear;
    float* rp = &c->spring[0].vel;
    for (int i = 0; i < 4; i++, rp += 0x34 / 4) {
        W6_Wheel* w = &c->wheel[i];
        float* r = rp;
        if (w->contact) {
            float f = r[-4] - r[-1];
            f *= r[-7];
            f += (((*(unsigned int*)r) & 0x80000000) ? r[-5] : r[-6]) * r[0];
            if (*(int*)&r[-1] < *(int*)&r[1]) {
                f += (r[1] - r[-1]) * r[2];
            }
            f += arb[i];
            if (*(unsigned int*)&f & 0x80000000) {
                f = 0.0f;
            }
            const float* matrix = body->rotation;
            const float* trans = body->position;
            const float* point = w->up;
            Vec3 dead;
            float* result = (float*)&dead;
            W6_XFORM();
            Vec3 force = *(Vec3*)&body->rotation[4] * f;
            point = r - 10;
            Vec3 world;
            result = (float*)&world;
            W6_XFORM_ADD();
            W6_AddTo(body->force, force);
            Vec3 arm;
            arm.x = world.x - body->centre[0];
            arm.y = world.y - body->centre[1];
            arm.z = world.z - body->centre[2];
            W6_AddTo(body->torque, FUN_004203e0(force, arm));
            float lf = (*(unsigned int*)&f & 0x80000000) ? 0.0f : f;
            c->lastLoad[i] = lf;
            w->load = lf;
        } else {
            c->lastLoad[i] = 0.0f;
            w->load = 0.0f;
        }
    }
}

// NONMATCHING: RALLYTROPHY 0x45c700
// original: FUN_0045c700. Current candidate: 1128/1144 bytes, 868 differing bytes outside relocations. Loading the
// target's 1.0f at 0x4e1c1c through a volatile read reproduces its `fld [constant]; fadd rideTop` order; reading
// rideHeight directly instead of caching rideBase reduces the frame and mismatch count. Constructor initialization
// for local and caching ly-r[-3] as a spring-minimum local were worse. Other open differences: local-y x87 stack use
// and hoisting zero in ECX for the contact flag. Evidence: ev_af5f5586f35ae18eb2ec962f2895369e53df17277d2c787e5bd8b75f92d812f0.
// Per wheel: ground ray cast, suspension integration step (explicit spring/damper/bump stop), surface lookup.
void Car::UpdateWheelContact()
{
    W6_Car* c = (W6_Car*)this;
    float radius = c->wheelRadius * 0.45f;
    float loadForce = c->loadScale * c->body->rotation[5] * 9.81f;
    float* r = &c->spring[0].vel;
    float* wp = &c->wheel[0].pos[2];
    int* surfP = &c->wheelSurface[0];
    float* nz = &c->hitNormal[0][2];
    for (int i = 0; i < 4; i++) {
        float rideTop = r[-2] + c->rideHeight;
        Vec3 local;
        local.x = wp[-2];
        local.y = wp[-1] + radius;
        local.z = wp[0];
        float t = *(volatile float*)0x004e1c1c + rideTop;
        const float* matrix = c->body->rotation;
        const float* point = (const float*)&local;
        Vec3 world;
        float* result = (float*)&world;
        W6_XFORM_ROW();
        Vec3 dir = *(Vec3*)&c->body->rotation[4];
        float k = -t;
        dir.x *= k;
        dir.y *= k;
        dir.z *= k;
        world.x += c->body->position[0];
        world.y += c->body->position[1];
        world.z += c->body->position[2];
        float hFrac = c->query.Trace(radius, &world, &dir, 0, 0);
        float dist = hFrac * t;
        float comp = c->body->depth + (dist + radius - local.y);
        if (comp < 0.0f) {
            comp = 0.0f;
        }
        wp[0x134 / 4] = comp;
        float d;
        if (dist < rideTop) {
            ((int*)wp)[0x16c / 4] = 1;
            d = dist;
        } else {
            ((int*)wp)[0x16c / 4] = 0;
            d = rideTop;
        }
        float x = d - c->rideHeight;
        if (x < 0.0f) {
            x = 0.0f;
        }
        float V = r[0];
        float s = (r[-4] - r[-1]) * r[-7];
        s += (((*(unsigned int*)r) & 0x80000000) ? r[-5] : r[-6]) * r[0];
        if (*(int*)&r[-1] < *(int*)&r[1]) {
            s += (r[1] - r[-1]) * r[2];
        }
        s += loadForce;
        float prev = wp[-0x114 / 4];
        float vNew = V - s * c->dt * c->invMass;
        float pNew = vNew * c->dt + prev;
        float ly = wp[-1];
        float t2 = ly - x;
        float a = (t2 - prev) * c->invDt;
        float vel;
        if (t2 < pNew) {
            if (pNew <= ly - r[-3]) {
                t2 = pNew;
                ((int*)wp)[0x16c / 4] = 0;
                vel = vNew;
            } else {
                ((int*)wp)[0x16c / 4] = 0;
                vel = (float)fabs(vNew) * -0.7f;
                t2 = ly - r[-3];
            }
        } else {
            if (t2 > ly - r[-3]) {
                if (a < 0.0f) {
                    a = 0.0f;
                } else if (a > 10.0f) {
                    a = 10.0f;
                }
                t2 = ly - r[-3];
            }
            if (hFrac == 1.0f) {
                a = 0.0f;
            }
            vel = a;
        }
        r[-1] = ly - t2;
        r[0] = vel;
        wp[-0x114 / 4] = t2;
        if (((int*)wp)[0x16c / 4]) {
            Vec3 tmp = *(Vec3*)&c->body->rotation[4] * dist;
            world.x -= tmp.x;
            world.y -= tmp.y;
            world.z -= tmp.z;
            if (!c->query.Surface(radius + 0.01f, &world, nz - 2, nz + 0x78 / 4 - 2, nz + 0x48 / 4 - 2, surfP)) {
                ((int*)wp)[0x16c / 4] = 0;
            } else {
                W6_Stage* st = c->stage;
                int type = st->surfaceType[*surfP];
                ((int*)wp)[0x188 / 4] = ((int*)nz)[-2];
                ((int*)wp)[0x18c / 4] = ((int*)nz)[-1];
                ((int*)wp)[0x184 / 4] = (int)this + ((type + 0x5a) << 6);
                ((int*)wp)[0x190 / 4] = ((int*)nz)[0];
                wp[0x1fc / 4] = W6_PassFloat(st->grip[*surfP]);
                ((int*)wp)[0x194 / 4] = st->pair[*surfP][0];
                ((int*)wp)[0x198 / 4] = st->pair[*surfP][1];
            }
        }
        surfP++;
        nz += 3;
        wp += 0x330 / 4;
        r += 0x34 / 4;
    }
}

// FUNCTION: RALLYTROPHY 0x45cb80
// original: FUN_0045cb80 (brake torque per wheel from input, bias and brake damage)
void Car::SetupBrakes()
{
    W6_Car* c = (W6_Car*)this;
    float b = c->brakeFrontIn * c->brakeFront * 0.5f;
    float f = b * c->brakeBalance;
    c->wheel[0].brakeTorque = (1.0f - c->wheel[0].brakeDamage * 0.7f) * f;
    c->wheel[1].brakeTorque = (1.0f - c->wheel[1].brakeDamage * 0.7f) * f;
    float r = (1.0f - c->brakeBalance) * b;
    float h = c->brakeRearIn * c->brakeRear * 0.5f;
    c->wheel[2].brakeTorque = (1.0f - c->wheel[2].brakeDamage * 0.7f) * r + h;
    c->wheel[3].brakeTorque = (1.0f - c->wheel[3].brakeDamage * 0.7f) * r + h;
}

// original: FUN_0045cc30 (Car::Reset / PlaceOnTrack: clear the per-race state, place the car at `start` with the
// orientation `world`, drop it onto the terrain and settle the suspension with 100 physics steps)
// NONMATCHING: RALLYTROPHY 0x45cc30
// Current candidate: 3635/3623 bytes, 1947 differing bytes outside relocations (baseline: 3581/3623, 2573 differences).
// A volatile induction pointer to &hitNormal[0].z (passing normalZ-2) aligns the final wheel loop and target frame;
// aliasing volatile hit bytes to the dead `world` argument home reproduces [ebp+index+0xc] and improves the prior
// hit-array form by three bytes. Volatile mode/pointer stores make the query setup follow the target schedule;
// the query bound stores then match the target's minX/maxX/minY/minZ/maxY/maxZ order. Evidence: ev_94c4b49df374c54afa3ccb2d12c5551b367a918fb3e2702ae31379d071864a29.
// Ordered volatile stores for v1408 and moving the step assignments before them help.
// Reversing the direction-component multiply order, with and without the vector copy, was much worse and was reverted.
// Remaining issues include transform operand/Vec3 frame slots, the x87 schedule of dir *= k, and query-box / settle-
// loop register allocation. Evidence: ev_dd5399404fe4d12b8d49a8529f09c470d3a5a926f29c4ce866384c642263bd97;
// hit-slot detail: ev_94c4b49df374c54afa3ccb2d12c5551b367a918fb3e2702ae31379d071864a29.
void Car::FUN_0045cc30(StartPosition* start, void* world)
{
    W7_Car* v = (W7_Car*)this;
    W7_Body* body = v->body;
    int i;
    v->z1c910 = 0;
    v->z1f118 = 0;
    v->z1f11c = 0;
    v->z1f114 = 0;
    v->z1420 = 0;
    v->z1424 = 0;
    v->z1428 = 0;
    v->z1f138 = 0;
    v->z1f134 = 0;
    for (i = 0; i < 4; i++)
        v->z1f124[i] = 0;
    v->emitter0.Reset();
    v->emitter1.Reset();
    v->emitter2.Reset();
    v->emitter3.Reset();
    v->emitter4.Reset();
    v->mix0 = 0.5f;
    v->mix1 = 0.5f;
    v->z20a4 = 0;
    v->z20a0 = 0;
    v->z1f15c = 0;
    v->queue.Clear();
    v->z1f248 = 0;
    for (i = 0; i < 16; i++)
        v->ring[i] = 0;
    v->v1384[0] = v->v1384[1] = v->v1384[2] = 0;
    v->z142c = 0;
    v->z14f4 = 0;
    v->z1334 = 0;
    v->z1338 = 0;
    v->z133c = 0;
    v->z1340 = 0;
    v->z1344 = 0;
    v->z1348 = 0;
    v->z134c = 0;
    v->drivetrain.ResetState();
    v->follower0.Reset();
    v->follower1.Reset();
    v->v13d8[0] = v->v13d8[1] = v->v13d8[2] = 0;
    v->v13e4[0] = v->v13e4[1] = v->v13e4[2] = 0;
    v->v13f0[0] = v->v13f0[1] = v->v13f0[2] = 0;
    v->v13fc[0] = v->v13fc[1] = v->v13fc[2] = 0;
    v->k1414[0] = 0.02f;
    v->k1414[1] = 0.02f;
    v->k1414[2] = 0.02f;
    ((volatile float*)v->v1408)[2] = 0.0f;
    ((volatile float*)v->v1408)[1] = 0.0f;
    ((volatile float*)v->v1408)[0] = 0.0f;
    v->z20ac[0] = v->z20ac[1] = v->z20ac[2] = 0;
    v->z20a8 = 0;
    v->z20b8[0] = v->z20b8[1] = v->z20b8[2] = 0;
    for (i = 0; i < 8; i++)
        v->z1354[i] = 0;
    v->z1374 = v->z1378 = 0;
    for (i = 0; i < 4; i++)
        v->wheel[i].Reset();

    Vec3 pos = *(Vec3*)start;
    W7_Sphere sphere;
    pos += *(Vec3*)((BYTE*)world + 0x10);
    memcpy(&body->right, world, 64);
    body->pos.x = pos.x;
    body->pos.y = pos.y;
    body->pos.z = pos.z;
    body->trans[0] = 0;
    body->trans[1] = 0;
    body->trans[2] = 0;
    Vec3 top(pos);
    Vec3 bottom(pos);
    bottom -= *(Vec3*)((BYTE*)world + 0x10) * W7_kRay;
    sphere.center = (bottom + top) * 0.5f;
    float r = (sphere.center - top).Length() + body->margin;
    sphere.radius = r;
    Vec3 mn;
    Vec3 mx;
    mn.x = sphere.center.x - r;
    mn.y = sphere.center.y - r;
    mn.z = sphere.center.z - r;
    mx.x = sphere.center.x + r;
    mx.y = sphere.center.y + r;
    mx.z = sphere.center.z + r;
    W7_Query* q = &v->query;
    q->Erase(v->query.first, v->query.last);
    *(volatile int*)&q->mode = 0x11;
    *(W7_Sphere* volatile*)((BYTE*)this + 0x145c) = &sphere;
    q->minX = mn.x;
    q->maxX = mx.x;
    q->minZ = mn.z;
    q->minY = mn.y;
    q->maxY = mx.y;
    q->maxZ = mx.z;
    v->stage->Collect(q, &sphere);

    if (body->fwd.z >= 0.999f && body->up.y >= 0.999f && body->right.x >= 0.999f) {
        float best = W7_kRay + W7_kRay;
        float lowDrop = 0.0f;
        float h[4];
        volatile BYTE* hit = (volatile BYTE*)&world;
        for (i = 0; i < 4; i++) {
            Vec3 wp;
            {
                const float* point = &v->wheel[i].localX;
                float* result = (float*)&wp;
                const float* matrix = (const float*)&body->right;
                W7_TRANSFORM();
            }
            wp.x += body->pos.x;
            wp.y += body->pos.y;
            wp.z += body->pos.z;
            Vec3 dir(body->up);
            float k = -W7_kRay;
            dir.x *= k;
            dir.y *= k;
            dir.z *= k;
            float r = q->Cast(v->rideHeight, &wp, &dir, 0, 0);
            if (r < 1.0f) {
                if (r < best) {
                    best = r;
                    float rest = v->spring[i].rest;
                    lowDrop = r * W7_kRay - rest;
                }
                h[i] = r * W7_kRay;
                hit[i] = 1;
                if (i < 2)
                    h[i] -= v->frontOffset[v->setup2];
                else
                    h[i] -= v->rearOffset[v->setup2];
            } else {
                hit[i] = 0;
            }
        }
        if ((hit[0] || hit[1]) && (hit[2] || hit[3])) {
            body->pos -= body->up * lowDrop;
            float front = 0.0f;
            float rear = 0.0f;
            float k = 2.0f;
            if (hit[0]) {
                front = h[0];
                k = 1.0f;
            }
            if (hit[1]) {
                front = h[1] + front;
                k *= 0.5f;
            }
            front = k * front;
            k = 2.0f;
            if (hit[2]) {
                rear = h[2];
                k = 1.0f;
            }
            if (hit[3]) {
                rear += h[3];
                k *= 0.5f;
            }
            rear = rear * k;
            float* a = &body->fwd.x;
            float* b = &body->right.x;
            *a = 0;
            *b = 1.0f;
            body->fwd.y = rear - front;
            body->right.y = 0;
            body->right.z = 0;
            body->fwd.z = v->wheel[0].posZ - v->wheel[2].posZ;
            W7_Normalize(a);
            float* dst = &body->up.x;
            W7_CROSS();
            W7_Normalize(&body->up.x);
        }
    }

    float best = W7_kRay + W7_kRay;
    float off = 0.0f;
    for (i = 0; i < 4; i++) {
        Vec3 wp;
        {
            const float* point = &v->wheel[i].localX;
            const float* matrix = (const float*)&body->right;
            float* result = (float*)&wp;
            W7_TRANSFORM();
        }
        wp.x += body->pos.x;
        wp.y += body->pos.y;
        wp.z += body->pos.z;
        Vec3 dir(body->up);
        float k = -W7_kRay;
        dir.x *= k;
        dir.y *= k;
        dir.z *= k;
        float r = q->Cast(v->rideHeight, &wp, &dir, 0, 0);
        if (r < best) {
            best = r;
            if (i < 2)
                off = r * W7_kRay - v->frontOffset[v->setup2];
            else
                off = r * W7_kRay - v->rearOffset[v->setup2];
        }
    }
    body->pos -= body->up * off;

    float rr = v->tyreRadius * W7_kTyreScale;
    float* volatile normalZ = &v->hitNormal[0].z;
    for (i = 0; i < 4; i++) {
        W7_Wheel* w = &v->wheel[i];
        float a = v->spring[i].rest + v->rideHeight;
        Vec3 wp;
        Vec3 qp(*(Vec3*)&w->localX);
        float len = a + W7_kOne;
        qp.y += rr;
        {
            const float* point = (const float*)&qp;
            const float* matrix = (const float*)&v->body->right;
            float* result = (float*)&wp;
            W7_TRANSFORM();
        }
        wp.x += v->body->pos.x;
        wp.y += v->body->pos.y;
        wp.z += v->body->pos.z;
        Vec3 dir(v->body->up);
        float k = -len;
        dir.x *= k;
        dir.y *= k;
        dir.z *= k;
        float r = q->Cast(rr, &wp, &dir, 0, 0);
        float dist = r * len;
        float base = v->body->f230;
        float c = base + (dist + rr - qp.y);
        if (c < 0.0f)
            c = 0.0f;
        w->distance = c;
        float e;
        if (dist > a) {
            w->contact = 0;
            e = a - v->rideHeight;
        } else {
            w->contact = 1;
            e = dist - v->rideHeight;
            if (e < 0.0f)
                e = 0.0f;
        }
        float f = w->localY - e;
        float g = w->localY - v->spring[i].top;
        if (f > g)
            f = g;
        v->spring[i].lower = w->localY - f;
        v->spring[i].lowerRate = 0;
        normalZ[0] = normalZ[-2] = 0;
        normalZ[-1] = 1.0f;
        w->z27c = 0;
        w->z280 = 0;
        w->z284 = 0;
        w->z288 = 0;
        w->z28c = 0;
        v->wheelSurface[i] = 0;
        v->z1e34[i] = 0;
        v->skid[i].Expire();
        w->posY = f;
        if (w->contact) {
            Vec3 u(w->posX, w->posY, w->posZ);
            u.y -= v->rideHeight - rr;
            {
                const float* trans = (const float*)&v->body->pos;
                const float* matrix = (const float*)&v->body->right;
                const float* point = (const float*)&u;
                float* result = (float*)&wp;
                W7_TRANSFORM_ADD();
            }
            if (!q->Probe(rr + 0.01f, &wp, (Vec3*)(normalZ - 2), &v->hitB[i], &v->hitA[i], &v->wheelSurface[i])) {
                w->contact = 0;
            } else {
                int s = v->stage->surfaceType[v->wheelSurface[i]];
                w->normalX = normalZ[-2];
                w->normalY = normalZ[-1];
                w->normalZ = normalZ[0];
                w->surface = (BYTE*)((W7_Block*)this + s + 0x5a);
                w->f328 = W7_PassFloat(v->stage->value[v->wheelSurface[i]]);
                const W7_Pair* pr = &v->stage->pair[v->wheelSurface[i]];
                w->grip0 = pr->a;
                w->grip1 = pr->b;
            }
        }
        normalZ += 3;
    }
    FUN_004581f0();
    body->ClearForces();
    v->step = 0.01f;
    v->stepMax = 100.0f;
    for (i = 0; i < 100; i++)
        PhysicsStep(0.01f);
    for (i = 0; i < 8; i++)
        v->z1354[i] = 0;
    v->z1374 = v->z1378 = 0;
    body->ClearForces();
    for (i = 0; i < 4; i++)
        v->wheel[i].Reset();
    v->drivetrain.ResetState();
}

// FUNCTION: RALLYTROPHY 0x45da60
void Car::FUN_0045da60(float a)
{
    W3_CarView* v = (W3_CarView*)this;
    v->brake = v->forceBrake ? 1.0f : a;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration. The pair is only emitted
// when the translation unit instantiates the STL statics (the other functions of this file do); with the static
// initialiser above the compiler numbers them _$E27 / _$E26 in this merged file.

// FUNCTION: RALLYTROPHY 0x45da90
// SYMBOL: _$E27
typedef void Generated_45da90();

// FUNCTION: RALLYTROPHY 0x45dab0
// SYMBOL: _$E26
typedef void Generated_45dab0();

