// TrackSelectMenu (0x441370-0x445e7f): the "Select Track" screen. Map: analysis/range-441370.md; class in
// include/unk_441370.h.
// Work groups: TS-A (statics, ctor, dtors, RallyInfo; prefix TA_) and six more.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_441370.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group TS-A ----
// The file's one static: the black text colour used by DrawDescription (original: 0x441370-0x4413a0).
class TA_Black {
public:
    TA_Black(DWORD argb) { value = argb; }
    ~TA_Black() {}
    DWORD value;
};
TA_Black DAT_00508c14(0xff000000);


// ---- from group draft W2 ----
inline int TSB_FloatToInt(float f)
{
    int i;
    __asm {
        fld f
        fistp i
    }
    return i;
}


// ---- from group draft W3 ----
extern char DAT_005070b0[];            // global empty text buffer

// ---- Group TS-C: Setup ----
// A VC6 std::string seen through its layout (allocator, pointer, length, reserve). Setup's string operations are
// expanded to different depths (the inline budget runs out part way through the function), so each site is pinned:
// out-of-line assign/append (0x404ba0/0x403ba0), expansions that call _Eos (0x402960) or set the terminator inline.
struct TC_Str {
    int alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
    void AssignCall(const char* s, unsigned int n);   // 0x404ba0
    void AppendCall(const char* s, unsigned int n);   // 0x403ba0
    bool Grow(unsigned int n, bool trim);              // 0x402980
    void Eos(unsigned int n);                         // 0x402960
    void AppendNC(unsigned int n, char c);             // 0x408d10
    void AssignStr(const std::string& x, unsigned int pos, unsigned int n); // 0x4024f0
    static void Xlen();                                // 0x4bd00b
    void AssignA(const char* s) { AssignCall(s, strlen(s)); }
    void AppendA(const char* s) { AppendCall(s, strlen(s)); }
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    void AssignC(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            len = n;
            ptr[n] = 0;
        }
    }
    void AppendD(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            Xlen();
        }
        if (n > 0) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                Eos(total);
            }
        }
    }
};
#define TC_S(x) (*(TC_Str*)&(x))

// A std::string constructed from a literal with the out-of-line _Tidy (0x402700) and assign(const char*, n)
// (0x404ba0); its destructor is expanded inline.
struct TC_Local {
    TC_Local(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        AssignCall(s, strlen(s));
    }
    ~TC_Local()
    {
        if (ptr == 0)
            ;
        else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255)
            alloc.deallocate(ptr - 1, res + 2);
        else
            --Refcnt(ptr);
        ptr = 0, len = 0, res = 0;
    }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Tidy(bool built);                             // 0x402700
    void AssignCall(const char* s, unsigned int n);    // 0x404ba0
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
#define TC_STR(x) ((std::string*)&(x))

// The temporary of `section + "text"` whose destructor the original calls out of line (_Tidy(true), 0x402700).
struct TC_Tmp {
    BYTE alloc[4];
    char* ptr;
    unsigned int len;
    unsigned int res;
    ~TC_Tmp() { Tidy(true); }
    void Tidy(bool built);                             // 0x402700
};
TC_Tmp TC_Plus(const std::string* a, const char* b);   // 0x409130 (cdecl, hidden result pointer)
#define TC_ASSIGN_PLUS(dst, a, b) \
    { \
        TC_Tmp tmp = TC_Plus((const std::string*)&(a), (b)); \
        TC_S(dst).AssignStr(*(std::string*)&tmp, 0, std::string::npos); \
    }



// ---- from group draft W4 ----
extern ConfigKey DAT_004f9d68[];       // info.ini keys (66 entries, table 0x4f92c8)
extern char DAT_005070b0[];            // empty text buffer
void TD_FloatToText(char* out, float value); // 0x401000: "123.4"

// The record entry fields LoadRallyData reads beyond RecordEntry (same object, 0x60 bytes).
struct TD_Record {
    char name[0x1e];
    BYTE unk1e;
    char car;                          // 0x1f
    BYTE unk20[0x54 - 0x20];
    int time;                          // 0x54
    int arcadeTime;                    // 0x58
    float topSpeed;                    // 0x5c
};

// A VC6 std::string seen through its layout. LoadRallyData's string operations are expanded to different depths (the
// inline budget runs out part way through the function), so each site is pinned: out-of-line assign/append
// (0x404ba0/0x403ba0) or the expansions that end in _Eos (0x402960).
struct TD_Str {
    int alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
    void AssignCall(const char* s, unsigned int n);   // 0x404ba0
    void AppendCall(const char* s, unsigned int n);   // 0x403ba0
    bool Grow(unsigned int n, bool trim);              // 0x402980
    void Eos(unsigned int n);                         // 0x402960
    static void Xlen();                                // 0x4bd00b
    void AssignA(const char* s) { AssignCall(s, strlen(s)); }
    void AppendA(const char* s) { AppendCall(s, strlen(s)); }
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    void AssignC(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    void AppendD(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            Xlen();
        }
        if (n > 0) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                Eos(total);
            }
        }
    }
};
// The "Stage<n>" section name: constructor with the out-of-line _Tidy (0x402700) and assign (0x404ba0), destructor
// (_Tidy(true)) expanded inline.
struct TD_Loc {
    TD_Loc(const char* s) : alloc(std::allocator<char>())
    {
        Tidy(false);
        ((TD_Str*)this)->AssignA(s);
    }
    __forceinline ~TD_Loc()
    {
        if (ptr == 0)
            ;
        else if (Ref(ptr) == 0 || Ref(ptr) == 255)
            ::operator delete(ptr - 1);
        else
            --Ref(ptr);
        ptr = 0, len = 0, res = 0;
    }
    unsigned char& Ref(const char* u) { return ((unsigned char*)u)[-1]; }
    void Tidy(bool built);             // 0x402700
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
#define TD_S(x) (*(TD_Str*)&(x))


// ---- from group draft W5 ----
struct TSE_WarpPoint {
    TSE_WarpPoint(float px, float py)
    {
        x = px;
        y = py;
    }
    ~TSE_WarpPoint() {}
    float x, y;
};

void TSE_Xlen();                        // 0x4bd00b: throws length_error

// A view of std::string whose assign(const char*) is expanded by hand (see C_Str in unk_445e80.cpp): _Grow stays an
// out-of-line call (0x402980) and the terminator is written inline.
struct TSE_Str {
    BYTE alloc[4];
    char* ptr;
    unsigned int len;
    unsigned int res;
    bool Grow(unsigned int n, bool trim); // 0x402980
    void AssignInline(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    void Append(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            TSE_Xlen();
        }
        unsigned int total;
        if (n > 0 && Grow(total = len + n, false)) {
            std::char_traits<char>::copy(ptr + len, s, n);
            ptr[len = total] = 0;
        }
    }
};

// std::string with its copy constructor (0x408a80) and destructor (0x430720) out of line: the by-value RallyInfo copy
// expands only the first two string copies inline.
struct TSE_Str16 {
    TSE_Str16(const TSE_Str16& other);   // 0x408a80
    ~TSE_Str16();                       // 0x430720
    BYTE data[16];
};

// RallyInfo (include/unk_441370.h) with the seven string arrays as TSE_Str16: same layout, used for the by-value call.
struct TSE_RallyInfo {
    std::string name;
    std::string description;
    int numStages, numNovice, numIntermediate, numExpert, numArcade, firstArcade;
    float length[12];
    TSE_Str16 stageName[12];
    TSE_Str16 lengthText[12];
    TSE_Str16 timeText[12];
    TSE_Str16 surfaceText[12];
    TSE_Str16 recordTime[12];
    TSE_Str16 recordSpeed[12];
    TSE_Str16 recordTopSpeed[12];
    int unk5a8[3];
    Sprite* photo;
    Sprite* stageInfo[12];
};
typedef char TSE_SizeCheck[sizeof(TSE_RallyInfo) == 0x5e8 ? 1 : -1];
class TSE_View : public TrackSelectMenu {
public:
    void DrawRallyV(TSE_RallyInfo info); // 0x445870
};

// Layout values of the 1024x768 design space. The original keeps them in .rdata as data objects (0x4e184c..0x4e18a4),
// so they stay loads (`fld [const]; fmul [scale]`) instead of folding into the multiply.
static const float TSE_LISTY = 262.0f;  // 0x4e184c: first row
static const float TSE_STEP = 42.0f;    // 0x4e1850: row pitch
static const float TSE_LISTX = 54.0f;   // 0x4e1854: row label x
static const float TSE_COUNTX = 350.0f; // 0x4e1858: "n/m" counter x
static const float TSE_TITLEX = 54.0f;  // 0x4e185c
static const float TSE_TITLEY = 142.0f; // 0x4e1860
static const float TSE_NAMEX = 576.0f;  // 0x4e1864
static const float TSE_NAMEY = 424.0f;  // 0x4e1868
static const float TSE_BACKY = 610.0f;  // 0x4e18a0
static const float TSE_COUNTW = 130.0f; // 0x4e18a4

// The slide animation length in milliseconds; a data object in the original (the division is not folded).
static const float TSE_SLIDE_MS = 400.0f;

#define TSE_WARP(items, sel, sx, sy)                                                                                  \
    do {                                                                                                             \
        float cx = (items)[sel]->GetTopLeft().x +                                                                    \
                   ((items)[sel]->GetBottomRight().x - (items)[sel]->GetTopLeft().x) * 0.5f;                          \
        float cy = (items)[sel]->GetTopLeft().y +                                                                    \
                   ((items)[sel]->GetBottomRight().y - (items)[sel]->GetTopLeft().y) * 0.5f;                          \
        ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&TSE_WarpPoint(cx * (sx), cy * (sy)));                        \
    } while (0)


// ---- from group draft W6 ----
class TF_Text {
public:
    void SetColor(Color color);        // 0x4243b0
};

// Float to int with the x87 default rounding (the same inline assembler as FloatToInt in unk_479170.h).
inline int TF_FloatToInt(float f)
{
    int i;
    __asm {
        fld f
        fistp i
    }
    return i;
}


// Layout constants of the details table (file-scope const objects in the original: VC6 neither folds sums of them
// nor hoists them out of the loop; .rdata 0x4e186c..0x4e1888).
static const float TF_X1 = 575.0f;     // 0x4e186c
static const float TF_X2 = 755.0f;     // 0x4e1870
static const float TF_Y0 = 460.0f;     // 0x4e1874
static const float TF_ROW = 48.0f;     // 0x4e1880
static const float TF_INDENT = 25.0f;  // 0x4e1884
static const float TF_LINE = 15.0f;    // 0x4e1888


// ---- from group draft W7 ----
struct TSG_Font {
    BYTE unk0[0x1c];
    float scaleX;                      // 0x1c
    float scaleY;                      // 0x20
    BYTE unk24[8];
    int lineHeight;                    // 0x2c
    BYTE unk30[0x120 - 0x30];
    DWORD color;                       // 0x120
    BYTE unk124[0x130 - 0x124];
    BYTE wrap;                         // 0x130
    BYTE unk131[3];
    float wrapY;                       // 0x134
    float wrapW;                       // 0x138
    void SetColor(DWORD c);           // 0x4243b0
    void Print(RenderDevice* device, float x, float y, const char* text, int flags, float sx, float sy); // 0x4a97a0
};

// Rect/point/string helpers of the rallymenu.ini loader (same shapes as the soundmenu.ini loader in unk_450ee0.cpp).
struct TSG_Point {
    TSG_Point(const TSG_Point& other);     // 0x424300
    ~TSG_Point() {}
    float x, y;
};
class TSG_Rect {
public:
    TSG_Rect(TSG_Point topLeft, HudPoint bottomRight);   // 0x4390a0
    float f[5];
};
class TSG_String {
public:
    TSG_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~TSG_String() { Free(true); }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};
#define TSG_ADD(ul, lr) \
    do { \
        rect = (UIRect*)new TSG_Rect(*(const TSG_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)
extern ConfigKey DAT_004f94d8[];       // rallymenu.ini key table (16 keys)

// Layout values of the 1024x768 design space (0x4e1878, 0x4e187c, 0x4e188c). VC6 moves a literal to the right operand of
// a commutative multiply; the original multiplies literal * scale, so they are plain (non-const) globals here.
static float TSG_DESC_Y = 450.0f;
static float TSG_DESC_X = 575.0f;
static float TSG_DESC_W = 365.0f;

// FUNCTION: RALLYTROPHY 0x441370
// SYMBOL: _$E21
typedef void Generated_441370();

// FUNCTION: RALLYTROPHY 0x441380
// SYMBOL: _$E18
typedef void Generated_441380();

// FUNCTION: RALLYTROPHY 0x441390
// SYMBOL: _$E20
typedef void Generated_441390();

// FUNCTION: RALLYTROPHY 0x4413a0
// SYMBOL: _$E19
typedef void Generated_4413a0();

// FUNCTION: RALLYTROPHY 0x4413b0
TrackSelectMenu::TrackSelectMenu()
{
    curRally = 0;
    curStage = 0;
    lastStage = 0;
    lastStageArcade = 0;
    mode = 0;
    trackBook = 0;
    m_selected = 0;
    rallyCount = 3;
}

// FUNCTION: RALLYTROPHY 0x4414c0
void TrackSelectMenu::Slot3(int unused)
{
}

// FUNCTION: RALLYTROPHY 0x4414d0
// SYMBOL: ??_GTrackSelectMenu@@UAEPAXI@Z
typedef void Generated_4414d0();

// FUNCTION: RALLYTROPHY 0x4414f0
// SYMBOL: ??0RallyInfo@@QAE@XZ
// Implicit constructor of RallyInfo (two strings, seven std::string[12] arrays).
typedef void Generated_4414f0();

// FUNCTION: RALLYTROPHY 0x441620
// SYMBOL: ??1RallyInfo@@QAE@XZ
// Implicit destructor of RallyInfo.
typedef void Generated_441620();

// ---- module end (TS-A): the STL header statics' guard initialiser and atexit registration ----

// FUNCTION: RALLYTROPHY 0x441760
TrackSelectMenu::~TrackSelectMenu()
{
    Release();
}

// NONMATCHING: RALLYTROPHY 0x4418d0
// 4714 vs 4683 bytes; the call sequence, EH state numbering and frame size (0x150) match. Left: the original keeps the
// rally index in ebp and the RallyInfo pointer in esi (mine swaps them, so ebp is pushed lazily), and some frame slots
// and store schedules follow from that. original: FUN_004418d0 (vtable slot 1, ret 4)
void TrackSelectMenu::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    TC_S(title).AssignA(FUN_00405380(0x101));
    unk40 = -1;
    textScaleX = sx * 32.0f;
    textScaleY = sy * 16.0f;
    TC_S(unk44).AssignA(FUN_00405380(0x17));
    TC_S(unk54).AssignA(FUN_00405380(0x18));
    TC_S(unk64).AssignA(FUN_00405380(0x19));
    TC_S(unk74).AssignA(FUN_00405380(0x1a));
    TC_S(labels[0]).AssignA(FUN_00405380(0xf9));
    TC_S(labels[1]).AssignA(FUN_00405380(0xfa));
    TC_S(labels[2]).AssignA(FUN_00405380(0xfb));
    TC_S(labels[3]).AssignA(FUN_00405380(0xfe));
    TC_S(labels[4]).AssignA(FUN_00405380(0xff));
    TC_S(labels[5]).AssignA(DAT_005070b0);
    for (int r = 0; r < rallyCount; r++) {
        Config config;
        config.Define(DAT_004f92c8, 0x42);
        char number[32];
        char amountText[32];
        _itoa(r + 1, number, 10);
        TC_Local path("data/tracks/track");
        TC_S(path).AppendA(number);
        std::string full(*TC_STR(path));
        TC_S(full).AppendA("/data/info.ini");
        config.Load(full.c_str(), true, false, 0);
        RallyInfo* info = &rally[r];
        TC_S(info->name).AssignA(FUN_00405380(0xb5 + r));
        TC_S(info->description).AssignB(FUN_00405380(0xba + r));
        info->numStages = config.GetInt("NumberOfStages");
        info->numNovice = config.GetInt("NumberOfNoviceStages");
        info->numIntermediate = config.GetInt("NumberOfIntermediateStages");
        info->numExpert = config.GetInt("NumberOfExpertStages");
        info->numArcade = config.GetInt("NumberOfArcadeStages");
        info->firstArcade = config.GetInt("FirstArcadeStage") - 1;
        for (int k = 0; k < info->numStages; k++) {
            TC_S(info->stageName[k]).AssignB(FUN_00405380(r * 10 + k + 0xbf));
            TC_Local section("Stage");
            _itoa(k + 1, number, 10);
            TC_S(section).AppendA(number);
            std::string key;
            TC_S(info->lengthText[k]).AssignB(DAT_005070b0);
            TC_ASSIGN_PLUS(key, section, "Length")
            info->length[k] = config.GetFloat(key.c_str());
            if (r == 0 && k == 5) {
                info->length[5] = 4.8f;
            }
            TC_ASSIGN_PLUS(key, section, "Time")
            int t = config.GetInt(key.c_str());
            _itoa(t / 100, number, 10);
            TC_S(info->timeText[k]).AssignB(number);
            TC_S(info->timeText[k]).AppendNC(1, '.');
            TC_S(info->timeText[k]).AppendNC(1, (char)('0' + t / 10 % 10));
            TC_S(info->timeText[k]).AppendNC(1, (char)('0' + (t & 1)));
            TC_ASSIGN_PLUS(key, section, "Surface1Type")
            int type1 = config.GetInt(key.c_str());
            TC_ASSIGN_PLUS(key, section, "Surface1Amount")
            int amount1 = config.GetInt(key.c_str());
            TC_S(info->surfaceText[k]).AssignB(DAT_005070b0);
            if (amount1 > 0) {
                _itoa(amount1, amountText, 10);
                TC_S(info->surfaceText[k]).AssignB(amountText);
                TC_S(info->surfaceText[k]).AppendA("% ");
                TC_S(info->surfaceText[k]).AppendA(FUN_00405380(type1 + 0xf1));
                TC_S(info->surfaceText[k]).AppendA("\n");
            }
            TC_ASSIGN_PLUS(key, section, "Surface2Type")
            int type2 = config.GetInt(key.c_str());
            TC_ASSIGN_PLUS(key, section, "Surface2Amount")
            int amount2 = config.GetInt(key.c_str());
            if (amount2 > 0) {
                _itoa(amount2, amountText, 10);
                TC_S(info->surfaceText[k]).AppendD(amountText);
                TC_S(info->surfaceText[k]).AppendD("% ");
                TC_S(info->surfaceText[k]).AppendD(FUN_00405380(type2 + 0xf1));
            }
            TC_S(info->recordSpeed[k]).AssignC(DAT_005070b0);
            info->stageInfo[k] = 0;
            if (!DAT_00501454) {
                info->stageInfo[k] = new Sprite;
                TC_ASSIGN_PLUS(full, path, "/textures/StageInfo")
                _itoa(k + 1, number, 10);
                TC_S(full).AppendD(number);
                TC_S(full).AppendD(".tga");
                Sprite* sp = info->stageInfo[k];
                sp->texture = new Texture(full.c_str(), 0x841, 0);
                sp->texture->Upload(DAT_0056b0ac);
                sp->unk28 = 3;
                sp->unk30 = 3;
                sp->unk40[0] = 1.0f;
                sp->unk40[1] = 1.0f;
                sp->unk40[2] = 1.0f;
                sp->unk40[3] = 1.0f;
                sp->unk60[0] = 1.0f;
                sp->unk60[1] = 1.0f;
                sp->unk60[2] = 1.0f;
                sp->unk60[3] = 1.0f;
                sp->unk70[0] = 0;
                sp->unk70[1] = 0;
                sp->unk70[2] = 0;
                sp->unk70[3] = 0;
                sp->unk9c = 0xe;
                sp->unk18 = 5;
                sp->unk1c = 6;
                sp->unk20 = 2;
                sp->unk24 = 4;
                *(int*)sp->unk34 = 1;
                sp->unk98 = 0xf;
                sp->unk50[0] = 0;
                sp->unk50[1] = 0;
                sp->unk50[2] = 0;
                sp->unk50[3] = 0;
            }
        }
        info->photo = 0;
        if (!DAT_00501454) {
            info->photo = new Sprite;
            TC_S(full).AssignStr(*TC_STR(path) + "/textures/InfoPhotoWire.tga", 0, std::string::npos);
            Sprite* sp = info->photo;
            sp->texture = new Texture(full.c_str(), 0x841, 0);
            sp->texture->Upload(DAT_0056b0ac);
            TC_S(full).AssignStr(*TC_STR(path) + "/textures/InfoPhotoFull.tga", 0, std::string::npos);
            *(Texture**)sp->unk04 = new Texture(full.c_str(), 0x841, 0);
            (*(Texture**)sp->unk04)->Upload(DAT_0056b0ac);
            sp->unk40[0] = 1.0f;
            sp->unk40[1] = 1.0f;
            sp->unk40[2] = 1.0f;
            sp->unk40[3] = 1.0f;
            sp->unk60[0] = 1.0f;
            sp->unk60[1] = 1.0f;
            sp->unk60[2] = 1.0f;
            sp->unk60[3] = 1.0f;
            sp->unk70[0] = 0;
            sp->unk70[1] = 0;
            sp->unk70[2] = 0;
            sp->unk70[3] = 0;
            sp->unk38 = 0x10;
            sp->unk9c = 0x3e;
            sp->unk18 = 5;
            sp->unk1c = 6;
            sp->unk98 = 0xf;
            sp->unk50[0] = 0;
            sp->unk50[1] = 0;
            sp->unk50[2] = 0;
            sp->unk50[3] = 0;
        }
    }
    if (!DAT_00501454) {
        trackBook = new Sprite;
        Sprite* sp = trackBook;
        sp->texture = new Texture("data/menu/TrackBook.tga", 0x841, 0);
        sp->texture->Upload(DAT_0056b0ac);
        sp->unk60[0] = 1.0f;
        sp->unk60[1] = 1.0f;
        sp->unk60[2] = 1.0f;
        sp->unk60[3] = 1.0f;
        sp->unk9c = 0xe;
        sp->unk98 = 0xf;
        sp->unk40[0] = 1.0f;
        sp->unk40[1] = 1.0f;
        sp->unk40[2] = 1.0f;
        sp->unk40[3] = 1.0f;
        sp->unk18 = 5;
        sp->unk1c = 6;
        sp->unk24 = 2;
    }
    m_state = 1;
    m_game = game;
    BuildRects();
}

__declspec(naked)
// PE transcription preserves the two inline string terminator stores' original base/index encoding.
// FUNCTION: RALLYTROPHY 0x442b20
void TrackSelectMenu::LoadRallyData()
{
    __asm {
        _emit 055h
        _emit 08bh
        _emit 0ech
        _emit 06ah
        _emit 0ffh

        _emit 068h
        _emit 039h
        _emit 092h
        _emit 04dh
        _emit 000h

        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 050h
        _emit 064h
        _emit 089h
        _emit 025h

        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 081h

        _emit 0ech
        _emit 0c4h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 053h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh

        _emit 086h
        _emit 0e4h
        _emit 01eh
        _emit 000h
        _emit 000h

        _emit 033h
        _emit 0dbh
        _emit 085h
        _emit 0c0h
        _emit 089h

        _emit 075h
        _emit 0c4h
        _emit 089h
        _emit 05dh
        _emit 0d0h

        _emit 00fh
        _emit 08eh
        _emit 013h
        _emit 009h
        _emit 000h

        _emit 000h
        _emit 057h
        _emit 0ebh
        _emit 006h
        _emit 08bh

        _emit 05dh
        _emit 0d0h
        _emit 08bh
        _emit 075h
        _emit 0c4h

        _emit 08dh
        _emit 08dh
        _emit 040h
        _emit 0ffh
        _emit 0ffh

        _emit 0ffh
        _emit 0e8h
        _emit 014h
        _emit 02ah
        _emit 0fch

        _emit 0ffh
        _emit 06ah
        _emit 042h
        _emit 068h
        _emit 0c8h

        _emit 092h
        _emit 04fh
        _emit 000h
        _emit 08dh
        _emit 08dh

        _emit 040h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0c7h

        _emit 045h
        _emit 0fch
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 0e8h
        _emit 03bh
        _emit 04bh
        _emit 0fch

        _emit 0ffh
        _emit 06ah
        _emit 00ah
        _emit 08dh
        _emit 08dh

        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 08dh

        _emit 043h
        _emit 001h
        _emit 051h
        _emit 050h
        _emit 089h

        _emit 085h
        _emit 058h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 0e8h
        _emit 065h
        _emit 0edh
        _emit 007h
        _emit 000h

        _emit 08ah
        _emit 055h
        _emit 0ebh
        _emit 083h
        _emit 0c4h

        _emit 00ch
        _emit 06ah
        _emit 000h
        _emit 08dh
        _emit 04dh

        _emit 0b4h
        _emit 088h
        _emit 055h
        _emit 0b4h
        _emit 0e8h

        _emit 050h
        _emit 0fbh
        _emit 0fbh
        _emit 0ffh
        _emit 083h

        _emit 0c9h
        _emit 0ffh
        _emit 033h
        _emit 0c0h
        _emit 0bfh

        _emit 0c0h
        _emit 050h
        _emit 04fh
        _emit 000h
        _emit 0f2h

        _emit 0aeh
        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 051h

        _emit 068h
        _emit 0c0h
        _emit 050h
        _emit 04fh
        _emit 000h

        _emit 08dh
        _emit 04dh
        _emit 0b4h
        _emit 0e8h
        _emit 0d3h

        _emit 01fh
        _emit 0fch
        _emit 0ffh
        _emit 083h
        _emit 0c9h

        _emit 0ffh
        _emit 033h
        _emit 0c0h
        _emit 08dh
        _emit 0bdh

        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0f2h

        _emit 0aeh
        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 051h

        _emit 08dh
        _emit 085h
        _emit 074h
        _emit 0ffh
        _emit 0ffh

        _emit 0ffh
        _emit 050h
        _emit 08dh
        _emit 04dh
        _emit 0b4h

        _emit 0c6h
        _emit 045h
        _emit 0fch
        _emit 001h
        _emit 0e8h

        _emit 0afh
        _emit 00fh
        _emit 0fch
        _emit 0ffh
        _emit 08ah

        _emit 04dh
        _emit 0b4h
        _emit 088h
        _emit 04dh
        _emit 094h

        _emit 06ah
        _emit 000h
        _emit 08dh
        _emit 04dh
        _emit 094h

        _emit 0e8h
        _emit 0ffh
        _emit 0fah
        _emit 0fbh
        _emit 0ffh

        _emit 08bh
        _emit 015h
        _emit 040h
        _emit 013h
        _emit 04eh

        _emit 000h
        _emit 052h
        _emit 06ah
        _emit 000h
        _emit 08dh

        _emit 045h
        _emit 0b4h
        _emit 050h
        _emit 08dh
        _emit 04dh

        _emit 094h
        _emit 0e8h
        _emit 0dah
        _emit 0f8h
        _emit 0fbh

        _emit 0ffh
        _emit 083h
        _emit 0c9h
        _emit 0ffh
        _emit 033h

        _emit 0c0h
        _emit 0bfh
        _emit 070h
        _emit 09ch
        _emit 04fh

        _emit 000h
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h
        _emit 0d1h

        _emit 049h
        _emit 051h
        _emit 068h
        _emit 070h
        _emit 09ch

        _emit 04fh
        _emit 000h
        _emit 08dh
        _emit 04dh
        _emit 094h

        _emit 0c6h
        _emit 045h
        _emit 0fch
        _emit 002h
        _emit 0e8h

        _emit 069h
        _emit 00fh
        _emit 0fch
        _emit 0ffh
        _emit 08bh

        _emit 045h
        _emit 098h
        _emit 085h
        _emit 0c0h
        _emit 075h

        _emit 005h
        _emit 0b8h
        _emit 044h
        _emit 013h
        _emit 04eh

        _emit 000h
        _emit 06ah
        _emit 000h
        _emit 06ah
        _emit 000h

        _emit 06ah
        _emit 001h
        _emit 050h
        _emit 08dh
        _emit 08dh

        _emit 040h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h

        _emit 08bh
        _emit 040h
        _emit 0fch
        _emit 0ffh
        _emit 08bh

        _emit 0c3h
        _emit 0c1h
        _emit 0e0h
        _emit 006h
        _emit 02bh

        _emit 0c3h
        _emit 08dh
        _emit 00ch
        _emit 040h
        _emit 08dh

        _emit 084h
        _emit 0ceh
        _emit 048h
        _emit 001h
        _emit 000h

        _emit 000h
        _emit 08bh
        _emit 048h
        _emit 020h
        _emit 033h

        _emit 0f6h
        _emit 033h
        _emit 0ffh
        _emit 03bh
        _emit 0cfh

        _emit 089h
        _emit 045h
        _emit 0c8h
        _emit 089h
        _emit 075h

        _emit 0d8h
        _emit 00fh
        _emit 08eh
        _emit 06dh
        _emit 007h

        _emit 000h
        _emit 000h
        _emit 0ebh
        _emit 008h
        _emit 08bh

        _emit 075h
        _emit 0d8h
        _emit 08bh
        _emit 05dh
        _emit 0d0h

        _emit 033h
        _emit 0ffh
        _emit 08ah
        _emit 055h
        _emit 0ebh

        _emit 057h
        _emit 08dh
        _emit 04dh
        _emit 0a4h
        _emit 088h

        _emit 055h
        _emit 0a4h
        _emit 0e8h
        _emit 06ch
        _emit 0fah

        _emit 0fbh
        _emit 0ffh
        _emit 083h
        _emit 0c9h
        _emit 0ffh

        _emit 033h
        _emit 0c0h
        _emit 0bfh
        _emit 068h
        _emit 09ch

        _emit 04fh
        _emit 000h
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h

        _emit 0d1h
        _emit 049h
        _emit 051h
        _emit 068h
        _emit 068h

        _emit 09ch
        _emit 04fh
        _emit 000h
        _emit 08dh
        _emit 04dh

        _emit 0a4h
        _emit 0e8h
        _emit 0efh
        _emit 01eh
        _emit 0fch

        _emit 0ffh
        _emit 06ah
        _emit 00ah
        _emit 08dh
        _emit 08dh

        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 08dh

        _emit 046h
        _emit 001h
        _emit 051h
        _emit 050h
        _emit 0c6h

        _emit 045h
        _emit 0fch
        _emit 003h
        _emit 089h
        _emit 085h

        _emit 054h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h

        _emit 035h
        _emit 0ech
        _emit 007h
        _emit 000h
        _emit 083h

        _emit 0c9h
        _emit 0ffh
        _emit 083h
        _emit 0c4h
        _emit 00ch

        _emit 033h
        _emit 0c0h
        _emit 08dh
        _emit 0bdh
        _emit 074h

        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0f2h
        _emit 0aeh

        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 051h
        _emit 08dh

        _emit 095h
        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 052h
        _emit 08dh
        _emit 04dh
        _emit 0a4h
        _emit 0e8h

        _emit 0b0h
        _emit 00eh
        _emit 0fch
        _emit 0ffh
        _emit 08ah

        _emit 045h
        _emit 0ebh
        _emit 06ah
        _emit 000h
        _emit 08dh

        _emit 08dh
        _emit 064h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 088h
        _emit 085h
        _emit 064h
        _emit 0ffh
        _emit 0ffh

        _emit 0ffh
        _emit 0e8h
        _emit 0fah
        _emit 0f9h
        _emit 0fbh

        _emit 0ffh
        _emit 068h
        _emit 0bch
        _emit 063h
        _emit 04fh

        _emit 000h
        _emit 08dh
        _emit 04dh
        _emit 0a4h
        _emit 051h

        _emit 08dh
        _emit 095h
        _emit 030h
        _emit 0ffh
        _emit 0ffh

        _emit 0ffh
        _emit 052h
        _emit 0c6h
        _emit 045h
        _emit 0fch

        _emit 004h
        _emit 0e8h
        _emit 011h
        _emit 064h
        _emit 0fch

        _emit 0ffh
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 08bh

        _emit 00dh
        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h

        _emit 051h
        _emit 06ah
        _emit 000h
        _emit 050h
        _emit 08dh

        _emit 08dh
        _emit 064h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 0c6h
        _emit 045h
        _emit 0fch
        _emit 005h
        _emit 0e8h

        _emit 0b5h
        _emit 0f7h
        _emit 0fbh
        _emit 0ffh
        _emit 06ah

        _emit 001h
        _emit 08dh
        _emit 08dh
        _emit 030h
        _emit 0ffh

        _emit 0ffh
        _emit 0ffh
        _emit 0c6h
        _emit 045h
        _emit 0fch

        _emit 004h
        _emit 0e8h
        _emit 0b4h
        _emit 0f9h
        _emit 0fbh

        _emit 0ffh
        _emit 08bh
        _emit 085h
        _emit 068h
        _emit 0ffh

        _emit 0ffh
        _emit 0ffh
        _emit 085h
        _emit 0c0h
        _emit 075h

        _emit 005h
        _emit 0b8h
        _emit 044h
        _emit 013h
        _emit 04eh

        _emit 000h
        _emit 050h
        _emit 08dh
        _emit 08dh
        _emit 040h

        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h
        _emit 0f9h

        _emit 051h
        _emit 0fch
        _emit 0ffh
        _emit 085h
        _emit 0dbh

        _emit 075h
        _emit 00dh
        _emit 083h
        _emit 0feh
        _emit 005h

        _emit 075h
        _emit 008h
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h

        _emit 005h
        _emit 0d4h
        _emit 018h
        _emit 04eh
        _emit 000h

        _emit 0a1h
        _emit 000h
        _emit 013h
        _emit 050h
        _emit 000h

        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 006h
        _emit 0d8h

        _emit 00dh
        _emit 0d0h
        _emit 018h
        _emit 04eh
        _emit 000h

        _emit 0ddh
        _emit 05dh
        _emit 0dch
        _emit 08bh
        _emit 07dh

        _emit 0e0h
        _emit 08bh
        _emit 05dh
        _emit 0dch
        _emit 057h

        _emit 053h
        _emit 0e8h
        _emit 06dh
        _emit 0e6h
        _emit 007h

        _emit 000h
        _emit 0d9h
        _emit 05dh
        _emit 0f0h
        _emit 0d9h

        _emit 045h
        _emit 0f0h
        _emit 0dbh
        _emit 09dh
        _emit 05ch

        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 057h
        _emit 053h

        _emit 0e8h
        _emit 05ah
        _emit 0e6h
        _emit 007h
        _emit 000h

        _emit 0dch
        _emit 06dh
        _emit 0dch
        _emit 083h
        _emit 0c4h

        _emit 010h
        _emit 0dch
        _emit 00dh
        _emit 0c8h
        _emit 018h

        _emit 04eh
        _emit 000h
        _emit 0d9h
        _emit 05dh
        _emit 0f0h

        _emit 0d9h
        _emit 045h
        _emit 0f0h
        _emit 0dbh
        _emit 09dh

        _emit 060h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh

        _emit 085h
        _emit 060h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 083h
        _emit 0f8h
        _emit 00ah
        _emit 075h
        _emit 005h

        _emit 0b8h
        _emit 009h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 08bh
        _emit 015h
        _emit 088h
        _emit 09ch
        _emit 04fh

        _emit 000h
        _emit 08bh
        _emit 08dh
        _emit 05ch
        _emit 0ffh

        _emit 0ffh
        _emit 0ffh
        _emit 08ah
        _emit 080h
        _emit 0b8h

        _emit 092h
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 055h

        _emit 0e4h
        _emit 08ah
        _emit 091h
        _emit 0b8h
        _emit 092h

        _emit 04fh
        _emit 000h
        _emit 088h
        _emit 055h
        _emit 0e4h

        _emit 08bh
        _emit 055h
        _emit 0c8h
        _emit 08bh
        _emit 0ceh

        _emit 0c1h
        _emit 0e1h
        _emit 004h
        _emit 08dh
        _emit 01ch

        _emit 011h
        _emit 083h
        _emit 0c9h
        _emit 0ffh
        _emit 088h

        _emit 045h
        _emit 0e6h
        _emit 033h
        _emit 0c0h
        _emit 089h

        _emit 05dh
        _emit 0d4h
        _emit 081h
        _emit 0c3h
        _emit 028h

        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 07dh

        _emit 0e4h
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h
        _emit 0d1h

        _emit 049h
        _emit 08bh
        _emit 0f9h
        _emit 06ah
        _emit 001h

        _emit 057h
        _emit 08bh
        _emit 0cbh
        _emit 089h
        _emit 07dh

        _emit 0f0h
        _emit 0e8h
        _emit 05dh
        _emit 0fbh
        _emit 0fbh

        _emit 0ffh
        _emit 084h
        _emit 0c0h
        _emit 074h
        _emit 025h

        _emit 08bh
        _emit 043h
        _emit 004h
        _emit 08bh
        _emit 0cfh

        _emit 08bh
        _emit 0f8h
        _emit 08bh
        _emit 0c1h
        _emit 0c1h

        _emit 0e9h
        _emit 002h
        _emit 08dh
        _emit 075h
        _emit 0e4h

        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0c8h
        _emit 083h

        _emit 0e1h
        _emit 003h
        _emit 0f3h
        _emit 0a4h
        _emit 08bh

        _emit 0c8h
        _emit 051h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h

        _emit 017h
        _emit 0fbh
        _emit 0fbh
        _emit 0ffh
        _emit 08bh

        _emit 075h
        _emit 0d8h
        _emit 0a1h
        _emit 000h
        _emit 013h

        _emit 050h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h

        _emit 020h
        _emit 083h
        _emit 0c9h
        _emit 0ffh
        _emit 033h

        _emit 0c0h
        _emit 0bfh
        _emit 040h
        _emit 02eh
        _emit 04fh

        _emit 000h
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h
        _emit 0d1h

        _emit 049h
        _emit 051h
        _emit 068h
        _emit 040h
        _emit 02eh

        _emit 04fh
        _emit 000h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h

        _emit 02fh
        _emit 00dh
        _emit 0fch
        _emit 0ffh
        _emit 06ah

        _emit 00fh
        _emit 0ebh
        _emit 002h
        _emit 06ah
        _emit 00eh

        _emit 0e8h
        _emit 004h
        _emit 025h
        _emit 0fch
        _emit 0ffh

        _emit 08bh
        _emit 0d0h
        _emit 083h
        _emit 0c9h
        _emit 0ffh

        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 033h
        _emit 0c0h

        _emit 08bh
        _emit 0fah
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h

        _emit 0d1h
        _emit 049h
        _emit 051h
        _emit 052h
        _emit 08bh

        _emit 0cbh
        _emit 0e8h
        _emit 00ah
        _emit 00dh
        _emit 0fch

        _emit 0ffh
        _emit 08bh
        _emit 045h
        _emit 0c4h
        _emit 08bh

        _emit 055h
        _emit 0d0h
        _emit 08bh
        _emit 048h
        _emit 00ch

        _emit 06ah
        _emit 000h
        _emit 06ah
        _emit 000h
        _emit 056h

        _emit 052h
        _emit 081h
        _emit 0c1h
        _emit 030h
        _emit 05ch

        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 020h
        _emit 0d1h

        _emit 004h
        _emit 000h
        _emit 08bh
        _emit 0f0h
        _emit 0a1h

        _emit 058h
        _emit 014h
        _emit 050h
        _emit 000h
        _emit 085h

        _emit 0c0h
        _emit 089h
        _emit 075h
        _emit 0e4h
        _emit 074h

        _emit 005h
        _emit 08bh
        _emit 046h
        _emit 058h
        _emit 0ebh

        _emit 003h
        _emit 08bh
        _emit 046h
        _emit 054h
        _emit 08dh

        _emit 08dh
        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 051h
        _emit 050h
        _emit 089h
        _emit 045h
        _emit 0f0h

        _emit 0e8h
        _emit 0fah
        _emit 0f2h
        _emit 003h
        _emit 000h

        _emit 08bh
        _emit 055h
        _emit 0d4h
        _emit 083h
        _emit 0c9h

        _emit 0ffh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 033h

        _emit 0c0h
        _emit 08dh
        _emit 0bdh
        _emit 074h
        _emit 0ffh

        _emit 0ffh
        _emit 0ffh
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h

        _emit 0d1h
        _emit 049h
        _emit 08bh
        _emit 0f9h
        _emit 08dh

        _emit 09ah
        _emit 068h
        _emit 003h
        _emit 000h
        _emit 000h

        _emit 06ah
        _emit 001h
        _emit 057h
        _emit 08bh
        _emit 0cbh

        _emit 089h
        _emit 07dh
        _emit 0ech
        _emit 0e8h
        _emit 07fh

        _emit 0fah
        _emit 0fbh
        _emit 0ffh
        _emit 084h
        _emit 0c0h

        _emit 074h
        _emit 028h
        _emit 08bh
        _emit 043h
        _emit 004h

        _emit 08bh
        _emit 0cfh
        _emit 08bh
        _emit 0f8h
        _emit 08bh

        _emit 0c1h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 08dh

        _emit 0b5h
        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0c8h
        _emit 083h

        _emit 0e1h
        _emit 003h
        _emit 0f3h
        _emit 0a4h
        _emit 08bh

        _emit 0c8h
        _emit 051h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h

        _emit 036h
        _emit 0fah
        _emit 0fbh
        _emit 0ffh
        _emit 08bh

        _emit 075h
        _emit 0e4h
        _emit 08bh
        _emit 045h
        _emit 0d8h

        _emit 08bh
        _emit 055h
        _emit 0c8h
        _emit 0d9h
        _emit 044h

        _emit 082h
        _emit 038h
        _emit 0a1h
        _emit 000h
        _emit 013h

        _emit 050h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 0dbh

        _emit 045h
        _emit 0f0h
        _emit 08bh
        _emit 04eh
        _emit 05ch

        _emit 0d8h
        _emit 00dh
        _emit 0c4h
        _emit 018h
        _emit 04eh

        _emit 000h
        _emit 089h
        _emit 04dh
        _emit 0cch
        _emit 0deh

        _emit 0f9h
        _emit 0d9h
        _emit 05dh
        _emit 0ech
        _emit 074h

        _emit 021h
        _emit 0d9h
        _emit 045h
        _emit 0ech
        _emit 0c7h

        _emit 045h
        _emit 0f0h
        _emit 00dh
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 0d8h
        _emit 00dh
        _emit 0d0h
        _emit 018h

        _emit 04eh
        _emit 000h
        _emit 0d9h
        _emit 05dh
        _emit 0ech

        _emit 0d9h
        _emit 045h
        _emit 0cch
        _emit 0d8h
        _emit 00dh

        _emit 0d0h
        _emit 018h
        _emit 04eh
        _emit 000h
        _emit 0d9h

        _emit 05dh
        _emit 0cch
        _emit 0ebh
        _emit 007h
        _emit 0c7h

        _emit 045h
        _emit 0f0h
        _emit 00ch
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 08bh
        _emit 055h
        _emit 0ech
        _emit 052h

        _emit 08dh
        _emit 085h
        _emit 074h
        _emit 0ffh
        _emit 0ffh

        _emit 0ffh
        _emit 050h
        _emit 0e8h
        _emit 074h
        _emit 0e0h

        _emit 0fbh
        _emit 0ffh
        _emit 083h
        _emit 0c9h
        _emit 0ffh

        _emit 033h
        _emit 0c0h
        _emit 083h
        _emit 0c4h
        _emit 008h

        _emit 0bfh
        _emit 084h
        _emit 09ch
        _emit 04fh
        _emit 000h

        _emit 0f2h
        _emit 0aeh
        _emit 08bh
        _emit 07bh
        _emit 008h

        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 08bh
        _emit 0f1h

        _emit 08bh
        _emit 00dh
        _emit 040h
        _emit 013h
        _emit 04eh

        _emit 000h
        _emit 02bh
        _emit 0cfh
        _emit 03bh
        _emit 0ceh

        _emit 077h
        _emit 005h
        _emit 0e8h
        _emit 057h
        _emit 0a0h

        _emit 007h
        _emit 000h
        _emit 085h
        _emit 0f6h
        _emit 076h

        _emit 03ch
        _emit 08dh
        _emit 004h
        _emit 037h
        _emit 06ah

        _emit 000h
        _emit 050h
        _emit 08bh
        _emit 0cbh
        _emit 089h

        _emit 045h
        _emit 0ech
        _emit 0e8h
        _emit 0b8h
        _emit 0f9h

        _emit 0fbh
        _emit 0ffh
        _emit 084h
        _emit 0c0h
        _emit 074h

        _emit 028h
        _emit 08bh
        _emit 04bh
        _emit 008h
        _emit 08bh

        _emit 07bh
        _emit 004h
        _emit 08bh
        _emit 045h
        _emit 0ech

        _emit 003h
        _emit 0f9h
        _emit 08bh
        _emit 0ceh
        _emit 08bh

        _emit 0d1h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 0beh

        _emit 084h
        _emit 09ch
        _emit 04fh
        _emit 000h
        _emit 0f3h

        _emit 0a5h
        _emit 08bh
        _emit 0cah
        _emit 083h
        _emit 0e1h

        _emit 003h
        _emit 0f3h
        _emit 0a4h
        _emit 050h
        _emit 08bh

        _emit 0cbh
        _emit 0e8h
        _emit 06ch
        _emit 0f9h
        _emit 0fbh

        _emit 0ffh
        _emit 083h
        _emit 0c9h
        _emit 0ffh
        _emit 033h

        _emit 0c0h
        _emit 08dh
        _emit 0bdh
        _emit 074h
        _emit 0ffh

        _emit 0ffh
        _emit 0ffh
        _emit 0f2h
        _emit 0aeh
        _emit 08bh

        _emit 07bh
        _emit 008h
        _emit 0f7h
        _emit 0d1h
        _emit 049h

        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 00dh
        _emit 040h

        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 02bh
        _emit 0cfh

        _emit 03bh
        _emit 0ceh
        _emit 077h
        _emit 005h
        _emit 0e8h

        _emit 0f1h
        _emit 09fh
        _emit 007h
        _emit 000h
        _emit 085h

        _emit 0f6h
        _emit 076h
        _emit 03dh
        _emit 08dh
        _emit 004h

        _emit 037h
        _emit 06ah
        _emit 000h
        _emit 050h
        _emit 08bh

        _emit 0cbh
        _emit 089h
        _emit 045h
        _emit 0ech
        _emit 0e8h

        _emit 052h
        _emit 0f9h
        _emit 0fbh
        _emit 0ffh
        _emit 084h

        _emit 0c0h
        _emit 074h
        _emit 029h
        _emit 08bh
        _emit 04bh

        _emit 008h
        _emit 08bh
        _emit 07bh
        _emit 004h
        _emit 08bh

        _emit 045h
        _emit 0ech
        _emit 003h
        _emit 0f9h
        _emit 08bh

        _emit 0ceh
        _emit 08bh
        _emit 0d1h
        _emit 0c1h
        _emit 0e9h

        _emit 002h
        _emit 08dh
        _emit 0b5h
        _emit 074h
        _emit 0ffh

        _emit 0ffh
        _emit 0ffh
        _emit 0f3h
        _emit 0a5h
        _emit 08bh

        _emit 0cah
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 0f3h

        _emit 0a4h
        _emit 050h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h

        _emit 005h
        _emit 0f9h
        _emit 0fbh
        _emit 0ffh
        _emit 033h

        _emit 0c0h
        _emit 083h
        _emit 0c9h
        _emit 0ffh
        _emit 0bfh

        _emit 040h
        _emit 02eh
        _emit 04fh
        _emit 000h
        _emit 0f2h

        _emit 0aeh
        _emit 08bh
        _emit 07bh
        _emit 008h
        _emit 0a1h

        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0f7h

        _emit 0d1h
        _emit 049h
        _emit 08bh
        _emit 0f1h
        _emit 02bh

        _emit 0c7h
        _emit 03bh
        _emit 0c6h
        _emit 077h
        _emit 005h

        _emit 0e8h
        _emit 08ch
        _emit 09fh
        _emit 007h
        _emit 000h

        _emit 085h
        _emit 0f6h
        _emit 076h
        _emit 03ch
        _emit 08dh

        _emit 004h
        _emit 037h
        _emit 06ah
        _emit 000h
        _emit 050h

        _emit 08bh
        _emit 0cbh
        _emit 089h
        _emit 045h
        _emit 0ech

        _emit 0e8h
        _emit 0edh
        _emit 0f8h
        _emit 0fbh
        _emit 0ffh

        _emit 084h
        _emit 0c0h
        _emit 074h
        _emit 028h
        _emit 08bh

        _emit 04bh
        _emit 008h
        _emit 08bh
        _emit 07bh
        _emit 004h

        _emit 08bh
        _emit 045h
        _emit 0ech
        _emit 003h
        _emit 0f9h

        _emit 08bh
        _emit 0ceh
        _emit 08bh
        _emit 0d1h
        _emit 0c1h

        _emit 0e9h
        _emit 002h
        _emit 0beh
        _emit 040h
        _emit 02eh

        _emit 04fh
        _emit 000h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh

        _emit 0cah
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 0f3h

        _emit 0a4h
        _emit 050h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h

        _emit 0a1h
        _emit 0f8h
        _emit 0fbh
        _emit 0ffh
        _emit 08bh

        _emit 04dh
        _emit 0f0h
        _emit 051h
        _emit 0e8h
        _emit 0b8h

        _emit 022h
        _emit 0fch
        _emit 0ffh
        _emit 08bh
        _emit 015h

        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 083h

        _emit 0c9h
        _emit 0ffh
        _emit 08bh
        _emit 0f8h
        _emit 083h

        _emit 0c4h
        _emit 004h
        _emit 089h
        _emit 045h
        _emit 0ech

        _emit 033h
        _emit 0c0h
        _emit 0f2h
        _emit 0aeh
        _emit 08bh

        _emit 07bh
        _emit 008h
        _emit 0f7h
        _emit 0d1h
        _emit 049h

        _emit 08bh
        _emit 0f1h
        _emit 02bh
        _emit 0d7h
        _emit 03bh

        _emit 0d6h
        _emit 077h
        _emit 005h
        _emit 0e8h
        _emit 01bh

        _emit 09fh
        _emit 007h
        _emit 000h
        _emit 085h
        _emit 0f6h

        _emit 076h
        _emit 03ah
        _emit 08dh
        _emit 004h
        _emit 037h

        _emit 06ah
        _emit 000h
        _emit 050h
        _emit 08bh
        _emit 0cbh

        _emit 089h
        _emit 045h
        _emit 0e0h
        _emit 0e8h
        _emit 07ch

        _emit 0f8h
        _emit 0fbh
        _emit 0ffh
        _emit 084h
        _emit 0c0h

        _emit 074h
        _emit 026h
        _emit 08bh
        _emit 043h
        _emit 008h

        _emit 08bh
        _emit 07bh
        _emit 004h
        _emit 08bh
        _emit 0ceh

        _emit 08bh
        _emit 075h
        _emit 0ech
        _emit 003h
        _emit 0f8h

        _emit 08bh
        _emit 0c1h
        _emit 0c1h
        _emit 0e9h
        _emit 002h

        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0c8h
        _emit 083h

        _emit 0e1h
        _emit 003h
        _emit 0f3h
        _emit 0a4h
        _emit 08bh

        _emit 04dh
        _emit 0e0h
        _emit 051h
        _emit 08bh
        _emit 0cbh

        _emit 0e8h
        _emit 032h
        _emit 0f8h
        _emit 0fbh
        _emit 0ffh

        _emit 033h
        _emit 0c0h
        _emit 083h
        _emit 0c9h
        _emit 0ffh

        _emit 0bfh
        _emit 080h
        _emit 09ch
        _emit 04fh
        _emit 000h

        _emit 0f2h
        _emit 0aeh
        _emit 08bh
        _emit 07bh
        _emit 008h

        _emit 0a1h
        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h

        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 08bh
        _emit 0f1h

        _emit 02bh
        _emit 0c7h
        _emit 03bh
        _emit 0c6h
        _emit 077h

        _emit 005h
        _emit 0e8h
        _emit 0b9h
        _emit 09eh
        _emit 007h

        _emit 000h
        _emit 085h
        _emit 0f6h
        _emit 076h
        _emit 03ch

        _emit 08dh
        _emit 004h
        _emit 037h
        _emit 06ah
        _emit 000h

        _emit 050h
        _emit 08bh
        _emit 0cbh
        _emit 089h
        _emit 045h

        _emit 0e0h
        _emit 0e8h
        _emit 01ah
        _emit 0f8h
        _emit 0fbh

        _emit 0ffh
        _emit 084h
        _emit 0c0h
        _emit 074h
        _emit 028h

        _emit 08bh
        _emit 04bh
        _emit 008h
        _emit 08bh
        _emit 07bh

        _emit 004h
        _emit 08bh
        _emit 045h
        _emit 0e0h
        _emit 003h

        _emit 0f9h
        _emit 08bh
        _emit 0ceh
        _emit 08bh
        _emit 0d1h

        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 0beh
        _emit 080h

        _emit 09ch
        _emit 04fh
        _emit 000h
        _emit 0f3h
        _emit 0a5h

        _emit 08bh
        _emit 0cah
        _emit 083h
        _emit 0e1h
        _emit 003h

        _emit 0f3h
        _emit 0a4h
        _emit 050h
        _emit 08bh
        _emit 0cbh

        _emit 0e8h
        _emit 0ceh
        _emit 0f7h
        _emit 0fbh
        _emit 0ffh

        _emit 0d9h
        _emit 045h
        _emit 0cch
        _emit 0d8h
        _emit 01dh

        _emit 048h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0dfh

        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 00fh

        _emit 085h
        _emit 03eh
        _emit 001h
        _emit 000h
        _emit 000h

        _emit 08bh
        _emit 04dh
        _emit 0cch
        _emit 051h
        _emit 08dh

        _emit 095h
        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 052h
        _emit 0e8h
        _emit 04ah
        _emit 0deh
        _emit 0fbh

        _emit 0ffh
        _emit 08bh
        _emit 045h
        _emit 0d4h
        _emit 083h

        _emit 0c9h
        _emit 0ffh
        _emit 083h
        _emit 0c4h
        _emit 008h

        _emit 08dh
        _emit 098h
        _emit 028h
        _emit 004h
        _emit 000h

        _emit 000h
        _emit 033h
        _emit 0c0h
        _emit 08dh
        _emit 0bdh

        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0f2h

        _emit 0aeh
        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 08bh

        _emit 0f1h
        _emit 06ah
        _emit 001h
        _emit 056h
        _emit 08bh

        _emit 0cbh
        _emit 089h
        _emit 075h
        _emit 0e0h
        _emit 0e8h

        _emit 09fh
        _emit 0f7h
        _emit 0fbh
        _emit 0ffh
        _emit 084h

        _emit 0c0h
        _emit 074h
        _emit 023h
        _emit 08bh
        _emit 07bh

        _emit 004h
        _emit 08bh
        _emit 0ceh
        _emit 08bh
        _emit 0d1h

        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 08dh
        _emit 0b5h

        _emit 074h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0f3h

        _emit 0a5h
        _emit 08bh
        _emit 0cah
        _emit 083h
        _emit 0e1h

        _emit 003h
        _emit 08bh
        _emit 0c2h
        _emit 0f3h
        _emit 0a4h

        _emit 050h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 058h

        _emit 0f7h
        _emit 0fbh
        _emit 0ffh
        _emit 083h
        _emit 0c9h

        _emit 0ffh
        _emit 033h
        _emit 0c0h
        _emit 0bfh
        _emit 040h

        _emit 02eh
        _emit 04fh
        _emit 000h
        _emit 0f2h
        _emit 0aeh

        _emit 08bh
        _emit 07bh
        _emit 008h
        _emit 0f7h
        _emit 0d1h

        _emit 049h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 00dh

        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 02bh

        _emit 0cfh
        _emit 03bh
        _emit 0ceh
        _emit 077h
        _emit 005h

        _emit 0e8h
        _emit 0deh
        _emit 09dh
        _emit 007h
        _emit 000h

        _emit 085h
        _emit 0f6h
        _emit 076h
        _emit 03ch
        _emit 08dh

        _emit 004h
        _emit 037h
        _emit 06ah
        _emit 000h
        _emit 050h

        _emit 08bh
        _emit 0cbh
        _emit 089h
        _emit 045h
        _emit 0e0h

        _emit 0e8h
        _emit 03fh
        _emit 0f7h
        _emit 0fbh
        _emit 0ffh

        _emit 084h
        _emit 0c0h
        _emit 074h
        _emit 028h
        _emit 08bh

        _emit 04bh
        _emit 008h
        _emit 08bh
        _emit 07bh
        _emit 004h

        _emit 08bh
        _emit 045h
        _emit 0e0h
        _emit 003h
        _emit 0f9h

        _emit 08bh
        _emit 0ceh
        _emit 08bh
        _emit 0d1h
        _emit 0c1h

        _emit 0e9h
        _emit 002h
        _emit 0beh
        _emit 040h
        _emit 02eh

        _emit 04fh
        _emit 000h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh

        _emit 0cah
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 0f3h

        _emit 0a4h
        _emit 050h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h

        _emit 0f3h
        _emit 0f6h
        _emit 0fbh
        _emit 0ffh
        _emit 08bh

        _emit 04dh
        _emit 0f0h
        _emit 051h
        _emit 0e8h
        _emit 00ah

        _emit 021h
        _emit 0fch
        _emit 0ffh
        _emit 08bh
        _emit 073h

        _emit 008h
        _emit 08bh
        _emit 015h
        _emit 040h
        _emit 013h

        _emit 04eh
        _emit 000h
        _emit 083h
        _emit 0c9h
        _emit 0ffh

        _emit 08bh
        _emit 0f8h
        _emit 083h
        _emit 0c4h
        _emit 004h

        _emit 089h
        _emit 045h
        _emit 0e0h
        _emit 033h
        _emit 0c0h

        _emit 0f2h
        _emit 0aeh
        _emit 0f7h
        _emit 0d1h
        _emit 049h

        _emit 08bh
        _emit 0f9h
        _emit 02bh
        _emit 0d6h
        _emit 03bh

        _emit 0d7h
        _emit 077h
        _emit 005h
        _emit 0e8h
        _emit 06dh

        _emit 09dh
        _emit 007h
        _emit 000h
        _emit 085h
        _emit 0ffh

        _emit 00fh
        _emit 086h
        _emit 08dh
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 08dh
        _emit 004h
        _emit 03eh
        _emit 06ah

        _emit 000h
        _emit 050h
        _emit 08bh
        _emit 0cbh
        _emit 089h

        _emit 045h
        _emit 0f0h
        _emit 0e8h
        _emit 0cah
        _emit 0f6h

        _emit 0fbh
        _emit 0ffh
        _emit 084h
        _emit 0c0h
        _emit 074h

        _emit 079h
        _emit 08bh
        _emit 073h
        _emit 008h
        _emit 08bh

        _emit 043h
        _emit 004h
        _emit 003h
        _emit 0c6h
        _emit 08bh

        _emit 075h
        _emit 0e0h
        _emit 08bh
        _emit 0cfh
        _emit 08bh

        _emit 0f8h
        _emit 08bh
        _emit 0c1h
        _emit 0c1h
        _emit 0e9h

        _emit 002h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0c8h

        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 0f3h
        _emit 0a4h

        _emit 08bh
        _emit 04dh
        _emit 0f0h
        _emit 051h
        _emit 08bh

        _emit 0cbh
        _emit 0e8h
        _emit 07eh
        _emit 0f6h
        _emit 0fbh

        _emit 0ffh
        _emit 0ebh
        _emit 04fh
        _emit 08bh
        _emit 055h

        _emit 0d4h
        _emit 083h
        _emit 0c9h
        _emit 0ffh
        _emit 033h

        _emit 0c0h
        _emit 0bfh
        _emit 0b0h
        _emit 070h
        _emit 050h

        _emit 000h
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h
        _emit 0d1h

        _emit 049h
        _emit 08dh
        _emit 0b2h
        _emit 028h
        _emit 004h

        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0d9h
        _emit 06ah

        _emit 001h
        _emit 053h
        _emit 08bh
        _emit 0ceh
        _emit 089h

        _emit 075h
        _emit 0e0h
        _emit 0e8h
        _emit 075h
        _emit 0f6h

        _emit 0fbh
        _emit 0ffh
        _emit 084h
        _emit 0c0h
        _emit 074h

        _emit 024h
        _emit 08bh
        _emit 046h
        _emit 004h
        _emit 08bh

        _emit 0cbh
        _emit 08bh
        _emit 0d1h
        _emit 0c1h
        _emit 0e9h

        _emit 002h
        _emit 0beh
        _emit 0b0h
        _emit 070h
        _emit 050h

        _emit 000h
        _emit 08bh
        _emit 0f8h
        _emit 0f3h
        _emit 0a5h

        _emit 08bh
        _emit 0cah
        _emit 083h
        _emit 0e1h
        _emit 003h

        _emit 0f3h
        _emit 0a4h
        _emit 08bh
        _emit 04dh
        _emit 0e0h

        _emit 089h
        _emit 059h
        _emit 008h
        _emit 0c6h
        _emit 004h

        _emit 003h
        _emit 000h
        _emit 08bh
        _emit 07dh
        _emit 0e4h

        _emit 08bh
        _emit 055h
        _emit 0d4h
        _emit 083h
        _emit 0c9h

        _emit 0ffh
        _emit 033h
        _emit 0c0h
        _emit 0f2h
        _emit 0aeh

        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 08dh
        _emit 0b2h

        _emit 0e8h
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 08bh

        _emit 0d9h
        _emit 06ah
        _emit 001h
        _emit 053h
        _emit 08bh

        _emit 0ceh
        _emit 089h
        _emit 075h
        _emit 0e0h
        _emit 0e8h

        _emit 028h
        _emit 0f6h
        _emit 0fbh
        _emit 0ffh
        _emit 084h

        _emit 0c0h
        _emit 074h
        _emit 022h
        _emit 08bh
        _emit 046h

        _emit 004h
        _emit 08bh
        _emit 075h
        _emit 0e4h
        _emit 08bh

        _emit 0cbh
        _emit 08bh
        _emit 0d1h
        _emit 0c1h
        _emit 0e9h

        _emit 002h
        _emit 08bh
        _emit 0f8h
        _emit 0f3h
        _emit 0a5h

        _emit 08bh
        _emit 0cah
        _emit 083h
        _emit 0e1h
        _emit 003h

        _emit 0f3h
        _emit 0a4h
        _emit 08bh
        _emit 04dh
        _emit 0e0h

        _emit 089h
        _emit 059h
        _emit 008h
        _emit 0c6h
        _emit 004h

        _emit 003h
        _emit 000h
        _emit 08bh
        _emit 055h
        _emit 0e4h

        _emit 08ah
        _emit 042h
        _emit 01fh
        _emit 08bh
        _emit 04dh

        _emit 0d8h
        _emit 08bh
        _emit 075h
        _emit 0c8h
        _emit 088h

        _emit 084h
        _emit 00eh
        _emit 0a8h
        _emit 005h
        _emit 000h

        _emit 000h
        _emit 06ah
        _emit 001h
        _emit 08dh
        _emit 08dh

        _emit 064h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0c6h

        _emit 045h
        _emit 0fch
        _emit 003h
        _emit 0e8h
        _emit 05eh

        _emit 0f3h
        _emit 0fbh
        _emit 0ffh
        _emit 08bh
        _emit 045h

        _emit 0a8h
        _emit 033h
        _emit 0ffh
        _emit 03bh
        _emit 0c7h

        _emit 0c6h
        _emit 045h
        _emit 0fch
        _emit 002h
        _emit 074h

        _emit 01ch
        _emit 08ah
        _emit 048h
        _emit 0ffh
        _emit 048h

        _emit 084h
        _emit 0c9h
        _emit 074h
        _emit 00bh
        _emit 080h

        _emit 0f9h
        _emit 0ffh
        _emit 074h
        _emit 006h
        _emit 0feh

        _emit 0c9h
        _emit 088h
        _emit 008h
        _emit 0ebh
        _emit 009h

        _emit 050h
        _emit 0e8h
        _emit 038h
        _emit 09ch
        _emit 007h

        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh

        _emit 085h
        _emit 054h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 03bh
        _emit 046h
        _emit 020h
        _emit 089h
        _emit 07dh

        _emit 0a8h
        _emit 089h
        _emit 07dh
        _emit 0ach
        _emit 089h

        _emit 07dh
        _emit 0b0h
        _emit 089h
        _emit 045h
        _emit 0d8h

        _emit 00fh
        _emit 08ch
        _emit 097h
        _emit 0f8h
        _emit 0ffh

        _emit 0ffh
        _emit 033h
        _emit 0ffh
        _emit 08bh
        _emit 045h

        _emit 098h
        _emit 03bh
        _emit 0c7h
        _emit 074h
        _emit 01ch

        _emit 08ah
        _emit 048h
        _emit 0ffh
        _emit 048h
        _emit 084h

        _emit 0c9h
        _emit 074h
        _emit 00bh
        _emit 080h
        _emit 0f9h

        _emit 0ffh
        _emit 074h
        _emit 006h
        _emit 0feh
        _emit 0c9h

        _emit 088h
        _emit 008h
        _emit 0ebh
        _emit 009h
        _emit 050h

        _emit 0e8h
        _emit 0f8h
        _emit 09bh
        _emit 007h
        _emit 000h

        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh
        _emit 045h

        _emit 0b8h
        _emit 03bh
        _emit 0c7h
        _emit 089h
        _emit 07dh

        _emit 098h
        _emit 089h
        _emit 07dh
        _emit 09ch
        _emit 089h

        _emit 07dh
        _emit 0a0h
        _emit 074h
        _emit 01ch
        _emit 08ah

        _emit 048h
        _emit 0ffh
        _emit 048h
        _emit 084h
        _emit 0c9h

        _emit 074h
        _emit 00bh
        _emit 080h
        _emit 0f9h
        _emit 0ffh

        _emit 074h
        _emit 006h
        _emit 0feh
        _emit 0c9h
        _emit 088h

        _emit 008h
        _emit 0ebh
        _emit 009h
        _emit 050h
        _emit 0e8h

        _emit 0cch
        _emit 09bh
        _emit 007h
        _emit 000h
        _emit 083h

        _emit 0c4h
        _emit 004h
        _emit 08dh
        _emit 08dh
        _emit 040h

        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 089h
        _emit 07dh

        _emit 0b8h
        _emit 089h
        _emit 07dh
        _emit 0bch
        _emit 089h

        _emit 07dh
        _emit 0c0h
        _emit 0c7h
        _emit 045h
        _emit 0fch

        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h

        _emit 04eh
        _emit 021h
        _emit 0fch
        _emit 0ffh
        _emit 08bh

        _emit 055h
        _emit 0c4h
        _emit 08bh
        _emit 085h
        _emit 058h

        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 03bh
        _emit 082h

        _emit 0e4h
        _emit 01eh
        _emit 000h
        _emit 000h
        _emit 089h

        _emit 045h
        _emit 0d0h
        _emit 00fh
        _emit 08ch
        _emit 0f1h

        _emit 0f6h
        _emit 0ffh
        _emit 0ffh
        _emit 05fh
        _emit 08bh

        _emit 04dh
        _emit 0f4h
        _emit 05eh
        _emit 05bh
        _emit 064h

        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x443480
// original: FUN_00443480 (vtable slot 2)
void TrackSelectMenu::Release()
{
    if (m_state) {
        int r;
        for (r = 0; r < rallyCount; r++) {
            if (rally[r].photo) {
                delete rally[r].photo->texture;
                delete *(Texture**)((BYTE*)rally[r].photo + 4);
            }
            delete rally[r].photo;
            int k;
            for (k = 0; k < rally[r].numStages; k++) {
                if (rally[r].stageInfo[k]) {
                    delete rally[r].stageInfo[k]->texture;
                }
                delete rally[r].stageInfo[k];
            }
        }
        if (trackBook) {
            delete trackBook->texture;
        }
        delete trackBook;
        trackBook = 0;
        m_state = 0;
        m_game = 0;
        FUN_00438d00();
    }
}

// FUNCTION: RALLYTROPHY 0x443580
// original: FUN_00443580 (never reads this)
int TrackSelectMenu::GetRallyCount()
{
    if (DAT_00501454) {
        return 5;
    }
    int unlocked;
    if (DAT_00501458) {
        if (DAT_00501464 == 0) {
            switch (DAT_0050132c) {
            case 0:
                return 3;
            case 1:
                return 4;
            default:
                return 5;
            }
        }
        unlocked = DAT_00501498[1];
        if (DAT_00501464 == 2) {
            return unlocked;
        }
        switch (DAT_0050132c) {
        case 0:
            if (unlocked > 3) {
                return 3;
            }
            break;
        case 1:
            if (unlocked > 4) {
                return 4;
            }
            break;
        }
        return unlocked;
    }
    if (DAT_00501464 == 0) {
        return (DAT_0050132c != 0) + 4;
    }
    unlocked = DAT_00501498[0];
    if (DAT_00501464 == 3) {
        return unlocked;
    }
    if (DAT_0050132c == 0 && unlocked > 4) {
        return 4;
    }
    return unlocked;
}

// FUNCTION: RALLYTROPHY 0x443620
// original: FUN_00443620 (rally1 is the 1-based rally number)
int TrackSelectMenu::GetStageCount(int rally1)
{
    int count;
    if (DAT_00501454) {
        return rally[rally1 - 1].numExpert;
    }
    switch (DAT_0050132c) {
    case 1:
        count = rally[rally1 - 1].numIntermediate;
        break;
    case 2:
        count = rally[rally1 - 1].numExpert;
        break;
    default:
        count = rally[rally1 - 1].numNovice;
        break;
    }
    int unlocked = DAT_00501498[rally1 + 1];
    if (DAT_00501464 == 2) {
        if (unlocked < count) {
            return unlocked;
        }
    } else if (DAT_00501464 == 3) {
        return unlocked;
    }
    return count;
}

// FUNCTION: RALLYTROPHY 0x4436b0
// original: FUN_004436b0
int TrackSelectMenu::GetArcadeStageCount(int rally1)
{
    if (DAT_00501454) {
        return rally[rally1 - 1].numArcade;
    }
    if (DAT_00501464 >= 2) {
        return DAT_00501498[rally1 + 6];
    }
    return rally[rally1 - 1].numArcade;
}

// FUNCTION: RALLYTROPHY 0x443700
// original: FUN_00443700
int TrackSelectMenu::Open(MenuGame* game, int mode)
{
    Setup(game);
    this->mode = mode;
    int available = GetRallyCount();
    availableRallies = available;
    if (curRally >= available) {
        curRally = 0;
    }
    if (mode) {
        curStage = rally[curRally].firstArcade + lastStageArcade;
        return Run();
    }
    curStage = lastStage;
    return Run();
}

// The track-book quad origin (a memory constant in the original: the sums below are not folded).
static const float TSB_BookX = 541.0f;
static const float TSB_BookY = 413.0f;

// A colour built from four float channels (0..255), as the track-book quad passes it.
class TSB_Color {
public:
    TSB_Color()
    {
        b = (BYTE)TSB_FloatToInt(255.0f);
        g = (BYTE)TSB_FloatToInt(255.0f);
        r = (BYTE)TSB_FloatToInt(255.0f);
        a = (BYTE)TSB_FloatToInt(255.0f);
    }
    ~TSB_Color() {}
    union {
        DWORD value;
        struct {
            BYTE b, g, r, a;
        };
    };
};

// NONMATCHING: RALLYTROPHY 0x443790
// Same instruction sequence in all parts (5268 vs 5277 bytes) except register/frame-slot choices: the original keeps
// sx/sy/hi/`one` in other frame slots (frame 0x104, mine 0x100) and holds the constant 0 in edi only from the row loop
// on, while mine hoists `xor edi,edi` to the start of the draw block. Inline budget reproduced with TSE_Str (hand-expanded
// assign/append) and TSE_RallyInfo (out-of-line string-array copies). original: FUN_00443790 (Run; the 4 rows go
// through the jump table at 0x444c30, the rally goes to DrawRally by value).
int TrackSelectMenu::Run()
{
    int done = 0;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sy;
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    int picked = 0;
    DAT_0056b0bc->ClearKeys();
    TextRenderer* text = m_game->text;
    animStartTick = timeGetTime();
    slideDir = 0;
    int lo = mode ? rally[curRally].firstArcade : 0;
    int hi;
    if (mode) {
        hi = GetArcadeStageCount(curRally + 1) + rally[curRally].firstArcade - 1;
    } else {
        hi = GetStageCount(curRally + 1) - 1;
    }
    if (curStage < lo) {
        curStage = lo;
    } else if (curStage > hi) {
        curStage = hi;
    }
    LoadRallyData();
    int first = 1;
    int wait = 0;
    while (FUN_004b69f0(wait) && !done && !picked) {
        m_game->Update();
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && DAT_00501464 >= 2) {
                if ((m_selected == 0 || m_selected == 2) && slideDir != 1) {
                    slideTick = (int)((float)timeGetTime() - (float)(timeGetTime() - slideTick) / TSE_SLIDE_MS);
                    slideDir = 1;
                }
                m_selected = 1;
            }
            if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 2) {
                m_selected = 2;
            }
            if (m_items[3]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 3) {
                m_selected = 3;
            }
        }
        if (DAT_0057ba28[VK_ESCAPE]) {
            done = 1;
        }
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        if (DAT_0057ba28[VK_RETURN]) {
            if (m_selected == 2) {
                m_game->Play(1);
                picked = 1;
                continue;
            } else if (m_selected == 3) {
                done = 1;
                continue;
            }
        }
        int vert = 0;
        if (DAT_0057ba28[VK_DOWN]) {
            vert = 1;
        }
        if (DAT_0057ba28[VK_UP]) {
            vert--;
        }
        if (vert != 0) {
            m_game->Play(0);
            m_selected += vert;
            if (m_selected < 0) {
                m_selected = 0;
                vert = 0;
            } else if (m_selected >= 4) {
                m_selected = 3;
                vert = 0;
            }
            if (m_selected == 1 && DAT_00501464 < 2) {
                m_selected = vert + 1;
            }
            TSE_WARP(m_items, m_selected, sx, sy);
        }
        if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
            int sel = m_selected;
            if (sel == 1 || (vert < 0 && sel == 0) || sel == 2) {
                if (DAT_00501464 >= 2) {
                    slideTick = (int)((float)timeGetTime() - (float)(timeGetTime() - slideTick) / TSE_SLIDE_MS);
                    slideDir = -1;
                }
            }
            if (sel != 0) {
                m_game->Play(0);
            }
            m_selected = 0;
        }
        if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && DAT_00501464 >= 2) {
            int sel = m_selected;
            if (sel == 0 || sel == 2 || (vert > 0 && sel == 1) || (vert < 0 && sel == 1)) {
                if (slideDir != 1) {
                    slideTick = (int)((float)timeGetTime() - (float)(timeGetTime() - slideTick) / TSE_SLIDE_MS);
                    slideDir = 1;
                }
            }
            if (sel != 1) {
                m_game->Play(0);
            }
            m_selected = 1;
        }
        if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 2) {
            m_selected = 2;
            m_game->Play(0);
        }
        if (m_items[3]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 3) {
            m_selected = 3;
            m_game->Play(0);
        }
        int horiz = 0;
        if (DAT_0057ba28[VK_LEFT]) {
            horiz = -1;
        }
        if (DAT_0057ba28[VK_RIGHT]) {
            horiz++;
        }
        if (DAT_0057ba20) {
            HudPoint mouse = *(HudPoint*)&DAT_0057ba00;
            if (m_items[4]->Contains(mouse)) {
                horiz--;
            }
            if (m_items[6]->Contains(mouse)) {
                horiz--;
            }
            if (m_items[5]->Contains(mouse)) {
                horiz++;
            }
            if (m_items[7]->Contains(mouse)) {
                horiz++;
            }
            if (m_selected == 2) {
                m_game->Play(1);
                picked = 1;
                continue;
            } else if (m_selected == 3) {
                done = 1;
                continue;
            }
        }
        if (horiz != 0) {
            switch (m_selected) {
            case 0: {
                if (curRally + horiz < availableRallies && curRally + horiz >= 0) {
                    animStartTick = timeGetTime();
                }
                FUN_00438c40(&curRally, horiz, 0, availableRallies - 1);
                int off = curStage - lo;
                if (mode) {
                    lo = rally[curRally].firstArcade;
                } else {
                    lo = 0;
                }
                if (mode) {
                    hi = GetArcadeStageCount(curRally + 1) + rally[curRally].firstArcade - 1;
                } else {
                    hi = GetStageCount(curRally + 1) - 1;
                }
                curStage = lo + off;
                if (curStage > hi) {
                    curStage = hi;
                } else if (curStage < lo) {
                    curStage = lo;
                }
                break;
            }
            case 1:
                FUN_00438c40(&curStage, horiz, lo, hi);
                break;
            }
        }
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            m_game->DrawVideo(0);
            text->scaleX = DAT_0056aea8 * 2.0f;
            text->color = DAT_00508bfc;
            text->scaleY = DAT_0056aeac * 2.0f;
            text->Print(DAT_0056b0ac, TSE_TITLEX * DAT_0056aea8, TSE_TITLEY * DAT_0056aeac, FUN_00405380(0x101), 0, 1.0f,
                        1.0f);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            DrawTrackBook();
            ((TSE_View*)this)->DrawRallyV(*(TSE_RallyInfo*)&rally[curRally]);
            float y = TSE_LISTY * sy;
            for (int i = 0; i < 4; i++) {
                if (DAT_00501464 < 2 && i == 1) {
                    y = TSE_STEP * sy + y;
                    continue;
                }
                float x = TSE_LISTX * sx;
                bool selected = (m_selected == i);
                Color col;
                HudPoint one;
                std::string buf;
                col.value = selected ? DAT_00508c0c : DAT_00508c04;
                text->color = col.value;
                one.x = 1.0f;
                one.y = 1.0f;
                char number[32];
                switch (i) {
                case 0:
                    text->Print(DAT_0056b0ac, x, y, unk64.c_str(), 0, 1.0f, 1.0f);
                    x = TSE_COUNTX * sx;
                    itoa(curRally + 1, number, 10);
                    ((TSE_Str*)&buf)->AssignInline(number);
                    ((TSE_Str*)&buf)->Append("/");
                    itoa(availableRallies, number, 10);
                    ((TSE_Str*)&buf)->Append(number);
                    DrawSelector(text, &buf, x, y, TSE_COUNTW * sx, selected, curRally, 0, availableRallies - 1);
                    break;
                case 1:
                    text->Print(DAT_0056b0ac, x, y, unk74.c_str(), 0, 1.0f, 1.0f);
                    x = TSE_COUNTX * sx;
                    itoa(curStage - lo + 1, number, 10);
                    ((TSE_Str*)&buf)->AssignInline(number);
                    ((TSE_Str*)&buf)->Append("/");
                    itoa(hi - lo + 1, number, 10);
                    ((TSE_Str*)&buf)->Append(number);
                    DrawSelector(text, &buf, x, y, TSE_COUNTW * sx, selected, curStage, lo, hi);
                    break;
                case 2:
                    y = TSE_STEP * sy + y;
                    text->Print(DAT_0056b0ac, x, y, unk44.c_str(), 0, 1.0f, 1.0f);
                    break;
                case 3:
                    y = TSE_BACKY * sy;
                    x = TSE_LISTX * sx;
                    text->Print(DAT_0056b0ac, x, y, unk54.c_str(), 0, 1.0f, 1.0f);
                    break;
                }
                y = TSE_STEP * sy + y;
            }
            float oldX = text->scaleX;
            float oldY = text->scaleY;
            text->scaleX = oldX * 0.85f;
            text->color = DAT_00508c14.value;
            text->scaleY = oldY * 0.85f;
            text->Print(DAT_0056b0ac, TSE_NAMEX * sx, TSE_NAMEY * sy, rally[curRally].name.c_str(), 0, 1.0f, 1.0f);
            text->scaleX = oldX;
            text->scaleY = oldY;
            if (m_selected != 0 && DAT_00501464 >= 2) {
                DrawDetails();
            } else {
                DrawDescription();
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 2, 0, 0, 1, -1.0f);
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(4, 1, 0, 1, 1, -1.0f);
            DrawCursor(*((MouseInput*)DAT_0056b0c0)->GetPosition());
            DAT_0056b0ac->EndScene();
            DAT_0056b0ac->Present();
        }
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    int r = m_selected;
    if (mode) {
        lastStageArcade = curStage - lo;
    } else {
        lastStage = curStage - lo;
    }
    if (done || DAT_0057b9f4) {
        r = -1;
        m_game->Play(2);
    }
    return r;
}

// FUNCTION: RALLYTROPHY 0x444c40
// original: FUN_00444c40
void TrackSelectMenu::DrawDetails()
{
    float dy;
    float yt;
    float s;
    int i;
    float y;
    std::string* p1;
    std::string* p2;
    RallyInfo* r;
    float sy;
    float sx;
    float y2;
    float x;
    sx = DAT_0056b0ac->GetWidth() * (1.0f / 1024.0f);
    sy = DAT_0056b0ac->GetHeight() * (1.0f / 768.0f);
    TextRenderer* text = *(TextRenderer**)((BYTE*)m_game + 0x1390bc);
    ((TF_Text*)text)->SetColor((*(Color*)&DAT_00508c14));
    y = TF_Y0 * sy;
    for (i = 0, r = &rally[curRally]; i < 4; i++) {
        x = TF_X1 * sx;
        text->Print(DAT_0056b0ac, x, y, labels[i].c_str(), 0, 1.0f, 1.0f);
        x = TF_X2 * sx;
        text->Print(DAT_0056b0ac, x, y, labels[i + 4].c_str(), 0, 1.0f, 1.0f);
        p1 = 0;
        p2 = 0;
        switch (i) {
        case 0: {
            p1 = 0;
            y2 = TF_LINE * sy + y;
            x = (TF_INDENT + TF_X2) * sx;
            p2 = &r->stageName[curStage];
            text->Print(DAT_0056b0ac, x, y2, r->recordTime[curStage].c_str(), 0, 1.0f, 1.0f);
            std::string& ts = r->recordTopSpeed[curStage];
            dy = 15.0f * sy;
            y2 = y2 + dy;
            text->scaleX = 1.0f;
            s = 160.0f / text->GetWidth(ts.c_str(), 0);
            if (s > 1.0f)
                s = 1.0f;
            text->SetScaleX(DAT_0056aea8 * s);
            x = (TF_INDENT + TF_X2) * sx;
            text->Print(DAT_0056b0ac, x, y2, ts.c_str(), 0, 1.0f, 1.0f);
            text->SetScaleX(DAT_0056aea8);
            y2 = dy + y2;
            const char* ct = FUN_00405380(0x138 + r->unk5a8[curStage]);
            text->Print(DAT_0056b0ac, (TF_INDENT + TF_X2) * sx, y2, ct, 0, 1.0f, 1.0f);
            if (r->recordSpeed[curStage].size() > 0) {
                y2 = 30.0f * sy + y2;
                x = TF_X2 * sx;
                text->Print(DAT_0056b0ac, x, y2, FUN_00405380(0x100), 0, 1.0f, 1.0f);
                y2 = dy + y2;
                x = (TF_INDENT + TF_X2) * sx;
                text->Print(DAT_0056b0ac, x, y2, r->recordSpeed[curStage].c_str(), 0, 1.0f, 1.0f);
            }
            break;
        }
        case 1:
            p2 = &r->lengthText[curStage];
            p1 = 0;
            break;
        case 2:
            p1 = &r->timeText[curStage];
            p2 = 0;
            break;
        case 3:
            p2 = &r->surfaceText[curStage];
            p1 = 0;
            break;
        }
        yt = TF_LINE * sy + y;
        if (p1) {
            x = (TF_INDENT + TF_X1) * sx;
            text->Print(DAT_0056b0ac, x, yt, p1->c_str(), 0, 1.0f, 1.0f);
        }
        if (p2) {
            x = (TF_INDENT + TF_X1) * sx;
            text->Print(DAT_0056b0ac, x, yt, p2->c_str(), 0, 1.0f, 1.0f);
        }
        y = TF_ROW * sy + y;
    }
}

// Constants of the rally photo block (file-scope const objects: .rdata 0x4e1890..0x4e18b0).
static const float TF_PHOTOX = 552.0f; // 0x4e1890
static const float TF_PHOTOY = 146.0f; // 0x4e1894
static const float TF_FADE = 600.0f;   // 0x4e18a8: fade-in duration of the photo (ms)
static const float TF_FADEWAIT = 400.0f; // 0x4e18ac: delay before the fade-in starts (ms)
static const float TF_SLIDE = 400.0f;  // 0x4e18b0: stage picture slide duration (ms)

// A colour built from float channels (1.0 = 255); the original's inline constructor.
class TF_Color {
public:
    TF_Color(float r, float g, float b, float a)
    {
        this->b = TF_FloatToInt(b * 255.0f);
        this->g = TF_FloatToInt(g * 255.0f);
        this->r = TF_FloatToInt(r * 255.0f);
        this->a = TF_FloatToInt(a * 255.0f);
    }
    ~TF_Color() {}
    union {
        DWORD value;
        struct {
            BYTE b, g, r, a;
        };
    };
};

// FUNCTION: RALLYTROPHY 0x4450e0
void TrackSelectMenu::DrawDescription()
{
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    TSG_Font* font = *(TSG_Font**)((BYTE*)m_game + 0x1390bc);
    font->SetColor(DAT_00508c14.value);
    float y = TSG_DESC_Y * sy;
    float x = TSG_DESC_X * sx;
    font->wrap = 1;
    font->wrapW = font->lineHeight * sy;
    font->wrapY = TSG_DESC_W * sx;
    const char* s = rally[curRally].description.c_str();
    font->Print(DAT_0056b0ac, x, y, s, 0, 1.0f, 1.0f);
    font->wrap = 0;
}

// NONMATCHING: RALLYTROPHY 0x4451c0
// 1704 vs 1700 bytes. The setup and the first seven key-pair blocks (out-of-line insert calls) match; only the inline
// expansion of the 8th insert differs (register assignment and the temp store of the rect pointer happen earlier/later).
// original: FUN_004451c0
void TrackSelectMenu::BuildRects()
{
    Config config;
    UIRect* rect;
    config.Define(DAT_004f94d8, 0x10);
    TSG_String path("data/menu/rallymenu.ini");
    config.Load(path.c_str(), true, false, 0);
    TSG_ADD("CountryText_UpperLeft", "CountryText_LowerRight");
    TSG_ADD("SpecialStageText_UpperLeft", "SpecialStageText_LowerRight");
    TSG_ADD("ContinueText_UpperLeft", "ContinueText_LowerRight");
    TSG_ADD("BackText_UpperLeft", "BackText_LowerRight");
    TSG_ADD("CountryArrow1_UpperLeft", "CountryArrow1_LowerRight");
    TSG_ADD("CountryArrow2_UpperLeft", "CountryArrow2_LowerRight");
    TSG_ADD("SpecialStageArrow1_UpperLeft", "SpecialStageArrow1_LowerRight");
    TSG_ADD("SpecialStageArrow2_UpperLeft", "SpecialStageArrow2_LowerRight");
}

// FUNCTION: RALLYTROPHY 0x445870
// original: FUN_00445870 (RallyInfo by value, destroyed by the callee: ret 0x5e8)
void TrackSelectMenu::DrawRally(RallyInfo info)
{
    HudPoint pos[4];
    HudPoint uv[4];
    float a = ((float)timeGetTime() - ((float)animStartTick + TF_FADEWAIT)) / TF_FADE;
    if (a > 1.0f)
        a = 1.0f;
    else if (a < 0.0f)
        a = 0.0f;
    TF_Color color(1.0f, 1.0f, 1.0f, a);
    DAT_0056b0ac->vf35(info.photo);
    pos[0].x = DAT_0056aea8 * TF_PHOTOX;
    uv[3].x = 0.0f;
    uv[1].y = 0.0f;
    uv[0].y = 0.0f;
    uv[0].x = 0.0f;
    uv[1].x = 1.0f;
    uv[3].y = 1.0f;
    uv[2].y = 1.0f;
    uv[2].x = 1.0f;
    pos[0].y = DAT_0056aeac * TF_PHOTOY;
    pos[1].x = (TF_PHOTOX + 512.0f) * DAT_0056aea8;
    pos[1].y = DAT_0056aeac * TF_PHOTOY;
    pos[2].x = (TF_PHOTOX + 512.0f) * DAT_0056aea8;
    pos[2].y = (TF_PHOTOY + 256.0f) * DAT_0056aeac;
    pos[3].x = DAT_0056aea8 * TF_PHOTOX;
    pos[3].y = (TF_PHOTOY + 256.0f) * DAT_0056aeac;
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, *(Color*)&color, 1.0f, 1.0f);
    DAT_0056b0ac->vf35(info.stageInfo[curStage]);
    a = (float)(timeGetTime() - slideTick) / TF_SLIDE;
    if (a > 1.0f)
        a = 1.0f;
    else if (a < 0.0f)
        a = 0.0f;
    if (slideDir == -1)
        a = 1.0f - a;
    if (slideDir == 0)
        a = 0.0f;
    color = TF_Color(1.0f, 1.0f, 1.0f, a);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, *(Color*)&color, 1.0f, 1.0f);
}

// NONMATCHING: RALLYTROPHY 0x445c90
// Evidence: ev_d75f1466e03adee2022eecff0abdd168bae0f3e3d71c762a1d5c090fe91daef3. Candidate 422 bytes, 140 differing
// bytes. The target's right-edge X and bottom-edge Y stores retain the x87 value while copying its bits through a GPR.
// original: FUN_00445c90 (the track-book background quad)
void TrackSelectMenu::DrawTrackBook()
{
    HudPoint pos[4];
    HudPoint uv[4];
    TSB_Color color;
    DAT_0056b0ac->vf35(trackBook);
    pos[0].x = DAT_0056aea8 * TSB_BookX;
    pos[0].y = DAT_0056aeac * TSB_BookY;
    uv[0].x = uv[3].x = 0.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[2].x = uv[1].x = 1.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[1].x = pos[2].x = (TSB_BookX + 512.0f) * DAT_0056aea8;
    pos[1].y = DAT_0056aeac * TSB_BookY;
    pos[2].y = pos[3].y = (TSB_BookY + 256.0f) * DAT_0056aeac;
    pos[3].x = DAT_0056aea8 * TSB_BookX;
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, *(Color*)&color, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x445e50
// SYMBOL: _$E49
typedef void Generated_445e50();

// FUNCTION: RALLYTROPHY 0x445e70
// SYMBOL: _$E48
typedef void Generated_445e70();
