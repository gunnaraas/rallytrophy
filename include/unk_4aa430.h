// Bitmap font (0x4aa430-0x4aac50): glyph table + wide-char measuring. Layout from the loader 0x4aa9c0.
#ifndef UNK_W1_H
#define UNK_W1_H

struct FontGlyph {          // 0x1c bytes, read from the .inf file
    int a;                  // 0x0
    int w;                  // 0x4 (0 = glyph absent)
    int b;                  // 0x8
    char unkC[0x10];
};

class FontTexture {         // 0x20-byte image object, ctor 0x4aacd0
public:
    FontTexture(const char* path);
    char data[0x20];
};

class GlyphFont {
public:
    FontGlyph* glyphs;      // 0x0
    char unk4[4];
    unsigned short* ids;    // 0x8
    unsigned short* map;    // 0xc
    char unk10[0xc];
    float scaleX;           // 0x1c
    float scaleY;           // 0x20
    int spaceW;             // 0x24
    int spacing;            // 0x28
    int lineH;              // 0x2c
    int mapCount;           // 0x30
    int firstChar;          // 0x34
    char unk38[4];
    int glyphCount;         // 0x3c
    char unk40[0x11c - 0x40];
    FontTexture* texture;   // 0x11c

    float GetWidth(const char* text, int raw);                 // 0x4aa430
    float GetWidthW(const unsigned short* text, int raw);      // 0x4aa460
    void Measure(const unsigned short* text, float sx, float sy, float* out); // 0x4aa5a0
    void Load(const char* name);                               // 0x4aa9c0
};

#endif
