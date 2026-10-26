// Car rigid-body integration and collision-model setup (0x41c390-0x41ca6f) plus the Vec3 helpers at 0x4203b0-0x42044f.
#include "unk_481390.h"

struct CollisionPart {                 // a "collision" ModelFilePart
    BYTE unk0[0xa4];
    int id;                            // 0xa4
};
class CollisionModel {                 // 0x34 bytes
public:
    CollisionModel(CollisionPart* part, int a);        // 0x41b7f0
    CollisionModel(CollisionPart* part, int a, int b); // 0x41b8c0
    BYTE data[0x34];
};
struct Raw3 { float x, y, z; };         // plain copy (no constructors)
struct Quat {
    float x, y, z, w;
    void ToMatrix(float* m);           // 0x4b5240
};

class PhysBody {
public:
    BYTE unk0[0x14];
    Vec3 pos;                          // 0x14
    float m[16];                       // 0x20: rows 0x10 apart, translation in row 3
    BYTE unk60[0x130 - 0x60];
    float t130;                        // 0x130
    BYTE unk134[0x14c - 0x134];
    Quat q;                            // 0x14c
    float basis[12];                   // 0x15c: rows 0x10 apart
    BYTE unk18c[0x19c - 0x18c];
    Vec3 v19c;                         // 0x19c
    Vec3 v1a8;                         // 0x1a8
    BYTE unk1b4[0x1cc - 0x1b4];
    DWORD v1cc[3];                     // 0x1cc
    Vec3 v1d8;                         // 0x1d8
    BYTE unk1e4[0x214 - 0x1e4];
    float dt;                          // 0x214
    float a, b, c;                     // 0x218: offset of the reference point in the body frame
    DWORD v224[3];                         // 0x224
    int id230;                         // 0x230
    CollisionModel* collision;         // 0x234
    void Refresh();                    // 0x41bed0
    void FUN_0041c390();               // 0x41c390
    void FUN_0041c540();               // 0x41c540
    void SetCollision(CollisionPart* part);       // 0x41c920
    void SetCollision2(CollisionPart* part);      // 0x41c990
    void SetCollision3(CollisionPart* part);      // 0x41ca00
};
