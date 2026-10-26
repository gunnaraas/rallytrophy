// Helper types for Stage::~Stage (0x40ebc0). Layouts inferred from pseudocode.
#ifndef UNK_40EBC0_H
#define UNK_40EBC0_H
#include <windows.h>

class SVirtual { public: virtual ~SVirtual(); };   // slot 0 = scalar deleting dtor
class SBodyAttach { public: ~SBodyAttach(); };     // 0x41b990
class SOwner40A7B0 { public: ~SOwner40A7B0(); };   // 0x40a7b0
class SOwnedSet { public: ~SOwnedSet(); };         // 0x401c30
class SSky { public: ~SSky(); };                   // 0x475a10
class SAiPath { public: ~SAiPath(); };             // 0x49f9e0
class SSoundSrc { public: ~SSoundSrc(); };         // 0x49b6e0
class SPlane { public: SPlane() {} ~SPlane() {} }; // arrays go through EH vector dtor 0x4c1116
class SNode49ADA0 { public: ~SNode49ADA0(); };     // list node deleted via 0x49ada0

struct SPNode { SPNode* next; SPNode* prev; void* data; };
struct SPList {                                    // 0x14 bytes
    SPNode* head; SPNode* tail; SPNode* freeList; int count; int freeCount;
    void RemoveNodeA(SPNode* n);                   // 0x4144e0
    void RemoveNodeB(SPNode* n);                   // 0x414490
};
struct SPtrVec { int alloc; void** first; void** last; void** end; };  // 0x10 bytes

// In-place member arrays: destroyed with the EH vector dtor 0x4c1116 (size/count from the original call args).
struct SHold2C { BYTE b[0x2c]; ~SHold2C(); };      // dtor 0x40c700
struct SArr23C8 { SHold2C a[16]; };                // at 0x23c8, 0x2108, 0x1e48, 0x1b88, 0x18c8, 0x1608
struct SE12 { float f[3]; ~SE12() {} };            // empty dtor (0x402c90)
struct SArr1310 { SE12 a[7]; };
struct SE48 { BYTE b[0x48]; ~SE48() {} };
struct SArrA0C { SE48 a[32]; };
struct SD4 { BYTE b[0xd4]; ~SD4(); };              // dtor 0x4a86d0
struct SArr908 { SD4 a[1]; };
struct SE10 { BYTE b[0x10]; ~SE10() {} };
struct SArr65C { SE10 a[4]; };
struct SE8 { BYTE b[8]; ~SE8() {} };
struct SArr38C { SE8 a[20]; };
void __fastcall SStrTidy(void* s, bool built);     // 0x402700
void __fastcall SHolderDtor(void* h);              // 0x401bb0
void __fastcall SBase4A86D0(void* h);              // 0x4a86d0
void __fastcall SCarModelCache(void* p);           // 0x4ade80
void __fastcall SCarTexCache(void* p);             // 0x4aee20
void __fastcall SElemDtor(void* e, int flag);      // 0x4147a0
#endif
