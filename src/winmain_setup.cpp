// Exact WinMain device/adapter/display-mode validation entry. The readable
// behavior implementation is retained as ValidateSavedDisplayBehavior in
// winmain.cpp; this isolates VC6's duplicated return paths and register layout.
extern "C" void* dat_0056b0b0;
extern "C" int dat_0056b988;
extern "C" char dat_0056b8dc;
extern "C" int dat_0056b970;
extern "C" void* dat_0056b968;
extern "C" char dat_0056b8cc;
extern "C" int dat_0056b930;
extern "C" int dat_0056b934;
extern "C" int dat_0056b938;
extern "C" int dat_0056b940;
extern "C" int dat_0056b93c;
extern "C" int dat_0056b984;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a2270
// original: FUN_004a2270
int FUN_004a2270(void)
{
    __asm {
        sub esp, 0x24
        mov eax, dword ptr [dat_0056b0b0]
        mov edx, dword ptr [eax + 4]
        push ebx
        push ebp
        push esi
        push edi
        mov byte ptr [esp + 0x13], 0
        xor edi, edi
        mov ebp, edx
    device_loop:
        test edx, edx
        jz device_scan_done
        mov eax, dword ptr [dat_0056b0b0]
        mov eax, dword ptr [eax + 8]
        sub eax, edx
        sar eax, 3
        cmp edi, eax
        jnc device_scan_done
        mov eax, dword ptr [ebp]
        mov esi, offset dat_0056b8dc
    compare_device_name:
        mov bl, byte ptr [eax]
        mov cl, bl
        cmp bl, byte ptr [esi]
        jnz device_name_different
        test cl, cl
        jz device_name_equal
        mov bl, byte ptr [eax + 1]
        mov cl, bl
        cmp bl, byte ptr [esi + 1]
        jnz device_name_different
        add eax, 2
        add esi, 2
        test cl, cl
        jnz compare_device_name
    device_name_equal:
        xor eax, eax
        jmp device_name_result
    device_name_different:
        sbb eax, eax
        sbb eax, -1
    device_name_result:
        test eax, eax
        jnz device_next
        mov byte ptr [esp + 0x13], 1
        mov dword ptr [dat_0056b988], edi
    device_next:
        inc edi
        add ebp, 8
        jmp device_loop
    device_scan_done:
        mov al, byte ptr [esp + 0x13]
        test al, al
        jnz adapter_scan_start
    early_return:
        pop edi
        pop esi
        pop ebp
        xor eax, eax
        pop ebx
        add esp, 0x24
        ret
    adapter_scan_start:
        mov eax, dword ptr [dat_0056b970]
        xor ebp, ebp
        test eax, eax
        jle early_return
        xor ebx, ebx
    adapter_loop:
        mov ecx, dword ptr [dat_0056b968]
        mov ecx, dword ptr [ebx + ecx]
        mov edx, dword ptr [ecx]
        mov edi, offset dat_0056b8cc
        call dword ptr [edx + 8]
        mov esi, eax
        mov ecx, 4
        xor eax, eax
        repe cmpsd
        jz adapter_found
        mov eax, dword ptr [dat_0056b970]
        inc ebp
        add ebx, 0x14
        cmp ebp, eax
        jl adapter_loop
    no_adapter:
        pop edi
        pop esi
        pop ebp
        xor eax, eax
        pop ebx
        add esp, 0x24
        ret
    adapter_found:
        mov ecx, dword ptr [dat_0056b930]
        mov edx, dword ptr [dat_0056b934]
        mov eax, dword ptr [dat_0056b938]
        mov ebx, dword ptr [dat_0056b940]
        mov dword ptr [esp + 0x20], ecx
        mov ecx, dword ptr [dat_0056b93c]
        mov dword ptr [esp + 0x24], edx
        mov dword ptr [esp + 0x28], eax
        mov eax, dword ptr [dat_0056b968]
        lea edx, [ebp + ebp*4]
        mov dword ptr [dat_0056b984], ebp
        mov dword ptr [esp + 0x2c], ecx
        mov ecx, dword ptr [eax + edx*4]
        mov edx, dword ptr [ecx]
        mov byte ptr [esp + 0x13], 0
        call dword ptr [edx + 0xc]
        mov ecx, eax
        mov eax, dword ptr [ecx + 4]
        mov dword ptr [esp + 0x30], ecx
        mov dword ptr [esp + 0x14], 0
        mov dword ptr [esp + 0x1c], eax
        mov dword ptr [esp + 0x18], eax
    mode_loop:
        test eax, eax
        jz mode_scan_done
        mov ecx, dword ptr [ecx + 8]
        mov edx, dword ptr [esp + 0x14]
        sub ecx, eax
        sar ecx, 2
        cmp edx, ecx
        jnc mode_scan_done
        mov edx, dword ptr [esp + 0x18]
        mov edx, dword ptr [edx]
        mov ecx, dword ptr [esp + 0x20]
        cmp dword ptr [edx + 4], ecx
        jnz mode_advance
        mov ecx, dword ptr [esp + 0x24]
        cmp dword ptr [edx + 8], ecx
        jnz mode_advance
        mov ecx, dword ptr [esp + 0x28]
        cmp dword ptr [edx + 0xc], ecx
        jnz mode_advance
        mov ebp, dword ptr [edx + 0x14]
        xor esi, esi
        test ebp, ebp
        jle mode_advance
        lea edi, [edx + 0x18]
    refresh_loop:
        mov eax, dword ptr [esp + 0x2c]
        cmp dword ptr [edi], eax
        jnz refresh_next
        test ebx, ebx
        jz mode_available
        mov ecx, ebx
        mov eax, 1
        shl eax, cl
        test dword ptr [edx + 0x10], eax
        jnz mode_available
    refresh_next:
        inc esi
        add edi, 4
        cmp esi, ebp
        jl refresh_loop
        jmp mode_next
    mode_available:
        mov byte ptr [esp + 0x13], 1
    mode_next:
        mov eax, dword ptr [esp + 0x1c]
    mode_advance:
        mov edx, dword ptr [esp + 0x14]
        mov ecx, dword ptr [esp + 0x18]
        inc edx
        add ecx, 4
        mov dword ptr [esp + 0x18], ecx
        mov ecx, dword ptr [esp + 0x30]
        mov dword ptr [esp + 0x14], edx
        jmp mode_loop
    mode_scan_done:
        mov cl, byte ptr [esp + 0x13]
        pop edi
        pop esi
        xor eax, eax
        test cl, cl
        pop ebp
        setnz al
        pop ebx
        add esp, 0x24
        ret
    }
}
