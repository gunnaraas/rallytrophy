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
// Tried 2026-10-31 re-pass (11->1 insn): vertex loop as plain `for (k = n; k != 0; k--)` (fixes je/setup), Dot arg v[0] re-derived from tr+0x28,
// Length as local c=z,b=y,a=x via `ColVec* q`, e1 from `float* p1 = &v[1]->x` (reproduces the dead add eax,4); only `dec ebx` still sits
// before the two [ebp+8]/[ebp+0xc] pointer spills instead of after. Tried while/do/--i/i>0/unsigned/volatile c,tr,i/pointer-arith c++/tr++ order, permute -r 40 (no MATCH, best 1).
void ColModel::Transform(float* m, float* t)
{
    float M[16] = { m[0], m[1], m[2], 0.0f, m[4], m[5], m[6], 0.0f, m[8], m[9], m[10], 0.0f, t[0], t[1], t[2], 1.0f };
    int n = nVerts;
    ColVert* d = wverts;
    ColVert* s = verts;
    for (int k = n; k != 0; k--) {
        {
            ColTransform(&d->x, &s->x, M);
            s++;
            d++;
        }
    }
    ColTri* tr = tris;
    ColLocal* c = centers;
    for (int i = nTris; i != 0; i--) {
        ColVert** v = (ColVert**)((char*)tr + 0x28);
        float* p1 = &v[1]->x;
        ColVec e1 = { p1[0], p1[1], p1[2] };
        e1 -= *(ColVec*)&v[0]->x;
        ColVec e2 = *(ColVec*)&v[2]->x;
        e2 -= *(ColVec*)&v[0]->x;
        ColCross(&e1.x, &e2.x, &tr->c[0] + 6);
        ColVec* q = (ColVec*)tr + 2;
        {
            float c = q->z, b = q->y, a = q->x;
            *q *= DAT_004e1348 / (float)sqrt(a * a + b * b + c * c);
        }
        ((float*)tr)[9] = (*((ColVec*)tr + 2)).Dot(*(ColVec*)&((ColVert**)((char*)tr + 0x28))[0]->x);
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
        lo = ((float*)v0)[o]; \
        hi = ((float*)v0)[o]; \
        maxi = 0; \
        mini = 0; \
        if (lo > ((float*)v1)[o]) { \
            mini = 1; \
            lo = ((float*)v1)[o]; \
        } else { \
            if (((float*)v1)[o] > hi) { hi = ((float*)v1)[o]; maxi = 1; } \
        } \
        if (lo > ((float*)v[1])[o]) { \
            mini = 2; \
        } else { \
            if (((float*)v[1])[o] > hi) maxi = 2; \
        } \
    }

// NONMATCHING: RALLYTROPHY 0x41b590
// Tried 2026-10-30 night: swapped compare shape (`lo > v1` else-arm, `v1 > hi`), hi loaded separately from lo, shared lo/hi locals, int mins/maxes, partial result kept in maxx, pointer-based loop, v2 re-read from v[]; 115->61 insns differ; still differs: original keeps the tri pointer in ebp with the counter spilled, ours spills the pointer; x/y/z register reuse (esi/edi/ebx) differs.
void ColModel::ComputeSel()
{
    ColTri* t = tris;
    for (int n = nTris; n != 0; n--) {
        TVert** v = (TVert**)((char*)t + 0x2c);
        TVert* v0 = v[-1];
        TVert* v1 = v[0];
        int minx, miny, maxx, maxy;
        int minz, maxz;
        float lo, hi;
        AXIS(1, lox, hix, minx, maxx)
        maxx = maxx << 2 | minx;
        AXIS(2, loy, hiy, miny, maxy)
        maxx |= (maxy * 4 | miny) << 4;
        AXIS(3, loz, hiz, minz, maxz)
        *(unsigned short*)((char*)v - 0x1a) = (unsigned short)(unsigned char)(maxz << 2 | minz) << 8 | maxx;
        t++;
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
// Volatile `o` plus the local alias moves the flag store ahead of the owner load and reproduces the target's EAX/LEA
// owner+0x124 setup (23→22 differences). Only the EH-state zero remains scheduled before, rather than after, that setup.
// Tried 2026-10-30 night: init-list vs body flag=0, local/volatile owner, model() init, split new, At via ColMeshList*; body flag=0 puts the EH-zero correctly but sinks flag store below it, init-list/volatile keep flag first but hoist EH-zero above flag store (1 insn either way).
// Tried 2026-10-31 re-pass (also applies to 0x41b8c0): ColPtrBase()/ColPtr() inline ctors, model() in init list, volatile/non-volatile owner and this, ColMeshList* local, volatile flag store, dummy int param copies, permute -r 40 (no MATCH, best 1 insn); EH state-0 store always lands before the flag store, original sinks it to just before the At call.
// SYMBOL: ??0ColHolder@@QAE@PAUColOwner@@H@Z
ColHolder::ColHolder(ColOwner* volatile o, int x) : flag(0)
{
    ColOwner* owner = o;
    ColSrc* s = (ColSrc*)owner->meshes.At(0);
    model.p = new ColModel(s, x);
    f[0] = s->f[0]; f[1] = s->f[1]; f[2] = s->f[2]; f[3] = s->f[3];
    f[4] = s->f[4]; f[5] = s->f[5]; f[6] = s->f[6]; f[7] = s->f[7];
    f[8] = s->f[8]; f[9] = s->f[9]; f[10] = s->f[10];
    flag = s->flag;
}

// NONMATCHING: RALLYTROPHY 0x41b8c0
// Tried 2026-10-30 night: volatile flag store in body (plain or volatile o), comma-expr flag=0 inside At arg, volatile-load-before-store, volatile local; EH zero is set at model position so any volatile store/load pins it above flag store, plain flag=0 sinks flag below EH store (1 insn each way, no scheduling barrier fixes it).
// The volatile owner pointer plus local alias gives the same 23→22 improvement as the one-argument constructor.
// SYMBOL: ??0ColHolder@@QAE@PAUColOwner@@HH@Z
ColHolder::ColHolder(ColOwner* volatile o, int a, int b) : flag(0)
{
    ColOwner* owner = o;
    ColSrc* s = (ColSrc*)owner->meshes.At(0);
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
