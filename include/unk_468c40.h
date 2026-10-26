/* Car model parts (0x468c40-0x46962f): texture reload, part visibility, cockpit view, damaged
 * model and body alpha. Map: analysis/range-4666a0.md section 4.
 * Include after rallytrophy.h and unk_481390.h.
 * The Car members are written against CarModelView, a raw view of the Car fields these use
 * (Car itself is not restructured). */
#ifndef UNK_468C40_H
#define UNK_468C40_H

// A model part / effect object: flag byte at +0xc, bit 0 = hidden.
struct ModelPart {
    void Show();                       // 0x4b2b90
    void SetParent(void* parent);      // 0x4b2b30
    BYTE unk0[0xc];
    BYTE flags;                        // 0x0c
};
typedef ModelPart ModelEffect;

// A wheel of the Car (stride 0x330 from Car+0x28); flag bit 1 = cockpit view.
struct ModelWheel {
    BYTE unk0[0xc];
    BYTE flags;                        // 0x0c
    BYTE unkd[0x330 - 0xd];
};

// A 16-byte block of the material (colour / matrix row), copied as a unit.
struct MaterialQuad {
    float a, b, c, d;
};

// Material data of a model node (blend states and colour blocks).
struct ModelMaterial {
    BYTE unk0[0x18];
    int blend18;                       // 0x18
    int blend1c;                       // 0x1c
    int unk20;                         // 0x20
    int unk24;                         // 0x24
    int unk28;                         // 0x28
    int unk2c;                         // 0x2c
    int unk30;                         // 0x30
    int unk34;                         // 0x34
    int unk38;                         // 0x38
    BYTE unk3c[4];
    MaterialQuad quad40;               // 0x40: 1.0 1.0 1.0 alpha (+0x4c is the body alpha)
    MaterialQuad quad50;               // 0x50
    MaterialQuad quad60;               // 0x60
    MaterialQuad quad70;               // 0x70
    BYTE unk80[0x98 - 0x80];
    int unk98;                         // 0x98
    int unk9c;                         // 0x9c (byte 0x9d bit 6 = alternate blend setup)
};

struct ModelNode {
    ModelNode* next;                   // 0x00
    int unk4;
    ModelMaterial* data;               // 0x08
};

// The texture list (+0xfa4) and the mesh root (+0xfa0) reloaded by FUN_00468c40.
class ModelTextures {
public:
    void Upload(int arg);              // 0x4af180
};
class ModelRoot {
public:
    void Reload(int arg);              // 0x4ae1f0
};

struct ModelHolder {
    BYTE unk0[0xc];
    ModelPart* part;                   // 0x0c
};
struct ModelLamp {
    BYTE unk0[0x38];
    int state;                         // 0x38
};
struct ModelSlot {                     // stride 0x1c
    ModelPart* part;
    BYTE unk4[0x18];
};
struct ModelViewPair {
    int a;
    int b;
};
struct ModelPlayer {
    BYTE unk0[0x40];
    int mode;                          // 0x40
};

// The growing pointer array the visible parts are collected into (+0x124 of the caller's object).
struct CarPtrArray {
    BYTE unk0[0x124];
    void** data;                       // 0x124
    void** end;                        // 0x128
    int count;                         // 0x12c
    int capacity;                      // 0x130
    int grow;                          // 0x134
};

// The Car as the model code sees it.
struct CarModelView {
    BYTE unk0[0x24];
    void* body;                        // 0x24
    ModelWheel wheel[4];               // 0x28
    BYTE unkce8[0x2b8];
    ModelRoot* root;                   // 0xfa0
    ModelTextures* textures;           // 0xfa4
    int texturesLoaded;                // 0xfa8
    ModelNode* nodes1;                 // 0xfac
    BYTE unkfb0[0x24];
    ModelNode* nodes2;                 // 0xfd4
    BYTE unkfd8[0xedc];
    int cockpit;                       // 0x1eb4
    int damaged;                       // 0x1eb8
    int model;                         // 0x1ebc
    BYTE unk1ec0[0x58];
    ModelPart* part1f18;               // 0x1f18
    BYTE unk1f1c[0x4c];
    ModelPart* part1f68;               // 0x1f68
    ModelPart* part1f6c;               // 0x1f6c
    BYTE unk1f70[0x84];
    ModelPart* part1ff4;               // 0x1ff4
    ModelHolder* holder1ff8;           // 0x1ff8
    ModelPart* part1ffc;               // 0x1ffc
    BYTE unk2000[0x4];
    ModelPart* part2004;               // 0x2004
    ModelPart* part2008;               // 0x2008
    ModelEffect* eff200c;              // 0x200c
    ModelEffect* eff2010;              // 0x2010
    ModelEffect* eff2014;              // 0x2014
    ModelEffect* eff2018;              // 0x2018
    ModelEffect* eff201c;              // 0x201c
    ModelPart* part2020;               // 0x2020
    ModelPart* part2024;               // 0x2024
    ModelPart* part2028;               // 0x2028
    ModelPart* part202c;               // 0x202c
    BYTE unk2030[0x18];
    ModelEffect* eff2048;              // 0x2048
    ModelEffect* eff204c;              // 0x204c
    ModelEffect* eff2050;              // 0x2050
    ModelPart* part2054;               // 0x2054
    ModelPart* part2058;               // 0x2058
    BYTE unk205c[0x18];
    int view[3];                       // 0x2074
    ModelViewPair pair[3];             // 0x2080
    BYTE unk2098[0x54];
    ModelPart* part20ec;               // 0x20ec
    BYTE unk20f0[0x1c];
    BYTE flag210c;                     // 0x210c
    BYTE unk210d[0x12b];
    BYTE flag2238;                     // 0x2238
    BYTE unk2239[0x12b];
    BYTE flag2364;                     // 0x2364
    BYTE unk2365[0x12f];
    BYTE flag2494;                     // 0x2494
    BYTE unk2495[0x15f];
    BYTE flag25f4;                     // 0x25f4
    BYTE unk25f5[0x19707];
    BYTE flag1bcfc;                    // 0x1bcfc
    BYTE unk1bcfd[0x33];
    ModelLamp* lamp;                   // 0x1bd30
    BYTE unk1bd34[0xc];
    ModelSlot slot[12];                // 0x1bd40
    BYTE unk1be90[0xc];
    ModelPart* part1be9c;              // 0x1be9c
    ModelPart* part1bea0;              // 0x1bea0
    ModelPart* ex[6];                  // 0x1bea4
    BYTE unk1bebc[0x10];
    ModelPlayer* player;               // 0x1becc
    BYTE unk1bed0[0x868];
    int v1c738;                        // 0x1c738

};

#endif
