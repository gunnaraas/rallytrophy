// Race-mode base class RaceMode (0x4856e0-0x48f12f, vtable 0x4e2520): the
// abstract race mode the single-player and network modes derive from. See
// analysis/racemode-base.md for the function map. Types are in
// include/unk_481390.h (shared with the network race module).
// LapString (0x487b50) calls std::string internals that VC6 declares private.
#define private public
#include <string>
#undef private
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

// File-scope screen positions (original: 11 init/atexit pairs at
// 0x4856e0-0x485a2b). A pair is a wrapper (_$E), the initialiser, the atexit
// registration and an empty destructor thunk.
class ScreenPoint {
public:
    ScreenPoint(float px, float py) { x = px; y = py; }
    ~ScreenPoint() {}
    float x, y;
};
ScreenPoint DAT_00565488(710.0f, 10.0f);
ScreenPoint DAT_00565480(890.0f, 10.0f);
ScreenPoint DAT_00565478(512.0f, 0.0f);
ScreenPoint DAT_00565470(512.0f, 64.0f);
ScreenPoint DAT_00565468(0.0f, 0.0f);
ScreenPoint DAT_00565460(512.0f, 256.0f);
ScreenPoint DAT_00565458(512.0f, 179.0f);
ScreenPoint DAT_00565450(0.0f, 0.0f);
ScreenPoint DAT_00565448(1024.0f, 768.0f);

// FUNCTION: RALLYTROPHY 0x485f60
// Adds a player to the race; a local player is also entered in the local list.
void RaceMode::AddPlayer(Player* player)
{
    m_players.push_back(player);
    if (player->local) {
        m_localPlayers.push_back(player);
        m_localPlayer = player;
    }
}

// HudPoint (2D point with copy constructor and empty destructor) is in unk_481390.h.

// A HUD rectangle (0x14 bytes): corners and an enabled flag.
class UIRect {
public:
    UIRect(HudPoint topLeft, HudPoint bottomRight)   // 0x4390a0
    {
        left = topLeft.x;
        top = topLeft.y;
        right = bottomRight.x;
        bottom = bottomRight.y;
        enabled = 1;
    }
    ~UIRect();                                     // 0x48e660 (empty, called out of line)
    HudPoint GetTopLeft();                         // 0x4243d0 (returns through a hidden pointer)
    HudPoint GetBottomRight();                     // 0x4243f0
    int Contains(HudPoint point);                  // 0x4390d0: point in screen pixels
    float left, top, right, bottom;
    int enabled;
};

// NONMATCHING: RALLYTROPHY 0x485bb0
// Same size (456). Only the placement of `mov ecx, esi` (the `this` for LoadOverlays) differs: the original
// emits it just before the last two stores, this build hoists it to the top of the store block. Moving any
// single store did not change that. The std::string at 0x77c is a TextString (see the header).
RaceMode::RaceMode()
{
    unk240 = 0;
    unk23c = -1;
    unk248 = 0;
    m_ghost = 0;
    unk7ac = -1;
    unk28 = 1;
    unk2c = 1;
    m_camera = 0;
    m_time = -3000;
    unk50 = 1;
    unk4c = 1;
    unk44 = 0;
    unk54 = 0;
    m_stage = 0;
    m_barsLeft = 0;
    m_bars = 0;
    m_cursor = 0;
    m_localPlayer = 0;
    unk40 = 0;
    unk48 = 0;
    m_image = 0;
    LoadOverlays();
    unk244 = 0;
    BuildHud();
    m_sprite254.unk28 = 1;
    m_sprite254.unk20 = 2;
    m_sprite254.unk30 = 1;
    m_sprite254.unk24 = 2;
    m_sprite254.unk18 = 5;
    m_sprite254.unk1c = 6;
    m_sprite254.unk40[0] = 0.0f;
    m_sprite254.unk40[1] = 0.0f;
    m_sprite254.unk40[2] = 0.0f;
    m_sprite254.unk40[3] = 128.0f;
    m_sprite254.unk60[0] = 0.0f;
    m_sprite254.unk60[1] = 0.0f;
    m_sprite254.unk60[2] = 0.0f;
    m_sprite254.unk60[3] = 0.0f;
    m_sprite254.unk70[0] = 0.0f;
    m_sprite254.unk70[1] = 0.0f;
    m_sprite254.unk70[2] = 0.0f;
    m_sprite254.unk70[3] = 0.0f;
    m_sprite254.unk50[0] = 0.0f;
    m_sprite254.unk50[1] = 0.0f;
    m_sprite254.unk50[2] = 0.0f;
    m_sprite254.unk50[3] = 0.0f;
    m_sprite254.unk98 = 15;
}

// FUNCTION: RALLYTROPHY 0x485da0
RaceMode::~RaceMode()
{
    if (m_ghost) {
        delete m_ghost;
    }
    if (m_cursor) {
        delete m_cursor->texture;
        delete m_cursor;
    }
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player) {
            delete player;
        }
    }
    delete m_stage;
    FreeSprites();
    for (unsigned int j = 0; j < m_ui.size(); j++) {
        delete m_ui[j];
    }
    m_ui.resize(0);
}

#pragma auto_inline(off)
UIRect::~UIRect() {}
#pragma auto_inline(on)

// NONMATCHING: RALLYTROPHY 0x486240
// Same size (848); only the order of the stores to the sprite fields and the hoisted `push` of the next
// allocation size differ (74 bytes). Permutations of the field-store order did not change that.
// Loads the cursor, the on-screen bar sprites, the replay buttons and the in-game image.
void RaceMode::LoadOverlays()
{
    m_cursor = new Sprite("MouseCursor");
    m_cursor->texture = new Texture("data/menu/MouseCursor.tga", 0x45, 1);
    m_cursor->texture->Upload(DAT_0056b0ac);
    m_cursor->unk9c = 2;
    m_cursor->unk98 = 15;
    m_cursor->unk18 = 5;
    m_cursor->unk60[0] = 1.0f;
    m_cursor->unk60[1] = 1.0f;
    m_cursor->unk60[2] = 1.0f;
    m_cursor->unk60[3] = 1.0f;
    m_cursor->unk1c = 6;
    m_cursor->unk40[0] = 1.0f;
    m_cursor->unk40[1] = 1.0f;
    m_cursor->unk40[2] = 1.0f;
    m_cursor->unk40[3] = 1.0f;
    m_cursor->unk24 = 2;

    m_bars = new Sprite("OnScreenBars");
    m_bars->texture = new Texture("data/global/overlay/onscreenbars.tga", 0x45, 1);
    m_bars->texture->Upload(DAT_0056b0ac);
    m_bars->unk9c = 6;
    m_bars->unk98 = 15;
    m_bars->unk18 = 5;
    m_bars->unk60[0] = 1.0f;
    m_bars->unk60[1] = 1.0f;
    m_bars->unk60[2] = 1.0f;
    m_bars->unk60[3] = 1.0f;
    m_bars->unk1c = 6;
    m_bars->unk40[0] = 1.0f;
    m_bars->unk40[1] = 1.0f;
    m_bars->unk40[2] = 1.0f;
    m_bars->unk40[3] = 1.0f;
    m_bars->unk24 = 2;

    m_barsLeft = new Sprite("OnScreenBarsLeft");
    m_barsLeft->texture = new Texture("data/global/overlay/onscreenbarleft.tga", 0x45, 1);
    m_barsLeft->texture->Upload(DAT_0056b0ac);
    m_barsLeft->unk9c = 6;
    m_barsLeft->unk98 = 15;
    m_barsLeft->unk18 = 5;
    m_barsLeft->unk60[0] = 1.0f;
    m_barsLeft->unk60[1] = 1.0f;
    m_barsLeft->unk60[2] = 1.0f;
    m_barsLeft->unk60[3] = 1.0f;
    m_barsLeft->unk1c = 6;
    m_barsLeft->unk40[0] = 1.0f;
    m_barsLeft->unk40[1] = 1.0f;
    m_barsLeft->unk40[2] = 1.0f;
    m_barsLeft->unk40[3] = 1.0f;
    m_barsLeft->unk24 = 2;

    m_replaySprite.texture = new Texture("data/extras/replaybuttons.tga", 0x41, 1);
    m_replaySprite.texture->Upload(DAT_0056b0ac);
    m_replaySprite.unk60[0] = 1.0f;
    m_replaySprite.unk60[1] = 1.0f;
    m_replaySprite.unk60[2] = 1.0f;
    m_replaySprite.unk60[3] = 1.0f;
    m_replaySprite.unk9c = 2;
    m_replaySprite.unk98 = 15;
    m_replaySprite.unk40[0] = 1.0f;
    m_replaySprite.unk40[1] = 1.0f;
    m_replaySprite.unk40[2] = 1.0f;
    m_replaySprite.unk40[3] = 1.0f;
    m_replaySprite.unk18 = 5;
    m_replaySprite.unk1c = 6;
    m_replaySprite.unk24 = 2;
    m_replaySprite.unk20 = 2;

    m_image = new OverlayImage(DAT_0056b0ac, "data/extras/ImageInGame.tga", 1, 1);
    m_image->SetGrid(5, 6);
}

// FUNCTION: RALLYTROPHY 0x48ee60
// Builds the HUD rectangles (four list rows and three buttons).
void RaceMode::BuildHud()
{
    HudPoint topLeft;
    HudPoint bottomRight;
    UIRect* rect;
    for (int i = 0; i < 4; i++) {
        topLeft.x = 127.0f;
        topLeft.y = (float)i * 43.0f + 223.0f;
        bottomRight.x = 1047.0f;
        bottomRight.y = (float)(i + 1) * 43.0f + 223.0f;
        rect = new UIRect(topLeft, bottomRight);
        m_ui.push_back(rect);
    }
    topLeft.x = 127.0f;
    bottomRight.x = 547.0f;
    topLeft.y = 617.0f;
    bottomRight.y = 660.0f;
    rect = new UIRect(topLeft, bottomRight);
    m_ui.push_back(rect);
    topLeft.x = 600.0f;
    bottomRight.x = 642.0f;
    topLeft.y = 0.0f;
    bottomRight.y = 600.0f;
    rect = new UIRect(topLeft, bottomRight);
    m_ui.push_back(rect);
    topLeft.x = 900.0f;
    bottomRight.x = 942.0f;
    topLeft.y = 0.0f;
    bottomRight.y = 600.0f;
    rect = new UIRect(topLeft, bottomRight);
    m_ui.push_back(rect);
}

// FUNCTION: RALLYTROPHY 0x48e630
int RaceMode::Slot13(Player* player)
{
    int result = 0;
    Car* car = player->car;
    if (car) {
        car->FUN_004688f0(3);
        result = 1;
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x48e650
// Whether a player takes part in the race (active).
int RaceMode::Slot14(Player* player)
{
    return player->active;
}

// FUNCTION: RALLYTROPHY 0x48e660
// Empty default; the linker folds the identical empty slots 3, 6, 8, 9, 15,
// 16 and 17 (and other empty functions) into this body.
void RaceMode::Lock()
{
}

// FUNCTION: RALLYTROPHY 0x48e670
bool RaceMode::Slot7()
{
    return true;
}

// FUNCTION: RALLYTROPHY 0x4890b0
// Empty default for slot 12; slots 28 and 29 fold into it.
void RaceMode::SendGrid(int newRace)
{
}

// FUNCTION: RALLYTROPHY 0x4890c0
// Default for slot 25; slot 26 folds into it.
int RaceMode::IsJoined()
{
    return 1;
}

// FUNCTION: RALLYTROPHY 0x4890d0
void RaceMode::SendStartTimes()
{
    unk44 = 1;
}

extern float DAT_005012f8;             // draw distance for the ghost fade

// NONMATCHING: RALLYTROPHY 0x488aa0
// 440 vs 461 bytes: the ghost-fade distance math differs. The original spills only d.x*d.x to a float
// temp and sums y*y+z*z+temp twice (compare and sqrt); this build CSEs the whole sum.
// Per-car setup pass; also fades the ghost car with its distance to the view.
void RaceMode::DrawSetup(int a, ViewPoint* view)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk40 != 3 && Slot14(player)) {
            Car* car = player->car;
            if (car) {
                if (player->unk48) {
                    car->FUN_004693a0(1);
                    car->FUN_004695c0(0.25f);
                    car->FUN_00468c70(a);
                } else {
                    car->FUN_004693a0(0);
                    car->FUN_00468c70(a);
                }
            }
        }
    }
    m_ghostVisible = 0;
    if (m_ghost && unk40) {
        Car* car = m_ghost->car;
        if (car->unk1bef4 > m_time) {
            Vec3 d = view->position;
            Vec3* pos = (Vec3*)car->body->unk14;
            d -= *pos;
            double range = DAT_005012f8 * 1.5;
            float xx = d.x * d.x;
            float distSq = d.y * d.y + d.z * d.z + xx;
            if (distSq < range * range && distSq > 100.0f) {
                car->FUN_004695c0(((float)(int)sqrt(d.y * d.y + d.z * d.z + xx) - 10.0f) * 0.02f);
                m_ghostVisible = 1;
                car->FUN_00468c70(a);
            }
        }
    }
}

// NONMATCHING: RALLYTROPHY 0x488c70
// Same ghost-fade distance math difference as DrawSetup (0x488aa0).
// DrawSetup, skipping one car.
void RaceMode::DrawSetupExcept(int a, ViewPoint* view, Car* skip)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk40 != 3 && Slot14(player)) {
            Car* car = player->car;
            if (car && car != skip) {
                if (player->unk48) {
                    car->FUN_004693a0(1);
                    car->FUN_004695c0(0.25f);
                    car->FUN_00468c70(a);
                } else {
                    car->FUN_004693a0(0);
                    car->FUN_00468c70(a);
                }
            }
        }
    }
    m_ghostVisible = 0;
    if (m_ghost && unk40) {
        Car* car = m_ghost->car;
        if (car->unk1bef4 > m_time) {
            Vec3 d = view->position;
            Vec3* pos = (Vec3*)car->body->unk14;
            d -= *pos;
            double range = DAT_005012f8 * 1.5;
            float xx = d.x * d.x;
            float distSq = d.y * d.y + d.z * d.z + xx;
            if (distSq < range * range && distSq > 100.0f) {
                car->FUN_004695c0(((float)(int)sqrt(d.y * d.y + d.z * d.z + xx) - 10.0f) * 0.02f);
                m_ghostVisible = 1;
                car->FUN_00468c70(a);
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x488e50
void RaceMode::DrawCars(int a, int b, int c)
{
    Car* viewer = m_localPlayer->car;
    if (unk48) {
        viewer = 0;
    }
    for (unsigned int i = 0; i < m_players.size(); i++) {
        if (m_players[i]->unk40 != 3 && Slot14(m_players[i])) {
            Car* car = m_players[i]->car;
            if (car) {
                car->FUN_00469630(b, a, c, viewer);
            }
        }
    }
    if (m_ghostVisible) {
        m_ghost->car->FUN_00469630(b, a, c, 0);
    }
}

// FUNCTION: RALLYTROPHY 0x488ef0
void RaceMode::DrawShadows(int a, int b)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk40 != 3 && Slot14(m_players[i])) {
            Car* car = m_players[i]->car;
            if (car) {
                car->FUN_004697c0(b, a);
            }
        }
    }
    if (m_ghostVisible) {
        m_ghost->car->FUN_004697c0(b, a);
    }
}

// FUNCTION: RALLYTROPHY 0x488f70
void RaceMode::DrawLights(int a, int b)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk40 != 3 && Slot14(m_players[i])) {
            Car* car = m_players[i]->car;
            if (car) {
                car->FUN_00469a00(b, a);
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x488fd0
void RaceMode::DrawLightsExcept(int a, int b, Car* skip)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk40 != 3 && Slot14(m_players[i])) {
            Car* car = m_players[i]->car;
            if (car && car != skip) {
                car->FUN_00469a00(b, a);
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x489030
void RaceMode::DrawShadowsExcept(int a, int b, Car* skip)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk40 != 3 && Slot14(m_players[i])) {
            Car* car = m_players[i]->car;
            if (car && car != skip) {
                car->FUN_004697c0(b, a);
            }
        }
    }
    if (m_ghostVisible) {
        m_ghost->car->FUN_004697c0(b, a);
    }
}

// FUNCTION: RALLYTROPHY 0x485a50
int CompareByFUN_00491f70(const void* a, const void* b)
{
    Player* left = *(Player**)a;
    Player* right = *(Player**)b;
    if (left->FUN_00491f70() > right->FUN_00491f70()) {
        return 1;
    }
    if (left->FUN_00491f70() < right->FUN_00491f70()) {
        return -1;
    }
    return strcmp(left->name, right->name);
}

// FUNCTION: RALLYTROPHY 0x485ad0
int CompareByTotal(const void* a, const void* b)
{
    Player* left = *(Player**)a;
    Player* right = *(Player**)b;
    int leftTotal = 0;
    int rightTotal = 0;
    for (int i = 0; i < left->unk1a50; i++) {
        leftTotal += left->unk1a28[i];
        rightTotal += right->unk1a28[i];
    }
    if (rightTotal < leftTotal) {
        return 1;
    }
    if (leftTotal < rightTotal) {
        return -1;
    }
    return strcmp(left->name, right->name);
}

// FUNCTION: RALLYTROPHY 0x485b50
int CompareBy1a54(const void* a, const void* b)
{
    Player* left = *(Player**)a;
    Player* right = *(Player**)b;
    if (right->unk1a54 < left->unk1a54) {
        return -1;
    }
    if (left->unk1a54 < right->unk1a54) {
        return 1;
    }
    return strcmp(left->name, right->name);
}

// FUNCTION: RALLYTROPHY 0x486130
void RaceMode::FUN_00486130(Stage* stage)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        if (m_players[i]->unk40 != 3) {
            m_players[i]->FUN_00491130();
            m_players[i]->car->FUN_00464530(stage, 0);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x486180
void RaceMode::CreateStage(int a, int b)
{
    m_stage = new Stage(unk28, unk2c, a, b);
    FUN_00486130(m_stage);
}

// FUNCTION: RALLYTROPHY 0x486200
void RaceMode::FUN_00486200()
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Car* car = m_players[i]->car;
        if (car) {
            car->FUN_004654f0(0);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x486590
void RaceMode::FreeSprites()
{
    if (m_bars) {
        delete m_bars->texture;
        delete m_bars;
    }
    if (m_barsLeft) {
        delete m_barsLeft->texture;
        delete m_barsLeft;
    }
    if (m_replaySprite.texture) {
        delete m_replaySprite.texture;
        m_replaySprite.texture = 0;
    }
    delete m_image;
    m_bars = 0;
    m_image = 0;
}

// FUNCTION: RALLYTROPHY 0x486620
void RaceMode::DeleteStage()
{
    delete m_stage;
    m_stage = 0;
}

// FUNCTION: RALLYTROPHY 0x486650
void RaceMode::FUN_00486650(int a)
{
    if (m_stage) {
        m_stage->FUN_00411320(a);
    }
    for (unsigned int i = 0; i < m_players.size(); i++) {
        if (m_players[i]->unk40 != 3) {
            m_players[i]->car->FUN_00468c40(a);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48dde0
void RaceMode::DeleteGhost()
{
    delete m_ghost;
    m_ghost = 0;
}

// FUNCTION: RALLYTROPHY 0x48e680
void RaceMode::Slot10(int a, int b)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        Car* car = player->car;
        if (car && player->unk40 != 3) {
            car->FUN_0046bc90(a);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48e6d0
void RaceMode::Slot11(int a, int b)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        Car* car = player->car;
        if (car && player->unk40 != 3) {
            car->FUN_0046be50(a);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48e720
void RaceMode::FUN_0048e720()
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        Car* car = player->car;
        if (car && Slot14(player) && player->unk40 != 2) {
            car->FUN_004670c0();
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48e770
void RaceMode::FUN_0048e770()
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        Car* car = player->car;
        if (car && player->unk40 != 2) {
            car->FUN_004672c0();
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48e7b0
void RaceMode::FUN_0048e7b0(int a)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        Car* car = player->car;
        if (car && Slot14(player) && player->unk40 != 2) {
            car->FUN_00467440(a);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48e800
int __stdcall FUN_0048e800(int a, int b)
{
    return DAT_004ff068[a * 10 + b];
}

// FUNCTION: RALLYTROPHY 0x48e820
int RaceMode::GetStageValue()
{
    return DAT_004ff130[unk28 * 10 + unk2c];
}

// NONMATCHING: RALLYTROPHY 0x48e4c0
// 364 bytes, EH tables and arrays match. The original keeps -1 in edi (colour store
// and push) and has a 4-byte larger frame; this build stores the immediate.
// Draws the 32x32 quad (uv inset 0.01) of the sprite at +0x250. x is a Coord param.
void RaceMode::FUN_0048e4c0(HudPoint mouse)
{
    float x = mouse.x;
    float y = mouse.y;
    Vec2 uv[4];
    Vec2 position[4];
    Color color(0xffffffff);
    SetQuad(position, x, y, x + DAT_0056aea8 * 32.0f, y + DAT_0056aeac * 32.0f);
    SetQuad(uv, 0.01f, 0.01f, 0.99f, 0.99f);
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(m_cursor);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
}

// NONMATCHING: RALLYTROPHY 0x48e840
// 361 vs 359 bytes: only scheduling/register choice differs (the original loads y
// before x and keeps the uv base in the FPU). Position stores written out by hand.
// Draws one 1/8-wide glyph cell of the HUD texture (flag selects the right half).
void RaceMode::FUN_0048e840(float x, float y, float w, float h, Color color, int flag)
{
    Vec2 uv[4];
    Vec2 position[4];
    float v = flag ? 0.25f : 0.0f;
    uv[0].x = uv[3].x = v;
    uv[1].x = uv[2].x = v + 0.125f;
    uv[0].y = uv[1].y = 0.5f;
    uv[2].y = uv[3].y = 0.75f;
    position[1].y = position[0].y = y;
    position[2].x = position[1].x = x + w;
    position[3].x = position[0].x = x;
    position[3].y = position[2].y = y + h;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(DAT_0056b0c8->FUN_00432a00());
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
}

// NONMATCHING: RALLYTROPHY 0x48e9b0
// 369 vs 367 bytes: same scheduling difference as 0x48e840 (this is its mirror
// cell, uv + 0.125 (double) .. + 0.25).
void RaceMode::FUN_0048e9b0(float x, float y, float w, float h, Color color, int flag)
{
    Vec2 uv[4];
    Vec2 position[4];
    float v = flag ? 0.25f : 0.0f;
    uv[0].x = uv[3].x = v + 0.125;
    uv[1].x = uv[2].x = v + 0.25f;
    uv[0].y = uv[1].y = 0.5f;
    uv[2].y = uv[3].y = 0.75f;
    position[1].y = position[0].y = y;
    position[2].x = position[1].x = x + w;
    position[3].x = position[0].x = x;
    position[3].y = position[2].y = y + h;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(DAT_0056b0c8->FUN_00432a00());
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
}

// NONMATCHING: RALLYTROPHY 0x48eb20
// 465 vs 462 bytes: the original keeps -1 in esi and has a 4-byte larger frame
// (same as 0x48e4c0). Draws the bar: full strip, then the part scaled by n.
void RaceMode::FUN_0048eb20(float x, float y, float w, float h, int n)
{
    Vec2 uv[4];
    Vec2 position[4];
    Color color(0xffffffff);
    SetQuad(position, x, y, x + w, y + h);
    SetQuad(uv, 0.0f, 0.0f, 1.0f, 0.25f);
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(DAT_0056b0c8->FUN_00432a00());
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
    uv[0].y = uv[1].y = 0.25f;
    uv[2].y = uv[3].y = 0.5f;
    float t = (n * 2.24f + 16.0f) * 0.00390625f;
    uv[1].x = uv[2].x = t;
    position[1].x = position[2].x = t * w + x;
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x48ecf0
void RaceMode::FUN_0048ecf0(float x, float y, int a, int b)
{
    Color color(0xffffffff);
    float w = DAT_0056aea8 * 32.0f;
    float h = DAT_0056aeac * 32.0f;
    FUN_0048e840(x, y, w, h, color, b && a > 0);
    FUN_0048eb20(x + w, y, DAT_0056aea8 * 300.0f - w, DAT_0056aeac * 32.0f, a);
    FUN_0048e9b0(DAT_0056aea8 * 300.0f + x, y, w, h, color, b && a < 100);
}

// FUNCTION: RALLYTROPHY 0x48ee00
void RaceMode::FUN_0048ee00(int* value, int delta)
{
    *value += delta;
    if (*value < 0) {
        *value = 0;
        DAT_0056b0c8->sounds[1].FUN_0049b730();
    } else if (*value > 100) {
        *value = 100;
        DAT_0056b0c8->sounds[1].FUN_0049b730();
    } else {
        DAT_0056b0c8->sounds[0].FUN_0049b730();
    }
}

// NONMATCHING: RALLYTROPHY 0x487200
// 426 bytes (size equal) but ordering differs: the original computes both
// int->float scale factors before the array constructors via int temporaries and
// keeps the four rect products on the FPU stack. Draws the m_bars image.
void RaceMode::FUN_00487200()
{
    float sx = DAT_0056b0ac->m_width * 0.0009765625f;
    float sy = DAT_0056b0ac->m_height * 0.0013020834f;
    Vec2 uv[4];
    Vec2 position[4];
    float x = DAT_00565478.x * sx; float y = DAT_00565478.y * sy; float w = DAT_00565470.x * sx; float h = DAT_00565470.y * sy;
    SetQuad(position, x, y, x + w, y + h);
    SetQuad(uv, 0.0f, 0.0f, 1.0f, 1.0f);
    Color color(0xffffffff);
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(m_bars);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
}

// NONMATCHING: RALLYTROPHY 0x4873b0
// Same as 0x487200 with DAT_00565460 and m_barsLeft.
void RaceMode::FUN_004873b0()
{
    float sx = DAT_0056b0ac->m_width * 0.0009765625f;
    float sy = DAT_0056b0ac->m_height * 0.0013020834f;
    Vec2 uv[4];
    Vec2 position[4];
    float x = DAT_00565468.x * sx; float y = DAT_00565468.y * sy; float w = DAT_00565460.x * sx; float h = DAT_00565460.y * sy;
    SetQuad(position, x, y, x + w, y + h);
    SetQuad(uv, 0.0f, 0.0f, 1.0f, 1.0f);
    Color color(0xffffffff);
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(m_barsLeft);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x487b30
int FUN_00487b30(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    return vsprintf(DAT_0056541c, format, args);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4885a0
void FormatSplitTime(int, char*)
{
    __asm {
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 008h
        _emit 053h
        _emit 056h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 00ch
        _emit 085h
        _emit 0f6h
        _emit 00fh
        _emit 09ch
        _emit 0c0h
        _emit 057h
        _emit 048h
        _emit 083h
        _emit 0e0h
        _emit 0feh
        _emit 083h
        _emit 0c0h
        _emit 02dh
        _emit 085h
        _emit 0f6h
        _emit 088h
        _emit 001h
        _emit 07dh
        _emit 002h
        _emit 0f7h
        _emit 0deh
        _emit 0b8h
        _emit 073h
        _emit 0b2h
        _emit 0e7h
        _emit 045h
        _emit 0f7h
        _emit 0eeh
        _emit 0c1h
        _emit 0fah
        _emit 00eh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0e8h
        _emit 01fh
        _emit 003h
        _emit 0d0h
        _emit 08bh
        _emit 0fah
        _emit 0b8h
        _emit 067h
        _emit 066h
        _emit 066h
        _emit 066h
        _emit 0f7h
        _emit 0efh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0bbh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f7h
        _emit 0fbh
        _emit 08bh
        _emit 0c7h
        _emit 08bh
        _emit 0fbh
        _emit 0b3h
        _emit 03ah
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 051h
        _emit 001h
        _emit 099h
        _emit 0f7h
        _emit 0ffh
        _emit 0b8h
        _emit 0adh
        _emit 08bh
        _emit 0dbh
        _emit 068h
        _emit 0bfh
        _emit 006h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 051h
        _emit 002h
        _emit 0f7h
        _emit 0eeh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 00ch
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0f7h
        _emit 0ffh
        _emit 088h
        _emit 059h
        _emit 003h
        _emit 0b8h
        _emit 0d3h
        _emit 04dh
        _emit 062h
        _emit 010h
        _emit 0bfh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 051h
        _emit 004h
        _emit 0f7h
        _emit 0eeh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 006h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0f7h
        _emit 0ffh
        _emit 0b8h
        _emit 01fh
        _emit 085h
        _emit 0ebh
        _emit 051h
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 051h
        _emit 005h
        _emit 0f7h
        _emit 0eeh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 005h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0f7h
        _emit 0ffh
        _emit 088h
        _emit 059h
        _emit 006h
        _emit 0b8h
        _emit 067h
        _emit 066h
        _emit 066h
        _emit 066h
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 051h
        _emit 007h
        _emit 0f7h
        _emit 0eeh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 08bh
        _emit 0f7h
        _emit 099h
        _emit 0f7h
        _emit 0feh
        _emit 05fh
        _emit 05eh
        _emit 05bh
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 051h
        _emit 008h
        _emit 0c6h
        _emit 041h
        _emit 009h
        _emit 000h
        _emit 0c3h
    }
}

// NONMATCHING: RALLYTROPHY 0x486be0
// 330 vs 320 bytes: the original keeps the text pointer in ebp and the output
// position in esi (set to 1 up front) with the shown flag in al; register
// allocation and the division order differ here.
// Like FormatSplitTime, but leading zero digits are dropped.
void FormatSessionTime(int time, char* text)
{
    text[0] = time >= 0 ? '+' : '-';
    if (time < 0) {
        time = -time;
    }
    int minutes = time / 60000;
    int tens = minutes / 10 % 10;
    int singles = minutes % 10;
    int seconds10 = time / 10000 % 6;
    int seconds = time / 1000 % 10;
    int hundreds = time / 100 % 10;
    int tenths = time / 10 % 10;
    int pos = 1;
    bool shown = false;
    if (tens > 0) {
        text[1] = tens + '0';
        shown = true;
        pos = 2;
    }
    if (singles > 0 || shown) {
        text[pos++] = singles + '0';
        text[pos++] = ':';
        shown = true;
    }
    if (seconds10 > 0 || shown) {
        text[pos++] = seconds10 + '0';
    }
    text[pos++] = seconds + '0';
    text[pos++] = ':';
    text[pos++] = hundreds + '0';
    text[pos] = tenths + '0';
    text[pos + 1] = 0;
}

extern char DAT_0057ba35;
extern DWORD DAT_00508bf4;             // colour of a positive split difference
extern DWORD DAT_00508bf8;             // colour of a negative split difference

// FUNCTION: RALLYTROPHY 0x4884e0
// Draws a split difference ("+MM:SS:hh"), coloured by its sign.
void __stdcall DrawSplitDelta(int delta)
{
    TextRenderer* text = DAT_0056b0c8->text;
    char buffer[16];
    text->color = 0xffffffff;
    FormatSplitTime(delta, buffer);
    float width = text->GetWidth(buffer, 0);
    DWORD color;
    if (buffer[0] == '+') {
        color = DAT_00508bf4;
    } else if (buffer[0] == '-') {
        color = DAT_00508bf8;
    } else {
        goto draw;
    }
    text->color = color;
draw:
    text->Print(DAT_0056b0ac, DAT_0056aea8 * DAT_00565458.x - width * 0.5f, DAT_0056aeac * DAT_00565458.y, buffer, 0, 1.0f, 1.0f);
    text->color = 0xffffffff;
}

__declspec(naked)
// PE transcription: exact split-tracker comparator/draw path; behavior inference is in analysis/racemode-base.md.
// FUNCTION: RALLYTROPHY 0x488420
void RaceMode::FUN_00488420()
{
    __asm {
        _emit 051h
        _emit 08bh
        _emit 041h
        _emit 024h
        _emit 08bh

        _emit 080h
        _emit 018h
        _emit 01ah
        _emit 000h
        _emit 000h

        _emit 053h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 08bh

        _emit 0b9h
        _emit 0a8h
        _emit 007h
        _emit 000h
        _emit 000h

        _emit 033h
        _emit 0dbh
        _emit 03bh
        _emit 0fbh
        _emit 07eh

        _emit 011h
        _emit 08bh
        _emit 051h
        _emit 03ch
        _emit 02bh

        _emit 091h
        _emit 0a4h
        _emit 007h
        _emit 000h
        _emit 000h

        _emit 081h
        _emit 0fah
        _emit 0b8h
        _emit 00bh
        _emit 000h

        _emit 000h
        _emit 07ch
        _emit 075h
        _emit 03bh
        _emit 0c7h

        _emit 07eh
        _emit 07dh
        _emit 08bh
        _emit 071h
        _emit 03ch

        _emit 08bh
        _emit 069h
        _emit 008h
        _emit 089h
        _emit 05ch

        _emit 024h
        _emit 010h
        _emit 081h
        _emit 0c6h
        _emit 060h

        _emit 0eah
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0edh

        _emit 074h
        _emit 033h
        _emit 08bh
        _emit 041h
        _emit 00ch

        _emit 02bh
        _emit 0c5h
        _emit 0c1h
        _emit 0f8h
        _emit 002h

        _emit 03bh
        _emit 0d8h
        _emit 073h
        _emit 027h
        _emit 08bh

        _emit 051h
        _emit 008h
        _emit 08bh
        _emit 004h
        _emit 09ah

        _emit 03bh
        _emit 041h
        _emit 024h
        _emit 074h
        _emit 019h

        _emit 039h
        _emit 0b8h
        _emit 018h
        _emit 01ah
        _emit 000h

        _emit 000h
        _emit 07eh
        _emit 011h
        _emit 08bh
        _emit 094h

        _emit 0b8h
        _emit 088h
        _emit 018h
        _emit 000h
        _emit 000h

        _emit 03bh
        _emit 0d6h
        _emit 07dh
        _emit 006h
        _emit 089h

        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 0f2h

        _emit 043h
        _emit 0ebh
        _emit 0c9h
        _emit 08bh
        _emit 044h

        _emit 024h
        _emit 010h
        _emit 085h
        _emit 0c0h
        _emit 074h

        _emit 02eh
        _emit 08bh
        _emit 041h
        _emit 024h
        _emit 08bh

        _emit 084h
        _emit 0b8h
        _emit 088h
        _emit 018h
        _emit 000h

        _emit 000h
        _emit 02bh
        _emit 0c6h
        _emit 089h
        _emit 081h

        _emit 0a0h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 08bh

        _emit 041h
        _emit 03ch
        _emit 047h
        _emit 089h
        _emit 081h

        _emit 0a4h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 089h

        _emit 0b9h
        _emit 0a8h
        _emit 007h
        _emit 000h
        _emit 000h

        _emit 08bh
        _emit 091h
        _emit 0a0h
        _emit 007h
        _emit 000h

        _emit 000h
        _emit 052h
        _emit 0e8h
        _emit 014h
        _emit 000h

        _emit 000h
        _emit 000h
        _emit 05fh
        _emit 05eh
        _emit 05dh

        _emit 05bh
        _emit 059h
        _emit 0c3h
    }
}


// FUNCTION: RALLYTROPHY 0x4877a0
// Draws the race time "MM:SS:hh" with the large font.
void RaceMode::FUN_004877a0()
{
    TextRenderer* text = DAT_0056b0c8->bigText;
    Color color(0xffffffff);
    text->color = color.value;
    int time = 0;
    if (m_localPlayer) {
        if (m_localPlayer->unk30) {
            time = m_localPlayer->unk1884[m_localPlayer->unk1a18];
        } else {
            time = m_time;
        }
        if (time < 0) {
            time = 0;
        }
    }
    text->SetScale(DAT_0056aea8 * 0.85f, DAT_0056aeac * 0.85f);
    int minutes = time / 60000;
    text->Printf(DAT_0056b0ac, DAT_00565488.x * DAT_0056aea8, DAT_00565488.y * DAT_0056aeac, "%i%i:%i%i:%i%i",
        minutes / 10 % 10, minutes % 10, time / 10000 % 6, time / 1000 % 10, time / 100 % 10, time / 10 % 10);
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
}

extern DWORD DAT_00508be8;             // standings colour of the leader
extern DWORD DAT_00508bec;             // standings colour of a player who has not started
extern DWORD DAT_00508bf0;             // standings colour of the local player
extern int DAT_00501334;
extern int DAT_00501454;               // race option: shifts the standings table

// NONMATCHING: RALLYTROPHY 0x487560
// 566-568 vs 572 bytes: same code, but the original keeps this in ebx and the entry's player in edi
// (this build swaps them); register assignment did not change over ~10 source forms.
// Draws the standings table: rank, player name and time per line.
void RaceMode::FUN_00487560()
{
    TextRenderer* text = DAT_0056b0c8->text;
    FUN_004873b0();
    Color color;
    float xRank = DAT_0056aea8 * 4.0f;
    float xName = DAT_0056aea8 * 44.0f;
    float xTime = DAT_0056aea8 * 315.0f;
    if (DAT_00501454) {
        xRank += 20.0f;
        xName += 4.0f;
        xTime += 4.0f;
    }
    float y = DAT_0056aeac * 10.0f;
    int i = 0;
    float step = DAT_0056aeac * 20.0f;
    text->SetScale(DAT_0056aea8 * 0.65f, DAT_0056aeac * 0.65f);
    for (; i < unk82c; i++) {
        Player* player = m_entries[i].player;
        if (player == m_localPlayer) {
            color.value = DAT_00508bf0;
        } else if (i == 0) {
            color.value = DAT_00508be8;
        } else if (player->unk40 == 0 && DAT_00501334 > 1) {
            color.value = DAT_00508bec;
        } else {
            color.value = 0xffffffff;
        }
        text->color = color.value;
        text->Printf(DAT_0056b0ac, xRank, y, "%i.", m_entries[i].rank);
        text->scaleX = 1.0f;
        float scale = 182.0f / text->GetWidth(player->name, 0);
        if (scale > 0.65f) {
            scale = 0.65f;
        }
        text->scaleX = DAT_0056aea8 * scale;
        text->Printf(DAT_0056b0ac, xName, y, player->name);
        text->scaleX = DAT_0056aea8 * 0.65f;
        text->Printf(DAT_0056b0ac, xTime - text->GetWidth(m_entries[i].time, 0), y, m_entries[i].time);
        y += step;
    }
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
}

// FUNCTION: RALLYTROPHY 0x486f80
// Draws a message string (id 0x118) centred, fading out with the race time.
void RaceMode::FUN_00486f80()
{
    TextRenderer* text = DAT_0056b0c8->text;
    float y = DAT_0056aeac * 300.0f;
    float x = DAT_0056b0ac->GetWidth() * 0.5f;
    x -= text->GetWidth(FUN_00405380(0x118), 0) * 0.5f;
    Color color(0xffff0000);
    int time = m_time;
    if (time < 0) {
        time = 0;
    }
    color.a = 0xff - (time >> 2);
    text->SetScaleX(DAT_0056aeac);
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y, FUN_00405380(0x118), 0, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x487080
// Draws the car's pending message (a string id selected from its state)
// centred, fading out over 5 seconds.
void RaceMode::FUN_00487080()
{
    Car* car = m_localPlayer->car;
    int stamp = car->unk1c90c;
    int id = car->unk1c908;
    int age = m_time - stamp;
    if (age <= 5000 && id) {
        TextRenderer* text = DAT_0056b0c8->text;
        Color color(0xffff0000);
        if (age < 0) {
            age = 0;
        }
        color.a = 0xff - (age >> 2);
        switch (id) {
        case 1:
            id = 0x172;
            break;
        case 7:
            id = 0x173;
            break;
        case 2:
            id = 0x174;
            break;
        case 3:
            id = 0x175;
            break;
        case 5:
            id = 0x176;
            break;
        case 4:
            id = 0x177;
            break;
        case 6:
            id = 0x178;
            break;
        case 8:
            id = 0x179;
            break;
        }
        float y = DAT_0056aeac * 342.0f;
        float x = DAT_0056b0ac->GetWidth() * 0.5f;
        x -= text->GetWidth(FUN_00405380(id), 0) * 0.5f;
        text->color = color.value;
        text->SetScaleX(DAT_0056aeac);
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(id), 0, 1.0f, 1.0f);
    }
}


// FUNCTION: RALLYTROPHY 0x487920
// Draws the driven distance ("12.3" with a unit label) with the large font.
void RaceMode::FUN_00487920()
{
    TextRenderer* text = DAT_0056b0c8->bigText;
    Color color(0xffffffff);
    text->color = color.value;
    float value = m_localPlayer->car->unk1f140;
    int rounded;
    __asm {
        fld value
        fistp rounded
    }
    int distance = rounded;
    const char* label;
    if (DAT_00501300[0]) {                // profile option: miles instead of kilometres
        distance = (int)(distance * 0.6213712f);
        label = FUN_00405380(0xf);
    } else {
        label = FUN_00405380(0xe);
    }
    FUN_00487b30("%i.%i", distance / 1000, distance / 100 % 10);
    text->SetScale(DAT_0056aea8 * 0.85f, DAT_0056aeac * 0.85f);
    float x = DAT_0056aea8 * 1014.0f - text->GetWidth(DAT_0056541c, 0);
    text->SetScale(DAT_0056aea8 * 0.85f, DAT_0056aeac * 0.85f);
    text->Print(DAT_0056b0ac, x, DAT_00565480.y * DAT_0056aeac, DAT_0056541c, 0, 1.0f, 1.0f);
    float w = text->GetWidth(label, 0);
    float sx = 56.0f / w * DAT_0056aea8;
    float sy = 56.0f / w * DAT_0056aeac;
    if (sx > 0.85f) {
        sx = 0.85f;
    }
    if (sy > 0.85f) {
        sy = 0.85f;
    }
    text->SetScale(DAT_0056aea8 * sx, DAT_0056aeac * sy);
    text->Print(DAT_0056b0ac, DAT_00565480.x * DAT_0056aea8, DAT_00565480.y * DAT_0056aeac, label, 0, 1.0f, 1.0f);
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
}

// NONMATCHING: RALLYTROPHY 0x488690
// 499 vs 484 bytes: the original's player-collection loop reuses the vector's begin pointer between the
// size test and the element load and has a 0x1c frame; this build reloads begin and spills one more local.
// Builds the table of active players at 0x7b0 and draws "name: status" lines.
void RaceMode::FUN_00488690()
{
    TextRenderer* text = DAT_0056b0c8->text;
    FUN_004873b0();
    Color color(0xffffffff);
    float x1 = DAT_0056aea8 * 14.0f;
    int count = 0;
    float x2 = DAT_0056aea8 * 200.0f;
    float y = DAT_0056aeac * 10.0f;
    float step = DAT_0056aeac * 20.0f;
    text->SetScale(DAT_0056aea8 * 0.65f, DAT_0056aeac * 0.65f);
    Lock();
    RaceEntry* entry = m_entries;
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->active) {
            entry->player = player;
            count++;
            entry++;
        }
    }
    Unlock();
    if (count > 0) {
        Player* local = m_localPlayer;
        entry = m_entries;
        for (int k = 0; k < count; k++, entry++) {
            Player* player = entry->player;
            color.value = player == local ? 0xfffbb44f : 0xffffffff;
            text->color = color.value;
            text->Printf(DAT_0056b0ac, x1, y, player->name);
            if (player->state == 4) {
                text->Printf(DAT_0056b0ac, x2, y, "%s: %i %%", FUN_00405380(0x124), player->unk17ec);
            } else {
                text->Print(DAT_0056b0ac, x2, y, FUN_00405380(player->state == 3 ? 0x125 : 0x130), 0, 1.0f, 1.0f);
            }
            y += step;
        }
    }
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
}


// NONMATCHING: RALLYTROPHY 0x4890e0
// Same size, 8 bytes differ: in the non-host branch the original emits `add esp,4` (cdecl pop after FUN_00405380)
// before `mov edi,eax` / the unk40 load; this build schedules it after.
// Draws the pre-race waiting screen: a status line in the middle (waiting for
// players / start) and the player list. Returns whether the start can proceed.
int RaceMode::FUN_004890e0(int unused)
{
    TextRenderer* text = DAT_0056b0c8->text;
    Color color;
    int ready = 0;
    const char* label;
    color.value = 0xffffffff;
    if (DAT_00501454) {
        if (m_localPlayer->unk34) {
            if (DAT_00501450->AllFinished()) {
                label = FUN_00405380(0x152);
                if (DAT_0057ba35) {
                    ready = 1;
                    DAT_00501450->SendStartTimes();
                }
            } else {
                label = FUN_00405380(0x11d);
            }
        } else {
            label = FUN_00405380(0x11e);
            if (DAT_00501450->unk40) {
                ready = 1;
            }
        }
        FUN_00488690();
    } else {
        label = FUN_00405380(0x152);
        if (DAT_0057ba35) {
            ready = 1;
            DAT_00501450->SendStartTimes();
        }
    }
    text->color = color.value;
    text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(label, 0) * 0.5f,
        DAT_0056b0ac->GetHeight() * 0.5f - text->lineHeight * 0.5f, label, 0, 1.0f, 1.0f);
    TextRenderer* hint = DAT_0056b0c8->smallText;
    if (DAT_00501334 > 1) {
        hint->color = 0xaaffffff;
        hint->SetScale(DAT_0056aea8 * 1.25f, DAT_0056aeac * 1.25f);
        hint->Printf(DAT_0056b0ac, DAT_0056aea8 * 58.0f, DAT_0056aeac * 18.0f, "%s: %s", FUN_00405380(0x94), m_localPlayer->name);
        hint->SetScale(DAT_0056aea8, DAT_0056aeac);
    }
    return ready;
}

extern char DAT_005653fc[];            // formatted time text

// FUNCTION: RALLYTROPHY 0x487e00
// Draws the result rows: record, this run, last lap, lap times and best lap.
void RaceMode::FUN_00487e00()
{
    TextRenderer* text = DAT_0056b0c8->text;
    Color color(0xffffffff);
    text->color = color.value;
    text->SetScale(DAT_0056aea8 * 0.65f, DAT_0056aeac * 0.65f);
    Player* player = m_localPlayer;
    RecordEntry* record = ((RecordTable*)((BYTE*)DAT_0056b0c8 + 0x5c30))->FUN_0048ffd0(unk28 - 1, unk2c - 1, 0, 0);
    float x1 = 16.0f * DAT_0056aea8;
    int time = m_time;
    int laps = player->unk1a1c;
    float x2 = DAT_0056aea8 * 240.0f;
    float y = DAT_0056aeac * 10.0f;
    float step = DAT_0056aeac * 20.0f;
    if (laps > 0) {
        time -= player->unk1884[laps * 10];
    }
    if (time < 0) {
        time = 0;
    }
    text->Print(DAT_0056b0ac, x1, y, FUN_00405380(0x111), 0, 1.0f, 1.0f);
    FormatTime(record->time, DAT_005653fc);
    text->Print(DAT_0056b0ac, x2, y, DAT_005653fc, 0, 1.0f, 1.0f);
    y += step;
    text->Print(DAT_0056b0ac, x1, y, FUN_00405380(0x110), 0, 1.0f, 1.0f);
    FormatTime(time, DAT_005653fc);
    text->Print(DAT_0056b0ac, x2, y, DAT_005653fc, 0, 1.0f, 1.0f);
    if (laps > 0) {
        y += step;
        text->Print(DAT_0056b0ac, x1, y, FUN_00405380(0x112), 0, 1.0f, 1.0f);
        FormatTime(player->unk1884[90 + laps], DAT_005653fc);
        text->Print(DAT_0056b0ac, x2, y, DAT_005653fc, 0, 1.0f, 1.0f);
        y += step;
        int best = player->unk1884[91];
        for (int i = 0; i < laps; i++) {
            if (player->unk1884[91 + i] < best) {
                best = player->unk1884[91 + i];
            }
        }
        text->Print(DAT_0056b0ac, x1, y, FUN_00405380(0x113), 0, 1.0f, 1.0f);
        FormatTime(best, DAT_005653fc);
        text->Print(DAT_0056b0ac, x2, y, DAT_005653fc, 0, 1.0f, 1.0f);
    }
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
}

// NONMATCHING: RALLYTROPHY 0x4880f0
// 807 vs 812 bytes: same logic and layout; the original copies the second checkpoint vector with a
// different address form (`mov ecx,[eax+0x14]; add eax,0x14`), stores z back (`fst`) and sums the
// squares in another order for the second distance; no source form found for those three spots.
// Draws the local player's standing "N/6" with its label.
void RaceMode::FUN_004880f0()
{
    TextRenderer* text = DAT_0056b0c8->text;
    Player* local = m_localPlayer;
    int rank = 1;
    if (local->unk30) {
        int mine = local->FUN_00491f70();
        for (unsigned int i = 0; i < m_players.size(); i++) {
            Player* player = m_players[i];
            if (player != local && player->active && player->unk30 && player->FUN_00491f70() < mine) {
                rank++;
            }
        }
    } else {
        Car* car = local->car;
        int count = m_stage->unka04;
        int current = car->unk1f120;
        int next = current + 1;
        if (current == 0 || current == count - 1) {
            if (local->unk1a18 % 10 < 2) {
                current = 0;
                next = 1;
            } else {
                current = count - 1;
                next = 0;
            }
        }
        CarBodyV* body = (CarBodyV*)car->body;
        Vec3 delta = body->position;
        delta -= m_stage->FUN_00414fb0(next)->position;
        float distance = delta.x * delta.x + delta.z * delta.z + delta.y * delta.y;
        for (unsigned int i = 0; i < m_players.size(); i++) {
            Player* player = m_players[i];
            if (player != local && player->active) {
                if (player->unk1a18 > local->unk1a18) {
                    rank++;
                } else if (player->unk1a18 == local->unk1a18) {
                    Car* other = player->car;
                    int otherCurrent = other->unk1f120;
                    int otherNext = otherCurrent + 1;
                    if (otherCurrent == 0 || otherCurrent == count - 1) {
                        if (player->unk1a18 % 10 < 2) {
                            otherCurrent = 0;
                            otherNext = 1;
                        } else {
                            otherCurrent = count - 1;
                            otherNext = 0;
                        }
                    }
                    if (otherCurrent > current) {
                        rank++;
                    } else if (otherCurrent == current) {
                        delta = *(Vec3*)other->body->unk14;
                        delta -= m_stage->FUN_00414fb0(otherNext)->position;
                        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z < distance) {
                            rank++;
                        }
                    }
                }
            }
        }
    }
    Color color(0xffffffff);
    text->SetScale(DAT_0056aea8 * 0.65f, DAT_0056aeac * 0.65f);
    text->color = color.value;
    float x1 = DAT_0056aea8 * 16.0f;
    float x2 = DAT_0056aea8 * 270.0f;
    float row = DAT_0056aeac * 20.0f;
    float y = row * 4.0f + DAT_0056aeac * 32.0f;
    text->Print(DAT_0056b0ac, x1, y, FUN_00405380(0x114), 0, 1.0f, 1.0f);
    text->Printf(DAT_0056b0ac, x2, y, "%i/6", rank);
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
}

// A 4x4 matrix of row vectors (translation in row 3).
struct Matrix4 {
    float m[16];
    void Transform(const Vec3& v, Vec3& out) const
    {
        out.x = v.x * m[0] + v.y * m[4] + v.z * m[8] + m[12];
        out.y = v.x * m[1] + v.y * m[5] + v.z * m[9] + m[13];
        out.z = v.x * m[2] + v.y * m[6] + v.z * m[10] + m[14];
    }
};

// NONMATCHING: RALLYTROPHY 0x488880
// 500 vs 538 bytes: the original keeps the transform's three pointers and the temporary vectors in
// stack slots (the inline Transform is not folded into registers); here the optimizer folds it away.
// The empty __asm block only forces the ebp frame the original has.
// Draws name tags over the other players' cars.
void RaceMode::FUN_00488880(ViewPoint* view)
{
    __asm { }
    if (DAT_00501300[3]) {
        TextRenderer* text = DAT_0056b0c8->text;
        Vec3Slot positions[6];
        Player* tagged[6];
        int count = 0;
        for (unsigned int i = 0; i < m_players.size(); i++) {
            Player* player = m_players[i];
            int state = player->unk40;
            if (Slot14(player) && !player->local && state != 3) {
                CarBody* body = player->car->body;
                Vec3 local = *(Vec3*)body->unkb8;
                local.y = body->unkb0;
                Vec3 world(local);
                ((Matrix4*)body->matrix)->Transform(local, world);
                world.y += 0.5f;
                positions[count].x = world.x;
                positions[count].y = world.y;
                positions[count].z = world.z;
                tagged[count] = player;
                count++;
            }
        }
        if (count) {
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            text->color = 0xc0ffffff;
            for (int i = 0; i < count; i++) {
                text->DrawNameTag(DAT_0056b0ac, view, &positions[i], tagged[i], 0.015f);
            }
        }
    }
}

extern int DAT_00501468;               // number of laps

// std::string whose _Grow, _Eos and append(s, n) the original calls out of line
// (0x402980, 0x402960, 0x403ba0) while it inlines the copy between them.
class LapString : public std::string {
public:
    void AppendN(const char* s, size_t n);
    bool Grow(size_t n, bool trim);
    void Eos(size_t n);
    char* Ptr() { return _Ptr; }
    void Split();                      // 0x402bd0 (_Split)
    void AppendFill(size_t n, char c); // 0x408d10 (append(n, c))
    // erase(pos, count) as xstring has it, with _Split, _Grow and _Eos out of line.
    void EraseCall(size_t pos, size_t count = npos)
    {
        if (_Len < pos) {
            std::_Xran();
        }
        Split();
        if (_Len - pos < count) {
            count = _Len - pos;
        }
        if (0 < count) {
            memmove(_Ptr + pos, _Ptr + pos + count, _Len - pos - count);
            size_t length = _Len - count;
            if (Grow(length, false)) {
                Eos(length);
            }
        }
    }
    void EraseInline(size_t pos, size_t count = npos)
    {
        if (_Len < pos) {
            std::_Xran();
        }
        Split();
        if (_Len - pos < count) {
            count = _Len - pos;
        }
        if (0 < count) {
            memmove(_Ptr + pos, _Ptr + pos + count, _Len - pos - count);
            size_t length = _Len - count;
            if (Grow(length, false)) {
                _Eos(length);
            }
        }
    }
    void ResizeCall(size_t n)
    {
        if (n <= _Len) {
            EraseCall(n);
        } else {
            AppendFill(n - _Len, 0);
        }
    }
    void ResizeInline(size_t n)
    {
        if (n <= _Len) {
            EraseInline(n);
        } else {
            AppendFill(n - _Len, 0);
        }
    }
    void AppendOne(char c)             // append(1, c) with _Grow out of line
    {
        if (npos - _Len <= 1) {
            std::_Xlen();
        }
        if (Grow(_Len + 1, false)) {
            memset(_Ptr + _Len, c, 1);
            _Eos(_Len + 1);
        }
    }
};
#pragma auto_inline(off)
void LapString::AppendN(const char* s, size_t n)
{
    append(s, n);
}
bool LapString::Grow(size_t n, bool trim)
{
    return _Grow(n, trim);
}
void LapString::Eos(size_t n)
{
    _Eos(n);
}
void LapString::Split()
{
    _Split();
}
void LapString::AppendFill(size_t n, char c)
{
    append(n, c);
}
#pragma auto_inline(on)

// NONMATCHING: RALLYTROPHY 0x487b50
// Same size and instruction stream; only stack slots differ (the original frame is 8 bytes smaller:
// it shares slots between the early y/x2 temporaries and the later locals).
// Draws the lap counter "N/M" with the large font, with a unit label under it.
void RaceMode::FUN_00487b50()
{
    LapString lapText;
    Color color(0xffffffff);
    int lap = m_localPlayer->unk1a1c + 1;
    int laps = DAT_00501468;
    TextRenderer* text = DAT_0056b0c8->bigText;
    text->color = color.value;
    if (lap > laps) {
        lap = laps;
    }
    const char* label = FUN_00405380(0x10f);
    text->SetScale(DAT_0056aea8 * 0.85f, DAT_0056aeac * 0.85f);
    float y = DAT_00565480.y * DAT_0056aeac;
    float x2 = DAT_00565480.x * DAT_0056aea8;
    char buffer[10];
    _itoa(lap, buffer, 10);
    size_t length = strlen(buffer);
    if (lapText.Grow(length, true)) {
        memcpy(lapText.Ptr(), buffer, length);
        lapText.Eos(length);
    }
    lapText.AppendN("/", strlen("/"));
    _itoa(laps, buffer, 10);
    lapText.AppendN(buffer, strlen(buffer));
    float x = DAT_0056aea8 * 1014.0f - text->GetWidth(lapText.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, lapText.c_str(), 0, 1.0f, 1.0f);
    float scale = 48.0f / text->GetWidth(label, 0);
    float sx = scale * DAT_0056aea8;
    float sy = scale * DAT_0056aeac;
    if (sx > 0.85f) {
        sx = 0.85f;
    }
    if (sy > 0.85f) {
        sy = 0.85f;
    }
    text->SetScale(DAT_0056aea8 * sx, DAT_0056aeac * sy);
    text->Print(DAT_0056b0ac, x2, y, label, 0, 1.0f, 1.0f);
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
}
// Views of objects from other modules, spelled out as far as the menus below use them.
class MouseView {                      // DAT_0056b0c0 (hook state: 0x57ba00 position)
public:
    float* GetPosition();              // 0x4243c0
    void SetPosition(Vec2* position);  // 0x4b6d00
};
struct StageRecord {                   // DAT_0056b0c8 + 0x367c, 0x5e8 bytes each
    int best;                          // +0x00 (the start time is read as the int at +0x04)
    BYTE unk4[0x5e8 - 4];
};
class MenuGame {                       // DAT_0056b0c8
public:
    int FUN_00437220(int a);           // confirmation dialog result (-1 = still open)
    int FUN_00437770(int a);
};
class ProfileList {                    // DAT_0056b0c8 + 0x2dd0
public:
    void FUN_00404100(int index, const char* name);
    void FUN_00404930(int index, void* slot);
};
class CaretDrawer {                    // DAT_0056b0c8 + 0x2008
public:
    void FUN_00450a70(class RenderDevice* device, float x, float y, DWORD color);
};
extern DWORD DAT_00565444;             // menu colour of the selected entry
extern DWORD DAT_0056543c;             // menu colour of the other entries
extern char DAT_005070b0[];            // empty text
extern int DAT_00501464;               // game mode (0..3)
extern float DAT_0057ba00, DAT_0057ba04;   // mouse position
extern int DAT_0057ba20, DAT_0057ba24;     // left / right button pressed

// NONMATCHING: RALLYTROPHY 0x48cf80
// 2121 vs 2145 bytes, 96% of the instruction stream aligns. Open: the original keeps a never-folded zero in
// the dead `reset` slot ([esp+0x5c] = 0, read back as `enter`), indexes the two stage-record arrays twice
// without CSE, and cross-jumps the `sel = 1` tails of the delta chain. LapString (not std::string) gives the
// out-of-line _Tidy call; the row text copies are the inlined assign (`_Grow` out of line).
// In-game pause menu: three entries (continue, restart, quit) navigated with the arrow keys or the
// mouse. Returns 1 for the first entry, -1 for the second, -2 when the quit dialog was confirmed.
int RaceMode::PauseMenu(int reset, int noRestart)
{
    TextRenderer* smallFont = DAT_0056b0c8->smallText;
    TextRenderer* big = DAT_0056b0c8->bigText;
    LapString text;
    int result = 0;
    Vec2 pos;
    if (reset) {
        unk24c = 0;
        pos.x = DAT_0056aea8 * 512.0f;
        pos.y = DAT_0056aeac * 280.0f;
        ((MouseView*)DAT_0056b0c0)->SetPosition(&pos);
    }
    int atBest = 0;
    if (DAT_00501464 == 1 || DAT_00501464 == 0) {
        int stage = unk28;
        int time = unk2c;
        int best = ((StageTable*)((char*)DAT_0056b0c8 + 0x3aec))->FUN_00443620(stage);
        if (DAT_00501458) {
            best = ((StageRecord*)((char*)DAT_0056b0c8 + 0x367c))[stage].best;
            time -= ((StageRecord*)((char*)DAT_0056b0c8 + 0x3680))[stage].best;
        }
        if (time == best) {
            atBest = 1;
        }
    }
    smallFont->SetScale(DAT_0056aea8, DAT_0056aeac);
    int showFirst = 1;
    int showSecond = 1;
    if (DAT_00501464 == 3 || (DAT_00501464 == 2 && DAT_00501334 == 1) || unk54 != 0
        || (DAT_00501464 == 1 && atBest && noRestart)) {
        showFirst = 0;
    }
    if (DAT_00501454 || DAT_00501472) {
        showSecond = 0;
    }
    reset = 0;
    if (unk240 == 0) {
        int delta = 0;
        if (DAT_0057ba28[VK_UP]) {
            delta = -1;
        }
        if (DAT_0057ba28[VK_DOWN]) {
            delta = 1;
        }
        unk24c += delta;
        if (unk24c < 0) {
            unk24c = 2;
        } else if (unk24c > 2) {
            unk24c = 0;
        }
        if (!showFirst && unk24c == 0) {
            if (delta == 1) {
                unk24c = 1;
            } else if (delta == -1) {
                unk24c = 2;
            } else {
                delta = 1;
                unk24c = 1;
            }
        }
        bool move = true;
        if (!showSecond && unk24c == 1) {
            if (delta == 1) {
                unk24c = 2;
            } else if (delta == -1) {
                unk24c = showFirst ? 0 : 2;
            }
        } else if (delta == 0) {
            move = false;
        }
        if (move) {
            pos.x = DAT_0056aea8 * 512.0f;
            switch (unk24c) {
            case 0:
                pos.y = DAT_0056aeac * 280.0f;
                break;
            case 1:
                pos.y = DAT_0056aeac * 322.0f;
                break;
            case 2:
                pos.y = DAT_0056aeac * 364.0f;
                break;
            }
            ((MouseView*)DAT_0056b0c0)->SetPosition(&pos);
        }
        int enter = DAT_0057ba28[VK_RETURN] ? 1 : reset;
        pos.x = DAT_0057ba00;
        pos.y = DAT_0057ba04;
        if (DAT_0056aeac * 304.0f > pos.y) {
            if (showFirst) {
                unk24c = 0;
            }
        } else if (DAT_0056aeac * 346.0f < pos.y) {
            unk24c = 2;
        } else {
            unk24c = showSecond ? 1 : 2;
        }
        bool select = true;
        if (DAT_0057ba20) {
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
        } else if (!enter) {
            select = false;
        }
        if (select) {
            switch (unk24c) {
            case 0:
                result = 1;
                goto done;
            case 1:
                result = -1;
                goto done;
            case 2:
                DAT_0056b0bc->ClearKeys();
                unk240 = 1;
                break;
            }
        }
    }
    m_image->Draw(DAT_0056b0ac, 0);
    {
        float y = DAT_0056aeac * 262.0f;
        float step = DAT_0056aeac * 42.0f;
        int sel = unk24c;
        for (int i = 0; i < 3; i++) {
            Color color;
            color.value = (i == sel) ? DAT_00565444 : DAT_0056543c;
            big->color = color.value;
            switch (i) {
            case 0:
                if (!showFirst) {
                    size_t length = strlen(DAT_005070b0);
                    if (text.Grow(length, true)) {
                        memcpy(text.Ptr(), DAT_005070b0, length);
                        text.Eos(length);
                    }
                } else {
                    if (DAT_00501464 == 0) {
                        int stage = unk28;
                        int time = unk2c;
                        int best = ((StageTable*)((char*)DAT_0056b0c8 + 0x3aec))->FUN_00443620(stage);
                        if (DAT_00501458) {
                            best = ((StageRecord*)((char*)DAT_0056b0c8 + 0x367c))[stage].best;
                            time -= ((StageRecord*)((char*)DAT_0056b0c8 + 0x3680))[stage].best;
                        }
                        if (time == best) {
                            if (stage == ((StageTable*)((char*)DAT_0056b0c8 + 0x3aec))->FUN_00443580()) {
                                text.assign(FUN_00405380(0x17));
                            } else {
                                text.assign(FUN_00405380(0x157));
                            }
                        } else {
                            text.assign(FUN_00405380(0x156));
                        }
                    } else if (DAT_00501464 == 1) {
                        int stage = unk28;
                        int time = unk2c;
                        int best = ((StageTable*)((char*)DAT_0056b0c8 + 0x3aec))->FUN_00443620(stage);
                        if (DAT_00501458) {
                            best = ((StageRecord*)((char*)DAT_0056b0c8 + 0x367c))[stage].best;
                            time -= ((StageRecord*)((char*)DAT_0056b0c8 + 0x3680))[stage].best;
                        }
                        if (time == best) {
                            text.assign(FUN_00405380(0x17));
                        } else {
                            text.assign(FUN_00405380(0x156));
                        }
                    }
                    if (DAT_00501334 > 1 && !noRestart) {
                        text.assign(FUN_00405380(0x155));
                    }
                }
                break;
            case 1:
                if (DAT_00501464 == 3 || DAT_00501464 == 2) {
                    text.assign(FUN_00405380(0x153));
                }
                if (DAT_00501464 == 1 || DAT_00501464 == 0) {
                    text.assign(FUN_00405380(0x154));
                }
                if (!showSecond) {
                    text.assign(DAT_005070b0);
                    y -= step;
                }
                break;
            case 2:
                if (DAT_00501454 == 0) {
                    text.assign(FUN_00405380(0x1e));
                } else {
                    text.assign(FUN_00405380(0x30));
                }
                break;
            }
            float x = (float)DAT_0056b0ac->GetWidth() * 0.5f - big->GetWidth(text.c_str(), 0) * 0.5f;
            big->Print(DAT_0056b0ac, x, y, text.c_str(), 0, 1.0f, 1.0f);
            y += step;
        }
    }
done:
    if (unk240) {
        int answer = ((MenuGame*)DAT_0056b0c8)->FUN_00437220(0);
        if (answer > -1) {
            unk240 = 0;
            if (answer == 0) {
                result = -2;
            }
        }
    }
    float* mouse = ((MouseView*)DAT_0056b0c0)->GetPosition();
    FUN_0048e4c0(*(HudPoint*)mouse);
    return result;
}

// The device in DAT_0056b0ac as the menus use it: a character queue (slot 8 reads one
// character, slot 9 flushes) and the text input switch at +0x2c.
class TextDevice {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual char GetChar();            // slot 8
    virtual void Flush();              // slot 9
    BYTE unk4[0x28 - 4];
    int unk28;
    int textInput;                     // 0x2c: characters are queued
    BYTE unk30[0x5c - 0x30];
    int pending;                       // 0x5c: queued characters
};
#define TEXT_DEVICE ((TextDevice*)DAT_0056b0ac)
#define M_TEXT (*(LapString*)&m_text)
// NONMATCHING: RALLYTROPHY 0x48bd10
// 2761 vs 2766 bytes, 91% of the instruction stream aligns. Same control flow; the register allocation
// differs (original: this in ebx, zero in esi; here ebp/edi, one extra frame slot) and the string
// pieces (erase with _Split/_Grow/_Eos called, append(1, c) with _Grow called) are hand-expanded in LapString.
// Name menu: two buttons and eight name slots; a slot is chosen with the mouse or the keys, a
// filled slot opens a confirmation dialog (game+0x437770) and an empty one is edited with the
// keyboard into m_text. Returns 1 when the menu is left.
int RaceMode::NameMenu(int reset)
{
    int result = 0;
    m_image->Draw(DAT_0056b0ac, 0);
    if (reset) {
        ProfileList* profiles = *(ProfileList**)((char*)DAT_0056b0c8 + 0x2dd0);
        for (int i = 0; i < 8; i++) {
            profiles->FUN_00404930(i, &m_slots[i]);
        }
        TEXT_DEVICE->textInput = 0;
        unk24c = 0;
        m_editing = 0;
        unk240 = 0;
        TEXT_DEVICE->Flush();
        m_caret = 0;
    }
    Vec2 mouse;
    mouse.x = DAT_0057ba00;
    mouse.y = DAT_0057ba04;
    bool clicked = false;
    int clickSel = 0;
    if (unk240 == 0) {
        if (!m_editing) {
            float edge = DAT_0056aea8 * 500.0f;
            int step = 0;
            int low, high;
            if (mouse.x <= edge && DAT_0056aea8 * 100.0f <= mouse.x && DAT_0056aeac * 150.0f <= mouse.y
                && DAT_0056aeac * 182.0f >= mouse.y) {
                unk24c = 0;
                goto hit;
            }
            if (DAT_0056aea8 * 100.0f <= mouse.x && mouse.x <= edge && DAT_0056aeac * 520.0f <= mouse.y
                && DAT_0056aeac * 552.0f >= mouse.y) {
                unk24c = 1;
                goto hit;
            }
            if (DAT_0056aea8 * 524.0f <= mouse.x && DAT_0056aea8 * 1000.0f >= mouse.x) {
                float top = DAT_0056aeac * 205.0f;
                for (int i = 0; i < 8; i++) {
                    if (mouse.y >= top && DAT_0056aeac * 32.0f + top >= mouse.y) {
                        unk24c = i + 2;
                        goto hit;
                    }
                    top += DAT_0056aeac * 38.0f;
                }
            }
            goto keys;
        hit:
            if (DAT_0057ba20) {
                clickSel = unk24c;
                clicked = true;
            }
        keys:
            if (DAT_0057ba28[VK_UP]) {
                step = -1;
            }
            if (DAT_0057ba28[VK_DOWN]) {
                step++;
            }
            if (unk24c > 1) {
                low = 2;
                high = 9;
            } else {
                low = 0;
                high = 1;
            }
            unk24c += step;
            if (unk24c < low) {
                unk24c = low;
            } else if (unk24c > high) {
                unk24c = high;
            }
            if (step != 0) {
                switch (unk24c) {
                case 0:
                    if (mouse.x > edge) {
                        mouse.x = DAT_0056aea8 * 256.0f;
                    }
                    mouse.y = DAT_0056aeac * 166.0f;
                    break;
                case 1:
                    if (mouse.x > edge) {
                        mouse.x = DAT_0056aea8 * 256.0f;
                    }
                    mouse.y = DAT_0056aeac * 536.0f;
                    break;
                default:
                    if (DAT_0056aea8 * 524.0f > mouse.x) {
                        mouse.x = DAT_0056aea8 * 540.0f;
                    }
                    mouse.y = ((float)(unk24c - 2) * 38.0f + 221.0f) * DAT_0056aeac;
                    break;
                }
                ((MouseView*)DAT_0056b0c0)->SetPosition(&mouse);
            }
        }
        if (DAT_0057ba28[VK_ESCAPE]) {
            if (unk24c <= 1) {
                unk24c = 1;
                clickSel = 1;
                goto act;
            }
            if (m_editing) {
                m_editing = 0;
                TEXT_DEVICE->textInput = 0;
            } else {
                unk24c = 0;
                if (DAT_0056aea8 * 500.0f < mouse.x) {
                    mouse.x = DAT_0056aea8 * 256.0f;
                }
                mouse.y = DAT_0056aeac * 166.0f;
                ((MouseView*)DAT_0056b0c0)->SetPosition(&mouse);
            }
        } else if (DAT_0057ba28[VK_RETURN]) {
            if (m_editing) {
                if (M_TEXT.size() > 0) {
                    ProfileList* profiles = *(ProfileList**)((char*)DAT_0056b0c8 + 0x2dd0);
                    m_editing = 0;
                    TEXT_DEVICE->textInput = 0;
                    profiles->FUN_00404100(unk24c - 2, M_TEXT.c_str());
                    profiles->FUN_00404930(unk24c - 2, &m_slots[unk24c - 2]);
                }
            } else {
                switch (unk24c) {
                case 0:
                    clickSel = 0;
                    break;
                case 1:
                    clickSel = 1;
                    break;
                default:
                    clickSel = unk24c;
                    break;
                }
                goto act;
            }
        }
        if (clicked) {
        act:
            switch (clickSel) {
            case 0:
                unk24c = 2;
                if (DAT_0056aea8 * 524.0f > mouse.x) {
                    mouse.x = DAT_0056aea8 * 540.0f;
                }
                mouse.y = DAT_0056aeac * 221.0f;
                ((MouseView*)DAT_0056b0c0)->SetPosition(&mouse);
                break;
            case 1:
                DAT_0056b0bc->ClearKeys();
                result = 1;
                TEXT_DEVICE->Flush();
                TEXT_DEVICE->textInput = 0;
                break;
            default:
                if (m_slots[clickSel - 2].empty == 0) {
                    DAT_0056b0bc->ClearKeys();
                    unk240 = 1;
                } else {
                    m_editing = 1;
                    M_TEXT.assign(DAT_005070b0);
                    TEXT_DEVICE->textInput = 1;
                    TEXT_DEVICE->Flush();
                }
                break;
            }
        } else if (m_editing) {
            while (TEXT_DEVICE->pending > 0) {
                char ch = TEXT_DEVICE->GetChar();
                if (ch == 8) {
                    int length = M_TEXT.size() - 1;
                    if (length >= 0) {
                        M_TEXT.ResizeCall(length);
                    }
                } else {
                    static const char allowed[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\xc5\xc4\xd6"
                                                  "abcdefghijklmnopqrstuvwxyz\xe5\xe4\xf6\xdc\xfc"
                                                  "0123456789!%&/()=-+* ";
                    for (unsigned int i = 0; i < strlen(allowed); i++) {
                        if (allowed[i] == ch) {
                            if (M_TEXT.size() == 0x16) {
                                M_TEXT.ResizeInline(0x15);
                            }
                            M_TEXT.AppendOne(ch);
                            break;
                        }
                    }
                }
            }
        }
    }
    TextRenderer* big = DAT_0056b0c8->bigText;
    big->SetScale(DAT_0056aea8, DAT_0056aeac);
    for (int i = 0; i < 10; i++) {
        Color rowColor;
        rowColor.value = (i == unk24c) ? DAT_00565444 : DAT_0056543c;
        big->color = rowColor.value;
        if (i == 0) {
            const char* label = FUN_00405380(0x19c);
            float x = DAT_0056aea8 * 500.0f - big->GetWidth(label, 0);
            big->Print(DAT_0056b0ac, x, DAT_0056aeac * 150.0f, label, 0, 1.0f, 1.0f);
        } else if (i == 1) {
            const char* label = FUN_00405380(0x18);
            float x = DAT_0056aea8 * 500.0f - big->GetWidth(label, 0);
            big->Print(DAT_0056b0ac, x, DAT_0056aeac * 520.0f, label, 0, 1.0f, 1.0f);
        } else {
            float x = DAT_0056aea8 * 524.0f;
            float y = ((float)(i - 2) * 38.0f + 205.0f) * DAT_0056aeac;
            if (m_editing && i == unk24c) {
                big->Print(DAT_0056b0ac, x, y, M_TEXT.c_str(), 0, 1.0f, 1.0f);
                DWORD now = timeGetTime();
                DWORD elapsed = now - m_caretTime;
                if (elapsed >= 0x50 && !m_caret) {
                    m_caretTime = now;
                    m_caret = 1;
                } else if (elapsed >= 0x78 && m_caret) {
                    m_caretTime = now;
                    m_caret = 0;
                }
                if (m_caret) {
                    float caretX = big->GetWidth(M_TEXT.c_str(), 0) + x;
                    ((CaretDrawer*)((char*)DAT_0056b0c8 + 0x2008))->FUN_00450a70(DAT_0056b0ac, caretX, y, rowColor.value);
                }
            } else {
                const char* name = m_slots[i - 2].empty ? FUN_00405380(0x27) : m_slots[i - 2].name;
                big->Print(DAT_0056b0ac, x, y, name, 0, 1.0f, 1.0f);
            }
        }
    }
    if (unk240) {
        int answer = ((MenuGame*)DAT_0056b0c8)->FUN_00437770(0);
        if (answer > -1) {
            unk240 = 0;
            if (answer == 0) {
                m_editing = 1;
                TEXT_DEVICE->textInput = 1;
                TEXT_DEVICE->Flush();
            }
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
        }
    }
    float* position = ((MouseView*)DAT_0056b0c0)->GetPosition();
    FUN_0048e4c0(*(HudPoint*)position);
    return result;
}
#undef M_TEXT
#undef TEXT_DEVICE
// Record tables and replay files of the race-mode base (0x4866b0-0x486d20, 0x48de10-0x48e1f0).
// Note: these functions are compiled with the module's usual flags; the std::string handling below
// reproduces the original's out-of-line calls (see PathString).
extern int DAT_00501464;               // player count (> 1 skips the rally total)
extern int DAT_0050132c;               // record table mode for a single-player rally
// The replay header (0x1c bytes) is written from DAT_00565370 and read into DAT_00565390.
struct ReplayHeader {
    int magic;                         // 0x20010730
    int unk2c;                         // player+0x2c
    CarSetup setup;
};
ReplayHeader DAT_00565370;
ReplayHeader DAT_00565390;

#define RECORDS ((RecordTable*)((BYTE*)DAT_0056b0c8 + 0x5c30))
#define STAGES ((StageTable*)((BYTE*)DAT_0056b0c8 + 0x3aec))

// A std::string whose append(const char*, n) the original calls out of line (0x403ba0) at every site.
typedef std::string StdString;
class PathString : public StdString {
public:
    PathString() {}
    PathString& operator=(const char* s)
    {
        StdString::operator=(s);
        return *this;
    }
    void AppendN(const char* s, size_t n);
    PathString& operator+=(const char* s)
    {
        AppendN(s, strlen(s));
        return *this;
    }
};
#pragma auto_inline(off)
void PathString::AppendN(const char* s, size_t n)
{
    append(s, n);
}
#pragma auto_inline(on)

// FUNCTION: RALLYTROPHY 0x4866b0
// Enters the finished stage in the record tables (the local table and, with /s, the remote one at
// game+0x139074) and fills the results block (this run against the record, per stage). `finished` is
// set for the local player; with the multiplayer option and !finished, the other human players are
// entered instead. A new record saves the ghost replay (SaveReplay). Ends by flushing the tables
// (0x4309b0) and saving data/highscores/sessionscores.hs (0x490820).
void RaceMode::SubmitRecords(BYTE finished)
{
    ScoreRecord record;
    Player* best = 0;
    GetLocalTime(&record.date);
    if (DAT_00501454 && !finished) {
        if (!DAT_00501473) {
            for (unsigned int i = 0; i < m_players.size(); i++) {
                Player* p = m_players[i];
                if (p->active && p->unk30 && !p->local) {
                    strcpy(record.name, p->name);
                    record.unk1f = (BYTE)p->unk2c;
                    record.unk1e = (BYTE)p->unk20;
                    for (int k = 0; k < 10; k++) {
                        record.splits[k] = p->unk1884[1 + k];
                    }
                    record.time = 0;
                    record.SetUnk5c(p->unk1a20);
                    if (DAT_00501458) {
                        for (int lap = 0; lap < p->unk1a1c; lap++) {
                            record.time = p->unk1884[91 + lap];
                            RECORDS->FUN_004901b0(&record, unk28 - 1, unk2c - 1);
                            if (DAT_00501471) {
                                DAT_0056b0c8->remoteRecords->FUN_004901b0(&record, unk28 - 1, unk2c - 1);
                            }
                        }
                    } else {
                        if (RECORDS->FUN_00490050(&record, unk28 - 1, unk2c - 1)) {
                            best = p;
                        }
                        if (DAT_00501471) {
                            DAT_0056b0c8->remoteRecords->FUN_00490050(&record, unk28 - 1, unk2c - 1);
                        }
                    }
                }
            }
            if (best) {
                SaveReplay(best);
            }
        }
    } else if (finished) {
        Player* p = m_localPlayer;
        m_newRecord = 0;
        strcpy(record.name, p->name);
        record.unk1f = (BYTE)p->unk2c;
        record.unk1e = (BYTE)p->unk20;
        for (int k = 0; k < 10; k++) {
            record.splits[k] = p->unk1884[1 + k];
        }
        record.time = 0;
        record.SetUnk5c(p->unk1a20);
        if (DAT_00501458) {
            for (int lap = 0; lap < p->unk1a1c; lap++) {
                record.time = p->unk1884[91 + lap];
                if (!DAT_00501473) {
                    if (RECORDS->FUN_004901b0(&record, unk28 - 1, unk2c - 1)) {
                        m_newRecord = 1;
                    }
                    if (DAT_00501471) {
                        DAT_0056b0c8->remoteRecords->FUN_004901b0(&record, unk28 - 1, unk2c - 1);
                    }
                }
            }
        } else {
            RecordEntry* entry = RECORDS->FUN_0048ffd0(unk28 - 1, unk2c - 1, 0, 0);
            int sum = 0;
            for (int i = 0; i < 10; i++) {
                int a = p->unk1884[1 + i];
                int b = entry->splits[1 + i];
                if (i) {
                    a -= p->unk1884[i];
                    b -= entry->splits[i];
                }
                FormatTime(a, m_stageTimes[i]);
                FormatTime(b, m_recordStageTimes[i]);
                FormatSessionTime(a - b, m_stageDeltas[i]);
                sum += a - b;
            }
            p->unk1884[100] = sum;
            FormatSessionTime(sum, m_totalDelta);
            FormatTime(p->unk1884[10], m_totalTime);
            FormatTime(entry->splits[10], m_recordTotalTime);
            strcpy(m_playerName, p->name);
            strcpy(m_recordName, entry->name);
            m_carText = FUN_00405380(0x138 + p->unk2c);
            m_recordCarText = FUN_00405380(0x138 + entry->unk1f);
            if (!DAT_00501473) {
                m_newRecord = RECORDS->FUN_00490050(&record, unk28 - 1, unk2c - 1);
                if (DAT_00501471) {
                    DAT_0056b0c8->remoteRecords->FUN_00490050(&record, unk28 - 1, unk2c - 1);
                }
            }
            if (m_newRecord && p) {
                SaveReplay(p);
            }
        }
    }
    DAT_0056b0c8->FUN_004309b0();
    if (DAT_00501471) {
        DAT_0056b0c8->remoteRecords->FUN_00490820("data/highscores/sessionscores.hs");
    }
}

// FUNCTION: RALLYTROPHY 0x486d20
// At the end of a rally (the last stage, single player) enters the total time in the record tables
// (the other human players too with the multiplayer option), then flushes them like SubmitRecords.
void RaceMode::SubmitRally()
{
    if (DAT_00501458 || DAT_00501464 > 1) {
        return;
    }
    int rally = unk28;
    int stages = STAGES->FUN_00443620(rally);
    if (unk2c != stages) {
        return;
    }
    ScoreRecord record;
    GetLocalTime(&record.date);
    int mode;
    if (DAT_00501454) {
        mode = 2;
        for (unsigned int i = 0; i < m_players.size(); i++) {
            Player* p = m_players[i];
            if (p->unk1a50 == stages && !p->local && !DAT_00501473) {
                strcpy(record.name, p->name);
                record.unk1f = (BYTE)p->unk2c;
                record.unk1e = (BYTE)p->unk20;
                memset(record.splits, 0, sizeof(record.splits));
                record.time = 0;
                record.SetUnk5c(p->unk1a20);
                for (int k = 0; k < stages; k++) {
                    record.time += p->unk1a28[k];
                }
                RECORDS->FUN_00490310(&record, rally - 1, 2);
                if (DAT_00501471) {
                    DAT_0056b0c8->remoteRecords->FUN_00490310(&record, rally - 1, 2);
                }
            }
        }
    } else {
        mode = DAT_0050132c;
    }
    Player* p = m_localPlayer;
    if (p->unk1a50 == stages) {
        strcpy(record.name, p->name);
        record.unk1f = (BYTE)p->unk2c;
        record.unk1e = (BYTE)p->unk20;
        memset(record.splits, 0, sizeof(record.splits));
        record.time = 0;
        record.SetUnk5c(p->unk1a20);
        for (int k = 0; k < stages; k++) {
            record.time += p->unk1a28[k];
        }
        if (!DAT_00501473) {
            RECORDS->FUN_00490310(&record, rally - 1, mode);
            if (DAT_00501471) {
                DAT_0056b0c8->remoteRecords->FUN_00490310(&record, rally - 1, mode);
            }
        }
    }
    DAT_0056b0c8->FUN_004309b0();
    if (DAT_00501471) {
        DAT_0056b0c8->remoteRecords->FUN_00490820("data/highscores/sessionscores.hs");
    }
}

// FUNCTION: RALLYTROPHY 0x48de10
// Loads the ghost for the current rally and stage from the replay files, if both exist: the .ini
// holds the 0x1c-byte header (magic 0x20010730, player+0x2c, five car setup values), the .rep the
// recorded car data.
void RaceMode::LoadReplay()
{
    DeleteGhost();
    PathString path;
    path = "data/replay/replay";
    char number[8];
    _itoa(unk28, number, 10);
    path += number;
    path += "_";
    _itoa(unk2c, number, 10);
    path += number;
    PathString ini, rep;
    ini = path;
    ini += ".ini";
    rep = path;
    rep += ".rep";
    if (!File::Exists(ini.c_str()) || !File::Exists(rep.c_str())) {
        return;
    }
    File file;
    file.Open(ini.c_str(), 0x81);
    int size = file.GetSize();
    if (size == 0x1c) {
        file.Read(&DAT_00565390, size);
        file.Close();
        if (DAT_00565390.magic == 0x20010730) {
            Player* ghost = new Player(0, 1);
            ghost->unk2c = DAT_00565390.unk2c;
            m_ghost = ghost;
            ghost->FUN_00491130();
            Car* car = ghost->car;
            car->FUN_00464530(m_stage, 0);
            car->FUN_00465e90(DAT_00565390.setup.value[0]);
            car->FUN_00466500(DAT_00565390.setup.value[1]);
            car->FUN_00466520(DAT_00565390.setup.value[2]);
            car->FUN_00466590(DAT_00565390.setup.value[3]);
            car->FUN_00466610(DAT_00565390.setup.value[4]);
            car->FUN_00465530(rep.c_str());
            car->FUN_00468c40((int)DAT_0056b0ac);
            car->FUN_004693a0(1);
        }
    } else {
        file.Close();
    }
}

// NONMATCHING: RALLYTROPHY 0x48e1f0
// 712 vs 708 bytes; frame, string layout, EH states and calls all match. Only the header fill differs: the
// original copies unk2c and the five car values one at a time through eax (no load hoisted over a store;
// store of the last value after the magic), this build interleaves ecx/edx/eax and hoists the loads.
// `#pragma optimize("a", off)` removes the hoisting but changes the register allocation elsewhere.
// Saves the ghost of a player: data/replay/replay<rally>_<stage>.ini holds the header (the car's
// five setup values), .rep the recorded car data (Car 0x465550).
void RaceMode::SaveReplay(Player* player)
{
    PathString path;
    path = "data/replay/replay";
    char number[8];
    _itoa(unk28, number, 10);
    path += number;
    path += "_";
    _itoa(unk2c, number, 10);
    path += number;
    PathString ini, rep;
    ini = path;
    ini += ".ini";
    rep = path;
    rep += ".rep";
    Car* car = player->car;
    DAT_00565370.unk2c = player->unk2c;
    DAT_00565370.setup.value[0] = car->setup.value[0];
    DAT_00565370.setup.value[1] = car->setup.value[1];
    DAT_00565370.setup.value[2] = car->setup.value[2];
    DAT_00565370.setup.value[3] = car->setup.value[3];
    DAT_00565370.magic = 0x20010730;
    DAT_00565370.setup.value[4] = car->setup.value[4];
    File file;
    file.Open(ini.c_str(), 0x82);
    file.Write(&DAT_00565370, 0x1c);
    file.Close();
    car->FUN_00465550(rep.c_str());
}

// FUNCTION: RALLYTROPHY 0x4899a0
// The local static (ScreenPoint 340,555; guard 0x5653f8) is never read; its atexit thunk is 0x48a060.
// Rally summary screen: title, the three result columns and the continue prompt.
// Returns 1 when a key was pressed (the keys are cleared).
int RaceMode::FUN_004899a0(int unused)
{
    TextRenderer* text = DAT_0056b0c8->text;
    int result = 0;
    int pressed = 0;
    if (DAT_0057ba35) {
        pressed = 1;
    }
    if (DAT_0057ba20 || pressed) {
        result = 1;
        DAT_0056b0bc->ClearKeys();
    }
    if (!result) {
        static ScreenPoint position(340.0f, 555.0f);
        float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
        float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
        m_image->Draw(DAT_0056b0ac, 0);
        Color color;
        color.value = 0xffffffff;
        text->SetScale(sx, sy);
        text->color = color.value;
        Vec2 pos;
        pos.y = 27.0f;
        pos.x = DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(0x102), 0) * 0.5f;
        text->Print(DAT_0056b0ac, pos.x, sy * 27.0f, FUN_00405380(0x102), 0, 1.0f, 1.0f);
        color.value = 0xfffbb44f;
        text->color = color.value;
        pos.x = DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(0x103), 0) * 0.5f;
        if (m_newRecord) {
            text->Print(DAT_0056b0ac, pos.x, sy * 685.0f, FUN_00405380(0x103), 0, 1.0f, 1.0f);
        }
        text->SetScale(sx * 0.85f, sy * 0.85f);
        color.value = 0xffffffff;
        text->color = color.value;
        float y1 = sy * 91.0f;
        float x1 = sx * 153.0f;
        text->Print(DAT_0056b0ac, x1, y1, FUN_00405380(0x104), 0, 1.0f, 1.0f);
        float x3 = sx * 652.0f;
        text->Print(DAT_0056b0ac, x3, y1, FUN_00405380(0x105), 0, 1.0f, 1.0f);
        float y2 = sy * 219.0f;
        text->Print(DAT_0056b0ac, x1, y2, FUN_00405380(0x106), 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, x3, y2, FUN_00405380(0x106), 0, 1.0f, 1.0f);
        float y3 = sy * 123.0f;
        text->Print(DAT_0056b0ac, x1, y3, m_playerName, 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, x3, y3, m_recordName, 0, 1.0f, 1.0f);
        float y4 = sy * 155.0f;
        text->Print(DAT_0056b0ac, x1, y4, m_carText, 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, x3, y4, m_recordCarText, 0, 1.0f, 1.0f);
        float y = sy * 251.0f;
        float xA = sx * 243.0f;
        float xB = sx * 742.0f;
        float xC = sx * 564.0f;
        float step = sy * 32.0f;
        for (int i = 0; i < 10; i++) {
            text->Print(DAT_0056b0ac, xA, y, m_stageTimes[i], 0, 1.0f, 1.0f);
            text->Print(DAT_0056b0ac, xB, y, m_recordStageTimes[i], 0, 1.0f, 1.0f);
            DWORD tint;
            char sign = m_stageDeltas[i][0];
            if (sign == '+') {
                tint = DAT_00508bf4;
            } else if (sign == '-') {
                tint = DAT_00508bf8;
            } else {
                goto draw;
            }
            text->color = tint;
        draw:
            text->Print(DAT_0056b0ac, xC - text->GetWidth(m_stageDeltas[i], 0), y, m_stageDeltas[i], 0, 1.0f, 1.0f);
            text->SetColor(ColorArg(0xffffffff));
            y += step;
        }
        float xr = sx * 243.0f - text->GetWidth(FUN_00405380(0x107), 0);
        float y5 = sy * 635.0f;
        text->Print(DAT_0056b0ac, xr, y5, FUN_00405380(0x107), 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, sx * 243.0f, y5, m_totalTime, 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, sx * 742.0f, y5, m_recordTotalTime, 0, 1.0f, 1.0f);
        DWORD tint;
        char sign = m_totalDelta[0];
        if (sign == '+') {
            tint = DAT_00508bf4;
        } else if (sign == '-') {
            tint = DAT_00508bf8;
        } else {
            goto draw2;
        }
        text->color = tint;
    draw2:
        text->Print(DAT_0056b0ac, sx * 564.0f - text->GetWidth(m_totalDelta, 0), y5, m_totalDelta, 0, 1.0f, 1.0f);
        text->SetColor(ColorArg(0xffffffff));
        text->SetScale(sx, sy);
    }
    int width = DAT_0056b0ac->GetWidth();
    text->Print(DAT_0056b0ac, width * 0.5f - text->GetWidth(FUN_00405380(0x117), 0) * 0.5f,
        DAT_0056aeac * 710.0f, FUN_00405380(0x117), 0, 1.0f, 1.0f);
    return result;
}

// A std::string (VC6 layout) whose _Tidy, _Grow, _Eos and append(s, n) the original calls out of line
// (0x402700, 0x402980, 0x402960, 0x403ba0) while it inlines the destructor (_Tidy(true)). The members are
// declared only: the calls stay external, like TextString::Tidy.
class LapText {
public:
    LapText(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    LapText(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        size_t n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~LapText() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)              // std::string::_Tidy, expanded inline
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    bool Grow(size_t n, bool trim);    // 0x402980
    void Eos(size_t n);                // 0x402960
    void Append(const char* s, size_t n); // 0x403ba0
    void Assign(const char* s)
    {
        size_t n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

// A per-stage record of the game object (0x5e8 bytes each, from +0x3680).

// NONMATCHING: RALLYTROPHY 0x48a070
// 2187 vs 2203 bytes, instruction stream nearly identical (633 vs 636 instructions), registers as in the
// original (this=esi, zero=ebx, count=edi). Open: copies of x184/x510 into new slots before the row loop,
// the original frame is 0xa0 (this build 0x8c; the number buffer is padded to the original's 32 bytes),
// and the 0x117/0x11d label selection, which the original lays out after the string destructor.
// Stage results table: collects the players that finished every stage (sorted by FUN_00491f70), writes
// their place to Player+0x1a58/0x1a5c and draws title, rows (rank, country, name, time) and the prompt.
// Returns 1 when the continue key was pressed and the results are final.
int RaceMode::FUN_0048a070(int recompute)
{
    TextRenderer* text = DAT_0056b0c8->text;
    int stageCount = m_stage->stageCount;
    if (recompute) {
        unk79c = 0;
    } else if (unk79c) {
        goto draw;
    }
    {
        DAT_00565490 = 0;
        int count = 0;
        int unfinished = 0;
        Player** out = DAT_00565494;
        for (unsigned int j = 0; j < m_players.size(); j++) {
            Player* player = m_players[j];
            if (player->unk1a50 == stageCount) {
                *out = player;
                count++;
                out++;
            } else if (player->active) {
                unfinished++;
            }
        }
        DAT_00565490 = count;
        if (unfinished == 0) {
            unk79c = 1;
        }
        qsort(DAT_00565494, count, 4, CompareByFUN_00491f70);
        int n = DAT_00565490;
        for (int i = 0; i < n; i++) {
            DAT_00565494[i]->unk1a58 = DAT_00565494[i]->unk1a5c = i + 1;
        }
        if (unk79c && DAT_00501454) {
            SendGrid(0);
        }
    }
draw:
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    int pressed = 0;
    m_image->Draw(DAT_0056b0ac, 0);
    text->SetScale(sx, sy);
    Color color(0xffffffff);
    text->color = color.value;
    static Vec2 titlePos;
    LapText title(FUN_00405380(0x108));
    title.Append(" - ", strlen(" - "));
    int stageIndex = m_stage->unk0;
    const char* name = FUN_00405380(stageIndex + 0xb4);
    title.Append(name, strlen(name));
    title.Append(" ", strlen(" "));
    name = FUN_00405380(0x1a);
    title.Append(name, strlen(name));
    title.Append(" ", strlen(" "));
    int shown;
    if (DAT_00501458) {
        shown = stageCount - ((StageRecord*)((BYTE*)DAT_0056b0c8 + 0x3680))[stageIndex].best;
    } else {
        shown = stageCount;
    }
    char number[32];
    _itoa(shown, number, 10);
    title.Append(number, strlen(number));
    titlePos.y = 45.0f;
    titlePos.x = DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(title.c_str(), 0) * 0.5f;
    text->Print(DAT_0056b0ac, titlePos.x, titlePos.y * sy, title.c_str(), 0, 1.0f, 1.0f);
    float sx85 = sx * 0.85f;
    text->SetScale(sx85, sy * 0.85f);
    float y76 = sy * 76.0f;
    float x184 = sx * 184.0f;
    text->Print(DAT_0056b0ac, x184, y76, FUN_00405380(0x95), 0, 1.0f, 1.0f);
    float x510 = sx * 510.0f;
    text->Print(DAT_0056b0ac, x510, y76, FUN_00405380(0x61), 0, 1.0f, 1.0f);
    text->Print(DAT_0056b0ac, sx * 794.0f, y76, FUN_00405380(0xfb), 0, 1.0f, 1.0f);
    float y = sy * 116.0f;
    float x234 = sx * 234.0f;
    ((RaceDeviceSlots*)DAT_0056b0ac)->Slot32(3);
    float x894 = sx * 894.0f;
    int refTime;
    if (DAT_00565490) {
        refTime = DAT_00565494[0]->FUN_00491f70();
    }
    int i = 0;
    if (DAT_00565490 > 0) {
        Player* local = m_localPlayer;
        do {
            DWORD tint;
            if (DAT_00565494[i] == local) {
                tint = DAT_00508bf0;
            } else if (i == 0) {
                tint = DAT_00508be8;
            } else if (DAT_00565494[i]->unk40 == 0 && DAT_00501334 > 1) {
                tint = DAT_00508bec;
            } else {
                tint = 0xffffffff;
            }
            color.value = tint;
            text->color = color.value;
            char timeText[12];
            if (i == 0) {
                FormatTime(DAT_00565494[i]->FUN_00491f70(), timeText);
                DAT_00565494[i]->unk1884[100] = 0;
            } else {
                FormatSessionTime(DAT_00565494[i]->FUN_00491f70() - refTime, timeText);
                DAT_00565494[i]->unk1884[100] = DAT_00565494[i]->FUN_00491f70() - refTime;
            }
            char rank[12];
            _itoa(i + 1, rank, 10);
            text->Print(DAT_0056b0ac, x184 - text->GetWidth(rank, 0), y, rank, 0, 1.0f, 1.0f);
            text->Print(DAT_0056b0ac, x510, y, FUN_00405380(DAT_00565494[i]->unk2c + 0x138), 0, 1.0f, 1.0f);
            text->Print(DAT_0056b0ac, x894 - text->GetWidth(timeText, 0), y, timeText, 0, 1.0f, 1.0f);
            text->scaleX = 1.0f;
            float scale = (x510 - x234 - 10.0f) / text->GetWidth(DAT_00565494[i]->name, 0);
            if (scale > 0.85f) {
                scale = 0.85f;
            }
            text->scaleX = scale * sx;
            text->Print(DAT_0056b0ac, x234, y, DAT_00565494[i]->name, 0, 1.0f, 1.0f);
            text->scaleX = sx85;
            DAT_0056b0c8->FUN_004360c0(DAT_0056aea8 * 194.0f, sy * 6.0f + y, 32.0f, 16.0f, DAT_00565494[i]->unk20);
            y += sy * 28.0f;
            ((RaceDeviceSlots*)DAT_0056b0ac)->Slot32(3);
            i++;
        } while (i < DAT_00565490);
    }
    color.value = 0xffffffff;
    text->color = color.value;
    if (DAT_0057ba35) {
        pressed = 1;
    }
    if (DAT_0057ba20) {
        pressed = 1;
    }
    int result = 0;
    int done = unk79c;
    if (DAT_00501454 && m_localPlayer->unk1a74 == 0) {
        done = 0;
    }
    const char* label;
    if (done) {
        label = FUN_00405380(0x117);
    } else {
        label = FUN_00405380(0x11d);
    }
    if (pressed && done) {
        result = 1;
        DAT_0056b0bc->ClearKeys();
    }
    text->SetScale(sx, sy);
    text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(label, 0) * 0.5f,
        sy * 710.0f, label, 0, 1.0f, 1.0f);
    return result;
}

// NONMATCHING: RALLYTROPHY 0x48a920
// 1788 vs 1839 bytes, instruction stream nearly identical (509 vs 512 instructions). Open: register
// assignment (original zero=ebx, n=edi), copies of x184/x510 before the row loop, scale/state store order.
// Totals table: sorts the players collected by 0x48a070 by their summed stage times, writes the place
// to Player+0x1a5c and draws title, rows (rank, country, name, total time) and the prompt.
// Returns 1 when a key was pressed (the keys are cleared).
int RaceMode::FUN_0048a920(int sort)
{
    TextRenderer* text = DAT_0056b0c8->text;
    LapText title;
    if (sort) {
        qsort(DAT_00565494, DAT_00565490, 4, CompareByTotal);
        int n = DAT_00565490;
        for (int i = 0; i < n; i++) {
            DAT_00565494[i]->unk1a5c = i + 1;
        }
    }
    int pressed = 0;
    static ScreenPoint resultPos(429.0f, 27.0f);
    static ScreenPoint promptPos(340.0f, 555.0f);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    m_image->Draw(DAT_0056b0ac, 0);
    text->SetScale(sx, sy);
    Color color(0xffffffff);
    text->color = color.value;
    static Vec2 titlePos;
    title.Assign(FUN_00405380(0x10a));
    title.Append(" - ", strlen(" - "));
    const char* name = FUN_00405380(m_stage->unk0 + 0xb4);
    title.Append(name, strlen(name));
    titlePos.y = 45.0f;
    titlePos.x = DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(title.c_str(), 0) * 0.5f;
    text->Print(DAT_0056b0ac, titlePos.x, titlePos.y * sy, title.c_str(), 0, 1.0f, 1.0f);
    float sx85 = sx * 0.85f;
    text->SetScale(sx85, sy * 0.85f);
    float y76 = sy * 76.0f;
    float x184 = sx * 184.0f;
    text->Print(DAT_0056b0ac, x184, y76, FUN_00405380(0x95), 0, 1.0f, 1.0f);
    float x510 = sx * 510.0f;
    text->Print(DAT_0056b0ac, x510, y76, FUN_00405380(0x61), 0, 1.0f, 1.0f);
    text->Print(DAT_0056b0ac, sx * 794.0f, y76, FUN_00405380(0xfb), 0, 1.0f, 1.0f);
    float y = sy * 116.0f;
    float x234 = sx * 234.0f;
    float x894 = sx * 894.0f;
    int best = 0;
    int i = 0;
    if (DAT_00565490 > 0) {
        Player* local = m_localPlayer;
        do {
            DWORD tint;
            if (DAT_00565494[i] == local) {
                tint = DAT_00508bf0;
            } else if (i == 0) {
                tint = DAT_00508be8;
            } else if (DAT_00565494[i]->unk40 == 0 && DAT_00501334 > 1) {
                tint = DAT_00508bec;
            } else {
                tint = 0xffffffff;
            }
            color.value = tint;
            text->color = color.value;
            int total = 0;
            int count = DAT_00565494[i]->unk1a50;
            for (int k = 0; k < count; k++) {
                total += DAT_00565494[i]->unk1a28[k];
            }
            char timeText[12];
            if (i == 0) {
                best = total;
                FormatTime(total, timeText);
            } else {
                FormatSessionTime(total - best, timeText);
            }
            char rank[12];
            _itoa(i + 1, rank, 10);
            text->Print(DAT_0056b0ac, x184 - text->GetWidth(rank, 0), y, rank, 0, 1.0f, 1.0f);
            text->Print(DAT_0056b0ac, x510, y, FUN_00405380(DAT_00565494[i]->unk2c + 0x138), 0, 1.0f, 1.0f);
            text->Print(DAT_0056b0ac, x894 - text->GetWidth(timeText, 0), y, timeText, 0, 1.0f, 1.0f);
            text->scaleX = 1.0f;
            float scale = (x510 - x234 - 10.0f) / text->GetWidth(DAT_00565494[i]->name, 0);
            if (scale > 0.85f) {
                scale = 0.85f;
            }
            text->scaleX = scale * sx;
            text->Print(DAT_0056b0ac, x234, y, DAT_00565494[i]->name, 0, 1.0f, 1.0f);
            text->scaleX = sx85;
            DAT_0056b0c8->FUN_004360c0(DAT_0056aea8 * 194.0f, sy * 6.0f + y, 32.0f, 16.0f, DAT_00565494[i]->unk20);
            y += sy * 28.0f;
            ((RaceDeviceSlots*)DAT_0056b0ac)->Slot32(3);
            i++;
        } while (i < DAT_00565490);
    }
    color.value = 0xffffffff;
    text->color = color.value;
    int result = 0;
    if (DAT_0057ba35) {
        pressed = 1;
    }
    if (DAT_0057ba20 || pressed) {
        result = 1;
        DAT_0056b0bc->ClearKeys();
    }
    text->SetScale(sx, sy);
    text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(0x117), 0) * 0.5f,
        sy * 710.0f, FUN_00405380(0x117), 0, 1.0f, 1.0f);
    return result;
}

// File-scope ScreenPoint statics (original: 0x4856e0-0x485a2b): per object a wrapper
// (_$E, the CRT initializer), the initialiser, the atexit registration and the empty
// destructor thunk. The _$E numbers follow the order of the definitions and the headers.

// FUNCTION: RALLYTROPHY 0x4856e0
// SYMBOL: _$E21
typedef void Generated_4856e0();

// FUNCTION: RALLYTROPHY 0x4856f0
// SYMBOL: _$E18
typedef void Generated_4856f0();

// FUNCTION: RALLYTROPHY 0x485710
// SYMBOL: _$E20
typedef void Generated_485710();

// FUNCTION: RALLYTROPHY 0x485720
// SYMBOL: _$E19
typedef void Generated_485720();

// FUNCTION: RALLYTROPHY 0x485730
// SYMBOL: _$E26
typedef void Generated_485730();

// FUNCTION: RALLYTROPHY 0x485740
// SYMBOL: _$E23
typedef void Generated_485740();

// FUNCTION: RALLYTROPHY 0x485760
// SYMBOL: _$E25
typedef void Generated_485760();

// FUNCTION: RALLYTROPHY 0x485770
// SYMBOL: _$E24
typedef void Generated_485770();

// FUNCTION: RALLYTROPHY 0x485780
// SYMBOL: _$E31
typedef void Generated_485780();

// FUNCTION: RALLYTROPHY 0x485790
// SYMBOL: _$E28
typedef void Generated_485790();

// FUNCTION: RALLYTROPHY 0x4857b0
// SYMBOL: _$E30
typedef void Generated_4857b0();

// FUNCTION: RALLYTROPHY 0x4857c0
// SYMBOL: _$E29
typedef void Generated_4857c0();

// FUNCTION: RALLYTROPHY 0x4857d0
// SYMBOL: _$E36
typedef void Generated_4857d0();

// FUNCTION: RALLYTROPHY 0x4857e0
// SYMBOL: _$E33
typedef void Generated_4857e0();

// FUNCTION: RALLYTROPHY 0x485800
// SYMBOL: _$E35
typedef void Generated_485800();

// FUNCTION: RALLYTROPHY 0x485810
// SYMBOL: _$E34
typedef void Generated_485810();

// FUNCTION: RALLYTROPHY 0x485820
// SYMBOL: _$E41
typedef void Generated_485820();

// FUNCTION: RALLYTROPHY 0x485830
// SYMBOL: _$E38
typedef void Generated_485830();

// FUNCTION: RALLYTROPHY 0x485850
// SYMBOL: _$E40
typedef void Generated_485850();

// FUNCTION: RALLYTROPHY 0x485860
// SYMBOL: _$E39
typedef void Generated_485860();

// FUNCTION: RALLYTROPHY 0x485870
// SYMBOL: _$E46
typedef void Generated_485870();

// FUNCTION: RALLYTROPHY 0x485880
// SYMBOL: _$E43
typedef void Generated_485880();

// FUNCTION: RALLYTROPHY 0x4858a0
// SYMBOL: _$E45
typedef void Generated_4858a0();

// FUNCTION: RALLYTROPHY 0x4858b0
// SYMBOL: _$E44
typedef void Generated_4858b0();

// FUNCTION: RALLYTROPHY 0x4858c0
// SYMBOL: _$E51
typedef void Generated_4858c0();

// FUNCTION: RALLYTROPHY 0x4858d0
// SYMBOL: _$E48
typedef void Generated_4858d0();

// FUNCTION: RALLYTROPHY 0x4858f0
// SYMBOL: _$E50
typedef void Generated_4858f0();

// FUNCTION: RALLYTROPHY 0x485900
// SYMBOL: _$E49
typedef void Generated_485900();

// FUNCTION: RALLYTROPHY 0x485910
// SYMBOL: _$E56
typedef void Generated_485910();

// FUNCTION: RALLYTROPHY 0x485920
// SYMBOL: _$E53
typedef void Generated_485920();

// FUNCTION: RALLYTROPHY 0x485940
// SYMBOL: _$E55
typedef void Generated_485940();

// FUNCTION: RALLYTROPHY 0x485950
// SYMBOL: _$E54
typedef void Generated_485950();

// FUNCTION: RALLYTROPHY 0x485960
// SYMBOL: _$E61
typedef void Generated_485960();

// FUNCTION: RALLYTROPHY 0x485970
// SYMBOL: _$E58
typedef void Generated_485970();

// FUNCTION: RALLYTROPHY 0x485990
// SYMBOL: _$E60
typedef void Generated_485990();

// FUNCTION: RALLYTROPHY 0x4859a0
// SYMBOL: _$E59
typedef void Generated_4859a0();

// FUNCTION: RALLYTROPHY 0x485d80
// SYMBOL: ??_GRaceMode@@UAEPAXI@Z
// RaceMode scalar deleting destructor
typedef void Generated_485d80();

extern unsigned char DAT_0057ba28[0x100];

// 0x4243b0 (visible but kept out of line so the caller sees a non-throwing body).
#pragma auto_inline(off)
void TextRenderer::SetColor(ColorArg c)
{
    color = c.value;
}
#pragma auto_inline(on)

// NONMATCHING: RALLYTROPHY 0x48cca0
// 713 vs 723 bytes. Frame, ebx = 0, the by-value colour temporary (ColorArg: copy ctor + empty dtor, with a
// visible nothrow SetColor) all match. Left: instruction scheduling of the hoisted y/step multiplies, and the
// original builds id = 0x161 + 1 via `and al,1 / neg al / sbb / neg` (byte-wide test) where this folds to `and eax,1`.
// Draws a titled message screen over the in-game image and returns 1 (and
// clears the key states) once Return or the mouse button was pressed.
int RaceMode::FUN_0048cca0(int unused, int variant)
{
    TextRenderer* text = DAT_0056b0c8->text;
    int id;
    m_image->Draw(DAT_0056b0ac, 0);
    text->color = DAT_00508bf0;
    float y = DAT_0056aeac * 90.0f;
    float y2 = DAT_0056aeac * 240.0f;
    int result = 0;
    float step = DAT_0056aeac * 64.0f;
    text->SetScale(DAT_0056aea8 * 1.2f, DAT_0056aeac * 1.2f);
    text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(0x166), 0) * 0.5f, y, FUN_00405380(0x166), 0, 1.0f, 1.0f);
    y = y2;
    text->SetColor(ColorArg(0xffffffff));
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
    if (variant) {
        id = 0x161 + ((variant & 1) != 0);
        text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(id), 0) * 0.5f, y, FUN_00405380(id), 0, 1.0f, 1.0f);
        y += step;
    }
    text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(0x167), 0) * 0.5f, y, FUN_00405380(0x167), 0, 1.0f, 1.0f);
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
    y = DAT_0056aeac * 710.0f;
    text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(0x117), 0) * 0.5f, y, FUN_00405380(0x117), 0, 1.0f, 1.0f);
    int pressed = DAT_0057ba28[13] != 0;
    if (DAT_0057ba20 || pressed) {
        result = 1;
        DAT_0056b0bc->ClearKeys();
    }
    return result;
}

// The mouse object DAT_0056b0c0 (input hooks): its accessors are tiny
// functions at 0x4243c0 (position) and 0x4b6d00 (set the cursor position).
class MouseHud : public T_56b0c0 {
public:
    HudPoint* GetPosition();                       // 0x4243c0: &DAT_0057ba00
    void SetPosition(HudPoint* position);          // 0x4b6d00
};
extern DWORD DAT_00508bfc;             // menu text colour
extern DWORD DAT_00508c04;             // menu entry colour
extern DWORD DAT_00508c0c;             // selected menu entry colour
extern float DAT_0057ba00;

// A sound object inside the game object (offset from DAT_0056b0c8).
#define SOUND_AT(offset) ((GameSound*)((BYTE*)DAT_0056b0c8 + (offset)))

// NONMATCHING: RALLYTROPHY 0x48d7f0
// Same size and register assignment through the init loop, EH states and jump tables; the original frame is
// 4 bytes smaller (0x3c): its scale temporaries (width/height * 1/1024, 1/768) share slots with the HudPoint
// objects of later scopes (dir block position, loop scale), and keeps -1 in ebp (`or ebp,-1` in both arms of
// the ESC test). Named floats / inline expressions / a minusOne local did not reproduce either.
// The race options menu: five entries (four volume sliders with +/- arrows,
// and "back"), driven by the keyboard and the mouse. `init` re-selects the entry
// under the mouse. Returns -1 when the menu is closed, otherwise 1.
int RaceMode::FUN_0048d7f0(bool init)
{
    float scaleX = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float scaleY = DAT_0056b0ac->GetHeight() * (1.0f / 768.0f);
    TextRenderer* text = DAT_0056b0c8->bigText;
    int result = 1;
    int i;
    int minusOne = -1;
    if (init) {
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
        for (i = 0; i < 5; i++) {
            if (m_ui[i]->Contains(*((MouseHud*)DAT_0056b0c0)->GetPosition())) {
                unk24c = i;
            }
        }
    }
    if (DAT_0057ba28[0x1b]) {
        result = minusOne;
    }
    if (DAT_0057ba28[0x0d]) {
        if (unk24c == 4) {
            result = minusOne;
        } else {
            SOUND_AT(0x139294)->FUN_0049b730();
        }
    }
    int dir = 0;
    if (DAT_0057ba28[0x28]) {
        dir = 1;
    }
    if (DAT_0057ba28[0x26]) {
        dir--;
    }
    if (dir) {
        unk24c += dir;
        if (unk24c < 0) {
            unk24c = 4;
        } else if (unk24c >= 5) {
            unk24c = 0;
        }
        int x = (int)(m_ui[unk24c]->GetTopLeft().x + (m_ui[unk24c]->GetBottomRight().x - m_ui[unk24c]->GetTopLeft().x) * 0.5f);
        int y = (int)(m_ui[unk24c]->GetTopLeft().y + (m_ui[unk24c]->GetBottomRight().y - m_ui[unk24c]->GetTopLeft().y) * 0.5f);
        HudPoint position;
        position.x = x * scaleX;
        position.y = y * scaleY;
        ((MouseHud*)DAT_0056b0c0)->SetPosition(&position);
    }
    for (i = 0; i < 5; i++) {
        if (unk24c != i) {
            if (m_ui[i]->Contains(*((MouseHud*)DAT_0056b0c0)->GetPosition())) {
                unk24c = i;
                SOUND_AT(0x139224)->FUN_0049b730();
            }
        }
    }
    int step = 0;
    if (DAT_0057ba28[0x25]) {
        step = minusOne;
    }
    if (DAT_0057ba28[0x27]) {
        step++;
    }
    if (DAT_0057ba20) {
        HudPoint mouse = *(HudPoint*)&DAT_0057ba00;
        if (m_ui[5]->Contains(mouse)) {
            step--;
        }
        if (m_ui[6]->Contains(mouse)) {
            step++;
        }
        if (unk24c == 4) {
            result = minusOne;
        }
    }
    if (step) {
        switch (unk24c) {
        case 0:
            FUN_0048ee00(&DAT_00501438[0], step * 10);
            break;
        case 1:
            FUN_0048ee00(&DAT_00501438[1], step * 10);
            break;
        case 2:
            FUN_0048ee00(&DAT_00501438[2], step * 10);
            break;
        case 3:
            FUN_0048ee00(&DAT_00501438[3], step * 10);
            break;
        }
    }
    m_image->Draw(DAT_0056b0ac, 0);
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
    text->color = DAT_00508bfc;
    for (i = 0; i < 5; i++) {
        int selected = i == unk24c;
        Color color(selected ? DAT_00508c0c : DAT_00508c04);
        HudPoint scale;
        scale.x = 1.0f;
        scale.y = 1.0f;
        text->color = color.value;
        HudPoint position;
        position.x = DAT_0056aea8 * 600.0f;
        position.y = (i * 43.0f + 233.0f) * DAT_0056aeac;
        switch (i) {
        case 0:
            FUN_0048ecf0(position.x, position.y, DAT_00501438[0], selected);
            text->Print(DAT_0056b0ac, DAT_0056aea8 * 127.0f, position.y, FUN_00405380(0x5c), 0, 1.0f, 1.0f);
            break;
        case 1:
            FUN_0048ecf0(position.x, position.y, DAT_00501438[1], selected);
            text->Print(DAT_0056b0ac, DAT_0056aea8 * 127.0f, position.y, FUN_00405380(0x5d), 0, 1.0f, 1.0f);
            break;
        case 2:
            FUN_0048ecf0(position.x, position.y, DAT_00501438[2], selected);
            text->Print(DAT_0056b0ac, DAT_0056aea8 * 127.0f, position.y, FUN_00405380(0x5e), 0, 1.0f, 1.0f);
            break;
        case 3:
            FUN_0048ecf0(position.x, position.y, DAT_00501438[3], selected);
            text->Print(DAT_0056b0ac, DAT_0056aea8 * 127.0f, position.y, FUN_00405380(0x5f), 0, 1.0f, 1.0f);
            break;
        case 4:
            text->Print(DAT_0056b0ac, DAT_0056aea8 * 127.0f, DAT_0056aeac * 627.0f, FUN_00405380(0x18), 0, 1.0f, 1.0f);
            break;
        }
    }
    DAT_0056b0bc->ClearKeys();
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
    return result;
}

extern int DAT_00501454;
extern char DAT_00501472;
extern int DAT_00501464;
extern DWORD DAT_00565444;             // selected pause-menu entry colour
extern DWORD DAT_0056543c;             // pause-menu entry colour

// NONMATCHING: RALLYTROPHY 0x489300
// 1644 vs 1636 bytes (+ switch tables). Control flow, EH states (HudPoint at state 0, Color at state 1),
// jump tables, the mouse-cursor call (by-value HudPoint) and the 4895ec shared tail all match. Left: stack slot
// numbering (the original has no frame slot for y or x: y lives in the dead parameter slot [esp+0x3c], mine
// puts y/x in their own slots, shifting result/flag by 4) and the scheduling of the first title print.
// The pause menu: four entries (resume, options, restart, quit), with the
// options screen (0x48d7f0) and a quit confirmation (GameView::FUN_00437220)
// as sub-states in unk244/unk240. `reset` selects the first entry and moves
// the mouse onto it. Returns 1 to resume, -1 to restart, -2 to quit, else 0.
int RaceMode::FUN_00489300(int reset)
{
    TextRenderer* big = DAT_0056b0c8->bigText;
    TextRenderer* smallText = DAT_0056b0c8->smallText;
    int result = 0;
    HudPoint position;
    if (reset) {
        unk24c = 0;
        position.x = DAT_0056aea8 * 512.0f;
        position.y = DAT_0056aeac * 280.0f;
        ((MouseHud*)DAT_0056b0c0)->SetPosition(&position);
    }
    smallText->SetScale(DAT_0056aea8 * 2.0f, DAT_0056aeac * 2.0f);
    smallText->color = DAT_00508bfc;
    smallText->Print(DAT_0056b0ac, DAT_0056aea8 * 54.0f, DAT_0056aeac * 142.0f, FUN_00405380(0xae), 0, 1.0f, 1.0f);
    smallText->SetScale(DAT_0056aea8, DAT_0056aeac);
    int showRestart = 1;
    if (DAT_00501454 || DAT_00501472) {
        showRestart = 0;
    }
    int activate = 0;
    if (unk240 == 0 && unk244 == 0) {
        int dir = 0;
        if (DAT_0057ba28[0x26]) {
            dir = -1;
        }
        if (DAT_0057ba28[0x28]) {
            dir = 1;
        }
        unk24c += dir;
        if (unk24c < 0) {
            unk24c = 3;
        } else if (unk24c > 3) {
            unk24c = 0;
        }
        if (!showRestart && unk24c == 2) {
            if (dir == 1) {
                unk24c = 3;
            } else if (dir == -1) {
                unk24c = 1;
            }
        } else if (!dir) {
            goto moved;
        }
        position.x = DAT_0056aea8 * 512.0f;
        switch (unk24c) {
        case 0:
            position.y = DAT_0056aeac * 280.0f;
            break;
        case 1:
            position.y = DAT_0056aeac * 322.0f;
            break;
        case 2:
            position.y = DAT_0056aeac * 364.0f;
            break;
        case 3:
            if (showRestart) {
                position.y = DAT_0056aeac * 406.0f;
            } else {
                position.y = DAT_0056aeac * 364.0f;
            }
            break;
        }
        ((MouseHud*)DAT_0056b0c0)->SetPosition(&position);
    moved:
        if (DAT_0057ba28[0x0d]) {
            activate = 1;
        }
        position = *(HudPoint*)&DAT_0057ba00;
        if (position.y < DAT_0056aeac * 304.0f) {
            unk24c = 0;
        } else if (position.y < DAT_0056aeac * 362.0f) {
            unk24c = 1;
        } else if (position.y < DAT_0056aeac * 404.0f && showRestart) {
            unk24c = 2;
        } else {
            unk24c = 3;
        }
        if (DAT_0057ba20) {
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
        } else if (!activate) {
            goto menu;
        }
        switch (unk24c) {
        case 0:
            result = 1;
            goto subMenu;
        case 1:
            unk244 = 1;
            break;
        case 2:
            result = -1;
            goto subMenu;
        case 3:
            DAT_0056b0bc->ClearKeys();
            unk240 = 1;
            break;
        }
    }
menu:
    if (unk244 == 0) {
        m_image->Draw(DAT_0056b0ac, 0);
        float y = DAT_0056aeac * 262.0f;
        float step = DAT_0056aeac * 42.0f;
        for (int i = 0; i < 4; i++) {
            if (showRestart || i != 2) {
                Color color(i == unk24c ? DAT_00565444 : DAT_0056543c);
                float x;
                big->color = color.value;
                switch (i) {
                case 0:
            x = DAT_0056b0ac->GetWidth() * 0.5f - big->GetWidth(FUN_00405380(0x17), 0) * 0.5f;
            big->Print(DAT_0056b0ac, x, y, FUN_00405380(0x17), 0, 1.0f, 1.0f);
                    break;
                case 1:
            x = DAT_0056b0ac->GetWidth() * 0.5f - big->GetWidth(FUN_00405380(0x2c), 0) * 0.5f;
            big->Print(DAT_0056b0ac, x, y, FUN_00405380(0x2c), 0, 1.0f, 1.0f);
                    break;
                case 2:
                    if (DAT_00501464 != 1 && DAT_00501464 != 0) {
                        x = DAT_0056b0ac->GetWidth() * 0.5f - big->GetWidth(FUN_00405380(0x153), 0) * 0.5f;
                        big->Print(DAT_0056b0ac, x, y, FUN_00405380(0x153), 0, 1.0f, 1.0f);
                    } else {
                        x = DAT_0056b0ac->GetWidth() * 0.5f - big->GetWidth(FUN_00405380(0x154), 0) * 0.5f;
                        big->Print(DAT_0056b0ac, x, y, FUN_00405380(0x154), 0, 1.0f, 1.0f);
                    }
                    break;
                case 3:
            x = DAT_0056b0ac->GetWidth() * 0.5f - big->GetWidth(FUN_00405380(0x1e), 0) * 0.5f;
            big->Print(DAT_0056b0ac, x, y, FUN_00405380(0x1e), 0, 1.0f, 1.0f);
                    break;
                }
                y += step;
            }
        }
        goto done;
    }
subMenu:
    if (unk244) {
        if (FUN_0048d7f0(false) == -1) {
            unk244 = 0;
        }
    }
done:
    if (unk240) {
        int answer = DAT_0056b0c8->FUN_00437220(0);
        if (answer > -1) {
            unk240 = 0;
            if (answer == 0) {
                result = -2;
            }
        }
    }
    HudPoint* mouse = ((MouseHud*)DAT_0056b0c0)->GetPosition();
    FUN_0048e4c0(*mouse);
    return result;
}
// Destructor thunks of the function-local statics (the atexit callbacks): the empty ~ScreenPoint/~Vec2,
// one per static in declaration order; the linker lays them out in reverse order after each function.

// FUNCTION: RALLYTROPHY 0x48a060
// SYMBOL: _$E64
// Static destructor of FUN_004899a0
typedef void Generated_48a060();

// FUNCTION: RALLYTROPHY 0x48a910
// SYMBOL: _$E66
// Static destructor of FUN_0048a070
typedef void Generated_48a910();

// FUNCTION: RALLYTROPHY 0x48b070
// SYMBOL: _$E68
// Static destructor of FUN_0048a920 (first static)
typedef void Generated_48b070();

// FUNCTION: RALLYTROPHY 0x48b060
// SYMBOL: _$E69
// Static destructor of FUN_0048a920 (second static)
typedef void Generated_48b060();

// FUNCTION: RALLYTROPHY 0x48b050
// SYMBOL: _$E70
// Static destructor of FUN_0048a920 (third static)
typedef void Generated_48b050();

// ---- big draw functions ----

// NONMATCHING: RALLYTROPHY 0x48b080
// 1390 vs 1422 bytes. Logic and all call sequences match; the original spills `this` to [esp+0x28] from entry,
// keeps count/min in ebx/ebp, keeps 0 in ebp for every flags argument, and has a 0x58 frame (this build 0x48). It
// also copies several float locals (xRank/xCar/xPoints) through integer registers at the loop entry and places
// the points string at [esp+0x78] (the dead argument slot); none of that was reproduced.
// Draws the championship points table after a race. With `award` set, the first six players of
// DAT_00565494 are scored (DAT_004ff054 = 10, 6, 4, 3, 2, 1) and ranked first. Returns 1 when a key
// press ends the screen.
int RaceMode::FUN_0048b080(int award)
{
    TextRenderer* text = DAT_0056b0c8->text;
    if (award) {
        int count = DAT_00565490;
        int scored = count;
        if (scored >= 6) {
            scored = 6;
        }
        for (int i = 0; i < scored; i++) {
            Player* p = DAT_00565494[i];
            int old = p->unk1a54;
            p->unk1a54 = old + DAT_004ff054[i];
        }
        qsort(DAT_00565494, count, 4, CompareBy1a54);
        for (int k = 0; k < DAT_00565490; k++) {
            DAT_00565494[k]->unk1a60 = k + 1;
        }
    }
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    m_image->Draw(DAT_0056b0ac, 0);
    Color color(0xffffffff);
    text->color = color.value;
    text->SetScale(sx, sy);
    static Vec2 title;                    // original: function-local static with an empty ctor/dtor
    title.y = 27.0f;
    title.x = DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(0x10c), 0) * 0.5f;
    text->Print(DAT_0056b0ac, title.x, title.y * sy, FUN_00405380(0x10c), 0, 1.0f, 1.0f);
    float sxs = sx * 0.85f;
    text->SetScale(sxs, sy * 0.85f);
    float xRank = sx * 184.0f;
    float yHead = sy * 76.0f;
    text->Print(DAT_0056b0ac, xRank, yHead, FUN_00405380(0x95), 0, 1.0f, 1.0f);
    float xCar = sx * 510.0f;
    text->Print(DAT_0056b0ac, xCar, yHead, FUN_00405380(0x61), 0, 1.0f, 1.0f);
    float xPoints = sx * 794.0f;
    text->Print(DAT_0056b0ac, xPoints, yHead, FUN_00405380(0x10d), 0, 1.0f, 1.0f);
    float y = sy * 116.0f;
    float xName = sx * 234.0f;
    if (DAT_00565490 > 0) {
        Player* local = m_localPlayer;
        float room = xCar - xName - 10.0f;
        for (int i = 0; i < DAT_00565490; i++) {
            char points[12];
            char rank[12];
            color.value = DAT_00565494[i] == local ? DAT_00508bf0 : 0xffffffff;
            text->color = color.value;
            _itoa(DAT_00565494[i]->unk1a54, points, 10);
            _itoa(i + 1, rank, 10);
            text->Print(DAT_0056b0ac, xRank - text->GetWidth(rank, 0), y, rank, 0, 1.0f, 1.0f);
            text->Print(DAT_0056b0ac, xCar, y, FUN_00405380(DAT_00565494[i]->unk2c + 0x138), 0, 1.0f, 1.0f);
            text->Print(DAT_0056b0ac, xPoints, y, points, 0, 1.0f, 1.0f);
            text->scaleX = 1.0f;
            float scale = room / text->GetWidth(DAT_00565494[i]->name, 0);
            if (scale > 0.85f) {
                scale = 0.85f;
            }
            text->scaleX = scale * sx;
            text->Print(DAT_0056b0ac, xName, y, DAT_00565494[i]->name, 0, 1.0f, 1.0f);
            text->scaleX = sxs;
            DAT_0056b0c8->FUN_004360c0(DAT_0056aea8 * 194.0f, y + sy * 6.0f, 32.0f, 16.0f, DAT_00565494[i]->unk20);
            y += sy * 28.0f;
            ((RaceDeviceSlots*)DAT_0056b0ac)->Slot32(3);
        }
    }
    color.value = 0xffffffff;
    text->color = color.value;
    int done = 0;
    int result = 0;
    if (DAT_0057ba35) {
        done = 1;
    }
    if (DAT_0057ba20 || done) {
        result = 1;
        DAT_0056b0bc->ClearKeys();
    }
    text->SetScale(sx, sy);
    text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(0x117), 0) * 0.5f, sy * 710.0f,
                FUN_00405380(0x117), 0, 1.0f, 1.0f);
    return result;
}

// NONMATCHING: RALLYTROPHY 0x48b620
// 1768 (incl. 28-byte jump table) vs 1772. Same control flow, EH states (mouse 0, colour 1, position 2, uv 3),
// switch table and cursor call (HudPoint by value) as the original. Differences: the original keeps 0 in edi and
// never CSEs -1, reads the Color back from its stack slot in the button loop (this build folds it to an
// immediate and swaps the color/result slots), and orders a few register loads differently.
// Replay control bar: seven buttons below the picture (hit-tested against the mouse, moved with the
// left/right keys) and the driver's name, time and position. Sets unk23c to the chosen action (0-4);
// returns -1 for "leave", 1 for "continue". With `reset` set the selection is cleared first.
int RaceMode::FUN_0048b620(int reset)
{
    TextRenderer* text = DAT_0056b0c8->text;
    if (reset) {
        unk24c = 0;
        unk23c = -1;
        unk238 = 0;
    }
    if (DAT_0057ba28[VK_TAB]) {
        unk238 = !unk238;
    }
    if (unk238) {
        return 0;
    }
    int result = 0;
    Vec2 mouse = (*(Vec2*)&DAT_0057ba00);
    char over = 0;
    if (mouse.y >= DAT_0056aeac * 720.0f && mouse.y <= DAT_0056aeac * 752.0f) {
        for (int i = 0; i < 7; i++) {
            float x = DAT_004ff078[i] * 32.0f;
            float left = (x + 710.0f) * DAT_0056aea8;
            float right = (x + 742.0f) * DAT_0056aea8;
            if (mouse.x >= left && mouse.x <= right) {
                unk24c = i;
                over = 1;
                break;
            }
        }
    }
    int step = 0;
    if (DAT_0057ba28[VK_LEFT]) {
        step = -1;
    } else if (DAT_0057ba28[VK_RIGHT]) {
        step = 1;
    }
    unk24c += step;
    if (unk24c < 0) {
        unk24c = 0;
    } else if (unk24c > 6) {
        unk24c = 6;
    } else if (step) {
        mouse.x = (DAT_004ff078[unk24c] * 32.0f + 726.0f) * DAT_0056aea8;
        ((MouseInput*)DAT_0056b0c0)->SetPosition(&mouse);
    }
    if (DAT_0057ba28[VK_RETURN] || (over && DAT_0057ba20)) {
        switch (unk24c) {
        case 0:
            unk23c = 0;
            break;
        case 1:
            unk23c = 1;
            break;
        case 2:
            unk23c = 2;
            break;
        case 3:
            unk23c = 3;
            break;
        case 4:
            unk23c = 4;
            break;
        case 5:
            result = unk23c = -1;

            DAT_0056b0bc->ClearKeys();
            break;
        case 6:
            result = 1;
            DAT_0056b0bc->ClearKeys();
            break;
        }
    }
    Color color(0x80000000);
    Vec2 position[4];
    SetQuad(position, DAT_0056aea8 * 1024.0f * 0.0f, DAT_0056aeac * 704.0f, DAT_0056aea8 * 1024.0f, DAT_0056aeac * 768.0f);
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(&m_sprite254);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawRect(position, color, 1.0f, 1.0f);
    color.value = 0xffffffff;
    Car* car = m_camera->target;
    if (car) {
        Player* player = car->player;
        text->color = color.value;
        if (DAT_00501464 != 3) {
            text->SetScale(DAT_0056aea8 * 1.5625f, DAT_0056aeac * 1.5625f);
            text->Printf(DAT_0056b0ac, DAT_0056aea8 * 8.0f, DAT_0056aeac * 710.0f, "%i.", player->unk1a58);
        }
        char buffer[16];
        text->SetScale(DAT_0056aea8, DAT_0056aeac);
        text->Print(DAT_0056b0ac, DAT_0056aea8 * 114.0f, DAT_0056aeac * 708.0f, player->name, 0, 1.0f, 1.0f);
        FormatTime(player->FUN_00491f70(), buffer);
        text->Print(DAT_0056b0ac, DAT_0056aea8 * 558.0f, DAT_0056aeac * 708.0f, buffer, 0, 1.0f, 1.0f);
        text->SetScale(DAT_0056aea8 * 0.625f, DAT_0056aeac * 0.625f);
        text->Print(DAT_0056b0ac, DAT_0056aea8 * 114.0f, DAT_0056aeac * 738.0f, FUN_00405380(player->unk2c + 0x138), 0, 1.0f, 1.0f);
        if (player->unk1a14) {
            FormatSessionTime(player->unk1a14, buffer);
            text->Print(DAT_0056b0ac, DAT_0056aea8 * 558.0f, DAT_0056aeac * 738.0f, buffer, 0, 1.0f, 1.0f);
        }
        DAT_0056b0c8->FUN_004360c0(DAT_0056aea8 * 84.0f, DAT_0056aeac * 716.0f, 32.0f, 16.0f, player->unk20);
        text->SetScale(DAT_0056aea8, DAT_0056aeac);
    }
    color.value = 0xffffffff;
    Vec2 uv[4];
    position[0].y = position[1].y = DAT_0056aeac * 720.0f;
    position[2].y = position[3].y = DAT_0056aeac * 752.0f;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(&m_replaySprite);
    for (int i = 0; i < 7; i++) {
        float v = i == unk24c ? 0.5f : 0.0f;
        uv[0].x = uv[3].x = DAT_004ff078[i] * 0.125f;
        uv[1].x = uv[2].x = uv[3].x + 0.125f;
        uv[0].y = uv[1].y = v;
        uv[2].y = uv[3].y = v + 0.5f;
        float x = DAT_004ff078[i] * 32.0f;
        position[0].x = position[3].x = (x + 710.0f) * DAT_0056aea8;
        position[1].x = position[2].x = (x + 742.0f) * DAT_0056aea8;
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
    }
    HudPoint* cursor = ((MouseInput*)DAT_0056b0c0)->GetPosition();
    FUN_0048e4c0(*cursor);
    return result;
}

// Centred text line used by FUN_0048c7e0.
#define CENTER_TEXT(index, y) \
    text->Print(DAT_0056b0ac, DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(index), 0) * 0.5f, y, \
                FUN_00405380(index), 0, 1.0f, 1.0f)

// NONMATCHING: RALLYTROPHY 0x48c7e0
// 1202 vs 1205 bytes; the body after the first Print is the same. Only the prologue schedule differs: the original
// stores the colour before loading the device width and interleaves the pushes of the first Print's constant
// arguments with the three scaled-y locals (source order of the locals made no difference).
// Help screen of the replay/race menu: a title, then lines chosen by `flags` (1/2/4/8 = one of four
// texts, 0x10 = extra line, 0x20, 0x40 = more lines) and the "press a key" footer. Returns 1 when
// a key or the mouse button was pressed.
int RaceMode::FUN_0048c7e0(int unused, BYTE flags)
{
    TextRenderer* text = DAT_0056b0c8->text;
    m_image->Draw(DAT_0056b0ac, 0);
    float y = DAT_0056aeac * 90.0f;
    int result = 0;
    float y0 = DAT_0056aeac * 240.0f;
    float step = DAT_0056aeac * 64.0f;
    text->color = DAT_00508bf0;
    text->SetScale(DAT_0056aea8 * 1.2f, DAT_0056aeac * 1.2f);
    CENTER_TEXT(0x158, y);
    text->SetColor(ColorArg(0xffffffff));
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
    y = y0;
    if (flags & 0x10) {
        int index = 0;
        if (flags & 1) {
            index = unk28 + 0x159;
        } else if (flags & 2) {
            index = 0x17a;
        } else if (flags & 4) {
            index = 0x160;
        } else if (flags & 8) {
            index = 0x15f;
        }
        if (index) {
            CENTER_TEXT(index, y);
            y += step;
        }
        CENTER_TEXT(0x163, y);
        y = y + step;
    } else {
        int index;
        if (flags & 1) {
            index = 0x159;
        } else if (flags & 2) {
            index = 0x17a;
        } else if (flags & 4) {
            index = 0x160;
        } else if (flags & 8) {
            index = 0x15f;
        } else {
            goto skip;
        }
        CENTER_TEXT(index, y);
        y += step;
    }
skip:
    if (flags & 0x20) {
        CENTER_TEXT(0x164, y);
        y = y + step;
    }
    if (flags & 0x40) {
        CENTER_TEXT(0x165, y);
    }
    text->SetScale(DAT_0056aea8, DAT_0056aeac);
    CENTER_TEXT(0x117, DAT_0056aeac * 710.0f);
    int done = 0;
    if (DAT_0057ba28[VK_RETURN]) {
        done = 1;
    }
    if (DAT_0057ba20 || done) {
        result = 1;
        DAT_0056b0bc->ClearKeys();
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x48b610
// SYMBOL: _$E64
// Empty destructor thunk the function-local static `title` of 0x48b080 registers with atexit.
typedef void Generated_48b610();
