#ifndef UNK_417540_H
#define UNK_417540_H
// Types for Stage::FUN_00417540 (per-player animal/prop update). Offsets are Stage-relative (see src/unk_417540.cpp).
#include "unk_481390.h"
#include "unk_411170.h"
struct AnimalPart { void SetColour(float k); };   // 0x4021b0

struct AnimalTrace {                   // the query object at Stage+0x13b8 (G2_Trace-like)
    void Erase(void* first, void* last);                                   // 0x484380
    float Cast(float radius, float* from, float* dir, float* hitNormal, void* unused); // 0x418b90
};
#endif
