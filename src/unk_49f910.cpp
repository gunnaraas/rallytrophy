// AiPath (module 0x49f910-0x49fedf): the AI racing line of a stage, loaded from "<stage>.ai".
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"

// FUNCTION: RALLYTROPHY 0x49f910
AiPath::AiPath()
{
    m_nodes.resize(0);
    unk44 = -1;
}

// FUNCTION: RALLYTROPHY 0x49f9e0
AiPath::~AiPath()
{
    for (unsigned int i = 0; i < m_nodes.size(); i++) {
        delete m_nodes[i];
    }
    m_nodes.resize(0);
    ClearSegments();
}

// FUNCTION: RALLYTROPHY 0x49faf0
void AiPath::Load(const char* path)
{
    File* file = new File(path, 0x81);
    m_nodes.resize(0);
    int nodeCount;
    int segmentCount;
    int i;
    file->Read(&nodeCount, 4);
    for (i = 0; i < nodeCount; i++) {
        AiNode* node = new AiNode;
        file->Read(node, 12);
        node->index = i;
        file->Read(&node->unk24, 4);
        file->Read(&node->unk28, 4);
        file->Read(&node->unk2c, 4);
        m_nodes.push_back(node);
    }
    file->Read(&segmentCount, 4);
    for (i = 0; i < segmentCount; i++) {
        AiSegment* segment = new AiSegment(0);
        segment->index = i;
        segment->unkc = 2;
        file->Read(&segment->unk8, 4);
        file->Read(&segment->unk14, 4);
        file->Read(&segment->unk28[0], 12);
        file->Read(&segment->unk28[1], 12);
        file->Read(&segment->unk40, 4);
        file->Read(&segment->unk44, 4);
        m_segments.push_back(segment);
    }
    file->Close();
    delete file;
    LinkSegments();
}

// FUNCTION: RALLYTROPHY 0x49fd50
void AiPath::LinkSegments()
{
    for (unsigned int i = 0; i < m_segments.size() - 1; i++) {
        m_segments[i]->a = m_nodes[i];
        m_segments[i]->b = m_nodes[i + 1];
    }
    m_segments[m_segments.size() - 1]->a = m_nodes[m_segments.size() - 1];
    m_segments[m_segments.size() - 1]->b = m_nodes[0];
}

// FUNCTION: RALLYTROPHY 0x49fdf0
void AiPath::ClearSegments()
{
    for (unsigned int i = 0; i < m_segments.size(); i++) {
        delete m_segments[i];
    }
    m_segments.resize(0);
}

// FUNCTION: RALLYTROPHY 0x49fe70
AiSegment* AiPath::At(int i)
{
    int size = m_segments.size();
    if (i >= size) {
        return m_segments[i - size];
    }
    if (i < 0) {
        return m_segments[size + i];
    }
    return m_segments[i];
}

// FUNCTION: RALLYTROPHY 0x49feb0
// SYMBOL: _$E23
typedef void Generated_49feb0();

// FUNCTION: RALLYTROPHY 0x49fed0
// SYMBOL: _$E22
typedef void Generated_49fed0();
