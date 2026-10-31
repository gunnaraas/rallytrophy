// Swept-sphere vs. collected triangles (0x418b90).
#include <string>
#include <math.h>
#include "unk_418b90.h"
#include "unk_418570.h"

extern float DAT_004e134c, DAT_004e138c, DAT_004e1408, DAT_004e1348;
extern "C" char __cdecl fun_004b3ee0(float* pt, float* n, float* a, float* b, float* c); // point in triangle
extern "C" char __cdecl fun_004b4090(float* org, float* dir, float r, float* a, float* b, float* t, float* hit); // sphere vs edge

struct SphX { Vec3 c; float d; SphX(const Vec3& p, float r) : c(p) { d = r; } ~SphX() {} };
// NONMATCHING: RALLYTROPHY 0x418b90
// reason: first draft; original frame 0xc4 keeps all pseudocode locals (no reuse), 0x418b90 EH states for Vec3 temps differ
// Tried 2026-10-31: original builds dead Vec3 e1/e2/e3 edge vectors, a dead bounding sphere (struct with dtor), Vec3 mn/mx/q locals (all added, 423->322); still differs in regalloc: original keeps this/esi, org/ebx, chain/ebp and spills i,p to [esp+0x18]/[0x14]; tried volatile self, head/chain reorder, first/last order (no effect).
float SweepQuery::Cast(float r, float* org, float* dir, float* outPt, float* outN)
{
    float d2 = dir[2] * dir[2] + dir[1] * dir[1] + dir[0] * dir[0];
    if (d2 == DAT_004e134c)
        return ((TriCollector*)this)->Closest(r, org, outPt, outN);

    Vec3 e = *(Vec3*)org; e += *(Vec3*)dir;
    Vec3 dv = *(Vec3*)dir;
    Vec3 sm = *(Vec3*)org; sm += e;
    sm.x *= 0.5f; sm.y *= 0.5f; sm.z *= 0.5f;
    SphX sph(sm, (float)sqrt(d2) * 0.5f + r);
    float ex = e.x, ey = e.y, ez = e.z;
    float dx = dv.x, dy = dv.y, dz = dv.z;
    CTri* first = 0;
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
    for (unsigned i = 0; p != 0 && i < (unsigned)(hits.end() - hits.begin()); i++, p++) {
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
    float hitPt[3];
    CTri* t = first;
    while (t != 0) {
        float tt = ((t->d - (t->n.x * org[0] + t->n.z * org[2] + t->n.y * org[1])) + r) /
                   (t->n.x * dir[0] + t->n.y * dir[1] + t->n.z * dir[2]);
        if (DAT_004e134c <= tt && tt < bestT) {
            TVert* c = t->v[2];
            TVert* a = t->v[0];
            TVert* b = t->v[1];
            float* pa = &a->x;
            float* pb = &b->x;
            float* pc = &c->x;
            Vec3 e1 = *(Vec3*)pb; e1 -= *(Vec3*)pa;
            Vec3 e2 = *(Vec3*)pc; e2 -= *(Vec3*)pa;
            Vec3 e3 = *(Vec3*)pc; e3 -= *(Vec3*)pb;
            Vec3 q = *(Vec3*)org;
            q += *(Vec3*)dir * tt;
            q -= t->n * r;
            if (fun_004b3ee0(&q.x, &t->n.x, pa, pb, pc) == 0) {
                if (*t->e[0] == 0) {
                    *t->e[0] = 1;
                    if (fun_004b4090(org, dir, r, pa, pb, &bestT, hitPt)) { edge = 1; bestTri = t; }
                }
                if (*t->e[1] == 0) {
                    *t->e[1] = 1;
                    if (fun_004b4090(org, dir, r, pb, pc, &bestT, hitPt)) { edge = 2; bestTri = t; }
                }
                if (*t->e[2] == 0) {
                    *t->e[2] = 1;
                    if (fun_004b4090(org, dir, r, pc, pa, &bestT, hitPt)) { edge = 3; bestTri = t; }
                }
            } else {
                edge = 0;
                hitPt[0] = q.x; hitPt[1] = q.y; hitPt[2] = q.z;
                bestTri = t;
                bestT = tt;
            }
        }
        t = t->next;
    }

    if (outPt != 0) {
        outPt[0] = hitPt[0]; outPt[1] = hitPt[1]; outPt[2] = hitPt[2];
        if (outN != 0) {
            if (edge == -1) {
                outN[0] = 0.0f; outN[1] = 0.0f; outN[2] = 0.0f;
            } else if (edge == 0) {
                outN[0] = bestTri->n.x; outN[1] = bestTri->n.y; outN[2] = bestTri->n.z;
            } else {
                Vec3 s = *(Vec3*)dir * bestT;
                Vec3 e = *(Vec3*)org + s;
                float vx = e.x - outPt[0], vy = e.y - outPt[1], vz = e.z - outPt[2];
                float inv = DAT_004e1348 / (float)sqrt(vx * vx + vy * vy + vz * vz);
                outN[0] = vx * inv; outN[1] = vy * inv; outN[2] = vz * inv;
            }
        }
    }
    return bestT;
}
