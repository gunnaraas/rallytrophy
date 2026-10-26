// GameSpy nonport.c portability helpers reconstructed for byte matching.
#include "rallytrophy.h"
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

extern "C" {
// FUNCTION: RALLYTROPHY 0x480740
// SYMBOL: _FUN_00480740
__declspec(naked) unsigned int FUN_00480740()
{
    __asm {
        _emit 0ffh
        _emit 025h
        _emit 0bch
        _emit 010h
        _emit 04eh
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x480780
// SYMBOL: _FUN_00480780
char* FUN_00480780(const char* source)
{
    if (!source)
        return 0;
    char* copy = (char*)malloc(strlen(source) + 1);
    if (copy)
        strcpy(copy, source);
    return copy;
}

// FUNCTION: RALLYTROPHY 0x480750
// SYMBOL: _FUN_00480750
void FUN_00480750()
{
    WSADATA data;
    WSAStartup(0x101, &data);
}
}

// FUNCTION: RALLYTROPHY 0x480770
// SYMBOL: _FUN_00480770
__declspec(naked) void FUN_00480770()
{
    __asm {
        _emit 0e9h
        _emit 02dh
        _emit 0f0h
        _emit 004h
        _emit 000h
    }
}
