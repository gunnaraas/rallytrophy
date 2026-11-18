#include <string>
#include <stdexcept>
#include "rallytrophy.h"
#include "unk_4ab910.h"
extern char* PTR_s_Out_of_memory_0050216c;
extern void* __cdecl PoolAlloc(unsigned int);
extern void __cdecl PoolFree(void*);
static inline int Bal(int a){return a;}
// NONMATCHING: RALLYTROPHY 0x4ab910
// MSVC6 exception temporary layout and per-format loop register allocation differ.
// Tried 2026-10-31: 6 discarded Bal(1) calls at start restore the out-of-line std::string _Eos/inline budget (348->199 with palette index form + exact 565/5551/4444 expressions from the asm); volatile zero `z` for the null test, h loaded before w: 175. Still differs: original keeps 0 in ebp (cmp esi,ebp in h guards, xor ebp,ebp after the call, not before) and `mov [esp+0x18],edi` spills sit after the alloc args; per-case row/h spill registers differ.
// Tried 2026-11-01 re-pass: 21 variants (z read-zero after alloc, h>z/x>z guards, volatile w/h loads, w-before-h, n=w*h*4 local, result split decl, fm local, named string temp, Bal moves): all 175-198 vs base 175; the compiler ignores z in guards and the ebp-zero / post-alloc spill order never moved.
// Tried 2026-11-01 Bal sweep: start 0-12 (0/1 worse 182/183, 2-12 all 175 at 1540 bytes), end 0-12 on start 0/2/6 (1 gives 225 at 1532, 2+ shrink to 1304/1264 and 250+ differ): no gain, still 175.
// 2026-11-16 pass: 175 -> 128 insns differ, code+jump table now 1556 = original (was 1540): case 5 '++s' in the for-increment, g/r split (g=,r=,g|=,a=,r|=), case 6 and case 1/2 declarations in b,g,r(,a) order; ~120 variants (z forms, or-order permutations, s++ tail in case 6) rejected. Left: volatile z prologue stores (orig keeps img/0 in edi/ebp across the whole function), case 3 row in eax vs ebp (ebp is the zero in the original), cmp/test h guards in cases 2/3.
unsigned int* __fastcall FUN_004ab910(ImgW5* img, int)
{
    Bal(1);Bal(1);Bal(1);Bal(1);Bal(1);Bal(1);
    int z; *(volatile int*)&z = 0;
    int h = img->height;
    int w = img->width;
    unsigned int* result = (unsigned int*)PoolAlloc(w*h*4);
    if (result == (unsigned int*)z) throw std::logic_error(std::string(PTR_s_Out_of_memory_0050216c));
    unsigned int* d = result;
    unsigned char* row;
    switch (img->format) {
    case 0:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride)
            for (int x=0; x<w; ++x) {
                int i=row[x]*3; unsigned char* pal=img->palette;
                *d++=(((pal[i+2]|0xffffff00)<<8)|pal[i+1])<<8|pal[i];
            }
        break;
    case 1:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned short* s=(unsigned short*)row;
            for (int x=w; x>0; --x) {
                unsigned int v=*s++, b=(v&31)<<3, g=(v&0x3e0)<<6, r=(v&0x7c00)<<9;
                *d++=((g&0x1fe000|r&0xffe0001f|b)>>5)|r|g|b|0xff000000;
            }
        }
        break;
    case 2:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned short* s=(unsigned short*)row;
            for (int x=w; x>0; --x) {
                unsigned int v=*s++, b=(v&31)<<3, g=(v&0x7e0)<<5, r=(v&0xf800)<<8;
                *d++=((((g>>1)&0x1fe000)|(r&0xffe0001f)|b)>>5)|r|g|b|0xff000000;
            }
        }
        break;
    case 3:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned char* s=row;
            for (int x=w; x>0; --x, s+=3) *d++=(((s[2]|0xffffff00)<<8)|s[1])<<8|s[0];
        }
        break;
    case 4:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned int* s=(unsigned int*)row;
            for (int x=w; x>0; --x) *d++=*s++|0xff000000;
        }
        break;
    case 5:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned short* s=(unsigned short*)row;
            for (int x=w; x>0; --x, ++s) {
                unsigned int v=*s;
                unsigned int b=(v&31)<<3; b|=b>>5;
                unsigned int g=(v&0x3e0)<<6;
                unsigned int r=(v&0x7c00)<<9;
                g|=(g>>5)&0xff00;
                unsigned int a=v&0x8000;
                r|=(r>>5)&0xff0000;
                if (a) a=0xff000000;
                *d++=a|r|g|b;
            }
        }
        break;
    case 6:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned short* s=(unsigned short*)row;
            for (int x=w; x>0; --x, ++s) {
                unsigned int v=*s, b=(v&15)<<4, g=(v&0xf0)<<8, r=(v&0xf00)<<12, a=(v&0xfffff000)<<16;
                *d++=((r&0xff00000|a&0xf000000f|g&0xff000|b)>>4)|a|r|g|b;
            }
        }
        break;
    case 7:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned int* s=(unsigned int*)row;
            for (int x=w; x>0; --x) *d++=*s++;
        }
    }
    return result;
}
// NONMATCHING: RALLYTROPHY 0x4abf30
// TGA loader exception temporaries, header access widths and RLE loop scheduling differ.
// Tried 2026-10-31: 395->172: switch() for format/palette depth (cmp/je chain, signed count), pixel[8] (moves it with the string temps), bpp stores before width/height, per-branch row/step setup from a (header[17]>>5)&1 flag, ReadByte declared int (no movzx), n computed inside each RLE arm; Bal(1) sweeps (start/mid/end, 1-13) and throw/ctor forms did not change the third throw. Still differs: stack slots of the exception temps (orig pixel above them), third throw's std::string ctor/copy inlined in the original only, flag/type register swap (cl/dl).
// Tried 2026-11-01 re-pass: 6 discarded Bal(1) at function start (the sweep that matters; 3-5 give 136, 2 none) 172->114; ~35 variants: pixel/header decl order and scope (no effect or worse), inline_depth 0-255, named string/throw forms for the pixel-alloc throw; still differs: exception-temp slot (orig 0x30, ours 0x28, pixel swapped with it), esi/edi in the string copy, one string tail inlined (store) where ours calls out of line, cl/dl flag swap.
// 2026-11-16 pass: pixel[8] -> pixel[5] (any size 3-5 puts the exception temps below it, as in the original; texel reads are at most 4 bytes), row/step setup reads img->stride per branch (no hoist) 114 -> 86 insns, 2023 -> 2030 bytes; ~60 variants rejected: Bal grids, top/type declaration forms (int/uint/short top cost movzx/xor), pixel/header order, HStr twin for site-1 temp (worse), msg local (306 insns). Left: logic_error copy _Eos stays out of line at the last throw (inline budget), top/type cl/dl swap, h/w registers follow it.
int __fastcall FUN_004abf30(ImgW5* img, int, const char* name)
{
    Bal(1);Bal(1);Bal(1);Bal(1);Bal(1);Bal(1);
    File file(name,0xa1);
    if (((ImageFileOps*)&file)->Error()) throw std::logic_error(std::string("Bitmap not loaded"));
    unsigned char header[18];
    unsigned char pixel[5];   // the original reserves this little; like it, an unvalidated bpp > 40 would overrun it
    file.Read(header,18);
    ((ImageFileOps*)&file)->Skip(header[0]);
    int count=header[5]|header[6]<<8;
    switch (header[16]) { case 16: img->format=1; break; case 24: img->format=3; break; case 32: img->format=7; break; default: img->format=0; }
    if (header[2]==1 || header[2]==9) {
        img->palette=(unsigned char*)PoolAlloc(0x300);
        if (!img->palette) throw std::logic_error(std::string(PTR_s_Out_of_memory_0050216c));
        unsigned char* p=img->palette+(header[3]|header[4]<<8)*3;
        switch (header[7]) {
        case 16:
            for (; count>0; --count, p+=3) {
                file.Read(pixel,2);
                unsigned int v=pixel[0]|pixel[1]<<8;
                p[0]=(unsigned char)(v<<3); p[1]=(v>>2)&0xf8; p[2]=(v>>7)&0xf8;
            }
            break;
        case 24:
            for (; count>0; --count, p+=3) {
                file.Read(pixel,3);
                p[0]=pixel[0]; p[1]=pixel[1]; p[2]=pixel[2];
            }
            break;
        case 32:
            for (; count>0; --count, p+=3) {
                file.Read(pixel,4);
                p[0]=pixel[0]; p[1]=pixel[1]; p[2]=pixel[2];
            }
        }
    }
    img->width=header[12]|header[13]<<8;
    img->height=header[15]<<8|header[14];
    img->bpp=header[16]; img->bytesPerPixel=(img->bpp+7)>>3;
    img->stride=img->width*img->bytesPerPixel;
    img->pixels=(unsigned char*)PoolAlloc(img->bytesPerPixel*img->height*img->width);
    if (!img->pixels) throw std::logic_error(std::string(PTR_s_Out_of_memory_0050216c));
    unsigned char top=(header[17]>>5)&1;
    if (header[2]==1 || header[2]==2) {
        if (top) file.Read(img->pixels,img->bytesPerPixel*img->height*img->width);
        else {
            unsigned char* row=img->pixels+(img->height-1)*img->stride;
            int stride=img->stride;
            for (int y=0; y<img->height; row-=stride, ++y) file.Read(row,stride);
        }
    } else if (header[2]==9) {
        int h=img->height; int w=img->width; int total=w*h;
        int x=0, done=0;
        unsigned char* row; int step, stride;
        if (top) { row=img->pixels; stride=img->stride; step=stride; }
        else { row=img->pixels+(h-1)*img->stride; stride=img->stride; step=-stride; }
        while (done<total) {
            int packet=((ImageFileOps*)&file)->ReadByte();
            if (packet&128) {
                int n=(packet&127)+1;
                done+=n;
                unsigned char v=((ImageFileOps*)&file)->ReadByte();
                for (; n>0; --n) {
                    row[x++]=v;
                    if (x>=stride) { x=0; row+=step; }
                }
            } else {
                int n=(packet&127)+1;
                done+=n;
                for (; n>0; --n) {
                    row[x++]=((ImageFileOps*)&file)->ReadByte();
                    if (x>=stride) { x=0; row+=step; }
                }
            }
        }
    } else if (header[2]==10) {
        int h=img->height; int w=img->width; int total=w*h;
        int x=0, done=0;
        unsigned char* row; int step;
        if (top) { step=img->stride; row=img->pixels; }
        else { step=-img->stride; row=img->pixels+(h-1)*img->stride; }
        while (done<total) {
            int packet=((ImageFileOps*)&file)->ReadByte();
            if (packet&128) {
                int n=(packet&127)+1;
                done+=n;
                file.Read(pixel,img->bytesPerPixel);
                for (; n>0; --n) for (int k=0; k<img->bytesPerPixel; ++k) {
                    row[x++]=pixel[k];
                    if (x>=img->stride) { x=0; row+=step; }
                }
            } else {
                int n=(packet&127)+1;
                done+=n;
                n*=img->bytesPerPixel;
                for (; n>0; --n) {
                    row[x++]=((ImageFileOps*)&file)->ReadByte();
                    if (x>=img->stride) { x=0; row+=step; }
                }
            }
        }
    } else { file.Close(); return 0; }
    file.Close();
    return 1;
}

// FUNCTION: RALLYTROPHY 0x4ac730
int __fastcall FUN_004ac730(ImgW5* img, int, const char* name)
{
    unsigned char h[18];
    h[0]=28; h[1]=0; h[2]=2;
    h[3]=0; h[4]=0; h[5]=0; h[6]=0; h[7]=0;
    h[8]=0; h[9]=0; h[10]=0; h[11]=0;
    h[12]=(unsigned char)img->width; h[13]=(unsigned char)(img->width>>8);
    h[14]=(unsigned char)img->height; h[15]=(unsigned char)(img->height>>8);
    h[17]=32;
    if (img->format<5) {
        const int& height = img->height;
        ((ImageOps*)img)->Convert(3);
        h[16]=24;
        File f;
        if ((char)f.Open(name,0x82)) {
            f.Write(h,18);
            f.Write("Bugbear Entertainment Ltd. \0",h[0]);
            int n = height * img->width;
            f.Write(img->pixels,n*3);
            f.Close();
        }
    } else {
        ((ImageOps*)img)->Convert(7);
        h[16]=32;
        File f;
        if ((char)f.Open(name,0x82)) {
            f.Write(h,18);
            f.Write("Bugbear Entertainment Ltd. \0",h[0]);
            const int& height = img->height;
            int n = height * img->width;
            f.Write(img->pixels,n*4);
            f.Close();
        }
    }
    return 1;
}

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}
static __forceinline void ClampChannel(float& v)
{
    if (*(int*)&v<0) v=0.0f;
    else if (*(int*)&v>0x437f0000) v=255.0f;
}
// NONMATCHING: RALLYTROPHY 0x4ac920
// Bilinear arithmetic is complete; x87 temporaries, clamp expansion and register allocation differ.
// Tried 2026-10-30 night: byte-struct texels (PxQ), per-channel hi/lo clamp locals in the x loop, bpp branch flip, b ternary/else forms (260->140); j=ix+(ix<w-1) still sbb/neg vs original mov 1/jb/xor, sum order c3..c0 and stack slots still differ.
// Tried 2026-11-01 re-pass: 4 j=ix+(ix<w-1) forms (if/else, ++, int k; 160-196 worse) and 6 hi/lo clamp-local declaration orders (140-142, no gain); remaining 140 insns are slot allocation of hi/lo/weights, sum order and hoisted y-loop regs, too many for local edits.
// Tried 2026-11-01 Bal sweep: Bal(1) x0-12 at start crossed with end 0,1,2,3,4,6 (77 variants) all identical 140 insns/1074 bytes (no std::string in this function, so no inline budget to move); sum order c3..c0 with 7 hi/lo declaration orders, and 24 weight declaration permutations (151+ worse, 1067 bytes): no gain; original stores w0->-8,w1->-0x14,w2->-0x1c with w3 on the x87 stack and sums terms in w3,w2,w1,w0 order.
// 2026-11-16 pass: sums c3..c0, branch 'one' + byte-offset j4=(ix+one)<<2 for c1/c3, dst via img->pixels (140 -> 4 normalised hunk lines, 1085 vs 1080); left: y-loop head reloads edi=height (orig restores it with mov edi,ecx at the loop tail).
// 2026-11-18: y-loop edi reload unchanged (4 lines, 1085) by: do/while+guard, goto loop, while loop, hh/H copy in cond or body or everywhere, register h, rows countdown, height>y, y++, y declared outside, extra height use; hm=height-1 with y<=hm 54 worse.
// 2026-11-18 round 2 (slot order): 256 variants of 'blue=lo0/hi0' assigned-from-local (extra refs) = identical code (const-folded); 24 hi-perms, 24 lo-perms, 24 c0..c3 declaration orders: raw insdiff never below 216 (c orders worse, up to 410).
void __fastcall FUN_004ac920(ImgW5* img, int edx, unsigned int width, unsigned int height, unsigned int format)
{
    struct PxQ { unsigned char b,g,r,a; };
    if (width==(unsigned int)img->width && height==(unsigned int)img->height) return;
    unsigned int oldFormat=img->format;
    unsigned int* src;
    if (img->bytesPerPixel!=4) {
        src=FUN_004ab910(img,edx);
        PoolFree(img->pixels); PoolFree(img->palette);
    } else src=(unsigned int*)img->pixels;
    img->format=7;
    img->pixels=(unsigned char*)PoolAlloc(width*height*4); img->palette=0;
    unsigned int* dst=(unsigned int*)img->pixels;
    for (unsigned int y=0; y<height; ++y) {
        unsigned int fy=((y<<16)/(height-1))*((unsigned int)img->height-1);
        unsigned int iy=fy>>16;
        unsigned int* a=src+img->width*iy;
        unsigned int* b;
        if (iy>=(unsigned int)img->height-1) b=a; else b=a+img->width;
        float ty=(float)(fy&65535)*0.0000152587890625f;
        float sy=1.0f-ty;
        for (unsigned int x=0; x<width; ++x) {
            float hi0=255.0f,lo0=0.0f,hi1=255.0f,lo1=0.0f,hi2=255.0f,lo2=0.0f,hi3=255.0f,lo3=0.0f;
            unsigned int fx=((x<<16)/(width-1))*((unsigned int)img->width-1);
            unsigned int ix=fx>>16;
            unsigned int one=1; if (ix>=(unsigned int)img->width-1) one=0; unsigned int j4=(ix+one)<<2;
            PxQ c0=((PxQ*)a)[ix], c1=*(PxQ*)((char*)a+j4), c2=((PxQ*)b)[ix], c3=*(PxQ*)((char*)b+j4);
            float tx=(float)(fx&65535)*0.0000152587890625f;
            float w0=(1.0f-tx)*sy, w1=tx*sy, w2=(1.0f-tx)*ty, w3=tx*ty;
            float blue=(float)c3.b*w3+(float)c2.b*w2+(float)c1.b*w1+(float)c0.b*w0+0.5f;
            float green=(float)c3.g*w3+(float)c2.g*w2+(float)c1.g*w1+(float)c0.g*w0+0.5f;
            float red=(float)c3.r*w3+(float)c2.r*w2+(float)c1.r*w1+(float)c0.r*w0+0.5f;
            float alpha=(float)c0.a*w0+(float)c2.a*w2+(float)c1.a*w1+(float)c3.a*w3+0.5f;
            if (*(int*)&blue<*(int*)&lo0) blue=0.0f; else if (*(int*)&blue>*(int*)&hi0) blue=255.0f; if (*(int*)&green<*(int*)&lo1) green=0.0f; else if (*(int*)&green>*(int*)&hi1) green=255.0f; if (*(int*)&red<*(int*)&lo2) red=0.0f; else if (*(int*)&red>*(int*)&hi2) red=255.0f; if (*(int*)&alpha<*(int*)&lo3) alpha=0.0f; else if (*(int*)&alpha>*(int*)&hi3) alpha=255.0f;
            int ib=RoundF(blue), ig=RoundF(green), ir=RoundF(red), ia=RoundF(alpha);
            *dst++=(((ia<<8|ir)<<8|ig)<<8)|ib;
        }
    }
    PoolFree(src);
    img->width=width; img->height=height; img->stride=width*4; img->bpp=32; img->bytesPerPixel=4;
    if (format!=0xffffffff) { ((ImageOps*)img)->Convert(format); return; }
    ((ImageOps*)img)->Convert(oldFormat);
}
