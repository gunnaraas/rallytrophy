#ifndef UNK_416240_H
#define UNK_416240_H
// Stage helpers 0x416240-0x416ef0 (zone lookup, 0x48-element vector insert, save block).
struct S10Rec {                      // 0x48-byte element (copy helper 0x416d40, dtor 0x402c90)
    int d[18];
    S10Rec();
    ~S10Rec();
};
struct S10CellSub {
    char pad[0x44];
    int** first;                     // 0x44 (cell+0x50)
    int** last;                      // 0x48 (cell+0x54)
};
struct S10Cell {
    char pad[0xc];
    S10CellSub sub;                  // 0xc
};
class S10Stage {
public:
    void* SoundForRoll(int i);       // 0x416240
    int* FindZone(float* p);         // 0x416980
    S10Cell* Cell(float* p);         // 0x411080
    void Save(class File* f);        // 0x416e70
};
struct S10Vec {
    int pad0;
    S10Rec* first;                   // 4
    S10Rec* last;                    // 8
    S10Rec* end;                     // 0xc
    S10Rec* Insert(S10Rec* pos, unsigned n, const S10Rec* val); // 0x416a40
};
#endif
