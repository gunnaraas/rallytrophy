// Car body vs. triangle-list collision (0x41ca70). Layouts inferred from pseudocode.
#ifndef UNK_41CA70_H
#define UNK_41CA70_H
#include <string>
#include "unk_481390.h"
#include "unk_419f00.h"

struct CTri {                   // collision triangle, 0x40 bytes (same as Tri with the marks filled in)
    float c[3];                 // 0x00 bounding sphere centre
    float r;                    // 0x0c
    short id;                   // 0x10
    unsigned short sel;         // 0x12
    CTri* next;                 // 0x14
    Vec3 n;                     // 0x18 plane normal
    float d;                    // 0x24 plane distance
    TVert* v[3];                // 0x28
    int* e[3];                  // 0x34 edge generation marks
};

struct CBox {                   // object at body+0x234 (0x34 bytes)
    ColModel* model;            // 0x00
    float mn[3];                // 0x04
    float mx[3];                // 0x10
    float ctr[3];               // 0x1c
    float rad;                  // 0x28
};

struct CHit {                   // 0x50-byte contact record (array at body+0x238)
    char p0[8];
    CTri* tri;                  // 0x08
    Vec3 pt;                    // 0x0c
    Vec3 n;                     // 0x18
    char p24[0x50 - 0x24];
};

Vec3 operator*(float k, const Vec3& v); // 0x420430
struct CPMask { static unsigned int Mask(const float* planes, const float* p); }; // 0x41ba60

struct CPlane {                 // plane with empty ctor/dtor (eh vector ctor 0x4c11f4 / dtor 0x4c1116)
    Vec3 n;
    float d;
    CPlane() {}
    ~CPlane() {}
};

struct Tri3 {                   // 0x34-byte triangle helper (0x4b5920)
    char data[0x34];
    Tri3(const float* a, const float* b, const float* c, const float* n, float d); // 0x4b5920
    int Overlap(const float* a, const float* b, const float* c, const float* n, float d); // 0x4b59b0
    float EdgeDist(const float* a, const float* b, float* out, float* nrm);       // 0x4b6800
};

struct EdgeSet {                // DAT_00508510 (ctor 0x402ca0)
    void Clear();                           // 0x402d70
    void Add(void* tri, int* edge);         // 0x402da0
    int Has(void* tri, int* edge);          // 0x402e40
};
extern EdgeSet DAT_00508510_;

class ColBody {                 // the car rigid body (CarBody in unk_41b9e0.h)
public:
    char u0[0x14];
    Vec3 pos;                   // 0x14
    Vec3 ax;                    // 0x20
    float p2c;
    Vec3 ay;                    // 0x30
    float p3c;
    Vec3 az;                    // 0x40
    char u4c[0x234 - 0x4c];
    CBox* box;                  // 0x234
    CHit* hits;                 // 0x238
    int nHits;                  // 0x23c
    int Collide(TriCollector* q, int unused); // 0x41ca70 (thiscall, ret 8; the second argument is 0 at both call sites)
};
#endif
