/* Spatial grid owner (0x40f390): fills each cell's object lists. Cell = 0xb0 bytes (TimedLists at +0x0c, int vectors at +0x60..+0xa0). */
#ifndef UNK_40F390_H
#define UNK_40F390_H
class StageGrid {
public:
    void FillCells();                  // 0x40f390
    BYTE unk0[0x130];
    float radius;                      // 0x130
    BYTE unk134[0x188 - 0x134];
    float cellW;                       // 0x188
    BYTE unk18c[4];
    float cellH;                       // 0x190
    int cols;                          // 0x194
    int rows;                          // 0x198
    BYTE* cells;                       // 0x19c
};
#endif
