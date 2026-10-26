#ifndef UNK_4AF960_H
#define UNK_4AF960_H
#include <string.h>
#include "unk_4af950.h"
#include "unk_4b13c0.h"
#include "scene_object.h"
#include "unk_481390.h"

struct BmfMeshes {
    void Insert(SceneEntry*, float); // 0x4a66e0
    int a, b;
};
class BmfObject : public SceneNode {
public:
    BmfObject(); // 0x4a6890
    BmfMeshes meshes;
};
class BmfTextures {
public:
    Texture* Load(const char*, int); // 0x4aef30
    Texture* LoadSpecial(const char*, int); // 0x4af030
};
class BmfPath : public std::string {
public:
    void Prefix(unsigned int pos, const std::string&, unsigned int start, unsigned int n); // 0x4b26f0
};
struct BmfString : public ModelRecordString {
    BmfString() {}
    BmfString(const char* s) { AssignText(s, strlen(s)); }
    BmfString(const std::string& s) { Assign(s, 0, std::string::npos); }
    void AssignText(const char*, unsigned int); // 0x404ba0
    void Append(unsigned int, char); // 0x408d10
    std::string& Str() { return *(std::string*)this; }
};
// Member-call view of BmfReader::ReadMaterial using the existing ModelFile layout.
class BmfMaterialMethods : public ModelFile {
public:
    Sprite* ReadMaterial(File*); // 0x4b0180
};
class BmfPointerVector : public std::vector<void*> {
public:
    void Insert(iterator, unsigned int, void* const&); // 0x4bc680
};
struct BmfTag {
    BmfTag(const ModelNamedValue& s) : alloc(*(const std::allocator<char>*)&s) {
        ((ModelRecordString*)this)->Tidy(false);
        ((ModelRecordString*)this)->Assign(s.name, 0, std::string::npos);
        value = s.value;
    }
    ~BmfTag() { ((std::string*)this)->~basic_string(); }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int size, capacity;
    unsigned int value;
};
class BmfReader {
public:
    BmfReader(const char* path, const char* dir, BmfTextures* textures, List<Sprite*>* materials, int flags); // 0x4af950
    Sprite* ReadMaterial(File*); // 0x4b0180
    BmfTextures* textures;
    List<Sprite*>* materials;
    BmfString path;
    std::vector<void*> materialRefs, parts, blocks, objects;
    Vector3 a, b, c;
    bool enabled;
};
#endif
