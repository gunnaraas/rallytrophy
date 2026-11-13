// Stage object-grid build (0x40fcc0).
#include <string>
#include "unk_40fcc0.h"

extern float DAT_00508460, DAT_00508464, DAT_00508468, DAT_0050846c;

struct GridRefPad;
struct ModelObj { void Refresh(); };               // original: FUN_004a6d30

// Hand-written x87 block (inline assembler in the original): c * 3x3 rows of m + t -> o.
inline void XformT(float* o, const float* c, const float* m, const float* t)
{
    __asm {
        mov ecx, c
        mov edx, m
        mov eax, o
        mov ebx, t
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x10]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x14]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x18]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x20]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x24]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x28]
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

// VC6 std::vector<int>::insert(end(), 1, x) with the unreachable arms folded away (see StageInsert in unk_40c280.cpp).
static __forceinline void GridInsert(GridPtrVec* v, int* P, const int& X)
{
    if ((unsigned int)(v->end - v->last) < 1) {
        unsigned int s = v->Size();
        int n = v->Size() + (1 < s ? v->Size() : 1);
        int* b = v->allocator.allocate(n, (void*)0);
        int* q = v->Ucopy(v->first, P, b);
        v->Ufill(q, 1, &X);
        v->Ucopy(P, v->last, q + 1);
        v->Destroy(v->first, v->last);
        GridFree(v->first);
        v->end = b + n;
        v->last = b + v->Size() + 1;
        v->first = b;
    } else if ((unsigned int)(v->last - P) < 1) {
        v->Ucopy(P, P, P + 1);
        v->Ufill(v->last, 1 - (v->last - P), &X);
        int* e = v->last;
        for (int* p = P; p != e; ++p)
            *p = X;
        v->last = e + 1;
    } else {
        v->Ucopy(v->last - 1, v->last, v->last);
        int* d = v->last;
        for (int* f = v->last - 1; P != f; )
            *--d = *--f;
        for (int* p = P; p != P + 1; ++p)
            *p = X;
        v->last += 1;
    }
}

#pragma optimize("a", off)
// NONMATCHING: RALLYTROPHY 0x40fcc0
// 1720 vs 1716 bytes: logic, calls and x87 blocks match; inline-asm helper argument stack slots and the loop-invariant ebx=0 register differ.
// Tried 2026-10-30 night: 292->187 via `float r2 = rr*rr; if (len < r2)` and local GridPtrVec* r/a for the two Ins calls; loop forms/goto/declaration order/pragma flags (a,g,w,t,y,s) no effect; still differs: original loops are unrotated (head test once, i/ebx=0 in regs) and GridInsert block layout/slots.
// Tried 2026-11-01 re-pass: ~25 variants (while/for-header/goto loop heads, volatile zero local for the ebx=0 compare, i/made/attached/ScenePoint/r,a declaration order, self pointer, ++i in progress expr, volatile flags, len-after-r2); none below 187 insns differ; still differs: loop-head layout (original unrotated, i in ecx, ebx=0 live) and GridInsert block layout.
// 2026-11-13 pass: 187 -> 87 insns differ (1722 -> 1709 bytes): loops as `while (1)` + break (unrotated), `++k` in progress expr, GridInsert std::max(1,s) order, e/P!=f/last=e+1 forms, per-loop v=&c->records..; left: ecx/eax regs for nodeFirst[k]->node, branch-3 `mov ecx,eax; add eax,-4` vs lea.
// original: FUN_0040fcc0
void StageGrid::Build()
{
    unsigned int i = 0;
    GridRef* made;
    while (1) {
        if (objFirst == 0 || i >= (unsigned int)(objLast - objFirst))
            break;
        SceneNode* o = objFirst[i];
        unsigned int fl = o->flags;
        if ((fl & 0x10) == 0) {
            if (o->type != 2 && o->type != 5)
                o->SetFlag04(1);
            if (o->type == 1)
                ((ModelObj*)o)->Refresh();
        }
        ScenePoint p;
        XformT(&p.x, &o->boundsA.c.x, o->local.m, &o->position.x);
        GridCell* cell = Cell(&p.x);
        int attached = 0;
        if ((fl & 0x20) == 0 && o->type != 2 && cell->nodeFirst != 0 && ((cell->nodeLast - cell->nodeFirst) != 0)) {
            SceneNode* f = (*cell->nodeFirst)->node;
            ScenePoint d;
            XformT(&d.x, &f->boundsA.c.x, f->local.m, &f->position.x);
            d.x -= p.x;
            d.y -= p.y;
            d.z -= p.z;
            float len = d.z * d.z + d.y * d.y + d.x * d.x;
            float rr = f->boundsA.d + o->boundsA.d;
            float r2 = rr * rr;
            if (len < r2) {
                o->AttachTo(f);
                attached = 1;
            }
        }
        if (!attached) {
            made = new GridRef(o);
            GridPtrVec* r = &cell->refs;
            GridPtrVec* a = &allRefs;
            r->Ins((GridRef**)r->last, 1, &made);
            a->Ins((GridRef**)a->last, 1, &made);
        }
        label->SetProgress((float)(int)(i + 1) / (float)(unsigned int)(objFirst == 0 ? 0 : objLast - objFirst) * DAT_0050846c + DAT_00508468);
        i++;
    }
    unsigned int k = 0;
    while (1) {
        if (nodeFirst == 0 || k >= (unsigned int)(nodeLast - nodeFirst))
            break;
        nodeFirst[k]->node->Slot3();
        label->SetProgress((float)(int)++k / (float)(unsigned int)(nodeFirst == 0 ? 0 : nodeLast - nodeFirst) * DAT_00508464 + DAT_00508460);
    }
    for (int j = 0; j < count48; j++) {
        BYTE* rec = records48 + j * 0x48;
        int x = (int)rec;
        GridCell* c = Cell((float*)(rec + 0x30));
        GridPtrVec* v = &c->records48;
        GridInsert(v, v->last, x);
    }
    for (int m = 0; m < count2c; m++) {
        BYTE* rec = records2c + m * 0x2c;
        int x = (int)rec;
        GridCell* c = Cell((float*)(rec + 0x14));
        GridPtrVec* v = &c->records2c;
        GridInsert(v, v->last, x);
    }
}
