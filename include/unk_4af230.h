#ifndef UNK_4AF230_H
#define UNK_4AF230_H
#include "rallytrophy.h"
#include <string>
struct ModelChunk {
    std::string name;
    unsigned int end;
};
struct ModelNamedValue {
    std::string name;
    unsigned int value;
};
struct ModelRecordString {
    ModelRecordString(const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { Tidy(false); }
    ~ModelRecordString() { ((std::string*)this)->~basic_string(); }
    void Tidy(bool built); // 0x402700
    void Assign(const std::string&, unsigned int, unsigned int); // 0x4024f0
    std::allocator<char> alloc;
    char* ptr;
    unsigned int size, capacity;
};
struct ModelPartRecord {
    ModelPartRecord();
    ModelRecordString name;
    int value;
    Matrix transform;
    Vector3 first, second;
    int flags;
    Vector3 third;
    float scalar;
    Vector3 fourth, fifth;
};
int __cdecl FUN_004af230(File* file);
char __cdecl FUN_004af250(File* file);
std::string* __cdecl FUN_004af270(File* file);
float __cdecl FUN_004af490(File* file);
Vector3* __cdecl FUN_004af4b0(File* file);
Matrix* __cdecl FUN_004af500(File* file);
ModelNamedValue* __cdecl FUN_004af560(File* file);
void __cdecl FUN_004af710(File* file, ModelChunk* chunk);
ModelPartRecord* __cdecl FUN_004af730(File* file);
unsigned int __cdecl FUN_004af890(const std::string& name);
#endif
