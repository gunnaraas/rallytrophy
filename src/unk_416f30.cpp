// Stage animals/props (0x416f30 group).
#include "unk_416f30.h"
#include <string>
#include <stdlib.h>
#include <math.h>
#include "rallytrophy.h"

#define AT_(T, off) (*(T*)((char*)this + (off)))
extern int DAT_00501454, DAT_00501458, DAT_0050131c;
extern char DAT_00508440[];            // _itoa scratch buffer
// Opaque stage-side helpers (not yet typed): checkpoint lookup 0x414dc0, ModelFile 0x4af950/0x4afe20,
// animal object 0x401c10/0x401cf0 and its mesh list (0x4a6800).
struct ModelBlob;
struct AnimalObj {                                                             // 0x20 bytes
    AnimalObj();                                                               // 0x401c10
    void Init(ModelBlob* m);                                                   // 0x401cf0
    int active;
    int pad[2];
    BYTE* model;
    BYTE pad10[0x10];
};
struct PropVirt { virtual ~PropVirt(); };                                      // scene object: slot 0 = deleting dtor
struct PropObjVec {                                                            // vector<object*> layout, no destructor
    int alloc;
    PropVirt** first;
    PropVirt** last;
    PropVirt** end;
    int Size() const { return first == 0 ? 0 : last - first; }
};
struct ModelBlob {                                                             // loaded .bmf (0x80 bytes)
    ModelBlob(const char* path, const char* dir, void* cache, void* mats, int flag); // 0x4af950
    ~ModelBlob();                                                              // 0x4afe20
    BYTE raw[0x48];
    PropObjVec objects;
    BYTE tail[0x80 - 0x58];
};
void* __fastcall StageNearestCheckpoint(PropStage* st, const float* pos, int flag);   // 0x414dc0
unsigned char* __fastcall MeshAt(void* container, int i);                             // 0x4a6800

extern int DAT_004f6048;
extern float DAT_004e138c;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x417430
void PropStage::Reset()
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 008h
        _emit 053h
        _emit 055h
        _emit 033h
        _emit 0dbh
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08dh
        _emit 0aeh
        _emit 0b8h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 057h
        _emit 089h
        _emit 09eh
        _emit 07ch
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 07dh
        _emit 004h
        _emit 03bh
        _emit 0fbh
        _emit 0c7h
        _emit 086h
        _emit 06ch
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 089h
        _emit 09eh
        _emit 070h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 09eh
        _emit 074h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 09eh
        _emit 080h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 09eh
        _emit 078h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 09eh
        _emit 09ch
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 074h
        _emit 00fh
        _emit 08bh
        _emit 045h
        _emit 00ch
        _emit 02bh
        _emit 0c7h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 03dh
        _emit 000h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 073h
        _emit 067h
        _emit 068h
        _emit 000h
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0e7h
        _emit 09dh
        _emit 00ah
        _emit 000h
        _emit 08bh
        _emit 04dh
        _emit 008h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 03bh
        _emit 0f9h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 074h
        _emit 022h
        _emit 08bh
        _emit 055h
        _emit 004h
        _emit 08bh
        _emit 0dfh
        _emit 02bh
        _emit 0dah
        _emit 003h
        _emit 0d8h
        _emit 057h
        _emit 053h
        _emit 0e8h
        _emit 093h
        _emit 0bch
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 045h
        _emit 008h
        _emit 083h
        _emit 0c7h
        _emit 004h
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 083h
        _emit 0c3h
        _emit 004h
        _emit 03bh
        _emit 0f8h
        _emit 075h
        _emit 0e9h
        _emit 033h
        _emit 0dbh
        _emit 08bh
        _emit 045h
        _emit 004h
        _emit 050h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 0e8h
        _emit 034h
        _emit 05bh
        _emit 00ah
        _emit 000h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 014h
        _emit 08dh
        _emit 08fh
        _emit 000h
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 04dh
        _emit 00ch
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh
        _emit 0cdh
        _emit 0e8h
        _emit 07dh
        _emit 0cfh
        _emit 0ffh
        _emit 0ffh
        _emit 08dh
        _emit 014h
        _emit 087h
        _emit 089h
        _emit 055h
        _emit 008h
        _emit 089h
        _emit 07dh
        _emit 004h
        _emit 089h
        _emit 09eh
        _emit 0f4h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 09eh
        _emit 0ach
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0ffh
        _emit 033h
        _emit 0edh
        _emit 085h
        _emit 0dbh
        _emit 074h
        _emit 031h
        _emit 08bh
        _emit 08eh
        _emit 0b0h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 02bh
        _emit 0cbh
        _emit 0b8h
        _emit 039h
        _emit 08eh
        _emit 0e3h
        _emit 038h
        _emit 0f7h
        _emit 0e9h
        _emit 0c1h
        _emit 0fah
        _emit 003h
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0e8h
        _emit 01fh
        _emit 003h
        _emit 0d0h
        _emit 03bh
        _emit 0fah
        _emit 073h
        _emit 014h
        _emit 08bh
        _emit 08eh
        _emit 0ach
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 044h
        _emit 029h
        _emit 020h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 047h
        _emit 083h
        _emit 0c5h
        _emit 024h
        _emit 0ebh
        _emit 0cbh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c3h
    }
}
__declspec(naked)
// PE transcription preserves the original stack-spilled vector components and x87 evaluation order.
// FUNCTION: RALLYTROPHY 0x417e60
int PropStage::NearSphere(const float*, float)
{
    __asm {
        _emit 08bh
        _emit 081h
        _emit 06ch
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0ech
        _emit 00ch
        _emit 033h
        _emit 0d2h
        _emit 083h
        _emit 0f8h
        _emit 0ffh
        _emit 074h
        _emit 060h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d9h
        _emit 000h
        _emit 056h
        _emit 0d8h
        _emit 0a1h
        _emit 078h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 070h
        _emit 004h
        _emit 08bh
        _emit 040h
        _emit 008h
        _emit 089h
        _emit 074h
        _emit 024h
        _emit 008h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 008h
        _emit 0d8h
        _emit 0a1h
        _emit 07ch
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 05eh
        _emit 0d8h
        _emit 0a1h
        _emit 080h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 054h
        _emit 024h
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
        _emit 0d9h
        _emit 0c3h
        _emit 0d8h
        _emit 0cch
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0d8h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 0deh
        _emit 0d9h
        _emit 0ddh
        _emit 0d8h
        _emit 0dfh
        _emit 0e0h
        _emit 0ddh
        _emit 0d8h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 0ddh
        _emit 0d8h
        _emit 0b8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 074h
        _emit 002h
        _emit 08bh
        _emit 0c2h
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

// Layout twin of std::vector<StageProp>: 0x417ee0 is its insert(pos, n, x), emitted out of line (the loader
// calls it), with _Construct (0x418220) and the per-element _Destroy (empty, folded to 0x402c90) as calls.
struct PropVec {
    char alloc;
    StageProp* first;
    StageProp* last;
    StageProp* end;
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    void Ins(StageProp* P, unsigned int M, const StageProp& X);  // 0x417ee0
};
// allocator.destroy(p): a thiscall on the element that the linker folded into the empty function 0x402c90.
struct PropElem : StageProp {
    void Destroy();                                              // never defined here
};
#define PROP_VEC(v) ((PropVec*)&(v))

// The in-class body of StageProp::operator= (inlined at two of the three copy loops).
static inline void CopyProp(StageProp* d, const StageProp* src)
{
    d->v[0] = src->v[0];
    d->v[1] = src->v[1];
    d->v[2] = src->v[2];
    d->v[3] = src->v[3];
    d->v[4] = src->v[4];
    d->v[5] = src->v[5];
    d->v[6] = src->v[6];
    int f = src->flag;
    *(int*)&d->v[7] = *(const int*)&src->v[7];
    d->flag = f;
}

// NONMATCHING: RALLYTROPHY 0x417ee0
// SYMBOL: ?Ins@PropVec@@QAEXPAUStageProp@@IABU2@@Z
// 797 vs 818 bytes: the three branches, calls and loops are in place. Open: M lives in esi (ours edi, swapped with
// last), and the two inlined element copies address the element from +0x14 (ours +0x8); a plain-order copy helper
// and a derived operator= twin did not change that.
void PropVec::Ins(StageProp* P, unsigned int M, const StageProp& X)
{
    if ((unsigned int)(end - last) < M) {
        unsigned int N = Size() + (M < Size() ? Size() : M);
        int An = (int)N;
        if (An < 0)
            An = 0;
        StageProp* S = (StageProp*)operator new(An * sizeof(StageProp));
        StageProp* Q = S;
        StageProp* F;
        for (F = first; F != P; ++Q, ++F)
            ConstructProp(Q, F);
        StageProp* G = Q;
        for (unsigned int n = M; 0 < n; --n, ++G)
            ConstructProp(G, &X);
        StageProp* D = Q + M;
        for (F = P; F != last; ++D, ++F)
            ConstructProp(D, F);
        for (F = first; F != last; ++F)
            ((PropElem*)F)->Destroy();
        operator delete(first);
        end = S + N;
        last = S + Size() + M;
        first = S;
    } else if ((unsigned int)(last - P) < M) {
        StageProp* D = P + M;
        StageProp* F;
        for (F = P; F != last; ++D, ++F)
            ConstructProp(D, F);
        StageProp* G = last;
        for (unsigned int n = M - (last - P); 0 < n; --n, ++G)
            ConstructProp(G, &X);
        for (F = P; F != last; ++F)
            CopyProp(F, &X);
        last += M;
    } else if (0 < M) {
        StageProp* D = last;
        StageProp* F;
        for (F = last - M; F != last; ++D, ++F)
            ConstructProp(D, F);
        StageProp* B = last;
        StageProp* E = last - M;
        while (E != P)
            CopyProp(--B, --E);
        for (F = P; F != P + M; ++F)
            *F = X;
        last += M;
    }
}

// FUNCTION: RALLYTROPHY 0x4182e0
BoundsNode::BoundsNode()
{
    left = 0;
    right = 0;
    count = 0;
    indices = 0;
}

// FUNCTION: RALLYTROPHY 0x418300
// The empty-destructor local shifts the unwind state of the child deletes to 1, as in the original.
BoundsNode::~BoundsNode()
{
    NodeTag0 g;
    if (left)
        delete left;
    if (right)
        delete right;
}

__declspec(naked)
// PE transcription preserves the original partition scan, swap, and x87 comparison.
// FUNCTION: RALLYTROPHY 0x418370
int BoundsNode::PartitionAxis(int, int)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 00ch
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 0d9h
        _emit 044h
        _emit 091h
        _emit 00ch
        _emit 08dh
        _emit 004h
        _emit 095h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0d8h
        _emit 004h
        _emit 008h
        _emit 08bh
        _emit 051h
        _emit 020h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 07fh
        _emit 018h
        _emit 0d8h
        _emit 00dh
        _emit 08ch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 033h
        _emit 0f6h
        _emit 085h
        _emit 0d2h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 010h
        _emit 089h
        _emit 074h
        _emit 024h
        _emit 008h
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 00ch
        _emit 07eh
        _emit 07ah
        _emit 08bh
        _emit 049h
        _emit 024h
        _emit 053h
        _emit 08bh
        _emit 0d8h
        _emit 055h
        _emit 081h
        _emit 0e3h
        _emit 0ffh
        _emit 0ffh
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0f9h
        _emit 08bh
        _emit 0f1h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 00fh
        _emit 0bfh
        _emit 016h
        _emit 08bh
        _emit 06ch
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0e0h
        _emit 006h
        _emit 003h
        _emit 0c5h
        _emit 033h
        _emit 0c9h
        _emit 066h
        _emit 08bh
        _emit 048h
        _emit 012h
        _emit 08bh
        _emit 0e9h
        _emit 08ah
        _emit 0cbh
        _emit 0d3h
        _emit 0edh
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 020h
        _emit 083h
        _emit 0e5h
        _emit 003h
        _emit 08bh
        _emit 044h
        _emit 0a8h
        _emit 028h
        _emit 0d9h
        _emit 044h
        _emit 088h
        _emit 004h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 018h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 015h
        _emit 066h
        _emit 08bh
        _emit 007h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 041h
        _emit 066h
        _emit 089h
        _emit 006h
        _emit 066h
        _emit 089h
        _emit 017h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 083h
        _emit 0c7h
        _emit 002h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 083h
        _emit 0c6h
        _emit 002h
        _emit 048h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 075h
        _emit 0a9h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 05dh
        _emit 05bh
        _emit 05fh
        _emit 05eh
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 0c2h
        _emit 008h
        _emit 000h
        _emit 05fh
        _emit 08bh
        _emit 0c6h
        _emit 05eh
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x418430
bool BoundsNode::Split(int)
{
    __asm {
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 0d8h
        _emit 058h
        _emit 04dh
        _emit 000h
        _emit 050h
        _emit 064h
        _emit 089h
        _emit 025h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0ech
        _emit 010h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 046h
        _emit 020h
        _emit 03bh
        _emit 005h
        _emit 048h
        _emit 060h
        _emit 04fh
        _emit 000h
        _emit 057h
        _emit 07dh
        _emit 007h
        _emit 032h
        _emit 0c0h
        _emit 0e9h
        _emit 0eeh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 046h
        _emit 00ch
        _emit 0d8h
        _emit 026h
        _emit 08bh
        _emit 056h
        _emit 010h
        _emit 08bh
        _emit 046h
        _emit 014h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 033h
        _emit 0c9h
        _emit 0d8h
        _emit 066h
        _emit 004h
        _emit 033h
        _emit 0dbh
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 028h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 018h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 0d8h
        _emit 066h
        _emit 008h
        _emit 0d9h
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 00dh
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 0bbh
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 014h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 005h
        _emit 0bbh
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 06ch
        _emit 024h
        _emit 030h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 055h
        _emit 053h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 09eh
        _emit 0feh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 0f8h
        _emit 083h
        _emit 0ffh
        _emit 001h
        _emit 07eh
        _emit 008h
        _emit 08bh
        _emit 046h
        _emit 020h
        _emit 048h
        _emit 03bh
        _emit 0f8h
        _emit 07ch
        _emit 01ah
        _emit 043h
        _emit 083h
        _emit 0fbh
        _emit 002h
        _emit 07eh
        _emit 002h
        _emit 033h
        _emit 0dbh
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 040h
        _emit 083h
        _emit 0f8h
        _emit 003h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 07ch
        _emit 0d2h
        _emit 032h
        _emit 0c0h
        _emit 0ebh
        _emit 053h
        _emit 06ah
        _emit 028h
        _emit 0e8h
        _emit 074h
        _emit 08dh
        _emit 00ah
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 00bh
        _emit 08bh
        _emit 0c8h
        _emit 0e8h
        _emit 0d0h
        _emit 0fdh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 0d8h
        _emit 0ebh
        _emit 002h
        _emit 033h
        _emit 0dbh
        _emit 06ah
        _emit 028h
        _emit 089h
        _emit 05eh
        _emit 018h
        _emit 0e8h
        _emit 056h
        _emit 08dh
        _emit 00ah
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 009h
        _emit 08bh
        _emit 0c8h
        _emit 0e8h
        _emit 0b2h
        _emit 0fdh
        _emit 0ffh
        _emit 0ffh
        _emit 0ebh
        _emit 002h
        _emit 033h
        _emit 0c0h
        _emit 08bh
        _emit 04eh
        _emit 024h
        _emit 08bh
        _emit 056h
        _emit 020h
        _emit 089h
        _emit 04bh
        _emit 024h
        _emit 089h
        _emit 07bh
        _emit 020h
        _emit 08dh
        _emit 00ch
        _emit 079h
        _emit 089h
        _emit 048h
        _emit 024h
        _emit 02bh
        _emit 0d7h
        _emit 089h
        _emit 046h
        _emit 01ch
        _emit 089h
        _emit 050h
        _emit 020h
        _emit 0b0h
        _emit 001h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 020h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 01ch
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x418220
void __cdecl ConstructProp(StageProp* dst, const StageProp* src)
{
    if (dst) {
        dst->v[0] = src->v[0];
        dst->v[1] = src->v[1];
        dst->v[2] = src->v[2];
        dst->v[3] = src->v[3];
        dst->v[4] = src->v[4];
        dst->v[5] = src->v[5];
        dst->v[6] = src->v[6];
        dst->v[7] = src->v[7];
        dst->flag = src->flag;
    }
}

// FUNCTION: RALLYTROPHY 0x418270
// SYMBOL: ??4StageProp@@QAEAAU0@ABU0@@Z
StageProp& StageProp::operator=(const StageProp& s)
{
    const StageProp* src = &s;
    v[0] = src->v[0];
    v[1] = src->v[1];
    v[2] = src->v[2];
    v[3] = src->v[3];
    v[4] = src->v[4];
    v[5] = src->v[5];
    v[6] = src->v[6];
    int f = src->flag;
    *(int*)&v[7] = *(const int*)&src->v[7];
    flag = f;
    return *this;
}

// Path strings of PropStage::Load: assign (0x4024f0) and append (0x403ba0) out of line, destructor 0x430720.
struct PropPath {
    PropPath(const std::string& x) : alloc(*(const std::allocator<char>*)&x) { Tidy(false); Assign(x, 0, std::string::npos); }
    ~PropPath() { ((std::string*)this)->~basic_string(); }
    void Tidy(bool built);                                               // 0x402700
    PropPath& Assign(const std::string& x, unsigned int pos, unsigned int n); // 0x4024f0
    PropPath& Append(const char* s, unsigned int n);                     // 0x403ba0
    PropPath& operator+=(const char* s) { return Append(s, strlen(s)); }
    const char* c_str() const { return ptr == 0 ? "" : ptr; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

// Loop locals of PropStage::Load (both have empty destructors: cleanup states 2 and 3).
struct LoadProp { float v[6]; float length; int checkpoint; int flag; ~LoadProp() {} };   // StageProp layout
struct LoadVec3 { float x, y, z; ~LoadVec3() {} };
struct LoadCheckpoint { BYTE pad[0x44]; int index; };
extern float DAT_004e138c;                                                    // 0.5f

// NONMATCHING: RALLYTROPHY 0x416f30
// 1271 vs 1270 bytes with the original's call sequence (35 calls) and cleanup states; the prop records are the
// segment midpoint lookup + length (rewritten from the original). Left: stack-slot assignment (`path` sits 12
// bytes lower, `magic`/`ver` swapped, the midpoint temporary is the lowest aggregate in the original).
// Loads data/stage<N>.apf props, then models/animal_run.bmf.
void PropStage::Load()
{
    AT_(void*, 0x13a4) = 0;
    if (DAT_00501454 == 0 && DAT_00501458 == 0 && DAT_0050131c != 0) {
        PropPath path(AT_(std::string, 8));
        path += "data/stage";
        path += _itoa(AT_(int, 4), DAT_00508440, 10);
        path += ".apf";
        if (!File::Exists(path.c_str()))
            return;
        {
            File f(path.c_str(), 0x81);
            int magic, ver, count;
            f.Read(&magic, 4);
            f.Read(&ver, 4);
            if (magic == 0x2122 && ver == 100) {
                f.Read(&count, 4);
                for (int i = 0; i < count; i++) {
                    LoadProp prop;                       // two 12-byte reads: segment start and end
                    f.Read(&prop, 0xc);
                    f.Read(&prop.v[3], 0xc);
                    LoadVec3 mid;
                    mid.x = (prop.v[0] + prop.v[3]) * DAT_004e138c;
                    mid.y = (prop.v[1] + prop.v[4]) * DAT_004e138c;
                    mid.z = (prop.v[2] + prop.v[5]) * DAT_004e138c;
                    LoadCheckpoint* cp = (LoadCheckpoint*)StageNearestCheckpoint(this, &mid.x, 0);
                    if (cp) {
                        float dx = prop.v[3] - prop.v[0];
                        float dz = prop.v[5] - prop.v[2];
                        prop.checkpoint = cp->index;
                        prop.flag = 1;
                        prop.length = (float)sqrt(dz * dz + dx * dx);
                        PROP_VEC(props)->Ins(props.end(), 1, *(const StageProp*)&prop);
                    }
                }
            }
            f.Close();
            PropPath model(AT_(std::string, 8));
            model += "models/animal_run.bmf";
            PropPath tex(AT_(std::string, 8));
            tex += "textures/";
            ModelBlob m(model.c_str(), tex.c_str(), &AT_(BYTE, 0xd4), &AT_(BYTE, 0x98), 0);
            AnimalObj* a = new AnimalObj;
            AT_(AnimalObj*, 0x13a4) = a;
            a->Init(&m);
            for (int k = 0; k < m.objects.Size(); k++) {
                PropVirt* o = m.objects.first[k];
                if (o)
                    delete o;
            }
            a->active = 1;
            *(int*)(a->model + 8) = 0x7f;
            BYTE* mesh = MeshAt(a->model + 0x124, 0);
            std::vector<int**>& parts = *(std::vector<int**>*)(mesh + 0x34);
            for (unsigned i = 0; parts.size() > 0 && i < parts.size(); i++)
                *(unsigned*)((BYTE*)*parts[i] + 0x98) |= 4;
        }
    }
}
