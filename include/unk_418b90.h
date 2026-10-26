// Swept-sphere query against the collected collision triangles (0x418b90). Same layout as TriCollector (unk_418570.h).
#ifndef UNK_418B90_H
#define UNK_418B90_H
#include <string>
#include <vector>
#include "unk_41ca70.h"

struct SweepQuery {
    std::vector<CTri*> hits;     // 0x00 (pad, first, last, end)
    CTri* head;                  // 0x10
    float Cast(float r, float* org, float* dir, float* outPt, float* outN); // 0x418b90
};
#endif
