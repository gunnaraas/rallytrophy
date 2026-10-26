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

// NONMATCHING: RALLYTROPHY 0x4150b0
// reason: original keeps all matrix floats in locals with fuller expression order; built 339B vs 697B
void StageSounds::FUN_004150b0(PlaceObj* o)
{
    const char* s = (o->name.ptr ? o->name.ptr : "");
    if (strlen(s) > 7) {
        const char* t = (o->name.ptr ? o->name.ptr : "");
        int n = t[7] - 0x210 + t[6] * 10;
        float* e = o->f;
        float k = e[40];
        float* p = (float*)((BYTE*)this + 0xa0c + n * 0x48);
        p[0] = e[16] * 0.0f + e[12] * 0.0f + k * e[8] + e[5];
        p[2] = e[18] * 0.0f + e[14] * 0.0f + k * e[10] + e[7];
        p[1] = e[17] * 0.0f + e[13] * 0.0f + k * e[9] + e[6];
        k = e[43];
        p[1] = e[6];
        p[3] = e[16] * 0.0f + e[12] * 0.0f + k * e[8] + e[5];
        p[5] = e[18] * 0.0f + e[14] * 0.0f + k * e[10] + e[7];
        p[4] = e[17] * 0.0f + e[13] * 0.0f + k * e[9] + e[6];
        p[6] = e[5];
        p[4] = e[6];
        p[7] = e[6];
        p[12] = p[3];
        p[8] = e[7];
        p[12] = p[12] - p[0];
        p[13] = p[4];
        p[14] = p[5];
        p[13] = p[13] - p[1];
        p[14] = p[14] - p[2];
        float len = (float)sqrt(p[14] * p[14] + p[13] * p[13] + p[12] * p[12]);
        p[15] = len;
        float r = 1.0f / len;
        p[12] = r * p[12];
        p[13] = r * p[13];
        p[14] = r * p[14];
        p[9] = p[13] * 0.0f - p[14] * 1.0f;
        p[10] = p[14] * 0.0f - p[12] * 0.0f;
        p[11] = p[12] * 1.0f - p[13] * 0.0f;
        n++;
        float d = (float)sqrt(k * k + 0.0f) * 1.1f;
        p[16] = d;
        p[17] = d * d;
        if (AT(int, 0x130c) < n)
            AT(int, 0x130c) = n;
    }
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
