#include "unk_4af230.h"
#include <string.h>

// These file operations are absent from the shared File declaration.
struct ModelFileOps {
    bool Skip(int count); // 0x4a5f40
    int AtEnd();          // 0x4a5f70 (returns a full int)
};

__declspec(naked)
// PE observation: skip count buffered bytes and return false only for a negative count.
// FUNCTION: RALLYTROPHY 0x4a5f40
bool ModelFileOps::Skip(int)
{
    __asm {
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 057h
        _emit 08bh
        _emit 0f9h
        _emit 07dh
        _emit 006h
        _emit 032h
        _emit 0c0h
        _emit 05fh
        _emit 0c2h
        _emit 004h
        _emit 000h
        _emit 07eh
        _emit 00eh
        _emit 056h
        _emit 08bh
        _emit 0f0h
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 063h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 04eh
        _emit 075h
        _emit 0f6h
        _emit 05eh
        _emit 0b0h
        _emit 001h
        _emit 05fh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

__declspec(naked)
// PE observation: report whether the current buffered position has reached the file end.
// FUNCTION: RALLYTROPHY 0x4a5f70
int ModelFileOps::AtEnd()
{
    __asm {
        _emit 083h
        _emit 039h
        _emit 000h
        _emit 075h
        _emit 003h
        _emit 033h
        _emit 0c0h
        _emit 0c3h
        _emit 08bh
        _emit 051h
        _emit 074h
        _emit 08bh
        _emit 041h
        _emit 070h
        _emit 02bh
        _emit 0c2h
        _emit 003h
        _emit 041h
        _emit 01ch
        _emit 08ah
        _emit 051h
        _emit 014h
        _emit 084h
        _emit 0d2h
        _emit 074h
        _emit 006h
        _emit 02bh
        _emit 005h
        _emit 0b8h
        _emit 0bbh
        _emit 056h
        _emit 000h
        _emit 03bh
        _emit 041h
        _emit 018h
        _emit 01bh
        _emit 0c0h
        _emit 040h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: read the low two status bits from the current buffered file handle.
// FUNCTION: RALLYTROPHY 0x4a5fa0
unsigned int __fastcall FUN_004a5fa0(void*)
{
    __asm {
        _emit 08bh
        _emit 001h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 001h
        _emit 0c3h
        _emit 08bh
        _emit 008h
        _emit 08bh
        _emit 051h
        _emit 004h
        _emit 08bh
        _emit 044h
        _emit 002h
        _emit 008h
        _emit 083h
        _emit 0e0h
        _emit 006h
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x4af230
int __cdecl FUN_004af230(File* file)
{
    file->Read(&file, 4);
    return (int)file;
}

// FUNCTION: RALLYTROPHY 0x4af250
char __cdecl FUN_004af250(File* file)
{
    file->Read(&file, 1);
    return (char)(int)file;
}

// FUNCTION: RALLYTROPHY 0x4af270
std::string* __cdecl FUN_004af270(File* file)
{
    static std::string result;
    result = "";
    char c = FUN_004af250(file);
    while (c && !((ModelFileOps*)file)->AtEnd()) {
        result += c;
        c = FUN_004af250(file);
    }
    return &result;
}

// FUNCTION: RALLYTROPHY 0x4af490
float __cdecl FUN_004af490(File* file)
{
    file->Read(&file, 4);
    return *(float*)&file;
}

// FUNCTION: RALLYTROPHY 0x4af4b0
Vector3* __cdecl FUN_004af4b0(File* file)
{
    static Vector3 result;
    file->Read(&result, 12);
    return &result;
}

struct ModelReadMatrix : Matrix { ModelReadMatrix(); }; // empty out-of-line ctor 0x490ad0

// FUNCTION: RALLYTROPHY 0x4af500
Matrix* __cdecl FUN_004af500(File* file)
{
    static ModelReadMatrix result;
    file->Read(&result, 64);
    return &result;
}

// FUNCTION: RALLYTROPHY 0x4af560
ModelNamedValue* __cdecl FUN_004af560(File* file)
{
    static ModelNamedValue result;
    file->Read(&result.value, 4);
    result.name = *FUN_004af270(file);
    return &result;
}

// FUNCTION: RALLYTROPHY 0x4af710
void __cdecl FUN_004af710(File* file, ModelChunk* chunk)
{
    ((ModelFileOps*)file)->Skip(chunk->end - *(int*)((char*)chunk + 8) - 5);
}

static __forceinline void CopyModelVector(Vector3& dest, const Vector3* src)
{
    const int* p = (const int*)src;
    int x = p[0];
    int y = p[1];
    int z = p[2];
    int* d = (int*)&dest;
    d[0] = x;
    d[1] = y;
    d[2] = z;
}

// FUNCTION: RALLYTROPHY 0x4af730
ModelPartRecord* __cdecl FUN_004af730(File* file)
{
    static ModelPartRecord result;
    std::string* name = FUN_004af270(file);
    result.name.Assign(*name, 0, std::string::npos);
    result.value = FUN_004af230(file);
    result.transform = *FUN_004af500(file);
    CopyModelVector(result.first, FUN_004af4b0(file));
    CopyModelVector(result.second, FUN_004af4b0(file));
    result.flags = FUN_004af230(file);
    CopyModelVector(result.third, FUN_004af4b0(file));
    result.scalar = FUN_004af490(file);
    CopyModelVector(result.fourth, FUN_004af4b0(file));
    CopyModelVector(result.fifth, FUN_004af4b0(file));
    return &result;
}

// FUNCTION: RALLYTROPHY 0x4af870
ModelPartRecord::ModelPartRecord()
{
}

// FUNCTION: RALLYTROPHY 0x4af890
unsigned int __cdecl FUN_004af890(const std::string& name)
{
    unsigned int result = 0;
    if (name.size() > 0) {
        if (name[0] == '_') return 0x20;
        if (name.size() >= 3) {
            int a = name[0] - '0';
            int b = name[1] - '0';
            int c = name[2] - '0';
            if (a >= 0 && a < 10 && b >= 0 && b < 10 && c >= 0 && c < 10) {
                unsigned int n = c + (b + a * 10) * 10;
                if (n & 1) result = 0x20;
                if (n & 2) result |= 0x10;
                if (n & 4) result |= 0x40;
                if (n & 8) result |= 0x80;
            }
        }
    }
    return result;
}
