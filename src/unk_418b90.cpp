// Swept-sphere vs. collected triangles (0x418b90).
#include <string>
#include <math.h>
#include "unk_418b90.h"
#include "unk_418570.h"

extern float DAT_004e134c, DAT_004e138c, DAT_004e1408, DAT_004e1348;
extern "C" char __cdecl fun_004b3ee0(float* pt, float* n, float* a, float* b, float* c); // point in triangle
extern "C" char __cdecl fun_004b4090(float* org, float* dir, float r, float* a, float* b, float* t, float* hit); // sphere vs edge

struct SphY { Vec3 c; float d; ~SphY() {} };
struct EmptyD { ~EmptyD() {} };
struct SphX { Vec3 c; float d; SphX(const Vec3& p, float r) : c(p) { d = r; } ~SphX() {} };
// NONMATCHING: RALLYTROPHY 0x418b90
// reason: first draft; original frame 0xc4 keeps all pseudocode locals (no reuse), 0x418b90 EH states for Vec3 temps differ
// Tried 2026-10-31: original builds dead Vec3 e1/e2/e3 edge vectors, a dead bounding sphere (struct with dtor), Vec3 mn/mx/q locals (all added, 423->322); still differs in regalloc: original keeps this/esi, org/ebx, chain/ebp and spills i,p to [esp+0x18]/[0x14]; tried volatile self, head/chain reorder, first/last order (no effect).
// Tried 2026-11-01 re-pass: float sums re-checked against asm (d2, plane num/den dots, normal length fixed to the original association; dead e3 = b-c); 322->326 insns differ is the cost of correct sums. d2 as double/volatile, bestT/chain/edge split decls, e1/e2 order, for-loop, folded operator chain: no gain; d2 stays on the x87 stack in the original (fcom then fsqrt) while ours spills it.
// Pass 2026-11-13 (r6418b90): 307 -> 253 insns differ (2140 -> 2157 bytes): hit test as if(fun) hit else edges (block layout), loop-2 iterates `volatile first` (orig keeps first/t in one [esp+0x10] slot, reloaded), loop bound hits.size() w/o p!=0, `Vec3 sm; sm = ...` default-ctor form. Rejected: non-volatile t (size 2059 but 373 insns), ScenePoint types (same bytes, only symbol names differ), mn/mx/bestT decl moves, extra self ptr, rv/else return shape.
float SweepQuery::Cast(float r, float* org, float* dir, float* outPt, float* outN)
{
    float d2 = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    if (d2 == DAT_004e134c)
        return ((TriCollector*)this)->Closest(r, org, outPt, outN);

    Vec3 e = *(Vec3*)org; e += *(Vec3*)dir;
    Vec3 dv = *(Vec3*)dir;
    SphY sph;
    Vec3 sm; sm = *(Vec3*)org; sm += e;
    sm.x *= 0.5f; sm.y *= 0.5f; sm.z *= 0.5f;
    sph.c = sm; sph.d = (float)sqrt(d2) * 0.5f + r;
    float ex = e.x, ey = e.y, ez = e.z;
    float dx = dv.x, dy = dv.y, dz = dv.z;
    CTri* volatile first = 0;
    CTri* last = 0;
    Vec3 mn = *(Vec3*)org;
    Vec3 mx = *(Vec3*)org;
    if (e.x < mn.x) mn.x = e.x; else if (e.x > mx.x) mx.x = e.x;
    if (e.y < mn.y) mn.y = e.y; else if (e.y > mx.y) mx.y = e.y;
    if (e.z < mn.z) mn.z = e.z; else if (e.z > mx.z) mx.z = e.z;
    mn.x -= r; mn.y -= r; mn.z -= r;
    mx.x += r; mx.y += r; mx.z += r;

    CTri* chain = 0;
    CTri** p = hits.begin();
    for (unsigned i = 0; i < hits.size(); i++, p++) {
        CTri* t = *p;
        if (dv.z * t->n.z + dv.y * t->n.y + dv.x * t->n.x < DAT_004e1408) {
            unsigned s = t->sel;
            if (t->v[(s >> 6) & 3]->y >= mn.y && t->v[(s >> 4) & 3]->y <= mx.y &&
                t->v[(s >> 2) & 3]->x >= mn.x && t->v[s & 3]->x <= mx.x &&
                t->v[(s >> 10) & 3]->z >= mn.z && t->v[(s >> 8) & 3]->z <= mx.z) {
                *t->e[0] = 0;
                *t->e[1] = 0;
                *t->e[2] = 0;
                t->next = 0;
                if (chain != 0)
                    last->next = t;
                else
                    first = t, chain = t;
                last = t;
            }
        }
    }
    head = chain;

    float bestT = 1.0f;
    int edge = -1;
    CTri* bestTri = 0;
    Vec3 hitPtV;
    EmptyD z7;
    while (first != 0) {
        float den = first->n.z * dir[2]; // chained: keeps the original's (z+y)+x association
        den = first->n.y * dir[1] + den;
        den = first->n.x * dir[0] + den;
        float num = first->n.y * org[1];
        num = first->n.z * org[2] + num;
        num = first->n.x * org[0] + num;
        float tt = ((first->d - num) + r) / den;
        if (DAT_004e134c <= tt && tt < bestT) {
            TVert* c = first->v[2];
            TVert* a = first->v[0];
            TVert* b = first->v[1];
            float* pa = &a->x;
            float* pb = &b->x;
            float* pc = &c->x;
            Vec3 e1 = *(Vec3*)pb; e1 -= *(Vec3*)pa;
            Vec3 e2 = *(Vec3*)pc; e2 -= *(Vec3*)pa;
            Vec3 e3 = *(Vec3*)pb; e3 -= *(Vec3*)pc;
            Vec3 q = *(Vec3*)org;
            q += *(Vec3*)dir * tt;
            q -= first->n * r;
            if (fun_004b3ee0(&q.x, &first->n.x, pa, pb, pc)) {
                edge = 0;
                hitPtV.x = q.x; hitPtV.y = q.y; hitPtV.z = q.z;
                bestTri = first;
                bestT = tt;
            } else {
                if (*first->e[0] == 0) {
                    *first->e[0] = 1;
                    if (fun_004b4090(org, dir, r, pa, pb, &bestT, &hitPtV.x)) { edge = 1; bestTri = first; }
                }
                if (*first->e[1] == 0) {
                    *first->e[1] = 1;
                    if (fun_004b4090(org, dir, r, pb, pc, &bestT, &hitPtV.x)) { edge = 2; bestTri = first; }
                }
                if (*first->e[2] == 0) {
                    *first->e[2] = 1;
                    if (fun_004b4090(org, dir, r, pc, pa, &bestT, &hitPtV.x)) { edge = 3; bestTri = first; }
                }
            }
        }
        first = first->next;
    }

    if (outPt != 0) {
        outPt[0] = hitPtV.x; outPt[1] = hitPtV.y; outPt[2] = hitPtV.z;
        if (outN != 0) {
            if (edge == -1) {
                outN[0] = 0.0f; outN[1] = 0.0f; outN[2] = 0.0f;
            } else if (edge == 0) {
                outN[0] = bestTri->n.x; outN[1] = bestTri->n.y; outN[2] = bestTri->n.z;
            } else {
                Vec3 s = *(Vec3*)dir * bestT;
                Vec3 e = *(Vec3*)org + s;
                float vx = e.x - outPt[0], vy = e.y - outPt[1], vz = e.z - outPt[2];
                float inv = DAT_004e1348 / (float)sqrt(vz * vz + vy * vy + vx * vx);
                outN[0] = vx * inv; outN[1] = vy * inv; outN[2] = vz * inv;
            }
        }
    }
    return bestT;
}
