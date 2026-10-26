// Shadow caster hierarchy, view frustum, dynamic mesh (0x4ba440-0x4bb240).
#include <string>
#include "unk_4ba440.h"
#include <math.h>
#include <string.h>

// Hand-written x87 blocks (inline assembler in the original; the pointers live in stack locals).
// Point times a 4x4 row-vector matrix, translation in row 3.
inline void Xform(float* o, const float* p, const float* m)
{
    __asm {
        mov ecx, p
        mov edx, m
        mov eax, o
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
        fadd dword ptr [edx+0x30]
        fxch st(1)
        fadd dword ptr [edx+0x34]
        fxch st(2)
        fadd dword ptr [edx+0x38]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax+8]
        fstp dword ptr [eax+4]
    }
}

inline void XformSelf(float* o, const float* p, const float* m)
{
    __asm {
        mov ecx, p
        mov edx, m
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
        fadd dword ptr [edx+0x30]
        fxch st(1)
        fadd dword ptr [edx+0x34]
        fxch st(2)
        fadd dword ptr [edx+0x38]
        fxch st(1)
        fstp dword ptr [ecx]
        fstp dword ptr [ecx+8]
        fstp dword ptr [ecx+4]
    }
}

// Vector times the rotation part of the matrix.
inline void Rot(float* o, const float* p, const float* m)
{
    __asm {
        mov ecx, p
        mov edx, m
        mov eax, o
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
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
}

struct P3 { float x, y, z; P3 operator*(float s) const; }; // 0x40b0c0
extern float DAT_004e134c;
extern float DAT_004e1348;

// NONMATCHING: RALLYTROPHY 0x4ba440
// Original sets the EH state to 3 after the base ctor (ours 0); the source of states 0-2 is unknown.
ShadowObject::ShadowObject()
{
    type = 2;
    kind = 0;
    Reset();
}

// FUNCTION: RALLYTROPHY 0x4ba4a0
void ShadowObject::Reset()
{
    id = 1;
    f128 = 1.0f;
    f12c = 1.0f;
    f130 = 1.0f;
    f134 = 1.0f;
    f138[0] = 0; f138[1] = 0; f138[2] = 0; f138[3] = 0;
    f148[0] = 0; f148[1] = 0; f148[2] = 0; f148[3] = 0;
    f158 = 1000.0f;
    f15c = 1.0f;
    f160 = 0;
    f164 = 1.0f;
    f168 = 0;
    f16c = 3.1415927f;
    f170 = 3.1415927f;
    f174 = 1.0f;
    float* w = world.m;
    w[3] = 0; w[2] = 0; w[1] = 0;
    w[7] = 0; w[6] = 0; w[4] = 0;
    w[11] = 0; w[9] = 0; w[8] = 0;
    w[14] = 0; w[13] = 0; w[12] = 0;
    w[15] = 1.0f; w[10] = 1.0f; w[5] = 1.0f; w[0] = 1.0f;
}

// FUNCTION: RALLYTROPHY 0x4ba580
ShadowObjectB::ShadowObjectB()
{
    kind = 1;
}

// FUNCTION: RALLYTROPHY 0x4ba5a0
ShadowCaster::ShadowCaster()
{
    kind = 2;
}

// FUNCTION: RALLYTROPHY 0x4ba5f0
ShadowObject::~ShadowObject()
{
}

// FUNCTION: RALLYTROPHY 0x4ba600
// SYMBOL: ??_GShadowObject@@UAEPAXI@Z
typedef void Generated_4ba600();

// FUNCTION: RALLYTROPHY 0x4baf40
// SYMBOL: ??0DynamicMesh@@QAE@XZ
DynamicMesh::DynamicMesh()
{
    f0 = 0;
    verts = 0;
    tris = 0;
    nVerts = 0;
    nTris = 0;
    obj = 0;
    f18 = 0;
    f1c = 0;
    f24 = 0;
    f28 = 0;
    f2c = 0;
    f30 = 0;
    f34 = 0;
    f10 = 0;
}

// FUNCTION: RALLYTROPHY 0x4baf70
// SYMBOL: ??0DynamicMesh@@QAE@ABV0@@Z
DynamicMesh::DynamicMesh(const DynamicMesh& o)
{
    Copy(&o);
    f10 = 0;
}

// FUNCTION: RALLYTROPHY 0x4bafe0
DynamicMesh* DynamicMesh::Copy(const DynamicMesh* o)
{
    nVerts = o->nVerts;
    f0 = o->f0;
    f18 = o->f18;
    f24 = o->f24;
    f1c = o->f1c;
    nTris = o->nTris;
    verts = new SceneVertex[nVerts];
    tris = new unsigned short[nTris * 3];
    memcpy(verts, o->verts, nVerts * sizeof(SceneVertex));
    memcpy(tris, o->tris, nTris * 3 * 2);
    f28 = 0;
    f2c = 0;
    f30 = 0;
    f34 = 0;
    obj = 0;
    return this;
}

// FUNCTION: RALLYTROPHY 0x4baf90
DynamicMesh::~DynamicMesh()
{
    if (verts)
        delete[] verts;
    delete[] tris;
    if (obj)
        delete obj;
}

// FUNCTION: RALLYTROPHY 0x4bb130
StageAnimatorBase::StageAnimatorBase()
{
    id = -1;
}

__declspec(naked)
// PE transcription preserves the original x87 division and multiply operand order.
// FUNCTION: RALLYTROPHY 0x4bb170
void SegmentInfo::Update()
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 00ch
        _emit 0d9h
        _emit 041h

        _emit 018h
        _emit 0d9h
        _emit 011h
        _emit 0d9h
        _emit 041h

        _emit 01ch
        _emit 0d9h
        _emit 054h
        _emit 024h
        _emit 004h

        _emit 0d9h
        _emit 059h
        _emit 004h
        _emit 0d9h
        _emit 041h

        _emit 020h
        _emit 0d9h
        _emit 054h
        _emit 024h
        _emit 008h

        _emit 0d9h
        _emit 059h
        _emit 008h
        _emit 0d8h
        _emit 061h

        _emit 00ch
        _emit 0d9h
        _emit 011h
        _emit 0d9h
        _emit 044h

        _emit 024h
        _emit 004h
        _emit 0d8h
        _emit 061h
        _emit 010h

        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 000h
        _emit 0d9h

        _emit 044h
        _emit 024h
        _emit 008h
        _emit 08bh
        _emit 044h

        _emit 024h
        _emit 000h
        _emit 0d8h
        _emit 061h
        _emit 014h

        _emit 089h
        _emit 041h
        _emit 004h
        _emit 089h
        _emit 041h

        _emit 028h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 004h

        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 004h
        _emit 08bh

        _emit 054h
        _emit 024h
        _emit 004h
        _emit 0d8h
        _emit 04ch

        _emit 024h
        _emit 004h
        _emit 089h
        _emit 051h
        _emit 008h

        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 000h
        _emit 089h

        _emit 051h
        _emit 02ch
        _emit 0d8h
        _emit 04ch
        _emit 024h

        _emit 000h
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 0c1h

        _emit 0d8h
        _emit 0cah
        _emit 0deh
        _emit 0c1h
        _emit 0d9h

        _emit 0fah
        _emit 0d9h
        _emit 059h
        _emit 034h
        _emit 0d9h

        _emit 059h
        _emit 024h
        _emit 0d9h
        _emit 041h
        _emit 034h

        _emit 0d8h
        _emit 03dh
        _emit 048h
        _emit 013h
        _emit 04eh

        _emit 000h
        _emit 0d9h
        _emit 0c0h
        _emit 0d8h
        _emit 049h

        _emit 024h
        _emit 0d9h
        _emit 059h
        _emit 024h
        _emit 0d9h

        _emit 0c0h
        _emit 0d8h
        _emit 049h
        _emit 028h
        _emit 0d9h

        _emit 059h
        _emit 028h
        _emit 0d8h
        _emit 049h
        _emit 02ch

        _emit 0d9h
        _emit 059h
        _emit 02ch
        _emit 0d9h
        _emit 041h

        _emit 014h
        _emit 0d8h
        _emit 049h
        _emit 02ch
        _emit 0d9h

        _emit 041h
        _emit 010h
        _emit 0d8h
        _emit 049h
        _emit 028h

        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 041h
        _emit 00ch

        _emit 0d8h
        _emit 049h
        _emit 024h
        _emit 0deh
        _emit 0c1h

        _emit 0d9h
        _emit 059h
        _emit 030h
        _emit 083h
        _emit 0c4h

        _emit 00ch
        _emit 0c3h
    }
}

// original: 0x4ba620 etc. follow

// NONMATCHING: RALLYTROPHY 0x4ba620
// Semantics complete (inline-asm Xform/Rot, EBP frame); original has different local/register allocation (428 bytes differ).
void ViewFrustum::Init(FrustumDevice* device)
{
    ViewFrustum* self = this;
    device->SetupFrustum(this);
    camera = device->GetCameraMatrix();
    float* q = (float*)((char*)this + 100);
    int* e = (int*)((char*)this + 0xe8);
    int n = 6;
    for (; n != 0; n--) {
        P3 t = *(P3*)(q - 0x19) * q[-0x16];
        float* v = &t.x;
        const float* m = self->camera;
        XformSelf(v, v, m);
        const float* m2 = self->camera;
        Rot(q - 1, q - 0x19, m2);
        q[2] = t.x * q[-1] + t.z * q[1] + t.y * q[0];
        e[-6] = (DAT_004e134c <= q[-1]) ? 3 : 0;
        e[0] = (DAT_004e134c <= q[0]) ? 4 : 1;
        e[6] = (DAT_004e134c <= q[1]) ? 5 : 2;
        e++;
        q += 4;
    }
    farD = -planes[7];
    nearD = planes[3];
    float a = planes[10] * farD;
    float c = planes[18] * farD;
    radius = (float)sqrt(a * a + farD * farD + c * c);
}

// NONMATCHING: RALLYTROPHY 0x4ba830
// Logic complete; original keeps the transformed point and the deltas in separate stack locals (ours reuses slots).
int ViewFrustum::TestSphere(const SceneBounds* b, const float* m)
{
    if (b->flags & 1) {
        float pt[3];
        Xform(pt, &b->c.x, m);
        float x = pt[0], y = pt[1], z = pt[2];
        float* cam = camera;
        float dx = x - cam[12];
        float dy = y - cam[13];
        float dz = z - cam[14];
        float d2 = dx * dx + dy * dy + dz * dz;
        if (d2 < b->d * b->d)
            return 1;
        float r = radius + b->d;
        if (r * r < d2)
            return 0;
        dx = dx * cam[8] + dy * cam[9] + dz * cam[10];
        if (dx < nearD - b->d)
            return 0;
        if ((int)(farD + b->d) < (int)dx)
            return 0;
        int i = 0;
        float* p = &tplanes[0];
        do {
            if (y * p[1] + x * p[0] + z * p[2] + b->d < p[3])
                return 0;
            i++;
            p += 4;
        } while (i < 4);
    }
    return 1;
}

// Sign test of a float through its bit pattern (the original masks 0x80000000).
static inline int Negative(float f)
{
    float v = f;
    return (*(unsigned int*)&v & 0x80000000) != 0;
}

// NONMATCHING: RALLYTROPHY 0x4ba9d0
// Logic complete; not compared in detail.
int ViewFrustum::TestBounds(const SceneBounds* b)
{
    const float* bf = (const float*)b;
    if ((b->flags & 1) == 0) {
        if (b->flags & 2) {
            const int* e = &idx[0] + 0;
            e = (const int*)((const char*)this + 0xe8);
            const float* p = (const float*)((const char*)this + 100);
            for (int i = 0; i < 6; i++) {
                if (Negative((bf[e[0]] * p[0] + bf[e[-6]] * p[-1] + bf[e[6]] * p[1]) - p[2]))
                    return 0;
                e++;
                p += 4;
            }
        }
    } else {
        const float* cam = camera;
        float dx = b->c.x - cam[12];
        float dy = b->c.y - cam[13];
        float dz = b->c.z - cam[14];
        float d2 = dx * dx + dy * dy + dz * dz;
        if ((int)(b->d * b->d) <= (int)d2) {
            float r = radius + b->d;
            if ((int)(r * r) < (int)d2)
                return 0;
            float f = dx * cam[8] + dy * cam[9] + dz * cam[10];
            if (nearD - b->d <= f) {
                if ((int)f <= (int)(farD + b->d)) {
                    const float* p = &tplanes[0] + 1;
                    int i = 0;
                    while (p[2] <= b->c.y * p[0] + p[1] * b->c.z + p[-1] * b->c.x + b->d) {
                        i++;
                        p += 4;
                        if (3 < i)
                            return 1;
                    }
                }
                return 0;
            }
            return 0;
        }
    }
    return 1;
}

// NONMATCHING: RALLYTROPHY 0x4bab80
// Logic complete; not compared in detail.
int ViewFrustum::TestBoundsB(const SceneBounds* b)
{
    const float* bf = (const float*)b;
    if (b->flags & 2) {
        const int* e = (const int*)((const char*)this + 0xe8);
        const float* p = (const float*)((const char*)this + 100);
        for (int i = 0; i < 6; i++) {
            if (Negative((bf[e[0]] * p[0] + bf[e[-6]] * p[-1] + bf[e[6]] * p[1]) - p[2]))
                return 0;
            e++;
            p += 4;
        }
        return 1;
    }
    if (b->flags & 1) {
        const float* cam = camera;
        float dx = b->c.x - cam[12];
        float dy = b->c.y - cam[13];
        float dz = b->c.z - cam[14];
        float d2 = dx * dx + dy * dy + dz * dz;
        if ((int)(b->d * b->d) <= (int)d2) {
            float r = radius + b->d;
            if ((int)(r * r) < (int)d2)
                return 0;
            float f = dx * cam[8] + dy * cam[9] + dz * cam[10];
            if (f < nearD - b->d)
                return 0;
            if ((int)(farD + b->d) < (int)f)
                return 0;
            const float* p = &tplanes[0] + 1;
            int i = 0;
            do {
                if (b->c.y * p[0] + p[-1] * b->c.x + p[1] * b->c.z + b->d < p[2])
                    return 0;
                i++;
                p += 4;
            } while (i < 4);
        }
    }
    return 1;
}

// NONMATCHING: RALLYTROPHY 0x4bad30
// Logic complete (EH frame in the original comes from a temporary with a destructor); not compared in detail.
int ViewFrustum::TestSphereMargin(const SceneBounds* b, const float* m, float margin)
{
    if (b->flags & 1) {
        float rr = margin + b->d;
        float pt[3];
        Xform(pt, &b->c.x, m);
        const float* cam = camera;
        float diff[3];
        diff[0] = pt[0] - cam[12];
        diff[1] = pt[1] - cam[13];
        diff[2] = pt[2] - cam[14];
        float d2 = diff[0] * diff[0] + diff[2] * diff[2] + diff[1] * diff[1];
        if (rr * rr <= d2) {
            float r = rr + radius;
            if (r * r < d2)
                return 0;
            float f = diff[0] * cam[8] + diff[2] * cam[10] + diff[1] * cam[9];
            if (f < nearD - rr)
                return 0;
            if (rr + farD < f)
                return 0;
            const float* p = &tplanes[0] + 1;
            int i = 0;
            do {
                if (pt[1] * p[0] + pt[0] * p[-1] + pt[2] * p[1] + rr < p[2])
                    return 0;
                i++;
                p += 4;
            } while (i < 4);
        }
    }
    return 1;
}

__declspec(naked)
// PE observation: pass DAT_00581de8 as ECX and tail-call its 3-byte constructor at 0x490ad0.
// FUNCTION: RALLYTROPHY 0x4bb230
void FUN_004bb230()
{
    __asm {
        _emit 0b9h
        _emit 0e8h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 0e9h
        _emit 096h
        _emit 058h
        _emit 0fdh
        _emit 0ffh
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for DAT_00581de8's static object.
// FUNCTION: RALLYTROPHY 0x4bb240
void FUN_004bb240()
{
    __asm {
        _emit 068h
        _emit 050h
        _emit 0b2h
        _emit 04bh
        _emit 000h
        _emit 0e8h
        _emit 0b7h
        _emit 05ah
        _emit 000h
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}
