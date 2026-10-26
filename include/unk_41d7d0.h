// Types for 0x41d7d0 (body-vs-body triangle collision). Layouts from pseudocode; see analysis/range-409690.md.
#ifndef UNK_41D7D0_H
#define UNK_41D7D0_H
#include "unk_40a850.h"

Vec3 operator*(float k, const Vec3& v); // 0x420430 (cdecl, hidden return pointer)
struct Plane {                        // 16 bytes; array of 6 built with the EH vector ctor/dtor (0x4c11f4/0x4c1116)
    float n[3];
    float d;
    Plane() {}
    ~Plane() {}
};
struct CTri {                         // 0x40-byte collision triangle (ColModel::tris)
    float c[3];                       // 0x00 centre
    float r;                          // 0x0c radius
    int u10;
    CTri* next;                       // 0x14 list link used while culling
    float n[3];                       // 0x18 normal
    float d;                          // 0x24
    int* v[3];                        // 0x28 vertex cells: [0] = cached plane mask, then x,y,z
    int* e[3];                        // 0x34 shared edge cells
};
struct CContact {                     // 0x50-byte element of body+0x238
    char u0[8];
    CTri* tri;                        // 0x08
    Vec3 pt;                          // 0x0c
    Vec3 n;                           // 0x18
    Vec3 v;                           // 0x24
    char pad[0x50 - 0x30];
};
struct CShape {                       // 0x34-byte triangle shape helper
    char d[0x34];
    void Init(float* a, float* b, float* c, float* n, float dd);     // 0x4b5920
    int Test(float* a, float* b, float* c, float* n, float dd);      // 0x4b59b0
    float Edge(float* a, float* b, float* hit, float* tmp);          // 0x4b6800
};
struct CPairSet {                     // static pair set DAT_00508510
    void Clear();                     // 0x402d70
    int Test(unsigned a, int b);      // 0x402e40
    void Set(unsigned a, int b);      // 0x402da0
};
struct CPointVel {                    // body seen through 0x4204f0
    Vec3 PointVel(const Vec3& p);     // 0x4204f0
};
#endif
