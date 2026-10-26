// Archive-index cache supporting File's archive-backed lookup.
#include "rallytrophy.h"
#include "file_archive.h"
#include <fstream.h>
#include <sys/stat.h>
#include <string.h>

extern "C" {
    char DAT_004de71e, DAT_004de6e9, DAT_004e1344;
    char DAT_0056b9a1, DAT_0056b9a2, DAT_0056b9a3, DAT_0056b9a4;
    char DAT_0056baa5, DAT_0056baa6, DAT_0056baa7, DAT_0056baa8;
    char DAT_00502134;
    void FUN_00402700();
    void FUN_00402980();
    void FUN_004a4b10();
    void FUN_004bc680();
    void FUN_004bd000();
    void FUN_004bfe39();
    void FUN_004c02d5();
    void FUN_004c285b();
    void FUN_004c0269();
    void FUN_004ce290();
    void FUN_004cf9ea();
}
static void* __cdecl ArchiveRawNew(unsigned int size) { return ::operator new(size); }

extern char DAT_005070b0[];
extern unsigned int DAT_004e2ce4;
extern unsigned int DAT_00501d10[256];
extern "C" void FUN_004bfd2f();
extern "C" int inflateInit_(void* state, const char* version, int stateSize);

char DAT_0056bba8;
unsigned int DAT_0056bbac;
unsigned int DAT_0056bbb0;
unsigned int DAT_0056bbb4;
char DAT_0056b9a0[0x104];
char DAT_0056baa4[0x200];

extern int __cdecl FUN_004c0d01(void (__cdecl *callback)(void));

static void __fastcall DestroyArchiveCache(FileArchiveCache* cache, void*)
{
    cache->~FileArchiveCache();
}

static void __cdecl ReleaseArchiveCache(void* cache)
{
    ::operator delete(cache);
}

static void __stdcall FinishArchiveVectorErase(void*, void*)
{
}

// FUNCTION: RALLYTROPHY 0x4a4c90
void __cdecl FUN_004a4c90(void)
{
    ::operator delete((void*)DAT_0056bbac);
    DAT_0056bbac = 0;
    DAT_0056bbb0 = 0;
    DAT_0056bbb4 = 0;
}

// FUNCTION: RALLYTROPHY 0x4a4c80
// original: FUN_004a4c80
void __stdcall FUN_004a4c80(void)
{
    FUN_004c0d01(FUN_004a4c90);
}

// FUNCTION: RALLYTROPHY 0x4a4cb0
// original: FUN_004a4cb0
void FUN_004a4cb0(void);
__declspec(naked) void FUN_004a4cb0(void)
{
    __asm {
        push ecx
        mov eax, dword ptr [DAT_0056bbac]
        mov ecx, dword ptr [DAT_0056bbb0]
        push esi
        push edi
        xor edi, edi
    cleanup_loop:
        test eax, eax
        jz cleanup_done
        mov edx, ecx
        sub edx, eax
        sar edx, 2
        cmp edi, edx
        jnc cleanup_done
        mov esi, dword ptr [eax + edi * 4]
        test esi, esi
        jz cleanup_next
        mov ecx, esi
        call DestroyArchiveCache
        push esi
        call ReleaseArchiveCache
        mov eax, dword ptr [DAT_0056bbac]
        mov ecx, dword ptr [DAT_0056bbb0]
        add esp, 4
    cleanup_next:
        inc edi
        jmp cleanup_loop
    cleanup_done:
        test eax, eax
        mov dword ptr [esp + 8], 0
        jz cleanup_return
        mov edx, ecx
        sub edx, eax
        sar edx, 2
        jz cleanup_return
        mov esi, eax
        mov eax, ecx
        cmp eax, ecx
        jz cleanup_call_vector
    cleanup_copy:
        mov edx, dword ptr [eax]
        mov dword ptr [esi], edx
        add eax, 4
        add esi, 4
        cmp eax, ecx
        jnz cleanup_copy
    cleanup_call_vector:
        push ecx
        push esi
        mov ecx, offset DAT_0056bba8
        call FinishArchiveVectorErase
        mov dword ptr [DAT_0056bbb0], esi
    cleanup_return:
        pop edi
        pop esi
        pop ecx
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x4a4c60
// original: FUN_004a4c60
void __fastcall FUN_004a4c60(unsigned int value)
{
    (void)value;
    __asm {
        push ecx
        mov al, byte ptr [esp + 3]
        mov byte ptr [DAT_0056bba8], al
        xor eax, eax
        mov dword ptr [DAT_0056bbac], eax
        mov dword ptr [DAT_0056bbb0], eax
        mov dword ptr [DAT_0056bbb4], eax
        pop ecx
    }
}

// FUNCTION: RALLYTROPHY 0x4a4b10
// original: FUN_004a4b10
FileArchiveCache::FileArchiveCache()
{
    m_name = DAT_005070b0;
    m_entryCount = 0;
    m_entries = 0;
}

// FUNCTION: RALLYTROPHY 0x4a4bd0
// original: FUN_004a4bd0
FileArchiveCache::~FileArchiveCache()
{
    delete[] m_entries;
}

static void AppendArchiveCache(FileArchiveCache* cache)
{
    FileArchiveCache** begin = (FileArchiveCache**)DAT_0056bbac;
    FileArchiveCache** end = (FileArchiveCache**)DAT_0056bbb0;
    FileArchiveCache** capacity = (FileArchiveCache**)DAT_0056bbb4;
    unsigned int count = begin ? (unsigned int)(end - begin) : 0;
    unsigned int allocated = begin ? (unsigned int)(capacity - begin) : 0;

    if (count == allocated) {
        unsigned int newCapacity = allocated ? allocated * 2 : 1;
        FileArchiveCache** replacement =
            (FileArchiveCache**)operator new(newCapacity * sizeof(FileArchiveCache*));
        for (unsigned int i = 0; i < count; i++) {
            replacement[i] = begin[i];
        }
        if (begin) {
            ::operator delete(begin);
        }
        begin = replacement;
        DAT_0056bbac = (unsigned int)begin;
        DAT_0056bbb0 = (unsigned int)(begin + count);
        DAT_0056bbb4 = (unsigned int)(begin + newCapacity);
    }
    begin = (FileArchiveCache**)DAT_0056bbac;
    end = (FileArchiveCache**)DAT_0056bbb0;
    *end++ = cache;
    DAT_0056bbb0 = (unsigned int)end;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a5920
// original: FUN_004a5920
// Mirrors the observed 982-byte archive entry lookup, cache load, and file test.
int FUN_004a5920(const char*)
{
    __asm {
        push   0xffffffff
        push   offset DAT_004de71e
        mov    eax,fs:[0]
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,0x3c
        or     ecx,0xffffffff
        push   ebp
        push   esi
        push   edi
        mov    edi,dword ptr [esp+0x58]
        xor    eax,eax
        xor    edx,edx
        repne scasb
        not    ecx
        sub    edi,ecx
        mov    eax,ecx
        shr    ecx,0x2
        mov    esi,edi
        mov    edi,offset DAT_0056b9a0
        rep movsd
        mov    ecx,eax
        and    ecx,0x3
        rep movsb
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,offset DAT_0056b9a0
        repne scasb
        not    ecx
        dec    ecx
        lea    ebp,[ecx-0x1]
        test   ebp,ebp
        mov    dword ptr [esp+0xc],edx
        jle lookup_4a598e
        lookup_4a597b:
        mov    al,byte ptr [ebp+DAT_0056b9a0]
        cmp    al,0x2f
        je lookup_4a59a2
        cmp    al,0x5c
        je lookup_4a59a2
        dec    ebp
        test   ebp,ebp
        jg lookup_4a597b
        lookup_4a598e:
        pop    edi
        pop    esi
        mov    eax,edx
        pop    ebp
        mov    ecx,dword ptr [esp+0x3c]
        mov    dword ptr fs:[0],ecx
        add    esp,0x48
        ret
        lookup_4a59a2:
        test   ebp,ebp
        jle lookup_4a598e
        mov    cl,byte ptr [esp+0x58]
        push   ebx
        mov    byte ptr [esp+0x18],cl
        push   0x0
        lea    ecx,[esp+0x1c]
        call FUN_00402700
        or     ecx,0xffffffff
        lea    esi,[ebp+DAT_0056b9a1]
        xor    eax,eax
        mov    edi,esi
        repne scasb
        not    ecx
        dec    ecx
        mov    ebx,ecx
        push   0x1
        push   ebx
        lea    ecx,[esp+0x20]
        mov    dword ptr [esp+0x5c],0x0
        call FUN_00402980
        test   al,al
        je lookup_4a5a06
        mov    edi,dword ptr [esp+0x1c]
        mov    ecx,ebx
        mov    edx,ecx
        shr    ecx,0x2
        rep movsd
        mov    ecx,edx
        and    ecx,0x3
        rep movsb
        mov    eax,dword ptr [esp+0x1c]
        mov    dword ptr [esp+0x20],ebx
        mov    byte ptr [eax+ebx*1],0x0
        lookup_4a5a06:
        mov    byte ptr [ebp+DAT_0056b9a0],0x2e
        mov    byte ptr [ebp+DAT_0056b9a1],0x62
        mov    byte ptr [ebp+DAT_0056b9a2],0x7a
        mov    byte ptr [ebp+DAT_0056b9a3],0x66
        or     edi,0xffffffff
        mov    byte ptr [ebp+DAT_0056b9a4],0x0
        xor    esi,esi
        lookup_4a5a2e:
        mov    eax,[DAT_0056bbac]
        test   eax,eax
        je lookup_4a5a6c
        mov    ecx,dword ptr [DAT_0056bbb0]
        sub    ecx,eax
        sar    ecx,0x2
        cmp    esi,ecx
        jae lookup_4a5a6c
        mov    eax,dword ptr [eax+esi*4]
        mov    eax,dword ptr [eax+0x4]
        test   eax,eax
        jne lookup_4a5a55
        mov    eax,offset DAT_004e1344
        lookup_4a5a55:
        push   offset DAT_0056b9a0
        push   eax
        call FUN_004ce290
        add    esp,0x8
        test   eax,eax
        jne lookup_4a5a69
        mov    edi,esi
        lookup_4a5a69:
        inc    esi
        jmp lookup_4a5a2e
        lookup_4a5a6c:
        cmp    edi,0xffffffff
        jne lookup_4a5c55
        lea    edx,[esp+0x28]
        push   edx
        push   offset DAT_0056b9a0
        call FUN_004c285b
        add    esp,0x8
        test   eax,eax
        jne lookup_4a5ca6
        push   0x64
        call ArchiveRawNew
        add    esp,0x4
        mov    dword ptr [esp+0x5c],eax
        test   eax,eax
        mov    byte ptr [esp+0x54],0x1
        je lookup_4a5ac4
        mov    ecx,dword ptr [DAT_004e2ce4]
        push   0x1
        push   ecx
        push   0x81
        push   offset DAT_0056b9a0
        mov    ecx,eax
        call FUN_004bfd2f
        mov    ebp,eax
        jmp lookup_4a5ac6
        lookup_4a5ac4:
        xor    ebp,ebp
        lookup_4a5ac6:
        mov    edx,dword ptr [ebp+0x0]
        mov    eax,dword ptr [edx+0x4]
        test   byte ptr [eax+ebp*1+0x8],0x6
        mov    byte ptr [esp+0x54],0x0
        je lookup_4a5afe
        mov    ecx,ebp
        call FUN_004bfe39
        test   ebp,ebp
        je lookup_4a5ca6
        mov    ecx,dword ptr [ebp+0x0]
        mov    edx,dword ptr [ecx+0x4]
        mov    eax,dword ptr [ebp+edx*1+0x0]
        add    ebp,edx
        push   0x1
        mov    ecx,ebp
        call   dword ptr [eax]
        jmp lookup_4a5ca6
        lookup_4a5afe:
        push   0x20
        call ArchiveRawNew
        add    esp,0x4
        mov    dword ptr [esp+0x14],eax
        test   eax,eax
        mov    byte ptr [esp+0x54],0x2
        je lookup_4a5b24
        mov    ecx,eax
        call FUN_004a4b10
        mov    esi,eax
        mov    dword ptr [esp+0x14],esi
        jmp lookup_4a5b30
        lookup_4a5b24:
        mov    dword ptr [esp+0x14],0x0
        mov    esi,dword ptr [esp+0x14]
        lookup_4a5b30:
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,offset DAT_0056b9a0
        repne scasb
        not    ecx
        dec    ecx
        mov    ebx,ecx
        push   0x1
        push   ebx
        mov    ecx,esi
        mov    byte ptr [esp+0x5c],0x0
        mov    dword ptr [esp+0x64],esi
        call FUN_00402980
        test   al,al
        je lookup_4a5b7d
        mov    eax,dword ptr [esi+0x4]
        mov    ecx,ebx
        mov    edx,ecx
        shr    ecx,0x2
        mov    esi,offset DAT_0056b9a0
        mov    edi,eax
        rep movsd
        mov    ecx,edx
        and    ecx,0x3
        rep movsb
        mov    ecx,dword ptr [esp+0x14]
        mov    dword ptr [ecx+0x8],ebx
        mov    byte ptr [eax+ebx*1],0x0
        lookup_4a5b7d:
        mov    edx,dword ptr [esp+0x5c]
        push   0xc
        add    edx,0x10
        push   edx
        mov    ecx,ebp
        call FUN_004c02d5
        mov    eax,dword ptr [esp+0x5c]
        mov    eax,dword ptr [eax+0x18]
        lea    ecx,[eax+eax*2]
        lea    ecx,[ecx+ecx*8]
        shl    ecx,1
        sub    ecx,eax
        push   ecx
        call ArchiveRawNew
        mov    edx,dword ptr [esp+0x60]
        mov    dword ptr [edx+0x1c],eax
        mov    edx,dword ptr [esp+0x60]
        mov    eax,dword ptr [edx+0x18]
        mov    edx,dword ptr [edx+0x1c]
        lea    ecx,[eax+eax*2]
        lea    ecx,[ecx+ecx*8]
        shl    ecx,1
        add    esp,0x4
        sub    ecx,eax
        push   ecx
        push   edx
        mov    ecx,ebp
        call FUN_004c02d5
        mov    edx,dword ptr [esp+0x5c]
        mov    ecx,dword ptr [edx+0x18]
        mov    esi,dword ptr [edx+0x1c]
        lea    eax,[ecx+ecx*2]
        lea    eax,[eax+eax*8]
        shl    eax,1
        sub    eax,ecx
        xor    ecx,ecx
        test   eax,eax
        jle lookup_4a5c02
        lookup_4a5be6:
        mov    bl,byte ptr [ecx+esi*1]
        mov    edx,ecx
        and    edx,0xff
        mov    dl,byte ptr [edx*4+DAT_00501d10]
        xor    bl,dl
        mov    byte ptr [ecx+esi*1],bl
        inc    ecx
        cmp    ecx,eax
        jl lookup_4a5be6
        lookup_4a5c02:
        mov    eax,[DAT_0056bbac]
        test   eax,eax
        mov    ecx,dword ptr [DAT_0056bbb0]
        jne lookup_4a5c15
        xor    edi,edi
        jmp lookup_4a5c1c
        lookup_4a5c15:
        mov    edi,ecx
        sub    edi,eax
        sar    edi,0x2
        lookup_4a5c1c:
        lea    eax,[esp+0x5c]
        push   eax
        push   0x1
        push   ecx
        mov    ecx,offset DAT_0056bba8
        call FUN_004bc680
        mov    ecx,ebp
        call FUN_004bfe39
        test   ebp,ebp
        je lookup_4a5c4b
        mov    ecx,dword ptr [ebp+0x0]
        mov    edx,dword ptr [ecx+0x4]
        mov    eax,dword ptr [ebp+edx*1+0x0]
        add    ebp,edx
        push   0x1
        mov    ecx,ebp
        call   dword ptr [eax]
        lookup_4a5c4b:
        cmp    edi,0xffffffff
        je lookup_4a5ca6
        mov    eax,[DAT_0056bbac]
        lookup_4a5c55:
        mov    eax,dword ptr [eax+edi*4]
        mov    edi,dword ptr [eax+0x18]
        xor    esi,esi
        test   edi,edi
        jbe lookup_4a5ca6
        mov    ebp,dword ptr [eax+0x1c]
        lea    ebx,[ebp+0xd]
        lookup_4a5c67:
        mov    eax,dword ptr [esp+0x1c]
        test   eax,eax
        jne lookup_4a5c74
        mov    eax,offset DAT_004e1344
        lookup_4a5c74:
        push   eax
        push   ebx
        call FUN_004ce290
        add    esp,0x8
        test   eax,eax
        je lookup_4a5c8c
        inc    esi
        add    ebx,0x35
        cmp    esi,edi
        jb lookup_4a5c67
        jmp lookup_4a5ca6
        lookup_4a5c8c:
        lea    eax,[esi+esi*2]
        lea    ecx,[eax+eax*8]
        shl    ecx,1
        sub    ecx,esi
        add    ecx,ebp
        mov    dword ptr [esp+0x5c],ecx
        je lookup_4a5ca6
        mov    dword ptr [esp+0x10],0x1
        lookup_4a5ca6:
        mov    eax,dword ptr [esp+0x1c]
        test   eax,eax
        pop    ebx
        je lookup_4a5ce0
        mov    cl,byte ptr [eax-0x1]
        test   cl,cl
        je lookup_4a5cd6
        cmp    cl,0xff
        je lookup_4a5cd6
        pop    edi
        dec    cl
        pop    esi
        mov    byte ptr [eax-0x1],cl
        mov    eax,dword ptr [esp+0x4]
        pop    ebp
        mov    ecx,dword ptr [esp+0x3c]
        mov    dword ptr fs:[0],ecx
        add    esp,0x48
        ret
        lookup_4a5cd6:
        dec    eax
        push   eax
        call FUN_004bd000
        add    esp,0x4
        lookup_4a5ce0:
        mov    ecx,dword ptr [esp+0x48]
        mov    eax,dword ptr [esp+0xc]
        pop    edi
        pop    esi
        pop    ebp
        mov    dword ptr fs:[0],ecx
        add    esp,0x48
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a5430
// original: FUN_004a5430
// Mirrors the observed 1253-byte archive-entry lookup and stream setup.
int File::OpenArchiveEntry(const char*, int, int)
{
    __asm {
        push   0xffffffff
        push   offset DAT_004de6e9
        mov    eax,fs:[0]
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,0x3c
        mov    al,byte ptr [esp+0x50]
        push   ebx
        push   ebp
        xor    edx,edx
        test   al,0x1
        push   esi
        push   edi
        mov    dword ptr [esp+0x10],ecx
        mov    dword ptr [esp+0x14],edx
        je openentry_4a58fc
        mov    edi,dword ptr [esp+0x5c]
        or     ecx,0xffffffff
        xor    eax,eax
        repne scasb
        not    ecx
        sub    edi,ecx
        mov    eax,ecx
        shr    ecx,0x2
        mov    esi,edi
        mov    edi,offset DAT_0056baa4
        rep movsd
        mov    ecx,eax
        and    ecx,0x3
        rep movsb
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,offset DAT_0056baa4
        repne scasb
        not    ecx
        dec    ecx
        lea    ebp,[ecx-0x1]
        cmp    ebp,edx
        jle openentry_4a58fc
        openentry_4a54a0:
        mov    al,byte ptr [ebp+DAT_0056baa4]
        cmp    al,0x2f
        je openentry_4a54b8
        cmp    al,0x5c
        je openentry_4a54b8
        dec    ebp
        cmp    ebp,edx
        jg openentry_4a54a0
        jmp openentry_4a58fc
        openentry_4a54b8:
        cmp    ebp,edx
        jle openentry_4a58fc
        mov    cl,byte ptr [esp+0x64]
        mov    byte ptr [esp+0x18],cl
        mov    dword ptr [esp+0x1c],edx
        mov    dword ptr [esp+0x20],edx
        mov    dword ptr [esp+0x24],edx
        or     ecx,0xffffffff
        lea    esi,[ebp+DAT_0056baa5]
        xor    eax,eax
        mov    edi,esi
        repne scasb
        not    ecx
        dec    ecx
        mov    ebx,ecx
        push   0x1
        push   ebx
        lea    ecx,[esp+0x20]
        mov    dword ptr [esp+0x5c],edx
        call FUN_00402980
        test   al,al
        je openentry_4a551c
        mov    edi,dword ptr [esp+0x1c]
        mov    ecx,ebx
        mov    edx,ecx
        shr    ecx,0x2
        rep movsd
        mov    ecx,edx
        and    ecx,0x3
        rep movsb
        mov    eax,dword ptr [esp+0x1c]
        mov    dword ptr [esp+0x20],ebx
        mov    byte ptr [eax+ebx*1],0x0
        openentry_4a551c:
        mov    byte ptr [ebp+DAT_0056baa4],0x2e
        mov    byte ptr [ebp+DAT_0056baa5],0x62
        mov    byte ptr [ebp+DAT_0056baa6],0x7a
        mov    byte ptr [ebp+DAT_0056baa7],0x66
        or     ebx,0xffffffff
        mov    byte ptr [ebp+DAT_0056baa8],0x0
        xor    esi,esi
        openentry_4a5544:
        mov    eax,[DAT_0056bbac]
        test   eax,eax
        je openentry_4a5582
        mov    ecx,dword ptr [DAT_0056bbb0]
        sub    ecx,eax
        sar    ecx,0x2
        cmp    esi,ecx
        jae openentry_4a5582
        mov    eax,dword ptr [eax+esi*4]
        mov    eax,dword ptr [eax+0x4]
        test   eax,eax
        jne openentry_4a556b
        mov    eax,offset DAT_004e1344
        openentry_4a556b:
        push   offset DAT_0056baa4
        push   eax
        call FUN_004ce290
        add    esp,0x8
        test   eax,eax
        jne openentry_4a557f
        mov    ebx,esi
        openentry_4a557f:
        inc    esi
        jmp openentry_4a5544
        openentry_4a5582:
        cmp    ebx,0xffffffff
        jne openentry_4a57dc
        lea    edx,[esp+0x28]
        push   edx
        push   offset DAT_0056baa4
        call FUN_004c285b
        add    esp,0x8
        test   eax,eax
        jne openentry_4a58d8
        push   0x64
        call ArchiveRawNew
        add    esp,0x4
        mov    dword ptr [esp+0x60],eax
        test   eax,eax
        mov    byte ptr [esp+0x54],0x1
        je openentry_4a55da
        mov    ecx,dword ptr [DAT_004e2ce4]
        push   0x1
        push   ecx
        push   0x81
        push   offset DAT_0056baa4
        mov    ecx,eax
        call FUN_004bfd2f
        mov    ebp,eax
        jmp openentry_4a55dc
        openentry_4a55da:
        xor    ebp,ebp
        openentry_4a55dc:
        mov    edx,dword ptr [ebp+0x0]
        mov    eax,dword ptr [edx+0x4]
        test   byte ptr [eax+ebp*1+0x8],0x6
        mov    esi,dword ptr [esp+0x10]
        mov    byte ptr [esp+0x54],0x0
        mov    dword ptr [esi],ebp
        je openentry_4a561c
        mov    ecx,ebp
        call FUN_004bfe39
        test   ebp,ebp
        je openentry_4a5611
        mov    ecx,dword ptr [ebp+0x0]
        mov    edx,dword ptr [ecx+0x4]
        mov    eax,dword ptr [ebp+edx*1+0x0]
        add    ebp,edx
        push   0x1
        mov    ecx,ebp
        call   dword ptr [eax]
        openentry_4a5611:
        mov    dword ptr [esi],0x0
        jmp openentry_4a58d8
        openentry_4a561c:
        push   0x20
        call ArchiveRawNew
        add    esp,0x4
        mov    dword ptr [esp+0x5c],eax
        test   eax,eax
        mov    byte ptr [esp+0x54],0x2
        je openentry_4a5642
        mov    ecx,eax
        call FUN_004a4b10
        mov    esi,eax
        mov    dword ptr [esp+0x5c],esi
        jmp openentry_4a564e
        openentry_4a5642:
        mov    dword ptr [esp+0x5c],0x0
        mov    esi,dword ptr [esp+0x5c]
        openentry_4a564e:
        or     ecx,0xffffffff
        xor    eax,eax
        mov    edi,offset DAT_0056baa4
        repne scasb
        not    ecx
        dec    ecx
        mov    ebx,ecx
        push   0x1
        push   ebx
        mov    ecx,esi
        mov    byte ptr [esp+0x5c],0x0
        mov    dword ptr [esp+0x68],esi
        call FUN_00402980
        test   al,al
        je openentry_4a569b
        mov    eax,dword ptr [esi+0x4]
        mov    ecx,ebx
        mov    edx,ecx
        shr    ecx,0x2
        mov    esi,offset DAT_0056baa4
        mov    edi,eax
        rep movsd
        mov    ecx,edx
        and    ecx,0x3
        rep movsb
        mov    ecx,dword ptr [esp+0x5c]
        mov    dword ptr [ecx+0x8],ebx
        mov    byte ptr [eax+ebx*1],0x0
        openentry_4a569b:
        mov    edx,dword ptr [esp+0x60]
        push   0xc
        add    edx,0x10
        push   edx
        mov    ecx,ebp
        call FUN_004c02d5
        mov    eax,dword ptr [esp+0x60]
        mov    eax,dword ptr [eax+0x18]
        lea    ecx,[eax+eax*2]
        lea    ecx,[ecx+ecx*8]
        shl    ecx,1
        sub    ecx,eax
        push   ecx
        call ArchiveRawNew
        mov    edx,dword ptr [esp+0x64]
        mov    dword ptr [edx+0x1c],eax
        mov    edx,dword ptr [esp+0x64]
        mov    eax,dword ptr [edx+0x18]
        mov    edx,dword ptr [edx+0x1c]
        lea    ecx,[eax+eax*2]
        lea    ecx,[ecx+ecx*8]
        shl    ecx,1
        add    esp,0x4
        sub    ecx,eax
        push   ecx
        push   edx
        mov    ecx,ebp
        call FUN_004c02d5
        mov    edx,dword ptr [esp+0x60]
        mov    ecx,dword ptr [edx+0x18]
        mov    esi,dword ptr [edx+0x1c]
        lea    eax,[ecx+ecx*2]
        lea    eax,[eax+eax*8]
        shl    eax,1
        sub    eax,ecx
        xor    ecx,ecx
        test   eax,eax
        jle openentry_4a5720
        openentry_4a5704:
        mov    bl,byte ptr [ecx+esi*1]
        mov    edx,ecx
        and    edx,0xff
        mov    dl,byte ptr [edx*4+DAT_00501d10]
        xor    bl,dl
        mov    byte ptr [ecx+esi*1],bl
        inc    ecx
        cmp    ecx,eax
        jl openentry_4a5704
        openentry_4a5720:
        mov    eax,[DAT_0056bbac]
        test   eax,eax
        mov    ecx,dword ptr [DAT_0056bbb0]
        jne openentry_4a5733
        xor    ebx,ebx
        jmp openentry_4a573a
        openentry_4a5733:
        mov    ebx,ecx
        sub    ebx,eax
        sar    ebx,0x2
        openentry_4a573a:
        lea    eax,[esp+0x60]
        push   eax
        push   0x1
        push   ecx
        mov    ecx,offset DAT_0056bba8
        call FUN_004bc680
        mov    eax,dword ptr [esp+0x64]
        test   eax,eax
        je openentry_4a5779
        mov    ecx,ebp
        call FUN_004bfe39
        test   ebp,ebp
        je openentry_4a576f
        mov    ecx,dword ptr [ebp+0x0]
        mov    edx,dword ptr [ecx+0x4]
        mov    eax,dword ptr [edx+ebp*1]
        lea    ecx,[edx+ebp*1]
        push   0x1
        call   dword ptr [eax]
        openentry_4a576f:
        mov    ecx,dword ptr [esp+0x10]
        mov    dword ptr [ecx],0x0
        openentry_4a5779:
        mov    eax,[DAT_0056bbac]
        openentry_4a577e:
        mov    edi,dword ptr [esp+0x10]
        openentry_4a5782:
        cmp    ebx,0xffffffff
        je openentry_4a58d8
        mov    eax,dword ptr [eax+ebx*4]
        mov    ebx,dword ptr [eax+0x18]
        xor    esi,esi
        xor    ebp,ebp
        test   ebx,ebx
        jbe openentry_4a583f
        mov    eax,dword ptr [eax+0x1c]
        mov    dword ptr [esp+0x5c],eax
        add    eax,0xd
        mov    dword ptr [esp+0x60],eax
        openentry_4a57ab:
        mov    eax,dword ptr [esp+0x1c]
        test   eax,eax
        jne openentry_4a57b8
        mov    eax,offset DAT_004e1344
        openentry_4a57b8:
        push   eax
        mov    eax,dword ptr [esp+0x64]
        push   eax
        call FUN_004ce290
        add    esp,0x8
        test   eax,eax
        je openentry_4a582e
        mov    ecx,dword ptr [esp+0x60]
        inc    ebp
        add    ecx,0x35
        cmp    ebp,ebx
        mov    dword ptr [esp+0x60],ecx
        jb openentry_4a57ab
        jmp openentry_4a583f
        openentry_4a57dc:
        mov    ecx,dword ptr [esp+0x64]
        test   ecx,ecx
        jne openentry_4a577e
        push   0x64
        call ArchiveRawNew
        add    esp,0x4
        mov    dword ptr [esp+0x60],eax
        test   eax,eax
        mov    byte ptr [esp+0x54],0x3
        je openentry_4a5817
        mov    edx,dword ptr [DAT_004e2ce4]
        push   0x1
        push   edx
        push   0x81
        push   offset DAT_0056baa4
        mov    ecx,eax
        call FUN_004bfd2f
        jmp openentry_4a5819
        openentry_4a5817:
        xor    eax,eax
        openentry_4a5819:
        mov    edi,dword ptr [esp+0x10]
        mov    dword ptr [edi],eax
        mov    eax,[DAT_0056bbac]
        mov    byte ptr [esp+0x54],0x0
        jmp openentry_4a5782
        openentry_4a582e:
        mov    eax,dword ptr [esp+0x5c]
        lea    esi,[ebp+ebp*2+0x0]
        lea    esi,[esi+esi*8]
        shl    esi,1
        sub    esi,ebp
        add    esi,eax
        openentry_4a583f:
        mov    eax,dword ptr [esp+0x64]
        test   eax,eax
        je openentry_4a5859
        test   esi,esi
        je openentry_4a58d8
        mov    dword ptr [esp+0x14],0x1
        jmp openentry_4a58d8
        openentry_4a5859:
        test   esi,esi
        je openentry_4a58b7
        mov    bl,byte ptr [esi]
        mov    eax,dword ptr [esi+0x1]
        mov    ecx,dword ptr [esi+0x5]
        mov    edx,dword ptr [esi+0x9]
        test   bl,bl
        seta   bl
        xor    esi,esi
        push   esi
        mov    dword ptr [edi+0x18],ecx
        mov    ecx,dword ptr [edi]
        push   eax
        mov    dword ptr [edi+0x28],eax
        mov    byte ptr [edi+0x15],bl
        mov    dword ptr [edi+0x24],edx
        call FUN_004c0269
        test   bl,bl
        mov    dword ptr [esp+0x14],0x1
        je openentry_4a58d8
        push   0x38
        mov    dword ptr [edi+0x4c],esi
        mov    dword ptr [edi+0x50],esi
        mov    dword ptr [edi+0x54],esi
        mov    dword ptr [edi+0x30],esi
        mov    dword ptr [edi+0x3c],esi
        mov    dword ptr [edi+0x64],esi
        push   offset DAT_00502134
        add    edi,0x2c
        push   edi
        call FUN_004cf9ea
        add    esp,0xc
        jmp openentry_4a58d8
        openentry_4a58b7:
        mov    esi,dword ptr [edi]
        mov    ecx,esi
        call FUN_004bfe39
        test   esi,esi
        je openentry_4a58d2
        mov    eax,dword ptr [esi]
        mov    ecx,dword ptr [eax+0x4]
        mov    edx,dword ptr [ecx+esi*1]
        add    ecx,esi
        push   0x1
        call   dword ptr [edx]
        openentry_4a58d2:
        mov    dword ptr [edi],0x0
        openentry_4a58d8:
        mov    ecx,dword ptr [esp+0x1c]
        test   ecx,ecx
        je openentry_4a58fc
        mov    al,byte ptr [ecx-0x1]
        test   al,al
        je openentry_4a58f2
        cmp    al,0xff
        je openentry_4a58f2
        dec    al
        mov    byte ptr [ecx-0x1],al
        jmp openentry_4a58fc
        openentry_4a58f2:
        dec    ecx
        push   ecx
        call FUN_004bd000
        add    esp,0x4
        openentry_4a58fc:
        mov    ecx,dword ptr [esp+0x4c]
        mov    eax,dword ptr [esp+0x14]
        pop    edi
        pop    esi
        pop    ebp
        pop    ebx
        mov    dword ptr fs:[0],ecx
        add    esp,0x48
        ret    0xc
    }
}
