#pragma once
#include "unk_4ab010.h"
#include "rallytrophy.h"

// Method views of existing layouts; neither adds storage.
class ImageOps : public ImgW5 {
public:
    void Convert(unsigned int); // 0x4ab010
};
class ImageFileOps : public File {
public:
    int Error(); // 0x4a5fa0
    void Skip(int); // 0x4a5f40
    unsigned char ReadByte(); // 0x4a5ec0
};
unsigned int* __fastcall FUN_004ab910(ImgW5*, int);
int __fastcall FUN_004abf30(ImgW5*, int, const char*);
int __fastcall FUN_004ac730(ImgW5*, int, const char*);
void __fastcall FUN_004ac920(ImgW5*, int, unsigned int, unsigned int, unsigned int);
