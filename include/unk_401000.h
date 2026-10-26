// Replay recorder pool (0x401270-0x4018d0). Record = 0x60 bytes, next-free link at +0x5c.
#ifndef UNK_401000_H
#define UNK_401000_H
#include <vector>
struct RecSlot {
    RecSlot() {}                       // 0x402c90 (empty)
    ~RecSlot() {}                      // 0x490ad0 (empty)
    BYTE data[0x5c];
    RecSlot* next;                     // 0x5c: free list / time-ordered list
};
class Car;
class RecPool {
public:
    RecPool();                         // 0x401270
    ~RecPool();                        // 0x4012d0
    void Start(int mode);              // 0x401380
    void Stop();                       // 0x4013b0
    void Push(Car* car, int clock);    // 0x4013c0
    void Grow();                       // 0x401650
    void Reset();                      // 0x401880
    RecSlot* Alloc();                  // 0x4018d0
    void Insert(RecSlot* s);           // 0x4018f0
    std::vector<RecSlot*> blocks;      // 0x00 (allocator byte), 0x04..0x0c
    RecSlot* head;                     // 0x10
    RecSlot* tail;                     // 0x14
    RecSlot* freeList;                 // 0x18
    RecSlot* cur;                      // 0x1c
    int count;                         // 0x20
    int lastTime;                      // 0x24
    int mode;                          // 0x28
};
#endif
