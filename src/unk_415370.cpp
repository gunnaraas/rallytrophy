// Stage sound loader (0x415370). Map: analysis/range-409690.md. Offsets are raw (the Stage class is declared elsewhere).
#include <string>
#include <stdlib.h>
#include <string.h>
#include "unk_415370.h"
#include "rallytrophy.h"
#include "sound.h"
#include "unk_414fe0.h"

extern int DAT_0050143c;   // sound volume percent

static ConfigKey AmbientKeys[0x1b] = {   // 0x4f56f0
    { "AmbientSound", 0 }, { "AmbientVolume", 1 }, { "NumberOfEvents", 1 },
    { "Event1File", 0 }, { "Event1Volume", 3 }, { "Event1Delay", 3 },
    { "Event2File", 0 }, { "Event2Volume", 3 }, { "Event2Delay", 3 },
    { "Event3File", 0 }, { "Event3Volume", 3 }, { "Event3Delay", 3 },
    { "Event4File", 0 }, { "Event4Volume", 3 }, { "Event4Delay", 3 },
    { "Event5File", 0 }, { "Event5Volume", 3 }, { "Event5Delay", 3 },
    { "Event6File", 0 }, { "Event6Volume", 3 }, { "Event6Delay", 3 },
    { "Event7File", 0 }, { "Event7Volume", 3 }, { "Event7Delay", 3 },
    { "Event8File", 0 }, { "Event8Volume", 3 }, { "Event8Delay", 3 },
};

static ConfigKey SurfaceKeys[0x1e] = {   // 0x4f57c8
    { "Roll", 0 }, { "RollPitchRange", 3 }, { "RollPitchMinMax", 3 }, { "RollVolumeRange", 3 }, { "RollVolumeMinMax", 3 }, { "RollAttenuation", 3 },
    { "Ground", 0 }, { "GroundPitchRange", 3 }, { "GroundPitchMinMax", 3 }, { "GroundVolumeRange", 3 }, { "GroundVolumeMinMax", 3 }, { "GroundAttenuation", 3 },
    { "Slide", 0 }, { "SlidePitchRange", 3 }, { "SlidePitchMinMax", 3 }, { "SlideVolumeRange", 3 }, { "SlideVolumeMinMax", 3 }, { "SlideAttenuation", 3 },
    { "Brake", 0 }, { "BrakePitchRange", 3 }, { "BrakePitchMinMax", 3 }, { "BrakeVolumeRange", 3 }, { "BrakeVolumeMinMax", 3 }, { "BrakeAttenuation", 3 },
    { "Acc", 0 }, { "AccPitchRange", 3 }, { "AccPitchMinMax", 3 }, { "AccVolumeRange", 3 }, { "AccVolumeMinMax", 3 }, { "AccAttenuation", 3 },
};

// Patched in place with the event digit (0x4f58b8, 0x4f58c4, 0x4f58d4).
static char Event1File[] = "Event1File";
static char Event1Volume[] = "Event1Volume";
static char Event1Delay[] = "Event1Delay";

struct V2 { float a, b; };

// One surface sound (0x2c bytes).
struct SurfSlot {
    Sample* sample;
    V2 pitchRange, pitchMinMax, volRange, volMinMax, atten;
};

#define FLD(T, o) (*(T*)((unsigned char*)this + (o)))

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

#define SURF_SET(S, KIND)                                                           \
    fx = cfg.GetString(KIND);                                                       \
    if (fx.length() > 1) {                                                          \
        S->sample = DAT_0056b0b0->LoadSample((base.Str() + fx).c_str());                  \
    }                                                                               \
    S->pitchRange = *(V2*)&cfg.GetVector2(KIND "PitchRange");                       \
    S->pitchMinMax = *(V2*)&cfg.GetVector2(KIND "PitchMinMax");                     \
    S->volRange = *(V2*)&cfg.GetVector2(KIND "VolumeRange");                        \
    S->volMinMax = *(V2*)&cfg.GetVector2(KIND "VolumeMinMax");                      \
    S->atten = *(V2*)&cfg.GetVector2(KIND "Attenuation");                           \
    S->pitchRange.a *= 0.2777778f;                                                  \
    S->pitchRange.b *= 0.2777778f;                                                  \
    S->volRange.a *= 0.2777778f;                                                    \
    S->volRange.b *= 0.2777778f;

// VC6 std::string layout twin for LoadSounds' mixed out-of-line and inline path operations.
struct LoadSoundsPath {
    LoadSoundsPath() : alloc(std::allocator<char>()) { Tidy(false); }
    LoadSoundsPath(const std::string& x) : alloc(*(const std::allocator<char>*)&x)
    {
        Tidy(false);
        Assign(x, 0, std::string::npos);
    }
    ~LoadSoundsPath() { ((std::string*)this)->~basic_string(); }
    void Tidy(bool built);                                                        // 0x402700
    LoadSoundsPath& Assign(const std::string& x, unsigned int pos, unsigned int n); // 0x4024f0
    LoadSoundsPath& AppendOut(const char* s, unsigned int n);                     // 0x403ba0
    LoadSoundsPath& AssignOut(const char* s, unsigned int n);                     // 0x404ba0
    LoadSoundsPath& AppendStringCall(const std::string& s, unsigned int pos, unsigned int n); // 0x408ef0
    bool Grow(unsigned int n, bool trim);                                         // 0x402980
    void Eos(unsigned int n);                                                     // 0x402960
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    void AppendCString(const char* s) { AppendOut(s, strlen(s)); }
    void AppendString(const std::string& s) { AppendStringCall(s, 0, std::string::npos); }
    const char* c_str() const { return ptr == 0 ? "" : ptr; }
    std::string& Str() { return *(std::string*)this; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len, res;
};

// NONMATCHING: RALLYTROPHY 0x415370
// `base` and `path` use target-shaped string calls via LoadSoundsPath (3302 bytes / 2145 differences). An earlier
// path append wrapper accidentally materialized a temporary std::string and regressed; the direct C-string form is kept.
// An explicit register self alias regressed to 3394/2219; an explicit std::string cast for operator+ regressed to 3439/2248.
void StageSoundLoader::LoadSounds()
{
    FLD(int, 0x15f8) = 0;
    FLD(int, 0x15fc) = 0;
    FLD(int, 0x1600) = 0;
    LoadSoundsPath base(FLD(std::string, 8));
    base.AppendOut("data/stage", strlen("data/stage"));
    char num[4];
    itoa(FLD(int, 4), num, 10);
    base.AppendOut(num, strlen(num));
    base.AppendOut(".snd", strlen(".snd"));
    Config cfg;
    if (File::Exists(base.c_str())) {
        LoadSoundsPath path;
        cfg.Define(AmbientKeys, 0x1b);
        cfg.Load(base.c_str(), true, false, 0);
        path.AssignOut("data/soundfx/", strlen("data/soundfx/"));
        path.AppendCString(cfg.GetString("AmbientSound"));
        int vol = cfg.GetInt("AmbientVolume");
        SoundSource* amb = new SoundSource;
        FLD(SoundSource*, 0x15f8) = amb;
        amb->SetSample(DAT_0056b0b0->LoadSample(path.c_str()));
        amb->SetVolume(DAT_0050143c * vol / 100);
        amb->loop = true;
        int n = cfg.GetInt("NumberOfEvents");
        FLD(int, 0x1600) = n;
        AmbientSound* ev = new AmbientSound[n];
        FLD(AmbientSound*, 0x15fc) = ev;
        for (int i = 0; i < n; i++) {
            Event1File[5] = (char)i + '1';
            Event1Volume[5] = Event1File[5];
            Event1Delay[5] = Event1File[5];
            path.AssignOut("data/soundfx/", strlen("data/soundfx/"));
            path.AppendCString(cfg.GetString(Event1File));
            AmbientSound* e = &ev[i];
            Sample* smp = DAT_0056b0b0->LoadSample(path.c_str());
            *(Sample**)((unsigned char*)e + 0x70) = smp;
            e->SetSample(smp);
            V2 d = *(V2*)&cfg.GetVector2(Event1Delay);
            e->minTime = d.a;
            e->maxTime = d.b;
            V2 v = *(V2*)&cfg.GetVector2(Event1Volume);
            e->minVol = v.a;
            e->maxVol = v.b;
            e->timer = RoundF((float)(rand() & 0x7fff) * 3.0518509e-05f * e->maxTime * 1000.0f);
        }
        FLD(int, 0x1604) = 0;
        cfg.Close();
    }
    std::string fx;
    SurfSlot* roll = (SurfSlot*)((unsigned char*)this + 0x1608);
    std::string* name = &FLD(std::string, 0x23c8);
    for (int i = 15; i >= 0; i--, name++, roll++) {
        if (name->length() > 1) {
            base.AssignB("data/global/surfacesounds/");
            base.AppendString(*name);
            cfg.Define(SurfaceKeys, 0x1e);
            cfg.Load(base.c_str(), true, false, 0);
            base.AssignB("data/soundfx/");
            SurfSlot* s = roll;
            SURF_SET(s, "Roll")
            s = roll + 16;
            SURF_SET(s, "Ground")
            s = roll + 32;
            SURF_SET(s, "Slide")
            s = roll + 48;
            SURF_SET(s, "Brake")
            s = roll + 64;
            SURF_SET(s, "Acc")
            cfg.Close();
        }
    }
}
