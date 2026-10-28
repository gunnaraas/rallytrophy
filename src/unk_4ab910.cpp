#include <string>
#include <stdexcept>
#include "rallytrophy.h"
#include "unk_4ab910.h"
extern char* PTR_s_Out_of_memory_0050216c;
extern void* __cdecl PoolAlloc(unsigned int);
extern void __cdecl PoolFree(void*);
// NONMATCHING: RALLYTROPHY 0x4ab910
// MSVC6 exception temporary layout and per-format loop register allocation differ.
unsigned int* __fastcall FUN_004ab910(ImgW5* img, int)
{
    int w = img->width;
    int h = img->height;
    unsigned int* result = (unsigned int*)PoolAlloc(w*h*4);
    if (!result) throw std::logic_error(std::string(PTR_s_Out_of_memory_0050216c));
    unsigned int* d = result;
    unsigned char* row;
    switch (img->format) {
    case 0:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride)
            for (int x=0; x<w; ++x) {
                unsigned char* p=img->palette+row[x]*3;
                *d++=(((p[2]|0xffffff00)<<8)|p[1])<<8|p[0];
            }
        break;
    case 1:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned short* s=(unsigned short*)row;
            for (int x=w; x>0; --x) {
                unsigned int v=*s++, r=(v&0x7c00)<<9, g=(v&0x3e0)<<6, b=(v&31)<<3;
                *d++=((g&0x1fe000|r&0xffe0001f|b)>>5)|r|g|b|0xff000000;
            }
        }
        break;
    case 2:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned short* s=(unsigned short*)row;
            for (int x=w; x>0; --x) {
                unsigned int v=*s++, r=(v&0xf800)<<8, b=(v&31)<<3;
                *d++=(((v&0x600)<<4|r&0xffe0001f|b)>>5)|r|(v&0x7e0)<<5|b|0xff000000;
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
            for (int x=w; x>0; --x) {
                unsigned int v=*s++, a=v&0x8000;
                if (a) a=0xff000000;
                *d++=a|(v&0x7c00)<<9|(v&0x7000)<<4|(v&0x3e0)<<6|(v&0x380)<<1|(v&31)<<3|(v&31)>>2;
            }
        }
        break;
    case 6:
        row=img->pixels;
        for (; h>0; --h, row+=img->stride) {
            unsigned short* s=(unsigned short*)row;
            for (int x=w; x>0; --x) {
                unsigned int v=*s++, a=(v&0xfffff000)<<16, r=(v&0xf00)<<12, g=(v&0xf0)<<8, b=(v&15)<<4;
                *d++=((a|r|g|b)>>4)|a|r|g|b;
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
int __fastcall FUN_004abf30(ImgW5* img, int, const char* name)
{
    File file(name,0xa1);
    if (((ImageFileOps*)&file)->Error()) throw std::logic_error(std::string("Bitmap not loaded"));
    unsigned char header[18];
    unsigned char pixel[4];
    file.Read(header,18);
    ((ImageFileOps*)&file)->Skip(header[0]);
    unsigned int count=header[5]|header[6]<<8;
    img->format=header[16]==16?1:header[16]==24?3:header[16]==32?7:0;
    if (header[2]==1 || header[2]==9) {
        img->palette=(unsigned char*)PoolAlloc(0x300);
        if (!img->palette) throw std::logic_error(std::string(PTR_s_Out_of_memory_0050216c));
        unsigned char* p=img->palette+(header[3]|header[4]<<8)*3;
        if (header[7]==16) {
            for (; count; --count, p+=3) {
                file.Read(pixel,2);
                unsigned int v=pixel[0]|pixel[1]<<8;
                p[0]=pixel[0]<<3; p[1]=(v>>2)&0xf8; p[2]=(v>>7)&0xf8;
            }
        } else if (header[7]==24) {
            for (; count; --count, p+=3) {
                file.Read(pixel,3);
                p[0]=pixel[0]; p[1]=pixel[1]; p[2]=pixel[2];
            }
        } else if (header[7]==32) {
            for (; count; --count, p+=3) {
                file.Read(pixel,4);
                p[0]=pixel[0]; p[1]=pixel[1]; p[2]=pixel[2];
            }
        }
    }
    img->width=header[12]|header[13]<<8;
    img->height=header[14]|header[15]<<8;
    img->bpp=header[16]; img->bytesPerPixel=(img->bpp+7)>>3;
    img->stride=img->width*img->bytesPerPixel;
    img->pixels=(unsigned char*)PoolAlloc(img->stride*img->height);
    if (!img->pixels) throw std::logic_error(std::string(PTR_s_Out_of_memory_0050216c));
    unsigned char* row=img->pixels;
    int step=img->stride;
    if (!(header[17]&0x20)) { row+=step*(img->height-1); step=-step; }
    if (header[2]==1 || header[2]==2) {
        if (!(header[17]&0x20)) {
            for (int y=0; y<img->height; ++y, row-=img->stride) file.Read(row,img->stride);
        } else file.Read(row,img->bytesPerPixel*img->height*img->width);
    } else if (header[2]==9) {
        int x=0, done=0, total=img->width*img->height;
        while (done<total) {
            unsigned char packet=((ImageFileOps*)&file)->ReadByte();
            int n=(packet&127)+1;
            if (packet&128) {
                unsigned char v=((ImageFileOps*)&file)->ReadByte();
                for (int k=n; k; --k) {
                    row[x++]=v;
                    if (x>=img->stride) { x=0; row+=step; }
                }
            } else {
                for (int k=n; k; --k) {
                    row[x++]=((ImageFileOps*)&file)->ReadByte();
                    if (x>=img->stride) { x=0; row+=step; }
                }
            }
            done+=n;
        }
    } else if (header[2]==10) {
        int x=0, done=0, total=img->width*img->height;
        while (done<total) {
            unsigned char packet=((ImageFileOps*)&file)->ReadByte();
            int n=(packet&127)+1;
            done+=n;
            if (packet&128) {
                file.Read(pixel,img->bytesPerPixel);
                for (; n; --n) for (int k=0; k<img->bytesPerPixel; ++k) {
                    row[x++]=pixel[k];
                    if (x>=img->stride) { x=0; row+=step; }
                }
            } else {
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

// NONMATCHING: RALLYTROPHY 0x4ac730
// Original size reproduced; width/height multiply operands are reversed (10 differing bytes). Reversing the
// accumulator order or writing a direct height*width expression leaves those bytes unchanged; separate width/height
// locals add 4 bytes and 191 additional differences; a volatile height read adds 4 bytes and 190 differences.
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
        ((ImageOps*)img)->Convert(3);
        h[16]=24;
        File f;
        if ((char)f.Open(name,0x82)) {
            f.Write(h,18);
            f.Write("Bugbear Entertainment Ltd. \0",h[0]);
            int n=img->width;
            n*=img->height;
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
            int n=img->width;
            n*=img->height;
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
void __fastcall FUN_004ac920(ImgW5* img, int edx, unsigned int width, unsigned int height, unsigned int format)
{
    if (width==(unsigned int)img->width && height==(unsigned int)img->height) return;
    unsigned int oldFormat=img->format;
    unsigned int* src;
    if (img->bytesPerPixel==4) src=(unsigned int*)img->pixels;
    else {
        src=FUN_004ab910(img,edx);
        PoolFree(img->pixels); PoolFree(img->palette);
    }
    img->format=7;
    unsigned int* dst=(unsigned int*)PoolAlloc(width*height*4);
    img->pixels=(unsigned char*)dst; img->palette=0;
    for (unsigned int y=0; y<height; ++y) {
        unsigned int fy=((y<<16)/(height-1))*((unsigned int)img->height-1);
        unsigned int iy=fy>>16;
        unsigned int* a=src+img->width*iy;
        unsigned int* b=a;
        if (iy<(unsigned int)img->height-1) b=a+img->width;
        float ty=(float)(fy&65535)*0.0000152587890625f;
        float sy=1.0f-ty;
        for (unsigned int x=0; x<width; ++x) {
            unsigned int fx=((x<<16)/(width-1))*((unsigned int)img->width-1);
            unsigned int ix=fx>>16;
            unsigned int c0=a[ix], j=(ix<(unsigned int)img->width-1)+ix;
            unsigned int c2=b[ix], c1=a[j], c3=b[j];
            float tx=(float)(fx&65535)*0.0000152587890625f;
            float w0=(1.0f-tx)*sy, w1=tx*sy, w2=(1.0f-tx)*ty, w3=tx*ty;
            float blue=(float)(c0&255)*w0+(float)(c1&255)*w1+(float)(c2&255)*w2+(float)(c3&255)*w3+0.5f;
            float green=(float)(c0>>8&255)*w0+(float)(c1>>8&255)*w1+(float)(c2>>8&255)*w2+(float)(c3>>8&255)*w3+0.5f;
            float red=(float)(c0>>16&255)*w0+(float)(c1>>16&255)*w1+(float)(c2>>16&255)*w2+(float)(c3>>16&255)*w3+0.5f;
            float alpha=(float)(c0>>24)*w0+(float)(c1>>24)*w1+(float)(c2>>24)*w2+(float)(c3>>24)*w3+0.5f;
            ClampChannel(blue); ClampChannel(green); ClampChannel(red); ClampChannel(alpha);
            int ib=RoundF(blue), ig=RoundF(green), ir=RoundF(red), ia=RoundF(alpha);
            *dst++=(((ia<<8|ir)<<8|ig)<<8)|ib;
        }
    }
    PoolFree(src);
    img->width=width; img->height=height; img->stride=width*4; img->bpp=32; img->bytesPerPixel=4;
    if (format!=0xffffffff) { ((ImageOps*)img)->Convert(format); return; }
    ((ImageOps*)img)->Convert(oldFormat);
}
