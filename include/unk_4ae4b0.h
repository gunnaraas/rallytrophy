#ifndef UNK_4AE4B0_H
#define UNK_4AE4B0_H
#include "rallytrophy.h"
#include "unk_4ade40.h"
#include "unk_4b9d30.h"
// Plugin buffer interface shared by vertex and index buffers.
class ModelUploadBuffer {
public:
    virtual void Slot0();
    virtual void* Lock(int start, int count, int flags);
    virtual int Unlock();
    virtual void Finish();
    virtual void SetVertices(int start, const void* vertices, int count);
};
class ModelUploadDevice {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Slot8();
    virtual void Slot9();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void Slot14();
    virtual void Slot15();
    virtual void Slot16();
    virtual void Slot17();
    virtual void Slot18();
    virtual void Slot19();
    virtual void Slot20();
    virtual void Slot21();
    virtual void Slot22();
    virtual void Slot23();
    virtual void Slot24();
    virtual void Slot25();
    virtual void Slot26();
    virtual void Slot27();
    virtual void Slot28();
    virtual void Slot29();
    virtual void Slot30();
    virtual void Slot31();
    virtual void Slot32();
    virtual void Slot33();
    virtual void Slot34();
    virtual void Slot35();
    virtual void Slot36();
    virtual void Slot37();
    virtual void Slot38();
    virtual void Slot39();
    virtual void Slot40();
    virtual void Slot41();
    virtual void Slot42();
    virtual void Slot43();
    virtual void Slot44();
    virtual void Slot45();
    virtual void Slot46();
    virtual void Slot47();
    virtual void Slot48();
    virtual void Slot49();
    virtual void Slot50();
    virtual void Slot51();
    virtual void Slot52();
    virtual void Slot53();
    virtual void Slot54();
    virtual void Slot55();
    virtual void Slot56();
    virtual void Slot57();
    virtual void Slot58();
    virtual void Slot59();
    virtual void Slot60();
    virtual void Slot61();
    virtual void Slot62();
    virtual void Slot63();
    virtual void Slot64();
    virtual void Slot65();
    virtual void Slot66();
    virtual void Slot67();
    virtual void Slot68();
    virtual void Slot69();
    virtual void Slot70();
    virtual ModelUploadBuffer* CreateVertices(int format, int count, int flags, int type);
    virtual void Slot72();
    virtual ModelUploadBuffer* CreateIndices(int count, int flags, int type);
};
class ModelBufferVector : public std::vector<ModelUploadBuffer*> {
public:
    void Insert(iterator pos, int count, ModelUploadBuffer* const& buffer); // 0x4bc680
};
// Adds operations to the existing cache layout without changing its storage.
class Mesh;
class ModelCacheUpload : public CarModelCache {
public:
    void UploadVertices(int count, unsigned char flags, std::vector<Mesh*>* meshes);
    void UploadIndices(int count, std::vector<Mesh*>* meshes);
};
// 0x24-byte upload format: first nine words of a ShadowVertex.
struct ModelUploadVertex {
    int words[9];
    ~ModelUploadVertex() {}
};
struct ModelUploadVertexUV {
    int words[11];
    ~ModelUploadVertexUV() {}
};
struct ModelUploadCreateMessage {
    ModelUploadCreateMessage(const char* s, const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { Tidy(false); Assign(s, strlen(s)); }
    ~ModelUploadCreateMessage() { ((std::string*)this)->~basic_string(); }
    void Tidy(bool built); // 0x402700
    void Assign(const char* text, unsigned int count); // 0x404ba0
    std::allocator<char> alloc;
    char* ptr;
    unsigned int size, capacity;
};
void __fastcall RecycleModelList(List<void*>* list);
#endif
