#ifndef UNK_414FE0_H
#define UNK_414FE0_H
// Stage helpers 0x414fe0-0x416220 (checkpoint-name placement, ambient sound holders and loop).
#include <string>
#include "rallytrophy.h"

// A VC6 std::string whose helpers stay out of line (names: max_size, _Xlen, _Copy, _Eos, _Tidy).
struct PlaceStr {
    BYTE alloc;
    char* ptr;                         // +0x04
    unsigned int len;                  // +0x08
    unsigned int res;                  // +0x0c
    unsigned int MaxSize();            // 0x402aa0
    static void Xlen();                       // 0x4bd00b
    void Copy(unsigned int n);         // 0x402ab0
    void Eos(unsigned int n);          // 0x402960
    void Tidy(char built);             // 0x402700
    // non-const operator[] on the 6th character: split a shared buffer, then freeze it
    const char* Chars6();
};

// Placement object: floats at 0x14.., name string (VC6 layout) at 0x114.
struct PlaceObj {
    float f[0x114 / 4];
    PlaceStr name;                     // 0x114
};

// One ambient sound slot (0x88 bytes): a SoundSource plus random timing.
struct AmbientSound : public SoundSource {
    BYTE pad[0x74 - sizeof(SoundSource)];
    int timer;                         // 0x74
    float minTime, maxTime;            // 0x78, 0x7c
    float minVol, maxVol;              // 0x80, 0x84
};

class StageSounds {
public:
    BYTE unk0[0xa0c];
    float placed[0x48 / 4];            // 0xa0c: 0x48-byte records
    void FUN_00414fe0(PlaceObj* o);    // 0x414fe0
    void FUN_004150b0(PlaceObj* o);    // 0x4150b0
    void FUN_00416010();               // 0x416010
    void FUN_00416050();               // 0x416050
    void FUN_004160c0(int dt);         // 0x4160c0
    BYTE* FUN_004161c0(int i);         // 0x4161c0
    BYTE* FUN_004161e0(int i);         // 0x4161e0
    BYTE* FUN_00416200(int i);         // 0x416200
    BYTE* FUN_00416220(int i);         // 0x416220
};

struct SoundHolder416000 : public SoundSource {
    SoundHolder416000();               // 0x416000
};
#endif
