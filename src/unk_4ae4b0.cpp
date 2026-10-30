#include "unk_4ae4b0.h"

// Mesh is already declared in the renderer headers; access its unnamed buffer
// fields at +0x28..+0x34 through the existing 0x38-byte storage.
#include "unk_481390.h"
#include "unk_47c460.h"
#include "unk_474c50.h"


// NONMATCHING: RALLYTROPHY 0x4ae4b0
// Exception string-copy inlining, local stack layout and vertex-loop register allocation differ.
// The direct first error string gives 824 differing bytes (1217-byte body) versus 852 (1169-byte body) with the custom wrapper.
void ModelCacheUpload::UploadVertices(int count, unsigned char flags, std::vector<Mesh*>* meshList)
{
    if (count > 0) {
        int format = 13;
        if (flags & 2) format = 45;
        FUN_00402c90("create vb: %i", count);
        ModelUploadBuffer* buffer = ((ModelUploadDevice*)device)->CreateVertices(format, count, 0, 2);
        if (!buffer) throw std::logic_error(std::string("failed to create vertexbuffer"));
        if (!buffer->Lock(0, 0, 0)) throw std::logic_error(std::string("failed to lock vertexbuffer"));
        int index = 0;
        for (unsigned int i = 0; i < meshList->size(); ++i) {
            Mesh* mesh = (*meshList)[i];
            *(int*)((char*)mesh + 0x28) = index;
            *(ModelUploadBuffer**)((char*)mesh + 0x30) = buffer;
            if (format & 0x20) {
                ShadowVertex* source = (ShadowVertex*)mesh->vertices;
                int n = mesh->vertexCount;
                ModelUploadVertexUV vertex;
                int* payload = (int*)source + 5;
                for (; n > 0; --n, ++source, payload += 11) {
                    vertex.words[0] = ((int*)source)[0];
                    vertex.words[1] = payload[-4];
                    vertex.words[2] = payload[-3];
                    vertex.words[3] = payload[-2];
                    vertex.words[4] = payload[-1];
                    vertex.words[5] = payload[0];
                    vertex.words[6] = payload[1];
                    vertex.words[7] = payload[2];
                    vertex.words[8] = payload[3];
                    vertex.words[9] = payload[4];
                    vertex.words[10] = payload[5];
                    buffer->SetVertices(index, &vertex, 1);
                    ++index;
                }
            } else {
                ShadowVertex* source = (ShadowVertex*)mesh->vertices;
                int n = mesh->vertexCount;
                ModelUploadVertex vertex;
                int* payload = (int*)source + 5;
                for (; n > 0; --n, ++source, payload += 11) {
                    vertex.words[0] = ((int*)source)[0];
                    vertex.words[1] = payload[-4];
                    vertex.words[2] = payload[-3];
                    vertex.words[3] = payload[-2];
                    vertex.words[4] = payload[-1];
                    vertex.words[5] = payload[0];
                    vertex.words[6] = payload[1];
                    vertex.words[7] = payload[2];
                    vertex.words[8] = payload[3];
                    buffer->SetVertices(index, &vertex, 1);
                    ++index;
                }
            }
        }
        if (!buffer->Unlock()) throw std::logic_error(std::string("failed to unlock vertexbuffer"));
        buffer->Finish();
        ModelBufferVector& buffers = *(ModelBufferVector*)((char*)this + 0x1c);
        buffers.Insert(buffers.end(), 1, buffer);
    }
}

// FUNCTION: RALLYTROPHY 0x4ae910
void ModelCacheUpload::UploadIndices(int count, std::vector<Mesh*>* meshList)
{
    if (count > 0) {
        FUN_00402c90("create ib: %i", count);
        ModelUploadBuffer* buffer = ((ModelUploadDevice*)device)->CreateIndices(count, 0, 2);
        if (!buffer) throw std::logic_error(std::string("failed to create indexbuffer"));
        unsigned char* destination = (unsigned char*)buffer->Lock(0, 0, 0);
        if (!destination) throw std::logic_error(std::string("failed to lock indexbuffer"));
        std::vector<Mesh*>& meshes = *meshList;
        int index = 0;
        for (unsigned int i = 0; i < meshes.size(); ++i) {
            Mesh* mesh = meshes[i];
            *(int*)((char*)mesh + 0x2c) = index;
            *(ModelUploadBuffer**)((char*)mesh + 0x34) = buffer;
            unsigned int n = mesh->primitiveCount * 3;
            unsigned int bytes = n * 2;
            memcpy(destination, mesh->indices, bytes);
            destination += bytes;
            index += n;
        }
        if (!buffer->Unlock()) throw std::logic_error(std::string("failed to unlock indexbuffer"));
        ModelBufferVector& buffers = *(ModelBufferVector*)((char*)this + 0x2c);
        buffers.Insert(buffers.end(), 1, buffer);
    }
}

// FUNCTION: RALLYTROPHY 0x4aecb0
void __fastcall RecycleModelList(List<void*>* list)
{
    List<void*>::Node* node = list->m_head;
    while (node) {
        list->m_head = node->next;
        if (node == list->m_tail) list->m_tail = node->prev;
        else list->m_head->prev = node->prev;
        int count = list->m_count;
        node->next = list->m_free;
        node->prev = 0;
        list->m_free = node;
        ++list->m_freeCount;
        node = list->m_head;
        list->m_count = count - 1;
    }
}
