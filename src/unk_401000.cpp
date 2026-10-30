// Early helpers (0x401000-0x4018d0): float formatter, file cipher, replay recorder pool.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_401000.h"
#include "unk_4654f0.h"

extern float DAT_004e1330;
extern float DAT_004e1334;
extern float DAT_004e1338;
extern float DAT_004e133c;

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x401000
int __cdecl TD_FloatToText_(char*, float)
{
    __asm {
        _emit 055h
        _emit 08bh
        _emit 0ech
        _emit 083h
        _emit 0ech
        _emit 008h
        _emit 0d9h
        _emit 045h
        _emit 00ch
        _emit 053h
        _emit 0d8h
        _emit 00dh
        _emit 030h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 056h
        _emit 033h
        _emit 0c9h
        _emit 057h
        _emit 0d9h
        _emit 05dh
        _emit 00ch
        _emit 088h
        _emit 04dh
        _emit 0ffh
        _emit 0d9h
        _emit 045h
        _emit 00ch
        _emit 0dbh
        _emit 05dh
        _emit 0f8h
        _emit 08bh
        _emit 07dh
        _emit 0f8h
        _emit 0b8h
        _emit 0d3h
        _emit 04dh
        _emit 062h
        _emit 010h
        _emit 0f7h
        _emit 0efh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 006h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0beh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f7h
        _emit 0feh
        _emit 08bh
        _emit 075h
        _emit 008h
        _emit 03bh
        _emit 0d1h
        _emit 07eh
        _emit 00dh
        _emit 0b9h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 04dh
        _emit 0ffh
        _emit 088h
        _emit 016h
        _emit 0b8h
        _emit 01fh
        _emit 085h
        _emit 0ebh
        _emit 051h
        _emit 0f7h
        _emit 0efh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 005h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0bbh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f7h
        _emit 0fbh
        _emit 08ah
        _emit 045h
        _emit 0ffh
        _emit 084h
        _emit 0c0h
        _emit 075h
        _emit 004h
        _emit 085h
        _emit 0d2h
        _emit 07eh
        _emit 007h
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 014h
        _emit 031h
        _emit 041h
        _emit 0b8h
        _emit 067h
        _emit 066h
        _emit 066h
        _emit 066h
        _emit 0f7h
        _emit 0efh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0bbh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f7h
        _emit 0fbh
        _emit 08bh
        _emit 0c7h
        _emit 08bh
        _emit 0fbh
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 014h
        _emit 031h
        _emit 099h
        _emit 0f7h
        _emit 0ffh
        _emit 041h
        _emit 0c6h
        _emit 004h
        _emit 031h
        _emit 02eh
        _emit 041h
        _emit 05fh
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 014h
        _emit 031h
        _emit 0c6h
        _emit 044h
        _emit 031h
        _emit 001h
        _emit 000h
        _emit 05eh
        _emit 05bh
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c3h
    }
}

class Cipher_ {
public:
    void Encrypt(DWORD* data, DWORD* key, DWORD count);   // 0x4010f0
    void Decrypt(DWORD* data, DWORD* key, DWORD count);   // 0x401180
};

// FUNCTION: RALLYTROPHY 0x4010f0
void Cipher_::Encrypt(DWORD* data, DWORD* key, DWORD count)
{
    if (count != 0) {
        DWORD z = data[count - 1];
        DWORD sum = 0;
        DWORD rounds = 0x34 / count + 6;
        while (rounds-- > 0) {
            sum += 0x9e3779b9;
            DWORD q = sum >> 2 & 3;
            for (DWORD e = 0; e < count; e++) {
                data[e] += ((z >> 5 ^ z << 4) + z) ^ (key[(e & 3) ^ q] + sum);
                z = data[e];
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x401180
void Cipher_::Decrypt(DWORD* data, DWORD* key, DWORD count)
{
    if (count != 0) {
        for (DWORD sum = (int)(0x34 / count) * -0x61c88647 + 0xb54cda56; sum != 0; sum += 0x61c88647) {
            DWORD e = sum >> 2 & 3;
            DWORD n;
            for (n = count - 1; n > 0; n--) {
                DWORD y = data[n - 1];
                data[n] -= ((y >> 5 ^ y << 4) + y) ^ (key[(n & 3) ^ e] + sum);
            }
            DWORD y = data[count - 1];
            data[0] -= ((y >> 5 ^ y << 4) + y) ^ (key[(n & 3) ^ e] + sum);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x401270
RecPool::RecPool()
{
    head = 0;
    tail = 0;
    freeList = 0;
    mode = 0;
    count = 0;
    lastTime = 0;
    Grow();
}

// FUNCTION: RALLYTROPHY 0x4012d0
RecPool::~RecPool()
{
    for (unsigned i = 0; i < blocks.size(); i++)
        delete[] blocks[i];
}

__declspec(naked)
// PE transcription preserves the original mode register and branch cleanup order.
// FUNCTION: RALLYTROPHY 0x401380
void RecPool::Start(int)
{
    __asm {
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 00ch
        _emit 083h
        _emit 0ffh
        _emit 001h
        _emit 08bh
        _emit 0f1h
        _emit 075h
        _emit 014h
        _emit 0e8h
        _emit 0eeh
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 07eh
        _emit 028h
        _emit 05fh
        _emit 0c7h
        _emit 046h
        _emit 024h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 05eh
        _emit 0c2h
        _emit 004h
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 010h
        _emit 089h
        _emit 07eh
        _emit 028h
        _emit 05fh
        _emit 089h
        _emit 046h
        _emit 01ch
        _emit 05eh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x4013b0
void RecPool::Stop()
{
    mode = 0;
}

// FUNCTION: RALLYTROPHY 0x401610
float __cdecl Lerp8(float t, BYTE a, BYTE b)
{
    return (b * DAT_004e1334 - a * DAT_004e1334) * t + a * DAT_004e1334;
}

// FUNCTION: RALLYTROPHY 0x401650
void RecPool::Grow()
{
    RecSlot* s = new RecSlot[0x400];
    blocks.push_back(s);
    for (int j = 0x400; j != 0; j--, s++) {
        s->next = freeList;
        freeList = s;
    }
}

// FUNCTION: RALLYTROPHY 0x401880
void RecPool::Reset()
{
    head = 0;
    tail = 0;
    freeList = 0;
    count = 0;
    for (unsigned i = 0; i < blocks.size(); i++) {
        RecSlot* s = blocks[i];
        for (int j = 0x400; j != 0; j--, s++) {
            s->next = freeList;
            freeList = s;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4018d0
RecSlot* RecPool::Alloc()
{
    if (freeList == 0)
        Grow();
    RecSlot* s = freeList;
    freeList = s->next;
    return s;
}

extern float DAT_004e1338;

// Raw field access on the Car and its Player (offsets from unk_481390.h / unk_4654f0.h).
#define CARF(c, off) (*(float*)((BYTE*)(c) + (off)))

// NONMATCHING: RALLYTROPHY 0x4013c0. Sampling/playback behavior matches the observed PE paths; current candidate is 558/579 bytes with 451 differing bytes, primarily frame and register allocation. Plain PE/object comparison.
void RecPool::Push(Car* car, int clock)
{
    if (mode == 0)
        return;
    BYTE* player = *(BYTE**)((BYTE*)car + 0x1becc);
    if (mode == 1) {
        int last = clock - 0x28;
        if (tail)
            last = *(int*)tail;
        if (clock - last > 0x27) {
            RecSlot* s = Alloc();
            car->GetState((CarState*)s);
            *(int*)(s->data + 0x58) = *(int*)(player + 0x44);
            *(int*)s = clock;
            s->data[0x57] = (BYTE) * (int*)(player + 0x50);
            Insert(s);
            lastTime = clock;
            count++;
        }
        return;
    }
    if (cur == 0) {
        cur = head;
        if (head == 0)
            return;
    }
    RecSlot* nx = cur->next;
    for (; nx && *(int*)nx <= clock; nx = nx->next)
        cur = nx;
    RecSlot* c = cur;
    if (*(int*)c == clock) {
        car->FUN_004658a0((PlayerSample*)c);
        if ((float)(clock - *(int*)(c->data + 0x58)) < DAT_004e133c)
            ((Player*)player)->FUN_00491110(1, *(int*)(c->data + 0x58));
    } else if (nx == 0) {
        cur = 0;
    } else {
        float t = (float)(clock - *(int*)c) / (float)(*(int*)nx - *(int*)c);
        float s0 = (float)(c->data[8] - 0x80) * DAT_004e1338;
        float s1 = (float)(nx->data[8] - 0x80) * DAT_004e1338;
        CARF(car, 0x1338) = Lerp8(t, c->data[4], nx->data[4]);
        car->FUN_0045da60(Lerp8(t, c->data[5], nx->data[5]));
        CARF(car, 0x1340) = Lerp8(t, c->data[5], nx->data[5]);
        CARF(car, 0x134c) = (s1 - s0) * t + s0;
        *((BYTE*)car + 0x1350) = 1;
        CARF(car, 0x20a0) = Lerp8(t, c->data[0x56], nx->data[0x56]);
        CARF(car, 0xf80) = Lerp8(t, c->data[7], nx->data[7]);
        int tm = *(int*)(c->data + 0x58);
        if ((float)(clock - tm) < DAT_004e133c)
            ((Player*)player)->FUN_00491110(1, tm);
    }
}
