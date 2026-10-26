#ifndef UNK_4BC680_H
#define UNK_4BC680_H
// End of game code (0x4bc680-0x4bcf70): shared vector<void*> instantiations, a tiny char* holder and the
// transparent/opaque draw queue (global 0x57bda0; its vectors sit at 0x581db4/0x581dc4/0x581dd4).
#include <vector>

// Renderer vtable as the draw queue uses it (slot N at +4*N).
class QDevice {
public:
    virtual void S00(); virtual void S04(); virtual void S08(); virtual void S0c();
    virtual void S10(); virtual void S14(); virtual void S18(); virtual void S1c();
    virtual void S20(); virtual void S24(); virtual void S28(); virtual void S2c();
    virtual void S30(); virtual void S34(); virtual void S38(); virtual void S3c();
    virtual void S40(); virtual void S44(); virtual void S48(); virtual void S4c();
    virtual void S50(); virtual void S54(); virtual void S58(); virtual void S5c();
    virtual void S60(); virtual void S64(); virtual void S68(); virtual void S6c();
    virtual void S70(); virtual void S74(); virtual void S78(); virtual void S7c();
    virtual void S80(); virtual void S84(); virtual void S88();
    virtual void SetTexture(int tex);              // 0x8c
    virtual void S90(); virtual void S94(); virtual void S98(); virtual void S9c();
    virtual void Sa0(); virtual void Sa4(); virtual void Sa8(); virtual void Sac();
    virtual void SetMatrix(int flag);              // 0xb0 (null matrix)
    virtual void Sb4();
    virtual void SetMatrix2(void* m, int flag);    // 0xb8
    virtual void Sbc();
    virtual void Sc0(); virtual void Sc4(); virtual void Sc8(); virtual void Scc();
    virtual void Sd0(); virtual void Sd4(); virtual void Sd8(); virtual void Sdc();
    virtual void Se0(); virtual void Se4(); virtual void Se8(); virtual void Sec();
    virtual void Sf0(); virtual void Sf4(); virtual void Sf8(); virtual void Sfc();
    virtual void DrawItem(void* item);             // 0x100
    virtual void DrawItem2(void* item, int flag);  // 0x104
    int pad[8];
    int frame;                                     // 0x24 (after vptr: this+0x24)
};

class QTexture {
public:
    virtual void S0();
    virtual void Upload();                         // 0x4
    int frame;                                     // +4
};

struct QItem {
    int key;           // 0x0
    char pad[0x10];
    QTexture* tex;     // 0x14
    void* matrix;      // 0x18
};

class CStr {
public:
    ~CStr();                       // 0x4bc8b0
    void Set(const char* s);       // 0x4bc8c0
    const char* Get();             // 0x4bc930
    char* p;
};

class DrawQueueX {
public:
    DrawQueueX();                                   // 0x4bc970
    void Flush(QDevice* device, int flag);          // 0x4bcb50
    void Begin(QDevice* device);                    // 0x4bce80
    int one;                                        // 0x0
    int unk4[2];
    int count;                                      // 0xc
    int zero;                                       // 0x10
    char body[0x6014 - 0x14];
    std::vector<void*> a;                           // 0x6014
    std::vector<void*> b;                           // 0x6024
    std::vector<void*> c;                           // 0x6034
};
#endif
