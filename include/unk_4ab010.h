#pragma once
// Pixel-format conversion on a VidImage-like object (0x4ab010).
struct ImgW5 {
    int width, height, stride, bpp, bytesPerPixel, format;
    unsigned char* pixels;
    unsigned char* palette;
};
