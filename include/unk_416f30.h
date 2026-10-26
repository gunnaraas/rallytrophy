/* Stage animals/props (M8, 0x416f30 group): prop array, bounds tree over triangles. */
#ifndef UNK_416F30_H
#define UNK_416F30_H
#include <windows.h>
#include <vector>

struct StageProp {                         // 0x24 bytes, copied field-wise by 0x418220 / 0x418270
    float v[8];
    int flag;                              // 0x20
    StageProp& operator=(const StageProp& s);    // 0x418270
};

void __cdecl ConstructProp(StageProp* dst, const StageProp* src); // 0x418220

struct NodeTag0 { ~NodeTag0() {} };
struct NodeTag : NodeTag0 { ~NodeTag() {} };
class BoundsNode : NodeTag {                         // 0x28 bytes: min/max box, two children, triangle index list
public:
    BoundsNode();                          // 0x4182e0 (zeroes 0x18..0x27)
    ~BoundsNode();                         // 0x418300 (frees both children)
    int PartitionAxis(int axis, int mesh); // 0x418370
    bool Split(int mesh);                  // 0x418430
    float min[3];
    float max[3];
    BoundsNode* left;                      // 0x18
    BoundsNode* right;                     // 0x1c
    int count;                             // 0x20
    short* indices;                        // 0x24
};

class PropStage {
public:
    void Load();                           // 0x416f30
    void Reset();                          // 0x417430
    int NearSphere(const float* p, float r); // 0x417e60
    void InsertProp(StageProp* where, const StageProp& v); // 0x417ee0 body is vector::_Insert_n
    BYTE pad0[0x136c];
    int f136c, f1370, f1374;
    float cx, cy, cz;                      // 0x1378
    BYTE pad1[0x139c - 0x1384];
    int f139c;
    BYTE pad2[0x13a8 - 0x13a0];
    std::vector<StageProp> props;          // 0x13a8
    std::vector<void*> ptrs;               // 0x13b8
    BYTE pad3[0x13f4 - 0x13c8];
    int f13f4;
};
#endif
