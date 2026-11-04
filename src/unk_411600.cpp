// Stage config parser (0x411600): reads the stage ini into the Stage object, then data/stagetimes<N>.ini.
// Field offsets are raw (the Stage class is declared elsewhere, see unk_481390.h).
#define private public
#define protected public
#include <string>
#undef private
#undef protected
#include <stdlib.h>
#include <string.h>
#include "unk_411600.h"
#include "rallytrophy.h"

extern ConfigKey DAT_004f3198[];     // 0x11d keys
extern ConfigKey DAT_004f3a80[];     // 0x1a keys
extern float DAT_004e13a8, DAT_004e13a0, DAT_0050150c, DAT_00501510, DAT_00501514;
extern int DAT_00501464, DAT_00501460, DAT_0050132c;
extern char DAT_004f3bd8[];          // "CPUSplit0": the digit at [8] is patched in the loop
extern char DAT_004f3bc8[];          // "CPUFTCSplit0": digit at [11]

struct Raw2 { int a, b; };
struct Raw3 { int a, b, c; };
struct Raw4 { int a, b, c, d; };

static inline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

static __forceinline void Set4(float* d, float a, float b, float c, float e) { d[0] = a; d[1] = b; d[2] = c; d[3] = e; }
#define FLD(T, o) (*(T*)((unsigned char*)this + (o)))
#define CP2(o, e) { r_ = (const int*)&(e); FLD(int, o) = r_[0]; FLD(int, (o) + 4) = r_[1]; }
#define CP3(o, e) { r_ = (const int*)&(e); FLD(int, o) = r_[0]; FLD(int, (o) + 4) = r_[1]; FLD(int, (o) + 8) = r_[2]; }
#define CP4(o, e) { r_ = (const int*)&(e); FLD(int, o) = r_[0]; FLD(int, (o) + 4) = r_[1]; FLD(int, (o) + 8) = r_[2]; FLD(int, (o) + 12) = r_[3]; }

// Sound-name assigns of R0-T6: the original calls std::string::_Grow out of line but copies and terminates inline.
struct SndStr {
    char al;
    char* p;
    unsigned n, r;
    bool Grow(unsigned n, bool trim);
};
#pragma comment(linker, "/alternatename:?Grow@SndStr@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
static inline int Bal(int a) { return a + 1; }
static __forceinline void SetSnd(std::string& d, const char* s)
{
    SndStr& t = (SndStr&)d;
    unsigned n = strlen(s);
    if (t.Grow(n, true)) {
        memcpy(t.p, s, n);
        char* e = t.p;
        t.n = n;
        e[n] = 0;
    }
}

static __forceinline void AppS(std::string& d, const char* s)
{
    SndStr& t = (SndStr&)d;
    unsigned n = strlen(s);
    if ((unsigned)-1 - t.n <= n)
        std::_Xlen();
    if (n != 0 && t.Grow(n += t.n, false)) {
        memcpy(t.p + t.n, s, n - t.n);
        t.p[t.n = n] = 0;
    }
}

// assign(const char*) with _Grow expanded but _Eos left as a call (T7-T9Sound).
struct XS : std::string {
    void EosX(unsigned n);
    void Asg(const char* s)
    {
        unsigned n = strlen(s);
        if (_Grow(n, true)) {
            memcpy(_Ptr, s, n);
            EosX(n);
        }
    }
};
#pragma comment(linker, "/alternatename:?EosX@XS@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")

// NONMATCHING: RALLYTROPHY 0x411600
// Tried 2026-10-31: switch(level), pointer-walk loops, name = copy of this+8 (not empty), SetSnd (out-of-line _Grow via /alternatename overlay) for R0-T6 sounds, 13 trailing discarded Bal(1) (10-15 tried; 13/14 best): 1306->680 insns. Still differs: cfg at ebp-0x20 not ebp-0x7c (frame 0x84 vs 0x70: VC6 does not share the buf/RoundF temp slots; block-scoping buf, decl order, plain/forceinline RoundF, Raw2/Raw3 tmp copies all no help), Vector2/3 copy load/store interleave.
// Tried 2026-10-31 re-pass: declaration order of cfg/fog/col/buf/name (all permutations tried) never moves cfg off ebp-0x20 (frame stays 0x84); block-scoped buf, buf in a forceinline helper, loop counter volatile (sizes 8613/8593 closer but insns differ rises) and a RoundF macro with function-scope float temp (size 9042) all no help; the original has the inlined RoundF float spill at ebp-0x10 and r temps at the top of the frame with cfg lowest, ours puts them below cfg.
void StageConfigLoader::LoadConfig(const char* path)
{
    const int* r_;
    ConfigVector3 fog;
    Config cfg;
    cfg.Define(DAT_004f3198, 0x11d);
    cfg.Load(path, true, false, 0);
    CP2(0x234, cfg.GetVector2("AnimalSpeed"));
    FLD(float, 0x23c) = cfg.GetFloat("AnimalAnimationSpeed");
    FLD(float, 0x240) = cfg.GetFloat("AnimalActivateDistance");
    FLD(float, 0x244) = cfg.GetFloat("AnimalScareDistance");
    FLD(float, 0x248) = cfg.GetFloat("AnimalPropability");
    FLD(int, 0x22c) = cfg.GetInt("WeatherEffect");
    FLD(int, 0x1a4) = cfg.GetInt("FogMode");
    CP2(0x1ac, cfg.GetVector2("FogRange"));
    FLD(float, 0x1b4) = cfg.GetFloat("FogDensity");
    fog = cfg.GetVector3("FogColor");
    FLD(unsigned char, 0x1ab) = 0;
    FLD(char, 0x1aa) = (char)RoundF(fog.values[0]);
    FLD(char, 0x1a9) = (char)RoundF(fog.values[1]);
    FLD(char, 0x1a8) = (char)RoundF(fog.values[2]);
    FLD(float, 0x230) = cfg.GetFloat("DriveLightIntensity");
    CP3(0x1b8, cfg.GetVector3("SkidMarkColor"));
    FLD(float, 0x1b8) *= 0.003921569f;
    FLD(float, 0x1bc) *= 0.003921569f;
    FLD(float, 0x1c0) *= 0.003921569f;
    CP3(0x1c4, cfg.GetVector3("AmbientColor"));
    FLD(float, 0x1c4) *= 0.003921569f;
    FLD(float, 0x1c8) *= 0.003921569f;
    FLD(float, 0x1cc) *= 0.003921569f;
    CP3(0x1d0, cfg.GetVector3("SunColorStatic"));
    CP3(0x1dc, cfg.GetVector3("SunColorDynamic"));
    FLD(float, 0x1d0) *= 0.003921569f;
    FLD(float, 0x1d4) *= 0.003921569f;
    FLD(float, 0x1d8) *= 0.003921569f;
    FLD(float, 0x1dc) *= 0.003921569f;
    FLD(float, 0x1e0) *= 0.003921569f;
    FLD(float, 0x1e4) *= 0.003921569f;
    CP2(0x1e8, cfg.GetVector2("ShadowRange"));
    CP2(0x1f0, cfg.GetVector2("ShadowScale"));
    ConfigVector4 col;
    col = cfg.GetVector4("ParticleColor");
    Set4(&FLD(float, 0x1f8), col.values[0] * 0.003921569f, col.values[1] * 0.003921569f, col.values[2] * 0.003921569f, col.values[3] * 0.003921569f);
    col = cfg.GetVector4("InteriorWindowColor");
    Set4(&FLD(float, 0x208), col.values[0] * 0.003921569f, col.values[1] * 0.003921569f, col.values[2] * 0.003921569f, col.values[3] * 0.003921569f);
    FLD(int, 0x218) = cfg.GetInt("LensFlare");
    FLD(std::string, 0x21c) = cfg.GetString("LensFlareSettings");
    FLD(float, 0x1a0) = cfg.GetFloat("FlareAlphaScale");
    *(Raw4*)&FLD(int, 0x65c) = *(Raw4*)&cfg.GetVector4("SplashColor1");
    *(Raw4*)&FLD(int, 0x66c) = *(Raw4*)&cfg.GetVector4("SplashColor2");
    *(Raw4*)&FLD(int, 0x67c) = *(Raw4*)&cfg.GetVector4("SplashColor3");
    *(Raw4*)&FLD(int, 0x68c) = *(Raw4*)&cfg.GetVector4("SplashColor4");
    FLD(float, 0x69c) = cfg.GetFloat("SplashRate1");
    FLD(float, 0x6a0) = cfg.GetFloat("SplashRate2");
    FLD(float, 0x6a4) = cfg.GetFloat("SplashRate3");
    FLD(float, 0x6a8) = cfg.GetFloat("SplashRate4");
    FLD(int, 0x24c) = cfg.GetInt("R0Dynamics");
    FLD(int, 0x250) = cfg.GetInt("R1Dynamics");
    FLD(int, 0x254) = cfg.GetInt("R2Dynamics");
    FLD(int, 0x258) = cfg.GetInt("R3Dynamics");
    FLD(int, 0x25c) = cfg.GetInt("R4Dynamics");
    FLD(int, 0x260) = cfg.GetInt("T0Dynamics");
    FLD(int, 0x264) = cfg.GetInt("T1Dynamics");
    FLD(int, 0x268) = cfg.GetInt("T2Dynamics");
    FLD(int, 0x26c) = cfg.GetInt("T3Dynamics");
    FLD(int, 0x270) = cfg.GetInt("T4Dynamics");
    FLD(int, 0x274) = cfg.GetInt("T5Dynamics");
    FLD(int, 0x278) = cfg.GetInt("T6Dynamics");
    FLD(int, 0x27c) = cfg.GetInt("T7Dynamics");
    FLD(int, 0x280) = cfg.GetInt("T8Dynamics");
    FLD(int, 0x284) = cfg.GetInt("T9Dynamics");
    FLD(int, 0x288) = cfg.GetInt("S0Dynamics");
    FLD(int, 0x28c) = cfg.GetInt("S1Dynamics");
    FLD(int, 0x290) = cfg.GetInt("S2Dynamics");
    FLD(int, 0x294) = cfg.GetInt("S3Dynamics");
    FLD(int, 0x298) = cfg.GetInt("S4Dynamics");
    FLD(float, 0x29c) = cfg.GetFloat("R0Elasticity");
    FLD(float, 0x2a0) = cfg.GetFloat("R1Elasticity");
    FLD(float, 0x2a4) = cfg.GetFloat("R2Elasticity");
    FLD(float, 0x2a8) = cfg.GetFloat("R3Elasticity");
    FLD(float, 0x2ac) = cfg.GetFloat("R4Elasticity");
    FLD(float, 0x2b0) = cfg.GetFloat("T0Elasticity");
    FLD(float, 0x2b4) = cfg.GetFloat("T1Elasticity");
    FLD(float, 0x2b8) = cfg.GetFloat("T2Elasticity");
    FLD(float, 0x2bc) = cfg.GetFloat("T3Elasticity");
    FLD(float, 0x2c0) = cfg.GetFloat("T4Elasticity");
    FLD(float, 0x2c4) = cfg.GetFloat("T5Elasticity");
    FLD(float, 0x2c8) = cfg.GetFloat("T6Elasticity");
    FLD(float, 0x2cc) = cfg.GetFloat("T7Elasticity");
    FLD(float, 0x2d0) = cfg.GetFloat("T8Elasticity");
    FLD(float, 0x2d4) = cfg.GetFloat("T9Elasticity");
    FLD(float, 0x2d8) = cfg.GetFloat("S0Elasticity");
    FLD(float, 0x2dc) = cfg.GetFloat("S1Elasticity");
    FLD(float, 0x2e0) = cfg.GetFloat("S2Elasticity");
    FLD(float, 0x2e4) = cfg.GetFloat("S3Elasticity");
    FLD(float, 0x2e8) = cfg.GetFloat("S4Elasticity");
    FLD(float, 0x42c) = cfg.GetFloat("R0BodyFriction");
    FLD(float, 0x430) = cfg.GetFloat("R1BodyFriction");
    FLD(float, 0x434) = cfg.GetFloat("R2BodyFriction");
    FLD(float, 0x438) = cfg.GetFloat("R3BodyFriction");
    FLD(float, 0x43c) = cfg.GetFloat("R4BodyFriction");
    FLD(float, 0x440) = cfg.GetFloat("T0BodyFriction");
    FLD(float, 0x444) = cfg.GetFloat("T1BodyFriction");
    FLD(float, 0x448) = cfg.GetFloat("T2BodyFriction");
    FLD(float, 0x44c) = cfg.GetFloat("T3BodyFriction");
    FLD(float, 0x450) = cfg.GetFloat("T4BodyFriction");
    FLD(float, 0x454) = cfg.GetFloat("T5BodyFriction");
    FLD(float, 0x458) = cfg.GetFloat("T6BodyFriction");
    FLD(float, 0x45c) = cfg.GetFloat("T7BodyFriction");
    FLD(float, 0x460) = cfg.GetFloat("T8BodyFriction");
    FLD(float, 0x464) = cfg.GetFloat("T9BodyFriction");
    FLD(float, 0x468) = cfg.GetFloat("S0BodyFriction");
    FLD(float, 0x46c) = cfg.GetFloat("S1BodyFriction");
    FLD(float, 0x470) = cfg.GetFloat("S2BodyFriction");
    FLD(float, 0x474) = cfg.GetFloat("S3BodyFriction");
    FLD(float, 0x478) = cfg.GetFloat("S4BodyFriction");
    FLD(float, 0x47c) = cfg.GetFloat("R0Roughness");
    FLD(float, 0x480) = cfg.GetFloat("R1Roughness");
    FLD(float, 0x484) = cfg.GetFloat("R2Roughness");
    FLD(float, 0x488) = cfg.GetFloat("R3Roughness");
    FLD(float, 0x48c) = cfg.GetFloat("R4Roughness");
    FLD(float, 0x490) = cfg.GetFloat("T0Roughness");
    FLD(float, 0x494) = cfg.GetFloat("T1Roughness");
    FLD(float, 0x498) = cfg.GetFloat("T2Roughness");
    FLD(float, 0x49c) = cfg.GetFloat("T3Roughness");
    FLD(float, 0x4a0) = cfg.GetFloat("T4Roughness");
    FLD(float, 0x4a4) = cfg.GetFloat("T5Roughness");
    FLD(float, 0x4a8) = cfg.GetFloat("T6Roughness");
    FLD(float, 0x4ac) = cfg.GetFloat("T7Roughness");
    FLD(float, 0x4b0) = cfg.GetFloat("T8Roughness");
    FLD(float, 0x4b4) = cfg.GetFloat("T9Roughness");
    FLD(float, 0x4b8) = cfg.GetFloat("S0Roughness");
    FLD(float, 0x4bc) = cfg.GetFloat("S1Roughness");
    FLD(float, 0x4c0) = cfg.GetFloat("S2Roughness");
    FLD(float, 0x4c4) = cfg.GetFloat("S3Roughness");
    FLD(float, 0x4c8) = cfg.GetFloat("S4Roughness");
    FLD(int, 0x4cc) = cfg.GetInt("R0Sparks");
    FLD(int, 0x4d0) = cfg.GetInt("R1Sparks");
    FLD(int, 0x4d4) = cfg.GetInt("R2Sparks");
    FLD(int, 0x4d8) = cfg.GetInt("R3Sparks");
    FLD(int, 0x4dc) = cfg.GetInt("R4Sparks");
    FLD(int, 0x4e0) = cfg.GetInt("T0Sparks");
    FLD(int, 0x4e4) = cfg.GetInt("T1Sparks");
    FLD(int, 0x4e8) = cfg.GetInt("T2Sparks");
    FLD(int, 0x4ec) = cfg.GetInt("T3Sparks");
    FLD(int, 0x4f0) = cfg.GetInt("T4Sparks");
    FLD(int, 0x4f4) = cfg.GetInt("T5Sparks");
    FLD(int, 0x4f8) = cfg.GetInt("T6Sparks");
    FLD(int, 0x4fc) = cfg.GetInt("T7Sparks");
    FLD(int, 0x500) = cfg.GetInt("T8Sparks");
    FLD(int, 0x504) = cfg.GetInt("T9Sparks");
    FLD(int, 0x508) = cfg.GetInt("S0Sparks");
    FLD(int, 0x50c) = cfg.GetInt("S1Sparks");
    FLD(int, 0x510) = cfg.GetInt("S2Sparks");
    FLD(int, 0x514) = cfg.GetInt("S3Sparks");
    FLD(int, 0x518) = cfg.GetInt("S4Sparks");
    FLD(int, 0x51c) = cfg.GetInt("R0Splash");
    FLD(int, 0x520) = cfg.GetInt("R1Splash");
    FLD(int, 0x524) = cfg.GetInt("R2Splash");
    FLD(int, 0x528) = cfg.GetInt("R3Splash");
    FLD(int, 0x52c) = cfg.GetInt("R4Splash");
    FLD(int, 0x530) = cfg.GetInt("T0Splash");
    FLD(int, 0x534) = cfg.GetInt("T1Splash");
    FLD(int, 0x538) = cfg.GetInt("T2Splash");
    FLD(int, 0x53c) = cfg.GetInt("T3Splash");
    FLD(int, 0x540) = cfg.GetInt("T4Splash");
    FLD(int, 0x544) = cfg.GetInt("T5Splash");
    FLD(int, 0x548) = cfg.GetInt("T6Splash");
    FLD(int, 0x54c) = cfg.GetInt("T7Splash");
    FLD(int, 0x550) = cfg.GetInt("T8Splash");
    FLD(int, 0x554) = cfg.GetInt("T9Splash");
    FLD(int, 0x558) = cfg.GetInt("S0Splash");
    FLD(int, 0x55c) = cfg.GetInt("S1Splash");
    FLD(int, 0x560) = cfg.GetInt("S2Splash");
    FLD(int, 0x564) = cfg.GetInt("S3Splash");
    FLD(int, 0x568) = cfg.GetInt("S4Splash");
    FLD(int, 0x56c) = cfg.GetInt("R0Skidmark");
    FLD(int, 0x570) = cfg.GetInt("R1Skidmark");
    FLD(int, 0x574) = cfg.GetInt("R2Skidmark");
    FLD(int, 0x578) = cfg.GetInt("R3Skidmark");
    FLD(int, 0x57c) = cfg.GetInt("R4Skidmark");
    FLD(int, 0x580) = cfg.GetInt("T0Skidmark");
    FLD(int, 0x584) = cfg.GetInt("T1Skidmark");
    FLD(int, 0x588) = cfg.GetInt("T2Skidmark");
    FLD(int, 0x58c) = cfg.GetInt("T3Skidmark");
    FLD(int, 0x590) = cfg.GetInt("T4Skidmark");
    FLD(int, 0x594) = cfg.GetInt("T5Skidmark");
    FLD(int, 0x598) = cfg.GetInt("T6Skidmark");
    FLD(int, 0x59c) = cfg.GetInt("T7Skidmark");
    FLD(int, 0x5a0) = cfg.GetInt("T8Skidmark");
    FLD(int, 0x5a4) = cfg.GetInt("T9Skidmark");
    FLD(int, 0x5a8) = cfg.GetInt("S0Skidmark");
    FLD(int, 0x5ac) = cfg.GetInt("S1Skidmark");
    FLD(int, 0x5b0) = cfg.GetInt("S2Skidmark");
    FLD(int, 0x5b4) = cfg.GetInt("S3Skidmark");
    FLD(int, 0x5b8) = cfg.GetInt("S4Skidmark");
    FLD(int, 0x5bc) = cfg.GetInt("R0Effect1");
    FLD(int, 0x5c0) = cfg.GetInt("R1Effect1");
    FLD(int, 0x5c4) = cfg.GetInt("R2Effect1");
    FLD(int, 0x5c8) = cfg.GetInt("R3Effect1");
    FLD(int, 0x5cc) = cfg.GetInt("R4Effect1");
    FLD(int, 0x5d0) = cfg.GetInt("T0Effect1");
    FLD(int, 0x5d4) = cfg.GetInt("T1Effect1");
    FLD(int, 0x5d8) = cfg.GetInt("T2Effect1");
    FLD(int, 0x5dc) = cfg.GetInt("T3Effect1");
    FLD(int, 0x5e0) = cfg.GetInt("T4Effect1");
    FLD(int, 0x5e4) = cfg.GetInt("T5Effect1");
    FLD(int, 0x5e8) = cfg.GetInt("T6Effect1");
    FLD(int, 0x5ec) = cfg.GetInt("T7Effect1");
    FLD(int, 0x5f0) = cfg.GetInt("T8Effect1");
    FLD(int, 0x5f4) = cfg.GetInt("T9Effect1");
    FLD(int, 0x5f8) = cfg.GetInt("S0Effect1");
    FLD(int, 0x5fc) = cfg.GetInt("S1Effect1");
    FLD(int, 0x600) = cfg.GetInt("S2Effect1");
    FLD(int, 0x604) = cfg.GetInt("S3Effect1");
    FLD(int, 0x608) = cfg.GetInt("S4Effect1");
    FLD(int, 0x60c) = cfg.GetInt("R0Effect2");
    FLD(int, 0x610) = cfg.GetInt("R1Effect2");
    FLD(int, 0x614) = cfg.GetInt("R2Effect2");
    FLD(int, 0x618) = cfg.GetInt("R3Effect2");
    FLD(int, 0x61c) = cfg.GetInt("R4Effect2");
    FLD(int, 0x620) = cfg.GetInt("T0Effect2");
    FLD(int, 0x624) = cfg.GetInt("T1Effect2");
    FLD(int, 0x628) = cfg.GetInt("T2Effect2");
    FLD(int, 0x62c) = cfg.GetInt("T3Effect2");
    FLD(int, 0x630) = cfg.GetInt("T4Effect2");
    FLD(int, 0x634) = cfg.GetInt("T5Effect2");
    FLD(int, 0x638) = cfg.GetInt("T6Effect2");
    FLD(int, 0x63c) = cfg.GetInt("T7Effect2");
    FLD(int, 0x640) = cfg.GetInt("T8Effect2");
    FLD(int, 0x644) = cfg.GetInt("T9Effect2");
    FLD(int, 0x648) = cfg.GetInt("S0Effect2");
    FLD(int, 0x64c) = cfg.GetInt("S1Effect2");
    FLD(int, 0x650) = cfg.GetInt("S2Effect2");
    FLD(int, 0x654) = cfg.GetInt("S3Effect2");
    FLD(int, 0x658) = cfg.GetInt("S4Effect2");
    FLD(int, 0x33c) = cfg.GetInt("R0Reset");
    FLD(int, 0x340) = cfg.GetInt("R1Reset");
    FLD(int, 0x344) = cfg.GetInt("R2Reset");
    FLD(int, 0x348) = cfg.GetInt("R3Reset");
    FLD(int, 0x34c) = cfg.GetInt("R4Reset");
    FLD(int, 0x350) = cfg.GetInt("T0Reset");
    FLD(int, 0x354) = cfg.GetInt("T1Reset");
    FLD(int, 0x358) = cfg.GetInt("T2Reset");
    FLD(int, 0x35c) = cfg.GetInt("T3Reset");
    FLD(int, 0x360) = cfg.GetInt("T4Reset");
    FLD(int, 0x364) = cfg.GetInt("T5Reset");
    FLD(int, 0x368) = cfg.GetInt("T6Reset");
    FLD(int, 0x36c) = cfg.GetInt("T7Reset");
    FLD(int, 0x370) = cfg.GetInt("T8Reset");
    FLD(int, 0x374) = cfg.GetInt("T9Reset");
    FLD(int, 0x378) = cfg.GetInt("S0Reset");
    FLD(int, 0x37c) = cfg.GetInt("S1Reset");
    FLD(int, 0x380) = cfg.GetInt("S2Reset");
    FLD(int, 0x384) = cfg.GetInt("S3Reset");
    FLD(int, 0x388) = cfg.GetInt("S4Reset");
    CP2(0x38c, cfg.GetVector2("R0ForceRange"));
    CP2(0x394, cfg.GetVector2("R1ForceRange"));
    CP2(0x39c, cfg.GetVector2("R2ForceRange"));
    CP2(0x3a4, cfg.GetVector2("R3ForceRange"));
    CP2(0x3ac, cfg.GetVector2("R4ForceRange"));
    CP2(0x3b4, cfg.GetVector2("T0ForceRange"));
    CP2(0x3bc, cfg.GetVector2("T1ForceRange"));
    CP2(0x3c4, cfg.GetVector2("T2ForceRange"));
    CP2(0x3cc, cfg.GetVector2("T3ForceRange"));
    CP2(0x3d4, cfg.GetVector2("T4ForceRange"));
    CP2(0x3dc, cfg.GetVector2("T5ForceRange"));
    CP2(0x3e4, cfg.GetVector2("T6ForceRange"));
    CP2(0x3ec, cfg.GetVector2("T7ForceRange"));
    CP2(0x3f4, cfg.GetVector2("T8ForceRange"));
    CP2(0x3fc, cfg.GetVector2("T9ForceRange"));
    CP2(0x404, cfg.GetVector2("S0ForceRange"));
    CP2(0x40c, cfg.GetVector2("S1ForceRange"));
    CP2(0x414, cfg.GetVector2("S2ForceRange"));
    CP2(0x41c, cfg.GetVector2("S3ForceRange"));
    CP2(0x424, cfg.GetVector2("S4ForceRange"));
    SetSnd(FLD(std::string, 0x23c8), cfg.GetString("R0Sound"));
    SetSnd(FLD(std::string, 0x23d8), cfg.GetString("R1Sound"));
    SetSnd(FLD(std::string, 0x23e8), cfg.GetString("R2Sound"));
    SetSnd(FLD(std::string, 0x23f8), cfg.GetString("R3Sound"));
    SetSnd(FLD(std::string, 0x2408), cfg.GetString("R4Sound"));
    SetSnd(FLD(std::string, 0x2418), cfg.GetString("T0Sound"));
    SetSnd(FLD(std::string, 0x2428), cfg.GetString("T1Sound"));
    SetSnd(FLD(std::string, 0x2438), cfg.GetString("T2Sound"));
    SetSnd(FLD(std::string, 0x2448), cfg.GetString("T3Sound"));
    SetSnd(FLD(std::string, 0x2458), cfg.GetString("T4Sound"));
    SetSnd(FLD(std::string, 0x2468), cfg.GetString("T5Sound"));
    SetSnd(FLD(std::string, 0x2478), cfg.GetString("T6Sound"));
    ((XS&)FLD(std::string, 0x2488)).Asg(cfg.GetString("T7Sound"));
    ((XS&)FLD(std::string, 0x2498)).Asg(cfg.GetString("T8Sound"));
    ((XS&)FLD(std::string, 0x24a8)).Asg(cfg.GetString("T9Sound"));
    cfg.Close();
    char buf[32];
    itoa(FLD(int, 4), buf, 10);
    std::string name(FLD(std::string, 8));
    AppS(name, "data/stagetimes");
    AppS(name, buf);
    AppS(name, ".ini");
    cfg.Define(DAT_004f3a80, 0x1a);
    cfg.Load(name.c_str(), true, false, 0);
    int level = DAT_0050132c;
    if (DAT_00501464 == 3 || DAT_00501454 != 0)
        level = 1;
    int& i = *(int*)&path;
    if (DAT_00501460) {
        switch (level) {
        case 0: CP3(0x24d8, cfg.GetVector3("CommentTimesFTCNovice")); break;
        case 2: CP3(0x24d8, cfg.GetVector3("CommentTimesFTCExpert")); break;
        default: CP3(0x24d8, cfg.GetVector3("CommentTimesFTCIntermediate")); break;
        }
        int* p = &FLD(int, 0x250c);
        for (i = 0; i < 10; i++) {
            DAT_004f3bc8[11] = (char)(i + '0');
            ConfigVector3 v; v = cfg.GetVector3("CPUFTCSplit0");
            switch (level) {
            case 0:
                p[-10] = RoundF(v.values[0] * 1000.0f);
                { float q = DAT_0050150c * v.values[0]; q = q * 1000.0f; p[0] = RoundF(q); }
                break;
            case 2:
                p[-10] = RoundF(v.values[2] * 1000.0f);
                { float q = DAT_00501514 * v.values[2]; q = q * 1000.0f; p[0] = RoundF(q); }
                break;
            default:
                p[-10] = RoundF(v.values[1] * 1000.0f);
                { float q = v.values[1] * DAT_00501510; q = q * 1000.0f; p[0] = RoundF(q); }
                break;
            }
            p++;
        }
    } else {
        switch (level) {
        case 0: CP3(0x24d8, cfg.GetVector3("CommentTimesNovice")); break;
        case 2: CP3(0x24d8, cfg.GetVector3("CommentTimesExpert")); break;
        default: CP3(0x24d8, cfg.GetVector3("CommentTimesIntermediate")); break;
        }
        int* p = &FLD(int, 0x250c);
        for (i = 0; i < 10; i++) {
            DAT_004f3bd8[8] = (char)(i + '0');
            ConfigVector3 v; v = cfg.GetVector3("CPUSplit0");
            switch (level) {
            case 0:
                p[-10] = RoundF(v.values[0] * 1000.0f);
                { float q = v.values[0] * DAT_0050150c; q = q * 1000.0f; p[0] = RoundF(q); }
                break;
            case 2:
                p[-10] = RoundF(v.values[2] * 1000.0f);
                { float q = v.values[2] * DAT_00501514; q = q * 1000.0f; p[0] = RoundF(q); }
                break;
            default:
                p[-10] = RoundF(v.values[1] * 1000.0f);
                { float q = v.values[1] * DAT_00501510; q = q * 1000.0f; p[0] = RoundF(q); }
                break;
            }
            p++;
        }
    }
    cfg.Close();
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
    Bal(1);
}
