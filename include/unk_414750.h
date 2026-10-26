#ifndef UNK_414750_H
#define UNK_414750_H
// Types for 0x414750-0x414fb0 (stage checkpoints / zones); layouts inferred from pseudocode.
#include <vector>
#include <string>
#include "unk_481390.h"

// Base of the 0x4e13d8 class: ctor 0x416df0, dtor 0x416e30 (other TU).
class StageBase {
public:
    StageBase(int a);
    virtual ~StageBase();
    int pad[0x10];
};
class StageDerived : public StageBase {   // vtable 0x4e13d8
public:
    StageDerived(int a);                  // 0x4147f0
    ~StageDerived();                      // 0x414830, scalar deleting 0x414810
};

// Virtual-destructor record whose scalar deleting dtor is 0x4147a0.
class CpRecord { public: CpRecord(); virtual ~CpRecord(); };

// A camera zone of the stage.
struct ZoneInfo {
    BYTE unk0[0x14];
    float pos[3];                         // 0x14
    float axisA[3];                       // 0x20 (stride 4 per component, rows at 0x20/0x30/0x40)
    BYTE unk2c[0x4];
    float axisB[3];                       // 0x30
    BYTE unk3c[0x4];
    float axisC[3];                       // 0x40
    BYTE unk4c[0x128 - 0x4c];
    float radius;                         // 0x128
    BYTE unk12c[0x1c8 - 0x12c];
    char always;                          // 0x1c8
};
struct ZoneStage {
    BYTE unk0[0x7c];
    ZoneInfo** first;                     // 0x7c
    ZoneInfo** last;                      // 0x80
    ZoneInfo* FindZone(const float* p);   // 0x414870
};

// 0x48-byte checkpoint record with an (empty) destructor.
struct TrackPoint {
    float m[12];                          // 0x00 matrix rows
    float pos[3];                         // 0x30
    float m3c;                            // 0x3c
    float distance;                       // 0x40
    int index;                            // 0x44
    TrackPoint() {}
    ~TrackPoint() {}
};
struct TrackBody { BYTE unk0[0x60]; float m[16]; };
struct TrackObj { BYTE unk0[0x24]; TrackBody* body; };
struct TrackOwner {
    BYTE unk0[0x9ec];
    TrackObj* obj;                        // 0x9ec
    std::vector<TrackPoint> pts;          // 0x9f0
    void FUN_004149d0();
};
// Stage object: loads data/reset<stage id>.bin (count + 0x48-byte checkpoint records).
class StageResetLoader {
public:
    void LoadReset();                     // 0x414b20
};
// VC6 reference-counted string layout used by the buffer freeze helper.
struct FreezeStringData {
    char allocator;
    char* data;
    unsigned int length, capacity;
    void Xlen();                     // 0x4bd00b: string length error
    void Copy(unsigned int n);       // 0x402ab0: detach a shared buffer
    void Freeze();
};
#endif
