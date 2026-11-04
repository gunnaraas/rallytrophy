// Prop-vs-cars physics step (0x40a850).
#include "unk_40a850.h"

extern U40A850Query DAT_00508298;
struct USph { float x, y, z, r; USph() {} ~USph() {} };
struct UPod { float x, y, z; };
UPod* __cdecl FUN_0040b0c0(Vec3* a, UPod* out, float s); // original: Vec3::operator*(float) at 0x40b0c0 (cdecl-looking in pseudocode)
struct UV3 { float x, y, z; UPod Mul(float k) const; }; // Vec3::operator*(float) at 0x40b0c0, POD result

// Point times a 3x3 row-vector matrix (rows 0x10 apart) plus translation t, as hand-written x87 (inline assembler in the original).
inline void XformT(float* o, const float* p, const float* m, const float* t)
{
    __asm {
        mov ecx, p
        mov edx, m
        mov eax, o
        mov ebx, t
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
        fadd dword ptr [ebx]
        fxch st(1)
        fadd dword ptr [ebx+4]
        fxch st(2)
        fadd dword ptr [ebx+8]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax+8]
        fstp dword ptr [eax+4]
    }
}

// NONMATCHING: RALLYTROPHY 0x40a850
// reason: Xform+translation x87 block (mn corner) is now inline asm like the original and the ebp frame matches; remaining: original keeps this/list in stack locals (ebp-0x44/-0x54) with different register allocation and FPU scheduling (1970 vs 2098 bytes, 1692 differing).
// Tried 2026-10-31: ~25 variants; in-place Vec3 copy-then-modify (lo/hi/ax/ay/az as the original's int copies), volatile this/list spill slots, l->items[i]->state re-read, car-first decl order took 417->405 insns; still differs: original keeps i/hit in stack slots with this=esi/list=edi, and slot order of lo/hi/ax/ay/az/v0/w0 plus FPU scheduling.
// Tried 2026-11-01 re-pass: fixed four float-sum associations to the original's fadd order (distance x2+z2+y2, sphere radius x2+z2+y2, velocity and angular delta sums z+y+x; products already read the right fields); 12 more variants (volatile this/list, decl orders, per-axis deltas, it->state) best 405->400; still differs: original frame 0xc4 vs ours 0xbc, this/list/i/hit spilled to slots, and FPU scheduling.
// original: FUN_0040a850
void U40A850Prop::Fn(int dt)
{
    U40A850List* l = list;
    time += dt;
    U40A850Prop* self = this;
    U40A850List* volatile lv = l;
    int hit = 0;
    int i = 0;
    if (0 < l->items.Size()) {
        do {
            U40A850Item* it = l->items[i];
            if (l->Accept(it) && l->items[i]->state != 3 && l->items[i]->state != 2) {
                U40A850Car* car = it->car;
                U40A850Body* a = body;
                U40A850Body* b = car->body;
                Vec3 d = *(Vec3*)a->pos; d -= *(Vec3*)b->pos;
                float q = d.x * d.x; q = d.z * d.z + q; q = d.y * d.y + q;
                if (q <= 1000.0f) {
                    if (FUN_004ad1d0((float*)a->box + 1, (float*)b->box + 1, a->mat[0], b->mat[0], a->pos, b->pos)) {
                        car->applied = 1;
                        if (it->flag48 == 0 && a->FUN_0041d7d0(b, 1)) {
                            hit++;
                            car->hits++;
                        }
                    }
                }
            }
            i++;
        } while (i < lv->items.Size());
        if (hit) {
            self->active = 1;
            self->time = 0;
        }
    }
    if (active) {
        U40A850Body* a = body;
        Vec3 v0 = *(Vec3*)a->vel;
        Vec3 w0 = *(Vec3*)a->ang;
        U40A850Box* bx = a->box;
        Vec3 lo = *(Vec3*)bx->lo;
        Vec3 hi = *(Vec3*)bx->hi;
        lo.x -= 0.1f; lo.y -= 0.1f; lo.z -= 0.1f;
        hi.x += 0.1f; hi.y += 0.1f; hi.z += 0.1f;
        float* m = a->mat[0];
        Vec3 ax = *(Vec3*)m;
        Vec3 ay = *(Vec3*)(m + 4);
        Vec3 az = *(Vec3*)(m + 8);
        ax.x = (hi.x - lo.x) * ax.x; ax.y = (hi.x - lo.x) * ax.y; ax.z = (hi.x - lo.x) * ax.z;
        ay.x = (hi.y - lo.y) * ay.x; ay.y = (hi.y - lo.y) * ay.y; ay.z = (hi.y - lo.y) * ay.z;
        az.x = (hi.z - lo.z) * az.x; az.y = (hi.z - lo.z) * az.y; az.z = (hi.z - lo.z) * az.z;
        Vec3 mn;
        XformT(&mn.x, &lo.x, m, a->pos);
        Vec3 mx = mn;
        if (*(unsigned*)&ax.x & 0x80000000) mn.x += ax.x; else mx.x += ax.x;
        if (*(unsigned*)&ax.y & 0x80000000) mn.y += ax.y; else mx.y += ax.y;
        if (*(unsigned*)&ax.z & 0x80000000) mn.z += ax.z; else mx.z += ax.z;
        if (*(unsigned*)&ay.x & 0x80000000) mn.x = ay.x + mn.x; else mx.x = ay.x + mx.x;
        if (*(unsigned*)&ay.y & 0x80000000) mn.y = ay.y + mn.y; else mx.y = ay.y + mx.y;
        if (*(unsigned*)&ay.z & 0x80000000) mn.z = ay.z + mn.z; else mx.z = ay.z + mx.z;
        if (*(unsigned*)&az.x & 0x80000000) mn.x = az.x + mn.x; else mx.x = az.x + mx.x;
        if (*(unsigned*)&az.y & 0x80000000) mn.y = az.y + mn.y; else mx.y = az.y + mx.y;
        if (*(unsigned*)&az.z & 0x80000000) mn.z = az.z + mn.z; else mx.z = az.z + mx.z;
        Vec3 s;
        s.x = mx.x - mn.x;
        s.y = mx.y - mn.y;
        s.z = mx.z - mn.z;
        s.x *= 0.5f;
        s.y *= 0.5f;
        s.z *= 0.5f;
        float rq = s.x * s.x; rq = s.z * s.z + rq; rq = s.y * s.y + rq;
        float rad = (float)sqrt(rq);
        s.x += mn.x;
        s.y += mn.y;
        s.z += mn.z;
        USph sp; sp.x = s.x; sp.y = s.y; sp.z = s.z; sp.r = rad;
        DAT_00508298.v.Erase(DAT_00508298.v.begin(), DAT_00508298.v.end());
        U40A850Stage* st = stage;
        DAT_00508298.minX = mn.x;
        DAT_00508298.sphere = &sp;
        DAT_00508298.maxX = mx.x;
        DAT_00508298.minY = mn.y;
        DAT_00508298.minZ = mn.z;
        DAT_00508298.mode = 0x11;
        DAT_00508298.maxY = mx.y;
        DAT_00508298.maxZ = mx.z;
        st->Collect(&DAT_00508298, &sp.x);

        U40A850Body* c = body;
        c->dt = (float)dt * 0.001f;
        Vec3 g(0.0f, c->mass * -14.715f, 0.0f);
        c->force[1] += g.y;
        Vec3 n = *(Vec3*)c->vel;
        float m2 = n.x * n.x + (n.y * n.y + n.z * n.z);
        if (0.1f < m2) {
            float inv = 1.0f / (float)sqrt(n.x * n.x + (n.z * n.z + n.y * n.y));
            n.x = inv * n.x;
            n.y = n.y * inv;
            n.z = n.z * inv;
            UPod r = ((UV3*)&n)->Mul(-(m2 * k));
            c->force[0] += r.x;
            c->force[1] += r.y;
            c->force[2] += r.z;
        }
        c->FUN_0041c010();
        c->FUN_0041fc60(0, st);
        c->FUN_0041c010();
        if (c->FUN_0041ca70(&DAT_00508298, 0)) {
            c->FUN_0041ef60(0, st, 1);
            c->FUN_0041c010();
        }
        c->FUN_0041c540();
        c->FUN_0041c220(0.1f, 0.15f);
        float a1 = c->vel[0], a2 = c->vel[1], a3 = c->vel[2];
        UPod b = *(UPod*)c->ang;
        UPod dv; dv.x = a1 - v0.x; dv.y = a2 - v0.y; dv.z = a3 - v0.z;
        UPod dw; dw.x = b.x - w0.x; dw.y = b.y - w0.y; dw.z = b.z - w0.z;
        if (a1 * a1 + a2 * a2 + a3 * a3 < 0.001f && b.z * b.z + b.y * b.y + b.x * b.x < 0.001f &&
            dv.z * dv.z + dv.y * dv.y + dv.x * dv.x < 0.001f &&
            dw.z * dw.z + dw.y * dw.y + dw.x * dw.x < 0.001f)
            active = 0;
        if (30000 < time)
            active = 0;
    }
}
