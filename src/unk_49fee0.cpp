// AI driver, part 2 (0x49fee0-0x4a14af): opponent scan and obstacle geometry.
// Obstacles are the other cars in front of this driver; see analysis/range-49b630.md section 4.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include <math.h>

extern int DAT_0050132c;               // difficulty class
extern float DAT_00500e98;             // base avoidance distance
extern float DAT_004e134c;
extern "C" void* dat_004de1d8;
extern "C" void fun_0049ffa0();
extern "C" void fun_004a0420();
void __stdcall FUN_004a1390(Obstacle* ob, AiBody* src);

// A rotation about an axis (0x4b5760); applied to a Vec3 by Vec3::operator* (0x4b5850).
struct RotMatrix {
    RotMatrix(const Vec3* axis, float angle); // 0x4b5760
    ~RotMatrix() {}
    float m[16];
};


__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49fee0
void AIDriver::FUN_0049fee0()
{
    __asm {
        push -1
        push offset dat_004de1d8
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 0xc
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 0x110]
        mov ecx, dword ptr [esi + 0x14]
        mov dword ptr [esi + 0xb0], eax
        mov eax, dword ptr [ecx + 0x28]
        mov eax, dword ptr [eax + 0x24]
        mov edx, dword ptr [eax + 0x1cc]
        fld dword ptr [eax + 0x1d4]
        fst dword ptr [esp + 0xc]
        mov dword ptr [esi + 0x110], 0
        mov dword ptr [esp + 4], edx
        fld st(0)
        fmulp st(1), st
        mov dword ptr [esp + 0x18], 0
        fld dword ptr [esp + 4]
        mov dword ptr [esp + 8], 0
        fmul dword ptr [esp + 4]
        faddp st(1), st
        fsqrt
        fcomp dword ptr [DAT_004e134c]
        fnstsw ax
        test ah, 0x41
        jne ai_driver_scan_done
        mov ecx, esi
        call fun_0049ffa0
        mov eax, dword ptr [esi + 0x554]
        test eax, eax
        je ai_driver_no_obstacles
        mov ecx, esi
        call fun_004a0420
        pop esi
        mov ecx, dword ptr [esp + 0xc]
        mov dword ptr fs:[0], ecx
        add esp, 0x18
        ret
    ai_driver_no_obstacles:
        mov dword ptr [esi + 0x164], 0
    ai_driver_scan_done:
        mov ecx, dword ptr [esp + 0x10]
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x18
        ret
    }
}

// A copy of a vector with y cleared (an inline helper in the original).
static inline Vec3 Flat(const Vec3& v)
{
    return Vec3(v.x, 0, v.z);
}

// NONMATCHING: RALLYTROPHY 0x49ffa0
// Logic complete (1110 vs 1149 bytes). The original has four Vec3 objects (EH state 4) alive before the loop and
// reads the results of the out-of-line Vec3 +/- (0x419f00/0x414330) straight from the returned pointers; the
// build's temporaries, EH state numbers and register allocation (original: this in edi) differ.
void AIDriver::FUN_0049ffa0()
{
    Car* ownCar = player->car;
    AiBody* own = (AiBody*)ownCar->body;
    Vec3 ownVel(*(Vec3*)&own->vx);
    ownVel.y = 0;
    Vec3 ownFwd(own->Forward());
    unk554 = 0;
    unkbc = 0;
    Obstacle* ob = obstacles;
    for (unsigned i = 0; i < listeners.size(); i++) {
        AiBody* ownBody = (AiBody*)ownCar->body;
        AiBody* other = (AiBody*)listeners[i]->car->body;
        Vec3 dir(forward.x, forward.y, forward.z);
        float inv = 1.0f / dir.Length();
        dir.x *= inv;
        dir.y *= inv;
        dir.z *= inv;
        Vec3 rel = Flat(other->position - ownBody->position);
        Vec3 diff = other->position - ownBody->position;
        diff.y = 0;
        Vec3 r3 = Flat((other->position + *(Vec3*)&other->vx) - (ownBody->position + *(Vec3*)&ownBody->vx));
        Vec3 v4 = Flat((other->position + *(Vec3*)&other->vx) - ownBody->position);
        float along = diff.z * forward.z + diff.y * forward.y + diff.x * forward.x;
        Vec3 otherFwd(other->Forward());
        float ownSpeed = 0;
        if (ownBody->vx * ownBody->vx + ownBody->vy * ownBody->vy + ownBody->vz * ownBody->vz > 0.0f) {
            ownSpeed = ownFwd.y * ownBody->vy + ownFwd.z * ownBody->vz + ownFwd.x * ownBody->vx;
        }
        unk568 = ownSpeed;
        float otherSpeed = 0;
        if (other->vx * other->vx + other->vy * other->vy + other->vz * other->vz > 0.0f) {
            otherSpeed = otherFwd.y * other->vy + otherFwd.z * other->vz + otherFwd.x * other->vx;
        }
        if (along > 0.0f) {
            float distance = (float)sqrt(rel.z * rel.z + rel.x * rel.x);
            if (distance < 30.0f || (distance < 40.0f && unk164 == 1)) {
                if (ownSpeed > otherSpeed || unk164 == 1) {
                    unkbc = 1;
                    ob->valid = 1;
                    ob->unk0.x = other->position.x;
                    ob->unk0.y = other->position.y;
                    ob->unk0.z = other->position.z;
                    ob->unk0.y = 0;
                    ob->leftDone = ob->rightDone = ob->side = 0;
                    ::FUN_004a1390(ob, other);
                    ob->unk4c.x = other->Forward().x;
                    ob->unk4c.z = other->Forward().z;
                    ob->unk4c.y = other->Forward().y;
                    ob->unk40.x = other->vx;
                    ob->unk40.y = other->vy;
                    ob->unk40.z = other->vz;
                    unk554++;
                } else {
                    ob->valid = 0;
                }
            } else {
                ob->valid = 0;
            }
        } else {
            ob->valid = 0;
        }
        ob++;
    }
}

// NONMATCHING: RALLYTROPHY 0x4a0420
// 2526 vs 2615 bytes; logic complete (every call, constant and branch of the original). The prologue differs: the
// original keeps the velocity/axis unit vectors in place (Vec3Slot copies with the x store elided, k in a stack
// slot) and gives loop 1 the obstacle base in edi; the frame is 0xc bytes smaller and the register use differs.
void AIDriver::FUN_004a0420()
{
    float best = 0.0f;
    float leftSum = 0.0f;
    float rightSum = 0.0f;
    unsigned int i = 0;
    Vec3Slot dir, up;
    int found = 0;
    float f;
    float dotV1, dotA1, dotV2, dotA2;
    int again;
    Car* car = player->car;

    // Unit vectors of the velocity (vel) and of the lateral axis (axis).
    Vec3Slot vel = *(Vec3Slot*)car->body->velocity;
    vel.y = 0.0f;
    float len = (float)sqrt(vel.z * vel.z + vel.x * vel.x);
    float k = 1.0f / len;
    vel.x = vel.x * k;
    vel.z = vel.z * k;
    Vec3Slot axis = unkd4;
    len = (float)sqrt(axis.x * axis.x + axis.z * axis.z + axis.y * axis.y);
    k = 1.0f / len;
    axis.x = axis.x * k;
    axis.y = axis.y * k;
    axis.z = axis.z * k;
    float velX = vel.x, velY = vel.y, velZ = vel.z;
    float axisX = axis.x, axisY = axis.y, axisZ = axis.z;

    for (i = 0; i < listeners.size(); i++) {
        Obstacle* o = &obstacles[i];
        if (o->valid) {
            Vec3Slot t = unkd4;
            t.y = 0.0f;
            float r = FUN_004a0e60(o, (Vec3*)&t);
            if (r != -1.0f) {
                if (best < r) {
                    unk104 = i;
                    best = r;
                }
                found = 1;
                o->side = 1;
            }
        }
    }
    if (!found) {
        unk164 = 0;
        return;
    }

    for (i = 0; i < listeners.size(); i++) {
        Obstacle* o = &obstacles[i];
        if (o->valid && o->side != 1) {
            Vec3 rel = *(Vec3*)&o->unk0 - ((CarBodyV*)car->body)->position;
            Vec3Slot p;
            p.x = unkd4.z;
            p.y = 0.0f;
            p.z = -unkd4.x;
            rel.y = 0.0f;
            if (rel.z * p.z + rel.x * p.x < 0.0f) {
                o->side = 2;
            } else {
                o->side = 3;
            }
        }
    }

    dir = unkd4;
    dir.y = 0.0f;
    do {
        again = 0;
        leftSum += FUN_004a1050(&obstacles[unk104], (Vec3*)&dir, 1);
        obstacles[unk104].leftDone = 1;
        if (leftSum == 0.0f) {
            dir.x = 0.0f;
            dir.y = 1.0f;
            dir.z = 0.0f;
        } else {
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            RotMatrix rot((Vec3*)&up, -leftSum);
            *(Vec3*)&dir = ((Vec3*)&unkd4)->operator*(rot);
            dir.y = 0.0f;
        }
        float best2 = 1000.0f;
        int calls = 0;
        for (i = 0; i < listeners.size(); i++) {
            Obstacle* o = &obstacles[i];
            f = -1.0f;
            if (o->valid && !o->leftDone && (o->side == 2 || o->side == 1)) {
                f = FUN_004a0e60(o, (Vec3*)&dir);
                calls++;
                if (f >= 0.0f) {
                    again = 1;
                    goto select1;
                }
            }
            {
                float inv = 1.0f / (float)sqrt(dir.x * dir.x + dir.z * dir.z + dir.y * dir.y);
                float nx = inv * dir.x, ny = inv * dir.y, nz = inv * dir.z;
                dotV1 = nx * velX + ny * velY + nz * velZ;
                dotA1 = nx * axisX + nz * axisZ + ny * axisY;
            }
        select1:
            if (best2 <= f && !listeners[unk104]->local) {
                continue;
            }
            if (f == -1.0f) {
                continue;
            }
            if (!listeners[unk104]->local) {
                unk104 = i;
                best2 = f;
            } else {
                best2 = 0.0f;
                unk104 = i;
            }
        }
        int more = calls ? again : 0;
        if (f > best) {
            best = f;
        }
        again = more;
    } while (again);

    dir = unkd4;
    dir.y = 0.0f;
    do {
        again = 0;
        rightSum += FUN_004a1050(&obstacles[unk104], (Vec3*)&dir, 0);
        obstacles[unk104].rightDone = 1;
        if (rightSum == 0.0f) {
            dir.x = 0.0f;
            dir.y = 1.0f;
            dir.z = 0.0f;
        } else {
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            RotMatrix rot((Vec3*)&up, rightSum);
            *(Vec3*)&dir = ((Vec3*)&unkd4)->operator*(rot);
            dir.y = 0.0f;
        }
        float best2 = 1000.0f;
        int calls = 0;
        for (i = 0; i < listeners.size(); i++) {
            Obstacle* o = &obstacles[i];
            f = -1.0f;
            if (o->valid && !o->rightDone && (o->side == 3 || o->side == 1)) {
                f = FUN_004a0e60(o, (Vec3*)&dir);
                calls++;
                if (f >= 0.0f) {
                    again = 1;
                    goto select2;
                }
            }
            {
                float inv = 1.0f / (float)sqrt(dir.x * dir.x + dir.z * dir.z + dir.y * dir.y);
                float nx = inv * dir.x, ny = inv * dir.y, nz = inv * dir.z;
                dotV2 = nx * velX + ny * velY + nz * velZ;
                dotA2 = nx * axisX + nz * axisZ + ny * axisY;
            }
        select2:
            if (best2 <= f && !listeners[unk104]->local) {
                continue;
            }
            if (f == -1.0f) {
                continue;
            }
            if (!listeners[unk104]->local) {
                unk104 = i;
                best2 = f;
            } else {
                best2 = 0.0f;
                unk104 = i;
            }
        }
        int more = calls ? again : 0;
        if (f > best) {
            best = f;
        }
        again = more;
    } while (again);

    unk564 = dotA2;
    unk56c = dotA1;
    if (dotV2 + dotA2 < dotA1 + dotV1) {
        unk110 = dotA2;
        unk558 = 2;
        if (dotA2 < 0.0f && (!listeners[unk104]->local || ((Vec3*)car->body->velocity)->Length() < 5.0f)) {
            unk110 = 0.0f;
        }
    } else {
        unk110 = -dotA1;
        unk558 = 1;
        if (unk110 > 0.0f && (!listeners[unk104]->local || ((Vec3*)car->body->velocity)->Length() < 5.0f)) {
            unk110 = 0.0f;
        }
    }
    float limit = DAT_00500e98;
    if (DAT_0050132c == 0) {
        limit = DAT_00500e98 + 0.15f;
    } else if (DAT_0050132c == 1) {
        limit = DAT_00500e98 + 0.03f;
    }
    if (limit < unk110) {
        if (!FUN_0049f1f0(listeners[unk104]) || !listeners[unk104]->local) {
            unk110 = 0.1f;
        }
    } else if (unk110 < -limit) {
        if (!FUN_0049f1f0(listeners[unk104]) || !listeners[unk104]->local) {
            unk110 = -0.1f;
        }
    }
    if (((Vec3*)car->body->velocity)->Length() < 1.0f) {
        unk110 = 0.0f;
    }
}


// Scales the x/z components of a vector to unit length (inline in the original, 0x4a0e60).
static inline void NormalizeXZ(Vec3* v)
{
    float inv = 1.0f / (float)sqrt(v->z * v->z + v->x * v->x);
    v->x = inv * v->x;
    v->z = inv * v->z;
}

// NONMATCHING: RALLYTROPHY 0x4a0e60
// Logic complete (472 vs 494 bytes). The original keeps the lateral axis in memory, so the y term of each corner
// dot product survives and the 1/length divide is `fdiv` on a kept length; VC6 folds lat.y == 0 here. The second
// parameter is unused (a Vec3* the caller passes).
float AIDriver::FUN_004a0e60(Obstacle* ob, Vec3* unused)
{
    Vec3Slot box[4];
    float hi = -100.0f;
    float lo = 100.0f;
    AiBody* own = (AiBody*)player->car->body;
    Vec3 lat;
    lat.y = 0;
    lat.z = -own->vx;
    lat.x = own->vz;
    NormalizeXZ(&lat);
    Vec3* pl = &lat;
    for (int i = 0; i < 8; i++) {
        Vec3 t = *(Vec3*)&ob->corners[i] - own->position;
        float d = t.y * pl->y + t.z * pl->z + t.x * pl->x;
        if (d > hi) {
            hi = d;
        }
        if (d < lo) {
            lo = d;
        }
    }
    unk90 = 1.0f;
    if ((hi > 1.2f && lo > 1.2f) || (hi < -1.2f && lo < -1.2f)) {
        return -1.0f;
    }
    Vec3 d = *(Vec3*)&ob->unk0 - own->position;
    d.y = 0;
    return (float)sqrt(d.z * d.z + d.x * d.x);
}

// Transforms *point in place by the 4x4 matrix (row vectors, translation in row 3); the same
// hand-written x87 block as the sound module's TRANSFORM_POINT, storing back into the point.
#define TRANSFORM_IN_PLACE() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
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
    __asm fstp dword ptr [ecx] \
    __asm fstp dword ptr [ecx+8] \
    __asm fstp dword ptr [ecx+4]


// NONMATCHING: RALLYTROPHY 0x4a1050
// Logic complete (766 vs 819 bytes). Frame layout of c/perp and the two asm pointer slots (-0x10/-0x14, swapped
// between the branches) match; remaining differences: the original keeps the Car* in edi and does not hoist
// `side` into ebx, and does not merge the two identical y/z position-add tails.
float AIDriver::FUN_004a1050(Obstacle* ob, Vec3* dir, int side)
{
    Vec3Slot box[4];
    Car* car = player->car;
    AiBody* own = (AiBody*)car->body;
    Vec3 c;
    float best;
    if (side) {
        c.x = own->boxMax[0];
        c.z = own->boxMax[2];
        c.y = own->boxMin[1];
        const float* matrix = ((AiBody*)player->car->body)->matrix;
        Vec3* point = &c;
        TRANSFORM_IN_PLACE();
        Vec3* pos = &((AiBody*)player->car->body)->position;
        c.x = c.x + pos->x;
        best = 1000.0f;
        c.y = c.y + pos->y;
        c.z = c.z + pos->z;
    } else {
        c.x = own->boxMin[0];
        c.z = own->boxMax[2];
        c.y = own->boxMin[1];
        Vec3* point;
        const float* matrix = ((AiBody*)player->car->body)->matrix;
        point = &c;
        TRANSFORM_IN_PLACE();
        Vec3* pos = &((AiBody*)player->car->body)->position;
        c.x = c.x + pos->x;
        best = -1000.0f;
        c.y = c.y + pos->y;
        c.z = c.z + pos->z;
    }
    Vec3 perp;
    perp.x = forward.z;
    perp.y = 0;
    perp.z = -forward.x;
    int idx = side;
    for (int i = 0; i < 8; i++) {
        Vec3 t = *(Vec3*)&ob->corners[i] - ((AiBody*)car->body)->position;
        float d = t.z * perp.z + t.x * perp.x;
        if (side) {
            if (d < best) {
                best = d;
                idx = i;
            }
        } else {
            if (d > best) {
                best = d;
                idx = i;
            }
        }
    }
    Vec3 e = *(Vec3*)&ob->corners[idx] - c;
    float inv = 1.0f / (float)sqrt(e.y * e.y + e.z * e.z + e.x * e.x);
    e.x *= inv;
    e.y *= inv;
    e.z *= inv;
    Vec3 f(*dir);
    float inv2 = 1.0f / (float)sqrt(f.z * f.z + f.y * f.y + f.x * f.x);
    return 1.0f - (e.z * (f.z * inv2) + e.y * (f.y * inv2) + e.x * (f.x * inv2));
}

// original: FUN_004a1390
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1390
void __stdcall FUN_004a1390(Obstacle*, AiBody*)
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push ebx
        push esi
        mov esi, dword ptr [ebp + 0xc]
        xor edx, edx
        push edi
        mov dword ptr [ebp - 4], edx
    obstacle_box_corner_layer:
        xor edi, edi
    obstacle_box_corner_row:
        mov ebx, dword ptr [ebp + 8]
        lea eax, [edx + edx*2 + 0x18]
        xor ecx, ecx
        lea eax, [ebx + eax*4]
    obstacle_box_corner_column:
        mov ebx, dword ptr [ebp - 4]
        test ebx, ebx
        jne obstacle_box_max_x
        mov ebx, dword ptr [esi + 0xa0]
        jmp obstacle_box_store_x
    obstacle_box_max_x:
        mov ebx, dword ptr [esi + 0xac]
    obstacle_box_store_x:
        test edi, edi
        mov dword ptr [eax - 4], ebx
        jne obstacle_box_max_y
        mov ebx, dword ptr [esi + 0xa4]
        jmp obstacle_box_store_y
    obstacle_box_max_y:
        mov ebx, dword ptr [esi + 0xb0]
    obstacle_box_store_y:
        test ecx, ecx
        mov dword ptr [eax], ebx
        jne obstacle_box_max_z
        mov ebx, dword ptr [esi + 0xa8]
        jmp obstacle_box_store_z
    obstacle_box_max_z:
        mov ebx, dword ptr [esi + 0xb4]
    obstacle_box_store_z:
        mov dword ptr [eax + 4], ebx
        inc edx
        add eax, 0xc
        inc ecx
        cmp ecx, 2
        jl obstacle_box_corner_column
        inc edi
        cmp edi, 2
        jl obstacle_box_corner_row
        mov eax, dword ptr [ebp - 4]
        inc eax
        cmp eax, 2
        mov dword ptr [ebp - 4], eax
        jl obstacle_box_corner_layer
        mov edx, dword ptr [ebp + 8]
        lea ecx, [esi + 0x20]
        mov dword ptr [ebp + 0xc], ecx
        lea eax, [edx + 0x64]
        mov ebx, 8
    obstacle_box_transform_loop:
        lea edi, [eax - 8]
        mov dword ptr [ebp + 8], edi
        mov ecx, dword ptr [ebp + 8]
        mov edx, dword ptr [ebp + 0xc]
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 4]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 8]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x10]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x14]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x18]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x20]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x24]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x28]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fadd dword ptr [edx + 0x30]
        fxch st(1)
        fadd dword ptr [edx + 0x34]
        fxch st(2)
        fadd dword ptr [edx + 0x38]
        fxch st(1)
        fstp dword ptr [ecx]
        fstp dword ptr [ecx + 8]
        fstp dword ptr [ecx + 4]
        fld dword ptr [edi]
        fadd dword ptr [esi + 0x14]
        add eax, 0xc
        dec ebx
        fstp dword ptr [edi]
        fld dword ptr [eax - 0x10]
        fadd dword ptr [esi + 0x18]
        fstp dword ptr [eax - 0x10]
        fld dword ptr [esi + 0x1c]
        fadd dword ptr [eax - 0xc]
        fstp dword ptr [eax - 0xc]
        jne obstacle_box_transform_loop
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret 8
    }
}

// FUNCTION: RALLYTROPHY 0x4a14b0
// SYMBOL: _$E23
typedef void Generated_4a14b0();

// FUNCTION: RALLYTROPHY 0x4a14d0
// SYMBOL: _$E22
typedef void Generated_4a14d0();
