// Prop-vs-cars physics step (0x40a850).
#include "unk_40a850.h"

extern U40A850Query DAT_00508298;
Vec3* __cdecl FUN_0040b0c0(Vec3* a, Vec3* out, float s); // original: Vec3::operator*(float) at 0x40b0c0 (cdecl-looking in pseudocode)

// NONMATCHING: RALLYTROPHY 0x40a850
// reason: 2131 vs 2098 bytes; original uses an EBP frame (ours ESP-based), keeps this/list in stack locals, different FPU scheduling; logic complete
// original: FUN_0040a850
void U40A850Prop::Fn(int dt)
{
    U40A850List* l = list;
    time += dt;
    int hit = 0;
    int i = 0;
    if (0 < l->items.Size()) {
        do {
            U40A850Item* it = l->items[i];
            if (l->Accept(it) && it->state != 3 && it->state != 2) {
                U40A850Body* a = body;
                U40A850Car* car = it->car;
                U40A850Body* b = car->body;
                Vec3 d(a->pos[0] - b->pos[0], a->pos[1] - b->pos[1], a->pos[2] - b->pos[2]);
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
        } while (i < l->items.Size());
        if (hit) {
            active = 1;
            time = 0;
        }
    }
    if (active) {
        U40A850Body* a = body;
        float v0x = a->vel[0], v0y = a->vel[1], v0z = a->vel[2];
        float w0x = a->ang[0], w0y = a->ang[1], w0z = a->ang[2];
        U40A850Box* bx = a->box;
        Vec3 lo(bx->lo[0] - 0.1f, bx->lo[1] - 0.1f, bx->lo[2] - 0.1f);
        Vec3 hi(bx->hi[0] + 0.1f, bx->hi[1] + 0.1f, bx->hi[2] + 0.1f);
        float ex = hi.x - lo.x;
        Vec3 ax(ex * a->mat[0][0], ex * a->mat[0][1], ex * a->mat[0][2]);
        float ey = hi.y - lo.y;
        Vec3 ay(ey * a->mat[1][0], ey * a->mat[1][1], ey * a->mat[1][2]);
        float ez = hi.z - lo.z;
        Vec3 az(ez * a->mat[2][0], ez * a->mat[2][1], ez * a->mat[2][2]);
        Vec3 mn(lo.z * a->mat[2][0] + lo.y * a->mat[1][0] + lo.x * a->mat[0][0] + a->pos[0],
                lo.z * a->mat[2][1] + lo.y * a->mat[1][1] + lo.x * a->mat[0][1] + a->pos[1],
                lo.z * a->mat[2][2] + lo.y * a->mat[1][2] + lo.x * a->mat[0][2] + a->pos[2]);
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
        c->FUN_0041fc60(0);
        c->FUN_0041c010();
        if (c->FUN_0041ca70(&DAT_00508298)) {
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
