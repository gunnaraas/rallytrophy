/* Race scene setup (0x4137c0): flares/sky from the flare ini, weather, skid marks, car environment sprites.
 * Include after unk_481390.h, rallytrophy.h, unk_47c460.h, unk_4758b0.h, unk_474c50.h, unk_472c90.h and
 * unk_465e90.h. Layout is raw (offsets in the source); the owning class is unknown. */
#ifndef UNK_4137C0_H
#define UNK_4137C0_H

// PathString with the out-of-line assignment from a temporary string (std::string::assign, 0x4024f0).
class FlareString {
public:
    FlareString(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~FlareString() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)              // std::string::_Tidy, expanded inline
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
    void Append(const char* s, unsigned int n); // 0x403ba0
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    void Add(const char* s)            // std::string::append(const char*), expanded inline
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n)
            Xlen();
        if (n != 0) {
            unsigned int newLen = len + n;
            if (Grow(newLen, false)) {
                memcpy(ptr + len, s, n);
                len = newLen;
                ptr[len] = 0;
            }
        }
    }
    static void Xlen();                // 0x4bd00b
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
    void AssignTmp(const TmpString& s, unsigned int pos, unsigned int n); // 0x4024f0
};
TmpString FlarePath(const char* prefix, const FlareString& name); // 0x408fd0: operator+(const char*, string)

TmpString MissingText2(const FlareString& prefix, const char* name); // 0x409130

class SceneSetup {
public:
    void FUN_004137c0(const FlareString& dir); // 0x4137c0 (thiscall, ret 4)
};
#endif
