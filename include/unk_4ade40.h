#pragma once
// CarModelCache (0x4ade40-0x4ae4b0), layout per analysis/range-4a9b60.md
class CarModelCache {
public:
    int   f0;        // 0x00
    void* device;    // 0x04 (vtable +0x120/+0x128 = release callbacks)
    void* listHead;  // 0x08
    void* listTail;  // 0x0c
    void* freeList;  // 0x10
    int   freeCnt;   // 0x14
    int   usedCnt;   // 0x18
    char  b1c;       // 0x1c
    int   v20, v24, v28;  // 0x20 vector first/last/end
    char  b2c;       // 0x2c
    int   v30, v34, v38;  // 0x30 vector
    CarModelCache();             // 0x4ade40
    void Destroy();              // 0x4ade80 (dtor body)
};
