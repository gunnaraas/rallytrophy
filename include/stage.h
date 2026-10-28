// Full layout of Stage (0x2534 bytes; ctor 0x40c9b0, dtor 0x40ebc0). The destructible members and their order come
// from the unwind maps of both functions (states 0-41 = members in declaration order); the member types are chosen so
// that VC6 generates the same destruction code as the original: see src/unk_40ebc0.cpp. Include this header first in
// the Stage TUs; it hides the partial declaration in unk_481390.h.
#ifndef STAGE_H
#define STAGE_H
#define STAGE_FULL_LAYOUT
#include <memory>
#include <string>
#include <vector>
#include "unk_481390.h"
#include "rallytrophy.h"
#include "unk_40c9b0.h"
#include "unk_40c280.h"
#include "unk_40c830.h"

// Owned objects (only their destructors matter here).
class SVirtual { public: virtual ~SVirtual(); };   // slot 0 = scalar deleting dtor
class SBodyAttach { public: ~SBodyAttach(); };     // 0x41b990
class SOwner40A7B0 { public: ~SOwner40A7B0(); };   // 0x40a7b0
class SOwnedSet { public: ~SOwnedSet(); };         // 0x401c30
class SSky { public: ~SSky(); };                   // 0x475a10
class SAiPath { public: ~SAiPath(); };             // 0x49f9e0
struct StSoundSrc { Sample* sample; BYTE b[0x6c]; ~StSoundSrc(); };            // 0x70 bytes, dtor 0x49b6e0
struct StSound88 { BYTE b[0x70]; Sample* sample; BYTE c[0x14]; ~StSound88(); }; // 0x88 bytes, dtor 0x40f380
struct StSign2c { BYTE b[0x2c]; ~StSign2c(); };                                 // 0x2c bytes, dtor 0x40b480

// Empty-destructor value members (unwind through 0x402c90; arrays through the EH vector iterators with 0x490ad0).
struct StVec3 { float x, y, z; ~StVec3() {} };
struct StVec2 { StVec2() {} ~StVec2() {} float x, y; };
struct StE10 { StE10() {} ~StE10() {} BYTE b[0x10]; };
struct StE48 { StE48() {} ~StE48() {} BYTE b[0x48]; };
struct StStart { StStart() {} ~StStart() {} float position[3]; };
// Records with a virtual destructor: the inlined vector destructor calls their scalar deleting dtor (0x4147a0) directly.
struct StRecord24 { void Destroy(int flag); BYTE b[0x24]; };
struct StRecord48 { void Destroy(int flag); BYTE b[0x48]; };
// Sound sample holder (0x2c bytes; ctor 0x40c6f0, dtor 0x40c700 = SampleHolder's).
struct StSample { StSample() : sample(0) {} ~StSample(); void* sample; BYTE pad[0x28]; };

// std::string layout whose destructor is the out-of-line _Tidy(true) (0x402700).
struct StString {
    StString(const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { Tidy(false); }
    ~StString() { Tidy(true); }
    void Tidy(bool built);                       // 0x402700, out of line in both directions
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

// std::vector<T> layout with the VC6 constructor (allocator temporary copied by value).
template <class T> struct StVecBase {
    StVecBase(const std::allocator<T>& a = std::allocator<T>()) : alloc(a) { first = 0; last = 0; end = 0; }
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    std::allocator<T> alloc;
    T* first;
    T* last;
    T* end;
};
// ~vector stays a call (0x401bb0).
template <class T> struct StPtrVecOut : StVecBase<T*> { ~StPtrVecOut(); };
// ~vector inlined with _Destroy as a call (0x403130).
template <class T> struct StVecD : StVecBase<T> {
    void Destroy(T* f, T* l);
    ~StVecD() { Destroy(first, last); alloc.deallocate(first, end - first); first = 0; last = 0; end = 0; }
};
// ~vector inlined, elements destroyed through their scalar deleting dtor with flag 0.
template <class T> struct StRecVec : StVecBase<T> {
    static void Destroy(T* f, T* l)
    {
        for (; f != l; ++f)
            f->Destroy(0);
    }
    ~StRecVec() { Destroy(first, last); operator delete(first); first = 0; last = 0; end = 0; }
};

// The game's intrusive list (0x14 bytes). A node with a user destructor is deleted through its scalar deleting
// destructor (0x49ada0, list at 0x98); plain nodes go to operator delete (lists at 0xac, 0xc0).
struct StNode {
    ~StNode() {}
    static void Free(StNode* n) { delete n; }
    StNode* next; StNode* prev; void* data;
};
struct StNodeD {
    void Delete(int flag);                       // the node's scalar deleting destructor (0x49ada0)
    static void Free(StNodeD* n) { if (n) n->Delete(1); }
    StNodeD* next; StNodeD* prev; void* data;
};
template <class N> struct StList {
    StList() { head = tail = freeList = 0; count = 0; freeCount = 0; }
    ~StList()
    {
        while (head)
            RemoveNode(head);
        N* n = freeList;
        while (n) {
            N* next = n->next;
            N::Free(n);
            n = next;
        }
        freeList = 0;
    }
    void RemoveNode(N* n);        // 0x414490: unlinks and recycles the node
    void RemoveNodeDelete(N* n);  // 0x4144e0: deletes the node's data, then unlinks
    N* head;
    N* tail;
    N* freeList;
    int count;
    int freeCount;
};

class Stage {
public:
    Checkpoint* FUN_00414fb0(int index);
    Stage(int a, int b, int c, int d);  // 0x40c9b0
    ~Stage();                          // 0x40ebc0
    void FUN_00411320(int a);
    void FUN_00411260();
    void FUN_004112a0(class RaceMode* race);
    void FUN_00416ec0(void* data);     // 0x416ec0
    void FUN_00416ef0(int mode);       // 0x416ef0
    void FUN_00417540(class Player* player, int time, int dt); // 0x417540
    Checkpoint* FUN_00414dc0(Vec3* position, int flag); // nearest checkpoint
    void FUN_004112e0(int dt);
    void FUN_004160c0(int dt);         // 0x4160c0: advance the stage by dt ms
    void FUN_00416010();               // 0x416010: begin the race loop
    void FUN_00416050();               // 0x416050: end the race loop
    void FUN_00411170(class ViewManager* view, class DrawContext* context); // 0x411170
    void FUN_00411350(class RaceDeviceSlots* device, class ViewManager* view); // 0x411350
    void FUN_00411460(class RaceDeviceSlots* device, class ViewManager* view, class DrawContext* context, float dt); // 0x411460
    void FUN_00411490(class RaceDeviceSlots* device, class ViewManager* view, class DrawContext* context, float dt); // 0x411490

    int unk0;                               // 0x000: track number
    int stageCount;                         // 0x004: stage number
    StString name;                          // 0x008: track directory (std::string layout)
    SVirtual* obj18;                        // 0x018
    StVec3 vec1c;                           // 0x01c
    StPtrVecOut<SVirtual> objs28;           // 0x028
    StPtrVecOut<SVirtual> objs38;           // 0x038
    StPtrVecOut<void> ptrs48;               // 0x048
    StVecD<int> ints58;                     // 0x058
    StVecD<int> ints68;                     // 0x068
    StVecD<int> ints78;                     // 0x078
    StVecD<SOwner40A7B0*> owners88;         // 0x088
    StList<StNodeD> list98;                 // 0x098
    StList<StNode> listac;                  // 0x0ac
    StList<StNode> listc0;                  // 0x0c0
    StTexCache texCache;                    // 0x0d4
    StModelCache modelCache;                // 0x0f4
    float unk130;                           // 0x130
    std::vector<SVirtual*> objs134;         // 0x134
    std::vector<SVirtual*> objs144;         // 0x144
    std::vector<SBodyAttach*> attach154;    // 0x154
    StVec3 vec164;                          // 0x164
    StVec3 vec170;                          // 0x170
    StVec3 vec17c;                          // 0x17c
    StVec3 vec188;                          // 0x188
    int unk194;                             // 0x194
    int unk198;                             // 0x198
    StageLoader* cells19c;                  // 0x19c: grid cells (0xb0 bytes each)
    StageScene scene;                       // 0x1a0 (0x1a0 bytes; its destructor is inlined here)
    BYTE pad340[0x38c - 0x340];
    StVec2 vec2s[20];                       // 0x38c
    BYTE pad42c[0x65c - 0x42c];
    StE10 e10[4];                           // 0x65c
    int unk69c[4];                          // 0x69c
    SSky* sky;                              // 0x6ac
    SVirtual* obj6b0;                       // 0x6b0
    Sprite sprite6b4;                       // 0x6b4
    Sprite sprite788;                       // 0x788
    BYTE pad85c[0x908 - 0x85c];
    Sprite sprites908[1];                   // 0x908
    Sprite* sprite9dc;                      // 0x9dc
    Sprite* sprite9e0;                      // 0x9e0
    int unk9e4;                             // 0x9e4
    int unk9e8;                             // 0x9e8
    int unk9ec;                             // 0x9ec
    StRecVec<StRecord48> recs9f0;           // 0x9f0
    int unka00;                             // 0xa00
    int unka04;                             // 0xa04
    StE48* arra08;                          // 0xa08
    StE48 e48[32];                          // 0xa0c
    int unk130c;                            // 0x130c: splits per section
    StStart starts[7];                      // 0x1310
    int unk1364;                            // 0x1364: loading label
    SAiPath* aiPath;                        // 0x1368
    int unk136c;                            // 0x136c
    int unk1370;                            // 0x1370
    int unk1374;                            // 0x1374
    StVec3 vec1378;                         // 0x1378
    StVec3 vec1384;                         // 0x1384
    StVec3 vec1390;                         // 0x1390
    int unk139c;                            // 0x139c
    int unk13a0;                            // 0x13a0
    SOwnedSet* owned13a4;                   // 0x13a4
    StRecVec<StRecord24> recs13a8;          // 0x13a8
    StPtrVecOut<void> ptrs13b8;             // 0x13b8
    BYTE pad13c8[0x15f8 - 0x13c8];
    StSoundSrc* sound15f8;                  // 0x15f8
    StSound88* sounds15fc;                  // 0x15fc
    int soundCount1600;                     // 0x1600
    int unk1604;                            // 0x1604
    StSample samples1608[16];               // 0x1608
    StSample samples18c8[16];               // 0x18c8
    StSample samples1b88[16];               // 0x1b88
    StSample samples1e48[16];               // 0x1e48
    StSample samples2108[16];               // 0x2108
    std::string names23c8[16];              // 0x23c8
    StSign2c* signs24c8;                    // 0x24c8
    int unk24cc;                            // 0x24cc
    int unk24d0;                            // 0x24d0
    StSoundSrc* sounds24d4;                 // 0x24d4
    StVec3 vec24d8;                         // 0x24d8: speech thresholds
    int unk24e4[10];                        // 0x24e4
    int unk250c[10];                        // 0x250c
};
#endif
