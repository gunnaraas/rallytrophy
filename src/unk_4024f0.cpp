// VC6 <string>/<stdexcept> instantiations at the start of the image (see include/unk_4024f0.h).
#include <string>
#include <stdexcept>
#include "unk_4024f0.h"

template class std::basic_string<char>;

// FUNCTION: RALLYTROPHY 0x4024f0
// SYMBOL: ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z
// std::string::assign(const string&, pos, n)
typedef void Inst_4024f0();

// FUNCTION: RALLYTROPHY 0x402700
// SYMBOL: ?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z
// std::string::_Tidy(bool)
typedef void Inst_402700();

// FUNCTION: RALLYTROPHY 0x402960
// SYMBOL: ?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z
// std::string::_Eos(size_t)
typedef void Inst_402960();

// FUNCTION: RALLYTROPHY 0x402980
// SYMBOL: ?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z
// std::string::_Grow(size_t, bool)
typedef void Inst_402980();

// FUNCTION: RALLYTROPHY 0x402ab0
// SYMBOL: ?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z
// std::string::_Copy(size_t)
typedef void Inst_402ab0();

// FUNCTION: RALLYTROPHY 0x402bd0
// SYMBOL: ?_Split@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXXZ
// std::string::_Split()
typedef void Inst_402bd0();

// FUNCTION: RALLYTROPHY 0x402aa0
// SYMBOL: ?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ
// std::string::max_size()
typedef void Inst_402aa0();

// Forces the implicit std::logic_error members into this object.
void UseLogicError(const std::logic_error& e, std::logic_error* p)
{
    std::logic_error copy(e);
    delete p;
    throw copy;
}

// FUNCTION: RALLYTROPHY 0x402750
// SYMBOL: ??1logic_error@std@@UAE@XZ
// std::logic_error::~logic_error
typedef void Inst_402750();

// FUNCTION: RALLYTROPHY 0x4027d0
// SYMBOL: ??_Glogic_error@std@@UAEPAXI@Z
// std::logic_error scalar deleting destructor
typedef void Inst_4027d0();

// FUNCTION: RALLYTROPHY 0x4027f0
// SYMBOL: ??0logic_error@std@@QAE@ABV01@@Z
// std::logic_error copy constructor
typedef void Inst_4027f0();
