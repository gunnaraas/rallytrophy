#ifndef UNK_415370_H
#define UNK_415370_H
// Stage sound setup (0x415370): ambient sound + events from data/stage<N>.snd, then the per-surface
// roll/ground/slide/brake/acc sound sets (16 surfaces) from data/global/surfacesounds/<name>.
class StageSoundLoader {
public:
    void LoadSounds();   // 0x415370
};
#endif
