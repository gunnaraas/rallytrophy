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
    m->unk24 = 0;
    m->triangleCount = (WORD)(dropCount * 2);
    m->vertexCount = (WORD)(dropCount * 4);
    *(WindowRain* volatile*)&m->owner = this;
    WORD* ix = *((WORD* volatile*)&m->indices);
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
    i = 0;
    if (count > 0) {
        EffectVertex* vx = *((EffectVertex* volatile*)&m->vertices);
        float* normal = &vx[0].normal.x;
        volatile DWORD* color = &vx[0].color;
        float* u = &vx[0].u;
        float* v = &vx[0].v;
        float* u2 = &vx[0].u2;
        float* v2 = &vx[0].v2;
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
    sprite.unk18 = 5;
    sprite.unk1c = 6;
    sprite.unk24 = 4;
    sprite.unk98 = 0xd;
    sprite.unk9c = 2;
    sprite.unk60[0] = 1.0f;
    sprite.unk60[1] = 1.0f;
    sprite.unk60[2] = 1.0f;
    sprite.unk60[3] = 1.0f;
    sprite.unk70[0] = 0.0f;
    sprite.unk70[1] = 0.0f;
    sprite.unk70[2] = 0.0f;
    sprite.unk70[3] = 0.0f;
    viewCount = 0;
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
// 949 vs 955 bytes / 362 differing bytes. Expanding the y-delta expressions removes 37 differences, but the frame
// remains four bytes larger and the bounds-copy registers and x87 comparison stack still differ.
void WindowRain::Init(RainScene* scene)
{
    int i = 0;
    SceneObject* obj;
    for (;;) {
        if (i >= (int)scene->objects.size()) {
            return;
        }
        obj = scene->objects[i];
        if (obj->type == 1) {
            const char* name = obj->name ? obj->name : "";
            if (_stricmp(name, "rain_window") == 0) {
                break;
            }
        }
        i++;
    }
    int a = obj->unk14;
    int c = obj->unk1c;
    memcpy(matrix, obj->matrix, sizeof(matrix));
    unk18 = obj->unk18;
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
    Vec3* src = md->vertices;
    int j;
    for (j = 0; j < n; j++, s++, src++) {
        s->world.x = src->x;
        s->world.y = src->y;
        s->world.z = src->z;
        s->right.y = 0.0f;
        s->right.z = 0.0f;
        s->up.x = 0.0f;
        s->up.z = 0.0f;
        float dx = s->world.x - viewPos[0].x;
        s->angleA = s->angleB = 100.0f;
        s->right.x = 1.0f;
        s->up.y = 1.0f;
        float d = (s->world.y - viewPos[0].y) * (s->world.y - viewPos[0].y) + dx * dx;
        if (d >= r1 && d <= r2) {
            float ang = (float)atan2(s->world.y - viewPos[0].y, dx);
            if (ang >= min0 && ang <= max0) {
                s->angleA = ang;
            }
        }
        if (viewCount > 1) {
            dx = s->world.x - viewPos[1].x;
            d = (s->world.y - viewPos[1].y) * (s->world.y - viewPos[1].y) + dx * dx;
            if (d >= r1 && d <= r2) {
                float ang = (float)atan2(s->world.y - viewPos[1].y, dx);
                if (ang >= min1 && ang <= max1) {
                    s->angleB = ang;
                }
            }
        }
    }
    RainDrop* dr = drops;
    for (j = 0; j < dropCount; j++, dr++) {
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
