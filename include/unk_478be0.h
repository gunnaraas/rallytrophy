/* Shared sprite tables of the particle effects (0x478be0-0x478e7f). Only
 * src/unk_478be0.cpp and the emitters that read the tables include this: the
 * six static data members consume _$E/_$S numbers in the including
 * translation unit, so unk_47c460.h must not declare them. Include after
 * unk_481390.h and unk_47c460.h. They are static members (not plain globals)
 * because their destructor thunks carry guard bits, which only static data
 * members of a class produce. The owning class is unknown; Effect::InitTables
 * (0x478f50) fills them. */
#ifndef UNK_478BE0_H
#define UNK_478BE0_H

class EffectTables {
public:
    static Vec2 sinCos[1024];          // 0x561ec0: (cos, sin) of i * 2pi / 1024
    static Vec2 uv8x8[64];             // 0x561cb8: 8x8 atlas cell origins (j / 8, i / 8)
    static Vec2 uv4x4a[16];            // 0x561c38: 4x4 sub-cell offsets (j / 4, i / 4)
    static Vec2 uv4x4b[16];            // 0x561bb8: 4x4 offsets (j / 32, i / 32)
    static Vec2 uv2x2a[4];             // 0x561b98: 2x2 offsets (j / 16, i / 16)
    static Vec2 uv2x2b[4];             // 0x561b78: 2x2 offsets (j / 8, i / 8)
};

#endif
