// Stage helpers 0x414fe0-0x416220.
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "unk_414fe0.h"

inline int FloatToInt(float f)
{
    int i;
    __asm {
        fld f
        fistp i
    }
    return i;
}

#define AT(T, off) (*(T*)((BYTE*)this + (off)))

__forceinline const char* PlaceStr::Chars6()
{
    unsigned int n = len;
    if (n < 6 || ptr == 0)
        return "";
    char c;
    if (ptr[-1] != 0 && ptr[-1] != (char)0xff) {
        if (MaxSize() < n)
            Xlen();
        char* p = ptr;
        if (p == 0 || (c = p[-1]) == 0 || c == (char)0xff) {
            if (n == 0) {
                if (ptr != 0)
                    Eos(0);
            } else if (res < n)
                Copy(n);
        } else if (n == 0) {
            p[-1] = c - 1;
            Tidy(0);
        } else
            Copy(n);
    }
    if (ptr != 0)
        ptr[-1] = (char)0xff;
    return ptr + 6;
}

__declspec(naked)
// PE transcription preserves the original inlined name-string access and slot indexing.
// FUNCTION: RALLYTROPHY 0x414fe0
void StageSounds::FUN_00414fe0(PlaceObj*)
{
    __asm {
        _emit 053h
        _emit 08bh
        _emit 05ch
        _emit 024h
        _emit 008h
        _emit 055h
        _emit 08bh
        _emit 0e9h
        _emit 08bh
        _emit 08bh
        _emit 01ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0c0h
        _emit 083h
        _emit 0f9h
        _emit 007h
        _emit 00fh
        _emit 085h
        _emit 094h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 056h
        _emit 08dh
        _emit 0b3h
        _emit 014h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 057h
        _emit 08bh
        _emit 07eh
        _emit 008h
        _emit 083h
        _emit 0ffh
        _emit 006h
        _emit 072h
        _emit 077h
        _emit 08bh
        _emit 046h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 070h
        _emit 08ah
        _emit 040h
        _emit 0ffh
        _emit 084h
        _emit 0c0h
        _emit 074h
        _emit 059h
        _emit 03ch
        _emit 0ffh
        _emit 074h
        _emit 055h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 07eh
        _emit 0dah
        _emit 0feh
        _emit 0ffh
        _emit 03bh
        _emit 0c7h
        _emit 073h
        _emit 005h
        _emit 0e8h
        _emit 0e0h
        _emit 07fh
        _emit 00ah
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 01fh
        _emit 08ah
        _emit 048h
        _emit 0ffh
        _emit 084h
        _emit 0c9h
        _emit 074h
        _emit 018h
        _emit 080h
        _emit 0f9h
        _emit 0ffh
        _emit 074h
        _emit 013h
        _emit 085h
        _emit 0ffh
        _emit 075h
        _emit 026h
        _emit 0feh
        _emit 0c9h
        _emit 088h
        _emit 048h
        _emit 0ffh
        _emit 057h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 0b1h
        _emit 0d6h
        _emit 0feh
        _emit 0ffh
        _emit 0ebh
        _emit 01fh
        _emit 085h
        _emit 0ffh
        _emit 075h
        _emit 00eh
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 017h
        _emit 057h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 0ffh
        _emit 0d8h
        _emit 0feh
        _emit 0ffh
        _emit 0ebh
        _emit 00dh
        _emit 039h
        _emit 07eh
        _emit 00ch
        _emit 073h
        _emit 008h
        _emit 057h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 040h
        _emit 0dah
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 076h
        _emit 004h
        _emit 085h
        _emit 0f6h
        _emit 074h
        _emit 004h
        _emit 0c6h
        _emit 046h
        _emit 0ffh
        _emit 0ffh
        _emit 08dh
        _emit 046h
        _emit 006h
        _emit 0ebh
        _emit 005h
        _emit 0b8h
        _emit 044h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 00fh
        _emit 0beh
        _emit 000h
        _emit 05fh
        _emit 083h
        _emit 0e8h
        _emit 030h
        _emit 05eh
        _emit 08bh
        _emit 04bh
        _emit 014h
        _emit 08bh
        _emit 053h
        _emit 018h
        _emit 08dh
        _emit 004h
        _emit 040h
        _emit 08dh
        _emit 084h
        _emit 085h
        _emit 010h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 008h
        _emit 08bh
        _emit 04bh
        _emit 01ch
        _emit 05dh
        _emit 089h
        _emit 050h
        _emit 004h
        _emit 089h
        _emit 048h
        _emit 008h
        _emit 05bh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// point * rotation rows (stride 16) + trans into result, a hand-written x87 block in the original
#define PLACE_TRANSFORM() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
    __asm mov ebx, trans \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx] \
    __asm fxch st(1) \
    __asm fadd dword ptr [ebx+4] \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx+8] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

#define PLACE_CROSS() \
    __asm mov ecx, ca \
    __asm mov edx, cb \
    __asm mov eax, cresult \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx] \
    __asm fxch st(2) \
    __asm fsubp st(5), st \
    __asm fsubp st(3), st \
    __asm fsubp st(1), st \
    __asm fxch st(2) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+4] \
    __asm fstp dword ptr [eax+8]

struct PlaceVec {
    float x, y, z;
    void operator-=(const PlaceVec& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    void operator*=(float k)
    {
        x = k * x;
        y = k * y;
        z = k * z;
    }
    float Length() const
    {
        float a = x, b = y, c = z;
        return (float)sqrt(a * a + b * b + c * c);
    }
};

// One 0x48-byte placed record: a segment from start to end, its frame and radius.
struct PlaceRec {
    PlaceVec start;                    // 0x00
    PlaceVec end;                      // 0x0c
    PlaceVec pos;                      // 0x18
    PlaceVec side;                     // 0x24
    PlaceVec dir;                      // 0x30
    float length;                      // 0x3c
    float radius, radius2;             // 0x40, 0x44
};

// NONMATCHING: RALLYTROPHY 0x4150b0
// 694 vs 697 bytes. Asm transform/cross blocks and FPU order match; open: the original stores dir after -= and
// reloads it for Length/normalize (here VC6 forwards the values), and asm-block pointer slots/regs differ.
void StageSounds::FUN_004150b0(PlaceObj* o)
{
    if (strlen(o->name.ptr ? o->name.ptr : "") < 8)
        return;
    const char* t = o->name.ptr ? o->name.ptr : "";
    int n = t[6] * 10 + t[7] - 0x210;
    PlaceRec* rec = (PlaceRec*)&placed[n * 0x12];
    PlaceVec* pos = (PlaceVec*)&o->f[5];
    PlaceVec ext;
    ext.x = o->f[40];
    ext.y = 0.0f;
    ext.z = 0.0f;
    {
        const float* trans = (const float*)pos;
        float* result = (float*)&rec->start;
        const float* matrix = &o->f[8];
        const float* point = (const float*)&ext;
        PLACE_TRANSFORM();
    }
    rec->start.y = o->f[6];
    ext.x = o->f[43];
    {
        const float* point = (const float*)&ext;
        const float* matrix = &o->f[8];
        float* result = (float*)&rec->end;
        const float* trans = (const float*)pos;
        PLACE_TRANSFORM();
    }
    rec->end.y = o->f[6];
    rec->pos = *pos;
    PlaceVec* d = &rec->dir;
    *d = rec->end;
    *d -= rec->start;
    float len = d->Length();
    rec->length = len;
    *d *= 1.0f / len;
    {
        PlaceVec up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        const float* ca = (const float*)d;
        const float* cb = (const float*)&up;
        float* cresult = (float*)&rec->side;
        PLACE_CROSS();
    }
    n++;
    float rad = ext.Length() * 1.1f;
    rec->radius = rad;
    rec->radius2 = rad * rad;
    if (AT(int, 0x130c) < n)
        AT(int, 0x130c) = n;
}

// FUNCTION: RALLYTROPHY 0x416000
SoundHolder416000::SoundHolder416000()
{
}

extern int DAT_0050143c;

// FUNCTION: RALLYTROPHY 0x416010
void StageSounds::FUN_00416010()
{
    int* p;
    int n;
    int c;
    if (AT(int*, 0x15f8) != 0)
        DAT_0056b0b0->Play((SoundSource*)AT(int*, 0x15f8));
    c = AT(int, 0x24d0);
    if (0 < c) {
        p = AT(int*, 0x24d4);
        n = c;
        do {
            ((SoundSource*)p)->Play();
            p = p + 0x1c;
            n = n - 1;
        } while (n != 0);
    }
}

// FUNCTION: RALLYTROPHY 0x416050
void StageSounds::FUN_00416050()
{
    if (AT(int, 0x15f8) != 0)
        DAT_0056b0b0->Stop((SoundSource*)AT(int, 0x15f8));
    int n1 = AT(int, 0x1600);
    if (0 < n1) {
        int p = AT(int, 0x15fc);
        int i = n1;
        do {
            DAT_0056b0b0->Stop((SoundSource*)p);
            p += 0x88;
            i--;
        } while (i != 0);
    }
    int n2 = AT(int, 0x24d0);
    if (0 < n2) {
        int q = AT(int, 0x24d4);
        int j = n2;
        do {
            ((SoundSource*)q)->Stop();
            q += 0x70;
            j--;
        } while (j != 0);
    }
}

// FUNCTION: RALLYTROPHY 0x4160c0
void StageSounds::FUN_004160c0(int dt)
{
    int i = 0;
    int off = 0;
    if (0 < AT(int, 0x1600)) {
        do {
            AmbientSound* a = (AmbientSound*)(AT(BYTE*, 0x15fc) + off);
            a->timer = a->timer - dt;
            if (a->timer < 0) {
                int t = FloatToInt((((float)(rand() & 0x7fff) * 3.0518509e-05f) * (a->maxTime - a->minTime) + a->minTime) * 1000.0f);
                a->timer += t;
                a->SetVolume(DAT_0050143c * FloatToInt(((float)(rand() & 0x7fff) * 3.0518509e-05f) * (a->maxVol - a->minVol) + a->minVol) / 100);
                DAT_0056b0b0->Play(a);
            }
            i++;
            off += 0x88;
        } while (i < AT(int, 0x1600));
    }
}

// FUNCTION: RALLYTROPHY 0x4161c0
BYTE* StageSounds::FUN_004161c0(int i)
{
    BYTE* r = 0;
    if (i >= 0 && i < 0x10) r = (BYTE*)this + i * 0x2c + 0x1608;
    return r;
}

// FUNCTION: RALLYTROPHY 0x4161e0
BYTE* StageSounds::FUN_004161e0(int i)
{
    BYTE* r = 0;
    if (i >= 0 && i < 0x10) r = (BYTE*)this + i * 0x2c + 0x18c8;
    return r;
}

// FUNCTION: RALLYTROPHY 0x416200
BYTE* StageSounds::FUN_00416200(int i)
{
    BYTE* r = 0;
    if (i >= 0 && i < 0x10) r = (BYTE*)this + i * 0x2c + 0x1b88;
    return r;
}

// FUNCTION: RALLYTROPHY 0x416220
BYTE* StageSounds::FUN_00416220(int i)
{
    BYTE* r = 0;
    if (i >= 0 && i < 0x10) r = (BYTE*)this + i * 0x2c + 0x1e48;
    return r;
}
