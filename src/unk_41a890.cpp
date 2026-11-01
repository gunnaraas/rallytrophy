// Collision model built from an axis-aligned box (0x41a890).
#include <string>
#include "unk_41a890.h"
#include <math.h>
#include "x87_cross.h"

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
// Tried 2026-10-31: locals V/W + chained float stores (429->226 insns), volatile pad24/ints/zero, z-pointer/ptr-increment copy loops, loop counters in dead mesh/b slots; still differs: original keeps 0 in edi (cmp eax,edi, immediate 1s) while ours puts 1 in a register, and the tri loop temps/regs swap.
// Tried 2026-11-01: 25 variants of the entry (zero as register var feeding root/b==/cnt, volatile or plain pad24, plain 1-store placement, bb=b first); all stay at 227-231 insns differ: original keeps 0 in edi (no stack zero slot, W null path needs no xor, 1s stay immediates) while ours CSEs 1 into edi and zero into ebx, which cascades through the whole function.
ColModel::ColModel(void* mesh, int a, int b)
{
    float* p = (float*)mesh;
    root = 0;
    int zero = 0;
    *(volatile int*)&zero = 0;
    pad24 = 1;
    if (b == 0) {
        nVerts = 8;
        nTris = 12;
        nInts = 18;
        ColVert* V = (ColVert*)new LV[8]; verts = V;
        ColVert* W = (ColVert*)new LV[8]; wverts = W;
        ColTri* TT = (ColTri*)new LT[12]; tris = TT;
        centers = (ColLocal*)new LL[12];
        float* vf = (float*)V;
        vf[1] = vf[0xd] = vf[0x11] = vf[0x1d] = p[0];
        vf[5] = vf[9] = vf[0x15] = vf[0x19] = p[3];
        vf[2] = vf[6] = vf[0x12] = vf[0x16] = p[4];
        vf[10] = vf[0xe] = vf[0x1a] = vf[0x1e] = p[1];
        vf[3] = vf[7] = vf[0xb] = vf[0xf] = p[5];
        vf[0x13] = vf[0x17] = vf[0x1b] = vf[0x1f] = p[2];
        for (int i = 0; i < 8; i++) {
            W[i].x = V[i].x;
            W[i].y = V[i].y;
            W[i].z = V[i].z;
        }
        int* ip = new int[18]; *(int* volatile*)&ints = ip;
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
        ColVert* V = (ColVert*)new LV[6]; verts = V;
        ColVert* W = (ColVert*)new LV[6]; wverts = W;
        ColTri* TT = (ColTri*)new LT[8]; tris = TT;
        centers = (ColLocal*)new LL[8];
        float* vf = (float*)V;
        vf[1] = vf[9] = vf[0x11] = p[0];
        vf[5] = vf[0xd] = vf[0x15] = p[3];
        vf[2] = vf[6] = p[4];
        vf[10] = vf[0xe] = vf[0x12] = vf[0x16] = p[1];
        vf[0xb] = vf[0xf] = p[5];
        vf[0x13] = vf[0x17] = p[2];
        float m = (p[2] + p[5]) * DAT_004e138c;
        vf[7] = m;
        vf[3] = m;
        for (int i = 0; i < 6; i++) {
            W[i].x = V[i].x;
            W[i].y = V[i].y;
            W[i].z = V[i].z;
        }
        int* ip = new int[12]; *(int* volatile*)&ints = ip;
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
    int& cnt = *(int*)&mesh;
    cnt = 0;
    b = 0;
    if (0 < nTris) {
        int& cb = b;
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
            X87Cross(n, l28, l20);
            char* tb = (char*)tris;
            float* ci = (float*)centers;
            float k = DAT_004e1348 / (float)sqrt(*(float*)(tb + off + 0x20) * *(float*)(tb + off + 0x20) + *(float*)(tb + off + 0x1c) * *(float*)(tb + off + 0x1c) + *(float*)(tb + off + 0x18) * *(float*)(tb + off + 0x18));
            *(float*)(tb + off + 0x18) = k * *(float*)(tb + off + 0x18);
            *(float*)(tb + off + 0x1c) = k * *(float*)(tb + off + 0x1c);
            *(float*)(tb + off + 0x20) = k * *(float*)(tb + off + 0x20);
            float* v0 = &(*(TVert**)(tb + off + 0x28))->x;
            float q = v0[2] * *(volatile float*)(tb + off + 0x20);
            q = v0[1] * *(float*)(tb + off + 0x1c) + q;
            q = v0[0] * *(float*)(tb + off + 0x18) + q;
            *(float*)(tb + off + 0x24) = q;
            float* c = (float*)((char*)ci + cb);
            c[0] = *(float*)(tb + off);
            c[1] = *(float*)(tb + off + 4);
            cb += 0xc;
            c[2] = *(float*)(tb + off + 8);
            cnt++;
            off += 0x40;
        } while (cnt < nTris);
    }
    ClearInts();
}
