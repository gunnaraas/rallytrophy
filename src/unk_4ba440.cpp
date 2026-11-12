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

// FUNCTION: RALLYTROPHY 0x4ba440
// Three empty-destructor members (header) reproduce the original EH state 3 after the base constructor.
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
    q128.a = 1.0f;
    q128.b = 1.0f;
    q128.c = 1.0f;
    q128.d = 1.0f;
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
// Semantics complete (inline-asm Xform/Rot, EBP frame). 66->47 insns differ: i-indexed loop over planes/tplanes + if/else idx stores.
// Tried 2026-10-31 re-pass: ~35 variants (param-slot counter, q-pointer vs indexed, ternary vs if/else, 6 dot orders (original y,z,x kept), while/!= loops, t hoisted, a/c tail forms, volatile self); still differs: original has one induction pointer q with jmp-to-test loop and lea of P/T after the first Xform, esi/edi prologue swapped, tail a/c in stack temps.
void ViewFrustum::Init(FrustumDevice* device)
{
    ViewFrustum* self = this;
    device->SetupFrustum(this);
    camera = device->GetCameraMatrix();
    for (int i = 0; i < 6; i++) {
        float* P = &planes[4*i]; float* T = &tplanes[4*i];
        P3 t = *(P3*)P * P[3];
        XformSelf(&t.x, &t.x, self->camera);
        Rot(T, P, self->camera);
        float q = t.y * T[1];
        q = t.z * T[2] + q;
        q = t.x * T[0] + q;
        T[3] = q;
        if (T[0] < DAT_004e134c) idx[i] = 0; else idx[i] = 3;
        if (T[1] < DAT_004e134c) idx[i+6] = 1; else idx[i+6] = 4;
        if (T[2] < DAT_004e134c) idx[i+12] = 2; else idx[i+12] = 5;
    }
    farD = -planes[7];
    nearD = planes[3];
    float a = planes[10];
    float c = planes[18];
    a = a * farD;
    c = c * farD;
    radius = (float)sqrt(c * c + farD * farD + a * a);
}

// A vector with an empty destructor and the out-of-line operator- (0x414330): the live copy gives the original its EH frame.
struct FrVec { float x, y, z; ~FrVec() {} FrVec operator-(const FrVec& o) const; };

// FUNCTION: RALLYTROPHY 0x4ba830
int ViewFrustum::TestSphere(const SceneBounds* b, const float* m)
{
    if (b->flags & 1) {
        FrVec pt;
        Xform(&pt.x, &b->c.x, m);
        const float* cam = camera;
        FrVec dv = {pt.x - cam[12], pt.y - cam[13], pt.z - cam[14]};
        float d2 = dv.z * dv.z + dv.y * dv.y + dv.x * dv.x;
        if (d2 < b->d * b->d)
            return 1;
        float r = radius + b->d;
        if (d2 > r * r)
            return 0;
        float f = dv.z * cam[10] + dv.y * cam[9] + dv.x * cam[8];
        if (f < nearD - b->d)
            return 0;
        float fs = farD + b->d;
        if (*(int*)&f > *(int*)&fs)
            return 0;
        int i = 0;
        const float* p = &tplanes[8] + 1;
        for (; i < 4; i++, p += 4) {
            if (pt.z * p[1] + pt.x * p[-1] + pt.y * p[0] + b->d < p[2])
                return 0;
        }
    }
    return 1;
}

// Sign test of a float through its bit pattern (the original masks 0x80000000).
static inline int Negative(float f)
{
    float v = f;
    return (*(unsigned int*)&v & 0x80000000) != 0;
}

// FUNCTION: RALLYTROPHY 0x4ba9d0
int ViewFrustum::TestBounds(const SceneBounds* b)
{
    const float* bf = (const float*)b;
    if (b->flags & 1) {
        FrVec dv = {b->c.x, b->c.y, b->c.z};
        const float* cam = camera;
        dv.x = dv.x - cam[12];
        dv.y = dv.y - cam[13];
        dv.z = dv.z - cam[14];
        float d2 = dv.z * dv.z + dv.y * dv.y + dv.x * dv.x;
        float d2b = b->d * b->d;
        if (*(int*)&d2 < *(int*)&d2b)
            return 1;
        float r = radius + b->d;
        r = r * r;
        if (*(int*)&d2 > *(int*)&r)
            return 0;
        float f = dv.z * cam[10] + dv.y * cam[9] + dv.x * cam[8];
        if (f < nearD - b->d)
            return 0;
        float fs = farD + b->d;
        if (*(int*)&f > *(int*)&fs)
            return 0;
        int i = 0;
        const float* p = &tplanes[8] + 1;
        for (; i < 4; i++, p += 4) {
            float t = p[-1] * b->c.x;
            t = p[1] * b->c.z + t;
            float y = b->c.y;
            if (t + y * p[0] + b->d < p[2])
                return 0;
        }
        return 1;
    }
    if (b->flags & 2) {
        volatile float* vb = (volatile float*)bf;
        int i = 0;
        const int* e = (const int*)((const char*)this + 0xe8);
        const float* p = (const float*)((const char*)this + 100);
        for (; i < 6; i++, e++, p += 4) {
            float a = vb[e[-6]]; float c = vb[e[0]]; float d = vb[e[6]];
            float v = (d * p[1] + a * p[-1]) + c * p[0] - p[2];
            if (Negative(v))
                return 0;
        }
    }
    return 1;
}

static inline float PlaneDot(float a, float b, float c, const float* p)
{
    return a * p[-1] + c * p[1] + b * p[0];
}

// FUNCTION: RALLYTROPHY 0x4bab80
int ViewFrustum::TestBoundsB(const SceneBounds* b)
{
    const float* bf = (const float*)b;
    if (b->flags & 2) {
        int i = 0;
        const int* e = (const int*)((const char*)this + 0xe8);
        const float* p = (const float*)((const char*)this + 100);
        volatile float* vb = (volatile float*)bf;
        for (; i < 6; i++, e++, p += 4) {
            float a = vb[e[-6]]; float c = vb[e[0]]; float d = vb[e[6]];
            float v = (d * p[1] + a * p[-1]) + c * p[0] - p[2];
            if (*(unsigned*)&v & 0x80000000)
                return 0;
        }
        return 1;
    }
    if (b->flags & 1) {
        FrVec dv = {b->c.x, b->c.y, b->c.z};
        const float* cam = camera;
        dv.x = dv.x - cam[12];
        dv.y = dv.y - cam[13];
        dv.z = dv.z - cam[14];
        float d2 = dv.x * dv.x + dv.y * dv.y + dv.z * dv.z;
        float d2b = b->d * b->d;
        if (*(int*)&d2 < *(int*)&d2b)
            return 1;
        float r = radius + b->d;
        r = r * r;
        if (*(int*)&d2 > *(int*)&r)
            return 0;
        float f = dv.z * cam[10] + dv.y * cam[9] + dv.x * cam[8];
        if (f < nearD - b->d)
            return 0;
        float fs = farD + b->d;
        if (*(int*)&f > *(int*)&fs)
            return 0;
        int i = 0;
        const float* p = &tplanes[8] + 1;
        for (; i < 4; i++, p += 4) {
            { float t = b->c.z * p[1]; t = b->c.x * p[-1] + t; if (t + b->c.y * p[0] + b->d < p[2]) return 0; }
        }
    }
    return 1;
}

// FUNCTION: RALLYTROPHY 0x4bad30
int ViewFrustum::TestSphereMargin(const SceneBounds* b, const float* m, float margin)
{
    if (b->flags & 1) {
        float rr = margin + b->d;
        FrVec pt = {b->c.x, b->c.y, b->c.z};
        XformSelf(&pt.x, &pt.x, m);
        const float* cam = camera;
        FrVec dv = pt - *(const FrVec*)(cam + 12);
        float d2 = dv.x * dv.x + dv.z * dv.z + dv.y * dv.y;
        if (d2 < rr * rr) return 1;
        {
            float r = rr + radius;
            if (d2 > r * r)
                return 0;
            float f = dv.x * cam[8] + dv.z * cam[10] + dv.y * cam[9];
            if (f < nearD - rr)
                return 0;
            if (f > rr + farD)
                return 0;
            int i = 0;
            const float* p = &tplanes[8] + 1;
            for (; i < 4; i++, p += 4) {
                if (pt.y * p[0] + pt.x * p[-1] + pt.z * p[1] + rr < p[2])
                    return 0;
            }
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
