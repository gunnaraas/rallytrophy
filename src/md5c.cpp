// GameSpy md5c.c routines reconstructed from the shipped executable for byte matching.
#include "rallytrophy.h"
#include <stdio.h>

extern "C" {
void FUN_0047fbe0(void*, const unsigned char*, unsigned int);
void FUN_00480690(void*, int, unsigned int);
void FUN_0047fc90(unsigned char*, void*);
void FUN_004806c0(const unsigned char*, char*);
void FUN_0047fd00(unsigned int*, const unsigned char*);
// FUNCTION: RALLYTROPHY 0x47fbb0
// SYMBOL: _FUN_0047fbb0
__declspec(naked) void FUN_0047fbb0(void* ctx)
{
    __asm {
        mov eax, dword ptr [esp+4]
        xor ecx, ecx
        mov dword ptr [eax+0x14], ecx
        mov dword ptr [eax+0x10], ecx
        mov dword ptr [eax], 0x67452301
        mov dword ptr [eax+4], 0xefcdab89
        mov dword ptr [eax+8], 0x98badcfe
        mov dword ptr [eax+0xc], 0x10325476
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x480670
// SYMBOL: _FUN_00480670
__declspec(naked) void FUN_00480670(void* dst, const void* src, unsigned int len)
{
    __asm {
        mov ecx, dword ptr [esp+0xc]
        push esi
        mov esi, dword ptr [esp+0xc]
        mov eax, ecx
        push edi
        mov edi, dword ptr [esp+0xc]
        shr ecx, 2
        rep movsd
        mov ecx, eax
        and ecx, 3
        rep movsb
        pop edi
        pop esi
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x480690
// SYMBOL: _FUN_00480690
__declspec(naked) void FUN_00480690(void* dst, int value, unsigned int len)
{
    __asm {
        mov al, byte ptr [esp+8]
        mov ecx, dword ptr [esp+0xc]
        push ebx
        mov bl, al
        mov bh, bl
        mov edx, ecx
        push edi
        mov edi, dword ptr [esp+0xc]
        shr ecx, 2
        mov eax, ebx
        shl eax, 0x10
        mov ax, bx
        rep stosd
        mov ecx, edx
        and ecx, 3
        rep stosb
        pop edi
        pop ebx
        ret
    }
}


// FUNCTION: RALLYTROPHY 0x4805d0
// SYMBOL: _FUN_004805d0
__declspec(naked) void FUN_004805d0(unsigned char* output, const unsigned int* input, unsigned int len)
{
    __asm {
        push ebp
        mov ebp, dword ptr [esp+0x10]
        test ebp, ebp
        jbe done
        mov ecx, dword ptr [esp+8]
        push esi
        mov esi, dword ptr [esp+0x10]
        push edi
        or edi, 0xffffffff
        lea eax, [ecx+1]
        sub edi, ecx
    encode_loop:
        mov cl, byte ptr [esi]
        mov byte ptr [eax-1], cl
        mov ecx, dword ptr [esi]
        mov edx, ecx
        shr edx, 8
        mov byte ptr [eax], dl
        mov edx, ecx
        shr edx, 0x10
        shr ecx, 0x18
        mov byte ptr [eax+1], dl
        mov byte ptr [eax+2], cl
        add eax, 4
        lea ecx, [edi+eax]
        add esi, 4
        cmp ecx, ebp
        jb encode_loop
        pop edi
        pop esi
    done:
        pop ebp
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x480620
// SYMBOL: _FUN_00480620
__declspec(naked) void FUN_00480620(unsigned int* output, const unsigned char* input, unsigned int len)
{
    __asm {
        push ebp
        mov ebp, dword ptr [esp+0x10]
        test ebp, ebp
        jbe done
        mov ecx, dword ptr [esp+0xc]
        push esi
        mov esi, dword ptr [esp+0xc]
        push edi
        mov edi, 0xfffffffe
        lea eax, [ecx+2]
        sub edi, ecx
    decode_loop:
        xor ecx, ecx
        mov ch, byte ptr [eax+1]
        xor edx, edx
        mov dl, byte ptr [eax-1]
        add eax, 4
        add esi, 4
        mov cl, byte ptr [eax-4]
        shl ecx, 8
        or ecx, edx
        xor edx, edx
        mov dl, byte ptr [eax-6]
        shl ecx, 8
        or ecx, edx
        mov dword ptr [esi-4], ecx
        lea ecx, [edi+eax]
        cmp ecx, ebp
        jb decode_loop
        pop edi
        pop esi
    done:
        pop ebp
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x4806f0
// SYMBOL: _FUN_004806f0
void FUN_004806f0(const unsigned char* input, unsigned int length, char* output)
{
    unsigned char context[0x58];
    unsigned char digest[16];
    FUN_0047fbb0(context);
    FUN_0047fbe0(context, input, length);
    FUN_0047fc90(digest, context);
    FUN_004806c0(digest, output);
}
}

// FUNCTION: RALLYTROPHY 0x4806c0
// SYMBOL: _FUN_004806c0
__declspec(naked) void FUN_004806c0()
{
    __asm {
        push ebx
        mov ebx, dword ptr [esp+8]
        push esi
        push edi
        mov edi, dword ptr [esp+0x14]
        xor esi, esi
    hex_loop:
        xor eax, eax
        mov al, byte ptr [esi+ebx]
        push eax
        push 04fe698h
        push edi
        call sprintf
        add esp, 0xc
        inc esi
        add edi, 2
        cmp esi, 0x10
        jb hex_loop
        pop edi
        pop esi
        pop ebx
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x47fc90
// SYMBOL: _FUN_0047fc90
__declspec(naked) void FUN_0047fc90(unsigned char* digest, void* context)
{
    __asm {
        sub esp, 8
        push esi
        push edi
        mov edi, dword ptr [esp+0x18]
        push 8
        lea esi, [edi+0x10]
        lea eax, [esp+0xc]
        push esi
        push eax
        call FUN_004805d0
        mov eax, dword ptr [esi]
        shr eax, 3
        and eax, 0x3f
        add esp, 0xc
        cmp eax, 0x38
        mov ecx, 0x38
        jb short pad_length_ready
        mov ecx, 0x78
    pad_length_ready:
        sub ecx, eax
        push ecx
        push 04fe658h
        push edi
        call FUN_0047fbe0
        push 8
        lea ecx, [esp+0x18]
        push ecx
        push edi
        call FUN_0047fbe0
        mov edx, dword ptr [esp+0x2c]
        push 0x10
        push edi
        push edx
        call FUN_004805d0
        push 0x58
        push 0
        push edi
        call FUN_00480690
        add esp, 0x30
        pop edi
        pop esi
        add esp, 8
        ret
    }
}

struct MD5Context {
    unsigned int state[4];
    unsigned int count[2];
    unsigned char buffer[64];
};

// FUNCTION: RALLYTROPHY 0x47fbe0
// SYMBOL: _FUN_0047fbe0
void FUN_0047fbe0(MD5Context* context, const unsigned char* input, unsigned int input_length)
{
    unsigned int index = (context->count[0] >> 3) & 0x3f;
    unsigned int part_length;
    unsigned int i;
    context->count[0] += input_length << 3;
    if (context->count[0] < (input_length << 3))
        ++context->count[1];
    context->count[1] += input_length >> 29;
    part_length = 64 - index;
    if (input_length >= part_length) {
        FUN_00480670(&context->buffer[index], input, part_length);
        FUN_0047fd00(context->state, context->buffer);
        for (i = part_length; i + 63 < input_length; i += 64)
            FUN_0047fd00(context->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }
    FUN_00480670(&context->buffer[index], &input[i], input_length - i);

}
