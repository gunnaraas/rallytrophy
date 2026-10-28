// Stage::Stage (0x40c9b0): loads a stage - track/stage config, the stage model (objects, lights,
// splits, checkpoints, signs), sky/sun, collision model, grid, AI path.
#include <new>
#include <string>
#include <vector>
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stage.h"
#include "unk_414330.h"

extern float DAT_005012f8, DAT_004e13cc, DAT_004e139c, DAT_004e13c8, DAT_004e1348, DAT_004e134c;
extern float DAT_004e13c4, DAT_004e1390, DAT_004e138c;
extern float DAT_00508490, DAT_00508494, DAT_00508488, DAT_0050848c, DAT_00508480, DAT_00508484;
extern float DAT_00508478, DAT_0050847c, DAT_00508470, DAT_00508474;
extern char DAT_00508440[];            // _itoa scratch buffer
extern int DAT_0050141c, DAT_00501458;
extern class GameView* DAT_0056b0c8;
extern const char* FUN_00405380(int index);   // text by index
extern void __cdecl FUN_004c11f4_vec(void* p, unsigned size, int count, void (*ctor)(), void (*dtor)());
extern unsigned int __cdecl SaveSceneControlWord();            // original: FUN_004a7da0
extern void __cdecl RestoreSceneControlWord(unsigned int word); // original: FUN_004a7d90
extern void FUN_00402c90();
extern void FUN_00490ad0();
extern void FUN_0040c6f0();
extern void FUN_0040c700();
extern void FUN_00404cd0();
extern void FUN_00430720();
extern void FUN_0040c520();
extern void FUN_0040c5e0();
extern void FUN_004a8580();
extern void FUN_004a86d0();

#define AT(T, off) (*(T*)((BYTE*)this + (off)))
#define IV(p, o) (*(int*)((BYTE*)(p) + (o)))
#define FV(p, o) (*(float*)((BYTE*)(p) + (o)))
#define VEC(off) (*(std::vector<void*>*)((BYTE*)this + (off)))
#define VECP(p, off) (*(std::vector<void*>*)((BYTE*)(p) + (off)))
#define RAW ((StStageRaw*)this)

static inline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

static const char* NameOf(void* p)    // object name (std::string at +0x114), "" when empty
{
    const char* s = (const char*)IV(p, 0x118);
    return s ? s : "";
}

// NONMATCHING: RALLYTROPHY 0x40c9b0
// 10022 vs 8656 bytes. The member construction now comes from the layout in stage.h and the cleanup-state
// sequence starts like the original's (11, 12, 30, 31, ...); the body is still written from the pseudocode with
// offset macros, so its frame (0x30c vs 0x258), temporaries and string handling are not reproduced yet.
// original: FUN_0040c9b0
Stage::Stage(int a, int b, int c, int d)
{
    int i;
    unsigned int n;

    // Members with constructors are built by the compiler (see stage.h); only the plain fields are set up here.
    AT(int, 0x13e8) = 0x11;
    texCache.SetDetail(DAT_0050141c > 2 ? 2 : DAT_0050141c);

    AT(int, 0x1364) = c;
    AT(int, 0x24d4) = 0; AT(int, 0x24d0) = 0;
    for (i = 0x85c; i <= 0x880; i += 4)
        AT(int, i) = 0;
    AT(int, 0x13a4) = 0; AT(int, 0x13ec) = 0; AT(int, 0x13f0) = 0; AT(int, 0xa00) = 0;
    AT(int, 0xe8) = 1;
    AT(int, 0x0) = a;
    AT(int, 0x4) = b;

    // track directory: data/tracks/track<N>/
    _itoa(b, DAT_00508440, 10);
    std::string& root = AT(std::string, 0x8);
    root.assign("data/tracks/track");
    root.append(DAT_00508440);
    root.append("/");
    unsigned int fpcw = SaveSceneControlWord();

    // stage config: <root>data/stage<N>.txt
    std::string cfg(root, 0, std::string::npos);
    cfg.append("data/stage");
    cfg.append(_itoa(AT(int, 0x4), DAT_00508440, 10));
    cfg.append(".txt");
    RAW->LoadConfig(cfg.c_str());

    AT(float, 0x130) = DAT_005012f8 * DAT_004e13cc;
    {   // start grid positions (7 starts)
        static const float st[7][3] = {
            { 0.0f, 5.0f, 3.0f }, { 1.5f, 5.0f, 3.0f }, { -1.5f, 5.0f, 1.0f }, { 1.5f, 5.0f, -1.0f },
            { -1.5f, 5.0f, -3.0f }, { 1.5f, 5.0f, -5.0f }, { -1.5f, 5.0f, -7.0f }
        };
        for (i = 0; i < 7; i++) {
            AT(float, 0x1310 + i * 12) = st[i][0];
            AT(float, 0x1314 + i * 12) = st[i][1];
            AT(float, 0x1318 + i * 12) = st[i][2];
        }
    }
    AT(int, 0x9e8) = 0; AT(int, 0xa04) = 0; AT(int, 0xa08) = 0; AT(int, 0x130c) = 0;
    AT(int, 0x6ac) = 0; AT(int, 0x6b0) = 0; AT(int, 0x9dc) = 0; AT(int, 0x9e0) = 0;
    AT(int, 0x18) = 0; AT(int, 0x9e4) = 0; AT(int, 0x1c) = 0;
    AT(float, 0x20) = 1000.0f;
    AT(int, 0x24) = 0; AT(int, 0x19c) = 0;

    // loading screen caption: "<name> - <text> <n>\n<text137>"
    LoadingLabel* label = (LoadingLabel*)AT(int, 0x1364);
    {
        std::string line;
        std::string text;
        BYTE* net = (BYTE*)DAT_00501450;
        int idx = IV(net, 0x28);
        char num[8];
        text.append(FUN_00405380(idx + 0xb4));
        text.append(" - ");
        if (DAT_00501458 == 0) {
            text.append(FUN_00405380(0x1a));
            text.append(" ");
            _itoa(IV(net, 0x2c), num, 10);
        } else {
            text.append(FUN_00405380(0x2a));
            text.append(" ");
            text.append(FUN_00405380(0x1b));
            text.append(" ");
            _itoa(IV(net, 0x2c) - IV((BYTE*)DAT_0056b0c8, 0x3680 + idx * 0x5e8), num, 10);
        }
        text.append(num);
        text.append("\n");
        line.assign(text, 0, std::string::npos);
        line.append(FUN_00405380(0x137));
        label->SetText(line.c_str());
    }

    // model file names
    std::string stageModel(root, 0, std::string::npos);
    stageModel.append("models/stage");
    stageModel.append(_itoa(AT(int, 0x4), DAT_00508440, 10));
    stageModel.append(".bmf");
    std::string texDir(root, 0, std::string::npos);
    texDir.append("textures/");

    StModelFile mf1(stageModel.c_str(), texDir.c_str(), &AT(StTexCache, 0xd4), &AT(BYTE, 0x98), 0);

    std::string skyModel(root, 0, std::string::npos);
    skyModel.append("models/stage");
    skyModel.append(_itoa(AT(int, 0x4), DAT_00508440, 10));
    skyModel.append("sky.bmf");
    StModelFile mf2(skyModel.c_str(), texDir.c_str(), &AT(StTexCache, 0xd4), &AT(BYTE, 0xc0), 0);

    // sky / sun from the sky model
    for (i = 0; i < (int)mf2.v28.size(); ) {
        void* p = mf2.v28[i];
        const char* nm = NameOf(p);
        if (_stricmp(nm, "sky") == 0) {
            i++;
            AT(void*, 0x18) = p;
            IV(p, 0x8) = 0;
        } else {
            if (_stricmp(nm, "sun") == 0) {
                AT(int, 0x1c) = IV(p, 0x14);
                AT(int, 0x20) = IV(p, 0x18);
                AT(int, 0x24) = IV(p, 0x1c);
            }
            if (p)
                delete (StVirt*)p;
            i++;
        }
    }
    for (void** node = (void**)AT(void*, 0xc0); node != 0; ) {       // materials list of the sky model
        BYTE* m = (BYTE*)node[2];
        node = (void**)*node;
        IV(m, 0x98) = 0xf;
        IV(m, 0x14) = 0;
    }

    // stage objects (model 1, list at +0x48)
    for (i = 0; i < (int)mf1.v48.size(); ) {
        void* p = mf1.v48[i];
        if (IV(p, 4) == 2) {
            if (IV(p, 0x124) == 1) {
                float fz = AT(float, 0x24), fy = AT(float, 0x20), fx = AT(float, 0x1c);
                float c1 = AT(float, 0x1e0), c0 = AT(float, 0x1dc);
                IV(p, 8) = 126;
                FV(p, 0x12c) = c1;
                FV(p, 0x128) = c0;
                FV(p, 0x130) = AT(float, 0x1e4);
                if (DAT_004e139c < fz * fz + fy * fy + fx * fx) {
                    float nx = -AT(float, 0x1c);
                    float nz = -AT(float, 0x24);
                    float len = (float)sqrt(nx * nx + nz * nz);
                    if (DAT_004e13c8 < len) {
                        float dz = nz / len;
                        float k = DAT_004e1348 - FV(p, 0x44) * FV(p, 0x44);
                        if (k <= DAT_004e134c)
                            k = DAT_004e134c;
                        float* f = (float*)p;
                        f[0xd] = 1.0f;
                        f[0xe] = 0.0f;
                        f[0xc] = 0.0f;
                        f[0x10] = (float)sqrt(k) * (nx / len);
                        f[0x12] = (float)sqrt(k) * dz;
                        f[8] = f[0xd] * f[0x12] - f[0xe] * f[0x11];
                        f[9] = f[0xe] * f[0x10] - f[0xc] * f[0x12];
                        f[10] = f[0xc] * f[0x11] - f[0xd] * f[0x10];
                        float inv = DAT_004e1348 / (float)sqrt(f[10] * f[10] + f[9] * f[9] + f[8] * f[8]);
                        f[8] = inv * f[8];
                        f[9] = inv * f[9];
                        f[10] = inv * f[10];
                        f[0xc] = f[0x11] * f[10] - f[0x12] * f[9];
                        f[0xd] = f[0x12] * f[8] - f[0x10] * f[10];
                        f[0xe] = f[0x10] * f[9] - f[0x11] * f[8];
                        inv = DAT_004e1348 / (float)sqrt(f[0xe] * f[0xe] + f[0xd] * f[0xd] + f[0xc] * f[0xc]);
                        f[0xc] = inv * f[0xc];
                        f[0xd] = inv * f[0xd];
                        f[0xe] = inv * f[0xe];
                    }
                }
            } else if (IV(p, 0x124) == 2 && FV(p, 0x10c) != 0.0f) {
                StCityLight* light = new StCityLight(p, &AT(StTexCache, 0xd4), &AT(Sprite, 0x6b4), &AT(Sprite, 0x788));
                ((StShow*)p)->Show();
                VEC(0x68).push_back(light);
                VEC(0x28).push_back(light);
            }
        } else {
            IV(p, 8) = 1;
            if (IV(p, 4) == 1) {
                ((StSceneObj*)p)->Place(&AT(float, 0x1d0));
                if ((IV(p, 0xc) & 0x80) != 0) {
                    BYTE* mesh = (BYTE*)((StMeshList*)((BYTE*)p + 0x124))->At(0);
                    for (unsigned int k = 0; IV(mesh, 0x34) != 0 && k < (unsigned int)((IV(mesh, 0x38) - IV(mesh, 0x34)) >> 2); k++) {
                        BYTE* e = *(BYTE**)(IV(mesh, 0x34) + k * 4);
                        IV(e, 0x40) = 0x3e800000;
                        IV(e, 0x44) = 0x3e800000;
                        IV(e, 0x48) = 0x3e800000;
                    }
                }
            }
        }
        VEC(0x28).push_back(p);
        i++;
        label->SetProgress(((float)i / (float)(int)mf1.v48.size()) * DAT_00508494 + DAT_00508490);
    }

    // model 1 list at +0x38
    for (i = 0; i < (int)mf1.v38.size(); i++) {
        VEC(0x38).push_back(mf1.v38[i]);
        void* o = VEC(0x38)[i];
        ((StSceneObj*)o)->Place(&AT(float, 0x1d0));
        ((StVirt*)o)->Prepare();
        ((StSceneObj2*)o)->Refresh();
        IV(o, 8) = 1;
        label->SetProgress(((float)(i + 1) / (float)(int)mf1.v38.size()) * DAT_0050848c + DAT_00508488);
    }

    // model 1 parts (+0x28): splits, starts, signs, checkpoints, plain objects
    int lodMask = (DAT_0050141c < 3) ? 0x40 : 0;
    for (i = 0; i < (int)mf1.v28.size(); ) {
        void* p = mf1.v28[i];
        if (IV(p, 4) == 3) {
            VEC(0x78).push_back(p);
            if (_strnicmp(NameOf(p), "z", 1) == 0)
                IV(p, 0x130) = 1;
            i++;
        } else if (IV(p, 4) == 2) {
            if (IV(p, 0x124) == 1) {
                VEC(0x68).push_back(p);
            } else if (IV(p, 0x124) == 2) {
                StCityLight* light = new StCityLight(p, &AT(StTexCache, 0xd4), &AT(Sprite, 0x6b4), &AT(Sprite, 0x788));
                VEC(0x68).push_back(light);
                VEC(0x28).push_back(light);
            }
            i++;
        } else if (_strnicmp(NameOf(p), "split", 5) == 0) {
            RAW->AddSplit(p);
            i++;
        } else {
            int kind = IV(p, 0x11c);
            if ((kind == 5 || kind == 7) && _strnicmp(NameOf(p), "start", 5) != 0) {
                RAW->AddStart(p);
                i++;
                continue;
            }
            StSign* s;
            if (_strnicmp(NameOf(p), "sign_", 5) == 0) {
                s = new StSign;
                s->pos[0] = FV(p, 0x14);
                s->pos[1] = FV(p, 0x18);
                s->pos[2] = FV(p, 0x1c);
                memcpy(s->mat, (BYTE*)p + 0x20, 0x40);
                float ext[3] = { 250.0f, 200.0f, 250.0f };
                float c[3];
                c[0] = FV(p, 0xb8);
                c[2] = FV(p, 0xc0);
                c[1] = FV(p, 0xa4) * DAT_004e13c4 + FV(p, 0xb0) * DAT_004e1390;
                s->body = new StBody(p, 150.0f, ext, (float*)((BYTE*)p + 0xb8));
                s->body->SetCollision2(p);
            } else if (_strnicmp(NameOf(p), "checkpoint", 10) != 0) {
                if ((IV(p, 0xc) & lodMask) == 0)
                    VEC(0x68).push_back(p);
                i++;
                continue;
            } else {
                s = new StSign;
                s->pos[0] = FV(p, 0x14);
                s->pos[1] = FV(p, 0x18);
                s->pos[2] = FV(p, 0x1c);
                memcpy(s->mat, (BYTE*)p + 0x20, 0x40);
                float ext[3] = { 100.0f, 100.0f, 100.0f };
                s->body = new StBody(p, 25.0f, ext, (float*)((BYTE*)p + 0xb8));
                s->body->SetCollision3(p);
            }
            s->body->pos[0] = FV(p, 0x14);
            s->body->pos[1] = FV(p, 0x18);
            s->body->pos[2] = FV(p, 0x1c);
            s->f4c = 1.45f;
            s->Sync();
            VEC(0x88).push_back(s);
            i++;
        }
    }

    // grid from the model bounds
    AT(float, 0x164) = mf1.bounds[0];
    AT(float, 0x168) = mf1.bounds[1];
    AT(float, 0x16c) = mf1.bounds[2];
    AT(float, 0x170) = mf1.bounds[3];
    AT(float, 0x174) = mf1.bounds[4];
    AT(float, 0x178) = mf1.bounds[5];
    AT(float, 0x188) = mf1.bounds[6];
    AT(float, 0x18c) = mf1.bounds[7];
    AT(float, 0x190) = mf1.bounds[8];
    AT(int, 0x194) = RoundF((AT(float, 0x170) - AT(float, 0x164)) / AT(float, 0x188) + DAT_004e138c);
    float rz = (AT(float, 0x178) - AT(float, 0x16c)) / AT(float, 0x190) + DAT_004e138c;
    AT(int, 0x198) = RoundF(rz);
    AT(float, 0x170) = (float)AT(int, 0x194) * AT(float, 0x188) + AT(float, 0x164);
    AT(float, 0x178) = (float)AT(int, 0x198) * AT(float, 0x190) + AT(float, 0x16c);
    {
        float diff[3];
        ((S414Vec3*)&AT(float, 0x170))->Sub(diff, &AT(float, 0x164));
        AT(float, 0x17c) = diff[0];
        AT(float, 0x180) = diff[1];
        AT(float, 0x184) = diff[2];
    }
    int cells = AT(int, 0x198) * AT(int, 0x194);
    {
        int* blk = (int*)operator new(cells * 0xb0 + 4);
        BYTE* arr = 0;
        if (blk) {
            *blk = cells;
            arr = (BYTE*)(blk + 1);
            FUN_004c11f4_vec(arr, 0xb0, cells, FUN_0040c520, FUN_0040c5e0);
        }
        AT(BYTE*, 0x19c) = arr;
    }
    {
        float x0 = AT(float, 0x188) * DAT_004e138c + AT(float, 0x164);
        float z = AT(float, 0x190) * DAT_004e138c + AT(float, 0x16c);
        BYTE* row = AT(BYTE*, 0x19c);
        for (int r = 0; r < AT(int, 0x198); r++) {
            float x = x0;
            BYTE* cell = row;
            for (int cc = 0; cc < AT(int, 0x194); cc++) {
                FV(cell, 0) = x;
                FV(cell, 4) = 0.0f;
                FV(cell, 8) = z;
                cell += 0xb0;
                x += AT(float, 0x188);
            }
            z += AT(float, 0x190);
            row += AT(int, 0x194) * 0xb0;
        }
    }
    for (n = 0; AT(int, 0x3c) != 0 && n < (unsigned int)((AT(int, 0x40) - AT(int, 0x3c)) >> 2); n++) {
        StGridEntry* e = new StGridEntry(VEC(0x38)[n]);
        VEC(0x134).push_back(e);
        void* cell = RAW->Cell((float*)((BYTE*)e->d[1] + 0xb8));
        VECP(cell, 0x60).push_back(e);
    }

    // collision model (c.bmf)
    std::string colModel(root, 0, std::string::npos);
    colModel.append("models/stage");
    colModel.append(_itoa(AT(int, 0x4), DAT_00508440, 10));
    colModel.append("c.bmf");
    StModelFile mf3(colModel.c_str(), texDir.c_str(), 0, &AT(BYTE, 0xac), 0);

    for (i = 0; i < (int)mf3.v48.size(); i++)
        VEC(0x48).push_back(mf3.v48[i]);
    for (i = 0; i < (int)mf3.v38.size(); ) {
        VEC(0x58).push_back(mf3.v38[i]);
        ((StVirt*)VEC(0x58)[i])->Prepare();
        i++;
        label->SetProgress(((float)i / (float)(int)mf3.v38.size()) * DAT_00508484 + DAT_00508480);
    }
    for (n = 0; AT(int, 0x5c) != 0 && n < (unsigned int)((AT(int, 0x60) - AT(int, 0x5c)) >> 2); ) {
        void* o = VEC(0x58)[n];
        StColModel* m = new StColModel(o, 0);
        m->Update();
        m->tree->Build(0x10);
        VEC(0x154).push_back(m);
        void* cell = RAW->Cell((float*)((BYTE*)o + 0xb8));
        VECP(cell, 0x80).push_back(m);
        if (o)
            delete (StVirt*)o;
        int total = AT(int, 0x5c) == 0 ? 0 : (AT(int, 0x60) - AT(int, 0x5c)) >> 2;
        n++;
        label->SetProgress(((float)(int)n / (float)total) * DAT_0050847c + DAT_00508478);
    }
    for (i = 0; i < (int)mf3.v28.size(); ) {
        void* p = mf3.v28[i];
        if (IV(p, 4) == 1) {
            BYTE* mesh = (BYTE*)((StMeshList*)((BYTE*)p + 0x124))->At(0);
            char msg[256];
            if (IV(mesh, 0x40) == 0) {
                sprintf(msg, "collision mesh %s has no vertices", NameOf(p));
                throw std::logic_error(std::string(msg));
            }
            if (IV(mesh, 0x44) == 0) {
                sprintf(msg, "collision mesh %s has no triangles", NameOf(p));
                throw std::logic_error(std::string(msg));
            }
            ((StVirt*)p)->Prepare();
            StColModel* m = new StColModel(p, 0);
            m->Update();
            m->tree->Build(0x10);
            void* cell = RAW->Cell((float*)((BYTE*)p + 0xb8));
            VECP(cell, 0x80).push_back(m);
            VEC(0x154).push_back(m);
        }
        if (p)
            delete (StVirt*)p;
        i++;
        label->SetProgress(((float)i / (float)(int)mf3.v28.size()) * DAT_00508474 + DAT_00508470);
    }

    RAW->F414b20();
    RAW->F416260();
    RAW->Build();
    RAW->Fill();
    RAW->ScanEffects();
    RAW->F415370();
    AT(StModelCache, 0xf4).Preload(VEC(0x68));
    for (n = 0; AT(int, 0x3c) != 0 && n < (unsigned int)((AT(int, 0x40) - AT(int, 0x3c)) >> 2); n++)
        AT(StModelCache, 0xf4).Release(VEC(0x38)[n]);
    AT(StModelCache, 0xf4).Release(AT(void*, 0x18));
    RestoreSceneControlWord(fpcw);
    RAW->SetTextures(&texDir);
    AT(StAiPath*, 0x1368) = new StAiPath;
    if (d != 0) {
        std::string ai(root, 0, std::string::npos);
        ai.append("data/stage");
        ai.append(_itoa(AT(int, 0x4), DAT_00508440, 10));
        ai.append(".ai");
        AT(StAiPath*, 0x1368)->Load(ai.c_str());
    }
    RAW->LoadAnimals();
    RAW->Finish();
}
