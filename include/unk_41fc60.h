// Types for 0x41fc60 (car body contact impulse resolution). Layouts from pseudocode; see analysis/range-409690.md.
#ifndef UNK_41FC60_H
#define UNK_41FC60_H
#include "unk_40a850.h"
struct ImpContact {                   // 0x50-byte contact record at body+0x238
    int active;                       // 0x00
    char u4[8];
    float pt[3];                      // 0x0c
    float n[3];                       // 0x18
    float v[3];                       // 0x24
    float imp[3];                     // 0x30 clipped velocity impulse
    char pad[0x50 - 0x3c];
};
struct ImpStage {                     // object passed as param_1 (stage+0x1080 = speed threshold)
    void AddContact(float* p, float* n, float strength);  // 0x459890
};
#endif
