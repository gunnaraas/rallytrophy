/* Car environment-map draw (0x459230-0x45988f, stub 0x459860). Map: analysis/range-4563e0.md section 4.
 * Include after unk_481390.h, rallytrophy.h and unk_464ab0.h (MirrorView, MirrorContext, MirrorStage).
 * The Car member reads its fields through the raw views below (Car itself is not restructured). */
#ifndef UNK_459230_H
#define UNK_459230_H

// A 16-float local with an empty constructor and destructor (it only owns an unwind entry).
struct EnvBuf {
    float m[16];
    EnvBuf() {}
    ~EnvBuf() {}
};

// A plain float triple (no ctor/dtor, so that it adds no unwind state).
struct EnvVec {
    float x, y, z;
};

// A 16-float block copied as a whole (rep movsd).
struct EnvBlock {
    float m[16];
};

// The renderer's slots as the six-face render calls them (byte offsets in the comments).
#define ENV_SLOT(n) virtual void slot##n();
class EnvDevice {
public:
    ENV_SLOT(0)
    ENV_SLOT(1)
    ENV_SLOT(2)
    ENV_SLOT(3)
    ENV_SLOT(4)
    ENV_SLOT(5)
    ENV_SLOT(6)
    ENV_SLOT(7)
    ENV_SLOT(8)
    ENV_SLOT(9)
    ENV_SLOT(10)
    ENV_SLOT(11)
    ENV_SLOT(12)
    virtual void slot13();
    virtual void slot14();
    virtual void GetViewport(int* x, int* y, int* w, int* h);      // 0x3c
    ENV_SLOT(16)
    ENV_SLOT(17)
    ENV_SLOT(18)
    ENV_SLOT(19)
    ENV_SLOT(20)
    ENV_SLOT(21)
    ENV_SLOT(22)
    ENV_SLOT(23)
    ENV_SLOT(24)
    ENV_SLOT(25)
    ENV_SLOT(26)
    virtual int BeginScene();                                      // 0x6c
    virtual void EndScene();                                       // 0x70
    virtual void BeginFrame();                                     // 0x74
    virtual void EndFrame();                                       // 0x78
    virtual void BeginFace(void* target, int face, int a, int b);  // 0x7c
    ENV_SLOT(32)
    ENV_SLOT(33)
    virtual int GetRenderState(int state);                         // 0x88
    ENV_SLOT(35)
    ENV_SLOT(36)
    ENV_SLOT(37)
    ENV_SLOT(38)
    ENV_SLOT(39)
    ENV_SLOT(40)
    ENV_SLOT(41)
    ENV_SLOT(42)
    ENV_SLOT(43)
    ENV_SLOT(44)
    ENV_SLOT(45)
    ENV_SLOT(46)
    ENV_SLOT(47)
    ENV_SLOT(48)
    ENV_SLOT(49)
    ENV_SLOT(50)
    ENV_SLOT(51)
};
#undef ENV_SLOT

// The Car body fields the render position reads (Car+0x24).
struct EnvBody {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    float rows[12];                    // 0x20: matrix rows (stride 0x10)
    BYTE unk50[0xa0 - 0x50];
    Vec3 center;                       // 0xa0
    float unkac;                       // 0xac
    float unkb0;                       // 0xb0
    float unkb4;                       // 0xb4
};

// The render-target object at Car+0x1bce4: its cursor at +0x50 (0 = not started).
struct EnvTarget {
    BYTE unk0[0x50];
    int cursor;                        // 0x50
};

// The Car as the environment-map draw sees it.
struct EnvCar {
    BYTE unk0[0x24];
    EnvBody* body;                     // 0x24
    BYTE unk28[0x1428 - 0x28];
    int frame;                         // 0x1428: draw counter (even frames render two faces)
    BYTE unk142c[0x14fc - 0x142c];
    Stage* stage;                      // 0x14fc
    BYTE unk1500[0x1bce4 - 0x1500];
    EnvTarget* target;                 // 0x1bce4
};

// A 4x4 matrix local that starts as the identity (empty destructor: it owns an unwind entry).
struct EnvMat {
    float m[16];
    EnvMat() {}
    void SetIdentity()
    {
        m[2] = 0.0f; m[1] = 0.0f; m[6] = 0.0f; m[4] = 0.0f;
        m[9] = 0.0f; m[8] = 0.0f; m[10] = 1.0f; m[5] = 1.0f;
        m[0] = 1.0f;
        m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f;
        m[14] = 0.0f; m[13] = 0.0f; m[12] = 0.0f; m[15] = 1.0f;
    }
    ~EnvMat() {}
};

// The camera argument: the two matrices at +0x20 and +0x60.
struct EnvCamera {
    BYTE unk0[0x20];
    EnvBlock frame;                    // 0x20
    EnvBlock view;                     // 0x60
};

// The second argument: a draw context whose entries get their +0x10 field reset (0x4b8340).
struct EnvContext {
    void FUN_004b8340();               // 0x4b8340
};

// ViewMat composition 0x4b55e0 (thiscall, ret 4).
class LightViewEnv {
public:
    float m[16];
    void Concat(const EnvBlock& other);  // 0x4b55e0
};

Vec3* FUN_004203e0(Vec3* result, const EnvVec& a, const EnvVec& b); // cross product a x b (cdecl; returns its first argument)

#endif
