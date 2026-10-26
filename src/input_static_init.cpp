#include <deque>

// Keep the shared VC6 deque constructor out of the startup initializer.
#pragma inline_depth(0)
std::deque<unsigned char> g_keyboardCharacterQueue;
extern "C" void __cdecl fun_004c0d01(void);

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b6d60
// original: FUN_004b6d60
// Assembly and pseudocode: ev_457d029188a35a8813c62d1211f07ca4d9ea86bca20b9024e6d2c4c70f0361ce
void __stdcall FUN_004b6d60(void)
{
    __asm {
        push 04b6d70h
        call fun_004c0d01
        pop ecx
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x4b6dd0
// SYMBOL: _$E2
// original: FUN_004b6dd0
// Evidence: ev_84a551c584a67e76f3b94a2967a1518713dceb1e203a79dd540b3f666184066e
typedef void Generated_4b6dd0();

// FUNCTION: RALLYTROPHY 0x4b6dc0
// SYMBOL: _$E5
// original: FUN_004b6dc0
// Instruction evidence: ev_23848954be2b36fbe2cf5d0dbf09307ed5a61ce59be50689f8f3bad5a12b9b81
// and ev_0fcd12520fbe22b9fb096df7eade2259493a4134a03eb09920f19decdd8aecd1
typedef void Generated_4b6dc0();

// FUNCTION: RALLYTROPHY 0x4b6df0
// SYMBOL: _$E4
// original: FUN_004b6df0
// Evidence: ev_c847cd2aa3c94f51b560f9358ee1a9280b29c7154ad8c165f173e28ea1319dd2
typedef void Generated_4b6df0();

// FUNCTION: RALLYTROPHY 0x4b6e00
// SYMBOL: _$E3
// original: FUN_004b6e00
// Instruction evidence: ev_4af00e43c8acfb090bcde32cb6a3ad5464d09f8452ecd35462e52450cb12335e
// and ev_55cbb4f10a232fff7b11b6bec8ddb367676f4a417a317686a1488fefe73541c5
typedef void Generated_4b6e00();
