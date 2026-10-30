// Car::SunFactor (0x45dac0-0x45de8f, stub 0x45de60). Map: analysis/range-459890.md section 3.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_45dac0.h"

// The Car as SunFactor sees it: the body at +0x24 (second matrix at +0x60, reference point at +0xb8).
struct SunCar {
    BYTE unk0[0x24];
    BYTE* body;                        // 0x24
};

bool PointInTriangle(const Vec3* point, const SunTri* tri, const SunVertex* a, const SunVertex* b,
                     const SunVertex* c); // 0x4b3ee0
void BarycentricWeights(const SunVertex* a, const SunVertex* b, const SunVertex* c, const Vec3* point,
                        Vec3* weights); // 0x4b3470

// Transforms *point by *matrix (row vectors, translation at +0x30) into *result.
#define SUN_TRANSFORM() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
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
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// NONMATCHING: RALLYTROPHY 0x45dac0
// One advancing triangle-record cursor for plane fields and nx, alongside the separately advancing tri pointer,
// cuts the differing-byte count from 468 to 403. A volatile WORD read assigned to int preserves the target's later
// count-load order and 32-bit decrement (422→403). The earlier 422-difference cursor-only variant was invalid
// because it passed a stale triangle pointer; this version keeps tri++.
// The sequential h assignments follow the target's x87 term order. Cursor/index registers, local homes, and
// vertex-index call setup still differ from the target.
float Car::SunFactor(void* models)
{
    SunModels* list = (SunModels*)models;
    Vec3 p;
    {
        BYTE* body = ((SunCar*)this)->body;
        const float* point;
        const float* matrix;
        float* result;
        matrix = (const float*)(body + 0x60);
        point = (const float*)(body + 0xb8);
        result = (float*)&p;
        SUN_TRANSFORM();
    }
    const SunTri* hitTri = 0;
    const SunVertex* hitA = 0;
    Vec3 hit;
    float best = 999999.0f;
    int hits = 0;
    const SunVertex* hitB;
    const SunVertex* hitC;
    for (int m = 0; m < list->count; m++) {
        SunModel* model = list->models[m];
        if (model->kind != 1 || !(model->flags & 4) || !(model->flagsCc & 1)) {
            continue;
        }
        std::vector<SunTri*>* tris = model->meshes.tris;
        SunMesh* mesh = model->meshes.At(0);
        if (!tris) {
            continue;
        }
        float dx = p.x - model->centreX;
        float dz = p.z - model->centreZ;
        float dist = dx * dx + dz * dz;
        float r = model->radius;
        float r2 = r * r;
        if (*(int*)&dist > *(int*)&r2) {
            continue;
        }
        for (unsigned i = 0; i < tris->size(); i++) {
            SunPart* part = mesh->parts[i];
            const unsigned short* idx;
            idx = part->indices;
            SunTri* tri;
            tri = (*tris)[i];
            int n = *(volatile unsigned short*)&part->count;
            SunVertex* verts = part->vertices;
            if (!n) {
                continue;
            }
            float eps = 0.00001f;
            const float* q = &tri->cz;
            do {
                if (*(int*)&q[-5] >= *(int*)&eps) {
                    float r = q[1];
                    if (!(q[-1] - p.y > r)) {
                        float tx = p.x - q[-2];
                        float tz = p.z - q[0];
                        if (!(tx * tx + tz * tz > r * r)) {
                            float h = p.z * q[-4] + p.y * q[-5];
                            h = h + p.x * q[-6];
                            h = h - q[-3];
                            if (!(*(unsigned*)&h & 0x80000000)) {
                                float t = h / q[-5];
                                if (!(t > best)) {
                                    Vec3 cand;
                                    cand.x = p.x;
                                    cand.z = p.z;
                                    cand.y = p.y - t;
                                    const SunVertex* c = &verts[idx[2]];
                                    const SunVertex* b = &verts[idx[1]];
                                    const SunVertex* a = &verts[idx[0]];
                                    if (PointInTriangle(&cand, tri, a, b, c)) {
                                        hit = cand;
                                        best = t;
                                        hitTri = tri;
                                        hitA = a;
                                        hitB = b;
                                        hitC = c;
                                        hits++;
                                    }
                                }
                            }
                        }
                    }
                }
                q += 8;
                tri++;
                idx += 3;
            } while (--n);
        }
    }
    if (hitTri) {
        Vec3 w;
        BarycentricWeights(hitA, hitB, hitC, &hit, &w);
        return (hitA->shade * w.x + hitB->shade * w.y + hitC->shade * w.z) * (1.0f / 255.0f);
    }
    return 1.0f;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x45de60
// SYMBOL: _$E23
typedef void Generated_45de60();

// FUNCTION: RALLYTROPHY 0x45de80
// SYMBOL: _$E22
typedef void Generated_45de80();
