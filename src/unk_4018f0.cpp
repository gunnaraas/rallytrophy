// Replay record list (0x4018f0-0x401b00), pointer holder and interpolated mesh set (0x401bb0-0x4021b0).
#include "unk_4018f0.h"
#include "unk_481390.h"
#include <math.h>
bool FUN_004a6050(const char* name); /* file exists */

// FUNCTION: RALLYTROPHY 0x4018f0
void RecList::Append(RecNode* n)
{
    if (tail == 0) {
        head = n;
        tail = n;
        n->next = 0;
        return;
    }
    tail->next = n;
    tail = n;
    n->next = 0;
}

// FUNCTION: RALLYTROPHY 0x401920
BOOL RecList::Save(const char* name)
{
    if (count == 0)
        return TRUE;
    RecFile f;
    if (!f.Open(name, 0x82))
        return FALSE;
    BOOL r = WriteTo(&f);
    f.Close();
    return r;
}

// FUNCTION: RALLYTROPHY 0x4019e0
BOOL RecList::WriteTo(RecFile* f)
{
    int magic = 0x42420008;
    f->Write(&magic, 4);
    f->Write(&count, 4);
    f->Write(&extra, 4);
    for (RecNode* n = head; n != 0; n = n->next)
        f->Write(n, 0x5c);
    return TRUE;
}

// FUNCTION: RALLYTROPHY 0x401a40
BOOL RecList::Load(const char* name)
{
    if (!FUN_004a6050(name))
        return FALSE;
    RecFile f;
    if (!f.Open(name, 0x81))
        return FALSE;
    BOOL r = ReadFrom(&f);
    f.Close();
    return r;
}

// FUNCTION: RALLYTROPHY 0x401b00
BOOL RecList::ReadFrom(RecFile* f)
{
    int magic, n, ex;
    f->Read(&magic, 4);
    f->Read(&n, 4);
    f->Read(&ex, 4);
    if (magic == 0x42420008 && n != 0) {
        Stop();
        Reset();
        count = n;
        extra = ex;
        for (; n != 0; n--) {
            RecNode* r = Alloc();
            f->Read(r, 0x5c);
            Append(r);
        }
        return TRUE;
    }
    return FALSE;
}

__declspec(naked)
// PE transcription preserves the original saved pointer slot before deletion.
// FUNCTION: RALLYTROPHY 0x401bb0
void PtrHolder::Release()
{
    __asm {
        _emit 051h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 046h
        _emit 004h
        _emit 050h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 008h
        _emit 0e8h
        _emit 03fh
        _emit 0b4h
        _emit 00bh
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 033h
        _emit 0c0h
        _emit 089h
        _emit 046h
        _emit 004h
        _emit 089h
        _emit 046h
        _emit 008h
        _emit 089h
        _emit 046h
        _emit 00ch
        _emit 05eh
        _emit 059h
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x401c10
MeshSet::MeshSet()
{
    owner = 0;
    elems = 0;
    count = 0;
    frames = 0;
    loop = 0;
}

// FUNCTION: RALLYTROPHY 0x401c30
MeshSet::~MeshSet()
{
    if (elems != 0) {
        for (int i = 0; i < count; i++) {
            if (elems[i].a != 0)
                delete[] elems[i].a;
            if (elems[i].b != 0)
                delete[] elems[i].b;
        }
        delete[] elems;
    }
    if (owner != 0)
        delete owner;
}

extern int DAT_00573bc0[];
extern float DAT_004e1348, DAT_004e134c;

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

// NONMATCHING: RALLYTROPHY 0x4021b0 (union mirrors the target reuse of the spline double slot high dword for an index; 836 vs 824 bytes, 564 differing bytes; remaining frame/register layout differs)
void MeshSet::Interp(float t)
{
    if (owner == 0)
        return;
    ((int*)owner)[5] = ((int*)&tint)[0];
    ((int*)owner)[6] = ((int*)&tint)[1];
    ((int*)owner)[7] = ((int*)&tint)[2];
    if (*(int*)&t < 0)
        t = 0.0f;
    else if (*(int*)&t > 0x3f800000)
        t = 1.0f;
    union IndexSplineSlot { double spline; int words[2]; } indexSpline;
    float f;
    int prev, nxt, nxt2;
    indexSpline.spline = (frames - 1) * t;
    f = (float)(indexSpline.spline - floor(indexSpline.spline));
    if (*(int*)&f < 0)
        f = 0.0f;
    else if (*(int*)&f > 0x3f800000)
        f = 1.0f;
    indexSpline.words[1] = RoundF((float)floor(indexSpline.spline));

    if (loop != 0) {
        indexSpline.spline = frames * t;
        indexSpline.words[1] = RoundF((float)floor(indexSpline.spline));
        if (indexSpline.words[1] == frames) {
            f = DAT_004e134c;
            indexSpline.words[1] = 0;
        } else {
            f = (float)(indexSpline.spline - floor(indexSpline.spline));
        }
        prev = indexSpline.words[1] - 1;
        nxt = indexSpline.words[1] + 1;
        nxt2 = indexSpline.words[1] + 2;
        if (prev < 0)
            prev = frames - 1;
        if (frames <= nxt)
            nxt -= frames;
        if (frames <= nxt2)
            nxt2 -= frames;
    } else {
        if (indexSpline.words[1] < 0)
            indexSpline.words[1] = 0;
        int last = frames - 1;
        if (last <= indexSpline.words[1]) {
            indexSpline.words[1] = frames - 2;
            f = DAT_004e1348;
        }
        if (indexSpline.words[1] < 1)
            prev = 0;
        else
            prev = indexSpline.words[1] - 1;
        nxt = indexSpline.words[1] + 1;
        nxt2 = nxt;
        if (nxt < last)
            nxt2 = indexSpline.words[1] + 2;
    }
    Vec3Slot sp;
    sp.x = f;
    sp.y = f * f;
    sp.z = f * f * f;
    MeshElem* e = elems;
    indexSpline.words[1] *= 12;
    prev *= 12;
    nxt *= 12;
    nxt2 *= 12;
    int oc = indexSpline.words[1], op = prev, on = nxt, on2 = nxt2;
    for (int k = count; k > 0; k--, e++) {
        float* d = e->dst;
        char* a = (char*)e->a;
        char* b = (char*)e->b;
        d[0] = sp.Spline(*(float*)(a + op), *(float*)(a + oc), *(float*)(a + on), *(float*)(a + on2));
        d[1] = sp.Spline(*(float*)(a + 4 + op), *(float*)(a + 4 + oc), *(float*)(a + 4 + on), *(float*)(a + 4 + on2));
        d[2] = sp.Spline(*(float*)(a + 8 + op), *(float*)(a + 8 + oc), *(float*)(a + 8 + on), *(float*)(a + 8 + on2));
        d[3] = sp.Spline(*(float*)(b + op), *(float*)(b + oc), *(float*)(b + on), *(float*)(b + on2));
        d[4] = sp.Spline(*(float*)(b + 4 + op), *(float*)(b + 4 + oc), *(float*)(b + 4 + on), *(float*)(b + 4 + on2));
        d[5] = sp.Spline(*(float*)(b + 8 + op), *(float*)(b + 8 + oc), *(float*)(b + 8 + on), *(float*)(b + 8 + on2));
        float q = d[5] * d[5] + d[4] * d[4] + d[3] * d[3];
        int bits = *(int*)&q;
        int r = DAT_00573bc0[(bits >> 12) & 0xfff] + 0x3f800000 + (-(int)((bits * 2 + 0x81000001U) & 0xfe000000) >> 2);
        r &= 0x7fffffff;
        float inv = *(float*)&r;
        d[3] = inv * d[3];
        d[4] = inv * d[4];
        d[5] = inv * d[5];
    }
}
