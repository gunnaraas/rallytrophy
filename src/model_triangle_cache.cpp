// ModelObject cache refresh entry: the behavior and field roles are mapped in
// analysis/mesh-triangle-cache.md; this isolates the observed VC6 EH/register
// layout while using the exact cache builder and cleanup entries.
class ModelObject {
public:
    void FUN_004a6d30();
    void CopyPartStateExact(const void* source);
    void CopyMeshListExact(const void* source);
    void ClearMeshListExact(unsigned int flags);
    void* FindByRadiusExact(float radius);
    void* FindByValueExact(int value);
    void* MeshAtExact(int index);
    int MeshCountExact();
    void InsertMeshExact(const void* mesh, float radius);
    void ConstructExact();
    ModelObject* DestroyExact(unsigned int flags);
    void DestroyBodyExact();
    void CopyMeshTransformExact();
    void FUN_004a75a0(int);
    int CheckMeshTrianglesExact();
};

extern "C" void* dat_004de8bb;
extern "C" void* dat_004de7db;
extern "C" void* dat_004de918;
extern "C" void fun_004a6800();
extern "C" void fun_004a6840();
extern "C" void fun_004bc5d0();
extern "C" void fun_004bd000();
extern "C" void fun_004c1276();
extern "C" void fun_004bc390();

extern "C" void RT_SceneEntryDtor4a66a0();
#pragma comment(linker, "/alternatename:_RT_SceneEntryDtor4a66a0=??1SceneEntry@@QAE@XZ")

/* FUNCTION: RALLYTROPHY 0x4a66a0 */
// SYMBOL: _FUN_004a66a0
// PE observation: this five-byte thunk jumps directly to SceneEntry::~SceneEntry.
extern "C" {
__declspec(naked) void FUN_004a66a0()
{
    __asm {
        jmp RT_SceneEntryDtor4a66a0
    }
}
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a6d30
// original: ModelObject::FUN_004a6d30
void ModelObject::FUN_004a6d30()
{
    __asm {
        push -1
        push offset dat_004de8bb
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push ebx
        push edi
        mov edi, ecx
        push 0
        lea ecx, [edi + 0x124]
        call fun_004a6800
        mov ebx, eax
        test ebx, ebx
        jz cache_refresh_done
        push esi
        mov esi, dword ptr [edi + 0x128]
        test esi, esi
        jz allocate_cache
        mov ecx, esi
        call fun_004bc5d0
        push esi
        call fun_004bd000
        add esp, 4
    allocate_cache:
        push 0x10
        call fun_004c1276
        add esp, 4
        mov dword ptr [esp + 0xc], eax
        test eax, eax
        mov dword ptr [esp + 0x18], 0
        pop esi
        jz no_new_cache
        push ebx
        mov ecx, eax
        call fun_004bc390
        jmp cache_stored
    no_new_cache:
        xor eax, eax
    cache_stored:
        mov dword ptr [edi + 0x128], eax
    cache_refresh_done:
        mov ecx, dword ptr [esp + 0xc]
        pop edi
        pop ebx
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

extern "C" void fun_004b2b20();
extern "C" void fun_004a65c0();
extern "C" void fun_004a6d30();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a7560
// original: FUN_004a7560
void ModelObject::CopyPartStateExact(const void*)
{
    __asm {
        push esi
        push edi
        mov edi, dword ptr [esp + 0xc]
        mov esi, ecx
        push edi
        call fun_004b2b20
        lea eax, [edi + 0x124]
        push eax
        lea ecx, [esi + 0x124]
        call fun_004a65c0
        mov eax, dword ptr [edi + 0x128]
        test eax, eax
        jz part_cache_done
        mov ecx, esi
        call fun_004a6d30
    part_cache_done:
        pop edi
        pop esi
        ret 4
    }
}

extern "C" void fun_004b8b10();
extern "C" void fun_004b8e50();
extern "C" void fun_004b8f70();
extern "C" void fun_004b90b0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a75a0
// original: FUN_004a75a0
void ModelObject::FUN_004a75a0(int)
{
    __asm {
        push ebp
        mov ebp, esp
        push -1
        push offset dat_004de918
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 0x20
        mov eax, dword ptr [ebp + 8]
        push ebx
        xor ebx, ebx
        cmp eax, ebx
        push esi
        mov esi, ecx
        je refresh_no_arg
        mov dl, byte ptr [esi + 0x0c]
        or dl, 4
        push edi
        lea ecx, [esi + 0x124]
        mov byte ptr [esi + 0x0c], dl
        call fun_004a6840
        cmp eax, ebx
        mov dword ptr [ebp + 8], eax
        jle refresh_after_prepare
    refresh_prepare_loop:
        push ebx
        lea ecx, [esi + 0x124]
        call fun_004a6800
        mov edi, eax
        lea eax, [esi + 0x20]
        push eax
        mov ecx, edi
        call fun_004b8f70
        lea eax, [esi + 0x14]
        push eax
        mov ecx, edi
        call fun_004b8e50
        lea eax, [esi + 0x20]
        push eax
        mov ecx, edi
        call fun_004b90b0
        mov eax, dword ptr [ebp + 8]
        inc ebx
        cmp ebx, eax
        jl refresh_prepare_loop
        xor ebx, ebx
    refresh_after_prepare:
        test byte ptr [esi + 0xcc], 3
        jne refresh_transform
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 8]
        jmp refresh_clear_fields
    refresh_transform:
        lea eax, [esi + 0x14]
        mov dword ptr [ebp + 8], eax
        lea eax, [esi + 0x20]
        lea edi, [esi + 0xb8]
        lea ecx, [ebp - 0x2c]
        mov dword ptr [ebp - 4], ebx
        mov dword ptr [ebp - 0x14], eax
        mov dword ptr [ebp - 0x18], edi
        mov dword ptr [ebp - 0x10], ecx
        mov ecx, dword ptr [ebp - 0x18]
        mov edx, dword ptr [ebp - 0x14]
        mov eax, dword ptr [ebp - 0x10]
        mov ebx, dword ptr [ebp + 8]
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 4]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 8]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x10]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x14]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x18]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x20]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x24]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x28]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fadd dword ptr [ebx]
        fxch st(1)
        fadd dword ptr [ebx + 4]
        fxch st(2)
        fadd dword ptr [ebx + 8]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax + 8]
        fstp dword ptr [eax + 4]
        mov ecx, dword ptr [ebp - 0x24]
        mov edx, dword ptr [ebp - 0x2c]
        mov eax, dword ptr [ebp - 0x28]
        mov dword ptr [edi + 8], ecx
        mov dword ptr [esi + 0xa8], ecx
        mov ecx, dword ptr [edi + 8]
        mov dword ptr [edi], edx
        mov dword ptr [edi + 4], eax
        mov dword ptr [esi + 0xa0], edx
        mov edx, dword ptr [edi]
        mov dword ptr [esi + 0xa4], eax
        mov eax, dword ptr [edi + 4]
        mov dword ptr [esi + 0xb4], ecx
        lea ecx, [esi + 0x124]
        mov dword ptr [esi + 0xac], edx
        mov dword ptr [esi + 0xb0], eax
        call fun_004a6840
        test eax, eax
        mov dword ptr [ebp - 0x20], eax
        mov dword ptr [ebp + 8], 0
        jle refresh_zero_accumulator
        lea eax, [esi + 0x14]
        mov dword ptr [ebp - 0x18], eax
        lea eax, [esi + 0x20]
        lea edx, [ebp - 0x2c]
        mov dword ptr [ebp - 0x10], eax
        mov dword ptr [ebp - 0x14], edx
    refresh_item_loop:
        mov eax, dword ptr [ebp + 8]
        push eax
        lea ecx, [esi + 0x124]
        call fun_004a6800
        mov edi, eax
        mov ecx, edi
        call fun_004b8b10
        lea eax, [edi + 0x18]
        mov dword ptr [ebp - 0x1c], eax
        mov ecx, dword ptr [ebp - 0x1c]
        mov edx, dword ptr [ebp - 0x10]
        mov eax, dword ptr [ebp - 0x14]
        mov ebx, dword ptr [ebp - 0x18]
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 4]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 8]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x10]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x14]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 0x18]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x20]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x24]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 0x28]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fadd dword ptr [ebx]
        fxch st(1)
        fadd dword ptr [ebx + 4]
        fxch st(2)
        fadd dword ptr [ebx + 8]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax + 8]
        fstp dword ptr [eax + 4]
        mov ecx, dword ptr [ebp - 0x2c]
        mov dword ptr [edi + 0x18], ecx
        fld dword ptr [edi]
        mov edx, dword ptr [ebp - 0x28]
        fcomp dword ptr [esi + 0xa0]
        mov dword ptr [edi + 0x1c], edx
        mov eax, dword ptr [ebp - 0x24]
        mov dword ptr [edi + 0x20], eax
        fnstsw ax
        test ah, 1
        je refresh_min_x_done
        mov ecx, dword ptr [edi]
        mov dword ptr [esi + 0xa0], ecx
    refresh_min_x_done:
        fld dword ptr [edi + 0x0c]
        fcomp dword ptr [esi + 0xac]
        fnstsw ax
        test ah, 0x41
        jne refresh_max_y_done
        mov edx, dword ptr [edi + 0x0c]
        mov dword ptr [esi + 0xac], edx
    refresh_max_y_done:
        fld dword ptr [edi + 4]
        fcomp dword ptr [esi + 0xa4]
        fnstsw ax
        test ah, 1
        je refresh_min_z_done
        mov eax, dword ptr [edi + 4]
        mov dword ptr [esi + 0xa4], eax
    refresh_min_z_done:
        fld dword ptr [edi + 0x10]
        fcomp dword ptr [esi + 0xb0]
        fnstsw ax
        test ah, 0x41
        jne refresh_max_z_done
        mov ecx, dword ptr [edi + 0x10]
        mov dword ptr [esi + 0xb0], ecx
    refresh_max_z_done:
        fld dword ptr [edi + 8]
        fcomp dword ptr [esi + 0xa8]
        fnstsw ax
        test ah, 1
        je refresh_min_y_done
        mov edx, dword ptr [edi + 8]
        mov dword ptr [esi + 0xa8], edx
    refresh_min_y_done:
        fld dword ptr [edi + 0x14]
        fcomp dword ptr [esi + 0xb4]
        fnstsw ax
        test ah, 0x41
        jne refresh_bounds_done
        mov eax, dword ptr [edi + 0x14]
        mov dword ptr [esi + 0xb4], eax
    refresh_bounds_done:
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0x20]
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp + 8], eax
        jl refresh_item_loop
    refresh_zero_accumulator:
        xor ebx, ebx
    refresh_clear_fields:
        mov eax, 0x3f800000
        mov dword ptr [esi + 0x1c], ebx
        mov dword ptr [esi + 0x18], ebx
        mov dword ptr [esi + 0x14], ebx
        mov dword ptr [esi + 0x2c], ebx
        mov dword ptr [esi + 0x28], ebx
        mov dword ptr [esi + 0x24], ebx
        mov dword ptr [esi + 0x3c], ebx
        mov dword ptr [esi + 0x38], ebx
        mov dword ptr [esi + 0x30], ebx
        mov dword ptr [esi + 0x4c], ebx
        mov dword ptr [esi + 0x44], ebx
        mov dword ptr [esi + 0x40], ebx
        mov dword ptr [esi + 0x58], ebx
        mov dword ptr [esi + 0x54], ebx
        mov dword ptr [esi + 0x50], ebx
        mov dword ptr [esi + 0x5c], eax
        mov dword ptr [esi + 0x48], eax
        mov dword ptr [esi + 0x34], eax
        mov dword ptr [esi + 0x20], eax
        pop edi
        mov dword ptr [esi + 0x6c], ebx
        mov dword ptr [esi + 0x68], ebx
        mov dword ptr [esi + 0x64], ebx
        mov dword ptr [esi + 0x7c], ebx
        mov dword ptr [esi + 0x78], ebx
        mov dword ptr [esi + 0x70], ebx
        mov dword ptr [esi + 0x8c], ebx
        mov dword ptr [esi + 0x84], ebx
        mov dword ptr [esi + 0x80], ebx
        mov dword ptr [esi + 0x98], ebx
        mov dword ptr [esi + 0x94], ebx
        mov dword ptr [esi + 0x90], ebx
        mov dword ptr [esi + 0x9c], eax
        mov dword ptr [esi + 0x88], eax
        mov dword ptr [esi + 0x74], eax
        mov dword ptr [esi + 0x60], eax
        pop esi
        pop ebx
        mov ecx, dword ptr [ebp - 0x0c]
        mov dword ptr fs:[0], ecx
        mov esp, ebp
        pop ebp
        ret 4
    refresh_no_arg:
        mov al, byte ptr [esi + 0x0c]
        mov ecx, dword ptr [ebp - 0x0c]
        and al, 0xfb
        mov byte ptr [esi + 0x0c], al
        pop esi
        pop ebx
        mov dword ptr fs:[0], ecx
        mov esp, ebp
        pop ebp
        ret 4
    }
}

extern "C" void fun_004c1116();
extern "C" void fun_004b8640();
extern "C" void fun_004b8680();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a65c0
// original: FUN_004a65c0
void ModelObject::CopyMeshListExact(const void*)
{
    __asm {
        push -1
        push offset dat_004de7db
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        mov eax, dword ptr [ecx]
        push ebx
        push ebp
        xor ebp, ebp
        cmp eax, ebp
        push esi
        push edi
        mov dword ptr [esp + 0x10], ecx
        jz destroy_done
    destroy_loop:
        cmp eax, ebp
        mov edi, dword ptr [eax + 0x6c]
        jz destroy_next
        mov ecx, dword ptr [eax - 4]
        lea esi, [eax - 4]
        push 0x4a66a0
        push ecx
        push 0x70
        push eax
        call fun_004c1116
        push esi
        call fun_004bd000
        mov ecx, dword ptr [esp + 0x14]
        add esp, 4
    destroy_next:
        cmp edi, ebp
        mov eax, edi
        jnz destroy_loop
    destroy_done:
        mov edx, dword ptr [esp + 0x24]
        mov edi, dword ptr [edx]
        xor ebx, ebx
        cmp edi, ebp
        mov dword ptr [ecx], ebp
        jz copy_done
    copy_loop:
        push 0x70
        call fun_004c1276
        mov esi, eax
        add esp, 4
        mov dword ptr [esp + 0x24], esi
        cmp esi, ebp
        mov dword ptr [esp + 0x1c], ebp
        jz no_initialize
        mov ecx, esi
        call fun_004b8640
        jmp initialized
    no_initialize:
        xor esi, esi
    initialized:
        mov ecx, dword ptr [edi + 0x68]
        mov eax, dword ptr [edi + 0x64]
        mov dword ptr [esi + 0x68], ecx
        push edi
        mov ecx, esi
        mov dword ptr [esp + 0x20], 0xffffffff
        mov dword ptr [esi + 0x64], eax
        call fun_004b8680
        cmp ebx, ebp
        mov dword ptr [esi + 0x6c], ebp
        jz first_node
        mov dword ptr [ebx + 0x6c], esi
        jmp append_done
    first_node:
        mov edx, dword ptr [esp + 0x10]
        mov dword ptr [edx], esi
    append_done:
        mov edi, dword ptr [edi + 0x6c]
        cmp edi, ebp
        mov ebx, esi
        jnz copy_loop
        mov eax, dword ptr [esp + 0x10]
        jmp copy_epilogue
    copy_done:
        mov eax, ecx
    copy_epilogue:
        mov ecx, dword ptr [esp + 0x14]
        pop edi
        pop esi
        pop ebp
        pop ebx
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret 4
    }
}

extern "C" void fun_004a66a0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a66b0
// original: FUN_004a66b0
void ModelObject::ClearMeshListExact(unsigned int)
{
    __asm {
        push esi
        mov esi, dword ptr [ecx]
        test esi, esi
        jz clear_done
        push edi
    clear_loop:
        test esi, esi
        mov edi, dword ptr [esi + 0x6c]
        jz clear_next
        mov ecx, esi
        call fun_004a66a0
        push esi
        call fun_004bd000
        add esp, 4
    clear_next:
        test edi, edi
        mov esi, edi
        jnz clear_loop
        pop edi
    clear_done:
        pop esi
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a6790
// original: FUN_004a6790
void* ModelObject::FindByRadiusExact(float)
{
    __asm {
        mov eax, dword ptr [ecx]
        test eax, eax
        jnz radius_nonempty
        ret 4
    radius_nonempty:
        mov edx, dword ptr [esp + 4]
        mov ecx, edx
        and ecx, 0x80000000
        neg ecx
        sbb ecx, ecx
        not ecx
        and ecx, edx
        mov edx, dword ptr [eax + 0x6c]
        test edx, edx
        jz radius_done
    radius_loop:
        cmp dword ptr [edx + 0x64], ecx
        jge radius_done
        mov eax, edx
        mov edx, dword ptr [eax + 0x6c]
        test edx, edx
        jnz radius_loop
    radius_done:
        ret 4
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a67d0
// original: FUN_004a67d0
void* ModelObject::FindByValueExact(int)
{
    __asm {
        mov eax, dword ptr [ecx]
        test eax, eax
        jnz value_nonempty
        ret 4
    value_nonempty:
        mov ecx, dword ptr [eax + 0x6c]
        test ecx, ecx
        jz value_done
        mov edx, dword ptr [esp + 4]
    value_loop:
        cmp dword ptr [ecx + 0x68], edx
        jge value_done
        mov eax, ecx
        mov ecx, dword ptr [eax + 0x6c]
        test ecx, ecx
        jnz value_loop
    value_done:
        ret 4
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a6800
// original: FUN_004a6800
void* ModelObject::MeshAtExact(int)
{
    __asm {
        mov ecx, dword ptr [ecx]
        test ecx, ecx
        jnz at_nonempty
        xor eax, eax
        ret 4
    at_nonempty:
        mov eax, dword ptr [ecx + 0x6c]
        xor edx, edx
        test eax, eax
        push esi
        mov esi, dword ptr [esp + 8]
        jz at_finish
    at_loop:
        cmp edx, esi
        jge at_finish
        mov ecx, eax
        mov eax, dword ptr [ecx + 0x6c]
        inc edx
        test eax, eax
        jnz at_loop
    at_finish:
        xor eax, eax
        cmp edx, esi
        setnz al
        pop esi
        dec eax
        and eax, ecx
        ret 4
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a6840
// original: FUN_004a6840
int ModelObject::MeshCountExact()
{
    __asm {
        mov ecx, dword ptr [ecx]
        test ecx, ecx
        jnz count_nonempty
        xor eax, eax
        ret
    count_nonempty:
        mov ecx, dword ptr [ecx + 0x6c]
        test ecx, ecx
        mov eax, 1
        jz count_done
    count_loop:
        mov ecx, dword ptr [ecx + 0x6c]
        inc eax
        test ecx, ecx
        jnz count_loop
    count_done:
        ret
    }
}

extern "C" void* dat_004de818;
extern "C" void* PTR_FUN_004e2930;
extern "C" void fun_004b2830();
extern "C" void fun_0040c6f0();
extern "C" void fun_004a6cb0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a6890
// original: FUN_004a6890
void ModelObject::ConstructExact()
{
    __asm {
        push -1
        push offset dat_004de818
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        mov esi, ecx
        mov dword ptr [esp + 4], esi
        call fun_004b2830
        lea ecx, [esi + 0x124]
        mov dword ptr [esp + 0x10], 0
        call fun_0040c6f0
        mov ecx, dword ptr [esp + 8]
        mov dword ptr [esi], offset PTR_FUN_004e2930
        mov dword ptr [esi + 4], 1
        mov dword ptr [esi + 0x128], 0
        mov eax, esi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a68f0
// original: FUN_004a68f0
ModelObject* ModelObject::DestroyExact(unsigned int)
{
    __asm {
        push esi
        mov esi, ecx
        call fun_004a6cb0
        test byte ptr [esp + 8], 1
        jz destroy_model_done
        push esi
        call fun_004bd000
        add esp, 4
    destroy_model_done:
        mov eax, esi
        pop esi
        ret 4
    }
}

extern "C" void* dat_004de8a6;
extern "C" void fun_004b29c0();
extern "C" void fun_004a66b0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a6cb0
// original: FUN_004a6cb0
void ModelObject::DestroyBodyExact()
{
    __asm {
        push -1
        push offset dat_004de8a6
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        mov esi, ecx
        push edi
        mov dword ptr [esp + 8], esi
        mov dword ptr [esi], offset PTR_FUN_004e2930
        mov edi, dword ptr [esi + 0x128]
        test edi, edi
        mov dword ptr [esp + 0x14], 1
        jz destroy_cache_done
        mov ecx, edi
        call fun_004bc5d0
        push edi
        call fun_004bd000
        add esp, 4
    destroy_cache_done:
        lea ecx, [esi + 0x124]
        mov byte ptr [esp + 0x14], 0
        call fun_004a66b0
        mov ecx, esi
        mov dword ptr [esp + 0x14], 0xffffffff
        call fun_004b29c0
        mov ecx, dword ptr [esp + 0xc]
        pop edi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

extern "C" void fun_004b8c30();
extern "C" void fun_004a6790();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a6dc0
// original: FUN_004a6dc0
void ModelObject::CopyMeshTransformExact()
{
    __asm {
        push esi
        push edi
        mov edi, ecx
        push 0
        lea ecx, [edi + 0x124]
        call fun_004a6790
        mov esi, eax
        test esi, esi
        jz transform_done
        mov ecx, esi
        call fun_004b8c30
        mov eax, dword ptr [esi]
        mov ecx, dword ptr [esi + 4]
        mov edx, dword ptr [esi + 8]
        mov dword ptr [edi + 0xa0], eax
        mov eax, dword ptr [esi + 0xc]
        mov dword ptr [edi + 0xa4], ecx
        mov ecx, dword ptr [esi + 0x10]
        mov dword ptr [edi + 0xa8], edx
        mov edx, dword ptr [esi + 0x14]
        mov dword ptr [edi + 0xac], eax
        mov eax, dword ptr [esi + 0x18]
        mov dword ptr [edi + 0xb0], ecx
        mov ecx, dword ptr [esi + 0x1c]
        mov dword ptr [edi + 0xb4], edx
        mov edx, dword ptr [esi + 0x20]
        mov dword ptr [edi + 0xb8], eax
        mov eax, dword ptr [esi + 0x24]
        mov dword ptr [edi + 0xbc], ecx
        mov ecx, dword ptr [esi + 0x28]
        mov dword ptr [edi + 0xc0], edx
        mov dl, byte ptr [esi + 0x2c]
        mov dword ptr [edi + 0xc4], eax
        mov dword ptr [edi + 0xc8], ecx
        mov byte ptr [edi + 0xcc], dl
    transform_done:
        pop edi
        pop esi
        ret
    }
}

extern "C" void fun_004a6840();
extern "C" void fun_004a6800();
extern "C" void fun_00414330();
extern "C" float dat_004e134c;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a7920
// original: FUN_004a7920
int ModelObject::CheckMeshTrianglesExact()
{
    __asm {
        push -1
        __emit 0x68
        __emit 0
        __emit 0
        __emit 0
        __emit 0
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 0x4c
        push esi
        lea esi, [ecx + 0x124]
        push edi
        mov edi, 1
        mov ecx, esi
        mov dword ptr [esp + 0x1c], edi
        mov dword ptr [esp + 0x20], esi
        call fun_004a6840
        xor ecx, ecx
        test eax, eax
        mov dword ptr [esp + 0x2c], eax
        mov dword ptr [esp + 0x18], ecx
        jle validate_no_meshes
        push ebx
        push ebp
        jmp validate_mesh_start
    validate_mesh_resume:
        mov esi, dword ptr [esp + 0x28]
    validate_mesh_start:
        push ecx
        mov ecx, esi
        call fun_004a6800
        mov edx, eax
        mov edi, dword ptr [edx + 0x34]
        xor ecx, ecx
        mov dword ptr [esp + 0x30], edx
        mov dword ptr [esp + 0x1c], ecx
        mov dword ptr [esp + 0x2c], edi
    validate_group_bound:
        test edi, edi
        jz validate_next_mesh
        mov eax, dword ptr [edx + 0x38]
        sub eax, edi
        sar eax, 2
        cmp ecx, eax
        jnc validate_next_mesh
        mov eax, dword ptr [edx + 0x34]
        mov esi, dword ptr [eax + ecx*4]
        mov ebx, dword ptr [eax + ecx*4]
        mov ebp, dword ptr [esi + 4]
        mov esi, dword ptr [esi + 8]
        xor eax, eax
        mov ax, word ptr [ebx + 0xe]
        mov dword ptr [esp + 0x14], esi
        test eax, eax
        jle validate_advance_group
        mov dword ptr [esp + 0x18], eax
        jmp validate_triangle_start
    validate_triangle_resume:
        mov esi, dword ptr [esp + 0x14]
    validate_triangle_start:
        xor eax, eax
        mov ax, word ptr [esi]
        xor ecx, ecx
        mov cx, word ptr [esi + 2]
        xor edx, edx
        mov dx, word ptr [esi + 4]
        mov dword ptr [esp + 0x10], 1
        cmp eax, ecx
        jz validate_duplicate_index
        cmp eax, edx
        jz validate_duplicate_index
        cmp ecx, edx
        jnz validate_indices_distinct
    validate_duplicate_index:
        mov dword ptr [esp + 0x10], 0
    validate_indices_distinct:
        lea esi, [eax + eax*4]
        lea eax, [eax + esi*2]
        lea esi, [ebp + eax*4]
        lea eax, [ecx + ecx*4]
        lea ecx, [ecx + eax*2]
        lea eax, [edx + edx*4]
        lea edi, [ebp + ecx*4]
        lea ecx, [edx + eax*2]
        push esi
        lea edx, [esp + 0x3c]
        lea ebx, [ebp + ecx*4]
        push edx
        mov ecx, edi
        call fun_00414330
        fld dword ptr [esp + 0x3c]
        fmul dword ptr [esp + 0x3c]
        xor ecx, ecx
        fld dword ptr [esp + 0x40]
        mov dword ptr [esp + 0x64], ecx
        fmul dword ptr [esp + 0x40]
        faddp st(1), st(0)
        fld dword ptr [esp + 0x38]
        fmul dword ptr [esp + 0x38]
        faddp st(1), st(0)
        fcomp dword ptr [dat_004e134c]
        fnstsw ax
        test ah, 0x40
        jz validate_first_edge_nonzero
        mov dword ptr [esp + 0x10], ecx
    validate_first_edge_nonzero:
        push esi
        lea eax, [esp + 0x48]
        push eax
        mov ecx, ebx
        call fun_00414330
        mov ecx, dword ptr [eax]
        mov dword ptr [esp + 0x38], ecx
        mov edx, dword ptr [eax + 4]
        mov dword ptr [esp + 0x3c], edx
        fld dword ptr [esp + 0x3c]
        fmul dword ptr [esp + 0x3c]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [esp + 0x40], eax
        fld dword ptr [esp + 0x40]
        fmul dword ptr [esp + 0x40]
        faddp st(1), st(0)
        fld dword ptr [esp + 0x38]
        fmul dword ptr [esp + 0x38]
        faddp st(1), st(0)
        fcomp dword ptr [dat_004e134c]
        fnstsw ax
        test ah, 0x40
        jz validate_second_edge_nonzero
        xor esi, esi
        jmp validate_second_edge_checked
    validate_second_edge_nonzero:
        mov esi, dword ptr [esp + 0x10]
    validate_second_edge_checked:
        push edi
        lea ecx, [esp + 0x54]
        push ecx
        mov ecx, ebx
        call fun_00414330
        mov edx, dword ptr [eax]
        mov dword ptr [esp + 0x38], edx
        mov ecx, dword ptr [eax + 4]
        mov dword ptr [esp + 0x3c], ecx
        fld dword ptr [esp + 0x3c]
        fmul dword ptr [esp + 0x3c]
        mov edx, dword ptr [eax + 8]
        mov dword ptr [esp + 0x40], edx
        fld dword ptr [esp + 0x40]
        fmul dword ptr [esp + 0x40]
        faddp st(1), st(0)
        fld dword ptr [esp + 0x38]
        fmul dword ptr [esp + 0x38]
        faddp st(1), st(0)
        fcomp dword ptr [dat_004e134c]
        fnstsw ax
        test ah, 0x40
        jnz validate_mark_degenerate
        test esi, esi
        jnz validate_triangle_valid
    validate_mark_degenerate:
        mov dword ptr [esp + 0x24], 0
    validate_triangle_valid:
        mov ecx, dword ptr [esp + 0x14]
        mov eax, dword ptr [esp + 0x18]
        add ecx, 6
        dec eax
        mov dword ptr [esp + 0x64], 0xffffffff
        mov dword ptr [esp + 0x14], ecx
        mov dword ptr [esp + 0x18], eax
        jnz validate_triangle_resume
        mov edi, dword ptr [esp + 0x2c]
        mov edx, dword ptr [esp + 0x30]
        mov ecx, dword ptr [esp + 0x1c]
    validate_advance_group:
        inc ecx
        mov dword ptr [esp + 0x1c], ecx
        jmp validate_group_bound
    validate_next_mesh:
        mov ecx, dword ptr [esp + 0x20]
        mov eax, dword ptr [esp + 0x34]
        inc ecx
        cmp ecx, eax
        mov dword ptr [esp + 0x20], ecx
        jl validate_mesh_resume
        mov eax, dword ptr [esp + 0x24]
        pop ebp
        pop ebx
        pop edi
        pop esi
        mov ecx, dword ptr [esp + 0x4c]
        mov dword ptr fs:[0], ecx
        add esp, 0x58
        ret
    validate_no_meshes:
        mov ecx, dword ptr [esp + 0x54]
        mov eax, edi
        pop edi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x58
        ret
    }
}

extern "C" void* dat_004de7fb;
extern "C" void fun_004b8640();
extern "C" void fun_004b8680();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a66e0
// original: FUN_004a66e0
void ModelObject::InsertMeshExact(const void*, float)
{
    __asm {
        push -1
        push offset dat_004de7fb
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        push edi
        push 0x70
        mov edi, ecx
        call fun_004c1276
        mov esi, eax
        add esp, 4
        mov dword ptr [esp + 8], esi
        test esi, esi
        mov dword ptr [esp + 0x14], 0
        jz insert_uninitialized
        mov ecx, esi
        call fun_004b8640
        jmp insert_initialized
    insert_uninitialized:
        xor esi, esi
    insert_initialized:
        fld dword ptr [esp + 0x20]
        mov ecx, dword ptr [esp + 0x1c]
        fmul dword ptr [esp + 0x20]
        mov eax, dword ptr [esp + 0x20]
        push ecx
        mov ecx, esi
        fstp dword ptr [esi + 0x68]
        mov dword ptr [esp + 0x18], 0xffffffff
        mov dword ptr [esi + 0x64], eax
        call fun_004b8680
        mov ecx, dword ptr [edi]
        xor edx, edx
        test ecx, ecx
        mov dword ptr [esi + 0x6c], 0
        jz insert_at_head
    insert_search:
        fld dword ptr [ecx + 0x64]
        fcomp dword ptr [esp + 0x20]
        fnstsw ax
        test ah, 1
        jz insert_position_found
        mov edx, ecx
        mov ecx, dword ptr [ecx + 0x6c]
        test ecx, ecx
        jnz insert_search
    insert_position_found:
        test edx, edx
        jnz insert_after
    insert_at_head:
        mov dword ptr [edi], esi
        jmp insert_linked
    insert_after:
        mov dword ptr [edx + 0x6c], esi
    insert_linked:
        mov dword ptr [esi + 0x6c], ecx
        mov ecx, dword ptr [esp + 0xc]
        pop edi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret 8
    }
}

extern "C" unsigned int dat_00577bc4;
extern "C" unsigned short dat_00577bc0;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a7d90
// original: FUN_004a7d90
void SetFpuControlWordExact(unsigned short)
{
    __asm {
        push ebp
        mov ebp, esp
        fldcw word ptr [ebp + 8]
        pop ebp
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a7da0
// original: FUN_004a7da0
unsigned short GetFpuControlWordExact()
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        fstcw word ptr [ebp - 2]
        mov eax, dword ptr [dat_00577bc4]
        test eax, eax
        jz fpu_control_ready
        fldcw word ptr [dat_00577bc0]
    fpu_control_ready:
        mov ax, word ptr [ebp - 2]
        mov esp, ebp
        pop ebp
        ret
    }
}

class SplineCoefficients {
public:
    float EvaluateExact(float p0, float p1, float p2, float p3);
};

extern "C" float dat_004e1f30;
extern "C" float dat_004e138c;
extern "C" float dat_004e1530;
extern "C" float dat_004e1eb8;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a7f30
// original: FUN_004a7f30
float SplineCoefficients::EvaluateExact(float, float, float, float)
{
    __asm {
        fld dword ptr [esp + 4]
        fmul dword ptr [dat_004e1f30]
        fld dword ptr [esp + 0x10]
        fmul dword ptr [dat_004e138c]
        fld dword ptr [esp + 8]
        fmul dword ptr [dat_004e1530]
        fadd st(0), st(2)
        fld dword ptr [esp + 0xc]
        fmul dword ptr [dat_004e1530]
        fsubp st(1), st(0)
        fadd st(0), st(1)
        fmul dword ptr [ecx + 8]
        fld dword ptr [esp + 8]
        fmul dword ptr [dat_004e1eb8]
        fsubr dword ptr [esp + 4]
        fld dword ptr [esp + 0xc]
        fadd st(0), st(0)
        faddp st(1), st(0)
        fsub st(0), st(2)
        fmul dword ptr [ecx + 4]
        faddp st(1), st(0)
        fld dword ptr [esp + 0xc]
        fmul dword ptr [dat_004e138c]
        fadd st(0), st(3)
        fmul dword ptr [ecx]
        faddp st(1), st(0)
        fadd dword ptr [esp + 8]
        fstp st(2)
        fstp st(0)
        ret 0x10
    }
}

class ModelNameState {
public:
    void __fastcall IncrementField64Exact();
    void ConstructExact(char* name);
    void InitializeNameExact(char* name);
    void ConstructWithValueExact(char* name, unsigned int value, int useDefaults);
    void DestroyStateExact();
    void InitializeResourceExact(int mode);
    void InitializeAlphaResourceExact();
    void SetResourceReadyExact();
    void ForwardResourceCommandExact(int first, int second, int third);
    void* RequestResourceStateExact();
    void CompleteResourceStateExact();
    ModelNameState* DeleteStateExact(unsigned int flags);
};

extern "C" void* dat_004de966;
extern "C" void* PTR_FUN_004e2970;
extern "C" void fun_004aaca0();
extern "C" void fun_004a8160();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a7fd0
// original: FUN_004a7fd0
void ModelNameState::ConstructExact(char*)
{
    __asm {
        push -1
        push offset dat_004de966
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        mov esi, ecx
        lea ecx, [esi + 4]
        mov dword ptr [esp + 4], esi
        call fun_0040c6f0
        lea ecx, [esi + 0x2c]
        mov dword ptr [esp + 0x10], 0
        call fun_004aaca0
        mov eax, dword ptr [esp + 0x18]
        push eax
        mov ecx, esi
        mov byte ptr [esp + 0x14], 1
        mov dword ptr [esi], offset PTR_FUN_004e2970
        call fun_004a8160
        mov ecx, dword ptr [esp + 8]
        mov eax, esi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret 4
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a81b0
// original: FUN_004a81b0
void __fastcall ModelNameState::IncrementField64Exact()
{
    __asm {
        inc dword ptr [ecx + 0x64]
        ret
    }
}

extern "C" void fun_004bc8c0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8160
// original: FUN_004a8160
void ModelNameState::InitializeNameExact(char*)
{
    __asm {
        mov eax, dword ptr [esp + 4]
        push esi
        mov esi, ecx
        push eax
        lea ecx, [esi + 4]
        call fun_004bc8c0
        xor eax, eax
        mov ecx, 1
        mov dword ptr [esi + 8], eax
        mov dword ptr [esi + 0xc], eax
        mov dword ptr [esi + 0x50], ecx
        mov dword ptr [esi + 0x14], eax
        mov dword ptr [esi + 0x20], eax
        mov dword ptr [esi + 0x24], eax
        mov dword ptr [esi + 0x28], eax
        mov dword ptr [esi + 0x4c], eax
        mov dword ptr [esi + 0x10], ecx
        mov dword ptr [esi + 0x60], eax
        mov dword ptr [esi + 0x58], eax
        mov dword ptr [esi + 0x5c], eax
        mov dword ptr [esi + 0x64], ecx
        pop esi
        ret 4
    }
}

extern "C" void* dat_004de986;
extern "C" void fun_004a81c0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8030
// original: FUN_004a8030
void ModelNameState::ConstructWithValueExact(char*, unsigned int, int)
{
    __asm {
        push -1
        push offset dat_004de986
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        mov esi, ecx
        push edi
        lea ecx, [esi + 4]
        mov dword ptr [esp + 8], esi
        call fun_0040c6f0
        lea ecx, [esi + 0x2c]
        mov dword ptr [esp + 0x14], 0
        call fun_004aaca0
        mov edi, dword ptr [esp + 0x1c]
        push edi
        mov ecx, esi
        mov byte ptr [esp + 0x18], 1
        mov dword ptr [esi], offset PTR_FUN_004e2970
        call fun_004a8160
        mov eax, dword ptr [esp + 0x24]
        test eax, eax
        jnz value_ctor_default
        push edi
        mov ecx, esi
        call fun_004a81c0
        mov eax, dword ptr [esp + 0x20]
        mov dword ptr [esi + 8], eax
        jmp value_ctor_done
    value_ctor_default:
        mov ecx, dword ptr [esp + 0x20]
        mov dword ptr [esi + 8], ecx
    value_ctor_done:
        mov ecx, dword ptr [esp + 0xc]
        pop edi
        mov eax, esi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret 0xc
    }
}

extern "C" void* dat_004de9a6;
extern "C" void fun_004aafa0();
extern "C" void fun_004bc8b0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a80c0
// original: FUN_004a80c0
void ModelNameState::DestroyStateExact()
{
    __asm {
        push -1
        push offset dat_004de9a6
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        mov esi, ecx
        push edi
        mov dword ptr [esp + 8], esi
        mov dword ptr [esi], offset PTR_FUN_004e2970
        mov eax, dword ptr [esi + 0x24]
        test eax, eax
        mov ecx, 1
        mov dword ptr [esp + 0x14], ecx
        jz state_dtor_callback_done
        test byte ptr [esi + 0xc], cl
        jz state_dtor_callback_done
        mov eax, dword ptr [eax + 0x20]
        mov edx, dword ptr [eax]
        push esi
        mov ecx, eax
        call dword ptr [edx + 0x2c]
    state_dtor_callback_done:
        mov edi, dword ptr [esi + 0x58]
        test edi, edi
        jz state_dtor_owned_string_done
        mov ecx, edi
        call fun_004aafa0
        push edi
        call fun_004bd000
        add esp, 4
    state_dtor_owned_string_done:
        lea ecx, [esi + 0x2c]
        mov dword ptr [esi + 0x44], 0
        mov dword ptr [esi + 0x48], 0
        mov byte ptr [esp + 0x14], 0
        call fun_004aafa0
        lea ecx, [esi + 4]
        mov dword ptr [esp + 0x14], 0xffffffff
        call fun_004bc8b0
        mov ecx, dword ptr [esp + 0xc]
        pop edi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

extern "C" void* dat_004de9bb;
extern "C" void fun_004aacd0();
extern "C" void fun_004ab010();
extern "C" void fun_004a82d0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a81c0
// original: FUN_004a81c0
void ModelNameState::InitializeResourceExact(int)
{
    __asm {
        push -1
        push offset dat_004de9bb
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 8
        push ebx
        mov ebx, ecx
        mov eax, dword ptr [ebx + 0x28]
        test eax, eax
        mov dword ptr [esp + 4], ebx
        jnz resource_already_initialized
        push ebp
        push esi
        push edi
        push 0x20
        call fun_004c1276
        add esp, 4
        mov dword ptr [esp + 0x14], eax
        test eax, eax
        mov dword ptr [esp + 0x20], 0
        jz resource_unconstructed
        mov ecx, dword ptr [esp + 0x28]
        push ecx
        mov ecx, eax
        call fun_004aacd0
        mov esi, eax
        jmp resource_constructed
    resource_unconstructed:
        xor esi, esi
    resource_constructed:
        mov ebp, dword ptr [ebx + 0x60]
        test ebp, ebp
        mov dword ptr [esp + 0x20], 0xffffffff
        mov dword ptr [ebx + 0x28], esi
        jz resource_adjust_done
        cmp dword ptr [esi + 0x14], 5
        jl resource_adjust_done
        push 7
        mov ecx, esi
        call fun_004ab010
        mov eax, dword ptr [esi + 4]
        test eax, eax
        mov edi, dword ptr [esi + 0x18]
        mov dword ptr [esp + 0x28], 0
        jle resource_adjust_done
        mov ebx, dword ptr [esi]
    resource_pixel_test:
        test ebx, ebx
        jle resource_next_row
        lea ecx, [edi + 3]
        mov edx, ebx
    resource_channel_loop:
        xor eax, eax
        mov al, byte ptr [ecx]
        add eax, ebp
        cmp eax, 0xff
        jbe resource_channel_saturated
        mov eax, 0xff
    resource_channel_saturated:
        mov byte ptr [ecx], al
        add ecx, 4
        dec edx
        jnz resource_channel_loop
    resource_next_row:
        mov eax, dword ptr [esi + 8]
        mov ecx, dword ptr [esi + 4]
        add edi, eax
        mov eax, dword ptr [esp + 0x28]
        inc eax
        cmp eax, ecx
        mov dword ptr [esp + 0x28], eax
        jl resource_pixel_test
        mov ebx, dword ptr [esp + 0x10]
    resource_adjust_done:
        mov eax, dword ptr [ebx + 0x5c]
        test eax, eax
        pop edi
        pop esi
        pop ebp
        jz resource_already_initialized
        mov ecx, ebx
        call fun_004a82d0
    resource_already_initialized:
        mov ecx, dword ptr [esp + 0xc]
        pop ebx
        mov dword ptr fs:[0], ecx
        add esp, 0x14
        ret 4
    }
}

extern "C" void fun_004aae80();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a82d0
// original: FUN_004a82d0
void ModelNameState::InitializeAlphaResourceExact()
{
    __asm {
        push -1
        push 0x4de9db
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        push edi
        mov edi, ecx
        mov eax, dword ptr [edi + 0x58]
        test eax, eax
        jnz alpha_resource_done
        mov esi, dword ptr [edi + 0x28]
        cmp dword ptr [esi + 0x14], 5
        jl alpha_resource_done
        push 0x20
        call fun_004c1276
        add esp, 4
        mov dword ptr [esp + 8], eax
        test eax, eax
        mov dword ptr [esp + 0x14], 0
        jz alpha_resource_unconstructed
        push 1
        push esi
        mov ecx, eax
        call fun_004aae80
        jmp alpha_resource_constructed
    alpha_resource_unconstructed:
        xor eax, eax
    alpha_resource_constructed:
        push 0xa
        mov ecx, eax
        mov dword ptr [esp + 0x18], 0xffffffff
        mov dword ptr [edi + 0x58], eax
        call fun_004ab010
    alpha_resource_done:
        mov ecx, dword ptr [esp + 0xc]
        pop edi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

extern "C" void fun_004a82d0();
extern "C" void fun_004a80c0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a82b0
// original: FUN_004a82b0
void ModelNameState::SetResourceReadyExact()
{
    __asm {
        mov eax, dword ptr [ecx + 0x28]
        test eax, eax
        mov dword ptr [ecx + 0x5c], 1
        jz ready_flag_done
        jmp fun_004a82d0
    ready_flag_done:
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8490
// original: FUN_004a8490
void ModelNameState::ForwardResourceCommandExact(int, int, int)
{
    __asm {
        mov eax, dword ptr [ecx + 0x24]
        test eax, eax
        jz forward_done
        mov eax, dword ptr [eax + 0x20]
        mov edx, dword ptr [eax]
        push esi
        mov esi, dword ptr [esp + 0x10]
        push esi
        mov esi, dword ptr [esp + 0x10]
        push esi
        mov esi, dword ptr [esp + 0x10]
        push esi
        push ecx
        mov ecx, eax
        call dword ptr [edx + 0x24]
        pop esi
    forward_done:
        ret 0xc
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a84c0
// original: FUN_004a84c0
void* ModelNameState::RequestResourceStateExact()
{
    __asm {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 0x24]
        test eax, eax
        push edi
        jz resource_request_failed
        test byte ptr [esi + 0xc], 1
        jz resource_request_failed
        mov ecx, dword ptr [eax + 0x20]
        mov eax, dword ptr [ecx]
        lea edi, [esi + 0x2c]
        push edi
        push esi
        mov dword ptr [esi + 0x44], 0
        call dword ptr [eax + 0x44]
        mov eax, dword ptr [esi + 0x44]
        test eax, eax
        jz resource_request_failed
        mov eax, edi
        pop edi
        mov dword ptr [esi + 0x4c], 1
        pop esi
        ret
    resource_request_failed:
        pop edi
        xor eax, eax
        pop esi
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8500
// original: FUN_004a8500
void ModelNameState::CompleteResourceStateExact()
{
    __asm {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 0x4c]
        test eax, eax
        jz resource_complete_done
        mov eax, dword ptr [esi + 0x24]
        test eax, eax
        jz resource_complete_done
        test byte ptr [esi + 0xc], 1
        jz resource_complete_done
        mov ecx, dword ptr [eax + 0x20]
        mov eax, dword ptr [ecx]
        push esi
        call dword ptr [eax + 0x48]
        mov dword ptr [esi + 0x4c], 0
        mov dword ptr [esi + 0x44], 0
    resource_complete_done:
        pop esi
        ret
    }
}

extern "C" void fun_004bd000();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8560
// original: FUN_004a8560
ModelNameState* ModelNameState::DeleteStateExact(unsigned int)
{
    __asm {
        push esi
        mov esi, ecx
        call fun_004a80c0
        test byte ptr [esp + 8], 1
        jz state_delete_done
        push esi
        call fun_004bd000
        add esp, 4
    state_delete_done:
        mov eax, esi
        pop esi
        ret 4
    }
}

class ModelResourceState {
public:
    ModelResourceState* ConstructExact();
    ModelResourceState* __fastcall ConstructFromNameExact(int mode, char* name);
    ModelResourceState* CopyAssignExact(ModelResourceState* source);
    void InitializeDefaultsExact();
    void DestroyStringExact();
};

class ModelResourceArrayOwner {
public:
    ModelResourceArrayOwner* ConstructExact(int, int, int, int);
    void __fastcall DestroyArraysExact(unsigned int mode);
    void __fastcall PopulateResourceGridExact(int scale, int mode);
    void SetEntryParametersExact(int first, int second);
    void __fastcall DrawResourceCellsExact(int state, int texture, int flags);
};

extern "C" void* dat_004dea46;
extern "C" void* dat_004deaf8;
extern "C" void* dat_004deaa6;
extern "C" void* dat_004deb61;
extern "C" void fun_004a8710();
extern "C" void fun_004a8580();
extern "C" void fun_004a86d0();
extern "C" void fun_004aa9c0();
extern "C" void fun_004a9660();
extern "C" void fun_004a7fd0();
extern "C" void fun_004a97f0();
extern "C" void fun_004c2431();
extern "C" void fun_004b7670();
extern "C" void fun_004aa690();
extern "C" void fun_004c2414();
extern "C" void fun_004aa5a0();
extern "C" void fun_00402700();
extern "C" void fun_00402ab0();
extern "C" void fun_004bd00b();
extern "C" void fun_004c1116();
extern "C" void fun_004c11f4();
extern "C" void fun_00402c90();
extern "C" void fun_00490ad0();
extern "C" void fun_004a8cc0();
extern "C" void* dat_004deb31;
extern "C" void* dat_004dec23;
extern "C" void* dat_004dec57;
extern "C" void* dat_004decbb;
extern "C" void* dat_004ded06;
extern "C" char DAT_0057a7d8[];
extern double DAT_004e2958;
extern float DAT_00579bd0;
extern float DAT_00579bd4;
extern "C" float dat_004e138c;
extern "C" float dat_004e2798;
extern "C" void* dat_004debfe;
extern "C" void* dat_004e1340;
extern "C" void* dat_004e1344;
extern "C" void fun_004bd237();
extern "C" void fun_00402bd0();
extern "C" void fun_004c14e0();
extern "C" void fun_00402980();
extern "C" void fun_00402960();
extern "C" void fun_004c1276();
extern "C" void fun_004aacd0();
extern "C" void fun_004ac920();
extern "C" void fun_004aad10();
extern "C" void fun_004aafc0();
extern "C" void fun_004ad110();
extern "C" void fun_004a7fd0();
extern "C" void fun_004aafa0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8580
// original: FUN_004a8580
ModelResourceState* ModelResourceState::ConstructExact()
{
    __asm {
        push -1
        push offset dat_004dea46
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 8
        push esi
        mov esi, ecx
        mov dword ptr [esp + 8], esi
        xor eax, eax
        mov dword ptr [esp + 0x14], eax
        mov cl, byte ptr [esp + 7]
        mov byte ptr [esi + 0xa4], cl
        mov dword ptr [esi + 0xa8], eax
        mov dword ptr [esi + 0xac], eax
        mov dword ptr [esi + 0xb0], eax
        mov ecx, esi
        mov byte ptr [esp + 0x14], 6
        call fun_004a8710
        mov ecx, dword ptr [esp + 0xc]
        mov eax, esi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x14
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a85e0
// original: FUN_004a85e0
ModelResourceState* __fastcall ModelResourceState::ConstructFromNameExact(int, char*)
{
    __asm {
        push -1
        push offset dat_004deaa6
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov dword ptr [esp + 0x10], ecx
        xor esi, esi
        mov dword ptr [esp + 0x1c], esi
        mov al, byte ptr [esp + 0x24]
        lea ebp, [ecx + 0xa4]
        mov byte ptr [ebp], al
        mov dword ptr [ebp + 4], esi
        mov dword ptr [ebp + 8], esi
        mov dword ptr [ebp + 0xc], esi
        mov edi, dword ptr [esp + 0x24]
        or ecx, -1
        xor eax, eax
        repne scasb
        not ecx
        dec ecx
        mov ebx, ecx
        cmp ebx, -3
        mov byte ptr [esp + 0x1c], 6
        jbe from_name_length_ready
        call fun_004bd00b
    from_name_length_ready:
        mov eax, dword ptr [ebp + 4]
        cmp eax, esi
        jz from_name_empty_or_unshared
        mov cl, byte ptr [eax - 1]
        test cl, cl
        jz from_name_empty_or_unshared
        cmp cl, 0xff
        jz from_name_empty_or_unshared
        cmp ebx, esi
        jnz from_name_assign
        dec cl
        mov byte ptr [eax - 1], cl
        push esi
        mov ecx, ebp
        call fun_00402700
        jmp from_name_initialized
    from_name_empty_or_unshared:
        cmp ebx, esi
        jnz from_name_check_capacity
        push 1
        mov ecx, ebp
        call fun_00402700
        jmp from_name_initialized
    from_name_check_capacity:
        mov eax, dword ptr [ebp + 0xc]
        cmp eax, 0x1f
        ja from_name_release_old
        cmp eax, ebx
        jnc from_name_copy
    from_name_release_old:
        push 1
        mov ecx, ebp
        call fun_00402700
    from_name_assign:
        push ebx
        mov ecx, ebp
        call fun_00402ab0
    from_name_copy:
        mov eax, dword ptr [ebp + 4]
        mov esi, dword ptr [esp + 0x24]
        mov ecx, ebx
        mov edx, ecx
        shr ecx, 2
        mov edi, eax
        rep movsd
        mov ecx, edx
        and ecx, 3
        rep movsb
        mov dword ptr [ebp + 8], ebx
        mov byte ptr [eax + ebx], 0
    from_name_initialized:
        mov esi, dword ptr [esp + 0x10]
        mov ecx, esi
        call fun_004a8710
        mov ecx, dword ptr [esp + 0x14]
        pop edi
        mov eax, esi
        pop esi
        pop ebp
        pop ebx
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret 4
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8710
// original: FUN_004a8710
void ModelResourceState::InitializeDefaultsExact()
{
    __asm {
        xor eax, eax
        mov edx, 1
        mov dword ptr [ecx], eax
        mov dword ptr [ecx + 4], eax
        mov dword ptr [ecx + 8], eax
        mov dword ptr [ecx + 0x1c], edx
        mov dword ptr [ecx + 0x24], edx
        mov dword ptr [ecx + 0x2c], edx
        mov dword ptr [ecx + 0x34], edx
        mov edx, 0x3f800000
        mov dword ptr [ecx + 0x40], edx
        mov dword ptr [ecx + 0x44], edx
        mov dword ptr [ecx + 0x48], edx
        mov dword ptr [ecx + 0x4c], edx
        mov dword ptr [ecx + 0x50], eax
        mov dword ptr [ecx + 0x54], eax
        mov dword ptr [ecx + 0x58], eax
        mov dword ptr [ecx + 0x5c], eax
        mov dword ptr [ecx + 0x70], edx
        mov dword ptr [ecx + 0x74], edx
        mov dword ptr [ecx + 0x78], edx
        mov dword ptr [ecx + 0x7c], edx
        mov dword ptr [ecx + 0x60], eax
        mov dword ptr [ecx + 0x64], eax
        mov dword ptr [ecx + 0x68], eax
        mov dword ptr [ecx + 0x6c], eax
        mov dword ptr [ecx + 0x10], eax
        mov dword ptr [ecx + 0x14], eax
        mov dword ptr [ecx + 0x18], 2
        mov dword ptr [ecx + 0x20], 4
        mov dword ptr [ecx + 0x28], 3
        mov dword ptr [ecx + 0x30], 3
        mov dword ptr [ecx + 0x38], eax
        mov dword ptr [ecx + 0x3c], eax
        mov dword ptr [ecx + 0x80], eax
        mov dword ptr [ecx + 0x84], -1
        mov dword ptr [ecx + 0x88], eax
        mov dword ptr [ecx + 0x8c], eax
        mov dword ptr [ecx + 0x90], eax
        mov dword ptr [ecx + 0x94], eax
        mov dword ptr [ecx + 0x98], 0xb
        mov dword ptr [ecx + 0x9c], eax
        mov dword ptr [ecx + 0xb4], eax
        mov dword ptr [ecx + 0xa0], eax
        mov dword ptr [ecx + 0xc4], eax
        mov dword ptr [ecx + 0xc8], eax
        mov dword ptr [ecx + 0xcc], eax
        mov dword ptr [ecx + 0xd0], eax
        mov dword ptr [ecx + 0xc0], eax
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a86d0
// original: FUN_004a86d0
void ModelResourceState::DestroyStringExact()
{
    __asm {
        push -1
        push offset dat_004deaf8
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        __emit 0x89
        __emit 0x4c
        __emit 0x24
        __emit 0x00
        push 1
        add ecx, 0xa4
        mov dword ptr [esp + 0x10], 5
        call fun_00402700
        mov ecx, dword ptr [esp + 4]
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8bb0
// original: FUN_004a8bb0
void __fastcall ModelResourceArrayOwner::DestroyArraysExact(unsigned int)
{
    __asm {
        push -1
        push offset dat_004deb61
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        mov esi, ecx
        push edi
        mov dword ptr [esp + 8], esi
        mov eax, dword ptr [esi + 0x18]
        test eax, eax
        mov dword ptr [esp + 0x14], 1
        jle resource_array_free_data
        mov edi, dword ptr [esi + 0x1c]
        push ebx
        mov ebx, eax
    resource_array_release_items:
        mov ecx, dword ptr [edi]
        test ecx, ecx
        jz resource_array_next_item
        mov eax, dword ptr [ecx]
        push 1
        call dword ptr [eax]
    resource_array_next_item:
        add edi, 0xd4
        dec ebx
        jnz resource_array_release_items
        pop ebx
    resource_array_free_data:
        mov eax, dword ptr [esi + 0x1c]
        test eax, eax
        jz resource_array_free_field20
        mov ecx, dword ptr [eax - 4]
        lea edi, [eax - 4]
        push offset fun_004a86d0
        push ecx
        push 0xd4
        push eax
        call fun_004c1116
        push edi
        call fun_004bd000
        add esp, 4
    resource_array_free_field20:
        mov eax, dword ptr [esi + 0x20]
        test eax, eax
        jz resource_array_free_field24
        mov edx, dword ptr [eax - 4]
        lea edi, [eax - 4]
        push offset fun_00402c90
        push edx
        push 8
        push eax
        call fun_004c1116
        push edi
        call fun_004bd000
        add esp, 4
    resource_array_free_field24:
        mov eax, dword ptr [esi + 0x24]
        test eax, eax
        jz resource_array_free_field28
        mov ecx, dword ptr [eax - 4]
        lea edi, [eax - 4]
        push offset fun_00402c90
        push ecx
        push 8
        push eax
        call fun_004c1116
        push edi
        call fun_004bd000
        add esp, 4
    resource_array_free_field28:
        mov eax, dword ptr [esi + 0x28]
        test eax, eax
        jz resource_array_free_tail
        mov edx, dword ptr [eax - 4]
        lea edi, [eax - 4]
        push offset fun_00402c90
        push edx
        push 8
        push eax
        call fun_004c1116
        push edi
        call fun_004bd000
        add esp, 4
    resource_array_free_tail:
        push offset fun_00402c90
        push 2
        push 8
        add esi, 4
        push esi
        mov dword ptr [esp + 0x24], -1
        call fun_004c1116
        mov ecx, dword ptr [esp + 0xc]
        pop edi
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8ae0
// original: FUN_004a8ae0
ModelResourceArrayOwner* ModelResourceArrayOwner::ConstructExact(int, int, int, int)
{
    __asm {
        push -1
        push offset dat_004deb31
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push ebx
        push esi
        push edi
        push offset fun_00402c90
        push offset fun_00490ad0
        mov esi, ecx
        push 2
        push 8
        lea edi, [esi + 4]
        push edi
        mov dword ptr [esp + 0x20], esi
        call fun_004c11f4
        mov eax, dword ptr [esp + 0x20]
        fild dword ptr [eax + 4]
        mov ebx, dword ptr [esp + 0x2c]
        mov edx, 0x3f800000
        mov dword ptr [esi + 0x38], edx
        fsub dword ptr dat_004e138c
        xor ecx, ecx
        mov dword ptr [esi + 0x14], eax
        fstp dword ptr [esi + 0xc]
        mov dword ptr [esi + 0x40], ebx
        fild dword ptr [eax + 8]
        mov eax, dword ptr [esp + 0x24]
        cmp eax, ecx
        mov ebx, dword ptr [esp + 0x28]
        fsub dword ptr dat_004e138c
        mov dword ptr [esi + 0x2c], edx
        mov dword ptr [esi + 0x30], edx
        mov dword ptr [esi + 0x34], edx
        fstp dword ptr [esi + 0x10]
        mov dword ptr [esp + 0x18], 1
        mov dword ptr [esi + 0x18], ecx
        mov dword ptr [esi + 0x1c], ecx
        mov dword ptr [esi + 0x20], ecx
        mov dword ptr [esi + 0x24], ecx
        mov dword ptr [esi + 0x28], ecx
        mov dword ptr [esi + 0x3c], edx
        mov dword ptr [esi + 0x44], ebx
        mov dword ptr [esi], edx
        mov dword ptr [edi], 0xbf000000
        mov dword ptr [esi + 8], 0xbf000000
        jz resource_owner_constructor_done
        push eax
        mov ecx, esi
        call fun_004a8cc0
    resource_owner_constructor_done:
        mov ecx, dword ptr [esp + 0x10]
        pop edi
        mov eax, esi
        pop esi
        pop ebx
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret 0x10
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a8cc0
// original: FUN_004a8cc0
void __fastcall ModelResourceArrayOwner::PopulateResourceGridExact(int, int)
{
    __asm {
        push -0x1
        push 0x4debbd
        mov eax, fs:[0x0]
        push eax
        mov dword ptr fs:[0x0], esp
        sub esp, 0x6c
        push ebx
        push ebp
        push esi
        push edi
        mov edi, ecx
        push 0x20
        mov dword ptr [esp + 0x64], edi
        call fun_004c1276
        add esp, 0x4
        mov dword ptr [esp + 0x78], eax
        xor esi, esi
        cmp eax, esi
        mov dword ptr [esp + 0x84], esi
        jz loc_4a8d14
        mov ecx, dword ptr [esp + 0x8c]
        push ecx
        mov ecx, eax
        call fun_004aacd0
        mov ebx, eax
        mov dword ptr [esp + 0x10], ebx
        jmp loc_4a8d1a
loc_4a8d14:
        mov dword ptr [esp + 0x10], esi
        mov ebx, esi
loc_4a8d1a:
        cmp dword ptr [edi + 0x40], esi
        mov dword ptr [esp + 0x84], 0xffffffff
        jz loc_4a8d4f
        mov eax, dword ptr [edi + 0x14]
        mov edx, dword ptr [eax + 0x4]
        mov ecx, dword ptr [ebx]
        cmp ecx, edx
        mov eax, dword ptr [eax + 0x8]
        mov esi, dword ptr [ebx + 0x4]
        jle loc_4a8d3e
        mov ecx, edx
loc_4a8d3e:
        cmp esi, eax
        jle loc_4a8d44
        mov esi, eax
loc_4a8d44:
        push -0x1
        push esi
        push ecx
        mov ecx, ebx
        call fun_004ac920
loc_4a8d4f:
        mov edx, dword ptr [edi + 0x14]
        mov ecx, dword ptr [edx + 0x20]
        mov eax, dword ptr [ebx]
        mov dword ptr [esp + 0x30], eax
        mov edx, dword ptr [ebx + 0x4]
        mov dword ptr [esp + 0x28], edx
        mov eax, dword ptr [ecx]
        push 0x1
        lea edx, [esp + 0x2c]
        push edx
        lea edx, [esp + 0x38]
        push edx
        mov dword ptr [esp + 0x60], ecx
        call dword ptr [eax + 0xc]
        mov ecx, dword ptr [ebx]
        mov ebp, dword ptr [esp + 0x30]
        mov eax, ecx
        cdq
        idiv ebp
        mov eax, ecx
        mov ebx, dword ptr [ebx + 0x4]
        mov dword ptr [esp + 0x18], ecx
        mov esi, edx
        cdq
        neg esi
        sbb esi, esi
        idiv ebp
        mov ebp, dword ptr [esp + 0x28]
        neg esi
        add esi, eax
        mov eax, ebx
        cdq
        idiv ebp
        mov eax, ebx
        mov ecx, edx
        neg ecx
        sbb ecx, ecx
        cdq
        idiv ebp
        neg ecx
        add ecx, eax
        imul ecx, esi
        mov esi, ecx
        lea eax, [esi + esi*0x2]
        lea eax, [eax + eax*0x8]
        shl eax, 0x1
        sub eax, esi
        lea ecx, [eax*0x4 + 0x4]
        push ecx
        mov dword ptr [esp + 0x7c], esi
        call fun_004c1276
        add esp, 0x4
        mov dword ptr [esp + 0x74], eax
        test eax, eax
        mov dword ptr [esp + 0x84], 0x1
        jz loc_4a8e07
        push offset fun_004a86d0
        push offset fun_004a8580
        push esi
        lea ebp, [eax + 0x4]
        push 0xd4
        push ebp
        mov dword ptr [eax], esi
        call fun_004c11f4
        mov dword ptr [esp + 0x2c], ebp
        jmp loc_4a8e0f
loc_4a8e07:
        mov dword ptr [esp + 0x2c], 0x0
loc_4a8e0f:
        mov edx, dword ptr [esp + 0x2c]
        lea eax, [esi*0x8 + 0x4]
        push eax
        mov dword ptr [esp + 0x88], 0xffffffff
        mov dword ptr [edi + 0x1c], edx
        call fun_004c1276
        add esp, 0x4
        mov dword ptr [esp + 0x74], eax
        test eax, eax
        mov dword ptr [esp + 0x84], 0x2
        jz loc_4a8e62
        push offset fun_00402c90
        push offset fun_00490ad0
        push esi
        lea ebp, [eax + 0x4]
        push 0x8
        push ebp
        mov dword ptr [eax], esi
        call fun_004c11f4
        mov dword ptr [esp + 0x40], ebp
        jmp loc_4a8e6e
loc_4a8e62:
        mov dword ptr [esp + 0x40], 0x0
        mov ebp, dword ptr [esp + 0x40]
loc_4a8e6e:
        lea eax, [esi*0x8 + 0x4]
        push eax
        mov dword ptr [esp + 0x88], 0xffffffff
        mov dword ptr [edi + 0x20], ebp
        call fun_004c1276
        add esp, 0x4
        mov dword ptr [esp + 0x74], eax
        test eax, eax
        mov dword ptr [esp + 0x84], 0x3
        jz loc_4a8ec5
        push offset fun_00402c90
        push offset fun_00490ad0
        push esi
        mov dword ptr [eax], esi
        add eax, 0x4
        push 0x8
        push eax
        mov dword ptr [esp + 0x70], eax
        call fun_004c11f4
        mov eax, dword ptr [esp + 0x5c]
        mov dword ptr [esp + 0x20], eax
        jmp loc_4a8ecd
loc_4a8ec5:
        mov dword ptr [esp + 0x20], 0x0
loc_4a8ecd:
        mov ecx, dword ptr [esp + 0x20]
        lea eax, [esi*0x8 + 0x4]
        push eax
        mov dword ptr [esp + 0x88], 0xffffffff
        mov dword ptr [edi + 0x24], ecx
        call fun_004c1276
        add esp, 0x4
        mov dword ptr [esp + 0x74], eax
        test eax, eax
        mov dword ptr [esp + 0x84], 0x4
        jz loc_4a8f20
        push offset fun_00402c90
        push offset fun_00490ad0
        push esi
        lea edi, [eax + 0x4]
        push 0x8
        push edi
        mov dword ptr [eax], esi
        call fun_004c11f4
        mov dword ptr [esp + 0x44], edi
        jmp loc_4a8f2c
loc_4a8f20:
        mov dword ptr [esp + 0x44], 0x0
        mov edi, dword ptr [esp + 0x44]
loc_4a8f2c:
        mov ecx, dword ptr [esp + 0x60]
        mov eax, dword ptr [ecx + 0x44]
        test eax, eax
        mov dword ptr [esp + 0x84], 0xffffffff
        mov dword ptr [ecx + 0x28], edi
        mov dword ptr [esp + 0x58], 0x41
        jz loc_4a8f55
        mov dword ptr [esp + 0x58], 0x841
loc_4a8f55:
        xor eax, eax
        xor edx, edx
        test ebx, ebx
        mov dword ptr [esp + 0x48], eax
        mov dword ptr [esp + 0x24], edx
        jle loc_4a924c
loc_4a8f69:
        sub ebx, edx
        mov ecx, ebx
        mov ebx, dword ptr [esp + 0x28]
        cmp ecx, ebx
        mov dword ptr [esp + 0x14], 0x0
        mov dword ptr [esp + 0x1c], ecx
        jle loc_4a8f87
        mov dword ptr [esp + 0x1c], ebx
        mov ecx, ebx
loc_4a8f87:
        mov ebx, dword ptr [esp + 0x18]
        test ebx, ebx
        jle loc_4a9233
        add edx, ecx
        lea esi, [ecx - 0x1]
        mov dword ptr [esp + 0x5c], esi
        fild dword ptr [esp + 0x5c]
        mov esi, dword ptr [esp + 0x2c]
        mov dword ptr [esp + 0x5c], edx
        lea edx, [eax + eax*0x2]
        fstp dword ptr [esp + 0x68]
        fild dword ptr [esp + 0x24]
        lea edx, [edx + edx*0x8]
        shl edx, 0x1
        sub edx, eax
        lea esi, [esi + edx*0x4]
        fstp dword ptr [esp + 0x6c]
        mov edx, dword ptr [esp + 0x20]
        fild dword ptr [esp + 0x5c]
        lea edx, [edx + eax*0x8 + 0x4]
        lea ebx, [ebp + eax*0x8]
        lea eax, [edi + eax*0x8 + 0x4]
        fstp dword ptr [esp + 0x74]
        mov dword ptr [esp + 0x38], eax
        mov eax, edi
        sub eax, ebp
        mov dword ptr [esp + 0x64], eax
        mov eax, dword ptr [esp + 0x20]
        mov dword ptr [esp + 0x34], esi
        add esi, 0x68
        sub eax, ebp
        mov dword ptr [esp + 0x3c], edx
        mov dword ptr [esp + 0x70], eax
        jmp loc_4a9000
loc_4a8ffc:
        mov ecx, dword ptr [esp + 0x1c]
loc_4a9000:
        mov ebp, dword ptr [esp + 0x18]
        mov edi, dword ptr [esp + 0x14]
        mov eax, dword ptr [esp + 0x30]
        sub ebp, edi
        cmp ebp, eax
        jle loc_4a9014
        mov ebp, eax
loc_4a9014:
        push 0x0
        lea eax, [esp + 0x50]
        push eax
        mov dword ptr [esp + 0x54], ecx
        mov ecx, dword ptr [esp + 0x5c]
        mov edx, dword ptr [ecx]
        lea eax, [esp + 0x58]
        push eax
        mov dword ptr [esp + 0x5c], ebp
        call dword ptr [edx + 0xc]
        push 0x20
        call fun_004c1276
        add esp, 0x4
        mov dword ptr [esp + 0x5c], eax
        test eax, eax
        mov dword ptr [esp + 0x84], 0x5
        jz loc_4a906b
        mov ecx, dword ptr [esp + 0x10]
        mov edx, dword ptr [ecx + 0x14]
        mov ecx, dword ptr [esp + 0x4c]
        push edx
        mov edx, dword ptr [esp + 0x54]
        push ecx
        push edx
        mov ecx, eax
        call fun_004aad10
        mov edi, eax
        jmp loc_4a906d
loc_4a906b:
        xor edi, edi
loc_4a906d:
        mov ecx, edi
        mov dword ptr [esp + 0x84], 0xffffffff
        call fun_004aafc0
        mov eax, dword ptr [esp + 0x1c]
        mov ecx, dword ptr [esp + 0x24]
        mov edx, dword ptr [esp + 0x14]
        push eax
        mov eax, dword ptr [esp + 0x14]
        push ebp
        push ecx
        push edx
        push 0x0
        push 0x0
        push eax
        mov ecx, edi
        call fun_004ad110
        push 0x68
        call fun_004c1276
        add esp, 0x4
        mov dword ptr [esp + 0x5c], eax
        test eax, eax
        mov dword ptr [esp + 0x84], 0x6
        jz loc_4a90d1
        mov ecx, dword ptr [esp + 0x8c]
        push ecx
        mov ecx, eax
        call fun_004a7fd0
        mov dword ptr [esp + 0x18], eax
        jmp loc_4a90d9
loc_4a90d1:
        mov dword ptr [esp + 0x18], 0x0
loc_4a90d9:
        mov eax, dword ptr [esp + 0x58]
        mov ecx, dword ptr [esp + 0x54]
        mov edx, dword ptr [ecx]
        push 0x0
        push eax
        mov eax, dword ptr [esp + 0x20]
        push edi
        push eax
        mov dword ptr [esp + 0x94], 0xffffffff
        call dword ptr [edx + 0x10]
        mov ecx, dword ptr [esp + 0x34]
        mov dword ptr [ecx], eax
        mov dword ptr [esi + 0x34], 0xe
        mov dword ptr [esi + 0x30], 0xd
        mov eax, 0x3f800000
        mov dword ptr [esi - 0x8], eax
        mov dword ptr [esi - 0x4], eax
        mov dword ptr [esi], eax
        mov dword ptr [esi + 0x4], eax
        mov dword ptr [esi - 0x28], eax
        mov dword ptr [esi - 0x24], eax
        mov dword ptr [esi - 0x20], eax
        mov dword ptr [esi - 0x1c], eax
        mov eax, dword ptr [edi]
        xor ecx, ecx
        lea edx, [ebp - 0x1]
        mov dword ptr [esp + 0x5c], edx
        fild dword ptr [esp + 0x5c]
        dec eax
        mov dword ptr [esp + 0x5c], eax
        fidiv dword ptr [esp + 0x5c]
        mov eax, dword ptr [edi + 0x4]
        mov edx, dword ptr [esp + 0x64]
        dec eax
        mov dword ptr [esp + 0x5c], eax
        mov eax, dword ptr [esp + 0x10]
        mov dword ptr [esi + 0x8], ecx
        mov dword ptr [esi + 0xc], ecx
        mov dword ptr [esi + 0x10], ecx
        mov dword ptr [esi + 0x14], ecx
        mov dword ptr [esi - 0x44], 0x4
        fstp dword ptr [ebx + edx*0x1]
        fild dword ptr [esp + 0x5c]
        mov edx, dword ptr [esp + 0x38]
        fdivr dword ptr [esp + 0x68]
        fstp dword ptr [edx]
        mov edx, dword ptr [eax]
        mov dword ptr [esp + 0x18], edx
        fild dword ptr [esp + 0x18]
        mov edx, dword ptr [esp + 0x70]
        fild dword ptr [esp + 0x14]
        fdiv st(0), st(1)
        fstp dword ptr [ebx]
        fild dword ptr [eax + 0x4]
        mov eax, dword ptr [esp + 0x14]
        add ebp, eax
        cmp edi, ecx
        fstp dword ptr [esp + 0x5c]
        mov dword ptr [esp + 0x14], ebp
        fld dword ptr [esp + 0x6c]
        mov eax, dword ptr [esp + 0x3c]
        fdiv dword ptr [esp + 0x5c]
        fstp dword ptr [ebx + 0x4]
        fild dword ptr [esp + 0x14]
        fdiv st(0), st(1)
        fstp dword ptr [ebx + edx*0x1]
        fstp st(0)
        fld dword ptr [esp + 0x74]
        fdiv dword ptr [esp + 0x5c]
        fstp dword ptr [eax]
        jz loc_4a91d5
        mov ecx, edi
        call fun_004aafa0
        push edi
        call fun_004bd000
        add esp, 0x4
loc_4a91d5:
        mov ecx, dword ptr [esp + 0x34]
        mov edi, dword ptr [esp + 0x38]
        mov edx, dword ptr [esp + 0x48]
        add ecx, 0xd4
        mov eax, 0x8
        mov dword ptr [esp + 0x34], ecx
        mov ecx, dword ptr [esp + 0x3c]
        add edi, eax
        add ecx, eax
        add ebx, eax
        mov eax, dword ptr [esp + 0x18]
        inc edx
        add esi, 0xd4
        cmp ebp, eax
        mov dword ptr [esp + 0x14], ebp
        mov dword ptr [esp + 0x48], edx
        mov dword ptr [esp + 0x38], edi
        mov dword ptr [esp + 0x3c], ecx
        jl loc_4a8ffc
        mov ebp, dword ptr [esp + 0x40]
        mov esi, dword ptr [esp + 0x78]
        mov edi, dword ptr [esp + 0x44]
        mov ecx, dword ptr [esp + 0x1c]
        mov eax, edx
        mov edx, dword ptr [esp + 0x24]
loc_4a9233:
        add edx, ecx
        mov ecx, dword ptr [esp + 0x10]
        mov ebx, dword ptr [ecx + 0x4]
        cmp edx, ebx
        mov dword ptr [esp + 0x24], edx
        jl loc_4a8f69
        mov ecx, dword ptr [esp + 0x60]
loc_4a924c:
        mov dword ptr [ecx + 0x18], esi
        mov esi, dword ptr [esp + 0x10]
        test esi, esi
        jz loc_4a9267
        mov ecx, esi
        call fun_004aafa0
        push esi
        call fun_004bd000
        add esp, 0x4
loc_4a9267:
        mov ecx, dword ptr [esp + 0x7c]
        pop edi
        pop esi
        pop ebp
        pop ebx
        mov dword ptr fs:[0x0], ecx
        add esp, 0x78
        ret 0x4
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a9280
// original: FUN_004a9280
void ModelResourceArrayOwner::SetEntryParametersExact(int, int)
{
    __asm {
        mov edx, dword ptr [ecx + 0x18]
        test edx, edx
        jle entry_parameters_done
        mov eax, dword ptr [ecx + 0x1c]
        mov ecx, dword ptr [esp + 8]
        push esi
        mov esi, dword ptr [esp + 8]
        add eax, 0x1c
    entry_parameters_loop:
        cmp ecx, 1
        mov dword ptr [eax - 4], esi
        mov dword ptr [eax], ecx
        jz entry_parameters_skip_option
        mov dword ptr [eax + 8], 4
    entry_parameters_skip_option:
        add eax, 0xd4
        dec edx
        jnz entry_parameters_loop
        pop esi
    entry_parameters_done:
        ret 8
    }
}
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a92c0
// original: FUN_004a92c0
void __fastcall ModelResourceArrayOwner::DrawResourceCellsExact(int, int, int)
{
    __asm {
        push ebp
        mov ebp, esp
        push -0x1
        push offset dat_004debfe
        mov eax, fs:[0x0]
        push eax
        mov dword ptr fs:[0x0], esp
        sub esp, 0x48
        push ebx
        push esi
        push offset fun_00402c90
        push offset fun_00490ad0
        push 0x2
        push 0x8
        lea eax, [ebp - 0x54]
        push eax
        mov esi, ecx
        call fun_004c11f4
        push offset fun_00402c90
        push offset fun_00490ad0
        push 0x2
        push 0x8
        lea ecx, [ebp - 0x44]
        push ecx
        mov dword ptr [ebp - 0x4], 0x0
        call fun_004c11f4
        fld dword ptr [esi + 0x3c]
        fmul dword ptr [dat_004e2798]
        mov edx, dword ptr [esi]
        mov dword ptr [ebp - 0x4], 0x1
        mov dword ptr [ebp - 0x34], 0xffffffff
        fstp dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0x44], 0x0
        mov dword ptr [ebp - 0x40], 0x0
        mov dword ptr [ebp - 0x3c], 0x3f800000
        mov dword ptr [ebp - 0x38], 0x3f800000
        mov dword ptr [ebp - 0x30], edx
        fld dword ptr [ebp - 0x24]
        fistp dword ptr [ebp - 0x20]
        mov eax, dword ptr [ebp - 0x20]
        test eax, eax
        jge loc_4a9360
        xor eax, eax
        jmp loc_4a936c
loc_4a9360:
        cmp eax, 0xff
        jle loc_4a936c
        mov eax, 0xff
loc_4a936c:
        fld dword ptr [esi + 0xc]
        mov edx, dword ptr [esi + 0x18]
        test edx, edx
        fsub dword ptr [esi + 0x4]
        mov byte ptr [ebp - 0x31], al
        fstp dword ptr [ebp - 0x1c]
        fld dword ptr [esi + 0x10]
        fsub dword ptr [esi + 0x8]
        fstp dword ptr [ebp - 0x20]
        jle loc_4a944a
        mov ebx, dword ptr [esi + 0x1c]
        mov ecx, dword ptr [esi + 0x28]
        mov eax, dword ptr [esi + 0x20]
        mov dword ptr [ebp - 0x10], ebx
        lea ebx, [ecx + 0x4]
        push edi
        mov edi, dword ptr [esi + 0x24]
        mov dword ptr [ebp - 0x18], ebx
        lea ebx, [eax + 0x4]
        sub eax, edi
        sub ecx, edi
        mov dword ptr [ebp - 0x14], ebx
        mov ebx, dword ptr [ebp + 0x8]
        mov dword ptr [ebp - 0x28], eax
        mov dword ptr [ebp - 0x2c], ecx
        mov dword ptr [ebp - 0x24], edx
loc_4a93b8:
        mov ecx, dword ptr [ebp - 0x10]
        mov eax, dword ptr [ebx]
        push ecx
        mov ecx, ebx
        call dword ptr [eax + 0x8c]
        fld dword ptr [ebp - 0x1c]
        mov edx, dword ptr [ebp - 0x28]
        fmul dword ptr [edx + edi*0x1]
        mov eax, dword ptr [ebp - 0x14]
        mov ecx, dword ptr [ebp - 0x2c]
        mov edx, dword ptr [ebp - 0x18]
        fadd dword ptr [esi + 0x4]
        fstp dword ptr [ebp - 0x54]
        fld dword ptr [ebp - 0x20]
        fmul dword ptr [eax]
        fadd dword ptr [esi + 0x8]
        fstp dword ptr [ebp - 0x50]
        fld dword ptr [ebp - 0x1c]
        fmul dword ptr [edi]
        fadd dword ptr [esi + 0x4]
        fstp dword ptr [ebp - 0x4c]
        fld dword ptr [ebp - 0x20]
        fmul dword ptr [edi + 0x4]
        fadd dword ptr [esi + 0x8]
        fstp dword ptr [ebp - 0x48]
        fld dword ptr [ecx + edi*0x1]
        mov ecx, ebx
        fstp dword ptr [ebp - 0x3c]
        mov eax, dword ptr [edx]
        mov edx, dword ptr [ebx]
        mov dword ptr [ebp - 0x38], eax
        lea eax, [ebp - 0x54]
        push eax
        call dword ptr [edx + 0x110]
        mov ecx, dword ptr [ebp - 0x14]
        mov edx, dword ptr [ebp - 0x10]
        mov eax, 0x8
        add ecx, eax
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp - 0x18]
        add ecx, eax
        add edi, eax
        mov eax, dword ptr [ebp - 0x24]
        add edx, 0xd4
        dec eax
        mov dword ptr [ebp - 0x10], edx
        mov dword ptr [ebp - 0x18], ecx
        mov dword ptr [ebp - 0x24], eax
        jnz loc_4a93b8
        pop edi
loc_4a944a:
        push offset fun_00402c90
        push 0x2
        push 0x8
        lea ecx, [ebp - 0x44]
        push ecx
        mov dword ptr [ebp - 0x4], 0x2
        call fun_004c1116
        push offset fun_00402c90
        push 0x2
        push 0x8
        lea edx, [ebp - 0x54]
        push edx
        mov dword ptr [ebp - 0x4], 0xffffffff
        call fun_004c1116
        mov ecx, dword ptr [ebp - 0xc]
        pop esi
        pop ebx
        mov dword ptr fs:[0x0], ecx
        mov esp, ebp
        pop ebp
        ret 0x8
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a94c0
// original: FUN_004a94c0
void __fastcall FUN_004a94c0(void*, void*)
{
    __asm {
        push -1
        push offset dat_004dec23
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push esi
        mov esi, ecx
        mov dword ptr [esp + 4], esi
        push offset fun_00402c90
        push 2
        push 8
        lea eax, [esi + 0x10]
        push eax
        mov dword ptr [esp + 0x20], 0
        call fun_004c1116
        push offset fun_00402c90
        push 2
        push 8
        push esi
        mov dword ptr [esp + 0x20], -1
        call fun_004c1116
        mov ecx, dword ptr [esp + 8]
        pop esi
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a9520
// original: TextRenderer constructor role inferred from direct callers
void FUN_004a9520()
{
    __asm {
        push -1
        push offset dat_004dec57
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        push ecx
        push ebx
        push esi
        mov esi, ecx
        push edi
        lea ecx, [esi + 0x48]
        mov dword ptr [esp + 0xc], esi
        call fun_004a8580
        mov eax, dword ptr [esp + 0x24]
        xor ebx, ebx
        push eax
        mov ecx, esi
        mov dword ptr [esp + 0x1c], 2
        mov dword ptr [esi + 0x11c], ebx
        mov dword ptr [esi + 0x120], -1
        call fun_004aa9c0
        mov ecx, dword ptr [esi + 0x11c]
        mov edx, dword ptr [esp + 0x20]
        push ecx
        push edx
        mov ecx, esi
        call fun_004a9660
        mov edi, dword ptr [esi + 0x11c]
        cmp edi, ebx
        mov eax, dword ptr [esp + 0x28]
        mov ecx, dword ptr [esp + 0x2c]
        mov byte ptr [esi + 0x130], bl
        mov dword ptr [esi + 0x1c], eax
        mov dword ptr [esi + 0x20], ecx
        je text_renderer_ctor_no_child
        mov ecx, edi
        call fun_004aafa0
        push edi
        call fun_004bd000
        add esp, 4
    text_renderer_ctor_no_child:
        mov ecx, dword ptr [esp + 0x10]
        pop edi
        mov dword ptr [esi + 0x11c], ebx
        mov eax, esi
        pop esi
        pop ebx
        mov dword ptr fs:[0], ecx
        add esp, 0x10
        ret 0x10
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a9660
// original: FUN_004a9660
void FUN_004a9660()
{
    __asm {
        mov eax, fs:[0]
        push -1
        push offset dat_004decbb
        push eax
        mov eax, dword ptr [esp + 0x10]
        mov dword ptr fs:[0], esp
        push ebx
        mov ebx, dword ptr [eax + 0x20]
        push esi
        push edi
        push 0x68
        mov esi, ecx
        call fun_004c1276
        add esp, 4
        mov dword ptr [esp + 0x1c], eax
        xor edi, edi
        cmp eax, edi
        mov dword ptr [esp + 0x14], edi
        je text_renderer_setup_no_state
        push 0x502154
        mov ecx, eax
        call fun_004a7fd0
        jmp text_renderer_setup_state_ready
    text_renderer_setup_no_state:
        xor eax, eax
    text_renderer_setup_state_ready:
        mov ecx, dword ptr [esp + 0x20]
        mov edx, dword ptr [ebx]
        push edi
        push 0x840
        push ecx
        push eax
        mov ecx, ebx
        mov dword ptr [esp + 0x24], -1
        call dword ptr [edx + 0x10]
        mov ecx, dword ptr [esp + 0xc]
        mov dword ptr [esi + 0x48], eax
        mov eax, dword ptr [esi + 0xe4]
        mov dword ptr [esi + 0xa8], edi
        mov dword ptr [esi + 0xac], edi
        mov dword ptr [esi + 0xb0], edi
        mov dword ptr [esi + 0xb4], edi
        mov dword ptr [esi + 0xb8], edi
        mov dword ptr [esi + 0xbc], edi
        mov dword ptr [esi + 0xc0], edi
        mov dword ptr [esi + 0xc4], edi
        mov dword ptr [esi + 0x5c], edi
        mov dword ptr [esi + 0x58], edi
        or eax, 2
        pop edi
        mov dword ptr [esi + 0xe0], 0xd
        mov dword ptr [esi + 0x60], 5
        mov dword ptr [esi + 0x64], 6
        mov dword ptr [esi + 0xe4], eax
        mov dword ptr [esi + 0x6c], 4
        pop esi
        mov dword ptr fs:[0], ecx
        pop ebx
        add esp, 0xc
        ret 8
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a97a0
// original: FUN_004a97a0
void FUN_004a97a0()
{
    __asm {
        mov eax, dword ptr [esp + 0x10]
        push esi
        push -1
        push eax
        push offset DAT_0057a7d8
        mov esi, ecx
        call fun_004b7670
        mov ecx, dword ptr [esp + 0x2c]
        mov edx, dword ptr [esp + 0x28]
        mov eax, dword ptr [esp + 0x24]
        add esp, 0xc
        push ecx
        mov ecx, dword ptr [esp + 0x14]
        push edx
        mov edx, dword ptr [esp + 0x14]
        push eax
        mov eax, dword ptr [esp + 0x14]
        push offset DAT_0057a7d8
        push ecx
        push edx
        push eax
        mov ecx, esi
        call fun_004a97f0
        pop esi
        ret 0x1c
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a9740
// original: FUN_004a9740
void FUN_004a9740()
{
    __asm {
        mov ecx, dword ptr [esp + 0x14]
        lea eax, [esp + 0x18]
        push eax
        push ecx
        push 0x579bd8
        call fun_004c2431
        push -1
        push 0x579bd8
        push 0x579fd8
        call fun_004b7670
        mov edx, dword ptr [esp + 0x28]
        mov eax, dword ptr [esp + 0x24]
        mov ecx, dword ptr [esp + 0x20]
        add esp, 0x18
        push 0x3f800000
        push 0x3f800000
        push 0
        push 0x579fd8
        push edx
        push eax
        push ecx
        mov ecx, dword ptr [esp + 0x20]
        call fun_004a97f0
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a97f0
// original: FUN_004a97f0
void FUN_004a97f0()
{
    __asm {
        mov eax,fs:[0]
        push 0xffffffff
        push offset dat_004ded06
        push eax
        mov dword ptr fs:[0],esp
        sub esp,0x34
        push ebp
        push esi
        mov esi,ecx
        mov al,byte ptr [esi+0x130]
        test al,al
        push edi
        je text_renderer_core_004a9855
        mov eax,dword ptr [esp+0x68]
        mov ecx,dword ptr [esp+0x64]
        mov edx,dword ptr [esp+0x60]
        push eax
        mov eax,dword ptr [esp+0x60]
        push ecx
        mov ecx,dword ptr [esp+0x60]
        push edx
        mov edx,dword ptr [esp+0x60]
        push eax
        mov eax,dword ptr [esp+0x60]
        push ecx
        push edx
        push eax
        mov ecx,esi
        call fun_004aa690
        pop edi
        pop esi
        pop ebp
        mov ecx,dword ptr [esp+0x34]
        mov dword ptr fs:[0],ecx
        add esp,0x40
        ret 0x1c
        text_renderer_core_004a9855:
        mov edi,dword ptr [esp+0x5c]
        push edi
        call fun_004c2414
        mov ebp,eax
        add esp,0x4
        test ebp,ebp
        je text_renderer_core_004a9aea
        mov ecx,dword ptr [esp+0x54]
        mov edx,dword ptr [esp+0x58]
        mov dword ptr [esp+0x5c],ecx
        mov ecx,dword ptr [esp+0x50]
        mov eax,dword ptr [ecx]
        mov dword ptr [esp+0xc],edx
        push ebx
        lea edx,[esi+0x48]
        push edx
        call dword ptr [eax+0x8c]
        push offset fun_00402c90
        push offset fun_00490ad0
        push 0x2
        push 0x8
        lea eax,[esp+0x2c]
        push eax
        call fun_004c11f4
        push offset fun_00402c90
        push offset fun_00490ad0
        push 0x2
        push 0x8
        lea ecx,[esp+0x3c]
        push ecx
        mov dword ptr [esp+0x60],0x0
        call fun_004c11f4
        mov eax,dword ptr [esi+0x120]
        mov edx,dword ptr [esp+0x6c]
        push edx
        mov dword ptr [esp+0x40],eax
        mov eax,dword ptr [esp+0x6c]
        push eax
        push edi
        lea ecx,[esp+0x20]
        push ecx
        mov ebx,0x1
        mov ecx,esi
        mov dword ptr [esp+0x5c],ebx
        mov dword ptr [esp+0x50],0x3f800000
        call fun_004aa5a0
        fld dword ptr [esp+0x58]
        test byte ptr [esp+0x64],bl
        mov byte ptr [esp+0x4c],0x2
        jne text_renderer_core_004a9936
        fld dword ptr [esp+0x14]
        fdiv dword ptr [esp+0x68]
        fsubr dword ptr [esp+0x14]
        fmul qword ptr [DAT_004e2958]
        fsubp st(1),st
        fstp dword ptr [esp+0x14]
        fld dword ptr [esp+0x5c]
        fld dword ptr [esp+0x18]
        fdiv dword ptr [esp+0x6c]
        fsubr dword ptr [esp+0x18]
        fmul qword ptr [DAT_004e2958]
        jmp text_renderer_core_004a9954
        text_renderer_core_004a9936:
        fld dword ptr [esp+0x14]
        fmul qword ptr [DAT_004e2958]
        fsubp st(1),st
        fstp dword ptr [esp+0x14]
        fld dword ptr [esp+0x5c]
        fld dword ptr [esp+0x18]
        fmul qword ptr [DAT_004e2958]
        text_renderer_core_004a9954:
        test ebp,ebp
        fsubp st(1),st
        fstp dword ptr [esp+0x18]
        jle text_renderer_core_004a9a9f
        fld dword ptr [esp+0x60]
        mov ebx,edi
        text_renderer_core_004a9968:
        mov ax,word ptr [ebx]
        cmp ax,0x20
        jne text_renderer_core_004a9980
        fild dword ptr [esi+0x24]
        fmul dword ptr [esi+0x1c]
        fld st(0)
        faddp st(2),st
        jmp text_renderer_core_004a9a87
        text_renderer_core_004a9980:
        cmp ax,0xa
        jne text_renderer_core_004a99b5
        fstp st(0)
        mov edx,dword ptr [esp+0x58]
        fld dword ptr [esp+0x58]
        mov dword ptr [esp+0x14],edx
        fild dword ptr [esi+0x2c]
        fmul dword ptr [esi+0x20]
        fld st(0)
        fadd dword ptr [esp+0x10]
        fstp dword ptr [esp+0x10]
        fmul dword ptr [esp+0x6c]
        fadd dword ptr [esp+0x18]
        fstp dword ptr [esp+0x18]
        jmp text_renderer_core_004a9a93
        text_renderer_core_004a99b5:
        mov ecx,dword ptr [esi+0x34]
        and eax,0xffff
        sub eax,ecx
        js text_renderer_core_004a99c6
        cmp eax,dword ptr [esi+0x30]
        jl text_renderer_core_004a99cb
        text_renderer_core_004a99c6:
        mov eax,0x5
        text_renderer_core_004a99cb:
        mov edx,dword ptr [esi+0xc]
        xor ecx,ecx
        mov cx,word ptr [edx+eax*2]
        lea eax,[ecx*8+0x0]
        sub eax,ecx
        mov ecx,dword ptr [esi]
        lea edi,[ecx+eax*4]
        mov eax,dword ptr [edi+0x4]
        test eax,eax
        je text_renderer_core_004a9a93
        fild dword ptr [edi]
        mov eax,dword ptr [esp+0x18]
        fmul dword ptr [esi+0x1c]
        fst dword ptr [esp+0x5c]
        fadd st,st(1)
        fstp dword ptr [esp+0x60]
        fstp st(0)
        fld dword ptr [esp+0x5c]
        fmul dword ptr [esp+0x68]
        fadd dword ptr [esp+0x14]
        fstp dword ptr [esp+0x14]
        mov edx,dword ptr [esp+0x14]
        mov dword ptr [esp+0x1c],edx
        fild dword ptr [edi+0x4]
        mov dword ptr [esp+0x20],eax
        fmul dword ptr [esi+0x1c]
        fmul dword ptr [esp+0x68]
        fadd dword ptr [esp+0x14]
        fstp dword ptr [esp+0x24]
        fild dword ptr [esi+0x2c]
        fmul dword ptr [esi+0x20]
        fmul dword ptr [esp+0x6c]
        fadd dword ptr [esp+0x18]
        fstp dword ptr [esp+0x28]
        mov ecx,dword ptr [edi+0xc]
        mov dword ptr [esp+0x2c],ecx
        mov edx,dword ptr [edi+0x10]
        mov dword ptr [esp+0x30],edx
        mov eax,dword ptr [edi+0x14]
        mov dword ptr [esp+0x34],eax
        mov ecx,dword ptr [edi+0x18]
        mov dword ptr [esp+0x38],ecx
        mov ecx,dword ptr [esp+0x54]
        mov edx,dword ptr [ecx]
        lea eax,[esp+0x1c]
        push eax
        call dword ptr [edx+0x110]
        fild dword ptr [edi+0x8]
        fiadd dword ptr [esi+0x28]
        fiadd dword ptr [edi+0x4]
        fmul dword ptr [esi+0x1c]
        fst dword ptr [esp+0x5c]
        fadd dword ptr [esp+0x60]
        fld dword ptr [esp+0x5c]
        text_renderer_core_004a9a87:
        fmul dword ptr [esp+0x68]
        fadd dword ptr [esp+0x14]
        fstp dword ptr [esp+0x14]
        text_renderer_core_004a9a93:
        add ebx,0x2
        dec ebp
        jne text_renderer_core_004a9968
        jmp text_renderer_core_004a9aa3
        text_renderer_core_004a9a9f:
        fld dword ptr [esp+0x60]
        text_renderer_core_004a9aa3:
        mov ecx,dword ptr [esp+0x10]
        fstp dword ptr [DAT_00579bd0]
        mov dword ptr [DAT_00579bd4],ecx
        push offset fun_00402c90
        push 0x2
        push 0x8
        lea edx,[esp+0x38]
        push edx
        mov dword ptr [esp+0x5c],0x3
        call fun_004c1116
        push offset fun_00402c90
        push 0x2
        push 0x8
        lea eax,[esp+0x28]
        push eax
        mov dword ptr [esp+0x5c],0xffffffff
        call fun_004c1116
        pop ebx
        text_renderer_core_004a9aea:
        mov ecx,dword ptr [esp+0x40]
        pop edi
        pop esi
        pop ebp
        mov dword ptr fs:[0],ecx
        add esp,0x40
        ret 0x1c
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a87f0
// original: FUN_004a87f0
ModelResourceState* ModelResourceState::CopyAssignExact(ModelResourceState*)
{
    __asm {
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov ebx, dword ptr [esp + 0x18]
        mov eax, dword ptr [ebx]
        mov edx, dword ptr [ebx + 0x8]
        mov ebp, ecx
        mov ecx, dword ptr [ebx + 0x4]
        mov dword ptr [ebp], eax
        mov eax, dword ptr [ebx + 0xc]
        mov dword ptr [ebp + 0x4], ecx
        mov ecx, dword ptr [ebx + 0x10]
        mov dword ptr [ebp + 0x8], edx
        mov edx, dword ptr [ebx + 0x14]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebx + 0x18]
        mov dword ptr [ebp + 0x10], ecx
        mov ecx, dword ptr [ebx + 0x1c]
        mov dword ptr [ebp + 0x14], edx
        mov edx, dword ptr [ebx + 0x20]
        mov dword ptr [ebp + 0x18], eax
        mov eax, dword ptr [ebx + 0x24]
        mov dword ptr [ebp + 0x1c], ecx
        mov ecx, dword ptr [ebx + 0x28]
        mov dword ptr [ebp + 0x20], edx
        mov edx, dword ptr [ebx + 0x2c]
        mov dword ptr [ebp + 0x24], eax
        mov eax, dword ptr [ebx + 0x30]
        mov dword ptr [ebp + 0x28], ecx
        mov ecx, dword ptr [ebx + 0x34]
        mov dword ptr [ebp + 0x2c], edx
        mov edx, dword ptr [ebx + 0x38]
        mov dword ptr [ebp + 0x30], eax
        mov eax, dword ptr [ebx + 0x3c]
        mov dword ptr [ebp + 0x34], ecx
        mov dword ptr [ebp + 0x38], edx
        mov dword ptr [ebp + 0x3c], eax
        lea ecx, [ebx + 0x40]
        mov eax, dword ptr [ecx]
        lea edx, [ebp + 0x40]
        mov dword ptr [edx], eax
        mov eax, dword ptr [ecx + 0x4]
        mov dword ptr [edx + 0x4], eax
        mov eax, dword ptr [ecx + 0x8]
        mov ecx, dword ptr [ecx + 0xc]
        mov dword ptr [edx + 0x8], eax
        mov dword ptr [edx + 0xc], ecx
        lea edx, [ebx + 0x50]
        mov ecx, dword ptr [edx]
        lea eax, [ebp + 0x50]
        mov dword ptr [eax], ecx
        mov ecx, dword ptr [edx + 0x4]
        mov dword ptr [eax + 0x4], ecx
        mov ecx, dword ptr [edx + 0x8]
        mov edx, dword ptr [edx + 0xc]
        mov dword ptr [eax + 0x8], ecx
        mov dword ptr [eax + 0xc], edx
        lea eax, [ebx + 0x60]
        mov edx, dword ptr [eax]
        lea ecx, [ebp + 0x60]
        mov dword ptr [ecx], edx
        mov edx, dword ptr [eax + 0x4]
        mov dword ptr [ecx + 0x4], edx
        mov edx, dword ptr [eax + 0x8]
        mov eax, dword ptr [eax + 0xc]
        mov dword ptr [ecx + 0x8], edx
        mov dword ptr [ecx + 0xc], eax
        lea ecx, [ebx + 0x70]
        mov eax, dword ptr [ecx]
        lea edx, [ebp + 0x70]
        mov dword ptr [edx], eax
        mov eax, dword ptr [ecx + 0x4]
        mov dword ptr [edx + 0x4], eax
        mov eax, dword ptr [ecx + 0x8]
        mov ecx, dword ptr [ecx + 0xc]
        mov dword ptr [edx + 0x8], eax
        mov dword ptr [edx + 0xc], ecx
        mov edx, dword ptr [ebx + 0x84]
        mov dword ptr [ebp + 0x84], edx
        lea eax, [ebx + 0x88]
        mov edx, dword ptr [eax]
        lea ecx, [ebp + 0x88]
        mov dword ptr [ecx], edx
        mov edx, dword ptr [eax + 0x4]
        mov dword ptr [ecx + 0x4], edx
        mov edx, dword ptr [eax + 0x8]
        mov eax, dword ptr [eax + 0xc]
        mov dword ptr [ecx + 0x8], edx
        mov edx, dword ptr [ebx + 0x9c]
        mov dword ptr [ecx + 0xc], eax
        mov ecx, dword ptr [ebx + 0x98]
        mov eax, dword ptr [ebx + 0xa0]
        lea esi, [ebx + 0xa4]
        mov dword ptr [ebp + 0x9c], edx
        mov edx, dword ptr [esi + 0x8]
        mov dword ptr [ebp + 0x98], ecx
        mov ecx, dword ptr [dat_004e1340]
        mov dword ptr [ebp + 0xa0], eax
        mov eax, edx
        cmp ecx, eax
        lea edi, [ebp + 0xa4]
        mov dword ptr [esp + 0x18], eax
        jnc loc_4a8941
        mov dword ptr [esp + 0x18], ecx
        mov eax, ecx
loc_4a8941:
        cmp edi, esi
        jnz loc_4a89ae
        mov esi, ecx
        cmp dword ptr [edi + 0x8], eax
        jnc loc_4a8951
        call fun_004bd237
loc_4a8951:
        mov ecx, edi
        call fun_00402bd0
        mov eax, dword ptr [edi + 0x8]
        mov ecx, dword ptr [esp + 0x18]
        mov dword ptr [esp + 0x10], eax
        sub eax, ecx
        cmp eax, esi
        jnc loc_4a896b
        mov esi, eax
loc_4a896b:
        test esi, esi
        jbe loc_4a89a2
        mov edx, dword ptr [edi + 0x4]
        sub eax, esi
        add ecx, edx
        push eax
        lea eax, [ecx + esi*0x1]
        push eax
        push ecx
        call fun_004c14e0
        mov eax, dword ptr [esp + 0x1c]
        add esp, 0xc
        sub eax, esi
        mov esi, eax
        push 0x0
        push esi
        mov ecx, edi
        call fun_00402980
        test al, al
        jz loc_4a89a2
        push esi
        mov ecx, edi
        call fun_00402960
loc_4a89a2:
        mov ecx, edi
        call fun_00402bd0
        jmp loc_4a8a58
loc_4a89ae:
        xor ecx, ecx
        cmp eax, ecx
        jbe loc_4a8a1d
        cmp eax, edx
        jnz loc_4a8a1d
        mov edx, dword ptr [esi + 0x4]
        cmp edx, ecx
        jnz loc_4a89c4
        mov edx, offset dat_004e1344
loc_4a89c4:
        cmp byte ptr [edx - 0x1], 0xfe
        jnc loc_4a8a1d
        mov eax, dword ptr [edi + 0x4]
        cmp eax, ecx
        jz loc_4a89ef
        mov dl, byte ptr [eax - 0x1]
        dec eax
        test dl, dl
        jz loc_4a89e4
        cmp dl, 0xff
        jz loc_4a89e4
        dec dl
        mov byte ptr [eax], dl
        jmp loc_4a89ef
loc_4a89e4:
        push eax
        call fun_004bd000
        add esp, 0x4
        xor ecx, ecx
loc_4a89ef:
        mov eax, dword ptr [esi + 0x4]
        cmp eax, ecx
        mov dword ptr [edi + 0x4], ecx
        mov dword ptr [edi + 0x8], ecx
        mov dword ptr [edi + 0xc], ecx
        jnz loc_4a8a04
        mov eax, offset dat_004e1344
loc_4a8a04:
        mov ecx, dword ptr [esi + 0x8]
        mov dword ptr [edi + 0x8], ecx
        mov ecx, dword ptr [esi + 0xc]
        mov dword ptr [edi + 0xc], ecx
        mov cl, byte ptr [eax - 0x1]
        inc cl
        mov dword ptr [edi + 0x4], eax
        mov byte ptr [eax - 0x1], cl
        jmp loc_4a8a58
loc_4a8a1d:
        push 0x1
        push eax
        mov ecx, edi
        call fun_00402980
        test al, al
        jz loc_4a8a58
        mov esi, dword ptr [esi + 0x4]
        test esi, esi
        jnz loc_4a8a37
        mov esi, offset dat_004e1344
loc_4a8a37:
        mov edx, dword ptr [edi + 0x4]
        mov ecx, dword ptr [esp + 0x18]
        mov eax, ecx
        shr ecx, 0x2
        mov edi, edx
        rep movsd
        mov ecx, eax
        and ecx, 0x3
        rep movsb
        mov dword ptr [ebp + 0xac], eax
        mov byte ptr [edx + eax*0x1], 0x0
loc_4a8a58:
        mov eax, dword ptr [ebx + 0xbc]
        mov ecx, dword ptr [ebx + 0xb4]
        mov edx, dword ptr [ebx + 0xb8]
        pop edi
        mov dword ptr [ebp + 0xbc], eax
        xor eax, eax
        pop esi
        mov dword ptr [ebp + 0xb4], ecx
        mov dword ptr [ebp + 0xb8], edx
        mov dword ptr [ebp + 0xc4], eax
        mov dword ptr [ebp + 0xc8], eax
        mov dword ptr [ebp + 0xcc], eax
        mov dword ptr [ebp + 0xd0], eax
        mov dword ptr [ebp + 0xc0], eax
        pop ebp
        pop ebx
        pop ecx
        ret 0x4
    }
}

