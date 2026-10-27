// Front-end video helpers (0x452530-0x452b1f): the victory video and the per-country videos.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_452530.h"

// original: FUN_00452530 (cdecl, no arguments)
// FUNCTION: RALLYTROPHY 0x452530
void PlayVictoryVideo()
{
    BinkVideo* video = new BinkVideo("data/video/victory.bik", DAT_0056b0ac, 1);
    int skip = 0;
    int first = 1;
    DAT_0056b0ac->SetRenderState(1, 2);
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->SetRenderState(13, 0);
    DAT_0056b0ac->vf42(8);
    while (FUN_004b69f0(0) && !skip) {
        if (first) {
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            first = 0;
        } else if (DAT_0057ba20) {
            goto pressed;
        }
        if (DAT_0057ba28[0x1b] || DAT_0057ba28[0x0d] || DAT_0057ba28[0x20]) {
        pressed:
            skip = 1;
        }
        int r = video->Update();
        if (r != -1) {
            if (r != 1) {
                FUN_004a18d0(500);
            } else if (DAT_0056b0ac->BeginScene()) {
                video->Render(DAT_0056b0ac);
                DAT_0056b0ac->EndScene();
                DAT_0056b0ac->Present();
            }
        } else {
            skip = 1;
        }
    }
    if (video) {
        delete video;
    }
    DAT_0056b0bc->ClearKeys();
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
}

// The caption fonts at GameView+0x1390c4 / +0x1390c0 as this file uses them.
struct CaptionFont {
    BYTE unk0[0x1c];
    float scaleX;                      // 0x1c
    float scaleY;                      // 0x20
    BYTE unk24[8];
    int lineHeight;                    // 0x2c
    BYTE unk30[0x120 - 0x30];
    DWORD color;                       // 0x120
    BYTE unk124[0x130 - 0x124];
    BYTE shadow;                       // 0x130
    BYTE unk131[3];
    float shadowX;                     // 0x134
    float shadowY;                     // 0x138
    void SetScale(float x, float y)
    {
        scaleX = x;
        scaleY = y;
    }
    float GetWidth(const char* text, int flags); // 0x4aa430
    void Print(RenderDevice* device, float x, float y, const char* text, int flags, float sx, float sy); // 0x4a97a0
};

// NONMATCHING: RALLYTROPHY 0x4526e0
// Same size (1040), 21 bytes differ: the original issues the `push 0` of the first FUN_004b69f0(0) after the four
// caption coordinate stores, VC6 hoists it above the first fsubr here (frame offsets shift by 4). Source variants tried: float
// order, inline coordinates, loop forms. Empty inline-asm barrier disables frame-pointer omission and causes 558 diff bytes.
void GameView::ShowCountryVideo(int n)
{
    char path[24] = "data/video/country1.bik";
    path[18] = (char)(n + '0');
    BinkVideo* video = new BinkVideo(path, DAT_0056b0ac, 0);
    OverlayImage* image = new OverlayImage(DAT_0056b0ac, "data/menu/ImageCountryVideo.tga", 1, 1);
    image->SetGrid(5, 6);
    CaptionFont* smallFont = *(CaptionFont**)((char*)this + 0x1390c4);
    CaptionFont* bigFont = *(CaptionFont**)((char*)this + 0x1390c0);
    smallFont->SetScale(DAT_0056aea8, DAT_0056aeac);
    bigFont->SetScale(DAT_0056aea8, DAT_0056aeac);
    smallFont->color = 0xffffffff;
    bigFont->color = 0xff000000;
    bigFont->shadow = 1;
    bigFont->shadowY = bigFont->lineHeight * DAT_0056aeac;
    bigFont->shadowX = DAT_0056aea8 * 780.0f;
    int skip = 0;
    int first = 1;
    DAT_0056b0ac->SetRenderState(1, 2);
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->SetRenderState(13, 0);
    DAT_0056b0ac->vf42(8);
    const char* caption1 = FUN_00405380(n + 0xb4);
    const char* caption2 = FUN_00405380(n + 0xb9);
    float x1 = DAT_0056aea8 * 512.0f - smallFont->GetWidth(caption1, 0) * 0.5f;
    float y1 = DAT_0056aeac * 414.0f;
    float x2 = DAT_0056aea8 * 122.0f;
    float y2 = DAT_0056aeac * 465.0f;
    while (FUN_004b69f0(0) && !skip) {
        DAT_0056b0c8->FUN_00432c00();
        if (first) {
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            first = 0;
        } else if (DAT_0057ba20) {
            goto pressed;
        }
        if (DAT_0057ba28[0x1b] || DAT_0057ba28[0x0d] || DAT_0057ba28[0x20]) {
        pressed:
            skip = 1;
        }
        if (!(video->Update() - 1) && DAT_0056b0ac->m_active && DAT_0056b0ac->BeginScene()) {
            video->Render(DAT_0056b0ac);
            image->Draw(DAT_0056b0ac, 0);
            smallFont->Print(DAT_0056b0ac, x1, y1, caption1, 0, 1.0f, 1.0f);
            int width = DAT_0056b0ac->m_width;
            smallFont->Print(DAT_0056b0ac, width * 0.5f - smallFont->GetWidth(FUN_00405380(0x117), 0) * 0.5f,
                         DAT_0056aeac * 710.0f, FUN_00405380(0x117), 0, 1.0f, 1.0f);
            bigFont->Print(DAT_0056b0ac, x2, y2, caption2, 0, 1.0f, 1.0f);
            DAT_0056b0ac->EndScene();
            DAT_0056b0ac->Present();
        }
    }
    bigFont->shadow = 0;
    if (image) {
        delete image;
    }
    if (video) {
        delete video;
    }
    DAT_0056b0bc->ClearKeys();
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x452af0
// SYMBOL: _$E23
typedef void Generated_452af0();

// FUNCTION: RALLYTROPHY 0x452b10
// SYMBOL: _$E22
typedef void Generated_452b10();
