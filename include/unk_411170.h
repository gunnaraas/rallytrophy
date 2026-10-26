#ifndef UNK_411170_H
#define UNK_411170_H
// Race scene (original class unknown) methods at 0x411170-0x411520. Layouts from pseudocode.
#include "unk_40a7b0.h"
#include "unk_468c40.h"

class SkyDevice;
class Camera;
class SkyWorld;
struct S6Sky {                                 // the sky at scene+0x6ac
    void Update(SkyDevice* device, Camera* camera, SkyWorld* world, float dt); // 0x475eb0
    void Render(SkyDevice* device, Camera* camera);                         // 0x475c00
};
struct S6Elem { void Fn(int a); };            // original: FUN_0040a850

// Device with a vtable; only the used slots are real (0x80, 0x84, 0xa8, 0xcc).
class S6Device {
public:
    virtual void s0() {} virtual void s1() {} virtual void s2() {} virtual void s3() {}
    virtual void s4() {} virtual void s5() {} virtual void s6() {} virtual void s7() {}
    virtual void s8() {} virtual void s9() {} virtual void s10() {} virtual void s11() {}
    virtual void s12() {} virtual void s13() {} virtual void s14() {} virtual void s15() {}
    virtual void s16() {} virtual void s17() {} virtual void s18() {} virtual void s19() {}
    virtual void s20() {} virtual void s21() {} virtual void s22() {} virtual void s23() {}
    virtual void s24() {} virtual void s25() {} virtual void s26() {} virtual void s27() {}
    virtual void s28() {} virtual void s29() {} virtual void s30() {} virtual void s31() {}
    virtual void v80(int a);                  // 0x80
    virtual void v84(int a, int b);           // 0x84
    virtual void s34() {} virtual void s35() {} virtual void s36() {} virtual void s37() {}
    virtual void s38() {} virtual void s39() {} virtual void s40() {} virtual void s41() {}
    virtual void va8(int a);                  // 0xa8
    virtual void s43() {} virtual void s44() {} virtual void s45() {} virtual void s46() {}
    virtual void s47() {} virtual void s48() {} virtual void s49() {} virtual void s50() {}
    virtual void vcc(float a, float b, float c, float d, int e); // 0xcc
};

// Object with slots 1 and 5 used.
class S6Object {
public:
    virtual void s0() {}
    virtual void v4(int a);                   // slot 1
    virtual void s2() {} virtual void s3() {} virtual void s4() {}
    virtual void v14(S6Device* dev, struct S6View* view); // slot 5
};

// Slot 1 of the object at scene+0x6b0.
class S6Hook {
public:
    virtual void s0() {}
    virtual void v4(int a, int b, int c, void* scene, int d);
};

struct S6View {                               // draw context / view
    BYTE unk0[0x14];
    int x, y, z;                              // 0x14
    BYTE unk20[0x124 - 0x20];
    float unk124;
    float unk128;
    float unk12c;
};

// Growable pointer array in the draw context (+0x124).
struct S6Array {
    int* data;                                // 0x124
    int* cur;                                 // 0x128
    int count;                                // 0x12c
    int cap;                                  // 0x130
    int grow;                                 // 0x134
};

struct S6Item { int id; int pad[6]; float x; float y; float z; float r; };   // x at 0x1c, r at 0x28
struct S6Inner {
    BYTE unk0[0x24];
    S6Item** first;                           // cell+0x30
    S6Item** last;                            // cell+0x34
    int Size() { return first == 0 ? 0 : (last - first); }
};
struct S6Cell {
    BYTE unk0[0xc];
    S6Inner in;
};
struct S6Pos { float x, y, z, r; int Near(float* center, float radius); }; // original: FUN_00414370
typedef S6Pos S6Sphere;
struct S6Collector { void Add(int id); };     // original: FUN_00418a70

struct S6Vec {                                // pointer vector (first, last)
    U40A7B0Owner** first;
    U40A7B0Owner** last;
    bool Has(unsigned int i) { return first != 0 && i < (unsigned int)(last - first); }
};

class RaceScene {
public:
    void FUN_00411170(float* view, struct S6Ctx* ctx);
    void FUN_00411260();
    void FUN_004112a0(void* race);
    void FUN_004112e0(int a);
    void FUN_00411320(int a);
    void FUN_00411350(S6Device* dev, S6View* view);
    void FUN_00411460(int a, int b, int c, int d);
    void FUN_00411490(SkyDevice* dev, Camera* cam, SkyWorld* world, float dt);
    void FUN_004114c0(S6Collector* out, S6Pos* pos);
    void FUN_00411520(S6Collector* out, S6Sphere* sph);
    S6Cell* Cell(float* pos);                 // original: FUN_00411080
    void Visit(S6Cell* cell, S6Ctx* ctx);     // original: FUN_00410f70
    void Refresh();                           // original: FUN_004149d0
    void Finish();                            // original: FUN_00417430
    BYTE unk0[0x8c];
    S6Vec elems;                              // 0x8c
};
struct S6Ctx {
    BYTE unk0[0x124];
    S6Array arr;
};
#endif
