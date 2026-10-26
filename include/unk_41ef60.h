// Car body vs. world contact response (0x41ef60). Contact record layout (0x50 bytes) inferred from pseudocode.
#ifndef UNK_41EF60_H
#define UNK_41EF60_H
#include "unk_481390.h"

class RespBody {                // the car rigid body (see ColBody / PhysBody for other views)
public:
    char u0[0x234];
    Vec3 FUN_004204f0(const Vec3& point); // 0x4204f0: velocity of a world point
    void FUN_0041c010();                  // 0x41c010: integrate / clear forces
    void Respond(void* car, void* stage, int flag); // 0x41ef60
};
#endif
