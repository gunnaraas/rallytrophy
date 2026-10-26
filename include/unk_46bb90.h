/* CarSnapshot (0x46bb90-0x46c28f): the 0x8c-byte car state saved/restored by Car::SaveSnapshot /
 * Car::RestoreSnapshot (3 slots at Car+0x1c75c). Map: analysis/range-46a8a0.md section 3.
 * Include after rallytrophy.h and unk_481390.h.
 * The Car members are written against CarSnapView, a raw view of the Car fields the snapshot touches
 * (Car itself is not restructured). */
#ifndef UNK_46BB90_H
#define UNK_46BB90_H

// The effect helpers the snapshot calls on the light sprites (original: 0x4b2b90 / 0x4b2b30).
class SnapEffect {
public:
    void Show();                       // 0x4b2b90
    void SetParent(void* parent);      // 0x4b2b30
};

// A mesh container's mesh (0x4a6800 returns the one at an index; Save/Restore move its deformation data).
class SnapMesh {
public:
    void Save(void** buffer, int* size);          // 0x4b99c0
    void Restore(void* buffer, int size);         // 0x4b9a60
};
class SnapMeshList {
public:
    SnapMesh* At(int index);           // 0x4a6800
};
// A scene object with a mesh list at +0x124 (the car body, the two other body parts).
struct SnapObject {
    BYTE unk0[0x124];
    SnapMeshList meshes;               // 0x124
};

struct CarSnapshot {
    CarSnapshot();                     // 0x46bb90
    ~CarSnapshot();                    // 0x46bbb0
    void Clear();                      // 0x46bbc0
    BYTE setup[5];                     // 0x00: the five CarSetup values (default 2)
    BYTE unk5[3];
    float damage[3];                   // 0x08: Car+0x1c74c..
    int values[4];                     // 0x14: Car+0x1c73c..
    float angleB[4];                   // 0x24: wheel +0x2a4
    float angleA[4];                   // 0x34: wheel +0x2a0
    float unk44;                       // 0x44: Car+0x1c758
    BYTE unk48[0x64 - 0x48];
    BYTE lamps[4];                     // 0x64: state of the four lamp entries
    BYTE flagA;                        // 0x68
    BYTE flagB;                        // 0x69
    BYTE lightsA[3];                   // 0x6a
    BYTE lightsB[3];                   // 0x6d
    BYTE lights2A[2];                  // 0x70
    BYTE lights2B[2];                  // 0x72
    int size[3];                       // 0x74: byte sizes of the three deformation buffers
    BYTE* buffer[3];                   // 0x80
    float GetAngleA(int i) const { return angleA[i]; }
    float GetAngleB(int i) const { return angleB[i]; }
    float GetUnk44() const { return unk44; }
};

// Four lamp entries at Car+0x1bcf0 (0x10 bytes each): a target dword that is set to `on` or `off`.
struct SnapLamp {
    int* target;                       // 0x00
    int off;                           // 0x04
    int on;                            // 0x08
    BYTE state;                        // 0x0c
    BYTE unkd[3];
};
// A light (0x1c bytes): glow sprite, state byte and a target dword set to `on` or `off`.
struct SnapLight {
    SnapEffect* sprite;                // 0x00
    BYTE state;                        // 0x04
    BYTE unk5[7];
    int* target;                       // 0x0c
    int off;                           // 0x10
    int unk14;                         // 0x14
    int on;                            // 0x18
};
// The tail of a wheel (wheel + 0x28 + n * 0x330 inside the Car): angles at +0x2a0 / +0x2a4.
struct SnapWheel {
    BYTE unk0[0x2a0];
    float angleA;                      // 0x2a0
    float angleB;                      // 0x2a4
    BYTE unk2a8[0x330 - 0x2a8];
    float GetAngleA() const { return angleA; }
    float GetAngleB() const { return angleB; }
};
// Marker object whose +0x38 dword is set with the first lamp.
struct SnapMarker {
    BYTE unk0[0x38];
    int mode;                          // 0x38
};

// Raw view of the Car fields the snapshot reads and writes.
struct CarSnapView {
    BYTE unk0[0x24];
    SnapObject* body;                  // 0x24
    SnapWheel wheel[4];                // 0x28
    BYTE unkce8[0x1f68 - 0xce8];
    SnapObject* partB;                 // 0x1f68
    SnapObject* partA;                 // 0x1f6c
    BYTE unk1f70[0x1bcf0 - 0x1f70];
    SnapLamp lamp[4];                  // 0x1bcf0
    SnapMarker* marker;                // 0x1bd30
    BYTE unk1bd34[0x1bd40 - 0x1bd34];
    SnapLight light[6];                // 0x1bd40
    BYTE unk1bde8[0x1be20 - 0x1bde8];
    SnapLight light2[4];               // 0x1be20
    BYTE unk1be90[0x1be9c - 0x1be90];
    SnapEffect* glow[8];               // 0x1be9c
    BYTE unk1bebc[0x1c730 - 0x1bebc];
    void ApplyWheel(int n);            // 0x45a7a0 (thiscall on the Car)
    int flagA;                         // 0x1c730
    int flagB;                         // 0x1c734
    BYTE unk1c738[4];
    int values[4];                     // 0x1c73c
    float damage[3];                   // 0x1c74c
    float unk1c758;                    // 0x1c758
    float GetUnk1c758() const { return unk1c758; }
    CarSnapshot snapshot[3];           // 0x1c75c
};

#endif
