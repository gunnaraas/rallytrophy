// Collision model built from an axis-aligned box (0x41a890).
#include <string>
#include "unk_41a890.h"
#include <math.h>

extern float DAT_004e1348, DAT_004e138c;
struct P3 { float x, y, z; P3() {} ~P3() {} P3 operator-(const P3& o) const; }; // operator- is 0x414330
// Triangle layout inside ColModel::tris: vertex pointers at 0x28, edge-int pointers at 0x34.
struct CT { char p0[0x28]; TVert* v[3]; int* e[3]; };
// Local element types with user ctor+dtor so new[] goes through the EH vector-ctor helper (0x4c11f4).
struct LV { int pad; float x, y, z; LV() {} ~LV() {} };
struct LT { char pad[0x40]; LT() {} ~LT() {} };
struct LL { float x, y, z; LL() {} ~LL() {} };
#define T(k) (((CT*)tris)[k])

// NONMATCHING: RALLYTROPHY 0x41a890
// reason: original uses an ebp frame (cause unknown; ours is esp-based) and reuses arg slots as temps; logic complete, tri loop/EH states approximated
ColModel::ColModel(void* mesh, int a, int b)
{
    float* p = (float*)mesh;
    root = 0;
    pad24 = 1;
    if (b == 0) {
        nVerts = 8;
        nTris = 12;
        nInts = 18;
        verts = (ColVert*)new LV[8];
        wverts = (ColVert*)new LV[8];
        tris = (ColTri*)new LT[12];
        centers = (ColLocal*)new LL[12];
        float* vf = (float*)verts;
        vf[0x1d] = p[0]; vf[0x11] = p[0]; vf[0xd] = p[0]; vf[1] = p[0];
        vf[0x19] = p[3]; vf[0x15] = p[3]; vf[9] = p[3]; vf[5] = p[3];
        vf[0x16] = p[4]; vf[0x12] = p[4]; vf[6] = p[4]; vf[2] = p[4];
        vf[0x1e] = p[1]; vf[0x1a] = p[1]; vf[0xe] = p[1]; vf[10] = p[1];
        vf[0xf] = p[5]; vf[0xb] = p[5]; vf[7] = p[5]; vf[3] = p[5];
        vf[0x1f] = p[2]; vf[0x1b] = p[2]; vf[0x17] = p[2]; vf[0x13] = p[2];
        for (int i = 0; i < 8; i++) {
            wverts[i].x = verts[i].x;
            wverts[i].y = verts[i].y;
            wverts[i].z = verts[i].z;
        }
        ints = new int[18];
        int* ip = ints;
        ColVert* wv = wverts;
    T(0).v[0] = (TVert*)(wv + 0);
    T(0).v[1] = (TVert*)(wv + 3);
    T(0).v[2] = (TVert*)(wv + 2);
    T(0).e[0] = ip + 0;
    T(0).e[1] = ip + 4;
    T(0).e[2] = ip + 12;
    T(1).v[0] = (TVert*)(wv + 0);
    T(1).v[1] = (TVert*)(wv + 2);
    T(1).v[2] = (TVert*)(wv + 1);
    T(1).e[0] = ip + 12;
    T(1).e[1] = ip + 1;
    T(1).e[2] = ip + 4;
    T(2).v[0] = (TVert*)(wv + 4);
    T(2).v[1] = (TVert*)(wv + 5);
    T(2).v[2] = (TVert*)(wv + 6);
    T(2).e[0] = ip + 6;
    T(2).e[1] = ip + 2;
    T(2).e[2] = ip + 13;
    T(3).v[0] = (TVert*)(wv + 4);
    T(3).v[1] = (TVert*)(wv + 6);
    T(3).v[2] = (TVert*)(wv + 7);
    T(3).e[0] = ip + 13;
    T(3).e[1] = ip + 7;
    T(3).e[2] = ip + 3;
    T(4).v[0] = (TVert*)(wv + 0);
    T(4).v[1] = (TVert*)(wv + 1);
    T(4).v[2] = (TVert*)(wv + 5);
    T(4).e[0] = ip + 4;
    T(4).e[1] = ip + 9;
    T(4).e[2] = ip + 14;
    T(5).v[0] = (TVert*)(wv + 0);
    T(5).v[1] = (TVert*)(wv + 5);
    T(5).v[2] = (TVert*)(wv + 4);
    T(5).e[0] = ip + 14;
    T(5).e[1] = ip + 6;
    T(5).e[2] = ip + 8;
    T(6).v[0] = (TVert*)(wv + 7);
    T(6).v[1] = (TVert*)(wv + 6);
    T(6).v[2] = (TVert*)(wv + 2);
    T(6).e[0] = ip + 7;
    T(6).e[1] = ip + 11;
    T(6).e[2] = ip + 15;
    T(7).v[0] = (TVert*)(wv + 7);
    T(7).v[1] = (TVert*)(wv + 2);
    T(7).v[2] = (TVert*)(wv + 3);
    T(7).e[0] = ip + 15;
    T(7).e[1] = ip + 5;
    T(7).e[2] = ip + 10;
    T(8).v[0] = (TVert*)(wv + 5);
    T(8).v[1] = (TVert*)(wv + 1);
    T(8).v[2] = (TVert*)(wv + 2);
    T(8).e[0] = ip + 9;
    T(8).e[1] = ip + 1;
    T(8).e[2] = ip + 16;
    T(9).v[0] = (TVert*)(wv + 5);
    T(9).v[1] = (TVert*)(wv + 2);
    T(9).v[2] = (TVert*)(wv + 6);
    T(9).e[0] = ip + 16;
    T(9).e[1] = ip + 11;
    T(9).e[2] = ip + 2;
    T(10).v[0] = (TVert*)(wv + 0);
    T(10).v[1] = (TVert*)(wv + 4);
    T(10).v[2] = (TVert*)(wv + 7);
    T(10).e[0] = ip + 8;
    T(10).e[1] = ip + 3;
    T(10).e[2] = ip + 17;
    T(11).v[0] = (TVert*)(wv + 0);
    T(11).v[1] = (TVert*)(wv + 7);
    T(11).v[2] = (TVert*)(wv + 3);
    T(11).e[0] = ip + 17;
    T(11).e[1] = ip + 10;
    T(11).e[2] = ip + 0;
    }
    else {
        nVerts = 6;
        nTris = 8;
        nInts = 12;
        verts = (ColVert*)new LV[6];
        wverts = (ColVert*)new LV[6];
        tris = (ColTri*)new LT[8];
        centers = (ColLocal*)new LL[8];
        float* vf = (float*)verts;
        vf[0x11] = p[0]; vf[9] = p[0]; vf[1] = p[0];
        vf[0x15] = p[3]; vf[0xd] = p[3]; vf[5] = p[3];
        vf[6] = p[4]; vf[2] = p[4];
        vf[0x16] = p[1]; vf[0x12] = p[1]; vf[0xe] = p[1]; vf[10] = p[1];
        vf[0xf] = p[5]; vf[0xb] = p[5];
        vf[0x17] = p[2]; vf[0x13] = p[2];
        float m = (p[2] + p[5]) * DAT_004e138c;
        vf[7] = m;
        vf[3] = m;
        for (int i = 0; i < 6; i++) {
            wverts[i].x = verts[i].x;
            wverts[i].y = verts[i].y;
            wverts[i].z = verts[i].z;
        }
        ints = new int[12];
        int* ip = ints;
        ColVert* wv = wverts;
    T(0).v[0] = (TVert*)(wv + 0);
    T(0).v[1] = (TVert*)(wv + 1);
    T(0).v[2] = (TVert*)(wv + 5);
    T(0).e[0] = ip + 0;
    T(0).e[1] = ip + 1;
    T(0).e[2] = ip + 9;
    T(1).v[0] = (TVert*)(wv + 0);
    T(1).v[1] = (TVert*)(wv + 5);
    T(1).v[2] = (TVert*)(wv + 4);
    T(1).e[0] = ip + 9;
    T(1).e[1] = ip + 2;
    T(1).e[2] = ip + 3;
    T(2).v[0] = (TVert*)(wv + 1);
    T(2).v[1] = (TVert*)(wv + 0);
    T(2).v[2] = (TVert*)(wv + 2);
    T(2).e[0] = ip + 0;
    T(2).e[1] = ip + 4;
    T(2).e[2] = ip + 10;
    T(3).v[0] = (TVert*)(wv + 1);
    T(3).v[1] = (TVert*)(wv + 2);
    T(3).v[2] = (TVert*)(wv + 3);
    T(3).e[0] = ip + 10;
    T(3).e[1] = ip + 5;
    T(3).e[2] = ip + 6;
    T(4).v[0] = (TVert*)(wv + 3);
    T(4).v[1] = (TVert*)(wv + 2);
    T(4).v[2] = (TVert*)(wv + 4);
    T(4).e[0] = ip + 5;
    T(4).e[1] = ip + 7;
    T(4).e[2] = ip + 11;
    T(5).v[0] = (TVert*)(wv + 3);
    T(5).v[1] = (TVert*)(wv + 4);
    T(5).v[2] = (TVert*)(wv + 5);
    T(5).e[0] = ip + 11;
    T(5).e[1] = ip + 2;
    T(5).e[2] = ip + 8;
    T(6).v[0] = (TVert*)(wv + 1);
    T(6).v[1] = (TVert*)(wv + 3);
    T(6).v[2] = (TVert*)(wv + 5);
    T(6).e[0] = ip + 6;
    T(6).e[1] = ip + 8;
    T(6).e[2] = ip + 1;
    T(7).v[0] = (TVert*)(wv + 0);
    T(7).v[1] = (TVert*)(wv + 4);
    T(7).v[2] = (TVert*)(wv + 2);
    T(7).e[0] = ip + 3;
    T(7).e[1] = ip + 7;
    T(7).e[2] = ip + 4;
    }
    b = 0;
    if (0 < nTris) {
        int cb = 0;
        int off = 0;
        do {
            Tri* t = (Tri*)((char*)tris + off);
            t->id = 0;
            ((SceneSphere*)t)->SetTriple((ScenePoint*)&t->v[0]->x, (ScenePoint*)&t->v[1]->x, (ScenePoint*)&t->v[2]->x);
            float* n = &t->nx;
            P3 s2 = *(P3*)&t->v[2]->x - *(P3*)&t->v[0]->x;
            float* l20 = &s2.x;
            P3 s1 = *(P3*)&t->v[1]->x - *(P3*)&t->v[0]->x;
            float* l28 = &s1.x;
            float f2 = l28[2], f3 = l20[0], f4 = l28[0], f5 = l20[1], f7 = l20[2], f8 = l28[1];
            n[0] = f8 * f7 - f2 * f5;
            n[1] = f2 * f3 - f4 * f7;
            n[2] = f4 * f5 - f8 * f3;
            char* tb = (char*)tris;
            float* ci = (float*)centers;
            float k = DAT_004e1348 / (float)sqrt(*(float*)(tb + off + 0x20) * *(float*)(tb + off + 0x20) + *(float*)(tb + off + 0x1c) * *(float*)(tb + off + 0x1c) + *(float*)(tb + off + 0x18) * *(float*)(tb + off + 0x18));
            *(float*)(tb + off + 0x18) = k * *(float*)(tb + off + 0x18);
            *(float*)(tb + off + 0x1c) = k * *(float*)(tb + off + 0x1c);
            *(float*)(tb + off + 0x20) = k * *(float*)(tb + off + 0x20);
            float* v0 = &(*(TVert**)(tb + off + 0x28))->x;
            *(float*)(tb + off + 0x24) = v0[0] * *(float*)(tb + off + 0x18) + v0[1] * *(float*)(tb + off + 0x1c) + v0[2] * *(float*)(tb + off + 0x20);
            float* c = (float*)((char*)ci + cb);
            c[0] = *(float*)(tb + off);
            c[1] = *(float*)(tb + off + 4);
            cb += 0xc;
            c[2] = *(float*)(tb + off + 8);
            b++;
            off += 0x40;
        } while (b < nTris);
    }
    ClearInts();
}
