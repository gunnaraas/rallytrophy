// Car body vs. triangle-list collision.
#include "unk_41ca70.h"

extern float DAT_004e1348;

inline void Xform(float* result, const float* point, const float* matrix, const float* trans)
{
    __asm {
        mov ecx, point
        mov edx, matrix
        mov eax, result
        mov ebx, trans
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+16]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+20]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+24]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+32]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+36]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+40]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fadd dword ptr [ebx]
        fxch st(1)
        fadd dword ptr [ebx+4]
        fxch st(2)
        fadd dword ptr [ebx+8]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax+8]
        fstp dword ptr [eax+4]
    }
}

#define NEG(f) (*(unsigned int*)&(f) & 0x80000000)

// original: FUN_0041ca70
// NONMATCHING: RALLYTROPHY 0x41ca70
// NONMATCHING: +19 bytes; frame is 12 bytes smaller (a Vec3 local missing), early-return epilogue and register allocation differ
int ColBody::Collide(TriCollector* q, int unused)
{
    nHits = 0;
    CBox* b = box;
    if (b == 0) {
        return 0;
    }
    Vec3 mn, mx;
    Vec3 ex = ax;
    Vec3 ey = ay;
    Vec3 ez = az;
    ex.x = (b->mx[0] - b->mn[0]) * ex.x;
    ex.y = (b->mx[0] - b->mn[0]) * ex.y;
    ex.z = (b->mx[0] - b->mn[0]) * ex.z;
    ey.x = (b->mx[1] - b->mn[1]) * ey.x;
    ey.y = (b->mx[1] - b->mn[1]) * ey.y;
    ey.z = (b->mx[1] - b->mn[1]) * ey.z;
    ez.x = (b->mx[2] - b->mn[2]) * ez.x;
    ez.y = (b->mx[2] - b->mn[2]) * ez.y;
    ez.z = (b->mx[2] - b->mn[2]) * ez.z;
    Xform(&mn.x, &b->mn[0], &ax.x, &pos.x);
    mx.x = mn.x + ex.x;
    if (NEG(ex.x)) { mx.x = mn.x; mn.x = mn.x + ex.x; }
    mx.y = mn.y + ex.y;
    if (NEG(ex.y)) { mx.y = mn.y; mn.y = mn.y + ex.y; }
    mx.z = mn.z + ex.z;
    if (NEG(ex.z)) { mx.z = mn.z; mn.z = mn.z + ex.z; }
    if (NEG(ey.x) == 0) mx.x = ey.x + mx.x; else mn.x = ey.x + mn.x;
    if (NEG(ey.y) == 0) mx.y = ey.y + mx.y; else mn.y = ey.y + mn.y;
    if (NEG(ey.z) == 0) mx.z = ey.z + mx.z; else mn.z = ey.z + mn.z;
    if (NEG(ez.x) == 0) mx.x = ez.x + mx.x; else mn.x = ez.x + mn.x;
    if (NEG(ez.y) == 0) mx.y = ez.y + mx.y; else mn.y = ez.y + mn.y;
    if (NEG(ez.z) == 0) mx.z = ez.z + mx.z; else mn.z = ez.z + mn.z;

    // keep the triangles whose box overlaps the body's box
    CTri* head = 0;
    CTri* tail = 0;
    CTri** it = (CTri**)q->hits.begin();
    CTri** end = (CTri**)q->hits.end();
    if (it != end) {
        do {
            CTri* t = *it;
            unsigned short s = t->sel;
            TVert** tv = (TVert**)t->v;
            if (mn.y <= tv[(s >> 6) & 3]->y && tv[(s >> 4) & 3]->y <= mx.y
                && mn.x <= tv[(s >> 2) & 3]->x && tv[s & 3]->x <= mx.x
                && mn.z <= tv[(s & 0xc00) >> 10]->z && tv[(s >> 8) & 3]->z <= mx.z) {
                *(int*)t->v[0] = 0;
                *(int*)t->v[2] = 0;
                *(int*)t->v[1] = 0;
                *t->e[1] = 0;
                *t->e[0] = 0;
                *t->e[2] = 0;
                t->next = 0;
                tail = t;
                if (head != 0) {
                    tail->next = t;
                } else {
                    head = t;
                }
            }
            it++;
        } while (it != end);
        if (head != 0) {
            CPlane pl[6];
            Vec3 pt;
            Vec3 sph;
            float sphR;
            Xform(&sph.x, &b->ctr[0], &ax.x, &pos.x);
            sphR = b->rad;
            pt = b->mn[2] * az + pos;
            pl[0].n = az;
            pl[0].d = pl[0].n.x * pt.x + pl[0].n.y * pt.y + pl[0].n.z * pt.z;
            pt = b->mx[2] * az + pos;
            pl[1].n = -az;
            pl[1].d = pl[1].n.x * pt.x + pl[1].n.y * pt.y + pl[1].n.z * pt.z;
            pt = b->mn[0] * ax + pos;
            pl[2].n = ax;
            pl[2].d = pl[2].n.x * pt.x + pl[2].n.y * pt.y + pl[2].n.z * pt.z;
            pt = b->mx[0] * ax + pos;
            pl[3].n = -ax;
            pl[3].d = pl[3].n.x * pt.x + pl[3].n.y * pt.y + pl[3].n.z * pt.z;
            pt = b->mn[1] * ay + pos;
            pl[4].n = ay;
            pl[4].d = pl[4].n.x * pt.x + pl[4].n.y * pt.y + pl[4].n.z * pt.z;
            pt = b->mx[1] * ay + pos;
            pl[5].n = -ay;
            pl[5].d = pl[5].n.x * pt.x + pl[5].n.y * pt.y + pl[5].n.z * pt.z;

            // keep the triangles not entirely outside one of the planes
            CTri* cur = head;
            head = 0;
            tail = 0;
            do {
                unsigned int c0 = *(unsigned int*)cur->v[0];
                if (c0 == 0) {
                    *(unsigned int*)cur->v[0] = CPMask::Mask(&pl[0].n.x, &cur->v[0]->x) | 0x100;
                } else {
                    c0 &= 0x3f;
                }
                unsigned int c1 = *(unsigned int*)cur->v[1];
                if (c1 == 0) {
                    *(unsigned int*)cur->v[1] = CPMask::Mask(&pl[0].n.x, &cur->v[1]->x) | 0x100;
                } else {
                    c1 &= 0x3f;
                }
                unsigned int c2 = *(unsigned int*)cur->v[2];
                if (c2 == 0) {
                    c2 = CPMask::Mask(&pl[0].n.x, &cur->v[2]->x);
                    *(unsigned int*)cur->v[2] = c2 | 0x100;
                } else {
                    c2 &= 0x3f;
                }
                CTri* next = cur->next;
                if ((c0 & c1 & c2) == 0) {
                    cur->next = 0;
                    tail = cur;
                    if (head != 0) {
                        tail->next = cur;
                    } else {
                        head = cur;
                    }
                }
                cur = next;
            } while (cur != 0);

            if (head != 0) {
                ColModel* model = box->model;
                model->Transform(&ax.x, &pos.x);
                model->ClearInts();
                DAT_00508510_.Clear();
                int result = 0;
                CTri* mt = (CTri*)model->tris;
                int nTris = model->nTris;
                CHit* hit = hits;
                int cnt = 0;
                int gen = 1;
                CTri* t = head;
                do {
                    const float* v0 = &t->v[0]->x;
                    const float* v1 = &t->v[1]->x;
                    const float* v2 = &t->v[2]->x;
                    Tri3 ta(v0, v1, v2, &t->n.x, t->d);
                    CTri* m = mt;
                    int n = nTris;
                    if (n != 0) {
                        do {
                            float rr = (m->r + t->r) * (m->r + t->r);
                            float dd = (t->c[0] - m->c[0]) * (t->c[0] - m->c[0])
                                + (t->c[1] - m->c[1]) * (t->c[1] - m->c[1])
                                + (t->c[2] - m->c[2]) * (t->c[2] - m->c[2]);
                            if (*(int*)&dd <= *(int*)&rr) {
                                const float* w0 = &m->v[0]->x;
                                const float* w1 = &m->v[1]->x;
                                const float* w2 = &m->v[2]->x;
                                if (ta.Overlap(w0, w1, w2, &m->n.x, m->d)) {
                                    Vec3 cp;
                                    Vec3 nrm;
                                    if (*m->e[0] != gen && ta.EdgeDist(w0, w1, &cp.x, &nrm.x) < DAT_004e1348) {
                                        *m->e[0] = gen;
                                        hit->tri = t;
                                        hit->pt = cp;
                                        hit->n = t->n;
                                        hit++;
                                        cnt++;
                                    }
                                    if (*m->e[1] != gen && ta.EdgeDist(w1, w2, &cp.x, &nrm.x) != DAT_004e1348) {
                                        *m->e[1] = gen;
                                        hit->tri = t;
                                        hit->pt = cp;
                                        hit->n = t->n;
                                        hit++;
                                        cnt++;
                                    }
                                    if (*m->e[2] != gen && ta.EdgeDist(w2, w0, &cp.x, &nrm.x) != DAT_004e1348) {
                                        *m->e[2] = gen;
                                        hit->tri = t;
                                        hit->pt = cp;
                                        hit->n = t->n;
                                        hit++;
                                        cnt++;
                                    }
                                    Tri3 tb(w0, w1, w2, &m->n.x, m->d);
                                    if (DAT_00508510_.Has(m, t->e[0]) == 0 && tb.EdgeDist(v0, v1, &cp.x, &nrm.x) < DAT_004e1348) {
                                        DAT_00508510_.Add(m, t->e[0]);
                                        hit->tri = t;
                                        hit->pt = cp;
                                        hit->n = t->n;
                                        hit++;
                                        cnt++;
                                    }
                                    if (DAT_00508510_.Has(m, t->e[1]) == 0 && tb.EdgeDist(v1, v2, &cp.x, &nrm.x) < DAT_004e1348) {
                                        DAT_00508510_.Add(m, t->e[1]);
                                        hit->tri = t;
                                        hit->pt = cp;
                                        hit->n = t->n;
                                        hit++;
                                        cnt++;
                                    }
                                    if (DAT_00508510_.Has(m, t->e[2]) == 0 && tb.EdgeDist(v2, v0, &cp.x, &nrm.x) < DAT_004e1348) {
                                        DAT_00508510_.Add(m, t->e[2]);
                                        hit->tri = t;
                                        hit->pt = cp;
                                        hit->n = t->n;
                                        hit++;
                                        cnt++;
                                    }
                                }
                            }
                            m++;
                            n--;
                        } while (n != 0);
                    }
                    t = t->next;
                    gen++;
                } while (t != 0);
                if (cnt != 0) {
                    nHits = cnt;
                    result = 2;
                }
                return result;
            }
        }
    }
    return 0;
}
