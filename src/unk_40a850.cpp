// Prop-vs-cars physics step (0x40a850).
#include "unk_40a850.h"

extern U40A850Query DAT_00508298;
Vec3* __cdecl FUN_0040b0c0(Vec3* a, Vec3* out, float s); // original: Vec3::operator*(float) at 0x40b0c0 (cdecl-looking in pseudocode)

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
// original: FUN_0040a850
void U40A850Prop::Fn(int dt)
{
    U40A850List* l = list;
    time += dt;
    U40A850Prop* volatile self = this;
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
                Vec3 d(a->pos[0], a->pos[1], a->pos[2]); d.x -= b->pos[0]; d.y -= b->pos[1]; d.z -= b->pos[2];
                if (d.y * d.y + d.z * d.z + d.x * d.x <= 1000.0f) {
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
        float v0x = a->vel[0], v0y = a->vel[1], v0z = a->vel[2];
        float w0x = a->ang[0], w0y = a->ang[1], w0z = a->ang[2];
        U40A850Box* bx = a->box;
        Vec3 lo = *(Vec3*)bx->lo;
        Vec3 hi = *(Vec3*)bx->hi;
        lo.x -= 0.1f; lo.y -= 0.1f; lo.z -= 0.1f;
        hi.x += 0.1f; hi.y += 0.1f; hi.z += 0.1f;
        Vec3 ax = *(Vec3*)a->mat[0];
        Vec3 ay = *(Vec3*)a->mat[1];
        Vec3 az = *(Vec3*)a->mat[2];
        float ex = hi.x - lo.x;
        ax.x = ex * ax.x; ax.y = ex * ax.y; ax.z = ex * ax.z;
        float ey = hi.y - lo.y;
        ay.x = ey * ay.x; ay.y = ey * ay.y; ay.z = ey * ay.z;
        float ez = hi.z - lo.z;
        az.x = ez * az.x; az.y = ez * az.y; az.z = ez * az.z;
        Vec3 mn;
        XformT(&mn.x, &lo.x, &a->mat[0][0], a->pos);
        Vec3 mx(mn.x + ax.x, mn.y + ax.y, mn.z + ax.z);
        if (*(unsigned*)&ax.x & 0x80000000) { mx.x = mn.x; mn.x += ax.x; }
        if (*(unsigned*)&ax.y & 0x80000000) { mx.y = mn.y; mn.y += ax.y; }
        if (*(unsigned*)&ax.z & 0x80000000) { mx.z = mn.z; mn.z += ax.z; }
        if (*(unsigned*)&ay.x & 0x80000000) mn.x += ay.x; else mx.x += ay.x;
        if (*(unsigned*)&ay.y & 0x80000000) mn.y += ay.y; else mx.y += ay.y;
        if (*(unsigned*)&ay.z & 0x80000000) mn.z += ay.z; else mx.z += ay.z;
        if (*(unsigned*)&az.x & 0x80000000) mn.x += az.x; else mx.x += az.x;
        if (*(unsigned*)&az.y & 0x80000000) mn.y += az.y; else mx.y += az.y;
        if (*(unsigned*)&az.z & 0x80000000) mn.z += az.z; else mx.z += az.z;
        float s[4];
        s[0] = (mx.x - mn.x) * 0.5f;
        s[1] = (mx.y - mn.y) * 0.5f;
        s[2] = (mx.z - mn.z) * 0.5f;
        s[3] = (float)sqrt(s[1] * s[1] + s[2] * s[2] + s[0] * s[0]);
        s[0] += mn.x;
        s[1] += mn.y;
        s[2] += mn.z;
        DAT_00508298.v.erase(DAT_00508298.v.begin(), DAT_00508298.v.end());
        U40A850Stage* st = stage;
        DAT_00508298.minX = mn.x;
        DAT_00508298.sphere = s;
        DAT_00508298.maxX = mx.x;
        DAT_00508298.minY = mn.y;
        DAT_00508298.minZ = mn.z;
        DAT_00508298.mode = 0x11;
        DAT_00508298.maxY = mx.y;
        DAT_00508298.maxZ = mx.z;
        st->Collect(&DAT_00508298, s);

        U40A850Body* c = body;
        c->dt = (float)dt * 0.001f;
        Vec3 g(0.0f, c->mass * -14.715f, 0.0f);
        c->force[1] += g.y;
        Vec3 n(c->vel[0], c->vel[1], c->vel[2]);
        float m2 = n.z * n.z + n.y * n.y + n.x * n.x;
        if (0.1f < m2) {
            float inv = 1.0f / (float)sqrt(n.y * n.y + n.z * n.z + n.x * n.x);
            n.x = inv * n.x;
            n.y = n.y * inv;
            n.z = n.z * inv;
            Vec3 tmp;
            Vec3* r = FUN_0040b0c0(&n, &tmp, -(m2 * k));
            c->force[0] += r->x;
            c->force[1] += r->y;
            c->force[2] += r->z;
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
        float b1 = c->ang[0], b2 = c->ang[1], b3 = c->ang[2];
        if (a1 * a1 + a2 * a2 + a3 * a3 < 0.001f && b1 * b1 + b2 * b2 + b3 * b3 < 0.001f &&
            (a1 - v0x) * (a1 - v0x) + (a2 - v0y) * (a2 - v0y) + (a3 - v0z) * (a3 - v0z) < 0.001f &&
            (b1 - w0x) * (b1 - w0x) + (b2 - w0y) * (b2 - w0y) + (b3 - w0z) * (b3 - w0z) < 0.001f)
            active = 0;
        if (30000 < time)
            active = 0;
    }
}
