// Replay file save/load (0x404100-0x404930). Map: include/unk_404100.h.
#include <string>
#include <string.h>
#include "unk_404100.h"
#include "rallytrophy.h"
#include "unk_4018f0.h"
#include "unk_416240.h"

#define H6AT(type, base, off) (*(type*)((char*)(base) + (off)))

// Current race mode DAT_00501450 (RaceMode in unk_481390.h): players vector at +8/+0xc, local player +0x24, stage +0x30.
struct H6Race {
    char pad0[8];
    int* first;                        // 0x08
    int* last;                         // 0x0c
    char pad10[0x14];
    class H6Player* player;            // 0x24
    int unk28;                         // 0x28
    int unk2c;                         // 0x2c
    S10Stage* stage;                   // 0x30
};
class H6Player {
public:
    char name[0x28];                   // 0x00
    char* car;                         // 0x28 (Car*)
};
extern H6Race* DAT_00501450_;
extern int DAT_0050132c, DAT_00501454, DAT_00501458, DAT_0050145c, DAT_00501464, DAT_00501468;
extern char* __cdecl FUN_004c1902(int value, char* buf, int radix); // _itoa

// std::string seen through its layout; each site is pinned to the expansion depth the compiler used there.
struct H6Str {
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
    void Tidy(bool built);                              // 0x402700
    void AssignCall(const char* s, unsigned int n);     // 0x404ba0
    bool Grow(unsigned int n, bool trim);               // 0x402980
    void Eos(unsigned int n);                           // 0x402960
    static void Xlen();                                 // 0x4bd00b
    H6Str(const char* s, char mode, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        if (mode == 'B')
            AssignB(s);
        else
            AssignC(s);
    }
    H6Str(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        ptr = 0;
        len = 0;
        res = 0;
        AssignCall(s, strlen(s));
    }
    ~H6Str() { Free(); }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free()
    {
        if (ptr != 0) {
            if (Refcnt(ptr) != 0 && Refcnt(ptr) != 255) {
                --Refcnt(ptr);
            } else {
                alloc.deallocate(ptr - 1, res + 2);
            }
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    __forceinline void AssignC(const char* s)
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
    void AppendC(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            Xlen();
        }
        if (n > 0) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                len = total;
                ptr[total] = 0;
            }
        }
    }
};

// NONMATCHING: RALLYTROPHY 0x404100
// 20 bytes: the original jumps to one shared epilogue after the inlined string dtor; VC6 duplicates it here.
// A result local and an explicit string scope both emit identical bytes; a volatile result grows the body to 473 bytes
// and 73 differing bytes. Reverted all forms.
bool ReplayFiles::SaveReplay(int index, const char* label)
{
    H6Player* player = DAT_00501450_->player;
    if (player == 0)
        return false;
    char num[2];
    H6Str s("savegame/replay");
    FUN_004c1902(index, num, 10);
    s.AppendC(num);
    s.AppendC(".dat");
    return Write(s.c_str(), label);
}

// FUNCTION: RALLYTROPHY 0x4042c0
bool ReplayFiles::SaveChamp(void)
{
    H6Race* race = DAT_00501450_;
    H6Player* pl = DAT_00501450_->player;
    if (pl == 0)
        return false;
    H6Str s("savegame/champ", 'B');
    char num[8];
    FUN_004c1902(race->unk28, num, 10);
    s.AppendD(num);
    s.AppendD("_");
    FUN_004c1902(race->unk2c, num, 10);
    s.AppendC(num);
    s.AppendC(".dat");
    bool r = Write(s.c_str(), "autosave");
    return r;
}

// FUNCTION: RALLYTROPHY 0x404590
bool ReplayFiles::Write(const char* filename, const char* label)
{
    H6Player* player = DAT_00501450_->player;
    if (player == 0)
        return false;
    File* f = new File(filename, 0x82);
    int magic = 0x198bf3ae;
    f->Write(&magic, 4);
    magic = 0x20020109;
    f->Write(&magic, 4);
    memset(this, 0, 0x70);
    strcpy((char*)this, label);
    H6AT(int, this, 0x24) = DAT_00501450_->unk28;
    H6AT(int, this, 0x28) = DAT_00501450_->unk2c;
    H6AT(int, this, 0x2c) = DAT_0050132c;
    H6AT(int, this, 0x30) = DAT_00501464;
    H6AT(int, this, 0x34) = DAT_00501468;
    b[0x6c] = DAT_0050145c != 0;
    GetLocalTime((SYSTEMTIME*)&b[0x38]);
    strcpy((char*)&b[0x48], DAT_00501450_->player->name);
    H6AT(int, this, 0x20) = H6AT(int, player, 0x2c);
    if (DAT_00501454 != 0)
        b[0x6d] = 1;
    if (DAT_00501458 != 0)
        b[0x6e] = 1;
    b[0x6f] = 0;
    if (b[0x6d] != 0 || b[0x6e] != 0) {
        int n;
        if (DAT_00501450_->first == 0)
            n = 0;
        else
            n = DAT_00501450_->last - DAT_00501450_->first;
        H6AT(int, this, 0x68) = n - 1;
    }
    f->Write(this, 0x70);
    H6Player* local = DAT_00501450_->player;
    WriteState(local, f);
    if (H6AT(int, this, 0x68) != 0) {
        for (int i = 0;; i++) {
            int n;
            if (DAT_00501450_->first == 0)
                n = 0;
            else
                n = DAT_00501450_->last - DAT_00501450_->first;
            if (i >= n)
                break;
            // shortcut: volatile read keeps the original's second load of the vector start
            H6Player* p = (H6Player*)(*(int* volatile*)&DAT_00501450_->first)[i];
            if (p != local)
                WriteState(p, f);
        }
    }
    DAT_00501450_->stage->Save(f);
    f->Close();
    delete f;
    return true;
}

// FUNCTION: RALLYTROPHY 0x4047c0
void ReplayFiles::WriteState(H6Player* p, File* f)
{
    // The 0x1d4-byte per-player block, in the order the original fills it.
    struct {
        int size;
        char name[0x20];
        int a, b, c;                   // player +0x20, +0x24, +0x2c
        float d;                       // player +0x1a20
        int block[0x5a];               // player +0x1888
        int tail[10];                  // player +0x1a28
        int e, g, h, i;                // player +0x1a50, +0x1a58, +0x1a14, +0x1a6c
    } rec;
    strcpy(rec.name, (char*)p);
    rec.a = H6AT(int, p, 0x20);
    rec.b = H6AT(int, p, 0x24);
    rec.c = H6AT(int, p, 0x2c);
    memcpy(rec.tail, (char*)p + 0x1a28, 0x28);
    rec.e = H6AT(int, p, 0x1a50);
    memcpy(rec.block, (char*)p + 0x1888, 0x168);
    rec.d = H6AT(float, p, 0x1a20);
    rec.g = H6AT(int, p, 0x1a58);
    rec.h = H6AT(int, p, 0x1a14);
    rec.i = H6AT(int, p, 0x1a6c);
    rec.size = 0x1d4;
    f->Write(&rec, 0x1d4);
    char* car = p->car;
    char* sub = car + 0x1c75c;
    f->Write(sub, 0x8c);
    if (H6AT(int, sub, 0x74) != 0)
        f->Write(H6AT(void*, sub, 0x80), H6AT(int, sub, 0x74) << 2);
    if (H6AT(int, sub, 0x78) != 0)
        f->Write(H6AT(void*, sub, 0x84), H6AT(int, sub, 0x78) * 4);
    if (H6AT(int, sub, 0x7c) != 0)
        f->Write(H6AT(void*, sub, 0x88), H6AT(int, sub, 0x7c) * 4);
    int v = H6AT(int, car, 0x1bef0);
    f->Write(&v, 4);
    ((RecList*)(car + 0x1bed0))->WriteTo((RecFile*)f);
}

// FUNCTION: RALLYTROPHY 0x404930
char ReplayFiles::Load(int index, ReplayFiles* record)
{
    record->b[0x6f] = 1;
    H6Str s("savegame/replay", 'C');
    char num[2];
    FUN_004c1902(index, num, 10);
    s.AppendD(num);
    s.AppendD(".dat");
    if (File::Exists(s.c_str())) {
        File* f = new File(s.c_str(), 0x81);
        int magic1, magic2;
        f->Read(&magic1, 4);
        f->Read(&magic2, 4);
        if (magic1 == 0x198bf3ae && magic2 == 0x20020109)
            f->Read(record, 0x70);
        f->Close();
        delete f;
    }
    return record->b[0x6f];
}
