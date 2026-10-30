// RenderBody::Update (0x4b9d30): fpatan-based shadow UV projection over a mesh list.
#include <math.h>
#include "unk_4b9d30.h"

extern float DAT_004e1398;
extern float DAT_004e1348;
extern float DAT_004e1be8;
extern float DAT_004e138c;
extern float DAT_004e1e88;
extern double DAT_004e2a20;
extern double DAT_004e2a18;

struct V3D { float x, y, z; V3D() {} ~V3D() {} };
struct V3Sub { float x, y, z; float* Sub(float* out, const float* o) const; }; // 0x414330

// NONMATCHING: RALLYTROPHY 0x4b9d30 (draft: the world-point transform (centre*matA + pos) is the Xform asm block (frame now ebp as original; 1563 vs 1750 bytes, ~1395 differ mostly from shifts). Remaining: EH state 3 and sub esp 0x224 (ours 2 / 0x210), the original reuses the arg slots [ebp+8..0x14] for temporaries (more asm), different dot-product order in the vertex loop)
void ShadowUpdater::Update(ShadowBody* body, ShadowBody* cam, const float* centre, float radius)
{
    Mat4Local invBody, invCam, prod;
    float* bm = body->matB;
    invBody.SetInverseOf(bm);
    invBody.m[12] = -(bm[14] * bm[2] + bm[13] * bm[1] + bm[0] * bm[12]);
    invBody.m[13] = -(bm[14] * bm[6] + bm[13] * bm[5] + bm[12] * bm[4]);
    invBody.m[14] = -(bm[14] * bm[10] + bm[13] * bm[9] + bm[12] * bm[8]);
    invCam.SetInverseOf(cam->matB);
    invCam.m[12] = -(cam->matB[2] * cam->matB[14] + cam->matB[1] * cam->matB[13] + cam->matB[0] * cam->matB[12]);
    invCam.m[13] = -(cam->matB[6] * cam->matB[14] + cam->matB[5] * cam->matB[13] + cam->matB[12] * cam->matB[4]);
    invCam.m[14] = -(cam->matB[10] * cam->matB[14] + cam->matB[9] * cam->matB[13] + cam->matB[8] * cam->matB[12]);
    prod.Multiply(bm, invCam);
    Mat4Local invProd;
    invProd.SetInverseOf(prod.m);
    float* p = prod.m;
    invProd.m[12] = -(p[12] * p[0] + p[2] * p[14] + p[1] * p[13]);
    invProd.m[13] = -(p[12] * p[4] + p[13] * p[5] + p[14] * p[6]);
    p[9] = p[9] * p[13];
    p[13] = 0.0f;
    p[10] = p[10] * p[14];
    p[14] = 0.0f;
    p[8] = p[8] * p[12];
    p[12] = 0.0f;
    invProd.m[14] = -(p[9] + p[10] + p[8]);

    float z = cam->matB[10];
    long double x = cam->matB[8];
    if (x * x + (long double)z * (long double)z < (long double)DAT_004e1398) {
        x = cam->matB[4];
        z = cam->matB[6];
    }
    float len = (float)sqrt(x * x + (long double)z * (long double)z);
    long double ang = atan2((double)(z / len), (double)(x / len));

    V3D w;
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
    V3D tmp;
    float* r = ((V3Sub*)&w)->Sub(&tmp.x, cam->pos);
    d[0] = r[0];
    d[1] = r[1];
    d[2] = r[2];
    float s = DAT_004e1348 / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[0] = s * d[0];
    d[1] = d[1] * s;
    d[2] = d[2] * s;
    float a[3];
    a[0] = d[2] * 1.0f - d[1] * 0.0f;
    a[1] = d[0] * 0.0f - d[2] * 0.0f;
    a[2] = d[1] * 0.0f - d[0] * 1.0f;
    s = DAT_004e1348 / sqrt(a[2] * a[2] + a[1] * a[1] + a[0] * a[0]);
    a[0] = a[0] * s;
    a[1] = a[1] * s;
    a[2] = a[2] * s;
    float u[3];
    u[0] = d[1] * a[2] - d[2] * a[1];
    u[1] = d[2] * a[0] - d[0] * a[2];
    u[2] = d[0] * a[1] - d[1] * a[0];
    float c0 = cam->pos[0], c2 = cam->pos[2], c1 = cam->pos[1];
    s = DAT_004e1348 / sqrt(u[1] * u[1] + u[2] * u[2] + u[0] * u[0]);
    u[0] = s * u[0];
    u[1] = u[1] * s;
    u[2] = u[2] * s;
    Mat4Local invAxes;
    invAxes.SetInverseOf(a);
    invAxes.m[12] = -(a[0] * c0 + a[2] * c2 + a[1] * c1);
    invAxes.m[13] = -(u[2] * c1 + u[1] * c2 + u[0] * c0);
    invAxes.m[14] = -(d[2] * c1 + d[1] * c2 + d[0] * c0);
    Mat4Local clip;
    clip.Multiply(bm, invAxes);

    float k = DAT_004e1be8 / radius;
    float q[16];
    for (int j = 0; j < 16; j++)
        q[j] = clip.m[j];
    float f7 = clip.m[0] * k * DAT_004e138c;
    float f6 = clip.m[4] * k * DAT_004e138c;
    float f5 = clip.m[8] * k * DAT_004e138c;
    float f4 = clip.m[12] * k * DAT_004e138c;
    q[0] = q[0] * DAT_004e1e88;
    q[1] = q[1] * DAT_004e1be8;
    q[4] = q[4] * DAT_004e1e88;
    q[5] = q[5] * DAT_004e1be8;
    q[8] = q[8] * DAT_004e1e88;
    q[9] = q[9] * DAT_004e1be8;
    float f8 = DAT_004e138c - clip.m[13] * k;
    for (unsigned i = 0; !meshes.empty() && i < (unsigned)meshes.size(); i++) {
        ShadowVertex* v = meshes[i]->verts;
        for (unsigned n = meshes[i]->count; n != 0; n--) {
            v->u = q[4] * v->normal[1] + q[8] * v->normal[2] + q[0] * v->normal[0] + f7 * v->pos[0] +
                   f6 * v->pos[1] + f5 * v->pos[2] + f4 + (float)((ang + DAT_004e2a20) * DAT_004e2a18);
            v->v = (f8 - (clip.m[1] * k * v->pos[0] + clip.m[5] * k * v->pos[1] + clip.m[9] * k * v->pos[2])) -
                   (q[5] * v->normal[1] + q[9] * v->normal[2] + q[1] * v->normal[0]);
            v++;
        }
    }
}
