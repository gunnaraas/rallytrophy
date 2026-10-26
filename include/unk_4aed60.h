#ifndef UNK_4AED60_H
#define UNK_4AED60_H
#include "unk_45e480.h"

// Typed view of the opaque CarTextureCache storage (0x20 bytes).
class TextureCacheState {
public:
    TextureCacheState();
    ~TextureCacheState();
    void SetDetail(int level);
    List<Texture*>::Node* Find(const char* name);
    Texture* Load(const char* name, int flags);
    Texture* LoadSpecial(const char* name, int flags);
    void Clear();
    void Upload(RenderDevice* device);
    int Count();
    int Step(RenderDevice* device);
    List<Texture*> textures;
    int deferred;
    int flags;
    int detail;
};

class TextureInsertList : public List<Texture*> {
public:
    void InsertAfter(Node* previous, Texture* texture);
};

// Additional operations on the existing, opaque Texture fields.
void __fastcall TextureAddRef(Texture* texture); // 0x4a81b0
void __fastcall TextureDefer(Texture* texture); // 0x4a82b0
const char* __fastcall TextureName(const void* name); // 0x4bc930
#endif
