/* Exhaust smoke ExhaustSmoke (0x478020-0x47886f), an Effect embedded in a Car
 * at +0x25e8 (vtable 0x4e20fc, 0x160 bytes: no members beyond Effect).
 * Map: analysis/range-4741a0.md section 2.8.
 * Include after rallytrophy.h, unk_481390.h and unk_47c460.h. */
#ifndef UNK_478020_H
#define UNK_478020_H

class ExhaustSmoke : public Effect {
public:
    ExhaustSmoke();                    // 0x478080 (scalar deleting dtor 0x4780c0, dtor thunk 0x4780e0)
    ~ExhaustSmoke();                   // 0x4780e0
    void Update(float dt);             // 0x4780f0
    virtual void Render(RenderDevice* device, Camera* camera); // 0x478220
    void Emit(const Vec3* pos, const Vec3* vel, float throttle, float step, float age); // 0x4786a0
};

#endif
