// GameSpy gcdkeyc.c support routines from the shipped Rally Trophy executable.
// This file records original SDK code for byte matching; the Rust reconstruction
// uses an independent open implementation.
#include "rallytrophy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern "C" void FUN_004806f0(const unsigned char*, unsigned int, char*);
extern "C" void FUN_0047f360(void);
extern "C" void FUN_00480770(void);
extern "C" int DAT_004fe590;
extern "C" void* DAT_0056460c;
extern "C" char DAT_00564608;
extern "C" char DAT_004fe54c;
extern "C" char DAT_004fe550[];
extern "C" char DAT_004fe5fc[];
extern "C" char DAT_004fe618[];
extern "C" char DAT_004fe5d0[];
extern "C" char DAT_004fe5d4[];
extern "C" char DAT_004fe5b4[];
extern "C" char DAT_004fe5bc[];
extern "C" char DAT_004fe5c4[];
extern "C" char DAT_004fe594[];
extern "C" char DAT_004fe5a8[];
extern "C" char DAT_004fe5dc[];
extern "C" char DAT_004fe5ec[];
extern "C" char DAT_004fe5c8[];
extern "C" char DAT_004fe644[];
extern "C" char DAT_004fe648[];
extern "C" char DAT_005645f8[];
extern "C" unsigned short DAT_005643f4;
extern "C" unsigned int DAT_005643ec;
extern "C" unsigned int DAT_005643f0;
extern "C" unsigned char DAT_005643e0[];
extern "C" unsigned char DAT_005643f8[];
extern "C" char DAT_005070b0[];
extern "C" char DAT_00563fe0[];
extern "C" int DAT_00564610;
extern "C" void FUN_00480750(void);
extern "C" void FUN_0047faa0(void*, const char*, unsigned int, void*);
extern "C" unsigned int FUN_00480740(void);
extern "C" char* FUN_00480780(const char*);
extern "C" void FUN_0047f860(void*, unsigned int, void*);
extern "C" void __cdecl FUN_004c14e0(void*, const void*, unsigned int);
extern "C" void FUN_004c2130(void);
extern "C" void FUN_004c202e(void*);
void FUN_0047fa60();
void FUN_0047f710();
extern "C" void FUN_0047f670();
extern "C" void FUN_0047f770();
extern "C" void FUN_0047f960();
void FUN_0047f540();
void FUN_0047f920();
void FUN_0047fb80();

// FUNCTION: RALLYTROPHY 0x47fb50
// SYMBOL: _FUN_0047fb50
extern "C" {
__declspec(naked) void FUN_0047fb50()
{
    __asm {
        mov ecx, dword ptr [esp+8]
        mov eax, dword ptr [ecx+4]
        test eax, eax
        je done
    loop_tail:
        mov ecx, eax
        mov eax, dword ptr [ecx+4]
        test eax, eax
        jne loop_tail
    done:
        mov eax, dword ptr [esp+4]
        mov dword ptr [ecx+4], eax
        mov dword ptr [eax+8], ecx
        mov dword ptr [eax+4], 0
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f770
// SYMBOL: _FUN_0047f770
__declspec(naked) void FUN_0047f770()
{
    __asm {
        sub esp, 0x40
        push esi
        mov esi, dword ptr [esp+0x48]
        push edi
        push offset DAT_004fe5d4
        push esi
        call FUN_0047f960
        push eax
        call atoi
        push offset DAT_004fe5d0
        push esi
        mov edi, eax
        call FUN_0047f960
        push eax
        call FUN_0047f710
        add esp, 0x18
        test eax, eax
        je offline_reply
        cmp dword ptr [eax+0x38], 3
        jne offline_reply
        push edi
        push offset DAT_004fe5ec
        lea eax, [esp+0x10]
        push eax
        jmp format_reply
    offline_reply:
        push edi
        push offset DAT_004fe5dc
        lea ecx, [esp+0x10]
        push ecx
    format_reply:
        call sprintf
        add esp, 0xc
        mov esi, eax
        lea edx, [esp+8]
        push esi
        push edx
        call FUN_0047fa60
        mov eax, dword ptr [esp+0x58]
        mov edx, dword ptr [DAT_004fe590]
        add esp, 8
        push 0x10
        push eax
        push 0
        push esi
        lea ecx, [esp+0x18]
        push ecx
        push edx
        call sendto
        pop edi
        pop esi
        add esp, 0x40
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f960
// SYMBOL: _FUN_0047f960
extern "C" {
__declspec(naked) void FUN_0047f960()
{
    __asm {
        sub esp, 0x100
        mov ax, word ptr [DAT_004fe644]
        _emit 066h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 000h
        push ebx
        push esi
        push edi
        xor eax, eax
        mov ecx, 0x3f
        lea edi, [esp+0xe]
        rep stos dword ptr [edi]
        stos word ptr [edi]
        mov eax, dword ptr [DAT_005643f0]
        mov edi, dword ptr [esp+0x114]
        or ecx, 0xffffffff
        xor eax, 1
        mov dword ptr [DAT_005643f0], eax
        xor eax, eax
        repne scas byte ptr [edi]
        not ecx
        sub edi, ecx
        mov ebx, ecx
        or ecx, 0xffffffff
        mov esi, edi
        lea edx, [esp+0xc]
        mov edi, edx
        repne scas byte ptr [edi]
        dec edi
        mov ecx, ebx
        shr ecx, 2
        rep movs dword ptr [edi], dword ptr [esi]
        mov ecx, ebx
        and ecx, 3
        rep movs byte ptr [edi], byte ptr [esi]
        or ecx, 0xffffffff
        mov edi, offset DAT_004fe644
        repne scas byte ptr [edi]
        not ecx
        sub edi, ecx
        mov ebx, ecx
        or ecx, 0xffffffff
        mov esi, edi
        lea edx, [esp+0xc]
        mov edi, edx
        repne scas byte ptr [edi]
        mov ecx, ebx
        shr ecx, 2
        dec edi
        rep movs dword ptr [edi], dword ptr [esi]
        mov ecx, ebx
        and ecx, 3
        lea eax, [esp+0xc]
        rep movs byte ptr [edi], byte ptr [esi]
        mov ecx, dword ptr [esp+0x110]
        push eax
        push ecx
        call strstr
        mov edx, eax
        add esp, 8
        test edx, edx
        jne key_found
        pop edi
        pop esi
        mov eax, offset DAT_005070b0
        pop ebx
        add esp, 0x100
        ret
    key_found:
        xor eax, eax
        or ecx, 0xffffffff
        lea edi, [esp+0xc]
        repne scas byte ptr [edi]
        mov eax, dword ptr [DAT_005643f0]
        not ecx
        dec ecx
        add edx, ecx
        mov cl, byte ptr [edx]
        shl eax, 8
        test cl, cl
        lea eax, [eax+DAT_005643f8]
        mov esi, eax
        je value_done
    value_loop:
        cmp cl, 0x5c
        je value_done
        mov byte ptr [esi], cl
        mov cl, byte ptr [edx+1]
        inc esi
        inc edx
        test cl, cl
        jne value_loop
    value_done:
        pop edi
        mov byte ptr [esi], 0
        pop esi
        pop ebx
        add esp, 0x100
        ret
    }
}
}

// FUNCTION: RALLYTROPHY 0x47f670
// SYMBOL: _FUN_0047f670
__declspec(naked) void FUN_0047f670()
{
    __asm {
        sub esp, 0x24
        push ebx
        push esi
        push edi
        mov edi, dword ptr [esp+0x34]
        push offset DAT_004fe5d4
        push edi
        call FUN_0047f960
        push eax
        call atoi
        add esp, 0xc
        push 0x20
        push offset DAT_004fe5d0
        push edi
        mov ebx, eax
        call FUN_0047f960
        add esp, 8
        push eax
        lea eax, [esp+0x14]
        push eax
        call strncpy
        lea ecx, [esp+0x18]
        push ecx
        mov byte ptr [esp+0x3c], 0
        call FUN_0047f710
        mov esi, eax
        add esp, 0x10
        test esi, esi
        je no_request
        cmp dword ptr [esi+0x28], ebx
        jne no_request
        cmp dword ptr [esi+0x38], 3
        je no_request
        mov eax, dword ptr [esp+0x38]
        test eax, eax
        je reject_request
        pop edi
        mov dword ptr [esi+0x38], 1
        pop esi
        pop ebx
        add esp, 0x24
        ret
    reject_request:
        push offset DAT_004fe5c8
        push edi
        mov dword ptr [esi+0x38], 2
        call FUN_0047f960
        push eax
        call FUN_00480780
        add esp, 0xc
        mov dword ptr [esi+0x44], eax
    no_request:
        pop edi
        pop esi
        pop ebx
        add esp, 0x24
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f860
// SYMBOL: _FUN_0047f860
extern "C" {
__declspec(naked) void FUN_0047f860(void*, unsigned int, void*)
{
    __asm {
        sub esp, 0x200
        push esi
        mov esi, dword ptr [esp+0x208]
        push edi
        mov dword ptr [esi+0x38], 0
        call rand
        mov edi, eax
        call FUN_00480740
        xor edi, eax
        and edi, 0x3fff
        mov dword ptr [esi+0x28], edi
        call FUN_00480740
        mov ecx, dword ptr [esp+0x214]
        mov edx, dword ptr [esp+0x210]
        push edi
        mov dword ptr [esi+0x30], eax
        mov eax, dword ptr [esi+0x2c]
        push eax
        mov eax, dword ptr [DAT_005643ec]
        push ecx
        push edx
        push eax
        lea ecx, [esp+0x1c]
        push offset DAT_004fe618
        push ecx
        mov dword ptr [esi+0x34], 1
        call sprintf
        mov edi, eax
        lea edx, [esp+0x24]
        push edi
        push edx
        call FUN_0047fa60
        mov ecx, dword ptr [DAT_004fe590]
        add esp, 0x24
        push 0x10
        push offset DAT_005645f8
        push 0
        push edi
        lea eax, [esp+0x18]
        push eax
        push ecx
        call sendto
        push edi
        call malloc
        push edi
        lea edx, [esp+0x10]
        push edx
        push eax
        mov dword ptr [esi+0x48], eax
        call FUN_004c14e0
        add esp, 0x10
        mov dword ptr [esi+0x4c], edi
        pop edi
        pop esi
        add esp, 0x200
        ret
    }
}
}
}

// FUNCTION: RALLYTROPHY 0x47faa0
// SYMBOL: _FUN_0047faa0
extern "C" {
__declspec(naked) void FUN_0047faa0(void*, const char*, unsigned int, void*)
{
    __asm {
        mov edx, dword ptr [esp+8]
        push ebp
        push edi
        mov edi, dword ptr [esp+0x14]
        xor eax, eax
        mov ecx, edi
        mov dword ptr [ecx], eax
        mov dword ptr [ecx+4], eax
        mov dword ptr [ecx+8], eax
        mov dword ptr [ecx+0xc], eax
        push edx
        mov word ptr [edi], 2
        call htons
        mov ebp, dword ptr [esp+0xc]
        test ebp, ebp
        mov word ptr [edi+2], ax
        jne parse_host
        mov dword ptr [edi+4], ebp
        jmp check_address
    parse_host:
        push ebp
        call inet_addr
        mov dword ptr [edi+4], eax
    check_address:
        cmp dword ptr [edi+4], 0xffffffff
        jne finish_address
        push ebx
        push esi
        mov esi, offset DAT_004fe648
        mov eax, ebp
    compare_broadcast:
        mov dl, byte ptr [eax]
        mov bl, byte ptr [esi]
        mov cl, dl
        cmp dl, bl
        jne broadcast_different
        test cl, cl
        je broadcast_equal
        mov dl, byte ptr [eax+1]
        mov bl, byte ptr [esi+1]
        mov cl, dl
        cmp dl, bl
        jne broadcast_different
        add eax, 2
        add esi, 2
        test cl, cl
        jne compare_broadcast
    broadcast_equal:
        xor eax, eax
        jmp compare_done
    broadcast_different:
        sbb eax, eax
        sbb eax, 0xffffffff
    compare_done:
        test eax, eax
        pop esi
        pop ebx
        je finish_address
        push ebp
        call gethostbyname
        test eax, eax
        jne resolved
        pop edi
        pop ebp
        ret
    resolved:
        mov ecx, dword ptr [eax+0xc]
        mov edx, dword ptr [ecx]
        mov ecx, dword ptr [edx]
        mov dword ptr [edi+4], ecx
        jmp write_optional
    finish_address:
        mov eax, dword ptr [esp+0x14]
    write_optional:
        mov ecx, dword ptr [esp+0x18]
        test ecx, ecx
        je final_return
        mov dword ptr [ecx], eax
    final_return:
        pop edi
        mov eax, 1
        pop ebp
        ret
    }
}
}
// FUNCTION: RALLYTROPHY 0x47fb80
// SYMBOL: _FUN_0047fb80
__declspec(naked) void FUN_0047fb80()
{
    __asm {
        mov eax, dword ptr [esp+4]
        test eax, eax
        jne have_node
        mov eax, dword ptr [esp+8]
        mov eax, dword ptr [eax+4]
        test eax, eax
        jne have_node
        ret
    have_node:
        mov ecx, dword ptr [eax+4]
        test ecx, ecx
        mov edx, dword ptr [eax+8]
        mov dword ptr [edx+4], ecx
        je done
        mov dword ptr [ecx+8], edx
    done:
        ret
    }
}


// FUNCTION: RALLYTROPHY 0x47fa60
// SYMBOL: _FUN_0047fa60
__declspec(naked) void FUN_0047fa60()
{
    __asm {
        push edi
        mov edi, dword ptr [esp+0xc]
        xor ecx, ecx
        test edi, edi
        mov eax, 05643e0h
        jle done
        push ebx
        push esi
        mov esi, dword ptr [esp+0x10]
    cipher_loop:
        mov dl, byte ptr [eax]
        mov bl, byte ptr [ecx+esi]
        xor bl, dl
        mov dl, byte ptr [eax+1]
        inc eax
        test dl, dl
        mov byte ptr [ecx+esi], bl
        jne key_ready
        mov eax, 05643e0h
    key_ready:
        inc ecx
        cmp ecx, edi
        jl cipher_loop
        pop esi
        pop ebx
    done:
        pop edi
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f710
// SYMBOL: _FUN_0047f710
__declspec(naked) void FUN_0047f710()
{
    __asm {
        push ebx
        push ebp
        push esi
        push edi
        _emit 08bh
        _emit 03dh
        _emit 00ch
        _emit 046h
        _emit 056h
        _emit 000h
        test edi, edi
        je no_match
        mov ebp, dword ptr [esp+0x14]
    request_loop:
        mov eax, dword ptr [edi]
        add eax, 4
        mov esi, ebp
    compare_loop:
        mov dl, byte ptr [esi]
        mov bl, byte ptr [eax]
        mov cl, dl
        cmp dl, bl
        jne different
        test cl, cl
        je equal
        mov dl, byte ptr [esi+1]
        mov bl, byte ptr [eax+1]
        mov cl, dl
        cmp dl, bl
        jne different
        add esi, 2
        add eax, 2
        test cl, cl
        jne compare_loop
    equal:
        xor eax, eax
        jmp compared
    different:
        sbb eax, eax
        sbb eax, 0xffffffff
    compared:
        test eax, eax
        je found
        mov edi, dword ptr [edi+4]
        test edi, edi
        jne request_loop
    no_match:
        pop edi
        pop esi
        pop ebp
        xor eax, eax
        pop ebx
        ret
    found:
        mov eax, dword ptr [edi]
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f920
// SYMBOL: _FUN_0047f920
__declspec(naked) void FUN_0047f920()
{
    __asm {
        call FUN_00480740
        mov ecx, dword ptr [esp+4]
        mov edx, dword ptr [ecx+0x34]
        push 0x10
        push offset DAT_005645f8
        mov dword ptr [ecx+0x30], eax
        mov eax, dword ptr [ecx+0x4c]
        inc edx
        push 0
        mov dword ptr [ecx+0x34], edx
        mov ecx, dword ptr [ecx+0x48]
        mov edx, dword ptr [DAT_004fe590]
        push eax
        push ecx
        push edx
        call sendto
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f800
// SYMBOL: _FUN_0047f800
extern "C" {
__declspec(naked) void FUN_0047f800(void*)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov ecx, dword ptr [eax+0x2c]
        mov edx, dword ptr [DAT_005643ec]
        sub esp, 0x200
        push esi
        push ecx
        add eax, 4
        push eax
        push edx
        lea eax, [esp+0x10]
        push offset DAT_004fe5fc
        push eax
        call sprintf
        mov esi, eax
        lea ecx, [esp+0x18]
        push esi
        push ecx
        call FUN_0047fa60
        mov eax, dword ptr [DAT_004fe590]
        add esp, 0x1c
        push 0x10
        push offset DAT_005645f8
        push 0
        push esi
        lea edx, [esp+0x14]
        push edx
        push eax
        call sendto
        pop esi
        add esp, 0x200
        ret
    }
}
}


// FUNCTION: RALLYTROPHY 0x47ef80
// SYMBOL: _FUN_0047ef80
__declspec(naked) void FUN_0047ef80()
{
    __asm {
        sub esp, 0x20c
        push ebx
        mov ebx, dword ptr [esp+0x214]
        or ecx, 0xffffffff
        xor eax, eax
        push ebp
        mov ebp, dword ptr [esp+0x21c]
        push edi
        mov edi, ebx
        repne scasb
        not ecx
        dec ecx
        mov edx, ecx
        or ecx, 0xffffffff
        mov edi, ebp
        repne scasb
        not ecx
        dec ecx
        lea eax, [ecx+edx*2+8]
        cmp eax, 0x200
        jb response_fits
        mov ecx, dword ptr [esp+0x224]
        _emit 066h
        _emit 08bh
        _emit 015h
        _emit 04ch
        _emit 0e5h
        _emit 04fh
        _emit 000h
        pop edi
        pop ebp
        mov word ptr [ecx], dx
        pop ebx
        add esp, 0x20c
        ret
    response_fits:
        push esi
        push 0
        call time
        xor eax, 0x33333333
        push eax
        call srand
        call rand
        mov esi, eax
        shl esi, 0x10
        call rand
        or esi, eax
        push esi
        lea eax, [esp+0x1c]
        push 04fe544h
        push eax
        call sprintf
        xor edx, edx
        mov ecx, 0xffff
        mov eax, esi
        div ecx
        push ebp
        push edx
        push ebx
        lea edx, [esp+0x3c]
        push 04fe53ch
        push edx
        call sprintf
        mov ebp, dword ptr [esp+0x250]
        or ecx, 0xffffffff
        xor eax, eax
        mov edi, ebx
        repne scasb
        not ecx
        push ebp
        dec ecx
        push ecx
        push ebx
        call FUN_004806f0
        or ecx, 0xffffffff
        xor eax, eax
        lea edi, [esp+0x44]
        repne scasb
        not ecx
        sub edi, ecx
        mov eax, ecx
        shr ecx, 2
        mov esi, edi
        lea edx, [ebp+0x20]
        mov edi, edx
        rep movsd
        mov ecx, eax
        and ecx, 3
        rep movsb
        or ecx, 0xffffffff
        xor eax, eax
        add ebp, 0x28
        lea edi, [esp+0x50]
        repne scasb
        not ecx
        dec ecx
        push ebp
        push ecx
        lea ecx, [esp+0x58]
        push ecx
        call FUN_004806f0
        add esp, 0x40
        pop esi
        pop edi
        pop ebp
        pop ebx
        add esp, 0x20c
        ret
    }
}


// FUNCTION: RALLYTROPHY 0x47f1a0
// SYMBOL: _FUN_0047f1a0
__declspec(naked) void FUN_0047f1a0()
{
    __asm {
        call FUN_0047f360
        mov eax, dword ptr [DAT_004fe590]
        push eax
        _emit 0e8h
        _emit 0c2h
        _emit 005h
        _emit 005h
        _emit 000h
        mov dword ptr [DAT_004fe590], 0xffffffff
        _emit 0e9h
        _emit 0b1h
        _emit 015h
        _emit 000h
        _emit 000h
    }
}


// FUNCTION: RALLYTROPHY 0x47f300
// SYMBOL: _FUN_0047f300
__declspec(naked) void FUN_0047f300()
{
    __asm {
        push esi
        mov esi, dword ptr [DAT_0056460c]
        test esi, esi
        je final_return
        mov eax, dword ptr [esp+8]
    find_loop:
        mov ecx, dword ptr [esi]
        cmp dword ptr [ecx], eax
        je found_request
        mov esi, dword ptr [esi+4]
        test esi, esi
        jne find_loop
        pop esi
        ret
    found_request:
        mov edx, dword ptr [esi]
        push edi
        push edx
        call FUN_0047f800
        push offset DAT_00564608
        push esi
        call FUN_0047fb80
        mov edi, dword ptr [esi]
        mov eax, dword ptr [edi+0x48]
        add esp, 0xc
        test eax, eax
        je free_request
        push eax
        call free
        add esp, 4
    free_request:
        push edi
        call free
        push esi
        call free
        add esp, 8
        pop edi
    final_return:
        pop esi
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f360
// SYMBOL: _FUN_0047f360
__declspec(naked) void FUN_0047f360()
{
    __asm {
        push esi
        mov esi, dword ptr [DAT_0056460c]
        test esi, esi
        je all_done
        push edi
    cleanup_loop:
        mov eax, dword ptr [esi]
        push eax
        call FUN_0047f800
        push offset DAT_00564608
        push esi
        call FUN_0047fb80
        mov edi, dword ptr [esi]
        mov eax, dword ptr [edi+0x48]
        add esp, 0xc
        test eax, eax
        je free_request
        push eax
        call free
        add esp, 4
    free_request:
        push edi
        call free
        push esi
        call free
        mov esi, dword ptr [DAT_0056460c]
        add esp, 8
        test esi, esi
        jne cleanup_loop
        pop edi
    all_done:
        pop esi
        ret
    }
}


// FUNCTION: RALLYTROPHY 0x47f1c0
// SYMBOL: _FUN_0047f1c0
__declspec(naked) void FUN_0047f1c0()
{
    __asm {
        sub esp, 0x28
        push ebx
        push ebp
        push esi
        push edi
        mov edi, dword ptr [esp+0x48]
        push 0x20
        lea eax, [esp+0x18]
        push edi
        xor ebx, ebx
        push eax
        mov dword ptr [esp+0x1c], ebx
        xor ebp, ebp
        call strncpy
        or ecx, 0xffffffff
        xor eax, eax
        add esp, 0xc
        repne scasb
        not ecx
        dec ecx
        cmp ecx, 0x48
        mov byte ptr [esp+0x34], bl
        jae bad_hash
        push offset DAT_004fe54c
        call FUN_00480780
        add esp, 4
        mov dword ptr [esp+0x10], eax
    bad_hash:
        mov edi, dword ptr [DAT_0056460c]
        cmp edi, ebx
        je no_duplicate
    duplicate_loop:
        mov eax, dword ptr [edi]
        add eax, 4
        lea esi, [esp+0x14]
    compare_key:
        mov dl, byte ptr [esi]
        mov cl, dl
        cmp dl, byte ptr [eax]
        jne key_diff
        cmp cl, bl
        je key_equal
        mov dl, byte ptr [esi+1]
        mov cl, dl
        cmp dl, byte ptr [eax+1]
        jne key_diff
        add esi, 2
        add eax, 2
        cmp cl, bl
        jne compare_key
    key_equal:
        xor eax, eax
        jmp key_compared
    key_diff:
        sbb eax, eax
        sbb eax, 0xffffffff
    key_compared:
        cmp eax, ebx
        jne next_request
        inc ebp
    next_request:
        mov edi, dword ptr [edi+4]
        cmp edi, ebx
        jne duplicate_loop
        cmp ebp, 1
        jle no_duplicate
        push offset DAT_004fe54c
        call FUN_00480780
        add esp, 4
        mov dword ptr [esp+0x10], eax
    no_duplicate:
        push 0x50
        call malloc
        mov ecx, dword ptr [esp+0x44]
        mov edx, dword ptr [esp+0x54]
        mov ebp, eax
        mov eax, dword ptr [esp+0x40]
        mov dword ptr [ebp+0x2c], ecx
        mov dword ptr [ebp], eax
        mov eax, dword ptr [esp+0x50]
        or ecx, 0xffffffff
        mov dword ptr [ebp+0x40], eax
        mov dword ptr [ebp+0x3c], edx
        xor eax, eax
        mov dword ptr [ebp+0x44], ebx
        mov dword ptr [ebp+0x48], ebx
        lea edi, [esp+0x18]
        repne scasb
        not ecx
        sub edi, ecx
        mov eax, ecx
        shr ecx, 2
        lea edx, [ebp+4]
        mov esi, edi
        mov edi, edx
        rep movsd
        mov ecx, eax
        and ecx, 3
        push 0xc
        rep movsb
        call malloc
        push offset DAT_00564608
        push eax
        mov dword ptr [eax], ebp
        call FUN_0047fb50
        mov eax, dword ptr [esp+0x20]
        add esp, 0x10
        cmp eax, ebx
        je send_auth
        pop edi
        pop esi
        mov dword ptr [ebp+0x38], 2
        mov dword ptr [ebp+0x44], eax
        pop ebp
        pop ebx
        add esp, 0x28
        ret
    send_auth:
        mov ecx, dword ptr [esp+0x48]
        mov edx, dword ptr [esp+0x44]
        push ecx
        push edx
        push ebp
        call FUN_0047f860
        add esp, 0xc
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 0x28
        ret
    }
}


// FUNCTION: RALLYTROPHY 0x47f090
// SYMBOL: _FUN_0047f090
__declspec(naked) void FUN_0047f090()
{
    __asm {
        sub esp, 0x14
        push ebx
        call FUN_00480750
        push 0x11
        push 2
        push 2
        call socket
        cmp eax, 0xffffffff
        mov dword ptr [DAT_004fe590], eax
        jne socket_ok
        or eax, eax
        pop ebx
        add esp, 0x14
        ret
    socket_ok:
        xor ebx, ebx
        push ebx
        lea eax, [esp+0xc]
        push eax
        push ebx
        push ebx
        call FUN_0047faa0
        mov edx, dword ptr [DAT_004fe590]
        add esp, 0x10
        push 0x10
        lea ecx, [esp+0xc]
        push ecx
        push edx
        call bind
        cmp eax, 0xffffffff
        jne bind_ok
        or eax, eax
        pop ebx
        add esp, 0x14
        ret
    bind_ok:
        mov edx, dword ptr [DAT_004fe590]
        lea eax, [esp+4]
        push eax
        lea ecx, [esp+0xc]
        push ecx
        push edx
        mov dword ptr [esp+0x10], 0x10
        call getsockname
        cmp eax, 0xffffffff
        jne local_name_ok
        or eax, eax
        pop ebx
        add esp, 0x14
        ret
    local_name_ok:
        mov ax, word ptr [esp+0xa]
        push ebx
        push offset DAT_005645f8
        push 0x74d6
        push offset DAT_004fe550
        mov word ptr [DAT_005643f4], ax
        call FUN_0047faa0
        mov ecx, dword ptr [esp+0x2c]
        mov dword ptr [DAT_005643ec], ecx
        mov dword ptr [DAT_0056460c], ebx
        mov dword ptr [DAT_00564610], ebx
        mov dword ptr [DAT_00564608], ebx
        call FUN_00480740
        push eax
        call srand
        add esp, 0x14
        mov byte ptr [DAT_005643e0+7], bl
        mov byte ptr [DAT_005643e0], 0x67
        mov byte ptr [DAT_005643e0+1], 0x61
        mov byte ptr [DAT_005643e0+2], 0x6d
        mov byte ptr [DAT_005643e0+3], 0x65
        mov byte ptr [DAT_005643e0+4], 0x73
        mov byte ptr [DAT_005643e0+5], 0x70
        mov byte ptr [DAT_005643e0+6], 0x79
        xor eax, eax
        pop ebx
        add esp, 0x14
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f540
// SYMBOL: _FUN_0047f540
__declspec(naked) void FUN_0047f540()
{
    __asm {
        sub esp, 0x20
        push ebp
        mov ebp, dword ptr [esp+0x28]
        cmp byte ptr [ebp], 0x5c
        push esi
        mov byte ptr [esp+8], 0
        jne no_dispatch
        push edi
        lea edi, [ebp+1]
        push 0x5c
        push edi
        call FUN_004c2130
        mov esi, eax
        sub esi, ebp
        add esp, 8
        cmp esi, 0x20
        jg parse_key
        lea eax, [esi-1]
        push eax
        lea ecx, [esp+0x10]
        push edi
        push ecx
        call strncpy
        add esp, 0xc
        mov byte ptr [esp+esi+0xb], 0
    parse_key:
        mov al, byte ptr [esp+0xc]
        test al, al
        pop edi
        je no_dispatch

        mov esi, offset DAT_004fe5c4
        lea eax, [esp+8]
    compare_uok:
        mov dl, byte ptr [eax]
        mov cl, dl
        cmp dl, byte ptr [esi]
        jne uok_different
        test cl, cl
        je uok_equal
        mov dl, byte ptr [eax+1]
        mov cl, dl
        cmp dl, byte ptr [esi+1]
        jne uok_different
        add eax, 2
        add esi, 2
        test cl, cl
        jne compare_uok
    uok_equal:
        xor eax, eax
        jmp uok_compared
    uok_different:
        sbb eax, eax
        sbb eax, 0xffffffff
    uok_compared:
        test eax, eax
        jne compare_unok_start
        push 1
        push ebp
        call FUN_0047f670
        add esp, 8
        pop esi
        pop ebp
        add esp, 0x20
        ret

    compare_unok_start:
        mov esi, offset DAT_004fe5bc
        lea eax, [esp+8]
    compare_unok:
        mov dl, byte ptr [eax]
        mov cl, dl
        cmp dl, byte ptr [esi]
        jne unok_different
        test cl, cl
        je unok_equal
        mov dl, byte ptr [eax+1]
        mov cl, dl
        cmp dl, byte ptr [esi+1]
        jne unok_different
        add eax, 2
        add esi, 2
        test cl, cl
        jne compare_unok
    unok_equal:
        xor eax, eax
        jmp unok_compared
    unok_different:
        sbb eax, eax
        sbb eax, 0xffffffff
    unok_compared:
        test eax, eax
        jne compare_ison_start
        push eax
        push ebp
        call FUN_0047f670
        add esp, 8
        pop esi
        pop ebp
        add esp, 0x20
        ret

    compare_ison_start:
        mov esi, offset DAT_004fe5b4
        lea eax, [esp+8]
    compare_ison:
        mov dl, byte ptr [eax]
        mov cl, dl
        cmp dl, byte ptr [esi]
        jne ison_different
        test cl, cl
        je ison_equal
        mov dl, byte ptr [eax+1]
        mov cl, dl
        cmp dl, byte ptr [esi+1]
        jne ison_different
        add eax, 2
        add esi, 2
        test cl, cl
        jne compare_ison
    ison_equal:
        xor eax, eax
        jmp ison_compared
    ison_different:
        sbb eax, eax
        sbb eax, 0xffffffff
    ison_compared:
        test eax, eax
        jne no_dispatch
        mov eax, dword ptr [esp+0x30]
        push eax
        push ebp
        call FUN_0047f770
        add esp, 8
    no_dispatch:
        pop esi
        pop ebp
        add esp, 0x20
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47f3b0
// SYMBOL: _FUN_0047f3b0
extern "C" {
__declspec(naked) void FUN_0047f3b0()
{
    __asm {
        sub esp, 0x120
        mov eax, dword ptr [DAT_004fe590]
        push ebx
        push ebp
        xor ebp, ebp
        mov dword ptr [esp+8], 0x10
        mov dword ptr [esp+0xc], ebp
        mov dword ptr [esp+0x10], ebp
        mov dword ptr [esp+0x28], eax
        mov dword ptr [esp+0x24], 1
    select_loop:
        lea ecx, [esp+0xc]
        push ecx
        push ebp
        push ebp
        lea edx, [esp+0x30]
        push edx
        push 0x40
        call select
        cmp eax, 0xffffffff
        je list_start
        cmp eax, ebp
        je list_start
        mov edx, dword ptr [DAT_004fe590]
        lea eax, [esp+8]
        push eax
        lea ecx, [esp+0x18]
        push ecx
        push ebp
        push 0x3ff
        push offset DAT_00563fe0
        push edx
        call recvfrom
        cmp eax, 0xffffffff
        je select_loop
        push eax
        push offset DAT_00563fe0
        mov byte ptr [eax+DAT_00563fe0], 0
        call FUN_0047fa60
        lea eax, [esp+0x1c]
        push eax
        push offset DAT_00563fe0
        call FUN_0047f540
        add esp, 0x10
        jmp select_loop
    list_start:
        mov ebx, dword ptr [DAT_0056460c]
        cmp ebx, ebp
        je list_done
        push esi
        push edi
    request_loop:
        mov esi, dword ptr [ebx]
        mov edi, dword ptr [esi+0x38]
        mov eax, edi
        sub eax, ebp
        je wait_request
        dec eax
        je report_request
        dec eax
        jne next_request
        mov ebp, ebx
        mov ebx, dword ptr [ebx+8]
        push offset DAT_00564608
        push ebp
        call FUN_0047fb80
        mov esi, dword ptr [ebp]
        mov edi, dword ptr [esi+0x44]
        add esp, 8
        test edi, edi
        mov eax, offset DAT_005070b0
        je no_error_message
        mov eax, edi
    no_error_message:
        mov ecx, dword ptr [esi+0x3c]
        mov edx, dword ptr [esi]
        push ecx
        push eax
        push 0
        push edx
        call dword ptr [esi+0x40]
        mov eax, dword ptr [esi+0x48]
        push eax
        call FUN_004c202e
        add esp, 0x14
        test edi, edi
        je free_request
        push edi
        call FUN_004c202e
        add esp, 4
    free_request:
        push esi
        call FUN_004c202e
        push ebp
        call FUN_004c202e
        add esp, 8
        xor ebp, ebp
        jmp next_request
    wait_request:
        call FUN_00480740
        mov ecx, dword ptr [esi+0x30]
        add ecx, 0x1388
        cmp eax, ecx
        jb next_request
        cmp dword ptr [esi+0x34], 3
        jg report_request
        push esi
        call FUN_0047f920
        add esp, 4
        jmp next_request
    report_request:
        cmp edi, 1
        mov eax, offset DAT_004fe5a8
        je report_callback
        mov eax, offset DAT_004fe594
    report_callback:
        mov edx, dword ptr [esi+0x3c]
        push edx
        push eax
        mov eax, dword ptr [esi]
        push 1
        push eax
        call dword ptr [esi+0x40]
        mov dword ptr [esi+0x38], 3
        mov ecx, dword ptr [esi+0x48]
        push ecx
        call FUN_004c202e
        add esp, 0x14
        mov dword ptr [esi+0x48], ebp
    next_request:
        mov ebx, dword ptr [ebx+4]
        cmp ebx, ebp
        jne request_loop
        pop edi
        pop esi
    list_done:
        pop ebp
        pop ebx
        add esp, 0x120
        ret
    }
}
}
