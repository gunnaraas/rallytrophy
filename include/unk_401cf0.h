// Mesh-set loader (0x401cf0): fills a MeshSet from a loaded model's "frameNN" parts.
#ifndef UNK_401CF0_H
#define UNK_401CF0_H
#include <vector>
#include "unk_4018f0.h"

struct MSVert { float v[11]; };                    // 0x2c-byte model vertex: pos, normal at floats 3..5 (see loader)
struct MSSub { BYTE a[4]; MSVert* verts; BYTE b[4]; unsigned short n; };   // verts 0x04, count 0x0c
struct MSSubList { BYTE a[0x30]; std::vector<MSSub*> subs; };                  // vector at 0x30
struct MSMeshes { BYTE a[8]; MSSubList* At(int i); };         // 0x4a6800
struct MSPart {                                    // model part object (0x12c, ctor 0x4a6910)
    MSPart(void* src);                             // 0x4a6910
    BYTE a[0x14];
    float pivot[3];                                // 0x14
    BYTE b[0x118 - 0x20];
    const char* name;                              // 0x118
    BYTE c[0x124 - 0x11c];
    MSMeshes meshes;                               // 0x124 (container)
};
struct MSModelFile { BYTE a[0x28]; std::vector<void*> parts; };   // parts at 0x28

struct MV { float f[3]; MV() {} ~MV() {} };
struct MeshSetLoader : MeshSet {
    void Load(MSModelFile* file);                  // 0x401cf0
};
#endif
