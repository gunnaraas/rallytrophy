// Stage grid cell population (0x40f390).
#include <string>
#include <vector>
#include <math.h>
#include "unk_481390.h"
#include "unk_40b560.h"
#include "unk_40c280.h"
#include "unk_40f390.h"
#include "x87_xform.h"

extern float DAT_004e1348, DAT_004e13b4, DAT_004e138c, DAT_004e13d4, DAT_004e13d0;

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

#define FV(p, o) (*(float*)((BYTE*)(p) + (o)))
#define IV(p, o) (*(int*)((BYTE*)(p) + (o)))

// NONMATCHING: RALLYTROPHY 0x40f390
// reason: RoundF applied (EBP frame now from inline asm); built 2391 vs 2340 bytes, 2153 differ, regalloc differs.
// Tried 2026-10-30 night: decl order/register/while-loop/pointer-walk/+1-folded nx variants, `StageGrid* g = this` member access + volatile diag in loop 1 (627->603 insns); still differs: original keeps this in esi and gx/gy/i in registers across loop tops (ours spills), x87 round interleave.
// Tried 2026-11-01 repass: x87 association checked (only fsub/fsubr and slot differences, sums/lim/round chains agree); 11 variants (this-> direct, pointer-walk and count-down x loops, rows/cols locals, volatile g, double diag, while gy) all 599-642 insns differ vs 603 base; original uses count-down inner loops with cursor spills, ours differs in regalloc throughout, not within permuter range.
// The point transform is the X87XformT asm helper (657 -> 627 differing instructions; the positional byte count got worse, 2153 -> 2190).
void StageGrid::FillCells()
{
    StageGrid* g = this;
    float invW = DAT_004e1348 / g->cellW;
    float margin = *(volatile float*)&DAT_004e13b4;
    int gy = 0, gx;
    float invH = DAT_004e1348 / g->cellH;
    float cw = g->cellW;
    float diag = (float)sqrt(cw * cw + g->cellH * g->cellH);
    float rd;
    for (; gy < g->rows; gy++) {
        for (gx = 0; gx < g->cols; gx++) {
            BYTE* cell = g->cells + (g->cols * gy + gx) * 0xb0;
            unsigned int i;
            for (i = 0; IV(cell, 0x64) != 0 && i < (unsigned int)((IV(cell, 0x68) - IV(cell, 0x64)) >> 2); i++) {
                int obj = IV(IV(IV(cell, 0x64) + i * 4, 0), 4);
                float r = *(volatile float*)&g->radius + FV(obj, 0xc4);
                int nx = RoundF((r + diag) * invW);
                int ny = RoundF((r + diag) * invH);
                int x0 = gx - (nx + 1), x1 = nx + 1 + gx;
                int y0 = gy - (ny + 1), y1 = gy + ny + 1;
                if (x0 < 0) x0 = 0;
                if (g->cols < x1) x1 = g->cols;
                if (y0 < 0) y0 = 0;
                if (g->rows < y1) y1 = g->rows;
                float lim = *(volatile float*)&diag * DAT_004e138c + r;
                lim = lim * lim;
                int y, x;
                for (y = y0; y < y1; y++) {
                    for (x = x0; x < x1; x++) {
                        BYTE* c = g->cells + (g->cols * y + x) * 0xb0;
                        float dx = FV(c, 0) - FV(obj, 0xb8);
                        float dy = FV(c, 8) - FV(obj, 0xc0);
                        float d2 = dx * dx + dy * dy;
                        if (d2 < lim)
                            ((TimedLists*)(c + 0xc))->InsertB(IV(IV(cell, 0x64), i * 4), (float)sqrt(d2) - FV(obj, 0xc4));
                    }
                }
            }
            for (i = 0; IV(cell, 0x74) != 0 && i < (unsigned int)((IV(cell, 0x78) - IV(cell, 0x74)) >> 2); ) {
                int obj = IV(IV(IV(cell, 0x74) + i * 4, 0), 4);
                if (IV(obj, 4) == 2) {
                    int n = g->rows * g->cols;
                    BYTE* c = g->cells + 0xc;
                    for (; n > 0; n--) {
                        ((TimedLists*)c)->InsertA(IV(IV(cell, 0x74), i * 4), 0.0f);
                        c += 0xb0;
                    }
                    i++;
                } else {
                    float r = g->radius + FV(obj, 0xc4);
                    float rd = r + diag;
                    int nx = RoundF(rd * invW);
                    int ny = RoundF(rd * invH);
                    int x0 = gx - (nx + 1), x1 = gx + nx + 1;
                    int y1 = gy + ny + 1, y0 = gy - (ny + 1);
                    if (x0 < 0) x0 = 0;
                    if (g->cols < x1) x1 = g->cols;
                    if (y0 < 0) y0 = 0;
                    if (g->rows < y1) y1 = g->rows;
                    float lim = *(volatile float*)&diag * DAT_004e138c + r;
                    lim = lim * lim;
                    Vec3Slot p;
                    X87XformT(&p.x, &FV(obj, 0xb8), &FV(obj, 0x20), &FV(obj, 0x14));
                    int y, x;
                    for (y = y0; y < y1; y++) {
                        for (x = x0; x < x1; x++) {
                            BYTE* c = g->cells + (g->cols * y + x) * 0xb0;
                            float d2 = (FV(c, 0) - p.x) * (FV(c, 0) - p.x) + (FV(c, 8) - p.z) * (FV(c, 8) - p.z);
                            if (d2 < lim)
                                ((TimedLists*)(c + 0xc))->InsertA(IV(IV(cell, 0x74), i * 4), (float)sqrt(d2) - FV(obj, 0xc4));
                        }
                    }
                    i++;
                }
            }
            for (i = 0; IV(cell, 0x84) != 0 && i < (unsigned int)((IV(cell, 0x88) - IV(cell, 0x84)) >> 2); i++) {
                int obj = IV(IV(cell, 0x84), i * 4);
                float r = margin + FV(obj, 0x28);
                int nx = RoundF((r + diag) * invW);
                int ny = RoundF((r + diag) * invH);
                int x0 = gx - (nx + 1), x1 = nx + 1 + gx;
                int y0 = gy - (ny + 1), y1 = gy + ny + 1;
                if (x0 < 0) x0 = 0;
                if (g->cols < x1) x1 = g->cols;
                if (y0 < 0) y0 = 0;
                if (g->rows < y1) y1 = g->rows;
                float lim = *(volatile float*)&diag * DAT_004e138c + r;
                lim = lim * lim;
                int y, x;
                for (y = y0; y < y1; y++)
                    for (x = x0; x < x1; x++) {
                        BYTE* c = g->cells + (g->cols * y + x) * 0xb0;
                        float dx = FV(c, 0) - FV(obj, 0x1c);
                        float dy = FV(c, 8) - FV(obj, 0x24);
                        if (dx * dx + dy * dy < lim)
                            ((TimedLists*)(c + 0xc))->PushC(obj);
                    }
            }
            for (i = 0; IV(cell, 0x94) != 0 && i < (unsigned int)((IV(cell, 0x98) - IV(cell, 0x94)) >> 2); i++) {
                rd = diag;
                int obj = IV(IV(cell, 0x94), i * 4);
                rd += DAT_004e13d4;
                int nx = RoundF(rd * invW);
                int ny = RoundF(rd * invH);
                int x0 = gx - (nx + 1), x1 = nx + 1 + gx;
                int y0 = gy - (ny + 1), y1 = gy + ny + 1;
                if (x0 < 0) x0 = 0;
                if (g->cols < x1) x1 = g->cols;
                if (y0 < 0) y0 = 0;
                if (g->rows < y1) y1 = g->rows;
                float lim = *(volatile float*)&diag * DAT_004e138c + DAT_004e13d4;
                lim = lim * lim;
                int y, x;
                for (y = y0; y < y1; y++)
                    for (x = x0; x < x1; x++) {
                        BYTE* c = g->cells + (g->cols * y + x) * 0xb0;
                        float dx = FV(c, 0) - FV(obj, 0x30);
                        float dy = FV(c, 8) - FV(obj, 0x38);
                        if (dx * dx + dy * dy < lim)
                            ((TimedLists*)(c + 0xc))->PushD(obj);
                    }
            }
            for (i = 0; IV(cell, 0xa4) != 0 && i < (unsigned int)((IV(cell, 0xa8) - IV(cell, 0xa4)) >> 2); i++) {
                rd = diag;
                int obj = IV(IV(cell, 0xa4), i * 4);
                rd += DAT_004e13d0;
                int nx = RoundF(rd * invW);
                int ny = RoundF(rd * invH);
                int x0 = gx - (nx + 1), x1 = nx + 1 + gx;
                int y0 = gy - (ny + 1), y1 = gy + ny + 1;
                if (x0 < 0) x0 = 0;
                if (g->cols < x1) x1 = g->cols;
                if (y0 < 0) y0 = 0;
                if (g->rows < y1) y1 = g->rows;
                float lim = *(volatile float*)&diag * DAT_004e138c + DAT_004e13d0;
                lim = lim * lim;
                int y, x;
                for (y = y0; y < y1; y++)
                    for (x = x0; x < x1; x++) {
                        BYTE* c = g->cells + (g->cols * y + x) * 0xb0;
                        float dx = FV(c, 0) - FV(obj, 0x14);
                        float dy = FV(c, 8) - FV(obj, 0x1c);
                        if (dx * dx + dy * dy < lim)
                            ((StageVecs5*)(c + 0xc))->InsertBack5(obj);
                    }
            }
        }
    }
}
