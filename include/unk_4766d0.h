/* WindowRain (0x4766d0-0x47736f): rain drops on the windscreen, one per
 * viewport owner (owner+0x2004), 0x240 bytes, vtable 0x4e206c. Map:
 * analysis/range-4741a0.md section 2.6. Include after rallytrophy.h,
 * unk_481390.h, unk_47c460.h and unk_476490.h. */
#ifndef UNK_4766D0_H
#define UNK_4766D0_H

// A vertex of the window mesh as Init records it (0x2c bytes, new[] with the empty ctor/dtor
// folded to 0x490ad0/0x402c90).
struct RainSpot {
    RainSpot() {}
    ~RainSpot() {}
    float angleA;                      // 0x00: azimuth of the vertex seen from view 0 (100.0 = out of range)
    float angleB;                      // 0x04: same for view 1
    Vec3 world;                        // 0x08: the window mesh vertex
    Vec3 right;                        // 0x14: (1, 0, 0), scaled by the drop size in Render
    Vec3 up;                           // 0x20: (0, 1, 0)
};

// One rain drop (8 bytes, a raw new[] array): its age (0..1) and the window vertex it runs from.
struct RainDrop {
    float age;                         // 0x00
    RainSpot* spot;                    // 0x04
};

// The first 44 bytes of a mesh data block: box and bookkeeping, copied as one block.
struct MeshBounds {
    int w[11];
};

// The window mesh's data block (returned by Sprite-like object method 0x4a6800).
struct WindowMeshData {
    MeshBounds bounds;                 // 0x00
    BYTE flags;                        // 0x2c
    BYTE unk2d[0x40 - 0x2d];
    int vertexCount;                   // 0x40
    BYTE unk44[4];
    Vec3* vertices;                    // 0x48
};

// The object that owns the window mesh (a Stage scene object).
class WindowSource {
public:
    WindowMeshData* GetMeshData(int index); // 0x4a6800 (thiscall on the member at +0x124)
};

// A Stage scene object: only what WindowRain::Init reads.
struct SceneObject {
    BYTE unk0[4];
    int type;                          // 0x04: 1 = mesh object
    BYTE unk8[0x14 - 8];
    int unk14;                         // 0x14
    int unk18;                         // 0x18
    int unk1c;                         // 0x1c
    float matrix[16];                  // 0x20
    BYTE unk60[0x118 - 0x60];
    const char* name;                  // 0x118: the std::string's data pointer (null = empty)
    BYTE unk11c[0x124 - 0x11c];
    WindowSource source;               // 0x124
};

// The Stage's scene object list.
struct RainScene {
    BYTE unk0[0x48];
    std::vector<SceneObject*> objects; // 0x48 (first at 0x4c, last at 0x50)
};

class WindowRain : public EffectBase {
public:
    WindowRain(int dropCount);         // 0x4766d0 (scalar deleting dtor 0x476a70)
    ~WindowRain();                     // 0x476a90
    void AddView(int unused, const Vec3* position, float a, float b, float c, float d); // 0x476b70
    void Init(RainScene* scene);       // 0x476be0
    void Update(float dt, float xMin, float xMax, float yMin, float yMax); // 0x476fa0
    virtual void vf6(RenderDevice* device, Camera* camera, int unused); // 0x477080
    virtual void Render(RenderDevice* device, Camera* camera); // 0x4770a0

    BYTE unk4[4];
    int unk8;                          // 0x08: zeroed by the ctor
    BYTE unkc[8];
    int unk14;                         // 0x14: Init copies of three dwords of the window object
    int unk18;                         // 0x18
    int unk1c;                         // 0x1c
    float matrix[16];                  // 0x20: the window object's transform
    BYTE unk60[0xa0 - 0x60];
    MeshBounds bounds;                 // 0xa0: copied from the window mesh data block
    BYTE flags;                        // 0xcc
    BYTE unkcd[0x124 - 0xcd];
    Sprite sprite;                     // 0x124
    int viewCount;                     // 0x1f8: 0..2 split-screen views
    Vec3 viewPos[2];                   // 0x1fc
    Vec2 viewRange[2];                 // 0x214
    Vec2 viewSize;                     // 0x224: half sizes of the view (its empty dtor gives the ctor/dtor an unwind entry)
    DynamicMesh* mesh;                 // 0x22c
    int dropCount;                     // 0x230
    int spotCount;                     // 0x234: vertices of the window mesh
    RainDrop* drops;                   // 0x238
    RainSpot* spots;                   // 0x23c
};

#endif
