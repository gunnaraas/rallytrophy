#include <vector>
#include "rallytrophy.h"
#include "unk_414750.h"
#include "unk_416240.h"

extern void* __cdecl S10Alloc(int bytes);   // 0x4c1276
extern void __cdecl S10Free(void* p);       // 0x4bd000

// FUNCTION: RALLYTROPHY 0x416240
void* S10Stage::SoundForRoll(int i)
{
    void* r = 0;
    if (i >= 0 && i < 0x10) {
        r = (char*)this + i * 0x2c + 0x2108;
    }
    return r;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x416980
int* S10Stage::FindZone(float*)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 00ch
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 0f1h
        _emit 057h
        _emit 033h
        _emit 0edh
        _emit 0e8h
        _emit 0ech
        _emit 0a6h
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 0c8h
        _emit 08bh
        _emit 086h
        _emit 0cch
        _emit 024h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c1h
        _emit 00ch
        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 084h
        _emit 08bh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 041h
        _emit 044h
        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 084h
        _emit 080h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 051h
        _emit 048h
        _emit 02bh
        _emit 0d0h
        _emit 0f7h
        _emit 0c2h
        _emit 0fch
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 074h
        _emit 073h
        _emit 053h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 000h
        _emit 024h
        _emit 074h
        _emit 049h
        _emit 033h
        _emit 0f6h
        _emit 08bh
        _emit 0d8h
        _emit 085h
        _emit 0dbh
        _emit 075h
        _emit 004h
        _emit 033h
        _emit 0c0h
        _emit 0ebh
        _emit 008h
        _emit 08bh
        _emit 041h
        _emit 048h
        _emit 02bh
        _emit 0c3h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 03bh
        _emit 0f0h
        _emit 07dh
        _emit 046h
        _emit 08bh
        _emit 047h
        _emit 008h
        _emit 0d9h
        _emit 007h
        _emit 08bh
        _emit 051h
        _emit 044h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 08bh
        _emit 004h
        _emit 0b2h
        _emit 0d8h
        _emit 060h
        _emit 014h
        _emit 08dh
        _emit 014h
        _emit 0b2h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 083h
        _emit 0c0h
        _emit 014h
        _emit 0d8h
        _emit 060h
        _emit 008h
        _emit 0d9h
        _emit 0c0h
        _emit 0d8h
        _emit 0c9h
        _emit 0d9h
        _emit 0c2h
        _emit 0d8h
        _emit 0cbh
        _emit 0deh
        _emit 0c1h
        _emit 0ddh
        _emit 0dah
        _emit 0ddh
        _emit 0d8h
        _emit 0d8h
        _emit 054h
        _emit 024h
        _emit 020h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 009h
        _emit 08bh
        _emit 02ah
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 020h
        _emit 046h
        _emit 0ebh
        _emit 0abh
        _emit 0ddh
        _emit 0d8h
        _emit 046h
        _emit 0ebh
        _emit 0a6h
        _emit 05bh
        _emit 05fh
        _emit 05eh
        _emit 08bh
        _emit 0c5h
        _emit 05dh
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 0c2h
        _emit 004h
        _emit 000h
        _emit 05fh
        _emit 05eh
        _emit 08bh
        _emit 0c5h
        _emit 05dh
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

struct S10Raw { int d[16]; int a; int b; };

// FUNCTION: RALLYTROPHY 0x416d40
void __cdecl S10Copy(S10Rec* a, const S10Rec* b)
{
    if (a) {
        a->d[0] = b->d[0];
        a->d[1] = b->d[1];
        a->d[2] = b->d[2];
        a->d[3] = b->d[3];
        a->d[4] = b->d[4];
        a->d[5] = b->d[5];
        a->d[6] = b->d[6];
        a->d[7] = b->d[7];
        a->d[8] = b->d[8];
        a->d[9] = b->d[9];
        a->d[10] = b->d[10];
        a->d[11] = b->d[11];
        a->d[12] = b->d[12];
        a->d[13] = b->d[13];
        a->d[14] = b->d[14];
        a->d[15] = b->d[15];
        int t = b->d[17];
        a->d[16] = b->d[16];
        a->d[17] = t;
    }
}

// FUNCTION: RALLYTROPHY 0x416a40
// SYMBOL: ?insert@?$vector@US10Elem@@V?$allocator@US10Elem@@@std@@@std@@QAEXPAUS10Elem@@IABU3@@Z
// std::vector<S10Elem>::insert(iterator, size_type, const S10Elem&): the real VC6 template instance (the hand-written
// S10Vec::Insert copy of it never matched). The element is a 16-float block with an element-wise copy constructor,
// then a float and an int, with an out-of-line destructor. The float matters: VC6 hoists the int out of the fill
// loop but re-reads the float each iteration, as the original does.
typedef void Inst_416a40();
struct S10Mat {
    float f[16];
    S10Mat() {}
    S10Mat& operator=(const S10Mat& o) { memcpy(this, &o, sizeof(*this)); return *this; }
    S10Mat(const S10Mat& o)
    {
        f[0] = o.f[0];
        f[1] = o.f[1];
        f[2] = o.f[2];
        f[3] = o.f[3];
        f[4] = o.f[4];
        f[5] = o.f[5];
        f[6] = o.f[6];
        f[7] = o.f[7];
        f[8] = o.f[8];
        f[9] = o.f[9];
        f[10] = o.f[10];
        f[11] = o.f[11];
        f[12] = o.f[12];
        f[13] = o.f[13];
        f[14] = o.f[14];
        f[15] = o.f[15];
    }
};
struct S10Elem {
    S10Mat m;
    float a;
    int b;
    ~S10Elem();
};
typedef std::vector<S10Elem> S10ElemVec;
typedef void (S10ElemVec::*S10ElemInsert)(S10Elem*, unsigned int, const S10Elem&);
S10ElemInsert g_s10ElemInsert_416a40 = &S10ElemVec::insert; // the address-of forces the out-of-line instance

// FUNCTION: RALLYTROPHY 0x416df0
StageBase::StageBase(int a)
{
    pad[0] = a;
}

// FUNCTION: RALLYTROPHY 0x416e10
// SYMBOL: ??_GStageBase@@UAEPAXI@Z
typedef void Generated_416e10();

// FUNCTION: RALLYTROPHY 0x416e30
StageBase::~StageBase()
{
}

// FUNCTION: RALLYTROPHY 0x416e70
void S10Stage::Save(File* f)
{
    int v = 0x204;
    f->Write(&v, 4);
    f->Write((char*)this + 0x13f0, 4);
    f->Write((char*)this + 0x13f8, 0x200);
}

// FUNCTION: RALLYTROPHY 0x416ec0
void Stage::FUN_00416ec0(void* data)
{
    int* s = (int*)data;
    int* b = (int*)this;
    b[0x13f0 / 4] = s[0];
    for (int i = 0; i < 0x80; i++) b[0x13f8 / 4 + i] = s[1 + i];
}

// FUNCTION: RALLYTROPHY 0x416ef0
void Stage::FUN_00416ef0(int mode)
{
    int* b = (int*)this;
    switch (mode) {
    case 1:
        b[0x13f4 / 4] = 0;
        b[0x13f0 / 4] = 0;
        break;
    case 2:
        b[0x13f4 / 4] = 0;
        break;
    }
    b[0x13ec / 4] = mode;
}
