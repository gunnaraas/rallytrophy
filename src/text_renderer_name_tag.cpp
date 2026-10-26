#include "unk_481390.h"
#include "rallytrophy.h"

extern "C" void fun_004a9b40();
extern "C" void fun_004b7670();
extern "C" char DAT_0057a7d8[];

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a9b00
// original: FUN_004a9b00
void TextRenderer::DrawNameTag(RenderDevice*, ViewPoint*, Vec3Slot*, Player*, float)
{
    __asm {
        mov eax, dword ptr [esp + 0x10]
        push esi
        push -1
        push eax
        push offset DAT_0057a7d8
        mov esi, ecx
        call fun_004b7670
        mov ecx, dword ptr [esp + 0x24]
        mov edx, dword ptr [esp + 0x1c]
        mov eax, dword ptr [esp + 0x18]
        add esp, 0xc
        push ecx
        mov ecx, dword ptr [esp + 0xc]
        push offset DAT_0057a7d8
        push edx
        push eax
        push ecx
        mov ecx, esi
        call fun_004a9b40
        pop esi
        ret 0x14
    }
}
