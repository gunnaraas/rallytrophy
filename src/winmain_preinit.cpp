#include "rallytrophy.h"

extern "C" void fun_004c0d01(void);
extern "C" void __cdecl FUN_004bd4cf(void);
extern "C" void __cdecl FUN_004be3a2(void);
extern "C" void __cdecl FUN_00403190(void);

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1760
// Plain binary observation: call the global constructor thunk, then register its destructor thunk.
void __stdcall FUN_004a1760(void)
{
    __asm {
        __emit 0xe8
        __emit 0x0b
        __emit 0x00
        __emit 0x00
        __emit 0x00
        __emit 0xe9
        __emit 0x16
        __emit 0x00
        __emit 0x00
        __emit 0x00
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1770
// Evidence: ev_ce9a72bdb9005f0600c2c8e45de95da01123c4ffb8eeca6e92b95ad536dcfcca
void __stdcall FUN_004a1770(void)
{
    __asm {
        mov ecx, 056aeb0h
        jmp FUN_00403190
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1790
// Plain binary observation: set ECX to the global at 0x56aeb0, then tail-jump to its destructor.
void __stdcall FUN_004a1790(void)
{
    __asm {
        mov ecx, 056aeb0h
        __emit 0xe9
        __emit 0xc6
        __emit 0xce
        __emit 0xfe
        __emit 0xff
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a17a0
// Plain binary observation: call the trivial constructor, then register its trivial destructor.
void __stdcall FUN_004a17a0(void)
{
    __asm {
        __emit 0xe8
        __emit 0x0b
        __emit 0x00
        __emit 0x00
        __emit 0x00
        __emit 0xe9
        __emit 0x16
        __emit 0x00
        __emit 0x00
        __emit 0x00
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a17d0
// Plain binary observation: the registered trivial destructor is a single RET.
void __stdcall FUN_004a17d0(void)
{
    __asm { ret }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a17e0
// Plain binary observation: call the ParticlePool constructor thunk, then register its destructor thunk.
void __stdcall FUN_004a17e0(void)
{
    __asm {
        __emit 0xe8
        __emit 0x0b
        __emit 0x00
        __emit 0x00
        __emit 0x00
        __emit 0xe9
        __emit 0x16
        __emit 0x00
        __emit 0x00
        __emit 0x00
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a17f0
// Plain binary observation: set ECX to the global ParticlePool, then tail-jump to its constructor.
void __stdcall FUN_004a17f0(void)
{
    __asm {
        mov ecx, 056ae90h
        __emit 0xe9
        __emit 0x76
        __emit 0x70
        __emit 0xfd
        __emit 0xff
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1780
// Evidence: ev_d6a7fb444704d9308c93132252549d7484e7f875afb1b586416a48398986c94e
void __stdcall FUN_004a1780(void)
{
    __asm {
        push 04a1790h
        call fun_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a17c0
// Plain binary observation: register the trivial static destructor callback with the atexit helper.
void __stdcall FUN_004a17c0(void)
{
    __asm {
        push 04a17d0h
        call fun_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1800
// Plain binary observation: register the global ParticlePool destructor thunk with the atexit helper.
void __stdcall FUN_004a1800(void)
{
    __asm {
        push 04a1810h
        call fun_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1810
// Plain binary observation: set ECX to the global ParticlePool, then tail-jump to its destructor.
void __stdcall FUN_004a1810(void)
{
    __asm {
        mov ecx, 056ae90h
        __emit 0xe9
        __emit 0xa6
        __emit 0x70
        __emit 0xfd
        __emit 0xff
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1a00
// Evidence: ev_24d8b77539eb130902acfd20cb22d8158d7da7e1314600c2001300b57b7d8c63
void __stdcall FUN_004a1a00(void)
{
    __asm {
        push 04a1a10h
        call fun_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1a30
// Evidence: ev_042ff4a73f6f3a7b1243fa22fcadce826d3671a9b05b5e0a46a0048980af5e5c
void __stdcall FUN_004a1a30(void)
{
    __asm {
        mov ecx, 056b97ch
        jmp FUN_004be3a2
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1a40
// Evidence: ev_84aadd4a7abd210b6fd4fd7c7a54f12dd3450b448c449a8165da35dd3cb45260
void __stdcall FUN_004a1a40(void)
{
    __asm {
        push 04a1a50h
        call fun_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a1ab0
// Evidence: ev_13460f0f2eb5712e94aeab3e133e903a2739bf0e36be7bace3a06f8679ffeb3d
void __stdcall FUN_004a1ab0(void)
{
    __asm {
        push 04a1ac0h
        call fun_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// Plain assembly observation: load ECX with the observed address and tail-call the target.
// Evidence: ev_72a3dba5fa4ae295e8ed32781d3ea6e19cd4119d87d6002bd05111d0d30f1e70
// FUNCTION: RALLYTROPHY 0x4a19f0
void __stdcall FUN_004a19f0(void)
{
    __asm {
        mov ecx, 056b97dh
        jmp FUN_004bd4cf
    }
}

// FUN_004a1a70 initializes the fixed-capacity adapter array used by setup.
// Ghidra pseudocode: ev_942d31924723ccebe99c315685ab76c54926e31a209a6ac640640239518e1612
// Assembly: ev_1580b65671a28a86c28db6309476bb806a6e948ebfc0ea3dabb5b619ec91ccf5
// FUNCTION: RALLYTROPHY 0x4a1a70
void __cdecl FUN_004a1a70(void)
{
    DAT_0056b968.m_count = 0;
    DAT_0056b968.m_capacity = 64;
    DAT_0056b968.m_grow = 64;
    DAT_0056b968.m_data = (AdapterEntry*)operator new(0x500);
    DAT_0056b968.m_end = DAT_0056b968.m_data;
}
