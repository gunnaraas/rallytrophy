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
// Tried 2026-10-30 night: out-of-line Size()/Erase() resize twins (ResizeB for the two-call sites), PopHead with hoisted prev, per-site node/count vars, loop shapes, one permute run (no MATCH); still differs: phase-2 count/entries regs (esi/edi vs edi/ebp), prologue push order, lea vs mov+add in count sums, unlink cmp scheduling.
// Tried 2026-10-31 re-pass: 20 variants (local decl order, count/flags after first loop, operand order of count sums, short-width temp, volatile self, field-assign order); all stay at 40 insns differ or worse (count/flags moved after loop 1 gives 141); regalloc (count/flags in ebp/edi, phase-2 count in edi, entries ptr in ebp) unmoved.
// Tried 2026-11-16 round 22: count/flags after `pending`, no nextCount temp (`count += w`), `tri = w*3` temp and count2 declared before loop 2: 40 -> 12 insns differ; left: m_head store sched in PopHead (phase 1), edx/ecx/eax picks in phase 2 and the ResizeB/Build* arg setup; PopHead rewrites (hoisted next/prev, reordered stores) and PopHead-before-Insert all worse.
// Tried 2026-11-16 round 26: Insert/PopHead swap (130 insns), hoisted h node, count/flags store order, unsigned cast, hand-expanded PopHead in phase 1 (4 forms, +/-3 bytes, 32-120 insns), reorder.py exhausted: stays 12 insns/18 bytes.
// Tried 2026-11-16 round 25 (~100 compiles, stays 12 insns/18 bytes): PopHead bodies (hoisted prev/next, m_head->prev, *(Node**) reads, tail temp, per-site helpers), phase-2 count forms (int, short temp, `*=3`, named/unnamed nextCount, store order in else), reorder.py/permute.py: none moved the m_head-store/cmp order (phase 1) or the edx/ecx and eax-avoiding picks in phase 2 else/tail.
// 2026-11-16 round 27: phase-specific PopHead bodies (PopHeadB: per-arm block-scoped prev, used in phase 1) fixes the phase-1 m_head store order; 18 -> 12 differing bytes, phase 2 (hoisted PopHead) unchanged: edx/ecx picks, lea/push eax vs edx, ResizeB arg regs.
// 2026-11-16 round 28: phase-2 twin PopHeadC (next local, per-arm prev): 11 -> 9 insns differ, 12 -> 10 bytes; left: free reg (ecx vs edx) and the lea/ResizeB register rotation that follows.
// 2026-11-18: PopHeadC with prev hoisted before the tail test and the else arm reading next into a local (`Node* q = node->next; q->prev = p`): 10 -> 6 differing bytes (edx/ecx and lea/push regs fixed); left: m_head store vs `cmp node,m_tail` order in phase 2.
void ModelCacheState::Reload(RenderDevice* renderer)
{
    device = renderer;
    CachePendingList pending;
    unsigned count = 0;
    int flags = -1;
    List<void*>::Node* node;
    for (node = entries.m_head; node; node = node->next) { void* d = node->data; pending.AddTail(d); }
    CacheBatch batch;
    while (pending.m_head) {
        void* mesh = pending.m_head->data;
        int nextFlags = *(int*)((char*)mesh + 0x1c);
        if ((flags == nextFlags || flags == -1) &&
            count + *(unsigned short*)((char*)mesh + 0xc) < 65000) {
            count += *(unsigned short*)((char*)mesh + 0xc);
            flags = nextFlags;
            batch.Insert(batch.end(), mesh);
            pending.PopHeadB();
        } else {
            BuildVertices(count, (unsigned char)flags, &batch);
            batch.Resize(0);
            count = 0;
            flags = -1;
        }
    }
    BuildVertices(count, (unsigned char)flags, &batch);
    unsigned count2 = 0;
    for (node = entries.m_head; node; node = node->next) { void* d = node->data; pending.AddTail(d); }
    batch.ResizeB(0);
    while (pending.m_head) {
        void* mesh = pending.m_head->data;
        unsigned tri = *(unsigned short*)((char*)mesh + 0xe) * 3;
        unsigned nextCount = count2 + tri;
        if (nextCount < 65000) {
            count2 = nextCount;
            batch.Insert(batch.end(), mesh);
            pending.PopHeadC();
        } else {
            BuildIndices(count2, &batch);
            batch.ResizeB(0);
            count2 = 0;
        }
    }
    BuildIndices(count2, &batch);
    ((CacheInsertList*)&entries)->Reset();
}
