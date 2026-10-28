#ifndef UNK_40A850_H
#define UNK_40A850_H
// Types for 0x40a850 (a "tumbling body" prop that collides with a list of cars). Layouts from pseudocode.
#include <vector>
#include "unk_481390.h"

struct U40A850Box {
    char unk0[4];
    float lo[3];                      // 0x04
    float hi[3];                      // 0x10
};
struct U40A850Body {
    char unk0[0x14];
    float pos[3];                     // 0x14
    float mat[3][4];                  // 0x20 (rows 0x10 apart)
    char unk50[0x12c - 0x50];
    float mass;                       // 0x12c
    char unk130[0x1cc - 0x130];
    float vel[3];                     // 0x1cc
    float ang[3];                     // 0x1d8
    float force[3];                   // 0x1e4
    char unk1f0[0x214 - 0x1f0];
    float dt;                         // 0x214
    char unk218[0x234 - 0x218];
    U40A850Box* box;                  // 0x234
    int FUN_0041d7d0(U40A850Body* other, int flag);
    void FUN_0041c010();
    void FUN_0041fc60(void* stage, void* unused);
    int FUN_0041ca70(void* query, int flag);
    void FUN_0041ef60(void* stage, void* owner, int flag);
    void FUN_0041c540();
    void FUN_0041c220(float a, float b);
};
struct U40A850Car {
    char unk0[0x24];
    U40A850Body* body;                // 0x24
    char unk28[0x14f4 - 0x28];
    int hits;                         // 0x14f4
    int applied;                      // 0x14f8
};
struct U40A850Item {
    char unk0[0x28];
    U40A850Car* car;                  // 0x28
    char unk2c[0x40 - 0x2c];
    int state;                        // 0x40
    char unk44[4];
    int flag48;                       // 0x48
};
struct U40A850Vec {
    int unk0;
    U40A850Item** first;              // +4 from list base
    U40A850Item** last;
    int Size();                       // original: FUN_00414460
    U40A850Item* operator[](int i) { return first[i]; }
};
struct U40A850List {
    virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {}
    virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {}
    virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {}
    virtual int Accept(U40A850Item* it);  // 0x38
    U40A850Vec items;                 // 0x04
};
struct U40A850Query {
    std::vector<int> v;               // 0x00
    char unk10[4];
    float minX, minY, minZ;           // 0x14
    float maxX, maxY, maxZ;           // 0x20
    void* sphere;                     // 0x2c
    int mode;                         // 0x30
};
struct U40A850Stage { void Collect(U40A850Query* q, float* sphere); }; // 0x4114c0
struct U40A850Prop {
    char unk0[0x4c];
    float k;                          // 0x4c
    U40A850Body* body;                // 0x50
    int active;                       // 0x54
    U40A850List* list;                // 0x58
    U40A850Stage* stage;              // 0x5c
    int unk60;
    int time;                         // 0x64
    void Fn(int dt);                  // original: FUN_0040a850
};
#endif
