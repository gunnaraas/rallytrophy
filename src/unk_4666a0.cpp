// Car sounds (0x4666a0-0x46871f, stub 0x4686f0). Map: analysis/range-4666a0.md section 2.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46a8a0.h"
#include "unk_4666a0.h"
#include <math.h>
#include <stdlib.h>

// Keys of the car Sounds.ini (0x4fdba0, 25 entries).
ConfigKey SoundKeys[25] = {
    { "EngineThrottleRpm", 2 },
    { "EngineSoundIdle", 0 },
    { "EngineSound1", 0 },
    { "EngineSound2", 0 },
    { "EngineSound3", 0 },
    { "EngineSound1Range", 4 },
    { "EngineSound2Range", 4 },
    { "EngineSound3Range", 4 },
    { "EngineSoundFadeRange", 2 },
    { "EngineSoundVolume", 2 },
    { "EngineSoundVolumeAcc", 2 },
    { "EngineLoc", 4 },
    { "EngineAttenuation", 3 },
    { "ExhaustSoundIdle", 0 },
    { "ExhaustSound1", 0 },
    { "ExhaustSound2", 0 },
    { "ExhaustSound3", 0 },
    { "ExhaustSound1Range", 4 },
    { "ExhaustSound2Range", 4 },
    { "ExhaustSound3Range", 4 },
    { "ExhaustSoundFadeRange", 2 },
    { "ExhaustSoundVolume", 2 },
    { "ExhaustSoundVolumeAcc", 2 },
    { "ExhaustLoc", 4 },
    { "ExhaustAttenuation", 3 },
};

#define COPYR(dst, src) \
    do { \
        const ConfigVector3& c_ = (src); \
        (dst).low = c_.values[0]; \
        (dst).unk4 = c_.values[1]; \
        (dst).high = c_.values[2]; \
    } while (0)

#define COPY3(dst, src) \
    do { \
        const ConfigVector3& c_ = (src); \
        (dst).x = c_.values[0]; \
        (dst).y = c_.values[1]; \
        (dst).z = c_.values[2]; \
    } while (0)

// Gives a sound slot its 3D parameters: the sample, silent, looping, nearly flat range.
#define INIT_SLOT(slot, sample, priority) \
    do { \
        (slot)->source.SetSample(sample); \
        (slot)->source.SetVolume(0); \
        (slot)->source.is3D = (slot)->source.loop = true; \
        (slot)->source.SetRange(0.1f, 1.0f); \
        (slot)->source.priorityClass = (priority); \
        (slot)->surface = 0; \
    } while (0)

// NONMATCHING: RALLYTROPHY 0x4666a0
// Same size (2361) and instruction stream; only the scheduling of the is3D/loop byte stores differs in the
// six SetRange blocks of the slot and crash sources (the original places the first store between the two
// float pushes, this build after them). The first loop, strings, EH states and registers all match.
void Car::InitSounds(const std::string& dir)
{
    CarSoundView* v = (CarSoundView*)this;
    int i;
    for (i = 0; i < 4; i++) {
        v->layer[i].sample = 0;
        v->layer[i].source.loop = v->layer[i].source.is3D = true;
        v->layer[i].source.SetRange(5.0f, 150.0f);
        v->layer[i].source.priorityClass = 0xf;
        v->layer[i + 4].sample = 0;
        v->layer[i + 4].source.loop = v->layer[i + 4].source.is3D = true;
        v->layer[i + 4].source.SetRange(7.0f, 300.0f);
        v->layer[i + 4].source.priorityClass = 0xf;
    }
    Config config;
    config.Define(SoundKeys, 25);
    config.Load(SoundPath(dir, "Sounds.ini").c_str(), true, false, 0);
    ConfigVector2 engineAtt = config.GetVector2("EngineAttenuation");
    ConfigVector2 exhaustAtt = config.GetVector2("ExhaustAttenuation");
    for (i = 0; i < 4; i++) {
        v->layer[i].source.SetRange(engineAtt.values[0], engineAtt.values[1]);
        v->layer[i + 4].source.SetRange(exhaustAtt.values[0], exhaustAtt.values[1]);
    }
    if (DAT_0056b0b0) {
        NameString name;
        name.Assign(SoundPath(dir, config.GetString("EngineSound1")), 0, std::string::npos);
        v->layer[1].source.SetSample(v->layer[1].sample = DAT_0056b0b0->LoadSample(name.c_str()));
        name.Assign(SoundPath(dir, config.GetString("EngineSound2")), 0, std::string::npos);
        v->layer[2].source.SetSample(v->layer[2].sample = DAT_0056b0b0->LoadSample(name.c_str()));
        name.Assign(SoundPath(dir, config.GetString("EngineSound3")), 0, std::string::npos);
        v->layer[3].source.SetSample(v->layer[3].sample = DAT_0056b0b0->LoadSample(name.c_str()));
        name.Assign(SoundPath(dir, config.GetString("ExhaustSound1")), 0, std::string::npos);
        v->layer[5].source.SetSample(v->layer[5].sample = DAT_0056b0b0->LoadSample(name.c_str()));
        name.Assign(SoundPath(dir, config.GetString("ExhaustSound2")), 0, std::string::npos);
        v->layer[6].source.SetSample(v->layer[6].sample = DAT_0056b0b0->LoadSample(name.c_str()));
        name.Assign(SoundPath(dir, config.GetString("ExhaustSound3")), 0, std::string::npos);
        v->layer[7].source.SetSample(v->layer[7].sample = DAT_0056b0b0->LoadSample(name.c_str()));
        COPY3(v->engineLoc, config.GetVector3("EngineLoc"));
        COPY3(v->exhaustLoc, config.GetVector3("ExhaustLoc"));
        COPYR(v->layer[1].range, config.GetVector3("EngineSound1Range"));
        COPYR(v->layer[2].range, config.GetVector3("EngineSound2Range"));
        COPYR(v->layer[3].range, config.GetVector3("EngineSound3Range"));
        COPYR(v->layer[5].range, config.GetVector3("ExhaustSound1Range"));
        COPYR(v->layer[6].range, config.GetVector3("ExhaustSound2Range"));
        COPYR(v->layer[7].range, config.GetVector3("ExhaustSound3Range"));
        float engineFade = config.GetFloat("EngineSoundFadeRange");
        float engineVolume = config.GetFloat("EngineSoundVolume");
        float engineAcc = config.GetFloat("EngineSoundVolumeAcc");
        float exhaustFade = config.GetFloat("ExhaustSoundFadeRange");
        float exhaustVolume = config.GetFloat("ExhaustSoundVolume");
        float exhaustAcc = config.GetFloat("ExhaustSoundVolumeAcc");
        v->throttleRpm = config.GetFloat("EngineThrottleRpm");
        for (i = 0; i < 4; i++) {
            v->layer[i].fade = engineFade;
            v->layer[i].volume = engineVolume;
            v->layer[i].acc = engineAcc;
            v->layer[i + 4].fade = exhaustFade;
            v->layer[i + 4].volume = exhaustVolume;
            v->layer[i + 4].acc = exhaustAcc;
        }
        Sample* idle = v->layer[1].sample;
        INIT_SLOT(&v->slot[0], idle, 0xe);
        INIT_SLOT(&v->slot[1], idle, 0xa);
        INIT_SLOT(&v->slot[2], idle, 0xe);
        INIT_SLOT(&v->slot[4], idle, 0xa);
        INIT_SLOT(&v->slot[3], idle, 0xa);
        v->crashSample[0] = DAT_0056b0b0->LoadSample("data/soundfx/window_smash1.wav");
        v->crashSample[1] = DAT_0056b0b0->LoadSample("data/soundfx/window_smash2.wav");
        v->crashSample[2] = DAT_0056b0b0->LoadSample("data/soundfx/light_smash.wav");
        v->crashSample[3] = DAT_0056b0b0->LoadSample("data/soundfx/hard_crash1.wav");
        v->crashSample[4] = DAT_0056b0b0->LoadSample("data/soundfx/hard_crash2.wav");
        v->crashSample[5] = DAT_0056b0b0->LoadSample("data/soundfx/soft_crash1.wav");
        v->crashSample[6] = DAT_0056b0b0->LoadSample("data/soundfx/soft_crash2.wav");
        v->crash[1].SetVolume(0);
        v->crash[1].loop = false;
        v->crash[1].is3D = true;
        v->crash[1].SetRange(5.0f, 300.0f);
        v->crash[1].priorityClass = 3;
        v->crash[2].SetVolume(0);
        v->crash[2].loop = false;
        v->crash[2].is3D = true;
        v->crash[2].SetRange(5.0f, 300.0f);
        v->crash[2].priorityClass = 1;
        v->crash[0].SetVolume(0);
        v->crash[0].loop = false;
        v->crash[0].is3D = true;
        v->crash[0].SetRange(5.0f, 300.0f);
        v->crash[0].priorityClass = 2;
    }
}

// FUNCTION: RALLYTROPHY 0x466fe0
void Car::ReleaseSounds()
{
    CarSoundView* v = (CarSoundView*)this;
    FUN_004672c0();
    if (DAT_0056b0b0) {
        int i;
        for (i = 0; i < 3; i++) {
            DAT_0056b0b0->ReleaseSample(v->layer[i + 1].sample);
            DAT_0056b0b0->ReleaseSample(v->layer[i + 5].sample);
            v->layer[i + 1].sample = 0;
            v->layer[i + 5].sample = 0;
        }
        DAT_0056b0b0->ReleaseSample(v->crashSample[0]);
        DAT_0056b0b0->ReleaseSample(v->crashSample[1]);
        DAT_0056b0b0->ReleaseSample(v->crashSample[2]);
        DAT_0056b0b0->ReleaseSample(v->crashSample[3]);
        DAT_0056b0b0->ReleaseSample(v->crashSample[4]);
        DAT_0056b0b0->ReleaseSample(v->crashSample[5]);
        DAT_0056b0b0->ReleaseSample(v->crashSample[6]);
    }
}

// FUNCTION: RALLYTROPHY 0x4670c0
void Car::FUN_004670c0()
{
    CarSoundView* v = (CarSoundView*)this;
    int i;
    v->crashHead = 0;
    for (i = 0; i < 8; i++) {
        v->crashSlot[i].id = 0;
    }
    for (i = 1; i <= 3; i++) {
        v->layer[i].source.SetVolume(0);
        v->layer[i + 4].source.SetVolume(0);
        DAT_0056b0b0->AddSource(&v->layer[i].source);
        DAT_0056b0b0->AddSource(&v->layer[i + 4].source);
        v->layer[i].source.Play();
        v->layer[i + 4].source.Play();
    }
    Sample* idle = v->layer[1].sample;
    v->slot[0].source.SetSample(idle);
    v->slot[2].source.SetSample(idle);
    v->slot[3].source.SetSample(idle);
    v->slot[4].source.SetSample(idle);
    v->slot[1].source.SetSample(idle);
    v->slot[0].surface = 0;
    v->slot[2].surface = 0;
    v->slot[3].surface = 0;
    v->slot[4].surface = 0;
    v->slot[1].surface = 0;
    if (v->player->local) {
        DAT_0056b0b0->AddSource(&v->slot[0].source);
        DAT_0056b0b0->AddSource(&v->slot[2].source);
        DAT_0056b0b0->AddSource(&v->slot[3].source);
        DAT_0056b0b0->AddSource(&v->slot[4].source);
        DAT_0056b0b0->AddSource(&v->slot[1].source);
        DAT_0056b0b0->AddSource(&v->crash[1]);
        DAT_0056b0b0->AddSource(&v->crash[0]);
        DAT_0056b0b0->AddSource(&v->crash[2]);
        v->slot[0].source.unk54 = 0;
        v->slot[0].source.Play();
        v->slot[0].source.SetVolume(0);
        v->slot[2].source.unk54 = 0;
        v->slot[2].source.Play();
        v->slot[2].source.SetVolume(0);
        v->slot[3].source.unk54 = 0;
        v->slot[3].source.Play();
        v->slot[3].source.SetVolume(0);
        v->slot[4].source.unk54 = 0;
        v->slot[4].source.Play();
        v->slot[4].source.SetVolume(0);
        v->slot[1].source.unk54 = 0;
        v->slot[1].source.Play();
        v->slot[1].source.SetVolume(0);
    }
}

// FUNCTION: RALLYTROPHY 0x4672c0
void Car::FUN_004672c0()
{
    CarSoundView* v = (CarSoundView*)this;
    int i;
    v->crashHead = 0;
    for (i = 0; i < 8; i++) {
        v->crashSlot[i].id = 0;
    }
    for (i = 1; i <= 3; i++) {
        v->layer[i].source.Stop();
        v->layer[i + 4].source.Stop();
    }
    for (i = 1; i <= 3; i++) {
        DAT_0056b0b0->RemoveSource(&v->layer[i].source);
        DAT_0056b0b0->RemoveSource(&v->layer[i + 4].source);
    }
    if (v->player->local) {
        v->slot[0].source.Stop();
        v->slot[2].source.Stop();
        v->slot[3].source.Stop();
        v->slot[4].source.Stop();
        v->slot[1].source.Stop();
        v->crash[1].Stop();
        v->crash[2].Stop();
        v->crash[0].Stop();
        DAT_0056b0b0->RemoveSource(&v->slot[0].source);
        DAT_0056b0b0->RemoveSource(&v->slot[2].source);
        DAT_0056b0b0->RemoveSource(&v->slot[3].source);
        DAT_0056b0b0->RemoveSource(&v->slot[4].source);
        DAT_0056b0b0->RemoveSource(&v->slot[1].source);
        DAT_0056b0b0->RemoveSource(&v->crash[1]);
        DAT_0056b0b0->RemoveSource(&v->crash[0]);
        DAT_0056b0b0->RemoveSource(&v->crash[2]);
    }
}

// Float to int with the x87 default rounding (inline assembler, as in the original).
inline int FloatToInt(float f)
{
    int i;
    __asm {
        fld f
        fistp i
    }
    return i;
}

// Transforms *point by the rotation *matrix (rows 0x10 apart) and adds *trans into *result.
#define TRANSFORM_ADD() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
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

// One looped surface source: fades the stage's surface sound for `x` in or out and applies the curves.
#define SURFACE_BLOCK(SRC, GETTER, XEXPR, GUARD) \
    { \
        SurfaceSound* entry = v->stage->GETTER(bestId); \
        Sample* sample = entry->sample; \
        if (sample && GUARD) { \
            int level = v->slot[SRC].source.unk54; \
            if (sample != v->slot[SRC].source.sample) { \
                level -= dt; \
                if (level < 0) { \
                    level = 0; \
                    v->slot[SRC].source.SetSample(sample); \
                    v->slot[SRC].surface = entry; \
                } \
            } else { \
                level += dt; \
            } \
            if (level > 0x100) \
                level = 0x100; \
            v->slot[SRC].source.unk54 = level; \
            SurfaceSound* ref = v->slot[SRC].surface; \
            if (ref) { \
                float x = XEXPR; \
                float volume = SoundCurveA(x, ref) * DAT_00501438[0] * 0.01f; \
                float pitch = SoundCurveB(x, ref); \
                float f = (float)level * 0.00390625f; \
                if (*(int*)&f < 0) \
                    f = 0.0f; \
                else if (*(int*)&f > 0x3f800000) \
                    f = 1.0f; \
                volume = f * volume; \
                v->slot[SRC].source.SetVolume(FloatToInt(volume)); \
                v->slot[SRC].source.SetRate((int)((float)sample->rate * pitch)); \
                v->slot[SRC].source.SetRange(v->slot[SRC].surface->nearDistance, v->slot[SRC].surface->farDistance); \
            } \
        } else { \
            v->slot[SRC].source.SetVolume(0); \
            v->slot[SRC].source.unk54 = 0; \
        } \
    }

// NONMATCHING: RALLYTROPHY 0x467440
// Same size (4039) but 950 bytes differ: register choices (ebx/edi swaps, edx/ecx for the pointer temps), the best-surface
// search keeps `best` in memory where the original has it in edi, the wheel sum loop uses fxch+fadd instead of fld+faddp,
// and the x*x+y*y+z*z order of the second speed block. Not resolved after ~35 variants.
// Car::UpdateSounds(dt): per-frame update of the speech/crash one-shots, the engine and exhaust layers and the surface loops.
void Car::FUN_00467440(int dt)
{
    CarSoundView* v = (CarSoundView*)this;
    int i;
    int own = v->player->local;
    if (own) {
        for (i = 0; i < 8; i++) {
            CrashSlot* q = &v->crashSlot[i];
            if (q->id > 0 && q->timeout >= v->raceTime) {
                DAT_0056b0b0->QueueSpeech(q->id, q->aux, 0);
                q->id = 0;
            }
        }
        if (v->state != 2) {
            if (DAT_0050147c) {
                if (v->flagA && v->raceTime - v->lastA > 5000) {
                    int id;
                    if ((rand() & 0xff) < 0x5c)
                        id = 0x2f;
                    else
                        id = rand() % 10 + 0x2f;
                    int slot = v->crashHead;
                    v->crashSlot[slot].id = id;
                    v->crashSlot[slot].aux = 0;
                    v->crashSlot[slot].timeout = v->raceTime + 1000;
                    v->crashHead = (v->crashHead + 1) & 7;
                    v->lastA = v->raceTime;
                }
                if (v->flagB) {
                    int t = v->raceTime;
                    if (t - v->lastB > 5000) {
                        int id;
                        if ((rand() & 0xff) < 0x5c)
                            id = 0x39;
                        else
                            id = rand() % 7 + 0x39;
                        DAT_0056b0b0->QueueSpeech(id, 0, 0);
                        v->lastB = t;
                    }
                }
            }
            int t = v->raceTime;
            if (t - v->lastC > 10000) {
                if (v->stage->FUN_00417e60(&v->body->position, 80.0f)) {
                    if ((rand() & 0xff) < 0x80) {
                        int r = rand() % 2;
                        DAT_0056b0b0->QueueSpeech(r + 0x40, 2, 0);
                    }
                    v->lastC = t;
                }
            }
        }
        if (v->light) {
            SoundSource* src = &v->crash[2];
            src->SetSample(v->crashSample[2]);
            src->SetVolume(DAT_00501438[1] * 127 / 100);
            src->SetPosition((const Vector3*)&v->lightPos);
            src->Play();
        }
        if (v->window) {
            SoundSource* src = &v->crash[1];
            src->SetSample(v->crashSample[rand() & 1]);
            src->SetVolume(DAT_00501438[1] * 127 / 100);
            src->SetPosition((const Vector3*)&v->windowPos);
            src->Play();
        }
        if (v->hard) {
            SoundSource* src = &v->crash[0];
            src->SetSample(v->crashSample[3 + (rand() & 1)]);
            src->SetVolume(v->hardVolume * DAT_00501438[1] / 100);
            src->SetPosition((const Vector3*)&v->hardPos);
            src->Play();
        } else if (v->soft) {
            SoundSource* src = &v->crash[0];
            src->SetSample(v->crashSample[5 + (rand() & 1)]);
            src->SetVolume(v->softVolume * DAT_00501438[1] / 100);
            src->SetPosition((const Vector3*)&v->softPos);
            src->Play();
        }
    }
    v->flagA = 0;
    v->flagB = 0;
    v->window = 0;
    v->light = 0;
    v->hard = 0;
    v->hardVolume = 0;
    v->soft = 0;
    v->softVolume = 0;

    {
        float smoothed = ((Drivetrain*)((char*)this + 0xdb8))->GetSmoothedRpm();
        float rpm = smoothed + v->throttleRpm * v->throttle;
        float fade[3];
        int volumes[3];
        fade[0] = SoundRangeFade(rpm, &v->layer[1].range, v->layer[1].fade, 0, 1);
        fade[1] = SoundRangeFade(rpm, &v->layer[2].range, v->layer[2].fade, 1, 1);
        fade[2] = SoundRangeFade(rpm, &v->layer[3].range, v->layer[3].fade, 1, 0);
        for (i = 0; i < 3; i++) {
            SoundLayer* layer = &v->layer[1 + i];
            int volume = FloatToInt((layer->acc * v->throttle + layer->volume) * fade[i]);
            volumes[i] = volume;
            if (volume) {
                int rate = layer->sample->rate;
                layer->source.SetRate((int)((float)rate * rpm / layer->range.unk4));
            }
            layer->source.SetVolume(volume * DAT_00501438[0] / 100);
        }
        fade[0] = SoundRangeFade(rpm, &v->layer[5].range, v->layer[5].fade, 0, 1);
        fade[1] = SoundRangeFade(rpm, &v->layer[6].range, v->layer[6].fade, 1, 1);
        fade[2] = SoundRangeFade(rpm, &v->layer[7].range, v->layer[7].fade, 1, 0);
        for (i = 0; i < 3; i++) {
            SoundLayer* layer = &v->layer[5 + i];
            int volume = FloatToInt((layer->acc * v->throttle + layer->volume) * fade[i]);
            volumes[i] = volume;
            if (volume) {
                int rate = layer->sample->rate;
                layer->source.SetRate((int)((float)rate * rpm / layer->range.unk4));
            }
            layer->source.SetVolume(volume * DAT_00501438[0] / 100);
        }

    }
    Vec3 enginePos;
    Vec3 exhaustPos;
    Vec3 engineLoc = v->engineLoc;
    Vec3 exhaustLoc = v->exhaustLoc;
    if (v->cockpit)
        *(int*)&exhaustLoc.x = *(int*)&engineLoc.x = v->viewOffset;
    {
        const float* trans = (const float*)&v->body->position;
        const float* matrix = &v->body->matrix[0][0];
        const float* point = (const float*)&engineLoc;
        float* result = (float*)&enginePos;
        TRANSFORM_ADD();
    }
    {
        const float* trans = (const float*)&v->body->position;
        const float* matrix = &v->body->matrix[0][0];
        const float* point = (const float*)&exhaustLoc;
        float* result = (float*)&exhaustPos;
        TRANSFORM_ADD();
    }
    for (i = 0; i < 4; i++) {
        v->layer[i].source.SetPosition((const Vector3*)&enginePos);
        v->layer[4 + i].source.SetPosition((const Vector3*)&exhaustPos);
    }

    if (v->player->local) {
        int best = 0;
        int bestId = -1;
        int counts[4];
        int ids[4];
        int nIds = 0;
        for (i = 0; i < 4; i++) {
            if (v->wheel[i].contact && v->wheelSurface[i] < 15) {
                int surf = v->wheelSurface[i];
                int j;
                for (j = 0; j < nIds; j++) {
                    if (ids[j] == surf) {
                        if (++counts[j] > best) {
                            best = counts[j];
                            bestId = surf;
                        }
                        break;
                    }
                }
                if (j >= nIds) {
                    ids[nIds] = surf;
                    counts[nIds] = 1;
                    nIds++;
                    if (!best) {
                        best = 1;
                        bestId = surf;
                    }
                }
            }
        }
        if (!best) {
            v->slot[0].source.SetVolume(0);
            v->slot[1].source.SetVolume(0);
            v->slot[2].source.SetVolume(0);
            v->slot[3].source.SetVolume(0);
            v->slot[4].source.SetVolume(0);
            return;
        }
        v->slot[0].source.SetVolume(0);
        v->slot[1].source.SetVolume(0);
        v->slot[2].source.SetVolume(0);
        v->slot[3].source.SetVolume(0);
        v->slot[4].source.SetVolume(0);
        v->slot[0].source.SetPosition((const Vector3*)&v->body->position);
        v->slot[1].source.SetPosition((const Vector3*)&v->body->position);
        v->slot[2].source.SetPosition((const Vector3*)&v->body->position);
        v->slot[3].source.SetPosition((const Vector3*)&v->body->position);
        v->slot[4].source.SetPosition((const Vector3*)&v->body->position);

        float sum64 = 0, sum6c = 0, sum70 = 0, sum68 = 0;
        int n = 0;
        for (i = 0; i < 4; i++) {
            SoundWheel* w = &v->wheel[i];
            if (w->contact) {
                sum68 += w->v68;
                sum64 += w->v64;
                sum6c += w->v6c;
                sum70 += w->v70;
                n++;
            }
        }
        float inv = 1.0f / (float)n;
        float a64 = sum64 * inv;
        float a6c = sum6c * inv;
        float a70 = sum70 * inv;

        SURFACE_BLOCK(0, SoundForSpeed, v->body->velocity.Length(), 1)
        SURFACE_BLOCK(1, SoundForSpeed2, v->body->velocity.Length(), 1)
        SURFACE_BLOCK(2, SoundForSlip, (float)(fabs(a70) * fabs(a64) * 0.63662), 1)
        SURFACE_BLOCK(3, SoundForRoll, a6c, a6c > 0)
        SURFACE_BLOCK(4, SoundForRoll2, (float)fabs(a6c), a6c < 0)
    }
}

// FUNCTION: RALLYTROPHY 0x468410
float SoundRangeFade(float x, const SoundRange* r, float fade, int lowSide, int highSide)
{
    float result = 1.0f;
    if (!(x < r->low) && x <= r->high) {
        return 1.0f;
    }
    if (x < r->low && lowSide) {
        float d = r->low - x;
        if (d >= fade) {
            return 0.0f;
        }
        result = 1.0f - d / fade;
    } else if (x > r->high && highSide) {
        float d = x - r->high;
        if (d >= fade) {
            return 0.0f;
        }
        result = 1.0f - d / fade;
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x4684b0
float SoundCurveA(float x, const SurfaceSound* c)
{
    if (x > c->lo1) {
        if (x > c->hi1) {
            return c->val1b;
        }
        float t = (x - c->lo1) / (c->hi1 - c->lo1);
        return (1.0f - t) * c->val1a + t * c->val1b;
    }
    return c->val1a;
}

// FUNCTION: RALLYTROPHY 0x468500
float SoundCurveB(float x, const SurfaceSound* c)
{
    if (x > c->lo2) {
        if (x > c->hi2) {
            return c->val2b;
        }
        float t = (x - c->lo1) / (c->hi2 - c->lo2);
        return (1.0f - t) * c->val2a + t * c->val2b;
    }
    return c->val2a;
}

static inline float LengthSquared(const Vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

// FUNCTION: RALLYTROPHY 0x468550
void Car::UpdateSurfaceSound()
{
    CarSoundView* v = (CarSoundView*)this;
    int i;
    const Vec3* pos = &v->body->position;
    SurfaceZone* z = v->stage->FindZone((const float*)&v->body->position);
    if (z && z != v->lastZone) {
        Vec3 d(*pos);
        d -= *(const Vec3*)&z->x0;
        d.y = 0;
        if (LengthSquared(d) < 100.0f) {
            d.y = 0;
            if ((float)fabs(d.x * z->dirX + d.y * z->unk24 + d.z * z->dirZ) < 0.5f) {
                v->ring[v->ringHead] = z;
                v->ringHead = (v->ringHead + 1) & 15;
                for (i = 0; i < 4; i++) {
                    z->done[i] = 0;
                    int id = z->sound[i];
                    if (id != -1) {
                        int* done = 0;
                        if (id < 14 || id > 15) {
                            done = &z->done[i];
                        }
                        DAT_0056b0b0->QueueSpeech(id, 1, done);
                    }
                }
                v->lastZone = z;
            }
        }
    }
    for (int n = 0; n < 16; n++) {
        SurfaceZone* queued = v->ring[n];
        if (queued) {
            for (i = 0; i < 4; i++) {
                if (queued->done[i]) {
                    queued->done[i] = 0;
                    v->queue.Push(queued->sound[i], v->raceTime);
                }
            }
        }
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x4686f0
// SYMBOL: _$E23
typedef void Generated_4686f0();

// FUNCTION: RALLYTROPHY 0x468710
// SYMBOL: _$E22
typedef void Generated_468710();
