// Collision model constructor (0x419f30): filters triangles of a source mesh, welds vertices, builds the edge table.
#include <stdexcept>
#include <math.h>
#include <string.h>
#include "unk_419f00.h"
#include "unk_419f30.h"
#include "unk_414330.h"
#include "x87_cross.h"

extern "C" void fun_004bd000(void* p);
extern float DAT_004e134c, DAT_004e1348;
extern int DAT_00508504, DAT_00508508, DAT_0050850c;

// Raw view of a ColTri (0x40 bytes).
struct ColTriRaw {
    float center[3], radius;     // 0x00 (bounding sphere)
    short id;                    // 0x10
    short pad12;
    int pad14;
    float n[3];                  // 0x18
    float d;                     // 0x24
    void* v[3];                  // 0x28
    int e[3];                    // 0x34
};
struct EdgeEnt { int lo, hi, id; };

// Local element types with user ctor+dtor so new[] goes through the EH vector-ctor helper.
struct LV { int pad; float x, y, z; LV() {} ~LV() {} };
struct LT { char pad[0x40]; LT() {} ~LT() {} };
struct LL { float x, y, z; LL() {} ~LL() {} };

// NONMATCHING: RALLYTROPHY 0x419f30
// reason: logic complete; original uses an ebp frame and 2386 vs 2510 bytes (register allocation differs); throw type/ctor arity (orig takes 1 stack arg) unverified
// Tried 2026-11-01 re-pass: plain zeroing loops + no volatile mesh reload + volatile zero reg z improved 591->557 insns differ (2529->2489 bytes); memset, tri cursor, srcV hoist, z in loops, volatile p2.y/z all worse or equal; plateau far from 10 so no permuter.
// Tried 2026-10-31: memset zeroing, LV/LT/LL vector-ctor new[], volatile reload of the mesh param (VM), per-iteration verts reload, size CSE (630->591 insns); still differs: original keeps 0 in esi, frame 0x74 vs ours 0x80, and the tri-filter keeps p2.y/z in stack temps; Vec3-copy and EH-state variants all landed worse.
ColModel::ColModel(void* meshp, int)
{
    ColSrcMesh* mesh = (ColSrcMesh*)meshp;
    int z; *(volatile int*)&z = 0;
    root = (TriNode*)z;
    if (mesh->nVerts == 0 || mesh->nTris == 0) {
        throw std::logic_error("missing collision model");
    }
#define VM mesh
    int srcT = mesh->nTris;
    pad24 = 1;
    nVerts = z;
    nTris = z;
    int* remap = new int[mesh->nVerts];
    int* triOk = new int[srcT];
    int i;
    for (i = 0; i < mesh->nVerts; i++) remap[i] = 0;
    for (i = 0; i < srcT; i++) triOk[i] = 0;
    ColSrcTri* st = VM->tris;
    for (i = 0; i < srcT; i++) {
        if (st[i].obj->kind < 0x11) {
            int ia = st[i].a, ib = st[i].b, ic = st[i].c;
            float* vb = VM->verts;
            S414Vec3* p2 = (S414Vec3*)(vb + ib * 3);
            S414Vec3* p1 = (S414Vec3*)(vb + ia * 3);
            S414Vec3* p3 = (S414Vec3*)(vb + ic * 3);
            float ey = p2->y, ez = p2->z;
            float ex = p2->x - p1->x; ey -= p1->y; ez -= p1->z;
            if (ey * ey + ez * ez + ex * ex > DAT_004e134c) {
                ey = p3->y; ez = p3->z;
                ex = p3->x - p2->x; ey -= p2->y; ez -= p2->z;
                if (ey * ey + ez * ez + ex * ex > DAT_004e134c) {
                    ey = p1->y; ez = p1->z;
                    ex = p1->x - p3->x; ey -= p3->y; ez -= p3->z;
                    if (ey * ey + ez * ez + ex * ex > DAT_004e134c) {
                        nTris++;
                        triOk[i] = 1;
                        remap[st[i].a] = 1;
                        remap[st[i].b] = 1;
                        remap[st[i].c] = 1;
                    }
                }
            }
        }
    }
    int next = 1;
    for (i = 0; i < VM->nVerts; i++) {
        if (remap[i] != 0) {
            remap[i] = next++;
            nVerts++;
        }
    }
    ColVert* V = (ColVert*)new LV[nVerts]; verts = V;
    ColVert* W = (ColVert*)new LV[nVerts]; wverts = W;
    ColTri* TT = (ColTri*)new LT[nTris]; tris = TT;
    centers = (ColLocal*)new LL[nTris];
    for (i = 0; i < VM->nVerts; i++) {
        int id = remap[i];
        if (id != 0) {
            float* s = VM->verts + i * 3;
            ColVert* lv = &verts[id - 1];
            lv->x = s[0];
            lv->y = s[1];
            lv->z = s[2];
            lv->pad = 0;
            wverts[id - 1].x = lv->x;
            wverts[id - 1].y = lv->y;
            wverts[id - 1].z = lv->z;
            wverts[id - 1].pad = 0;
        }
    }
    ColTriRaw* t = (ColTriRaw*)tris;
    int* ti = (int*)t;
    int k = 0;
    for (i = 0; i < srcT; i++) {
        if (triOk[i] != 0) {
            ti[k * 16 + 10] = remap[st[i].a] - 1;
            ti[k * 16 + 11] = remap[st[i].b] - 1;
            ti[k * 16 + 12] = remap[st[i].c] - 1;
            t[k].id = (short)st[i].obj->kind;
            k++;
        }
    }
    delete triOk;
    for (i = 0; i < nVerts; i++) remap[i] = 0;
    for (i = 0; i < nTris; i++) {
        int a = ti[i * 16 + 10], b = ti[i * 16 + 11], c = ti[i * 16 + 12];
        int m1 = a;
        if (b <= a) m1 = b;
        int m2 = b;
        if (c <= b) m2 = c;
        if (c < a) a = c;
        remap[m1]++;
        remap[m2]++;
        remap[a]++;
    }
    int total = 0;
    int* cur = remap;
    for (i = 0; i < nVerts; i++) {
        int n = *cur;
        *cur = total;
        total += n;
        cur++;
    }
    // the prefix sums stay in `remap` (original keeps the base pointer in a separate local)
    int* table = new int[total * 3];
    for (i = 0; i < total * 3; i++) table[i] = 0;
    next = 1;
    for (i = 0; i < nTris; i++) {
        ColTriRaw* tr = &t[i];
        int a = ti[i * 16 + 10], b = ti[i * 16 + 11], c = ti[i * 16 + 12];
        ti[i * 16 + 10] = (int)((char*)wverts + a * 0x10);
        ti[i * 16 + 11] = (int)((char*)wverts + b * 0x10);
        ti[i * 16 + 12] = (int)((char*)wverts + c * 0x10);
        float* pa = &verts[a].x;
        float* pb = &verts[b].x;
        float* pc = &verts[c].x;
        ((SceneSphere*)tr)->SetTriple((ScenePoint*)pa, (ScenePoint*)pb, (ScenePoint*)pc);
        if (tr->radius == DAT_004e134c) {
            tr->id = 0x13;
            pad24 = 0;
            tr->n[0] = 0;
            ti[i * 16 + 7] = 0x3f800000;
            tr->n[2] = 0;
        } else {
            float u[3], w[3];
            ((S414Vec3*)pc)->Sub(u, pa);
            ((S414Vec3*)pb)->Sub(w, pa);
            X87Cross(tr->n, w, u);
            float kk = DAT_004e1348 / (float)sqrt(tr->n[2] * tr->n[2] + tr->n[1] * tr->n[1] + tr->n[0] * tr->n[0]);
            tr->n[0] = kk * tr->n[0];
            tr->n[1] = kk * tr->n[1];
            tr->n[2] = kk * tr->n[2];
        }
        tr->d = verts[a].x * tr->n[0] + verts[a].y * tr->n[1] + verts[a].z * tr->n[2];
        centers[i].x = tr->center[0];
        centers[i].y = tr->center[1];
        centers[i].z = tr->center[2];
        int e0lo, e0hi;
        if (b <= a) { e0hi = a; e0lo = b; } else { e0hi = b; e0lo = a; }
        int e1lo = b, e1hi = c;
        if (c <= b) { e1lo = c; e1hi = b; }
        int e2lo, e2hi;
        if (c < a) { e2lo = c; e2hi = a; } else { e2lo = a; e2hi = c; }
        int lo[3] = { e0lo, e1lo, e2lo };
        int hi[3] = { e0hi, e1hi, e2hi };
        for (k = 0; k < 3; k++) {
            EdgeEnt* e = (EdgeEnt*)(table + remap[lo[k]] * 3);
            int id;
            for (; e->id != 0; e++) {
                if (e->lo == lo[k] && e->hi == hi[k]) break;
            }
            if (e->id != 0) {
                id = e->id;
            } else {
                e->id = next;
                e->lo = lo[k];
                e->hi = hi[k];
                id = next++;
            }
            tr->e[k] = id;
        }
    }
    nInts = next - 1;
    ints = new int[nInts];
    for (i = 0; i < nTris; i++) {
        for (k = 0; k < 3; k++) t[i].e[k] = (int)((char*)ints - 4 + t[i].e[k] * 4);
    }
    delete table;
    delete remap;
    ClearInts();
    DAT_00508508 += nTris;
    DAT_0050850c += nInts;
    DAT_00508504 += nVerts;
    ComputeSel();
}
