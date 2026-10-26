// Car animation and model loaders (0x452b20-0x4563df, module end stub 0x4563b0). Map: analysis/range-450ee0.md
// section 4. Work groups F (lamp points 0x452b20, interior tilt 0x454d10), G (springs 0x452eb0), H (0x453940 and its
// static thunks), I (LoadDriver 0x455070) and J (wipers 0x455ec0, rain 0x4560a0); file-local helpers carry the
// group prefix.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_45e480.h"
#include "unk_465e90.h"
#include "unk_476490.h"
#include "unk_4766d0.h"
#include "unk_46d830.h"
#include "unk_46d720.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// ---- F helpers ----

// x87 transform of the vector *point by the 3x3 part of *matrix (row vectors) plus *trans, written to *result. The
// original is an inline function with an assembler body: its argument temporaries live in stack slots.
inline void F_Xform(float* result, const float* point, const float* matrix, const float* trans)
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

inline void F_Set(Vec3& v, float x, float y, float z)
{
    v.x = x;
    v.y = y;
    v.z = z;
}
// A plain vector (copied as three dwords).
struct F_Vec {
    float x, y, z;
};
// The body as the lamp-point code sees it: the velocity of a point comes back through a hidden pointer.
struct F_Body {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    float matrix[3];                   // 0x20
    BYTE unk2c[0xa8 - 0x2c];
    float unka8;                       // 0xa8
    BYTE unkac[0xb4 - 0xac];
    float unkb4;                       // 0xb4
    void ToWorld(Vec3* result, const Vec3* point) { F_Xform((float*)result, (const float*)point, matrix, pos); }
    F_Vec* FUN_004204f0(F_Vec* out, const Vec3* point); // 0x4204f0 (the Vec3-returning member, hidden pointer spelled out)
};
// The Car as the lamp-point code sees it.
struct F_CarView {
    BYTE unk0[0x24];
    F_Body* body;                      // 0x24
    BYTE unk28[0x1018 - 0x28];
    Vec3 lamp0;                        // 0x1018
    Vec3 lamp1;                        // 0x1024
    BYTE unk1030[0x13a8 - 0x1030];
    F_Vec point0;                      // 0x13a8
    F_Vec point1;                      // 0x13b4
    F_Vec point2;                      // 0x13c0
    F_Vec point3;                      // 0x13cc
    BYTE unk13d8[0x203c - 0x13d8];
    Vec3 driverA;                      // 0x203c
    BYTE unk2048[0x20d0 - 0x2048];
    Vec3 driverB;                      // 0x20d0
};

// Copies the point velocity of w (returned through the hidden pointer t) into a Car vector member.
#define F_STORE(dst) \
    { F_Vec* r = c->body->FUN_004204f0(&t, &w); (dst).x = r->x; (dst).y = r->y; (dst).z = r->z; }

// NONMATCHING: RALLYTROPHY 0x452b20
// 897 bytes, same size: only instruction scheduling of the FP copies differs (the original loads the call result before p.x).
void Car::FUN_00452b20()
{
    F_CarView* c = (F_CarView*)this;
    Vec3 p;
    Vec3 w;
    F_Set(p, 0.0f, 0.0f, c->body->unkb4);
    F_Vec t;
    c->body->ToWorld(&w, &p);
    F_STORE(c->point1);
    F_Set(p, 0.0f, 0.0f, c->body->unka8);
    c->body->ToWorld(&w, &p);
    F_STORE(c->point0);
    p.x = c->driverB.x;
    p.y = c->driverB.y;
    p.z = c->driverB.z;
    p += c->lamp0;
    c->body->ToWorld(&w, &p);
    F_STORE(c->point2);
    p.x = c->driverA.x;
    p.y = c->driverA.y;
    p.z = c->driverA.z;
    p += c->lamp1;
    c->body->ToWorld(&w, &p);
    F_STORE(c->point3);
}

// ---- G: Car springs (0x452eb0) ----

// The body of a car (Car+0x24) as the spring update reads it.
struct G_Body {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    float mat[16];                     // 0x20: rotation rows 0x20/0x30/0x40, translation 0x50
    BYTE unk60[0x88 - 0x60];
    float unk88;                       // 0x88
    BYTE unk8c[0xb4 - 0x8c];
    float unkb4;                       // 0xb4
    BYTE unkb8[0x1cc - 0xb8];
    Vec3 velocity;                     // 0x1cc
    Vec3 FUN_004204f0(const Vec3& point); // 0x4204f0: velocity of a world point
};

// The Car members this function touches.
struct G_Car {
    BYTE unk0[0x24];
    G_Body* body;                      // 0x24
    BYTE unk28[0x1018 - 0x28];
    Vec3 vec1018;                      // 0x1018
    Vec3 vec1024;                      // 0x1024
    BYTE unk1030[0x12bc - 0x1030];
    Vec3 vec12bc;                      // 0x12bc
    Vec3 vec12c8;                      // 0x12c8
    Vec3 vec12d4;                      // 0x12d4
    Vec3 vec12e0;                      // 0x12e0
    BYTE unk12ec[0x1380 - 0x12ec];
    float gain;                        // 0x1380
    BYTE unk1384[0x139c - 0x1384];
    Vec3 ctrl2;                        // 0x139c
    Vec3 ctrl3;                        // 0x13a8
    Vec3 ctrl4;                        // 0x13b4
    Vec3 ctrl5;                        // 0x13c0
    Vec3 ctrl6;                        // 0x13cc
    Vec3 ctrl7;                        // 0x13d8
    Vec3 ctrl8;                        // 0x13e4
    Vec3 ctrl9;                        // 0x13f0
    Vec3 ctrl10;                       // 0x13fc
    Vec3 ctrl11;                       // 0x1408
    BYTE unk1414[0x1f30 - 0x1414];
    SpringFollower follower0;          // 0x1f30
    SpringFollower follower1;          // 0x1f4c
    BYTE unk1f68[0x203c - 0x1f68];
    Vec3 vec203c;                      // 0x203c
    BYTE unk2048[0x20d0 - 0x2048];
    Vec3 vec20d0;                      // 0x20d0
    BYTE unk20dc[0x1c730 - 0x20dc];
    int flag1c730;                     // 0x1c730
    int flag1c734;                     // 0x1c734
};

// Transforms the vector `in` by the 3x3 rows at `mat` and adds `trans` (the inline matrix multiply of the original).
#define G_TRANSFORM_ADD() \
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
// The same multiply with the translation row inside the matrix (+0x30..0x38).
#define G_TRANSFORM() \
    __asm mov ecx, in \
    __asm mov edx, mat \
    __asm mov eax, out \
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

static inline float G_DotV(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float G_DotC(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline void G_Mul(Vec3& v, float k) { v.x *= k; v.y *= k; v.z *= k; }
static inline void G_Set(Vec3& v, float x, float y, float z) { v.x = x; v.y = y; v.z = z; }
#define V ((G_Car*)this)

// NONMATCHING: RALLYTROPHY 0x452eb0
// 2726 vs 2693 bytes; logic complete (all four spring-point accelerations, the 0.5/0.65/0.35 blends and both
// SpringFollower updates). Open: struct copy of a call result still goes through `mov reg,ebx` (original stores
// straight through ebx), fadd operand order for ctrl += k*vec, register choice (edx/eax/ecx) for the asm pointer
// locals, and the load order of the ctrl9/ctrl3 dot products.
// Body-frame accelerations of the four spring points and the two SpringFollower updates.
void Car::FUN_00452eb0(float dt)
{
    Vec3 p;
    Vec3 world;
    Vec3 v(V->body->velocity);
    float sp2 = v.y * v.y + v.z * v.z + v.x * v.x;
    float factor = 0.0f;
    if (sp2 > 0.01f) {
        if (sp2 > 0.25f) {
            factor = 1.0f;
        } else {
            factor = ((float)sqrt(sp2) - 0.1f) * 2.5f;
        }
    }
    {
        G_Set(p, 0.0f, 0.0f, V->body->unkb4);
        const float* trans = V->body->pos;
        const float* in = (const float*)&p;
        const float* mat = V->body->mat;
        float* out = (float*)&world;
        G_TRANSFORM_ADD();
        V->ctrl4 = V->body->FUN_004204f0(world) - V->ctrl4;
        G_Mul(V->ctrl4, V->gain);
        V->ctrl4 += V->ctrl2;
    }
    {
        G_Set(p, 0.0f, 0.0f, V->body->unk88);
        const float* trans = V->body->pos;
        const float* in = (const float*)&p;
        const float* mat = V->body->mat;
        float* out = (float*)&world;
        G_TRANSFORM_ADD();
        V->ctrl3 = V->body->FUN_004204f0(world) - V->ctrl3;
        G_Mul(V->ctrl3, V->gain);
        V->ctrl3 += V->ctrl2;
    }
    {
        p = V->vec20d0;
        p += V->vec1018;
        const float* trans = V->body->pos;
        const float* in = (const float*)&p;
        const float* mat = V->body->mat;
        float* out = (float*)&world;
        G_TRANSFORM_ADD();
        V->ctrl5 = V->body->FUN_004204f0(world) - V->ctrl5;
        G_Mul(V->ctrl5, V->gain);
        V->ctrl5 += V->ctrl2;
        V->ctrl5.x *= factor;
        V->ctrl5.y *= factor;
        V->ctrl5.z *= factor;
    }
    {
        p = V->vec203c;
        p += V->vec1024;
        const float* trans = V->body->pos;
        const float* in = (const float*)&p;
        const float* mat = V->body->mat;
        float* out = (float*)&world;
        G_TRANSFORM_ADD();
        V->ctrl6 = V->body->FUN_004204f0(world) - V->ctrl6;
        G_Mul(V->ctrl6, V->gain);
        V->ctrl6 += V->ctrl2;
        V->ctrl6.x *= factor;
        V->ctrl6.y *= factor;
        V->ctrl6.z *= factor;
    }
    V->ctrl7.x *= 0.5f;
    V->ctrl7.y *= 0.5f;
    V->ctrl7.z *= 0.5f;
    V->ctrl7 += V->ctrl2 * 0.5f;
    V->ctrl8.x *= 0.5f;
    V->ctrl8.y *= 0.5f;
    V->ctrl8.z *= 0.5f;
    V->ctrl8 += V->ctrl3 * 0.5f;
    V->ctrl9.x *= 0.5f;
    V->ctrl9.y *= 0.5f;
    V->ctrl9.z *= 0.5f;
    V->ctrl9 += V->ctrl4 * 0.5f;
    V->ctrl10.x *= 0.65f;
    V->ctrl10.y *= 0.65f;
    V->ctrl10.z *= 0.65f;
    V->ctrl10 += V->ctrl5 * (1.0f - 0.65f);
    V->ctrl11.x *= 0.65f;
    V->ctrl11.y *= 0.65f;
    V->ctrl11.z *= 0.65f;
    V->ctrl11 += V->ctrl6 * (1.0f - 0.65f);
    if (V->flag1c734) {
        Vec3 a;
        Vec3 b;
        {
            const float* mat = V->body->mat;
            const float* in = (const float*)&V->vec12bc;
            float* out = (float*)&a;
            G_TRANSFORM();
        }
        {
            const float* mat = V->body->mat;
            const float* in = (const float*)&V->vec12c8;
            float* out = (float*)&b;
            G_TRANSFORM();
        }
        float d = G_DotV(b, V->body->velocity);
        float s = d * (float)fabs(d) * 0.001f;
        V->follower1.Update(dt, G_DotC(a, V->ctrl9), a.y - s);
    }
    if (V->flag1c730) {
        Vec3 a;
        Vec3 b;
        {
            const float* mat = V->body->mat;
            const float* in = (const float*)&V->vec12d4;
            float* out = (float*)&a;
            G_TRANSFORM();
        }
        {
            const float* mat = V->body->mat;
            const float* in = (const float*)&V->vec12e0;
            float* out = (float*)&b;
            G_TRANSFORM();
        }
        float d = G_DotV(b, V->body->velocity);
        float s = d * (float)fabs(d) * 0.001f;
        V->follower0.Update(dt, G_DotC(a, V->ctrl3), a.y + s);
    }
}
#undef V

// ---- H: Car::FUN_00453940 (wipers, hood/trunk flaps, driver, lamps) ----

// A 4x4 matrix with an empty constructor and destructor (EH state per local).
struct H_Matrix {
    H_Matrix() {}
    ~H_Matrix() {}
    void SetIdentity()
    {
        m[1] = m[2] = m[3] = 0.0f;
        m[4] = m[6] = m[7] = 0.0f;
        m[8] = m[9] = m[11] = 0.0f;
        m[12] = m[13] = m[14] = 0.0f;
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
    void FromAxisAngle(const Vec3* axis, float angle); // 0x4b5760
    void Concat(const H_Matrix* other);                // 0x4b55e0 (thiscall, ret 4)
    void Multiply(const H_Matrix* a, const H_Matrix* b); // 0x4b5540: this = a * b
    float m[16];
};

// A plain vector (no destructor): the result of Vec3 operator- (0x414330).
struct H_V3 {
    float x, y, z;
};

// A vector with an (empty) destructor whose difference is the out-of-line 0x414330.
struct H_Vec {
    H_Vec() {}
    H_Vec(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    H_Vec(const H_Vec& o)
    {
        *(int*)&x = *(const int*)&o.x;
        *(int*)&y = *(const int*)&o.y;
        *(int*)&z = *(const int*)&o.z;
    }
    ~H_Vec() {}
    H_Vec& operator-=(float k) { x -= k; return *this; }
    H_V3 operator-(const H_Vec& o) const; // 0x414330
    float x, y, z;
};

// A model part of the interior (0x12c bytes): position at +0x14, matrix at +0x20, flag bits at +0xc.
struct H_Rgba {
    H_Rgba() {}
    H_Rgba(float r_, float g_, float b_, float a_) { r = r_; g = g_; b = b_; a = a_; }
    float r, g, b, a;
};
struct H_Part {
    void Show();                       // 0x4b2b90
    void SetParent(void* parent);      // 0x4b2b30
    BYTE unk0[0x14];
    H_Vec pos;                         // 0x14
    H_Matrix matrix;                   // 0x20
    BYTE unk60[0xa8 - 0x60];
    float boxMin;                      // 0xa8
    BYTE unkac[0xb4 - 0xac];
    float boxMax;                      // 0xb4
};

// A coloured/model slot of the car (Car+0x2074..0x207c): the part object is at +0xc.
struct H_Slot {
    BYTE unk0[0xc];
    H_Part* part;                      // 0x0c
    BYTE unk10[4];
    Vec3 pos;                          // 0x14
};

struct H_Wiper {
    void SetAngle(float t);            // 0x4021b0
};
struct H_Emitter {
    void Update(float dt, float xMin, float xMax, float yMin, float yMax); // 0x476fa0
};
struct H_Rain {
    void Update(float dt);             // 0x46e2d0
};
struct H_Flap {
    BYTE unk0[0x20];
    H_Matrix matrix;                   // 0x20
};
struct H_Drive {
    float GetShiftProgress();          // 0x46b1d0
    float GetSmoothedRpm();            // 0x46ba60
};
struct H_Body {
    BYTE unk0[0x20];
    float m[11];                       // 0x20: rows 0x20/0x30/0x40
    BYTE unk4c[0x1cc - 0x4c];
    Vec3 vel;                          // 0x1cc
};
struct H_Colours {
    BYTE unk0[0x65c];
    H_Rgba colour[4];                  // 0x65c
};
struct H_Lamp {
    BYTE unk0[0x88];
    H_Rgba colour;                     // 0x88
};
struct H_PtrVec {
    BYTE alloc;
    H_Lamp** first;                    // 0x04
    H_Lamp** last;                     // 0x08
    H_Lamp** end;                      // 0x0c
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
};

// The Car as 0x453940 sees it (raw view cast from this).
struct H_Car {
    BYTE unk0[0x24];
    H_Body* body;                      // 0x24
    BYTE unk28[0xfe8 - 0x28];
    float unkfe8;                      // 0xfe8
    float thrLo;                       // 0xfec
    float thrHi;                       // 0xff0
    BYTE unkff4[0x1018 - 0xff4];
    Vec3 iniOffset;                    // 0x1018
    BYTE unk1024[0x12f4 - 0x1024];
    float hoodGain;                    // 0x12f4
    float trunkGain;                   // 0x12f8
    BYTE unk12fc[4];
    float wiperSpeed;                  // 0x1300
    BYTE unk1304[0x130c - 0x1304];
    float wipeLo1;                     // 0x130c
    float wipeHi1;                     // 0x1310
    float wipeLo2;                     // 0x1314
    float wipeHi2;                     // 0x1318
    BYTE unk131c[0x1340 - 0x131c];
    float handbrake;                   // 0x1340
    BYTE unk1344[0x134c - 0x1344];
    float steer;                       // 0x134c
    BYTE unk1350[0x13fc - 0x1350];
    H_Vec ctrl10;                      // 0x13fc
    BYTE unk1408[0x14fc - 0x1408];
    H_Colours* colours;                // 0x14fc
    BYTE unk1500[0x1e00 - 0x1500];
    float unk1e00;                     // 0x1e00
    BYTE unk1e04[0x1f10 - 0x1e04];
    float rpmScale;                    // 0x1f10
    float speedScale;                  // 0x1f14
    BYTE unk1f18[0x1f3c - 0x1f18];
    float trunkPos;                    // 0x1f3c
    BYTE unk1f40[4];
    float trunkLimit;                  // 0x1f44
    BYTE unk1f48[0x1f58 - 0x1f48];
    float hoodPos;                     // 0x1f58
    BYTE unk1f5c[4];
    float hoodLimit;                   // 0x1f60
    BYTE unk1f64[0x1f68 - 0x1f64];
    H_Flap* hood;                      // 0x1f68
    H_Flap* trunk;                     // 0x1f6c
    H_Matrix hoodMatrix;               // 0x1f70
    H_Matrix trunkMatrix;              // 0x1fb0
    BYTE unk1ff0[0x1ff8 - 0x1ff0];
    H_Wiper* wiper;                    // 0x1ff8
    int wiperOn;                       // 0x1ffc
    float wiperPhase;                  // 0x2000
    H_Emitter* emitter;                // 0x2004
    H_Rain* rain;                      // 0x2008
    BYTE unk200c[0x2018 - 0x200c];
    H_Part* part2018;                  // 0x2018
    H_Part* part201c;                  // 0x201c
    H_Part* part2020;                  // 0x2020
    H_Part* part2024;                  // 0x2024
    BYTE unk2028[0x2048 - 0x2028];
    H_Part* driver;                    // 0x2048
    BYTE unk204c[0x2054 - 0x204c];
    H_Part* indicatorSpeed;            // 0x2054
    H_Part* indicatorRpm;              // 0x2058
    Vec3 vec205c;                      // 0x205c
    Vec3 vec2068;                      // 0x2068
    H_Slot* slot2074;                  // 0x2074
    H_Slot* slot2078;                  // 0x2078
    H_Slot* slot207c;                  // 0x207c
    BYTE unk2080[0x2098 - 0x2080];
    float steerHalf;                   // 0x2098
    float level209c;                   // 0x209c
    float level20a0;                   // 0x20a0
    int flag20a4;                      // 0x20a4
    float noise;                       // 0x20a8
    Vec3 vec20ac;                      // 0x20ac
    Vec3 vec20b8;                      // 0x20b8
    Vec3 vec20c4;                      // 0x20c4
    Vec3 vec20d0;                      // 0x20d0
    BYTE unk20dc[0x1bcac - 0x20dc];
    float lamp[4];                     // 0x1bcac
    BYTE unk1bcbc[0x1bcc4 - 0x1bcbc];
    H_PtrVec lamps;                    // 0x1bcc4
    BYTE unk1bcd4[0x1befc - 0x1bcd4];
    int replayMode;                    // 0x1befc
};

// Cross product *dst = *a x *b as a hand-written x87 block (inline assembler in the original; the pointers live in
// stack locals).
inline void H_Cross(const float* a, const float* b, float* dst)
{
    __asm {
        mov ecx, a
        mov edx, b
        mov eax, dst
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx]
        fxch st(2)
        fsubp st(5), st
        fsubp st(3), st
        fsubp st(1), st
        fxch st(2)
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
}

// Adds the offset to a position (inline function with pointer arguments in the original).
inline void H_AddPos(H_Vec* p, const Vec3* o)
{
    p->x = o->x + p->x;
    p->y = o->y + p->y;
    p->z = o->z + p->z;
}

inline float H_Dot(const H_Vec* a, const float* b)
{
    return a->x * b[0] + a->y * b[1] + a->z * b[2];
}

static const float H_kSpeedLo = 5.0f;   // 0x4e1ad8
static const float H_kSpeedHi = 20.0f;  // 0x4e1adc

inline float H_Ratio(float a, float b)
{
    return a / b;
}

// Unit-length scaling of a three-float vector (inline function in the original).
inline void H_Normalize(float* v)
{
    float len = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] *= 1.0f / len;
    v[1] *= 1.0f / len;
    v[2] *= 1.0f / len;
}
inline void H_Normalize1(float* v)
{
    float len = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    float inv = 1.0f / len;
    v[0] = v[0] * inv;
    v[1] *= 1.0f / len;
    v[2] *= 1.0f / len;
}

// NONMATCHING: RALLYTROPHY 0x453940
// Logic complete (5002 of 5036 bytes, 86 diff hunks, tail from the lamp loop on matches). Open: the trunk/hood angle
// divides by a limit the original keeps preloaded on the x87 stack, the three inline normalisations differ in which
// product copies the reciprocal (fld st(0)), the offset adds load operands in the other order, and the frame
// layout/register choice of the rotation locals (edi/esi, argument slots) is not reproduced.
void Car::FUN_00453940(float k)
{
    H_Car* V = (H_Car*)this;
    if (V->wiperOn && V->wiper) {
        float t = V->wiperPhase + V->wiperPhase;
        if (!(t < 1.0f)) {
            t = 2.0f - t;
        }
        float range1 = V->wipeHi1 - V->wipeLo1;
        float a1 = range1 * t + V->wipeLo1;
        float range2 = V->wipeHi2 - V->wipeLo2;
        float b2 = range2 * t + V->wipeLo2;
        V->wiperPhase = k * V->wiperSpeed + V->wiperPhase;
        double phase = V->wiperPhase;
        V->wiperPhase = (float)(phase - floor(phase));
        float t2 = V->wiperPhase + V->wiperPhase;
        if (!(t2 < 1.0f)) {
            t2 = 2.0f - t2;
        }
        V->wiper->SetAngle(t2);
        float a2 = range1 * t2 + V->wipeLo1;
        float b1 = range2 * t2 + V->wipeLo2;
        if (a2 < a1) {
            float tmp = a2;
            a2 = a1;
            a1 = tmp;
        }
        if (b1 < b2) {
            float tmp = b1;
            b1 = b2;
            b2 = tmp;
        }
        if (V->emitter) {
            V->emitter->Update(k, a1, a2, b2, b1);
        }
        if (V->rain) {
            V->rain->Update(k);
        }
    }
    H_Matrix m;
    if (V->trunk) {
        float angle = H_Ratio(V->trunkPos * V->trunkGain, V->trunkLimit);
        m.SetIdentity();
        float c = (float)cos(angle);
        float s = (float)sin(angle);
        m.m[5] = c;
        m.m[6] = s;
        m.m[9] = -s;
        m.m[10] = c;
        V->trunk->matrix.Multiply(&V->trunkMatrix, &m);
    }
    if (V->hood) {
        float angle = H_Ratio(V->hoodPos * V->hoodGain, V->hoodLimit);
        m.SetIdentity();
        float c = (float)cos(angle);
        float s = (float)sin(angle);
        m.m[5] = c;
        m.m[6] = -s;
        m.m[9] = -m.m[6];
        m.m[10] = c;
        V->hood->matrix.Multiply(&V->hoodMatrix, &m);
    }
    static float shakeGain = V->unkfe8;
    static Vec3 up(0.0f, 1.0f, 0.0f);
    static Vec3 fwd(0.0f, 0.0f, 1.0f);
    H_Matrix basis;
    float speed = V->body->vel.Length();
    float steer = V->steer;
    float f1 = 0.0f;
    float a = (float)fabs(V->unk1e00);
    if (a > V->thrLo) {
        if (a < V->thrHi) {
            f1 = (a - V->thrLo) / (V->thrHi - V->thrLo);
        } else {
            f1 = 1.0f;
        }
    }
    float shake;
    if (speed > H_kSpeedLo) {
        float f2 = 1.0f;
        if (speed < H_kSpeedHi) {
            f2 = (speed - H_kSpeedLo) / (H_kSpeedHi - H_kSpeedLo);
        }
        shake = ((rand() & 0x7fff) * 6.103515625e-05f - 1.0f) * (f2 * 0.4f + f1 * 0.6f);
    } else {
        shake = ((rand() & 0x7fff) * 6.103515625e-05f - 1.0f) * f1;
    }
    shake *= shakeGain;
    float keep = (float)pow(1e-5f, k);
    V->noise = (1.0f - keep) * shake + V->noise * keep;
    steer += V->noise;
    if (steer < -1.0f) {
        steer = -1.0f;
    } else if (steer > 1.0f) {
        steer = 1.0f;
    }
    if (V->driver) {
        H_Part* o = V->driver;
        basis.m[8] = o->matrix.m[8];
        basis.m[9] = o->matrix.m[9];
        basis.m[10] = o->matrix.m[10];
        H_Cross(&up.x, &basis.m[8], &basis.m[0]);
        H_Normalize1(&basis.m[0]);
        H_Cross(&basis.m[8], &basis.m[0], &basis.m[4]);
        H_Matrix tmp;
        float angle = steer * 1.5707964f;
        tmp.FromAxisAngle(&fwd, angle);
        tmp.m[12] = o->matrix.m[12];
        tmp.m[13] = o->matrix.m[13];
        tmp.m[14] = o->matrix.m[14];
        o->matrix = tmp;
        o->matrix.Concat(&basis);
    }
    if (V->indicatorRpm) {
        H_Part* o = V->indicatorRpm;
        basis.m[8] = V->vec2068.x;
        basis.m[9] = V->vec2068.y;
        basis.m[10] = V->vec2068.z;
        H_Cross(&up.x, &basis.m[8], &basis.m[0]);
        H_Normalize(&basis.m[0]);
        H_Cross(&basis.m[8], &basis.m[0], &basis.m[4]);
        float r = ((H_Drive*)((BYTE*)this + 0xdb8))->GetSmoothedRpm() / V->rpmScale;
        if (r > 1.0f) {
            r = 1.0f;
        }
        float angle = (r + r - 1.0f) * 2.3561945f;
        o->matrix.FromAxisAngle(&fwd, angle);
        o->matrix.Concat(&basis);
    }
    if (V->indicatorSpeed) {
        H_Part* o = V->indicatorSpeed;
        basis.m[8] = V->vec205c.x;
        basis.m[9] = V->vec205c.y;
        basis.m[10] = V->vec205c.z;
        H_Cross(&up.x, &basis.m[8], &basis.m[0]);
        H_Normalize(&basis.m[0]);
        H_Cross(&basis.m[8], &basis.m[0], &basis.m[4]);
        float r = V->body->vel.Length() * 3.6f / V->speedScale;
        if (r > 1.0f) {
            r = 1.0f;
        }
        float angle = (r + r - 1.0f) * 2.3561945f;
        o->matrix.FromAxisAngle(&fwd, angle);
        o->matrix.Concat(&basis);
    }
    if (V->part2020) {
        V->part2020->Show();
    }
    if (V->part2024) {
        V->part2024->Show();
    }
    V->steerHalf = (steer + 1.0f) * 0.5f;
    if (V->slot2074) {
        H_Part* p = V->slot2074->part;
        V->part2020 = p;
        if (p) {
            p->pos.x = V->slot2074->pos.x;
            p->pos.y = V->slot2074->pos.y;
            p->pos.z = V->slot2074->pos.z;
            p->SetParent(V->body);
        }
    }
    float level;
    if (V->replayMode == 2) {
        level = V->level20a0;
    } else {
        level = ((H_Drive*)((BYTE*)this + 0xdb8))->GetShiftProgress();
        if (V->handbrake > 0.01f) {
            if (level < V->handbrake) {
                level = V->handbrake;
            }
        }
    }
    if (level > 0.0f) {
        V->level209c = level;
        V->flag20a4 = 1;
        if (V->slot207c) {
            H_Part* p = V->slot207c->part;
            V->part2024 = p;
            if (p) {
                p->pos.x = V->slot207c->pos.x;
                p->pos.y = V->slot207c->pos.y;
                p->pos.z = V->slot207c->pos.z;
                p->SetParent(V->body);
            }
        }
    } else if (V->slot2078) {
        H_Part* p = V->slot2078->part;
        V->part2024 = p;
        if (p) {
            p->pos.x = V->slot2078->pos.x;
            p->pos.y = V->slot2078->pos.y;
            p->pos.z = V->slot2078->pos.z;
            p->SetParent(V->body);
        }
        V->level209c = V->steerHalf;
        V->flag20a4 = 0;
    }
    float keep1 = (float)pow(9.999999974752427e-07, k);
    float keep2 = (float)pow(1.9999999494757503e-05, k);
    float gain1 = 1.0f - keep1;
    float gain2 = 1.0f - keep2;
    H_Vec d(V->ctrl10);
    H_Vec loc;
    {
        float lenSq = d.x * d.x + d.z * d.z + d.y * d.y;
        float c;
        *(int*)&c = *(int*)&lenSq;
        float scale;
        if (c > 196.0f) {
            scale = 1.0f / (float)sqrt(lenSq);
        } else if (c < 4.0f) {
            d.z = 0.0f;
            d.y = 0.0f;
            d.x = 0.0f;
            goto scaled;
        } else {
            float len = (float)sqrt(c);
            float inv = 1.0f / (float)sqrt(lenSq);
            d.x *= inv;
            d.y *= inv;
            d.z *= inv;
            scale = (len - 2.0f) * 0.083333336f;
        }
        d.x *= scale;
        d.y *= scale;
        d.z *= scale;
    }
scaled:
    loc.x = H_Dot(&d, &V->body->m[0]);
    loc.y = H_Dot(&d, &V->body->m[4]);
    loc.z = H_Dot(&d, &V->body->m[8]);
    V->vec20ac.x = loc.x * gain1 + keep1 * V->vec20ac.x;
    V->vec20ac.y = loc.y * gain1 + keep1 * V->vec20ac.y;
    V->vec20ac.z = loc.z * gain1 + keep1 * V->vec20ac.z;
    V->vec20b8.x = loc.x * gain2 + keep2 * V->vec20b8.x;
    V->vec20b8.y = loc.y * gain2 + keep2 * V->vec20b8.y;
    V->vec20b8.z = loc.z * gain2 + keep2 * V->vec20b8.z;
    if (V->part201c) {
        H_Part* p = V->part201c;
        float t = loc.x * 0.024f;
        p->pos.x = V->vec20d0.x - t;
        p->pos.y = V->vec20d0.y - loc.y * 0.02f;
        p->pos.z = V->vec20d0.z - loc.z * 0.02f;
        H_AddPos(&p->pos, &V->iniOffset);
        if (V->part2018) {
            H_Part* q = V->part2018;
            q->pos.x = V->vec20c4.x - t;
            q->pos.y = V->vec20c4.y - loc.y * 0.024f;
            q->pos.z = V->vec20c4.z - loc.z * 0.016f;
            H_AddPos(&q->pos, &V->iniOffset);
        }
        if (V->part2020) {
            H_Part* q = V->part2020;
            H_Vec A(q->pos);
            H_Vec B(q->pos);
            A.z = q->boxMin;
            A.x -= t;
            B.z = q->boxMax;
            A.y = A.y - loc.y * 0.024f;
            H_V3 dir = B - A;
            float inv = 1.0f / (float)sqrt(dir.x * dir.x + dir.z * dir.z + dir.y * dir.y);
            dir.x = dir.x * inv;
            dir.y = dir.y * inv;
            dir.z = dir.z * inv;
            *(H_V3*)&q->matrix.m[8] = dir;
            H_Cross(&up.x, &dir.x, &q->matrix.m[0]);
            H_Cross(&dir.x, &V->part2020->matrix.m[0], &V->part2020->matrix.m[4]);
            V->part2020->pos.z = V->part2020->pos.z - loc.z * 0.008f;
            H_AddPos(&V->part2020->pos, &V->iniOffset);
        }
        if (V->part2024) {
            H_Part* q = V->part2024;
            H_Vec A(q->pos);
            H_Vec B(q->pos);
            A.z = q->boxMin;
            A.x -= t;
            B.z = q->boxMax;
            A.y = A.y - loc.y * 0.024f;
            H_V3 dir = B - A;
            float inv = 1.0f / (float)sqrt(dir.z * dir.z + dir.y * dir.y + dir.x * dir.x);
            dir.x = dir.x * inv;
            dir.y = dir.y * inv;
            dir.z = dir.z * inv;
            *(H_V3*)&q->matrix.m[8] = dir;
            H_Cross(&up.x, &dir.x, &q->matrix.m[0]);
            H_Cross(&dir.x, &V->part2024->matrix.m[0], &V->part2024->matrix.m[4]);
            V->part2024->pos.z = V->part2024->pos.z - loc.z * 0.008f;
            H_AddPos(&V->part2024->pos, &V->iniOffset);
        }
        {
            float wa = loc.x * -0.3f;
            float wb = loc.z * -0.2f;
            float n = (float)sqrt(wb * wb + wa * wa + 1.0f);
            float inv = 1.0f / n;
            H_Vec w(wa * inv, inv, wb * inv);
            H_Matrix tmp;
            H_Cross(&w.x, &fwd.x, &tmp.m[0]);
            tmp.m[4] = w.x;
            tmp.m[5] = w.y;
            tmp.m[6] = w.z;
            H_Cross(&tmp.m[0], &tmp.m[4], &tmp.m[8]);
            float angle = V->steer * -0.2617994f;
            p->matrix.FromAxisAngle(&up, angle);
            p->matrix.Concat(&tmp);
        }
    }
    this->FUN_00454d10();
    if (V->lamps.Size() != 0) {
        float sum = 0.0f;
        for (int j = 0; j < 4; j++) {
            float x = V->lamp[j];
            if (x <= 0.0f) {
                V->lamp[j] = 0.0f;
            } else {
                sum += x;
            }
        }
        H_Rgba acc(0.0f, 0.0f, 0.0f, 0.0f);
        if (sum > 0.001f) {
            const H_Rgba* tbl = V->colours->colour;
            for (int i = 0; i < 4; i++) {
                float w = V->lamp[i] / sum;
                acc.r += w * tbl[i].r;
                acc.g += w * tbl[i].g;
                acc.b += w * tbl[i].b;
                acc.a += w * tbl[i].a;
            }
            acc.r *= 0.003921569f;
            acc.g *= 0.003921569f;
            acc.b *= 0.003921569f;
            acc.a *= 0.003921569f;
            if (sum < 1.0f) {
                acc.a *= sum;
            }
        }
        for (unsigned int n = 0; n < V->lamps.Size(); n++) {
            H_Lamp* l = V->lamps.first[n];
            l->colour = acc;
        }
    }
}

// Compiler-generated: the atexit callbacks of the function statics of 0x453940 (empty Vec3 destructors).

// FUNCTION: RALLYTROPHY 0x454cf0
// SYMBOL: _$E20
typedef void Generated_454cf0();

// FUNCTION: RALLYTROPHY 0x454d00
// SYMBOL: _$E19
typedef void Generated_454d00();

// ---- F: interior tilt (0x454d10) ----

// A plain vector (no destructor: the function has no unwind frame).
struct F2_V {
    F2_V() {}
    F2_V(const F2_V& o)
    {
        x = o.x;
        y = o.y;
        z = o.z;
    }
    float x, y, z;
};
// A 3x3 rotation stored as three rows of a 4x4 matrix (row stride 0x10).
struct F2_Rot {
    F2_V r0;                           // 0x00
    float pad0;
    F2_V r1;                           // 0x10
    float pad1;
    F2_V r2;                           // 0x20
};
// A model part: position at +0x14, rotation rows at +0x20.
struct F2_Part {
    BYTE unk0[0x14];
    F2_V pos;                          // 0x14
    F2_Rot rot;                        // 0x20
};
// The Car as the interior-tilt code sees it.
struct F2_CarView {
    BYTE unk0[0x24];
    struct F2_BodyView* body;          // 0x24
    BYTE unk28[0x1024 - 0x28];
    F2_V lamp1;                        // 0x1024
    BYTE unk1030[0x1408 - 0x1030];
    F2_V ctrl;                         // 0x1408
    BYTE unk1414[0x2028 - 0x1414];
    F2_Part* partA;                    // 0x2028
    F2_Part* partB;                    // 0x202c
    F2_V posA;                         // 0x2030
    F2_V posB;                         // 0x203c
};
struct F2_BodyView {
    BYTE unk0[0x20];
    F2_Rot rot;                        // 0x20
};

// The cross product *a x *b into *out (inline assembler in the original).
inline void F2_Cross(const F2_V* a, const F2_V* b, F2_V* dst)
{
    __asm {
        mov ecx, a
        mov edx, b
        mov eax, dst
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx]
        fxch st(2)
        fsubp st(5), st
        fsubp st(3), st
        fsubp st(1), st
        fxch st(2)
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
}

// NONMATCHING: RALLYTROPHY 0x454d10
// 824 vs 843 bytes: logic and the function static (0x508c60, atexit 0x455060) are in place. Open: the original keeps
// only x of the clamped vector on the FPU stack (y, z in memory, squares in the order x, y, z) and the first part
// builds y/z of the part position in memory; ours enregisters differently.
void Car::FUN_00454d10()
{
    F2_CarView* c = (F2_CarView*)this;
    F2_V v = c->ctrl;
    float len2 = v.x * v.x + v.y * v.y + v.z * v.z;
    if (len2 > 196.0f) {
        float s = 1.0f / (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        v.x = v.x * s;
        v.y = v.y * s;
        v.z = v.z * s;
    } else if (len2 < 4.0f) {
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
    } else {
        float len = (float)sqrt(len2);
        float k = (len - 2.0f) / (len * 12.0f);
        v.x = v.x * k;
        v.y = v.y * k;
        v.z = v.z * k;
    }
    F2_BodyView* b = c->body;
    F2_V d;
    d.x = v.y * b->rot.r0.y + v.z * b->rot.r0.z + v.x * b->rot.r0.x;
    d.y = v.y * b->rot.r1.y + v.z * b->rot.r1.z + v.x * b->rot.r1.x;
    d.z = v.y * b->rot.r2.y + v.z * b->rot.r2.z + v.x * b->rot.r2.x;
    F2_Part* a = c->partA;
    if (a) {
        F2_V t = c->posA;
        t.x = t.x - d.x * 0.024f;
        t.y = t.y - d.y * 0.024f;
        t.z = t.z - d.z * 0.016f;
        a->pos = t;
        a->pos.x += c->lamp1.x;
        a->pos.y += c->lamp1.y;
        a->pos.z += c->lamp1.z;
    }
    F2_Part* q = c->partB;
    if (q) {
        F2_V t = c->posB;
        t.x = t.x - d.x * 0.024f;
        t.y = t.y - d.y * 0.02f;
        t.z = t.z - d.z * 0.02f;
        q->pos = t;
        q->pos.x += c->lamp1.x;
        q->pos.y += c->lamp1.y;
        q->pos.z += c->lamp1.z;
        float ax = d.x * -0.3f;
        float cz = d.z * -0.2f;
        float inv = 1.0f / (float)sqrt(cz * cz + ax * ax + 1.0f);
        d.x = ax * inv;
        d.y = inv;
        d.z = cz * inv;
        static Vec3 axis(0.0f, 0.0f, 1.0f);
        F2_Cross(&d, (const F2_V*)&axis, &q->rot.r0);
        q->rot.r1 = d;
        F2_Cross(&q->rot.r0, &q->rot.r1, &q->rot.r2);
    }
}

// Atexit callback of the function static `axis` of 0x454d10 (the empty ~Vec3).

// FUNCTION: RALLYTROPHY 0x455060
// SYMBOL: _$E22
typedef void Generated_455060();

#pragma function(strcmp)

// ---- group I: LoadDriver (0x455070) ----

// Anything the loaded model owns through a pointer and deletes virtually.
class LoadDriverPoly {
public:
    virtual ~LoadDriverPoly();
};
// A part of a .bmf model (name pointer at +0x118).
struct LoadDriverPart {
    BYTE unk0[0x118];
    char* name;                        // 0x118
};
// The loaded .bmf model (0x80 bytes, ctor 0x4af950, dtor 0x4afe20): parts at +0x28, owned objects at +0x48.
class LoadDriverModelFile {
public:
    LoadDriverModelFile(const char* path, const char* dir, void* cache, void* materials, int flag); // 0x4af950
    ~LoadDriverModelFile();            // 0x4afe20
    BYTE unk0[0x28];
    std::vector<LoadDriverPart*> parts; // 0x28
    BYTE unk38[0x48 - 0x38];
    std::vector<LoadDriverPoly*> objects; // 0x48
    BYTE unk58[0x80 - 0x58];
};
// A model-part object (0x12c bytes, ctor 0x4a6910).
class LoadDriverObject {
public:
    LoadDriverObject(LoadDriverPart* part); // 0x4a6910
    void SetParent(void* parent);      // 0x4b2b30
    BYTE unk0[0x14];
    float pivot[3];                    // 0x14
    BYTE unk20[0x12c - 0x20];
};
// A mesh container (0x20 bytes, ctor 0x401c10, loader 0x401cf0).
class LoadDriverMeshSet {
public:
    LoadDriverMeshSet();               // 0x401c10
    void Load(LoadDriverModelFile* file); // 0x401cf0
    BYTE unk0[0x20];
};
// Car fields written by LoadDriver.
struct LoadDriverView {
    BYTE unk0[0x24];
    void* body;                        // 0x24
    BYTE unk28[0xfa4 - 0x28];
    void* cache;                       // 0xfa4
    BYTE unkfa8[4];
    BYTE materials[4];                 // 0xfac
    BYTE unkfb0[0x2018 - 0xfb0];
    LoadDriverObject* driverBody;      // 0x2018
    LoadDriverObject* driverHead;      // 0x201c
    BYTE unk2020[0x2028 - 0x2020];
    LoadDriverObject* coDriverBody;    // 0x2028
    LoadDriverObject* coDriverHead;    // 0x202c
    float coBodyPos[3];                // 0x2030
    float coHeadPos[3];                // 0x203c
    BYTE unk2048[0x2080 - 0x2048];
    LoadDriverMeshSet* armLeftLo;      // 0x2080
    LoadDriverMeshSet* armLeftHi;      // 0x2084
    LoadDriverMeshSet* armRightLo;     // 0x2088
    LoadDriverMeshSet* armRightHi;     // 0x208c
    LoadDriverMeshSet* transLo;        // 0x2090
    LoadDriverMeshSet* transHi;        // 0x2094
    BYTE unk2098[0x20c4 - 0x2098];
    float driverBodyPos[3];            // 0x20c4
    float driverHeadPos[3];            // 0x20d0
};


// std::string operations the original keeps out of line at some call sites and expands inline at others (VC6 spends
// a small inline budget per function). The view has the layout of std::string (allocator, _Ptr, _Len, _Res);
// Tidy 0x402700, Grow 0x402980, Eos 0x402960, append 0x403ba0, max_size 0x402aa0, _Copy 0x402ab0.
struct LoadDriverStr {
    BYTE alloc[4];
    char* ptr;
    unsigned int len;
    unsigned int res;
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    void AppendOut(const char* s, unsigned int n); // 0x403ba0
    unsigned int MaxSize() const;      // 0x402aa0
    void Copy(unsigned int n);         // 0x402ab0
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    // assign(const char*): _Grow and _Eos as calls
    void AssignA(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    // assign(const char*): _Grow as a call, _Eos expanded
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    // assign(const char*): _Grow expanded (max_size and _Tidy as calls), _Eos as a call
    void AssignC(const char* s)
    {
        unsigned int n = strlen(s);
        if (MaxSize() < n) {
            std::_Xlen();
        }
        bool ok;
        if (ptr != 0 && Refcnt(ptr) != 0 && Refcnt(ptr) != 255) {
            if (n == 0) {
                --Refcnt(ptr), Tidy(false);
                ok = false;
            } else {
                Copy(n);
                ok = true;
            }
        } else if (n == 0) {
            Tidy(true);
            ok = false;
        } else {
            if (31 < res || res < n) {
                Tidy(true);
                Copy(n);
            }
            ok = true;
        }
        if (ok) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    // append(const char*): _Eos as a call
    void AppendA(const char* s)
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = m + len, false)) {
            std::char_traits<char>::copy(ptr + len, s, m);
            Eos(n);
        }
    }
    // append(const char*): _Eos expanded
    void AppendB(const char* s)
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = m + len, false)) {
            std::char_traits<char>::copy(ptr + len, s, m);
            ptr[len = n] = 0;
        }
    }
};
#define LD_STR(s) ((LoadDriverStr*)&(s))

// NONMATCHING: RALLYTROPHY 0x455070
// 3651 vs 3651 bytes; 10 bytes differ: the operand order of five `lea ebp,[m+len]` / `lea edi,[ptr+len]` pairs in the
// hand-expanded std::string::append sites (the original puts the old length in the index register). Everything else
// (call sequence, EH states 0..0x12, string literals, scheduling) matches.
// Car::LoadDriver(dir): loads <dir>driver.bmf and co_driver.bmf (driver / co-driver body and head objects and their
// pivots), then the optional arm and transmission models.
void Car::LoadDriver(const char* dir)
{
    LoadDriverView* c = (LoadDriverView*)this;
    int i;
    std::string path;
    {
        float* v = c->driverBodyPos;
        v[0] = v[1] = v[2] = 0;
        v = c->driverHeadPos;
        v[0] = v[1] = v[2] = 0;
        v = c->coBodyPos;
        v[0] = v[1] = v[2] = 0;
        v = c->coHeadPos;
        v[0] = v[1] = v[2] = 0;
    }
    LD_STR(path)->AssignA(dir);
    LD_STR(path)->AppendOut("driver.bmf", strlen("driver.bmf"));
    LoadDriverModelFile driver(path.c_str(), dir, c->cache, c->materials, 0);
    for (i = 0; i < (int)driver.parts.size(); i++) {
        LoadDriverPart* part = driver.parts[i];
        const char* name = !part->name ? "" : part->name;
        if (!strcmp(name, "driver_body")) {
            LoadDriverObject* o = new LoadDriverObject(part);
            c->driverBody = o;
            c->driverBodyPos[0] = o->pivot[0];
            c->driverBodyPos[1] = o->pivot[1];
            c->driverBodyPos[2] = o->pivot[2];
        } else if (!strcmp(name, "driver_head")) {
            LoadDriverObject* o = new LoadDriverObject(part);
            c->driverHead = o;
            c->driverHeadPos[0] = o->pivot[0];
            c->driverHeadPos[1] = o->pivot[1];
            c->driverHeadPos[2] = o->pivot[2];
        }
    }
    for (i = 0; i < (int)driver.objects.size(); i++) {
        delete driver.objects[i];
    }
    LD_STR(path)->AssignA(dir);
    LD_STR(path)->AppendOut("co_driver.bmf", strlen("co_driver.bmf"));
    LoadDriverModelFile coDriver(path.c_str(), dir, c->cache, c->materials, 0);
    for (i = 0; i < (int)coDriver.parts.size(); i++) {
        LoadDriverPart* part = coDriver.parts[i];
        const char* name = !part->name ? "" : part->name;
        if (!strcmp(name, "co_driver_body")) {
            LoadDriverObject* o = new LoadDriverObject(part);
            c->coDriverBody = o;
            c->coBodyPos[0] = o->pivot[0];
            c->coBodyPos[1] = o->pivot[1];
            c->coBodyPos[2] = o->pivot[2];
            o->SetParent(c->body);
        } else if (!strcmp(name, "co_driver_head")) {
            LoadDriverObject* o = new LoadDriverObject(part);
            c->coDriverHead = o;
            c->coHeadPos[0] = o->pivot[0];
            c->coHeadPos[1] = o->pivot[1];
            c->coHeadPos[2] = o->pivot[2];
            o->SetParent(c->body);
        }
    }
    for (i = 0; i < (int)coDriver.objects.size(); i++) {
        delete coDriver.objects[i];
    }
#define LOADDRIVER_OPTIONAL(FILE, SLOT)                                                              \
    ASSIGN(path, dir);                                                                               \
    APPEND(path, FILE);                                                                              \
    if (FUN_004a6050(path.c_str())) {                                                                \
        LoadDriverModelFile file(path.c_str(), dir, c->cache, c->materials, 0);                      \
        LoadDriverMeshSet* set = new LoadDriverMeshSet;                                              \
        c->SLOT = set;                                                                               \
        set->Load(&file);                                                                            \
        for (i = 0; i < (int)file.objects.size(); i++) {                                             \
            delete file.objects[i];                                                                  \
        }                                                                                            \
    }
#define ASSIGN(s, d) LD_STR(s)->AssignA(d)
#define APPEND(s, f) LD_STR(s)->AppendOut(f, strlen(f))
    LOADDRIVER_OPTIONAL("arm_left_lo.bmf", armLeftLo)
#undef ASSIGN
#undef APPEND
#define ASSIGN(s, d) LD_STR(s)->AssignB(d)
#define APPEND(s, f) LD_STR(s)->AppendA(f)
    LOADDRIVER_OPTIONAL("arm_left_hi.bmf", armLeftHi)
    LOADDRIVER_OPTIONAL("arm_right_lo.bmf", armRightLo)
#undef APPEND
#define APPEND(s, f) LD_STR(s)->AppendB(f)
    LOADDRIVER_OPTIONAL("arm_right_hi.bmf", armRightHi)
    LOADDRIVER_OPTIONAL("transmission_lo.bmf", transLo)
#undef ASSIGN
#define ASSIGN(s, d) LD_STR(s)->AssignC(d)
    LOADDRIVER_OPTIONAL("transmission_hi.bmf", transHi)
}

// ---- J: wiper and rain loaders ----

// A part node of a loaded wiper/rain set: flags at +0x0c, parent link 0x4b2b30.
struct J_Node {
    BYTE unk0[0x0c];
    BYTE flags;                        // 0x0c
    void SetParent(void* parent);      // 0x4b2b30
};
// The wiper set object (0x20 bytes, ctor 0x401c10 clears the first five dwords, load 0x401cf0).
struct J_WiperSet {
    J_WiperSet();                      // 0x401c10
    void Load(ModelFile* file);        // 0x401cf0
    BYTE unk0[0x0c];
    J_Node* node;                      // 0x0c
    BYTE unk10[0x20 - 0x10];
};
// The Car as the loaders see it.
struct J_CarView {
    BYTE unk0[0x1ffc];
    int wiperPresent;                  // 0x1ffc
    float wiperPhase;                  // 0x2000
};

// A copy of the directory name: the constructor calls the out-of-line _Tidy (0x402700) and assign(dir, 0, npos)
// (0x4024f0); append is out of line (0x403ba0); the destructor is expanded inline (the first site calls _Tidy).
class J_Path {
public:
    J_Path(const std::string& dir) : alloc(*(const std::allocator<char>*)&dir)
    {
        Tidy(false);
        AssignString(dir, 0, std::string::npos);
    }
    ~J_Path() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)              // std::string::_Tidy, expanded inline
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    void AssignString(const std::string& src, unsigned int pos, unsigned int n); // 0x4024f0
    void Append(const char* s, unsigned int n); // 0x403ba0
    void Add(const char* s) { Append(s, strlen(s)); }
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    void AddInline(const char* s)      // append(const char*) expanded inline around the out-of-line _Grow and _Eos
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = len + m, false)) {
            memcpy(ptr + len, s, m);
            Eos(n);
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

// original: FUN_00455ec0 (thiscall, loads wipers.bmf of the car directory)
// FUNCTION: RALLYTROPHY 0x455ec0
void CarObject::FUN_00455ec0(const std::string& dir)
{
    J_CarView* v = (J_CarView*)this;
    v->wiperPresent = 0;
    v->wiperPhase = 0.0f;
    if (slot == 0) {
        return;
    }
    J_Path path(dir);
    path.Add("wipers.bmf");
    if (!FUN_004a6050(path.c_str())) {
        return;
    }
    ModelFile file(path.c_str(), dir.c_str(), textureCache, &materials, 0);
    J_WiperSet* wipers = new J_WiperSet;
    set1ff8 = (OwnedSet*)wipers;
    wipers->Load(&file);
    for (int i = 0; i < (int)file.objects.size(); i++) {
        delete file.objects[i];
    }
    J_Node* node = wipers->node;
    node->SetParent(body);
    node->flags |= 1;
}

// The rain-drop fields of the Car (the wiper/rain geometry read from car.ini).
struct J_RainView {
    BYTE unk0[0x12fc];
    int rainCount;                     // 0x12fc
    float unk1300;                     // 0x1300
    float g0;                          // 0x1304
    float g1;                          // 0x1308
    float g2;                          // 0x130c
    float g3;                          // 0x1310
    float g4;                          // 0x1314
    float g5;                          // 0x1318
    float pos0[3];                     // 0x131c
    float pos1[3];                     // 0x1328
};
// An effect attached to the car body (non-virtual 0x4b2b30).
struct J_Effect {
    void SetParent(void* parent);      // 0x4b2b30
};

// Directory copy for the rain loader. Force the normal cleanup inline while retaining
// a real destructor for exception unwinding (matches the folded string dtor at 0x430720).
class J_Path2 {
public:
    J_Path2(const std::string& dir) : alloc(*(const std::allocator<char>*)&dir)
    {
        Tidy(false);
        AssignString(dir, 0, std::string::npos);
    }
    ~J_Path2() { Free(); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    __forceinline void Free()          // std::string::_Tidy(true), expanded inline
    {
        if (ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    void AssignString(const std::string& src, unsigned int pos, unsigned int n); // 0x4024f0
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    void AddInline(const char* s)      // append(const char*) expanded inline around the out-of-line _Grow and _Eos
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = len + m, false)) {
            char* dest = ptr + len;
            memcpy(dest, s, m);
            Eos(n);
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

// original: FUN_004560a0 (thiscall, loads rain_drop.bmf: the windscreen rain and the body rain)
// FUNCTION: RALLYTROPHY 0x4560a0
// Evidence: ev_046b6a95d3842d3fb060ae2ee4e5601ea23699ece8452983e9576767a3153def.
// A named append destination preserves the target pointer/count register allocation.
void CarObject::FUN_004560a0(const std::string& dir)
{
    J_RainView* v = (J_RainView*)this;
    if (set1ff8 == 0) {
        return;
    }
    J_Path2 path(dir);
    path.AddInline("rain_drop.bmf");
    if (!FUN_004a6050(path.c_str())) {
        return;
    }
    {
        ModelFile file(path.c_str(), dir.c_str(), textureCache, &materials, 0);
        WindowRain* rain = new WindowRain(0x80);
        obj2004 = (ModelObject*)rain;
        if (v->rainCount) {
            rain->AddView(0, (const Vec3*)v->pos0, v->g0, v->g1, v->g2, v->g3);
            if (v->rainCount > 1) {
                ((WindowRain*)obj2004)->AddView(1, (const Vec3*)v->pos1, v->g0, v->g1, v->g4, v->g5);
            }
        }
        WindowRain* emitter = (WindowRain*)obj2004;
        emitter->Init((RainScene*)&file);
        void* parent = body;
        ((J_Effect*)emitter)->SetParent(parent);
        CarRain* drops = new CarRain(0x20);
        obj2008 = (ModelObject*)drops;
        drops->Init((RainScene*)&file);
        ((J_Effect*)drops)->SetParent(parent);
        drops->SetBounds((CarBodyObject*)parent, (CarBodyObject*)trunkCrash, (CarBodyObject*)hoodCrash);
        for (int i = 0; i < (int)file.objects.size(); i++) {
            delete file.objects[i];
        }
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration (init-table entry).

// FUNCTION: RALLYTROPHY 0x4563b0
// SYMBOL: _$E28
typedef void Generated_4563b0();

// FUNCTION: RALLYTROPHY 0x4563d0
// SYMBOL: _$E27
typedef void Generated_4563d0();
