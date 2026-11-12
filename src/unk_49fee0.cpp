// AI driver, part 2 (0x49fee0-0x4a14af): opponent scan and obstacle geometry.
// Obstacles are the other cars in front of this driver; see analysis/range-49b630.md section 4.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include <math.h>

extern int DAT_0050132c;               // difficulty class
extern float DAT_00500e98;             // base avoidance distance
extern float DAT_004e134c;
extern float DAT_004e1348;
extern "C" void* dat_004de1d8;
extern "C" void fun_0049ffa0();
extern "C" void fun_004a0420();
void __stdcall FUN_004a1390(Obstacle* ob, AiBody* src);

// A rotation about an axis (0x4b5760); applied to a Vec3 by Vec3::operator* (0x4b5850).
struct RotMatrix {
    RotMatrix() {}
    void Set(const Vec3* axis, float angle); // 0x4b5760
    ~RotMatrix() {}
    float m[16];
};
// Result of the out-of-line 0x4b5850 Vec3 * RotMatrix (plain struct: no unwind state) and its thiscall view.
struct MulPod { float x, y, z; };
struct MulThis { MulPod* Mul(MulPod* out, const RotMatrix* m) const; };


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

// Plain result of the out-of-line 0x414330 subtraction (no destructor, so no unwind state) and a thiscall view of it.
struct SubPod { float x, y, z; };
struct SubThis { SubPod* Sub(SubPod* out, const SubThis* o) const; }; // 0x414330, returns out
static inline SubPod* SubP(const Vec3& a, const Vec3& b, SubPod* out)
{
    return ((const SubThis*)&a)->Sub(out, (const SubThis*)&b);
}

// NONMATCHING: RALLYTROPHY 0x49ffa0
// 2026-11-04 (ws-I): EH map now exact (states 4,5,6,7,8,6,9,10 as the original). Cause: the out-of-line Sub (0x414330) returns a plain struct (SubPod, no unwind state) read through the returned pointer; rel/r3/v4 are three function-scope Vec3 declared before ownVel (states 0-2), dir/diff in-loop, `other` is re-derived from the Car pointer (oc->body) at every use. Left: this in esi vs edi, frame 0xc4 vs 0xc0.
// Tried 2026-10-30 night: ~25 variants; 215->170 insns by declaring rel/diff Vec3 outside the loop (assigned per pass) and copying forward into dir field by field; original still has frame 0xc0 (ours 0xd8), EH state 4 before the loop and this in edi/i in ecx with the dir inverse sharing i's slot.
// Tried 2026-11-01: ~18 more variants (dir/r3/v4/otherFwd hoisted out of the loop, extra dummy Vec3, int i, double length, result=0); only hoisting otherFwd up to after 'other' helped, 170->168; still frame 0xd8 vs 0xc0 and EH state 3 vs 4 at loop entry.
// Logic complete (1110 vs 1149 bytes). The original has four Vec3 objects (EH state 4) alive before the loop and
// reads the results of the out-of-line Vec3 +/- (0x419f00/0x414330) straight from the returned pointers; the
// build's temporaries, EH state numbers and register allocation (original: this in edi) differ.
void AIDriver::FUN_0049ffa0()
{
    Car* ownCar = player->car;
    AiBody* own = (AiBody*)ownCar->body;
    Vec3 rel;
    Vec3 r3;
    Vec3 v4;
    Vec3 ownVel(*(Vec3*)&own->vx);
    ownVel.y = 0;
    Vec3 ownFwd(own->Forward());
    unk554 = 0;
    unkbc = 0;
    Obstacle* ob = obstacles;
    for (unsigned i = 0; i < listeners.size(); i++) {
        Car* oc = listeners[i]->car;
        AiBody* ownBody = (AiBody*)ownCar->body;
        Vec3 dir; dir.x = forward.x; dir.y = forward.y; dir.z = forward.z;
        float inv = 1.0f / dir.Length();
        dir.x *= inv;
        dir.y *= inv;
        dir.z *= inv;
        SubPod t1, t2, t3;
        SubPod* q = SubP(((AiBody*)oc->body)->position, ownBody->position, &t1);
        rel.x = q->x; rel.y = 0; rel.z = q->z;
        Vec3 diff = ((AiBody*)oc->body)->position - ownBody->position;
        diff.y = 0.0f;
        q = SubP(((AiBody*)oc->body)->position + *(Vec3*)&((AiBody*)oc->body)->vx, ownBody->position + *(Vec3*)&ownBody->vx, &t2);
        r3.x = q->x; r3.y = 0; r3.z = q->z;
        q = SubP(((AiBody*)oc->body)->position + *(Vec3*)&((AiBody*)oc->body)->vx, ownBody->position, &t3);
        v4.x = q->x; v4.y = 0; v4.z = q->z;
        Vec3 otherFwd(((AiBody*)oc->body)->Forward());
        float along = diff.z * forward.z;
        along = *(volatile float*)&diff.y * forward.y + along;
        along = diff.x * forward.x + along;
        float ownSpeed = 0;
        if (ownBody->vx * ownBody->vx + ownBody->vy * ownBody->vy + ownBody->vz * ownBody->vz > 0.0f) {
            ownSpeed = ownFwd.y * ownBody->vy;
            ownSpeed = ownFwd.z * ownBody->vz + ownSpeed;
            ownSpeed = ownFwd.x * ownBody->vx + ownSpeed;
        }
        unk568 = ownSpeed;
        float otherSpeed = 0;
        if (((AiBody*)oc->body)->vx * ((AiBody*)oc->body)->vx + ((AiBody*)oc->body)->vy * ((AiBody*)oc->body)->vy + ((AiBody*)oc->body)->vz * ((AiBody*)oc->body)->vz > 0.0f) {
            otherSpeed = otherFwd.y * ((AiBody*)oc->body)->vy;
            otherSpeed = otherFwd.z * ((AiBody*)oc->body)->vz + otherSpeed;
            otherSpeed = otherFwd.x * ((AiBody*)oc->body)->vx + otherSpeed;
        }
        if (along > 0.0f) {
            float distance = (float)sqrt(rel.z * rel.z + rel.x * rel.x);
            if (distance < 30.0f || (distance < 40.0f && unk164 == 1)) {
                if (ownSpeed > otherSpeed || unk164 == 1) {
                    unkbc = 1;
                    ob->valid = 1;
                    ob->unk0.x = ((AiBody*)oc->body)->position.x;
                    ob->unk0.y = ((AiBody*)oc->body)->position.y;
                    ob->unk0.z = ((AiBody*)oc->body)->position.z;
                    ob->unk0.y = 0;
                    ob->leftDone = ob->rightDone = ob->side = 0;
                    ::FUN_004a1390(ob, (AiBody*)oc->body);
                    ob->unk4c.x = ((AiBody*)oc->body)->Forward().x;
                    ob->unk4c.z = ((AiBody*)oc->body)->Forward().z;
                    ob->unk4c.y = ((AiBody*)oc->body)->Forward().y;
                    ob->unk40.x = ((AiBody*)oc->body)->vx;
                    ob->unk40.y = ((AiBody*)oc->body)->vy;
                    ob->unk40.z = ((AiBody*)oc->body)->vz;
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
// 2026-11-04 (ws-I): EH states and frame (0xb0) now exact. The missing 4th persistent object is a function-scope RotMatrix (empty dtor, state 0) re-initialised by the out-of-line 0x4b5760 (rot.Set(&up,..)); `up` is a block-local Vec3 destroyed before the multiply; Vec3*Rot (0x4b5850) and Sub return plain structs read through eax; p is a Vec3 declared before the Sub call. Left: scheduling/register differences.
// Current: 2546/2615 bytes, 2189 differing bytes, instruction-multiset delta 54. The target frame is 0xb0 vs 0xa4.
// Its unwind map arms separate cleanups for the argument/result Vec3 temporaries and the two branch-local rotation
// axes. Branch-local Vec3 `up` gives candidate states 4/5/6/7; the exact frame/register schedule still differs.
// Reading normalized vector fields directly reduced instruction-multiset delta 69->65 before the lifetime probes.
// Tried 2026-10-31: ~30 variants; 288->250 insns differ by building vel field-wise as a Vec3 (not Vec3Slot copy); p-before-rel, Vec3Slot/ctor forms of dir/vel/axis, sqrt term order did not help; original has baseline EH state 3 (ours 2) and frame 0xb0 (ours 0xa4), i.e. one more function-scope Vec3 object we cannot identify.
// Tried 2026-11-01 re-pass: 14 more variants (unused/dead-store/volatile/copy-init extra function-scope Vec3 at 6 positions, branchy `more` forms); best 250->243 insns (extra Vec3 pre with dead stores) but bytes grow and frame/EH states still differ; unreferenced Vec3 gets elided, and the 12-byte slot is still unidentified.
void AIDriver::FUN_004a0420()
{
    RotMatrix rot;
    SubPod rs;
    MulPod tmp;
    float best = 0.0f;
    float leftSum = 0.0f;
    float rightSum = 0.0f;
    unsigned int i = 0;
    Vec3 dir;
    int found = 0;
    float f;
    float dotV1, dotA1, dotV2, dotA2;
    int again;
    Car* car = player->car;

    // Unit vectors of the velocity (vel) and of the lateral axis (axis).
    Vec3 vel; vel.x=car->body->velocity[0]; vel.y=0.0f; vel.z=car->body->velocity[2];
    float len = (float)sqrt(vel.z * vel.z + vel.x * vel.x);
    float k = 1.0f / len;
    vel.x = vel.x * k;
    vel.z = vel.z * k;
    Vec3 axis(*(Vec3*)&unkd4);
    float q = axis.x * axis.x;
    q = axis.z * axis.z + q;
    q = axis.y * axis.y + q;
    len = (float)sqrt(q);
    k = 1.0f / len;
    axis.x = axis.x * k;
    axis.y = axis.y * k;
    axis.z = axis.z * k;
    Obstacle* o = obstacles;
    for (i = 0; i < listeners.size(); i++, o++) {
        if (o->valid) {
            Vec3Slot t;
            t.x = unkd4.x;
            t.y = 0.0f;
            t.z = unkd4.z;
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
            Vec3 p;
            SubP(*(Vec3*)&o->unk0, ((CarBodyV*)car->body)->position, &rs);
            p.x = unkd4.z;
            p.y = 0.0f;
            p.z = -unkd4.x;
            rs.y = 0.0f;
            if (rs.z * p.z + rs.x * p.x < 0.0f) {
                o->side = 2;
            } else {
                o->side = 3;
            }
        }
    }

    dir.x = unkd4.x;
    dir.y = 0.0f;
    dir.z = unkd4.z;
    do {
        again = 0;
        leftSum += FUN_004a1050(&obstacles[unk104], (Vec3*)&dir, 1);
        obstacles[unk104].leftDone = 1;
        if (leftSum == 0.0f) {
            dir.x = 0.0f;
            dir.y = 1.0f;
            dir.z = 0.0f;
        } else {
            {
                Vec3 up;
                up.x = 0.0f;
                up.y = 1.0f;
                up.z = 0.0f;
                rot.Set(&up, -leftSum);
            }
            MulPod* r = ((const MulThis*)&unkd4)->Mul(&tmp, &rot);
            dir.x = r->x; dir.y = r->y; dir.z = r->z;
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
                dotV1 = nx * vel.x + ny * *(volatile float*)&vel.y + nz * vel.z;
                dotA1 = nx * axis.x + nz * axis.z + ny * axis.y;
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
        int more = again;
        if (!calls) more = 0;
        if (f > best) {
            best = f;
        }
        again = more;
    } while (again);

    dir.x = unkd4.x;
    dir.y = 0.0f;
    dir.z = unkd4.z;
    do {
        again = 0;
        rightSum += FUN_004a1050(&obstacles[unk104], (Vec3*)&dir, 0);
        obstacles[unk104].rightDone = 1;
        if (rightSum == 0.0f) {
            dir.x = 0.0f;
            dir.y = 1.0f;
            dir.z = 0.0f;
        } else {
            {
                Vec3 up;
                up.x = 0.0f;
                up.y = 1.0f;
                up.z = 0.0f;
                rot.Set(&up, rightSum);
            }
            MulPod* r = ((const MulThis*)&unkd4)->Mul(&tmp, &rot);
            dir.x = r->x; dir.y = r->y; dir.z = r->z;
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
                dotV2 = nx * vel.x + ny * *(volatile float*)&vel.y + nz * vel.z;
                dotA2 = nx * axis.x + nz * axis.z + ny * axis.y;
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
        int more = again;
        if (!calls) more = 0;
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

// FUNCTION: RALLYTROPHY 0x4a0e60
// lat.y is read through a volatile cast so the y term of each dot product survives (original keeps it); the second
// parameter is unused (a Vec3* the caller passes).
float AIDriver::FUN_004a0e60(Obstacle* ob, Vec3* unused)
{
    Vec3Slot box[4];
    float hi = -100.0f;
    float lo = 100.0f;
    Car* car = player->car;
    AiBody* own;
    Vec3 lat;
    own = (AiBody*)car->body;
    lat.x = own->vz;
    lat.y = 0;
    lat.z = -own->vx;
    float len = (float)sqrt(lat.z * lat.z + lat.x * lat.x);
    float inv = 1.0f / len;
    lat.x = inv * lat.x;
    lat.z = inv * lat.z;
    Vec3* pl = &lat;
    for (int i = 0; i < 8; i++) {
        Vec3 t = *(Vec3*)&ob->corners[i] - own->position;
        float d = t.y * ((volatile Vec3*)pl)->y;
        d = t.z * pl->z + d;
        d = t.x * pl->x + d;
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
    return (float)sqrt(d.x * d.x + d.z * d.z);
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
// Logic complete (819 bytes, 70 -> 18 insns differ). Tried 2026-10-31 re-pass: f scale via `double ls = sqrt(..); float inv2 = (float)(DAT_004e1348 / ls);` (fld dword K; fdiv st,st(1)), ez/ey temps with e.x read directly, idx read after perp. Fixed: `best` assigned after the z add in the side branch
// (keeps the two y/z add tails from merging), volatile read of `side` for idx (no ebx hoist), e/f sums in original
// order. Tried 2026-10-31: inline-function and macro forms of the transform, operand swaps, volatile on matrix/point,
// many tails for f; Still differs: matrix/position pointer chain regs (ecx vs edx/eax), ebx=side load one slot early, return fld [ebp+0x10] one slot early; older note: the
// perp.z / EH-state store scheduling before the loop, and f's 1/len kept in memory (fdiv st,st(1)).
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
        Vec3* point = &c;
        const float* matrix = ((AiBody*)player->car->body)->matrix;
        TRANSFORM_IN_PLACE();
        Vec3* pos = &((AiBody*)player->car->body)->position;
        c.x = c.x + pos->x;
        c.y = c.y + pos->y;
        c.z = c.z + pos->z;
        best = 1000.0f;
    } else {
        c.x = own->boxMin[0];
        c.z = own->boxMax[2];
        c.y = own->boxMin[1];
        const float* matrix = ((AiBody*)player->car->body)->matrix;
        Vec3* point = &c;
        TRANSFORM_IN_PLACE();
        Vec3* pos = &((AiBody*)player->car->body)->position;
        c.x = c.x + pos->x;
        best = -1000.0f;
        c.y = c.y + pos->y;
        c.z = c.z + pos->z;
    }
    int idx = (*(volatile int*)&side);
    Vec3 perp;
    perp.x = forward.z;
    perp.y = 0;
    perp.z = -forward.x;
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
    float inv = 1.0f / (float)sqrt(e.x * e.x + e.z * e.z + e.y * e.y);
    e.x *= inv;
    e.y *= inv;
    e.z *= inv;
    Vec3 f;
    f.x = dir->x;
    f.y = dir->y;
    f.z = dir->z;
    double ls = sqrt(f.z * f.z + f.y * f.y + f.x * f.x);
    float inv2 = (float)(DAT_004e1348 / ls);
    float x = f.x, y = f.y, z = f.z;
    f.x = inv2 * x;
    f.y = inv2 * y;
    f.z = inv2 * z;
    float ez = e.z, ey = e.y;
    return 1.0f - (ez * f.z + ey * f.y + e.x * f.x);
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
