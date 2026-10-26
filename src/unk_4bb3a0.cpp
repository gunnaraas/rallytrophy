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
struct TraceGuard { ~TraceGuard() {} };
extern SegmentInfo DAT_00581de8;
extern int DAT_00581e28;
extern float DAT_00581e20;
extern float DAT_004e134c, DAT_004e1348, DAT_004e2a98;
char __cdecl PointInTri(float* pt, float* n, char* a, char* b, char* c);   // original: FUN_004b3ee0
bool __cdecl TriTestA(void* model, unsigned short* tri, void* verts, float* pt);   // original: FUN_004bba10
float __cdecl TriTestB(void* model, unsigned short* tri, void* verts, float* pt); // original: FUN_004bbb30

// NONMATCHING: RALLYTROPHY 0x4bb3a0
// Logic complete; original keeps the node in ebx and 0 in edi, and orders the translation dot-products differently (1662 vs 1635 bytes).
float __cdecl TraceNodeSweep(TraceNode* n)
{
    float* nf = (float*)n;
    std::vector<float*>* list = n->triData;
    TraceMeshSet* set = ((TraceMeshOwner*)&n->pad124)->At(0);
    SegmentInfo* seg = &DAT_00581de8;
    SegmentInfo loc;
    TraceGuard g;
    if (list != 0 && set != 0) {
        if ((n->flags & 4) == 0) {
            if ((n->flags & 8) == 0)
                return DAT_004e1348;
            TraceMat m;
            float* w = nf + 0x18;
            m.SetInverseOf(w);
            m.m[12] = -(w[14] * w[2] + w[13] * w[1] + w[12] * w[0]);
            m.m[13] = -(w[14] * w[6] + w[13] * w[5] + w[12] * w[4]);
            m.m[14] = -(w[8] * w[12] + w[13] * w[9] + w[14] * w[10]);
            Xform(loc.a, DAT_00581de8.a, m.m);
            Xform(loc.b, DAT_00581de8.b, m.m);
            loc.Update();
            seg = &loc;
        }
        float* s = (float*)seg;
        float* c = &nf[0x2e];              // sphere center 0xb8
        float rad = nf[0x31];              // sphere radius 0xc4
        float d = (c[0] * s[9] + c[1] * s[10] + c[2] * s[11]) - s[12];
        if (-rad <= d && d <= s[13] + rad) {
            Vec3 q(s[3], s[4], s[5]);
            q = q + d * *(Vec3*)(s + 9);
            float dx = q.x - c[0], dy = q.y - c[1], dz = q.z - c[2];
            if (dy * dy + dz * dz + dx * dx <= rad * rad) {
                float best = 1.0f;
                for (unsigned i = 0; list->size() > 0 && i < list->size(); i++) {
                    TraceMeshPart* part = set->parts[i];
                    float* tv = (*list)[i];
                    unsigned cnt = part->nTris;
                    float* model = (float*)part->model;
                    char* verts = (char*)part->verts;
                    unsigned short* tri = part->tris;
                    unsigned twoSided = ((int*)model)[0x27] & 1;
                    float* p = tv + 6;
                    while (cnt != 0) {
                        if (twoSided != 0 || (int)(s[9] * tv[0] + p[-5] * s[10] + p[-4] * s[11]) < 1) {
                            float r = p[1];
                            float e = (s[9] * p[-2] + s[10] * p[-1] + s[11] * p[0]) - s[12];
                            if (-r <= e && e <= r + s[13]) {
                                Vec3 w(s[3], s[4], s[5]);
                                w = w + e * *(Vec3*)(s + 9);
                                float ex = w.x - p[-2], ey = w.y - p[-1], ez = w.z - p[0];
                                float t;
                                float den;
                                if (ey * ey + ez * ez + ex * ex <= r * r
                                    && (t = (tv[0] * s[3] + s[4] * p[-5] + s[5] * p[-4]) - p[-3],
                                        ((s[6] * tv[0] + s[7] * p[-5] + s[8] * p[-4]) - p[-3]) * t <= DAT_004e134c)
                                    && (den = s[0] * tv[0] + s[1] * p[-5] + s[2] * p[-4], DAT_004e2a98 <= (float)fabs(den))
                                    && (t = -(t / den), DAT_004e134c <= t && t <= DAT_004e1348)) {
                                    Vec3 hit(s[3], s[4], s[5]);
                                    hit = hit + t * *(Vec3*)s;
                                    if (PointInTri(&hit.x, tv, verts + tri[0] * 0x2c, verts + tri[1] * 0x2c, verts + tri[2] * 0x2c)) {
                                        if (DAT_00581e28 == 0) {
                                            if (t < best)
                                                best = t;
                                        } else if (DAT_00581e28 == 1) {
                                            if (t < best && TriTestA(model, tri, verts, &hit.x))
                                                best = t;
                                        } else if (DAT_00581e28 == 2) {
                                            DAT_00581e20 = TriTestB(model, tri, verts, &hit.x) + DAT_00581e20;
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
                    }
                }
                return best;
            }
        }
    }
    return DAT_004e1348;
}
