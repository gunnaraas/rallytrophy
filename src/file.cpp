// Buffered disk/archive file access.
#define RT_FILE_TYPED_LAYOUT
#include "rallytrophy.h"
#undef RT_FILE_TYPED_LAYOUT
#include <fstream.h>
#include <sys/stat.h>

extern "C" {
    char DAT_004de6ae, DAT_00502110, DAT_00502120;
    void FUN_00404ba0();
    void FUN_00408a80();
    void FUN_00408ba0();
    void FUN_00408fd0();
    void FUN_00409130();
    void FUN_00409260();
}
static void* __cdecl FileRawNew(unsigned int size) { return ::operator new(size); }

extern unsigned int DAT_004e2ce4;
extern "C" {
    char DAT_004de7c6, DAT_004de776, DAT_004e1344, DAT_004e1360, DAT_004e46b8;
    char DAT_004f2e2c, DAT_004f2e40, DAT_004f2e44, DAT_00502148, DAT_0050213c;
    void FUN_00404ba0();
    void FUN_00408fd0();
    void FUN_00409130();
    void FUN_00409260();
    void FUN_00408ba0();
    void FUN_00408a80();
    void FUN_00402700();
    void FUN_00402980();
    void FUN_00402960();
    int FUN_004a5430(); // original: File::OpenArchiveEntry
    void __cdecl CxxThrowException(void*, void*);
}

extern unsigned int DAT_0056bbb8;
extern unsigned int DAT_00501d10[256];
extern unsigned int DAT_004e1340;
extern char DAT_00501ce4[];
extern "C" void FUN_004bfd2f();
extern "C" void FUN_004c02d5();
extern "C" void FUN_004c031d();
extern "C" void __cdecl FUN_004c14e0(void*, const void*, unsigned int);
extern "C" int inflate(void* state, int flush);
extern "C" int inflateEnd(void* state);
extern void __fastcall FUN_004a6090(void* file);
extern void __fastcall FUN_004a4e60(void* file);

static void __fastcall FileClose(File* file, void*)
{
    file->Close();
}

static int __fastcall FileRead(File* file, int, void* buffer, int count)
{
    return file->Read(buffer, count);
}

static int __fastcall FileGetSize(File* file, void*)
{
    return file->GetSize();
}

struct FileNameStringOps {
    // VC6 string operations called by FUN_004a4e60.
    void Tidy(char built);                    // original: FUN_00402700
    char Grow(unsigned int length, char trim); // original: FUN_00402980
    void Eos(unsigned int length);            // original: FUN_00402960
};

static void ReadStreamBytes(void* stream, void* buffer, int byteCount)
{
    __asm {
        push byteCount
        push buffer
        mov ecx, stream
        call FUN_004c02d5
    }
}

static fstream* OpenFileStream(const char* name, int mode)
{
    fstream* stream = (fstream*)operator new(100);
    if (stream) {
        __asm {
            push 1
            push DAT_004e2ce4
            push mode
            push name
            mov ecx, stream
            call FUN_004bfd2f
        }
    }
    return stream;
}

// FUNCTION: RALLYTROPHY 0x4a4d40
// SYMBOL: ??0File@@QAE@XZ
File::File()
{
    FUN_004a4e60(this);
}

// FUNCTION: RALLYTROPHY 0x4a4e00
// SYMBOL: ??0File@@QAE@PBDH@Z
File::File(const char* name, int mode)
{
    FUN_004a4e60(this);
    Open(name, mode);
}

// FUNCTION: RALLYTROPHY 0x4a4d90
// SYMBOL: ??1File@@QAE@XZ
File::~File()
{
    Close();
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a4f50
// original: FUN_004a4f50
// Mirrors the observed 1247-byte file open, archive fallback, and signature check.
int File::Open(const char*, int)
{
    __asm {
        push   0xffffffff
        push   offset DAT_004de6ae
        mov    eax,fs:[0]
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,0x1f0
        push   ebx
        mov    ebx,dword ptr [esp+0x204]
        push   ebp
        mov    ebp,ecx
        push   esi
        push   edi
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,ebx
        repne scasb
        not    ecx
        dec    ecx
        mov    edi,ecx
        push   0x1
        lea    esi,[ebp+0x4]
        push   edi
        mov    ecx,esi
        mov    dword ptr [esp+0x1c],edi
        call FUN_00402980
        test   al,al
        je fileopen_4a4fbd
        mov    eax,dword ptr [esi+0x4]
        mov    ecx,edi
        mov    edi,eax
        mov    eax,ecx
        shr    ecx,0x2
        mov    esi,ebx
        rep movsd
        mov    ecx,eax
        and    ecx,0x3
        rep movsb
        mov    ecx,eax
        push   ecx
        lea    ecx,[ebp+0x4]
        call FUN_00402960
        fileopen_4a4fbd:
        mov    eax,dword ptr [esp+0x214]
        test   ah,0x1
        je fileopen_4a4fcd
        mov    byte ptr [ebp+0x16],0x1
        fileopen_4a4fcd:
        xor    esi,esi
        push   esi
        push   eax
        push   ebx
        mov    ecx,ebp
        mov    byte ptr [ebp+0x14],0x0
        call FUN_004a5430
        test   eax,eax
        jne fileopen_4a5252
        push   0x64
        call FileRawNew
        add    esp,0x4
        mov    dword ptr [esp+0x14],eax
        cmp    eax,esi
        mov    dword ptr [esp+0x208],esi
        je fileopen_4a5019
        mov    edx,dword ptr [DAT_004e2ce4]
        mov    ecx,dword ptr [esp+0x214]
        push   0x1
        push   edx
        push   ecx
        push   ebx
        mov    ecx,eax
        call FUN_004bfd2f
        jmp fileopen_4a501b
        fileopen_4a5019:
        xor    eax,eax
        fileopen_4a501b:
        cmp    eax,esi
        mov    dword ptr [esp+0x208],0xffffffff
        mov    dword ptr [ebp+0x0],eax
        je fileopen_4a507c
        mov    edx,dword ptr [eax]
        mov    ecx,dword ptr [edx+0x4]
        test   byte ptr [ecx+eax*1+0x8],0x6
        jne fileopen_4a507c
        test   byte ptr [esp+0x214],0x2
        je fileopen_4a5066
        mov    dword ptr [ebp+0x18],esi
        mov    dword ptr [ebp+0x24],esi
        fileopen_4a5049:
        mov    ecx,dword ptr [esp+0x200]
        pop    edi
        pop    esi
        pop    ebp
        mov    al,0x1
        pop    ebx
        mov    dword ptr fs:[0],ecx
        add    esp,0x1fc
        ret    0x8
        fileopen_4a5066:
        mov    ecx,ebp
        call FileGetSize
        mov    ecx,ebp
        mov    dword ptr [ebp+0x18],eax
        call FileGetSize
        mov    dword ptr [ebp+0x24],eax
        jmp fileopen_4a5049
        fileopen_4a507c:
        mov    ecx,ebp
        call FileClose
        mov    dl,byte ptr [esp+0x13]
        push   esi
        lea    ecx,[esp+0x4c]
        mov    byte ptr [esp+0x4c],dl
        call FUN_00402700
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,offset DAT_00502120
        repne scasb
        not    ecx
        dec    ecx
        mov    ebp,ecx
        push   0x1
        push   ebp
        lea    ecx,[esp+0x50]
        call FUN_00402980
        test   al,al
        je fileopen_4a50d9
        mov    edi,dword ptr [esp+0x4c]
        mov    ecx,ebp
        mov    eax,ecx
        shr    ecx,0x2
        mov    esi,offset DAT_00502120
        rep movsd
        mov    ecx,eax
        and    ecx,0x3
        rep movsb
        push   ebp
        lea    ecx,[esp+0x4c]
        call FUN_00402960
        fileopen_4a50d9:
        mov    cl,byte ptr [esp+0x13]
        mov    byte ptr [esp+0x38],cl
        push   0x0
        lea    ecx,[esp+0x3c]
        mov    dword ptr [esp+0x20c],0x1
        call FUN_00402700
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,ebx
        repne scasb
        not    ecx
        dec    ecx
        mov    ebp,ecx
        push   0x1
        push   ebp
        lea    ecx,[esp+0x40]
        call FUN_00402980
        test   al,al
        je fileopen_4a5135
        mov    edi,dword ptr [esp+0x3c]
        mov    ecx,ebp
        mov    edx,ecx
        shr    ecx,0x2
        mov    esi,ebx
        rep movsd
        mov    ecx,edx
        and    ecx,0x3
        rep movsb
        push   ebp
        lea    ecx,[esp+0x3c]
        call FUN_00402960
        fileopen_4a5135:
        lea    eax,[esp+0x48]
        push   eax
        lea    ecx,[esp+0x5c]
        push   offset DAT_004f2e44
        mov    bl,0x2
        push   ecx
        mov    byte ptr [esp+0x214],bl
        call FUN_00408fd0
        push   offset DAT_004f2e40
        push   eax
        lea    edx,[esp+0x2c]
        push   edx
        mov    byte ptr [esp+0x220],0x3
        call FUN_00409130
        lea    ecx,[esp+0x50]
        push   ecx
        push   eax
        lea    edx,[esp+0x48]
        push   edx
        mov    byte ptr [esp+0x22c],0x4
        call FUN_00409260
        add    esp,0x24
        push   eax
        lea    ecx,[esp+0x8c]
        mov    byte ptr [esp+0x20c],0x5
        call FUN_00408ba0
        push   0x1
        lea    ecx,[esp+0x2c]
        mov    byte ptr [esp+0x20c],0x9
        call FUN_00402700
        push   0x1
        lea    ecx,[esp+0x1c]
        mov    byte ptr [esp+0x20c],0x8
        call FUN_00402700
        push   0x1
        lea    ecx,[esp+0x5c]
        mov    byte ptr [esp+0x20c],0x7
        call FUN_00402700
        lea    eax,[esp+0x48]
        push   eax
        lea    ecx,[esp+0xa8]
        call FUN_00408a80
        lea    ecx,[esp+0x38]
        push   ecx
        lea    ecx,[esp+0xb8]
        mov    byte ptr [esp+0x20c],0xa
        call FUN_00408a80
        mov    eax,dword ptr [esp+0x4c]
        test   eax,eax
        mov    byte ptr [esp+0x208],0xb
        mov    dword ptr [esp+0x88],offset DAT_004e1360
        jne fileopen_4a521d
        mov    eax,offset DAT_004e1344
        fileopen_4a521d:
        mov    ecx,dword ptr [esp+0x3c]
        test   ecx,ecx
        jne fileopen_4a522a
        mov    ecx,offset DAT_004e1344
        fileopen_4a522a:
        push   eax
        push   ecx
        push   offset DAT_004f2e2c
        call FUN_00402c90
        add    esp,0xc
        push   offset DAT_004e46b8
        lea    edx,[esp+0x8c]
        push   edx
        mov    byte ptr [esp+0x210],bl
        call CxxThrowException
        fileopen_4a5252:
        mov    eax,[DAT_0056bbb8]
        push   eax
        lea    ecx,[esp+0x104]
        push   ecx
        mov    ecx,ebp
        mov    byte ptr [ebp+0x14],0x1
        call FileRead
        mov    ecx,dword ptr [DAT_0056bbb8]
        mov    edi,offset DAT_00501ce4
        lea    esi,[esp+0x100]
        xor    edx,edx
        repz cmps byte ptr [esi],byte ptr es:[edi]
        je fileopen_4a5049
        mov    al,byte ptr [esp+0x13]
        push   edx
        lea    ecx,[esp+0x1c]
        mov    byte ptr [esp+0x1c],al
        call FUN_00402700
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,offset DAT_00502110
        repne scasb
        not    ecx
        dec    ecx
        mov    ebp,ecx
        push   0x1
        push   ebp
        lea    ecx,[esp+0x20]
        call FUN_00402980
        test   al,al
        je fileopen_4a52dd
        mov    edi,dword ptr [esp+0x1c]
        mov    ecx,ebp
        mov    edx,ecx
        shr    ecx,0x2
        mov    esi,offset DAT_00502110
        rep movsd
        mov    ecx,edx
        and    ecx,0x3
        rep movsb
        push   ebp
        lea    ecx,[esp+0x1c]
        call FUN_00402960
        fileopen_4a52dd:
        mov    al,byte ptr [esp+0x13]
        push   0x0
        lea    ecx,[esp+0x2c]
        mov    dword ptr [esp+0x20c],0xc
        mov    byte ptr [esp+0x2c],al
        call FUN_00402700
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,ebx
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   ebx
        lea    ecx,[esp+0x30]
        call FUN_00404ba0
        lea    ecx,[esp+0x18]
        push   ecx
        lea    edx,[esp+0x7c]
        push   offset DAT_004f2e44
        mov    bl,0xd
        push   edx
        mov    byte ptr [esp+0x214],bl
        call FUN_00408fd0
        push   offset DAT_004f2e40
        push   eax
        lea    eax,[esp+0x7c]
        push   eax
        mov    byte ptr [esp+0x220],0xe
        call FUN_00409130
        lea    ecx,[esp+0x40]
        push   ecx
        push   eax
        lea    edx,[esp+0x78]
        push   edx
        mov    byte ptr [esp+0x22c],0xf
        call FUN_00409260
        add    esp,0x24
        push   eax
        lea    ecx,[esp+0xc8]
        mov    byte ptr [esp+0x20c],0x10
        call FUN_00408ba0
        push   0x1
        lea    ecx,[esp+0x5c]
        mov    byte ptr [esp+0x20c],0x14
        call FUN_00402700
        push   0x1
        lea    ecx,[esp+0x6c]
        mov    byte ptr [esp+0x20c],0x13
        call FUN_00402700
        push   0x1
        lea    ecx,[esp+0x7c]
        mov    byte ptr [esp+0x20c],0x12
        call FUN_00402700
        lea    eax,[esp+0x18]
        push   eax
        lea    ecx,[esp+0xe4]
        call FUN_00408a80
        lea    ecx,[esp+0x28]
        push   ecx
        lea    ecx,[esp+0xf4]
        mov    byte ptr [esp+0x20c],0x15
        call FUN_00408a80
        mov    eax,dword ptr [esp+0x1c]
        test   eax,eax
        mov    byte ptr [esp+0x208],0x16
        mov    dword ptr [esp+0xc4],offset DAT_004e1360
        jne fileopen_4a53fa
        mov    eax,offset DAT_004e1344
        fileopen_4a53fa:
        mov    ecx,dword ptr [esp+0x2c]
        test   ecx,ecx
        jne fileopen_4a5407
        mov    ecx,offset DAT_004e1344
        fileopen_4a5407:
        push   eax
        push   ecx
        push   offset DAT_004f2e2c
        call FUN_00402c90
        add    esp,0xc
        push   offset DAT_004e46b8
        lea    edx,[esp+0xc8]
        push   edx
        mov    byte ptr [esp+0x210],bl
        call CxxThrowException
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a5e00
// original: FUN_004a5e00
// Mirrors the observed x86 loop, including its refill call and rep string copies.
int File::Read(void*, int)
{
    __asm {
        push ecx
        push ebx
        push ebp
        mov ebp, dword ptr [esp + 0x14]
        test ebp, ebp
        push esi
        push edi
        mov edi, dword ptr [esp + 0x18]
        mov esi, ebp
        mov ebx, ecx
        mov dword ptr [esp + 0x10], esi
        mov dword ptr [esp + 0x18], edi
        jle read_done

    read_loop:
        mov edx, dword ptr [ebx + 0x70]
        mov eax, dword ptr [ebx + 0x74]
        sub eax, edx
        test eax, eax
        jg read_window_ready
        mov ecx, ebx
        call FUN_004a6090
        mov eax, dword ptr [ebx + 0x74]
        mov edx, dword ptr [ebx + 0x70]
        sub eax, edx

    read_window_ready:
        cmp eax, esi
        jle read_count_ready
        mov eax, esi

    read_count_ready:
        test eax, eax
        jle read_advance
        mov ecx, eax
        mov ebp, ecx
        shr ecx, 2
        lea esi, [edx + ebx + 0x2078]
        rep movsd
        mov ecx, ebp
        mov ebp, dword ptr [esp + 0x1c]
        and ecx, 3
        rep movsb
        mov edi, dword ptr [esp + 0x18]
        mov esi, dword ptr [esp + 0x10]
        add edi, eax
        mov dword ptr [esp + 0x18], edi

    read_advance:
        sub esi, eax
        add edx, eax
        test esi, esi
        mov dword ptr [ebx + 0x70], edx
        mov dword ptr [esp + 0x10], esi
        jg read_loop

    read_done:
        pop edi
        pop esi
        mov eax, ebp
        pop ebp
        pop ebx
        pop ecx
        ret 8
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a63b0
// original: FUN_004a63b0
// Mirrors the observed stream check and file_error exception construction.
void __fastcall FUN_004a63b0(void*)
{
    __asm {
        mov    eax,fs:[0]
        push   0xffffffff
        push   offset DAT_004de7c6
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,0x90
        push   ebp
        mov    ebp,ecx
        mov    eax,dword ptr [ebp+0x0]
        push   esi
        xor    esi,esi
        cmp    eax,esi
        je read_4a63e8
        mov    ecx,dword ptr [eax]
        mov    edx,dword ptr [ecx+0x4]
        test   byte ptr [edx+eax*1+0x8],0x6
        je read_4a656e
        read_4a63e8:
        mov    al,byte ptr [esp+0xb]
        push   edi
        or     ecx,0xffffffff
        mov    byte ptr [esp+0x10],al
        xor    eax,eax
        mov    edi,offset DAT_00502148
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   offset DAT_00502148
        lea    ecx,[esp+0x18]
        mov    dword ptr [esp+0x1c],esi
        mov    dword ptr [esp+0x20],esi
        mov    dword ptr [esp+0x24],esi
        call FUN_00404ba0
        mov    edx,dword ptr [ebp+0x8]
        cmp    edx,esi
        mov    dword ptr [esp+0xa4],esi
        jne read_4a642e
        mov    edx,offset DAT_004e1344
        read_4a642e:
        mov    cl,byte ptr [esp+0xf]
        mov    byte ptr [esp+0x20],cl
        xor    eax,eax
        mov    edi,edx
        mov    dword ptr [esp+0x24],esi
        mov    dword ptr [esp+0x28],esi
        mov    dword ptr [esp+0x2c],esi
        or     ecx,0xffffffff
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   edx
        lea    ecx,[esp+0x28]
        call FUN_00404ba0
        lea    edx,[esp+0x10]
        push   edx
        lea    eax,[esp+0x44]
        push   offset DAT_004f2e44
        push   eax
        mov    byte ptr [esp+0xb0],0x1
        call FUN_00408fd0
        push   offset DAT_004f2e40
        push   eax
        lea    ecx,[esp+0x64]
        push   ecx
        mov    byte ptr [esp+0xbc],0x2
        call FUN_00409130
        lea    edx,[esp+0x38]
        push   edx
        push   eax
        lea    eax,[esp+0x50]
        push   eax
        mov    byte ptr [esp+0xc8],0x3
        call FUN_00409260
        add    esp,0x24
        push   eax
        lea    ecx,[esp+0x64]
        mov    byte ptr [esp+0xa8],0x4
        call FUN_00408ba0
        push   0x1
        lea    ecx,[esp+0x34]
        mov    byte ptr [esp+0xa8],0x8
        call FUN_00402700
        push   0x1
        lea    ecx,[esp+0x54]
        mov    byte ptr [esp+0xa8],0x7
        call FUN_00402700
        push   0x1
        lea    ecx,[esp+0x44]
        mov    byte ptr [esp+0xa8],0x6
        call FUN_00402700
        lea    ecx,[esp+0x10]
        push   ecx
        lea    ecx,[esp+0x80]
        call FUN_00408a80
        lea    edx,[esp+0x20]
        push   edx
        lea    ecx,[esp+0x90]
        mov    byte ptr [esp+0xa8],0x9
        call FUN_00408a80
        mov    eax,dword ptr [esp+0x14]
        cmp    eax,esi
        mov    byte ptr [esp+0xa4],0xa
        mov    dword ptr [esp+0x60],offset DAT_004e1360
        pop    edi
        jne read_4a653b
        mov    eax,offset DAT_004e1344
        read_4a653b:
        mov    ecx,dword ptr [esp+0x20]
        cmp    ecx,esi
        jne read_4a6548
        mov    ecx,offset DAT_004e1344
        read_4a6548:
        push   eax
        push   ecx
        push   offset DAT_004f2e2c
        call FUN_00402c90
        add    esp,0xc
        push   offset DAT_004e46b8
        lea    eax,[esp+0x60]
        push   eax
        mov    byte ptr [esp+0xa8],0x1
        call CxxThrowException
        read_4a656e:
        mov    ecx,dword ptr [esp+0x98]
        pop    esi
        pop    ebp
        mov    dword ptr fs:[0],ecx
        add    esp,0x9c
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a61d0
// original: FUN_004a61d0
// Mirrors the observed stream check and file_error exception construction.
void __fastcall FUN_004a61d0(void*)
{
    __asm {
        mov    eax,fs:[0]
        push   0xffffffff
        push   offset DAT_004de776
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,0x90
        push   ebp
        mov    ebp,ecx
        mov    eax,dword ptr [ebp+0x0]
        push   esi
        xor    esi,esi
        cmp    eax,esi
        je write_4a6208
        mov    ecx,dword ptr [eax]
        mov    edx,dword ptr [ecx+0x4]
        test   byte ptr [edx+eax*1+0x8],0x6
        je write_4a638e
        write_4a6208:
        mov    al,byte ptr [esp+0xb]
        push   edi
        or     ecx,0xffffffff
        mov    byte ptr [esp+0x10],al
        xor    eax,eax
        mov    edi,offset DAT_0050213c
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   offset DAT_0050213c
        lea    ecx,[esp+0x18]
        mov    dword ptr [esp+0x1c],esi
        mov    dword ptr [esp+0x20],esi
        mov    dword ptr [esp+0x24],esi
        call FUN_00404ba0
        mov    edx,dword ptr [ebp+0x8]
        cmp    edx,esi
        mov    dword ptr [esp+0xa4],esi
        jne write_4a624e
        mov    edx,offset DAT_004e1344
        write_4a624e:
        mov    cl,byte ptr [esp+0xf]
        mov    byte ptr [esp+0x20],cl
        xor    eax,eax
        mov    edi,edx
        mov    dword ptr [esp+0x24],esi
        mov    dword ptr [esp+0x28],esi
        mov    dword ptr [esp+0x2c],esi
        or     ecx,0xffffffff
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   edx
        lea    ecx,[esp+0x28]
        call FUN_00404ba0
        lea    edx,[esp+0x10]
        push   edx
        lea    eax,[esp+0x44]
        push   offset DAT_004f2e44
        push   eax
        mov    byte ptr [esp+0xb0],0x1
        call FUN_00408fd0
        push   offset DAT_004f2e40
        push   eax
        lea    ecx,[esp+0x64]
        push   ecx
        mov    byte ptr [esp+0xbc],0x2
        call FUN_00409130
        lea    edx,[esp+0x38]
        push   edx
        push   eax
        lea    eax,[esp+0x50]
        push   eax
        mov    byte ptr [esp+0xc8],0x3
        call FUN_00409260
        add    esp,0x24
        push   eax
        lea    ecx,[esp+0x64]
        mov    byte ptr [esp+0xa8],0x4
        call FUN_00408ba0
        push   0x1
        lea    ecx,[esp+0x34]
        mov    byte ptr [esp+0xa8],0x8
        call FUN_00402700
        push   0x1
        lea    ecx,[esp+0x54]
        mov    byte ptr [esp+0xa8],0x7
        call FUN_00402700
        push   0x1
        lea    ecx,[esp+0x44]
        mov    byte ptr [esp+0xa8],0x6
        call FUN_00402700
        lea    ecx,[esp+0x10]
        push   ecx
        lea    ecx,[esp+0x80]
        call FUN_00408a80
        lea    edx,[esp+0x20]
        push   edx
        lea    ecx,[esp+0x90]
        mov    byte ptr [esp+0xa8],0x9
        call FUN_00408a80
        mov    eax,dword ptr [esp+0x14]
        cmp    eax,esi
        mov    byte ptr [esp+0xa4],0xa
        mov    dword ptr [esp+0x60],offset DAT_004e1360
        pop    edi
        jne write_4a635b
        mov    eax,offset DAT_004e1344
        write_4a635b:
        mov    ecx,dword ptr [esp+0x20]
        cmp    ecx,esi
        jne write_4a6368
        mov    ecx,offset DAT_004e1344
        write_4a6368:
        push   eax
        push   ecx
        push   offset DAT_004f2e2c
        call FUN_00402c90
        add    esp,0xc
        push   offset DAT_004e46b8
        lea    eax,[esp+0x60]
        push   eax
        mov    byte ptr [esp+0xa8],0x1
        call CxxThrowException
        write_4a638e:
        mov    ecx,dword ptr [esp+0x98]
        pop    esi
        pop    ebp
        mov    dword ptr fs:[0],ecx
        add    esp,0x9c
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a6090
// original: FUN_004a6090
// Mirrors the observed 308-byte plain/archive refill and decryption routine.
void __fastcall FUN_004a6090(void*)
{
    __asm {
        push ebx
        push ebp
        push esi
        mov esi, ecx
        mov al, byte ptr [esi + 0x15]
        test al, al
        push edi
        jne refill_archive
        mov edi, dword ptr [esi + 0x24]
        sub edi, dword ptr [esi + 0x20]
        cmp edi, 0x2000
        jle refill_plain_count_ready
        mov edi, 0x2000
    refill_plain_count_ready:
        mov ecx, dword ptr [esi]
        push edi
        lea ebp, [esi + 0x2078]
        push ebp
        call FUN_004c02d5
        mov ecx, dword ptr [esi + 0x1c]
        mov ebx, dword ptr [esi + 0x20]
        add ecx, edi
        lea eax, [ebx + edi]
        mov dword ptr [esi + 0x1c], ecx
        mov ecx, esi
        mov dword ptr [esi + 0x70], 0
        mov dword ptr [esi + 0x74], edi
        mov dword ptr [esi + 0x20], eax
        call FUN_004a63b0
        mov al, byte ptr [esi + 0x14]
        test al, al
        je refill_done
        mov eax, dword ptr [esi + 0x74]
        xor ecx, ecx
        test eax, eax
        jle refill_done
        mov edi, ebx
        mov eax, ebp
        sub edi, ebp
    refill_plain_decrypt:
        mov bl, byte ptr [eax]
        lea edx, [edi + eax]
        and edx, 0xff
        mov dl, byte ptr [edx*4 + DAT_00501d10]
        xor bl, dl
        mov byte ptr [eax], bl
        mov edx, dword ptr [esi + 0x74]
        inc ecx
        inc eax
        cmp ecx, edx
        jl refill_plain_decrypt
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret
    refill_archive:
        lea eax, [esi + 0x2078]
        mov dword ptr [esi + 0x38], eax
        mov eax, dword ptr [esi + 0x30]
        test eax, eax
        mov dword ptr [esi + 0x3c], 0x2000
        ja refill_inflate
        mov edi, dword ptr [esi + 0x24]
        sub edi, dword ptr [esi + 0x20]
        cmp edi, 0x2000
        jle refill_archive_count_ready
        mov edi, 0x2000
    refill_archive_count_ready:
        mov ecx, dword ptr [esi]
        push edi
        lea ebp, [esi + 0x78]
        push ebp
        call FUN_004c02d5
        mov ecx, dword ptr [esi + 0x20]
        mov al, byte ptr [esi + 0x14]
        test al, al
        lea edx, [ecx + edi]
        mov dword ptr [esi + 0x2c], ebp
        mov dword ptr [esi + 0x30], edi
        mov dword ptr [esi + 0x20], edx
        je refill_inflate
        test edi, edi
        jle refill_inflate
        mov eax, ebp
        sub ecx, ebp
    refill_archive_decrypt:
        mov bl, byte ptr [eax]
        lea edx, [ecx + eax]
        and edx, 0xff
        mov dl, byte ptr [edx*4 + DAT_00501d10]
        xor bl, dl
        mov byte ptr [eax], bl
        inc eax
        dec edi
        jne refill_archive_decrypt
    refill_inflate:
        lea eax, [esi + 0x2c]
        push 1
        push eax
        call inflate
        mov edx, dword ptr [esi + 0x3c]
        mov ecx, dword ptr [esi + 0x1c]
        mov eax, 0x2000
        sub eax, edx
        add esp, 8
        add ecx, eax
        mov dword ptr [esi + 0x70], 0
        mov dword ptr [esi + 0x74], eax
        mov dword ptr [esi + 0x1c], ecx
    refill_done:
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret
    }
}

static void __fastcall CloseFileStream(fstream* stream, int)
{
    stream->close();
}

static void __fastcall TidyFileName(FileNameStringOps* name, int, char built)
{
    name->Tidy(built);
}

static char __fastcall GrowFileName(FileNameStringOps* name, int, unsigned int length, char trim)
{
    return name->Grow(length, trim);
}

static void __fastcall SetFileNameEnd(FileNameStringOps* name, int, unsigned int length)
{
    name->Eos(length);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a5d00
// original: FUN_004a5d00
// Mirrors the observed 241-byte x86 close/reset body.
void File::Close()
{
    __asm {
        sub esp, 8
        push ebx
        push ebp
        push esi
        push edi
        mov edi, ecx
        mov esi, dword ptr [edi]
        test esi, esi
        mov dword ptr [esp + 0x10], edi
        je close_stream_done
        mov ecx, esi
        call CloseFileStream
        test esi, esi
        je close_stream_delete_done
        mov eax, dword ptr [esi]
        mov ecx, dword ptr [eax + 4]
        mov edx, dword ptr [ecx + esi]
        add ecx, esi
        push 1
        call dword ptr [edx]
    close_stream_delete_done:
        mov dword ptr [edi], 0
    close_stream_done:
        mov al, byte ptr [edi + 0x15]
        test al, al
        je close_inflate_done
        lea eax, [edi + 0x2c]
        push eax
        call inflateEnd
        add esp, 4
    close_inflate_done:
        mov esi, dword ptr [edi + 8]
        test esi, esi
        mov ebp, dword ptr [DAT_004e1340]
        lea ebx, [edi + 4]
        je close_trim
        mov al, byte ptr [esi - 1]
        test al, al
        je close_trim
        cmp al, 0xff
        je close_trim
        push 1
        mov ecx, ebx
        call TidyFileName
        or ecx, -1
        xor eax, eax
        mov edi, esi
        repne scasb
        not ecx
        dec ecx
        mov edi, ecx
        push 1
        push edi
        mov ecx, ebx
        mov dword ptr [esp + 0x1c], edi
        call GrowFileName
        test al, al
        je close_tidy_done
        mov ecx, edi
        mov edi, dword ptr [ebx + 4]
        mov edx, ecx
        shr ecx, 2
        rep movsd
        mov ecx, edx
        and ecx, 3
        mov eax, edx
        rep movsb
        push eax
        mov ecx, ebx
        call SetFileNameEnd
    close_tidy_done:
        mov edi, dword ptr [esp + 0x10]
    close_trim:
        mov esi, dword ptr [ebx + 8]
        cmp esi, ebp
        jae close_trim_ready
        mov ebp, esi
    close_trim_ready:
        test ebp, ebp
        jbe close_reset
        mov eax, dword ptr [ebx + 4]
        sub esi, ebp
        push esi
        lea ecx, [eax + ebp]
        push ecx
        push eax
        call FUN_004c14e0
        add esp, 0xc
        push 0
        push esi
        mov ecx, ebx
        call GrowFileName
        test al, al
        je close_reset
        mov edx, dword ptr [ebx + 4]
        mov dword ptr [ebx + 8], esi
        mov byte ptr [edx + esi], 0
    close_reset:
        mov dword ptr [edi + 0x18], -1
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 8
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x4a5ff0
// Return the cached size, populating it from the stored filename on first use.
int File::GetSize()
{
    if (*(fstream**)((char*)this) == 0) {
        return -1;
    }
    int& size = *(int*)(((char*)this) + 0x18);
    if (size == -1) {
        struct _stat info;
        const char* name = m_name.c_str();
        if (_stat(name, &info) == 0) {
            size = (int)info.st_size;
        } else {
            size = -1;
        }
    }
    return size;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a5e90
// original: FUN_004a5e90
int File::Write(const void*, int)
{
    __asm {
        mov eax, dword ptr [esp + 4]
        push esi
        push edi
        mov edi, dword ptr [esp + 0x10]
        mov esi, ecx
        mov ecx, dword ptr [esi]
        push edi
        push eax
        add ecx, 0xc
        call FUN_004c031d
        mov ecx, esi
        call FUN_004a61d0
        mov eax, edi
        pop edi
        pop esi
        ret 8
    }
}

// FUNCTION: RALLYTROPHY 0x4a5ec0
char __fastcall FUN_004a5ec0(void* file)
{
    char value;
    ((File*)file)->Read(&value, 1);
    return value;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a5ee0
// original: FUN_004a5ee0
int File::ReadLine(char*, int)
{
    __asm {
        mov eax, dword ptr [esp + 8]
        push ebx
        push ebp
        push esi
        mov esi, dword ptr [esp + 0x10]
        xor bl, bl
        xor ebp, ebp
        test eax, eax
        push edi
        mov edi, ecx
        jbe read_line_done
    read_line_loop:
        mov ecx, edi
        call FUN_004a5ec0
        xor cl, cl
        cmp al, 0xd
        je read_line_delimiter
        cmp al, 0xa
        jne read_line_check_started
    read_line_delimiter:
        mov cl, 1
    read_line_check_started:
        test bl, bl
        je read_line_not_started
        test cl, cl
        jne read_line_terminate
        jmp read_line_store
    read_line_not_started:
        test cl, cl
        jne read_line_advance
    read_line_store:
        mov byte ptr [esi], al
        mov bl, 1
        inc esi
    read_line_advance:
        mov eax, dword ptr [esp + 0x18]
        inc ebp
        cmp ebp, eax
        jb read_line_loop
        pop edi
        pop esi
        pop ebp
        mov eax, 1
        pop ebx
        ret 8
    read_line_terminate:
        mov byte ptr [esi], 0
    read_line_done:
        pop edi
        pop esi
        pop ebp
        mov eax, 1
        pop ebx
        ret 8
        }
}

// FUNCTION: RALLYTROPHY 0x4a6050
// Check the filesystem first, then the game's archive-backed file index.
bool File::Exists(const char* name)
{
    bool found = false;
    struct _stat info;
    if (_stat(name, &info) == 0) {
        found = true;
    } else if (FUN_004a5920(name)) {
        found = true;
    }
    return found;
}
