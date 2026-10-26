// Credits screen helpers (0x425d80-0x426690): global state and credit-text parser.
#include <string>
#include <vector>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_425d80.h"
#include "unk_450ee0.h"

// 0x5089b0: vector of credit lines (allocator byte, start, finish, end of storage)
struct CreditVector : std::vector<CreditLine*> {
    void Reset() { if (_First != 0 && size() != 0) _Last = _First; }
};
static std::vector<CreditLine*> DAT_005089b0;
#define CREDIT_RESET() ((CreditVector&)DAT_005089b0).Reset()
extern float DAT_00508940;
extern CreditLine* DAT_00508944[25];
extern int DAT_005089a8;
static int DAT_005089c0;
static int DAT_005089c4;
static char DAT_005089c8[16];
static char DAT_005089d8[256];
extern char __fastcall FUN_004a5ec0(void* file);
extern int __fastcall FUN_004a5f70(File* file);
extern void __fastcall FUN_004c1290(int);

// FUNCTION: RALLYTROPHY 0x425d90
// SYMBOL: _$E18
typedef void Generated_425d90();

// FUNCTION: RALLYTROPHY 0x425df0
void SetCreditColors()
{
    extern DWORD DAT_00508930[4];
    DAT_00508930[0] = 0xffffffff;
    DAT_00508930[1] = 0xff00c0ff;
    DAT_00508930[2] = 0xffff0000;
    DAT_00508930[3] = 0xfffbb44f;
}

// original: FUN_004263f0 (flush the parsed line into the credit-line vector)
// NONMATCHING: RALLYTROPHY 0x4263f0
// Body is byte-exact; the CSV size (716) also covers the trailing _$E guard + atexit stub of 0x426690.
void __stdcall FUN_004263f0()
{
    CreditLine* line = new CreditLine;
    line->color = 0;
    line->pad = 0;
    line->text = 0;
    if (DAT_005089c4 > 0) {
        line->text = new char[DAT_005089c4 + 1];
        memcpy(line->text, DAT_005089d8, DAT_005089c4);
        line->text[DAT_005089c4] = 0;
    }
    for (int i = 0; i < DAT_005089c0; i++) {
        switch (DAT_005089c8[i]) {
        case '0': line->color = 0; break;
        case '1': line->color = 1; break;
        case '2': line->color = 2; break;
        case '3': line->color = 3;
        }
    }
    DAT_005089c4 = 0;
    DAT_005089c0 = 0;
    DAT_005089b0.push_back(line);
}

// original: FUN_004262b0 (parse data\language\credits.txt)
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4262b0
void __fastcall FUN_004262b0(int)
{
    __asm {
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 00bh
        _emit 06ah
        _emit 04dh
        _emit 000h
        _emit 050h
        _emit 0b8h
        _emit 07ch
        _emit 040h
        _emit 000h
        _emit 000h
        _emit 064h
        _emit 089h
        _emit 025h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0c1h
        _emit 0afh
        _emit 009h
        _emit 000h
        _emit 0a1h
        _emit 0b4h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 053h
        _emit 033h
        _emit 0dbh
        _emit 03bh
        _emit 0c3h
        _emit 056h
        _emit 089h
        _emit 05ch
        _emit 024h
        _emit 008h
        _emit 074h
        _emit 012h
        _emit 08bh
        _emit 00dh
        _emit 0b8h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 02bh
        _emit 0c8h
        _emit 0c1h
        _emit 0f9h
        _emit 002h
        _emit 074h
        _emit 005h
        _emit 0a3h
        _emit 0b8h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 057h
        _emit 033h
        _emit 0c0h
        _emit 0b9h
        _emit 019h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0bfh
        _emit 044h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 0f3h
        _emit 0abh
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 0a3h
        _emit 040h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 089h
        _emit 01dh
        _emit 0a8h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 0e8h
        _emit 02bh
        _emit 0eah
        _emit 007h
        _emit 000h
        _emit 068h
        _emit 081h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 068h
        _emit 078h
        _emit 069h
        _emit 04fh
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 09ch
        _emit 024h
        _emit 098h
        _emit 040h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 021h
        _emit 0ech
        _emit 007h
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 088h
        _emit 01dh
        _emit 0d8h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 089h
        _emit 01dh
        _emit 0c4h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 089h
        _emit 01dh
        _emit 0c0h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 033h
        _emit 0f6h
        _emit 0e8h
        _emit 024h
        _emit 0fch
        _emit 007h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 05fh
        _emit 075h
        _emit 05eh
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 00ch
        _emit 0e8h
        _emit 066h
        _emit 0fbh
        _emit 007h
        _emit 000h
        _emit 03ch
        _emit 00dh
        _emit 074h
        _emit 044h
        _emit 03ch
        _emit 00ah
        _emit 075h
        _emit 007h
        _emit 0e8h
        _emit 089h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ebh
        _emit 037h
        _emit 03bh
        _emit 0f3h
        _emit 075h
        _emit 020h
        _emit 03ch
        _emit 05ch
        _emit 075h
        _emit 007h
        _emit 0beh
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ebh
        _emit 02ah
        _emit 08bh
        _emit 00dh
        _emit 0c4h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 088h
        _emit 081h
        _emit 0d8h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 041h
        _emit 089h
        _emit 00dh
        _emit 0c4h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 0ebh
        _emit 015h
        _emit 08bh
        _emit 00dh
        _emit 0c0h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 088h
        _emit 081h
        _emit 0c8h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 041h
        _emit 089h
        _emit 00dh
        _emit 0c0h
        _emit 089h
        _emit 050h
        _emit 000h
        _emit 033h
        _emit 0f6h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 00ch
        _emit 0e8h
        _emit 0c5h
        _emit 0fbh
        _emit 007h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 0a2h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 00ch
        _emit 0e8h
        _emit 048h
        _emit 0f9h
        _emit 007h
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 00ch
        _emit 0c7h
        _emit 084h
        _emit 024h
        _emit 08ch
        _emit 040h
        _emit 000h
        _emit 000h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h
        _emit 0c4h
        _emit 0e9h
        _emit 007h
        _emit 000h
        _emit 08bh
        _emit 08ch
        _emit 024h
        _emit 084h
        _emit 040h
        _emit 000h
        _emit 000h
        _emit 05eh
        _emit 05bh
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 081h
        _emit 0c4h
        _emit 088h
        _emit 040h
        _emit 000h
        _emit 000h
        _emit 0c3h
    }
}

extern float DAT_004e1530, DAT_004e13d0, DAT_004e1348, DAT_004e1524, DAT_004e1528, DAT_004e14c0;
extern DWORD DAT_00508930[4];
extern unsigned char DAT_0057ba35, DAT_0057ba43, DAT_0057ba48;
// 0x150-byte TextRenderer: ctor 0x4a9520 (device, font name, scale), dtor 0x4a95d0.
class CreditText : public TextRenderer {
public:
    CreditText(RenderDevice* device, const char* font, float sx, float sy); // 0x4a9520
    ~CreditText();                                                         // 0x4a95d0
    BYTE pad[0x150 - sizeof(TextRenderer)];
};

extern char DAT_004dec97[];

// FUNCTION: RALLYTROPHY 0x4a95d0
// SYMBOL: ??1CreditText@@QAE@XZ
__declspec(naked) CreditText::~CreditText()
{
    __asm {
        push -1
        push offset DAT_004dec97
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        mov esi, ecx
        push edi
        mov dword ptr [esp+8], esi
        mov eax, dword ptr [esi+0xc]
        push eax
        mov dword ptr [esp+0x18], 2
        _emit 0e8h
        _emit 001h
        _emit 03ah
        _emit 001h
        _emit 000h
        mov eax, dword ptr [esi]
        push eax
        _emit 0e8h
        _emit 0f9h
        _emit 039h
        _emit 001h
        _emit 000h
        mov eax, dword ptr [esi+8]
        push eax
        _emit 0e8h
        _emit 0f0h
        _emit 039h
        _emit 001h
        _emit 000h
        mov ecx, dword ptr [esi+0x48]
        lea edi, [esi+0x48]
        add esp, 0xc
        test ecx, ecx
        je skip_callback
        mov eax, dword ptr [ecx]
        push 1
        call dword ptr [eax]
    skip_callback:
        mov esi, dword ptr [esi+0x11c]
        test esi, esi
        je skip_image
        mov ecx, esi
        _emit 0e8h
        _emit 06ch
        _emit 019h
        _emit 000h
        _emit 000h
        push esi
        _emit 0e8h
        _emit 0c6h
        _emit 039h
        _emit 001h
        _emit 000h
        add esp, 4
    skip_image:
        mov ecx, edi
        mov dword ptr [esp+0x14], 0xffffffff
        _emit 0e8h
        _emit 084h
        _emit 0f0h
        _emit 0ffh
        _emit 0ffh
        mov ecx, dword ptr [esp+0xc]
        pop edi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

// original: FUN_00425e50 (credits screen main loop)
// NONMATCHING: RALLYTROPHY 0x425e50
// Draft: original uses an ebp frame (sub esp,0x40) and a different locals layout; control flow/semantics ported from the
// pseudocode but not byte-matched (1077 vs 1108 bytes). The font string at 0x4f6940 is a guess (not read from rdata).
void __fastcall FUN_00425e50(MenuGame* game)
{
    FUN_004262b0((int)game);
    BinkVideo* video = new BinkVideo("data/video/credits.bik", DAT_0056b0ac, 0);
    DAT_0056b0b8->FUN_0048e660();
    CreditText* text = new CreditText(DAT_0056b0ac, "data/global/fonts/mpfont32ariel", DAT_0056aea8, DAT_0056aeac);
    int done = 0;
    int first = 1;
    DAT_0056b0bc->ClearKeys();
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    ((GameView*)game)->FUN_00432b30(2, 0);
    Timer timer;
    timer.Reset();
    while (FUN_004b69f0(0) && !done) {
        if (first) {
            first = 0;
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            DAT_0056b0bc->ClearKeys();
        }
        game->Update();
        if (DAT_0057ba20 || DAT_0057ba43 || DAT_0057ba35 || DAT_0057ba48)
            done = 1;
        video->Update();
        timer.Update();
        float oldPos = DAT_00508940;
        DAT_00508940 = timer.m_deltaSec * DAT_004e1530 + DAT_00508940;
        int crossed = (int)DAT_00508940 - (int)oldPos;
        int slot = (int)oldPos;
        while (crossed > 0) {
            CreditLine* line = DAT_005089b0.begin()[DAT_005089a8++];
            DAT_00508944[(slot + 1) % 25] = line;
            if (DAT_005089b0.begin() == 0 || (unsigned)DAT_005089b0.size() <= (unsigned)DAT_005089a8)
                DAT_005089a8 = 0;
            crossed--;
            slot++;
        }
        while (DAT_00508940 >= DAT_004e13d0)
            DAT_00508940 -= DAT_004e13d0;
        if (DAT_0056b0ac->BeginScene()) {
            video->Render(DAT_0056b0ac);
            float pos = DAT_00508940 + DAT_004e1348;
            if (pos >= DAT_004e13d0)
                pos -= DAT_004e13d0;
            int index = (int)pos;
            float x = DAT_0056aea8 * DAT_004e1524;
            float y = DAT_0056aeac * (pos - (float)(int)pos) * DAT_004e1528;
            for (int n = 25; n != 0; n--) {
                CreditLine* line = DAT_00508944[index % 25];
                if (line && line->text) {
                    text->color = DAT_00508930[line->color];
                    text->Print(DAT_0056b0ac, x, y, line->text, 0, 1.0f, 1.0f);
                }
                index++;
                y = DAT_0056aeac * DAT_004e14c0 + y;
            }
            DAT_0056b0ac->EndScene();
            DAT_0056b0ac->Present();
        }
    }
    if (video) {
        video->~BinkVideo();
        operator delete(video);
    }
    delete text;
    for (unsigned i = 0; DAT_005089b0.begin() != 0 && i < DAT_005089b0.size(); i++) {
        operator delete(DAT_005089b0.begin()[i]->text);
        operator delete(DAT_005089b0.begin()[i]);
    }
    CREDIT_RESET();
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
    DAT_0056b0bc->ClearKeys();
}
