// Car collision broadphase members (0x4581f0-0x4586cf, module end pair 0x4586d0/0x4586f0).
// Map: analysis/range-4563e0.md section 2.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"

extern float DAT_004e13a0;
extern "C" int fun_004ad1d0();
extern "C" int fun_0041d7d0();

// The terrain/stage object at Car+0x14fc: collects the surface triangles inside a sphere.
struct M2_Sphere {
    Vec3 center;
    float radius;
};

// The query object at Car+0x1464: an index vector followed by the world box and the sphere pointer.
class M2_Query {
public:
    std::vector<int> v;                // 0x00: first at +4, last at +8
    BYTE unk10[4];
    float minX, minY, minZ;            // 0x14
    float maxX, maxY, maxZ;            // 0x20
    M2_Sphere* sphere;                 // 0x2c
    int mode;                          // 0x30
};

struct M2_Stage {
    void Collect(M2_Query* query, M2_Sphere* sphere); // 0x4114c0
};

// The local box of the body (body+0x234).
struct M2_Box {
    BYTE unk0[4];
    float lo[3];                       // 0x04
    float hi[3];                       // 0x10
    BYTE unk1c[0x40 - 0x1c];
};

struct M2_Body {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    BYTE unk20[0x234 - 0x20];
    M2_Box* box;                       // 0x234
    int FUN_0041d7d0(M2_Body* other, int flag);     // 0x41d7d0: body collision
    void FUN_0041fc60(void* car, M2_Stage* stage);  // 0x41fc60
    void FUN_0041c010();                            // 0x41c010
};

struct M2_Car {
    BYTE unk0[0x24];
    M2_Body* body;                     // 0x24
    BYTE unk28[0x1464 - 0x28];
    M2_Query query;                    // 0x1464
    BYTE unk1498[0x14f4 - 0x1498];
    int hits;                          // 0x14f4
    int applied;                       // 0x14f8
    M2_Stage* stage;                   // 0x14fc
};

// Transforms *point by the rotation *matrix (rows 0x10 apart), adds *trans and stores the result in *result.
#define M2_TRANSFORM_ADD() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
    __asm mov ebx, trans \
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
    __asm fadd dword ptr [ebx] \
    __asm fxch st(1) \
    __asm fadd dword ptr [ebx+4] \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx+8] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// Adds the edge e to the lower or upper corner of the box, per component, by the sign bit of e.
inline void Extend(const Vec3& e, Vec3& mn, Vec3& mx)
{
    unsigned int sign = 0x80000000;
    if (sign & *(unsigned int*)&e.x) mn.x += e.x; else mx.x += e.x;
    if (sign & *(unsigned int*)&e.y) mn.y += e.y; else mx.y += e.y;
    if (sign & *(unsigned int*)&e.z) mn.z += e.z; else mx.z += e.z;
}

// Registers the car's world box (the body's local box, enlarged, rotated into the world) with the stage.
// NONMATCHING: RALLYTROPHY 0x4581f0
// 931 vs 923 bytes. Logic and EH state order (lo, hi, ax, ay, az, mn, mx, h, sphere) match; the frame layout (h sits above ay in
// the original) and the sign-test code (original loads the edge component on the FPU before the test) do not.
// original: FUN_004581f0
void Car::FUN_004581f0()
{
    M2_Car* v = (M2_Car*)this;
    M2_Body* body = v->body;
    M2_Box* box = body->box;
    Vec3 lo(*(Vec3*)box->lo);
    Vec3 hi(*(Vec3*)box->hi);
    const float* matrix = (const float*)body + 8;
    const float* trans = body->pos;
    lo.x -= 1.0f;
    lo.y -= 1.5f;
    lo.z -= 1.0f;
    hi.x += 1.0f;
    hi.y += 1.0f;
    hi.z += 1.0f;
    Vec3 ax(*(Vec3*)(matrix));
    Vec3 ay(*(Vec3*)(matrix + 4));
    Vec3 az(*(Vec3*)(matrix + 8));
    float dx = hi.x - lo.x;
    ax.x *= dx;
    ax.y *= dx;
    ax.z *= dx;
    float dy = hi.y - lo.y;
    ay.x *= dy;
    ay.y *= dy;
    ay.z *= dy;
    float dz = hi.z - lo.z;
    az.x *= dz;
    az.y *= dz;
    az.z *= dz;
    Vec3 mn;
    {
        const float* point = (const float*)&lo;
        float* result = (float*)&mn;
        M2_TRANSFORM_ADD();
    }
    Vec3 mx(mn);
    Extend(ax, mn, mx);
    Extend(ay, mn, mx);
    Extend(az, mn, mx);
    Vec3 h;
    h.x = mx.x - mn.x;
    h.y = mx.y - mn.y;
    h.z = mx.z - mn.z;
    h.x *= 0.5f;
    h.y *= 0.5f;
    h.z *= 0.5f;
    M2_Sphere sphere;
    sphere.radius = h.Length();
    h.x += mn.x;
    h.y += mn.y;
    h.z += mn.z;
    sphere.center = h;
    M2_Query* q = &v->query;
    q->v.erase(q->v.begin(), q->v.end());
    q->mode = 0x11;
    q->sphere = &sphere;
    q->minX = mn.x;
    q->minY = mn.y;
    q->minZ = mn.z;
    q->maxX = mx.x;
    q->maxY = mx.y;
    q->maxZ = mx.z;
    v->stage->Collect(q, &sphere);
}

// Tests this car against another: when the bodies are close enough and their boxes overlap, both count a hit.
// original: FUN_00458590
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x458590
int Car::FUN_00458590(Car* other)
{
    __asm {
        push -1
        push 0x4da878
        mov eax, dword ptr fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 0xc
        mov eax, dword ptr [esp + 0x1c]
        push ebp
        mov ebp, ecx
        push esi
        mov esi, dword ptr [ebp + 0x24]
        mov edx, dword ptr [esi + 0x18]
        fld dword ptr [esi + 0x14]
        lea ecx, [esi + 0x14]
        push edi
        mov edi, dword ptr [eax + 0x24]
        mov eax, dword ptr [ecx + 8]
        mov dword ptr [esp + 0x10], edx
        mov dword ptr [esp + 0x14], eax
        fsub dword ptr [edi + 0x14]
        lea edx, [edi + 0x14]
        mov dword ptr [esp + 0x20], 0
        fstp dword ptr [esp + 0xc]
        fld dword ptr [esp + 0x10]
        fsub dword ptr [edx + 4]
        fstp dword ptr [esp + 0x10]
        fld dword ptr [esp + 0x14]
        fsub dword ptr [edx + 8]
        fst dword ptr [esp + 0x14]
        fmul dword ptr [esp + 0x14]
        fld dword ptr [esp + 0x10]
        fmul dword ptr [esp + 0x10]
        faddp st(1), st
        fld dword ptr [esp + 0xc]
        fmul dword ptr [esp + 0xc]
        faddp st(1), st
        fcomp dword ptr [DAT_004e13a0]
        fnstsw ax
        test ah, 0x41
        je car_collision_done
        mov eax, dword ptr [esi + 0x234]
        push ebx
        mov ebx, dword ptr [edi + 0x234]
        push edx
        push ecx
        lea ecx, [edi + 0x20]
        push ecx
        lea edx, [esi + 0x20]
        push edx
        add ebx, 4
        add eax, 4
        push ebx
        push eax
        call fun_004ad1d0
        add esp, 0x18
        test eax, eax
        pop ebx
        je car_collision_done
        push 1
        push edi
        mov ecx, esi
        call fun_0041d7d0
        test eax, eax
        je car_collision_done
        mov eax, dword ptr [esp + 0x28]
        mov edx, dword ptr [ebp + 0x14f4]
        mov ecx, dword ptr [eax + 0x14f4]
        inc edx
        inc ecx
        mov dword ptr [ebp + 0x14f4], edx
        mov dword ptr [eax + 0x14f4], ecx
    car_collision_done:
        mov ecx, dword ptr [esp + 0x18]
        pop edi
        pop esi
        xor eax, eax
        pop ebp
        mov dword ptr fs:[0], ecx
        add esp, 0x18
        ret 4
    }
}

// Applies the collisions counted since the last call to the body.
// FUNCTION: RALLYTROPHY 0x458690
// original: FUN_00458690
void Car::FUN_00458690()
{
    M2_Car* v = (M2_Car*)this;
    if (v->hits > 0) {
        v->applied = 1;
        M2_Body* body = v->body;
        body->FUN_0041fc60(this, v->stage);
        body->FUN_0041c010();
        v->hits = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x4586d0
// SYMBOL: _$E23
typedef void Generated_4586d0();

// FUNCTION: RALLYTROPHY 0x4586f0
// SYMBOL: _$E22
typedef void Generated_4586f0();
