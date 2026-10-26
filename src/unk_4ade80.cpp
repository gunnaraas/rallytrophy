#include "unk_4ade80.h"

// FUNCTION: RALLYTROPHY 0x4ade80
ModelCacheState::~ModelCacheState()
{
    Clear();
}

// FUNCTION: RALLYTROPHY 0x4adf50
void ModelCacheState::Clear()
{
    unsigned i;
    for (i = 0; i < vertices.size(); ++i)
        ((CacheBufferDevice*)device)->ReleaseVertices(vertices[i]);
    for (i = 0; i < indices.size(); ++i)
        ((CacheBufferDevice*)device)->ReleaseIndices(indices[i]);
    vertices.resize(0);
    indices.resize(0);
    entries.RemoveAll();
}

// FUNCTION: RALLYTROPHY 0x4ae070
void ModelCacheState::Preload(std::vector<ModelObject*> objects)
{
    for (unsigned i = 0; i < objects.size(); ++i) {
        ModelObject* object = objects[i];
        if (*(int*)((char*)object + 4) == 1) Release(object);
    }
}

// FUNCTION: RALLYTROPHY 0x4ae0e0
void ModelCacheState::Release(ModelObject* object)
{
    CacheMeshList* meshes = (CacheMeshList*)((char*)object + 0x124);
    for (int i = 0; i < meshes->Count(); ++i) {
        MSSubList* mesh = meshes->At(i);
        for (unsigned j = 0; j < mesh->subs.size(); ++j) Add(mesh->subs[j]);
    }
}

// FUNCTION: RALLYTROPHY 0x4ae160
void ModelCacheState::Add(void* mesh)
{
    int* material = *(int**)mesh;
    unsigned flags = 0;
    if (material[0x9c/4] & 0x180) flags = 1;
    if (material[1]) flags |= 2;
    *(unsigned*)((char*)mesh + 0x1c) = flags;
    List<void*>::Node* node = entries.m_head;
    if (grouped) {
        for (; node; node = node->next)
            if (*(unsigned*)((char*)node->data + 0x1c) == flags) break;
    } else {
        while (node) {
            if (*(int**)node->data == material) break;
            node = node->next;
        }
    }
    if (node) ((CacheInsertList*)&entries)->InsertAfter(node, mesh);
    else entries.AddTail(mesh);
}

// NONMATCHING: RALLYTROPHY 0x4ae1f0
// Register allocation, list unlink scheduling and vector resize inlining differ.
void ModelCacheState::Reload(RenderDevice* renderer)
{
    device = renderer;
    unsigned count = 0;
    int flags = -1;
    CachePendingList pending;
    List<void*>::Node* node;
    for (node = entries.m_head; node; node = node->next) pending.AddTail(node->data);
    CacheBatch batch;
    while (pending.m_head) {
        void* mesh = pending.m_head->data;
        int nextFlags = *(int*)((char*)mesh + 0x1c);
        unsigned nextCount;
        if ((flags == nextFlags || flags == -1) &&
            (nextCount = count + *(unsigned short*)((char*)mesh + 0xc)) < 65000) {
            count = nextCount;
            flags = nextFlags;
            batch.Insert(batch.end(), mesh);
            pending.RemoveHead();
        } else {
            BuildVertices(count, (unsigned char)flags, &batch);
            batch.resize(0);
            count = 0;
            flags = -1;
        }
    }
    BuildVertices(count, (unsigned char)flags, &batch);
    for (node = entries.m_head; node; node = node->next) pending.AddTail(node->data);
    batch.resize(0);
    count = 0;
    while (pending.m_head) {
        void* mesh = pending.m_head->data;
        unsigned nextCount = count + *(unsigned short*)((char*)mesh + 0xe) * 3;
        if (nextCount < 65000) {
            count = nextCount;
            batch.Insert(batch.end(), mesh);
            pending.RemoveHead();
        } else {
            BuildIndices(count, &batch);
            batch.resize(0);
            count = 0;
        }
    }
    BuildIndices(count, &batch);
    ((CacheInsertList*)&entries)->Reset();
}
