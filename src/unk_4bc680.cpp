// End of game code: vector<void*> instantiations, char* holder, draw queue.
#include <string.h>
#include <stdlib.h>
#include "unk_4bc680.h"

extern const char DAT_005024f0[];
extern int __cdecl DrawCompare(const void* a, const void* b); // 0x4bcd50 (qsort comparator)

// FUNCTION: RALLYTROPHY 0x4bc680
// SYMBOL: ?insert@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXPAPAXIABQAX@Z
// std::vector<void*>::insert(it, n, x)
template class std::vector<void*>;
typedef void Generated_4bc680();

// FUNCTION: RALLYTROPHY 0x4bc8b0
CStr::~CStr()
{
    operator delete(p);
}

// FUNCTION: RALLYTROPHY 0x4bc8c0
void CStr::Set(const char* s)
{
    operator delete(p);
    if (s == 0) {
        p = (char*)operator new(1);
        *p = 0;
        return;
    }
    p = (char*)operator new(strlen(s) + 1);
    strcpy(p, s);
}

// FUNCTION: RALLYTROPHY 0x4bc930
const char* CStr::Get()
{
    return p ? p : DAT_005024f0;
}

// FUNCTION: RALLYTROPHY 0x4bc970
DrawQueueX::DrawQueueX()
{
    one = 1;
    zero = 0;
    a.reserve(0x4000);
    b.reserve(0x4000);
    c.reserve(0x4000);
}

// NONMATCHING: RALLYTROPHY 0x4bcb50
// Register allocation differs (original: ebx=this, edi=0, i in ebp; first size test uses sar, ours test mask); 516 vs 499 bytes.
void DrawQueueX::Flush(QDevice* device, int flag)
{
    if (!a.empty())
        qsort(&a[0], a.size(), 4, DrawCompare);
    int curMat = 0;
    int curKey = 0;
    int na = a.size();
    int nb = b.size();
    int n = na;
    if (na <= nb)
        n = nb;
    for (int i = 0; i < n; i++) {
        if (i < na) {
            QItem* it = (QItem*)a[i];
            if (it->key != curKey) {
                device->SetTexture(it->key);
                curKey = it->key;
            }
            if (it->matrix != (void*)curMat) {
                curMat = (int)it->matrix;
                if (curMat)
                    device->SetMatrix2((char*)curMat + 0x60, 1);
                else
                    device->SetMatrix(1);
            }
            if (it->tex && it->tex->frame != device->frame)
                it->tex->Upload();
            device->DrawItem(it);
        }
        if (i < nb) {
            QItem* it = (QItem*)b[i];
            if (it->tex && it->tex->frame != device->frame)
                it->tex->Upload();
            device->DrawItem2(it, 1);
        }
    }
    a.resize(0);
    b.resize(0);
}

// FUNCTION: RALLYTROPHY 0x4bce80
void DrawQueueX::Begin(QDevice* device)
{
    unsigned n = c.size();
    count = n;
    for (unsigned i = 0; i < n; i++) {
        QItem* it = (QItem*)c[i];
        QTexture* t = it->tex;
        if (t && t->frame != device->frame)
            t->Upload();
        device->DrawItem2(it, 1);
    }
    c.resize(0);
}

// FUNCTION: RALLYTROPHY 0x4bcf30
// SYMBOL: ?reserve@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXI@Z
// std::vector<void*>::reserve
typedef void Generated_4bcf30();
