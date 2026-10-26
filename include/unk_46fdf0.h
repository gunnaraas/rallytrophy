/* ParallelShadow (0x46fdf0-0x472c8f): the car shadow renderer owned by the Stage at +0x20ec
 * (0x330 bytes, vtable 0x4e1fc8), its software scan converter and the file statics.
 * Map: analysis/range-46fdf0.md.
 * Include after rallytrophy.h, unk_481390.h, unk_47c460.h, unk_47b4c0.h and unk_474c50.h.
 *
 * Several file statics are declared twice under different names and types: the code of the
 * different parts of the module views the same memory differently (DAT_ names are the
 * typed-by-address views, g_ names the structured ones). Both generate the same absolute
 * addressing, so the duplicates are kept. */
#ifndef UNK_46FDF0_H
#define UNK_46FDF0_H

// Abstract base with vtable 0x4e1fb4 (slot 0 the scalar deleting destructor 0x46fe90, slot 1
// _purecall). The destructor body 0x46fe80 is emitted out of line without callers. The real
// name is unknown.
class ShadowInterface {
public:
    virtual ~ShadowInterface() {} // 0x46fe80 (body reproduced by a standalone ABI helper); scalar deleting destructor 0x46fe90
    virtual void Pure() = 0;
};

// A projected shadow vertex (0x14 bytes) with empty ctor/dtor: array of 0x4000 at 0x5096f8.
struct ProjVertex {
    ProjVertex() {}
    ~ProjVertex() {}
    float x;                           // 0x00: shadow-plane x
    float y;                           // 0x04: shadow-plane y (non-negative: compared as an int)
    int fx;                            // 0x08: x in 16.16 fixed point
    int fy;                            // 0x0c: y in 16.16 fixed point
    int visited;                       // 0x10: projected this frame
    int yBits() const { return *(const int*)&y; }
};

// A clip plane (0x10 bytes): normal and distance. Array of three at 0x509500.
struct ClipPlane {
    ClipPlane() {}
    ~ClipPlane() {}
    float a, b, c, d;
};

// Colour look-up tables built by BuildColourLuts (0x4701b0) from a light colour; read by the car code
// and by ProjectMeshes. 0x561af8..0x561b04 is the light colour (r, g, b) and its weighted average.
extern float DAT_00561af8, DAT_00561afc, DAT_00561b00, DAT_00561b04;
extern BYTE g_shadowLutR[256];         // 0x5619f8
extern BYTE g_shadowLutG[256];         // 0x5618f8
extern BYTE g_shadowLutB[256];         // 0x5617f8

Vec3 operator*(float k, const Vec3& v); // 0x420430 (cdecl, hidden return pointer)

// A clear colour passed by value to the renderer (empty ctor/dtor: gives its users an EH state).
class ShadowColor {
public:
    ShadowColor() {}
    ShadowColor(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
    ~ShadowColor() {}
    float x, y, z, w;
};

struct ShadowMesh;
// The renderer plugin's slots the shadow uses (module-local view of DAT_0056b0ac).
#define SHADOW_SLOT(n) virtual void Slot##n();
class ShadowDevice {
public:
    SHADOW_SLOT(0)
    SHADOW_SLOT(1)
    SHADOW_SLOT(2)
    SHADOW_SLOT(3)
    SHADOW_SLOT(4)
    SHADOW_SLOT(5)
    SHADOW_SLOT(6)
    SHADOW_SLOT(7)
    SHADOW_SLOT(8)
    SHADOW_SLOT(9)
    SHADOW_SLOT(10)
    SHADOW_SLOT(11)
    SHADOW_SLOT(12)
    SHADOW_SLOT(13)
    virtual void SetViewport(int x, int y, int w, int h);        // 0x38
    virtual void GetViewport(int* x, int* y, int* w, int* h);    // 0x3c
    SHADOW_SLOT(16)
    SHADOW_SLOT(17)
    SHADOW_SLOT(18)
    SHADOW_SLOT(19)
    SHADOW_SLOT(20)
    SHADOW_SLOT(21)
    virtual void Clear(ShadowColor color);                       // 0x58
    SHADOW_SLOT(23)
    virtual void Slot24(int value);                              // 0x60
    SHADOW_SLOT(25)
    SHADOW_SLOT(26)
    virtual int BeginScene();                                    // 0x6c
    virtual void EndScene();                                     // 0x70
    virtual void Slot29();                                       // 0x74
    virtual void Slot30();                                       // 0x78: leave the render target
    virtual void SetRenderTarget(Texture* texture, int a, int b, int c); // 0x7c
    SHADOW_SLOT(32)
    virtual void SetRenderState(int state, int value);           // 0x84
    SHADOW_SLOT(34)
    virtual void SetMaterial(Sprite* material);                  // 0x8c
    SHADOW_SLOT(36)
    virtual void Slot37();                                       // 0x94
    virtual void Slot38(int value);                              // 0x98
    SHADOW_SLOT(39)
    SHADOW_SLOT(40)
    SHADOW_SLOT(41)
    virtual void Slot42(int value);                              // 0xa8
    SHADOW_SLOT(43)
    SHADOW_SLOT(44)
    SHADOW_SLOT(45)
    virtual void SetWorldMatrix(const float* m, int flag);       // 0xb8
    SHADOW_SLOT(47)
    virtual void SetViewMatrix(const float* m, int flag);        // 0xc0
    SHADOW_SLOT(49)
    virtual void SetOrtho(const float* m, float a, float b, float c, int flag); // 0xc8
    SHADOW_SLOT(51)
    SHADOW_SLOT(52)
    SHADOW_SLOT(53)
    SHADOW_SLOT(54)
    SHADOW_SLOT(55)
    SHADOW_SLOT(56)
    SHADOW_SLOT(57)
    SHADOW_SLOT(58)
    SHADOW_SLOT(59)
    SHADOW_SLOT(60)
    SHADOW_SLOT(61)
    SHADOW_SLOT(62)
    SHADOW_SLOT(63)
    virtual void DrawMesh(ShadowMesh* mesh);                     // 0x100
};
#undef SHADOW_SLOT

// The factory the engine hands to Create (Engine+0x20); the slots allocate texture storage.
class ShadowFactory {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual int GetFormat(int a, int b);                         // 0x08
    virtual void AdjustSize(int* w, int* h, int flag);           // 0x0c: round the size up to what the device supports
    virtual void Slot4();
    virtual void Slot5();
    virtual int CreateRenderTarget(Texture* texture, int w, int h, int flag); // 0x18
    virtual int CreateTexture(Texture* texture, int format, int w, int h, int flags); // 0x1c
    virtual Texture* CreateSystemTexture(Texture* texture, int format, int w, int h, int flags); // 0x20
};

// The engine argument of Create: only the factory is read.
class ShadowEngine {
public:
    BYTE unk0[0x20];
    ShadowFactory* factory;            // 0x20
};

// The locked pixels of the system-memory texture (4a84c0).
struct ShadowSurface {
    int size;                          // 0x00: edge length in pixels
    int width;                         // 0x04
    int pitch;                         // 0x08: bytes per row (the texture is square: the clear covers pitch * width bytes)
    BYTE unkc[0x18 - 0xc];
    BYTE* bits;                        // 0x18: first scanline
};

// View of the 0x68-byte texture object for its non-virtual methods.
class ShadowTexture {
public:
    ShadowSurface* Lock();             // 0x4a84c0
    void Unlock();                     // 0x4a8500
    void CopyFrom(ShadowTexture* source, int x, int y); // 0x4a8490
};

// The render target texture's first fields (0x18 and 0x1c are non-zero once the target exists).
class ShadowTarget {
public:
    BYTE unk0[0x18];
    void* unk18;                       // 0x18
    void* unk1c;                       // 0x1c
};

// A vertex of a shadow caster mesh (0x2c bytes) and of the projected shadow mesh built from it.
struct ShadowVertex {
    float x, y, z;                     // 0x00
    float nx, ny, nz;                  // 0x0c: normal (input); the projection pushes the position out along it
    BYTE colour[4];                    // 0x18: input colour bytes, mapped through the colour LUTs on output
    float u, v;                        // 0x1c, 0x20: texture coordinates in the shadow plane (output)
    BYTE unk24[0x2c - 0x24];
    const Vec3* Pos() const { return (const Vec3*)this; } // the position as a Vec3
};

// A face record of the caster: normal, bounding sphere (0x20 bytes).
struct ShadowFace {
    float nx, ny, nz;                  // 0x00
    float unkc;                        // 0x0c
    float cx, cy, cz;                  // 0x10: sphere centre
    float radius;                      // 0x1c
};

// A mesh of the caster (the Mesh layout of unk_474c50.h with the caster's vertex type): vertex and
// triangle-index arrays.
struct ShadowMeshInfo {
    BYTE unk0[0x1c];
    int type;                          // 0x1c: 1 = drawn into the shadow
};
struct ShadowMesh {
    ShadowMeshInfo* info;              // 0x00
    ShadowVertex* vertices;            // 0x04
    WORD* indices;                     // 0x08: 3 WORDs per triangle
    WORD vertexCount;                  // 0x0c
    WORD faceCount;                    // 0x0e: triangles
    BYTE unk10[0x24 - 0x10];
    int unk24;                         // 0x24: cleared while the mesh is drawn into the shadow
    BYTE unk28[0x38 - 0x28];
};

// A node of the caster's mesh list (linked through +0x6c; 0x4a6800 returns the node of an index).
struct ShadowMeshNode {
    BYTE unk0[0x30];
    std::vector<ShadowMesh*> meshes;   // 0x30 (first at 0x34, last at 0x38)
    BYTE unk40[0x6c - 0x40];
    ShadowMeshNode* next;              // 0x6c
};
// The same node seen through the raw mesh vector pointers (RasteriseObject reads it this way; the
// std::vector accessors of ShadowMeshNode compile differently there).
struct ShadowMeshNodeRaw {
    BYTE unk0[0x34];
    ShadowMesh** begin;                // 0x34
    ShadowMesh** end;                  // 0x38
    unsigned Size() const { return begin ? end - begin : 0; }
};
class ShadowMeshList {
public:
    ShadowMeshNode* At(int index);     // 0x4a6800 (thiscall; the list head is the first member)
    ShadowMeshNode* head;
};
struct ShadowFaceTable {
    BYTE unk0[4];
    ShadowFace** faces;                // 0x04: one face array per mesh
};

// An object that casts a shadow (a car, a Stage scene object; also the "lead" object the module is
// handed the camera as): position, world matrix, bounding sphere, mesh list and face table.
class ShadowCaster {
public:
    int IsCurrent();                   // 0x4b3030: [this+0x10] == DAT_00502470
    BYTE unk0[4];
    int type;                          // 0x04
    BYTE unk8[4];
    BYTE flags;                        // 0x0c: bit 2 = visible
    BYTE unkd[0x14 - 0xd];
    Vec3 position;                     // 0x14
    BYTE unk20[0x60 - 0x20];
    ViewMat world;                     // 0x60
    BYTE unka0[0xb8 - 0xa0];
    Vec3 center;                       // 0xb8
    float radius;                      // 0xc4
    BYTE unkc8[0x124 - 0xc8];
    ShadowMeshList meshList;           // 0x124
    ShadowFaceTable* faceTable;        // 0x128
};

// The shadow plane state written by Prepare (0x470940) and read by the back ends and ProjectMeshes
// (original statics at 0x509500-0x5096f8).
extern Vec3 DAT_00509500;              // clip plane 0 normal, distance at 0x50950c
extern float DAT_0050950c;
extern Vec3 DAT_00509510;              // clip plane 1, distance at 0x50951c
extern float DAT_0050951c;
extern Vec3 DAT_00509520;              // clip plane 2, distance at 0x50952c
extern float DAT_0050952c;
extern Vec3 DAT_00509540;
extern Vec3 DAT_00509550;
extern Vec3 DAT_00509560;
extern Vec3 DAT_00509570;
extern Vec3 DAT_005096a0, DAT_005096b0;
extern ViewMat DAT_005095c0;
extern ViewMat DAT_00509600;
extern ViewMat DAT_00509640;
extern ViewMat DAT_00509680;
extern float DAT_005096d4;
extern float DAT_005096d8;
extern float DAT_005096dc, DAT_005096e0;
extern int DAT_005012e4;                // nonzero: the device can render into textures

// The same memory as the structured views ProjectMeshes uses.
struct ShadowPlane {
    float nx, ny, nz, d;
};
struct ShadowAxes {                    // 0x509540
    float vx, vy, vz;                  // 0x509540: texture v axis
    float unk0c;
    float ux, uy, uz;                  // 0x509550: texture u axis
};
struct ShadowSphere {                  // 0x5096e4
    float radius;
    Vec3 centre;                       // 0x5096e8
};
extern ShadowPlane g_shadowPlanes[3];  // 0x509500
extern ShadowAxes g_shadowAxes;        // 0x509540
extern int g_shadowVertexBase;         // 0x5096c0: next output vertex index
extern ShadowVertex* g_shadowVertexOut; // 0x5096c4
extern WORD* g_shadowIndexOut;         // 0x5096c8
extern int g_shadowVertexCount;        // 0x5096cc
extern int g_shadowTriangleCount;      // 0x5096d0
extern ShadowSphere g_shadowSphere;    // 0x5096e4
extern WORD g_shadowRemap[0x4000];     // 0x5596f8

// Scan-converter state (file statics 0x5094d0-0x50953c; written by the software back end).
extern ProjVertex* g_tri[3];           // 0x5094d0: the three projected vertices of the current triangle
extern int g_fillColor32;              // 0x509530
extern WORD g_fillColor16;             // 0x509534
extern int g_pitch;                    // 0x509538
extern BYTE* g_scanline;               // 0x50953c

// Shadow transform statics used by the rasteriser.
extern ViewMat DAT_00509580;
extern Vec3 g_lightDir;                // 0x5095a0
extern Vec3 g_origin;                  // 0x5095b0
extern ProjVertex g_proj[0x4000];      // 0x5096f8

class ParallelShadow : public EffectBase {
public:
    ParallelShadow();                  // 0x4703c0
    virtual ~ParallelShadow();         // 0x470560 (scalar deleting dtor 0x470540)
    void Create(ShadowEngine* engine, int size); // 0x470610: allocate the shadow texture (hardware target or system memory)
    virtual void Render(RenderDevice* device, Camera* camera); // 0x4708e0 (slot 5)
    void Prepare(RenderDevice* device, void* unused, DrawContext* context, Camera* camera, int unused2); // 0x470940
    void RasteriseSoftware(ShadowCaster* lead);   // 0x471650: software back end
    void RenderToTexture(ShadowCaster* lead);     // 0x4717e0: hardware back end
    void ProjectMeshes(ShadowCaster* caster);     // 0x471ab0
    void RasteriseObject(ShadowCaster* obj);      // 0x4721b0
    void DrawCaster(ShadowCaster* caster);        // 0x472bb0
    void SetLightMatrix(const float* m);          // 0x472c20
    void SetCaster(ShadowCaster* caster, int index); // 0x472c40

    BYTE unk4[4];                      // 0x04
    int unk8;                          // 0x08: 1
    BYTE flags;                        // 0x0c: bit 0 set by the constructor and Update
    BYTE unkd[0xcc - 0xd];
    BYTE flagsCC;                      // 0xcc: engine base flags (low two bits cleared by the constructor)
    BYTE unkcd[0x124 - 0xcd];
    ShadowCaster* casters[4];          // 0x124
    Sprite spriteA;                    // 0x134: the shadow texture ("ParallelShadow") as a material
    ShadowSurface* surface;            // 0x208: locked system texture (software back end)
    Texture* sysTexture;               // 0x20c: system-memory texture ("sys")
    ViewMat lightMatrix;               // 0x210: SetLightMatrix copy (rows right/up/forward)
    Mesh* mesh;                        // 0x250
    int format;                        // 0x254: texture format 1..6, 7
    Sprite spriteB;                    // 0x258: material of the hardware pass; its unk60[0..2] (0x2b8) is the shadow colour, 0..1
    BYTE is32bit;                      // 0x32c: the software path writes 32-bit pixels (formats 4 and 7)
    BYTE ready;                        // 0x32d: Create succeeded
    BYTE unk32e[2];

    Vec3& LightRow(int i) { return *(Vec3*)&lightMatrix.m[i * 4]; } // rows right/up/forward (row 2 = light direction)
};

void FillTriangle(ProjVertex** tri);      // 0x472710 (cdecl, 16-bit pixels)
void StepEdgeA();                         // 0x4728b0: advance the edge that walks the triangle forwards
void StepEdgeB();                         // 0x472970: advance the edge that walks it backwards
void FillTriangleFlat(ProjVertex** tri);  // 0x472a30 (cdecl, 32-bit pixels)

void BuildColourLuts(const Vec3* light); // 0x4701b0

#endif
