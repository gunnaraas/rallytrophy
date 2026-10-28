// Collision model helpers (0x419f00, 0x41b190-0x41b990).
#include <string>
#include "unk_419f00.h"
#include <math.h>

extern "C" void fun_004bd000(void* p);
extern float DAT_004e1348;
struct ColSrc { int f[11]; char flag; };

// FUNCTION: RALLYTROPHY 0x419f00
// SYMBOL: ??HScenePoint@@QBE?AU0@ABU0@@Z
ScenePoint ScenePoint::operator+(const ScenePoint& other) const
{
    return ScenePoint(x + other.x, y + other.y, z + other.z);
}

// FUNCTION: RALLYTROPHY 0x41b190
ColModel::~ColModel()
{
    if (verts) delete[] verts;
    if (wverts) delete[] wverts;
    if (tris) delete[] tris;
    delete[] ints;
    if (centers) delete[] centers;
    if (root) {
        root->Dtor();
        fun_004bd000(root);
    }
}

// FUNCTION: RALLYTROPHY 0x41b250
void ColModel::ClearInts()
{
    int* p = ints;
    int n = nInts;
    if (0 < n) {
        for (; n != 0; n--) *p++ = 0;
    }
}

// point * M (rows at stride 16, translation at +48) into result, a hand-written x87 block in the original.
inline void ColTransform(float* result, const float* point, const float* matrix)
{
    __asm mov ecx, point
    __asm mov edx, matrix
    __asm mov eax, result
    __asm fld dword ptr [ecx]
    __asm fmul dword ptr [edx]
    __asm fld dword ptr [ecx]
    __asm fmul dword ptr [edx+4]
    __asm fld dword ptr [ecx]
    __asm fmul dword ptr [edx+8]
    __asm fld dword ptr [ecx+4]
    __asm fmul dword ptr [edx+16]
    __asm fld dword ptr [ecx+4]
    __asm fmul dword ptr [edx+20]
    __asm fld dword ptr [ecx+4]
    __asm fmul dword ptr [edx+24]
    __asm fxch st(2)
    __asm faddp st(5), st
    __asm faddp st(3), st
    __asm faddp st(1), st
    __asm fld dword ptr [ecx+8]
    __asm fmul dword ptr [edx+32]
    __asm fld dword ptr [ecx+8]
    __asm fmul dword ptr [edx+36]
    __asm fld dword ptr [ecx+8]
    __asm fmul dword ptr [edx+40]
    __asm fxch st(2)
    __asm faddp st(5), st
    __asm faddp st(3), st
    __asm faddp st(1), st
    __asm fxch st(2)
    __asm fadd dword ptr [edx+48]
    __asm fxch st(1)
    __asm fadd dword ptr [edx+52]
    __asm fxch st(2)
    __asm fadd dword ptr [edx+56]
    __asm fxch st(1)
    __asm fstp dword ptr [eax]
    __asm fstp dword ptr [eax+8]
    __asm fstp dword ptr [eax+4]
}

inline void ColCross(const float* ca, const float* cb, float* cresult)
{
    __asm mov ecx, ca
    __asm mov edx, cb
    __asm mov eax, cresult
    __asm fld dword ptr [ecx+4]
    __asm fmul dword ptr [edx+8]
    __asm fld dword ptr [ecx+8]
    __asm fmul dword ptr [edx]
    __asm fld dword ptr [ecx]
    __asm fmul dword ptr [edx+4]
    __asm fld dword ptr [ecx+8]
    __asm fmul dword ptr [edx+4]
    __asm fld dword ptr [ecx]
    __asm fmul dword ptr [edx+8]
    __asm fld dword ptr [ecx+4]
    __asm fmul dword ptr [edx]
    __asm fxch st(2)
    __asm fsubp st(5), st
    __asm fsubp st(3), st
    __asm fsubp st(1), st
    __asm fxch st(2)
    __asm fstp dword ptr [eax]
    __asm fstp dword ptr [eax+4]
    __asm fstp dword ptr [eax+8]
}

struct ColVec {
    float x, y, z;
    void operator-=(const ColVec& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    void operator*=(float k)
    {
        x = k * x;
        y = k * y;
        z = k * z;
    }
    float Length() const
    {
        float a = x, b = y, c = z;
        return (float)sqrt(a * a + b * b + c * c);
    }
    float Dot(const ColVec& v) const { return x * v.x + y * v.y + z * v.z; }
};

// NONMATCHING: RALLYTROPHY 0x41b270
// A named plane vertex and reversed Dot receiver reduce the best candidate from 696/434 to 698/322 bytes/differences;
// the Dot expression now loads vertex components before multiplying by the normal, as in the target. Remaining
// differences are mostly frame and register allocation; pointer and loop-counter register hints did not change output.
void ColModel::Transform(float* m, float* t)
{
    float M[16] = { m[0], m[1], m[2], 0.0f, m[4], m[5], m[6], 0.0f, m[8], m[9], m[10], 0.0f, t[0], t[1], t[2], 1.0f };
    int n = nVerts;
    if (n != 0) {
        ColVert* s = verts;
        ColVert* d = wverts;
        do {
            ColTransform(&d->x, &s->x, M);
            s++;
            d++;
        } while (--n != 0);
    }
    ColTri* tr = tris;
    ColLocal* c = centers;
    for (int i = nTris; i != 0; i--) {
        ColVert** v = (ColVert**)((char*)tr + 0x28);
        ColVert* const v0 = v[0];
        ColVec e1 = *(ColVec*)&v[1]->x;
        e1 -= *(ColVec*)&v0->x;
        ColVec e2 = *(ColVec*)&v[2]->x;
        e2 -= *(ColVec*)&v0->x;
        ColCross(&e1.x, &e2.x, &tr->c[0] + 6);
        *((ColVec*)tr + 2) *= DAT_004e1348 / ((ColVec*)tr + 2)->Length();
        ((float*)tr)[9] = (*((ColVec*)tr + 2)).Dot(*(ColVec*)&v0->x);
        ColTransform((float*)tr, &c->x, M);
        tr++;
        c++;
    }
}

// FUNCTION: RALLYTROPHY 0x41b530
void ColModel::FreeVerts()
{
    if (verts) delete[] verts;
    if (centers) delete[] centers;
    verts = 0;
    centers = 0;
}

#define AXIS(o, minv, maxv, mini, maxi) \
    { \
        float lo = ((float*)v0)[o]; \
        float hi = lo; \
        maxi = 0; \
        mini = 0; \
        if (lo <= ((float*)v1)[o]) { \
            if (hi < ((float*)v1)[o]) { hi = ((float*)v1)[o]; maxi = 1; } \
        } else { \
            mini = 1; \
            lo = ((float*)v1)[o]; \
        } \
        if (lo <= ((float*)v2)[o]) { \
            if (hi < ((float*)v2)[o]) maxi = 2; \
        } else { \
            mini = 2; \
        } \
    }

// NONMATCHING: RALLYTROPHY 0x41b590
// Current candidate is 385 bytes vs. 412; entry loads, frame setup and axis-selection code still differ substantially.
void ColModel::ComputeSel()
{
    int n = nTris;
    if (n != 0) {
        ColTri* t = tris;
        do {
            TVert** v = (TVert**)((char*)t + 0x28);
            TVert* v0 = v[0];
            TVert* v1 = v[1];
            TVert* v2 = v[2];
            unsigned short minx, miny;
            short maxx, maxy;
            unsigned char minz, maxz;
            AXIS(1, lox, hix, minx, maxx)
            AXIS(2, loy, hiy, miny, maxy)
            AXIS(3, loz, hiz, minz, maxz)
            *(unsigned short*)((char*)t + 0x12) =
                (unsigned short)(unsigned char)(maxz << 2 | minz) << 8 | maxx << 2 | minx | (maxy * 4 | miny) << 4;
            t++;
            n--;
        } while (n != 0);
    }
}

// FUNCTION: RALLYTROPHY 0x41b730
void ColModel::BuildTree(int minTris)
{
    if (nTris >= minTris) {
        root = new ColNode;
        if (!root->Build((TriMesh*)this)) {
            if (root) {
                root->Dtor();
                fun_004bd000(root);
            }
            root = 0;
        }
    }
}

// NONMATCHING: RALLYTROPHY 0x41b7f0
// reason: only the flag-byte store is scheduled after the mesh lookup args instead of before (same bytes otherwise)
// SYMBOL: ??0ColHolder@@QAE@PAUColOwner@@H@Z
ColHolder::ColHolder(ColOwner* o, int x) : flag(0)
{
    ColSrc* s = (ColSrc*)o->meshes.At(0);
    model.p = new ColModel(s, x);
    f[0] = s->f[0]; f[1] = s->f[1]; f[2] = s->f[2]; f[3] = s->f[3];
    f[4] = s->f[4]; f[5] = s->f[5]; f[6] = s->f[6]; f[7] = s->f[7];
    f[8] = s->f[8]; f[9] = s->f[9]; f[10] = s->f[10];
    flag = s->flag;
}

// NONMATCHING: RALLYTROPHY 0x41b8c0
// reason: only the flag-byte store is scheduled after the mesh lookup args instead of before (same bytes otherwise)
// SYMBOL: ??0ColHolder@@QAE@PAUColOwner@@HH@Z
ColHolder::ColHolder(ColOwner* o, int a, int b) : flag(0)
{
    ColSrc* s = (ColSrc*)o->meshes.At(0);
    model.p = new ColModel(s, a, b);
    f[0] = s->f[0]; f[1] = s->f[1]; f[2] = s->f[2]; f[3] = s->f[3];
    f[4] = s->f[4]; f[5] = s->f[5]; f[6] = s->f[6]; f[7] = s->f[7];
    f[8] = s->f[8]; f[9] = s->f[9]; f[10] = s->f[10];
    flag = s->flag;
}

// FUNCTION: RALLYTROPHY 0x41b990
ColPtr::~ColPtr()
{
    delete p;
}
