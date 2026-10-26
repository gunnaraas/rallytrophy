// Bonus-unlock helpers (0x452380-0x45252f): mark cars unlocked in the global masks at the end of a race.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_452380.h"

// FUNCTION: RALLYTROPHY 0x452380
int GameView::FUN_00452380(int position, Player* p)
{
    int r = 0;
    switch (DAT_0050132c) {
    case 0:
        if (position == 1 && MarkCarUnlocked(p->unk2c)) {
            r = 1;
        }
        break;
    case 1:
        if (position <= 2 && MarkCarUnlocked(p->unk2c)) {
            r = 1;
        }
        break;
    case 2:
        if (position <= 3 && MarkCarUnlocked(p->unk2c)) {
            r = 1;
        }
        break;
    }
    if (!DAT_005014c9 && DAT_0050132c == 1) {
        r |= 2;
        DAT_005014c9 = 1;
    }
    return r;
}

// FUNCTION: RALLYTROPHY 0x452410
int MarkCarUnlocked(int model)
{
    int r = 0;
    DWORD bit = (1 << ((model - 1) & 0xfffe)) << 1;
    if (DAT_00501458) {
        if (!(bit & DAT_005014d0)) {
            DAT_005014d0 |= bit;
            r = 1;
        }
    } else {
        if (!(bit & DAT_005014cc)) {
            DAT_005014cc |= bit;
            r = 1;
        }
    }
    return r;
}

// FUNCTION: RALLYTROPHY 0x452470
int GameView::FUN_00452470(int position, Player* p)
{
    int r = 0;
    switch (DAT_0050132c) {
    case 0:
        if (position == 1 && MarkCarUnlocked(p->unk2c)) {
            r = 1;
        }
        break;
    case 1:
        if (position <= 3 && MarkCarUnlocked(p->unk2c)) {
            r = 1;
        }
        break;
    case 2:
        if (position <= 6 && MarkCarUnlocked(p->unk2c)) {
            r = 1;
        }
        break;
    }
    if (!DAT_005014c8 && DAT_0050132c == 1 && p->unk1a5c <= 6) {
        r |= 2;
        DAT_005014c8 = 1;
    }
    return r;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x452500
// SYMBOL: _$E23
typedef void Generated_452500();

// FUNCTION: RALLYTROPHY 0x452520
// SYMBOL: _$E22
typedef void Generated_452520();
