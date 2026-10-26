#include <math.h>
#include <new>
#include <stdlib.h>
#include <vector>

// MeshGroup and MeshSourceView field offsets are observed in FUN_004bc390:
// group vertex/index pointers are +4/+8 and the triangle count is +0x0e;
// the source's group-pointer vector begins at +0x34.
struct MeshGroupView {
    unsigned char unknown0[4];
    const unsigned char* vertices;
    const unsigned char* triangle_indices;
    unsigned short unknown_c;
    unsigned short triangle_count;
};

struct MeshSourceView {
    unsigned char flags_and_payload[0x34];
    std::vector<MeshGroupView*> groups;
};

struct TriangleCircumcircle {
    float center_radius[4];
    void Calculate(const float* p0, const float* p1, const float* p2);
    void CalculateBehavior(const float* p0, const float* p1, const float* p2);
};

struct MeshTriangleCache {
    unsigned char source_flags;
    unsigned char padding[3];
    std::vector<float*> group_records;
    MeshTriangleCache* Build(const MeshSourceView* source);
    MeshTriangleCache* BuildBehavior(const MeshSourceView* source);
    void __fastcall Clear(void*);
    void ClearBehavior();
};

// Behavior reference for the exact x87 entry below.
// Inference from the three edge dot products and the four output writes:
// store the circumcenter and radius unless the triangle is degenerate.
void TriangleCircumcircle::CalculateBehavior(const float* p0, const float* p1, const float* p2)
{
    double ax = p0[0] - p2[0];
    double ay = p0[1] - p2[1];
    double az = p0[2] - p2[2];
    double bx = p1[0] - p2[0];
    double by = p1[1] - p2[1];
    double bz = p1[2] - p2[2];
    double aa = ax * ax + ay * ay + az * az;
    double ab = bx * ax + by * ay + bz * az;
    double bb = bx * bx + by * by + bz * bz;
    double determinant = bb * aa - ab * ab;
    if (fabs(determinant) > 0.000001) {
        double half_inverse = 0.5 / determinant;
        double w0 = (aa - ab) * half_inverse * bb;
        double w1 = (bb - ab) * half_inverse * aa;
        double w2 = (1.0 - w0) - w1;
        double dx = w0 * ax + w1 * bx;
        double dy = w0 * ay + w1 * by;
        double dz = w0 * az + w1 * bz;
        center_radius[0] = (float)(p0[0] * w0 + p1[0] * w1 + p2[0] * w2);
        center_radius[1] = (float)(p0[1] * w0 + p1[1] * w1 + p2[1] * w2);
        center_radius[2] = (float)(p0[2] * w0 + p1[2] * w1 + p2[2] * w2);
        center_radius[3] = (float)sqrt(dx * dx + dy * dy + dz * dz);
    }
}

extern "C" double dat_004e29b0;
extern "C" double dat_004e2958;
extern "C" double dat_004e2978;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b4cd0
// The readable operation is CalculateBehavior; this entry preserves VC6 x87 temporaries.
void TriangleCircumcircle::Calculate(const float*, const float*, const float*)
{
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0xfffffff8
        sub esp, 0x68
        mov edx, dword ptr [ebp + 0x10]
        push esi
        mov esi, dword ptr [ebp + 0x8]
        fld dword ptr [esi]
        push edi
        fsub dword ptr [edx]
        mov edi, dword ptr [ebp + 0xc]
        fst qword ptr [esp + 0x58]
        fld dword ptr [esi + 0x4]
        fsub dword ptr [edx + 0x4]
        fst qword ptr [esp + 0x60]
        fld dword ptr [esi + 0x8]
        fsub dword ptr [edx + 0x8]
        fst qword ptr [esp + 0x68]
        fld dword ptr [edi]
        fsub dword ptr [edx]
        fstp qword ptr [esp + 0x28]
        fld dword ptr [edi + 0x4]
        fsub dword ptr [edx + 0x4]
        fstp qword ptr [esp + 0x30]
        fld dword ptr [edi + 0x8]
        fsub dword ptr [edx + 0x8]
        fstp qword ptr [esp + 0x38]
        fld st(0)
        fmul st, st(1)
        fld st(2)
        fmul st, st(3)
        faddp st(1), st(0)
        fld st(3)
        fmul st, st(4)
        faddp st(1), st(0)
        fstp qword ptr [esp + 0x8]
        fld qword ptr [esp + 0x38]
        fmul st, st(1)
        fld qword ptr [esp + 0x30]
        fmul st, st(3)
        faddp st(1), st(0)
        fld qword ptr [esp + 0x28]
        fmul st, st(4)
        faddp st(1), st(0)
        fst qword ptr [esp + 0x20]
        fstp st(3)
        fstp st(0)
        fstp st(0)
        fld qword ptr [esp + 0x38]
        fmul qword ptr [esp + 0x38]
        fld qword ptr [esp + 0x30]
        fmul qword ptr [esp + 0x30]
        faddp st(1), st(0)
        fld qword ptr [esp + 0x28]
        fmul qword ptr [esp + 0x28]
        faddp st(1), st(0)
        fst qword ptr [esp + 0x18]
        fld st(0)
        fmul qword ptr [esp + 0x8]
        fld st(2)
        fmul st, st(3)
        fsubp st(1), st(0)
        fld st(0)
        fabs
        fcomp qword ptr [dat_004e29b0]
        fnstsw ax
        test ah, 0x41
        jnz circle_degenerate
        fld qword ptr [dat_004e2958]
        fdiv st(0), st(1)
        fstp qword ptr [esp + 0x10]
        fstp st(0)
        fld qword ptr [esp + 0x8]
        fsubrp st(2), st(0)
        fxch st(1)
        fmul qword ptr [esp + 0x10]
        fmul st, st(1)
        fxch st(1)
        fstp st(0)
        fld qword ptr [esp + 0x18]
        fsub qword ptr [esp + 0x20]
        fmul qword ptr [esp + 0x10]
        fmul qword ptr [esp + 0x8]
        fld qword ptr [dat_004e2978]
        fsub st(0), st(2)
        fsub st(0), st(1)
        fld st(1)
        fmul qword ptr [esp + 0x28]
        fld st(3)
        fmul qword ptr [esp + 0x58]
        faddp st(1), st(0)
        fstp qword ptr [esp + 0x40]
        fld st(1)
        fmul qword ptr [esp + 0x30]
        fld st(3)
        fmul qword ptr [esp + 0x60]
        faddp st(1), st(0)
        fstp qword ptr [esp + 0x48]
        fld st(1)
        fmul qword ptr [esp + 0x38]
        fld st(3)
        fmul qword ptr [esp + 0x68]
        faddp st(1), st(0)
        fstp qword ptr [esp + 0x50]
        fld dword ptr [esi]
        fmul st, st(3)
        fld dword ptr [edi]
        fmul st, st(3)
        faddp st(1), st(0)
        fld dword ptr [edx]
        fmul st, st(2)
        faddp st(1), st(0)
        fstp dword ptr [ecx]
        fld dword ptr [edx + 0x4]
        fmul st, st(1)
        fld dword ptr [esi + 0x4]
        fmul st, st(4)
        faddp st(1), st(0)
        fld dword ptr [edi + 0x4]
        fmul st, st(3)
        faddp st(1), st(0)
        fstp dword ptr [ecx + 0x4]
        fld dword ptr [edi + 0x8]
        fmul st, st(2)
        fld dword ptr [edx + 0x8]
        fmul st, st(2)
        faddp st(1), st(0)
        fld dword ptr [esi + 0x8]
        fmul st, st(4)
        faddp st(1), st(0)
        fstp dword ptr [ecx + 0x8]
        fstp st(0)
        fstp st(0)
        fstp st(0)
        fld qword ptr [esp + 0x50]
        fmul qword ptr [esp + 0x50]
        fld qword ptr [esp + 0x48]
        fmul qword ptr [esp + 0x48]
        faddp st(1), st(0)
        fld qword ptr [esp + 0x40]
        fmul qword ptr [esp + 0x40]
        faddp st(1), st(0)
        fsqrt
        fstp dword ptr [ecx + 0xc]
        pop edi
        pop esi
        mov esp, ebp
        pop ebp
        ret 0xc
        circle_degenerate:
        fstp st(0)
        pop edi
        fstp st(0)
        pop esi
        fstp st(0)
        mov esp, ebp
        pop ebp
        ret 0xc
    }
}

// Each group buffer stores one eight-float record per indexed triangle: the
// unit face plane followed by circumcenter and radius. Record and input layout
// interpretations are inferred from the shipped function's offsets/writes.
MeshTriangleCache* MeshTriangleCache::BuildBehavior(const MeshSourceView* source)
{
    source_flags = ((const unsigned char*)&source)[3];
    new (&group_records) std::vector<float*>();

    for (unsigned int group_index = 0; group_index < source->groups.size(); ++group_index) {
        MeshGroupView* group = source->groups[group_index];
        unsigned int count = group->triangle_count;
        unsigned int* allocation = (unsigned int*)malloc(count * 0x20 + 4);
        float* records = 0;
        if (allocation) {
            allocation[0] = count;
            records = (float*)(allocation + 1);
        }
        group_records.insert(group_records.end(), 1, records);

        for (unsigned int triangle = 0; triangle < count && records; ++triangle) {
            const unsigned short* indices = (const unsigned short*)group->triangle_indices;
            const float* p0 = (const float*)(group->vertices + indices[triangle * 3] * 0x2c);
            const float* p1 = (const float*)(group->vertices + indices[triangle * 3 + 1] * 0x2c);
            const float* p2 = (const float*)(group->vertices + indices[triangle * 3 + 2] * 0x2c);
            float* record = records + triangle * 8;

            TriangleCircumcircle* circle = (TriangleCircumcircle*)(record + 4);
            circle->Calculate(p0, p1, p2);

            float x1 = p2[0] - p0[0];
            float y1 = p2[1] - p0[1];
            float z1 = p2[2] - p0[2];
            float x2 = p1[0] - p0[0];
            float y2 = p1[1] - p0[1];
            float z2 = p1[2] - p0[2];
            record[0] = y2 * z1 - z2 * y1;
            record[1] = z2 * x1 - x2 * z1;
            record[2] = x2 * y1 - y2 * x1;
            float inverse_length = 1.0f / sqrtf(record[0] * record[0] +
                                                record[1] * record[1] +
                                                record[2] * record[2]);
            record[0] *= inverse_length;
            record[1] *= inverse_length;
            record[2] *= inverse_length;
            record[3] = record[0] * p0[0] + record[1] * p0[1] + record[2] * p0[2];
        }
    }
    return this;
}

extern "C" void fun_004c1276();
extern "C" void fun_004c11f4();
extern "C" void fun_004bc680();
extern "C" void fun_004b4cd0();
extern "C" void fun_00414330();
extern "C" void fun_00490ad0();
extern "C" void fun_004c1116();
extern "C" void fun_004bd000();
extern "C" void fun_00402c90();
extern "C" void* dat_004dfaab;
extern "C" float dat_004e1348;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4bc390
// Readable behavior reference is BuildBehavior; this preserves the original
// exception scope, vector insertion, and float-register sequence.
MeshTriangleCache* MeshTriangleCache::Build(const MeshSourceView*)
{
    __asm {
        push ebp
        mov ebp, esp
        push -1
        push offset dat_004dfaab
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 0x3c
        mov al, byte ptr [ebp + 0xb]
        push ebx
        mov ebx, ecx
        push esi
        xor esi, esi
        push edi
        mov dword ptr [ebp - 0x28], ebx
        mov byte ptr [ebx], al
        mov dword ptr [ebx + 4], esi
        mov dword ptr [ebx + 8], esi
        mov dword ptr [ebx + 0xc], esi
        xor ecx, ecx
        mov dword ptr [ebp - 4], esi
        mov dword ptr [ebp - 0x18], ecx
    group_loop:
        mov eax, dword ptr [ebp + 8]
        mov edx, dword ptr [eax + 0x34]
        cmp edx, esi
        jz build_done
        mov esi, dword ptr [eax + 0x38]
        sub esi, edx
        sar esi, 2
        cmp ecx, esi
        jnc build_done
        mov eax, edx
        mov edi, dword ptr [eax + ecx*4]
        xor esi, esi
        mov si, word ptr [edi + 0xe]
        mov dword ptr [ebp - 0x24], edi
        mov ecx, esi
        shl ecx, 5
        add ecx, 4
        push ecx
        call fun_004c1276
        add esp, 4
        mov dword ptr [ebp - 0x30], eax
        test eax, eax
        mov byte ptr [ebp - 4], 1
        jz no_group_buffer
        push offset fun_00402c90
        push offset fun_00490ad0
        push esi
        lea ebx, [eax + 4]
        push 0x20
        push ebx
        mov dword ptr [eax], esi
        call fun_004c11f4
        mov eax, ebx
        mov ebx, dword ptr [ebp - 0x28]
        jmp have_group_buffer
    no_group_buffer:
        xor eax, eax
    have_group_buffer:
        lea edx, [ebp - 0x10]
        push edx
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebx + 8]
        push 1
        push eax
        mov ecx, ebx
        mov byte ptr [ebp - 4], 0
        call fun_004bc680
        cmp word ptr [edi + 0xe], 0
        mov dword ptr [ebp - 0x20], 0
        jbe group_complete
        xor ebx, ebx
        mov dword ptr [ebp - 0x14], ebx
        jmp triangle_start
    triangle_next:
        mov edi, dword ptr [ebp - 0x24]
    triangle_start:
        mov eax, dword ptr [ebp - 0x10]
        mov ecx, dword ptr [ebp - 0x14]
        lea esi, [ecx + eax]
        mov ecx, dword ptr [edi + 8]
        mov edi, dword ptr [edi + 4]
        xor eax, eax
        mov ax, word ptr [ebx + ecx]
        mov dword ptr [ebp - 0x1c], edi
        add ebx, 2
        add ebx, 2
        mov dword ptr [ebp - 0x30], esi
        add ebx, 2
        lea edx, [eax + eax*4]
        lea eax, [eax + edx*2]
        lea edi, [edi + eax*4]
        xor eax, eax
        mov ax, word ptr [ebx + ecx - 4]
        lea edx, [eax + eax*4]
        lea eax, [eax + edx*2]
        mov edx, dword ptr [ebp - 0x1c]
        lea eax, [edx + eax*4]
        mov dword ptr [ebp - 0x1c], eax
        xor eax, eax
        mov ax, word ptr [ebx + ecx - 2]
        lea ecx, [eax + eax*4]
        lea eax, [eax + ecx*2]
        mov ecx, dword ptr [ebp - 0x1c]
        lea edx, [edx + eax*4]
        push edx
        push ecx
        push edi
        lea ecx, [esi + 0x10]
        mov dword ptr [ebp - 0x2c], edx
        call fun_004b4cd0
        mov ecx, dword ptr [ebp - 0x2c]
        push edi
        lea edx, [ebp - 0x3c]
        push edx
        call fun_00414330
        mov dword ptr [ebp - 0x2c], eax
        mov ecx, dword ptr [ebp - 0x1c]
        push edi
        lea eax, [ebp - 0x48]
        push eax
        mov byte ptr [ebp - 4], 2
        call fun_00414330
        mov dword ptr [ebp - 0x1c], eax
        mov ecx, dword ptr [ebp - 0x1c]
        mov edx, dword ptr [ebp - 0x2c]
        mov eax, dword ptr [ebp - 0x30]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 8]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 4]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 4]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 8]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx]
        fxch st(2)
        fsubp st(5), st(0)
        fsubp st(3), st(0)
        fsubp st(1), st(0)
        fxch st(2)
        fstp dword ptr [eax]
        fstp dword ptr [eax + 4]
        fstp dword ptr [eax + 8]
        fld dword ptr [esi + 8]
        fld dword ptr [esi + 4]
        mov byte ptr [ebp - 4], 0
        fld dword ptr [esi]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fld st(3)
        fmul st(0), st(4)
        faddp st(1), st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        fdivr dword ptr [dat_004e1348]
        fld st(0)
        fmul dword ptr [esi]
        fst dword ptr [ebp - 0x2c]
        fstp dword ptr [esi]
        fld st(0)
        fmul dword ptr [esi + 4]
        fst dword ptr [ebp - 0x30]
        fstp dword ptr [esi + 4]
        fmul dword ptr [esi + 8]
        fst dword ptr [esi + 8]
        fmul dword ptr [edi + 8]
        fld dword ptr [ebp - 0x30]
        fmul dword ptr [edi + 4]
        faddp st(1), st(0)
        fld dword ptr [ebp - 0x2c]
        mov edx, dword ptr [ebp - 0x24]
        fmul dword ptr [edi]
        mov eax, dword ptr [ebp - 0x20]
        xor ecx, ecx
        mov cx, word ptr [edx + 0xe]
        faddp st(1), st(0)
        inc eax
        mov dword ptr [ebp - 0x20], eax
        fstp dword ptr [esi + 0xc]
        mov esi, dword ptr [ebp - 0x14]
        add esi, 0x20
        mov dword ptr [ebp - 0x14], esi
        cmp eax, ecx
        jl triangle_next
        mov ebx, dword ptr [ebp - 0x28]
    group_complete:
        mov eax, dword ptr [ebp - 0x18]
        inc eax
        mov dword ptr [ebp - 0x18], eax
        xor esi, esi
        mov ecx, eax
        jmp group_loop
    build_done:
        mov ecx, dword ptr [ebp - 0xc]
        pop edi
        pop esi
        mov eax, ebx
        pop ebx
        mov dword ptr fs:[0], ecx
        mov esp, ebp
        pop ebp
        ret 4
    }
}

// Readable behavior reference for the exact entry below.
void MeshTriangleCache::ClearBehavior()
{
    for (unsigned int i = 0; i < group_records.size(); ++i) {
        if (group_records[i]) {
            free((unsigned int*)group_records[i] - 1);
        }
    }
    std::vector<float*> empty;
    group_records.swap(empty);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4bc5d0
void __fastcall MeshTriangleCache::Clear(void*)
{
    __asm {
        push -1
        push 0x4dfac8
        mov eax, fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 8
        push ebx
        push ebp
        push esi
        mov ebx, ecx
        push edi
        mov dword ptr [esp + 0x14], ebx
        mov edi, dword ptr [ebx + 4]
        xor esi, esi
        mov dword ptr [esp + 0x20], esi
        xor ebp, ebp
        mov dword ptr [esp + 0x10], edi
    clear_loop:
        cmp edi, esi
        jz clear_done
        mov eax, dword ptr [ebx + 8]
        sub eax, edi
        sar eax, 2
        cmp ebp, eax
        jnc clear_done
        mov ecx, dword ptr [esp + 0x10]
        mov eax, dword ptr [ecx]
        cmp eax, esi
        jz clear_next
        mov edx, dword ptr [eax - 4]
        lea esi, [eax - 4]
        push offset fun_00402c90
        push edx
        push 0x20
        push eax
        call fun_004c1116
        push esi
        call fun_004bd000
        add esp, 4
        xor esi, esi
    clear_next:
        mov eax, dword ptr [esp + 0x10]
        inc ebp
        add eax, 4
        mov dword ptr [esp + 0x10], eax
        jmp clear_loop
    clear_done:
        push edi
        mov dword ptr [esp + 0x18], edi
        call fun_004bd000
        mov ecx, dword ptr [esp + 0x1c]
        add esp, 4
        pop edi
        mov dword ptr [ebx + 4], esi
        mov dword ptr [ebx + 8], esi
        mov dword ptr [ebx + 0xc], esi
        pop esi
        pop ebp
        pop ebx
        mov dword ptr fs:[0], ecx
        add esp, 0x14
        ret
    }
}
