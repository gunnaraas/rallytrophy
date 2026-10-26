// Stage::~Stage (0x40ebc0): releases all owned sub-objects. Offsets per include/unk_481390.h / pseudocode.
#include <string>
#include "unk_481390.h"
#include "rallytrophy.h"
#include "unk_40ebc0.h"
#include "unk_40c280.h"

#define AT(T, off) (*(T*)((BYTE*)this + (off)))
#define COUNT(off) ((unsigned int)((AT(int, (off) + 4) - AT(int, (off))) >> 2))

// Deletes every non-null element of a pointer vector at `off` (VC6 loop shape: vector may be null/empty).
#define DELETE_ALL(Type, off)                                                      \
    { for (unsigned int i = 0; AT(int, off) != 0 && i < COUNT(off); i++) {           \
        Type* e = ((Type**)AT(void*, off))[i];                                     \
        if (e) delete e;                                                           \
    } }

// std::vector<int> at `off` (alloc, first, last, end): destroy elements, free, zero the three pointers.
#define DESTROY_VEC(off)                                                           \
    {                                                                              \
        StageIntVec* v = (StageIntVec*)((BYTE*)this + (off));                      \
        v->Destroy(v->first, v->last);                                             \
        StageFree(v->first);                                                       \
        v->first = 0; v->last = 0; v->end = 0;                                     \
    }

static void FreeList(SPList* l)
{
    SPNode* n;
    while ((n = l->head) != 0)
        l->RemoveNodeB(n);
    n = l->freeList;
    while (n) {
        SPNode* next = n->next;
        delete n;
        n = next;
    }
}

// NONMATCHING: RALLYTROPHY 0x40ebc0
// Embedded-array/unwind details and register allocation of the original are not reproduced; the in-place array offsets other than 0x1e48/0x1608 are guesses.
Stage::~Stage()
{
    if (AT(SPlane*, 0x19c))
        delete[] AT(SPlane*, 0x19c);
    DELETE_ALL(SVirtual, 0x2c)
    DELETE_ALL(SVirtual, 0x3c)
    DELETE_ALL(SVirtual, 0x138)
    DELETE_ALL(SVirtual, 0x148)
    DELETE_ALL(SBodyAttach, 0x158)
    DELETE_ALL(SOwner40A7B0, 0x8c)
    if (AT(SOwnedSet*, 0x13a4))
        delete AT(SOwnedSet*, 0x13a4);
    if (AT(SVirtual*, 0x18))
        delete AT(SVirtual*, 0x18);
    if (AT(SSky*, 0x6ac))
        delete AT(SSky*, 0x6ac);
    if (AT(SVirtual*, 0x6b0))
        delete AT(SVirtual*, 0x6b0);
    SPNode* n;
    while ((n = AT(SPNode*, 0x98)) != 0)
        ((SPList*)((BYTE*)this + 0x98))->RemoveNodeA(n);
    while ((n = AT(SPNode*, 0xac)) != 0)
        ((SPList*)((BYTE*)this + 0xac))->RemoveNodeA(n);
    while ((n = AT(SPNode*, 0xc0)) != 0)
        ((SPList*)((BYTE*)this + 0xc0))->RemoveNodeA(n);
    if (AT(void*, 0x9e0)) {
        SBase4A86D0(AT(void*, 0x9e0));
        delete (char*)AT(void*, 0x9e0);
    }
    if (AT(void*, 0x9dc)) {
        SBase4A86D0(AT(void*, 0x9dc));
        delete (char*)AT(void*, 0x9dc);
    }
    if (AT(SPlane*, 0xa08))
        delete[] AT(SPlane*, 0xa08);
    if (AT(SAiPath*, 0x1368))
        delete AT(SAiPath*, 0x1368);
    SSoundSrc* snd = AT(SSoundSrc*, 0x15f8);
    if (snd) {
        DAT_0056b0b0->ReleaseSample(*(Sample**)snd);
        delete snd;
    }
    int cnt = AT(int, 0x1600);
    if (0 < cnt) {
        int* p = (int*)(AT(int, 0x15fc) + 0x70);
        do {
            DAT_0056b0b0->ReleaseSample(*(Sample**)p);
            p += 0x22;
        } while (--cnt);
    }
    if (AT(SPlane*, 0x15fc))
        delete[] AT(SPlane*, 0x15fc);
    if (AT(SPlane*, 0x24c8))
        delete[] AT(SPlane*, 0x24c8);
    if (AT(SPlane*, 0x24d4))
        delete[] AT(SPlane*, 0x24d4);
#define DTOR_AT(T, off) ((T*)((BYTE*)this + (off)))->~T()
    DTOR_AT(SArr23C8, 0x23c8);
    DTOR_AT(SArr23C8, 0x2108);
    DTOR_AT(SArr23C8, 0x1e48);
    DTOR_AT(SArr23C8, 0x1b88);
    DTOR_AT(SArr23C8, 0x18c8);
    DTOR_AT(SArr23C8, 0x1608);
    SHolderDtor((BYTE*)this + 0x13b8);
    { for (BYTE* e = AT(BYTE*, 0x13ac); e != AT(BYTE*, 0x13b0); e += 0x24)
        SElemDtor(e, 0); }
    delete (char*)AT(void*, 0x13ac);
    AT(int, 0x13ac) = 0;
    AT(int, 0x13b0) = 0;
    AT(int, 0x13b4) = 0;
    { for (BYTE* e = AT(BYTE*, 0x9f4); e != AT(BYTE*, 0x9f8); e += 0x48)
        SElemDtor(e, 0); }
    delete (char*)AT(void*, 0x9f4);
    AT(int, 0x9f4) = 0;
    AT(int, 0x9f8) = 0;
    AT(int, 0x9fc) = 0;
    DTOR_AT(SArr1310, 0x1310);
    DTOR_AT(SArrA0C, 0xa0c);
    DTOR_AT(SArr908, 0x908);
    SBase4A86D0((BYTE*)this + 0x788);
    SBase4A86D0((BYTE*)this + 0x6b4);
    DTOR_AT(SArr65C, 0x65c);
    DTOR_AT(SArr38C, 0x38c);
    SStrTidy((BYTE*)this + 0x21c, true);
    delete (char*)AT(void*, 0x158);
    AT(int, 0x158) = 0;
    AT(int, 0x15c) = 0;
    AT(int, 0x160) = 0;
    delete (char*)AT(void*, 0x148);
    AT(int, 0x148) = 0;
    AT(int, 0x14c) = 0;
    AT(int, 0x150) = 0;
    delete (char*)AT(void*, 0x138);
    AT(int, 0x138) = 0;
    AT(int, 0x13c) = 0;
    AT(int, 0x140) = 0;
    SCarModelCache((BYTE*)this + 0xf4);
    SCarTexCache((BYTE*)this + 0xd4);
    FreeList((SPList*)((BYTE*)this + 0xc0));
    AT(int, 0xc8) = 0;
    FreeList((SPList*)((BYTE*)this + 0xac));
    AT(int, 0xb4) = 0;
    {
        SPNode* h;
        while ((h = AT(SPNode*, 0x98)) != 0)
            ((SPList*)((BYTE*)this + 0x98))->RemoveNodeB(h);
        SPNode* f = AT(SPNode*, 0xa0);
        while (f) {
            SPNode* next = f->next;
            delete (SNode49ADA0*)f;
            f = next;
        }
        AT(int, 0xa0) = 0;
    }
    DESTROY_VEC(0x88);
    DESTROY_VEC(0x78);
    DESTROY_VEC(0x68);
    DESTROY_VEC(0x58);
    SHolderDtor((BYTE*)this + 0x48);
    SHolderDtor((BYTE*)this + 0x38);
    SHolderDtor((BYTE*)this + 0x28);
    SStrTidy((BYTE*)this + 8, true);
}
