/* CarRain (0x46d830-0x46e84f): rain drops running over the car body, owned by the Viewport at
 * +0x2008 (0x300 bytes, vtable 0x4e1f3c). Map: analysis/range-46d830.md section 3.
 * Include after rallytrophy.h, unk_481390.h, unk_47c460.h, unk_476490.h and unk_4766d0.h. */
#ifndef UNK_46D830_H
#define UNK_46D830_H

// A 4x4 float matrix (row vectors).
struct CarMatrix {
    CarMatrix() {}                     // empty ctor/dtor: each matrix member is one unwind state of CarRain
    ~CarMatrix() {}
    float m[16];
    void Multiply(const CarMatrix* other); // 0x4b55e0 (thiscall, callee pops the argument)
};

// The 44 bytes at +0xa0 of an effect: bounding box (6 floats), bookkeeping and the radius at +0xc4.
struct CarBounds {
    union {
        int w[11];
        float f[11];
    };
};

// A scene object as SetBounds and Render read it: the world matrix at +0x60, the bounds block at +0xa0.
struct CarBodyObject {
    BYTE unk0[0x60];
    CarMatrix world;                   // 0x60
    CarBounds bounds;                  // 0xa0
    BYTE flags;                        // 0xcc
};

// One drop (0x18 bytes, new[] with the empty ctor/dtor folded to 0x490ad0/0x402c90).
struct CarDrop {
    CarDrop() {}
    ~CarDrop() {}
    float phase;                       // 0x00: 0..1, the drop respawns when it reaches 1
    float size;                        // 0x04
    Vec3 pos;                          // 0x08: vertex of one of the three rain meshes
    CarMatrix* matrix;                 // 0x14: the live matrix that places the vertex
};

class CarRain : public EffectBase {
public:
    CarRain(int dropCount);            // 0x46d830 (scalar deleting dtor 0x46dc50)
    ~CarRain();                        // 0x46dc70
    void Init(RainScene* scene);       // 0x46dd80
    void SetBounds(CarBodyObject* body, CarBodyObject* trunk, CarBodyObject* hood); // 0x46e1a0
    void Update(float dt);             // 0x46e2d0
    virtual void Render(RenderDevice* device, Camera* camera); // 0x46e430
    void Respawn(CarDrop* drop);       // inlined into Init and Update

    BYTE unk4[4];
    int unk8;                          // 0x08: zeroed by the ctor
    BYTE unkc[0xa0 - 0x0c];
    CarBounds bounds;                  // 0xa0: copied from the body object by SetBounds (box x1.1)
    BYTE flags;                        // 0xcc
    BYTE unkcd[0x124 - 0xcd];
    Sprite* texture;                   // 0x124: set by the viewport (base setter 0x4b2b30)
    DynamicMesh* mesh;                 // 0x128: 4 vertices and 6 indices per drop
    int dropCount;                     // 0x12c
    CarDrop* drops;                    // 0x130
    Vec3* bodyVerts;                   // 0x134: counted copies of the rain_body/hood/trunk vertices
    Vec3* hoodVerts;                   // 0x138
    Vec3* trunkVerts;                  // 0x13c
    int bodyCount;                     // 0x140
    int hoodCount;                     // 0x144
    int trunkCount;                    // 0x148
    int totalCount;                    // 0x14c
    // Nine members with empty ctors/dtors: the nine unwind states of the ctor and dtor (separate
    // members, not arrays, which would use the vector iterators).
    CarMatrix liveBody;                // 0x150: matrices of this frame
    CarMatrix liveHood;                // 0x190
    CarMatrix liveTrunk;               // 0x1d0
    CarMatrix savedBody;               // 0x210: matrices of the rain meshes (hood and trunk start as identity)
    CarMatrix savedHood;               // 0x250
    CarMatrix savedTrunk;              // 0x290
    Vec3 bodyPos;                      // 0x2d0: translations of the rain meshes
    Vec3 hoodPos;                      // 0x2dc
    Vec3 trunkPos;                     // 0x2e8
    CarBodyObject* bodyObject;         // 0x2f4
    CarBodyObject* hoodObject;         // 0x2f8
    CarBodyObject* trunkObject;        // 0x2fc
};

#endif
