// CarRain (0x46d830-0x46e84f, vtable 0x4e1f3c). Map: analysis/range-46d830.md section 3.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_476490.h"
#include "unk_4766d0.h"
#include "unk_46d830.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

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

static const float kDropSizeStep = 5.371e-5f; // 0x4e1f60: 0.055 / 1024
static const float kDropPeriod = 0.25f;       // 0x4e1f38: seconds a drop takes to run its course

// dst += o with the operands added in the original's order (o first).
inline void AddRev(Vec3* dst, const Vec3& o)
{
    dst->x = o.x + dst->x;
    dst->y = o.y + dst->y;
    dst->z = o.z + dst->z;
}

// Copies a Vec3 through integer registers.
inline void CopyBits(Vec3* dst, const Vec3& src)
{
    int* d = (int*)dst;
    d[0] = *(const int*)&src.x;
    d[1] = *(const int*)&src.y;
    d[2] = *(const int*)&src.z;
}

// NONMATCHING: RALLYTROPHY 0x46d830
// 1117 vs 1048 bytes. The identity/zero store order of the ten-odd member matrices and vectors now equals the
// original's (the hood's tail is deliberately written late), but VC6 hoists the operator new size arithmetic and
// sinks the vptr store, keeps the count in ebx and the mesh pointer in ebp (original: ebp and ebx), and the vertex
// fill loop spills differently (six per-field induction pointers as in the original, other register choice).
CarRain::CarRain(int count)
{
    savedTrunk.m[5] = savedTrunk.m[10] = savedTrunk.m[15] = savedTrunk.m[0] = 1.0f;
    savedTrunk.m[1] = savedTrunk.m[2] = 0.0f;
    savedHood.m[5] = savedHood.m[10] = savedHood.m[15] = savedHood.m[0] = 1.0f;
    savedHood.m[1] = savedHood.m[2] = 0.0f;
    savedTrunk.m[4] = savedTrunk.m[6] = savedTrunk.m[7] = savedTrunk.m[3] = 0.0f;
    savedTrunk.m[8] = savedTrunk.m[9] = savedTrunk.m[11] = 0.0f;
    savedTrunk.m[12] = savedTrunk.m[13] = savedTrunk.m[14] = 0.0f;
    trunkPos.x = trunkPos.y = trunkPos.z = 0.0f;
    texture = 0;
    mesh = 0;
    drops = 0;
    bodyVerts = 0;
    hoodVerts = 0;
    trunkVerts = 0;
    bodyObject = 0;
    trunkObject = 0;
    hoodObject = 0;
    bodyCount = 0;
    hoodCount = 0;
    trunkCount = 0;
    dropCount = count;
    savedHood.m[4] = savedHood.m[6] = savedHood.m[7] = savedHood.m[3] = 0.0f;
    savedHood.m[8] = savedHood.m[9] = savedHood.m[11] = 0.0f;
    savedHood.m[12] = savedHood.m[13] = savedHood.m[14] = 0.0f;
    hoodPos.x = hoodPos.y = hoodPos.z = 0.0f;
    drops = new CarDrop[count];
    DynamicMesh* m = new DynamicMesh;
    mesh = m;
    m->material = 0;
    m->vertices = new EffectVertex[count * 4];
    m->indices = new WORD[count * 6];
    m->unk24 = 0;
    m->triangleCount = (WORD)(dropCount * 2);
    m->vertexCount = (WORD)(dropCount * 4);
    m->owner = 0;
    WORD* ix = m->indices;
    int i;
    for (i = 0; i < count; i++) {
        int v = i << 2;
        *ix++ = v;
        *ix++ = v + 1;
        *ix++ = v + 2;
        *ix++ = v;
        *ix++ = v + 2;
        *ix++ = v + 3;
    }
    EffectVertex* v = m->vertices;
    for (i = 0; i < count; i++) {
        memset(&v->normal, 0, sizeof(v->normal));
        v->color = 0xffffffff;
        v->u = 0.25f;
        v->v = 0.5f;
        v->u2 = 0.0f;
        v->v2 = 0.0f;
        v++;
        memset(&v->normal, 0, sizeof(v->normal));
        v->color = 0xffffffff;
        v->u = 0.5f;
        v->v = 0.5f;
        v->u2 = 0.0f;
        v->v2 = 0.0f;
        v++;
        memset(&v->normal, 0, sizeof(v->normal));
        v->color = 0xffffffff;
        v->u = 0.5f;
        v->v = 1.0f;
        v->u2 = 0.0f;
        v->v2 = 0.0f;
        v++;
        memset(&v->normal, 0, sizeof(v->normal));
        v->color = 0xffffffff;
        v->u = 0.25f;
        v->v = 1.0f;
        v->u2 = 0.0f;
        v->v2 = 0.0f;
        v++;
    }
    unk8 = 0;
}

// FUNCTION: RALLYTROPHY 0x46dc50
// SYMBOL: ??_GCarRain@@UAEPAXI@Z
typedef void Generated_46dc50();

// FUNCTION: RALLYTROPHY 0x46dc70
CarRain::~CarRain()
{
    if (mesh != 0) {
        delete mesh;
    }
    delete[] drops;
    delete[] bodyVerts;
    delete[] hoodVerts;
    delete[] trunkVerts;
}


// Picks a new size and a random rain mesh vertex for a drop (inlined into Init and Update).
inline void CarRain::Respawn(CarDrop* drop)
{
    drop->size = (float)(rand() & 0x3ff) * kDropSizeStep + 0.015f;
    int r = rand() % totalCount;
    if (r < bodyCount) {
        CopyBits(&drop->pos, bodyVerts[r]);
        drop->matrix = &liveBody;
    } else if (r < hoodCount + bodyCount) {
        CopyBits(&drop->pos, hoodVerts[r - bodyCount]);
        drop->matrix = &liveHood;
    } else {
        CopyBits(&drop->pos, trunkVerts[r - hoodCount - bodyCount]);
        drop->matrix = &liveTrunk;
    }
}

// NONMATCHING: RALLYTROPHY 0x46dd80
// 1046 vs 1050 bytes. Same flow and the same int-wise position copies; the original keeps a zero in edi for the
// whole function (locals, vector-null tests, first GetMeshData(0), `cmp eax,edi` after new) and recomputes
// n*12 for the memcpy, where this version keeps the zero in ecx (shared with i) and CSEs n*12 into edi.
void CarRain::Init(RainScene* scene)
{
    WindowMeshData* body = 0;
    WindowMeshData* trunk = 0;
    WindowMeshData* hood = 0;
    int i = 0;
    for (;;) {
        if (i >= (int)scene->objects.size()) {
            break;
        }
        SceneObject* obj = scene->objects[i];
        if (obj->type == 1) {
            const char* name = "";
            if (obj->name != 0) {
                name = obj->name;
            }
            if (_stricmp(name, "rain_body") == 0) {
                body = obj->source.GetMeshData(0);
                memcpy(&savedBody, obj->matrix, sizeof(savedBody));
                {
                    int* dst = (int*)&bodyPos;
                    dst[0] = obj->unk14;
                    dst[1] = obj->unk18;
                    dst[2] = obj->unk1c;
                }
            }
            if (_stricmp(name, "rain_hood") == 0) {
                hood = obj->source.GetMeshData(0);
                memcpy(&savedHood, obj->matrix, sizeof(savedHood));
                {
                    int* dst = (int*)&hoodPos;
                    dst[0] = obj->unk14;
                    dst[1] = obj->unk18;
                    dst[2] = obj->unk1c;
                }
            }
            if (_stricmp(name, "rain_trunk") == 0) {
                trunk = obj->source.GetMeshData(0);
                memcpy(&savedTrunk, obj->matrix, sizeof(savedTrunk));
                {
                    int* dst = (int*)&trunkPos;
                    dst[0] = obj->unk14;
                    dst[1] = obj->unk18;
                    dst[2] = obj->unk1c;
                }
            }
        }
        i++;
    }
    int n = body->vertexCount;
    bodyCount = n;
    bodyVerts = new Vec3[n];
    memcpy(bodyVerts, body->vertices, n * sizeof(Vec3));
    if (trunk != 0) {
        n = trunk->vertexCount;
        trunkCount = n;
        trunkVerts = new Vec3[n];
        memcpy(trunkVerts, trunk->vertices, n * sizeof(Vec3));
    }
    if (hood != 0) {
        n = hood->vertexCount;
        hoodCount = n;
        hoodVerts = new Vec3[n];
        memcpy(hoodVerts, hood->vertices, n * sizeof(Vec3));
    }
    totalCount = trunkCount + hoodCount + bodyCount;
    int count = dropCount;
    if (count > 0) {
        CarDrop* d = drops;
        for (i = 0; i < count; i++, d++) {
            d->phase = (float)i / (float)count;
            Respawn(d);
        }
    }
}

extern float DAT_004e13e4;

// original: FUN_0046e1a0
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x46e1a0
void CarRain::SetBounds(CarBodyObject*, CarBodyObject*, CarBodyObject*)
{
    __asm {
        mov eax, dword ptr [esp + 4]
        fld dword ptr [ecx + 0xc4]
        mov edx, dword ptr [eax + 0xa0]
        fmul dword ptr [DAT_004e13e4]
        mov dword ptr [ecx + 0xa0], edx
        mov edx, dword ptr [eax + 0xa4]
        mov dword ptr [ecx + 0xa4], edx
        fstp dword ptr [ecx + 0xc4]
        mov edx, dword ptr [eax + 0xa8]
        mov dword ptr [ecx + 0xa8], edx
        mov edx, dword ptr [eax + 0xac]
        mov dword ptr [ecx + 0xac], edx
        mov edx, dword ptr [eax + 0xb0]
        mov dword ptr [ecx + 0xb0], edx
        mov edx, dword ptr [eax + 0xb4]
        mov dword ptr [ecx + 0xb4], edx
        mov edx, dword ptr [eax + 0xb8]
        mov dword ptr [ecx + 0xb8], edx
        fld dword ptr [ecx + 0xa0]
        fmul dword ptr [DAT_004e13e4]
        mov edx, dword ptr [eax + 0xbc]
        mov dword ptr [ecx + 0xbc], edx
        mov edx, dword ptr [eax + 0xc0]
        fstp dword ptr [ecx + 0xa0]
        mov dword ptr [ecx + 0xc0], edx
        fld dword ptr [ecx + 0xa4]
        mov edx, dword ptr [eax + 0xc4]
        fmul dword ptr [DAT_004e13e4]
        mov dword ptr [ecx + 0xc4], edx
        mov edx, dword ptr [eax + 0xc8]
        mov dword ptr [ecx + 0xc8], edx
        fstp dword ptr [ecx + 0xa4]
        mov dl, byte ptr [eax + 0xcc]
        fld dword ptr [ecx + 0xa8]
        mov byte ptr [ecx + 0xcc], dl
        fmul dword ptr [DAT_004e13e4]
        mov edx, dword ptr [esp + 0xc]
        mov dword ptr [ecx + 0x2f4], eax
        mov eax, dword ptr [esp + 8]
        fstp dword ptr [ecx + 0xa8]
        mov dword ptr [ecx + 0x2fc], eax
        fld dword ptr [ecx + 0xac]
        mov dword ptr [ecx + 0x2f8], edx
        fmul dword ptr [DAT_004e13e4]
        fstp dword ptr [ecx + 0xac]
        fld dword ptr [ecx + 0xb0]
        fmul dword ptr [DAT_004e13e4]
        fstp dword ptr [ecx + 0xb0]
        fld dword ptr [ecx + 0xb4]
        fmul dword ptr [DAT_004e13e4]
        fstp dword ptr [ecx + 0xb4]
        ret 0xc
    }
}

extern float DAT_004e1f38;
extern float DAT_004e1348;
extern float DAT_004e1f60;
extern float DAT_004e1ebc;
extern "C" double fun_004c1404();

// original: FUN_0046e2d0
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x46e2d0
void CarRain::Update(float)
{
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0xfffffff8
        sub esp, 0x14
        fld dword ptr [ebp + 8]
        push ebx
        fdiv dword ptr [DAT_004e1f38]
        push esi
        push edi
        mov edi, ecx
        mov eax, dword ptr [edi + 0x12c]
        test eax, eax
        fstp dword ptr [esp + 0x1c]
        jle car_rain_update_done
        mov ebx, dword ptr [edi + 0x130]
        mov dword ptr [esp + 0x14], ebx
        lea esi, [ebx + 0x10]
        mov dword ptr [esp + 0x18], eax
    car_rain_update_loop:
        fld dword ptr [esp + 0x1c]
        fadd dword ptr [ebx]
        fst dword ptr [esp + 0x10]
        fstp dword ptr [ebx]
        fld dword ptr [esp + 0x10]
        fcomp dword ptr [DAT_004e1348]
        fnstsw ax
        test ah, 1
        jne car_rain_update_next
        fld dword ptr [esp + 0x10]
        sub esp, 8
        fstp qword ptr [esp]
        call fun_004c1404
        fsubr dword ptr [esp + 0x18]
        add esp, 8
        fstp dword ptr [ebx]
        call rand
        and eax, 0x3ff
        mov dword ptr [esp + 0x10], eax
        fild dword ptr [esp + 0x10]
        fmul dword ptr [DAT_004e1f60]
        fadd dword ptr [DAT_004e1ebc]
        fstp dword ptr [esi - 0xc]
        call rand
        cdq
        idiv dword ptr [edi + 0x14c]
        mov eax, dword ptr [edi + 0x140]
        cmp edx, eax
        jge car_rain_update_high
        mov ecx, dword ptr [edi + 0x134]
        lea eax, [edx + edx*2]
        mov edx, dword ptr [ecx + eax*4]
        lea eax, [ecx + eax*4]
        mov ecx, dword ptr [eax + 4]
        mov dword ptr [esi - 8], edx
        mov edx, dword ptr [eax + 8]
        lea eax, [edi + 0x150]
        mov dword ptr [esi - 4], ecx
        mov dword ptr [esi], edx
        mov dword ptr [esi + 4], eax
        jmp car_rain_update_next
    car_rain_update_high:
        mov ecx, dword ptr [edi + 0x144]
        lea ebx, [ecx + eax]
        cmp edx, ebx
        jge car_rain_update_highest
        sub edx, eax
        lea ecx, [edx + edx*2]
        mov edx, dword ptr [edi + 0x138]
        lea eax, [edx + ecx*4]
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [esi - 8], ecx
        mov dword ptr [esi - 4], edx
        mov dword ptr [esi], eax
        lea ecx, [edi + 0x190]
        jmp car_rain_update_set_mesh
    car_rain_update_highest:
        sub edx, ecx
        sub edx, eax
        mov eax, dword ptr [edi + 0x13c]
        lea edx, [edx + edx*2]
        mov ecx, dword ptr [eax + edx*4]
        lea eax, [eax + edx*4]
        mov edx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [esi - 8], ecx
        mov dword ptr [esi - 4], edx
        mov dword ptr [esi], eax
        lea ecx, [edi + 0x1d0]
    car_rain_update_set_mesh:
        mov ebx, dword ptr [esp + 0x14]
        mov dword ptr [esi + 4], ecx
    car_rain_update_next:
        mov eax, dword ptr [esp + 0x18]
        add ebx, 0x18
        add esi, 0x18
        dec eax
        mov dword ptr [esp + 0x14], ebx
        mov dword ptr [esp + 0x18], eax
        jne car_rain_update_loop
    car_rain_update_done:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret 4
    }
}

// Transforms *point by *matrix (row vectors, translation in row 3) into *result: the original's
// hand-written x87 block (also used by the sound module); pointer locals are reloaded into registers.
#define CAR_TRANSFORM_POINT() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
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

// NONMATCHING: RALLYTROPHY 0x46e430
// 997 bytes both, 16 differing. Reading hoodObject through a volatile lvalue after the liveBody matrix copy
// matches the target's load schedule. The remaining differences are register choices for the liveBody destination
// and Multiply arguments; the x87 transform, vertex fill, EH states, and original hood/trunk copy-paste behavior match.
void CarRain::Render(RenderDevice* device, Camera* camera)
{
    if (texture == 0) {
        return;
    }
    CarMatrix* body = &liveBody;
    memcpy(body, &bodyObject->world, sizeof(liveBody));
    CarBodyObject* hood = *(CarBodyObject* volatile*)&hoodObject;
    if (hood != 0) {
        memcpy(&liveHood, &hood->world, sizeof(liveHood));
    } else {
        memcpy(&liveHood, &savedHood, sizeof(liveHood));
        CopyBits((Vec3*)&liveHood.m[12], hoodPos);
        liveHood.Multiply(body);
    }
    if (trunkObject != 0) {
        memcpy(&liveTrunk, &trunkObject->world, sizeof(liveTrunk));
    } else {
        // The original builds the trunk matrix in the hood's matrix here (copy and paste slip).
        memcpy(&liveHood, &savedTrunk, sizeof(liveHood));
        CopyBits((Vec3*)&liveHood.m[12], trunkPos);
        liveHood.Multiply(&liveBody);
    }
    mesh->material = texture;
    Vec3 right = camera->right;
    Vec3 up = camera->up;
    EffectVertex* v = mesh->vertices;
    Color color;
    color.value = 0xffffffff;
    int i;
    for (i = 0; i < dropCount; i++) {
        CarDrop* d = &drops[i];
        float s = (d->phase * 0.8f + 0.2f) * d->size;
        float h = s * 0.5f;
        color.a = (BYTE)FloatToInt((1.0f - d->phase) * 255.0f);
        Vec3 pos;
        const CarMatrix* matrix = d->matrix;
        const Vec3* point = &d->pos;
        Vec3* result = &pos;
        CAR_TRANSFORM_POINT();
        CopyBits(&v->position, pos);
        AddRev(&v->position, up * s);
        v->position -= right * h;
        v->color = color.value;
        v++;
        CopyBits(&v->position, pos);
        AddRev(&v->position, up * s);
        AddRev(&v->position, right * h);
        v->color = color.value;
        v++;
        CopyBits(&v->position, pos);
        AddRev(&v->position, right * h);
        v->color = color.value;
        v++;
        CopyBits(&v->position, pos);
        v->position -= right * h;
        v->color = color.value;
        v++;
    }
    device->vf44(1);
    device->DrawMesh(mesh, 1);
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x46e820
// SYMBOL: _$E28
typedef void Generated_46e820();

// FUNCTION: RALLYTROPHY 0x46e840
// SYMBOL: _$E27
typedef void Generated_46e840();
