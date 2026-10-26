/* Rear-view mirror (0x464ab0-0x4653ef, stub 0x4653c0; SkidFrustum ctor/dtor 0x4653f0/0x465460).
 * Map: analysis/range-4649f0.md section 1. Include after unk_481390.h and rallytrophy.h (and
 * unk_469630.h / unk_4741a0.h for CanSee and the frustum static).
 * The Car members read their fields through MirrorCar (Car itself is not restructured). */
#ifndef UNK_464AB0_H
#define UNK_464AB0_H

extern int DAT_00501308;               // mirror enabled (option)

// The Car body as the mirror camera reads it (Car+0x24).
struct MirrorBody {
    BYTE unk0[0x20];
    float row0[3];                     // 0x20
    float unk2c;
    float row1[3];                     // 0x30
    float unk3c;
    Vec3 row2;                         // 0x40 (negated into the view)
    BYTE unk4c[0x60 - 0x4c];
    float m0[3];                       // 0x60: matrix rows (stride 0x10), translation at 0x90
    float unk6c;
    float m1[3];                       // 0x70
    float unk7c;
    Vec3 m2;                           // 0x80 (negated into the view)
    BYTE unk8c[0xa4 - 0x8c];
    float unka4;                       // 0xa4
    BYTE unka8[0xb0 - 0xa8];
    float unkb0;                       // 0xb0
    float unkb4;                       // 0xb4
    Vec3 unkb8;                        // 0xb8
};

// The Stage fields copied into the draw context (Car+0x14fc).
struct MirrorStage {
    BYTE unk0[0x1a4];
    int unk1a4;                        // 0x1a4
    int unk1a8;                        // 0x1a8
    int unk1ac;                        // 0x1ac
    int unk1b0;                        // 0x1b0
    float unk1b4;                      // 0x1b4 (copied through the FPU)
    BYTE unk1b8[0x1c4 - 0x1b8];
    int unk1c4;                        // 0x1c4
    int unk1c8;                        // 0x1c8
    int unk1cc;                        // 0x1cc
};

// The Car as the mirror code sees it.
struct MirrorCar {
    BYTE unk0[0x24];
    MirrorBody* body;                  // 0x24
    BYTE unk28[0x14fc - 0x28];
    Stage* stage;                      // 0x14fc
    int CanSee(struct MirrorObject* object); // 0x464ea0 (declared as CarRenderView::CanSee elsewhere)
};

// MirrorView (the view helper static at 0x509080, 0x1cc bytes) moved to include/unk_481390.h: ViewManager derives from it.

// The draw context static at 0x509250 (DrawContext, 0x194 bytes) as the mirror fills it, plus the
// method 0x4b8310 that DrawContext does not declare.
struct MirrorContext {
    BYTE unk0[0x124];
    ViewArray views;                   // 0x124
    BYTE unk138[0x174 - 0x138];
    int unk174;                        // 0x174
    int unk178;
    int unk17c;
    float unk180;
    int unk184;
    int unk188;
    int unk18c;
    int unk190;
    void FUN_004b8310(int arg);        // 0x4b8310
};

// The frustum static at 0x508f68 with the sphere test 0x4ba830 (SkidFrustum declares 0x4ba9d0 only).
struct MirrorFrustum {
    int TestSphere(const float* sphere, const float* matrix); // 0x4ba830
};

// A scene object as the visibility test reads it: flags at +0xc, matrix at +0x60, sphere at +0xa0.
struct MirrorObject {
    BYTE unk0[0xc];
    BYTE flags;                        // 0x0c
    BYTE unkd[0x60 - 0xd];
    float matrix[16];                  // 0x60
    float sphere[4];                   // 0xa0
};

// The renderer's viewport slots (0x38 set, 0x3c get), which RaceDeviceSlots leaves as filler.
#define MIRROR_SLOT(n) virtual void slot##n();
class MirrorDevice {
public:
    MIRROR_SLOT(0)
    MIRROR_SLOT(1)
    MIRROR_SLOT(2)
    MIRROR_SLOT(3)
    MIRROR_SLOT(4)
    MIRROR_SLOT(5)
    MIRROR_SLOT(6)
    MIRROR_SLOT(7)
    MIRROR_SLOT(8)
    MIRROR_SLOT(9)
    MIRROR_SLOT(10)
    MIRROR_SLOT(11)
    MIRROR_SLOT(12)
    MIRROR_SLOT(13)
    virtual void SetViewport(int x, int y, int w, int h);          // 0x38
    virtual void GetViewport(int* x, int* y, int* w, int* h);      // 0x3c
};
#undef MIRROR_SLOT

#endif
