#ifndef UNK_403270_H
#define UNK_403270_H
#include <string>

// Resource owned by a SpriteA (virtual destructor, deleted through the base pointer).
class Res403 {
public:
    virtual ~Res403();
    Res403(const char* path, int flags, int mode);   // 0x4a8030
    void Load(void* device);                          // 0x4a8350
    int data[0x19];                                   // 0x68 bytes in all
};
// Small quad objects used by the loading screen.
class SpriteA {                                       // ctor 0x4a8580, dtor 0x4a86d0 (+0 = resource)
public:
    Res403* res;
    int pad[0x26];
    int f9c;
    int tail[0xd];                                    // 0xd4 bytes in all
    SpriteA();
    ~SpriteA();
};
class SpriteB {                                       // ctor 0x4a8ae0, dtor 0x4a8bb0
public:
    SpriteB(void* device, const char* texture, int a, int b);
    ~SpriteB();
    void SetBlend(int a, int b);                      // 0x4a9280
    void Draw(void* device, int unused);              // 0x4a92c0
};

// The loading-screen object (size 0x34): ctor 0x403270, dtor 0x403380, init 0x4033f0, draw 0x403890.
class LoadScreen {
public:
    SpriteA* quad;      // +0x00
    SpriteB* back;      // +0x04
    SpriteB* front;     // +0x08
    int lastDraw;       // +0x0c
    int lastLevel;      // +0x10
    std::string text;   // +0x14
    void* font;         // +0x24 text renderer
    float offset;       // +0x28
    float scale;        // +0x2c
    int mode;           // +0x30
    LoadScreen();
    ~LoadScreen();
    void Cleanup();
    void Init(int level);
    void SetText(const char* s);
    void* Self();
    void Draw(float t);
};

// Header of a savegame/replayN.dat (0x70 bytes).
struct ReplayHeader { unsigned char raw[0x70]; };
unsigned int LoadReplay(ReplayHeader* hdr, int index);
#endif
