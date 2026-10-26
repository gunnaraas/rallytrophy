/* ViewManager camera file (0x4563e0-0x4581ef): the camera preset table and the statics of the chase/replay camera.
 * Map: analysis/range-4563e0.md section 1. Include after unk_481390.h and rallytrophy.h. */
#ifndef UNK_4563E0_H
#define UNK_4563E0_H

// One of the nine camera presets (0x20 bytes) filled from data/settings/camera.txt by 0x457b10. The empty
// constructor and destructor give the array its EH vector constructor/destructor iterators (folded 0x490ad0/0x402c90).
struct CameraPreset {
    CameraPreset() {}
    ~CameraPreset() {}
    float lookAt[3];                   // 0x00: CameraNLookAt
    float position[3];                 // 0x0c: CameraNPosition
    float stiffness;                   // 0x18: CameraNStiffness
    float fov;                         // 0x1c: CameraNFov (radians)
};

// The preset table (0x508ca8, 9 x 0x20 bytes). A static data member: its guarded destructor registration (guard bit 0
// of the byte at 0x508ca4) differs from a file-scope array's.
class CameraTable {
public:
    static CameraPreset presets[9];    // 0x508ca8
};

// The Vec3 {0, -0.1, 10} the ViewManager constructor stores into three globals on every construction; the loader
// 0x457b10 overwrites them with the Camera7 vector.
extern float DAT_00508c98, DAT_00508c9c, DAT_00508ca0;

#endif
