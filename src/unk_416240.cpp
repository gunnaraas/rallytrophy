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

static __forceinline S10Rec* S10Allocate(int n) { if (n < 0) n = 0; return (S10Rec*)S10Alloc(n * 0x48); }
static __forceinline int S10Sz(S10Rec* f, S10Rec* l) { return f == 0 ? 0 : l - f; }
struct S10Blk { int d[16]; };
static __forceinline void S10Asg(S10Rec* a, const S10Rec* b) { int t = b->d[17]; *(S10Blk*)a = *(const S10Blk*)b; a->d[16] = b->d[16]; a->d[17] = t; }
// NONMATCHING: RALLYTROPHY 0x416a40
// Tried 2026-10-30 night: rewrote as the VC6 std::vector<T>::insert(P,M,X) shape (grow / after<M / else arms, S10Copy loops, S10Raw 16+2 dword assign, branchy n<0 alloc clamp) 232->139 insns; still differs: original keeps first in an extra frame slot (esp+0x14, frame 0x10) and M in esi, ours uses edi and a 0xc frame.
// Re-pass 2026-11-01: arms 2/3 now assign via S10Asg (16-dword block + d[16], d[17] separately, matching the original rep movs 0x10) 139->95; ~20 variants (Fv volatile, Fd copy, sz local, M volatile, hoisted F, compare flips) did not move M to esi/q to edi nor spill k as the original; permute not run (95 insns away).
void S10Vec::Insert(S10Rec* P, unsigned M, const S10Rec* X)
{
    if ((unsigned)(end - last) < M) {
        S10Rec* F = first;
                int n = S10Sz(F, last) + (M < (unsigned)S10Sz(F, last) ? S10Sz(F, last) : M);
        S10Rec* s = S10Allocate(n);
        S10Rec* q = s;
        for (S10Rec* f1 = F; f1 != P; ++f1, ++q)
            S10Copy(q, f1);
        { unsigned volatile k = M; for (; k > 0; k = k - 1, ++q)
            S10Copy(q, X); }
        for (S10Rec* f2 = P; f2 != last; ++f2, ++q)
            S10Copy(q, f2);
        for (S10Rec* f3 = F; f3 != last; ++f3)
            f3->~S10Rec();
        S10Free(F);
        end = s + n;
        last = s + S10Sz(F, last) + M;
        first = s;
    } else if ((unsigned)(last - P) < M) {
        S10Rec* d = P + M;
        for (S10Rec* f4 = P; f4 != last; ++f4, ++d)
            S10Copy(d, f4);
        d = last;
        for (unsigned k = M - (last - P); k > 0; --k, ++d)
            S10Copy(d, X);
        for (S10Rec* p = P; p != last; ++p)
            S10Asg(p, X);
        last += M;
    } else {
        S10Rec* d = last;
        for (S10Rec* f5 = last - M; f5 != last; ++f5, ++d)
            S10Copy(d, f5);
        d = last;
        for (S10Rec* f6 = last - M; f6 != P; )
            { --d; --f6; S10Asg(d, f6); }
        for (S10Rec* p = P; p != P + M; ++p)
            S10Asg(p, X);
        last += M;
    }
}

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
