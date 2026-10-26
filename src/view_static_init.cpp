// Address-named entry for DrawQueueX::DrawQueueX (src/unk_4bc680.cpp).
extern "C" void __cdecl fun_004bc970(void);
extern "C" void __cdecl fun_004c0d01(void);
extern "C" void __cdecl fun_004b85c0(void);

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b7a10
// original: FUN_004b7a10
// Evidence: ev_ea2e1dc8aed1e15089456eb645c2459ff3fddc687b034b748ccd47e19d91fa29
void __stdcall FUN_004b7a10(void)
{
    __asm {
        mov ecx, 057bda0h
        jmp fun_004bc970
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b7a20
// original: FUN_004b7a20
// Evidence: ev_d5cf5e337c7c8a5ecff88b2388e9a4388a6818b5dd6a809de71d71ba150ea8a3
void __stdcall FUN_004b7a20(void)
{
    __asm {
        push 04b7a30h
        call fun_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b7a30
// original: FUN_004b7a30
// Instruction evidence: ev_b8c93741b5c1abe41fc2a07e22fd84a6c1789651e5150ee789fefa105aa3bf62
// and ev_10a86d08564873e47d56fa7ede088e0b4df10ced3b66ac61e3b8769e80b856db
void __stdcall FUN_004b7a30(void)
{
    __asm {
        mov ecx, 057bda0h
        jmp fun_004b85c0
    }
}
