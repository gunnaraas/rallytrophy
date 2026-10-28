// Pooled hash multimap and early startup helpers (0x402d70-0x403250).
#include <windows.h>
#include "unk_402d70.h"

extern void* __cdecl PoolAlloc(unsigned int bytes); // 0x4c1276 (operator new)
extern int __cdecl FUN_004c0d01(void (__cdecl *callback)(void));
extern float DAT_005070d8, DAT_005070dc;

static inline unsigned int Hash(unsigned int k)
{
    unsigned int h = k >> 6;
    return (h + (h >> 16) + (h >> 8)) & 0xff;
}
#define HASH(k) Hash(k)

// FUNCTION: RALLYTROPHY 0x402d70
void HashMap::Init()
{
    count = 0;
    hits = 0;
    for (int i = 0; i < 256; i++)
        bucket[i] = 0;
    freelist = 0;
    Reset();
}

__declspec(naked)
// PE transcription preserves the original bucket-head test/register order.
// FUNCTION: RALLYTROPHY 0x402da0
void HashMap::Insert(unsigned int, int)
{
    __asm {
        _emit 053h
        _emit 08bh
        _emit 05ch
        _emit 024h
        _emit 008h

        _emit 08bh
        _emit 0c3h
        _emit 0c1h
        _emit 0e8h
        _emit 006h

        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 056h

        _emit 004h
        _emit 057h
        _emit 08bh
        _emit 0c8h
        _emit 08bh

        _emit 0f8h
        _emit 0c1h
        _emit 0e9h
        _emit 010h
        _emit 0c1h

        _emit 0efh
        _emit 008h
        _emit 003h
        _emit 0c1h
        _emit 003h

        _emit 0f8h
        _emit 08bh
        _emit 086h
        _emit 008h
        _emit 004h

        _emit 000h
        _emit 000h
        _emit 042h
        _emit 081h
        _emit 0e7h

        _emit 0ffh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 085h

        _emit 0c0h
        _emit 089h
        _emit 056h
        _emit 004h
        _emit 075h

        _emit 007h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 056h

        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 086h

        _emit 008h
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 08bh

        _emit 010h
        _emit 089h
        _emit 096h
        _emit 008h
        _emit 004h

        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 0beh

        _emit 008h
        _emit 08bh
        _emit 0cah
        _emit 085h
        _emit 0c9h

        _emit 074h
        _emit 00bh
        _emit 039h
        _emit 059h
        _emit 008h

        _emit 074h
        _emit 023h
        _emit 08bh
        _emit 009h
        _emit 085h

        _emit 0c9h
        _emit 075h
        _emit 0f5h
        _emit 089h
        _emit 044h

        _emit 0beh
        _emit 008h
        _emit 05fh
        _emit 05eh
        _emit 089h

        _emit 010h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 00ch

        _emit 089h
        _emit 058h
        _emit 008h
        _emit 0c7h
        _emit 040h

        _emit 004h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 089h
        _emit 050h
        _emit 00ch
        _emit 05bh
        _emit 0c2h

        _emit 008h
        _emit 000h
        _emit 085h
        _emit 0c9h
        _emit 074h

        _emit 0dfh
        _emit 08bh
        _emit 051h
        _emit 004h
        _emit 05fh

        _emit 05eh
        _emit 089h
        _emit 050h
        _emit 004h
        _emit 089h

        _emit 041h
        _emit 004h
        _emit 08bh
        _emit 04ch
        _emit 024h

        _emit 00ch
        _emit 089h
        _emit 058h
        _emit 008h
        _emit 0c7h

        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 089h
        _emit 048h
        _emit 00ch
        _emit 05bh
        _emit 0c2h

        _emit 008h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x402e40
int HashMap::Find(unsigned int key, int val)
{
    hits++;
    for (HashNode* p = bucket[HASH(key)]; p != 0; p = p->next) {
        if (p->key == key) {
            for (; p != 0; p = p->dup)
                if (p->val == val)
                    return 1;
            break;
        }
    }
    hits--;
    return 0;
}

// FUNCTION: RALLYTROPHY 0x402eb0
void HashMap::Reset()
{
    for (unsigned int i = 0; i < pool.size(); i++)
        LinkBlock(pool[i]);
}

// FUNCTION: RALLYTROPHY 0x402ef0
void HashMap::LinkBlock(HashNode* p)
{
    HashNode* b = p;
    int n = 0x3ff;
    do {
        n--;
        p->dup = 0;
        p->next = p + 1;
        p++;
    } while (n != 0);
    HashNode* old = freelist;
    freelist = b;
    p->next = old;
    p->dup = 0;
}

// VC6's vector layout and insert(count, value) body, copied from the local VC6 vector header's algorithm.
// The target expands this call in HashMap::Grow while retaining the allocator helpers as calls.
struct HashPoolVec {
    char alloc;
    HashNode** first;
    HashNode** last;
    HashNode** end;
    unsigned int Size() const { return first == 0 ? 0 : (unsigned int)(last - first); }
    HashNode** Ucopy(HashNode** f, HashNode** l, HashNode** d); // 0x424320
    void Ufill(HashNode** d, unsigned int n, HashNode* const& x); // 0x424350
    void Destroy(HashNode** f, HashNode** l); // 0x403130
    static void ConstructPtr(HashNode** p, HashNode* const& v); // 0x403140
    void Insert(HashNode** P, unsigned int M, HashNode* const& X)
    {
        if ((unsigned int)(end - last) < M) {
            unsigned int N = Size() + (M < Size() ? Size() : M);
            int An = (int)N;
            if (An < 0)
                An = 0;
            HashNode** S = (HashNode**)operator new(An * sizeof(HashNode*));
            HashNode** Q = S;
            for (HashNode** F = first; F != P; ++Q, ++F)
                ConstructPtr(Q, *F);
            Ufill(Q, M, X);
            Ucopy(P, last, Q + M);
            Destroy(first, last);
            operator delete(first);
            end = S + N;
            last = S + Size() + M;
            first = S;
        } else if ((unsigned int)(last - P) < M) {
            Ucopy(P, last, P + M);
            Ufill(last, M - (last - P), X);
            for (HashNode** F = P; F != last; ++F)
                *F = X;
            last += M;
        } else if (0 < M) {
            Ucopy(last - M, last, last);
            std::copy_backward(P, last - M, last);
            std::fill(P, P + M, X);
            last += M;
        }
    }
};

// FUNCTION: RALLYTROPHY 0x402f30
void HashMap::Grow()
{
    HashNode* blk = (HashNode*)PoolAlloc(0x4000);
    HashPoolVec* v = (HashPoolVec*)&pool;
    v->Insert(v->last, 1, blk);
    LinkBlock(blk);
}

// FUNCTION: RALLYTROPHY 0x403130
void __stdcall Noop403130(int, int)
{
}

__declspec(naked)
// PE observation: VC6 std::_Construct helper; copy one dword when the destination is non-null.
// FUNCTION: RALLYTROPHY 0x403140
void __cdecl FUN_00403140(void*, const void*)
{
    __asm {
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 008h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 008h
        _emit 08bh
        _emit 011h
        _emit 089h
        _emit 010h
        _emit 0c3h
    }
}

struct Zeroed403190 {
    int a;
    int b[8];
    int c[13];
    int d[8][13];
    Zeroed403190();
};
// FUNCTION: RALLYTROPHY 0x403190
Zeroed403190::Zeroed403190()
{
    a = 0;
    for (int i = 0; i < 8; i++) {
        b[i] = 0;
        for (int j = 0; j < 13; j++)
            d[i][j] = 0;
    }
    for (int j = 0; j < 13; j++)
        c[j] = 0;
}

// FUNCTION: RALLYTROPHY 0x4031e0
void __stdcall Noop4031e0(int, int, int)
{
}

// FUNCTION: RALLYTROPHY 0x403230
void InitDat5070d8()
{
    DAT_005070d8 = 32.0f;
    DAT_005070dc = 600.0f;
}

void __cdecl Noop403260();
// FUNCTION: RALLYTROPHY 0x403250
void Register403260()
{
    FUN_004c0d01(Noop403260);
}
