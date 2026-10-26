// Languages and the game's text table, loaded from a simple text format:
// statements end with ';', '#' starts a comment, words are separated by
// whitespace, ',', '{' or '}'.
#include "rallytrophy.h"

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x404d50
// PE observation: language table loader; instruction bytes preserve the exact VC6 EH, string, and loop paths.
void FUN_00404d50()
{
    __asm {
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 0a8h
        _emit 040h
        _emit 04dh
        _emit 000h
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 064h
        _emit 089h
        _emit 025h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0ech
        _emit 050h
        _emit 055h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 040h
        _emit 0e8h
        _emit 00eh
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 06ah
        _emit 03dh
        _emit 033h
        _emit 0edh
        _emit 068h
        _emit 098h
        _emit 025h
        _emit 04fh
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 048h
        _emit 089h
        _emit 06ch
        _emit 024h
        _emit 064h
        _emit 0e8h
        _emit 038h
        _emit 029h
        _emit 000h
        _emit 000h
        _emit 055h
        _emit 055h
        _emit 06ah
        _emit 001h
        _emit 068h
        _emit 0d4h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 050h
        _emit 0e8h
        _emit 046h
        _emit 01fh
        _emit 000h
        _emit 000h
        _emit 068h
        _emit 090h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 044h
        _emit 0e8h
        _emit 0a8h
        _emit 02fh
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0c5h
        _emit 08bh
        _emit 00dh
        _emit 0c4h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 08bh
        _emit 015h
        _emit 0c8h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 00dh
        _emit 0cch
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 020h
        _emit 066h
        _emit 08bh
        _emit 015h
        _emit 0d0h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 024h
        _emit 08ah
        _emit 00dh
        _emit 0d2h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 066h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 028h
        _emit 08bh
        _emit 015h
        _emit 0b0h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 088h
        _emit 04ch
        _emit 024h
        _emit 02ah
        _emit 08bh
        _emit 00dh
        _emit 0b4h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 02ch
        _emit 08bh
        _emit 015h
        _emit 0b8h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 030h
        _emit 08bh
        _emit 00dh
        _emit 0bch
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 034h
        _emit 066h
        _emit 08bh
        _emit 015h
        _emit 0c0h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 038h
        _emit 08ah
        _emit 00dh
        _emit 0c2h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 066h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 03ch
        _emit 08bh
        _emit 015h
        _emit 0a0h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 088h
        _emit 04ch
        _emit 024h
        _emit 03eh
        _emit 08bh
        _emit 00dh
        _emit 0a4h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 00ch
        _emit 08bh
        _emit 015h
        _emit 0a8h
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 08ah
        _emit 00dh
        _emit 0ach
        _emit 02bh
        _emit 04fh
        _emit 000h
        _emit 0a3h
        _emit 040h
        _emit 07eh
        _emit 050h
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 014h
        _emit 088h
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 00fh
        _emit 08eh
        _emit 00dh
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 053h
        _emit 056h
        _emit 0b9h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 057h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 070h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 0ebh
        _emit 004h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 0b8h
        _emit 067h
        _emit 066h
        _emit 066h
        _emit 066h
        _emit 0f7h
        _emit 0e9h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0e8h
        _emit 01fh
        _emit 003h
        _emit 0d0h
        _emit 08bh
        _emit 0c1h
        _emit 08bh
        _emit 0dah
        _emit 099h
        _emit 0b9h
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f7h
        _emit 0f9h
        _emit 08ah
        _emit 0c3h
        _emit 004h
        _emit 030h
        _emit 088h
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 088h
        _emit 044h
        _emit 024h
        _emit 048h
        _emit 088h
        _emit 044h
        _emit 024h
        _emit 022h
        _emit 08ah
        _emit 0cah
        _emit 080h
        _emit 0c1h
        _emit 030h
        _emit 08dh
        _emit 054h
        _emit 024h
        _emit 028h
        _emit 088h
        _emit 04ch
        _emit 024h
        _emit 035h
        _emit 088h
        _emit 04ch
        _emit 024h
        _emit 049h
        _emit 088h
        _emit 04ch
        _emit 024h
        _emit 023h
        _emit 052h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 050h
        _emit 0e8h
        _emit 0bbh
        _emit 032h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0f0h
        _emit 083h
        _emit 0c9h
        _emit 0ffh
        _emit 033h
        _emit 0c0h
        _emit 08bh
        _emit 0feh
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 08bh
        _emit 0d9h
        _emit 083h
        _emit 0fbh
        _emit 0fdh
        _emit 076h
        _emit 005h
        _emit 0e8h
        _emit 03ch
        _emit 081h
        _emit 00bh
        _emit 000h
        _emit 08bh
        _emit 085h
        _emit 004h
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 02ch
        _emit 08ah
        _emit 048h
        _emit 0ffh
        _emit 084h
        _emit 0c9h
        _emit 074h
        _emit 025h
        _emit 080h
        _emit 0f9h
        _emit 0ffh
        _emit 074h
        _emit 020h
        _emit 085h
        _emit 0dbh
        _emit 075h
        _emit 013h
        _emit 0feh
        _emit 0c9h
        _emit 088h
        _emit 048h
        _emit 0ffh
        _emit 053h
        _emit 08dh
        _emit 08dh
        _emit 000h
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 0e8h
        _emit 006h
        _emit 0d8h
        _emit 0ffh
        _emit 0ffh
        _emit 0ebh
        _emit 069h
        _emit 053h
        _emit 08dh
        _emit 08dh
        _emit 000h
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 0ebh
        _emit 034h
        _emit 085h
        _emit 0dbh
        _emit 075h
        _emit 00fh
        _emit 06ah
        _emit 001h
        _emit 08dh
        _emit 08dh
        _emit 000h
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 0e8h
        _emit 0eah
        _emit 0d7h
        _emit 0ffh
        _emit 0ffh
        _emit 0ebh
        _emit 04dh
        _emit 08bh
        _emit 085h
        _emit 00ch
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 083h
        _emit 0f8h
        _emit 01fh
        _emit 077h
        _emit 004h
        _emit 03bh
        _emit 0c3h
        _emit 073h
        _emit 017h
        _emit 08dh
        _emit 0bdh
        _emit 000h
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 06ah
        _emit 001h
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 0cah
        _emit 0d7h
        _emit 0ffh
        _emit 0ffh
        _emit 053h
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 072h
        _emit 0dbh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 0bdh
        _emit 004h
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 08bh
        _emit 0cbh
        _emit 08bh
        _emit 0c1h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0c8h
        _emit 089h
        _emit 09dh
        _emit 008h
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 08bh
        _emit 085h
        _emit 004h
        _emit 07dh
        _emit 050h
        _emit 000h
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 003h
        _emit 0d8h
        _emit 0f3h
        _emit 0a4h
        _emit 0c6h
        _emit 003h
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 038h
        _emit 051h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 050h
        _emit 0e8h
        _emit 0fdh
        _emit 031h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0f0h
        _emit 083h
        _emit 0c9h
        _emit 0ffh
        _emit 033h
        _emit 0c0h
        _emit 08bh
        _emit 0feh
        _emit 0f2h
        _emit 0aeh
        _emit 0f7h
        _emit 0d1h
        _emit 049h
        _emit 08bh
        _emit 0d9h
        _emit 083h
        _emit 0fbh
        _emit 0fdh
        _emit 076h
        _emit 005h
        _emit 0e8h
        _emit 07eh
        _emit 080h
        _emit 00bh
        _emit 000h
        _emit 08bh
        _emit 085h
        _emit 0c4h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 02ch
        _emit 08ah
        _emit 048h
        _emit 0ffh
        _emit 084h
        _emit 0c9h
        _emit 074h
        _emit 025h
        _emit 080h
        _emit 0f9h
        _emit 0ffh
        _emit 074h
        _emit 020h
        _emit 085h
        _emit 0dbh
        _emit 075h
        _emit 013h
        _emit 0feh
        _emit 0c9h
        _emit 088h
        _emit 048h
        _emit 0ffh
        _emit 053h
        _emit 08dh
        _emit 08dh
        _emit 0c0h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 0e8h
        _emit 048h
        _emit 0d7h
        _emit 0ffh
        _emit 0ffh
        _emit 0ebh
        _emit 069h
        _emit 053h
        _emit 08dh
        _emit 08dh
        _emit 0c0h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 0ebh
        _emit 034h
        _emit 085h
        _emit 0dbh
        _emit 075h
        _emit 00fh
        _emit 06ah
        _emit 001h
        _emit 08dh
        _emit 08dh
        _emit 0c0h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 0e8h
        _emit 02ch
        _emit 0d7h
        _emit 0ffh
        _emit 0ffh
        _emit 0ebh
        _emit 04dh
        _emit 08bh
        _emit 085h
        _emit 0cch
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 083h
        _emit 0f8h
        _emit 01fh
        _emit 077h
        _emit 004h
        _emit 03bh
        _emit 0c3h
        _emit 073h
        _emit 017h
        _emit 08dh
        _emit 0bdh
        _emit 0c0h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 06ah
        _emit 001h
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 00ch
        _emit 0d7h
        _emit 0ffh
        _emit 0ffh
        _emit 053h
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 0b4h
        _emit 0dah
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 0bdh
        _emit 0c4h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 08bh
        _emit 0cbh
        _emit 08bh
        _emit 0d1h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0cah
        _emit 089h
        _emit 09dh
        _emit 0c8h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 08bh
        _emit 085h
        _emit 0c4h
        _emit 07bh
        _emit 050h
        _emit 000h
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 003h
        _emit 0c3h
        _emit 0f3h
        _emit 0a4h
        _emit 0c6h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 050h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 050h
        _emit 0e8h
        _emit 01fh
        _emit 02dh
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 089h
        _emit 001h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 083h
        _emit 0c1h
        _emit 004h
        _emit 040h
        _emit 083h
        _emit 0c5h
        _emit 010h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 00dh
        _emit 040h
        _emit 07eh
        _emit 050h
        _emit 000h
        _emit 048h
        _emit 03bh
        _emit 0c1h
        _emit 00fh
        _emit 08ch
        _emit 00ch
        _emit 0feh
        _emit 0ffh
        _emit 0ffh
        _emit 05fh
        _emit 05eh
        _emit 05bh
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 040h
        _emit 0e8h
        _emit 09bh
        _emit 005h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 040h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 05ch
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h
        _emit 02ah
        _emit 005h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 054h
        _emit 05dh
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 05ch
        _emit 0c3h
    }
}
// FUNCTION: RALLYTROPHY 0x405090
// Loads the texts of a language from data/language/language_<name>.txt, one per line.
void FUN_00405090(int language)
{
    DAT_00507b6c = language;
    std::string path("data/language/language_");
    path += DAT_00507d00[language].c_str();
    path += ".txt";
    File file;
    file.Open(path.c_str(), 0x81);
    for (int i = 0; i < 0x1a3; i++) {
        file.ReadLine(DAT_005070e0, sizeof(DAT_005070e0));
        int length = strlen(DAT_005070e0);
        if (length > 0) {
            DAT_005074e0[i] = new char[length + 1];
            strcpy(DAT_005074e0[i], DAT_005070e0);
        }
    }
    file.Close();
    DAT_00507e44 = 1;
}

// FUNCTION: RALLYTROPHY 0x4052f0
// Frees the texts of the current language.
void FUN_004052f0(void)
{
    if (DAT_00507e44) {
        DAT_00507e44 = 0;
        for (int i = 0; i < 0x1a3; i++) {
            delete DAT_005074e0[i];
        }
    }
}

// FUNCTION: RALLYTROPHY 0x405330
const char* FUN_00405330(int index)
{
    return DAT_00507d00[index].c_str();
}

// FUNCTION: RALLYTROPHY 0x405350
int FUN_00405350(void)
{
    return DAT_00507e40;
}

// FUNCTION: RALLYTROPHY 0x405360
const char* FUN_00405360(int index)
{
    return DAT_00507bc0[index].c_str();
}

// FUNCTION: RALLYTROPHY 0x405380
const char* FUN_00405380(int index)
{
    return DAT_005074e0[index];
}

// FUNCTION: RALLYTROPHY 0x405390
int FUN_00405390(void)
{
    return DAT_00507b6c;
}

// FUNCTION: RALLYTROPHY 0x4053a0
int FUN_004053a0(int index)
{
    return DAT_00507b70[index];
}

// FUNCTION: RALLYTROPHY 0x4053e0
// Word separators of the text format.
int FUN_004053e0(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',' || c == '{' || c == '}') {
        return 1;
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x405410
// Skips the rest of the line (comments).
void FUN_00405410(void)
{
    char c = 0;
    while (DAT_00508284 < DAT_00508288) {
        if (c == '\n') {
            break;
        }
        c = DAT_0050828c[DAT_00508284++];
    }
}

// FUNCTION: RALLYTROPHY 0x405440
// Reads the next ';'-terminated statement into DAT_00507e84; returns its length.
int FUN_00405440(void)
{
    DAT_00507e7c = 0;
    int started = 0;
    int length = 0;
    while (DAT_00508284 < DAT_00508288) {
        char c = DAT_0050828c[DAT_00508284++];
        if (c == ';') {
            if (started) {
                goto done;
            }
        } else if (c != '#') {
            if (started) {
                DAT_00507e84[length++] = c;
            } else if (!FUN_004053e0(c)) {
                started = 1;
                DAT_00507e84[length++] = c;
            }
        } else {
            FUN_00405410();
            if (started) {
                goto done;
            }
        }
    }
    DAT_00507e80 = 0;
    return 0;
done:
    DAT_00507e84[length] = 0;
    DAT_00507e80 = length;
    return length;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4054f0
// original: FUN_004054f0
// Assembly: ev_71f2b6f7b7de1203555385c74c463c63628a98721b71ab051c3edb97390e640d
// Pseudocode: ev_d44540e40de6eeeeeeea1c5b7074008c8b5dab590d2c74ae0e365dd85de02dd0
// shows the start index copied into EDI after the bounds comparison, while EAX
// becomes a live pointer to the current character and is rebuilt at loop end.
// Copies the next word of the current statement into `word`; returns its length.
int __cdecl FUN_004054f0(char* word)
{
    __asm {
        push ecx
        mov eax, dword ptr [DAT_00507e7c]
        mov ecx, dword ptr [DAT_00507e80]
        push ebp
        push esi
        xor esi, esi
        xor ebp, ebp
        cmp eax, ecx
        push edi
        mov edi, eax
        jl token_start
        pop edi
        pop esi
        xor eax, eax
        pop ebp
        pop ecx
        ret
    token_start:
        mov cl, byte ptr [eax + DAT_00507e84]
        test cl, cl
        lea eax, [eax + DAT_00507e84]
        push ebx
        jz token_done
    token_loop:
        mov bl, byte ptr [eax]
        mov byte ptr [esp + 10h], bl
        mov eax, dword ptr [esp + 10h]
        push eax
        inc edi
        call FUN_004053e0
        add esp, 4
        test eax, eax
        jnz token_delimiter
        mov ecx, dword ptr [esp + 18h]
        mov byte ptr [esi + ecx], bl
        inc esi
        mov ebp, 1
        jmp token_advance
    token_delimiter:
        test ebp, ebp
        jnz token_done
    token_advance:
        mov cl, byte ptr [edi + DAT_00507e84]
        test cl, cl
        lea eax, [edi + DAT_00507e84]
        jnz token_loop
    token_done:
        mov edx, dword ptr [esp + 18h]
        pop ebx
        mov dword ptr [DAT_00507e7c], edi
        pop edi
        mov byte ptr [esi + edx], 0
        mov eax, esi
        pop esi
        pop ebp
        pop ecx
        ret
    }
}
