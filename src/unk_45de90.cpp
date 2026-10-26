// Car::UpdateLights (0x45de90-0x45e44d, stub 0x45e450). Map: analysis/range-459890.md section 4.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_45de90.h"

// Transforms *in by the 3x3 rows at *mat and adds *trans (body frame to world) into *out.
#define LIGHT_TRANSFORM() \
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

// FUNCTION: RALLYTROPHY 0x45de90
void Car::UpdateLights()
{
    LightsCar* v = (LightsCar*)this;
    int i;
    int intact;
    Vec3 front;
    Vec3 rear;
    LightsBody* body = v->body;
    front.x = front.y = front.z = 0.0f;
    rear.x = rear.y = rear.z = 0.0f;
    front.z = body->unkb4;
    rear.z = body->unka8;
    float level;
    intact = 0;
    i = 0;
    level = (body->unkb0 + body->unka4) * 0.5f;
    rear.y = level;
    front.y = level;
    for (; i < 3; i++) {
        LightsRec* rec = &v->rec[i];
        if (rec->light && !rec->dead) {
            if (!rec->target || !(*rec->target > 0.3f)) {
                intact++;
            } else {
                v->broken++;
                const float* trans;
                float* out;
                const float* mat;
                const float* in;
                trans = body->pos;
                mat = body->m;
                in = (const float*)&front;
                out = (float*)&v->position;
                LIGHT_TRANSFORM();
                rec->dead = 1;
                rec->light->Show();
                if (v->glowL[i]) {
                    v->glowL[i]->Show();
                }
                if (i == 0 && rec->out) {
                    *rec->out = rec->value;
                }
            }
        }
        rec = &v->rec[3 + i];
        if (rec->light && !rec->dead) {
            if (!rec->target || !(*rec->target > 0.3f)) {
                intact++;
            } else {
                v->broken++;
                const float* in;
                const float* mat;
                float* out;
                const float* trans;
                trans = body->pos;
                mat = body->m;
                in = (const float*)&front;
                out = (float*)&v->position;
                LIGHT_TRANSFORM();
                rec->dead = 1;
                rec->light->Show();
                if (v->glowR[i]) {
                    v->glowR[i]->Show();
                }
                if (i == 0 && rec->out) {
                    *rec->out = rec->value;
                }
            }
        }
    }
    if (intact == 0) {
        v->noFrontLight = 1;
    } else {
        v->noFrontLight = 0;
    }
    for (i = 0; i < 2; i++) {
        LightsRec* rec = &v->rec[8 + i];
        if (rec->light && !rec->dead && rec->target && *rec->target > 0.3f) {
            rec->dead = 1;
            rec->light->Show();
            v->broken++;
            const float* in;
            const float* mat;
            float* out;
            const float* trans;
            trans = body->pos;
            mat = body->m;
            in = (const float*)&rear;
            out = (float*)&v->position;
            LIGHT_TRANSFORM();
            LightSprite* glow = i == 0 ? v->glow0 : v->glow1;
            if (glow) {
                glow->Show();
            }
            if (v->rec[6 + i].light) {
                v->rec[6 + i].light->Show();
                v->rec[6 + i].dead = 1;
            }
            if (rec->out) {
                *rec->out = rec->value;
            }
        }
        rec = &v->rec[10 + i];
        if (rec->light && !rec->dead && rec->target && *rec->target > 0.3f) {
            v->broken++;
            const float* in;
            const float* mat;
            float* out;
            const float* trans;
            trans = body->pos;
            mat = body->m;
            in = (const float*)&rear;
            out = (float*)&v->position;
            LIGHT_TRANSFORM();
            rec->dead = 1;
            rec->light->Show();
        }
    }
    bool braking = v->unk133c > 0.0f;
    bool reversing = v->unkf8c == -1;
    if (v->unk1be90) {
        if (braking) {
            *v->unk1be90 = v->unk1be98;
        } else {
            *v->unk1be90 = v->unk1be94;
        }
    }
    for (i = 0; i < 2; i++) {
        LightSprite* lamp = v->rec[8 + i].light;
        if (lamp) {
            if (braking) {
                lamp->flags &= 0xfe;
            } else {
                lamp->flags |= 1;
            }
        }
        lamp = v->rec[10 + i].light;
        if (lamp) {
            if (reversing) {
                lamp->flags &= 0xfe;
            } else {
                lamp->flags |= 1;
            }
        }
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x45e450
// SYMBOL: _$E23
typedef void Generated_45e450();

// FUNCTION: RALLYTROPHY 0x45e470
// SYMBOL: _$E22
typedef void Generated_45e470();
