// Stage sound-point loader (0x416260): data/stage<N>.npf (0x2c-byte records at +0x24c8, count +0x24cc)
// and data/stage<N>.spf (SoundSource array at +0x24d4, count +0x24d0).
#include <string>
#include <new>
#include <stdlib.h>
#include "rallytrophy.h"
#include "unk_416260.h"

#define AT(T, off) (*(T*)((char*)this + (off)))

struct NpfRec {            // 0x2c bytes: 4 speech ids, two Vector3 at 0x14 and 0x20
    char id[4];
    char pad[0x28];
    NpfRec() {}
};
struct SfxRec {            // sound table entry (stride 0x20) in the Game object at +0x139490
    BYTE alloc;
    const char* ptr;       // +4
    unsigned int len;      // +8
    unsigned int res;      // +0xc
    int pad10;
    int volume;            // +0x14
    float nearDist;        // +0x18
    float farDist;         // +0x1c
};
extern int DAT_0050143c;
extern char* DAT_0056b0c8;

#define AB(T, off) (*(T*)(base + (off)))

static __forceinline void LoadNpf(char* base, const std::string& path, void* mem)
{
    if (File::Exists(path.c_str())) {
        File& f = *new (mem) File;
        int magic, ver, count, tmp;
        f.Open(path.c_str(), 0x81);
        f.Read(&magic, 4);
        f.Read(&ver, 4);
        if (magic == 0x1234 && ver == 100) {
            f.Read(&count, 4);
            AB(int, 0x24cc) = count;
            NpfRec* recs = new NpfRec[count];
            AB(NpfRec*, 0x24c8) = recs;
            for (int i = 0; i < count; i++) {
                for (int j = 0; j < 4; j++) {
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
        f.~File();
    }
}

static __forceinline void LoadSpf(char* base, std::string& spf, void* mem)
{
    if (File::Exists(spf.c_str())) {
        File& f = *new (mem) File;
        int magic, ver, count, idx;
        Vector3 pos;
        f.Open(spf.c_str(), 0x81);
        f.Read(&magic, 4);
        f.Read(&ver, 4);
        if (magic == 0x3100 && ver == 100) {
            f.Read(&count, 4);
            AB(int, 0x24d0) = count;
            SoundSource* src = new SoundSource[count];
            AB(SoundSource*, 0x24d4) = src;
            for (int i = 0; i < AB(int, 0x24d0); i++) {
                f.Read(&pos, 0xc);
                f.Read(&idx, 4);
                SfxRec* r = (SfxRec*)(DAT_0056b0c8 + idx * 0x20 + 0x139490);
                spf.assign("data/soundfx/");
                spf.append(r->ptr, r->len < spf.max_size() ? r->len : spf.max_size());
                src->is3D = true;
                src->loop = true;
                src->SetSample(DAT_0056b0b0->LoadSample(spf.c_str()));
                src->SetVolume(r->volume * DAT_0050143c / 100);
                src->SetRange(r->nearDist, r->farDist);
                src->SetPosition(&pos);
                DAT_0056b0b0->AddSource(src);
                src++;
            }
        }
        f.Close();
        f.~File();
    }
}

// NONMATCHING: RALLYTROPHY 0x416260
// 1603 vs 1822 bytes: original inlines the string appends (_Grow/copy loops) and builds each path with default-ctor + assign; frame 0x8158 vs 0x812c.
void StageSoundLoader::LoadSoundFiles()
{
    AT(NpfRec*, 0x24c8) = 0;
    AT(int, 0x24cc) = 0;

    char num[32];
    char mem1[0x4078], mem2[0x4078];
    std::string path(AT(std::string, 8));
    path += "data/stage";
    _itoa(AT(int, 4), num, 10);
    path += num;
    path += ".npf";
    LoadNpf((char*)this, path, mem1);

    std::string spf(AT(std::string, 8));
    spf += "data/stage";
    _itoa(AT(int, 4), num, 10);
    spf += num;
    spf += ".spf";
    LoadSpf((char*)this, spf, mem2);
}
