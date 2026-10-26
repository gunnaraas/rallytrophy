// Collision model (0x419f30 ctor family) and its helpers 0x41b190-0x41b990. Layout inferred from pseudocode.
#ifndef UNK_419F00_H
#define UNK_419F00_H
#include "scene_object.h"
#include "unk_418570.h"

// Array element types (each has the empty dtor that the vector-deleting calls reference, 0x402c90).
struct ColVert { int pad; float x, y, z; ~ColVert() {} };            // 0x10
struct ColTri { float c[4]; char pad[0x30]; ~ColTri() {} };          // 0x40
struct ColLocal { float x, y, z; ~ColLocal() {} };                   // 0xc

struct ColModel {
    int nVerts;                  // 0x00
    int nTris;                   // 0x04
    int nInts;                   // 0x08
    int* ints;                   // 0x0c
    ColVert* verts;              // 0x10 local
    ColVert* wverts;             // 0x14 world
    ColTri* tris;                // 0x18
    ColLocal* centers;           // 0x1c local tri centres
    TriNode* root;               // 0x20
    int pad24;
    ColModel(void* mesh, int x);                      // 0x419f30
    ColModel(void* mesh, int a, int b);               // 0x41a890
    ~ColModel();                 // 0x41b190
    void ClearInts();            // 0x41b250
    void Transform(float* m, float* t); // 0x41b270
    void FreeVerts();            // 0x41b530
    void ComputeSel();           // 0x41b590
    void BuildTree(int minTris); // 0x41b730
};
// TriNode allocated with its 0x4186b0 constructor (the shared TriNode has no real ctor).
struct ColNode : TriNode { ColNode(); };


struct ColMeshList { void* At(int i); };            // 0x4a6800
struct ColOwner { char pad[0x124]; ColMeshList meshes; };
struct ColPtrBase { ~ColPtrBase() {} };
struct ColPtr : ColPtrBase {
    ColModel* p;
    ~ColPtr();                   // 0x41b990
};
struct ColHolder {               // 0x34 bytes
    ColPtr model;
    int f[11];
    char flag;
    ColHolder(ColOwner* o, int);                          // 0x41b7f0
    ColHolder(ColOwner* o, int a, int b);                 // 0x41b8c0
};
#endif
