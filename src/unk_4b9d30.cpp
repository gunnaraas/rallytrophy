// RenderBody::Update (0x4b9d30): fpatan-based shadow UV projection over a mesh list.
#include <math.h>
#include "unk_4b9d30.h"
#include "x87_cross.h"

extern float DAT_004e1398;
extern float DAT_004e1348;
extern float DAT_004e1be8;
extern float DAT_004e138c;
extern float DAT_004e1e88;
extern double DAT_004e2a20;
extern double DAT_004e2a18;

struct V3D { float x, y, z; V3D() {} };
struct V3Sub { float x, y, z; float* Sub(float* out, const float* o) const; }; // 0x414330

// NONMATCHING: RALLYTROPHY 0x4b9d30 (draft: the world-point transform (centre*matA + pos) is the Xform asm block (frame now ebp as original; 1563 vs 1750 bytes, ~1395 differ mostly from shifts). Remaining: EH state 3 and sub esp 0x224 (ours 2 / 0x210), the original reuses the arg slots [ebp+8..0x14] for temporaries (more asm), different dot-product order in the vertex loop)
// Tried 2026-10-30 night: cam/body matrices via local pointers (cm), 4 early + 5 late dtor objects (V3E w/tmp/X5) gives sp 0x224, EH state 3 and 1748 bytes (326->316 insns; cm alone 308); operand order of the translation products ignores source order, matrix slots ignore declaration order (original: invBody -0x230, invProd -0x1f0, clip -0x1b0, invCam -0x170, invAxes -0x130, prod -0xf0); X5 candidates u/a/d/up all worse.
// Tried 2026-11-01 re-pass: vertex loop and pre-loop scaling rewritten to the original's fadd association (clip scaled in place after the q copy; u=(g3+g1)+(f4+ang), v=(f8-g2)-g4; a rounding fix, 316->319 insns only because of slot cascade); matrix declaration order (6 orders, incl. original slot order) and q[16] position make no difference to slots; temps/volatile on g2/g4 and bm-sum temp variants worse; 313-331 insns differ, too far for permute.
void ShadowUpdater::Update(ShadowBody* body, ShadowBody* cam, const float* centre, float radius)
{
    struct V3E { float x, y, z; V3E() {} ~V3E() {} };
    Mat4Local invBody, invCam, prod, invProd;
    float* bm = body->matB;
    invBody.SetInverseOf(bm);
    invBody.m[12] = -(bm[14] * bm[2] + bm[13] * bm[1] + bm[0] * bm[12]);
    invBody.m[13] = -(bm[14] * bm[6] + bm[13] * bm[5] + bm[12] * bm[4]);
    invBody.m[14] = -(bm[14] * bm[10] + bm[13] * bm[9] + bm[12] * bm[8]);
    float* cm = cam->matB;
    invCam.SetInverseOf(cm);
    invCam.m[12] = -(cm[2] * cm[14] + cm[1] * cm[13] + cm[0] * cm[12]);
    invCam.m[13] = -(cm[6] * cm[14] + cm[5] * cm[13] + cm[12] * cm[4]);
    invCam.m[14] = -(cm[10] * cm[14] + cm[9] * cm[13] + cm[8] * cm[12]);
    prod.Multiply(bm, invCam);
    invProd.SetInverseOf(prod.m);
    float* p = prod.m;
    invProd.m[12] = -(p[12] * p[0] + p[2] * p[14] + p[1] * p[13]);
    invProd.m[13] = -(p[12] * p[4] + p[13] * p[5] + p[14] * p[6]);
    float t = p[9] * p[13];
    t = p[10] * p[14] + t;
    t = p[8] * p[12] + t;
    invProd.m[14] = -t;

    float z = cm[10];
    long double x = cm[8];
    if (x * x + (long double)z * (long double)z < (long double)DAT_004e1398) {
        x = cm[4];
        z = cm[6];
    }
    float len = (float)sqrt(x * x + (long double)z * (long double)z);
    float ang = (float)((atan2((double)(z / len), (double)(x / len)) + DAT_004e2a20) * DAT_004e2a18);

    Mat4Local invAxes, clip;
    V3E w, tmp, X5;
    {
        float* mA = body->matA;
        float* mT = body->pos;
        float* mO = &w.x;
        const float* mP = centre;
        __asm {
            mov ecx, mP
            mov edx, mA
            mov eax, mO
            mov ebx, mT
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
    float d[3];
    float* r = ((V3Sub*)&w)->Sub(&tmp.x, cam->pos);
    d[0] = r[0];
    d[1] = r[1];
    d[2] = r[2];
    float s = DAT_004e1348 / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[0] = s * d[0];
    d[1] = d[1] * s;
    d[2] = d[2] * s;
    float up[3] = {0.0f, 1.0f, 0.0f};
    float a[3];
    X87Cross(a, up, d);
    s = DAT_004e1348 / sqrt(a[2] * a[2] + a[1] * a[1] + a[0] * a[0]);
    a[0] = a[0] * s;
    a[1] = a[1] * s;
    a[2] = a[2] * s;
    float u[3];
    X87Cross(u, d, a);
    float c0 = cam->pos[0], c2 = cam->pos[2], c1 = cam->pos[1];
    s = DAT_004e1348 / sqrt(u[1] * u[1] + u[2] * u[2] + u[0] * u[0]);
    u[0] = s * u[0];
    u[1] = u[1] * s;
    u[2] = u[2] * s;
    invAxes.SetInverseOf(a);
    invAxes.m[12] = -(a[0] * c0 + a[2] * c2 + a[1] * c1);
    invAxes.m[13] = -(u[2] * c1 + u[1] * c2 + u[0] * c0);
    invAxes.m[14] = -(d[2] * c1 + d[1] * c2 + d[0] * c0);
    clip.Multiply(bm, invAxes);

    float k = DAT_004e1be8 / radius;
    float q[16];
    for (int j = 0; j < 16; j++)
        q[j] = clip.m[j];
    float* cl = clip.m;
    cl[0] = cl[0] * k * DAT_004e138c;
    cl[1] = cl[1] * k;
    cl[4] = cl[4] * k * DAT_004e138c;
    cl[5] = cl[5] * k;
    cl[8] = cl[8] * k * DAT_004e138c;
    cl[9] = cl[9] * k;
    cl[12] = cl[12] * k * DAT_004e138c;
    cl[13] = cl[13] * k;
    q[0] = q[0] * DAT_004e1e88;
    q[1] = q[1] * DAT_004e1be8;
    q[4] = q[4] * DAT_004e1e88;
    q[5] = q[5] * DAT_004e1be8;
    q[8] = q[8] * DAT_004e1e88;
    q[9] = q[9] * DAT_004e1be8;
    float f8 = DAT_004e138c - cl[13];
    float f4 = cl[12] + ang;
    for (unsigned i = 0; !meshes.empty() && i < (unsigned)meshes.size(); i++) {
        ShadowVertex* v = meshes[i]->verts;
        for (unsigned n = meshes[i]->count; n != 0; n--) {
            float g1 = cl[8] * v->pos[2] + cl[4] * v->pos[1] + cl[0] * v->pos[0];
            float g2 = cl[9] * v->pos[2] + cl[5] * v->pos[1] + cl[1] * v->pos[0];
            float g3 = q[0] * v->normal[0] + q[8] * v->normal[2] + q[4] * v->normal[1];
            float g4 = q[1] * v->normal[0] + q[9] * v->normal[2] + q[5] * v->normal[1];
            float uu = g3 + g1; uu = uu + f4;
            float vv = f8 - g2; vv = vv - g4;
            v->u = uu; v->v = vv;
            v++;
        }
    }
}
