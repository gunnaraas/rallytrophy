#ifndef UNK_402D70_H
#define UNK_402D70_H
#include <vector>
// Pooled 256-bucket hash multimap 0x402d70-0x402f30: key = address-like uint, value = int.
struct HashNode {            // 16 bytes
    HashNode* next;          // 0x0 bucket chain (distinct keys)
    HashNode* dup;           // 0x4 chain of equal keys
    unsigned int key;        // 0x8
    int val;                 // 0xc
};
struct HashMap {
    int hits;                // 0x0 (bumped by Find on a hit)
    int count;               // 0x4
    HashNode* bucket[256];   // 0x8
    HashNode* freelist;      // 0x408
    std::vector<HashNode*> pool; // 0x40c (allocator) / 0x410..0x418; blocks of 0x400 nodes
    HashMap();
    void Init();                         // 0x402d70
    void Insert(unsigned int key, int val); // 0x402da0
    int Find(unsigned int key, int val);    // 0x402e40
    void Reset();                        // 0x402eb0
    void LinkBlock(HashNode* b);         // 0x402ef0
    void Grow();                         // 0x402f30
};
#endif
