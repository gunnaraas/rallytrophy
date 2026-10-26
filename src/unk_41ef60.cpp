// Car body contact response: turns the contact records into forces/torques on the body.
#include "unk_41ef60.h"

Vec3 FUN_004203e0(const Vec3& a, const Vec3& b); // 0x4203e0 cross product
#include <math.h>
extern float DAT_004e1348;
extern float DAT_004e1448;
extern float DAT_004e134c;
extern float DAT_004e1444;
extern float DAT_004e1440;
extern float DAT_004e13dc;

#define NEG(f) (*(unsigned int*)&(f) & 0x80000000)
#define BF(o) (*(float*)((char*)this + (o)))
#define SF(o) (*(float*)((char*)stage + (o)))
// Stage per-surface tables indexed by the triangle's id.
#define SID(h) (*(short*)(*(char**)&(h)[2] + 0x10))

// original: FUN_0041ef60
// NONMATCHING: RALLYTROPHY 0x41ef60
// NONMATCHING: first-draft port, not yet compared against the original
void RespBody::Respond(void* car, void* stage, int flag)
{
    if (*(int*)((char*)this + 0x23c) == 0) {
        return;
    }
    float posS[3], negS[3];
    posS[0] = posS[1] = posS[2] = 0.0f;
    negS[0] = negS[1] = negS[2] = 0.0f;
    float limit;
    if (car == 0) {
        limit = 1000.0f;
    } else {
        limit = *(float*)((char*)car + 0x1080);
    }
    float c0 = BF(0x1cc), c1 = BF(0x1d0), c2 = BF(0x1d4);
    float* h = *(float**)((char*)this + 0x238);
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)this + 0x23c); i++, h += 20) {
        float vx, vy, vz;
        if (flag == 0) {
            vx = h[9];
            vy = h[10];
            vz = h[11];
        } else {
            Vec3 pt(h[3], h[4], h[5]);
            Vec3 w = FUN_004204f0(pt);
            vx = c0 + w.x;
            vy = c1 + w.y;
            vz = c2 + w.z;
        }
        float d = vz * h[8] + vy * h[7] + vx * h[6];
        if (NEG(d) == 0) {
            *(int*)h = 0;
        } else {
            *(int*)h = 1;
            float ax = h[6] * d, ay = h[7] * d, az = h[8] * d;
            h[10] = vy;
            h[11] = vz;
            h[9] = vx;
            h[12] = ax;
            h[13] = ay;
            h[14] = az;
            if (NEG(ax) == 0) posS[0] = posS[0] + ax; else negS[0] = negS[0] + ax;
            if (NEG(ay) == 0) posS[1] = posS[1] + ay; else negS[1] = negS[1] + ay;
            if (NEG(az) == 0) posS[2] = posS[2] + az; else negS[2] = negS[2] + az;
        }
    }
    if ((int)posS[0] < 0x3a83126f) posS[0] = 0.001f;
    if ((int)posS[1] < 0x3a83126f) posS[1] = 0.001f;
    if ((int)posS[2] < 0x3a83126f) posS[2] = 0.001f;
    if (DAT_004e1448 < negS[0]) negS[0] = -0.001f;
    if (DAT_004e1448 < negS[1]) negS[1] = -0.001f;
    if (DAT_004e1448 < negS[2]) negS[2] = -0.001f;
    negS[0] = DAT_004e1348 / negS[0];
    negS[1] = DAT_004e1348 / negS[1];
    negS[2] = DAT_004e1348 / negS[2];
    posS[0] = DAT_004e1348 / posS[0];
    posS[1] = DAT_004e1348 / posS[1];
    posS[2] = DAT_004e1348 / posS[2];
    float invDt = DAT_004e1348 / BF(0x214);

    float fp[3], fn[3];             // accumulated impulse per axis (positive / negative parts)
    fp[0] = fp[1] = fp[2] = 0.0f;
    fn[0] = fn[1] = fn[2] = 0.0f;
    h = *(float**)((char*)this + 0x238);
    for (i = 0; i < *(unsigned int*)((char*)this + 0x23c); i++, h += 20) {
        if (*(int*)h == 0) continue;
        float s0 = NEG(h[12]) ? negS[0] : posS[0];
        float s1 = NEG(h[13]) ? negS[1] : posS[1];
        float s2 = NEG(h[14]) ? negS[2] : posS[2];
        float e0 = h[12] * s0 * h[12];
        float e1 = h[13] * s1 * h[13];
        float e2 = h[14] * s2 * h[14];
        float pen = -(e1 * h[7] + e2 * h[8] + e0 * h[6]);
        if (pen <= DAT_004e134c) {
            *(int*)h = 0;
            continue;
        }
        Vec3 r(h[3] - BF(0x224), h[4] - BF(0x228), h[5] - BF(0x22c));
        Vec3 nrm(h[6], h[7], h[8]);
        Vec3 rn = FUN_004203e0(r, nrm);
        float bx = rn.z * BF(0x17c) + rn.y * BF(0x16c) + rn.x * BF(0x15c) + BF(0x18c);
        float by = rn.z * BF(0x180) + rn.y * BF(0x170) + rn.x * BF(0x160) + BF(0x190);
        float bz = rn.z * BF(0x184) + rn.y * BF(0x174) + rn.x * BF(0x164) + BF(0x194);
        float cx = by * r.z - bz * r.y;
        float cy = bz * r.x - bx * r.z;
        float cz = bx * r.y - by * r.x;
        float k = DAT_004e1348 / (cy * h[7] + cz * h[8] + cx * h[6] + BF(0x130));
        float imp = k * pen;

        // tangential (friction) direction: velocity minus its normal component
        Vec3 vel(h[9], h[10], h[11]);
        Vec3 nv(h[6], h[7], h[8]);
        float dn = vel.x * h[6] + vel.y * h[7] + vel.z * h[8];
        Vec3 nn = nv * dn;
        float tx = vel.x - nn.x, ty = vel.y - nn.y, tz = vel.z - nn.z;
        float t2 = tx * tx + ty * ty + tz * tz;
        if (t2 <= DAT_004e1444) {
            *(int*)h = 0;
        } else {
            float tl = (float)sqrt(t2);
            float inv = DAT_004e1348 / tl;
            tx = inv * tx;
            h[16] = tx;
            ty = ty * inv;
            h[17] = ty;
            tz = inv * tz;
            h[19] = tl;
            h[18] = tz;
            float grip = SF(0x42c + SID(h) * 4) * DAT_004e1440;
            tl = tl * invDt;
            float f = imp * invDt;
            if (tl < grip) {
                f = (tl / grip) * f;
            }
            h[15] = f;
            float fx = h[16] * f, fy = h[17] * f;
            if (NEG(h[16]) == 0) fp[0] = fp[0] + fx; else fn[0] = fn[0] - fx;
            if (NEG(h[17]) == 0) fp[1] = fp[1] + fy; else fn[1] = fn[1] - fy;
            if (NEG(h[18]) == 0) fp[2] = fp[2] + f * h[18]; else fn[2] = fn[2] - f * h[18];
        }
        float bounce = SF(0x29c + SID(h) * 4) * imp;
        Vec3 F(h[6] * bounce, h[7] * bounce, h[8] * bounce);
        BF(0x1fc) = F.x + BF(0x1fc);
        BF(0x200) = F.y + BF(0x200);
        BF(0x204) = F.z + BF(0x204);
        Vec3 arm(h[3] - BF(0x224), h[4] - BF(0x228), h[5] - BF(0x22c));
        Vec3 tq = FUN_004203e0(F, arm);
        BF(0x208) = BF(0x208) + tq.x;
        BF(0x20c) = tq.y + BF(0x20c);
        BF(0x210) = tq.z + BF(0x210);
        if (car != 0) {
            float depth = -(h[10] * h[7] + h[11] * h[8] + h[9] * h[6]);
            if (limit < depth) {
                float lx = h[3] - BF(0x14), ly = h[4] - BF(0x18), lz = h[5] - BF(0x1c);
                float p[3], q[3];
                p[0] = lx * BF(0x20) + ly * BF(0x24) + lz * BF(0x28);
                p[1] = lx * BF(0x30) + ly * BF(0x34) + lz * BF(0x38);
                p[2] = lx * BF(0x40) + ly * BF(0x44) + lz * BF(0x48);
                q[0] = BF(0x20) * h[6] + BF(0x24) * h[7] + BF(0x28) * h[8];
                q[1] = h[6] * BF(0x30) + BF(0x34) * h[7] + BF(0x38) * h[8];
                q[2] = BF(0x40) * h[6] + BF(0x44) * h[7] + BF(0x48) * h[8];
                ((Car*)car)->AddContact((Vec3*)p, (Vec3*)q, depth);
                if (BF(0x240) < depth * k) {
                    BF(0x25c) = h[6];
                    BF(0x260) = h[7];
                    BF(0x264) = h[8];
                    BF(0x250) = F.x * BF(0x20) + F.y * BF(0x24) + F.z * BF(0x28);
                    BF(0x254) = F.x * BF(0x30) + F.y * BF(0x34) + F.z * BF(0x38);
                    BF(0x24c) = p[2];
                    BF(0x248) = p[1];
                    BF(0x244) = p[0];
                    BF(0x258) = F.x * BF(0x40) + F.y * BF(0x44) + F.z * BF(0x48);
                    BF(0x240) = depth * k;
                }
            }
        }
    }

    float cap = BF(0x12c) * DAT_004e1440;
    float sc[6];
    float acc[6] = { fp[0], fp[1], fp[2], fn[0], fn[1], fn[2] };
    for (int j = 0; j < 6; j++) {
        if ((int)cap < (int)acc[j]) sc[j] = cap / acc[j]; else sc[j] = 1.0f;
    }

    h = *(float**)((char*)this + 0x238);
    for (i = 0; i < *(unsigned int*)((char*)this + 0x23c); i++, h += 20) {
        if (*(int*)h == 0) continue;
        float dx = h[16], dy = h[17], dz = h[18];
        float m0 = NEG(dx) == 0 ? sc[0] : sc[3];
        float m1 = NEG(dy) == 0 ? sc[1] : sc[4];
        float m2 = (NEG(dz) != 0) ? sc[5] : sc[2];
        float fm = h[15];
        float g0 = fm * dx * m0;
        float g1 = fm * dy * m1;
        float g2 = fm * m2 * dz;
        float work = (dy * g1 + dz * g2 + dx * g0) * SF(0x42c + SID(h) * 4);
        if (*(int*)((char*)stage + 0x4cc + SID(h) * 4) != 0 && DAT_004e13dc < h[19] && BF(0x268) < work) {
            BF(0x268) = work;
            BF(0x26c) = h[3];
            BF(0x270) = h[4];
            BF(0x274) = h[5];
            BF(0x278) = h[6];
            BF(0x27c) = h[7];
            BF(0x280) = h[8];
        }
        Vec3 g(dx * work, dy * work, dz * work);
        Vec3 ng = -g;
        BF(0x1e4) = BF(0x1e4) + ng.x;
        BF(0x1e8) = ng.y + BF(0x1e8);
        BF(0x1ec) = ng.z + BF(0x1ec);
        Vec3 arm(h[3] - BF(0x224), h[4] - BF(0x228), h[5] - BF(0x22c));
        Vec3 tq = FUN_004203e0(ng, arm);
        BF(0x1f0) = tq.x + BF(0x1f0);
        BF(0x1f4) = tq.y + BF(0x1f4);
        BF(0x1f8) = tq.z + BF(0x1f8);
    }
    FUN_0041c010();
    *(int*)((char*)this + 0x23c) = 0;
}
