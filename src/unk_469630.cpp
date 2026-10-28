// Car render transforms, lighting, shadow hook, surface mix and gauges (0x469630-0x46a89f, stub 0x46a870).
// Map: analysis/range-4666a0.md section 5. The Car members read their fields through the raw views of
// include/unk_469630.h.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46a8a0.h"
#include <math.h>
#include <string.h>
#include "unk_47c460.h"
#include "unk_477370.h"
#include "unk_478020.h"
#include "unk_479170.h"
#include "unk_47b4c0.h"
#include "unk_4741a0.h"
#include <stdlib.h>
#include "unk_469630.h"

// From the particle/tyre-track draft:
// Car render, effects and gauges (0x469630-0x46a89f). Map: analysis/range-4666a0.md section 5.
// Work group S7: particle emission (0x469a20) and tyre tracks (0x46a000).
// Transforms the vector `in` by the 3x3 rows at `mat` and adds `trans` (body frame to world), as the inline
// matrix multiply of the original does (same macro as unk_47c460.cpp).
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
static const float kInv32767 = 1.0f / 32767.0f;

// FUNCTION: RALLYTROPHY 0x469630
void Car::FUN_00469630(int a, int b, int c, Car* viewer)
{
    CarRenderView* v = (CarRenderView*)this;
    RenderBody* body = v->body;
    int visible = body->IsCurrent();
    if (!visible && viewer && viewer != this) {
        if (((CarRenderView*)viewer)->CanSee(body)) {
            visible = 1;
        }
    }
    v->timerA -= c;
    v->timerB -= c;
    if (v->timerA <= 0) {
        v->timerA = 0x28;
    }
    if (v->timerB <= 0) {
        v->timerB = 0x28;
    }
    if (v->shadow) {
        float* wp;
        RenderCaster** lp;
        int i;
        for (i = 0, lp = v->light, wp = &v->wheel[0].pos.z; i < 4; i++, lp++, wp += 0xcc) {
            RenderCaster* light = *lp;
            if (light) {
                memcpy(light->matrixB, wp + 17, 64);
                light->pos.x = wp[-2];
                light->pos.y = wp[-1];
                light->pos.z = wp[0];
                v->shadow->SetCaster(light, i);
            }
        }
        v->shadow->SetLightMatrix(((RenderView*)b)->camera->lead->world);
        RenderCaster* target = (RenderCaster*)v->body;
        if (v->caster) {
            target = v->caster;
            RenderCaster* src = (RenderCaster*)v->body;
            target->pos.x = src->pos.x;
            target->pos.y = src->pos.y;
            target->pos.z = src->pos.z;
            memcpy(target->matrixA, src->matrixA, 64);
            memcpy(target->matrixB, src->matrixB, 64);
        }
        v->shadow->Prepare(DAT_0056b0ac, a, (void*)b, target, 1);
    }
    if (v->unk1bce4 && visible) {
        v->Draw459270(a, (void*)b);
    }
}

// FUNCTION: RALLYTROPHY 0x4697c0
void Car::FUN_004697c0(int a, int b)
{
    CarRenderView* v = (CarRenderView*)this;
    RenderBody* body = v->body;
    if (body->IsCurrent()) {
        body->meshes.At(0)->Refresh();
        if (v->partA) {
            v->partA->meshes.At(0)->Refresh();
        }
        if (v->partB) {
            v->partB->meshes.At(0)->Refresh();
        }
        Vec2 range1, range2;
        if (v->stage) {
            range1.x = v->stage->light[0];
            range1.y = v->stage->light[1];
            range2.x = v->stage->light[2];
            range2.y = v->stage->light[3];
        } else {
            range1.x = 0.0f;
            range1.y = 1.0f;
            range2.x = 0.0f;
            range2.y = 1.0f;
        }
        float t = (v->SunFactor(b) - range1.x) / (range1.y - range1.x);
        if (*(int*)&t < 0) {
            t = 0.0f;
        } else if (*(int*)&t > 0x3f800000) {
            t = 1.0f;
        }
        float k = (range2.y - range2.x) * t + range2.x;
        if (*(int*)&k < 0) {
            k = 0.0f;
        } else if (*(int*)&k > 0x3f800000) {
            k = 1.0f;
        }
        for (MaterialNode* n = v->materials; n; n = n->next) {
            MaterialData* d = n->data;
            d->ambient[0] = k;
            d->ambient[1] = k;
            d->ambient[2] = k;
            d->diffuse[0] = 1.0f;
            d->diffuse[1] = 1.0f;
            d->diffuse[2] = 1.0f;
        }
        FUN_004699d0(a, b);
        ColourPart* p = v->colourA;
        if (p && p->scene->IsCurrent()) {
            p->SetColour(v->colourScaleA);
        }
        p = v->colourB;
        if (p || v->colourC) {
            if (p->scene->IsCurrent()) {
                p->SetColour(v->colourScaleB);
            } else {
                p = v->colourC;
                if (p->scene->IsCurrent()) {
                    p->SetColour(v->colourScaleB);
                }
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4699d0
void Car::FUN_004699d0(int a, int b)
{
    CarRenderView* v = (CarRenderView*)this;
    if (v->shadowMode == 1) {
        RenderBody* body = v->body;
        v->shadowTarget.Update(body, a, body->Centre(), body->Radius());
    }
}

// FUNCTION: RALLYTROPHY 0x469a00
void Car::FUN_00469a00(int a, int b)
{
    CarRenderView* v = (CarRenderView*)this;
    if (v->shadow) {
        v->shadow->Render(DAT_0056b0ac, a);
    }
}

// NONMATCHING: RALLYTROPHY 0x469a20
// 998 vs 1009 bytes. Logic and call order match. The original keeps the 3rd argument in edi for the whole body and
// spills the exhaust level into its stack slot (frame 0x40); the compiler here promotes nothing and uses edi for -1.
// Per-frame particle emission of a car: exhaust smoke, spray/debris from the body, and the dust, smoke and
// spray of each wheel that touches a surface with effects.
void Car::EmitEffects(float a, float b, float c)
{
    EmitCar* v = (EmitCar*)this;
    if (v->detail) {
        if (v->exhaustOn) {
            float level = v->drive.GetRpmFraction() * v->throttle;
            if (!((float)rand() * kInv32767 <= 0.5f - level * 0.25f)) {
                Vec3 pos;
                Vec3 vel2;
                Vec3 vel;
                const float* in = (const float*)&v->exhaustPos;
                const float* mat = v->body->m;
                const float* trans = (const float*)&v->body->pos;
                float* out = (float*)&pos;
                BODY_TRANSFORM();
                EmitBody* body = v->body;
                vel.x = body->vel.x;
                vel.y = body->vel.y;
                vel.z = body->vel.z;
                vel += body->PointVelocity(pos);
                vel2 = vel * 0.9f;
                vel2 -= body->Forward40() * ((float)rand() * kInv32767 * 0.5f + level * 1.8f + 3.0f);
                vel2.y += ((float)rand() * kInv32767 + 1.0f) * 0.5f;
                vel2.x += (1.0f - (float)rand() * kInv32767 * 2.0f) * 0.9f;
                vel2.y += (1.0f - (float)rand() * kInv32767 * 2.0f) * 0.5f;
                vel2.z += (1.0f - (float)rand() * kInv32767 * 2.0f) * 0.9f;
                v->Exhaust().Emit(&pos, &vel2, level, b, c);
            }
        }
        if (v->detail > 1) {
            if (v->body->sprayLevel > 0.0f) {
                Vec3 dir;
                Vec3 origin;
                dir.x = v->body->sprayDir.x;
                dir.y = v->body->sprayDir.y;
                dir.z = v->body->sprayDir.z;
                origin.x = v->body->sprayOrigin.x;
                origin.y = v->body->sprayOrigin.y;
                origin.z = v->body->sprayOrigin.z;
                v->debris.Emit(&origin, &dir, b, c);
            } else {
                v->debris.SpawnOne(a, b, c);
                v->debris.SpawnOne(0.0f, b, c);
            }
        }
        for (int i = 0; i < 4; i++) {
            EmitWheel* w = &v->wheel[i];
            if (w->contact) {
                int surf = v->surface[i];
                int type = v->surfaces->dust[surf];
                int effect = v->surfaces->smoke[surf];
                if (type) {
                    v->dust.Spawn(type - 1, b, c, (Car*)w, (CarBody*)v->body);
                }
                if (effect) {
                    effect--;
                    v->smokeB.Spawn(effect, b, c, (Car*)w, (CarBody*)v->body);
                    if (v->detail == 2) {
                        v->spray.Spawn(effect, b, c, (Car*)w, (CarBody*)v->body);
                    }
                }
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x469e20
void Car::FUN_00469e20()
{
    CarMixView* v = (CarMixView*)this;
    for (int j = 0; j < 4; j++) {
        v->mix[j] = 0;
    }
    RawVec4 zero(0, 0, 0, 0);
    for (unsigned int i = 0; i < v->parts.size(); i++) {
        v->parts[i]->v = zero;
    }
}

// FUNCTION: RALLYTROPHY 0x469ea0
void Car::FUN_00469ea0(float k)
{
    CarMixView* v = (CarMixView*)this;
    for (int i = 0; i < 4; i++) {
        if (v->wheel[i].active) {
            RenderStage* stage = v->stage;
            int surface = v->surface[i];
            if (stage->dust[surface] > 0) {
                int idx = stage->dust[surface] - 1;
                RawVec3* vel = &v->body->vel;
                float m = (float)sqrt((float)fabs(v->wheel[i].slipA) + (float)fabs(v->wheel[i].slipB)) * (1.0f / 6.0f);
                float speed = (float)sqrt(vel->x * vel->x + vel->y * vel->y + vel->z * vel->z);
                float w = speed * (1.0f / 60.0f) + m;
                if (*(int*)&w < 0) {
                    w = 0.0f;
                } else if (*(int*)&w > 0x3f800000) {
                    w = 1.0f;
                }
                w = w * stage->scale[idx] * k;
                v->mix[idx] += w;
                for (int j = 0; j < 4; j++) {
                    if (j != idx) {
                        v->mix[j] -= w;
                    }
                }
            }
        }
    }
    float sum = 0.0f;
    for (int n = 0; n < 4; n++) {
        if (v->mix[n] <= 0.0f) {
            v->mix[n] = 0.0f;
        } else {
            sum += v->mix[n];
        }
    }
    if (sum > 1.0f) {
        float inv = 1.0f / sum;
        for (int n = 0; n < 4; n++) {
            v->mix[n] *= inv;
        }
    }
}

// NONMATCHING: RALLYTROPHY 0x46a000
// 511 vs 511 bytes. Identical except register choice: the original keeps the wheel pointer in ecx/memory and uses ebx for
// the track-id test, the compiler here promotes the wheel pointer to ebx. A per-iteration EmitWheel* alias raises
// the diff 55→62; an offset alias based at wheel.position expands it to 348. Both probes were reverted.
// Stamps a tyre mark for each wheel on a marked surface whose slip exceeds 2.5, or ends its mark.
void Car::UpdateTyreTracks()
{
    EmitCar* v = (EmitCar*)this;
    for (int i = 0; i < 4; i++) {
        int surf = v->surface[i];
        int mark;
        if (v->wheel[i].contact && (mark = v->surfaces->trackMark[surf], mark != 0)) {
            Vec3 pos;
            const float* trans = (const float*)&v->body->pos;
            const float* mat = v->body->m;
            const float* in = (const float*)&v->wheel[i].position;
            float* out = (float*)&pos;
            BODY_TRANSFORM();
            pos -= v->wheelAxis[i] * (v->groundOffset - 0.015f);
            float slip = v->wheel[i].slip;
            if (slip < 2.5f) {
                v->skid[i].AddSegment(0, &v->wheelAxis[i], (Vec3*)&v->body->m[0], 0.0f, 0);
            } else {
                float alpha = (slip - 2.5f) * 0.1f;
                if (alpha > 1.0f) {
                    alpha = 1.0f;
                }
                v->skid[i].AddSegment(&pos, &v->wheelAxis[i], (Vec3*)&v->body->m[0], alpha, v->surfaces->trackMark[surf]);
            }
        } else {
            v->skid[i].AddSegment(0, &v->wheelAxis[i], (Vec3*)&v->body->m[0], 0.0f, 0);
        }
    }
}

// NONMATCHING: RALLYTROPHY 0x46a200
// Same instruction stream apart from register choice: the original keeps -1 (the Color value and the
// DrawRect/DrawQuad colour argument) in edi, base/gear texture in ebp/ebx and spills the needle texture
// to the stack (frame 0x58 vs 0x50). Same unresolved -1 register issue as 0x484190 / 0x48e4c0.
void Car::FUN_0046a200()
{
    CarGaugeView* v = (CarGaugeView*)this;
#define TEX ((GaugeTextures*)DAT_0056b0c8)
    int base;
    if (!DAT_00501310) {
        base = TEX->plain;
    } else if (DAT_00501300[0]) {
        base = v->textureB;
    } else {
        base = v->textureA;
    }
    int gear = TEX->gear;
    int needles = TEX->needles;
    Vec2 pos[4];
    Vec2 uv[4];
    DWORD white = 0xffffffff;
    Color color(white);
    int iw = DAT_0056b0ac->m_width;
    int ih = DAT_0056b0ac->m_height;
    float fw = (float)iw;
    float fh = (float)ih;
    float sx = fw / 1024.0f;
    float sy = fh / 768.0f;
    pos[0].x = pos[3].x = sx * v->x;
    pos[1].x = pos[2].x = (v->x + 256.0f) * sx;
    pos[0].y = pos[1].y = sy * v->y;
    pos[2].y = pos[3].y = (v->y + 256.0f) * sy;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture((void*)base);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawRect(pos, color, 1.0f, 1.0f);
    uv[0].x = uv[3].x = (float)(v->drive.GetEngagedGear() + 1) * 0.125f;
    uv[1].x = uv[2].x = uv[3].x + 0.125f;
    uv[0].y = uv[1].y = 0.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[0].x = pos[3].x = v->gearX;
    pos[0].y = pos[1].y = v->gearY;
    pos[1].x = pos[2].x = v->gearX + 16.0f;
    pos[2].y = pos[3].y = v->gearY + 32.0f;
    int i;
    float tx = v->x;
    float ty = v->y;
    tx *= sx;
    ty *= sy;
    for (i = 0; i < 4; i++) {
        pos[i].x *= sx;
        pos[i].y *= sy;
        pos[i].x += tx;
        pos[i].y += ty;
    }
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture((void*)gear);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, color, 1.0f, 1.0f);
    if (!DAT_00501310) {
        return;
    }
    {
        RawVec3* vel = &v->body->vel;
        float speed = (float)sqrt(vel->x * vel->x + vel->y * vel->y + vel->z * vel->z) * 3.6f / v->speedMax;
        if (*(int*)&speed < 0) {
            speed = 0.0f;
        } else if (*(int*)&speed > 0x3f800000) {
            speed = 1.0f;
        }
        RotateQuad((float*)pos, 0.78539819f, speed, v->speedHalf * 0.5f, v->speedFrom - v->speedTo, v->speedTo);
        for (i = 0; i < 4; i++) {
            pos[i].x *= sx;
            pos[i].y *= sy;
            pos[i].x += (v->rpmX + v->x) * sx;
            pos[i].y += (v->rpmY + v->y) * sy;
        }
        uv[0].x = uv[3].x = 0.0f;
        uv[1].x = uv[2].x = 1.0f;
        uv[0].y = uv[1].y = 0.0f;
        uv[2].y = uv[3].y = 0.5f;
        ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture((void*)needles);
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, color, 1.0f, 1.0f);
        float rpm = v->drive.GetSmoothedRpm() / v->rpmMax;
        if (*(int*)&rpm < 0) {
            rpm = 0.0f;
        } else if (*(int*)&rpm > 0x3f800000) {
            rpm = 1.0f;
        }
        RotateQuad((float*)pos, 0.78539819f, rpm, v->rpmHalf * 0.5f, v->rpmFrom - v->rpmTo, v->rpmTo);
        for (i = 0; i < 4; i++) {
            pos[i].x *= sx;
            pos[i].y *= sy;
            pos[i].x += (v->speedX + v->x) * sx;
            pos[i].y += (v->speedY + v->y) * sy;
        }
        uv[0].x = uv[3].x = 0.0f;
        uv[1].x = uv[2].x = 1.0f;
        uv[0].y = uv[1].y = 0.5f;
        uv[2].y = uv[3].y = 1.0f;
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, color, 1.0f, 1.0f);
    }
}

// FUNCTION: RALLYTROPHY 0x46a790
// Corners of a needle quad rotated about its pivot: half width a, lengths b (forward) and c (back).
void RotateQuad(float* out, float base, float fraction, float a, float b, float c)
{
    float angle = (base + 3.1415927f) - (base + base + 3.1415927f) * fraction;
    float cs = (float)cos(angle);
    float sn = -(float)sin(angle);
    out[0] = sn * a - cs * c;
    out[1] = -(sn * c) - cs * a;
    out[2] = cs * b + sn * a;
    out[3] = sn * b - cs * a;
    out[4] = cs * b - sn * a;
    out[5] = sn * b + cs * a;
    out[6] = -(cs * c) - sn * a;
    out[7] = cs * a - sn * c;
}

// FUNCTION: RALLYTROPHY 0x46a850
void Car::FUN_0046a850()
{
    ((CarGaugeView*)this)->queue.Flush(((CarGaugeView*)this)->time);
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x46a870
// SYMBOL: _$E23
typedef void Generated_46a870();

// FUNCTION: RALLYTROPHY 0x46a890
// SYMBOL: _$E22
typedef void Generated_46a890();
