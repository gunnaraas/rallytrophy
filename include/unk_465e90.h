/* Car setup (0x465e90-0x46669f, stub 0x466670). Map: analysis/range-4649f0.md section 3.
 * Include after unk_481390.h, rallytrophy.h and unk_46a8a0.h (Drivetrain). The Car members of
 * this module are written against CarSetupView, a raw view of the Car fields they touch (Car
 * itself is not restructured). */
#ifndef UNK_465E90_H
#define UNK_465E90_H

#include <string.h>
#include "unk_4666a0.h"

struct TyreF2 {
    union {
        float v[2];
        int i[2];
    };
};
struct TyreF3 {
    union {
        float v[3];
        int i[3];
    };
};

// Tyre force constants of one surface (0x40 bytes). The 30 records at Car+0x1680 are the
// Dyn<NN>... values of tire<set>dynamics.ini, each multiplied by the tyre set's scale record at
// Car+0x1500+set*0x40 (same layout).
struct TyreSurface {
    float rollingResistance;           // 0x00
    float inducedDrag;                 // 0x04
    float pneumaticTrail;              // 0x08
    float pneumaticOffset;             // 0x0c
    TyreF3 zStiffness;               // 0x10
    TyreF3 xStiffness;               // 0x1c
    TyreF2 cStiffness;               // 0x28
    TyreF2 zFriction;                // 0x30
    TyreF2 xFriction;                // 0x38
};
typedef char TyreSurface_size_check[sizeof(TyreSurface) == 0x40 ? 1 : -1];

// std::string layouts (VC6: allocator byte, pointer, length, reserve) as the tyre loader uses them:
// the constructor calls the out-of-line _Tidy (0x402700), assignment from a C string is expanded
// inline around the out-of-line _Grow (0x402980) / _Eos (0x402960), append (0x403ba0) is out of
// line, and the destructor is expanded inline.
class PathString {
public:
    PathString(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    PathString(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        Assign(s);
    }
    ~PathString() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)              // std::string::_Tidy, expanded inline
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    void Append(const char* s, unsigned int n); // 0x403ba0
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    void Add(const char* s) { Append(s, strlen(s)); }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};
inline const std::string& AsString(const PathString& s) { return *(const std::string*)&s; }
TmpString MissingText(const PathString& prefix, const char* name); // 0x409130: operator+(string, const char*)

// The Car fields of the setup module.
struct CarSetupView {
    CarSetup setup;                    // 0x00: the five preset numbers
    float balance;                     // 0x14: SetBalancePreset
    float steerRange;                  // 0x18: SetSteeringPreset
    float steerRange2;                 // 0x1c
    BYTE unk20[0xd00 - 0x20];
    float spring0;                     // 0xd00: spring rate of wheel 0 (stride 0x34)
    BYTE unkd04[0xd34 - 0xd04];
    float spring1;                     // 0xd34
    BYTE unkd38[0xd68 - 0xd38];
    float damper0;                     // 0xd68
    BYTE unkd6c[0xd9c - 0xd6c];
    float damper1;                     // 0xd9c
    BYTE unkda0[0xdb8 - 0xda0];
    Drivetrain drive;                  // 0xdb8
    BYTE unkfa0[0x10ec - 0xfa0];
    float steerBase;                   // 0x10ec
    float steerBase2;                  // 0x10f0
    BYTE unk10f4[0x1108 - 0x10f4];
    float balanceLevel[3];             // 0x1108
    BYTE unk1114[0x1138 - 0x1114];
    float springTable[16];             // 0x1138 (indexed by SetSuspensionPreset)
    BYTE unk1178[0x11fc - 0x1178];
    float damperTable[16];             // 0x11fc
    BYTE unk123c[0x1500 - 0x123c];
    TyreSurface tyreScale[6];          // 0x1500: per tyre set
    TyreSurface surface[30];           // 0x1680: loaded surfaces
    void ApplyWheel(int n);            // 0x45a7a0 (thiscall on the Car)
};

#endif
