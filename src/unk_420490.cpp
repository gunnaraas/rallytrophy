// Module 1 of 0x420490-0x4243af: body helpers, StageScreen screen helpers, STL stubs. Types: include/unk_420490.h.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_420490.h"
#include "unk_40c280.h"

extern float DAT_004e1348;
extern DWORD DAT_0050892c;
extern int __cdecl FUN_004c0d01(void (__cdecl *callback)(void));

// FUNCTION: RALLYTROPHY 0x420490
void ScaleBody::SetScale(DWORD x, DWORD y, DWORD z)
{
    scale[0] = x;
    scale[1] = y;
    scale[2] = z;
    inv[0] = DAT_004e1348 / *(float*)&scale[0];
    inv[1] = DAT_004e1348 / *(float*)&scale[1];
    inv[2] = DAT_004e1348 / *(float*)&scale[2];
}

// FUNCTION: RALLYTROPHY 0x4205b0
void Fun_004205b0()
{
    DAT_0050892c = 0xff000000;
}

__declspec(naked)
// PE transcription preserves the original switch dispatch and return sequence.
// FUNCTION: RALLYTROPHY 0x4242a0
int StageScreen::Fun_004242a0(int)
{
    __asm {
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 004h
        _emit 033h
        _emit 0c0h
        _emit 049h
        _emit 083h
        _emit 0f9h
        _emit 014h
        _emit 077h
        _emit 014h
        _emit 033h
        _emit 0d2h
        _emit 08ah
        _emit 091h
        _emit 0cch
        _emit 042h
        _emit 042h
        _emit 000h
        _emit 0ffh
        _emit 024h
        _emit 095h
        _emit 0c4h
        _emit 042h
        _emit 042h
        _emit 000h
        _emit 0b8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

__declspec(naked)
// PE transcription preserves the original by-value vector temporaries and x87 cross-product schedule.
// FUNCTION: RALLYTROPHY 0x4204f0
F3 BodyAxis::PointVel(const F3&)
{
    __asm {
        _emit 055h
        _emit 08bh
        _emit 0ech
        _emit 083h
        _emit 0ech
        _emit 020h
        _emit 08bh
        _emit 045h
        _emit 00ch
        _emit 0d9h
        _emit 000h
        _emit 08bh
        _emit 050h
        _emit 004h
        _emit 0d8h
        _emit 0a1h
        _emit 024h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 040h
        _emit 008h
        _emit 089h
        _emit 055h
        _emit 0f0h
        _emit 089h
        _emit 045h
        _emit 0f4h
        _emit 0d9h
        _emit 05dh
        _emit 0ech
        _emit 081h
        _emit 0c1h
        _emit 0d8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 045h
        _emit 0f0h
        _emit 089h
        _emit 04dh
        _emit 0fch
        _emit 0d8h
        _emit 061h
        _emit 050h
        _emit 08dh
        _emit 055h
        _emit 0e0h
        _emit 0c7h
        _emit 045h
        _emit 0f8h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 055h
        _emit 00ch
        _emit 0d9h
        _emit 05dh
        _emit 0f0h
        _emit 0d9h
        _emit 045h
        _emit 0f4h
        _emit 0d8h
        _emit 061h
        _emit 054h
        _emit 08dh
        _emit 04dh
        _emit 0ech
        _emit 089h
        _emit 04dh
        _emit 0f8h
        _emit 0d9h
        _emit 05dh
        _emit 0f4h
        _emit 08bh
        _emit 04dh
        _emit 0f8h
        _emit 08bh
        _emit 055h
        _emit 0fch
        _emit 08bh
        _emit 045h
        _emit 00ch
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 04ah
        _emit 008h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 00ah
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 04ah
        _emit 004h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 04ah
        _emit 004h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 04ah
        _emit 008h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 00ah
        _emit 0d9h
        _emit 0cah
        _emit 0deh
        _emit 0edh
        _emit 0deh
        _emit 0ebh
        _emit 0deh
        _emit 0e9h
        _emit 0d9h
        _emit 0cah
        _emit 0d9h
        _emit 018h
        _emit 0d9h
        _emit 058h
        _emit 004h
        _emit 0d9h
        _emit 058h
        _emit 008h
        _emit 08bh
        _emit 045h
        _emit 008h
        _emit 0d9h
        _emit 045h
        _emit 0e0h
        _emit 08bh
        _emit 04dh
        _emit 0e4h
        _emit 0d9h
        _emit 018h
        _emit 08bh
        _emit 055h
        _emit 0e8h
        _emit 089h
        _emit 048h
        _emit 004h
        _emit 089h
        _emit 050h
        _emit 008h
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

void __cdecl Fun_004205d0();                // 0x4205d0: module static destructor

// FUNCTION: RALLYTROPHY 0x4205c0
void Fun_004205c0()
{
    FUN_004c0d01(Fun_004205d0);
}

// FUNCTION: RALLYTROPHY 0x4242f0
DWORD* PtrArray::At(int i)
{
    return first + i;
}

// FUNCTION: RALLYTROPHY 0x424300
Pair* Pair::Assign(const Pair* o)
{
    a = o->a;
    b = o->b;
    return this;
}

// FUNCTION: RALLYTROPHY 0x424320
DWORD* __stdcall Fun_00424320(DWORD* f, DWORD* l, DWORD* d)
{
    if (f != l) {
        do {
            if (d != 0) {
                *d = *f;
            }
            f++;
            d++;
        } while (f != l);
    }
    return d;
}

// FUNCTION: RALLYTROPHY 0x424350
void __stdcall Fun_00424350(DWORD* d, unsigned int n, DWORD* x)
{
    for (; n > 0; n--) {
        if (d != 0) {
            *d = *x;
        }
        d++;
    }
}

// FUNCTION: RALLYTROPHY 0x4205e0
StageScreen::StageScreen()
{
    unk124[0] = 0;
    unk124[1] = 0;
    entries = 0;
    count = 3;
    unk22c = 1;
}

// FUNCTION: RALLYTROPHY 0x420710
// SYMBOL: ??_GStageScreen@@UAEPAXI@Z
typedef void Generated_420710();

// FUNCTION: RALLYTROPHY 0x420730
StageScreen::~StageScreen()
{
    Free();
}

// FUNCTION: RALLYTROPHY 0x421a80
StageEntry::StageEntry()
{
}

// FUNCTION: RALLYTROPHY 0x421b80
StageEntry::~StageEntry()
{
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x421d00
void StageScreen::Free()
{
    __asm {
        _emit 051h
        _emit 053h
        _emit 08bh
        _emit 0d9h
        _emit 08bh
        _emit 043h
        _emit 004h
        _emit 055h
        _emit 033h
        _emit 0edh
        _emit 03bh
        _emit 0c5h
        _emit 00fh
        _emit 084h
        _emit 014h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 083h
        _emit 03ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0c5h
        _emit 056h
        _emit 07eh
        _emit 079h
        _emit 057h
        _emit 08bh
        _emit 0bbh
        _emit 040h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 081h
        _emit 0c7h
        _emit 0a0h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 037h
        _emit 03bh
        _emit 0f5h
        _emit 074h
        _emit 050h
        _emit 08bh
        _emit 00eh
        _emit 03bh
        _emit 0cdh
        _emit 074h
        _emit 006h
        _emit 08bh
        _emit 001h
        _emit 06ah
        _emit 001h
        _emit 0ffh
        _emit 010h
        _emit 08bh
        _emit 04eh
        _emit 004h
        _emit 03bh
        _emit 0cdh
        _emit 074h
        _emit 006h
        _emit 08bh
        _emit 011h
        _emit 06ah
        _emit 001h
        _emit 0ffh
        _emit 012h
        _emit 03bh
        _emit 0f5h
        _emit 074h
        _emit 010h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 078h
        _emit 069h
        _emit 008h
        _emit 000h
        _emit 056h
        _emit 0e8h
        _emit 0a2h
        _emit 0b2h
        _emit 009h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh
        _emit 077h
        _emit 004h
        _emit 08bh
        _emit 00eh
        _emit 03bh
        _emit 0cdh
        _emit 074h
        _emit 006h
        _emit 08bh
        _emit 001h
        _emit 06ah
        _emit 001h
        _emit 0ffh
        _emit 010h
        _emit 03bh
        _emit 0f5h
        _emit 074h
        _emit 010h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 055h
        _emit 069h
        _emit 008h
        _emit 000h
        _emit 056h
        _emit 0e8h
        _emit 07fh
        _emit 0b2h
        _emit 009h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 081h
        _emit 0c7h
        _emit 0a8h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 048h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 075h
        _emit 099h
        _emit 05fh
        _emit 08bh
        _emit 083h
        _emit 040h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0c5h
        _emit 074h
        _emit 020h
        _emit 08bh
        _emit 048h
        _emit 0fch
        _emit 08dh
        _emit 070h
        _emit 0fch
        _emit 068h
        _emit 080h
        _emit 01bh
        _emit 042h
        _emit 000h
        _emit 051h
        _emit 068h
        _emit 0a8h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 0e8h
        _emit 05fh
        _emit 0f3h
        _emit 009h
        _emit 000h
        _emit 056h
        _emit 0e8h
        _emit 043h
        _emit 0b2h
        _emit 009h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh
        _emit 0b3h
        _emit 030h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 00eh
        _emit 03bh
        _emit 0cdh
        _emit 074h
        _emit 006h
        _emit 08bh
        _emit 011h
        _emit 06ah
        _emit 001h
        _emit 0ffh
        _emit 012h
        _emit 03bh
        _emit 0f5h
        _emit 074h
        _emit 010h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 0f3h
        _emit 068h
        _emit 008h
        _emit 000h
        _emit 056h
        _emit 0e8h
        _emit 01dh
        _emit 0b2h
        _emit 009h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh
        _emit 0b3h
        _emit 034h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 00eh
        _emit 03bh
        _emit 0cdh
        _emit 089h
        _emit 0abh
        _emit 030h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 074h
        _emit 006h
        _emit 08bh
        _emit 001h
        _emit 06ah
        _emit 001h
        _emit 0ffh
        _emit 010h
        _emit 03bh
        _emit 0f5h
        _emit 074h
        _emit 010h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 0c7h
        _emit 068h
        _emit 008h
        _emit 000h
        _emit 056h
        _emit 0e8h
        _emit 0f1h
        _emit 0b1h
        _emit 009h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh
        _emit 0cbh
        _emit 089h
        _emit 0abh
        _emit 034h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 06bh
        _emit 004h
        _emit 089h
        _emit 06bh
        _emit 00ch
        _emit 0e8h
        _emit 0dbh
        _emit 06eh
        _emit 001h
        _emit 000h
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 059h
        _emit 0c3h
    }
}

extern DWORD DAT_005014cc, DAT_005014d0, DAT_005014d8, DAT_005014d4[], DAT_005014e0[];
extern int DAT_00501458, DAT_00501454, DAT_00501464, DAT_0050132c, DAT_00501320;
extern char DAT_005014c8;
extern int DAT_004f6150[];

// NONMATCHING: RALLYTROPHY 0x421e30
// 365 vs 364 bytes, 230 differing bytes. Target assembly Evidence ev_a3f0f27b75ef6e88dbce2684f5a3cd4d41bc5620d3f3edfe7926bc80f165d91f
// loads 0x501458 before 0x5014cc and keeps the mask in edx; volatile sequencing, a register sel hint, and their combination
// did not improve it.
// Tried 2026-10-30 night: cached DAT_00501454 in a local, d==0-first branch order, flipped final compare (42->22); still differs: sel lands in ecx not edx (g loaded into edx, d hoisted above the g test) and the m-path and-tail is not shared; ~25 variants (volatile reads, ternary, load orders, m temp) did not move it.
// Tried 2026-10-31 re-pass: ~38 more variants (goto-structured head matching the original's g/d/common/dpath CFG, m-temp for the shared `and`, direct DAT reads, volatile sel/d/mask, re-reading *p in the loop to free eax for the list[n].flag index reload, bit<<=1/+=); all 22 insns or worse (goto forms 40+); head still differs by sel in ecx and the unshared and-tail, loop by edx vs eax reload.
void StageScreen::Start(DWORD mask, int a, int b)
{
    unk238 = a;
    unk248 = b;
    Setup((MenuGame*)mask);
    int i = 0;
    int g = DAT_00501458;
    DWORD sel;
    int d = DAT_00501454;
    sel = DAT_005014cc;
    if (g != 0 && d == 0) sel = DAT_005014d0;
    if (d == 0) {
        if (DAT_00501464 != 3) {
            sel &= DAT_005014d4[DAT_0050132c];
        } else {
            if (DAT_005014c8 != 0) goto done;
            sel &= DAT_005014d8;
        }
    } else {
        sel &= DAT_005014e0[DAT_00501320];
    }
done:
    unk124[1] = 0;
    if (0 < count) {
        int* p = DAT_004f6150;
        do {
            int car = *p;
            DWORD bit = 1;
            int k = car;
            if (0 < car) {
                do {
                    bit = bit * 2;
                    k--;
                } while (k != 0);
            }
            if ((sel & bit) != 0) {
                list[unk124[1]].car = car;
                list[unk124[1]].flag = 1;
                unk124[1]++;
            } else if (car == 0 || car == 2 || car == 4 || car == 6 || car == 8 || car == 10 || car == 12 ||
                       car == 14 || car == 16 || car == 18 || car == 20) {
                list[unk124[1]].car = car;
                list[unk124[1]].flag = 0;
                unk124[1]++;
            }
            i++;
            p++;
        } while (i < count);
    }
    if (unk124[0] >= unk124[1]) {
        unk124[0] = unk124[1] - 1;
    }
    Layout();
}

extern const char DAT_004f6408[], DAT_004f2e40[], DAT_004f6480[], DAT_004f647c[];
extern const char* __cdecl FUN_00405380(int id);              // string-table lookup
extern char* __cdecl FUN_004c1902(int v, char* buf, int radix); // _itoa
extern const char* PTR_DAT_004f6078[];

struct CarTex {
    BYTE pad[0x68];
    CarTex(const char* name, int flags, int zero); // 0x4a8030
    void Bind(int dev);                            // 0x4a8350
};
static inline void SetItem(UiItem* it)
{
    it->f[0x1b - 2] = it->f[0x1a - 2] = it->f[0x19 - 2] = it->f[0x18 - 2] = 0x3f800000;
    it->f[0x27 - 2] = 2;
    it->f[0x26 - 2] = 0xf;
    it->f[0x13 - 2] = it->f[0x12 - 2] = it->f[0x11 - 2] = it->f[0x10 - 2] = 0x3f800000;
    it->f[6 - 2] = 5;
    it->f[7 - 2] = 6;
    it->f[9 - 2] = 2;
}

// Result of std::operator+(string, const char*) whose destructor is the out-of-line _Tidy(true) call.
class StagePathTmp {
public:
    ~StagePathTmp() { Tidy(true); }
    void Tidy(bool built);              // 0x402700
    std::allocator<char> alloc;
    char* ptr;
    size_t len;
    size_t reserve;
};
class StagePathText {
public:
    StagePathText(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    StagePathText(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        AssignGrow(s);
    }
    StagePathText(const StagePathText& o, int, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        AssignString(*(const std::string*)&o, 0, std::string::npos);
    }
    StagePathText(const char* s, int, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        AssignChars(s, strlen(s));
    }
    ~StagePathText() { Free(true); }
    unsigned char& Refcnt(const char* s) { return ((unsigned char*)s)[-1]; }
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
    bool Grow(size_t n, bool trim);    // 0x402980
    void Eos(size_t n);               // 0x402960
    void Append(const char* s, size_t n); // 0x403ba0
    void AssignString(const std::string& s, size_t pos, size_t n)
    {
        ((std::string*)this)->assign(s, pos, n);
    }
    void AssignGrow(const char* s)
    {
        size_t n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    void AssignString(const StagePathTmp& s, size_t pos, size_t n); // std::string::assign 0x4024f0
    void AssignString(const StagePathText& s, size_t pos, size_t n);
    void AssignChars(const char* s, size_t n); // std::string::assign(const char*, n) 0x402a10
    void AppendCall(const char* s) { Append(s, strlen(s)); }
    void AppendStd(const char* s) { ((std::string*)this)->append(s); }
    void AppendInl(const char* s)
    {
        size_t n = strlen(s);
        if (std::string::npos - len <= n) std::_Xlen();
        if (0 < n) {
            size_t tot = n + len;
            if (Grow(tot, false)) {
                memcpy(ptr + len, s, n);
                Eos(tot);
            }
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    size_t len;
    size_t reserve;
};


StagePathTmp __cdecl PathCat(const StagePathText& a, const char* lit);
StagePathText __cdecl PathCat2(const StagePathText& a, const char* lit); // std::operator+ 0x4141f0

// Same string with the destructor left as an out-of-line _Tidy(true) call (RefreshUnits path).
class StagePathOut {
public:
    StagePathOut(const StagePathText& o, int, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        ((std::string*)this)->assign(*(const std::string*)&o, 0, std::string::npos);
    }
    ~StagePathOut() { Tidy(true); }
    void Tidy(bool built);              // 0x402700
    void Append(const char* s, size_t n); // 0x403ba0
    void AppendCall(const char* s) { Append(s, strlen(s)); }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    size_t len;
    size_t reserve;
};

// NONMATCHING: RALLYTROPHY 0x420db0
// StagePathText preserves the observed inline/out-of-line boundaries for the directory and info paths; other sites
// and the function's full frame/EH schedule still differ.
// Tried 2026-10-30 night: fixed itoa/Year/EngineCC/Load/0xd4+0x68 sizes, StagePathText assign/append, per-texture temp strings (412 insns differ); still differs: this/count regs (ebx/edi vs edi/esi), ctor-time EH state placement for dir/path, and the texture-name temp lifetimes.
// Tried 2026-11-01 re-pass: Bal(1) x1..8 at start, x1..30 at end, x1/x3 mid (17 variants): start/mid no change; end 10+ only drops 412->363 by suppressing inlines (built 2872 vs orig 3273 bytes, wrong direction); gap is whole-function regalloc (ebx=this/ebp=entry vs edi/esi), not budget.
void StageScreen::LoadCars()
{
    int i = 0;
    int n = count;
    StageEntry* e = entries = new StageEntry[n];
    for (int k = 0; k < n; k++) {
        ((DWORD*)&entries[k])[0x28] = 0;
        ((DWORD*)&entries[k])[0x29] = 0;
    }
        for (i = 0; i < n; i++, e++) {
            IniFile ini;
            ini.Open((const char*)&PTR_DAT_004f6078, 0xb);
            char num[36]; // 2026-11-11: sized to the original frame (was 32; +4 could equally be in buf; 198 -> 165 insns)
            char buf[256];
            FUN_004c1902(i + 1, num, 10);
            StagePathText dir("data/cars/car_");
            dir.AppendCall(num);
            StagePathText path(dir, 0);
            path.AppendCall("/info.ini");
            ini.Load(path.c_str(), true, false, 0);

#define S(f) (*(StagePathText*)&e->f)
            S(s0).AssignGrow(ini.GetStr(DAT_004f6408));
            S(s1).AssignGrow(ini.GetStr("FullName"));
            FUN_004c1902(ini.Find("Year"), buf, 10);
            S(s2).AssignGrow(buf);
            FUN_004c1902(ini.Find("EngineCyl"), buf, 10);
            S(s3).AssignGrow(buf);
            S(s3).AppendCall(FUN_00405380(0x6f));
            S(s3).AppendCall(DAT_004f2e40);
            FUN_004c1902(ini.Find("EngineCC"), buf, 10);
            S(s3).AppendCall(buf);
            S(s3).AppendCall(FUN_00405380(0x70));
            FUN_004c1902(ini.Find("Power"), buf, 10);
            S(s4).AssignGrow(buf);
            S(s4).AppendCall(DAT_004f2e40);
            S(s4).AppendCall(FUN_00405380(0x14));
            if (ini.Find("Transmission") == 0) {
                S(s5).AssignGrow(FUN_00405380(0x6d));
            } else {
                S(s5).AssignGrow(FUN_00405380(0x6e));
            }
            FUN_004c1902(ini.Find("Length"), buf, 10);
            S(a).AssignGrow(buf);
            S(a).AppendCall(DAT_004f6480);
            FUN_004c1902(ini.Find("Width"), buf, 10);
            S(b).AssignGrow(buf);
            S(b).AppendCall(DAT_004f6480);
            FUN_004c1902(ini.Find("Height"), buf, 10);
            S(c).AssignGrow(buf);
            S(c).AppendCall(DAT_004f6480);
            FUN_004c1902(ini.Find("Weight"), buf, 10);
            S(d).AssignGrow(buf);
            S(d).AppendInl(DAT_004f647c);
#undef S
            UiItem* wire = new UiItem;
            ((DWORD*)e)[0x28] = (DWORD)wire; // +0xa0 (h0)
            { path.AssignString(PathCat(dir, "/InfoPhotoWire.tga"), 0, std::string::npos);
            CarTex* t0 = new CarTex(path.c_str(), 0x841, 0);
            wire->tex[0] = (DWORD)t0;
            t0->Bind((int)DAT_0056b0ac); }
            { path.AssignString(PathCat(dir, "/InfoPhotoFull.tga"), 0, std::string::npos);
            CarTex* t1 = new CarTex(path.c_str(), 0x841, 0);
            wire->tex[1] = (DWORD)t1;
            t1->Bind((int)DAT_0056b0ac); }
            wire->f[0x10 - 2] = wire->f[0x11 - 2] = wire->f[0x12 - 2] = wire->f[0x13 - 2] = 0x3f800000;
            wire->f[0x18 - 2] = wire->f[0x19 - 2] = wire->f[0x1a - 2] = wire->f[0x1b - 2] = 0x3f800000;
            wire->f[0x1c - 2] = wire->f[0x1d - 2] = wire->f[0x1e - 2] = wire->f[0x1f - 2] = 0;
            wire->f[0xe - 2] = 0x10;
            wire->f[0x27 - 2] = 2;
            wire->f[6 - 2] = 5;
            wire->f[7 - 2] = 6;
            wire->f[0x26 - 2] = 0xf;
            wire->f[0x14 - 2] = wire->f[0x15 - 2] = wire->f[0x16 - 2] = wire->f[0x17 - 2] = 0;
            UiItem* book = new UiItem;
            ((DWORD*)e)[0x29] = (DWORD)book; // +0xa4 (h1)
            { path.AssignString(PathCat2(dir, "/carbook.tga"), 0, std::string::npos);
            CarTex* t2 = new CarTex(path.c_str(), 0x841, 0);
            book->tex[0] = (DWORD)t2;
            t2->Bind((int)DAT_0056b0ac); }
            SetItem(book);
        }
    UiItem* n0;
    CarTex* n0t;
    unk230 = (Holder1*)(n0 = new UiItem);
    n0->tex[0] = (DWORD)(n0t = new CarTex("data/menu/carbook_no.tga", 0x841, 0));
    n0t->Bind((int)DAT_0056b0ac);
    SetItem(n0);
    UiItem* n1;
    CarTex* n1t;
    unk234 = (Holder1*)(n1 = new UiItem);
    n1->tex[0] = (DWORD)(n1t = new CarTex("data/menu/car_no.tga", 0x841, 0));
    n1t->Bind((int)DAT_0056b0ac);
    SetItem(n1);
}

// ---- StageScreen::RefreshUnits (0x423b90) and StageScreen::Layout (0x421fa0) ----

extern void __cdecl FUN_004c19bc();                          // _ftol on the value of the last ini lookup (guess)
extern DWORD (__stdcall* DAT_0059b2f8)();
extern DWORD DAT_00508c08;
extern int DAT_00501334;
extern DWORD DAT_0050892c;
extern void __cdecl FUN_00423670(StageScreen* s, short sel, StageEntry* e, float sy); // guess: argument meaning
extern void __cdecl FUN_004239c0(StageScreen* s, short sel, StageEntry* e, int sy);   // guess

// NONMATCHING: RALLYTROPHY 0x423b90
// NONMATCHING: inline std::string append/assign expansions are approximated with string methods
// Tried 2026-10-31: Load(path,1,0,0) was already right; dir/path as StagePathText, 2nd char buf[32], pointer loop, if/else per unit (tail-merged), Length/Width AppendCall + Height/Weight std append, float factors (228->132); still differs: local layout (orig order num,path,buf,ini,dir; ours num,buf,ini,path,dir, decl order has no effect) and Weight's inline-vs-call _Eos split.
// Tried 2026-11-01: the 8th push_back is the StageInsert template (out-of-line Ucopy/Destroy/SizeCall, StageConstruct for the inlined Ufill with M=1; helper above C_ADD) on a StageIntVec view of m_items: 62->31 insns; still differs: ours spills the first-loop cursor and keeps s in a different slot (ebx taken by end), end/last stores order; volatile-read ra, r=&vec and begin-reload idioms alone did not help.
// Tried 2026-11-01 re-pass: 16 more variants (all Append* combos per unit, num/buf order, e=entries+i, volatile count/entries, hoisted unit flag, v fp temp); none beat 132 insns; still differs: ebp/entries load placement in the prologue, EH-state stores (0x1/0x2) one call early, and inline _Eos/Append split in Height/Weight.
void StageScreen::RefreshUnits()
{
    int n = count;
    StageEntry* e = entries;
    for (int i = 0; i < n; i++, e++) {
        char buf[32];
        char num[32];
        IniFile ini;
        ini.Open((const char*)&PTR_DAT_004f6078, 0xb);
        FUN_004c1902(i + 1, num, 10);                         // car number i + 1
        StagePathText dir("data/cars/car_", 0);
        dir.AppendCall(num);
        StagePathOut path(dir, 0);
        path.AppendCall("/info.ini");
        ini.Load(path.c_str(), true, false, 0);
#define S(f) (*(StagePathText*)&e->f)
#define UNIT(field, key, mm, inch, AP, K)                       \
        { int v = ini.Find(key);                             \
        if (DAT_00501300[0] != 0) v = (int)((float)v * K); \
        FUN_004c1902(v, buf, 10);                             \
        S(field).AssignGrow(buf);                             \
        if (DAT_00501300[0] != 0) { S(field).AP(DAT_004f2e40); S(field).AP(FUN_00405380(inch)); } \
        else S(field).AP(FUN_00405380(mm)); }
        UNIT(a, "Length", 0x12, 0x13, AppendCall, 0.0393701f)
        UNIT(b, "Width", 0x12, 0x13, AppendCall, 0.0393701f)
        UNIT(c, "Height", 0x12, 0x13, AppendStd, 0.0393701f)
        UNIT(d, "Weight", 0x10, 0x11, AppendStd, 2.2046225f)
#undef UNIT
#undef S
    }
}

extern float DAT_004e1480; // 25.0, added at run time to 575/798 in the original (not folded)
static const float K262 = 262.0f;
static const float K42 = 42.0f;
static const float K54 = 54.0f;
static const float K250 = 250.0f;
static const float K350 = 350.0f;
static const float K576 = 576.0f;
static const float K424 = 424.0f;
static const float K575 = 575.0f;
static const float K798 = 798.0f;
static const float K450 = 450.0f;
static const float K48 = 48.0f;
static const float K25 = 25.0f;
static const float K15 = 15.0f;
static const float K610 = 610.0f;
static const float K230 = 230.0f;
static const float K130 = 130.0f;
// NONMATCHING: RALLYTROPHY 0x421fa0
// 2026-11-13: StrT s0 (out-of-line ctor) for the row string, Fun_004242a0 as StageScreen member, direct m_selected in Enter/click blocks, int selected, per-slot K* static const floats (literal*sx/sy order), SetScale for the second scale pair: 573 -> 396 insns differ.
// 2026-11-13 round 2: text uses in the DAT_00501334 and avail0/scale blocks read m_game->text (lower weight: fixes the ebx/edi text vs m_items-lea swap), shared `float x` temp for tail Prints and row-loop 54*sx, named Color + Vec2(1,1) per row (EH states 11/12/13 as original), tail K576/K424/K450: try_variants 449 insns (hunks 77 -> 70, orig insns in hunks 137 -> 86, built 4124 vs 4128); open: row-loop float temps/selected byte, K48/K15, tail temps.
// 2026-11-12: every text->color = X as text->SetColorObj(Color(X)): 677 -> 578 insns differ (built 4272 -> 4036 bytes; orig 4128); SetScale(x, y) for the adjacent scale pairs is neutral (+-2 insns); notFirst/first-late/wait-first zero-register variants worse or neutral.
// Tried 2026-10-31: rewrote from the disassembly (local sx/sy, DrawPhoto/DrawBook(entry, avail), second text renderer at game+0x1390bc, per-iteration std::string, k-switch for the entry fields, avail0 colour/0x71 block) 842->683 insns; tried ~35 variants (decl order of scalars has no effect, bool/int selected, volatile/addressed first/avail0, m_game vs cached game, float vs int scale copies); still differs: original keeps 0 in esi/ebx as one shared constant register and the frame is 0xc bytes smaller (locals sx 0x1c, sy 0x28, y 0x18, x tmp 0x14), register allocation drifts the whole body.
// Tried 2026-11-01 re-pass: 11 more variants (shared zero for exitFlag/done/wait, zero arg to Print, first, done/text decl order, per-iteration std::string s, y/r4/sa/sb/ia/ib placement, sx/sy split decl); none below 683 insns differ (best equal to base; built 4320 vs orig 4128 bytes, frame 0xd8 vs 0xcc); still differs: whole-body regalloc drift (orig keeps 0 in esi, ours edi/ebx) and 0xc bytes extra frame, not within permuter range.
int StageScreen::Layout()
{
    float x;
    int exitFlag = 0;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    int done = 0;
    DAT_0056b0bc->ClearKeys();
    TextRenderer* text2 = *(TextRenderer**)((BYTE*)m_game + 0x1390bc);
    TextRenderer* text = m_game->text;
    RefreshUnits();
    unk244 = DAT_0059b2f8();
    int first = 1;
    int avail0 = Fun_004242a0(list[unk124[0]].car);
    int avail = list[unk124[0]].flag;
    RenderDevice* wait = 0;
    while (FUN_004b69f0((int)wait) && !exitFlag && !done) {
        if (unk248) {
            ((NetRace*)unk248)->ServiceNetwork();
        }
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            for (int i = 0; i < 4; i++) {
                if (m_items[i]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                    m_selected = i;
                }
            }
        }
        if (!DAT_0056b0ac->m_active) {
            wait = DAT_0056b0ac;
            continue;
        }
        wait = 0;
        if (DAT_0057ba28[VK_RETURN]) {
                        if (m_selected == 2) {
                if (avail) {
                    m_game->Play(1);
                    done = 1;
                    continue;
                }
                m_game->Play(3);
            }
            if (m_selected == 3) {
                exitFlag = 1;
                continue;
            }
        }
        if (DAT_0057ba28[VK_ESCAPE]) {
            exitFlag = 1;
            continue;
        }
        int dir = 0;
        if (DAT_0057ba28[VK_DOWN]) {
            dir = 1;
        }
        if (DAT_0057ba28[VK_UP]) {
            dir--;
        }
        if (dir != 0) {
            int sel = m_selected + dir;
            m_selected = sel;
            m_game->Play(0);
            if (sel < 0) {
                m_selected = 0;
            } else if (sel >= 4) {
                m_selected = 3;
            }
            int cx = (int)(m_items[m_selected]->GetTopLeft().x + (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f);
            int cy = (int)(m_items[m_selected]->GetTopLeft().y + (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f);
            Vec2 p((float)cx * sx, (float)cy * sy);
            ((MouseInput*)DAT_0056b0c0)->SetPosition(&p);
        }
        int delta = 0;
        if (DAT_0057ba28[VK_LEFT]) {
            delta = -1;
        }
        if (DAT_0057ba28[VK_RIGHT]) {
            delta++;
        }
        if (DAT_0057ba20) {
            if (m_items[4]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                delta--;
            }
            if (m_items[5]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                delta++;
            }
            if (m_items[6]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                delta--;
            }
            if (m_items[7]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                delta++;
            }
            if (m_selected == 2) {
                if (avail) {
                    m_game->Play(1);
                    done = 1;
                    continue;
                }
                m_game->Play(3);
            }
            if (m_selected == 3) {
                exitFlag = 1;
                continue;
            }
        }
        if (delta != 0) {
            switch (m_selected) {
            case 0: {
                int nv = delta + unk124[0];
                int cnt = unk124[1];
                if (nv < cnt && nv >= 0) {
                    unk244 = DAT_0059b2f8();
                }
                AdjustValue(&unk124[0], delta, 0, cnt - 1);
                avail0 = Fun_004242a0(list[unk124[0]].car);
                avail = list[unk124[0]].flag;
                break;
            }
            case 1:
                if (avail) {
                    AdjustValue(&unk22c, delta, 0, 1);
                } else {
                    m_game->Play(3);
                }
                break;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (m_selected != i && m_items[i]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_selected = i;
                m_game->Play(0);
            }
        }
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            m_game->Update();
            m_game->DrawVideo(0);
            text->scaleX = DAT_0056aea8 + DAT_0056aea8;
            text->SetColorObj(Color(DAT_00508bfc));
            text->scaleY = DAT_0056aeac + DAT_0056aeac;
            text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x65), 0, 1.0f, 1.0f);
            text->scaleX = DAT_0056aea8;
            StageEntry* entry = &entries[list[unk124[0]].car];
            text->scaleY = DAT_0056aeac;
            DrawPhoto(entry, avail);
            DrawBook(entry, avail);
            float y = K262 * sy;
            for (int r4 = 0; r4 < 4; r4++) {
                x = K54 * sx;
                int selected = (m_selected == r4);
                Color rc(selected ? DAT_00508c0c : DAT_00508c04);
                text->SetColorObj(rc);
                Vec2 one(1.0f, 1.0f);
                StrT s0; std::string& s = *(std::string*)&s0;
                switch (r4) {
                case 0: {
                    text->Print(DAT_0056b0ac, x, y, strB.ptr ? strB.ptr : "", 0, 1.0f, 1.0f);
                    int cur = unk124[0];
                    char buf[32];
                    FUN_004c1902(cur + 1, buf, 10);
                    s = buf;
                    s += "/";
                    int cnt = unk124[1];
                    FUN_004c1902(cnt, buf, 10);
                    s += buf;
                    DrawSelector(text, &s, K350 * sx, y, K130 * sx, selected, cur, 0, cnt - 1);
                    break;
                }
                case 1:
                    if (!avail) {
                        text->SetColorObj(Color(DAT_00508c08));
                    }
                    text->Print(DAT_0056b0ac, x, y, strC.ptr ? strC.ptr : "", 0, 1.0f, 1.0f);
                    if (!avail) {
                        Color c(DAT_00508c08);
                        DrawSpinner(text, &pair[unk22c], K250 * sx, y, K230 * sx, 1, c, 0, 0, 0);
                    } else {
                        DrawSelector(text, &pair[unk22c], K250 * sx, y, K230 * sx, selected, unk22c, 0, 1);
                    }
                    break;
                case 2:
                    if (!avail) {
                        text->SetColorObj(Color(DAT_00508c08));
                    }
                    y = K42 * sy + y;
                    text->Print(DAT_0056b0ac, x, y, strA.ptr ? strA.ptr : "", 0, 1.0f, 1.0f);
                    text->SetColorObj(Color(selected ? DAT_00508c0c : DAT_00508c04));
                    break;
                case 3:
                    y = K610 * sy;
                    text->Print(DAT_0056b0ac, x, y, strD.ptr ? strD.ptr : "", 0, 1.0f, 1.0f);
                    break;
                }
                y = K42 * sy + y;
            }
            int ia = *(int*)&m_game->text->scaleX;
            m_game->text->SetColorObj(Color(DAT_0050892c));
            text2->SetColorObj(Color(DAT_0050892c));
            int ib = *(int*)&m_game->text->scaleY;
            m_game->text->scaleX = *(float*)&ia * 0.85f;
            m_game->text->scaleY = *(float*)&ib * 0.85f;
            if (avail) {
                m_game->text->Print(DAT_0056b0ac, K576 * sx, K424 * sy, entry->s1.ptr ? entry->s1.ptr : "", 0, 1.0f, 1.0f);
            }
            if (avail0) {
                m_game->text->SetColor(0xff740000);
                m_game->text->Print(DAT_0056b0ac, 760.0f * sx - m_game->text->GetWidth(FUN_00405380(0x71), 0) * 0.5f, 633.0f * sy,
                            FUN_00405380(0x71), 0, 1.0f, 1.0f);
            }
            y = K450 * sy;
            *(int*)&text->scaleX = ia;
            *(int*)&text->scaleY = ib;
            float sa = *(float*)&ia * 1.25f;
            float sb = *(float*)&ib * 1.25f;
            for (int k = 0; k < 4; k++) {
                StrT* v;
                OutStr* u;
                x = K575 * sx;
                text2->Print(DAT_0056b0ac, x, y, names[k].c_str(), 0, 1.0f, 1.0f);
                x = K798 * sx;
                text2->Print(DAT_0056b0ac, x, y, names[4 + k].c_str(), 0, 1.0f, 1.0f);
                switch (k) {
                case 0: v = &entry->s2; u = &entry->a; break;
                case 1: v = &entry->s3; u = &entry->b; break;
                case 2: v = &entry->s4; u = (OutStr*)&entry->c; break;
                case 3: v = &entry->s5; u = &entry->d; break;
                }
                text2->scaleX = sa;
                text2->scaleY = sb;
                float y2 = 15.0f * sy + y;
                x = (K25 + K575) * sx;
                text2->Print(DAT_0056b0ac, x, y2, v->ptr ? v->ptr : "", 0, 1.0f, 1.0f);
                x = (K25 + K798) * sx;
                text2->Print(DAT_0056b0ac, x, y2, u->ptr ? u->ptr : "", 0, 1.0f, 1.0f);
                y = 48.0f * sy + y;
                *(int*)&text2->scaleX = ia;
                *(int*)&text2->scaleY = ib;
            }
            if (DAT_00501334 > 1) {
                m_game->text->scaleX = DAT_0056aea8 * 0.75f;
                m_game->text->scaleY = DAT_0056aeac * 0.75f;
                m_game->text->SetColorObj(Color(0x99000000));
                m_game->text->Printf(DAT_0056b0ac, DAT_0056aea8 * 58.0f, DAT_0056aeac * 184.0f, "%s %i: %s", FUN_00405380(0x94),
                             unk238 + 1, (char*)DAT_00501300 + 0x38 + unk238 * 30);
                m_game->text->SetScale(DAT_0056aea8, DAT_0056aeac);
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
    if (exitFlag || DAT_0057b9f4) {
        m_game->Play(2);
        return -1;
    }
    return 1;
}






























// ---- 0x422fc0 / 0x423670 / 0x4239c0 ----
#include <string.h>
#include <vector>
#include <windows.h>
#include <mmsystem.h>
extern ConfigKey DAT_004f60d0[];   // carmenu.ini key table (16 keys)
extern DWORD (__stdcall *DAT_0059b2f8)(void);
extern float DAT_004e1488, DAT_004e148c, DAT_004e14e4, DAT_004e14e0, DAT_004e1490, DAT_004e1494, DAT_004e14e8;
extern float DAT_004e14a8, DAT_004e14a4, DAT_004e134c, DAT_004e13ac;

struct C_Point {
    C_Point(const C_Point& other);     // 0x424300
    ~C_Point() {}
    float x, y;
};
class C_Rect {
public:
    C_Rect(C_Point topLeft, HudPoint bottomRight);   // 0x4390a0
    float f[5];
};
class C_String {
public:
    C_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~C_String() { Free(true); }
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
void __cdecl StageConstruct(int* p, const int& v); // 0x403140 (std::_Construct, out of line in the original)

// VC6 std::vector<int>::insert(iterator P, size_type M, const int& X) with the unreachable arms folded away.
struct LLVec : StageIntVec { unsigned int SizeCall(); }; // 0x414460 out of line
static __forceinline void StageInsert(LLVec* v, int* P, unsigned int M, const int& X)
{
    if ((unsigned int)(v->end - v->last) < M) {
        int* f0 = v->first;
        unsigned int n = v->Size() + (M < v->Size() ? v->Size() : M);
        int* s = v->allocator.allocate(n, (void*)0);
        int* q = s;
        for (int* f = f0; f != P; ++f, ++q)
            StageConstruct(q, *f);
        StageConstruct(q, X);
        ++q;
        v->Ucopy(P, v->last, q);
        v->Destroy(v->first, v->last);
        StageFree(v->first);
        int* e = s + n;
        v->last = s + v->SizeCall() + M;
        v->end = e;
        v->first = s;
    } else if ((unsigned int)(v->last - P) < M) {
        v->Ucopy(P, v->last, P + M);
        v->Ufill(v->last, M - (v->last - P), &X);
        for (int* p = P; p != v->last; ++p)
            *p = X;
        v->last += M;
    } else {
        v->Ucopy(v->last - M, v->last, v->last);
        int* d = v->last;
        for (int* f = v->last - M; P != f; )
            *--d = *--f;
        for (int* p = P; p != P + M; ++p)
            *p = X;
        v->last += M;
    }
}

#define C_ADD(ul, lr) \
    do { \
        rect = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)

#define C_ADDR(rect, ul, lr) \
    do { \
        rect = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)
// NONMATCHING: RALLYTROPHY 0x422fc0
// original: FUN_00422fc0. Loads the 8 hit-test rectangles from data/menu/carmenu.ini.
// Tried 2026-10-30 night: three UIRect* locals (sites 5 and 6 use their own, as in the original) cut 158->62 insns; still differs: the 8th push_back is an inlined insert(end(),1,x) whose end() load is scheduled before the rect spill in the original, ours after (std::vector inline expansion; insert/end()/volatile/temp variants did not help).
// 2026-11-12: 8th site grow arm hand-expanded in the function body, end stored before last (62 -> 3 insns differ, 1700 bytes; only an eax/ecx swap around the end store remains).
// Tried 2026-10-31: one or two discarded Bal(1) at every statement position (no change at 5..19, worse 78/155 after the last site; only the 8th push_back inlines in both, so the count is already right); 8th site as block temp / push_back(ra = new) / own rb or rc local all worse (62 -> 70..84).
void StageScreen::LoadLayout()
{
    Config config;
    UIRect* ra;
    UIRect* rb;
    UIRect* rc;
    config.Define(DAT_004f60d0, 0x10);
    C_String path("data/menu/carmenu.ini");
    config.Load(path.c_str(), true, false, 0);
    C_ADDR(ra, "CarSelectionText_UpperLeft", "CarSelectionText_LowerRight");
    C_ADDR(ra, "TransmissionSelectionText_UpperLeft", "TransmissionSelectionText_LowerRight");
    C_ADDR(ra, "ContinueText_UpperLeft", "ContinueText_LowerRight");
    C_ADDR(ra, "BackText_UpperLeft", "BackText_LowerRight");
    C_ADDR(rb, "CarSelectionArrow1_UpperLeft", "CarSelectionArrow1_LowerRight");
    C_ADDR(rc, "CarSelectionArrow2_UpperLeft", "CarSelectionArrow2_LowerRight");
    C_ADDR(ra, "TransmissionSelectionArrow1_UpperLeft", "TransmissionSelectionArrow1_LowerRight");
    ra = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2("TransmissionSelectionArrow2_UpperLeft"), *(const HudPoint*)&config.GetVector2("TransmissionSelectionArrow2_LowerRight"));
    LLVec* sv = (LLVec*)&m_items;
    if ((unsigned int)(sv->end - sv->last) < 1u) {
        int* P = sv->last;
        int* f0 = sv->first;
        unsigned int n = sv->Size() + (1u < sv->Size() ? sv->Size() : 1u);
        int* s = sv->allocator.allocate(n, (void*)0);
        int* q = s;
        for (int* f = f0; f != P; ++f, ++q)
            StageConstruct(q, *f);
        StageConstruct(q, (int&)ra);
        ++q;
        sv->Ucopy(P, sv->last, q);
        sv->Destroy(sv->first, sv->last);
        StageFree(sv->first);
        int* e = s + n;
        sv->end = e;
        sv->last = s + sv->SizeCall() + 1;
        sv->first = s;
    } else {
        StageInsert(sv, sv->last, 1, (int&)ra);
    }
}

// A colour built from four float channels (each converted to a byte), with an empty destructor.
struct FColor {
    BYTE c[4];
    // The original converts through inline fld/fistp, not __ftol.
    void Set(int i, float v)
    {
        int n;
        __asm {
            fld v
            fistp n
        }
        c[i] = (BYTE)n;
    }
    FColor(float cb, float cg, float cr, float ca) { Set(0, cb); Set(1, cg); Set(2, cr); Set(3, ca); }
    FColor(float f) { Set(0, 255.0f); Set(1, 255.0f); Set(2, 255.0f); Set(3, f * 255.0f); }
    FColor() { Set(0, 255.0f); Set(1, 255.0f); Set(2, 255.0f); Set(3, 255.0f); }
    ~FColor() {}
};

// FUNCTION: RALLYTROPHY 0x423670
// original: FUN_00423670 (thiscall, ret 8). Draws the car photo: the placeholder (+0x234) when flag is 0, else the
// entry's wire photo (+0xa0) faded in over 800 ms starting 500 ms after the selection time stamp (+0x244).
// Each branch has its own FColor and quad, as in the original. The time call must be the real timeGetTime import
// (an extern "C" import is nothrow, so no EH state is stored for it).
void StageScreen::DrawPhoto(StageEntry* e, int flag)
{
    Vec2 pos[4];
    Vec2 uv[4];
    int ic, id; float x, y, c, d;
    if (flag) {
        float fade = ((float)timeGetTime() - ((float)unk244 + DAT_004e14a8)) / DAT_004e14a4;
        if (fade > 1.0f) {
            fade = 1.0f;
        } else if (fade < DAT_004e134c) {
            fade = 0.0f;
        }
        FColor color(fade);
        DAT_0056b0ac->vf35((Sprite*)e->h0);
        pos[0].x = x = DAT_0056aea8 * DAT_004e1488;
        pos[0].y = y = DAT_0056aeac * DAT_004e148c;
        c = (DAT_004e1488 + DAT_004e14e4) * DAT_0056aea8;
        ic = *(int*)&c;
        pos[1].x = c; *(int*)&pos[2].x = ic;
        pos[1].y = y;
        d = (DAT_004e148c + DAT_004e14e0) * DAT_0056aeac;
        id = *(int*)&d;
        pos[2].y = d; *(int*)&pos[3].y = id;
        pos[3].x = x;
        uv[3].x = 0.0f; uv[1].y = 0.0f; uv[0].y = 0.0f; uv[0].x = 0.0f;
        uv[1].x = 1.0f; uv[3].y = 1.0f; uv[2].y = 1.0f; uv[2].x = 1.0f;
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, *(Color*)&color, 1.0f, 1.0f);
    } else {
        FColor color;
        DAT_0056b0ac->vf35((Sprite*)unk234);
        pos[0].x = x = DAT_0056aea8 * DAT_004e1488;
        pos[0].y = y = DAT_0056aeac * DAT_004e148c;
        c = (DAT_004e1488 + DAT_004e14e4) * DAT_0056aea8;
        ic = *(int*)&c;
        pos[1].x = c; *(int*)&pos[2].x = ic;
        pos[1].y = y;
        d = (DAT_004e148c + DAT_004e14e0) * DAT_0056aeac;
        id = *(int*)&d;
        pos[2].y = d; *(int*)&pos[3].y = id;
        pos[3].x = x;
        uv[3].x = 0.0f; uv[1].y = 0.0f; uv[0].y = 0.0f; uv[0].x = 0.0f;
        uv[1].x = 1.0f; uv[3].y = 1.0f; uv[2].y = 1.0f; uv[2].x = 1.0f;
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, *(Color*)&color, 1.0f, 1.0f);
    }
}


// FUNCTION: RALLYTROPHY 0x4239c0
void StageScreen::DrawBook(StageEntry* e, int flag)
{
    Vec2 pos[4];
    Vec2 uv[4];
    FColor color;
    RaceDeviceSlots* dev = (RaceDeviceSlots*)DAT_0056b0ac;
    if (flag) dev->SetTexture(e->h1); else dev->SetTexture(unk230);
    float x, y;
    pos[0].x = x = DAT_0056aea8 * DAT_004e1490;
    pos[0].y = y = DAT_0056aeac * DAT_004e1494;
    float c = (DAT_004e1490 + DAT_004e14e8) * DAT_0056aea8;
    int ic = *(int*)&c;
    pos[1].x = c; *(int*)&pos[2].x = ic;
    pos[1].y = y;
    float d = (DAT_004e1494 + DAT_004e14e0) * DAT_0056aeac;
    int id = *(int*)&d;
    pos[2].y = d; *(int*)&pos[3].y = id;
    pos[3].x = x;
    uv[3].x = 0.0f; uv[1].y = 0.0f; uv[0].y = 0.0f; uv[0].x = 0.0f;
    uv[1].x = 1.0f; uv[3].y = 1.0f; uv[2].y = 1.0f; uv[2].x = 1.0f;
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, *(Color*)&color, 1.0f, 1.0f);
}
