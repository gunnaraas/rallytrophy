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

class StageLightVector : public std::vector<StCityLight*> {
public:
    void Insert(iterator pos, unsigned int n, StCityLight* const& value);
};
#pragma comment(linker, "/alternatename:?Insert@StageLightVector@@QAEXPAPAUStCityLight@@IABQAU2@@Z=?insert@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXPAPAXIABQAX@Z")

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
// Pointer vectors whose push_back stays the shared out-of-line insert(end(), 1, x) instance (0x4bc680).
struct StPtrVecIns : std::vector<void*> {
    void Ins(iterator pos, unsigned int n, void* const& x);           // 0x4bc680
    void PushBack(void* const& x) { Ins(end(), 1, x); }
};
#define VEC(off) (*(StPtrVecIns*)((BYTE*)this + (off)))
#define VECP(p, off) (*(StPtrVecIns*)((BYTE*)(p) + (off)))
#define RAW ((StStageRaw*)this)

static inline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

struct StNetInfo { BYTE pad[0x28]; int raceIndex; int stage; };   // DAT_00501450 fields used here

// Path strings of this constructor. Early sites keep assign/append out of line and the destructor is the
// out-of-line string destructor (0x430720); the collision-model path (a later site) has its constructor's _Tidy
// and its appends expanded, with _Xlen (0x4bd00b) and _Grow (0x402980) as calls.
struct StPath {
    StPath(const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { Tidy(false); }
    StPath(const StString& x) : alloc(x.alloc) { Tidy(false); Assign(x, 0, std::string::npos); }
    ~StPath();                                                          // 0x430720
    void Tidy(bool built);                                              // 0x402700
    StPath& Assign(const StString& x, unsigned int pos, unsigned int n); // 0x4024f0
    StPath& Assign(const StPath& x, unsigned int pos, unsigned int n);   // 0x4024f0
    StPath& Append(const char* s, unsigned int n);                      // 0x403ba0
    StPath& operator+=(const char* s) { return Append(s, strlen(s)); }
    const char* c_str() const { return ptr == 0 ? "" : ptr; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
struct StPathLate {
    StPathLate(const StString& x) : alloc(x.alloc) { ptr = 0; len = 0; res = 0; Assign(x, 0, std::string::npos); }
    ~StPathLate();                                                      // 0x430720
    StPathLate& Assign(const StString& x, unsigned int pos, unsigned int n); // 0x4024f0
    bool Grow(unsigned int n, bool trim);                               // 0x402980
    StPathLate& operator+=(const char* s)
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = m + len, false)) {
            memcpy(ptr + len, s, m);
            ptr[len = n] = 0;
        }
        return *this;
    }
    const char* c_str() const { return ptr == 0 ? "" : ptr; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

void __cdecl StageLog(const char* fmt, ...);                 // 0x402c90: the release build's empty debug logger
// Error-message string: inline _Tidy stores, begin()/end() through the out-of-line _Freeze (0x414750), the stage
// module's range-replace helper (0x414550) and the out-of-line string destructor (0x430720). Caller arguments still
// need semantic confirmation: the PE pushes a format literal and object field where the helper expects a source range.
struct StMsg {
    StMsg(const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { ptr = 0; len = 0; res = 0; }
    ~StMsg();
    void Freeze();
    char* begin() { Freeze(); return ptr; }
    char* end() { Freeze(); return ptr == 0 ? 0 : ptr + len; }
    StMsg& Format(char* first, char* last, const char* fmt, const char* arg);
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
// std::logic_error as VC6 lays it out: exception("") out of line, the message copied with assign(x, 0, npos).
struct StErrStr {
    StErrStr(const StMsg& x) : alloc(x.alloc) { ptr = 0; len = 0; res = 0; Assign(x, 0, std::string::npos); }
    ~StErrStr();                                                        // 0x430720
    StErrStr& Assign(const StMsg& x, unsigned int pos, unsigned int n);  // 0x4024f0
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
class StLogicError : public std::exception {
public:
    StLogicError(const StMsg& s) : std::exception(""), str(s) {}
    virtual ~StLogicError();
    StErrStr str;
};

// The AI-path string is the fully expanded flavour: the copy constructor's assign(x, 0, npos) and the appends'
// _Grow are inline, with erase (0x408de0), _Tidy (0x402700), _Grow (0x402980), _Xlen (0x4bd00b), _Eos (0x402960)
// and _Copy (0x402ab0) as calls, like VC6's <xstring> bodies.
struct StPathFull {
    StPathFull(const StString& x) : alloc(x.alloc)
    {
        ptr = 0; len = 0; res = 0;
        unsigned int n = x.len;
        if (std::string::npos < n)
            n = std::string::npos;
        if ((StString*)this == &x) {
            Erase(n, std::string::npos);
            Erase(0, 0);
        } else if (0 < n && n == x.len && Refcnt(x.ptr) < 0xfe) {
            Tidy(true);
            ptr = x.ptr;
            len = x.len;
            res = x.res;
            ++Refcnt(ptr);
        } else if (Grow(n, true)) {
            memcpy(ptr, x.c_str(), n);
            ptr[len = n] = 0;
        }
    }
    ~StPathFull();                                                      // 0x430720
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Erase(unsigned int pos, unsigned int n);                       // 0x408de0
    void Tidy(bool built);                                              // 0x402700
    bool Grow(unsigned int n, bool trim);                               // 0x402980
    void Eos(unsigned int n);                                           // 0x402960
    void Copy(unsigned int n);                                          // 0x402ab0
    bool GrowInline(unsigned int n, bool trim)
    {
        if (0xfffffffd < n)
            std::_Xlen();
        if (ptr != 0 && Refcnt(ptr) != 0 && Refcnt(ptr) != 0xff) {
            if (n == 0) {
                --Refcnt(ptr);
                Tidy(false);
                return false;
            }
            Copy(n);
            return true;
        }
        if (n == 0) {
            if (trim)
                Tidy(true);
            else if (ptr != 0)
                Eos(0);
            return false;
        }
        if (res < n || trim && n < res)
            Copy(n);
        return true;
    }
    StPathFull& operator+=(const char* s)
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m)
            std::_Xlen();
        unsigned int n;
        if (0 < m && GrowInline(n = len + m, false)) {
            memcpy(ptr + len, s, m);
            ptr[len = n] = 0;
        }
        return *this;
    }
    static const char* NullStr();                                       // 0x414480: basic_string::_Nullstr out of line
    const char* c_str() const { return ptr == 0 ? NullStr() : ptr; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

static inline const char* NameOf(void* p)    // object name (std::string at +0x114), "" when empty
{
    const char* s = (const char*)IV(p, 0x118);
    return s ? s : "";
}

// NONMATCHING: RALLYTROPHY 0x40c9b0
// 8451 vs 8656 bytes; the call sequence is identical to the original (217 calls, four string flavours, the
// logger and the format helper at the error sites). Open: the 21 start-position stores are scheduled
// differently, the two sign branches' shared tail is kept in the checkpoint copy (the original keeps the sign_
// copy and jumps back), the plain-object push_back sits before the checkpoint block, and the frame layout.
// original: FUN_0040c9b0
Stage::Stage(int a, int b, int c, int d)
{
    int i;
    unsigned int n;

    // Members with constructors are built by the compiler (see stage.h); only the plain fields are set up here.
    AT(StTexCache, 0xd4).SetDetail(DAT_0050141c > 2 ? 2 : DAT_0050141c);

    AT(int, 0x1364) = c;
    AT(int, 0x24d4) = 0; AT(int, 0x24d0) = 0;
    AT(int, 0x85c) = 0; AT(int, 0x860) = 0; AT(int, 0x864) = 0; AT(int, 0x868) = 0; AT(int, 0x86c) = 0;
    AT(int, 0x870) = 0; AT(int, 0x874) = 0; AT(int, 0x878) = 0; AT(int, 0x87c) = 0; AT(int, 0x880) = 0;
    AT(int, 0x13a4) = 0; AT(int, 0x13ec) = 0; AT(int, 0x13f0) = 0; AT(int, 0xa00) = 0;
    AT(int, 0xe8) = 1;
    AT(int, 0x0) = a;
    AT(int, 0x4) = b;

    // track directory: data/tracks/track<a>/ (the original pushes the first argument here)
    _itoa(a, DAT_00508440, 10);
    StString& root = name;
    root = "data/tracks/track";
    root += DAT_00508440;
    root += "/";
    unsigned int fpcw = SaveSceneControlWord();

    // stage config: <root>data/stage<N>.txt
    StPath cfg(root);
    cfg += "data/stage";
    cfg += _itoa(stageCount, DAT_00508440, 10);
    cfg += ".txt";
    RAW->LoadConfig(cfg.c_str());

    AT(float, 0x130) = DAT_005012f8 * DAT_004e13cc;
    // start grid positions (7 starts)
    starts[0].position[0] = 0.0f;  starts[0].position[1] = 5.0f; starts[0].position[2] = 3.0f;
    starts[1].position[0] = 1.5f;  starts[1].position[1] = 5.0f; starts[1].position[2] = 3.0f;
    starts[2].position[0] = -1.5f; starts[2].position[1] = 5.0f; starts[2].position[2] = 1.0f;
    starts[3].position[0] = 1.5f;  starts[3].position[1] = 5.0f; starts[3].position[2] = -1.0f;
    starts[4].position[0] = -1.5f; starts[4].position[1] = 5.0f; starts[4].position[2] = -3.0f;
    starts[5].position[0] = 1.5f;  starts[5].position[1] = 5.0f; starts[5].position[2] = -5.0f;
    starts[6].position[0] = -1.5f; starts[6].position[1] = 5.0f; starts[6].position[2] = -7.0f;
    AT(int, 0x9e8) = 0; AT(int, 0xa04) = 0; AT(int, 0xa08) = 0; AT(int, 0x130c) = 0;
    AT(int, 0x6ac) = 0; AT(int, 0x6b0) = 0; AT(int, 0x9dc) = 0; AT(int, 0x9e0) = 0;
    AT(int, 0x18) = 0; AT(int, 0x9e4) = 0; AT(int, 0x1c) = 0;
    AT(float, 0x20) = 1000.0f;
    AT(int, 0x24) = 0; AT(int, 0x19c) = 0;

    // loading screen caption: "<name> - <text> <n>\n<text137>"
    StPath line;                                     // function-scope strings: destroyed at the end
    StPath text;
    StNetInfo* net = (StNetInfo*)DAT_00501450;
    int idx = net->raceIndex;
    char num[8];
    text += FUN_00405380(idx + 0xb4);
    text += " - ";
    if (DAT_00501458 == 0) {
        text += FUN_00405380(0x1a);
        text += " ";
        _itoa(net->stage, num, 10);
        text += num;
    } else {
        text += FUN_00405380(0x2a);
        text += " ";
        text += FUN_00405380(0x1b);
        text += " ";
        _itoa(net->stage - IV((BYTE*)DAT_0056b0c8, 0x3680 + idx * 0x5e8), num, 10);
        text += num;
    }
    text += "\n";
    line.Assign(text, 0, std::string::npos);
    line += FUN_00405380(0x137);
    ((LoadingLabel*)AT(int, 0x1364))->SetText(line.c_str());
    LoadingLabel* label = (LoadingLabel*)AT(int, 0x1364);   // progress updates below

    // model file names
    StPath stageModel(root);
    stageModel += "models/stage";
    stageModel += _itoa(stageCount, DAT_00508440, 10);
    stageModel += ".bmf";
    StPath texDir(root);
    texDir += "textures/";

    StModelFile mf1(stageModel.c_str(), texDir.c_str(), &AT(StTexCache, 0xd4), &AT(BYTE, 0x98), 0);

    StPath skyModel(root);
    skyModel += "models/stage";
    skyModel += _itoa(stageCount, DAT_00508440, 10);
    skyModel += "sky.bmf";
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
                (*(StageLightVector*)((BYTE*)this + 0x68)).Insert((*(StageLightVector*)((BYTE*)this + 0x68)).end(), 1, light);
                VEC(0x28).PushBack(light);
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
        VEC(0x28).PushBack(p);
        i++;
        label->SetProgress(((float)i / (float)(int)mf1.v48.size()) * DAT_00508494 + DAT_00508490);
    }

    // model 1 list at +0x38
    for (i = 0; i < (int)mf1.v38.size(); i++) {
        VEC(0x38).PushBack(mf1.v38[i]);
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
            VEC(0x78).PushBack(p);
            if (_strnicmp(NameOf(p), "z", 1) == 0)
                IV(p, 0x130) = 1;
            i++;
        } else if (IV(p, 4) == 2) {
            if (IV(p, 0x124) != 1) {          // the == 1 block sits out of line after the loop (VC6: if (!x) A else B)
                if (IV(p, 0x124) == 2) {
                    StCityLight* light = new StCityLight(p, &AT(StTexCache, 0xd4), &AT(Sprite, 0x6b4), &AT(Sprite, 0x788));
                    VEC(0x68).PushBack(light);
                    VEC(0x28).PushBack(light);
                }
            } else {
                VEC(0x68).PushBack(p);
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
            if (_strnicmp(NameOf(p), "sign_", 5) == 0) {
                StSign* s = new StSign;
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
                s->body->pos[0] = FV(p, 0x14);
                s->body->pos[1] = FV(p, 0x18);
                s->body->pos[2] = FV(p, 0x1c);
                s->f4c = 1.45f;
                s->Sync();
                VEC(0x88).PushBack(s);
                i++;
            } else if (_strnicmp(NameOf(p), "checkpoint", 10) != 0) {
                if ((IV(p, 0xc) & lodMask) == 0)
                    VEC(0x68).PushBack(p);
                i++;
            } else {                                   // the tail is written in both sign branches; VC6 merges it
                StSign* s = new StSign;
                s->pos[0] = FV(p, 0x14);
                s->pos[1] = FV(p, 0x18);
                s->pos[2] = FV(p, 0x1c);
                memcpy(s->mat, (BYTE*)p + 0x20, 0x40);
                float ext[3] = { 100.0f, 100.0f, 100.0f };
                s->body = new StBody(p, 25.0f, ext, (float*)((BYTE*)p + 0xb8));
                s->body->SetCollision3(p);
                s->body->pos[0] = FV(p, 0x14);
                s->body->pos[1] = FV(p, 0x18);
                s->body->pos[2] = FV(p, 0x1c);
                s->f4c = 1.45f;
                s->Sync();
                VEC(0x88).PushBack(s);
                i++;
            }
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
        VEC(0x134).PushBack(e);
        void* cell = RAW->Cell((float*)((BYTE*)e->d[1] + 0xb8));
        VECP(cell, 0x60).PushBack(e);
    }

    // collision model (c.bmf)
    StPathLate colModel(root);
    colModel += "models/stage";
    colModel += _itoa(stageCount, DAT_00508440, 10);
    colModel += "c.bmf";
    StModelFile mf3(colModel.c_str(), texDir.c_str(), 0, &AT(BYTE, 0xac), 0);

    for (i = 0; i < (int)mf3.v48.size(); i++)
        VEC(0x48).PushBack(mf3.v48[i]);
    for (i = 0; i < (int)mf3.v38.size(); ) {
        VEC(0x58).PushBack(mf3.v38[i]);
        ((StVirt*)VEC(0x58)[i])->Prepare();
        i++;
        label->SetProgress(((float)i / (float)(int)mf3.v38.size()) * DAT_00508484 + DAT_00508480);
    }
    for (n = 0; AT(int, 0x5c) != 0 && n < (unsigned int)((AT(int, 0x60) - AT(int, 0x5c)) >> 2); ) {
        void* o = VEC(0x58)[n];
        StColModel* m = new StColModel(o, 0);
        m->Update();
        m->tree->Build(0x10);
        VEC(0x154).PushBack(m);
        void* cell = RAW->Cell((float*)((BYTE*)o + 0xb8));
        VECP(cell, 0x80).PushBack(m);
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
            if (IV(mesh, 0x40) == 0) {
                StageLog("FATAL ERROR: collision mesh %s has no vertices.", NameOf(p));
                StMsg msg;
                msg.Format(msg.begin(), msg.end(), "collision mesh %s has no vertices", NameOf(p));
                throw StLogicError(msg);
            }
            if (IV(mesh, 0x44) == 0) {
                StageLog("FATAL ERROR: collision mesh %s has no triangles.", NameOf(p));
                StMsg msg;
                msg.Format(msg.begin(), msg.end(), "collision mesh %s has no triangles", NameOf(p));
                throw StLogicError(msg);
            }
            ((StVirt*)p)->Prepare();
            StColModel* m = new StColModel(p, 0);
            m->Update();
            m->tree->Build(0x10);
            if (IV(m->tree, 0x24) == 0)
                StageLog("ERROR: collision mesh %s has degenerate triangles.", NameOf(p));
            void* cell = RAW->Cell((float*)((BYTE*)p + 0xb8));
            VECP(cell, 0x80).PushBack(m);
            VEC(0x154).PushBack(m);
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
    AT(StModelCache, 0xf4).Preload(*(std::vector<void*>*)&VEC(0x68));
    for (n = 0; AT(int, 0x3c) != 0 && n < (unsigned int)((AT(int, 0x40) - AT(int, 0x3c)) >> 2); n++)
        AT(StModelCache, 0xf4).Release(VEC(0x38)[n]);
    AT(StModelCache, 0xf4).Release(AT(void*, 0x18));
    RestoreSceneControlWord(fpcw);
    RAW->SetTextures((const std::string*)&texDir);
    AT(StAiPath*, 0x1368) = new StAiPath;
    if (d != 0) {
        StPathFull ai(root);
        ai += "data/stage";
        ai += _itoa(stageCount, DAT_00508440, 10);
        ai += ".ai";
        AT(StAiPath*, 0x1368)->Load(ai.c_str());
    }
    RAW->LoadAnimals();
    RAW->Finish();
}
