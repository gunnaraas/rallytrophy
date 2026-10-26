// StageScene::LoadEffect (0x410720): picks the river/waterfall/water material effect for a mesh by name.
#include <windows.h>
#include <string.h>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_476490.h"
#include "unk_46fa80.h"
#include "unk_474c50.h"
#include "unk_40c830.h"
#include "unk_410720.h"

extern ConfigKey DAT_004f3b50[];   // Fx1UVScale, Fx1UVSpeed, Fx2UVScale, Fx2UVSpeed (4)
extern ConfigKey DAT_004f3b70[];   // Fx1UVScale, Fx1UVSpeed (2)
extern ConfigKey DAT_004f3b80[];   // FxAnimSpeed, FxUVScale, Fx1UVSpeed, Fx2UVSpeed (4)
extern char DAT_004e1344[];
extern "C" int __cdecl FUN_004ce290(const char* a, const char* b);   // case-insensitive compare

#define LOADER ((TextureLoader*)((BYTE*)this + 0xd4))
#define AT(T, o) (*(T*)((BYTE*)this + (o)))

// original: FUN_00410720
// NONMATCHING: RALLYTROPHY 0x410720
// Register allocation/frame differs (original keeps the effect kind in edi and reloads p from the stack; ours pins p in edi, kind spills); 2113 vs 2128 bytes.
void StageScene::LoadEffect(int* p)
{
    int* m = (int*)p[0];                               // the mesh's material
    const char* volatile* namep = (const char* volatile*)((BYTE*)m + 0xa8);
    const char* name = *namep;
    volatile int kindv = m[0x2f];
    if (name == 0) name = DAT_004e1344;
    if (FUN_004ce290("waterfall_1", name) == 0) {
        kindv = 2;
    } else {
        name = *namep;
        if (name == 0) name = DAT_004e1344;
        if (FUN_004ce290("waterfall_2", name) == 0) {
            kindv = 3;
        } else {
            name = *namep;
            if (name == 0) name = DAT_004e1344;
            if (FUN_004ce290("water", name) == 0)
                kindv = 6;
        }
    }

    int kind = kindv;
    if (kind == 2 || kind == 3) {
        if (AT(int, 0x85c) == 0) {
            AT(Texture*, 0x860) = LOADER->Load("data/global/effects/waterfall_1.tga", 0x40);
            AT(Texture*, 0x864) = LOADER->Load("data/global/effects/waterfall_2.tga", 0x40);
            AT(int, 0x85c) = 1;
            Config cfg;
            cfg.Define(DAT_004f3b50, 4);
            cfg.Load("data/global/effects/waterfall.ini", true, false, 0);
            DAT_00508438 = *(EffectUV*)&cfg.GetVector2("Fx1UVScale");
            DAT_00508430 = *(EffectUV*)&cfg.GetVector2("Fx2UVScale");
            DAT_00508428 = *(EffectUV*)&cfg.GetVector2("Fx1UVSpeed");
            DAT_00508420 = *(EffectUV*)&cfg.GetVector2("Fx2UVSpeed");
        }
        m[0x18] = 0x3f800000; m[0x19] = 0x3f800000; m[0x1a] = 0x3f800000; m[0x1b] = 0x3f800000;
        m[0x26] = 0xf; m[0x1c] = 0; m[0x1d] = 0; m[0xe] = 0xf;
        m[0x1e] = 0; m[0x1f] = 0; m[0x10] = 0; m[0x11] = 0;
        m[0x27] = 2; m[6] = 5; m[7] = 6; m[9] = 4; m[0x28] = 0; m[0x12] = 0; m[0x13] = 0x3f000000;
        m[0] = AT(int, 0x860);
        m[1] = AT(int, 0x864);
        float sx, sy, vx, vy;
        if (kind == 2) {
            sx = DAT_00508438.x; sy = DAT_00508438.y;
            vx = DAT_00508428.x; vy = DAT_00508428.y;
        } else {
            sx = DAT_00508430.x; sy = DAT_00508430.y;
            vx = DAT_00508420.x; vy = DAT_00508420.y;
        }
        TexScroller* s = new TexScroller((DynamicMesh*)p, sx, sy, vx, vy);
        p[5] = (int)s;
    } else if (kind == 4) {
        if (AT(int, 0x868) == 0) {
            AT(Texture*, 0x86c) = LOADER->Load("data/global/effects/river1_1.tga", 0x40);
            AT(Texture*, 0x870) = LOADER->Load("data/global/effects/river1_2.tga", 0x40);
            AT(int, 0x868) = 1;
            Config cfg;
            cfg.Define(DAT_004f3b70, 2);
            cfg.Load("data/global/effects/river1.ini", true, false, 0);
            DAT_00508418 = *(EffectUV*)&cfg.GetVector2("Fx1UVScale");
            DAT_00508410 = *(EffectUV*)&cfg.GetVector2("Fx1UVSpeed");
        }
        m[0x18] = 0x3f800000; m[0x19] = 0x3f800000; m[0x1a] = 0x3f800000; m[0x1b] = 0x3f800000;
        m[0x26] = 0xf; m[0xe] = 0xf;
        m[0x1c] = 0; m[0x1d] = 0; m[0x10] = 0; m[0x11] = 0;
        m[0] = AT(int, 0x86c);
        m[1] = AT(int, 0x870);
        m[0x1e] = 0; m[0x1f] = 0;
        m[0x27] = 2; m[6] = 5; m[7] = 6; m[9] = 4; m[0x28] = 0; m[0x12] = 0; m[0x13] = 0x3f000000;
        float sx, sy, vx, vy;
        sx = DAT_00508418.x; sy = DAT_00508418.y; vx = DAT_00508410.x; vy = DAT_00508410.y;
        TexScroller* s = new TexScroller((DynamicMesh*)p, sx, sy, vx, vy);
        p[5] = (int)s;
    } else if (kind == 5) {
        if (AT(int, 0x874) == 0) {
            AT(Texture*, 0x878) = LOADER->Load("data/global/effects/river2_1.tga", 0x40);
            AT(Texture*, 0x87c) = LOADER->Load("data/global/effects/river2_2.tga", 0x40);
            AT(int, 0x874) = 1;
            Config cfg;
            cfg.Define(DAT_004f3b70, 2);
            cfg.Load("data/global/effects/river2.ini", true, false, 0);
            DAT_00508408 = *(EffectUV*)&cfg.GetVector2("Fx1UVScale");
            DAT_00508400 = *(EffectUV*)&cfg.GetVector2("Fx1UVSpeed");
        }
        m[0x18] = 0x3f800000; m[0x19] = 0x3f800000; m[0x1a] = 0x3f800000; m[0x1b] = 0x3f800000;
        m[0x26] = 0xf; m[0xe] = 0xf;
        m[0x1c] = 0; m[0x1d] = 0; m[0x10] = 0; m[0x11] = 0;
        m[0] = AT(int, 0x878);
        m[1] = AT(int, 0x87c);
        m[0x1e] = 0; m[0x1f] = 0;
        m[0x27] = 2; m[6] = 5; m[7] = 6; m[9] = 4; m[0x28] = 0; m[0x12] = 0; m[0x13] = 0x3f000000;
        float sx, sy, vx, vy;
        sx = DAT_00508408.x; sy = DAT_00508408.y; vx = DAT_00508400.x; vy = DAT_00508400.y;
        TexScroller* s = new TexScroller((DynamicMesh*)p, sx, sy, vx, vy);
        p[5] = (int)s;
    } else if (kind == 6) {
        if (AT(int, 0x880) == 0) {
            int len = strlen(DAT_004f3ba0);
            AT(int, 0x880) = 1;
            Texture** slot = (Texture**)((BYTE*)this + 0x884);
            for (int i = 0; i < 0x20; i++) {
                DAT_004f3ba0[len - 6] = (char)(i / 10) + '0';
                DAT_004f3ba0[len - 5] = (char)(i % 10) + '0';
                *slot = LOADER->Load(DAT_004f3ba0, 0x40);
                slot++;
            }
            AT(int, 0x904) = AT(int, 0x884);
            Config cfg;
            cfg.Define(DAT_004f3b80, 4);
            cfg.Load("data/global/effects/water.ini", true, false, 0);
            DAT_005083f8 = *(EffectUV*)&cfg.GetVector2("FxUVScale");
            DAT_005083f0 = *(EffectUV*)&cfg.GetVector2("Fx1UVSpeed");
            DAT_005083e8 = *(EffectUV*)&cfg.GetVector2("Fx2UVSpeed");
            DAT_005083e0 = cfg.GetFloat("FxAnimSpeed");
        }
        m[0x19] = 0x3f800000; m[0x18] = 0x3f800000; m[0x1a] = 0x3f800000; m[0x1b] = 0x3f800000;
        m[0x1d] = 0; m[0x1c] = 0; m[0x1e] = 0; m[0x1f] = 0;
        m[0x11] = 0x3f800000; m[0x10] = 0x3f800000; m[0x12] = 0x3f800000; m[0x13] = 0x3f800000;
        m[0x26] = 0xf; m[0x27] = 2; m[6] = 5; m[7] = 6; m[9] = 4; m[0x28] = 0; m[0xe] = 0x18;
        m[1] = AT(int, 0x904);
        WaterAnimator* w = new WaterAnimator((DynamicMesh*)p, (WaterOwner*)this, DAT_005083e0,
                                             (Vec2*)&DAT_005083f8, (Vec2*)&DAT_005083f0, (Vec2*)&DAT_005083e8);
        p[5] = (int)w;
    }
}
