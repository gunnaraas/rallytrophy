// Bonus-unlock helpers (0x452380-0x4524ff). Layout notes: analysis/range-450ee0.md section 2.
#ifndef UNK_452380_H
#define UNK_452380_H

class Player;

extern BYTE DAT_005014c8;  // one-shot flag of 0x452470
extern BYTE DAT_005014c9;  // one-shot flag of 0x452380
extern DWORD DAT_005014cc; // unlocked-car mask (normal)
extern DWORD DAT_005014d0; // unlocked-car mask (when DAT_00501458)

int MarkCarUnlocked(int model);                      // original: FUN_00452410 (cdecl)
// The two bonus checks are GameView::FUN_00452380 / FUN_00452470 (include/unk_481390.h).

#endif
