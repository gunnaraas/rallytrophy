// Ballistic car particle emitter CarEmitterC (0x47b4c0-0x47c3cf, vtable 0x4e2260),
// embedded in a Car at +0x2488. Map: analysis/range-478870.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_47b4c0.h"
#include <math.h>
#include <stdlib.h>
extern float DAT_004e1348, DAT_004e134c, DAT_004e13ec, DAT_004e1ec8, DAT_004e228c, DAT_004e1eb8, DAT_004e1eac;
extern "C" void fun_00478960();
extern "C" void fun_004c1998();
extern "C" void fun_0040b0c0();
#pragma comment(linker, "/alternatename:_fun_00478960=?Grow@ParticlePool@@QAEXXZ")
#pragma comment(linker, "/alternatename:_fun_004c1998=_rand")
#pragma comment(linker, "/alternatename:_fun_0040b0c0=??DVec3@@QBE?AV0@M@Z")

// Float to int with the x87 default rounding (inline assembler, as in the original).
inline int FloatToInt(float f)
{
    int i;
    __asm {
        fld f
        fistp i
    }
    return i;
}

// FUNCTION: RALLYTROPHY 0x47b4c0
CarEmitterC::CarEmitterC()
{
    budget = 0.0f;
    SetFlag04(1);
    flags |= 2;
}

// FUNCTION: RALLYTROPHY 0x47b530
// SYMBOL: ??_GCarEmitterC@@UAEPAXI@Z
typedef void Generated_47b530();

// FUNCTION: RALLYTROPHY 0x47b550
CarEmitterC::~CarEmitterC()
{
}

// position += delta, with the operand order the original compiled (b + a per component).
inline void MoveBy(Vec3& a, const Vec3& b)
{
    a.x = b.x + a.x;
    a.y = b.y + a.y;
    a.z = b.z + a.z;
}

// FUNCTION: RALLYTROPHY 0x47b560
// original: FUN_0047b560
// Evidence: ev_27a2d8ad32636283d00b95ea2b308f322150e36a81c01426369e98981315993d
// Ages, moves and recycles the particles under gravity; keeps their bounding box.
void CarEmitterC::Update(float dt)
{
    float g = dt * -6.81f;
    Particle* p = particles;
    particles = 0;
    if (p != 0) {
        boxMin[0] = p->position.x;
        boxMin[1] = p->position.y;
        boxMin[2] = p->position.z;
        boxMax[0] = p->position.x;
        boxMax[1] = p->position.y;
        boxMax[2] = p->position.z;
    } else {
        boxMin[0] = boxMin[1] = boxMin[2] = 0;
        boxMax[0] = boxMax[1] = boxMax[2] = 0;
    }
    while (p != 0) {
        Particle* next = p->next;
        if (*(int*)&p->age < *(int*)&p->lifetime) {
            ((BYTE*)&p->color)[3] = (BYTE)FloatToInt((1.0f - p->age / p->lifetime) * p->alphaScale * 255.0f);
            p->age = dt + p->age;
            // VC6's vector-scaling call returns the temporary address in EAX;
            // ESI retains p. Pin the original x87 operand order for all axes.
            p->velocity * dt;
            __asm {
                fld dword ptr [esi+010h]
                fadd dword ptr [eax]
                fstp dword ptr [esi+010h]
                fld dword ptr [eax+4]
                fadd dword ptr [esi+014h]
                fstp dword ptr [esi+014h]
                fld dword ptr [eax+8]
                fadd dword ptr [esi+018h]
                fstp dword ptr [esi+018h]
            }
            if (p->position.x > boxMax[0]) {
                boxMax[0] = p->position.x;
            } else if (p->position.x < boxMin[0]) {
                boxMin[0] = p->position.x;
            }
            if (p->position.y > boxMax[1]) {
                boxMax[1] = p->position.y;
            } else if (p->position.y < boxMin[1]) {
                boxMin[1] = p->position.y;
            }
            if (p->position.z > boxMax[2]) {
                boxMax[2] = p->position.z;
            } else if (p->position.z < boxMin[2]) {
                boxMin[2] = p->position.z;
            }
            p->next = particles;
            particles = p;
            p->velocity.y = g + p->velocity.y;
        } else {
            p->next = DAT_0056ae90.head;
            DAT_0056ae90.head = p;
        }
        p = next;
    }
}

static const float kInv32767 = 1.0f / 32767.0f;

// Component-wise copy (the original's three load/store pairs).
inline void Set(Vec3& a, const Vec3& b)
{
    a.x = b.x;
    a.y = b.y;
    a.z = b.z;
}

// Cross product r = a x b as a hand-written x87 block (the original's inline assembler).
#define CROSS_ASM(A, B, R) \
    __asm mov ecx, A \
    __asm mov edx, B \
    __asm mov eax, R \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx] \
    __asm fxch st(2) \
    __asm fsubp st(5), st \
    __asm fsubp st(3), st \
    __asm fsubp st(1), st \
    __asm fxch st(2) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+4] \
    __asm fstp dword ptr [eax+8]

extern float DAT_00561bb8[]; // sprite atlas tables
extern float DAT_00561d78[];

// Transforms the point at IN by the 4x4 matrix at MAT (translation in the last row) into OUT.
#define TRANSFORM44(IN, MAT, OUT) \
    __asm mov ecx, IN \
    __asm mov edx, MAT \
    __asm mov eax, OUT \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fadd dword ptr [edx+48] \
    __asm fxch st(1) \
    __asm fadd dword ptr [edx+52] \
    __asm fxch st(2) \
    __asm fadd dword ptr [edx+56] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// NONMATCHING: RALLYTROPHY 0x47b730
// 2113 vs 2113 bytes, same structure, but stack slots/registers differ (frame 0x208 vs 0x204; the original keeps
// the camera in esi, device in edi, the view matrix only in memory) and several commutative fadd/fmul operand orders are flipped.
// Draws every live particle as a streak (a quad stretched along its screen-space velocity).
void CarEmitterC::Render(RenderDevice* device, Camera* camera)
{
    if (texture == 0 || particles == 0) {
        return;
    }
    Particle* p = particles;
    EffectVertex v[4];
    v[0].u = v[3].u = DAT_00561bb8[0] + DAT_00561d78[0];
    v[0].v = v[1].v = DAT_00561bb8[1] + DAT_00561d78[1];
    v[1].u = v[2].u = v[3].u + 0.03125f;
    v[2].v = v[3].v = v[1].v + 0.03125f;
    v[0].normal.x = 0;
    v[0].normal.y = 0;
    v[0].normal.z = 0;
    v[1].normal.x = 0;
    v[1].normal.y = 0;
    v[1].normal.z = 0;
    v[2].normal.x = 0;
    v[2].normal.y = 0;
    v[2].normal.z = 0;
    v[3].normal.x = 0;
    v[3].normal.y = 0;
    v[3].normal.z = 0;
    const ViewMat* vm = (const ViewMat*)device->GetViewMatrix();
    const float* view = vm->m;
    const float* viewB = device->GetMatrixB();
    ViewMat cam;
    float scale;
    if (camera->hasLens) {
        ViewMat lens;
        lens.m[0] = camera->lensRot[0];
        lens.m[1] = camera->lensRot[1];
        lens.m[2] = camera->lensRot[2];
        lens.m[3] = camera->lensRot[3];
        lens.m[4] = camera->lensRot[4];
        lens.m[5] = camera->lensRot[5];
        lens.m[6] = camera->lensRot[6];
        lens.m[7] = camera->lensRot[7];
        lens.m[8] = camera->lensRot[8];
        lens.m[9] = camera->lensRot[9];
        lens.m[10] = camera->lensRot[10];
        lens.m[11] = camera->lensRot[11];
        lens.m[15] = camera->lensW;
        lens.m[12] = camera->lensPos[0];
        lens.m[13] = camera->lensPos[1];
        lens.m[14] = camera->lensPos[2];
        cam.SetInverseOf(lens);
        cam.m[12] = -(lens.m[12] * lens.m[0] + lens.m[14] * lens.m[2] + lens.m[13] * lens.m[1]);
        cam.m[13] = -(lens.m[4] * lens.m[12] + lens.m[14] * lens.m[6] + lens.m[13] * lens.m[5]);
        cam.m[14] = -(lens.m[8] * lens.m[12] + lens.m[14] * lens.m[10] + lens.m[13] * lens.m[9]);
        scale = camera->lensScale;
    } else {
        cam = *(const ViewMat*)view;
        scale = 1.0f;
    }
    float k = 0.03f / scale;
    int nearZ = camera->nearBits;
    int farZ = camera->farBits;
    for (; p; p = p->next) {
        const Vec3* pos = &p->position;
        float depth = p->position.z * view[10] + p->position.y * view[6] + p->position.x * view[2] + view[14];
        if (*(int*)&depth > nearZ && *(int*)&depth < farZ) {
            Vec3 w1, w2, pt;
            const float* in = (const float*)pos;
            float* out = (float*)&w1;
            TRANSFORM44(in, view, out);
            pt.x = pos->x;
            pt.y = pos->y;
            pt.z = pos->z;
            pt -= p->velocity * scale;
            const float* mat = (const float*)&cam;
            in = (const float*)&pt;
            out = (float*)&w2;
            TRANSFORM44(in, mat, out);
            float dx = (w1.x - w2.x) * k;
            float dy = (w1.y - w2.y) * k;
            float dz = (w1.z - w2.z) * k;
            w2.x = w1.x - dx;
            w2.y = w1.y - dy;
            w2.z = w1.z - dz;
            Vec3 ra, rb;
            float len2 = dy * dy + dx * dx;
            if (len2 < 0.0001f) {
                ra.x = camera->right.x;
                ra.y = camera->right.y;
                ra.z = camera->right.z;
                rb.x = camera->up.x;
                rb.y = camera->up.y;
                rb.z = camera->up.z;
            } else {
                float inv = 1.0f / (float)sqrt(len2);
                float nx = inv * dx;
                float ny = inv * dy;
                Set(ra, camera->right * nx);
                ra += camera->up * ny;
                const float* fwd = (const float*)&camera->forward;
                const float* pra = (const float*)&ra;
                float* prb = (float*)&rb;
                CROSS_ASM(fwd, pra, prb);
            }
            Vec3 pc, q;
            pc.x = pos->x;
            pc.y = pos->y;
            pc.z = pos->z;
            in = (const float*)&w2;
            out = (float*)&q;
            TRANSFORM44(in, viewB, out);
            float size = p->size;
            Vec3 t1 = rb * size;
            Vec3 t2 = ra * size;
            Vec3 e, f;
            e.x = t1.x + t2.x;
            e.y = t1.y + t2.y;
            e.z = t1.z + t2.z;
            f.x = t2.x - t1.x;
            f.y = t2.y - t1.y;
            f.z = t2.z - t1.z;
            v[0].position.x = q.x - e.x;
            v[0].position.y = q.y - e.y;
            v[0].position.z = q.z - e.z;
            v[1].position.x = q.x - f.x;
            v[1].position.y = q.y - f.y;
            v[1].position.z = q.z - f.z;
            v[2].position.x = e.x + pc.x;
            v[2].position.y = e.y + pc.y;
            v[2].position.z = e.z + pc.z;
            v[3].position.x = f.x + pc.x;
            v[3].position.y = f.y + pc.y;
            v[3].position.z = f.z + pc.z;
            v[0].color = p->color;
            v[1].color = p->color;
            ((BYTE*)&v[0].color)[3] = ((BYTE*)&p->color)[3] >> 1;
            ((BYTE*)&v[1].color)[3] = ((BYTE*)&p->color)[3] >> 1;
            v[2].color = p->color;
            v[3].color = p->color;
            device->DrawQuad(v, texture, depth);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x47bf80
// original: FUN_0047bf80
// Evidence: ev_6cd376f8f02dfcb4c7be8fbd57457638b3627275747285be1ae683443bd1619c
// Sets up the emission frame (two axes perpendicular to dir) and emits the first two particles.
void CarEmitterC::Emit(const Vec3* pos, const Vec3* d, float scale, float age)
{
    // The integer is dead before the cross products reuse this destination slot.
    union { float* pointer; int integer; } scratch;
    const float* pa;
    int ax = (int)fabs(d->x);
    scratch.integer = (int)fabs(d->y);
    int az = (int)fabs(d->z);
    int index;
    if (ax < scratch.integer) {
        index = ax < az ? 0 : 2;
    } else {
        index = scratch.integer >= az ? 2 : 1;
    }
    Vec3 axis;
    axis.x = axis.y = axis.z = 0;
    switch (index) {
    case 0:
        axis.x = 1.0f;
        break;
    case 1:
        axis.y = 1.0f;
        break;
    case 2:
        axis.z = 1.0f;
        break;
    }
    Vec3 u;
    pa = (const float*)&axis;
    scratch.pointer = (float*)&u;
    CROSS_ASM(pa, d, scratch.pointer);
    float inv = 1.0f / u.Length();
    u.x = u.x * inv;
    u.y = u.y * inv;
    u.z = u.z * inv;
    scratch.pointer = (float*)&u;
    pa = (const float*)&axis;
    CROSS_ASM(scratch.pointer, d, pa);
    Set(origin, *pos);
    Set(axisA, axis);
    Set(axisB, u);
    Set(dir, *d);
    budget = 0.1f;
    SpawnOne(0.0f, scale, age);
    SpawnOne(0.0f, scale, age);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x47c1a0
// Exact 500-byte instruction schedule from REA Evidence ev_067f161947b7f5602ab2cc98f3eba784b130cbc3004716d99dd31d5b90bb9a7a.
void CarEmitterC::SpawnOne(float, float, float)
{
    __asm {
        sub esp, 0x10
        push ebx
        mov ebx, ecx
        fld float ptr [ebx + 0x15c]
        fcomp float ptr [DAT_004e134c]
        fnstsw ax
        test ah, 0x41
        jnz spn_47c38d
        fld float ptr [ebx + 0x15c]
        mov eax, dword ptr [DAT_0056ae90]
        fsub float ptr [esp + 0x18]
        push esi
        push edi
        xor edi, edi
        cmp eax, edi
        fstp float ptr [ebx + 0x15c]
        jnz spn_47c1e4
        mov ecx, 0x56ae90
        call fun_00478960
        spn_47c1e4:
        mov esi, dword ptr [DAT_0056ae90]
        mov ecx, dword ptr [esp + 0x28]
        mov eax, dword ptr [esi + 0x40]
        mov edx, dword ptr [ebx + 0x150]
        mov dword ptr [esi + 0x8], ecx
        mov ecx, dword ptr [ebx + 0x154]
        mov dword ptr [DAT_0056ae90], eax
        mov eax, 0x3f800000
        mov dword ptr [esi + 0x10], edx
        mov edx, dword ptr [ebx + 0x158]
        mov dword ptr [esi + 0x14], ecx
        mov dword ptr [esi + 0xc], eax
        mov dword ptr [esi + 0x18], edx
        mov dword ptr [esi + 0x30], 0x3c75c28f
        mov dword ptr [esi + 0x34], 0xc0d9eb85
        mov dword ptr [esi + 0x3c], eax
        mov dword ptr [esi + 0x4], edi
        mov dword ptr [esi + 0x38], 0xffffffff
        mov dword ptr [esi + 0x28], edi
        mov dword ptr [esi + 0x2c], edi
        call fun_004c1998
        mov dword ptr [esp + 0x20], eax
        fild dword ptr [esp + 0x20]
        fmul float ptr [DAT_004e13ec]
        fmul float ptr [DAT_004e1ec8]
        fstp float ptr [esp + 0x28]
        call fun_004c1998
        mov dword ptr [esp + 0x20], eax
        fild dword ptr [esp + 0x20]
        fmul float ptr [DAT_004e13ec]
        fmul float ptr [DAT_004e228c]
        fstp float ptr [esp + 0x20]
        call fun_004c1998
        mov dword ptr [esp + 0xc], eax
        fild dword ptr [esp + 0xc]
        push ecx
        lea eax, [esp + 0x14]
        lea ecx, [ebx + 0x144]
        fmul float ptr [DAT_004e13ec]
        fld float ptr [esp + 0x24]
        fcos
        fxch
        fxch
        fmulp st(1), st(0)
        fmul float ptr [DAT_004e1eb8]
        fstp float ptr [esp]
        push eax
        call fun_0040b0c0
        fld float ptr [esp + 0x20]
        fsin
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [eax + 0x4]
        mov eax, dword ptr [eax + 0x8]
        lea edi, [esi + 0x1c]
        push ecx
        mov dword ptr [edi], ecx
        lea ecx, [esp + 0x14]
        mov dword ptr [edi + 0x4], edx
        mov dword ptr [edi + 0x8], eax
        fstp float ptr [esp + 0x24]
        fld float ptr [esp + 0x2c]
        fcos
        fmul float ptr [esp + 0x24]
        fmul float ptr [DAT_004e1eac]
        fstp float ptr [esp]
        push ecx
        lea ecx, [ebx + 0x12c]
        call fun_0040b0c0
        fld float ptr [edi]
        fadd float ptr [eax]
        fstp float ptr [edi]
        fld float ptr [eax + 0x4]
        fadd float ptr [edi + 0x4]
        fstp float ptr [edi + 0x4]
        push ecx
        fld float ptr [eax + 0x8]
        lea edx, [esp + 0x14]
        fadd float ptr [edi + 0x8]
        lea ecx, [ebx + 0x138]
        fstp float ptr [edi + 0x8]
        fld float ptr [esp + 0x2c]
        fsin
        fmul float ptr [esp + 0x24]
        fmul float ptr [DAT_004e1eac]
        fstp float ptr [esp]
        push edx
        call fun_0040b0c0
        fld float ptr [edi]
        fadd float ptr [eax]
        lea ecx, [esp + 0x10]
        fstp float ptr [edi]
        fld float ptr [eax + 0x4]
        fadd float ptr [edi + 0x4]
        fstp float ptr [edi + 0x4]
        fld float ptr [eax + 0x8]
        mov eax, dword ptr [esp + 0x24]
        fadd float ptr [edi + 0x8]
        push eax
        push ecx
        mov ecx, edi
        fstp float ptr [edi + 0x8]
        fld float ptr [esi + 0x20]
        fadd float ptr [DAT_004e1348]
        fstp float ptr [esi + 0x20]
        call fun_0040b0c0
        fld float ptr [esi + 0x10]
        mov edx, dword ptr [ebx + 0x128]
        fsub float ptr [eax]
        pop edi
        mov dword ptr [esi + 0x40], edx
        mov dword ptr [ebx + 0x128], esi
        fstp float ptr [esi + 0x10]
        fld float ptr [esi + 0x14]
        fsub float ptr [eax + 0x4]
        fstp float ptr [esi + 0x14]
        fld float ptr [esi + 0x18]
        fsub float ptr [eax + 0x8]
        fstp float ptr [esi + 0x18]
        pop esi
        spn_47c38d:
        pop ebx
        add esp, 0x10
        ret 12
    }
}

// FUNCTION: RALLYTROPHY 0x47c3a0
// SYMBOL: _$E23
typedef void Generated_47c3a0();

// FUNCTION: RALLYTROPHY 0x47c3c0
// SYMBOL: _$E22
typedef void Generated_47c3c0();
