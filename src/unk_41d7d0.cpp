// 0x41d7d0: body-vs-body collision (triangle model against triangle model). Types: include/unk_41d7d0.h.
#include "unk_41d7d0.h"
#include "unk_419f00.h"

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

#define RECORD() \
    { \
        Vec3 va = ((CPointVel*)this)->PointVel(hit); \
        Vec3 vb = ((CPointVel*)other)->PointVel(hit); \
        va.x = va.x + v1x; va.y = va.y + v1y; va.z = va.z + v1z; \
        vb.x = vb.x + v2x; vb.y = vb.y + v2y; vb.z = vb.z + v2z; \
        Vec3 dv(va.x - vb.x, va.y - vb.y, va.z - vb.z); \
        float dot = dv.x * A->n[0] + dv.y * A->n[1] + dv.z * A->n[2]; \
        if (*(unsigned int*)&dot & 0x80000000) { \
            c1->pt = hit; \
            c1->n.x = A->n[0]; c1->n.y = A->n[1]; c1->n.z = A->n[2]; \
            c1->tri = A; \
            c1->v.x = dv.x; c1->v.y = dv.y; c1->v.z = dv.z; \
            c1->v.x = f1 * c1->v.x; c1->v.y = f1 * c1->v.y; c1->v.z = f1 * c1->v.z; \
            B1(this)->cnt = B1(this)->cnt + 1; \
            c1++; \
        } \
        if (flag != 0) { \
            Vec3 nd = -dv; \
            float dot2 = nd.x * B->n[0] + nd.y * B->n[1] + nd.z * B->n[2]; \
            if (*(unsigned int*)&dot2 & 0x80000000) { \
                c2->tri = B; \
                c2->pt = hit; \
                c2->n.x = B->n[0]; c2->n.y = B->n[1]; c2->n.z = B->n[2]; \
                c2->v.x = nd.x; c2->v.y = nd.y; c2->v.z = nd.z; \
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
int U40A850Body::FUN_0041d7d0(U40A850Body* other, int flag)
{
    float* bx = (float*)box;
    if (bx == 0) {
        return 0;
    }
    Plane planes[6];
    Vec3& pos_ = *(Vec3*)&this->pos[0];
    Vec3& row0 = *(Vec3*)&this->mat[0][0];
    Vec3& row1 = *(Vec3*)&this->mat[1][0];
    Vec3& row2 = *(Vec3*)&this->mat[2][0];
    float cc[3];
    XformT(cc, bx + 7, &this->mat[0][0], this->pos);
    float cx = cc[0], cy = cc[1], cz = cc[2];
    float rad = bx[10];

    // six box planes: +z/-z, +x/-x, +y/-y (box +0xc/+0x18, +4/+0x10, +8/+0x14)
    {
        Vec3 p = bx[3] * row2 + pos_;
        planes[0].n[0] = row2.x; planes[0].n[1] = row2.y; planes[0].n[2] = row2.z;
        planes[0].d = planes[0].n[1] * p.y + planes[0].n[2] * p.z + planes[0].n[0] * p.x;
    }
    {
        Vec3 p = bx[6] * row2 + pos_;
        Vec3 nn = -row2;
        planes[1].n[0] = nn.x; planes[1].n[1] = nn.y; planes[1].n[2] = nn.z;
        planes[1].d = planes[1].n[1] * p.y + planes[1].n[2] * p.z + planes[1].n[0] * p.x;
    }
    {
        Vec3 p = bx[1] * row0 + pos_;
        planes[2].n[0] = row0.x; planes[2].n[1] = row0.y; planes[2].n[2] = row0.z;
        planes[2].d = p.z * planes[2].n[2] + planes[2].n[1] * p.y + planes[2].n[0] * p.x;
    }
    {
        Vec3 p = bx[4] * row0 + pos_;
        Vec3 nn = -row0;
        planes[3].n[0] = nn.x; planes[3].n[1] = nn.y; planes[3].n[2] = nn.z;
        planes[3].d = p.y * planes[3].n[1] + p.z * planes[3].n[2] + planes[3].n[0] * p.x;
    }
    {
        Vec3 p = bx[2] * row1 + pos_;
        planes[4].n[0] = row1.x; planes[4].n[1] = row1.y; planes[4].n[2] = row1.z;
        planes[4].d = p.y * planes[4].n[1] + p.z * planes[4].n[2] + planes[4].n[0] * p.x;
    }
    {
        Vec3 p = bx[5] * row1 + pos_;
        Vec3 nn = -row1;
        planes[5].n[0] = nn.x; planes[5].n[1] = nn.y; planes[5].n[2] = nn.z;
        planes[5].d = p.y * planes[5].n[1] + p.z * planes[5].n[2] + planes[5].n[0] * p.x;
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
        float d2 = (cy - t->c[1]) * (cy - t->c[1]) + (cz - t->c[2]) * (cz - t->c[2]) + (cx - t->c[0]) * (cx - t->c[0]);
        if (d2 <= r2) {
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
    float v1x = FL(this, 0x1cc), v1y = FL(this, 0x1d0), v1z = FL(this, 0x1d4);
    float v2x = FL(other, 0x1cc), v2y = FL(other, 0x1d0), v2z = FL(other, 0x1d4);
    int hits = 0;
    int gen = 1;
    float f = DAT_004e1348 / (FL(this, 0x130) + FL(other, 0x130));
    float f1 = FL(this, 0x130) * f;
    float f2 = FL(other, 0x130) * f;
    CShape shA, shB;
    Vec3 tmp, hit;
    for (CTri* A = head; A != 0; A = A->next, gen++) {
        CTri* bt = (CTri*)tm->tris;
        float* a0 = (float*)(A->v[0] + 1);
        float* a1 = (float*)(A->v[1] + 1);
        float* a2 = (float*)(A->v[2] + 1);
        shA.Init(a0, a1, a2, A->n, A->d);
        for (int k = tm->nTris; k != 0; k--, bt++) {
            CTri* B = bt;
            float rr = (B->r + A->r) * (B->r + A->r);
            float dd = (A->c[1] - B->c[1]) * (A->c[1] - B->c[1]) + (A->c[2] - B->c[2]) * (A->c[2] - B->c[2]) +
                       (A->c[0] - B->c[0]) * (A->c[0] - B->c[0]);
            if (dd <= rr) {
                float* b0 = (float*)(B->v[0] + 1);
                float* b1 = (float*)(B->v[1] + 1);
                float* b2 = (float*)(B->v[2] + 1);
                if (shA.Test(b0, b1, b2, B->n, B->d)) {
                    if ((int)*B->e[0] != gen && shA.Edge(b0, b1, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        *B->e[0] = gen;
                        RECORD()
                    }
                    if ((int)*B->e[1] != gen && shA.Edge(b1, b2, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        *B->e[1] = gen;
                        RECORD()
                    }
                    if ((int)*B->e[2] != gen && shA.Edge(b2, b0, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        *B->e[2] = gen;
                        RECORD()
                    }
                    shB.Init(b0, b1, b2, B->n, B->d);
                    if (DAT_00508510.Test((unsigned int)B, (int)A->e[0]) == 0 &&
                        shB.Edge(a0, a1, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        DAT_00508510.Set((unsigned int)B, (int)A->e[0]);
                        RECORD()
                    }
                    if (DAT_00508510.Test((unsigned int)B, (int)A->e[1]) == 0 &&
                        shB.Edge(a1, a2, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        DAT_00508510.Set((unsigned int)B, (int)A->e[1]);
                        RECORD()
                    }
                    if (DAT_00508510.Test((unsigned int)B, (int)A->e[2]) == 0 &&
                        shB.Edge(a2, a0, (float*)&hit, (float*)&tmp) < DAT_004e1348) {
                        DAT_00508510.Set((unsigned int)B, (int)A->e[2]);
                        RECORD()
                    }
                }
            }
        }
    }
    return hits;
}
