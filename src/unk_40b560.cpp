#include <windows.h>
#include "unk_481390.h"
#include "rallytrophy.h"
#include "unk_40b560.h"

extern float DAT_004e13a4, DAT_004e13a8, DAT_004e13ac, DAT_004e13b0, DAT_004e1348, DAT_004e1354, DAT_004e1358, DAT_004e138c, DAT_004f2ffc;
extern float DAT_005082d0[][2];
extern float DAT_005083d0, DAT_005083d4, DAT_005083d8, DAT_005083dc;

__declspec(naked)
// PE transcription preserves the original address calculation and store order.
// FUNCTION: RALLYTROPHY 0x40b560
void TrailQueue::Clear()
{
    __asm {
        _emit 08dh
        _emit 081h
        _emit 0a0h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 081h
        _emit 0a4h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x40b580
void TrailQueue::Push(int corner, int t)
{
    int i = head;
    time[i] = t;
    used[i] = 1;
    quad[i][0] = quad[i][6] = DAT_005082d0[corner][0];
    quad[i][1] = quad[i][3] = DAT_005082d0[corner][1];
    float dx = quad[i][0];
    quad[i][2] = quad[i][4] = dx + DAT_004e13a4;
    float dy = quad[i][1];
    quad[i][5] = quad[i][7] = dy + DAT_004e13a4;
    head = (head + 1) & 3;
}

// NONMATCHING: RALLYTROPHY 0x40b600 (draw via raw vtable calls; float/ftol order unverified)
void TrailQueue::Draw(int now)
{
    int total = 0, alive = 0;
    for (int j = 0; j < 4; j++) {
        if (!used[j]) continue;
        int age = now - time[j];
        if (time[j] < now - 5000) { used[j] = 0; continue; }
        int a;
        if (age < 500) a = (int)((float)age * DAT_004e13b0 * DAT_004e13ac);
        else if (age < 0x1195) a = 0xff;
        else a = 0xff - (int)((float)(age - 0x1194) * DAT_004e13b0 * DAT_004e13ac);
        ((BYTE*)&color[j])[3] = (BYTE)a;
        total += a;
        alive++;
    }
    if (alive == 0 || DAT_00501478 == 0) return;
    float s = (float)total * DAT_004e13a8;
    if (s < DAT_004e1348) s = 1.0f;
    float ys = (float)((float*)DAT_0056b0ac)[1] * DAT_004e1358;
    HudPoint uv[4];
    float* f = (float*)uv;
    f[0] = f[6] = DAT_005083dc * ((float*)DAT_0056b0ac)[2] * DAT_004e1354;
    f[2] = f[4] = (DAT_005083d4 + DAT_005083dc) * ((float*)DAT_0056b0ac)[2] * DAT_004e1354;
    float y = (DAT_005083d8 - (DAT_005083d0 * s + (s - DAT_004e1348) * DAT_004f2ffc) * DAT_004e138c) * ys;
    float h = DAT_005083d0 * ys;
    float step = DAT_004f2ffc * ys + h;
    DAT_0056b0ac->vf38(0);
    DAT_0056b0ac->vf35(*(Sprite**)((BYTE*)DAT_0056b0c8 + 0x139488));
    unsigned i = head;
    for (int n = 4; n; n--) {
        if (used[i]) {
            f[1] = f[3] = y;
            f[5] = f[7] = y + h;
            DAT_0056b0ac->vf66(uv, (HudPoint*)&quad[i][0], color[i], 1.0f, 1.0f);
            y += step;
        }
        i = (i + 1) & 3;
    }
}

// FUNCTION: RALLYTROPHY 0x40b8d0
TimedLists::TimedLists()
{
}

// FUNCTION: RALLYTROPHY 0x40b930
TimedLists::~TimedLists()
{
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x40b9b0
void TimedLists::Collect(GrowArray*)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 010h
        _emit 08bh
        _emit 051h
        _emit 014h
        _emit 053h
        _emit 08bh
        _emit 05ch
        _emit 024h
        _emit 018h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 079h
        _emit 018h
        _emit 03bh
        _emit 0d7h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 014h
        _emit 00fh
        _emit 084h
        _emit 0a9h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 002h
        _emit 08bh
        _emit 040h
        _emit 004h
        _emit 08bh
        _emit 0b3h
        _emit 030h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 083h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0c6h
        _emit 075h
        _emit 05fh
        _emit 08bh
        _emit 08bh
        _emit 034h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 004h
        _emit 031h
        _emit 08dh
        _emit 014h
        _emit 085h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 052h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 028h
        _emit 0e8h
        _emit 06eh
        _emit 058h
        _emit 00bh
        _emit 000h
        _emit 08bh
        _emit 0e8h
        _emit 08bh
        _emit 083h
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0ceh
        _emit 08bh
        _emit 0f0h
        _emit 08bh
        _emit 0fdh
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 028h
        _emit 050h
        _emit 089h
        _emit 08bh
        _emit 030h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0d8h
        _emit 015h
        _emit 00bh
        _emit 000h
        _emit 08bh
        _emit 083h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 020h
        _emit 08dh
        _emit 054h
        _emit 085h
        _emit 000h
        _emit 089h
        _emit 093h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 089h
        _emit 0abh
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 083h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 01ch
        _emit 089h
        _emit 030h
        _emit 08bh
        _emit 0b3h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c2h
        _emit 008h
        _emit 046h
        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 03bh
        _emit 0d7h
        _emit 089h
        _emit 0b3h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 083h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 00fh
        _emit 085h
        _emit 057h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 051h
        _emit 004h
        _emit 08bh
        _emit 079h
        _emit 008h
        _emit 03bh
        _emit 0d7h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 018h
        _emit 00fh
        _emit 084h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 002h
        _emit 08bh
        _emit 048h
        _emit 004h
        _emit 08bh
        _emit 083h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0b3h
        _emit 030h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0c6h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 075h
        _emit 057h
        _emit 08bh
        _emit 093h
        _emit 034h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 004h
        _emit 032h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 0c1h
        _emit 0e0h
        _emit 002h
        _emit 050h
        _emit 0e8h
        _emit 0b3h
        _emit 057h
        _emit 00bh
        _emit 000h
        _emit 08bh
        _emit 0e8h
        _emit 08bh
        _emit 083h
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0ceh
        _emit 08bh
        _emit 0f0h
        _emit 08bh
        _emit 0fdh
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 020h
        _emit 050h
        _emit 089h
        _emit 08bh
        _emit 030h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 01dh
        _emit 015h
        _emit 00bh
        _emit 000h
        _emit 08bh
        _emit 083h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 020h
        _emit 08dh
        _emit 054h
        _emit 085h
        _emit 000h
        _emit 089h
        _emit 093h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 02ch
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 089h
        _emit 0abh
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 083h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 089h
        _emit 008h
        _emit 08bh
        _emit 08bh
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c2h
        _emit 008h
        _emit 041h
        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 03bh
        _emit 0d7h
        _emit 089h
        _emit 08bh
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 083h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 00fh
        _emit 085h
        _emit 05fh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 010h
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x40bb40
void TimedLists::InsertA(int ptr, float key)
{
    std::vector<TimedEntry>::iterator it = a.begin();
    while (it != a.end() && it->key <= key) ++it;
    TimedEntry e = { ptr, key };
    a.insert(it, e);
}

// FUNCTION: RALLYTROPHY 0x40bd30
void TimedLists::InsertB(int ptr, float key)
{
    std::vector<TimedEntry>::iterator it = b.begin();
    while (it != b.end() && it->key <= key) ++it;
    TimedEntry e = { ptr, key };
    b.insert(it, e);
}

// FUNCTION: RALLYTROPHY 0x40bf20
void TimedLists::PushC(int v)
{
    c.push_back(v);
}

// FUNCTION: RALLYTROPHY 0x40c0d0
void TimedLists::PushD(int v)
{
    d.push_back(v);
}
