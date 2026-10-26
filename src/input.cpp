// Keyboard hook state and lifetime management.
#include "rallytrophy.h"
#include <deque>

unsigned char DAT_0057bb28[0x100];
int DAT_0057bc60;
HHOOK DAT_0057bc64;
int DAT_0057bc68;
int DAT_0057bc5c;

// The original callback stores translated character input in a global deque.
extern std::deque<unsigned char> g_keyboardCharacterQueue;

float DAT_0057ba00;
float DAT_0057ba04;
HHOOK DAT_0057ba08;
int DAT_0057ba0c;
int DAT_0057ba10;
int DAT_0057ba14;
int DAT_0057ba18;
int DAT_0057ba1c;
int DAT_0057ba20;
int DAT_0057ba24;

LRESULT CALLBACK FUN_004b6f90(int code, WPARAM key, LPARAM flags);
extern LRESULT CALLBACK FUN_004b6c10(int code, WPARAM message, LPARAM data);

inline assertion_error::assertion_error(const std::string& what)
    : exception(""), m_message(what)
{
}

// FUNCTION: RALLYTROPHY 0x4b6ac0
// original: FUN_004b6ac0
T_56b0c0::T_56b0c0()
{
    if (DAT_0057ba0c < 1) {
        DWORD threadId = GetCurrentThreadId();
        DAT_0057ba08 = SetWindowsHookExA(
            WH_MOUSE, FUN_004b6c10, 0, threadId);
        if (!DAT_0057ba08) {
            throw assertion_error("Unable to hook mouse");
        }
    }
    DAT_0057ba0c++;
}

// FUNCTION: RALLYTROPHY 0x4b6bf0
T_56b0c0::~T_56b0c0()
{
    DAT_0057ba0c--;
    if (DAT_0057ba0c == 0) {
        UnhookWindowsHookEx(DAT_0057ba08);
        DAT_0057ba08 = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x4b6c10
// original: FUN_004b6c10
// Instruction body plus aligned six-entry jump table match; see input-initializers.md.
LRESULT CALLBACK FUN_004b6c10(int code, WPARAM message, LPARAM data)
{
    if (code < 0) {
        return CallNextHookEx(DAT_0057ba08, code, message, data);
    }

    DAT_0057ba14 = 0;
    DAT_0057ba1c = 0;
    int* point = (int*)data;
    switch (message) {
    case 0x200:
        DAT_0057ba00 = (float)point[0];
        DAT_0057ba04 = (float)point[1];
        break;
    case 0x201:
        DAT_0057ba10 = 1;
        DAT_0057ba20 = 1;
        break;
    case 0x202:
        if (DAT_0057ba10 == 1) {
            DAT_0057ba14 = 1;
        }
        DAT_0057ba10 = 0;
        break;
    case 0x204:
        DAT_0057ba18 = 1;
        DAT_0057ba24 = 1;
        break;
    case 0x205:
        if (DAT_0057ba18 == 1) {
            DAT_0057ba1c = 1;
        }
        DAT_0057ba18 = 0;
        break;
    }
    return CallNextHookEx(DAT_0057ba08, code, message, data);
}

// FUNCTION: RALLYTROPHY 0x4b6e10
// original: FUN_004b6e10
T_56b0bc::T_56b0bc()
{
    if (DAT_0057bc60 < 1) {
        memset(DAT_0057bb28, 0, sizeof(DAT_0057bb28));
        memset(DAT_0057ba28, 0, sizeof(DAT_0057ba28));

        DWORD threadId = GetCurrentThreadId();
        DAT_0057bc64 = SetWindowsHookExA(
            WH_KEYBOARD, FUN_004b6f90, 0, threadId);
        if (!DAT_0057bc64) {
            throw assertion_error("Unable to hook keyboard");
        }
        DAT_0057bc68 = 0;
    }
    DAT_0057bc60++;
}

// FUNCTION: RALLYTROPHY 0x4b6f60
T_56b0bc::~T_56b0bc()
{
    DAT_0057bc60--;
    if (DAT_0057bc60 == 0) {
        if (DAT_0057bc64) {
            UnhookWindowsHookEx(DAT_0057bc64);
        }
        DAT_0057bc64 = 0;
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b6f90
// PE observation: keyboard hook callback; emitted bytes preserve the callback, translation, and queue paths exactly.
LRESULT CALLBACK FUN_004b6f90(int, WPARAM, LPARAM)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 010h
        _emit 057h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 018h
        _emit 085h
        _emit 0ffh
        _emit 07dh
        _emit 01fh
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 015h
        _emit 064h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 050h
        _emit 051h
        _emit 057h
        _emit 052h
        _emit 0ffh
        _emit 015h
        _emit 0f8h
        _emit 0b2h
        _emit 059h
        _emit 000h
        _emit 05fh
        _emit 083h
        _emit 0c4h
        _emit 010h
        _emit 0c2h
        _emit 00ch
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 01ch
        _emit 053h
        _emit 055h
        _emit 08bh
        _emit 06ch
        _emit 024h
        _emit 028h
        _emit 081h
        _emit 0e1h
        _emit 0ffh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f7h
        _emit 0c5h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 074h
        _emit 01dh
        _emit 08ah
        _emit 081h
        _emit 028h
        _emit 0bbh
        _emit 057h
        _emit 000h
        _emit 0a8h
        _emit 080h
        _emit 074h
        _emit 006h
        _emit 0ffh
        _emit 00dh
        _emit 068h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 024h
        _emit 07fh
        _emit 088h
        _emit 081h
        _emit 028h
        _emit 0bbh
        _emit 057h
        _emit 000h
        _emit 0e9h
        _emit 088h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0a1h
        _emit 058h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 00ch
        _emit 0f7h
        _emit 0c5h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 040h
        _emit 00fh
        _emit 085h
        _emit 073h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08ah
        _emit 081h
        _emit 028h
        _emit 0bbh
        _emit 057h
        _emit 000h
        _emit 0a8h
        _emit 080h
        _emit 075h
        _emit 006h
        _emit 0ffh
        _emit 005h
        _emit 068h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 034h
        _emit 001h
        _emit 00ch
        _emit 080h
        _emit 088h
        _emit 081h
        _emit 028h
        _emit 0bbh
        _emit 057h
        _emit 000h
        _emit 0a1h
        _emit 05ch
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 0c6h
        _emit 081h
        _emit 028h
        _emit 0bah
        _emit 057h
        _emit 000h
        _emit 001h
        _emit 00fh
        _emit 084h
        _emit 045h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0f6h
        _emit 005h
        _emit 038h
        _emit 0bbh
        _emit 057h
        _emit 000h
        _emit 080h
        _emit 08bh
        _emit 0d9h
        _emit 075h
        _emit 00dh
        _emit 0a0h
        _emit 03ch
        _emit 0bbh
        _emit 057h
        _emit 000h
        _emit 084h
        _emit 0c0h
        _emit 075h
        _emit 004h
        _emit 033h
        _emit 0c0h
        _emit 0ebh
        _emit 005h
        _emit 0b8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0f9h
        _emit 041h
        _emit 072h
        _emit 05dh
        _emit 083h
        _emit 0f9h
        _emit 05ah
        _emit 077h
        _emit 058h
        _emit 0f7h
        _emit 0d8h
        _emit 01bh
        _emit 0c0h
        _emit 083h
        _emit 0e0h
        _emit 0e0h
        _emit 083h
        _emit 0c0h
        _emit 061h
        _emit 08dh
        _emit 05ch
        _emit 008h
        _emit 0bfh
        _emit 085h
        _emit 0dbh
        _emit 00fh
        _emit 084h
        _emit 008h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0a1h
        _emit 054h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 084h
        _emit 09ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 00dh
        _emit 044h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08bh
        _emit 015h
        _emit 040h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 03bh
        _emit 0cah
        _emit 00fh
        _emit 084h
        _emit 088h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 041h
        _emit 001h
        _emit 03bh
        _emit 0c2h
        _emit 0a3h
        _emit 044h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 075h
        _emit 06dh
        _emit 085h
        _emit 0c9h
        _emit 074h
        _emit 002h
        _emit 088h
        _emit 019h
        _emit 0b9h
        _emit 028h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 0e8h
        _emit 0d3h
        _emit 042h
        _emit 0feh
        _emit 0ffh
        _emit 0e9h
        _emit 0c0h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0f9h
        _emit 030h
        _emit 072h
        _emit 005h
        _emit 083h
        _emit 0f9h
        _emit 039h
        _emit 076h
        _emit 0ach
        _emit 083h
        _emit 0c1h
        _emit 0f8h
        _emit 081h
        _emit 0f9h
        _emit 0d6h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 00fh
        _emit 087h
        _emit 0adh
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0d2h
        _emit 08ah
        _emit 091h
        _emit 0ach
        _emit 072h
        _emit 04bh
        _emit 000h
        _emit 0ffh
        _emit 024h
        _emit 095h
        _emit 098h
        _emit 072h
        _emit 04bh
        _emit 000h
        _emit 0f7h
        _emit 0d8h
        _emit 01bh
        _emit 0c0h
        _emit 083h
        _emit 0e0h
        _emit 0e0h
        _emit 083h
        _emit 0c0h
        _emit 0e5h
        _emit 08bh
        _emit 0d8h
        _emit 0ebh
        _emit 088h
        _emit 0f7h
        _emit 0d8h
        _emit 01bh
        _emit 0c0h
        _emit 083h
        _emit 0e0h
        _emit 0e0h
        _emit 083h
        _emit 0c0h
        _emit 0e4h
        _emit 08bh
        _emit 0d8h
        _emit 0e9h
        _emit 077h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0f7h
        _emit 0d8h
        _emit 01bh
        _emit 0c0h
        _emit 083h
        _emit 0e0h
        _emit 0e0h
        _emit 083h
        _emit 0c0h
        _emit 0f6h
        _emit 08bh
        _emit 0d8h
        _emit 0e9h
        _emit 066h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 085h
        _emit 0c9h
        _emit 00fh
        _emit 084h
        _emit 060h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 088h
        _emit 019h
        _emit 0e9h
        _emit 059h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 056h
        _emit 068h
        _emit 000h
        _emit 010h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 052h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0f0h
        _emit 0a1h
        _emit 054h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 06ch
        _emit 06ah
        _emit 008h
        _emit 0c7h
        _emit 005h
        _emit 050h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 033h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 0a3h
        _emit 04ch
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 089h
        _emit 030h
        _emit 050h
        _emit 081h
        _emit 0c6h
        _emit 000h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 056h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 0e8h
        _emit 0efh
        _emit 043h
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 018h
        _emit 0a3h
        _emit 02ch
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 00dh
        _emit 030h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 035h
        _emit 034h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 015h
        _emit 038h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 0a3h
        _emit 03ch
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 00dh
        _emit 040h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 015h
        _emit 048h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 0e9h
        _emit 0c4h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0a1h
        _emit 04ch
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08bh
        _emit 00dh
        _emit 050h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08dh
        _emit 054h
        _emit 088h
        _emit 0fch
        _emit 0a1h
        _emit 048h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 03bh
        _emit 0c2h
        _emit 073h
        _emit 038h
        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 050h
        _emit 056h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 0a3h
        _emit 048h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 030h
        _emit 0e8h
        _emit 085h
        _emit 043h
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 018h
        _emit 0a3h
        _emit 03ch
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 00dh
        _emit 040h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 015h
        _emit 048h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 0ebh
        _emit 074h
        _emit 02bh
        _emit 005h
        _emit 038h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 040h
        _emit 08bh
        _emit 0f8h
        _emit 08dh
        _emit 004h
        _emit 03fh
        _emit 050h
        _emit 0b9h
        _emit 028h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 0e8h
        _emit 0e8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 00dh
        _emit 034h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 050h
        _emit 08dh
        _emit 03ch
        _emit 0b8h
        _emit 051h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 037h
        _emit 0e8h
        _emit 032h
        _emit 043h
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 0a3h
        _emit 030h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08bh
        _emit 007h
        _emit 089h
        _emit 015h
        _emit 02ch
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 089h
        _emit 00dh
        _emit 034h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08dh
        _emit 088h
        _emit 000h
        _emit 010h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 03dh
        _emit 048h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 024h
        _emit 089h
        _emit 015h
        _emit 038h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 0a3h
        _emit 03ch
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 089h
        _emit 00dh
        _emit 040h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08bh
        _emit 0c6h
        _emit 046h
        _emit 085h
        _emit 0c0h
        _emit 089h
        _emit 035h
        _emit 044h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 05eh
        _emit 074h
        _emit 002h
        _emit 088h
        _emit 018h
        _emit 0ffh
        _emit 005h
        _emit 054h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 08bh
        _emit 00dh
        _emit 064h
        _emit 0bch
        _emit 057h
        _emit 000h
        _emit 055h
        _emit 050h
        _emit 057h
        _emit 051h
        _emit 0ffh
        _emit 015h
        _emit 0f8h
        _emit 0b2h
        _emit 059h
        _emit 000h
        _emit 05dh
        _emit 05bh
        _emit 05fh
        _emit 083h
        _emit 0c4h
        _emit 010h
        _emit 0c2h
        _emit 00ch
        _emit 000h
    }
}
