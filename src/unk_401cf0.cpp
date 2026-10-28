// Mesh-set loader (0x401cf0).
#include <string>
#include <stdexcept>
#include <string.h>
#include "unk_401cf0.h"

// Error-message string of MeshSetLoader::Load: constructed with _Tidy (0x402700) as a call and assign(s, n)
// expanded (_Grow 0x402980 as a call, _Eos inline); appends expanded the same way with _Xlen (0x4bd00b).
struct MsgStr {
    MsgStr(const char* s, const std::allocator<char>& a = std::allocator<char>()) : alloc(a)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    ~MsgStr();                                       // 0x430720
    void Tidy(bool built);                           // 0x402700
    bool Grow(unsigned int n, bool trim);            // 0x402980
    MsgStr& operator+=(const char* s)
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
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
struct MsgError : std::exception {                   // std::logic_error layout: exception("") + the string copy
    MsgError(const MsgStr& s) : std::exception(""), str(s) {}
    virtual ~MsgError();
    struct Copy {
        Copy(const MsgStr& x) : alloc(x.alloc) { ptr = 0; len = 0; res = 0; Assign(x, 0, std::string::npos); }
        ~Copy();
        Copy& Assign(const MsgStr& x, unsigned int pos, unsigned int n);   // 0x4024f0
        std::allocator<char> alloc;
        char* ptr;
        unsigned int len, res;
    } str;
};
// NONMATCHING: RALLYTROPHY 0x401cf0
// 1186 vs 1213 bytes; the call sequence matches (20 calls) and the message string is a twin shaped like the
// original (Tidy call, inline assign/append around _Grow). Left: register allocation (the original keeps `this`
// in a slot and re-reads `frames` for every allocation) and the frame layout (0x48 vs 0x40).
void MeshSetLoader::Load(MSModelFile* file)
{
    frames = file->parts.size();
    if (frames != 0) {
        MSPart* first = new MSPart(file->parts[0]);
        int* pv = (int*)first->pivot;
        int px = pv[0];
        int py = pv[1];
        ((int*)tint.f)[0] = px;
        ((int*)tint.f)[2] = pv[2];
        owner = (Owner*)first;
        ((int*)tint.f)[1] = py;
        MSSubList* sl = first->meshes.At(0);
        unsigned i;
        for (i = 0; i < sl->subs.size(); i++)
            count += sl->subs[i]->n;
        MeshElem* el = new MeshElem[count];
        elems = el;
        if (count > 0) {
            MeshElem* q = el;
            for (int n = count; n != 0; n--) {
                q->a = (MeshVec*)new MV[frames];
                q->b = (MeshVec*)new MV[frames];
                q++;
            }
        }
        MeshElem* e = el;
        for (i = 0; i < sl->subs.size(); i++) {
            MSVert* v = sl->subs[i]->verts;
            int m = sl->subs[i]->n;
            for (; m > 0; m--) {
                e->dst = (float*)v;
                e++;
                v++;
            }
        }
        int nf = frames;
        void** pp = file->parts.begin();
        for (int k = 0; k < nf; k++) {
            MSPart* p = (MSPart*)pp[k];
            const char* name = p->name;
            if (name == 0)
                name = "";
            if (strnicmp(name, "frame", 5) == 0 && strlen(name) >= 7) {
                int idx = name[5] * 10 + name[6] - 0x210;
                if (idx < 0 || idx >= nf) {
                    MsgStr msg("Invalid frame '");
                    msg += name;
                    msg += "'";
                    throw MsgError(msg);
                }
                MSSubList* s = p->meshes.At(0);
                MeshElem* d = el;
                for (i = 0; i < s->subs.size(); i++) {
                    float* v = (float*)s->subs[i]->verts;
                    int m = s->subs[i]->n;
                    if (m > 0)
                        do {
                            MeshVec* a = &d->a[idx];
                            a->f[0] = v[0];
                            a->f[1] = v[1];
                            a->f[2] = v[2];
                            MeshVec* b = &d->b[idx];
                            b->f[0] = v[3];
                            b->f[1] = v[4];
                            b->f[2] = v[5];
                            d++;
                            v += 11;
                        } while (--m != 0);
                }
            }
        }
    }
}
