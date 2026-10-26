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

// Inline matrix * point (row-vector convention, translation in M[12..14]).
static inline void Xf(const float* M, const float* in, float* out)
{
    float x = in[0] * M[0] + in[1] * M[4] + in[2] * M[8] + M[12];
    float y = in[0] * M[1] + in[1] * M[5] + in[2] * M[9] + M[13];
    float z = in[0] * M[2] + in[1] * M[6] + in[2] * M[10] + M[14];
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

// NONMATCHING: RALLYTROPHY 0x41b270
// reason: original inlines a 4x4 Matrix*Vec3 helper (ebp frame, pointer locals spilled); ours computes the same results with a different schedule
void ColModel::Transform(float* m, float* t)
{
    float M[16] = { m[0], m[1], m[2], 0.0f, m[4], m[5], m[6], 0.0f, m[8], m[9], m[10], 0.0f, t[0], t[1], t[2], 1.0f };
    if (nVerts != 0) {
        float* s = &verts->x;
        float* d = &wverts->x;
        int n = nVerts;
        do {
            Xf(M, s, d);
            s += 4;
            d += 4;
            n--;
        } while (n != 0);
    }
    ColTri* tr = tris;
    ColLocal* c = centers;
    for (int i = nTris; i != 0; i--) {
        TVert** v = (TVert**)((char*)tr + 0x28);
        float* f = (float*)tr;
        float e1x = v[1]->x - v[0]->x;
        float e1y = v[1]->y - v[0]->y;
        float e1z = v[1]->z - v[0]->z;
        float e2x = v[2]->x - v[0]->x;
        float e2y = v[2]->y - v[0]->y;
        float e2z = v[2]->z - v[0]->z;
        f[6] = e1y * e2z - e1z * e2y;
        f[7] = e1z * e2x - e1x * e2z;
        f[8] = e1x * e2y - e1y * e2x;
        float k = DAT_004e1348 / (float)sqrt(f[8] * f[8] + f[7] * f[7] + f[6] * f[6]);
        f[6] = k * f[6];
        f[7] = k * f[7];
        f[8] = k * f[8];
        f[9] = v[0]->x * f[6] + v[0]->y * f[7] + v[0]->z * f[8];
        Xf(M, &c->x, f);
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
// reason: original keeps the running max in a stack slot loaded separately from the min (FPU scheduling differs)
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
