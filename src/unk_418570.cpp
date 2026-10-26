// Collision triangle tree and sphere-query collector (0x418570-0x419be0).
#include "unk_418570.h"
#include <math.h>

extern "C" void* fun_004c1276(int bytes);
extern "C" void fun_004bd000(void* p);
extern "C" float fun_004b3800(float* p, float* org, float* e1, float* e2, float* out); // closest point on triangle, returns dist^2
extern float DAT_004e134c, DAT_004e1348;
extern Tri* DAT_00508500;
struct V3Diff { float x, y, z; float* Sub(float* out, const float* o) const; }; // 0x414330

// FUNCTION: RALLYTROPHY 0x418570
void TriNode::FitAll(TriMesh* m)
{
    TriNode* n = this;
    do {
        n->FitBox(m);
        n->Fit2(m);
        if (n->child != 0) {
            n->child->FitAll(m);
        }
        n = n->sibling;
    } while (n != 0);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4185b0
void TriNode::FitBox(TriMesh*)
{
    __asm {
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 004h
        _emit 08bh
        _emit 051h
        _emit 024h
        _emit 055h
        _emit 08bh
        _emit 068h
        _emit 018h
        _emit 00fh
        _emit 0bfh
        _emit 002h
        _emit 0c1h
        _emit 0e0h
        _emit 006h
        _emit 08bh
        _emit 044h
        _emit 028h
        _emit 028h
        _emit 056h
        _emit 08bh
        _emit 070h
        _emit 004h
        _emit 089h
        _emit 031h
        _emit 08bh
        _emit 070h
        _emit 008h
        _emit 089h
        _emit 071h
        _emit 004h
        _emit 08bh
        _emit 070h
        _emit 00ch
        _emit 089h
        _emit 071h
        _emit 008h
        _emit 08bh
        _emit 070h
        _emit 004h
        _emit 089h
        _emit 071h
        _emit 00ch
        _emit 08bh
        _emit 070h
        _emit 008h
        _emit 08bh
        _emit 040h
        _emit 00ch
        _emit 089h
        _emit 041h
        _emit 014h
        _emit 08bh
        _emit 041h
        _emit 020h
        _emit 085h
        _emit 0c0h
        _emit 089h
        _emit 071h
        _emit 010h
        _emit 00fh
        _emit 08eh
        _emit 0aah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 053h
        _emit 057h
        _emit 08bh
        _emit 0dah
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 00fh
        _emit 0bfh
        _emit 003h
        _emit 0c1h
        _emit 0e0h
        _emit 006h
        _emit 003h
        _emit 0c5h
        _emit 08dh
        _emit 070h
        _emit 028h
        _emit 0bfh
        _emit 003h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 016h
        _emit 0d9h
        _emit 042h
        _emit 004h
        _emit 0d8h
        _emit 019h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 007h
        _emit 08bh
        _emit 042h
        _emit 004h
        _emit 089h
        _emit 001h
        _emit 0ebh
        _emit 013h
        _emit 0d9h
        _emit 042h
        _emit 004h
        _emit 0d8h
        _emit 059h
        _emit 00ch
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 006h
        _emit 08bh
        _emit 042h
        _emit 004h
        _emit 089h
        _emit 041h
        _emit 00ch
        _emit 0d9h
        _emit 042h
        _emit 008h
        _emit 0d8h
        _emit 059h
        _emit 004h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 008h
        _emit 08bh
        _emit 042h
        _emit 008h
        _emit 089h
        _emit 041h
        _emit 004h
        _emit 0ebh
        _emit 013h
        _emit 0d9h
        _emit 042h
        _emit 008h
        _emit 0d8h
        _emit 059h
        _emit 010h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 006h
        _emit 08bh
        _emit 042h
        _emit 008h
        _emit 089h
        _emit 041h
        _emit 010h
        _emit 0d9h
        _emit 042h
        _emit 00ch
        _emit 0d8h
        _emit 059h
        _emit 008h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 008h
        _emit 08bh
        _emit 052h
        _emit 00ch
        _emit 089h
        _emit 051h
        _emit 008h
        _emit 0ebh
        _emit 013h
        _emit 0d9h
        _emit 042h
        _emit 00ch
        _emit 0d8h
        _emit 059h
        _emit 014h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 006h
        _emit 08bh
        _emit 042h
        _emit 00ch
        _emit 089h
        _emit 041h
        _emit 014h
        _emit 083h
        _emit 0c6h
        _emit 004h
        _emit 04fh
        _emit 075h
        _emit 082h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 083h
        _emit 0c3h
        _emit 002h
        _emit 048h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 00fh
        _emit 085h
        _emit 060h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 05fh
        _emit 05bh
        _emit 05eh
        _emit 05dh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x4186b0
TriNode* TriNode::Ctor()
{
    Init();
    return this;
}

// FUNCTION: RALLYTROPHY 0x4186c0
void TriNode::Dtor()
{
    fun_004bd000(idx);
    Free();
}

// FUNCTION: RALLYTROPHY 0x4186e0
bool TriNode::Build(TriMesh* m)
{
    int n = m->count;
    count = n;
    short* p = (short*)fun_004c1276(n * 2);
    idx = p;
    int i = 0;
    if (0 < n) {
        do {
            *p = (short)i;
            i++;
            p++;
        } while (i < n);
    }
    FitAll(m);
    return child != 0;
}

#define IN_BOX(t, s) \
    (!((t)->v[((s) >> 2) & 3]->x < mn[0]) && (t)->v[(s) & 3]->x <= mx[0] && \
     !((t)->v[((s) >> 6) & 3]->y < mn[1]) && (t)->v[((s) >> 4) & 3]->y <= mx[1] && \
     !((t)->v[((s) >> 10) & 3]->z < mn[2]) && (t)->v[((s) >> 8) & 3]->z <= mx[2])
#define IN_BOX_XZY(t, s) \
    (!((t)->v[((s) >> 2) & 3]->x < mn[0]) && (t)->v[(s) & 3]->x <= mx[0] && \
     !((t)->v[((s) >> 10) & 3]->z < mn[2]) && (t)->v[((s) >> 8) & 3]->z <= mx[2] && \
     !((t)->v[((s) >> 6) & 3]->y < mn[1]) && (t)->v[((s) & 0x30) >> 4]->y <= mx[1])

// NONMATCHING: RALLYTROPHY 0x418760
// reason: structure matches through the node tests; leaf loop keeps count/tri ptr in different stack slots/regs
// reason: first draft; unchecked against original
void TriCollector::Walk(TriNode* n)
{
    while (n->mn[0] <= mx[0]) {
        if (!(n->mn[1] <= mx[1])) return;
        if (!(n->mn[2] <= mx[2])) return;
        if (n->mx[0] < mn[0]) return;
        if (n->mx[1] < mn[1]) return;
        if (n->mx[2] < mn[2]) return;
        if (n->child == 0) {
            int c = n->count;
            if (c > 0) {
                short* ip = n->idx;
                do {
                    Tri* t = &DAT_00508500[*ip];
                    int s = t->sel;
                    if (t->id < limit && IN_BOX(t, s)) {
                        hits.push_back(t);
                    }
                    ip++;
                } while (--c != 0);
            }
            return;
        }
        Walk(n->child);
        n = n->sibling;
    }
}

// FUNCTION: RALLYTROPHY 0x418a70
// reason: first draft; unchecked against original
void TriCollector::Gather(TriMesh* m)
{
    TriNode* root = m->root;
    if (root != 0) {
        DAT_00508500 = m->tris;
        Walk(root->child);
        Walk(root->sibling);
        return;
    }
    Tri* t = m->tris;
    int c = m->count;
    for (; c != 0; c--) {
        if (t->id < limit && IN_BOX_XZY(t, t->sel)) {
            InsN(hits.end(), 1, &t);
        }
        t++;
    }
}

// NONMATCHING: RALLYTROPHY 0x4193a0
// reason: first draft; SEH/temp layout unchecked
float TriCollector::Closest(float r, float* pos, float* out, float* nrm)
{
    float px = pos[0], py = pos[1], pz = pos[2];
    float lx = px - r, ly = py - r, lz = pz - r;
    Tri* headT = 0;
    Tri* tail = 0;
    for (unsigned i = 0; hits.begin() != 0 && i < hits.size(); i++) {
        Tri* t = hits[i];
        if ((int)((px - t->cx) * (px - t->cx) + (py - t->cy) * (py - t->cy) + (pz - t->cz) * (pz - t->cz)) <=
            (int)((r + t->r) * (r + t->r))) {
            unsigned s = t->sel;
            if (lx <= t->v[(s >> 2) & 3]->x && t->v[s & 3]->x <= px + r &&
                ly <= t->v[(s >> 6) & 3]->y && t->v[(s >> 4) & 3]->y <= py + r &&
                lz <= t->v[(s >> 10) & 3]->z && t->v[(s & 0x300) >> 8]->z <= pz + r) {
                t->next = 0;
                if (headT != 0) {
                    tail->next = t;
                } else {
                    headT = t;
                }
                tail = t;
            }
        }
    }
    head = headT;
    float ret = 1.0f;
    float cp[3];
    int hit = -1;
    Tri* cur = headT;
    Tri* found = 0;
    while (cur != 0) {
        TVert* a = cur->v[0];
        float e1[3], e2[3];
        e1[0] = cur->v[1]->x - a->x; e1[1] = cur->v[1]->y - a->y; e1[2] = cur->v[1]->z - a->z;
        e2[0] = cur->v[2]->x - a->x; e2[1] = cur->v[2]->y - a->y; e2[2] = cur->v[2]->z - a->z;
        if (fun_004b3800(pos, &a->x, e1, e2, cp) <= r * r) {
            ret = 0.0f;
            hit = 0;
            found = cur;
            break;
        }
        cur = cur->next;
    }
    if (out != 0) {
        if (hit == -1) {
            out[0] = 0.0f; out[1] = 0.0f; out[2] = 0.0f;
        } else if (hit == 0) {
            out[0] = cp[0]; out[1] = cp[1]; out[2] = cp[2];
        }
        if (nrm != 0) {
            if (hit == 0) {
                nrm[0] = found->nx; nrm[1] = found->ny; nrm[2] = found->nz;
            } else {
                nrm[0] = 0.0f; nrm[1] = 0.0f; nrm[2] = 0.0f;
            }
        }
    }
    return ret;
}

// Shared contact test for 0x419770 / 0x419be0 on one candidate triangle; returns 1 on contact.
#define CONTACT_BODY(T) \
    do { \
        TVert* a = (T)->v[0]; \
        ScenePoint e1s, e2s, cps; float* e1 = &e1s.x; float* e2 = &e2s.x; float* cp = &cps.x; float tmp[3]; \
        e1[0] = (T)->v[1]->x - a->x; e1[1] = (T)->v[1]->y - a->y; e1[2] = (T)->v[1]->z - a->z; \
        e2[0] = (T)->v[2]->x - a->x; e2[1] = (T)->v[2]->y - a->y; e2[2] = (T)->v[2]->z - a->z; \
        if (fun_004b3800(pos, &a->x, e1, e2, cp) <= r * r) { \
            if (n == 0) { \
                if (b != 0) { b[0] = cp[0]; b[1] = cp[1]; b[2] = cp[2]; } \
                if (c != 0) { c[0] = (T)->nx; c[1] = (T)->ny; c[2] = (T)->nz; } \
                if (surf != 0) { *surf = (T)->id; } \
            } \
            float* d = ((V3Diff*)pos)->Sub(tmp, cp); \
            sx += d[0]; sy += d[1]; sz += d[2]; \
            n++; \
        } \
    } while (0)

// NONMATCHING: RALLYTROPHY 0x419770
// reason: first draft; SEH/temp layout unchecked
int TriCollector::Contact(float r, float* pos, float* a_, float* b, float* c, int* surf)
{
    float sx = 0.0f, sy = 0.0f, sz = 0.0f;
    int n = 0;
    float px = pos[0], py = pos[1], pz = pos[2];
    for (unsigned i = 0; hits.begin() != 0 && i < hits.size(); i++) {
        Tri* t = hits[i];
        if ((int)((px - t->cx) * (px - t->cx) + (pz - t->cz) * (pz - t->cz) + (py - t->cy) * (py - t->cy)) <=
            (int)((r + t->r) * (r + t->r))) {
            TVert* a = t->v[0];
            if (DAT_004e134c <= (pos[0] - a->x) * t->nx + (pos[1] - a->y) * t->ny + (pos[2] - a->z) * t->nz) {
                unsigned s = t->sel;
                if (px - r <= t->v[(s >> 2) & 3]->x && t->v[s & 3]->x <= px + r &&
                    py - r <= t->v[(s >> 6) & 3]->y && t->v[(s >> 4) & 3]->y <= py + r &&
                    pz - r <= t->v[(s & 0xc00) >> 10]->z && t->v[(s >> 8) & 3]->z <= pz + r) {
                    CONTACT_BODY(t);
                }
            }
        }
    }
    if (n != 0) {
        float k = DAT_004e1348 / (float)sqrt(sz * sz + sx * sx + sy * sy);
        sx = k * sx; sy = sy * k; sz = sz * k;
    }
    a_[0] = sx; a_[1] = sy; a_[2] = sz;
    return n;
}

// NONMATCHING: RALLYTROPHY 0x419be0
// reason: first draft; SEH/temp layout unchecked
int TriCollector::ContactList(float r, float* pos, float* a_, float* b, float* c, int* surf)
{
    float sx = 0.0f, sy = 0.0f, sz = 0.0f;
    int n = 0;
    Tri* t = head;
    if (t != 0) {
        do {
            if ((int)((pos[0] - t->cx) * (pos[0] - t->cx) + (pos[1] - t->cy) * (pos[1] - t->cy) +
                      (pos[2] - t->cz) * (pos[2] - t->cz)) <= (int)((r + t->r) * (r + t->r))) {
                TVert* a = t->v[0];
                if (DAT_004e134c <= (pos[0] - a->x) * t->nx + (pos[1] - a->y) * t->ny + (pos[2] - a->z) * t->nz) {
                    CONTACT_BODY(t);
                }
            }
            t = t->next;
        } while (t != 0);
        if (n != 0) {
            float k = DAT_004e1348 / (float)sqrt(sz * sz + sx * sx + sy * sy);
            sx = k * sx; sy = sy * k; sz = sz * k;
        }
    }
    a_[0] = sx; a_[1] = sy; a_[2] = sz;
    return n;
}
