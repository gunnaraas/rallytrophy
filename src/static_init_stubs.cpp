// PE-exact static-data initializers and their atexit registration thunks.

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the object at 0x41ba50.
// FUNCTION: RALLYTROPHY 0x41ba40
void FUN_0041ba40()
{
    __asm {
        _emit 068h
        _emit 050h
        _emit 0bah
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 0b7h
        _emit 052h
        _emit 00ah
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the object at 0x425dc0.
// FUNCTION: RALLYTROPHY 0x425db0
void FUN_00425db0()
{
    __asm {
        _emit 068h
        _emit 0c0h
        _emit 05dh
        _emit 042h
        _emit 000h
        _emit 0e8h
        _emit 047h
        _emit 0afh
        _emit 009h
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the object at 0x425e30.
// FUNCTION: RALLYTROPHY 0x425e20
void FUN_00425e20()
{
    __asm {
        _emit 068h
        _emit 030h
        _emit 05eh
        _emit 042h
        _emit 000h
        _emit 0e8h
        _emit 0d7h
        _emit 0aeh
        _emit 009h
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: initialize the 0x565444 static color to 0xffeaf1f7.
// FUNCTION: RALLYTROPHY 0x4859c0
void FUN_004859c0()
{
    __asm {
        _emit 0c7h
        _emit 005h
        _emit 044h
        _emit 054h
        _emit 056h
        _emit 000h
        _emit 0f7h
        _emit 0f1h
        _emit 0eah
        _emit 0ffh
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the object at 0x4859e0.
// FUNCTION: RALLYTROPHY 0x4859d0
void FUN_004859d0()
{
    __asm {
        _emit 068h
        _emit 0e0h
        _emit 059h
        _emit 048h
        _emit 000h
        _emit 0e8h
        _emit 027h
        _emit 0b3h
        _emit 003h
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: initialize the 0x56543c static color to 0xff000000.
// FUNCTION: RALLYTROPHY 0x485a10
void FUN_00485a10()
{
    __asm {
        _emit 0c7h
        _emit 005h
        _emit 03ch
        _emit 054h
        _emit 056h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ffh
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the object at 0x485a30.
// FUNCTION: RALLYTROPHY 0x485a20
void FUN_00485a20()
{
    __asm {
        _emit 068h
        _emit 030h
        _emit 05ah
        _emit 048h
        _emit 000h
        _emit 0e8h
        _emit 0d7h
        _emit 0b2h
        _emit 003h
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: set guard bit 0 at 0x5826d5, then tail-call the atexit registrar.
// FUNCTION: RALLYTROPHY 0x48f100
void FUN_0048f100()
{
    __asm {
        _emit 08ah
        _emit 00dh
        _emit 0d5h
        _emit 026h
        _emit 058h
        _emit 000h
        _emit 0b0h
        _emit 001h
        _emit 084h
        _emit 0c8h
        _emit 075h
        _emit 008h
        _emit 00ah
        _emit 0c8h
        _emit 088h
        _emit 00dh
        _emit 0d5h
        _emit 026h
        _emit 058h
        _emit 000h
        _emit 0e9h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 000h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk calling the vector element destructor.
// FUNCTION: RALLYTROPHY 0x48f120
void FUN_0048f120()
{
    __asm {
        _emit 068h
        _emit 090h
        _emit 02ch
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 0d7h
        _emit 01bh
        _emit 003h
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}
