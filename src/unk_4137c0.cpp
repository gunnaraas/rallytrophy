// Race scene setup (0x4137c0). Map: analysis/range-409690.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_4758b0.h"
#include "unk_4666a0.h"
#include <string.h>
#include <memory>
// unk_474c50.h redeclares TextureLoader (same layout as unk_4758b0.h); rename its copy.
#define TextureLoader TextureLoader474
#include "unk_474c50.h"
#include "unk_472c90.h"
#undef TextureLoader
#include "unk_4137c0.h"
#include <stdlib.h>
#include <string.h>
#include <stdexcept>

// Keys of the flare ini (0x4f3000, 51 entries).
ConfigKey FlareKeys[51] = {
    { "GlowMap", 0 },
    { "GlowSize", 2 },
    { "NumberOfRings", 1 },
    { "Ring00Map", 0 },
    { "Ring00SizeSharpness", 3 },
    { "Ring00ScaleLocRot", 4 },
    { "Ring01Map", 0 },
    { "Ring01SizeSharpness", 3 },
    { "Ring01ScaleLocRot", 4 },
    { "Ring02Map", 0 },
    { "Ring02SizeSharpness", 3 },
    { "Ring02ScaleLocRot", 4 },
    { "Ring03Map", 0 },
    { "Ring03SizeSharpness", 3 },
    { "Ring03ScaleLocRot", 4 },
    { "Ring04Map", 0 },
    { "Ring04SizeSharpness", 3 },
    { "Ring04ScaleLocRot", 4 },
    { "Ring05Map", 0 },
    { "Ring05SizeSharpness", 3 },
    { "Ring05ScaleLocRot", 4 },
    { "Ring06Map", 0 },
    { "Ring06SizeSharpness", 3 },
    { "Ring06ScaleLocRot", 4 },
    { "Ring07Map", 0 },
    { "Ring07SizeSharpness", 3 },
    { "Ring07ScaleLocRot", 4 },
    { "Ring08Map", 0 },
    { "Ring08SizeSharpness", 3 },
    { "Ring08ScaleLocRot", 4 },
    { "Ring09Map", 0 },
    { "Ring09SizeSharpness", 3 },
    { "Ring09ScaleLocRot", 4 },
    { "Ring10Map", 0 },
    { "Ring10SizeSharpness", 3 },
    { "Ring10ScaleLocRot", 4 },
    { "Ring11Map", 0 },
    { "Ring11SizeSharpness", 3 },
    { "Ring11ScaleLocRot", 4 },
    { "Ring12Map", 0 },
    { "Ring12SizeSharpness", 3 },
    { "Ring12ScaleLocRot", 4 },
    { "Ring13Map", 0 },
    { "Ring13SizeSharpness", 3 },
    { "Ring13ScaleLocRot", 4 },
    { "Ring14Map", 0 },
    { "Ring14SizeSharpness", 3 },
    { "Ring14ScaleLocRot", 4 },
    { "Ring15Map", 0 },
    { "Ring15SizeSharpness", 3 },
    { "Ring15ScaleLocRot", 4 },
};

extern void __cdecl FUN_004bd00b(); // std::_Xlen
extern char* __cdecl FUN_004c1902(int value, char* buf, int radix); // _itoa

#define AT(T, off) (*(T*)((char*)this + (off)))
#define ATP(p, T, off) (*(T*)((char*)(p) + (off)))

// NONMATCHING: RALLYTROPHY 0x4137c0
// 2942 vs 2914 bytes; frame is 0x24 larger (original shares stack slots between the ini-scope locals and the
// CarBodyEnv number buffer) and the ring-key array initialisation order differs; flow, calls and strings match
void SceneSetup::FUN_004137c0(const FlareString& dir)
{
    if (AT(int, 0x218) != 0) {
        char sizeKey[20] = "Ring00SizeSharpness";
        char mapKey[10] = "Ring00Map";
        char locKey[18] = "Ring00ScaleLocRot";
        FlareString path;
        path.AssignTmp(FlarePath("data/global/flares/", AT(FlareString, 0x21c)), 0, std::string::npos);
        Config config;
        config.Define(FlareKeys, 0x33);
        if (!config.Load(path.c_str(), true, false, 0)) {
            FlareString msg;
            msg.Assign("Invalid storage file: ");
            throw std::logic_error(*(std::string*)&MissingText2(msg, path.c_str()));
        }
        Sky* sky = new Sky;
        AT(Sky*, 0x6ac) = sky;
        ATP(sky, int, 0xf60) = AT(int, 0x1c);
        ATP(sky, int, 0xf64) = AT(int, 0x20);
        ATP(sky, int, 0xf68) = AT(int, 0x24);
        int rings = config.GetInt("NumberOfRings");
        path.Assign("data/global/flares/");
        FlareString layer;
        layer.AssignTmp(*(TmpString*)&path, 0, std::string::npos);
        const char* glow = config.GetString("GlowMap");
        layer.Append(glow, strlen(glow));
        float glowSize = config.GetFloat("GlowSize");
        sky->SetBase(layer.c_str(), (TextureLoader*)&AT(char, 0xd4), glowSize);
        for (int i = 0; i < rings; i++) {
            sizeKey[4] = locKey[4] = mapKey[4] = "0123456789"[i / 10];
            sizeKey[5] = locKey[5] = mapKey[5] = "0123456789"[i % 10];
            layer.AssignTmp(*(TmpString*)&path, 0, std::string::npos);
            const char* map = config.GetString(mapKey);
            layer.Append(map, strlen(map));
            ConfigVector2 size = config.GetVector2(sizeKey);
            ConfigVector3 loc = config.GetVector3(locKey);
            AT(Sky*, 0x6ac)->AddLayer(layer.c_str(), (TextureLoader*)&AT(char, 0xd4), size.values[0],
                                      size.values[1], loc.values[0], loc.values[1], loc.values[2]);
        }
    }
    switch (AT(int, 0x22c)) {
    case 1:
        AT(WeatherEffect*, 0x6b0) = new RainEffect((TextureLoader474*)&AT(char, 0xd4), 0x400, 0x200);
        break;
    case 2:
        AT(WeatherEffect*, 0x6b0) = new SnowFall((TextureLoader474*)&AT(char, 0xd4), 0x1000);
        break;
    }
    TextureLoader* loader = (TextureLoader*)&AT(char, 0xd4);
    FlareString skid;
    skid.AssignTmp(MissingText2(dir, "SkidMark1.tga"), 0, std::string::npos);
    AT(Texture*, 0x908) = loader->Load(skid.c_str(), 0x40);
    AT(int, 0x974) = 0;
    AT(int, 0x968) = AT(int, 0x1b8);
    AT(int, 0x978) = 0;
    AT(int, 0x984) = 0;
    AT(int, 0x96c) = AT(int, 0x1bc);
    AT(int, 0x97c) = 0;
    AT(int, 0x970) = AT(int, 0x1c0);
    AT(int, 0x980) = 0;
    AT(int, 0x958) = 0;
    AT(int, 0x95c) = 0;
    AT(int, 0x960) = 0;
    AT(int, 0x964) = 0;
    AT(int, 0x92c) = 4;
    AT(int, 0x9a0) = 0xd;
    AT(int, 0x9a4) = 2;
    AT(int, 0x920) = 5;
    AT(int, 0x924) = 6;
    AT(int, 0x948) = 0;
    AT(int, 0x94c) = 0;
    AT(int, 0x950) = 0;
    AT(int, 0x954) = 0x3f800000;
    AT(int, 0x9a8) = 0xf;

    FlareString env;
    env.AssignTmp(MissingText2(dir, "CarBodyEnv"), 0, std::string::npos);
    char num[32];
    FUN_004c1902(AT(int, 4), num, 10);
    env.Add(num);
    env.Add(".tga");
    AT(Texture*, 0x9e4) = loader->Load(env.c_str(), 0x40);
    env.AssignTmp(AT(TmpString, 8), 0, std::string::npos);
    env.Add("textures/particles.tga");
    int c0 = AT(int, 0x1f8), c1 = AT(int, 0x1fc), c2 = AT(int, 0x200), c3 = AT(int, 0x204);
    (void)c3;
    Sprite* a = new Sprite;
    AT(Sprite*, 0x9dc) = a;
    ATP(a, Texture*, 0) = loader->Load(env.c_str(), 0x40);
    ATP(a, int, 0x60) = c0;
    ATP(a, int, 0x98) = 0xd;
    ATP(a, int, 0x9c) = 2;
    ATP(a, int, 0x64) = c1;
    ATP(a, int, 0x18) = 5;
    ATP(a, int, 0x1c) = 2;
    ATP(a, int, 0x68) = c2;
    ATP(a, int, 0x6c) = 0;
    ATP(a, int, 0x70) = 0;
    ATP(a, int, 0x74) = 0;
    ATP(a, int, 0x78) = 0;
    ATP(a, int, 0x7c) = 0;
    ATP(a, int, 0x24) = 4;
    Sprite* b = new Sprite;
    ATP(b, Texture*, 0) = ATP(a, Texture*, 0);
    ATP(b, int, 0x60) = c0;
    ATP(b, int, 0x98) = 0xd;
    ATP(b, int, 0x9c) = 2;
    ATP(b, int, 0x64) = c1;
    ATP(b, int, 0x18) = 5;
    ATP(b, int, 0x1c) = 6;
    ATP(b, int, 0x6c) = 0;
    ATP(b, int, 0x68) = c2;
    AT(Sprite*, 0x9e0) = b;
    ATP(b, int, 0x70) = 0;
    ATP(b, int, 0x74) = 0;
    ATP(b, int, 0x78) = 0;
    ATP(b, int, 0x7c) = 0;
    ATP(b, int, 0x24) = 4;
    AT(int, 0x714) = 0;
    AT(int, 0x718) = 0;
    AT(int, 0x71c) = 0;
    AT(int, 0x720) = 0;
    AT(int, 0x76c) = -1;
    AT(int, 0x840) = -1;
    AT(int, 0x724) = 0;
    AT(int, 0x728) = 0;
    AT(int, 0x72c) = 0;
    AT(int, 0x730) = 0;
    AT(int, 0x7e8) = 0;
    AT(int, 0x7ec) = 0;
    AT(int, 0x7f0) = 0;
    AT(int, 0x7f4) = 0;
    AT(int, 0x6cc) = 2;
    AT(int, 0x6d0) = 2;
    AT(int, 0x74c) = 0xb;
    AT(int, 0x750) = 2;
    AT(int, 0x6f0) = 1;
    AT(int, 0x7a0) = 2;
    AT(int, 0x7a4) = 2;
    AT(int, 0x820) = 0xb;
    AT(int, 0x7f8) = 0;
    AT(int, 0x7fc) = 0;
    AT(int, 0x800) = 0;
    AT(int, 0x804) = 0;
    AT(int, 0x824) = 2;
    AT(int, 0x7c4) = 1;
}
