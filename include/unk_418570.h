// Collision triangle tree and sphere-query collector (0x418570-0x419be0). Layouts inferred from pseudocode.
#ifndef UNK_418570_H
#define UNK_418570_H
#include <vector>
#include "scene_object.h"

struct TVert { int pad; float x, y, z; };
struct Tri {                     // 0x40 bytes
    float cx, cy, cz, r;         // bounding sphere
    short id;                    // 0x10
    unsigned short sel;          // 0x12: packed min/max vertex index per axis
    Tri* next;                   // 0x14
    float nx, ny, nz;            // 0x18 plane normal
    char pad[0x28 - 0x24];
    TVert* v[3];                 // 0x28
    char pad2[0x40 - 0x34];
};
struct TriMesh;
struct TriNode {                 // BVH node
    float mn[3], mx[3];
    TriNode* child;              // 0x18
    TriNode* sibling;            // 0x1c
    int count;                   // 0x20
    short* idx;                  // 0x24
    void Init();                 // 0x4182e0
    void Free();                 // 0x418300
    void FitAll(TriMesh* m);     // 0x418570 (calls Fit2 0x418430 per node)
    void Fit2(TriMesh* m);       // 0x418430
    void FitBox(TriMesh* m);     // 0x4185b0
    TriNode* Ctor();             // 0x4186b0
    void Dtor();                 // 0x4186c0
    bool Build(TriMesh* m); // 0x4186e0
};
struct TriMesh { int pad0; int count; char pad[0x10]; Tri* tris; char pad2[4]; TriNode* root; };

struct TriCollector {
    std::vector<Tri*> hits;      // 0x00 (pad,first,last,end)
    Tri* head;                   // 0x10
    float mn[3], mx[3];          // 0x14, 0x20
    int pad2c;
    int limit;                   // 0x30
    void Walk(TriNode* n);       // 0x418760
    void InsN(Tri** pos, int n, Tri** x); // 0x4bc680: vector<Tri*>::_Insert_n (thiscall, out-of-line instantiation)
    void Gather(TriMesh* m);     // 0x418a70
    float Closest(float r, float* pos, float* out, float* nrm);                          // 0x4193a0
    int Contact(float r, float* pos, float* a, float* b, float* c, int* surf);            // 0x419770
    int ContactList(float r, float* pos, float* a, float* b, float* c, int* surf);        // 0x419be0
};
#endif
