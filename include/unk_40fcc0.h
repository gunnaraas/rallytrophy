#ifndef UNK_40FCC0_H
#define UNK_40FCC0_H
// Stage object-grid build (0x40fcc0): refreshes every scene object, then registers each object, trigger and
// zone in the grid cell that contains it. Offsets from the pseudocode (analysis/range-409690.md, M5).
#include <memory>
#include "scene_object.h"
#include <windows.h>
#include "unk_481390.h"

struct GridNodeRef { int pad; SceneNode* node; };  // element of the list at +0x134 and of the cell list at +0x64
struct GridRef { GridRef(SceneNode* node); int pad[2]; };      // 0x416df0 (8-byte reference object)

// vector<T*> layout {alloc, first, last, end}; the size is the out-of-line helper 0x414460.
struct GridPtrVec {
    std::allocator<int> allocator;
    int* first;
    int* last;
    int* end;
    unsigned int Size();                           // 0x414460
    int* Ucopy(int* f, int* l, int* d);            // 0x424320
    void Ufill(int* d, unsigned int n, const int* x); // 0x424350
    void Destroy(int* f, int* l);                  // 0x403130
    void Ins(GridRef** pos, int n, GridRef** x);   // 0x4bc680 (vector<GridRef*>::_Insert_n)
};
void __cdecl GridFree(void* p);                    // 0x4bd000

struct GridCell {
    BYTE pad0[0x64];
    GridNodeRef** nodeFirst;                       // 0x64
    GridNodeRef** nodeLast;                        // 0x68
    BYTE pad6c[4];
    GridPtrVec refs;                               // 0x70: first at 0x74, last at 0x78
    BYTE pad80[0x90 - 0x80];
    GridPtrVec records48;                          // 0x90
    GridPtrVec records2c;                          // 0xa0
};

class StageGrid {
public:
    void Build();                                  // 0x40fcc0
    BYTE pad0[0x68];
    int alloc68;
    SceneNode** objFirst;                          // 0x6c
    SceneNode** objLast;                           // 0x70
    BYTE pad74[0x134 - 0x74];
    int alloc134;
    GridNodeRef** nodeFirst;                       // 0x138
    GridNodeRef** nodeLast;                        // 0x13c
    BYTE pad140[4];
    GridPtrVec allRefs;                            // 0x144
    BYTE pad154[0xa04 - 0x154];
    int count48;                                   // 0xa04
    BYTE* records48;                               // 0xa08
    BYTE pad_a0c[0x1364 - 0xa0c];
    LoadingLabel* label;                           // 0x1364
    BYTE pad1368[0x24c8 - 0x1368];
    BYTE* records2c;                               // 0x24c8
    int count2c;                                   // 0x24cc
    GridCell* Cell(float* pos);                    // 0x411080
};
#endif
