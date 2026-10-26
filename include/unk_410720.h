/* StageScene::LoadEffect (0x410720): river/waterfall/water material effects. Include after unk_40c830.h. */
#ifndef UNK_410720_H
#define UNK_410720_H
struct EffectUV { float x, y; };
extern EffectUV DAT_00508438, DAT_00508430, DAT_00508428, DAT_00508420;  // waterfall layers: scale1, scale2, speed1, speed2
extern EffectUV DAT_00508418, DAT_00508410;  // river1 scale, speed
extern EffectUV DAT_00508408, DAT_00508400;  // river2 scale, speed
extern EffectUV DAT_005083f8, DAT_005083f0, DAT_005083e8;  // water scale, speed1, speed2
extern float DAT_005083e0;                   // water animation rate
extern char DAT_004f3ba0[];                  // "data/global/effects/water_loop1000.tga" (digits patched in place)
#endif
