#include "unk_4aed60.h"
#include <string.h>

typedef List<Texture*> TextureList;

__declspec(naked)
// PE transcription preserves the original free-list and successor-link update order.
// FUNCTION: RALLYTROPHY 0x4aed60
void TextureInsertList::InsertAfter(Node*, Texture*)
{
    __asm {
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 028h
        _emit 06ah
        _emit 00ch
        _emit 0e8h
        _emit 005h
        _emit 025h
        _emit 001h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 016h
        _emit 0c7h
        _emit 040h
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 040h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ebh
        _emit 00ch
        _emit 033h
        _emit 0c0h
        _emit 0ebh
        _emit 008h
        _emit 08bh
        _emit 008h
        _emit 089h
        _emit 04eh
        _emit 008h
        _emit 0ffh
        _emit 04eh
        _emit 010h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 008h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 00ch
        _emit 089h
        _emit 050h
        _emit 008h
        _emit 08bh
        _emit 011h
        _emit 089h
        _emit 048h
        _emit 004h
        _emit 089h
        _emit 001h
        _emit 08bh
        _emit 0cah
        _emit 085h
        _emit 0c9h
        _emit 089h
        _emit 010h
        _emit 074h
        _emit 00ah
        _emit 089h
        _emit 041h
        _emit 004h
        _emit 0ffh
        _emit 046h
        _emit 00ch
        _emit 05eh
        _emit 0c2h
        _emit 008h
        _emit 000h
        _emit 089h
        _emit 046h
        _emit 004h
        _emit 0ffh
        _emit 046h
        _emit 00ch
        _emit 05eh
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x4aee00
TextureCacheState::TextureCacheState()
{
    deferred = 0;
    flags = 0;
    detail = 2;
}

// FUNCTION: RALLYTROPHY 0x4aee20
TextureCacheState::~TextureCacheState()
{
    Clear();
}

// FUNCTION: RALLYTROPHY 0x4aeec0
void TextureCacheState::SetDetail(int level)
{
    detail = level;
    flags = 0;
    if (level == 0) { flags = 0x100; return; }
    if (level == 1) flags = 0x200;
}

// FUNCTION: RALLYTROPHY 0x4aeef0
TextureList::Node* TextureCacheState::Find(const char* name)
{
    TextureList::Node* node = textures.m_head;
    for (;;) {
        if (!node) return 0;
        if (_stricmp(name, TextureName((char*)node->data + 4)) == 0) break;
        node = node->next;
    }
    return node;
}

static __forceinline void AppendTexture(TextureList* list, Texture* texture)
{
    TextureList::Node* node = list->m_free;
    if (!node) {
        node = (TextureList::Node*)operator new(sizeof(TextureList::Node));
        if (node) {
            node->prev = 0;
            node->next = 0;
            node->data = 0;
        } else node = 0;
    } else {
        list->m_free = node->next;
        list->m_freeCount--;
    }
    TextureList::Node* tail = list->m_tail;
    node->data = texture;
    node->prev = tail;
    node->next = 0;
    if (tail) tail->next = node;
    else list->m_head = node;
    list->m_tail = node;
    list->m_count++;
}

// FUNCTION: RALLYTROPHY 0x4aef30
Texture* TextureCacheState::Load(const char* name, int loadFlags)
{
    TextureList::Node* node = Find(name);
    if (node) {
        Texture* texture = node->data;
        TextureAddRef(texture);
        return texture;
    }
    Texture* texture = new Texture(name, flags | loadFlags, 1);
    if (deferred) TextureDefer(texture);
    AppendTexture(&textures, texture);
    return texture;
}

// FUNCTION: RALLYTROPHY 0x4af030
Texture* TextureCacheState::LoadSpecial(const char* name, int loadFlags)
{
    TextureList::Node* node = Find(name);
    if (node) return node->data;
    Texture* texture = new Texture(name, loadFlags, 1);
    AppendTexture(&textures, texture);
    *(int*)((char*)texture + 0x10) = 2;
    return texture;
}

// FUNCTION: RALLYTROPHY 0x4af120
void TextureCacheState::Clear()
{
    TextureList::Node* node = textures.m_head;
    while (node) {
        delete node->data;
        textures.m_head = node->next;
        if (node == textures.m_tail) textures.m_tail = node->prev;
        else textures.m_head->prev = node->prev;
        node->next = textures.m_free;
        node->prev = 0;
        textures.m_free = node;
        textures.m_freeCount++;
        textures.m_count--;
        node = textures.m_head;
    }
}

// FUNCTION: RALLYTROPHY 0x4af180
void TextureCacheState::Upload(RenderDevice* device)
{
    for (TextureList::Node* node = textures.m_head; node; node = node->next) {
        if ((*(unsigned char*)((char*)node->data + 0xc) & 1) == 0)
            node->data->Upload(device);
    }
}

// FUNCTION: RALLYTROPHY 0x4af1b0
int TextureCacheState::Count()
{
    return textures.m_count;
}

// FUNCTION: RALLYTROPHY 0x4af1c0
int TextureCacheState::Step(RenderDevice* device)
{
    TextureList::Node* node = textures.m_head;
    int count = 0;
    Texture* texture;
    while (node) {
        texture = node->data;
        ++count;
        if ((*(unsigned char*)((char*)texture + 0xc) & 1) == 0) {
            texture->Upload(device);
            break;
        }
        node = node->next;
    }
    return count;
}
