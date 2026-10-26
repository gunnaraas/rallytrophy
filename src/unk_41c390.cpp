// Car rigid-body integration (PhysBody) and Vec3 helpers.
#include <string>
#include "unk_481390.h"
#include "unk_41c390.h"

extern float DAT_004e138c;
extern float DAT_004e1438;
extern float DAT_004e1430;
extern float DAT_004e1348;

static inline const Vec3* Addr(const Vec3& v) { return &v; }

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

// FUNCTION: RALLYTROPHY 0x41c390
void PhysBody::FUN_0041c390()
{
    Vec3 off;
    q.ToMatrix(m);
    Xform(&off.x, &a, m);
    v224[0] = *(DWORD*)&pos.x;
    v224[1] = *(DWORD*)&pos.y;
    v224[2] = *(DWORD*)&pos.z;
    *(float*)&v224[0] = off.x + *(float*)&v224[0];
    *(float*)&v224[1] = off.y + *(float*)&v224[1];
    *(float*)&v224[2] = off.z + *(float*)&v224[2];
    Refresh();
    const DWORD* r = (const DWORD*)Addr(v19c * t130);
    v1cc[0] = r[0];
    v1cc[1] = r[1];
    v1cc[2] = r[2];
    Rot(&v1d8.x, &v1a8.x, basis);
}

// NONMATCHING: RALLYTROPHY 0x41c540
// NONMATCHING: logic complete; original keeps the v1cc*dt temp without a pointer local and adds it to v224 in a separate pass (frame 0x40 vs ours 0x34)
void PhysBody::FUN_0041c540()
{
    Vec3 w = v1d8;
    Vec3 off;
    Xform(&off.x, &a, m);
    v224[0] = *(DWORD*)&pos.x;
    v224[1] = *(DWORD*)&pos.y;
    v224[2] = *(DWORD*)&pos.z;
    const Vec3* dv = Addr(*(Vec3*)v1cc * dt);
    *(float*)&v224[0] = *(float*)&v224[0] + dv->x;
    *(float*)&v224[1] = dv->y + *(float*)&v224[1];
    *(float*)&v224[2] = dv->z + *(float*)&v224[2];
    *(float*)&v224[0] = off.x + *(float*)&v224[0];
    *(float*)&v224[1] = off.y + *(float*)&v224[1];
    *(float*)&v224[2] = off.z + *(float*)&v224[2];
    float t1 = w.z * q.w + w.y * q.x - w.x * q.y;
    float t2 = -(w.x * q.x) - w.y * q.y - w.z * q.z;
    float k = dt * DAT_004e138c;
    float x0 = q.x;
    q.x = k * ((w.x * q.w + w.z * q.y) - w.y * q.z) + q.x;
    q.y = k * ((w.x * q.z + w.y * q.w) - w.z * x0) + q.y;
    q.z = k * t1 + q.z;
    q.w = k * t2 + q.w;
    float n = q.w * q.w + q.z * q.z + q.y * q.y + q.x * q.x;
    if (n < DAT_004e1438 || DAT_004e1430 < n) {
        float s = DAT_004e1348 / (float)sqrt(n);
        q.x = s * q.x;
        q.y = s * q.y;
        q.z = s * q.z;
        q.w = s * q.w;
    }
    q.ToMatrix(m);
    pos.x = *(float*)&v224[0];
    pos.y = *(float*)&v224[1];
    pos.z = *(float*)&v224[2];
    Xform(&off.x, &a, m);
    pos.x -= off.x;
    pos.y -= off.y;
    pos.z -= off.z;
    const DWORD* r = (const DWORD*)Addr(*dv + pos);
    v224[0] = r[0];
    v224[1] = r[1];
    v224[2] = r[2];
    Refresh();
    Rot(&v1d8.x, &v1a8.x, basis);
}

// FUNCTION: RALLYTROPHY 0x41c920
void PhysBody::SetCollision(CollisionPart* part)
{
    id230 = part->id;
    collision = new CollisionModel(part, 0);
}

// FUNCTION: RALLYTROPHY 0x41c990
void PhysBody::SetCollision2(CollisionPart* part)
{
    id230 = part->id;
    collision = new CollisionModel(part, 0, 0);
}

// FUNCTION: RALLYTROPHY 0x41ca00
void PhysBody::SetCollision3(CollisionPart* part)
{
    id230 = part->id;
    collision = new CollisionModel(part, 0, 1);
}

// FUNCTION: RALLYTROPHY 0x4203b0
// SYMBOL: ??GVec3@@QBE?AV0@XZ
Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

// FUNCTION: RALLYTROPHY 0x4203e0
Vec3 FUN_004203e0(const Vec3& a, const Vec3& b)
{
    return Vec3(b.z * a.y - a.z * b.y, a.z * b.x - b.z * a.x, b.y * a.x - b.x * a.y);
}

// FUNCTION: RALLYTROPHY 0x420430
// SYMBOL: ??D@YA?AVVec3@@MABV0@@Z
Vec3 operator*(float k, const Vec3& v)
{
    return v * k;
}
