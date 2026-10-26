// GameScreen2DD4 (0x4266c0-0x4275ff): the 0x20-byte menu screen at Game+0x2dd4. Map: analysis/range-4266c0.md;
// class in include/unk_4266c0.h. Work group H1a (lifecycle, Run, module end).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_4266c0.h"
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <string.h>

extern int DAT_00501458;


// ---- from group draft H1b ----
extern int DAT_00501458;               // race option: nonzero hides item 3 and draws text 0x33 instead of 0x32
extern int DAT_004f6998[5];            // text ids of the five items (0x35 .. 0x38, 0x18)

// ---- Group H1b ----
// Layout constants of Frame (the original reads each from .rdata as a separate object: fld const; fmul scale).
static float H1b_titleX = 54.0f;       // 0x4e1540
static float H1b_titleY = 142.0f;      // 0x4e1544
static float H1b_firstY = 262.0f;      // 0x4e1534
static float H1b_step = 42.0f;         // 0x4e1538
static float H1b_backY = 610.0f;       // 0x4e153c


// ---- from group draft H1c ----
extern ConfigKey DAT_004f69b0[];

// FUNCTION: RALLYTROPHY 0x4266c0
GameScreen2DD4::GameScreen2DD4()
{
    m_selected = 0;
    m_state = 0;
}

// FUNCTION: RALLYTROPHY 0x4266e0
// SYMBOL: ??_GGameScreen2DD4@@UAEPAXI@Z
typedef void Generated_4266e0();

// FUNCTION: RALLYTROPHY 0x426700
GameScreen2DD4::~GameScreen2DD4()
{
    Release();
}

// FUNCTION: RALLYTROPHY 0x426750
void GameScreen2DD4::Setup(MenuGame* game)
{
    if (m_state == 0) {
        m_game = game;
        m_state = 1;
        LoadLayout();
    }
}

// FUNCTION: RALLYTROPHY 0x426770
int GameScreen2DD4::Run(MenuGame* game)
{
    Setup(game);
    if (DAT_00501458 && m_selected == 3) {
        m_selected = 2;
    }
    return Frame();
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// NONMATCHING: RALLYTROPHY 0x4267a0
// original: FUN_004267a0. Same size and code except 54 bytes of scheduling in the title-print prologue: the original
// loads DAT_00501458 after the text->color store (color in eax), VC6 here hoists the load above the stores (color in
// ecx) and picks other temps for the DAT_0056b0ac loads. Tried statement order, !x, swapped arms, SetScale/SetColor.
int GameScreen2DD4::Frame()
{
    int done = 0;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    int clicked = 0;
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    DAT_0056b0bc->ClearKeys();
    TextRenderer* text = m_game->text;
    int first = 1;
    int wait = 0;
    while (FUN_004b69f0(wait)) {
        if (done || clicked) {
            break;
        }
        m_game->Update();
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 0) {
                m_selected = 0;
                m_game->Play(0);
            }
            if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 1) {
                m_selected = 1;
                m_game->Play(0);
            }
            if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 2) {
                m_selected = 2;
                m_game->Play(0);
            }
        }
        if (DAT_0057ba28[VK_ESCAPE]) {
            done = 1;
        }
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        if (DAT_0057ba28[VK_RETURN]) {
            if (m_selected == 4) {
                done = 1;
            } else {
                m_game->Play(1);
                clicked = 1;
            }
            continue;
        }
        int dir = 0;
        if (DAT_0057ba28[VK_DOWN]) {
            dir = 1;
        }
        if (DAT_0057ba28[VK_UP]) {
            dir--;
        }
        if (dir != 0) {
            m_game->Play(0);
            m_selected += dir;
            if (m_selected < 0) {
                m_selected = 4;
            } else if (m_selected > 4) {
                m_selected = 0;
            }
            if (m_selected == 3 && DAT_00501458) {
                m_selected = dir > 0 ? 4 : 2;
            }
            int cx = (int)(m_items[m_selected]->GetTopLeft().x +
                           (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f);
            int cy = (int)(m_items[m_selected]->GetTopLeft().y +
                           (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f);
            SetCursorPos((int)(cx * sx), (int)(cy * sy));
        }
        if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 0) {
            m_selected = 0;
            m_game->Play(0);
        }
        if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 1) {
            m_selected = 1;
            m_game->Play(0);
        }
        if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 2) {
            m_selected = 2;
            m_game->Play(0);
        }
        if (DAT_00501458 == 0) {
            if (m_items[3]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 3) {
                m_selected = 3;
                m_game->Play(0);
            }
        }
        if (m_items[4]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 4) {
            m_selected = 4;
            m_game->Play(0);
        }
        if (DAT_0057ba20) {
            if (m_items[4]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                done = 1;
                continue;
            }
            if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
            if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
            if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
            if (m_items[3]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
        }
        MenuGame* game = m_game;
        game->UpdateVideo();
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            game->FUN_00432a40();
            game->DrawVideo(1);
            text->scaleX = DAT_0056aea8 * 2.0f;
            text->color = DAT_00508bfc;
            text->scaleY = DAT_0056aeac * 2.0f;
            if (DAT_00501458 == 0) {
                text->Print(DAT_0056b0ac, H1b_titleX * DAT_0056aea8, H1b_titleY * DAT_0056aeac, FUN_00405380(0x32), 0, 1.0f, 1.0f);
            } else {
                text->Print(DAT_0056b0ac, H1b_titleX * DAT_0056aea8, H1b_titleY * DAT_0056aeac, FUN_00405380(0x33), 0, 1.0f, 1.0f);
            }
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            float y = H1b_firstY * sy;
            float x = sx * 54.0f;
            for (int i = 0; i < 5; i++) {
                if (DAT_00501458 == 0 || i != 3) {
                    int selected = (i == m_selected);
                    Color col;
                    col.value = selected ? DAT_00508c0c : DAT_00508c04;
                    if (i == 4) {
                        y = H1b_backY * sy;
                    }
                    text->color = col.value;
                    text->Print(DAT_0056b0ac, x, y, FUN_00405380(DAT_004f6998[i]), 0, 1.0f, 1.0f);
                    y += H1b_step * sy;
                }
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, DAT_00501458 ? 3 : 4, 1, 1, 1, -1.0f);
            DrawCursor(*((MouseInput*)DAT_0056b0c0)->GetPosition());
            DAT_0056b0ac->EndScene();
            DAT_0056b0ac->Present();
        }
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    int r = m_selected;
    if (done || DAT_0057b9f4) {
        r = -1;
        m_game->Play(2);
    }
    return r;
}

// NONMATCHING: RALLYTROPHY 0x4270c0
// Loads the five game-mode hit-test rectangles from gamemodemenu.ini; inline vector growth/register allocation still differs.
void GameScreen2DD4::LoadLayout()
{
    Config config;
    config.Define(DAT_004f69b0, 10);
    std::string filename;
    filename = "data/menu/gamemodemenu.ini";
    config.Load(filename.c_str(), 1, 0, 0);

    UIRect* rect;
    rect = new UIRect(*(HudPoint*)&config.GetVector2("ChampionshipText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("ChampionshipText_LowerRight"));
    m_items.insert(m_items.end(), rect);
    rect = new UIRect(*(HudPoint*)&config.GetVector2("SingleRaceText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("SingleRaceText_LowerRight"));
    m_items.insert(m_items.end(), rect);
    rect = new UIRect(*(HudPoint*)&config.GetVector2("SingleStageText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("SingleStageText_LowerRight"));
    m_items.insert(m_items.end(), rect);
    rect = new UIRect(*(HudPoint*)&config.GetVector2("TimeTrialText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("TimeTrialText_LowerRight"));
    m_items.insert(m_items.end(), rect);
    rect = new UIRect(*(HudPoint*)&config.GetVector2("BackText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("BackText_LowerRight"));
    m_items.insert(m_items.end(), rect);
}

// FUNCTION: RALLYTROPHY 0x4275d0
// SYMBOL: _$E23
typedef void Generated_4275d0();

// FUNCTION: RALLYTROPHY 0x4275f0
// SYMBOL: _$E22
typedef void Generated_4275f0();
