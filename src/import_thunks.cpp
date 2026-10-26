// PE import-address thunks are mapped here; imported API behavior is external.

__declspec(naked)
// PE observation: jump through the DirectInput8Create import address at 0x4e100c.
// FUNCTION: RALLYTROPHY 0x4a4b06
void FUN_004a4b06()
{
    __asm {
        _emit 0ffh
        _emit 025h
        _emit 00ch
        _emit 010h
        _emit 04eh
        _emit 000h
    }
}
