/* Car::UpdateLights (0x45de90): the light records of the car and the damage they take. Map:
 * analysis/range-459890.md section 4. Include after unk_481390.h; Car only gets the method declaration.
 * The fields are read through the raw views below (offsets from CarObject in unk_45e480.h). */
#ifndef UNK_45DE90_H
#define UNK_45DE90_H

// A light sprite (LightRec::light, the glow objects at Car+0x1be9c): Show 0x4b2b90, flags byte at +0xc.
struct LightSprite {
    void Show();                       // 0x4b2b90
    BYTE unk0[0xc];
    BYTE flags;                        // 0x0c
};

// One light record (0x1c bytes, 12 per car at Car+0x1bd40: 3 front left, 3 front right, 2 rear, 2 brake,
// 2 reverse). Same layout as LightRec in unk_45e480.h.
struct LightsRec {
    LightSprite* light;                // 0x00
    BYTE dead;                         // 0x04
    BYTE unk5[3];
    float* target;                     // 0x08: the mesh value that breaks the light above 0.3
    int* out;                          // 0x0c: set to `value` when the light breaks
    int unk10;
    int unk14;
    int value;                         // 0x18
};

// The car body (Car+0x24): position at +0x14, matrix rows at +0x20, light inputs at +0xa4...
struct LightsBody {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    float m[12];                       // 0x20 (rows 0x10 apart)
    BYTE unk50[0xa4 - 0x50];
    float unka4;                       // 0xa4
    float unka8;                       // 0xa8
    BYTE unkac[4];
    float unkb0;                       // 0xb0
    float unkb4;                       // 0xb4
};

// The Car as UpdateLights sees it.
struct LightsCar {
    BYTE unk0[0x24];
    LightsBody* body;                  // 0x24
    BYTE unk28[0xf8c - 0x28];
    int unkf8c;                        // 0xf8c: -1 = reverse gear
    BYTE unkf90[0x133c - 0xf90];
    float unk133c;                     // 0x133c: > 0 when braking
    BYTE unk1340[0x1bd40 - 0x1340];
    LightsRec rec[12];                 // 0x1bd40
    int* unk1be90;                     // 0x1be90
    int unk1be94;                      // 0x1be94
    int unk1be98;                      // 0x1be98
    LightSprite* glow0;                // 0x1be9c
    LightSprite* glow1;                // 0x1bea0
    LightSprite* glowL[3];             // 0x1bea4
    LightSprite* glowR[3];             // 0x1beb0
    BYTE unk1bebc[0x1c738 - 0x1bebc];
    int noFrontLight;                  // 0x1c738
    BYTE unk1c73c[0x1f2c8 - 0x1c73c];
    int broken;                        // 0x1f2c8: count of broken lights
    BYTE unk1f2cc[0x1f300 - 0x1f2cc];
    Vec3 position;                     // 0x1f300: position of the last broken light
};

#endif
