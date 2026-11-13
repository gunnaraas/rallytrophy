// 0x4bb3a0: sweep a segment-vs-sphere test over a scene node's model triangles.
#include <string>
#include "unk_41d7d0.h"
#include "unk_4bb3a0.h"

class TraceMat {
public:
    TraceMat() {}
    ~TraceMat() {}
    void SetInverseOf(const float* src);   // original: FUN_00472c90
    float m[16];
};
inline void Xform(float* o, const float* p, const float* m)
{
    __asm {
        mov ecx, p
        mov edx, m
        mov eax, o
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
        fadd dword ptr [edx+0x30]
        fxch st(1)
        fadd dword ptr [edx+0x34]
        fxch st(2)
        fadd dword ptr [edx+0x38]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax+8]
        fstp dword ptr [eax+4]
    }
}
__forceinline void AddTo(Vec3& a, const Vec3& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; }
struct GV3 { float v[3]; GV3() {} ~GV3() {} };
struct TraceGuard { ~TraceGuard() {} };
extern SegmentInfo DAT_00581de8;
extern int DAT_00581e28;
extern float DAT_00581e20;
extern float DAT_004e134c, DAT_004e1348;
extern double DAT_004e2a98; // 0.0001
char __cdecl PointInTri(float* pt, float* n, char* a, char* b, char* c);   // original: FUN_004b3ee0
bool __cdecl TriTestA(void* model, unsigned short* tri, void* verts, float* pt);   // original: FUN_004bba10
float __cdecl TriTestB(void* model, unsigned short* tri, void* verts, float* pt); // original: FUN_004bbb30

// NONMATCHING: RALLYTROPHY 0x4bb3a0
// Logic complete; original keeps the node in ebx and 0 in edi, and orders the translation dot-products differently (1662 vs 1635 bytes).
// Tried 2026-10-30 night: AddTo inline vec add, copy-init Vec3, switch(DAT_00581e28), d/e/facing in dead param slot, GV3 memory diff vectors, TraceMeshPart nTris at 0x0e, guard first, reversed dot order; still differs: original keeps 0 in edi (i=0 CSE'd with push 0/cmp), c/rad reloaded from [ebx+0xc4] without address CSE, translation dot loads w[14] first, stack slot order.
// Re-pass 2026-11-01: all 15 float sums re-associated to the asm fadd order (m12-14, d, df, tri d, ef, t, second, den; rounding fix, score unchanged 251 insns); also tried zero-in-edi (i/volatile z at top), set/list/seg/guard declaration order, volatile list load: no change or worse.
// Pass 2026-11-13 (big-function agent): 252 -> 206 insns differ, size 1616 -> 1633 (orig 1635). Kept: sphere centre/radius read straight from the node (CC/RAD macros), parts[i] re-read per member, p = tv+24 via the re-read, dir pointer for the first sphere dot and first loop dot, if(cnt) do-while. Rejected: zero-in-edi via i declared early (pools 0 but spills i), inlined dq, named SegmentInfo fields, dir for the 2nd loop dot. Still open: 0 pooled in edi / node in ebx, tv kept in a stack slot with tris in ebx, 2nd loop dot lea of &s[9].
float __cdecl TraceNodeSweep(TraceNode* nParam)
{
    TraceNode* n = nParam;
    TraceGuard g;
    float* nf = (float*)n;
    std::vector<float*>* list = n->triData;
    TraceMeshSet* set = ((TraceMeshOwner*)&n->pad124)->At(0);
    SegmentInfo* seg = &DAT_00581de8;
    SegmentInfo loc;
    if (list != 0 && set != 0) {
        if ((n->flags & 4) == 0) {
            if ((n->flags & 8) == 0)
                return DAT_004e1348;
            TraceMat m;
            float* w = nf + 0x18;
            m.SetInverseOf(w);
            m.m[12] = -(w[14] * w[2] + w[13] * w[1] + w[12] * w[0]);
            m.m[13] = -(w[14] * w[6] + w[13] * w[5] + w[12] * w[4]);
            m.m[14] = -(w[14] * w[10] + w[13] * w[9] + w[8] * w[12]);
            Xform(loc.a, DAT_00581de8.a, m.m);
            Xform(loc.b, DAT_00581de8.b, m.m);
            loc.Update();
            seg = &loc;
        }
        float* s = (float*)seg;
#define CC(k) (((float*)n)[0x2e+(k)])
#define RAD (((float*)n)[0x31])
        float& d = *(float*)&nParam;
        float* dir = s + 9; d = CC(2) * dir[2]; d = CC(1) * dir[1] + d; d = CC(0) * dir[0] + d; d = d - s[12]; // chained: VC6 re-associates the one-expression sum
        if (-RAD <= d && d <= s[13] + RAD) {
            Vec3 q = *(Vec3*)(s + 3);
            AddTo(q, d * *(Vec3*)(s + 9));
            GV3 dfv; float* df = dfv.v;
            df[0] = q.x - CC(0); df[1] = q.y - CC(1); df[2] = q.z - CC(2);
            float dq = df[0] * df[0]; dq = df[2] * df[2] + dq; dq = df[1] * df[1] + dq;
            if (dq <= RAD * RAD) {
                float best = 1.0f;
                for (unsigned i = 0; i < list->size(); i++) {
                    float* tv = (*list)[i];
                    unsigned cnt = set->parts[i]->nTris;
                    float* model = (float*)set->parts[i]->model;
                    char* verts = (char*)set->parts[i]->verts;
                    unsigned twoSided = ((int*)model)[0x27] & 1;
                    unsigned short* tri = set->parts[i]->tris;
                    float* p = (float*)((char*)(*list)[i] + 24);
                    if (cnt != 0) do {
                        d = p[-4] * dir[2]; d = p[-5] * dir[1] + d; d = dir[0] * tv[0] + d;
                        if (twoSided != 0 || *(int*)&d <= 0) {
                            float r = p[1];
                            d = s[11] * p[0]; d = s[10] * p[-1] + d; d = s[9] * p[-2] + d; d = d - s[12];
                            if (-r <= d && d <= r + s[13]) {
                                Vec3 w = *(Vec3*)(s + 3);
                                AddTo(w, d * *(Vec3*)(s + 9));
                                GV3 efv; float* ef = efv.v;
                                ef[0] = w.x - p[-2]; ef[1] = w.y - p[-1]; ef[2] = w.z - p[0];
                                float t;
                                float den;
                                float eq = ef[0] * ef[0]; eq = ef[2] * ef[2] + eq; eq = ef[1] * ef[1] + eq;
                                float t2;
                                if (eq <= r * r
                                    && (t = (s[5] * p[-4] + s[4] * p[-5] + tv[0] * s[3]) - p[-3],
                                        t2 = s[8] * p[-4], t2 = s[7] * p[-5] + t2, t2 = s[6] * tv[0] + t2,
                                        (t2 - p[-3]) * t <= DAT_004e134c)
                                    && (den = s[2] * p[-4] + s[1] * p[-5] + s[0] * tv[0], DAT_004e2a98 <= (float)fabs(den))
                                    && (t = -(t / den), DAT_004e134c <= t && t <= DAT_004e1348)) {
                                    Vec3 hit = *(Vec3*)(s + 3);
                                    AddTo(hit, t * *(Vec3*)s);
                                    if (PointInTri(&hit.x, tv, verts + tri[0] * 0x2c, verts + tri[1] * 0x2c, verts + tri[2] * 0x2c)) {
                                        switch (DAT_00581e28) {
                                        case 0:
                                            if (t < best)
                                                best = t;
                                            break;
                                        case 1:
                                            if (t < best && TriTestA(model, tri, verts, &hit.x))
                                                best = t;
                                            break;
                                        case 2:
                                            DAT_00581e20 = TriTestB(model, tri, verts, &hit.x) + DAT_00581e20;
                                            break;
                                        }
                                        if (DAT_004e1348 <= DAT_00581e20)
                                            return DAT_004e134c;
                                    }
                                }
                            }
                        }
                        cnt--;
                        p += 8;
                        tri += 3;
                        tv += 8;
                    } while (cnt != 0);
                }
                return best;
            }
        }
    }
    return DAT_004e1348;
}
