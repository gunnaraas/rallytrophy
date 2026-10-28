#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_43a390.h"
#include <stdlib.h>
#include "unk_43f9e0.h"
#include "unk_439170.h"
#include "unk_43c590.h"
#include "unk_43d770.h"
#include <math.h>
#include <string.h>
#include <vector>

// ---- Group C1 ----


// ---- from group draft Z6 ----
class Z6_Game : public MenuGame {
public:
    int FUN_004368a0(MenuScreen* screen);
};
struct Z6_Mouse {
    float x, y;
};
struct Z6_Entry {
    int value, unk4;
};
class Z6_GameData {
public:
    BYTE unk0[0x5b08];
    int index;                         // 0x5b08
    int unk5b0c;
    Z6_Entry table[32];                // 0x5b10
    int unk5c10;                       // 0x5c10
};
#define Z6_DATA ((Z6_GameData*)DAT_0056b0c8)


// ---- from group draft Z7 ----

// ---- from group draft Z8 ----
struct C4_Icon {
    Sprite* sprite;                    // 0x00
    float left, top, right, bottom;    // 0x04: texture rectangle
    float width, height;               // 0x14: size in 1024x768 pixels
};

extern int DAT_004f8d14[];             // text ids by m_rallyInfo[3]
extern int DAT_004f8d24[];             // by m_ghostMode
extern int DAT_004f8d2c[];             // by m_damage
extern int DAT_004f8d34[];             // by m_cars
extern const char* DAT_004f8d44[];     // by m_option4 (filled by the ctor, 0x43a3c0)
static const char C4_SEP[] = ": ";     // 0x4f75c8
static const char C4_SPACE[] = " ";    // 0x4f2e40

// FUNCTION: RALLYTROPHY 0x43a3c0
Screen1378::Screen1378()
{
    unk24 = 0;
    unk28 = 0;
    m_selected = 1;
    DAT_004f8d44[0] = FUN_00405380(0x28);
}

// FUNCTION: RALLYTROPHY 0x43a420
// SYMBOL: ??_GScreen1378@@UAEPAXI@Z
typedef void Generated_43a420();

// FUNCTION: RALLYTROPHY 0x43a440
Screen1378::~Screen1378()
{
    Release();
}

// NONMATCHING: RALLYTROPHY 0x43a490
// Setup resource initialization and the two Sprite layouts are reconstructed; VC6 EH-state, field-store order,
// and register allocation remain different (built 426 bytes; original 432; 140 differing bytes).
void Screen1378::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    unk24 = new Sprite;
    unk28 = new Sprite;
    static_cast<Sprite*>(unk24)->texture = new Texture("data/menu/network_box1.tga", 0x841, 1);
    static_cast<Sprite*>(unk28)->texture = new Texture("data/menu/network_box2.tga", 0x841, 1);

    volatile Sprite* first = static_cast<Sprite*>(unk24);
    volatile Sprite* second = static_cast<Sprite*>(unk28);
    first->unk9c = 0xe;
    second->unk9c = 0xe;
    first->unk98 = 0xf;
    first->unk60[0] = 1.0f;
    first->unk60[1] = 1.0f;
    first->unk60[2] = 1.0f;
    first->unk60[3] = 1.0f;
    first->unk18 = 5;
    second->unk98 = 0xf;
    second->unk60[0] = 1.0f;
    second->unk60[1] = 1.0f;
    second->unk60[2] = 1.0f;
    second->unk60[3] = 1.0f;
    first->unk40[0] = 1.0f;
    first->unk40[1] = 1.0f;
    first->unk40[2] = 1.0f;
    first->unk40[3] = 1.0f;
    first->unk1c = 6;
    second->unk40[0] = 1.0f;
    second->unk40[1] = 1.0f;
    second->unk40[2] = 1.0f;
    second->unk40[3] = 1.0f;
    second->unk18 = 5;
    second->unk1c = 6;
    first->unk24 = 2;
    second->unk24 = 2;
    first->texture->Upload(DAT_0056b0ac);
    second->texture->Upload(DAT_0056b0ac);
    Begin(game);
}

// FUNCTION: RALLYTROPHY 0x43a640
void Screen1378::Release()
{
    if (!m_state) {
        return;
    }
    if (unk24) {
        delete unk24->texture;
        delete unk24;
    }
    if (unk28) {
        delete unk28->texture;
        delete unk28;
    }
    unk24 = 0;
    unk28 = 0;
    m_state = 0;
    m_game = 0;
}

// FUNCTION: RALLYTROPHY 0x43a6b0
// Entry dispatch is reconstructed; modal frame body is implemented by the parallel group.
int Screen1378::Open(MenuGame* game, NetRace* net)
{
    Setup(game);
    this->net = net;
    m_selected = 1;
    return Frame();
}

// NONMATCHING: RALLYTROPHY 0x43a6e0
// Same size (1685) and identical except register allocation: the original keeps `this` in esi and the shared zero
// constant in edi, mine the reverse (118 bytes differ, all esi/edi swaps). Tried: declaration order, dummy `this` uses.
// original: FUN_0043a6e0 (Screen1378::Frame: modal loop; returns -1 (back), 1 or 2)
int Screen1378::Frame()
{
    int wait = 0;
    int quit = 0;
    HudPoint mouse;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    Layout();
    int finished = 0;
    DAT_0056b0bc->ClearKeys();
    int first = 1;
    unk2c = 0;
    int entered = 0;
    int joined = 0;
    while (FUN_004b69f0(wait) && !quit && !finished) {
        m_game->Update();
        NetRace* n = net;
        n->ServiceNetwork();
        mouse = *(HudPoint*)&DAT_0057ba00;
        if (!entered && DAT_0056b0b8->GetStatus() == 1) {
            Player* player = n->m_localPlayer;
            entered = 1;
            player->Lock();
            int cls = Z6_DATA->table[Z6_DATA->index].value;
            int unk = Z6_DATA->unk5c10;
            player->unk2c = cls + 1;
            player->unk24 = unk;
            player->FUN_00490fb0(DAT_00501338[0]);
            player->unk20 = DAT_005013ec[0];
            player->Unlock();
            n->SendPlayerInfo();
            if (n->m_host) {
                n->SendState(2);
            } else {
                n->SendState(1);
            }
        }
        if (DAT_0056b0b8->GetStatus() == 1) {
            int cls = Z6_DATA->table[Z6_DATA->index].value;
            int bit = 1 << cls;
            int cars = DAT_0056b0b8->m_cars;
            if (!(DAT_005014e0[cars] & bit)) {
                n->SendState(1);
                n->ServiceNetwork();
                joined = 1;
                finished = 1;
                continue;
            }
        }
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            int hit = HitTest(mouse, 0, 2);
            if (hit != -1) {
                m_selected = hit;
            }
        }
        if (DAT_0057ba43) {
            if (((Z6_Game*)m_game)->FUN_004368a0(this)) {
                quit = 1;
            }
        }
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        if (DAT_0057ba35) {
            switch (m_selected) {
            case 0:
                if (unk2c == 0) {
                    ((MenuManager*)((BYTE*)DAT_0056b0c8 + 0x2df4))->Run((MenuGame*)DAT_0056b0c8, net);
                    DAT_0056b0bc->ClearKeys();
                }
                break;
            case 1:
                if (unk2c == 0) {
                    NetRace* m = net;
                    if (m->m_host) {
                        if (count > 1) {
                            m->FUN_00482510();
                            m->SendState(3);
                            unk2c = 1;
                        } else {
                            m_game->Play(3);
                        }
                    } else if (DAT_0056b0b8->GetStatus() == 1 && entered) {
                        m->SendState(2);
                        unk2c = 1;
                    }
                }
                break;
            case 2:
                if (((Z6_Game*)m_game)->FUN_004368a0(this)) {
                    quit = 1;
                }
                break;
            }
        }
        NetRace* m = net;
        if ((unsigned)m->GetUnk844() > 2) {
            if (!m->m_host) {
                m->SendState(3);
            }
            m->ServiceNetwork();
            m_selected = 1;
            m_game->Play(1);
            finished = 1;
            continue;
        }
        int dir = 0;
        if (DAT_0057ba50) {
            dir = 1;
        }
        if (DAT_0057ba4e) {
            dir--;
        }
        if (dir != 0) {
            m_game->Play(0);
            m_selected += dir;
            if (m_selected < 0) {
                m_selected = 2;
            } else if (m_selected >= 3) {
                m_selected = 0;
            }
            float cx = m_items[m_selected]->GetTopLeft().x +
                       (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f;
            float cy = m_items[m_selected]->GetTopLeft().y +
                       (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f;
            SetCursorPos((int)(cx * DAT_0056aea8), (int)(cy * DAT_0056aeac));
        }
        int hit = HitTest(mouse, 0, 2);
        if (hit != -1 && dir == 0) {
            if (m_selected != hit) {
                m_game->Play(1);
            }
            m_selected = hit;
        }
        if (DAT_0057ba20) {
            if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                if (((Z6_Game*)m_game)->FUN_004368a0(this)) {
                    quit = 1;
                }
            }
            if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                if (unk2c == 0) {
                    ((MenuManager*)((BYTE*)DAT_0056b0c8 + 0x2df4))->Run((MenuGame*)DAT_0056b0c8, net);
                }
            }
            if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                if (unk2c == 0) {
                    NetRace* m2 = net;
                    if (m2->m_host) {
                        if (count > 1) {
                            m2->FUN_00482510();
                            m2->SendState(3);
                        } else {
                            m_game->Play(3);
                            goto skip;
                        }
                    } else if (DAT_0056b0b8->GetStatus() == 1 && entered) {
                        m2->SendState(2);
                    }
                    unk2c = 1;
                }
            }
        }
    skip:
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
        Slot3(0);
    }
    FUN_00438d00();
    int r;
    if (!quit && net->IsJoined()) {
        r = (joined != 0) + 1;
    } else {
        r = -1;
        m_game->Play(2);
    }
    return r;
}

// NONMATCHING: RALLYTROPHY 0x43ad80
// 532/532 bytes, 15 differences: second-panel coordinate stores versus material loads.
// Draws two textured panels; the separate Layout function builds hit boxes.
void Screen1378::BuildRects()
{
    HudPoint uv[4];
    HudPoint pos[4];
    Color white;
    float x = DAT_0056aea8 * 47.0f;
    float y = DAT_0056aeac * 355.0f;
    uv[0].x = uv[3].x = 0.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[0].x = pos[3].x = x;
    pos[0].y = pos[1].y = y;
    pos[1].x = pos[2].x = DAT_0056aea8 * 896.0f + x;
    pos[2].y = pos[3].y = DAT_0056aeac * 256.0f + y;
    white.value = -1;
    DAT_0056b0ac->vf35(unk24);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, white, 1.0f, 1.0f);
    float w = DAT_0056aea8 * 512.0f;
    float h = DAT_0056aeac * 256.0f;
    float x2 = DAT_0056aea8 * 432.0f;
    float y2 = DAT_0056aeac * 205.0f;
    pos[0].x = pos[3].x = x2;
    pos[0].y = pos[1].y = y2;
    pos[1].x = pos[2].x = w + x2;
    pos[2].y = pos[3].y = h + y2;
    DAT_0056b0ac->vf35(unk28);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, white, 1.0f, 1.0f);
}

struct LL_Point {
    LL_Point() {}
    LL_Point(const LL_Point& other);
    ~LL_Point() {}
    float x, y;
};
class LL_Rect {
public:
    LL_Rect(LL_Point upper, HudPoint lower);
    float fields[5];
};
struct LL_Vec {
    std::allocator<UIRect*> alloc;
    UIRect** first;
    UIRect** last;
    UIRect** end;
    void Insert(UIRect** pos, unsigned int n, UIRect* const& value);
    void InsertOne(UIRect** pos, UIRect* const& value) { Insert(pos, 1, value); }
    void PushBack(UIRect* const& value) { InsertOne(last, value); }
};
#pragma comment(linker, "/alternatename:??0LL_Point@@QAE@ABU0@@Z=?Assign@Pair@@QAEPAU1@PBU1@@Z")
#pragma comment(linker, "/alternatename:??0LL_Rect@@QAE@ULL_Point@@VHudPoint@@@Z=??0A_Rect@@QAE@VHudPoint@@0@Z")
#pragma comment(linker, "/alternatename:?Insert@LL_Vec@@QAEXPAPAVUIRect@@IABQAV2@@Z=?insert@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXPAPAXIABQAX@Z")
typedef char LL_VectorLayoutCheck[sizeof(LL_Vec) == sizeof(std::vector<UIRect*>) ? 1 : -1];
static float LL_Y = 262.0f;
static float LL_Margin = 20.0f;
static float LL_X = 54.0f;
static float LL_Step = 42.0f;
static float LL_BackY = 610.0f;
static float LL_BackMargin = 10.0f;

/* FUNCTION: RALLYTROPHY 0x43afa0 */
// Exact 557-byte match / 34 relocations; 12 calls and 18 float operands audited.
// Evidence: docs/analysis/lobby-hit-boxes.md.
void Screen1378::Layout()
{
    LL_Point top, bottom;
    float y = LL_Y - LL_Margin;
    top.x = LL_X;
    top.y = y;
    bottom.x = LL_X + 420.0f;
    bottom.y = LL_Step + y;
    UIRect* r = (UIRect*)new LL_Rect(top, *(HudPoint*)&bottom);
    ((LL_Vec*)&m_items)->PushBack(r);
    top.x = LL_X;
    bottom.x = LL_X + 420.0f;
    top.y = LL_Step + y;
    bottom.y = (LL_Step + LL_Step) + y;
    r = (UIRect*)new LL_Rect(top, *(HudPoint*)&bottom);
    ((LL_Vec*)&m_items)->PushBack(r);
    top.x = LL_X;
    bottom.x = LL_X + 200.0f;
    top.y = LL_BackY - LL_BackMargin;
    bottom.y = LL_BackY + 42.0f;
    r = (UIRect*)new LL_Rect(top, *(HudPoint*)&bottom);
    ((LL_Vec*)&m_items)->PushBack(r);
}

// NONMATCHING: RALLYTROPHY 0x43b1d0
// Semantics complete (header rows, player list with car icons, session options, status text, three buttons, cursor);
// not compared at instruction level: the original builds each text line with inlined string appends (assign, ": ",
// value) whose exact inlining and local layout were not reproduced.
// original: FUN_0043b1d0 (vtable slot 3, Draw)
void Screen1378::Slot3(int noPresent)
{
    MenuGame* game = m_game;
    TextRenderer* big = *(TextRenderer**)((BYTE*)game + 0x1390bc);
    TextRenderer* text = game->text;
    float sx = DAT_0056aea8, sy = DAT_0056aeac;
    float colA = sx * 88.0f, colB = sx * 345.0f, colC = sx * 630.0f, colD = sx * 880.0f, colE = sx * 920.0f;
    float colF = sx * 450.0f;
    float rowTop = sy * 362.0f, lineH = sy * 32.0f, rowH = sy * 20.0f, rowBot = sy * 585.0f, rowY = sy * 210.0f;
    float x0 = sx * 54.0f;
    std::string line;
    big->scaleX = sx;
    big->scaleY = sy;
    game->UpdateVideo();
    if (DAT_0056b0ac->BeginScene()) {
        DAT_0056b0ac->vf38(0);
        game->DrawVideo(0);
        text->scaleX = sx * 2.0f;
        text->scaleY = sy * 2.0f;
        text->color = DAT_00508bfc;
        text->Print(DAT_0056b0ac, 54.0f * sx, 142.0f * sy, FUN_00405380(0x12d), 0, 1.0f, 1.0f);
        text->scaleX = sx;
        text->scaleY = sy;
        BuildRects();
        Color white(0xffffffff);
        text->color = white.value;
        text->Print(DAT_0056b0ac, colA, rowTop, FUN_00405380(0x94), 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, colB, rowTop, FUN_00405380(0x61), 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, colC, rowTop, FUN_00405380(0x132), 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, colE - text->GetWidth(FUN_00405380(0x131), 0), rowTop, FUN_00405380(0x131), 0, 1.0f,
                    1.0f);
        count = 0;
        if (DAT_0056b0b8->GetStatus() == 1) {
            // Snapshot the players under the session lock.
            NetRace* race = net;
            race->Lock();
            count = race->m_players.size();
            for (int i = 0; i < count; i++) {
                Player* p = race->m_players[i];
                strcpy(entries[i].name, p->name);
                entries[i].unk20 = p->unk20;
                entries[i].state = p->state;
                entries[i].ping = race->GetLatency(p);
                if (entries[i].state == 2) {
                    entries[i].unk28 = p->unk2c;
                }
            }
            race->Unlock();
            big->SetColor(ColorArg(0xff000000));
            float ly = rowTop + lineH;
            big->scaleX = sx * 1.5f;
            big->scaleY = sy * 1.5f;
            for (int k = 0; k < count; k++) {
                LobbyEntry* e = &entries[k];
                C4_Icon* icon = &((C4_Icon*)DAT_0056b0c8)[e->unk20 + 0xd3];
                DAT_0056b0ac->vf35(icon->sprite);
                HudPoint uv[4];
                HudPoint pos[4];
                Color col(0xffffffff);
                uv[0].x = uv[3].x = icon->left;
                uv[0].y = uv[1].y = icon->top;
                uv[1].x = uv[2].x = icon->right;
                uv[2].y = uv[3].y = icon->bottom;
                float x1 = x0 + sx * icon->width * 0.5f;
                float y1 = ly + sy * icon->height * 0.5f;
                pos[0].x = pos[3].x = x0;
                pos[0].y = pos[1].y = ly;
                pos[1].x = pos[2].x = x1;
                pos[2].y = pos[3].y = y1;
                ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, col, 1.0f, 1.0f);
                big->Print(DAT_0056b0ac, colA, ly, e->name, 0, 1.0f, 1.0f);
                big->Printf(DAT_0056b0ac, colD, ly, "%i", e->ping);
                if (e->state == 2) {
                    big->Print(DAT_0056b0ac, colB, ly, FUN_00405380(e->unk28 + 0x138), 0, 1.0f, 1.0f);
                    big->Print(DAT_0056b0ac, colC, ly, FUN_00405380(0x130), 0, 1.0f, 1.0f);
                } else {
                    big->Print(DAT_0056b0ac, colC, ly, FUN_00405380(0x12f), 0, 1.0f, 1.0f);
                }
                ly += lineH;
            }
            big->scaleX = sx;
        big->scaleY = sy;
        // Session description, one "label: value" line per row.
        T_56b0b8* session = DAT_0056b0b8;
        float y = rowY;
        line = FUN_00405380(0x19);
        line += C4_SEP;
        line += FUN_00405380(session->m_rallyInfo[0] + 0xb4);
        if (session->m_rallyInfo[2]) {
            line += FUN_00405380(0x2a);
            line += C4_SPACE;
        }
        line += FUN_00405380(DAT_004f8d14[session->m_rallyInfo[3]]);
        big->Print(DAT_0056b0ac, colF, y, line.c_str(), 0, 1.0f, 1.0f);
        y = rowY + rowH;
        line = FUN_00405380(0x1a);
        line += C4_SEP;
        char buf[16];
        itoa(session->m_rallyInfo[1], buf, 10);
        line += buf;
        big->Print(DAT_0056b0ac, colF, y, line.c_str(), 0, 1.0f, 1.0f);
        y += rowH;
        line = FUN_00405380(0x11a);
        line += C4_SEP;
        line += FUN_00405380(DAT_004f8d24[session->m_ghostMode]);
        big->Print(DAT_0056b0ac, colF, y, line.c_str(), 0, 1.0f, 1.0f);
        y += rowH;
        line = FUN_00405380(0x119);
        line += C4_SEP;
        line += FUN_00405380(DAT_004f8d2c[session->m_damage]);
        big->Print(DAT_0056b0ac, colF, y, line.c_str(), 0, 1.0f, 1.0f);
        y += rowH;
        line = FUN_00405380(0x11c);
        line += C4_SEP;
        line += FUN_00405380(DAT_004f8d34[session->m_cars]);
        big->Print(DAT_0056b0ac, colF, y, line.c_str(), 0, 1.0f, 1.0f);
        y += rowH;
        line = FUN_00405380(0x11b);
        line += C4_SEP;
        line += DAT_004f8d44[session->m_option4];
        big->Print(DAT_0056b0ac, colF, y, line.c_str(), 0, 1.0f, 1.0f);
        }

        // Connection status, centred.
        const char* status = 0;
        switch (DAT_0056b0b8->GetStatus()) {
        case -1:
            status = FUN_00405380(0x121);
            break;
        case 0:
            status = FUN_00405380(0x120);
            break;
        case 1:
            if (!DAT_0056b0b8->m_joined) {
                status = FUN_00405380(0x122);
            } else if (net->m_host == 0) {
                status = FUN_00405380(0x11e);
            } else {
                status = FUN_00405380(0x11d);
            }
            break;
        }
        text->Print(DAT_0056b0ac, sx * 512.0f - text->GetWidth(status, 0) * 0.5f, rowBot, status, 0, 1.0f, 1.0f);

        // The three buttons.
        ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 2, 1, 1, 0, -1.0f);
        float by = sy * 262.0f;
        for (int j = 0; j < 3; j++) {
            Color col(0);
            col.value = (j == m_selected) ? DAT_00508c0c : DAT_00508c04;
            text->color = col.value;
            const char* label;
            switch (j) {
            case 0:
                label = FUN_00405380(0x2c);
                break;
            case 1:
                if (net->m_host == 0) {
                    label = FUN_00405380(0x17 + (unk2c ? 0x118 : 0));
                } else {
                    label = FUN_00405380(0x7d + (unk2c ? 0xb2 : 0));
                }
                break;
            default:
                by = 610.0f * sy;
                label = FUN_00405380(0x30);
                break;
            }
            text->Print(DAT_0056b0ac, x0, by, label, 0, 1.0f, 1.0f);
            by += sy * 42.0f;
        }
        DrawCursor(*((MouseInput*)DAT_0056b0c0)->GetPosition());
        DAT_0056b0ac->EndScene();
        if (noPresent != 1) {
            DAT_0056b0ac->Present();
        }
    }
    big->scaleX = sx;
    big->scaleY = sy;
}

// FUNCTION: RALLYTROPHY 0x43c560
// SYMBOL: _$E23
typedef void Generated_43c560();

// FUNCTION: RALLYTROPHY 0x43c580
// SYMBOL: _$E22
typedef void Generated_43c580();
