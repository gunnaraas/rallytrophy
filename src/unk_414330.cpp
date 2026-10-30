#include "unk_414330.h"

struct Elem48 { Elem48(); ~Elem48(); char pad[0x48]; }; // dtor 0x402c90
struct Elem24 { Elem24(); ~Elem24(); char pad[0x24]; }; // dtor 0x402c90 (same shared helper)

struct Vec48 {
    int pad0;
    Elem48* first;
    Elem48* last;
    Elem48* end;
    void Clear(); // 0x4143e0
};
struct Vec24 {
    int pad0;
    Elem24* first;
    Elem24* last;
    Elem24* end;
    void Clear(); // 0x414420
};
struct DwordVec {
    int pad0;
    int* first;
    int* last;
    int Size(); // 0x414460
};
struct Holder6b0 {
    char pad[0x6b0];
    int* obj;
    int Func414360(); // 0x414360
};

__declspec(naked)
// PE transcription preserves the original stack-local zero store and x87 order.
// FUNCTION: RALLYTROPHY 0x414330
void S414Vec3::Sub(float*, const float*) const
{
    __asm {
        _emit 051h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 00ch
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 022h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 008h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 018h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 062h
        _emit 004h
        _emit 0d9h
        _emit 058h
        _emit 004h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 062h
        _emit 008h
        _emit 0d9h
        _emit 058h
        _emit 008h
        _emit 059h
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x414360
int Holder6b0::Func414360()
{
    int r = 0;
    int* o = obj;
    if (o) {
        r = o[1];
    }
    return r;
}

// FUNCTION: RALLYTROPHY 0x414370
int S414Sphere::Contains(const float* p, float extra) const
{
    float rr = extra + r;
    struct V { int x; float y, z; } c;
    *(int*)&c.y = *(int*)&y;
    float rr2 = rr * rr;
    float dx = x - p[0];
    *(int*)&c.z = *(int*)&z;
    float dy = c.y - p[1];
    float dz = c.z - p[2];
    float d2 = dx * dx + dy * dy + dz * dz;
    // squared distance and radius compared as raw float bits (valid for non-negative floats)
    return *(int*)&d2 <= *(int*)&rr2;
}

// FUNCTION: RALLYTROPHY 0x4143e0
void Vec48::Clear()
{
    for (Elem48* p = first; p != last; ++p) {
        p->~Elem48();
    }
    operator delete(first);
    first = 0;
    last = 0;
    end = 0;
}

// FUNCTION: RALLYTROPHY 0x414420
void Vec24::Clear()
{
    for (Elem24* p = first; p != last; ++p) {
        p->~Elem24();
    }
    operator delete(first);
    first = 0;
    last = 0;
    end = 0;
}

// FUNCTION: RALLYTROPHY 0x414460
int DwordVec::Size()
{
    if (first == 0) {
        return 0;
    }
    return last - first;
}

// FUNCTION: RALLYTROPHY 0x414480
extern int DAT_004e1344;
int* __cdecl Func414480()
{
    return &DAT_004e1344;
}

struct S414Data { ~S414Data(); }; // dtor original: FUN_004a86d0

// FUNCTION: RALLYTROPHY 0x4144e0
void S414List::RemoveNode(S414Node* n)
{
    delete (S414Data*)n->data;
    if (n == head) {
        head = n->next;
    } else {
        n->prev->next = n->next;
    }
    if (n == tail) {
        tail = n->prev;
    } else {
        n->next->prev = n->prev;
    }
    int c = count;
    n->next = freeList;
    int f = freeCount;
    freeList = n;
    n->prev = 0;
    count = c + 1;
    freeCount = f - 1;
}

#pragma comment(linker, "/alternatename:?Grow@StMsg@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Tidy@StMsg@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?AssignCall@StMsg@@QAEAAV1@PBDI@Z=?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z")
#pragma comment(linker, "/alternatename:?MaxSize@StMsg@@QBEIXZ=?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ")
#pragma comment(linker, "/alternatename:?Copy@StMsg@@QAEXI@Z=?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")

// NONMATCHING: RALLYTROPHY 0x414550
// The Stage caller uses this as a thiscall member on its string-layout message object.
StMsg& StMsg::Format(char* first, char* last, const char* fmt, const char* arg)
{
    if (ptr != 0 && ((unsigned char*)ptr)[-1] != 0 &&
        ((unsigned char*)ptr)[-1] != 0xff) {
        Grow(len, false);
    }
    char* oldPtr = ptr;
    if (oldPtr != 0)
        oldPtr[-1] = (char)0xff;

    // Keep the iterator offset in its stack slot, as in the target body.
    volatile unsigned int pos;
    if (first == 0)
        pos = (unsigned int)first;
    else
        pos = (unsigned int)(first - ptr);
    unsigned int sourceLen = (unsigned int)(arg - fmt);
    unsigned int eraseLen = last == 0 ? 0 : (unsigned int)(last - first);
    if (len < pos)
        std::_Xran();
    if (len - pos < eraseLen)
        eraseLen = len - pos;
    if (std::string::npos - sourceLen <= len - eraseLen)
        std::_Xlen();

    if (oldPtr != 0 && ((unsigned char*)oldPtr)[-1] != 0 &&
        ((unsigned char*)oldPtr)[-1] != 0xff) {
        Tidy(true);
        AssignCall(oldPtr, (unsigned int)strlen(oldPtr));
    }

    unsigned int baseLen = len - eraseLen;
    unsigned int suffixLen = baseLen - pos;
    if (sourceLen < eraseLen) {
        memmove(ptr + pos + sourceLen, ptr + pos + eraseLen,
                suffixLen);
    }
    unsigned int newLen = baseLen + sourceLen;
    if (sourceLen > eraseLen || eraseLen != 0) {
        if (Grow(newLen, false)) {
            if (sourceLen < eraseLen) {
                memset(ptr + pos + sourceLen + len - pos - eraseLen, 0,
                       eraseLen - sourceLen);
            } else if (sourceLen > eraseLen) {
                memmove(ptr + pos + sourceLen, ptr + pos + eraseLen,
                        len - pos - eraseLen);
            }
            len = newLen;
            ptr[newLen] = 0;
        }
    }
    if (ptr != 0 && ptr[-1] != 0 && ptr[-1] != (char)0xff) {
        unsigned int max = MaxSize();
        if (max < newLen)
            std::_Xlen();
        if (newLen == 0) {
            ptr[-1]--;
            Tidy(false);
        } else if (res < newLen) {
            Copy(newLen);
        }
    }
    if (ptr != 0)
        ptr[-1] = (char)0xff;
    if (sourceLen != 0)
        memcpy(ptr + pos, fmt, sourceLen);
    return *this;
}
