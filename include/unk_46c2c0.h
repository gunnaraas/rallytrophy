/* Wheel (0x46c2c0-0x46d6ef, vtable 0x4e1ee8, 0x330 bytes): one tyre of a Car, an EffectBase-derived object with a
 * "tire" model child. Map: analysis/range-46a8a0.md section 4. The Car's real wheel array is at Car+0x28 (stride
 * 0x330); the CarWheel in unk_481390.h is the tail view of each wheel from wheel+0x298.
 * Include after rallytrophy.h, unk_481390.h and unk_47c460.h. */
#ifndef UNK_46C2C0_H
#define UNK_46C2C0_H

#include <string.h>

class Wheel;

// A std::string (VC6 layout) as the Wheel constructor assigns its name: _Grow stays out of line (0x402980) while the
// rest of assign is expanded inline.
struct NameString {
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    int alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

// 4x4 matrix with an empty constructor and destructor (the steering and spin matrices of UpdateSpinMatrix, and the
// tire object's matrix).
struct RawMatrix {
    RawMatrix() {}
    ~RawMatrix() {}
    void SetIdentity()
    {
        m[1] = m[2] = m[3] = 0.0f;
        m[4] = m[6] = m[7] = 0.0f;
        m[8] = m[9] = m[11] = 0.0f;
        m[12] = m[13] = m[14] = 0.0f;
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
    void FromAxisAngle(const Vec3& axis, float angle); // 0x4b5760
    void FromEuler(float rx, float ry, float rz);      // 0x4b5670
    void Multiply(const RawMatrix* a, const RawMatrix* b); // 0x4b5540: this = a * b
    float m[16];
};

// Mesh container of the tire model (8 bytes at wheel+0x258).
struct TireMeshes {
    void Load(const char* name, int flags); // 0x4a66e0
    int a, b;
};

// The embedded "tire" model object (0x12c bytes at wheel+0x134, ctor 0x4a6890, dtor 0x4a6cb0).
class TireObject {
public:
    TireObject();                      // 0x4a6890
    virtual ~TireObject();             // 0x4a6cb0
    virtual void vf1();
    virtual void vf2();                // slot 2: called by Wheel::LoadModel
    void SetParent(Wheel* wheel);      // 0x4b2b30
    BYTE unk4[4];
    int flags;                         // 0x08: 0x7f
    BYTE unkc[0x20 - 0xc];
    RawMatrix matrix;                  // 0x20: wheel+0x154, rewritten by UpdateSpinMatrix
    BYTE unk60[0x114 - 0x60];
    BYTE nameStorage[0x10];            // 0x114: std::string, wheel+0x248
    std::string& Name() { return *(std::string*)nameStorage; }
    TireMeshes meshes;                 // 0x124: wheel+0x258
};

// The tyre/surface parameter block a wheel points at (Wheel+0x2b0); floats only.
struct TyreSurface {
    float grip;                        // 0x00: scales the load reported at Wheel+0x2e4
    float alignScale;                  // 0x04: scales the aligning torque at Wheel+0x2e0
    float latScale;                    // 0x08: scales the lateral Pacejka output
    float lonPeakScale;                // 0x0c: scales the argument of the lateral Pacejka curve
    float p10, p14;                    // 0x10, 0x14: load-dependent term of the lateral stiffness
    float p18;
    float peakScale;                   // 0x1c: peak factor of the lateral force
    float loadRef;                     // 0x20: reference load of the peak curve
    float p24;
    float p28, p2c;                    // 0x28, 0x2c: load-dependent term of the longitudinal stiffness
    float p30, p34;                    // 0x30, 0x34: longitudinal peak, linear in the load
    float p38, p3c;                    // 0x38, 0x3c: lateral peak, linear in the load
};

// The part of the car body (Car+0x24) the tyre forces read and write.
struct WheelBody {
    BYTE unk0[0x14];
    float position[3];                 // 0x14
    float rotation[16];                // 0x20: 4x4 matrix, rows 0x20/0x30/0x40, translation row 0x50
    BYTE unk60[0x12c - 0x60];
    float mass;                        // 0x12c
    float invMass;                     // 0x130
    BYTE unk134[0x15c - 0x134];
    float inertia[16];                 // 0x15c: 4x4 (inverse inertia tensor in the upper 3x3)
    BYTE unk19c[0x1cc - 0x19c];
    float velocity[3];                 // 0x1cc
    BYTE unk1d8[0x1e4 - 0x1d8];
    float force[3];                    // 0x1e4: accumulated force
    float torque[3];                   // 0x1f0: accumulated torque
    BYTE unk1fc[0x224 - 0x1fc];
    float centre[3];                   // 0x224: centre of mass
    Vec3 FUN_004204f0(const Vec3& point); // 0x4204f0: velocity of a world point (out of line, hidden return pointer)
};

Vec3 FUN_004203e0(const Vec3& a, const Vec3& b); // cross product a x b (cdecl, hidden return pointer)
Vec3 operator*(float k, const Vec3& v); // 0x420430 (cdecl, hidden return pointer)

class Wheel : public EffectBase {
public:
    Wheel();                           // 0x46c2c0
    virtual ~Wheel();                  // 0x46c420 (scalar deleting dtor 0x46c400)
    void LoadModel(const char* name);  // 0x46c480
    void UpdateSpinMatrix(float dt);   // 0x46c550
    void Reset();                      // 0x46c6b0
    void UpdateForces();               // 0x46c790
    void IntegrateSpin(float dt);      // 0x46d5d0

    BYTE unk4[4];
    int unk8;                          // 0x08: zeroed by the ctor
    BYTE unkc[0x14 - 0xc];
    float position[3];                 // 0x14: relative to the body
    float axle[3];                     // 0x20: wheel axes (UpdateForces)
    BYTE unk2c[4];
    float up[3];                       // 0x30
    BYTE unk3c[4];
    float forward[3];                  // 0x40
    BYTE unk4c[0x114 - 0x4c];
    BYTE nameStorage[0x10];            // 0x114: std::string name = "tire null obj" (constructed by the base)
    NameString& Name() { return *(NameString*)nameStorage; }
    Vec3 vec124;                       // 0x124 (empty ctor/dtor: EH unwind entry 1)
    Car* car;                          // 0x130: owner
    TireObject tire;                   // 0x134 (size 0x12c): model object
    float radius;                      // 0x260: 0.3
    float unk264;                      // 0x264
    float unk268;                      // 0x268: radius-like factor, loaded from 0x264 by UpdateForces
    float unk26c;                      // 0x26c
    float inertia;                     // 0x270: wheel inertia (divisor in IntegrateSpin)
    float unk274;                      // 0x274
    float unk278;                      // 0x278
    float unk27c;                      // 0x27c
    float extraTorque;                 // 0x280
    float spin;                        // 0x284: angular velocity
    float slipA;                       // 0x288
    float slipB;                       // 0x28c
    float angle;                       // 0x290: spin angle
    float unk294;  /* combined slip (UpdateForces) */                        // 0x294
    int active;                        // 0x298: CarWheel.active
    int touching;                      // 0x29c: CarWheel.touching
    float unk2a0;                      // 0x2a0
    float steer;                       // 0x2a4: steer angle in units of 5 degrees
    int side;                          // 0x2a8
    float load;                        // 0x2ac
    struct TyreSurface* surface;                       // 0x2b0
    Vec3 normal;                       // 0x2b4: (0, 1, 0)
    Vec3 params;                       // 0x2c0: x = 0.005, y = 2.0, z = 0
    Vec3 forceA;                       // 0x2cc
    Vec3 torqueB;                      // 0x2d8
    float unk2e4;                      // 0x2e4
    float unk2e8;                      // 0x2e8
    float unk2ec;                      // 0x2ec
    float unk2f0;                      // 0x2f0
    float unk2f4;                      // 0x2f4
    float unk2f8;                      // 0x2f8
    float unk2fc;                      // 0x2fc
    float value;                       // 0x300: CarWheel.value
    float unk304;                      // 0x304
    float unk308;                      // 0x308
    float unk30c;                      // 0x30c
    float unk310;                      // 0x310
    float unk314;                      // 0x314
    float unk318;                      // 0x318: 0.0004
    float unk31c;                      // 0x31c: 0.0004
    float unk320;                      // 0x320: 1.5
    float unk324;                      // 0x324: 2.0
    float unk328;                      // 0x328
    float share;                       // 0x32c: drive torque share
};

#endif
