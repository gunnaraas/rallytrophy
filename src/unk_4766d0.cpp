// WindowRain (0x4766d0-0x47736f, vtable 0x4e206c). Map: analysis/range-4741a0.md section 2.6.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_476490.h"
#include "unk_479170.h"
#include "unk_4766d0.h"
#include <math.h>
#include <stdlib.h>

extern float DAT_004e1348;
extern "C" void fun_004c1404();
extern "C" void fun_004c1998();

// NONMATCHING: RALLYTROPHY 0x4766d0
// 916 vs 924 bytes / 369 differing bytes. Continuous field streams, a guarded do/while, and delayed UV offsets
// approach the target quad layout; FPU/store scheduling still differs.
// Tried 2026-10-30 night: ~25 variants (volatile/plain vertices+color, guard placement, store/zero-reg order, u1/v1 hoist, tail store reorder); only the tail store order helped (49->45); the quad loop interleaves FPU stores across vertices and the 8-byte size gap is unexplained.
// Tried 2026-10-31 re-pass: cursor pointers declared reverse (v2,u2,v,u,normal,color) fixes the lea/register order; vx load hoisted above the guard (45->39); ~14 more variants (plain color, volatile u/v/u2, u1/v1 hoist, u0/v0 swap, for-loop) no better; original defers u/v fstp across vertices with pending 0x2c adds folded into disp (e.g. [edx-0x2c]).
// 2026-11-11: counts, owner and array pointers through this->mesh (no volatile reloads) and the tail stores in the
// original order (unk70[2] and viewCount right after the unk60 block): 357 -> 344 bytes differ; the head now matches
// up to the quad loop, whose FPU/store interleaving is the remaining difference.
WindowRain::WindowRain(int count)
{
    drops = 0;
    spots = 0;
    spotCount = 0;
    dropCount = count;
    drops = (RainDrop*)operator new(count * 8);
    DynamicMesh* m = new DynamicMesh;
    m->material = &sprite;
    mesh = m;
    m->vertices = new EffectVertex[count * 4];
    m->indices = new WORD[count * 6];
    mesh->unk24 = 0;
    mesh->triangleCount = (WORD)(dropCount * 2);
    mesh->vertexCount = (WORD)(dropCount * 4);
    mesh->owner = this;
    WORD* ix = mesh->indices;
    int i;
    for (i = 0; i < count; i++) {
        WORD v = (WORD)(i * 4);
        *ix++ = v;
        *ix++ = v + 1;
        *ix++ = v + 2;
        *ix++ = v;
        *ix++ = v + 2;
        *ix++ = v + 3;
    }
    EffectVertex* vx = mesh->vertices;
    i = 0;
    if (count > 0) {
        float* v2 = &vx[0].v2;
        float* u2 = &vx[0].u2;
        float* v = &vx[0].v;
        float* u = &vx[0].u;
        float* normal = &vx[0].normal.x;
        volatile DWORD* color = &vx[0].color;
        do {
            float u0 = (float)(i & 3) * 0.0625f + 0.75f;
            float v0 = (float)((i >> 2) & 3) * 0.0625f + 0.75f;
            normal[0] = 0.0f;
            normal[1] = 0.0f;
            normal[2] = -1.0f;
            *color = 0xffffffff;
            *u2 = 0.0f;
            *v2 = 0.0f;
            *u = u0;
            *v = v0;
            normal += 11; color += 11; u += 11; v += 11; u2 += 11; v2 += 11;
            float u1 = u0 + 0.0624f;
            float v1 = v0 + 0.0624f;
            normal[0] = 0.0f;
            normal[1] = 0.0f;
            normal[2] = -1.0f;
            *color = 0xffffffff;
            *u = u1;
            *v = v0;
            *u2 = 0.0f;
            *v2 = 0.0f;
            normal += 11; color += 11; u += 11; v += 11; u2 += 11; v2 += 11;
            normal[0] = 0.0f;
            normal[1] = 0.0f;
            normal[2] = -1.0f;
            *color = 0xffffffff;
            *u = u1;
            *v = v1;
            *u2 = 0.0f;
            *v2 = 0.0f;
            normal += 11; color += 11; u += 11; v += 11; u2 += 11; v2 += 11;
            normal[0] = 0.0f;
            normal[1] = 0.0f;
            normal[2] = -1.0f;
            *color = 0xffffffff;
            *u = u0;
            *v = v1;
            *u2 = 0.0f;
            *v2 = 0.0f;
            normal += 11; color += 11; u += 11; v += 11; u2 += 11; v2 += 11;
        } while (++i < count);
    }
    sprite.unk60[2] = 1.0f;
    sprite.unk60[0] = 1.0f;
    sprite.unk60[1] = 1.0f;
    sprite.unk60[3] = 1.0f;
    sprite.unk70[2] = 0.0f;
    viewCount = 0;
    sprite.unk98 = 0xd;
    sprite.unk9c = 2;
    sprite.unk18 = 5;
    sprite.unk1c = 6;
    sprite.unk70[0] = 0.0f;
    sprite.unk70[1] = 0.0f;
    sprite.unk70[3] = 0.0f;
    sprite.unk24 = 4;
    unk8 = 0;
}

// FUNCTION: RALLYTROPHY 0x476a70
// SYMBOL: ??_GWindowRain@@UAEPAXI@Z
typedef void Generated_476a70();

// FUNCTION: RALLYTROPHY 0x476a90
WindowRain::~WindowRain()
{
    operator delete(drops);
    delete[] spots;
    if (mesh != 0) {
        delete mesh;
    }
}

// FUNCTION: RALLYTROPHY 0x476b70
void WindowRain::AddView(int unused, const Vec3* position, float a, float b, float c, float d)
{
    int i = viewCount;
    viewPos[i] = *position;
    viewSize.x = a;
    viewSize.y = b;
    viewRange[i].x = c;
    viewRange[i].y = d;
    viewCount++;
}

// FUNCTION: RALLYTROPHY 0x477080
void WindowRain::vf6(RenderDevice* device, Camera* camera, int unused)
{
    Render(device, camera);
}

// NONMATCHING: RALLYTROPHY 0x476be0
// 955 vs 955 bytes; 72 -> 36 insns differ (cursor/idiom pass 2026-11-01).
// Tried 2026-10-30 night: dy as a local, int copy of the vertex into a Vec3* w = &s->world (gets the ecx=&world.z base, frame size now matches; 127->72 insns); separate angle pointers, store reorders, indexed spots[j] did not add the original second induction pointer (edx=s) or its imm 100.0 stores; bounds-copy register rotation still differs.
// Tried 2026-11-01: cursor float* cz=&s->world.z re-derived in the for-increment (keeps edx=s and ecx separate), dy declared after the right/up constant stores, drops loop as indexed dr=&drops[j]; 72->36 insns, size 955 matches. Left: bounds-copy register rotation (orig starts eax, ours edx; bounds.w/struct copy/vmd/reverse order no help), zero/imm store order in loop, add ecx,0x2c vs lea ecx,[edx+0x10]; volatile barriers on cz stores only reach 34.
void WindowRain::Init(RainScene* scene)
{
    int i = 0;
    SceneObject* obj;
    for (;;) {
        if (i >= (int)scene->objects.size()) {
            return;
        }
        int objBits = *(int*)&scene->objects[i]; // int-typed copies (found by permute.py): 36 -> 8 insns
        *(int*)&obj = objBits;
        if (obj->type == 1) {
            const char* name = obj->name ? obj->name : "";
            if (_stricmp(name, "rain_window") == 0) {
                break;
            }
        }
        ++i;
    }
    int a = obj->unk14;
    int c = obj->unk1c;
    memcpy(matrix, obj->matrix, sizeof(matrix));
    int unk18Bits = *(int*)&obj->unk18;
    *(int*)&unk18 = unk18Bits;
    unk14 = a;
    unk1c = c;
    WindowMeshData* md = obj->source.GetMeshData(0);
    if (md == 0) {
        return;
    }
    int n = md->vertexCount;
    if (n <= 0 || viewCount == 0) {
        return;
    }
    bounds.w[0] = md->bounds.w[0];
    bounds.w[1] = md->bounds.w[1];
    bounds.w[2] = md->bounds.w[2];
    bounds.w[3] = md->bounds.w[3];
    bounds.w[4] = md->bounds.w[4];
    bounds.w[5] = md->bounds.w[5];
    bounds.w[6] = md->bounds.w[6];
    bounds.w[7] = md->bounds.w[7];
    bounds.w[8] = md->bounds.w[8];
    bounds.w[9] = md->bounds.w[9];
    bounds.w[10] = md->bounds.w[10];
    flags = md->flags;
    spotCount = n;
    spots = new RainSpot[n];
    float r1 = viewSize.x * viewSize.x;
    float r2 = viewSize.y * viewSize.y;
    float min0 = viewRange[0].x < viewRange[0].y ? viewRange[0].x : viewRange[0].y;
    float max0 = viewRange[0].x > viewRange[0].y ? viewRange[0].x : viewRange[0].y;
    float min1 = viewRange[1].x < viewRange[1].y ? viewRange[1].x : viewRange[1].y;
    float max1 = viewRange[1].x > viewRange[1].y ? viewRange[1].x : viewRange[1].y;
    RainSpot* s = spots;
    float* cz = &s->world.z;
    Vec3* src = md->vertices;
    int j;
    for (j = 0; j < n; j++, s++, src++, cz = &s->world.z) {
        cz[-2] = src->x; cz[-1] = src->y; cz[0] = src->z;
        cz[3] = 0.0f;
        cz[6] = 0.0f;
        float dx = cz[-2] - viewPos[0].x;
        s->angleA = 100.0f; cz[-3] = 100.0f;
        cz[1] = 1.0f;
        cz[2] = 0.0f;
        cz[5] = 1.0f;
        cz[4] = 0.0f;
        float dy = cz[-1] - viewPos[0].y;
        float d = dy * dy + dx * dx;
        if (d >= r1 && d <= r2) {
            float ang = (float)atan2(dy, dx);
            if (ang >= min0 && ang <= max0) {
                s->angleA = ang;
            }
        }
        if (viewCount > 1) {
            dx = cz[-2] - viewPos[1].x;
            dy = cz[-1] - viewPos[1].y;
            d = dy * dy + dx * dx;
            if (d >= r1 && d <= r2) {
                float ang = (float)atan2(dy, dx);
                if (ang >= min1 && ang <= max1) {
                    cz[-3] = ang;
                }
            }
        }
    }
    RainDrop* dr;
    for (j = 0; j < dropCount; ++j) {
        dr = &drops[j];
        dr->age = (float)j / (float)dropCount;
        dr->spot = &spots[rand() % n];
    }
}

static const float kTwo = 2.0f; // 0x4e2064

// original: FUN_00476fa0
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x476fa0
void WindowRain::Update(float dt, float xMin, float xMax, float yMin, float yMax)
{
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0xfffffff8
        sub esp, 0xc
        fld dword ptr [ebp + 8]
        push ebx
        fdiv dword ptr [kTwo]
        push esi
        push edi
        mov edi, ecx
        mov eax, dword ptr [edi + 0x230]
        test eax, eax
        fstp dword ptr [esp + 0x14]
        jle window_rain_done
        mov esi, dword ptr [edi + 0x238]
        mov ebx, eax
    window_rain_loop:
        fld dword ptr [esp + 0x14]
        fadd dword ptr [esi]
        fst dword ptr [esp + 0x10]
        fstp dword ptr [esi]
        fld dword ptr [esp + 0x10]
        fcomp dword ptr [DAT_004e1348]
        fnstsw ax
        test ah, 1
        je window_rain_recycle
        mov ecx, dword ptr [esi + 4]
        fld dword ptr [ecx]
        fcomp dword ptr [ebp + 0xc]
        fnstsw ax
        test ah, 1
        jne window_rain_check_second_view
        fld dword ptr [ecx]
        fcomp dword ptr [ebp + 0x10]
        fnstsw ax
        test ah, 0x41
        jne window_rain_recycle
    window_rain_check_second_view:
        cmp dword ptr [edi + 0x1f8], 2
        jne window_rain_advance
        fld dword ptr [ecx + 4]
        fcomp dword ptr [ebp + 0x14]
        fnstsw ax
        test ah, 1
        jne window_rain_advance
        fld dword ptr [ecx + 4]
        fcomp dword ptr [ebp + 0x18]
        fnstsw ax
        test ah, 0x41
        je window_rain_advance
    window_rain_recycle:
        fld dword ptr [esp + 0x10]
        sub esp, 8
        fstp qword ptr [esp]
        call fun_004c1404
        fsubr dword ptr [esp + 0x18]
        add esp, 8
        fstp dword ptr [esi]
        call fun_004c1998
        cdq
        idiv dword ptr [edi + 0x234]
        lea eax, [edx + edx*4]
        lea ecx, [edx + eax*2]
        mov edx, dword ptr [edi + 0x23c]
        lea eax, [edx + ecx*4]
        mov dword ptr [esi + 4], eax
    window_rain_advance:
        add esi, 8
        dec ebx
        jne window_rain_loop
    window_rain_done:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret 0x14
        }
}

// FUNCTION: RALLYTROPHY 0x4770a0
void WindowRain::Render(RenderDevice* device, Camera* camera)
{
    EffectVertex* v = mesh->vertices;
    Color color;
    color.value = 0xffffffff;
    int i;
    for (i = 0; i < dropCount; i++) {
        RainSpot* sp = drops[i].spot;
        float size = drops[i].age * 0.01f + 0.002f;
        color.a = (BYTE)FloatToInt((1.0f - drops[i].age) * 255.0f);
        v->position.x = sp->world.x;
        v->position.y = sp->world.y;
        v->position.z = sp->world.z;
        v->position += sp->up * size;
        v->position -= sp->right * size;
        v->color = color.value;
        v++;
        v->position.x = sp->world.x;
        v->position.y = sp->world.y;
        v->position.z = sp->world.z;
        v->position += sp->up * size;
        v->position += sp->right * size;
        v->color = color.value;
        v++;
        v->position.x = sp->world.x;
        v->position.y = sp->world.y;
        v->position.z = sp->world.z;
        v->position -= sp->up * size;
        v->position += sp->right * size;
        v->color = color.value;
        v++;
        v->position.x = sp->world.x;
        v->position.y = sp->world.y;
        v->position.z = sp->world.z;
        v->position -= sp->up * size;
        v->position -= sp->right * size;
        v->color = color.value;
        v++;
    }
    device->DrawMesh(mesh, 0);
}

// FUNCTION: RALLYTROPHY 0x477340
// SYMBOL: _$E25
typedef void Generated_477340();

// FUNCTION: RALLYTROPHY 0x477360
// SYMBOL: _$E24
typedef void Generated_477360();
