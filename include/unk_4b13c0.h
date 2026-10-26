#ifndef UNK_4B13C0_H
#define UNK_4B13C0_H
// BMF model chunk readers (0x4b13c0-0x4b1af0), members of the model loader (this = 0x4af950 object).
#include "unk_4af230.h"
#include "unk_4b83a0.h"

int __fastcall FUN_004a5fc0(File* file); // File::Tell

class ModelLoader {
public:
    SceneEntry* ReadMesh(File* file, unsigned int size); // 0x4b13c0
    void ReadLight(File* file);                          // 0x4b1640
    void ReadView(File* file);                           // 0x4b18b0
    void ReadView2(File* file);                          // 0x4b19b0
    void Attach(void* obj, int parent);                  // 0x4b2530
    void ReadPolys(File* file, SceneEntry* e);           // 0x4b2330
};
void __stdcall FUN_004b2290(File* file);
void __stdcall FUN_004b1af0(File* file);
void __stdcall FUN_004b1ec0(File* file);
#endif
