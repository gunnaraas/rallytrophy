// Exact VC6 reset helper kept out of file.cpp so its caller retains the original EH model.
#define RT_FILE_TYPED_LAYOUT
#include "rallytrophy.h"
#undef RT_FILE_TYPED_LAYOUT

extern unsigned int DAT_004e1340;
extern "C" {
    void FUN_00402700();
    void FUN_00402980();
    void FUN_00402960();
    void __cdecl FUN_004c14e0(void*, const void*, unsigned int);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a4e60
// original: FUN_004a4e60
// Mirrors the observed 229-byte filename reset and stream-state cleanup.
void __fastcall FUN_004a4e60(void*)
{
    __asm {
        sub    esp,0x8
        push   ebx
        mov    ebx,ecx
        push   ebp
        or     eax,0xffffffff
        xor    edx,edx
        push   esi
        mov    esi,dword ptr [ebx+0x8]
        cmp    esi,edx
        lea    ebp,[ebx+0x4]
        mov    dword ptr [ebx+0x18],eax
        mov    dword ptr [ebx+0x24],eax
        mov    eax,[DAT_004e1340]
        push   edi
        mov    dword ptr [ebx+0x20],edx
        mov    dword ptr [ebx+0x1c],edx
        mov    dword ptr [ebx],edx
        mov    dword ptr [esp+0x10],eax
        je reset_4a4ee7
        mov    cl,byte ptr [esi-0x1]
        test   cl,cl
        je reset_4a4ee7
        cmp    cl,0xff
        je reset_4a4ee7
        push   0x1
        mov    ecx,ebp
        call FUN_00402700
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,esi
        repne scasb
        not    ecx
        dec    ecx
        mov    edi,ecx
        push   0x1
        push   edi
        mov    ecx,ebp
        mov    dword ptr [esp+0x1c],edi
        call FUN_00402980
        test   al,al
        je reset_4a4ee1
        mov    ecx,edi
        mov    edi,dword ptr [ebp+0x4]
        mov    eax,ecx
        shr    ecx,0x2
        rep movsd
        mov    ecx,eax
        and    ecx,0x3
        rep movsb
        mov    ecx,eax
        push   ecx
        mov    ecx,ebp
        call FUN_00402960
        reset_4a4ee1:
        mov    eax,dword ptr [esp+0x10]
        xor    edx,edx
        reset_4a4ee7:
        mov    edi,dword ptr [ebp+0x8]
        cmp    edi,eax
        jae reset_4a4ef4
        mov    dword ptr [esp+0x10],edi
        mov    eax,edi
        reset_4a4ef4:
        cmp    eax,edx
        jbe reset_4a4f25
        mov    ecx,dword ptr [ebp+0x4]
        sub    edi,eax
        push   edi
        lea    edx,[ecx+eax*1]
        push   edx
        push   ecx
        call FUN_004c14e0
        add    esp,0xc
        push   0x0
        push   edi
        mov    ecx,ebp
        call FUN_00402980
        test   al,al
        je reset_4a4f23
        mov    eax,dword ptr [ebp+0x4]
        mov    dword ptr [ebp+0x8],edi
        mov    byte ptr [eax+edi*1],0x0
        reset_4a4f23:
        xor    edx,edx
        reset_4a4f25:
        pop    edi
        pop    esi
        pop    ebp
        mov    byte ptr [ebx+0x14],0x0
        mov    byte ptr [ebx+0x15],0x0
        mov    byte ptr [ebx+0x16],0x0
        mov    dword ptr [ebx+0x68],edx
        mov    dword ptr [ebx+0x6c],edx
        mov    dword ptr [ebx+0x70],edx
        mov    dword ptr [ebx+0x74],edx
        pop    ebx
        add    esp,0x8
        ret
    }
}

