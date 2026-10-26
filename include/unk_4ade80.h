#ifndef UNK_4ADE80_H
#define UNK_4ADE80_H
#include "unk_45e480.h"
#include "unk_401cf0.h"
// Typed view of CarModelCache's opaque storage; same 0x3c-byte layout.
class ModelCacheState {
public:
    int grouped;
    RenderDevice* device;
    List<void*> entries;
    std::vector<void*> vertices;
    std::vector<void*> indices;
    ~ModelCacheState();
    void Clear();
    void Preload(std::vector<ModelObject*> objects);
    void Release(ModelObject* object);
    void Add(void* mesh);
    void Reload(RenderDevice* renderer);
    void BuildVertices(unsigned count, unsigned char flags, std::vector<void*>* meshes);
    void BuildIndices(unsigned count, std::vector<void*>* meshes);
};
class CacheMeshList : public MSMeshes {
public:
    int Count(); // 0x4a6840
};
class CacheInsertList : public List<void*> {
public:
    void InsertAfter(Node* previous, void* mesh); // 0x4aed60
    void Reset(); // 0x4aecb0
};
class CacheBatch : public std::vector<void*> {
public:
    ~CacheBatch(); // 0x401bb0
    void Insert(iterator where, void* const& mesh); // 0x4293c0
};
class CachePendingList : public List<void*> {
public:
    ~CachePendingList(); // 0x49ad30: destroys active and recycled nodes
};
class CacheBufferDevice {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual void slot53();
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59();
    virtual void slot60();
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65();
    virtual void slot66();
    virtual void slot67();
    virtual void slot68();
    virtual void slot69();
    virtual void slot70();
    virtual void slot71();
    virtual void ReleaseVertices(void* buffer);
    virtual void slot73();
    virtual void ReleaseIndices(void* buffer);
};
#endif
