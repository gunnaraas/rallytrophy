// Sky ray tracing (cdecl), 0x4bb260-0x4bbdef (bb3a0 lives elsewhere). Map: analysis/range-4b7a40.md.
#include "unk_481390.h"
#include "unk_4ba440.h"
#include "unk_4bb260.h"

extern float DAT_004e1348, DAT_004e134c, DAT_004e138c, DAT_004e1ff4, DAT_004e2058;
extern SegmentInfo DAT_00581de8;
extern float DAT_00581e20;
extern int DAT_00581e24, DAT_00581e28;
float FUN_004bb3a0(int node);                                          // 0x4bb3a0
void FUN_004b3470(float* a, float* b, float* c, float* p, Vec3Slot* out); // 0x4b3470

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4bb260
float TraceRay(SkyWorld*, const float*, const float*)
{
    __asm {
        _emit 081h
        _emit 0ech
        _emit 004h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 084h
        _emit 024h
        _emit 00ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 008h
        _emit 08bh
        _emit 050h
        _emit 004h
        _emit 08bh
        _emit 040h
        _emit 008h
        _emit 053h
        _emit 0a3h
        _emit 0fch
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 08bh
        _emit 084h
        _emit 024h
        _emit 014h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 055h
        _emit 089h
        _emit 00dh
        _emit 0f4h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 08bh
        _emit 008h
        _emit 089h
        _emit 015h
        _emit 0f8h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 08bh
        _emit 050h
        _emit 004h
        _emit 08bh
        _emit 040h
        _emit 008h
        _emit 056h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 01eh
        _emit 058h
        _emit 000h
        _emit 057h
        _emit 0b9h
        _emit 0e8h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 089h
        _emit 015h
        _emit 004h
        _emit 01eh
        _emit 058h
        _emit 000h
        _emit 0a3h
        _emit 008h
        _emit 01eh
        _emit 058h
        _emit 000h
        _emit 0e8h
        _emit 0bch
        _emit 0feh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 08ch
        _emit 024h
        _emit 018h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 081h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0dbh
        _emit 085h
        _emit 0c0h
        _emit 0c7h
        _emit 005h
        _emit 024h
        _emit 01eh
        _emit 058h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 005h
        _emit 028h
        _emit 01eh
        _emit 058h
        _emit 000h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 005h
        _emit 020h
        _emit 01eh
        _emit 058h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 00fh
        _emit 08eh
        _emit 087h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 094h
        _emit 024h
        _emit 018h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 08ah
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 034h
        _emit 099h
        _emit 033h
        _emit 0edh
        _emit 085h
        _emit 0f6h
        _emit 074h
        _emit 068h
        _emit 08dh
        _emit 07ch
        _emit 024h
        _emit 014h
        _emit 083h
        _emit 07eh
        _emit 004h
        _emit 001h
        _emit 075h
        _emit 027h
        _emit 0f6h
        _emit 086h
        _emit 0cch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 001h
        _emit 074h
        _emit 01eh
        _emit 056h
        _emit 0e8h
        _emit 084h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h
        _emit 005h
        _emit 020h
        _emit 01eh
        _emit 058h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 0d8h
        _emit 01dh
        _emit 048h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 051h
        _emit 08bh
        _emit 086h
        _emit 008h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 00ah
        _emit 089h
        _emit 037h
        _emit 045h
        _emit 083h
        _emit 0c7h
        _emit 004h
        _emit 08bh
        _emit 0f0h
        _emit 0ebh
        _emit 01bh
        _emit 08bh
        _emit 0b6h
        _emit 000h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0f6h
        _emit 075h
        _emit 0b5h
        _emit 085h
        _emit 0edh
        _emit 074h
        _emit 011h
        _emit 08bh
        _emit 057h
        _emit 0fch
        _emit 08bh
        _emit 0b2h
        _emit 000h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 04dh
        _emit 083h
        _emit 0efh
        _emit 004h
        _emit 085h
        _emit 0f6h
        _emit 075h
        _emit 0a0h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 043h
        _emit 03bh
        _emit 0d8h
        _emit 00fh
        _emit 08ch
        _emit 079h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 005h
        _emit 020h
        _emit 01eh
        _emit 058h
        _emit 000h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 081h
        _emit 0c4h
        _emit 004h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0c3h
        _emit 0d9h
        _emit 005h
        _emit 048h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 081h
        _emit 0c4h
        _emit 004h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0c3h
    }
}
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4bba10
int SkySampleBit(SkyTexOwner*, const unsigned short*, int, float*)
{
    __asm {
        _emit 055h
        _emit 08bh
        _emit 0ech
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 038h
        _emit 0fah
        _emit 04dh
        _emit 000h
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
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
        _emit 00ch
        _emit 08bh
        _emit 045h
        _emit 008h
        _emit 08bh
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 056h
        _emit 074h
        _emit 007h
        _emit 08bh
        _emit 070h
        _emit 058h
        _emit 085h
        _emit 0f6h
        _emit 075h
        _emit 014h
        _emit 0b8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 05eh
        _emit 08bh
        _emit 04dh
        _emit 0f4h
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c3h
        _emit 08bh
        _emit 04dh
        _emit 00ch
        _emit 033h
        _emit 0c0h
        _emit 066h
        _emit 08bh
        _emit 001h
        _emit 053h
        _emit 057h
        _emit 08dh
        _emit 014h
        _emit 080h
        _emit 08dh
        _emit 004h
        _emit 050h
        _emit 08bh
        _emit 055h
        _emit 010h
        _emit 08dh
        _emit 03ch
        _emit 082h
        _emit 033h
        _emit 0c0h
        _emit 066h
        _emit 08bh
        _emit 041h
        _emit 002h
        _emit 08dh
        _emit 01ch
        _emit 080h
        _emit 08dh
        _emit 004h
        _emit 058h
        _emit 08dh
        _emit 01ch
        _emit 082h
        _emit 033h
        _emit 0c0h
        _emit 066h
        _emit 08bh
        _emit 041h
        _emit 004h
        _emit 08dh
        _emit 00ch
        _emit 080h
        _emit 08dh
        _emit 004h
        _emit 048h
        _emit 08dh
        _emit 004h
        _emit 082h
        _emit 089h
        _emit 045h
        _emit 008h
        _emit 08bh
        _emit 055h
        _emit 014h
        _emit 08dh
        _emit 04dh
        _emit 0e8h
        _emit 051h
        _emit 052h
        _emit 050h
        _emit 053h
        _emit 057h
        _emit 0c7h
        _emit 045h
        _emit 0fch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0d2h
        _emit 079h
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 045h
        _emit 0f0h
        _emit 08bh
        _emit 045h
        _emit 008h
        _emit 0d8h
        _emit 048h
        _emit 01ch
        _emit 083h
        _emit 0c4h
        _emit 014h
        _emit 0d9h
        _emit 045h
        _emit 0ech
        _emit 0d8h
        _emit 04bh
        _emit 01ch
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 045h
        _emit 0e8h
        _emit 0d8h
        _emit 04fh
        _emit 01ch
        _emit 0deh
        _emit 0c1h
        _emit 0dah
        _emit 00eh
        _emit 0d9h
        _emit 05dh
        _emit 008h
        _emit 0d9h
        _emit 045h
        _emit 0f0h
        _emit 0d8h
        _emit 048h
        _emit 020h
        _emit 0d9h
        _emit 045h
        _emit 0ech
        _emit 0d8h
        _emit 04bh
        _emit 020h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 045h
        _emit 0e8h
        _emit 0d8h
        _emit 04fh
        _emit 020h
        _emit 0deh
        _emit 0c1h
        _emit 0dah
        _emit 04eh
        _emit 004h
        _emit 0d9h
        _emit 05dh
        _emit 010h
        _emit 0d9h
        _emit 045h
        _emit 008h
        _emit 0dbh
        _emit 05dh
        _emit 00ch
        _emit 08bh
        _emit 00eh
        _emit 08bh
        _emit 05dh
        _emit 00ch
        _emit 049h
        _emit 023h
        _emit 0cbh
        _emit 0d9h
        _emit 045h
        _emit 010h
        _emit 0dbh
        _emit 05dh
        _emit 008h
        _emit 08bh
        _emit 046h
        _emit 004h
        _emit 08bh
        _emit 055h
        _emit 008h
        _emit 048h
        _emit 023h
        _emit 0c2h
        _emit 00fh
        _emit 0afh
        _emit 046h
        _emit 008h
        _emit 08bh
        _emit 0d1h
        _emit 0c1h
        _emit 0fah
        _emit 003h
        _emit 003h
        _emit 0c2h
        _emit 08bh
        _emit 056h
        _emit 018h
        _emit 08ah
        _emit 004h
        _emit 010h
        _emit 083h
        _emit 0e1h
        _emit 007h
        _emit 0bah
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0d3h
        _emit 0e2h
        _emit 08bh
        _emit 04dh
        _emit 0f4h
        _emit 05fh
        _emit 05bh
        _emit 022h
        _emit 0c2h
        _emit 0f6h
        _emit 0d8h
        _emit 05eh
        _emit 01bh
        _emit 0c0h
        _emit 0f7h
        _emit 0d8h
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c3h
    }
}

// NONMATCHING: RALLYTROPHY 0x4bbb30 (not tuned; 684 vs 646 bytes)
// Tried 2026-10-31: literals (65536.0f, 1/65536, 0.5f) not globals, m=0 then obj?map, early return, int masks, 60 shuffled decl orders; 34 insns differ, remaining is int reg allocation (u1/row0/r0 in wrong regs, r0/r1 spill).
// Tried 2026-10-31 re-pass: fx/fy as refs to dead tri/t param slots (worse, 52-64), 40 random statement-order shuffles (best 36), volatile r0/r1/acc, operand swaps (best 33); original computes fx and fy back to back with one hoisted zero and r0 in eax, r1 in the verts slot, but ours hoists the r0/r1 imuls early and spills both.
float SkySampleBilinear(SkyTexOwner* t, const unsigned short* tri, int verts, float* point)
{
    SkyShadeMap* m;
    m = 0;
    if (t->obj != 0)
        m = t->obj->map;
    if (m == 0)
        return DAT_004e1348;
    {
        float* a = (float*)(verts + tri[0] * 0x2c);
        float* b = (float*)(verts + tri[1] * 0x2c);
        float* c = (float*)(verts + tri[2] * 0x2c);
        Vec3Slot w;
        FUN_004b3470(a, b, c, point, &w);
        float fu = (w.x * a[7] + w.y * b[7] + w.z * c[7]) * (float)m->width - 0.5f;
        float fv = (w.x * a[8] + w.y * b[8] + w.z * c[8]) * (float)m->height - 0.5f;
        if (t->flags & 4) {
            if (fu < DAT_004e134c)
                fu = DAT_004e134c;
            else {
                float lim = (float)(m->width - 1);
                if (!(fu < lim))
                    fu = lim;
            }
        }
        if (t->flags & 8) {
            if (fv < DAT_004e134c)
                fv = 0.0f;
            else {
                float lim = (float)(m->height - 1);
                if (!(fv < lim))
                    fv = lim;
            }
        }
        unsigned iu = ((m->width * 0x10000 - 1U) | 0xffff) & RoundF(65536.0f * fu);
        unsigned iv = ((m->height * 0x10000 - 1U) | 0xffff) & RoundF(65536.0f * fv);
        unsigned col0 = iu >> 16;
        unsigned u1 = (m->width - 1U) & (col0 + 1);
        unsigned row0 = iv >> 16;
        float fx = (float)(iu & 0xffff) * 0.0000152587890625f;
        int r0 = m->pitch * row0;
        float fy = (float)(iv & 0xffff) * 0.0000152587890625f;
        double acc = DAT_004e134c;
        int r1 = ((m->height - 1U) & (row0 + 1)) * m->pitch;
        unsigned char* p0 = m->bits + (col0 >> 3);
        int b0 = 1 << (col0 & 7);
        if (p0[r0] & b0)
            acc = (DAT_004e1348 - (double)fx) * (DAT_004e1348 - (double)fy);
        unsigned char* p1 = m->bits + (u1 >> 3);
        int b1 = 1 << (u1 & 7);
        if (p1[r0] & b1)
            acc = (DAT_004e1348 - (double)fy) * (double)fx + acc;
        if (p0[r1] & b0)
            acc = (DAT_004e1348 - (double)fx) * (double)fy + acc;
        if (b1 & p1[r1])
            acc = (double)fy * (double)fx + acc;
        return (float)acc;
    }
}
