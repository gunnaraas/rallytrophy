// CarModelCache (G9). Only the ctor is attempted exactly; the rest need VC6 std::list/vector EH instantiations.
#include "unk_4ade40.h"

/* FUNCTION: RALLYTROPHY 0x4ade40 */
CarModelCache::CarModelCache()
{
    char u2, u; // original stores uninitialized bytes into the two flags
    listHead = 0;
    *(volatile char*)&b1c = u;
    listTail = 0;
    freeList = 0;
    freeCnt = 0;
    v20 = 0;
    v24 = 0;
    v28 = 0;
    b2c = u2;
    v30 = 0;
    usedCnt = 0;
    v34 = 0;
    v38 = 0;
    device = 0;
    f0 = 0;
}
