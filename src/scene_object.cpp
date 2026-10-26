#include "rallytrophy.h"
#include "scene_object.h"

typedef char SceneNameSizeCheck[sizeof(SceneName) == sizeof(std::string) ? 1 : -1];
typedef char SceneNodeSizeCheck[sizeof(SceneNode) == 0x124 ? 1 : -1];

extern int DAT_00502470;
unsigned int __cdecl SaveSceneControlWord(); // original: FUN_004a7da0
void __cdecl RestoreSceneControlWord(unsigned int word); // original: FUN_004a7d90

// Original string::assign instance at 0x4024f0; keep this call out of line.
class SceneStringView {
public:
    SceneStringView& Assign(const SceneStringView&, unsigned int, unsigned int);
};

// FUNCTION: RALLYTROPHY 0x4b2830
// original: FUN_004b2830
// Evidence: ev_10517f3127cb2180a7660aaf933f4be1b46ec86260f54c5ed748a4e00f32acbc
// Member layout follows the five EH unwind states. Volatile stores preserve
// the observed initialization order; ordinary stores retain VC6 scheduling
// of the identity constant and bounds-flag mask. 0x3f800000 is float 1.0.
SceneNode::SceneNode()
{
    volatile int* fields = (volatile int*)this;
    volatile unsigned char* bytes = (volatile unsigned char*)this;
    ((int*)this)[0x14 / 4] = 0;
    fields[0x20 / 4] = 0x3f800000;
    fields[0x4 / 4] = 0;
    fields[0x8 / 4] = -1;
    fields[0x60 / 4] = 0x3f800000;
    fields[0x5c / 4] = 0x3f800000;
    fields[0x48 / 4] = 0x3f800000;
    fields[0x34 / 4] = 0x3f800000;
    fields[0x1c / 4] = 0;
    fields[0x18 / 4] = 0;
    fields[0x4c / 4] = 0;
    fields[0x9c / 4] = 0x3f800000;
    fields[0x88 / 4] = 0x3f800000;
    fields[0x74 / 4] = 0x3f800000;
    fields[0x28 / 4] = 0;
    fields[0x24 / 4] = 0;
    fields[0x8c / 4] = 0;
    unsigned char selected = bytes[0xcc];
    fields[0x68 / 4] = 0;
    fields[0x64 / 4] = 0;
    bytes[0xc] = 0;
    fields[0x10 / 4] = 0;
    fields[0xc0 / 4] = 0;
    fields[0xbc / 4] = 0;
    fields[0xb8 / 4] = 0;
    selected &= 0xfc;
    ((int*)this)[0x2c / 4] = 0;
    ((int*)this)[0x3c / 4] = 0;
    ((int*)this)[0x38 / 4] = 0;
    ((int*)this)[0x30 / 4] = 0;
    ((int*)this)[0x44 / 4] = 0;
    ((int*)this)[0x40 / 4] = 0;
    ((int*)this)[0x58 / 4] = 0;
    ((int*)this)[0x54 / 4] = 0;
    ((int*)this)[0x50 / 4] = 0;
    bytes[0xcc] = selected;
    fields[0xa8 / 4] = 0;
    fields[0xa4 / 4] = 0;
    fields[0xa0 / 4] = 0;
    fields[0x104 / 4] = 0;
    fields[0x100 / 4] = 0;
    fields[0x10c / 4] = 0;
    fields[0x108 / 4] = 0;
    fields[0x110 / 4] = 0;
    fields[0x6c / 4] = 0;
    fields[0x7c / 4] = 0;
    fields[0x78 / 4] = 0;
    fields[0x70 / 4] = 0;
    fields[0x84 / 4] = 0;
    fields[0x80 / 4] = 0;
    fields[0x98 / 4] = 0;
    fields[0x94 / 4] = 0;
    fields[0x90 / 4] = 0;
    fields[0xc4 / 4] = 0;
    fields[0xc8 / 4] = 0;
    fields[0xb4 / 4] = 0;
    fields[0xb0 / 4] = 0;
    fields[0xac / 4] = 0;
}

// FUNCTION: RALLYTROPHY 0x4b29c0
// original: FUN_004b29c0
SceneNode::~SceneNode()
{
}

// FUNCTION: RALLYTROPHY 0x4b29a0
// SYMBOL: ??_GSceneNode@@UAEPAXI@Z
typedef void Generated_4b29a0();

// FUNCTION: RALLYTROPHY 0x4b2b20
// original: FUN_004b2b20
void SceneNode::Assign(const SceneNode* source)
{
    CopyFrom(*source);
}

// FUNCTION: RALLYTROPHY 0x4b2a10
// original: FUN_004b2a10
SceneNode& SceneNode::CopyFrom(const SceneNode& source)
{
    ((SceneStringView*)&name)->Assign(
        *(const SceneStringView*)&source.name, 0, std::string::npos);
    type = source.type;
    flags = source.flags;
    stamp = source.stamp;
    position = source.position;
    id = source.id;
    local = source.local;
    world = source.world;
    boundsA.a = source.boundsA.a;
    boundsA.b = source.boundsA.b;
    boundsA.c = source.boundsA.c;
    boundsA.d = source.boundsA.d;
    boundsA.e = source.boundsA.e;
    boundsA.flags = source.boundsA.flags;
    return *this;
}

// FUNCTION: RALLYTROPHY 0x4b2b30
// original: FUN_004b2b30
void SceneNode::AttachTo(SceneNode* target)
{
    if (target) {
        Detach();
        SceneNode* tail = target->lastChild;
        parent = target;
        next = 0;
        previous = tail;
        if (tail) {
            target->lastChild = this;
            tail->next = this;
        } else {
            target->firstChild = this;
            target->lastChild = this;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4b2b90
// original: FUN_004b2b90
void SceneNode::Detach()
{
    if (parent) {
        if (this == parent->firstChild) parent->firstChild = next;
        if (this == parent->lastChild) parent->lastChild = previous;
        if (next) next->previous = previous;
        if (previous) previous->next = next;
        next = 0;
        previous = 0;
        parent = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x4b3010
// original: FUN_004b3010
void SceneNode::SetFlag04(int on)
{
    if (on) flags |= 4;
    else flags &= ~4;
}

// FUNCTION: RALLYTROPHY 0x4b3030
// original: FUN_004b3030
int SceneNode::IsCurrent()
{
    return stamp == DAT_00502470;
}

void __cdecl FUN_004b2c30(SceneNode* root, void* view);

// FUNCTION: RALLYTROPHY 0x4b2c10
// original: FUN_004b2c10
void SceneNode::Slot1(void* view)
{
    FUN_004b2c30(this, view);
}

// FUNCTION: RALLYTROPHY 0x4b2fc0
// original: FUN_004b2fc0
void SceneNode::Slot4(void* context)
{
    for (SceneNode* child = firstChild; child; child = child->next) {
        if (child->firstChild) child->Slot4(context);
    }
}

// FUNCTION: RALLYTROPHY 0x4b3000
// original: FUN_004b3000
void SceneNode::Slot5(void* renderer, void* view)
{
}

// FUNCTION: RALLYTROPHY 0x4b2d50
// original: FUN_004b2d50
void SceneNode::Slot3()
{
    boundsB = boundsA;
    if (firstChild) {
        unsigned int savedWord = SaveSceneControlWord();
        for (SceneNode* node = firstChild; node; node = node->next) {
            if (node->firstChild) node->Slot3();
        }
        SceneSphere sphere;
        sphere.ResetPoints();
        sphere.center.x = sphere.center.y = sphere.center.z = 0;
        sphere.radius = 0;
        for (SceneNode* child = firstChild; child; child = child->next) {
            if (child->firstChild) child->Slot4(&sphere);
            if (child->boundsB.flags & 2) {
                if (child->boundsB.a.x < boundsB.a.x) boundsB.a.x = child->boundsB.a.x;
                if (child->boundsB.a.y < boundsB.a.y) boundsB.a.y = child->boundsB.a.y;
                if (child->boundsB.a.z < boundsB.a.z) boundsB.a.z = child->boundsB.a.z;
                if (child->boundsB.b.x > boundsB.b.x) boundsB.b.x = child->boundsB.b.x;
                if (child->boundsB.b.y > boundsB.b.y) boundsB.b.y = child->boundsB.b.y;
                if (child->boundsB.b.z > boundsB.b.z) boundsB.b.z = child->boundsB.b.z;
            }
        }
        sphere.Build();
        boundsB.SetSphere(sphere.center, sphere.radius);
        RestoreSceneControlWord(savedWord);
    }
}

struct SceneViewBasis {
    unsigned char opaque[0x184];
    SceneTransform basis;
};

// FUNCTION: RALLYTROPHY 0x4b2c30
// original: FUN_004b2c30
void __cdecl FUN_004b2c30(SceneNode* root, void* view)
{
    SceneNode* ancestors[128];
    int depth = 0;
    SceneNode* current = root;
    if (current) {
        SceneNode** top = ancestors;
        do {
            if (!(current->flags & 4)) {
                if ((current->flags & 0x10) && view) {
                    current->local = ((SceneViewBasis*)view)->basis;
                    current->world = ((SceneViewBasis*)view)->basis;
                    *(ScenePoint*)&current->world.m[12] = current->position;
                } else {
                    current->world = current->local;
                    *(ScenePoint*)&current->world.m[12] = current->position;
                    if (current->parent) {
                        current->world.Multiply(&current->parent->world);
                    }
                }
            }
            SceneNode* child = current->firstChild;
            if (child) {
                ++depth;
                *top++ = current;
                current = child;
            } else {
                current = current->next;
                if (current) continue;
                if (depth <= 0) return;
                // Original only pops one ancestor here. Preserve that exit
                // behavior when the ancestor has no following sibling.
                --depth;
                --top;
                current = (*top)->next;
            }
        } while (current);
    }
}
