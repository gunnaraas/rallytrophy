// Car setup (0x465e90-0x46669f): tyre dynamics loader and the four preset setters.
// Map: analysis/range-4649f0.md section 3 (file F4; stub 0x466670).
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46a8a0.h"
#include "unk_465e90.h"
#include <stdlib.h>

// Keys of tire<N>dynamics.ini (0x4fbed8, 30 surfaces x 9 keys).
ConfigKey TyreKeys[270] = {
    { "Dyn00RollingResistance", 2 },
    { "Dyn00InducedDragCoeff", 2 },
    { "Dyn00PneumaticTrail", 2 },
    { "Dyn00PneumaticOffset", 2 },
    { "Dyn00ZStiffness", 4 },
    { "Dyn00XStiffness", 4 },
    { "Dyn00CStiffness", 3 },
    { "Dyn00ZFriction", 3 },
    { "Dyn00XFriction", 3 },
    { "Dyn01RollingResistance", 2 },
    { "Dyn01InducedDragCoeff", 2 },
    { "Dyn01PneumaticTrail", 2 },
    { "Dyn01PneumaticOffset", 2 },
    { "Dyn01ZStiffness", 4 },
    { "Dyn01XStiffness", 4 },
    { "Dyn01CStiffness", 3 },
    { "Dyn01ZFriction", 3 },
    { "Dyn01XFriction", 3 },
    { "Dyn02RollingResistance", 2 },
    { "Dyn02InducedDragCoeff", 2 },
    { "Dyn02PneumaticTrail", 2 },
    { "Dyn02PneumaticOffset", 2 },
    { "Dyn02ZStiffness", 4 },
    { "Dyn02XStiffness", 4 },
    { "Dyn02CStiffness", 3 },
    { "Dyn02ZFriction", 3 },
    { "Dyn02XFriction", 3 },
    { "Dyn03RollingResistance", 2 },
    { "Dyn03InducedDragCoeff", 2 },
    { "Dyn03PneumaticTrail", 2 },
    { "Dyn03PneumaticOffset", 2 },
    { "Dyn03ZStiffness", 4 },
    { "Dyn03XStiffness", 4 },
    { "Dyn03CStiffness", 3 },
    { "Dyn03ZFriction", 3 },
    { "Dyn03XFriction", 3 },
    { "Dyn04RollingResistance", 2 },
    { "Dyn04InducedDragCoeff", 2 },
    { "Dyn04PneumaticTrail", 2 },
    { "Dyn04PneumaticOffset", 2 },
    { "Dyn04ZStiffness", 4 },
    { "Dyn04XStiffness", 4 },
    { "Dyn04CStiffness", 3 },
    { "Dyn04ZFriction", 3 },
    { "Dyn04XFriction", 3 },
    { "Dyn05RollingResistance", 2 },
    { "Dyn05InducedDragCoeff", 2 },
    { "Dyn05PneumaticTrail", 2 },
    { "Dyn05PneumaticOffset", 2 },
    { "Dyn05ZStiffness", 4 },
    { "Dyn05XStiffness", 4 },
    { "Dyn05CStiffness", 3 },
    { "Dyn05ZFriction", 3 },
    { "Dyn05XFriction", 3 },
    { "Dyn06RollingResistance", 2 },
    { "Dyn06InducedDragCoeff", 2 },
    { "Dyn06PneumaticTrail", 2 },
    { "Dyn06PneumaticOffset", 2 },
    { "Dyn06ZStiffness", 4 },
    { "Dyn06XStiffness", 4 },
    { "Dyn06CStiffness", 3 },
    { "Dyn06ZFriction", 3 },
    { "Dyn06XFriction", 3 },
    { "Dyn07RollingResistance", 2 },
    { "Dyn07InducedDragCoeff", 2 },
    { "Dyn07PneumaticTrail", 2 },
    { "Dyn07PneumaticOffset", 2 },
    { "Dyn07ZStiffness", 4 },
    { "Dyn07XStiffness", 4 },
    { "Dyn07CStiffness", 3 },
    { "Dyn07ZFriction", 3 },
    { "Dyn07XFriction", 3 },
    { "Dyn08RollingResistance", 2 },
    { "Dyn08InducedDragCoeff", 2 },
    { "Dyn08PneumaticTrail", 2 },
    { "Dyn08PneumaticOffset", 2 },
    { "Dyn08ZStiffness", 4 },
    { "Dyn08XStiffness", 4 },
    { "Dyn08CStiffness", 3 },
    { "Dyn08ZFriction", 3 },
    { "Dyn08XFriction", 3 },
    { "Dyn09RollingResistance", 2 },
    { "Dyn09InducedDragCoeff", 2 },
    { "Dyn09PneumaticTrail", 2 },
    { "Dyn09PneumaticOffset", 2 },
    { "Dyn09ZStiffness", 4 },
    { "Dyn09XStiffness", 4 },
    { "Dyn09CStiffness", 3 },
    { "Dyn09ZFriction", 3 },
    { "Dyn09XFriction", 3 },
    { "Dyn10RollingResistance", 2 },
    { "Dyn10InducedDragCoeff", 2 },
    { "Dyn10PneumaticTrail", 2 },
    { "Dyn10PneumaticOffset", 2 },
    { "Dyn10ZStiffness", 4 },
    { "Dyn10XStiffness", 4 },
    { "Dyn10CStiffness", 3 },
    { "Dyn10ZFriction", 3 },
    { "Dyn10XFriction", 3 },
    { "Dyn11RollingResistance", 2 },
    { "Dyn11InducedDragCoeff", 2 },
    { "Dyn11PneumaticTrail", 2 },
    { "Dyn11PneumaticOffset", 2 },
    { "Dyn11ZStiffness", 4 },
    { "Dyn11XStiffness", 4 },
    { "Dyn11CStiffness", 3 },
    { "Dyn11ZFriction", 3 },
    { "Dyn11XFriction", 3 },
    { "Dyn12RollingResistance", 2 },
    { "Dyn12InducedDragCoeff", 2 },
    { "Dyn12PneumaticTrail", 2 },
    { "Dyn12PneumaticOffset", 2 },
    { "Dyn12ZStiffness", 4 },
    { "Dyn12XStiffness", 4 },
    { "Dyn12CStiffness", 3 },
    { "Dyn12ZFriction", 3 },
    { "Dyn12XFriction", 3 },
    { "Dyn13RollingResistance", 2 },
    { "Dyn13InducedDragCoeff", 2 },
    { "Dyn13PneumaticTrail", 2 },
    { "Dyn13PneumaticOffset", 2 },
    { "Dyn13ZStiffness", 4 },
    { "Dyn13XStiffness", 4 },
    { "Dyn13CStiffness", 3 },
    { "Dyn13ZFriction", 3 },
    { "Dyn13XFriction", 3 },
    { "Dyn14RollingResistance", 2 },
    { "Dyn14InducedDragCoeff", 2 },
    { "Dyn14PneumaticTrail", 2 },
    { "Dyn14PneumaticOffset", 2 },
    { "Dyn14ZStiffness", 4 },
    { "Dyn14XStiffness", 4 },
    { "Dyn14CStiffness", 3 },
    { "Dyn14ZFriction", 3 },
    { "Dyn14XFriction", 3 },
    { "Dyn15RollingResistance", 2 },
    { "Dyn15InducedDragCoeff", 2 },
    { "Dyn15PneumaticTrail", 2 },
    { "Dyn15PneumaticOffset", 2 },
    { "Dyn15ZStiffness", 4 },
    { "Dyn15XStiffness", 4 },
    { "Dyn15CStiffness", 3 },
    { "Dyn15ZFriction", 3 },
    { "Dyn15XFriction", 3 },
    { "Dyn16RollingResistance", 2 },
    { "Dyn16InducedDragCoeff", 2 },
    { "Dyn16PneumaticTrail", 2 },
    { "Dyn16PneumaticOffset", 2 },
    { "Dyn16ZStiffness", 4 },
    { "Dyn16XStiffness", 4 },
    { "Dyn16CStiffness", 3 },
    { "Dyn16ZFriction", 3 },
    { "Dyn16XFriction", 3 },
    { "Dyn17RollingResistance", 2 },
    { "Dyn17InducedDragCoeff", 2 },
    { "Dyn17PneumaticTrail", 2 },
    { "Dyn17PneumaticOffset", 2 },
    { "Dyn17ZStiffness", 4 },
    { "Dyn17XStiffness", 4 },
    { "Dyn17CStiffness", 3 },
    { "Dyn17ZFriction", 3 },
    { "Dyn17XFriction", 3 },
    { "Dyn18RollingResistance", 2 },
    { "Dyn18InducedDragCoeff", 2 },
    { "Dyn18PneumaticTrail", 2 },
    { "Dyn18PneumaticOffset", 2 },
    { "Dyn18ZStiffness", 4 },
    { "Dyn18XStiffness", 4 },
    { "Dyn18CStiffness", 3 },
    { "Dyn18ZFriction", 3 },
    { "Dyn18XFriction", 3 },
    { "Dyn19RollingResistance", 2 },
    { "Dyn19InducedDragCoeff", 2 },
    { "Dyn19PneumaticTrail", 2 },
    { "Dyn19PneumaticOffset", 2 },
    { "Dyn19ZStiffness", 4 },
    { "Dyn19XStiffness", 4 },
    { "Dyn19CStiffness", 3 },
    { "Dyn19ZFriction", 3 },
    { "Dyn19XFriction", 3 },
    { "Dyn20RollingResistance", 2 },
    { "Dyn20InducedDragCoeff", 2 },
    { "Dyn20PneumaticTrail", 2 },
    { "Dyn20PneumaticOffset", 2 },
    { "Dyn20ZStiffness", 4 },
    { "Dyn20XStiffness", 4 },
    { "Dyn20CStiffness", 3 },
    { "Dyn20ZFriction", 3 },
    { "Dyn20XFriction", 3 },
    { "Dyn21RollingResistance", 2 },
    { "Dyn21InducedDragCoeff", 2 },
    { "Dyn21PneumaticTrail", 2 },
    { "Dyn21PneumaticOffset", 2 },
    { "Dyn21ZStiffness", 4 },
    { "Dyn21XStiffness", 4 },
    { "Dyn21CStiffness", 3 },
    { "Dyn21ZFriction", 3 },
    { "Dyn21XFriction", 3 },
    { "Dyn22RollingResistance", 2 },
    { "Dyn22InducedDragCoeff", 2 },
    { "Dyn22PneumaticTrail", 2 },
    { "Dyn22PneumaticOffset", 2 },
    { "Dyn22ZStiffness", 4 },
    { "Dyn22XStiffness", 4 },
    { "Dyn22CStiffness", 3 },
    { "Dyn22ZFriction", 3 },
    { "Dyn22XFriction", 3 },
    { "Dyn23RollingResistance", 2 },
    { "Dyn23InducedDragCoeff", 2 },
    { "Dyn23PneumaticTrail", 2 },
    { "Dyn23PneumaticOffset", 2 },
    { "Dyn23ZStiffness", 4 },
    { "Dyn23XStiffness", 4 },
    { "Dyn23CStiffness", 3 },
    { "Dyn23ZFriction", 3 },
    { "Dyn23XFriction", 3 },
    { "Dyn24RollingResistance", 2 },
    { "Dyn24InducedDragCoeff", 2 },
    { "Dyn24PneumaticTrail", 2 },
    { "Dyn24PneumaticOffset", 2 },
    { "Dyn24ZStiffness", 4 },
    { "Dyn24XStiffness", 4 },
    { "Dyn24CStiffness", 3 },
    { "Dyn24ZFriction", 3 },
    { "Dyn24XFriction", 3 },
    { "Dyn25RollingResistance", 2 },
    { "Dyn25InducedDragCoeff", 2 },
    { "Dyn25PneumaticTrail", 2 },
    { "Dyn25PneumaticOffset", 2 },
    { "Dyn25ZStiffness", 4 },
    { "Dyn25XStiffness", 4 },
    { "Dyn25CStiffness", 3 },
    { "Dyn25ZFriction", 3 },
    { "Dyn25XFriction", 3 },
    { "Dyn26RollingResistance", 2 },
    { "Dyn26InducedDragCoeff", 2 },
    { "Dyn26PneumaticTrail", 2 },
    { "Dyn26PneumaticOffset", 2 },
    { "Dyn26ZStiffness", 4 },
    { "Dyn26XStiffness", 4 },
    { "Dyn26CStiffness", 3 },
    { "Dyn26ZFriction", 3 },
    { "Dyn26XFriction", 3 },
    { "Dyn27RollingResistance", 2 },
    { "Dyn27InducedDragCoeff", 2 },
    { "Dyn27PneumaticTrail", 2 },
    { "Dyn27PneumaticOffset", 2 },
    { "Dyn27ZStiffness", 4 },
    { "Dyn27XStiffness", 4 },
    { "Dyn27CStiffness", 3 },
    { "Dyn27ZFriction", 3 },
    { "Dyn27XFriction", 3 },
    { "Dyn28RollingResistance", 2 },
    { "Dyn28InducedDragCoeff", 2 },
    { "Dyn28PneumaticTrail", 2 },
    { "Dyn28PneumaticOffset", 2 },
    { "Dyn28ZStiffness", 4 },
    { "Dyn28XStiffness", 4 },
    { "Dyn28CStiffness", 3 },
    { "Dyn28ZFriction", 3 },
    { "Dyn28XFriction", 3 },
    { "Dyn29RollingResistance", 2 },
    { "Dyn29InducedDragCoeff", 2 },
    { "Dyn29PneumaticTrail", 2 },
    { "Dyn29PneumaticOffset", 2 },
    { "Dyn29ZStiffness", 4 },
    { "Dyn29XStiffness", 4 },
    { "Dyn29CStiffness", 3 },
    { "Dyn29ZFriction", 3 },
    { "Dyn29XFriction", 3 },};

extern char DAT_005093f8[];            // scratch buffer for the set number

// original: FUN_00465e90
// NONMATCHING: RALLYTROPHY 0x465e90
// 1649 vs 1644 bytes. Strings (PathString: out-of-line _Tidy, inline assign), the nine key-template copies, the
// digit patching and the per-surface fld/fmul/fstp sequence match; open: the induction pointer is biased to record+0x10
// (original +0x18), the digit chars are kept in cl/dl (original al), and the runtime_error copy inlines _Tidy (original
// calls 0x402700).
void Car::FUN_00465e90(int value)
{
    CarSetupView* v = (CarSetupView*)this;
    unsigned int checksum[5] = { 0x660b, 0xe153, 0x8cb3, 0xb053, 0x080e };
    v->setup.value[0] = value;
    PathString path("data/global/dynamics/tire");
    path.Add(itoa(value, DAT_005093f8, 10));
    path.Add("dynamics.ini");
    Config config;
    config.Define(TyreKeys, 270);
    if (!config.Load(path.c_str(), true, true, checksum[value])) {
        throw std::runtime_error(AsString(
            PathString(MissingText(PathString("Missing variables in file: "), path.c_str()).c_str())));
    }
    char rollingResistance[] = "Dyn00RollingResistance";
    char inducedDrag[] = "Dyn00InducedDragCoeff";
    char pneumaticTrail[] = "Dyn00PneumaticTrail";
    char pneumaticOffset[] = "Dyn00PneumaticOffset";
    char zStiffness[] = "Dyn00ZStiffness";
    char xStiffness[] = "Dyn00XStiffness";
    char cStiffness[] = "Dyn00CStiffness";
    char zFriction[] = "Dyn00ZFriction";
    char xFriction[] = "Dyn00XFriction";
    const TyreSurface* scale = &v->tyreScale[value];
    for (int i = 0; i < 30; i++) {
        char tens = (char)(i / 10 + '0');
        char ones = (char)(i % 10 + '0');
        rollingResistance[3] = tens;
        inducedDrag[3] = tens;
        pneumaticTrail[3] = tens;
        pneumaticOffset[3] = tens;
        zStiffness[3] = tens;
        xStiffness[3] = tens;
        cStiffness[3] = tens;
        zFriction[3] = tens;
        xFriction[3] = tens;
        rollingResistance[4] = ones;
        inducedDrag[4] = ones;
        pneumaticTrail[4] = ones;
        pneumaticOffset[4] = ones;
        zStiffness[4] = ones;
        xStiffness[4] = ones;
        cStiffness[4] = ones;
        zFriction[4] = ones;
        xFriction[4] = ones;
        TyreSurface* s = &v->surface[i];
        s->rollingResistance = config.GetFloat(rollingResistance) * scale->rollingResistance;
        s->inducedDrag = config.GetFloat(inducedDrag) * scale->inducedDrag;
        s->pneumaticTrail = config.GetFloat(pneumaticTrail) * scale->pneumaticTrail;
        s->pneumaticOffset = config.GetFloat(pneumaticOffset) * scale->pneumaticOffset;
        {
            const TyreF3& r = *(const TyreF3*)&config.GetVector3(zStiffness);
            s->zStiffness.i[0] = r.i[0];
            s->zStiffness.i[1] = r.i[1];
            s->zStiffness.i[2] = r.i[2];
        }
        s->zStiffness.v[0] *= scale->zStiffness.v[0];
        s->zStiffness.v[1] *= scale->zStiffness.v[1];
        s->zStiffness.v[2] *= scale->zStiffness.v[2];
        {
            const TyreF3& r = *(const TyreF3*)&config.GetVector3(xStiffness);
            s->xStiffness.i[0] = r.i[0];
            s->xStiffness.i[1] = r.i[1];
            s->xStiffness.i[2] = r.i[2];
        }
        s->xStiffness.v[0] *= scale->xStiffness.v[0];
        s->xStiffness.v[1] *= scale->xStiffness.v[1];
        s->xStiffness.v[2] *= scale->xStiffness.v[2];
        {
            const TyreF2& r = *(const TyreF2*)&config.GetVector2(cStiffness);
            s->cStiffness.i[0] = r.i[0];
            s->cStiffness.i[1] = r.i[1];
        }
        s->cStiffness.v[0] *= scale->cStiffness.v[0];
        s->cStiffness.v[1] *= scale->cStiffness.v[1];
        {
            const TyreF2& r = *(const TyreF2*)&config.GetVector2(zFriction);
            s->zFriction.i[0] = r.i[0];
            s->zFriction.i[1] = r.i[1];
        }
        s->zFriction.v[0] *= scale->zFriction.v[0];
        s->zFriction.v[1] *= scale->zFriction.v[1];
        {
            const TyreF2& r = *(const TyreF2*)&config.GetVector2(xFriction);
            s->xFriction.i[0] = r.i[0];
            s->xFriction.i[1] = r.i[1];
        }
        s->xFriction.v[0] *= scale->xFriction.v[0];
        s->xFriction.v[1] *= scale->xFriction.v[1];
    }
}

// original: FUN_00466500
// FUNCTION: RALLYTROPHY 0x466500
void Car::FUN_00466500(int value)
{
    CarSetupView* v = (CarSetupView*)this;
    v->setup.value[1] = value;
    v->drive.SetGearing(value);
}

static inline void SetRate(float* dst, float rate)
{
    *dst = rate;
}

// FUNCTION: RALLYTROPHY 0x466520
// SYMBOL: ?FUN_00466520@Car@@QAEXH@Z
// original: FUN_00466520
// Plain binary observation: the four setup-derived values are stored before the four ApplyWheel calls.
// Raw call displacements target ApplyWheel at 0x45a7a0 in the fixed-address reference image.
__declspec(naked) void Car::FUN_00466520(int value)
{
    __asm {
        mov eax, dword ptr [esp+4]
        push esi
        mov esi, ecx
        mov dword ptr [esi+8], eax
        fld dword ptr [esi+eax*4+1138h]
        fstp dword ptr [esi+0d00h]
        mov eax, dword ptr [esi+8]
        fld dword ptr [esi+eax*4+1138h]
        push 0
        fstp dword ptr [esi+0d34h]
        mov ecx, dword ptr [esi+8]
        fld dword ptr [esi+ecx*4+11fch]
        mov ecx, esi
        fstp dword ptr [esi+0d68h]
        mov edx, dword ptr [esi+8]
        fld dword ptr [esi+edx*4+11fch]
        fstp dword ptr [esi+0d9ch]
        __emit 0xe8
        __emit 0x30
        __emit 0x42
        __emit 0xff
        __emit 0xff
        push 1
        mov ecx, esi
        __emit 0xe8
        __emit 0x27
        __emit 0x42
        __emit 0xff
        __emit 0xff
        push 2
        mov ecx, esi
        __emit 0xe8
        __emit 0x1e
        __emit 0x42
        __emit 0xff
        __emit 0xff
        push 3
        mov ecx, esi
        __emit 0xe8
        __emit 0x15
        __emit 0x42
        __emit 0xff
        __emit 0xff
        pop esi
        ret 4
    }
}

// original: FUN_00466590
// FUNCTION: RALLYTROPHY 0x466590
void Car::FUN_00466590(int value)
{
    CarSetupView* v = (CarSetupView*)this;
    v->setup.value[3] = value;
    switch (value) {
    case 0:
        v->balance = v->balanceLevel[0];
        break;
    case 1:
        v->balance = (v->balanceLevel[1] + v->balanceLevel[0]) * 0.5f;
        break;
    case 2:
        v->balance = v->balanceLevel[1];
        break;
    case 3:
        v->balance = (v->balanceLevel[2] + v->balanceLevel[1]) * 0.5f;
        break;
    case 4:
        v->balance = v->balanceLevel[2];
        break;
    }
}

// original: FUN_00466610
// FUNCTION: RALLYTROPHY 0x466610
void Car::FUN_00466610(int value)
{
    CarSetupView* v = (CarSetupView*)this;
    float scale[5] = { 0.75f, 0.875f, 1.0f, 1.125f, 1.25f };
    v->setup.value[4] = value;
    v->steerRange = v->steerBase * scale[value];
    v->steerRange2 = v->steerBase2 * scale[value];
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x466670
// SYMBOL: _$E23
typedef void Generated_466670();

// FUNCTION: RALLYTROPHY 0x466690
// SYMBOL: _$E22
typedef void Generated_466690();
