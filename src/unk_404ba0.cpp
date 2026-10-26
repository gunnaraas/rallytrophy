// Early startup helpers: std::string::assign(const char*, n), static array init stubs, HashMap ctor.
#include <string>
#include "unk_404ba0.h"

template class std::basic_string<char>;

// original: FUN_00404ba0
// FUNCTION: RALLYTROPHY 0x404ba0
// SYMBOL: ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z
typedef void Inst_404ba0();

// FUNCTION: RALLYTROPHY 0x402c90
void FUN_00402c90()
{
}

// FUNCTION: RALLYTROPHY 0x402ca0
HashMap::HashMap()
{
    Init();
    Grow();
}

Elem16A DAT_00507d00[20];
Elem16B DAT_00507bc0[20];

// Compiler-generated static-init stubs of the two arrays above (vector ctor, atexit vector dtor);
// the dummy declarations only anchor the match markers.
// FUNCTION: RALLYTROPHY 0x404c80
// SYMBOL: _$E14
void Anchor0(int);
// FUNCTION: RALLYTROPHY 0x404ca0
// SYMBOL: _$E16
void Anchor1(int);
// FUNCTION: RALLYTROPHY 0x404d00
// SYMBOL: _$E19
void Anchor2(int);
// FUNCTION: RALLYTROPHY 0x404d20
// SYMBOL: _$E21
void Anchor3(int);
