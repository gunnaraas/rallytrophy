/* Tyre marks SkidMarks (0x4741a0-0x474c4f, vtable 0x4e2014, 0x6558 bytes): one
 * object per wheel, 4 embedded in a Car at +0x2748. A ring of 128 (64 at
 * quality 1, 32 at 0) quads. Map: analysis/range-4741a0.md section 2.1.
 * Include after rallytrophy.h, unk_481390.h and unk_47c460.h. */
#ifndef UNK_4741A0_H
#define UNK_4741A0_H

// One tyre-mark quad (0xc8 bytes); the implicit ctor/dtor (0x474480/0x4744d0)
// are the EH vector callbacks of the ring array.
struct SkidSeg {
    Vec3 a;                            // 0x00: near edge centre
    Vec3 b;                            // 0x0c: far edge centre (the wheel position)
    EffectVertex v[4];                 // 0x18: a+side, a-side, b-side, b+side
};

// One clip plane (normal and distance); arrays of it go through the EH vector iterators.
struct SkidPlane {
    SkidPlane() {}
    ~SkidPlane() {}
    float v[4];
};

// Sphere argument of the visibility test (0x30 bytes, a local of Render).
struct SkidSphere {
    SkidSphere() {}
    ~SkidSphere() {}
    BYTE unk0[0x18];
    float center[3];                   // 0x18
    float radius;                      // 0x24
    BYTE unk28[4];
    BYTE flag;                         // 0x2c
    BYTE unk2d[3];
};

// The view-frustum test helper Render builds over the device (0x118 bytes);
// implicit ctor/dtor (the dtor is 0x465460).
class SkidFrustum {
public:
    void Init(RenderDevice* device);   // 0x4ba620
    int Test(SkidSphere* sphere);      // 0x4ba9d0
    SkidPlane planesA[6];              // 0x00
    SkidPlane planesB[6];              // 0x60
    const float* view;                 // 0xc0
    BYTE unkc4[0x118 - 0xc4];
};

class SkidMarks : public EffectBase {
public:
    SkidMarks();                       // 0x4741a0 (scalar deleting dtor 0x474460)
    ~SkidMarks();                      // 0x474520
    void SetQuality(int q);            // 0x474590
    void Reset();                      // 0x4745c0
    void Expire();                     // 0x474600
    void AddSegment(Vec3* pos, Vec3* axis, Vec3* fallback, float alpha, int id); // 0x4746b0
    virtual void Render(RenderDevice* device, Camera* camera); // 0x474a80

    BYTE unk4[4];
    int unk8;                          // 0x08
    BYTE unkc[0xcc - 0xc];
    BYTE flags;                        // 0xcc
    BYTE unkcd[0x114 - 0xcd];
    BYTE nameStorage[0x10];            // 0x114: std::string name = "SkidMark" (constructed by the base)
    std::string& Name() { return *(std::string*)nameStorage; }
    int capacity;                      // 0x124: 0x80 >> (2 - quality)
    int quality;                       // 0x128
    void* texture;                     // 0x12c
    int active;                        // 0x130: a current segment exists
    int linked;                        // 0x134: it continues the previous one
    int head;                          // 0x138: ring index of the current segment
    int count;                         // 0x13c
    int id;                            // 0x140
    float unk144;                      // 0x144: 0.4
    float width;                       // 0x148: 0.2, half width of the mark
    float lastLength;                  // 0x14c: length of the current segment
    SkidSeg* cur;                      // 0x150
    SkidSeg* prev;                     // 0x154
    SkidSeg seg[128];                  // 0x158
};

#endif
