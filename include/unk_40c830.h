/* Stage scene helpers (map: analysis/range-409690.md, M5/M6): mesh effect scan, handle collection, object grid. */
#ifndef UNK_40C830_H
#define UNK_40C830_H
#include <memory>
#include "unk_40b560.h"

template <class T> struct SceneVec {      // std::vector<T*> layout without the destructor
    std::allocator<T*> allocator;
    T** first;
    T** last;
    T** end;
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    T*& operator[](unsigned int i) { return first[i]; }
};

struct SceneMesh {                         // vector<int*> of effect objects at 0x30
    BYTE pad[0x34];
    int** first;                           // 0x34
    int** last;                            // 0x38
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
};

struct SceneHolder {                       // a scene object with a mesh container at 0x124 (At = 0x4a6800)
    BYTE pad0[4];
    int type;                              // 0x04
    BYTE pad1[0x124 - 8];
    BYTE meshes[1];                        // 0x124
};

struct SceneMeshes { SceneMesh* At(int index); };   // 0x4a6800

struct SceneStage;
struct SceneHandleOwner {
    BYTE pad[0x50];
    int handle;                            // 0x50
};

class SceneObj7c {                         // 12-byte member at 0x7c destroyed by 0x40eb80 (Tidy = 0x402700)
public:
    ~SceneObj7c() { Tidy(true); }
    void Tidy(bool built);
    BYTE data[12];
};

struct SceneDt { ~SceneDt() {} };          // empty destructor members: only shift the unwind state numbering

class StageScene {
public:
    ~StageScene();                         // 0x40eb80 (destroys the member at 0x7c only)
    void ScanEffects();                    // 0x410380
    void LoadEffect(int* p);               // 0x410720
    void CollectHandles(SceneStage* s, GrowArray* out); // 0x410f70
    int CellIndex(float* pos);             // 0x411080
    void CellCoords(float* pos, int* x, int* z); // 0x4110c0
    BYTE pad0[0x28 - 10];
    SceneDt dt0, dt1, dt2, dt3, dt4, dt5, dt6, dt7, dt8, dt9;   // 0x1e..0x27
    SceneVec<SceneHolder> vecA;            // 0x28
    SceneVec<SceneHolder> vecB;            // 0x38
    BYTE pad48[0x7c - 0x48];
    SceneObj7c obj7c;                      // 0x7c
    SceneVec<SceneHandleOwner> vecH;       // 0x88
    BYTE pad98[0x164 - 0x98];
    float originX;                         // 0x164
    BYTE pad168[4];
    float originZ;                         // 0x16c
    BYTE pad170[0x188 - 0x170];
    float cellW;                           // 0x188
    BYTE pad18c[4];
    float cellH;                           // 0x190
    int cols;                              // 0x194
    int rows;                              // 0x198
    BYTE* cells;                           // 0x19c: 0xb0-byte cells
};
struct SceneStage {                        // object whose TimedLists lives at 0xc
    BYTE pad[0xc];
    TimedLists lists;
};
#endif
