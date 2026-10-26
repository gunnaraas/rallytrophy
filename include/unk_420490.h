/* Module 1 of range 0x420490-0x4243af (analysis/range-420490.md): body helpers, StageScreen screen, STL stubs. */
#ifndef UNK_420490_H
#define UNK_420490_H
#include <string>

// Reciprocal-scale holder at +0x134 of the body (0x420490).
struct ScaleBody {
    BYTE unk0[0x134];
    DWORD scale[3];                    // 0x134
    float inv[3];                      // 0x140
    void SetScale(DWORD x, DWORD y, DWORD z); // 0x420490
};

// Inline vector helpers local to this module (0x4204f0 expands them in place).
struct F3 {
    float x, y, z;
    F3() {}
    F3(float a, float b, float c) { x = a; y = b; z = c; }
    F3 Sub(const F3& o) const { F3 r; r.x = x - o.x; r.y = y - o.y; r.z = z - o.z; return r; }
    F3 Cross(const F3& o) const { F3 r; r.x = y * o.z - z * o.y; r.y = z * o.x - x * o.z; r.z = x * o.y - y * o.x; return r; }
};

// Body fields read by 0x4204f0 (same object as BodyObject of unk_481390.h).
struct BodyAxis {
    BYTE unk0[0x1d8];
    F3 axis;                           // 0x1d8
    BYTE unk1e4[0x224 - 0x1e4];
    F3 pos;                            // 0x224
    F3 PointVel(const F3& point);      // 0x4204f0
};

struct PtrArray {                      // vector<T*>-like: element array pointer at +4 (0x4242f0)
    BYTE alloc;
    DWORD* first;                      // 0x04
    DWORD* At(int i);                  // 0x4242f0
};

struct Pair {                          // 0x424300: copy two dwords
    DWORD a, b;
    Pair* Assign(const Pair* o);
};

// std::string layouts: StrT keeps its constructor out of line (_Tidy, 0x402700); InlStr builds in place. Both expand
// the destructor inline (reference-count byte in front of the buffer).
class StrT {
public:
    StrT(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~StrT()
    {
        if (ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, res + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, res = 0;
    }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Tidy(bool built);             // 0x402700
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

class InlStr {
public:
    InlStr(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { ptr = 0; len = 0; res = 0; }
    ~InlStr()
    {
        if (ptr == 0) {
        } else if (StrT::Refcnt(ptr) == 0 || StrT::Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, res + 2);
        } else {
            --StrT::Refcnt(ptr);
        }
        ptr = 0, len = 0, res = 0;
    }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

// Inline constructor, out-of-line destructor (_Tidy(true), 0x402700).
class OutStr {
public:
    OutStr(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { ptr = 0; len = 0; res = 0; }
    ~OutStr() { Tidy(true); }
    void Tidy(bool built);             // 0x402700
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
// Out-of-line constructor and destructor (the plain std::string layout).
class FullStr {
public:
    FullStr(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~FullStr() { Tidy(true); }
    void Tidy(bool built);             // 0x402700
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

// Polymorphic parts owned by a stage entry; the holders expand their destructors inline and call the base destructor
// 0x4a86d0 out of line.
class Poly {
public:
    virtual ~Poly();
};
class Holder1 {
public:
    void BaseDtor();                   // 0x4a86d0 (called out of line after the members are deleted by hand)
    Poly* a;
};
class Holder2 {
public:
    void BaseDtor();                   // 0x4a86d0
    Poly* a;
    Poly* b;
};

// One stage entry (0xa8 bytes; ctor 0x421a80, dtor 0x421b80), elements of StageScreen::entries.
class StageEntry {
public:
    StageEntry();                      // 0x421a80
    ~StageEntry();                     // 0x421b80
    StrT s0, s1, s2, s3, s4, s5;       // 0x00..0x50
    OutStr a;                          // 0x60
    OutStr b;                          // 0x70
    FullStr c;                         // 0x80
    OutStr d;                          // 0x90
    Holder2* h0;                       // 0xa0
    Holder1* h1;                       // 0xa4
};

// The StageScreen screen (Game+0x59e4, 0x24c bytes, vtable 0x4e14ac = {dtor 0x420730, Setup, Release, Slot3}).
class StageScreen : public MenuScreen {
public:
    StageScreen();                      // 0x4205e0
    virtual ~StageScreen();             // 0x420730 (deleting form 0x420710)
    virtual void Setup(MenuGame* game);
    virtual void Release();
    virtual void Slot3(int unused);
    void Start(DWORD game, int a, int b); // 0x421e30: build the car list, then lay it out
    int Layout();                      // 0x421fa0: 1 once a car is chosen, -1 on back/quit
    void RefreshUnits();               // 0x423b90: rewrite the four car-info strings of every entry in the chosen units
    void LoadCars();                   // 0x420db0: read the per-car info.ini files and load the photo sprites
    void Free();                       // 0x421d00: release the stage entries and the item rectangles
    void LoadLayout();                 // 0x422fc0: hit rectangles from data/menu/carmenu.ini
    void DrawPhoto(StageEntry* e, int flag); // 0x423670
    void DrawBook(StageEntry* e, int flag);  // 0x4239c0

    StrT title;                        // 0x20
    BYTE unk30[0x14];
    std::string names[8];              // 0x44: eh vector ctor 0x4c11f4
    InlStr strA;                       // 0xc4
    InlStr strB;                       // 0xd4
    InlStr strC;                       // 0xe4
    std::string pair[2];               // 0xf4
    InlStr strD;                       // 0x114
    int unk124[2];                     // 0x124: selected index, number of listed cars
    struct { int car; int flag; } list[32]; // 0x12c: car id and availability flag

    int unk22c;                        // 0x22c
    Holder1* unk230;                   // 0x230
    Holder1* unk234;                   // 0x234
    int unk238;                        // 0x238
    int count;                         // 0x23c
    StageEntry* entries;               // 0x240
    int unk244;
    int unk248;                        // 0x248
};


// Ini reader used by StageScreen::LoadCars (ctor 0x405580, dtor 0x4055a0).
struct IniFile {
    BYTE unk0[0x14];
    IniFile();                         // 0x405580
    ~IniFile();                        // 0x4055a0
    void Open(const char* tbl, int n); // 0x4076c0
    const char* GetStr(const char* key); // 0x408170
    int Find(const char* key);         // 0x407d50 (value text lands in the shared number buffer, see ReadValue)
    void Load(const char* path);       // 0x406ce0
};
// UI sprite item (0xa0 bytes, ctor 0x4a8580): texture handles at +0/+4, fixed colour/flag words.
struct UiItem {
    DWORD tex[2];
    DWORD f[0x26];
};
#endif
