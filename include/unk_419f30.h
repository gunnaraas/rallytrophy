// Raw source-mesh layout read by the ColModel constructor 0x419f30 (inferred from pseudocode).
#ifndef UNK_419F30_H
#define UNK_419F30_H
struct ColSrcObj { char pad[0xb8]; int kind; };
struct ColSrcTri { ColSrcObj* obj; int a, b, c; };   // 0x10
struct ColSrcMesh {
    char pad[0x40];
    int nVerts;                  // 0x40
    int nTris;                   // 0x44
    float* verts;                // 0x48 (3 floats each)
    ColSrcTri* tris;             // 0x4c
};
#endif
