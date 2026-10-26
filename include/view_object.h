#ifndef VIEW_OBJECT_H
#define VIEW_OBJECT_H

// Module-local layout of the scene-object base (constructor 0x4b2830,
// destructor 0x4b29c0). Its implementation remains in the scene-object module.
class ViewObjectBase {
public:
    ViewObjectBase();
    virtual ~ViewObjectBase();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual int Slot8();
    unsigned char data4[0x124 - 4];
};

struct ViewMatrix {
    void ClearOffDiagonal()
    {
        m[1] = m[2] = m[3] = 0;
        m[4] = m[6] = m[7] = 0;
        m[8] = m[9] = m[11] = 0;
        m[12] = m[13] = m[14] = 0;
    }
    void SetIdentityDiagonal() { m[0] = m[5] = m[10] = m[15] = 1; }
    float m[16];
};

class ViewObject : public ViewObjectBase {
public:
    ViewObject();
    virtual ~ViewObject();
    void UpdateBasis();
    float nearPlane;                // 0x124
    float farPlane;                 // 0x128
    float fov;                      // 0x12c
    int zone;                       // 0x130
    int resetSmoothing;             // 0x134
    float savedPosition[3];         // 0x138
    ViewMatrix previous;            // 0x144
    ViewMatrix basis;               // 0x184
    int stepArg;                    // 0x1c4
    unsigned char flag;             // 0x1c8
};

#endif
