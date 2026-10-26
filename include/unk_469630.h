/* Car render transforms, lighting, shadow hook, surface mix and gauges (0x469630-0x46a89f).
 * Map: analysis/range-4666a0.md section 5. Include after unk_481390.h, rallytrophy.h and unk_46a8a0.h.
 * The Car members are written against the raw views below (Car itself is not restructured). */
#ifndef UNK_469630_H
#define UNK_469630_H

// The ParallelShadow object at Car+0x20ec (vtable slot 5 renders it); only the entry points the car uses.
class CarShadow {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void Render(RenderDevice* device, int arg);              // 0x14 (slot 5)
    void Prepare(RenderDevice* device, int a, void* context, void* caster, int flag); // 0x470940
    void SetLightMatrix(const float* m);                              // 0x472c20
    void SetCaster(void* caster, int index);                          // 0x472c40
};

extern int DAT_00501310;               // HUD gauges enabled

struct RenderBody;
// The shadow target embedded at Car+0x20f0.
struct CarShadowTarget {
    void Update(struct RenderBody* body, int a, const float* centre, int radius); // 0x4b9d30
};

struct RawVec3 {
    float x, y, z;
};
struct RawVec4 {
    RawVec4() {}
    RawVec4(float a, float b, float c, float d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    }
    float x, y, z, w;
};

// A scene object whose field +0x10 is compared with the current scene (0x4b3030).
struct SceneObject {
    int IsCurrent();                   // 0x4b3030
};

// A mesh container's meshes (0x4a6800 returns the one at an index; 0x4b95b0 refreshes it).
struct RenderMesh {
    void Refresh();                    // 0x4b95b0
};
struct RenderMeshList {
    RenderMesh* At(int index);         // 0x4a6800
};
// A scene object with a mesh list at +0x124 (the body, the parts at Car+0x1f68 / +0x1f6c).
struct RenderMeshOwner {
    BYTE unk0[0x124];
    RenderMeshList meshes;             // 0x124
};

// The Car body (Car+0x24).
struct RenderBody : public RenderMeshOwner {
    int IsCurrent();                   // 0x4b3030
    const float* Centre() { return (const float*)((BYTE*)this + 0xb8); }
    int Radius() { return *(int*)((BYTE*)this + 0xc4); }
};
// Position and matrices of a body or of the caster copy at Car+0x1ff0 (also the per-wheel shadow lights at
// Car+0x1f1c).
struct RenderCaster {
    BYTE unk0[0x14];
    RawVec3 pos;                       // 0x14
    float matrixA[16];                 // 0x20
    float matrixB[16];                 // 0x60
};

// A node of the material list at Car+0xfac: next at +0, data at +8.
struct MaterialData {
    BYTE unk0[0x40];
    float ambient[3];                  // 0x40
    BYTE unk4c[0x70 - 0x4c];
    float diffuse[3];                  // 0x70
};
struct MaterialNode {
    MaterialNode* next;                // 0x00
    int unk4;
    MaterialData* data;                // 0x08
};

// A coloured part (Car+0x2074..0x207c): the scene object at +0xc decides whether it is current.
struct ColourPart {
    BYTE unk0[0xc];
    SceneObject* scene;                // 0x0c
    void SetColour(float k);           // 0x4021b0
};

// The view object passed to the render code: +0x160 is the camera, whose first field is the lead object with the
// world matrix at +0x60.
struct RenderLead {
    BYTE unk0[0x60];
    float world[16];                   // 0x60
};
struct RenderCamera {
    RenderLead* lead;                  // 0x00
};
struct RenderView {
    BYTE unk0[0x160];
    RenderCamera* camera;              // 0x160
};

// The Stage object at Car+0x14fc: lighting range and the per-surface dust counts and scales.
struct RenderStage {
    BYTE unk0[0x1e8];
    float light[4];                    // 0x1e8
    BYTE unk1f8[0x51c - 0x1f8];
    int dust[16];                      // 0x51c: per surface id
    BYTE unk55c[0x69c - 0x55c];
    float scale[16];                   // 0x69c: per surface id - 1
};

// One wheel of a Car as the shadow lights read it (Car+0x28, stride 0x330).
struct RenderWheel {
    BYTE unk0[0x14];
    RawVec3 pos;                       // 0x14
    BYTE unk20[0x60 - 0x20];
    float matrix[16];                  // 0x60
    BYTE unka0[0x330 - 0xa0];
};

class Car;
// The Car as the render and shadow code sees it (0x469630-0x469a00).
struct CarRenderView {
    int CanSee(RenderBody* body);      // 0x464ea0: the viewer car's visibility test
    void Draw459270(int a, void* view); // 0x459270
    float SunFactor(int a);            // 0x45dac0
    BYTE unk0[0x24];
    RenderBody* body;                  // 0x24
    RenderWheel wheel[4];              // 0x28
    BYTE unkce8[0xfac - 0xce8];
    MaterialNode* materials;           // 0xfac
    BYTE unkfb0[0x1420 - 0xfb0];
    int timerA;                        // 0x1420
    int timerB;                        // 0x1424
    BYTE unk1428[0x14fc - 0x1428];
    RenderStage* stage;                // 0x14fc
    BYTE unk1500[0x1f1c - 0x1500];
    RenderCaster* light[4];            // 0x1f1c
    BYTE unk1f2c[0x1f68 - 0x1f2c];
    RenderMeshOwner* partA;            // 0x1f68
    RenderMeshOwner* partB;            // 0x1f6c
    BYTE unk1f70[0x1ff0 - 0x1f70];
    RenderCaster* caster;              // 0x1ff0
    BYTE unk1ff4[0x2074 - 0x1ff4];
    ColourPart* colourA;               // 0x2074
    ColourPart* colourB;               // 0x2078
    ColourPart* colourC;               // 0x207c
    BYTE unk2080[0x2098 - 0x2080];
    float colourScaleA;                // 0x2098
    float colourScaleB;                // 0x209c
    BYTE unk20a0[0x20ec - 0x20a0];
    CarShadow* shadow;                 // 0x20ec
    CarShadowTarget shadowTarget;      // 0x20f0
    BYTE unk20f1[0x1bce4 - 0x20f1];
    int unk1bce4;                      // 0x1bce4
    int shadowMode;                    // 0x1bce8
};

struct PartSub {
    BYTE unk0[0x88];
    RawVec4 v;                         // 0x88
};
// The Car as the surface mix and part clear read it (0x469e20, 0x469ea0).
struct MixWheel {
    int active;                        // 0x00
    BYTE unk4[0x6c - 4];
    float slipA;                       // 0x6c
    float slipB;                       // 0x70
    BYTE unk74[0x330 - 0x74];
};
struct MixBody {
    BYTE unk0[0x1cc];
    RawVec3 vel;                       // 0x1cc
};
struct CarMixView {
    BYTE unk0[0x24];
    MixBody* body;                     // 0x24
    BYTE unk28[0x2c0 - 0x28];
    MixWheel wheel[4];                 // 0x2c0
    BYTE unkf80[0x14fc - 0xf80];
    RenderStage* stage;                // 0x14fc
    BYTE unk1500[0x1e44 - 0x1500];
    int surface[4];                    // 0x1e44: surface id under each wheel
    BYTE unk1e54[0x1bcac - 0x1e54];
    float mix[4];                      // 0x1bcac: normalised surface weights
    BYTE unk1bcbc[0x1bcc4 - 0x1bcbc];
    std::vector<PartSub*> parts;       // 0x1bcc4 (allocator at +0, begin 0x1bcc8, end 0x1bccc)
};

// The message queue embedded at Car+0x1f160 (0x40b600 flushes entries up to a time).
struct CarQueue {
    void Flush(int time);              // 0x40b600
};

// The texture ids the game object (DAT_0056b0c8) hands to DrawGauges.
struct GaugeTextures {
    BYTE unk0[0x1390fc];
    int needles;                       // 0x1390fc
    int gear;                          // 0x139100
    int plain;                         // 0x139104
};

// The Car as DrawGauges and the message flush read it (0x46a200, 0x46a850).
struct CarGaugeView {
    BYTE unk0[0x24];
    MixBody* body;                     // 0x24
    BYTE unk28[0xdb8 - 0x28];
    Drivetrain drive;                  // 0xdb8
    BYTE unkfa0[0x1ec8 - 0xfa0];
    int textureA;                      // 0x1ec8
    int textureB;                      // 0x1ecc
    float x, y;                        // 0x1ed0: position of the dashboard (1024x768 units)
    float speedX, speedY;              // 0x1ed8: rpm needle pivot offset
    float rpmX, rpmY;                  // 0x1ee0: speed needle pivot offset
    float gearX, gearY;                // 0x1ee8: gear indicator position
    float rpmHalf, rpmFrom, rpmTo;     // 0x1ef0: rpm needle arc
    float speedHalf, speedFrom, speedTo; // 0x1efc: speed needle arc
    float rpmMax;                      // 0x1f08
    float speedMax;                    // 0x1f0c
    BYTE unk1f10[0x1f148 - 0x1f10];
    int time;                          // 0x1f148
    BYTE unk1f14c[0x1f160 - 0x1f14c];
    CarQueue queue;                    // 0x1f160
};

void RotateQuad(float* out, float base, float fraction, float a, float b, float c); // 0x46a790


// ---- particle emission / tyre tracks views (0x469a20, 0x46a000) ----
/* Car render and effect members (0x469630-0x46a89f). Map: analysis/range-4666a0.md section 5.
 * Views of the Car fields EmitEffects (0x469a20) and UpdateTyreTracks (0x46a000) touch; Car itself is
 * not restructured. Include after rallytrophy.h, unk_481390.h, unk_47c460.h and the emitter headers
 * unk_477370.h, unk_478020.h, unk_479170.h, unk_47b4c0.h, unk_4741a0.h, unk_46a8a0.h. */

// Car body as the effect code reads it: ParticleBody (position 0x14, matrix 0x20, velocity 0x1cc) plus the
// exhaust-spray emission block.
struct EmitBody : public ParticleBody {
    BYTE unk1d8[0x268 - 0x1d8];
    float sprayLevel;                  // 0x268: > 0 while the car throws up spray
    Vec3 sprayOrigin;                  // 0x26c
    Vec3 sprayDir;                     // 0x278
    Vec3& Forward40() { return *(Vec3*)&m[8]; }
};

// The table object at Car+0x14fc: per surface id effect handles (1-based, 0 = none).
struct EmitSurfaceTable {
    BYTE unk0[0x56c];
    int trackMark[20];                 // 0x56c: tyre-track mark id
    int dust[20];                      // 0x5bc: CarParticles type
    int smoke[20];                     // 0x60c: CarEmitterB / WheelSpray type
};

// One wheel (0x330 bytes at Car+0x28): only the flag read here.
struct EmitWheel {
    BYTE unk0[0x14];
    Vec3 position;                     // 0x14: relative to the body
    BYTE unk20[0x294 - 0x20];
    float slip;                        // 0x294: slip / load value
    int contact;                       // 0x298
    BYTE unk29c[0x330 - 0x29c];
};

// The Car as the effect code sees it.
struct EmitCar {
    BYTE unk0[0x24];
    EmitBody* body;                    // 0x24
    EmitWheel wheel[4];                // 0x28, stride 0x330
    BYTE unkce8[0xdb8 - 0xce8];
    Drivetrain drive;                  // 0xdb8
    BYTE unkfa0[0x10fc - 0xfa0];
    float groundOffset;                // 0x10fc
    BYTE unk1100[0x1338 - 0x1100];
    float throttle;                    // 0x1338
    BYTE unk133c[0x14fc - 0x133c];
    EmitSurfaceTable* surfaces;        // 0x14fc
    BYTE unk1500[0x1e44 - 0x1500];
    int surface[4];                    // 0x1e44
    Vec3 wheelAxis[4];                 // 0x1e54: up axis per wheel
    BYTE unk1e84[0x1ec0 - 0x1e84];
    int detail;                        // 0x1ec0: effect detail level 0..2
    BYTE unk1ec4[0x20dc - 0x1ec4];
    Vec3 exhaustPos;                   // 0x20dc: exhaust outlet, body frame
    int exhaustOn;                     // 0x20e8
    BYTE unk20ec[0x2100 - 0x20ec];
    WheelSpray spray;                  // 0x2100
    CarEmitterB smokeB;                // 0x222c
    CarParticles dust;                 // 0x2358
    CarEmitterC debris;                // 0x2488
    BYTE exhaustStorage[0x160];        // 0x25e8: ExhaustSmoke (its class in unk_478020.h lacks the tail members)
    ExhaustSmoke& Exhaust() { return *(ExhaustSmoke*)exhaustStorage; }
    SkidMarks skid[4];                 // 0x2748, stride 0x6558
};

typedef char SZ_WheelSpray[sizeof(WheelSpray) == 0x12c ? 1 : -1];
typedef char SZ_CarEmitterB[sizeof(CarEmitterB) == 0x12c ? 1 : -1];
typedef char SZ_CarParticles[sizeof(CarParticles) == 0x130 ? 1 : -1];
typedef char SZ_CarEmitterC[sizeof(CarEmitterC) == 0x160 ? 1 : -1];
typedef char SZ_SkidMarks[sizeof(SkidMarks) == 0x6558 ? 1 : -1];

#endif
