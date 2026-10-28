#ifndef UNK_4B7A40_H
#define UNK_4B7A40_H
// DrawContext (0x194 bytes, vtable 0x4e29ec): per-frame scene draw lists. See analysis/range-4b7a40.md.
#include <string>
#include <vector>
#include <string.h>
#include <stddef.h>
#include "scene_object.h"

// std::string (VC6 layout) whose _Grow stays out of line (0x402980).
struct DrawNameString {
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    int alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

// Growable pointer array with a destructor (EH-unwound member); layout of Array<T> in rallytrophy.h.
template <class T>
class DrawArray {
public:
    DrawArray(int capacity, int grow)
    {
        m_count = 0;
        m_capacity = capacity;
        m_grow = grow;
        m_data = (T*)operator new(capacity * sizeof(T));
        m_end = m_data;
    }
    ~DrawArray() { operator delete(m_data); }
    void Add(const T& item)
    {
        if (m_count == m_capacity) {
            T* data = (T*)operator new((m_capacity + m_grow) * sizeof(T));
            memcpy(data, m_data, m_capacity * sizeof(T));
            m_capacity += m_grow;
            operator delete(m_data);
            m_data = data;
            m_end = m_data + m_count;
        }
        *m_end = item;
        m_count++;
        m_end++;
    }
    T* m_data;      // 0x0
    T* m_end;       // 0x4
    int m_count;    // 0x8
    int m_capacity; // 0xc
    int m_grow;     // 0x10
};

// Renderer vtable as 0x4b8160 uses it (slot N at +4*N); unnamed slots are padding.
class DrawDevice {
public:
    virtual ~DrawDevice();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0c();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1c();
    virtual void Slot20();
    virtual void Slot24();
    virtual void Slot28();
    virtual void Slot2c();
    virtual void Slot30();
    virtual void Slot34();
    virtual void Slot38();
    virtual void Slot3c();
    virtual void Slot40();
    virtual void Slot44();
    virtual void Slot48();
    virtual void Slot4c();
    virtual void Slot50();
    virtual void Slot54();
    virtual void Slot58();
    virtual void Slot5c();
    virtual void Slot60();
    virtual void Slot64();
    virtual void Slot68();
    virtual void Slot6c();
    virtual void Slot70();
    virtual void Slot74();
    virtual void Slot78();
    virtual void Slot7c();
    virtual void Slot80();
    virtual void Slot84(int state, int value);
    virtual int Slot88(int what);
    virtual void Slot8c();
    virtual void Slot90(int a, void* b, int c, int d, int e);
    virtual void Slot94();
    virtual void Slot98();
    virtual void Slot9c(void* a, int b);
    virtual void Slota0(void* a);
    virtual void Slota4();
    virtual void Slota8();
    virtual void Slotac();
    virtual void Slotb0(int a);
};

// The view-frustum helper (global 0x57bc88) and the transparent-pass queue (global 0x57bda0).
class DrawFrustum {
public:
    void Construct();                                   // 0x4653f0
    void Init(DrawDevice* device);                      // 0x4ba620
    int TestSphere(SceneBounds* bounds, SceneTransform* world); // 0x4ba830
    int TestBoundsA(SceneBounds* bounds);               // 0x4ba9d0
    int TestBoundsB(SceneBounds* bounds);               // 0x4bab80
    int TestSized(SceneBounds* bounds, SceneTransform* world, float size); // 0x4bad30
};
class DrawQueue {
public:
    void Flush(DrawDevice* device, int flag);                                      // 0x4bcb50
    void Begin(DrawDevice* device);                     // 0x4bce80
};
// Global vector at 0x581db4: its size() and erase() are out of line (0x414460, 0x484380) in the inlined resize(0).
class DrawVec : public std::vector<void*> {
public:
    void Resize0()
    {
        if (Size() < 0u) {
        } else if (0u < Size()) {
            Erase(begin(), end());
        }
    }
    unsigned int Size() const;                          // 0x414460
    iterator Erase(iterator first, iterator last);      // 0x484380
};
class DrawFill {
public:
    void Fill(int n, int* value);                       // 0x4b83a0
};
extern DrawVec DAT_00581db4;
extern DrawFill DAT_00581dc4;
extern DrawFill DAT_00581dd4;
extern DrawFrustum DAT_0057bc88;
extern DrawQueue DAT_0057bda0;
extern int DAT_00502470;
extern int DAT_0057bdb0;

// A float with an empty destructor (gives the constructor one unwind state).
struct DrawScalar {
    ~DrawScalar() {}
    float value;
};

class DrawContext : public SceneNode {
public:
    DrawContext();                                      // 0x4b7a40
    virtual ~DrawContext();                             // 0x4b7bd0 (scalar deleting), body 0x4b7bf0
    virtual void Slot1(void* view);                     // 0x4b8310
    void CullViews(DrawDevice* device, int unused);     // 0x4b7c80 (ret 8; the callers pass a second pointer it never reads)
    void DrawViews(DrawDevice* device, int unused);                 // 0x4b8160
    void BeginQueue(DrawDevice* device, int unused);    // 0x4b82f0
    void StampVisible();                                // 0x4b8340
    DrawArray<SceneNode*> roots;                        // 0x124
    DrawArray<SceneNode*> solids;                       // 0x138: flagged-4 (translucent-bounds) nodes
    DrawArray<SceneNode*> normals;                      // 0x14c
    DrawArray<SceneNode*> specials;                     // 0x160: type-2 nodes
    // Three destructible members (EH states 5-7 in the constructor; 0x184 is plain); their roles are unknown.
    ScenePoint unk174;                                  // 0x174
    DrawScalar unk180;                                  // 0x180
    float unk184;                                       // 0x184
    ScenePoint unk188;                                  // 0x188
};
#endif
