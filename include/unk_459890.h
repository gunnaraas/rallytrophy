/* Car damage helpers (0x459890-0x45ad0f). Map: analysis/range-459890.md section 1.
 * Include after unk_481390.h and rallytrophy.h. The Car members of this module read their fields through
 * CarDamageView, a raw view of the Car (offsets from CarObject in unk_45e480.h). */
#ifndef UNK_459890_H
#define UNK_459890_H

struct DmgVec3 {
    float x, y, z;
};
// One contact record at Car+0x1c914 (0x28 bytes).
struct DmgContact {
    float strength;                    // 0x00
    DmgVec3 a;                         // 0x04
    DmgVec3 b;                         // 0x10
    BYTE unk1c[0x28 - 0x1c];           // 0x1c: written by the contact flush 0x459910
};

// A mesh handle of a scene object's mesh list (0x4a6800 returns the mesh at an index).
class DmgMesh {
public:
    void Repair();                     // 0x4b9530
    int Find(void* name);              // 0x4b9790
};
class DmgMeshList {
public:
    DmgMesh* At(int index);            // 0x4a6800
};
struct DmgBody {
    BYTE unk0[0x124];
    DmgMeshList meshes;                // 0x124
};
// A light or glow sprite attached to the body.
class DmgSprite {
public:
    void SetParent(void* parent);      // 0x4b2b30
    BYTE unk0[0x14];
    BYTE name[1];                      // 0x14
};
// One lamp record (Car+0x1bcf0, stride 0x10).
struct DmgLamp {
    int* target;                       // 0x00
    int off;                           // 0x04
    int on;                            // 0x08
    BYTE state;                        // 0x0c
    BYTE unkd[3];
};
// One light record (Car+0x1bd40..., stride 0x1c).
struct DmgLight {
    DmgSprite* sprite;                 // 0x00
    BYTE dead;                         // 0x04
    int mesh;                          // 0x08
    int* target;                       // 0x0c
    int value;                         // 0x10
    BYTE unk14[0x1c - 0x14];
};
struct DmgWheel {
    BYTE unk0[0x2a0];
    float brake;                       // 0x2a0 (Car+0x2c8): brake damage
    float steer;                       // 0x2a4 (Car+0x2cc): steering damage
    BYTE unk2a8[0x330 - 0x2a8];
};
struct DmgSpring {
    BYTE unk0[0xc];
    float a;                           // 0x0c
    float b;                           // 0x10
    float c;                           // 0x14
    BYTE unk18[0x34 - 0x18];
};
struct DmgMarker {
    BYTE unk0[0x38];
    int state;                         // 0x38
};

struct CarDamageView {
    BYTE unk0[0x8];
    int setup;                         // 0x08: setup index into the car.ini tables
    BYTE unkc[0x24 - 0xc];
    union {
        DmgBody* body;                 // 0x24
        DmgBody* volatile bodyV;       // 0x24 (volatile: the original re-reads it after every call)
    };
    DmgWheel wheel[4];                 // 0x28
    DmgSpring spring[4];               // 0xce8
    BYTE unkdb8[0x114c - 0xdb8];
    float tabA[5];                     // 0x114c (front grip base per setup)
    BYTE unk1160[0x11b8 - 0x1160];
    float tabB[5];                     // 0x11b8
    float tabC[5];                     // 0x11cc
    BYTE unk11e0[0x1210 - 0x11e0];
    float tabD[5];                     // 0x1210 (rear)
    BYTE unk1224[0x127c - 0x1224];
    float tabE[5];                     // 0x127c
    float tabF[5];                     // 0x1290
    BYTE unk12a4[0x1f68 - 0x12a4];
    DmgBody* hoodCrash;                // 0x1f68
    DmgBody* trunkCrash;               // 0x1f6c
    BYTE unk1f70[0x1bcf0 - 0x1f70];
    DmgLamp lamp[4];                   // 0x1bcf0
    DmgMarker* marker;                 // 0x1bd30
    BYTE unk1bd34[0x1bd40 - 0x1bd34];
    DmgLight lightFrontLeft[3];        // 0x1bd40
    DmgLight lightFrontRight[3];       // 0x1bd94
    DmgLight lightRear[2];             // 0x1bde8
    DmgLight lightBrake[2];            // 0x1be20
    DmgLight lightReverse[2];          // 0x1be58
    BYTE unk1be90[0x1be9c - 0x1be90];
    DmgSprite* glow[8];                // 0x1be9c
    BYTE unk1bebc[0x1c730 - 0x1bebc];
    int unk1c730;                      // 0x1c730
    int unk1c734;                      // 0x1c734
    int noFrontLight;                  // 0x1c738
    float wheelDamage[4];              // 0x1c73c
    float zone0;                       // 0x1c74c
    float zone2;                       // 0x1c750
    float zone3;                       // 0x1c754
    float zone1;                       // 0x1c758
    BYTE unk1c75c[0x1c900 - 0x1c75c];
    BYTE latch[8];                     // 0x1c900
    BYTE unk1c908[0x1c914 - 0x1c908];
    DmgContact contacts[0x100];        // 0x1c914
    int contactCount;                  // 0x1f114 (after the 256 records)
};

#endif
