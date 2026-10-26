/* CarObject (0x45e480-0x464aaf): the complete layout of the Car object (0x1f30c bytes) as the real
 * Car constructor 0x45e480 / destructor 0x461a40 see it. Map: analysis/range-45e480.md.
 *
 * Why a second class: `Car` in unk_481390.h only names the fields the earlier modules read and is
 * viewed through file-local structs everywhere else. CarObject declares EVERY member that the
 * constructor's EH unwind table (handler 0x4db2a0, 78 member states 0..0x4d) lists, as its real
 * type, in construction order at its exact offset, so that
 *   - the member construction sequence and the unwind state numbers of the constructor, and
 *   - the member destruction sequence and the unwind table of the destructor (handler 0x4db7c1)
 * come out of the compiler as in the original. CarObject::CarObject / CarObject::~CarObject ARE the
 * original Car::Car / Car::~Car; the class name is free (match.py compares by address).
 *
 * Rules for writers of other functions of this TU (the constructor, the three loaders):
 *   - Include order: "unk_481390.h" "unk_4843e0.h" "rallytrophy.h" "unk_45e480.h" (this header includes
 *     them itself, and the headers of every member type, so it is self-contained).
 *   - Do not add data members with destructors: every embedded object below is one EH state, and
 *     the state numbers are checked against the original table (see the list in the .cpp).
 *   - Plain data between the EH members is `unkXXXX` BYTE padding or a named scalar; rename/split
 *     padding freely (keep the offsets: the typedef checks at the end of this file enforce them).
 *   - Types whose constructor/destructor is "empty" in the original (all of them fold to the shared
 *     0x490ad0 / 0x402c90 in the exe) are declared with empty user ctor/dtor (EmptyBlock<N>,
 *     RawMatrix, Vec3): this produces the EH vector constructor iterator calls the original has.
 *   - Classes whose ctor/dtor are out of line in the original (cache objects, recorder, mesh list,
 *     the emitters, Wheel, SkidMarks, Drivetrain...) are only declared here, never defined inline.
 * Verified against the original: construction order = member order, 78 states (0x4e members
 * 0..0x4d), see the table above CarObject::~CarObject in src/unk_45e480.cpp. */
#ifndef UNK_45E480_H
#define UNK_45E480_H

#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_46a8a0.h"
#include "unk_46d720.h"
#include "unk_477370.h"
#include "unk_478020.h"
#include "unk_479170.h"
#include "unk_47b4c0.h"
#include "unk_4741a0.h"
#include "unk_46e880.h"
#include "unk_46bb90.h"
// unk_46c2c0.h (Wheel) and unk_4666a0.h (SoundLayer) both define a NameString helper; rename the
// one of the Wheel header so that the two can be included into one TU.
#define NameString NameString_46c2c0
#define TyreSurface TyreSurface_46c2c0
#include "unk_46c2c0.h"
#undef NameString
#undef TyreSurface
#include "unk_4666a0.h"
#include <stddef.h>

// ---------------------------------------------------------------------------------------------
// Helper types (everything here is only a layout/ctor/dtor shape)
// ---------------------------------------------------------------------------------------------

// A block with an empty constructor and destructor (the original's ctor folds to 0x490ad0, the
// dtor to 0x402c90). Used for the members of unknown content.
template <int N>
struct EmptyBlock {
    EmptyBlock() {}
    ~EmptyBlock() {}
    BYTE data[N];
};

// Anything the car owns through a pointer and deletes virtually (`push 1; call [vtable+0]`): the
// car body (0x24, 0x284 bytes, ctor 0x41bab0), the shadow caster (0x1f18, 0x178 bytes, ctor
// 0x4ba5a0), the shadow renderer (0x20ec, ParallelShadow, 0x330 bytes, ctor 0x4703c0) and 0x1bce4.
class PolyObject {
public:
    virtual ~PolyObject();
};

// A model-part object (0x12c bytes, ctor 0x4a6910(partName), vtable 0x4e2930): wipers, interior,
// steering wheel, indicators, tyre shadows, crash parts, window/glow sprites. Virtual destructor.
class ModelObject : public EffectBase {
public:
    // Builds from a model part; helper 0x4a7560 copies base state and mesh refs,
    // then 0x4a6d30 refreshes the derived triangle cache when the source has one.
    ModelObject(class ModelFilePart* part); // 0x4a6910
    void SetParent(void* parent);      // 0x4b2b30
    // Rebuilds the derived triangle cache at +0x128 from mesh zero at +0x124
    // (observation: REA evidence ev_d69f45e2e135b4a78e1d9995998d6f288e76143b87ad21962eb819f860567ec3).
    void FUN_004a6d30();               // 0x4a6d30
    BYTE unk4[8];
    BYTE flags;                        // 0x0c
    BYTE unkd[0x12c - 0xd];
};

// Intrusive doubly linked list with a free list (the same layout and out-of-line AddTail 0x4aecf0 /
// RemoveNode 0x414490 as List<T> in rallytrophy.h; dtor shape differs slightly: see ~CarObject).
template <class T>
class CarList {
public:
    struct Node {
        ~Node() {}
        Node* Next();  // 0x464990 (out of line: the constructor calls it)
        T Data();      // 0x4649a0
        Node* next; // 0x0
        Node* prev; // 0x4
        T data;     // 0x8
    };
    CarList()
    {
        m_head = m_tail = m_free = 0;
        m_count = 0;
        m_freeCount = 0;
    }
    ~CarList()
    {
        Node* head = m_head;
        while (head) {
            Node* node = head;
            Node* next = node->next;
            m_head = next;
            if (node == m_tail) {
                m_tail = node->prev;
            } else {
                next->prev = node->prev;
            }
            node->next = m_free;
            node->prev = 0;
            m_free = node;
            m_freeCount++;
            m_count--;
            head = next;
        }
        Node* node = m_free;
        while (node) {
            Node* next = node->next;
            delete node;
            node = next;
        }
        m_free = 0;
    }
    void AddTail(T item);              // 0x4aecf0 (out of line)
    void RemoveNode(Node* node);       // 0x414490 (out of line)
    Node* m_head;    // 0x0
    Node* m_tail;    // 0x4
    Node* m_free;    // 0x8
    int m_count;     // 0xc
    int m_freeCount; // 0x10
};

// A 0xd4-byte sprite/material held by the three model lists. It is destroyed through the
// compiler-style scalar deleting destructor 0x462330 (a direct call, `push 1`), see the .cpp.
class MaterialSprite : public Sprite {
public:
    MaterialSprite() {}
    void* ScalarDelete(unsigned int flags); // 0x462330
    void Assign(const MaterialSprite* other); // 0x4a87f0
};

// A model-file part (the objects of the .bmf, 0x12c+ bytes): the mesh container lives at +0x124.
class ModelFilePart {
public:
    SnapMesh* FirstMesh();             // 0x4649b0: meshes.At(0), out of line
    BYTE unk0[8];
    int bits;                          // 0x08: 1 << car counter (which cars use the part)
    BYTE unkc[0x14 - 0xc];
    float pivot[3];                    // 0x14: part origin
    RawMatrix matrix;                  // 0x20 (the row at 0x40 is the part's z axis)
    BYTE unk60[0x118 - 0x60];
    char* name;                        // 0x118: std::string pointer of the part name at 0x114
    BYTE unk11c[0x124 - 0x11c];
    SnapMeshList meshes;               // 0x124
};

// The loaded .bmf model of a car part (stack object of the constructor, 0x80 bytes, ctor 0x4af950, dtor
// 0x4afe20): the parts list at +0x28 and the owned objects at +0x48.
class ModelFile {
public:
    ModelFile(const char* path, const char* dir, class CarTextureCache* cache, void* materials, int flag); // 0x4af950
    ~ModelFile();                      // 0x4afe20
    BYTE unk0[0x28];
    std::vector<ModelFilePart*> parts; // 0x28
    BYTE unk38[0x48 - 0x38];
    std::vector<PolyObject*> objects;  // 0x48
    BYTE unk58[0x80 - 0x58];
};

// One row of the car class table at 0x4fb0f8 (12 bytes).
struct CarClassRow {
    int kind;                          // 0x00
    BYTE noLod;                        // 0x04
    BYTE tyreLod;                      // 0x05
    BYTE hasInterior;                  // 0x06
    BYTE unk7;                         // 0x07
    int shadowSize;                    // 0x08
};

// The object behind Car+0xfa0 (0x3c bytes) and Car+0xfa4 (0x20 bytes): shared model/texture
// caches, either borrowed from the game object (arg a) or owned by the car (Car+0xfa8 != 0).
class CarModelCache {
public:
    CarModelCache();                   // 0x4ade40
    ~CarModelCache();                  // 0x4ade80
    void Release(void* object);        // 0x4ae0e0: give a model object back to the cache
    int flag0;                         // 0x00: set to 1 by the car constructor that creates the cache
    BYTE data[0x38];
};
class CarTextureCache {
public:
    CarTextureCache();                 // 0x4aee00
    ~CarTextureCache();                // 0x4aee20
    void SetDetail(int level);         // 0x4aeec0
    Texture* Load(const char* name, int flags); // 0x4aef30: loads a file, or finds it again
    BYTE data[0x20];
};

// Owned helper objects with a non-virtual out-of-line destructor 0x401c30 (deleted as `~T(); delete`).
class OwnedSet {
public:
    ~OwnedSet();                       // 0x401c30
    BYTE data[0x14];
};

// The list of mesh handles at Car+0x20f0: ctor 0x405580 (shared tiny ctor), dtor 0x4b9b20.
class MeshHandleList {
public:
    MeshHandleList();                  // 0x405580 (folded with Config::Config)
    ~MeshHandleList();                 // 0x4b9b20
    void Add(SnapMesh* mesh);          // 0x4b9b50
    BYTE data[0x10];
};

// The replay/ghost recorder at Car+0x1bed0 (0x2c bytes): ctor 0x401270, dtor 0x4012d0.
class CarRecorder {
public:
    CarRecorder();                     // 0x401270
    ~CarRecorder();                    // 0x4012d0
    BYTE data[0x2c];
};

// One of the four lamp records at Car+0x1bcf0 (0x10 bytes, inline zeroing ctor, no dtor).
struct CarLamp {
    CarLamp()
    {
        target = 0;
        off = 0;
        on = 0;
        state = 0;
    }
    int* target;                       // 0x00
    int off;                           // 0x04
    int on;                            // 0x08
    BYTE state;                        // 0x0c
    BYTE unkd[3];
};

// One light record (0x1c bytes; 12 per car in five arrays at Car+0x1bd40...): ctor 0x464a70 (the
// vector constructor iterator calls it), empty destructor (0x402c90).
struct LightRec {
    LightRec();                        // 0x464a70
    ~LightRec() {}
    CarLight* light;                   // 0x00: the glow sprite (deleted by ~CarObject)
    BYTE flag;                         // 0x04
    int mesh;                          // 0x08
    int unkc;                          // 0x0c
    int unk10;                         // 0x10
    int unk14;                         // 0x14
    int unk18;                         // 0x18
};

// The queue at Car+0x1f160 (0xa8 bytes; ctor 0x40b4e0, push 0x40b580, flush 0x40b600): two arrays
// with empty element ctor/dtor; the destructor is compiler-generated (0x4649f0).
struct QueueId {
    QueueId() {}
    ~QueueId() {}
    int v;
};
struct QueueEntry {
    QueueEntry() {}
    ~QueueEntry() {}
    int id;
    int time;
};
class CarEventQueue {
public:
    CarEventQueue();                   // 0x40b4e0
    void Push(int id, int time);       // 0x40b580
    void Flush(int time);              // 0x40b600
    QueueId ids[4];                    // 0x00
    QueueEntry entries[16];            // 0x10
    BYTE unk90[0xa0 - 0x90];
    int unka0;                         // 0xa0
    int unka4;                         // 0xa4
};

// The glow parameters of one kind of light (car.ini: {Front,Rear,Brake,Reverse}Flare{Radius,Sharpness,AlphaMin}).
struct FlareParams {
    float radius;                      // 0x00
    float exponent;                    // 0x04
    float bias;                        // 0x08
};

// A 0x34-byte recording buffer (Car+0x1430 and Car+0x1464): a vector with the capacity limit 0x11 stored at +0x30.
struct CarTrace {
    CarTrace() { limit = 0x11; }
    void Reserve(unsigned int n);      // 0x4bcf30 (vector::reserve, out of line)
    std::vector<int> v;                // 0x00 (reserve(0x400) by the car constructor)
    BYTE unk10[0x30 - 0x10];
    int limit;                         // 0x30
};

// ---------------------------------------------------------------------------------------------
// CarObject
// ---------------------------------------------------------------------------------------------
class CarObject {
public:
    // Arguments (all but a are stored): a = shared cache block (Game+0x139404) or 0 = own caches,
    // b = car model number (data/cars/car_<b>/), c = car class row, d = kind (0 local, 1 remote,
    // 2 replay; guess), e, f = player slot / detail (+0x1ec0), g = flags (bit0 spectator view,
    // bit1 no reflection object, bit2 no sounds).
    CarObject(CarTextureCache* a, int b, int c, int d, int e, int f, unsigned int g); // 0x45e480 (original Car::Car)
    ~CarObject();                      // 0x461a40 (original Car::~Car)
    void ResetMessage();               // 0x464a50 (inline setter used by the constructor)

    // Methods that live in this module (implemented by other work groups).
    void LoadCarIni(const char* path, unsigned int checksum);    // 0x462350
    void LoadTiresIni(const char* path, unsigned int checksum);  // 0x4634e0
    void LoadTextures(const std::string& dir);                   // 0x463940
    void SetLightIntensity(float level);                         // 0x4644b0
    void AttachStage(Stage* stage, int b);                       // 0x464530
    void ResetControls();                                        // 0x464680
    void AttachLights();                                         // 0x4646c0
    void Reset();                                                // 0x464800
    void SetVectorValue(int v);                                  // 0x464940
    // Methods implemented elsewhere that the destructor and the constructor call.
    void ReleaseSounds();                                        // 0x466fe0
    void InitSounds(const std::string& dir);                     // 0x4666a0
    void FUN_00455070(const char* driverDir);                    // 0x455070
    void FUN_00455ec0(const std::string& dir);                   // 0x455ec0
    void FUN_004560a0(const std::string& dir);                   // 0x4560a0
    void SetCockpit(int cockpit);                                // 0x4691e0
    void FUN_00465e90(int value);                                // 0x465e90
    void FUN_00466500(int value);                                // 0x466500
    void FUN_00466520(int value);                                // 0x466520
    void FUN_00466590(int value);                                // 0x466590
    void FUN_00466610(int value);                                // 0x466610
    void FUN_0048e660();                                         // 0x48e660 (empty, folded)
    void FUN_0045a850();                                         // 0x45a850

    int setup[5];                      // 0x00: CarSetup values (default 2; +0x8 = 2 set by the ctor)
    BYTE unk14[0x24 - 0x14];
    PolyObject* body;                  // 0x24: CarBody (new 0x284, ctor 0x41bab0)
    Wheel wheel[4];                    // 0x28: state 0x00 (vector ctor 0x4c11f4, dtor callback 0x46c420), stride 0x330
    EmptyBlock<0x34> spring[4];        // 0xce8: state 0x01, per-wheel geometry (Car+0xd00 + n*0x34)
    Drivetrain drivetrain;             // 0xdb8: state 0x02 (0x1e8 bytes)
    CarModelCache* modelCache;         // 0xfa0: borrowed (Game+0x139424) or new 0x3c
    CarTextureCache* textureCache;     // 0xfa4: borrowed (arg a) or new 0x20
    int ownsCaches;                    // 0xfa8
    CarList<MaterialSprite*> materials;   // 0xfac: state 0x03 (dtor 0x49ad30)
    CarList<MaterialSprite*> materials2;  // 0xfc0: state 0x04
    CarList<MaterialSprite*> materials3;  // 0xfd4: state 0x05 (copies made by the constructor's last loop)
    Vec3 unkfe8;                       // 0xfe8: state 0x06 (first car.ini field, LoadCarIni writes 0xfe8..0x1430)
    BYTE unkff4[0x1030 - 0xff4];
    BYTE driverType;                   // 0x1030: digit of the driver model data/drivers/driver<N>/
    BYTE unk1031[3];
    float bodyParam;                   // 0x1034: second argument of the CarBody constructor
    RawVec3 bodyVecA;                  // 0x1038
    RawVec3 bodyVecB;                  // 0x1044
    BYTE unk1050[0x1088 - 0x1050];
    float alphaA;                      // 0x1088
    float alphaB;                      // 0x108c
    RawVec3 shadowPos;                 // 0x1090
    RawVec3 shadowTint;                // 0x109c
    float shadowAngle;                 // 0x10a8
    FlareParams flare[4];              // 0x10ac: front, rear, brake, reverse
    BYTE unk10dc[0x1384 - 0x10dc];
    Vec3 ctrl0;                        // 0x1384: states 0x07..0x12 (twelve separate Vec3 members)
    Vec3 ctrl1;                        // 0x1390
    Vec3 ctrl2;                        // 0x139c
    Vec3 ctrl3;                        // 0x13a8
    Vec3 ctrl4;                        // 0x13b4
    Vec3 ctrl5;                        // 0x13c0
    Vec3 ctrl6;                        // 0x13cc
    Vec3 ctrl7;                        // 0x13d8
    Vec3 ctrl8;                        // 0x13e4
    Vec3 ctrl9;                        // 0x13f0
    Vec3 ctrl10;                       // 0x13fc
    Vec3 ctrl11;                       // 0x1408
    BYTE unk1414[0x1430 - 0x1414];
    CarTrace vec1430;                  // 0x1430: state 0x13 (reserve(0x400) by the ctor); +0x1460 = 0x11
    CarTrace vec1464;                  // 0x1464: state 0x14 (reserve(0x400)); +0x1494 = 0x11
    EmptyBlock<0x10> blk1498;          // 0x1498: state 0x15
    RawMatrix mat14a8;                 // 0x14a8: state 0x16
    EmptyBlock<0x18> blk14e8;          // 0x14e8: state 0x17 (Car+0x14fc, the stage, lies in 0x14f0..0x1500)
    RawMatrix mat1500[6];              // 0x1500: state 0x18
    RawMatrix tyreMat[30];             // 0x1680: state 0x19 (30 surfaces)
    BYTE unk1e00[4];
    Vec3 hitNormal[4];                 // 0x1e04: state 0x1a
    BYTE unk1e34[0x1e44 - 0x1e34];
    int wheelSurface[4];               // 0x1e44
    Vec3 hitPoint[4];                  // 0x1e54: state 0x1b
    Vec3 hitDepth[4];                  // 0x1e84: state 0x1c
    BYTE unk1eb4[4];
    int damaged;                       // 0x1eb8
    int model;                         // 0x1ebc: arg c
    int slot;                          // 0x1ec0: arg f
    BYTE unk1ec4[0x1ed0 - 0x1ec4];
    EmptyBlock<8> dashPos;             // 0x1ed0: state 0x1d (dashboard position x, y)
    BYTE unk1ed8[0x1f18 - 0x1ed8];
    PolyObject* shadow;                // 0x1f18: ShadowCaster (new 0x178, ctor 0x4ba5a0)
    ModelObject* shadowTyre[4];        // 0x1f1c
    ModelObject* obj1f2c;              // 0x1f2c
    SpringFollower follower0;          // 0x1f30: state 0x1e (250.0, 3.5, 0.1 set by the ctor)
    SpringFollower follower1;          // 0x1f4c: state 0x1f
    ModelObject* hoodCrash;            // 0x1f68
    ModelObject* trunkCrash;           // 0x1f6c
    RawMatrix hoodMatrix;              // 0x1f70: state 0x20
    RawMatrix trunkMatrix;             // 0x1fb0: state 0x21
    ModelObject* obj1ff0;              // 0x1ff0
    ModelObject* wipers;               // 0x1ff4
    OwnedSet* set1ff8;                 // 0x1ff8
    BYTE unk1ffc[0x2004 - 0x1ffc];
    ModelObject* obj2004;              // 0x2004
    ModelObject* obj2008;              // 0x2008
    BYTE unk200c[4];
    ModelObject* interiorLo;           // 0x2010
    ModelObject* interiorHi;           // 0x2014
    ModelObject* obj2018;              // 0x2018
    ModelObject* obj201c;              // 0x201c
    BYTE unk2020[0x2028 - 0x2020];
    ModelObject* obj2028;              // 0x2028
    ModelObject* obj202c;              // 0x202c
    Vec3 vec2030;                      // 0x2030: state 0x22
    Vec3 vec203c;                      // 0x203c: state 0x23
    BYTE unk2048[0x204c - 0x2048];
    ModelObject* steeringLo;           // 0x204c
    ModelObject* steeringHi;           // 0x2050
    ModelObject* indicatorSpeed;       // 0x2054
    ModelObject* indicatorRpm;         // 0x2058
    Vec3 vec205c;                      // 0x205c: state 0x24
    Vec3 vec2068;                      // 0x2068: state 0x25
    BYTE unk2074[0x2080 - 0x2074];
    OwnedSet* set2080;                 // 0x2080
    OwnedSet* set2084;                 // 0x2084
    OwnedSet* set2088;                 // 0x2088
    OwnedSet* set208c;                 // 0x208c
    OwnedSet* set2090;                 // 0x2090
    OwnedSet* set2094;                 // 0x2094
    BYTE unk2098[0x20ac - 0x2098];
    Vec3 vec20ac;                      // 0x20ac: states 0x26..0x2a
    Vec3 vec20b8;                      // 0x20b8
    Vec3 vec20c4;                      // 0x20c4
    Vec3 vec20d0;                      // 0x20d0
    Vec3 exhaustPos;                   // 0x20dc: ext_pipe01 location
    int exhaustFlag;                   // 0x20e8: set when the model has ext_pipe01
    PolyObject* reflection;            // 0x20ec: 0x330-byte ParallelShadow (ctor 0x4703c0)
    MeshHandleList meshes;             // 0x20f0: state 0x2b
    WheelSpray spray;                  // 0x2100: state 0x2c (size 0x12c)
    CarEmitterB smokeB;                // 0x222c: state 0x2d (size 0x12c)
    CarParticles dust;                 // 0x2358: state 0x2e (size 0x130)
    CarEmitterC debris;                // 0x2488: state 0x2f (size 0x160)
    ExhaustSmoke exhaust;              // 0x25e8: state 0x30 (the class in unk_478020.h is 0x12c, the object 0x160)
    BYTE exhaustTail[0x160 - 0x12c];
    SkidMarks skid[4];                 // 0x2748: state 0x31, stride 0x6558
    BYTE unk1bca8[0x1bcbc - 0x1bca8];
    EmptyBlock<8> unk1bcbc;            // 0x1bcbc: state 0x32 (overlaps the mix floats written by Reset)
    std::vector<MaterialSprite*> vec1bcc4; // 0x1bcc4: state 0x33 (vector of part pointers)
    std::vector<MaterialSprite*> vec1bcd4; // 0x1bcd4: state 0x34 (vector of objects, SetVectorValue)
    PolyObject* obj1bce4;              // 0x1bce4
    int kind;                          // 0x1bce8: arg d
    int arg1bcec;                      // 0x1bcec: arg e
    CarLamp lamp0;                     // 0x1bcf0
    CarLamp lamp1;                     // 0x1bd00
    CarLamp lamp2;                     // 0x1bd10
    CarLamp lamp3;                     // 0x1bd20
    void* marker;                      // 0x1bd30
    int window[3];                     // 0x1bd34
    LightRec lightFrontLeft[3];        // 0x1bd40: state 0x35
    LightRec lightFrontRight[3];       // 0x1bd94: state 0x36
    LightRec lightRear[2];             // 0x1bde8: state 0x37
    LightRec lightBrake[2];            // 0x1be20: state 0x38
    LightRec lightReverse[2];          // 0x1be58: state 0x39
    Sprite* sprite1be90;               // 0x1be90 (deleted with ~Sprite + operator delete)
    int unk1be94;                      // 0x1be94
    int unk1be98;                      // 0x1be98
    ModelObject* glow[8];              // 0x1be9c: virtual-deleted
    void* flareTex[4];                 // 0x1bebc: front, rear, brake, reverse flare textures (LoadTextures)
    void* player;                      // 0x1becc
    CarRecorder recorder;              // 0x1bed0: state 0x3a
    int replayMode;                    // 0x1befc
    unsigned int flags;                // 0x1bf00: arg g (bit 2: no sounds)
    SoundSource slotSound0;            // 0x1bf04: states 0x3b..0x42 (eight separate members)
    SurfaceSound* slotSurface0;        // 0x1bf74
    SoundSource slotSound1;            // 0x1bf78
    SurfaceSound* slotSurface1;        // 0x1bfe8
    SoundSource slotSound2;            // 0x1bfec
    SurfaceSound* slotSurface2;        // 0x1c05c
    SoundSource slotSound3;            // 0x1c060
    SurfaceSound* slotSurface3;        // 0x1c0d0
    SoundSource slotSound4;            // 0x1c0d4
    SurfaceSound* slotSurface4;        // 0x1c144
    SoundSource crashSound0;           // 0x1c148 (hard)
    SoundSource crashSound1;           // 0x1c1b8 (window)
    SoundSource crashSound2;           // 0x1c228 (light)
    Sample* crashSample[7];            // 0x1c298
    SoundLayer engineLayer[4];         // 0x1c2b4: state 0x43 (engine idle, 1-3)
    SoundLayer exhaustLayer[4];        // 0x1c4e4: state 0x44
    float throttleRpm;                 // 0x1c714
    Vec3 engineLoc;                    // 0x1c718: state 0x45
    Vec3 exhaustLoc;                   // 0x1c724: state 0x46
    BYTE unk1c730[0x1c75c - 0x1c730];
    CarSnapshot snapshot[3];           // 0x1c75c: state 0x47
    BYTE unk1c900[0x1c908 - 0x1c900];
    int messageId;                     // 0x1c908
    int messageTime;                   // 0x1c90c
    BYTE unk1c910[4];
    EmptyBlock<0x28> history[0x100];   // 0x1c914: state 0x48 (256 x 0x28 replay records)
    BYTE unk1f114[0x1f160 - 0x1f114];
    CarEventQueue queue;               // 0x1f160: state 0x49 (dtor 0x4649f0)
    BYTE unk1f208[0x1f2d4 - 0x1f208];
    Vec3 vec1f2d4;                     // 0x1f2d4: states 0x4a..0x4d
    Vec3 vec1f2e0;                     // 0x1f2e0
    int unk1f2ec;                      // 0x1f2ec
    Vec3 vec1f2f0;                     // 0x1f2f0
    int unk1f2fc;                      // 0x1f2fc
    Vec3 vec1f300;                     // 0x1f300 (ends at sizeof = 0x1f30c)
};

#define CAROBJECT_OFFSET(m) ((int)&((CarObject*)0)->m)
typedef char CarObject_flare_check[CAROBJECT_OFFSET(flare) == 0x10ac ? 1 : -1];
typedef char CarObject_bodyVecB_check[CAROBJECT_OFFSET(bodyVecB) == 0x1044 ? 1 : -1];
typedef char CarObject_exhaustFlag_check[CAROBJECT_OFFSET(exhaustFlag) == 0x20e8 ? 1 : -1];
typedef char ModelObject_size_check[sizeof(ModelObject) == 0x12c ? 1 : -1];
typedef char CarObject_size_check[sizeof(CarObject) == 0x1f30c ? 1 : -1];
typedef char CarObject_wheel_check[CAROBJECT_OFFSET(wheel) == 0x28 ? 1 : -1];
typedef char CarObject_spring_check[CAROBJECT_OFFSET(spring) == 0xce8 ? 1 : -1];
typedef char CarObject_drive_check[CAROBJECT_OFFSET(drivetrain) == 0xdb8 ? 1 : -1];
typedef char CarObject_modelCache_check[CAROBJECT_OFFSET(modelCache) == 0xfa0 ? 1 : -1];
typedef char CarObject_materials_check[CAROBJECT_OFFSET(materials) == 0xfac ? 1 : -1];
typedef char CarObject_materials3_check[CAROBJECT_OFFSET(materials3) == 0xfd4 ? 1 : -1];
typedef char CarObject_ctrl11_check[CAROBJECT_OFFSET(ctrl11) == 0x1408 ? 1 : -1];
typedef char CarObject_vec1430_check[CAROBJECT_OFFSET(vec1430) == 0x1430 ? 1 : -1];
typedef char CarObject_vec1464_check[CAROBJECT_OFFSET(vec1464) == 0x1464 ? 1 : -1];
typedef char CarObject_blk1498_check[CAROBJECT_OFFSET(blk1498) == 0x1498 ? 1 : -1];
typedef char CarObject_mat1500_check[CAROBJECT_OFFSET(mat1500) == 0x1500 ? 1 : -1];
typedef char CarObject_tyreMat_check[CAROBJECT_OFFSET(tyreMat) == 0x1680 ? 1 : -1];
typedef char CarObject_hitNormal_check[CAROBJECT_OFFSET(hitNormal) == 0x1e04 ? 1 : -1];
typedef char CarObject_hitPoint_check[CAROBJECT_OFFSET(hitPoint) == 0x1e54 ? 1 : -1];
typedef char CarObject_hitDepth_check[CAROBJECT_OFFSET(hitDepth) == 0x1e84 ? 1 : -1];
typedef char CarObject_dashPos_check[CAROBJECT_OFFSET(dashPos) == 0x1ed0 ? 1 : -1];
typedef char CarObject_follower_check[CAROBJECT_OFFSET(follower1) == 0x1f4c ? 1 : -1];
typedef char CarObject_hood_check[CAROBJECT_OFFSET(hoodMatrix) == 0x1f70 ? 1 : -1];
typedef char CarObject_trunk_check[CAROBJECT_OFFSET(trunkMatrix) == 0x1fb0 ? 1 : -1];
typedef char CarObject_vec2068_check[CAROBJECT_OFFSET(vec2068) == 0x2068 ? 1 : -1];
typedef char CarObject_exhaustPos_check[CAROBJECT_OFFSET(exhaustPos) == 0x20dc ? 1 : -1];
typedef char CarObject_meshes_check[CAROBJECT_OFFSET(meshes) == 0x20f0 ? 1 : -1];
typedef char CarObject_spray_check[CAROBJECT_OFFSET(spray) == 0x2100 ? 1 : -1];
typedef char CarObject_smokeB_check[CAROBJECT_OFFSET(smokeB) == 0x222c ? 1 : -1];
typedef char CarObject_dust_check[CAROBJECT_OFFSET(dust) == 0x2358 ? 1 : -1];
typedef char CarObject_debris_check[CAROBJECT_OFFSET(debris) == 0x2488 ? 1 : -1];
typedef char CarObject_exhaust_check[CAROBJECT_OFFSET(exhaust) == 0x25e8 ? 1 : -1];
typedef char CarObject_skid_check[CAROBJECT_OFFSET(skid) == 0x2748 ? 1 : -1];
typedef char CarObject_unk1bcbc_check[CAROBJECT_OFFSET(unk1bcbc) == 0x1bcbc ? 1 : -1];
typedef char CarObject_vec1bcc4_check[CAROBJECT_OFFSET(vec1bcc4) == 0x1bcc4 ? 1 : -1];
typedef char CarObject_vec1bcd4_check[CAROBJECT_OFFSET(vec1bcd4) == 0x1bcd4 ? 1 : -1];
typedef char CarObject_lamp3_check[CAROBJECT_OFFSET(lamp3) == 0x1bd20 ? 1 : -1];
typedef char CarObject_lightFL_check[CAROBJECT_OFFSET(lightFrontLeft) == 0x1bd40 ? 1 : -1];
typedef char CarObject_lightFR_check[CAROBJECT_OFFSET(lightFrontRight) == 0x1bd94 ? 1 : -1];
typedef char CarObject_lightRear_check[CAROBJECT_OFFSET(lightRear) == 0x1bde8 ? 1 : -1];
typedef char CarObject_lightBrake_check[CAROBJECT_OFFSET(lightBrake) == 0x1be20 ? 1 : -1];
typedef char CarObject_lightReverse_check[CAROBJECT_OFFSET(lightReverse) == 0x1be58 ? 1 : -1];
typedef char CarObject_glow_check[CAROBJECT_OFFSET(glow) == 0x1be9c ? 1 : -1];
typedef char CarObject_recorder_check[CAROBJECT_OFFSET(recorder) == 0x1bed0 ? 1 : -1];
typedef char CarObject_flags_check[CAROBJECT_OFFSET(flags) == 0x1bf00 ? 1 : -1];
typedef char CarObject_sound4_check[CAROBJECT_OFFSET(slotSound4) == 0x1c0d4 ? 1 : -1];
typedef char CarObject_crash2_check[CAROBJECT_OFFSET(crashSound2) == 0x1c228 ? 1 : -1];
typedef char CarObject_layer0_check[CAROBJECT_OFFSET(engineLayer) == 0x1c2b4 ? 1 : -1];
typedef char CarObject_layer1_check[CAROBJECT_OFFSET(exhaustLayer) == 0x1c4e4 ? 1 : -1];
typedef char CarObject_snapshot_check[CAROBJECT_OFFSET(snapshot) == 0x1c75c ? 1 : -1];
typedef char CarObject_history_check[CAROBJECT_OFFSET(history) == 0x1c914 ? 1 : -1];
typedef char CarObject_queue_check[CAROBJECT_OFFSET(queue) == 0x1f160 ? 1 : -1];
typedef char CarObject_vec1f300_check[CAROBJECT_OFFSET(vec1f300) == 0x1f300 ? 1 : -1];
typedef char CarEventQueue_size_check[sizeof(CarEventQueue) == 0xa8 ? 1 : -1];
typedef char LightRec_size_check[sizeof(LightRec) == 0x1c ? 1 : -1];
typedef char CarLamp_size_check[sizeof(CarLamp) == 0x10 ? 1 : -1];
typedef char Wheel_size_check[sizeof(Wheel) == 0x330 ? 1 : -1];
typedef char SoundSource_size_check[sizeof(SoundSource) == 0x70 ? 1 : -1];

#endif
