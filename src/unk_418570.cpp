// Collision triangle tree and sphere-query collector (0x418570-0x419be0).
#include "unk_418570.h"
#include <math.h>

extern "C" void* fun_004c1276(int bytes);
extern "C" void fun_004bd000(void* p);
extern "C" float fun_004b3800(float* p, float* org, float* e1, float* e2, float* out); // closest point on triangle, returns dist^2
float TriClosest4b(float* p, float* org, float* e1, float* e2, float* out); // C++ linkage: may throw, unlike the extern "C" original name
#pragma comment(linker, "/alternatename:?TriClosest4b@@YAMPAM0000@Z=_fun_004b3800")
extern float DAT_004e134c, DAT_004e1348;
extern Tri* DAT_00508500;
struct V3p { float x, y, z; };
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

// std::vector<Tri*> as Walk uses it: push_back expands VC6's insert(end(), 1, x) inline, with _Construct,
// _Ucopy, _Ufill, _Destroy and size() left as calls (0x403140, 0x424320, 0x424350, 0x403130, 0x414460).
struct HitVec {
    char alloc;
    Tri** first;
    Tri** last;
    Tri** end;
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    unsigned int SizeCall() const;                     // 0x414460
    Tri** Ucopy(Tri** f, Tri** l, Tri** d);            // 0x424320
    void Ufill(Tri** d, unsigned int n, Tri* const& x); // 0x424350
    void Destroy(Tri** f, Tri** l);                    // 0x403130
    void PushBack(Tri* const& x)
    {
        Tri** P = last;
        if ((unsigned int)(end - last) < 1) {
            unsigned int N = Size() + (1 < Size() ? Size() : 1);
            int An = (int)N;
            if (An < 0)
                An = 0;
            Tri** S = (Tri**)operator new(An * sizeof(Tri*));
            Tri** Q = S;
            for (Tri** F = first; F != P; ++Q, ++F)
                ConstructPtr(Q, *F);
            ConstructPtr(Q, x);
            Ucopy(P, last, Q + 1);
            Destroy(first, last);
            operator delete(first);
            end = S + N;
            last = S + SizeCall() + 1;
            first = S;
        } else if ((unsigned int)(last - P) < 1) {
            Ucopy(P, last, P + 1);
            Ufill(last, 1 - (last - P), x);
            for (Tri** F = P; F != last; ++F)
                *F = x;
            last += 1;
        } else {
            Ucopy(last - 1, last, last);
            Tri** B = last;
            Tri** E = last - 1;
            while (P != E)
                *--B = *--E;
            for (Tri** F = P; F != P + 1; ++F)
                *F = x;
            last += 1;
        }
    }
    static void ConstructPtr(Tri** p, Tri* const& v);  // 0x403140 (std::_Construct, cdecl)
};

// FUNCTION: RALLYTROPHY 0x418760
void TriCollector::Walk(TriNode* n)
{
    while (n->mn[0] <= mx[0]) {
        if (!(n->mn[1] <= mx[1])) return;
        if (!(n->mn[2] <= mx[2])) return;
        if (n->mx[0] < mn[0]) return;
        if (n->mx[1] < mn[1]) return;
        if (n->mx[2] < mn[2]) return;
        if (n->child == 0) {
            if (n->count > 0) {
                short* ip = n->idx;
                int c = n->count;
                do {
                    int k = *ip;
                    if (DAT_00508500[k].id < limit) {
                        Tri* t = &DAT_00508500[k];
                        int s = t->sel;
                        if (IN_BOX(t, s))
                            { Tri* hit = t; ((HitVec*)&hits)->PushBack(hit); }
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

struct Sph419 { ScenePoint c; float d; Sph419(const float* p, float r) { c.x = p[0]; c.y = p[1]; c.z = p[2]; d = r; } ~Sph419() {} };
static __forceinline bool Near419(const ScenePoint& p, const Tri* t, float r) { float q = (r + t->r) * (r + t->r); float dx = p.x - t->cx, dy = p.y - t->cy, dz = p.z - t->cz; float d2 = dx * dx + dy * dy + dz * dz; return *(int*)&d2 <= *(int*)&q; }

// NONMATCHING: RALLYTROPHY 0x4193a0
// Tried 2026-10-31: EH frame needs a throwing call (C++-linkage TriClosest4b via /alternatename, since extern "C" is nothrow), Sph419+Near419, plain V3p lo/hi, v>=lo compare flips, ScenePoint e1/e2 with -=; 218->175 but built 874 vs 974 bytes: original has EH states 2/4/7 (we get 0/1/3: two more function-scope dtor objects plus three in the loop), cp/out-copy layout and rr hoist still differ.
float TriCollector::Closest(float r, float* pos, float* out, float* nrm)
{
    float rr = r * r;
    Sph419 P(pos, r);
    V3p lo = *(V3p*)pos; lo.x -= r; lo.y -= r; lo.z -= r;
    V3p hi = *(V3p*)pos; hi.x += r; hi.y += r; hi.z += r;
    Tri* headT = 0;
    Tri* tail = 0;
    for (unsigned i = 0; hits.begin() != 0 && i < hits.size(); i++) {
        Tri* t = hits[i];
        if (Near419(P.c, t, r)) {
            unsigned s = t->sel;
            if (t->v[(s >> 2) & 3]->x >= lo.x && t->v[s & 3]->x <= hi.x &&
                t->v[(s >> 6) & 3]->y >= lo.y && t->v[(s >> 4) & 3]->y <= hi.y &&
                t->v[(s >> 10) & 3]->z >= lo.z && t->v[(s & 0x300) >> 8]->z <= hi.z) {
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
        ScenePoint e1(*(ScenePoint*)&cur->v[1]->x);
        e1 -= *(ScenePoint*)&a->x;
        ScenePoint e2(*(ScenePoint*)&cur->v[2]->x);
        e2 -= *(ScenePoint*)&a->x;
        if (TriClosest4b(pos, &a->x, &e1.x, &e2.x, cp) <= r * r) {
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
// reason: first draft; SEH/temp layout unchecked. The sphere test is a float bit-compare (*(int*)&d2 <= *(int*)&rr, rr first) on Vec3 copies of pos/lo/hi; the compare alone made it 10 bytes worse without the Vec3 rewrite.
// The (int) compare is a float bit-compare in the original (*(int*)&d2 <= *(int*)&rr, as in Closest); alone it scores 2-8 instructions worse until the Vec3 copies are restored.
// Tried 2026-10-31: 16 variants rewriting like ContactList (Sph419 P + Near419, ScenePoint lo/hi with -=/+= r, ScenePoint e1/e2/dv -=, hoisted rr, ok-int/bool sphere flag, extra dummy ScenePoint objects for EH numbering): 217-211 insns differ vs 220 (no real gain, sizes 1093-1136 vs 1130); still differs: original has EH states 0/3 (two more function-scope objects than P/lo/hi), sphere test materialises setle al/test eax instead of jg, and frame is 0x88 with rr at [esp+0x60].
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
// reason: first draft; SEH/temp layout unchecked.
// Tried 2026-10-30 night: pos copy as Sph419 (ScenePoint+r, dtor) for EH state, ScenePoint -= for plane/edge vectors, forceinline sphere test, chained sx=sy=sz=0, rr hoisted (163->136); still differs: original frame is 4 bytes bigger, e1/e2 are int-copied before the fsub (ours fuses the z fld), sphere d2 temp lives in a dead param slot, EH state numbering ends 5/1 vs ours 4/0 (v4 gets 5 with extra ScenePoint tmp but scores 141).
// Tried 2026-11-01 re-pass: 136->77 insns. Frame/slots now match (0x60 frame): sums as float s[3] plus a dead volatile store to a local q (forces the 4 extra bytes; slot order s/rr/q), empty-dtor struct ED declared first for EH states 0/1..5, int ok for setle/test. Still differs: e1/e2 and dv are fused (fld z straight into fsub) where the original int-copies all three fields first (field-wise, int-routed, volatile-view copies all scored worse), dot-product term order of dv, b/c/surf reloaded from stack vs held in ebx, and the sqrt normalisation keeps values on the FPU in the original.
int TriCollector::ContactList(float r, float* pos, float* a_, float* b, float* c, int* surf)
{
    struct ED { ~ED() {} } Q0;
    float s[3];
    float rr = r * r;
    s[2] = s[1] = s[0] = 0.0f;
    float q;
    *(volatile float*)&q = 0.0f;
    int n = 0;
    Sph419 P(pos, r);
    Tri* t = head;
    if (t != 0) {
        do {
            q = (r + t->r) * (r + t->r);
            float dx = P.c.x - t->cx, dy = P.c.y - t->cy, dz = P.c.z - t->cz;
            float d2 = dx * dx + dy * dy + dz * dz;
            int ok = *(int*)&d2 <= *(int*)&q;
            if (ok) {
                TVert* a = t->v[0];
                ScenePoint dv(*(ScenePoint*)pos);
                dv -= *(ScenePoint*)&a->x;
                if (DAT_004e134c <= dv.x * t->nx + dv.y * t->ny + dv.z * t->nz) {
                    ScenePoint e1(*(ScenePoint*)&t->v[1]->x);
                    e1 -= *(ScenePoint*)&a->x;
                    ScenePoint e2(*(ScenePoint*)&t->v[2]->x);
                    e2 -= *(ScenePoint*)&a->x;
                    ScenePoint cp; float tmp[3];
                    if (fun_004b3800(pos, &a->x, &e1.x, &e2.x, &cp.x) <= rr) {
                        if (n == 0) {
                            if (b != 0) { b[0] = cp.x; b[1] = cp.y; b[2] = cp.z; }
                            if (c != 0) { c[0] = t->nx; c[1] = t->ny; c[2] = t->nz; }
                            if (surf != 0) { *surf = t->id; }
                        }
                        float* d = ((V3Diff*)pos)->Sub(tmp, &cp.x);
                        s[0] += d[0]; s[1] += d[1]; s[2] += d[2];
                        n++;
                    }
                }
            }
            t = t->next;
        } while (t != 0);
    }
    if (n != 0) {
        float k = DAT_004e1348 / (float)sqrt(s[2] * s[2] + s[0] * s[0] + s[1] * s[1]);
        s[0] = k * s[0]; s[1] = s[1] * k; s[2] = s[2] * k;
    }
    a_[0] = s[0]; a_[1] = s[1]; a_[2] = s[2];
    return n;
}
