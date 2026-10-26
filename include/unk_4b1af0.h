// BMF model chunk readers (0x4b1af0-0x4b2330). Map: analysis/range-4a9b60.md.
#ifndef UNK_4B1AF0_H
#define UNK_4B1AF0_H
#include "rallytrophy.h"
#include "unk_4af230.h"
#include <vector>
#include <string>

class DynamicMesh;
class Sprite;

struct BmfVertex { // 0x2c bytes, empty ctor/dtor (eh vector ctor 0x4c11f4)
    BmfVertex() {}
    ~BmfVertex() {}
    char b[0x2c];
};

struct BmfVec12 { // 12 bytes, empty ctor/dtor
    BmfVec12() {}
    ~BmfVec12() {}
    char b[12];
};

struct ModelOut { // destination of the chunk readers
    char unk00[0x30];
    std::vector<DynamicMesh*> batches; // 0x30
    int vertCount;   // 0x40 (written by 0x4b2290)
    int polyCount;   // 0x44 (written by 0x4b2330)
    BmfVec12* verts; // 0x48
    unsigned int* polys; // 0x4c
};

class ModelFile {
public:
    char unk00[0x18];
    std::vector<void*> materials; // 0x18 (begin 0x1c, end 0x20)
    void ReadBatch(File* file, ModelOut* out);     // 0x4b1af0
    void ReadBatchSmall(File* file, ModelOut* out); // 0x4b1ec0
    void ReadVertices(File* file, ModelOut* out);  // 0x4b2290
    void ReadPolylist(File* file, ModelOut* out);  // 0x4b2330
};
#endif
