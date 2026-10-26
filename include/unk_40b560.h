/* Sprite trail queue (0x40b560-0x40b600, 0xa8 bytes, W7_Queue in unk_45ad10.cpp) and a five-vector holder (0x40b8d0-0x40c0d0). */
#ifndef UNK_40B560_H
#define UNK_40B560_H
#include <vector>

class TrailQueue {
public:
    void Clear();                      // 0x40b560
    void Push(int corner, int time);   // 0x40b580
    void Draw(int now);                // 0x40b600
    DWORD color[4];                    // 0x00: alpha is the top byte
    float quad[4][8];                  // 0x10: four corner (x,y) pairs per slot
    int time[4];                       // 0x90
    char used[4];                      // 0xa0
    int head;                          // 0xa4
};

struct TimedEntry {
    int ptr;
    float key;
};

// Plain int vector the 0x40b9b0 routine appends to (begin 0x124, end 0x128, size 0x12c, cap 0x130, grow 0x134).
struct GrowArray {
    BYTE pad[0x124];
    int* begin;
    int* end;
    int size;
    int cap;
    int grow;
};

class TimedLists {
public:
    TimedLists();                      // 0x40b8d0
    ~TimedLists();                     // 0x40b930
    void Collect(GrowArray* out);      // 0x40b9b0
    void InsertA(int ptr, float key);  // 0x40bb40
    void InsertB(int ptr, float key);  // 0x40bd30
    void PushC(int v);                 // 0x40bf20
    void PushD(int v);                 // 0x40c0d0
    std::vector<TimedEntry> a, b;      // 0x00, 0x10
    std::vector<int> c, d, e;          // 0x20, 0x30, 0x40
};
#endif
