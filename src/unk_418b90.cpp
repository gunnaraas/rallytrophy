// Swept-sphere vs. collected triangles (0x418b90).
#include <string>
#include <math.h>
#include "unk_418b90.h"
#include "unk_418570.h"

extern float DAT_004e134c, DAT_004e138c, DAT_004e1408, DAT_004e1348;
extern "C" char __cdecl fun_004b3ee0(float* pt, float* n, float* a, float* b, float* c); // point in triangle
extern "C" char __cdecl fun_004b4090(float* org, float* dir, float r, float* a, float* b, float* t, float* hit); // sphere vs edge

// NONMATCHING: RALLYTROPHY 0x418b90
// reason: first draft; original frame 0xc4 keeps all pseudocode locals (no reuse), 0x418b90 EH states for Vec3 temps differ
float SweepQuery::Cast(float r, float* org, float* dir, float* outPt, float* outN)
{
    float d2 = dir[2] * dir[2] + dir[1] * dir[1] + dir[0] * dir[0];
    if (d2 == DAT_004e134c)
        return ((TriCollector*)this)->Closest(r, org, outPt, outN);

    float ex = org[0] + dir[0], ey = org[1] + dir[1], ez = org[2] + dir[2];
    float dx = dir[0], dy = dir[1], dz = dir[2];
    CTri* first = 0;
    CTri* last = 0;
    float mnx = org[0], mny = org[1], mnz = org[2];
    float mxx = org[0], mxy = org[1], mxz = org[2];
    if (ex < mnx) mnx = ex; else if (mxx < ex) mxx = ex;
    if (ey < mny) mny = ey; else if (mxy < ey) mxy = ey;
    if (ez < mnz) mnz = ez; else if (mxz < ez) mxz = ez;
    mnx -= r; mny -= r; mnz -= r;
    mxx += r; mxy += r; mxz += r;

    CTri* chain = 0;
    CTri** p = hits.begin();
    for (unsigned i = 0; p != 0 && i < hits.size(); i++, p++) {
        CTri* t = *p;
        if (dx * t->n.x + dy * t->n.y + dz * t->n.z < DAT_004e1408) {
            unsigned s = t->sel;
            if (mny <= t->v[(s >> 6) & 3]->y && t->v[(s >> 4) & 3]->y <= mxy &&
                mnx <= t->v[(s >> 2) & 3]->x && t->v[s & 3]->x <= mxx &&
                mnz <= t->v[(s >> 10) & 3]->z && t->v[(s & 0x300) >> 8]->z <= mxz) {
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
    CTri* t = chain;
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
            float cx = org[0], cy = org[1], cz = org[2];
            Vec3 tmp, tmp2;
            Vec3 m = *(Vec3*)dir * tt;
            cx += m.x; cy += m.y; cz += m.z;
            Vec3 k = t->n * r;
            cx -= k.x; cy -= k.y; cz -= k.z;
            float q[3] = { cx, cy, cz };
            if (fun_004b3ee0(q, &t->n.x, pa, pb, pc) == 0) {
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
                hitPt[0] = q[0]; hitPt[1] = q[1]; hitPt[2] = q[2];
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
