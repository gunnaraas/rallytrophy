#ifndef UNK_425D80_H
#define UNK_425D80_H
#include <vector>

// One parsed credits line (8 bytes, allocated with new in 0x4263f0).
struct CreditLine {
    unsigned char color; // 0: index into the color table at 0x508930
    unsigned char pad;
    char* text;          // 4: heap copy of the line text
};

#endif
