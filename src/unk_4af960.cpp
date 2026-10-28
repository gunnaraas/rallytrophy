#include "unk_4af960.h"
#include "unk_4b8b10.h"
#include <math.h>
#include <string.h>

extern int DAT_0057b95c;
struct BmfFileName { void* stream; std::string name; };
static __forceinline const std::string& Name(File* f) { return ((BmfFileName*)f)->name; }
#define WORD(p,n) (((unsigned int*)(p))[n])
#define FLOAT(p,n) (((float*)(p))[n])

// std::string built from a C string in one class: _Tidy (0x402700) and assign(s, n) (0x404ba0) as calls, the
// object's cleanup state set after the assign returns.
struct BmfDirString {
    BmfDirString(const char* s, const std::allocator<char>& a = std::allocator<char>()) : alloc(a)
    {
        Tidy(false);
        AssignText(s, strlen(s));
    }
    ~BmfDirString() { ((std::string*)this)->~basic_string(); }
    void Tidy(bool built);                       // 0x402700
    void AssignText(const char*, unsigned int);  // 0x404ba0
    void Append(unsigned int, char);             // 0x408d10
    std::string& Str() { return *(std::string*)this; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int size, capacity;
};
// NONMATCHING: RALLYTROPHY 0x4af950
// 1227/1227 bytes, 29 differ: the original schedules `push edx; mov ecx,ebp` and the cleanup-state 7 store of the
// path assign between the b and c zero stores; every statement order and a call-after-stores form were tried.
BmfReader::BmfReader(const char* filename, const char* dir, BmfTextures* cache, List<Sprite*>* mats, int flags)
{
    DAT_0057b95c = flags;
    a.x = a.y = a.z = 0;
    b.x = b.y = b.z = 0;
    c.x = c.y = c.z = 0;
    enabled = true;
    path.AssignText(dir, strlen(dir));
    textures = cache;
    materials = mats;
    File file(filename, 0x81);
    BmfDirString directory(dir);
    if (!directory.Str().empty() && directory.Str()[directory.Str().size()-1] != '/')
        directory.Append(1, '/');
    ModelNamedValue* chunk = FUN_004af560(&file);
    BmfTag tag(*chunk);
    if ((*(std::string*)&tag) != "MAIN") {
        std::string message("BMF main chunk loading failed");
        throw file_error(Name(&file), message);
    }

    while ((unsigned int)FUN_004a5fc0(&file) < tag.value) {
        chunk = FUN_004af560(&file);
        if (chunk->name == "MATERIALLIST")
            ((ModelFile*)this)->ReadMaterialList(&file);
        else if (chunk->name == "HIERARCHY")
            ((ModelFile*)this)->ReadHierarchy(&file);
        else if (chunk->name == "TRACK")
            ((ModelFile*)this)->ReadTrack(&file);
        else
            FUN_004af710(&file, (ModelChunk*)chunk);
    }
}

// FUNCTION: RALLYTROPHY 0x4afec0
void ModelFile::ReadMaterialList(File* file)
{
    int count = FUN_004af230(file);
    BmfPointerVector& refs = *(BmfPointerVector*)&m_v18;
    refs.reserve(count);
    while (count-- > 0) {
        ModelNamedValue* chunk = FUN_004af560(file);
        if (chunk->name != "MATERIAL") {
            std::string message("BMF Material list chunk read failed");
            throw file_error(Name(file), message);
        }
        void* material = ((BmfMaterialMethods*)this)->ReadMaterial(file);
        refs.Insert(refs.end(), 1, material);
    }
}

// NONMATCHING: RALLYTROPHY 0x4b0180
// Static string setup, material color copies, and texture path insertion code generation differ.
Sprite* BmfReader::ReadMaterial(File* file)
{
    int a = FUN_004af230(file);
    int b = FUN_004af230(file);
    int c = FUN_004af230(file);
    int d = FUN_004af230(file);
    int e = FUN_004af230(file);
    static std::string name;
    static BmfPath texture;
    static BmfPath texture2;
    static BmfPath texture3;
    float colors[4][3];
    float power;
    int f, g, h, i, j;
    file->Read(colors[0], 12);
    file->Read(colors[1], 12);
    file->Read(colors[2], 12);
    file->Read(colors[3], 12);
    file->Read(&power, 4);
    file->Read(&f, 4);
    file->Read(&g, 4);
    file->Read(&h, 4);
    file->Read(&i, 4);
    file->Read(&j, 4);
    ((ModelRecordString*)&name)->Assign(*FUN_004af270(file), 0, std::string::npos);
    ((ModelRecordString*)&texture)->Assign(*FUN_004af270(file), 0, std::string::npos);
    ((ModelRecordString*)&texture2)->Assign(*FUN_004af270(file), 0, std::string::npos);
    ((ModelRecordString*)&texture3)->Assign(*FUN_004af270(file), 0, std::string::npos);
    for (List<Sprite*>::Node* scan = materials->m_head; scan; scan = scan->next)
        if (name == *(std::string*)((char*)scan->data + 0xa4))
            return scan->data;
    Sprite* result = new Sprite;
    WORD(result,4) = a;
    WORD(result,6) = c;
    WORD(result,3) = e;
    WORD(result,5) = b;
    WORD(result,7) = d;
    WORD(result,0x1f) = 0;
    
    FLOAT(result,0x1c) = colors[0][0];
    FLOAT(result,0x1c+1) = colors[0][1];
    FLOAT(result,0x1c+2) = colors[0][2];
    
    FLOAT(result,0x10) = colors[1][0];
    FLOAT(result,0x10+1) = colors[1][1];
    FLOAT(result,0x10+2) = colors[1][2];
    WORD(result,0x13) = h;
    WORD(result,0x1b) = 0;
    
    FLOAT(result,0x18) = colors[2][0];
    FLOAT(result,0x18+1) = colors[2][1];
    FLOAT(result,0x18+2) = colors[2][2];
    WORD(result,0x17) = 0;
    
    FLOAT(result,0x14) = colors[3][0];
    FLOAT(result,0x14+1) = colors[3][1];
    FLOAT(result,0x14+2) = colors[3][2];
    FLOAT(result,0x20) = power * 100.0f;
    if (d != 1) {
        WORD(result,9) = 2;
        WORD(result,12) = 3;
    }
    WORD(result,0x2e) = f;
    WORD(result,0x2f) = g;
    if (FLOAT(result,0x20) < 0.99f) WORD(result,0x20) = 0;
    WORD(result,0x27) = i;
    WORD(result,0x2d) = j;
    *(std::string*)((char*)result + 0xa4) = name.c_str();
    if (textures) {
        if (texture != "") {
            texture.Prefix(0, path.Str(), 0, std::string::npos);
            WORD(result,0) = (unsigned int)textures->Load(texture.c_str(), 0x40);
        }
        if (texture2 != "") {
            texture2.Prefix(0, path.Str(), 0, std::string::npos);
            WORD(result,1) = (unsigned int)textures->Load(texture2.c_str(), 0x40);
        }
        if ((DAT_0057b95c & 1) && texture3 != "") {
            texture3.insert(0, path.Str(), 0, std::string::npos);
            WORD(result,2) = (unsigned int)textures->LoadSpecial(texture3.c_str(), 0x40);
        }
    }
    List<Sprite*>::Node* node = materials->m_free;
    if (!node) {
        node = new List<Sprite*>::Node;
        if (node) { node->prev = 0; node->next = 0; node->data = 0; }
    } else {
        materials->m_free = node->next;
        --materials->m_freeCount;
    }
    List<Sprite*>::Node* tail = materials->m_tail;
    node->data = result;
    node->prev = tail;
    node->next = 0;
    if (!tail) materials->m_head = node;
    else tail->next = node;
    materials->m_tail = node;
    ++materials->m_count;
    return result;
}

// NONMATCHING: RALLYTROPHY 0x4b0a30
// Allocation register selection, bounds copy scheduling, and inline vector insertion differ.
void ModelFile::ReadBlock(File* file, unsigned int* size)
{
    SceneEntry* entry = ((ModelLoader*)this)->ReadMesh(file, (unsigned int)size);
    size = (unsigned int*)new BmfObject;
    ((BmfObject*)size)->name = "block";
    ((BmfObject*)size)->meshes.Insert(entry, 0.0f);
    unsigned int* dest = (unsigned int*)&((BmfObject*)size)->boundsA;
    unsigned int* src = (unsigned int*)entry;
    unsigned int u = src[1];
    dest[0] = src[0];
    unsigned int v = src[2]; dest[1] = u;
    u = src[3]; dest[2] = v;
    v = src[4]; dest[3] = u;
    u = src[5]; dest[4] = v;
    v = src[6]; dest[5] = u;
    u = src[7]; dest[6] = v;
    v = src[8]; dest[7] = u;
    u = src[9]; dest[8] = v;
    v = src[10]; dest[9] = u;
    unsigned char flags = *(unsigned char*)&src[11];
    dest[10] = v;
    *(unsigned char*)&dest[11] = flags;
    delete entry;
    m_v38.push_back(size);
}

// NONMATCHING: RALLYTROPHY 0x4b0e90
// Record/matrix copy scheduling, x87 temporaries, and stack layout differ.
void ModelFile::ReadMesh(File* file)
{
    ModelPartRecord* record = FUN_004af730(file);
    FUN_004af230(file);
    BmfObject* object = new BmfObject;
    ((ModelRecordString*)&object->name)->Assign(*(std::string*)record, 0, std::string::npos);
    object->flags |= (unsigned char)FUN_004af890(*(std::string*)record);
    unsigned int* w = (unsigned int*)object;
    object->position.x = FLOAT(record,0x15); object->position.z = FLOAT(record,0x17); object->position.y = FLOAT(record,0x16);
    *(Matrix*)&object->local = record->transform;
    w[0x16] = w[0x15] = w[0x14] = 0;
    object->position.x = FLOAT(record,0x15); object->position.z = FLOAT(record,0x17); object->position.y = FLOAT(record,0x16);
    w[0x29] = WORD(record,0x21); w[0x28] = WORD(record,0x20);
    w[0x2b] = WORD(record,0x23); w[0x2a] = WORD(record,0x22);
    w[0x2d] = WORD(record,0x25); w[0x2c] = WORD(record,0x24);
    w[0x2f] = WORD(record,0x1d); w[0x2e] = WORD(record,0x1c);
    w[0x31] = WORD(record,0x1f); w[0x30] = WORD(record,0x1e);
    if (record->flags & 1) object->boundsA.flags |= 1;
    if (record->flags & 2) object->boundsA.flags |= 2;
    EntryVec3 scale;
    float* m = object->local.m;
    scale.x = (float)sqrt(m[2]*m[2] + m[1]*m[1] + m[0]*m[0]);
    scale.y = (float)sqrt(m[6]*m[6] + m[5]*m[5] + m[4]*m[4]);
    scale.z = (float)sqrt(m[10]*m[10] + m[9]*m[9] + m[8]*m[8]);
    float k = 1.0f / (float)sqrt(m[2]*m[2] + m[1]*m[1] + m[0]*m[0]);
    m[0] = k*m[0]; m[1] = k*m[1]; m[2] = k*m[2];
    int parent = record->value;
    k = 1.0f / (float)sqrt(m[6]*m[6] + m[5]*m[5] + m[4]*m[4]);
    m[4] = k*m[4]; m[5] = k*m[5]; m[6] = k*m[6];
    k = 1.0f / (float)sqrt(m[10]*m[10] + m[9]*m[9] + m[8]*m[8]);
    m[8] = k*m[8]; m[9] = k*m[9]; m[10] = k*m[10];
    ((ModelLoader*)this)->Attach(object, parent);
    int start = FUN_004a5fc0(file);
    ModelNamedValue* chunk = FUN_004af560(file);
    BmfTag tag(*chunk);
    if ((*(std::string*)&tag) == "MODEL") {
        tag.value += start - FUN_004a5fc0(file);
        SceneEntry* entry = ((ModelLoader*)this)->ReadMesh(file, tag.value);
        ((MeshModel*)entry)->Translate((float*)((char*)record + 0x60));
        ((MeshModel*)entry)->Scale(&scale.x);
        if (!(object->boundsA.flags & 3)) ((MeshModel*)entry)->ComputeBoundsAndSphere();
        object->meshes.Insert(entry, 0.0f);
        delete entry;
    } else {
        std::string message("BMF mesh chunk read failed");
        throw file_error(Name(file), message);
    }
}
