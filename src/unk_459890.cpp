// Car damage and contacts, first half (0x459890-0x45ad0f, stub 0x45ace0). Map: analysis/range-459890.md section 1.
// Work group W1: the contact append, ApplyWheel, the repair helpers and the damage getters. The Car members
// read their fields through CarDamageView (include/unk_459890.h); offsets from CarObject (unk_45e480.h).
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46a8a0.h"
#include <math.h>
#include <string.h>
#include "unk_47c460.h"
#include "unk_477370.h"
#include "unk_459890.h"
#include <stdlib.h>

// ---- ProcessContacts draft: file-local types ----
// Car damage and contacts (0x459890-0x45ad0f). Map: analysis/range-459890.md section 1.
// Work group W2: ProcessContacts (0x459910). The Car fields are read through the raw view below.
// Float to int with the x87 default rounding (inline assembler, as in unk_479170.h).
inline int FloatToInt(float f)
{
    int i;
    __asm {
        fld f
        fistp i
    }
    return i;
}
static const float kHitScale = 1.0f / 30.0f;
extern int DAT_0050145c;               // damage enabled (0 in modes without damage)
struct DentMesh {
    void Dent(const float* point, float x, float y, float z); // 0x4b9830: x/y/z = the impulse (passed as a Vec3 by value)
};
struct DentMeshList {
    DentMesh* At(int index);           // 0x4a6800
};
// A scene object with a mesh list at +0x124 (the body, the hood and trunk parts).
struct DentOwner {
    BYTE unk0[0x124];
    DentMeshList meshes;               // 0x124
};
// The Car body (Car+0x24).
struct HitBody {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    float matrix[16];                  // 0x20
    BYTE unk60[0xa4 - 0x60];
    float f0a4;                        // 0xa4
    BYTE unka8[0xb4 - 0xa8];
    float f0b4;                        // 0xb4
    float centre[3];                   // 0xb8
    BYTE unkc4[0x124 - 0xc4];
    DentMeshList meshes;               // 0x124
};
// The hood and trunk crash models (Car+0x1f68 / 0x1f6c).
struct HitPart {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    float matrix[16];                  // 0x20
    BYTE unk60[0x124 - 0x60];
    DentMeshList meshes;               // 0x124
};
struct HitPlayer {
    BYTE unk0[0x30];
    int noDamage;                      // 0x30
};
struct HitVec {
    float x, y, z;
};
struct HitRec {                        // 0x28 bytes
    float strength;                    // 0x00
    float pos[3];                      // 0x04: local point
    float dir[3];                      // 0x10
    float f1c;                         // 0x1c: factor 0..1
    float f20;                         // 0x20: factor * 0.2
    float f24;                         // 0x24: factor * 0.7 + 0.1
};
struct HitWheel {                      // 0x330 bytes, from Car+0x28
    BYTE unk0[0x14];
    HitVec pos;                        // 0x14
    BYTE unk20[0x2a0 - 0x20];
    float brakeDamage;                 // 0x2a0
    float steerDamage;                 // 0x2a4
    BYTE unk2a8[0x330 - 0x2a8];
};
struct HitSpring {                     // 0x34 bytes, from Car+0xce8
    HitVec pos;                      // 0x00
    BYTE unkc[0x34 - 0xc];
};
struct HitLamp {                       // 0x10 bytes, from Car+0x1bcf0
    int* target;                       // 0x00
    int off;                           // 0x04
    int on;                            // 0x08
    BYTE state;                        // 0x0c
    BYTE unkd[3];
};
struct HitObject {
    BYTE unk0[0x38];
    int state;                         // 0x38
};
struct ContactView {
    BYTE unk0[0x24];
    HitBody* body;                     // 0x24
    HitWheel wheel[4];                 // 0x28
    HitSpring spring[4];               // 0xce8
    BYTE unkdb8[0x1080 - 0xdb8];
    float hitMin;                      // 0x1080
    float hitMax;                      // 0x1084
    BYTE unk1088[0x133c - 0x1088];
    float brake;                       // 0x133c
    BYTE unk1340[0x1f68 - 0x1340];
    HitPart* hood;                     // 0x1f68
    HitPart* trunk;                    // 0x1f6c
    BYTE unk1f70[0x1bcf0 - 0x1f70];
    HitLamp lamp[4];                   // 0x1bcf0
    HitObject* lampObject;             // 0x1bd30
    BYTE unk1bd34[0x1becc - 0x1bd34];
    HitPlayer* player;                 // 0x1becc
    BYTE unk1bed0[0x1befc - 0x1bed0];
    int replayMode;                    // 0x1befc
    BYTE unk1bf00[0x1c730 - 0x1bf00];
    int hitA;                          // 0x1c730
    int hitB;                          // 0x1c734
    BYTE unk1c738[0x1c73c - 0x1c738];
    float wheelDamage[4];              // 0x1c73c
    float zone74c;                     // 0x1c74c
    float zone750;                     // 0x1c750
    float zone754;                     // 0x1c754
    float zone758;                     // 0x1c758
    BYTE unk1c75c[0x1c910 - 0x1c75c];
    float maxStrength;                 // 0x1c910
    HitRec rec[0x100];                 // 0x1c914
    int count;                         // 0x1f114
    BYTE unk1f118[0x1f2c0 - 0x1f118];
    int hardHits;                      // 0x1f2c0
    int breaks;                        // 0x1f2c4
    BYTE unk1f2c8[4];
    int hardCount;                     // 0x1f2cc
    int mediumCount;                   // 0x1f2d0
    float lastPos[3];                  // 0x1f2d4
    float hardPos[3];                  // 0x1f2e0
    int hardVolume;                    // 0x1f2ec
    float mediumPos[3];                // 0x1f2f0
    int mediumVolume;                  // 0x1f2fc
    void ApplyWheel(int n);            // 0x45a7a0
};
#define CV_OFFSET(m) ((int)&((ContactView*)0)->m)
typedef char ContactView_check1[CV_OFFSET(lamp) == 0x1bcf0 ? 1 : -1];
typedef char ContactView_check2[CV_OFFSET(rec) == 0x1c914 ? 1 : -1];
typedef char ContactView_check3[CV_OFFSET(count) == 0x1f114 ? 1 : -1];
typedef char ContactView_check4[CV_OFFSET(mediumVolume) == 0x1f2fc ? 1 : -1];
typedef char ContactView_check5[CV_OFFSET(zone74c) == 0x1c74c ? 1 : -1];
typedef char ContactView_check6[CV_OFFSET(spring) == 0xce8 ? 1 : -1];
typedef char ContactView_check7[CV_OFFSET(hood) == 0x1f68 ? 1 : -1];
typedef char ContactView_check8[CV_OFFSET(brake) == 0x133c ? 1 : -1];
// Transforms the vector `in` by the 3x3 rows at `mat` and adds `trans` (body frame to world), as the inline
// matrix multiply of the original does (same macro as unk_469630.cpp).
#define BODY_TRANSFORM() \
    __asm mov ecx, in \
    __asm mov edx, mat \
    __asm mov eax, out \
    __asm mov ebx, trans \
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
    __asm fadd dword ptr [ebx] \
    __asm fxch st(1) \
    __asm fadd dword ptr [ebx+4] \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx+8] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]
// The same multiply with the translation in the matrix itself (+0x30..0x38).
#define MATRIX_TRANSFORM() \
    __asm mov ecx, in \
    __asm mov edx, mat \
    __asm mov eax, out \
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
// A 4x4 matrix with an empty destructor (the part matrices below get EH states).
class PartMatrix {
public:
    PartMatrix() {}
    ~PartMatrix() {}
    float m[16];
};
// Inverts a rotation + translation matrix in place (inlined in the original).
static inline void InvertRigid(float* m, float tz)
{
    float x = -(m[0] * m[12] + m[2] * tz + m[1] * m[13]);
    float y = -(m[4] * m[12] + m[6] * tz + m[5] * m[13]);
    float z = -(m[8] * m[12] + m[10] * tz + m[9] * m[13]);
    m[12] = x;
    m[13] = y;
    m[14] = z;
    float t = m[1];
    m[1] = m[4];
    m[4] = t;
    t = m[2];
    m[2] = m[8];
    m[8] = t;
    t = m[6];
    m[6] = m[9];
    m[9] = t;
}
// original: FUN_00459910

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x459890
void Car::AddContact(Vec3* a, Vec3* b, float strength)
{
    __asm {
        mov eax, dword ptr [ecx + 0x1f114]
        cmp eax, 0x100
        jge contacts_full
        fld dword ptr [esp + 0xc]
        lea eax, [eax + eax * 4]
        fstp dword ptr [ecx + eax * 8 + 0x1c914]
        mov edx, dword ptr [ecx + 0x1f114]
        push esi
        mov esi, dword ptr [esp + 8]
        lea eax, [edx + edx * 4]
        lea eax, [ecx + eax * 8]
        inc edx
        push edi
        mov edi, dword ptr [esi]
        mov dword ptr [ecx + 0x1f114], edx
        mov ecx, dword ptr [esp + 0x10]
        mov edx, dword ptr [ecx]
        mov dword ptr [eax + 0x1c918], edi
        mov edi, dword ptr [esi + 4]
        mov esi, dword ptr [esi + 8]
        mov dword ptr [eax + 0x1c91c], edi
        mov dword ptr [eax + 0x1c924], edx
        mov edx, dword ptr [ecx + 4]
        mov ecx, dword ptr [ecx + 8]
        mov dword ptr [eax + 0x1c920], esi
        pop edi
        mov dword ptr [eax + 0x1c928], edx
        mov dword ptr [eax + 0x1c92c], ecx
        pop esi
    contacts_full:
        ret 0xc
    }
}

// NONMATCHING: RALLYTROPHY 0x459910
// 3695 vs 3722 bytes; logic complete (every branch, constant and call in the original is present). Open: register
// and stack-slot allocation (the original pushes ebx/edi after the early exits and counts the dent loops in edi),
// the instruction scheduling of the loop tests, the trunk/hood inverse (the original keeps the part's z in st0 and
// the EH state 1 store before At(0)), and the wheel loops (original biases the record pointer to rec+0xc).
// Applies the buffered contact records (AddContact, 0x459890) to the damage state: impact statistics, dents in the
// body, trunk and hood meshes, the four zone/wheel damages, broken lamps and the shatter flags.
void Car::ProcessContacts()
{
    ContactView* v = (ContactView*)this;
    int i1, i2, i3, i4, i5, i6, i7, i8, i9;
    int w;
    HitRec* recs = (HitRec*)((BYTE*)this + 0x1c914);
    if (v->count == 0) {
        return;
    }
    if (DAT_0050145c == 0 || (v->replayMode != 2 && v->player->noDamage != 0)) {
        v->count = 0;
        return;
    }
    float lo = v->hitMin;
    float inv = 1.0f / (v->hitMax - lo);
    HitBody* body = v->body;
    Vec3 centre;
    const float* centreP = body->centre;
    const float* in = centreP;
    const float* mat = body->matrix;
    float* out = (float*)&centre;
    const float* trans = body->pos;
    BODY_TRANSFORM();

    // statistics and strength scaling per record
    for (i1 = 0; i1 < v->count; i1++) {
        HitRec* r = &recs[i1];
        if (r->strength > 3.0f) {
            float s = (r->strength - 3.0f) * 0.05f;
            if (*(int*)&s < 0) {
                s = 0.0f;
            } else if (*(int*)&s > 0x3f800000) {
                s = 1.0f;
            }
            if (s > v->maxStrength) {
                v->maxStrength = s;
            }
        }
        if (r->strength > 15.0f) {
            v->hardHits++;
        }
        if (r->strength > 15.0f) {
            float f = kHitScale * r->strength * 127.0f;
            int vol = FloatToInt(f);
            if (vol > v->hardVolume) {
                if (vol > 0x7f) {
                    vol = 0x7f;
                } else if (vol < 0) {
                    vol = 0;
                }
                v->hardVolume = vol;
                v->hardCount++;
                v->hardPos[0] = centre.x;
                v->hardPos[1] = centre.y;
                v->hardPos[2] = centre.z;
            }
        } else if (r->strength > 6.0f) {
            float f = kHitScale * r->strength * 127.0f;
            int vol = FloatToInt(f);
            if (vol > v->mediumVolume) {
                if (vol > 0x7f) {
                    vol = 0x7f;
                } else if (vol < 0) {
                    vol = 0;
                }
                v->mediumVolume = vol;
                v->mediumCount++;
                v->mediumPos[0] = centre.x;
                v->mediumPos[1] = centre.y;
                v->mediumPos[2] = centre.z;
            }
        }
        float k = (r->strength - lo) * inv;
        if (*(int*)&k < 0) {
            k = 0.0f;
        } else if (*(int*)&k > 0x3f800000) {
            k = 1.0f;
        }
        r->f1c = k;
        r->f20 = k * 0.2f;
        r->f24 = k * 0.7f + 0.1f;
    }

    // dents in the body mesh
    HitBody* b2 = v->body;
    DentMesh* mesh = b2->meshes.At(0);
    for (i2 = 0; i2 < v->count; i2++) {
        HitRec* r = &v->rec[i2];
        float fx = r->f1c;
        float fy = r->f20;
        float fz = r->f24;
        mesh->Dent(r->pos, fx, fy, fz);
        v->zone74c += fx * 0.005f;
        if ((rand() & 0xff) < 0x40) {
            v->zone758 += fx * 0.01f;
        }
    }
    if (*(int*)&v->zone74c < 0) {
        v->zone74c = 0.0f;
    } else if (*(int*)&v->zone74c > 0x3f800000) {
        v->zone74c = 1.0f;
    }
    if (*(int*)&v->zone758 < 0) {
        v->zone758 = 0.0f;
    } else if (*(int*)&v->zone758 > 0x3f800000) {
        v->zone758 = 1.0f;
    }

    // dents in the trunk and hood parts, in their own frame
    HitPart* part = v->trunk;
    if (part) {
        DentMesh* pm = part->meshes.At(0);
        PartMatrix inv2 = *(PartMatrix*)part->matrix;
        inv2.m[12] = part->pos[0];
        inv2.m[13] = part->pos[1];
        InvertRigid(inv2.m, part->pos[2]);
        for (i3 = 0; i3 < v->count; i3++) {
            HitRec* r = &recs[i3];
            Vec3 local;
            const float* in = r->pos;
            const float* mat = inv2.m;
            float* out = (float*)&local;
            MATRIX_TRANSFORM();
            float fx = r->f1c;
            float fy = r->f20;
            float fz = r->f24;
            pm->Dent((float*)&local, fx, fy, fz);
        }
    }
    part = v->hood;
    if (part) {
        DentMesh* pm = part->meshes.At(0);
        PartMatrix inv2 = *(PartMatrix*)part->matrix;
        inv2.m[12] = part->pos[0];
        inv2.m[13] = part->pos[1];
        InvertRigid(inv2.m, part->pos[2]);
        for (i4 = 0; i4 < v->count; i4++) {
            HitRec* r = &recs[i4];
            Vec3 local;
            const float* in = r->pos;
            const float* mat = inv2.m;
            float* out = (float*)&local;
            MATRIX_TRANSFORM();
            float fx = r->f1c;
            float fy = r->f20;
            float fz = r->f24;
            pm->Dent((float*)&local, fx, fy, fz);
        }
    }

    // wheel damage and brake/steering damage
    float extra = 0.0f;
    if (v->brake > 0.5f) {
        extra = v->brake * 0.0002f;
    }
    for (w = 0; w < 4; w++) {
        if (v->wheelDamage[w] < 1.0f) {
            HitVec wp = v->spring[w].pos;
            for (i5 = 0; i5 < v->count; i5++) {
                HitRec* r = &v->rec[i5];
                float dx = wp.x - r->pos[0];
                float dy = wp.y - r->pos[1];
                float dz = wp.z - r->pos[2];
                float d2 = dy * dy + dz * dz + dx * dx;
                float rad = r->f24;
                float r2 = rad * rad;
                if (d2 < r2) {
                    float g = r->f20;
                    float g2 = g * g;
                    float k = r->f1c;
                    if (d2 > g2) {
                        float t = (r2 - d2) * (1.0f / (r2 - g2));
                        k = (3.0f - (t + t)) * r->f1c * t * t;
                    }
                    v->wheelDamage[w] += k * 0.02f;
                }
            }
            if (v->wheelDamage[w] > 1.0f) {
                v->wheelDamage[w] = 1.0f;
            }
        }
        if (v->wheel[w].brakeDamage < 1.0f) {
            float a = v->wheel[w].brakeDamage + extra;
            float b = v->wheel[w].steerDamage;
            HitVec wp = v->wheel[w].pos;
            for (i6 = 0; i6 < v->count; i6++) {
                HitRec* r = &v->rec[i6];
                float dx = wp.x - r->pos[0];
                float dy = wp.y - r->pos[1];
                float dz = wp.z - r->pos[2];
                float d2 = dy * dy + dz * dz + dx * dx;
                float rad = r->f24 * 1.5f;
                float r2 = rad * rad;
                if (d2 < r2) {
                    float g = r->f20;
                    float g2 = g * g;
                    float k = r->f1c;
                    if (d2 > g2) {
                        float t = (r2 - d2) * (1.0f / (r2 - g2));
                        k = (3.0f - (t + t)) * r->f1c * t * t;
                    }
                    if ((rand() & 0x7f) < 0x28) {
                        b += k * 0.03f;
                    }
                    if ((rand() & 0x7f) < 0x32) {
                        a += k * 0.02f;
                    }
                }
            }
            if (*(int*)&b < 0) {
                b = 0.0f;
            } else if (*(int*)&b > 0x3f800000) {
                b = 1.0f;
            }
            if (*(int*)&a < 0) {
                a = 0.0f;
            } else if (*(int*)&a > 0x3f800000) {
                a = 1.0f;
            }
            v->wheel[w].brakeDamage = a;
            v->wheel[w].steerDamage = b;
        }
    }

    // front zone
    if (v->zone750 < 1.0f) {
        float px = *centreP;
        float pz = body->f0b4 - 0.25f;
        float acc = v->zone750;
        for (i7 = 0; i7 < v->count; i7++) {
            HitRec* r = &v->rec[i7];
            float dx = px - r->pos[0];
            float dy = -r->pos[1];
            float dz = pz - r->pos[2];
            float d2 = dy * dy + dz * dz + dx * dx;
            float rad = r->f24;
            float r2 = rad * rad;
            if (d2 < r2) {
                float g = r->f20;
                float g2 = g * g;
                float k = r->f1c;
                if (d2 > g2) {
                    float t = (r2 - d2) * (1.0f / (r2 - g2));
                    k = (3.0f - (t + t)) * t * t * r->f1c;
                }
                if ((rand() & 0x7f) < 0x23) {
                    acc += k * 0.03f;
                }
            }
        }
        v->zone750 = acc;
        if (*(int*)&v->zone750 < 0) {
            v->zone750 = 0.0f;
        } else if (*(int*)&v->zone750 > 0x3f800000) {
            v->zone750 = 1.0f;
        }
    }
    // rear zone
    if (v->zone754 < 1.0f) {
        float acc = v->zone754;
        for (i8 = 0; i8 < v->count; i8++) {
            HitRec* r = &v->rec[i8];
            float dx = r->pos[0];
            float dy = r->pos[1] - body->f0a4;
            float d2 = dx * dx + dy * dy;
            float rad = r->f24;
            float r2 = rad * rad;
            if (d2 < r2) {
                float g = r->f20;
                float g2 = g * g;
                float k = r->f1c;
                if (d2 > g2) {
                    float t = (r2 - d2) * (1.0f / (r2 - g2));
                    k = (3.0f - (t + t)) * t * t * r->f1c;
                }
                if ((rand() & 0x7f) < 0x40) {
                    acc += k * 0.03f;
                }
            }
        }
        v->zone754 = acc;
        if (*(int*)&v->zone754 < 0) {
            v->zone754 = 0.0f;
        } else if (*(int*)&v->zone754 > 0x3f800000) {
            v->zone754 = 1.0f;
        }
    }
    v->ApplyWheel(0);
    v->ApplyWheel(1);
    v->ApplyWheel(2);
    v->ApplyWheel(3);

    // lamps and windows
    bool lamp0 = false;
    bool lamp1 = false;
    bool lamp2 = false;
    bool lamp3 = false;
    for (i9 = 0; i9 < v->count; i9++) {
        HitRec* r = &v->rec[i9];
        float s = r->strength;
        if (s > 10.0f) {
            float px = s * r->dir[0];
            float py = s * r->dir[1];
            float pz = s * r->dir[2];
            if (pz < -10.0f) {
                lamp0 = true;
            } else if (pz > 10.0f) {
                lamp1 = true;
            }
            if (px > 10.0f) {
                lamp2 = true;
            } else if (px < -10.0f) {
                lamp3 = true;
            }
            if (s > 35.0f || fabs(py) > 20.0) {
                lamp3 = true;
                lamp2 = true;
                lamp1 = true;
                lamp0 = true;
            }
        }
        if (s > 25.0f) {
            if (rand() & 1) {
                v->hitB = 1;
            } else {
                v->hitA = 1;
            }
        } else if (s > 15.0f) {
            if (s * r->dir[2] < -15.0f) {
                v->hitB = 1;
            }
            if (s * r->dir[2] > 15.0f) {
                v->hitA = 1;
            }
        }
    }
    if (lamp0) {
        v->lamp[0].state = 1;
    }
    if (lamp1) {
        v->lamp[1].state = 1;
    }
    if (lamp2) {
        v->lamp[2].state = 1;
    }
    if (lamp3) {
        v->lamp[3].state = 1;
    }
    v->lastPos[0] = centre.x;
    v->lastPos[1] = centre.y;
    v->lastPos[2] = centre.z;
    if (lamp0) {
        int* t = v->lamp[0].target;
        if (t) {
            int value = v->lamp[0].on;
            if (*t != value) {
                v->breaks++;
            }
            *t = value;
            if (v->lampObject) {
                v->lampObject->state = 0x11;
            }
        }
    }
    if (lamp1) {
        int* t = v->lamp[1].target;
        if (t) {
            int value = v->lamp[1].on;
            if (*t != value) {
                v->breaks++;
            }
            *t = value;
        }
    }
    if (lamp2) {
        int* t = v->lamp[2].target;
        if (t) {
            int value = v->lamp[2].on;
            if (*t != value) {
                v->breaks++;
            }
            *t = value;
        }
    }
    if (lamp3) {
        int* t = v->lamp[3].target;
        if (t) {
            int value = v->lamp[3].on;
            *t = value;
            if (value) {
                v->breaks++;
            }
            *t = value;
        }
    }
    v->count = 0;
}

// FUNCTION: RALLYTROPHY 0x45a7a0
void Car::ApplyWheel(int n)
{
    CarDamageView* v = (CarDamageView*)this;
    float base, b, c;
    if (n < 2) {
        base = v->tabA[v->setup];
        b = v->tabB[v->setup];
        c = v->tabC[v->setup];
    } else {
        base = v->tabD[v->setup];
        b = v->tabE[v->setup];
        c = v->tabF[v->setup];
    }
    float d = v->wheelDamage[n];
    v->spring[n].a = (1.0f - d * 0.45f) * base;
    float k = 1.0f - d * 0.96f;
    v->spring[n].b = k * b;
    v->spring[n].c = k * c;
}

// FUNCTION: RALLYTROPHY 0x45a850
void Car::FUN_0045a850()
{
    FUN_0045a890();
    ResetZone1();
    ResetZone2();
    ResetZone3();
    ResetBrakeDamage();
    ResetWheelDamage();
    ResetSteeringDamage();
    ((CarDamageView*)this)->latch[7] = 0;
}

// FUNCTION: RALLYTROPHY 0x45a890
void Car::FUN_0045a890()
{
    CarDamageView* v = (CarDamageView*)this;
    v->zone0 = 0;
    v->body->meshes.At(0)->Repair();
    if (v->trunkCrash) {
        v->trunkCrash->meshes.At(0)->Repair();
    }
    if (v->hoodCrash) {
        v->hoodCrash->meshes.At(0)->Repair();
    }
    if (v->lamp[0].target) {
        *v->lamp[0].target = v->lamp[0].off;
    }
    if (v->marker) {
        v->marker->state = 0;
    }
    if (v->lamp[1].target) {
        *v->lamp[1].target = v->lamp[1].off;
    }
    if (v->lamp[2].target) {
        *v->lamp[2].target = v->lamp[2].off;
    }
    if (v->lamp[3].target) {
        *v->lamp[3].target = v->lamp[3].off;
    }
    v->lamp[0].state = 0;
    v->lamp[1].state = 0;
    v->lamp[2].state = 0;
    v->lamp[3].state = 0;
    v->unk1c730 = 0;
    v->unk1c734 = 0;
    v->latch[0] = 0;
    RebuildLights();
}

// FUNCTION: RALLYTROPHY 0x45a980
// Volatile byte stores preserve the target's ordering before each target-pointer load.
void Car::RebuildLights()
{
    CarDamageView* v = (CarDamageView*)this;
    int i;
    DmgMesh* mesh = v->body->meshes.At(0);
    v->noFrontLight = 0;
    DmgSprite** g = &v->glow[5];
    DmgLight* l = v->lightFrontLeft;
    for (i = 0; i < 3; i++) {
        if (l->sprite) {
            l->sprite->SetParent(v->bodyV);
            l->mesh = mesh->Find(&l->sprite->name);
            *(volatile BYTE*)&l->dead = 0;
            int* t = l->target;
            if (t) {
                *t = l->value;
            }
        }
        if (l[3].sprite) {
            l[3].sprite->SetParent(v->bodyV);
            l[3].mesh = mesh->Find(&l[3].sprite->name);
            *(volatile BYTE*)&l[3].dead = 0;
            int* t3 = l[3].target;
            if (t3) {
                *t3 = l[3].value;
            }
        }
        if (g[-3]) {
            g[-3]->SetParent(v->bodyV);
        }
        if (g[0]) {
            g[0]->SetParent(v->bodyV);
        }
        g++;
        l++;
    }
    l = v->lightBrake;
    for (i = 0; i < 2; i++) {
        if (l->sprite) {
            l->sprite->SetParent(v->bodyV);
            l->mesh = mesh->Find(&l->sprite->name);
            *(volatile BYTE*)&l->dead = 0;
            int* t = l->target;
            if (t) {
                *t = l->value;
            }
        }
        if (l[-2].sprite) {
            l[-2].sprite->SetParent(v->bodyV);
            l[-2].mesh = mesh->Find(&l[-2].sprite->name);
            l[-2].dead = 0;
        }
        if (l[2].sprite) {
            l[2].sprite->SetParent(v->bodyV);
            l[2].mesh = mesh->Find(&l[2].sprite->name);
            l[2].dead = 0;
        }
        l++;
    }
    if (v->glow[0]) {
        v->glow[0]->SetParent(v->bodyV);
    }
    if (v->glow[1]) {
        v->glow[1]->SetParent(v->bodyV);
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x45ab30
void Car::ResetZone1()
{
    CarDamageView* v = (CarDamageView*)this;
    v->zone1 = 0;
    v->latch[1] = 0;
}

// FUNCTION: RALLYTROPHY 0x45ab40
void Car::ResetZone2()
{
    CarDamageView* v = (CarDamageView*)this;
    v->zone2 = 0;
    v->latch[2] = 0;
}

// FUNCTION: RALLYTROPHY 0x45ab50
void Car::ResetZone3()
{
    CarDamageView* v = (CarDamageView*)this;
    v->zone3 = 0;
    v->latch[3] = 0;
}

// FUNCTION: RALLYTROPHY 0x45ab60
void Car::ResetBrakeDamage()
{
    CarDamageView* v = (CarDamageView*)this;
    for (int i = 0; i < 4; i++) {
        v->wheel[i].brake = 0;
    }
    v->latch[4] = 0;
}

// FUNCTION: RALLYTROPHY 0x45ab90
void Car::ResetWheelDamage()
{
    CarDamageView* v = (CarDamageView*)this;
    for (int i = 0; i < 4; i++) {
        v->wheelDamage[i] = 0;
        ApplyWheel(i);
    }
    v->latch[5] = 0;
}

// FUNCTION: RALLYTROPHY 0x45abc0
void Car::ResetSteeringDamage()
{
    CarDamageView* v = (CarDamageView*)this;
    for (int i = 0; i < 4; i++) {
        v->wheel[i].steer = 0;
    }
    v->latch[6] = 0;
}

// FUNCTION: RALLYTROPHY 0x45abf0
float Car::GetBrakeDamage()
{
    CarDamageView* v = (CarDamageView*)this;
    float sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += v->wheel[i].brake;
    }
    sum *= 0.5f;
    int bits = *(int*)&sum;
    if (bits < 0) {
        return 0.0f;
    }
    if (bits > 0x3f800000) {
        return 1.0f;
    }
    return sum;
}

// FUNCTION: RALLYTROPHY 0x45ac40
float Car::GetWheelDamage()
{
    CarDamageView* v = (CarDamageView*)this;
    float sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += v->wheelDamage[i];
    }
    sum *= 0.5f;
    int bits = *(int*)&sum;
    if (bits < 0) {
        return 0.0f;
    }
    if (bits > 0x3f800000) {
        return 1.0f;
    }
    return sum;
}

// FUNCTION: RALLYTROPHY 0x45ac90
float Car::GetSteeringDamage()
{
    CarDamageView* v = (CarDamageView*)this;
    float sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += v->wheel[i].steer;
    }
    sum *= 0.5f;
    int bits = *(int*)&sum;
    if (bits < 0) {
        return 0.0f;
    }
    if (bits > 0x3f800000) {
        return 1.0f;
    }
    return sum;
}

// FUNCTION: RALLYTROPHY 0x45ace0
// SYMBOL: _$E23
typedef void Generated_45ace0();

// FUNCTION: RALLYTROPHY 0x45ad00
// SYMBOL: _$E22
typedef void Generated_45ad00();
