// Stage sound-point loader (0x416260): data/stage<N>.npf (0x2c-byte records at +0x24c8, count +0x24cc)
// and data/stage<N>.spf (SoundSource array at +0x24d4, count +0x24d0).
#include <string>
#include <new>
#include <stdlib.h>
#include <string.h>
#include "rallytrophy.h"
#include "unk_416260.h"

#define AT(T, off) (*(T*)((char*)this + (off)))

struct NpfRec {            // 0x2c bytes: 4 speech ids, two Vector3 at 0x14 and 0x20 (array through the EH iterators)
    char id[4];
    char pad[0x28];
    NpfRec() {}
    ~NpfRec();             // 0x40b480
};
struct SfxRec {            // sound table entry (stride 0x20) in the Game object at +0x139490
    BYTE alloc;
    const char* ptr;       // +4
    unsigned int len;      // +8
    unsigned int res;      // +0xc
    float height;          // +0x10
    int volume;            // +0x14
    float nearDist;        // +0x18
    float farDist;         // +0x1c
};
extern int DAT_0050143c;
extern char* DAT_0056b0c8;
extern char DAT_00508440[];   // _itoa scratch buffer

// The one path string of LoadSoundFiles, in the flavours the original has at each site: default ctor and the
// first appends out of line, then appends expanded around _Grow (0x402980) / _Eos (0x402960) calls, and the
// in-loop assign/append expanded with _Eos inline.
struct SndPath {
    SndPath(const std::string& x) : alloc(*(const std::allocator<char>*)&x) { Tidy(false); Assign(x, 0, std::string::npos); }
    ~SndPath() { ((std::string*)this)->~basic_string(); }
    void Tidy(bool built);                                               // 0x402700
    SndPath& Assign(const std::string& x, unsigned int pos, unsigned int n); // 0x4024f0
    SndPath& AppendOut(const char* s, unsigned int n);                   // 0x403ba0
    bool Grow(unsigned int n, bool trim);                                // 0x402980
    void Eos(unsigned int n);                                            // 0x402960
    void AppendA(const char* s)                                          // append(s): _Grow and _Eos as calls
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = m + len, false)) {
            memcpy(ptr + len, s, m);
            Eos(n);
        }
    }
    void AssignB(const char* s)                                          // assign(s): _Grow as a call, _Eos inline
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    void AppendB(const char* s, unsigned int m)                          // append(s, n): _Grow as a call, _Eos inline
    {
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = m + len, false)) {
            memcpy(ptr + len, s, m);
            ptr[len = n] = 0;
        }
    }
    void AppendS(const SfxRec* x)                                        // append(*(string*)x, 0, npos)
    {
        unsigned int n = std::string::npos;
        unsigned int k = x->len;
        if (k < n) n = k;
        if (std::string::npos - len <= n) {
            std::_Xlen();
        }
        unsigned int e;
        if (0 < n && Grow(e = n + len, false)) {
            memcpy(len + ptr, x->ptr == 0 ? "" : x->ptr, n);
            ptr[len = e] = 0;
        }
    }
    const char* c_str() const { return ptr == 0 ? "" : ptr; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

// NONMATCHING: RALLYTROPHY 0x416260
// Tried 2026-10-31: found pos.y += r->height, append(*(string*)r,0,npos), per-iteration Vector3 (EH state 5), float pos/ints declared at top
// (229->162; 142 if num is char[4], which the original frame implies but we keep char[8] in its own blocks); still differs:
// `this` in ebx / zero in ebp (original swapped), NpfRec loop strength-reduced (original steps a plain pointer), and the
// path dtor exit ordering. Volatile zero/self, pointer loops, i++/src++ placement, decl orders did not flip ebx/ebp.
// num[4] rejected (2026-11-01): _itoa of the stage int overflows 4 bytes from 1000 up, so the buffer stays 8.
// Tried 2026-11-01 re-pass (num[8] kept): 23 more variants (volatile zero/self, decl orders, hoisted num/pos/count/recs/i, recs++ and src++ loops, float pos[3] = 1821 bytes but 184 insns) stay at 162 insns; ebx/ebp roles unchanged.
void StageSoundLoader::LoadSoundFiles()
{
    int idx, count2, ver, magic;
    AT(NpfRec*, 0x24c8) = 0;
    AT(int, 0x24cc) = 0;
    SndPath path(AT(std::string, 8));
    path.AppendOut("data/stage", strlen("data/stage"));
    {
        char num[8];
        _itoa(AT(int, 4), num, 10);
        path.AppendOut(num, strlen(num));
    }
    path.AppendA(".npf");
    if (File::Exists(path.c_str())) {
        File f;
        f.Open(path.c_str(), 0x81);
        f.Read(&magic, 4);
        f.Read(&ver, 4);
        if (magic == 0x1234 && ver == 100) {
            int count;
            f.Read(&count, 4);
            AT(int, 0x24cc) = count;
            NpfRec* recs = new NpfRec[count];
            AT(NpfRec*, 0x24c8) = recs;
            for (int i = 0; i < count; i++) {
                for (int j = 0; j < 4; j++) {
                    int tmp;
                    f.Read(&tmp, 4);
                    recs[i].id[j] = (char)tmp;
                    if (tmp != -1)
                        DAT_0056b0b0->LoadSpeech(tmp);
                }
                f.Read((char*)&recs[i] + 0x14, 0xc);
                f.Read((char*)&recs[i] + 0x20, 0xc);
            }
        }
        f.Close();
    }
    path.Assign(AT(std::string, 8), 0, std::string::npos);
    path.AppendA("data/stage");
    {
        char num[8];
        _itoa(AT(int, 4), num, 10);
        path.AppendA(num);
    }
    path.AppendA(".spf");
    if (!File::Exists(path.c_str()))
        return;
    File f2;
    f2.Open(path.c_str(), 0x81);
    f2.Read(&magic, 4);
    f2.Read(&ver, 4);
    if (magic == 0x3100 && ver == 100) {
        f2.Read(&count2, 4);
        AT(int, 0x24d0) = count2;
        SoundSource* src = new SoundSource[count2];
        AT(SoundSource*, 0x24d4) = src;
        for (int i = 0; i < AT(int, 0x24d0); i++, src++) {
            Vector3 pos;
            f2.Read(&pos, 0xc);
            f2.Read(&idx, 4);
            SfxRec* r = (SfxRec*)(DAT_0056b0c8 + idx * 0x20 + 0x139490);
            path.AssignB("data/soundfx/");
            path.AppendS(r);
            src->is3D = true;
            src->loop = true;
            src->SetSample(DAT_0056b0b0->LoadSample(path.c_str()));
            src->SetVolume(r->volume * DAT_0050143c / 100);
            src->SetRange(r->nearDist, r->farDist);
            pos.y = pos.y + r->height;
            src->SetPosition(&pos);
            DAT_0056b0b0->AddSource(src);
        }
    }
    f2.Close();
}
