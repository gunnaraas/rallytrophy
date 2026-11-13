// 0x41d7d0: body-vs-body collision (triangle model against triangle model). Types: include/unk_41d7d0.h.
#include "unk_41d7d0.h"
#include "unk_419f00.h"
#include "scene_object.h"

extern float DAT_004e1348;
extern CPairSet DAT_00508510;
extern unsigned int __cdecl FUN_0041ba60(const float* planes, const float* p);   // plane outcode

struct BodyTail {                      // contact bookkeeping at the end of a body
    char u0[0x234];
    ColModel** model;                  // 0x234
    CContact* contacts;                // 0x238
    int cnt;                           // 0x23c
};
#define B1(p) ((BodyTail*)(p))
#define FL(p, o) (*(float*)((char*)(p) + (o)))

// DOT2: the original chains dot2 z,y,x for the shA edges and x,z,y for the shB edges.
#define DOT2_ZYX float dot2 = dv.z * B->n[2]; dot2 = dv.y * B->n[1] + dot2; dot2 = dv.x * B->n[0] + dot2;
#define DOT2_XZY float dot2 = dv.x * B->n[0]; dot2 = dv.z * B->n[2] + dot2; dot2 = dv.y * B->n[1] + dot2;
#define RECORD(DOT2) \
    { \
        Vec3 va = ((CPointVel*)this)->PointVel(hit); \
        Vec3 vb = ((CPointVel*)other)->PointVel(hit); \
        va.x = va.x + v1.x; va.y = va.y + v1.y; va.z = va.z + v1.z; \
        vb.x = vb.x + v2.x; vb.y = vb.y + v2.y; vb.z = vb.z + v2.z; \
        Vec3 dv = va; dv.x = va.x - vb.x; dv.y = va.y - vb.y; dv.z = va.z - vb.z; \
        float dot = dv.z * An[2]; dot = dv.y * An[1] + dot; dot = dv.x * An[0] + dot; \
        if (*(unsigned int*)&dot & 0x80000000) { \
            *(int*)&c1->pt.x = *(int*)&hit.x; *(int*)&c1->pt.y = *(int*)&hit.y; *(int*)&c1->pt.z = *(int*)&hit.z; \
            c1->n.x = An[0]; c1->n.y = An[1]; c1->n.z = An[2]; \
            c1->tri = A; \
            *(int*)&c1->v.x = *(int*)&dv.x; *(int*)&c1->v.y = *(int*)&dv.y; *(int*)&c1->v.z = *(int*)&dv.z; \
            c1->v.x = f1 * c1->v.x; c1->v.y = f1 * c1->v.y; c1->v.z = f1 * c1->v.z; \
            B1(this)->cnt = B1(this)->cnt + 1; \
            c1++; \
        } \
        if (flag != 0) { \
            dv = -dv; \
            DOT2 \
            if (*(unsigned int*)&dot2 & 0x80000000) { \
                c2->tri = B; \
                c2->pt.x = hit.x; c2->pt.y = hit.y; c2->pt.z = hit.z; \
                c2->n.x = B->n[0]; c2->n.y = B->n[1]; c2->n.z = B->n[2]; \
                c2->v.x = dv.x; c2->v.y = dv.y; c2->v.z = dv.z; \
                c2->v.x = f2 * c2->v.x; c2->v.y = f2 * c2->v.y; c2->v.z = f2 * c2->v.z; \
                B1(other)->cnt = B1(other)->cnt + 1; \
                c2++; \
            } \
        } \
        hits++; \
    }

// Point times a 3x3 row-vector matrix (rows 0x10 apart) plus translation t, as hand-written x87 (inline assembler in the original).
inline void XformT(float* o, const float* p, const float* m, const float* t)
{
    __asm {
        mov ecx, p
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

// NONMATCHING: RALLYTROPHY 0x41d7d0
// NONMATCHING: centre-point Xform x87 block (box+0x1c through mat, pos) now inline asm like the original; frame matches; rest (plane build, triangle loop, register/stack allocation) differs (5502 of 6021 bytes, 5419 differing).
// Tried 2026-10-31: plane n/d via Vec3 view with the original d sum load order, Vec3 cc/p + 2 dead Vec3 (EH state 3), rad next to cc, cull compare as int bits (dx*dx+dz*dz+dy*dy) 1399->1348 insns; still differs: this/box register roles (orig ebx/esi), EH state-0 store before plane ctor, temps at -0xd0/-0xe0, loop induction vars, frame 0x284.
// Tried 2026-11-01 re-pass: x87 stream now identical to the original (532 ops): contact dots are z,y,x order, plane2 d is x,y,z, both cull sums are x,z,y chained, dd compare as int bits, dv built field-wise with dot chained, v1/v2 velocities copied as ints; 1348->1323 insns; still differs: this/box register roles, frame 0x284 vs 0x278, EH states, temps at -0xd0/-0xe0, loop induction vars.
// Pass 2026-11-13: this=ebx/box=esi via direct this->mat/pos paths (no row/pos aliases); dv = -dv in place (6 unary-minus temps like the original); A->n kept in An; contact v/pt copied int/member-wise (struct copies made &c->pt induction vars); no cx/cy/cz locals; tmp declared after q. 1348 -> 1058 insns differ, size 5700 -> 5842 of 6021. Still differs: An/B-pointer register roles (orig edi=An, ebx=c1, esi=c2, B pointers on stack), plane-stage load scheduling, frame 0x278 vs 0x284.
int U40A850Body::FUN_0041d7d0(U40A850Body* other, int flag)
{
    float* bx = (float*)box;
    if (bx == 0) {
        return 0;
    }
    Vec3 cc;
    float rad;
    Plane planes[6];
    ScenePoint p;
    Vec3 q;
    Vec3 tmp;
    XformT(&cc.x, bx + 7, &this->mat[0][0], this->pos);
        rad = bx[10];
    {
        p = *(ScenePoint*)&(bx[3] * (*(Vec3*)&this->mat[2][0])) + *(ScenePoint*)&this->pos[0];
        Vec3& N = *(Vec3*)planes[0].n;
        N = (*(Vec3*)&this->mat[2][0]);
        planes[0].d = N.y * p.y + N.z * p.z + N.x * p.x;
    }
    {
        p = *(ScenePoint*)&(bx[6] * (*(Vec3*)&this->mat[2][0])) + *(ScenePoint*)&this->pos[0];
        Vec3& N = *(Vec3*)planes[1].n;
        N = -(*(Vec3*)&this->mat[2][0]);
        planes[1].d = N.y * p.y + N.z * p.z + N.x * p.x;
    }
    {
        p = *(ScenePoint*)&(bx[1] * (*(Vec3*)&this->mat[0][0])) + *(ScenePoint*)&this->pos[0];
        Vec3& N = *(Vec3*)planes[2].n;
        N = (*(Vec3*)&this->mat[0][0]);
        planes[2].d = N.x * p.x + N.y * p.y + N.z * p.z;
    }
    {
        p = *(ScenePoint*)&(bx[4] * (*(Vec3*)&this->mat[0][0])) + *(ScenePoint*)&this->pos[0];
        Vec3& N = *(Vec3*)planes[3].n;
        N = -(*(Vec3*)&this->mat[0][0]);
        planes[3].d = N.y * p.y + N.z * p.z + N.x * p.x;
    }
    {
        p = *(ScenePoint*)&(bx[2] * (*(Vec3*)&this->mat[1][0])) + *(ScenePoint*)&this->pos[0];
        Vec3& N = *(Vec3*)planes[4].n;
        N = (*(Vec3*)&this->mat[1][0]);
        planes[4].d = N.y * p.y + N.z * p.z + N.x * p.x;
    }
    {
        p = *(ScenePoint*)&(bx[5] * (*(Vec3*)&this->mat[1][0])) + *(ScenePoint*)&this->pos[0];
        Vec3& N = *(Vec3*)planes[5].n;
        N = -(*(Vec3*)&this->mat[1][0]);
        planes[5].d = N.y * p.y + N.z * p.z + N.x * p.x;
    }

    // cull the other body's triangles against this body's box
    ColModel* om = *B1(other)->model;
    om->Transform(&FL(other, 0x20), &FL(other, 0x14));
    CTri* tris = (CTri*)om->tris;
    int ntris = om->nTris;
    int i;
    for (i = 0; i < ntris; i++) {
        *tris[i].v[0] = 0; *tris[i].v[1] = 0; *tris[i].v[2] = 0;
        *tris[i].e[0] = 0; *tris[i].e[1] = 0; *tris[i].e[2] = 0;
    }
    CTri* head = 0;
    CTri* tail = 0;
    for (i = 0; i < ntris; i++) {
        CTri* t = &tris[i];
        float r2 = (rad + t->r) * (rad + t->r);
        float dx = cc.x - t->c[0];
        float dy = cc.y - t->c[1];
        float dz = cc.z - t->c[2];
        float d2 = dx * dx; d2 = dz * dz + d2; d2 = dy * dy + d2;
        if (*(int*)&d2 <= *(int*)&r2) {
            unsigned int m0, m1, m2;
            if (*t->v[0] == 0) { m0 = FUN_0041ba60((float*)planes, (float*)(t->v[0] + 1)); *t->v[0] = m0 | 0x100; } else { m0 = *t->v[0] & 0x3f; }
            if (*t->v[1] == 0) { m1 = FUN_0041ba60((float*)planes, (float*)(t->v[1] + 1)); *t->v[1] = m1 | 0x100; } else { m1 = *t->v[1] & 0x3f; }
            if (*t->v[2] == 0) { m2 = FUN_0041ba60((float*)planes, (float*)(t->v[2] + 1)); *t->v[2] = m2 | 0x100; } else { m2 = *t->v[2] & 0x3f; }
            if ((m0 & m1 & m2) == 0) {
                t->next = 0;
                if (head != 0) {
                    tail->next = t;
                } else {
                    head = t;
                }
                tail = t;
            }
        }
    }
    if (head == 0) {
        return 0;
    }

    // this body's triangles against the surviving ones
    ColModel* tm = *B1(this)->model;
    tm->Transform(&FL(this, 0x20), &FL(this, 0x14));
    tm->ClearInts();
    DAT_00508510.Clear();
    CContact* c1 = B1(this)->contacts + B1(this)->cnt;
    CContact* c2 = B1(other)->contacts + B1(other)->cnt;
    Vec3 v2, v1;
    *(int*)&v1.x = *(int*)&FL(this, 0x1cc); *(int*)&v1.y = *(int*)&FL(this, 0x1d0); *(int*)&v1.z = *(int*)&FL(this, 0x1d4);
    *(int*)&v2.x = *(int*)&FL(other, 0x1cc); *(int*)&v2.y = *(int*)&FL(other, 0x1d0); *(int*)&v2.z = *(int*)&FL(other, 0x1d4);
    int hits = 0;
    int gen = 1;
    float f = DAT_004e1348 / (FL(this, 0x130) + FL(other, 0x130));
    float f2 = FL(other, 0x130) * f;
    float f1 = FL(this, 0x130) * f;
    CShape shA, shB;
    for (CTri* A = head; A != 0; A = A->next, gen++) {
        CTri* bt = (CTri*)tm->tris;
        float* An = A->n;
        float* a0 = (float*)(A->v[0] + 1);
        float* a1 = (float*)(A->v[1] + 1);
        float* a2 = (float*)(A->v[2] + 1);
        shA.Init(a0, a1, a2, An, A->d);
        for (int k = tm->nTris; k != 0; k--, bt++) {
            CTri* B = bt;
            float rr = (B->r + A->r) * (B->r + A->r);
            float ex = A->c[0] - B->c[0]; float ey = A->c[1] - B->c[1]; float ez = A->c[2] - B->c[2];
            float dd = ex * ex; dd = ez * ez + dd; dd = ey * ey + dd;
            if (*(int*)&dd <= *(int*)&rr) {
                float* b0 = (float*)(B->v[0] + 1);
                float* b1 = (float*)(B->v[1] + 1);
                float* b2 = (float*)(B->v[2] + 1);
                if (shA.Test(b0, b1, b2, B->n, B->d)) {
                    Vec3 hit;
                    if ((int)*B->e[0] != gen && shA.Edge(b0, b1, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        *B->e[0] = gen;
                        RECORD(DOT2_ZYX)
                    }
                    if ((int)*B->e[1] != gen && shA.Edge(b1, b2, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        *B->e[1] = gen;
                        RECORD(DOT2_ZYX)
                    }
                    if ((int)*B->e[2] != gen && shA.Edge(b2, b0, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        *B->e[2] = gen;
                        RECORD(DOT2_ZYX)
                    }
                    shB.Init(b0, b1, b2, B->n, B->d);
                    if (DAT_00508510.Test((unsigned int)B, (int)A->e[0]) == 0 &&
                        shB.Edge(a0, a1, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        DAT_00508510.Set((unsigned int)B, (int)A->e[0]);
                        RECORD(DOT2_XZY)
                    }
                    if (DAT_00508510.Test((unsigned int)B, (int)A->e[1]) == 0 &&
                        shB.Edge(a1, a2, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        DAT_00508510.Set((unsigned int)B, (int)A->e[1]);
                        RECORD(DOT2_XZY)
                    }
                    if (DAT_00508510.Test((unsigned int)B, (int)A->e[2]) == 0 &&
                        shB.Edge(a2, a0, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        DAT_00508510.Set((unsigned int)B, (int)A->e[2]);
                        RECORD(DOT2_XZY)
                    }
                }
            }
        }
    }
    return hits;
}
