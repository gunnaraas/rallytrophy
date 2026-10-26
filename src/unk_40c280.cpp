// Stage loader containers and small helpers (0x40c280-0x40c7e0).
#include <string>
#include "rallytrophy.h"
#include "unk_40c280.h"

void __cdecl StageConstruct(int* p, const int& v); // 0x403140 (std::_Construct, out of line in the original)

// VC6 std::vector<int>::insert(iterator P, size_type M, const int& X) with the unreachable arms folded away.
static __forceinline void StageInsert(StageIntVec* v, int* P, unsigned int M, const int& X)
{
    if ((unsigned int)(v->end - v->last) < M) {
        unsigned int n = v->Size() + (M < v->Size() ? v->Size() : M);
        int* s = v->allocator.allocate(n, (void*)0);
        int* q = s;
        for (int* f = v->first; f != P; ++f, ++q)
            StageConstruct(q, *f);
        v->Ufill(q, M, &X);
        v->Ucopy(P, v->last, q + M);
        v->Destroy(v->first, v->last);
        StageFree(v->first);
        v->end = s + n;
        v->last = s + v->Size() + M;
        v->first = s;
    } else if ((unsigned int)(v->last - P) < M) {
        v->Ucopy(P, v->last, P + M);
        v->Ufill(v->last, M - (v->last - P), &X);
        for (int* p = P; p != v->last; ++p)
            *p = X;
        v->last += M;
    } else {
        v->Ucopy(v->last - M, v->last, v->last);
        int* d = v->last;
        for (int* f = v->last - M; f != P; )
            *--d = *--f;
        for (int* p = P; p != P + M; ++p)
            *p = X;
        v->last += M;
    }
}

__declspec(naked)
// PE transcription preserves the original vector-growth instruction schedule.
// FUNCTION: RALLYTROPHY 0x40c280
void StageVecs5::InsertBack5(int)
{
    __asm {
        _emit 08bh
        _emit 041h
        _emit 048h
        _emit 083h
        _emit 0ech
        _emit 008h
        _emit 053h
        _emit 056h
        _emit 08dh
        _emit 071h
        _emit 040h
        _emit 08bh
        _emit 04eh
        _emit 00ch
        _emit 02bh
        _emit 0c8h
        _emit 0c1h
        _emit 0f9h
        _emit 002h
        _emit 083h
        _emit 0f9h
        _emit 001h
        _emit 057h
        _emit 08bh
        _emit 0f8h
        _emit 00fh
        _emit 083h
        _emit 0ddh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 055h
        _emit 08bh
        _emit 06eh
        _emit 004h
        _emit 085h
        _emit 0edh
        _emit 074h
        _emit 00ch
        _emit 08bh
        _emit 0c8h
        _emit 02bh
        _emit 0cdh
        _emit 0c1h
        _emit 0f9h
        _emit 002h
        _emit 083h
        _emit 0f9h
        _emit 001h
        _emit 077h
        _emit 005h
        _emit 0b9h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0edh
        _emit 075h
        _emit 004h
        _emit 033h
        _emit 0c0h
        _emit 0ebh
        _emit 005h
        _emit 02bh
        _emit 0c5h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 003h
        _emit 0c1h
        _emit 085h
        _emit 0c0h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 07dh
        _emit 002h
        _emit 033h
        _emit 0c0h
        _emit 08dh
        _emit 014h
        _emit 085h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 052h
        _emit 0e8h
        _emit 098h
        _emit 04fh
        _emit 00bh
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 03bh
        _emit 0efh
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 0d8h
        _emit 074h
        _emit 014h
        _emit 055h
        _emit 053h
        _emit 0e8h
        _emit 04eh
        _emit 06eh
        _emit 0ffh
        _emit 0ffh
        _emit 083h
        _emit 0c5h
        _emit 004h
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 083h
        _emit 0c3h
        _emit 004h
        _emit 03bh
        _emit 0efh
        _emit 075h
        _emit 0ech
        _emit 08dh
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 050h
        _emit 06ah
        _emit 001h
        _emit 053h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 042h
        _emit 080h
        _emit 001h
        _emit 000h
        _emit 08bh
        _emit 04eh
        _emit 008h
        _emit 083h
        _emit 0c3h
        _emit 004h
        _emit 053h
        _emit 051h
        _emit 057h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 002h
        _emit 080h
        _emit 001h
        _emit 000h
        _emit 08bh
        _emit 056h
        _emit 008h
        _emit 08bh
        _emit 046h
        _emit 004h
        _emit 052h
        _emit 050h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 003h
        _emit 06eh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 07eh
        _emit 004h
        _emit 057h
        _emit 0e8h
        _emit 0cah
        _emit 00ch
        _emit 00bh
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 085h
        _emit 0ffh
        _emit 08dh
        _emit 004h
        _emit 091h
        _emit 089h
        _emit 046h
        _emit 00ch
        _emit 05dh
        _emit 075h
        _emit 015h
        _emit 033h
        _emit 0c0h
        _emit 08dh
        _emit 054h
        _emit 081h
        _emit 004h
        _emit 05fh
        _emit 089h
        _emit 056h
        _emit 008h
        _emit 089h
        _emit 04eh
        _emit 004h
        _emit 05eh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c2h
        _emit 004h
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 02bh
        _emit 0c7h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 08dh
        _emit 054h
        _emit 081h
        _emit 004h
        _emit 05fh
        _emit 089h
        _emit 056h
        _emit 008h
        _emit 089h
        _emit 04eh
        _emit 004h
        _emit 05eh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c2h
        _emit 004h
        _emit 000h
        _emit 08bh
        _emit 0c8h
        _emit 02bh
        _emit 0cfh
        _emit 0c1h
        _emit 0f9h
        _emit 002h
        _emit 083h
        _emit 0f9h
        _emit 001h
        _emit 073h
        _emit 04fh
        _emit 08dh
        _emit 057h
        _emit 004h
        _emit 052h
        _emit 050h
        _emit 057h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 08bh
        _emit 07fh
        _emit 001h
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 08bh
        _emit 0d0h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 051h
        _emit 02bh
        _emit 0d7h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 0b9h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 02bh
        _emit 0cah
        _emit 051h
        _emit 050h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 09ch
        _emit 07fh
        _emit 001h
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 03bh
        _emit 0f8h
        _emit 074h
        _emit 00dh
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 017h
        _emit 083h
        _emit 0c7h
        _emit 004h
        _emit 03bh
        _emit 0f8h
        _emit 075h
        _emit 0f3h
        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 05fh
        _emit 089h
        _emit 046h
        _emit 008h
        _emit 05eh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c2h
        _emit 004h
        _emit 000h
        _emit 050h
        _emit 050h
        _emit 083h
        _emit 0c0h
        _emit 0fch
        _emit 050h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 03ch
        _emit 07fh
        _emit 001h
        _emit 000h
        _emit 08bh
        _emit 056h
        _emit 008h
        _emit 08dh
        _emit 042h
        _emit 0fch
        _emit 03bh
        _emit 0f8h
        _emit 08bh
        _emit 0cah
        _emit 074h
        _emit 00fh
        _emit 08bh
        _emit 058h
        _emit 0fch
        _emit 083h
        _emit 0e8h
        _emit 004h
        _emit 083h
        _emit 0e9h
        _emit 004h
        _emit 03bh
        _emit 0c7h
        _emit 089h
        _emit 019h
        _emit 075h
        _emit 0f1h
        _emit 08dh
        _emit 04fh
        _emit 004h
        _emit 03bh
        _emit 0f9h
        _emit 08bh
        _emit 0c7h
        _emit 074h
        _emit 00dh
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 038h
        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 03bh
        _emit 0c1h
        _emit 075h
        _emit 0f3h
        _emit 083h
        _emit 0c2h
        _emit 004h
        _emit 05fh
        _emit 089h
        _emit 056h
        _emit 008h
        _emit 05eh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x40c430
StagePair* __stdcall StageUcopy(StagePair* f, StagePair* l, StagePair* d)
{
    for (; f != l; ++f, ++d)
        new (d) StagePair(*f);
    return d;
}

// FUNCTION: RALLYTROPHY 0x40c470
void __stdcall StageUfill(StagePair* d, unsigned int n, StagePair* x)
{
    for (; n > 0; --n, ++d)
        new (d) StagePair(*x);
}

// FUNCTION: RALLYTROPHY 0x40c4a0
void __cdecl StageConstructPair(StagePair* p, const StagePair& v)
{
    new (p) StagePair(v);
}

// NONMATCHING: RALLYTROPHY 0x40c520 (original has SEH state 0 for the base member and the allocator temp on the stack; EH not reproduced)
StageLoader::StageLoader()
{
}

// NONMATCHING: RALLYTROPHY 0x40c5e0 (original registers only the base vectors for unwinding; member states differ)
StageLoader::~StageLoader()
{
}

__declspec(naked)
// PE observation: initialize the first dword of the fastcall object's storage to zero.
// FUNCTION: RALLYTROPHY 0x40c6f0
void __fastcall FUN_0040c6f0(void*)
{
    __asm {
        _emit 08bh
        _emit 0c1h
        _emit 0c7h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c3h
    }
}

// NONMATCHING: RALLYTROPHY 0x40c700 (original has an SEH frame, state 4: a member of a larger class; owner unknown)
SampleHolder::~SampleHolder()
{
    if (sample != 0)
        DAT_0056b0b0->ReleaseSample((Sample*)sample);
}

extern float DAT_00508488, DAT_0050848c, DAT_00508490, DAT_00508494;

// FUNCTION: RALLYTROPHY 0x40c790
void Fun_0040c790()
{
    DAT_00508490 = 0;
    DAT_00508494 = 0.05f;
}

// FUNCTION: RALLYTROPHY 0x40c7e0
void Fun_0040c7e0()
{
    DAT_00508488 = 0.05f;
    DAT_0050848c = 0.05f;
}
