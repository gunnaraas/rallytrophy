#ifndef UNK_40A7B0_H
#define UNK_40A7B0_H
// Types for 0x40a7b0-0x40b4e0 helpers (layouts inferred from pseudocode).
struct U40A7B0Target {
    virtual ~U40A7B0Target() {}
    char pad[0x10];
    void Refresh(); // original: FUN_0041be00
};
struct U40Empty { ~U40Empty() {} };
struct U40A7B0Owner {
    float pos[3];
    float mat[16];               // 0x0c
    char pad1[0x50 - 0x4c];
    U40A7B0Target* m_target;     // 0x50
    int m_54, m_58, m_5c, m_60, m_64;
    U40Empty m_unw;
    U40Empty m_unw2;
    ~U40A7B0Owner();             // original: FUN_0040a7b0
    void Sync();                 // original: FUN_0040a7f0
    void Set58(int a, int b);    // original: FUN_0040a830
};
struct U40B4Elem4 { int v; U40B4Elem4(); ~U40B4Elem4(); };
struct U40B4Elem8 { int v[2]; U40B4Elem8(); ~U40B4Elem8(); };
struct U40B0F0 {
    char flag; char pad[3];
    int a, b, c;
    char pad2[0x30 - 0x10];
    int d;
    U40B0F0(); // original: FUN_0040b0f0
};
struct U40B480 {
    char slot[4];
    U40Empty m_unw;
    U40Empty m_unw2;
    ~U40B480(); // original: FUN_0040b480
};
struct U40B4E0 {
    U40B4Elem4 e4[4];  // v = -1 after ctor
    U40B4Elem8 e8[16];
    U40B4E0();  // original: FUN_0040b4e0
    void Init(); // original: FUN_0040b560
};
#endif
