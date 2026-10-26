/* TexScroller (0x4764c0-0x4766cf): scrolls the texture coordinates of a Stage
 * mesh (material animation kinds 1, 3 and 5 of the Stage loader), and the
 * dynamic mesh class it shares with WindowRain. Map: analysis/range-4741a0.md
 * section 2.4. Include after rallytrophy.h, unk_481390.h and unk_47c460.h. */
#ifndef UNK_476490_H
#define UNK_476490_H

// A mesh of 0x2c-byte vertices drawn by the device (ctor 0x4baf40, dtor 0x4baf90, 0x38 bytes).
class DynamicMesh {
public:
    DynamicMesh();                     // 0x4baf40
    ~DynamicMesh();                    // 0x4baf90
    Sprite* material;                  // 0x00
    EffectVertex* vertices;            // 0x04: new[] array of vertexCount vertices
    WORD* indices;                     // 0x08
    WORD vertexCount;                  // 0x0c
    WORD triangleCount;                // 0x0e
    BYTE unk10[8];
    void* owner;                       // 0x18
    BYTE unk1c[8];
    int unk24;                         // 0x24
    BYTE unk28[0x38 - 0x28];
};

// Base of the Stage animators (ctor 0x4bb130, vtable 0x4e1fb4: slot 0 dtor 0x46fe90, slot 1 pure virtual).
class StageAnimator {
public:
    StageAnimator();                   // 0x4bb130
    virtual ~StageAnimator() {}
    virtual void Update() = 0;
    float stamp;                       // 0x04: clock time of the last Update
};

// The global clock (DAT_0056b0ac); only the time stamp is read.
struct StageClock {
    BYTE unk0[0x24];
    float time;                        // 0x24
};

class TexScroller : public StageAnimator {
public:
    TexScroller(DynamicMesh* mesh, float scaleU, float scaleV, float speedU, float speedV); // 0x4764c0 (scalar deleting dtor 0x476590)
    ~TexScroller();                    // 0x4765b0
    virtual void Update();             // 0x4765d0

    float* base;                       // 0x08: the mesh's original v values
    DynamicMesh* mesh;                 // 0x0c
    float speedU;                      // 0x10
    float speedV;                      // 0x14
    float offsetU;                     // 0x18
    float offsetV;                     // 0x1c
    float scrollU;                     // 0x20
    float scrollV;                     // 0x24
    float scaleU;                      // 0x28
    float scaleV;                      // 0x2c
};

#endif
