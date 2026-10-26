#pragma once
// S7 group 0x414330: small vector/sphere helpers.
struct S414Vec3 {
    float x, y, z;
    void Sub(float* out, const float* other) const; // 0x414330
};
struct S414Sphere {
    float x, y, z, r;
    int Contains(const float* p, float extra) const; // 0x414370
};
struct S414Node {
    S414Node* next;
    S414Node* prev;
    void* data;
};
struct S414List {
    S414Node* head;
    S414Node* tail;
    S414Node* freeList;
    int freeCount;
    int count;
    void RemoveNode(S414Node* n); // 0x4144e0
};
#include <string>
typedef std::string Str414;
Str414& __fastcall Func414550(Str414* self, int, char* first, char* last, const char* s1, const char* s2); // 0x414550
