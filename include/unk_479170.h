/* Car wheel/slide emitter CarEmitterB (0x479170-0x47b48f): the tyre smoke and
 * dust effect embedded in a Car at +0x222c (vtable 0x4e2204, 0x12c bytes: no
 * members beyond Effect). Map: analysis/range-478870.md section 2.3.
 * Include after rallytrophy.h, unk_481390.h and unk_47c460.h. */
#ifndef UNK_479170_H
#define UNK_479170_H

// Float to int with the x87 default rounding (inline assembler, as in the original).
inline int FloatToInt(float f)
{
    int i;
    __asm {
        fld f
        fistp i
    }
    return i;
}

// A float compared as an integer (the original orders non-negative floats by their bit patterns).
inline bool FloatBelow(const float& a, const float& b)
{
    return *(const int*)&a < *(const int*)&b;
}

class CarEmitterB : public Effect {
public:
    CarEmitterB();                     // 0x479170 (scalar deleting dtor 0x479220)
    ~CarEmitterB();                    // 0x479240
    void Update(float dt);             // 0x479250
    float Spawn(int type, float scale, float age, Car* car, CarBody* body); // 0x479490
    virtual void Render(RenderDevice* device, Camera* camera); // 0x479530

    // The five spawn cores; each returns the emission ratio (0..1) it computed.
    float SpawnCore1(float scale, float age); // 0x4799f0
    float SpawnCore2(float scale, float age); // 0x479f60
    float SpawnCore3(float scale, float age); // 0x47a4c0
    float SpawnCore4(float scale, float age); // 0x47aa00
    float SpawnCore5(float scale, float age); // 0x47af50
};

#endif
