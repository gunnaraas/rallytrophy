// DrawContext (0x4b7a40-0x4b8340): per-frame scene draw lists.
#include <string.h>
#include <stdlib.h>
#include "unk_4b7a40.h"

// Item draw call: slot 6 of the scene node vtable.
class DrawItem {
public:
    virtual void S0(); virtual void S1(); virtual void S2(); virtual void S3(); virtual void S4(); virtual void S5();
    virtual void Draw(DrawDevice* device, int flag, DrawQueue* queue);
};

extern "C" void __cdecl Thunk_004b7c70();

// FUNCTION: RALLYTROPHY 0x4b7a40
DrawContext::DrawContext()
    : roots(0x40, 0x40), solids(0x40, 0x40), normals(0x40, 0x40), specials(0x40, 0x40)
{
    DrawNameString* str = (DrawNameString*)&name;
    unsigned int n = strlen("Scene");
    type = 4;
    if (str->Grow(n, true)) {
        char* p = str->ptr;
        memcpy(p, "Scene", n);
        p[str->len = n] = 0;
    }
    unk188.x = 0;
    unk188.y = 0;
    unk188.z = 0;
    unk174.x = 0;
    unk184 = 0;
    unk174.y = 0;
    unk180.value = 0;
    unk174.z = 1.0f;
}

// FUNCTION: RALLYTROPHY 0x4b7bd0
// SYMBOL: ??_GDrawContext@@UAEPAXI@Z
typedef void Generated_4b7bd0();

// FUNCTION: RALLYTROPHY 0x4b7bf0
DrawContext::~DrawContext()
{
}

// FUNCTION: RALLYTROPHY 0x4b7c50
void __stdcall InitDrawFrustum()
{
    DAT_0057bc88.Construct();
}

// FUNCTION: RALLYTROPHY 0x4b7c60
void __stdcall RegisterDrawFrustumExit()
{
    atexit((void(__cdecl*)())Thunk_004b7c70);
}

// NONMATCHING: RALLYTROPHY 0x4b7c80
// Original duplicates the inline list-grow code per case and shares only the tails; ours is 43 bytes shorter.
void DrawContext::CullViews(DrawDevice* device, int)
{
    solids.m_end = solids.m_data;
    normals.m_end = normals.m_data;
    solids.m_count = 0;
    normals.m_count = 0;
    specials.m_end = specials.m_data;
    specials.m_count = 0;
    DAT_0057bc88.Init(device);
    DAT_00502470++;
    for (int i = 0; i < roots.m_count; i++) {
        SceneNode* node = roots.m_data[i];
        SceneNode* stack[128];
        int depth = 0;
        SceneNode** sp = stack;
        while (node) {
            int skip = 0;
            switch (node->type) {
            case 1:
                if (node->flags & 4) {
                    if (node->firstChild == 0 || DAT_0057bc88.TestBoundsB(&node->boundsB)) {
                        if (DAT_0057bc88.TestBoundsA(&node->boundsA)) {
                            solids.Add(node);
                            node->stamp = DAT_00502470;
                        }
                    } else {
                        skip = 1;
                    }
                } else if (node->flags & 2) {
                    skip = 1;
                } else if ((node->flags & 3) == 0 && DAT_0057bc88.TestSphere(&node->boundsA, &node->world)) {
                    normals.Add(node);
                    node->stamp = DAT_00502470;
                }
                break;
            case 2:
                if ((node->flags & 3) == 0 && DAT_0057bc88.TestSized(&node->boundsA, &node->world, 25.0f)) {
                    specials.Add(node);
                    node->stamp = DAT_00502470;
                }
                break;
            case 3:
            case 4:
                break;
            default:
                if ((node->flags & 3) == 0) {
                    if ((node->flags & 4) == 0) {
                        if (DAT_0057bc88.TestSphere(&node->boundsA, &node->world)) {
                            normals.Add(node);
                            node->stamp = DAT_00502470;
                        }
                    } else if (DAT_0057bc88.TestBoundsA(&node->boundsA)) {
                        normals.Add(node);
                        node->stamp = DAT_00502470;
                    }
                }
            }
            SceneNode* next = node->firstChild;
            if (next == 0 || skip) {
                node = node->next;
                if (node == 0) {
                    if (depth < 1)
                        break;
                    depth--;
                    sp--;
                    node = (*sp)->next;
                }
            } else {
                *sp = node;
                sp++;
                depth++;
                node = next;
            }
        }
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b8160
void DrawContext::DrawViews(DrawDevice*, int)
{
    __asm {
        _emit 051h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 0f9h
        _emit 033h
        _emit 0edh
        _emit 0b9h
        _emit 0b4h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 089h
        _emit 06ch
        _emit 024h
        _emit 010h
        _emit 0e8h
        _emit 0e9h
        _emit 0c2h
        _emit 0f5h
        _emit 0ffh
        _emit 0b9h
        _emit 0b4h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 0e8h
        _emit 0dfh
        _emit 0c2h
        _emit 0f5h
        _emit 0ffh
        _emit 085h
        _emit 0c0h
        _emit 076h
        _emit 017h
        _emit 0a1h
        _emit 0bch
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 08bh
        _emit 00dh
        _emit 0b8h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 050h
        _emit 051h
        _emit 0b9h
        _emit 0b4h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 0e8h
        _emit 0e4h
        _emit 0c1h
        _emit 0fch
        _emit 0ffh
        _emit 08dh
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 052h
        _emit 055h
        _emit 0b9h
        _emit 0c4h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 089h
        _emit 06ch
        _emit 024h
        _emit 018h
        _emit 0e8h
        _emit 0f0h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 050h
        _emit 055h
        _emit 0b9h
        _emit 0d4h
        _emit 01dh
        _emit 058h
        _emit 000h
        _emit 089h
        _emit 06ch
        _emit 024h
        _emit 018h
        _emit 0e8h
        _emit 0dch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 018h
        _emit 08bh
        _emit 016h
        _emit 06ah
        _emit 00bh
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 092h
        _emit 088h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 08fh
        _emit 088h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0a3h
        _emit 0b0h
        _emit 0bdh
        _emit 057h
        _emit 000h
        _emit 08bh
        _emit 006h
        _emit 051h
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 090h
        _emit 0a0h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 09fh
        _emit 084h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0ddh
        _emit 074h
        _emit 037h
        _emit 08bh
        _emit 016h
        _emit 06ah
        _emit 001h
        _emit 06ah
        _emit 00dh
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 092h
        _emit 084h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 08fh
        _emit 080h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 097h
        _emit 07ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 006h
        _emit 051h
        _emit 08bh
        _emit 08fh
        _emit 078h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 052h
        _emit 051h
        _emit 08dh
        _emit 097h
        _emit 074h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 052h
        _emit 053h
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 090h
        _emit 090h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ebh
        _emit 00dh
        _emit 08bh
        _emit 006h
        _emit 055h
        _emit 06ah
        _emit 00dh
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 090h
        _emit 084h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 016h
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 092h
        _emit 094h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 087h
        _emit 068h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0dbh
        _emit 03bh
        _emit 0c5h
        _emit 07eh
        _emit 023h
        _emit 08bh
        _emit 0afh
        _emit 060h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04dh
        _emit 000h
        _emit 08bh
        _emit 006h
        _emit 053h
        _emit 051h
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 090h
        _emit 09ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 087h
        _emit 068h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 043h
        _emit 083h
        _emit 0c5h
        _emit 004h
        _emit 03bh
        _emit 0d8h
        _emit 07ch
        _emit 0e3h
        _emit 08bh
        _emit 016h
        _emit 06ah
        _emit 001h
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 092h
        _emit 0b0h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 09fh
        _emit 038h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0afh
        _emit 03ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0ddh
        _emit 074h
        _emit 01dh
        _emit 08bh
        _emit 00bh
        _emit 085h
        _emit 0c9h
        _emit 074h
        _emit 010h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 001h
        _emit 068h
        _emit 0a0h
        _emit 0bdh
        _emit 057h
        _emit 000h
        _emit 052h
        _emit 056h
        _emit 0ffh
        _emit 050h
        _emit 018h
        _emit 083h
        _emit 0c3h
        _emit 004h
        _emit 03bh
        _emit 0ddh
        _emit 075h
        _emit 0e3h
        _emit 08bh
        _emit 09fh
        _emit 04ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0bfh
        _emit 050h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0dfh
        _emit 074h
        _emit 01dh
        _emit 08bh
        _emit 00bh
        _emit 085h
        _emit 0c9h
        _emit 074h
        _emit 010h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 001h
        _emit 068h
        _emit 0a0h
        _emit 0bdh
        _emit 057h
        _emit 000h
        _emit 052h
        _emit 056h
        _emit 0ffh
        _emit 050h
        _emit 018h
        _emit 083h
        _emit 0c3h
        _emit 004h
        _emit 03bh
        _emit 0dfh
        _emit 075h
        _emit 0e3h
        _emit 06ah
        _emit 001h
        _emit 056h
        _emit 0b9h
        _emit 0a0h
        _emit 0bdh
        _emit 057h
        _emit 000h
        _emit 0e8h
        _emit 06ch
        _emit 048h
        _emit 000h
        _emit 000h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 059h
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x4b82f0
void DrawContext::BeginQueue(DrawDevice* device, int unused)
{
    DAT_0057bda0.Begin(device);
}

// FUNCTION: RALLYTROPHY 0x4b8310
void DrawContext::Slot1(void* view)
{
    for (int n = roots.m_count, i = 0; n > 0; n--, i++)
        roots.m_data[i]->Slot1(view);
}

// FUNCTION: RALLYTROPHY 0x4b8340
void DrawContext::StampVisible()
{
    int stamp = DAT_00502470;
    SceneNode** p;
    for (p = solids.m_data; p != solids.m_end; p++)
        (*p)->stamp = stamp;
    for (p = normals.m_data; p != normals.m_end; p++)
        (*p)->stamp = stamp;
    for (p = specials.m_data; p != specials.m_end; p++)
        (*p)->stamp = stamp;
}
