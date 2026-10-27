/* Stage loader containers (map: analysis/range-409690.md, M4). Elements are VC6 std::vector<> with the 4-byte
 * allocator head: {alloc, first, last, end}. Include after rallytrophy.h. */
#ifndef UNK_40C280_H
#define UNK_40C280_H
#include <memory>
#include <vector>

struct StagePair { int a, b; };            // 8-byte element copied by 0x40c430 / 0x40c470 / 0x40c4a0

void __cdecl StageFree(void* p);           // 0x4bd000 (operator delete; not declared throw())

struct StageIntVec {                       // std::vector<int> layout (0x10 bytes)
    std::allocator<int> allocator;
    int* first;                            // 0x04
    int* last;                             // 0x08
    int* end;                              // 0x0c
    int* Ucopy(int* f, int* l, int* d);    // 0x424320 (member, args on stack)
    void Ufill(int* d, unsigned int n, const int* x); // 0x424350
    void Destroy(int* f, int* l);          // 0x403130
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    StageIntVec(const std::allocator<int>& a = std::allocator<int>()) : allocator(a), first(0), last(0), end(0) {}
    ~StageIntVec() { StageFree(first); first = 0; last = 0; end = 0; }
};

// Five vectors at 0x00..0x4f (ctor 0x40b8d0, dtor 0x40b930; both out of line). 0x40c280 is vector::insert(end(), 1, x)
// of the 5th (the vector at 0x40).
struct StageVecs5 {
    BYTE pad[0x40];
    StageIntVec v4;                        // 0x40
    StageVecs5();                          // 0x40b8d0
    ~StageVecs5();                         // 0x40b930
    void InsertBack5(int x);               // 0x40c280
};

// The stage loader object: base vectors at 0x0c, own vectors at 0x60..0xa0 (ctor 0x40c520, dtor 0x40c5e0).
// The head at 0x00 has an empty dtor (EH state 0); the own vectors are real std::vector<int> (no unwind states).
struct StageHead { BYTE b[0xc]; ~StageHead() {} };
class StageLoader {
public:
    StageHead unk0;
    StageVecs5 base;                       // 0x0c
    BYTE unk5c[4];
    std::vector<int> w0, w1, w2, w3, w4;     // 0x60, 0x70, 0x80, 0x90, 0xa0
    StageLoader();                         // 0x40c520
    ~StageLoader();                        // 0x40c5e0
};

// Holder of one sound sample, released through DAT_0056b0b0 (0x40c6f0 ctor, 0x40c700 dtor).
struct SampleDt { ~SampleDt() {} };   // empty-destructor members only reproduce the original unwind state 4
class SampleHolder {
public:
    void* sample;
    SampleDt d0, d1, d2, d3, d4;
    SampleHolder() : sample(0) {}  // 0x40c6f0
    ~SampleHolder();               // 0x40c700
};
#endif
