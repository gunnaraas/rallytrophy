#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

struct ScenePoint {
    ScenePoint() {}
    ScenePoint(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    ScenePoint(const ScenePoint& other) { x = other.x; y = other.y; z = other.z; }
    ~ScenePoint() {}
    ScenePoint operator+(const ScenePoint& other) const; // original: FUN_00419f00
    ScenePoint operator*(float scale) const; // original: FUN_0040b0c0
    ScenePoint& operator=(const ScenePoint& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
    ScenePoint& operator-=(const ScenePoint& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    float SquaredLength() const { return x * x + y * y + z * z; }
    float x, y, z;
};

struct SceneTransform {
    SceneTransform() {}
    ~SceneTransform() {}
    void Multiply(const SceneTransform* other); // original: FUN_004b55e0
    void Identity()
    {
        m[0] = m[5] = m[10] = m[15] = 1;
        m[1] = m[2] = m[3] = 0;
        m[4] = m[6] = m[7] = 0;
        m[8] = m[9] = m[11] = 0;
        m[12] = m[13] = m[14] = 0;
    }
    float m[16];
};

struct SceneBounds {
    SceneBounds() { flags = 0; }
    ~SceneBounds() {}
    SceneBounds& operator=(const SceneBounds& other)
    {
        a = other.a;
        b = other.b;
        c = other.c;
        d = other.d;
        e = other.e;
        flags = other.flags;
        return *this;
    }
    void SetSphere(const ScenePoint& point, float radius)
    {
        c = point;
        d = radius;
        flags |= 3;
    }
    ScenePoint a, b, c;
    float d, e;
    unsigned char flags;
};

struct SceneVertex {
    ScenePoint position;
    unsigned char payload[0x2c - sizeof(ScenePoint)];
};

struct SceneSphere {
    void AppendVertices(SceneVertex* vertices, int count, int build); // original: FUN_004b4530
    void ResetPoints();                // original: FUN_004b47e0 (shared scratch list)
    void Build();                      // original: FUN_004b4820
    void BuildFromPoints(int count);   // original: FUN_004b4870
    void SetPoint(const ScenePoint* point); // original: FUN_004b4c40
    void SetPair(const ScenePoint* a, const ScenePoint* b); // original: FUN_004b4c60
    void SetTriple(const ScenePoint* a, const ScenePoint* b, const ScenePoint* c); // original: FUN_004b4cd0
    void SetQuad(const ScenePoint* a, const ScenePoint* b, const ScenePoint* c, const ScenePoint* d); // original: FUN_004b4e80
    int Contains(const ScenePoint* point) const; // original: FUN_004b50c0
    ScenePoint center;
    float radius;
};

// VC6 string layout: keep construction cleanup out of line and destruction inline.
struct SceneName {
    SceneName(const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { Tidy(false); }
    SceneName(const SceneName& other) : alloc(other.alloc)
    {
        Tidy(false);
        *(std::string*)this = *(const std::string*)&other;
    }
    ~SceneName() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, capacity + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, length = 0, capacity = 0;
    }
    SceneName& operator=(const char* text) { *(std::string*)this = text; return *this; }
    SceneName& operator=(const SceneName& other)
    {
        *(std::string*)this = *(const std::string*)&other;
        return *this;
    }
    void Tidy(bool built); // original: FUN_00402700
    std::allocator<char> alloc;
    char* ptr;
    unsigned int length, capacity;
};

// Local layout of the common scene-object base. See docs/analysis/scene-object.md.
class SceneNode {
public:
    SceneNode();
    virtual ~SceneNode();
    virtual void Slot1(void* view);
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4(void* context);
    virtual void Slot5(void* renderer, void* view);
    virtual void Slot6();
    virtual void SetFlag04(int on);
    virtual int Slot8();

    SceneNode& CopyFrom(const SceneNode& source);
    void Assign(const SceneNode* source);
    void AttachTo(SceneNode* parent);
    void Detach();
    int IsCurrent();

    int type;                         // 0x04
    int id;                           // 0x08
    unsigned char flags;              // 0x0c
    unsigned char pad0d[3];
    int stamp;                        // 0x10
    ScenePoint position;              // 0x14
    SceneTransform local;             // 0x20
    SceneTransform world;             // 0x60
    SceneBounds boundsA;               // 0xa0 (roles of its scalar fields pending)
    SceneBounds boundsB;               // 0xd0
    SceneNode* next;                   // 0x100
    SceneNode* previous;               // 0x104
    SceneNode* firstChild;             // 0x108
    SceneNode* parent;                 // 0x10c
    SceneNode* lastChild;              // 0x110
    SceneName name;                    // 0x114
};

#endif
