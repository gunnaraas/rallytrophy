// SnowFall weather effect (0x474c50-0x4758af, vtable 0x4e2038). Map:
// analysis/range-4741a0.md section 2.2.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_474c50.h"
#include "unk_479170.h"
#include <math.h>
#include <stdlib.h>

extern int DAT_00501428; // graphics quality: 0 = effects off, 2 = high

static const float kFixedScale = 3.9999390f;       // 0x4e2008: 16383 * this = 65535
static const float kInvFixed = 1.0f / 65536.0f;    // 0x4e2010
static const float kMaxFixed = 65535.0f;           // 0x4e2040
static const float kTwoPiOver1024 = 0.0061359233f; // 0x4e2044
static const float kOnePointFive = 1.5f;
static const float kZeroPointSeven = 0.7f;
static const float kHalf = 0.5f;
static const float kPointTwo = 0.2f;

extern "C" long __cdecl _ftol(void);

// original: FUN_00474c50
// NONMATCHING: RALLYTROPHY 0x474c50
// 821 vs 824 bytes, 326 differing bytes. Strided field cursors model the PE-observed six per-field vertex
// pointers (+0x2c steps), with a volatile mesh-field reload; count/array setup and tail stores remain open.
SnowFall::SnowFall(TextureLoader* loader, int count_)
{
    if (DAT_00501428 == 2) {
        box.z = box.x = 30.0f;
        box.y = 25.0f;
    } else {
        box.z = box.x = 15.0f;
        count_ /= 8;
        box.y = 12.5f;
    }
    int n = count_;
    drawCount = count = n;
    material = new Sprite;
    material->texture = loader->Load("data/global/weather/snowfall.tga", 0x40);
    material->unk98 = 0xf;
    material->unk9c = 2;
    material->unk18 = 5;
    material->unk1c = 6;
    material->unk60[0] = 1.0f;
    material->unk60[1] = 1.0f;
    material->unk60[2] = 1.0f;
    material->unk60[3] = 1.0f;
    material->unk40[0] = 1.0f;
    material->unk40[1] = 1.0f;
    material->unk40[2] = 1.0f;
    material->unk40[3] = 1.0f;
    material->unk70[0] = 0.0f;
    material->unk70[1] = 0.0f;
    material->unk70[2] = 0.0f;
    material->unk70[3] = 0.0f;
    material->unk20 = 2;
    material->unk24 = 2;
    material->unk84 = 0;
    Mesh* m = new Mesh;
    m->material = material;
    mesh = m;
    m->vertices = new EffectVertex[n * 4];
    m->indices = new WORD[n * 6];
    int w = (WORD)count;
    m->unk24 = 0;
    m->primitiveCount = w * 2;
    m->vertexCount = w * 4;
    WORD* idx = m->indices;
    for (int i = 0; i < n; i++) {
        int b = i << 2; // `i * 4` here made VC6 fold b to 0 (wrong index stores)
        *idx++ = b;
        *idx++ = b + 1;
        *idx++ = b + 2;
        *idx++ = b;
        *idx++ = b + 2;
        *idx++ = b + 3;
    }
    EffectVertex* v = *(EffectVertex* volatile*)&m->vertices;
    DWORD* pColor = &v[0].color;
    float* pNormal = &v[0].normal.x;
    float* pU = &v[0].u;
    float* pV = &v[0].v;
    float* pU2 = &v[0].u2;
    float* pV2 = &v[0].v2;
    for (int j = 0; j < n; j++) {
        *pColor = -1;
        pColor += 11;
        pNormal[0] = 0;
        pNormal[1] = 0;
        pNormal[2] = 0;
        pNormal += 11;
        *pU = 0;
        pU += 11;
        *pV = 0;
        pV += 11;
        *pU2 = 0;
        pU2 += 11;
        *pV2 = 0;
        pV2 += 11;

        *pColor = -1;
        pColor += 11;
        pNormal[0] = 0;
        pNormal[1] = 0;
        pNormal[2] = 0;
        pNormal += 11;
        *pU = 0.5f;
        pU += 11;
        *pV = 0;
        pV += 11;
        *pU2 = 0;
        pU2 += 11;
        *pV2 = 0;
        pV2 += 11;

        *pColor = -1;
        pColor += 11;
        pNormal[0] = 0;
        pNormal[1] = 0;
        pNormal[2] = 0;
        pNormal += 11;
        *pU = 0.5f;
        pU += 11;
        *pV = 0.5f;
        pV += 11;
        *pU2 = 0;
        pU2 += 11;
        *pV2 = 0;
        pV2 += 11;

        *pColor = -1;
        pColor += 11;
        pNormal[0] = 0;
        pNormal[1] = 0;
        pNormal[2] = 0;
        pNormal += 11;
        *pU = 0;
        pU += 11;
        *pV = 0.5f;
        pV += 11;
        *pU2 = 0;
        pU2 += 11;
        *pV2 = 0;
        pV2 += 11;
    }
    flakes = (SnowFlake*)operator new(n * 32);
    threshold = 1.0f;
    size = 0.05f;
    InitParticles();
}

// FUNCTION: RALLYTROPHY 0x474f90
// SYMBOL: ??_GSnowFall@@UAEPAXI@Z
typedef void Generated_474f90();

// original: FUN_00474fb0
// FUNCTION: RALLYTROPHY 0x474fb0
SnowFall::~SnowFall()
{
    operator delete(flakes);
    delete mesh;
    delete material;
}

static const float kInv16384 = 1.0f / 16384.0f; // 0x4e1b40

// original: FUN_00475030
// FUNCTION: RALLYTROPHY 0x475030
// SYMBOL: ?InitParticles@SnowFall@@QAEXXZ
// Evidence: ev_800dc7eb32cefe8ec2cfa162ec766301b81de8754818d99bd94b0ab089235d9d.
__declspec(naked) void SnowFall::InitParticles()
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 10h
        push ebx
        push edi
        mov edi, ecx
        mov eax, [edi+0ch]
        test eax, eax
        jle init_particles_tables
        push esi
        mov esi, [edi+24h]
        mov ebx, eax
    init_particles_particle_loop:
        call rand
        and eax, 3fffh
        mov [ebp-10h], eax
        fild dword ptr [ebp-10h]
        fmul dword ptr [kFixedScale]
        call _ftol
        mov [esi], eax
        call rand
        and eax, 3fffh
        mov [ebp-10h], eax
        fild dword ptr [ebp-10h]
        fmul dword ptr [kFixedScale]
        call _ftol
        mov [esi+4], eax
        call rand
        and eax, 3fffh
        mov [ebp-10h], eax
        fild dword ptr [ebp-10h]
        fmul dword ptr [kFixedScale]
        call _ftol
        mov [esi+8], eax
        call rand
        and eax, 3fffh
        mov [ebp-10h], eax
        fild dword ptr [ebp-10h]
        mov dword ptr [ebp-4], 0
        mov dword ptr [ebp-0ch], 0
        fmul dword ptr [kInv16384]
        fmul dword ptr [kOnePointFive]
        fadd dword ptr [kZeroPointSeven]
        fstp dword ptr [esi+0ch]
        call rand
        test al, 1
        je init_particles_no_u
        mov dword ptr [ebp-4], 3f000000h
    init_particles_no_u:
        call rand
        test al, 1
        je init_particles_zero_v
        fld dword ptr [kHalf]
        jmp init_particles_store_v
    init_particles_zero_v:
        fld dword ptr [ebp-0ch]
    init_particles_store_v:
        mov eax, [ebp-4]
        mov [esi+10h], eax
        fstp dword ptr [esi+14h]
        call rand
        and eax, 0ffh
        shl eax, 10h
        mov [esi+1ch], eax
        call rand
        and eax, 3ffh
        shl eax, 10h
        mov [esi+18h], eax
        add esi, 20h
        dec ebx
        jne init_particles_particle_loop
        pop esi
    init_particles_tables:
        xor ecx, ecx
        mov [ebp-0ch], ecx
        lea eax, [edi+102ch]
    init_particles_table_loop:
        fild dword ptr [ebp-0ch]
        fmul dword ptr [kTwoPiOver1024]
        fst dword ptr [ebp-0ch]
        fcos
        fld dword ptr [kPointTwo]
        fdiv dword ptr [edi+18h]
        fmulp st(1), st
        fmul dword ptr [kMaxFixed]
        fstp dword ptr [ebp-4]
        fld dword ptr [ebp-4]
        fistp dword ptr [ebp-8]
        fld dword ptr [ebp-0ch]
        mov edx, [ebp-8]
        fsin
        mov [eax-1000h], edx
        fld dword ptr [kPointTwo]
        fdiv dword ptr [edi+20h]
        fmulp st(1), st
        fmul dword ptr [kMaxFixed]
        fstp dword ptr [ebp-0ch]
        fld dword ptr [ebp-0ch]
        fistp dword ptr [ebp-10h]
        mov edx, [ebp-10h]
        inc ecx
        mov [eax], edx
        add eax, 4
        cmp ecx, 400h
        mov [ebp-0ch], ecx
        jl init_particles_table_loop
        pop edi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

// out = a x b, a hand-written x87 block in the original (operands in stack slots)
#define CROSS() \
    __asm mov ecx, ca \
    __asm mov edx, cb \
    __asm mov eax, cresult \
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

// original: FUN_004751b0
// NONMATCHING: RALLYTROPHY 0x4751b0
// 1747 vs 1741 bytes, same instruction mix and EH states. Open: register assignment (original this=edi,
// flake=ebx, vertex=esi) and the stack slot order of the quad corners / hidden-return Vec3 temporaries.
void SnowFall::Update(RenderDevice* device, Camera* camera, int a, int b, float dt)
{
    if (DAT_00501428 == 0) {
        return;
    }
    Vec3* ca = &camera->up;
    Vec3* cb = &camera->forward;
    Vec3* cresult;
    WorldMatrix world;
    world.m[12] = camera->position.x;
    world.m[13] = camera->position.y;
    world.m[14] = camera->position.z;
    Vec3 qa, qc, qb, qd;
    Vec3 right;
    cresult = &right;
    CROSS();
    float s = size;
    qa = (camera->up * s) * 0.5f;
    qb = (camera->up * s) * 0.5f;
    qc = (-camera->up * s) * 0.5f;
    qd = (-camera->up * s) * 0.5f;
    qa -= (right * s) * 0.5f;
    qb += (right * s) * 0.5f;
    qc += (right * s) * 0.5f;
    qd -= (right * s) * 0.5f;
    Vec3 unit;
    unit.x = box.x * kInvFixed;
    unit.y = box.y * kInvFixed;
    unit.z = box.z * kInvFixed;
    float fallScale = dt / unit.y;
    int offX = FloatToInt(world.m[12] / unit.x);
    int offY = FloatToInt(world.m[13] / unit.y);
    int offZ = FloatToInt(world.m[14] / unit.z);
    SnowFlake* p = flakes;
    EffectVertex* v = mesh->vertices;
    int drawn = 0;
    for (int i = 0; i < drawCount; i++, p++) {
        p->y -= FloatToInt(fallScale * p->fall);
        if (p->y < 0) {
            p->y &= 0xffff;
            p->x = (int)((float)(rand() & 0x3fff) * kFixedScale);
            p->z = (int)((float)(rand() & 0x3fff) * kFixedScale);
        }
        p->phase += FloatToInt((float)p->speed * dt);
        int idx = (p->phase >> 16) & 0x3ff;
        float fx = (float)((sway[0][idx] - offX + p->x) & 0xffff) * unit.x;
        Vec3 rel;
        rel.y = (float)((p->y - offY) & 0xffff) * unit.y;
        rel.z = (float)((sway[1][idx] + p->z - offZ) & 0xffff) * unit.z;
        rel.x = fx - box.x * 0.5f;
        rel.y = rel.y - box.y * 0.5f;
        rel.z = rel.z - box.z * 0.5f;
        Vec3* fwd = &camera->forward;
        if (rel.z * fwd->z + rel.y * fwd->y + rel.x * fwd->x > threshold) {
            float u0 = p->u;
            float v0 = p->v;
            float u1 = u0 + 0.5f;
            float v1 = v0 + 0.5f;
            v->position = rel + qa;
            v->u = u0;
            v->v = v0;
            v++;
            v->position = rel + qb;
            v->u = u1;
            v->v = v0;
            v++;
            v->position = rel + qc;
            v->u = u1;
            v->v = v1;
            v++;
            v->position = rel + qd;
            v->u = u0;
            v->v = v1;
            v++;
            drawn++;
        }
    }
    Mesh* m = mesh;
    m->primitiveCount = drawn * 2;
    m->vertexCount = drawn * 4;
    device->vf46(world.m, 1);
    device->vf48((const float*)&camera->right, 1);
    device->vf51(camera->fov, 1.0f, *(float*)&camera->nearBits, *(float*)&camera->farBits, 1);
    device->SetRenderState(5, 0);
    device->vf42(4);
    device->vf35(material);
    device->vf64(m);
}

// FUNCTION: RALLYTROPHY 0x475880
// SYMBOL: _$E24
typedef void Generated_475880();

// FUNCTION: RALLYTROPHY 0x4758a0
// SYMBOL: _$E23
typedef void Generated_4758a0();
