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
struct StMsg {
    StMsg(const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { ptr = 0; len = 0; res = 0; }
    ~StMsg();
    void Freeze();
    char* begin() { Freeze(); return ptr; }
    char* end() { Freeze(); return ptr == 0 ? 0 : ptr + len; }
    bool Grow(unsigned int n, bool trim);
    void Tidy(bool built);
    StMsg& AssignCall(const char* text, unsigned int n);
    unsigned int MaxSize() const;
    void Copy(unsigned int n);
    StMsg& Format(char* first, char* last, const char* fmt, const char* arg);
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
