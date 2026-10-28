// Module 1 of 0x420490-0x4243af: body helpers, StageScreen screen helpers, STL stubs. Types: include/unk_420490.h.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_420490.h"

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
int __stdcall Fun_004242a0(int)
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
void StageScreen::Start(DWORD mask, int a, int b)
{
    unk238 = a;
    unk248 = b;
    Setup((MenuGame*)mask);
    int i = 0;
    DWORD sel = DAT_005014cc;
    if (DAT_00501458 != 0 && DAT_00501454 == 0) {
        sel = DAT_005014d0;
    }
    if (DAT_00501454 != 0) {
        sel &= DAT_005014e0[DAT_00501320];
    } else {
        DWORD m;
        if (DAT_00501464 != 3) {
            m = DAT_005014d4[DAT_0050132c];
        } else {
            if (DAT_005014c8 != 0) {
                goto done;
            }
            m = DAT_005014d8;
        }
        sel &= m;
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
    if (unk124[1] <= unk124[0]) {
        unk124[0] = unk124[1] - 1;
    }
    Layout();
}

extern const char DAT_004f6408[], DAT_004f2e40[], DAT_004f6480[], DAT_004f647c[];
extern const char* __cdecl FUN_00405380(int id);              // string-table lookup
extern void __cdecl FUN_004c1902(char* buf);                  // value text of the last ini lookup -> buf
extern void* __cdecl FUN_004a8030(void* mem, const char* name, int flags, int zero);
extern void __cdecl FUN_004a8350(void* tex, int screen);
extern UiItem* __cdecl FUN_004a8580(void* mem);               // UiItem ctor

static UiItem* MakeItem(const char* tga)
{
    UiItem* it = FUN_004a8580(operator new(0xa0));
    void* t = FUN_004a8030(operator new(0x40), tga, 0x841, 0);
    it->tex[0] = (DWORD)t;
    FUN_004a8350(t, (int)DAT_0056b0ac);
    return it;
}

static void SetItem(UiItem* it)
{
    it->f[0x18 - 2] = it->f[0x19 - 2] = it->f[0x1a - 2] = it->f[0x1b - 2] = 0x3f800000;
    it->f[0x10 - 2] = it->f[0x11 - 2] = it->f[0x12 - 2] = it->f[0x13 - 2] = 0x3f800000;
    it->f[0x27 - 2] = 2;
    it->f[0x26 - 2] = 0xf;
    it->f[9 - 2] = 2;
    it->f[6 - 2] = 5;
    it->f[7 - 2] = 6;
}

class StagePathText {
public:
    StagePathText(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
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
void StageScreen::LoadCars()
{
    int n = count;
    entries = new StageEntry[n];
    if (0 < n) {
        for (int i = 0; i < n; i++) {
            ((DWORD*)&entries[i])[0x28] = 0;
            ((DWORD*)&entries[i])[0x29] = 0;
        }
        for (int j = 0; j < n; j++) {
            StageEntry* e = &entries[j];
            IniFile ini;
            char num[32];
            char buf[256];
            FUN_004c1902(num);
            StagePathText dir;
            dir.AssignGrow("data/cars/car_");
            dir.AppendCall(num);
            StagePathText path;
            path.AssignString(*(std::string*)&dir, 0, std::string::npos);
            path.AppendCall("/info.ini");
            ini.Open(path.c_str(), 0xb);

#define S(f) (*(std::string*)&e->f)
            S(s0).assign(ini.GetStr(DAT_004f6408));
            S(s1).assign(ini.GetStr("FullName"));
            ini.Find("EngineCyl");
            FUN_004c1902(buf);
            S(s2).assign(buf);
            ini.Find("EngineCC");
            FUN_004c1902(buf);
            S(s3).assign(buf);
            S(s3).append(FUN_00405380(0x6f));
            S(s3).append(DAT_004f2e40);
            S(s3).append(buf);
            S(s3).append(FUN_00405380(0x70));
            ini.Find("Power");
            FUN_004c1902(buf);
            S(s4).assign(buf);
            S(s4).append(DAT_004f2e40);
            S(s4).append(FUN_00405380(0x14));
            if (ini.Find("Transmission") == 0) {
                S(s5).assign(FUN_00405380(0x6d));
            } else {
                S(s5).assign(FUN_00405380(0x6e));
            }
            ini.Find("Length");
            FUN_004c1902(buf);
            S(a).assign(buf);
            S(a).append(DAT_004f6480);
            ini.Find("Width");
            FUN_004c1902(buf);
            S(b).assign(buf);
            S(b).append(DAT_004f6480);
            ini.Find("Height");
            FUN_004c1902(buf);
            S(c).assign(buf);
            S(c).append(DAT_004f6480);
            ini.Find("Weight");
            FUN_004c1902(buf);
            S(d).assign(buf);
            S(d).append(DAT_004f647c);
#undef S
            UiItem* wire = MakeItem((*(std::string*)&dir + "/InfoPhotoWire.tga").c_str());
            wire->tex[1] = (DWORD)FUN_004a8030(operator new(0x40), (*(std::string*)&dir + "/InfoPhotoFull.tga").c_str(), 0x841, 0);
            FUN_004a8350((void*)wire->tex[1], (int)DAT_0056b0ac);
            wire->f[0x10 - 2] = wire->f[0x11 - 2] = wire->f[0x12 - 2] = wire->f[0x13 - 2] = 0x3f800000;
            wire->f[0x18 - 2] = wire->f[0x19 - 2] = wire->f[0x1a - 2] = wire->f[0x1b - 2] = 0x3f800000;
            wire->f[0x1c - 2] = wire->f[0x1d - 2] = wire->f[0x1e - 2] = wire->f[0x1f - 2] = 0;
            wire->f[0xe - 2] = 0x10;
            wire->f[0x27 - 2] = 2;
            wire->f[6 - 2] = 5;
            wire->f[7 - 2] = 6;
            wire->f[0x26 - 2] = 0xf;
            wire->f[0x14 - 2] = wire->f[0x15 - 2] = wire->f[0x16 - 2] = wire->f[0x17 - 2] = 0;
            ((DWORD*)e)[0x28] = (DWORD)wire; // +0xa0 (h0)
            UiItem* book = MakeItem((*(std::string*)&dir + "/carbook.tga").c_str());
            SetItem(book);
            ((DWORD*)e)[0x29] = (DWORD)book; // +0xa4 (h1)
        }
    }
    unk230 = (Holder1*)MakeItem("data/menu/carbook_no.tga");
    SetItem((UiItem*)unk230);
    unk234 = (Holder1*)MakeItem("data/menu/car_no.tga");
    SetItem((UiItem*)unk234);
}

// ---- StageScreen::RefreshUnits (0x423b90) and StageScreen::Layout (0x421fa0) ----
extern const char* PTR_DAT_004f6078[];
extern void __cdecl FUN_004c19bc();                          // _ftol on the value of the last ini lookup (guess)
extern DWORD (__stdcall* DAT_0059b2f8)();
extern DWORD DAT_00508c08;
extern int DAT_00501334;
extern DWORD DAT_0050892c;
extern void __cdecl FUN_00423670(StageScreen* s, short sel, StageEntry* e, float sy); // guess: argument meaning
extern void __cdecl FUN_004239c0(StageScreen* s, short sel, StageEntry* e, int sy);   // guess

// NONMATCHING: RALLYTROPHY 0x423b90
// NONMATCHING: inline std::string append/assign expansions are approximated with string methods
void StageScreen::RefreshUnits()
{
    for (int i = 0; i < count; i++) {
        StageEntry* e = &entries[i];
        IniFile ini;
        char num[32];
        ini.Open((const char*)&PTR_DAT_004f6078, 0xb);
        FUN_004c1902(num);                                    // car number i + 1
        std::string dir("data/cars/car_");
        dir.append(num);
        std::string path(dir);
        path.append("/info.ini");
        ini.Load(path.c_str());
#define S(f) (*(std::string*)&e->f)
#define UNIT(field, key, mm, inch, plain)                     \
        ini.Find(key);                                        \
        if (DAT_00501300[0] != 0) FUN_004c19bc();             \
        FUN_004c1902(num);                                    \
        S(field).assign(num);                                 \
        if (DAT_00501300[0] == 0) {                           \
            S(field).append(FUN_00405380(mm));                \
        } else {                                              \
            S(field).append(DAT_004f2e40);                    \
            S(field).append(FUN_00405380(inch));              \
        }
        UNIT(a, "Length", 0x12, 0x13, 0)
        UNIT(b, "Width", 0x12, 0x13, 0)
        UNIT(c, "Height", 0x12, 0x13, 0)
        UNIT(d, "Weight", 0x10, 0x11, 0)
#undef UNIT
#undef S
    }
}

// NONMATCHING: RALLYTROPHY 0x421fa0
// NONMATCHING: draft; stack frame, inline strings and the argument lists of 0x423670/0x4239c0/DrawSelector are guesses
// TODO: restore the first-iteration iStack_84 block (colour 0xff740000, text 0x71) the draft dropped as dead.
int StageScreen::Layout()
{
    MenuGame* game = m_game;
    int exitFlag = 0, done = 0;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    DAT_0056b0bc->ClearKeys();
    TextRenderer* text = game->text;
    RenderDevice* dev = DAT_0056b0ac;
    RenderDevice* waitDev = 0;
    int first = 1;
    RenderDevice* wait = 0;
    unk244 = DAT_0059b2f8();
    RefreshUnits();
    unsigned avail0 = Fun_004242a0(list[unk124[0]].car);
    int avail = list[unk124[0]].flag;
    (void)avail0;
    (void)waitDev;
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
        if (!dev->m_active) {
            wait = dev;
            continue;
        }
        wait = 0;
        if (DAT_0057ba28[VK_RETURN]) {
            if (m_selected == 2) {
                if (avail) {
                    game->Play(1);
                    done = 1;
                    continue;
                }
                game->Play(3);
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
        int dir = DAT_0057ba28[VK_DOWN] != 0;
        if (DAT_0057ba28[VK_UP]) {
            dir--;
        }
        if (dir != 0) {
            int sel = m_selected + dir;
            m_selected = sel;
            game->Play(0);
            if (sel < 0) {
                m_selected = 0;
            } else if (sel > 3) {
                m_selected = 3;
            }
            UIRect* r = m_items[m_selected];
            int cx = (int)(r->GetTopLeft().x + (r->GetBottomRight().x - r->GetTopLeft().x) * 0.5f);
            int cy = (int)(r->GetTopLeft().y + (r->GetBottomRight().y - r->GetTopLeft().y) * 0.5f);
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
            HudPoint mouse = *(HudPoint*)&DAT_0057ba00;
            if (m_items[4]->Contains(mouse)) {
                delta--;
            }
            if (m_items[5]->Contains(mouse)) {
                delta++;
            }
            if (m_items[6]->Contains(mouse)) {
                delta--;
            }
            if (m_items[7]->Contains(mouse)) {
                delta++;
            }
            int s = m_selected;
            if (s == 2) {
                if (avail) {
                    game->Play(1);
                    done = 1;
                    continue;
                }
                game->Play(3);
            }
            if (s == 3) {
                exitFlag = 1;
                continue;
            }
        }
        if (delta != 0) {
            if (m_selected == 0) {
                int nv = delta + unk124[0];
                if (nv < unk124[1] && -1 < nv) {
                    unk244 = DAT_0059b2f8();
                }
                AdjustValue(&unk124[0], delta, 0, unk124[1] - 1);
                Fun_004242a0(list[unk124[0]].car);
                avail = list[unk124[0]].flag;
            } else if (m_selected == 1) {
                if (avail == 0) {
                    game->Play(3);
                } else {
                    AdjustValue(&unk22c, delta, 0, 1);
                }
            }
        }
        for (int i = 0; i < 4; i++) {
            if (m_selected != i && m_items[i]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_selected = i;
                game->Play(0);
            }
        }
        if (dev->BeginScene()) {
            dev->vf38(0);
            game->Update();
            game->DrawVideo(0);
            float xs = DAT_0056aea8, ys = DAT_0056aeac;
            text->scaleX = xs + xs;
            text->scaleY = ys + ys;
            text->color = DAT_00508bfc;
            text->Print(dev, 54.0f * xs, 142.0f * ys, FUN_00405380(0x65), 0, 1.0f, 1.0f);
            StageEntry* entry = &entries[list[unk124[0]].car];
            text->scaleX = xs;
            text->scaleY = ys;
            FUN_00423670(this, (short)unk124[0], entry, sy);
            FUN_004239c0(this, (short)unk124[0], entry, (int)sy);
            float y = 262.0f * ys;
            for (int r4 = 0; r4 < 4; r4++) {
                int selected = (m_selected == r4);
                text->color = selected ? DAT_00508c0c : DAT_00508c04;
                switch (r4) {
                case 0: {
                    text->Print(dev, 54.0f * xs, y, strB.ptr ? strB.ptr : "", 0, 1.0f, 1.0f);
                    char b1[32], b2[32];
                    sprintf(b1, "%d", unk124[0] + 1);  // guess: "current/total"
                    sprintf(b2, "%d", unk124[1]);
                    std::string s(b1);
                    s.append("/");
                    s.append(b2);
                    DrawSelector(text, &s, 350.0f * xs, y, 130.0f * xs, selected, 0, 0, 1);
                    break;
                }
                case 1:
                    if (!avail) {
                        text->color = DAT_00508c08;
                    }
                    text->Print(dev, 54.0f * xs, y, strC.ptr ? strC.ptr : "", 0, 1.0f, 1.0f);
                    if (!avail) {
                        Color c(DAT_00508c08);
                        DrawSpinner(text, &pair[unk22c], 250.0f * xs, y, 230.0f * xs, 1, c, 0, 0, 1);
                    } else {
                        DrawSelector(text, &pair[unk22c], 250.0f * xs, y, 230.0f * xs, selected, 0, 0, 1);
                    }
                    break;
                case 2:
                    if (!avail) {
                        text->color = DAT_00508c08;
                    }
                    y = 42.0f * ys + y;
                    text->Print(dev, 54.0f * xs, y, strA.ptr ? strA.ptr : "", 0, 1.0f, 1.0f);
                    text->color = selected ? DAT_00508c0c : DAT_00508c04;
                    break;
                case 3:
                    y = 610.0f * ys;
                    text->Print(dev, 54.0f * xs, y, strD.ptr ? strD.ptr : "", 0, 1.0f, 1.0f);
                    break;
                }
                y = 42.0f * ys + y;
            }
            float a0 = text->scaleX, b0 = text->scaleY;
            text->color = DAT_0050892c;
            text->scaleX = a0 * 0.85f;
            text->scaleY = b0 * 0.85f;
            if (avail) {
                text->Print(dev, 576.0f * xs, 424.0f * ys, entry->s1.ptr ? entry->s1.ptr : "", 0, 1.0f, 1.0f);
            }
            text->scaleX = a0;
            text->scaleY = b0;
            y = 450.0f * ys;
            for (int k = 0; k < 4; k++) {
                text->Print(dev, 575.0f * xs, y, names[k].c_str(), 0, 1.0f, 1.0f);
                text->Print(dev, 798.0f * xs, y, names[4 + k].c_str(), 0, 1.0f, 1.0f);
                StrT* v = &entry->s2 + k;                    // 0x20 + i * 0x10
                OutStr* u = &entry->a + k;                   // 0x60 + i * 0x10
                text->scaleX = a0 * 1.25f;
                text->scaleY = b0 * 1.25f;
                float y2 = 15.0f * ys + y;
                text->Print(dev, 600.0f * xs, y2, v->ptr ? v->ptr : "", 0, 1.0f, 1.0f);
                text->Print(dev, 823.0f * xs, y2, u->ptr ? u->ptr : "", 0, 1.0f, 1.0f);
                y = 48.0f * ys + y;
                text->scaleX = a0;
                text->scaleY = b0;
            }
            if (DAT_00501334 > 1) {
                text->scaleX = xs * 0.75f;
                text->scaleY = ys * 0.75f;
                text->color = 0x99000000;
                text->Printf(dev, 58.0f * xs, 184.0f * ys, "%s %i: %s", FUN_00405380(0x94), DAT_00501334,
                             (char*)DAT_00501300 + 0x38); // guess: arguments
                text->scaleX = xs;
                text->scaleY = ys;
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 2, 0, 0, 1, -1.0f);
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(4, 1, 0, 1, 1, -1.0f);
            DrawCursor(*((MouseInput*)DAT_0056b0c0)->GetPosition());
            dev->EndScene();
            dev->Present();
        }
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    if (exitFlag || DAT_0057b9f4) {
        game->Play(2);
        return -1;
    }
    return 1;
}

// ---- 0x422fc0 / 0x423670 / 0x4239c0 ----
#include <string.h>
#include <vector>
#include <windows.h>
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
#define C_ADD(ul, lr) \
    do { \
        rect = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)

// NONMATCHING: RALLYTROPHY 0x422fc0
// original: FUN_00422fc0. Loads the 8 hit-test rectangles from data/menu/carmenu.ini.
void StageScreen::LoadLayout()
{
    Config config;
    UIRect* rect;
    config.Define(DAT_004f60d0, 0x10);
    C_String path("data/menu/carmenu.ini");
    config.Load(path.c_str(), true, false, 0);
    C_ADD("CarSelectionText_UpperLeft", "CarSelectionText_LowerRight");
    C_ADD("TransmissionSelectionText_UpperLeft", "TransmissionSelectionText_LowerRight");
    C_ADD("ContinueText_UpperLeft", "ContinueText_LowerRight");
    C_ADD("BackText_UpperLeft", "BackText_LowerRight");
    C_ADD("CarSelectionArrow1_UpperLeft", "CarSelectionArrow1_LowerRight");
    C_ADD("CarSelectionArrow2_UpperLeft", "CarSelectionArrow2_LowerRight");
    C_ADD("TransmissionSelectionArrow1_UpperLeft", "TransmissionSelectionArrow1_LowerRight");
    C_ADD("TransmissionSelectionArrow2_UpperLeft", "TransmissionSelectionArrow2_LowerRight");
}

// A colour built from four float channels (each converted to a byte), with an empty destructor.
struct FColor {
    BYTE c[4];
    void Set(int i, float v) { c[i] = (BYTE)v; }
    FColor(float cb, float cg, float cr, float ca) { Set(0, cb); Set(1, cg); Set(2, cr); Set(3, ca); }
    ~FColor() {}
};

// NONMATCHING: RALLYTROPHY 0x423670
// original: FUN_00423670 (thiscall, ret 8). Draws the car photo: the placeholder (+0x234) when flag is 0, else the
// entry's wire photo (+0xa0) faded in over 800 ms starting 500 ms after the selection time stamp (+0x244).
void StageScreen::DrawPhoto(StageEntry* e, int flag)
{
    Vec2 pos[4];
    Vec2 uv[4];
    Color color;
    if (flag == 0) {
        DAT_0056b0ac->vf35((Sprite*)unk234);
        color.value = -1;
    } else {
        float fade = ((float)DAT_0059b2f8() - ((float)unk244 + DAT_004e14a8)) / DAT_004e14a4;
        if (fade <= 1.0f) {
            if (fade < DAT_004e134c) {
                fade = 0.0f;
            }
        } else {
            fade = 1.0f;
        }
        int alpha = (int)(fade * DAT_004e13ac);
        color.value = (alpha << 24) | 0xffffff;
        DAT_0056b0ac->vf35((Sprite*)e->h0);
    }
    pos[0].x = pos[3].x = DAT_0056aea8 * DAT_004e1488;
    pos[0].y = pos[1].y = DAT_0056aeac * DAT_004e148c;
    pos[1].x = pos[2].x = (DAT_004e1488 + DAT_004e14e4) * DAT_0056aea8;
    pos[2].y = pos[3].y = (DAT_004e148c + DAT_004e14e0) * DAT_0056aeac;
    uv[0].x = uv[3].x = 0.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[2].y = uv[3].y = 1.0f;
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, *(Color*)&color, 1.0f, 1.0f);
}

// NONMATCHING: RALLYTROPHY 0x4239c0
// original: FUN_004239c0 (thiscall, ret 8). Draws the car book page: the placeholder (+0x230) when flag is 0, else the
// entry's book sprite (+0xa4).
void StageScreen::DrawBook(StageEntry* e, int flag)
{
    Vec2 pos[4];
    Vec2 uv[4];
    FColor color(255.0f, 255.0f, 255.0f, 255.0f);
    DAT_0056b0ac->vf35((Sprite*)(flag == 0 ? (void*)unk230 : (void*)e->h1));
    pos[0].x = pos[3].x = DAT_0056aea8 * DAT_004e1490;
    pos[0].y = pos[1].y = DAT_0056aeac * DAT_004e1494;
    pos[1].x = pos[2].x = (DAT_004e1490 + DAT_004e14e8) * DAT_0056aea8;
    pos[2].y = pos[3].y = (DAT_004e1494 + DAT_004e14e0) * DAT_0056aeac;
    uv[0].x = uv[3].x = 0.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[2].y = uv[3].y = 1.0f;
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, *(Color*)&color, 1.0f, 1.0f);
}
