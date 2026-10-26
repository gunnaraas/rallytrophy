/* Car wheel spray effect WheelSpray (0x477370-0x47801f): the debris/spray
 * effect embedded in a Car at +0x2100 (vtable 0x4e20bc, 0x12c bytes: no members
 * beyond Effect), spawned only at the highest effect detail. Map:
 * analysis/range-4741a0.md section 2.7. Include after rallytrophy.h,
 * unk_481390.h and unk_47c460.h. */
#ifndef UNK_477370_H
#define UNK_477370_H

class WheelSpray : public Effect {
public:
    WheelSpray();                      // 0x4773d0 (scalar deleting dtor 0x477430, dtor thunk 0x477450)
    ~WheelSpray();
    void Update(float dt);             // 0x477460
    void Spawn(int type, float scale, float age, Car* car, CarBody* body); // 0x477b60
    virtual void Render(RenderDevice* device, Camera* camera); // 0x477690
    void SpawnCore(float scale, float age); // 0x477b90
};

#endif
