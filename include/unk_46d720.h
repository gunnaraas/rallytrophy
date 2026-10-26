/* SpringFollower (0x46d720-0x46d7f8): a 0x1c-byte spring/damper that follows an offset between two limits.
 * Embedded twice in Car (+0x1f30, +0x1f4c). Map: analysis/range-46a8a0.md section 5. */
#ifndef UNK_46D720_H
#define UNK_46D720_H

struct SpringFollower {
    float stiffness;   // 0x00
    float damping;     // 0x04
    float rest;        // 0x08
    float position;    // 0x0c
    float lower;       // 0x10
    float upper;       // 0x14
    float velocity;    // 0x18
    SpringFollower();                                   // 0x46d720
    ~SpringFollower() {}                                // empty (folds to 0x402c90): gives Car an EH state per member
    void Update(float dt, float a, float b);            // 0x46d740
    void Reset();                                       // 0x46d7f0
};

#endif
