// PlayerNameMenu (0x43d770-0x43f9df): the "Enter your name" screen. Map: analysis/range-43d770.md section 1; classes in
// include/unk_43d770.h.
// Work groups: A1 (lifecycle: ctor/dtors/SpriteCell/Release/Open, module-end stubs; prefix A1_), A2 (Setup, SetupLabels,
// CheckCheatName, DrawTextCursor, DrawPlate), A3 (BuildRects), A4 (Run).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_43d770.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group A1 ----
// Layout constants of the constructor (the original reads each from .rdata; sums are not folded).
static const float A1_NameX = 381.0f;      // 0x4e17f0
static const float A1_NameXAdd = 58.0f;    // 0x4e14c4
static const float A1_RowY = 262.0f;       // 0x4e17d8
static const float A1_RowStep = 42.0f;     // 0x4e17dc
static const float A1_Up = 8.0f;           // 0x4e1644
static const float A1_LabelX = 54.0f;      // 0x4e17ec


// ---- from group draft X2 ----
extern int DAT_00501334;               // number of name rows in use (initially 1)
extern BYTE DAT_005014c8, DAT_005014c9;
extern int DAT_004f8f1c[2];            // text ids of the Continue and Back captions (0x17, 0x18)


// ---- from group draft X3 ----
struct A3_Point {
    A3_Point() {}
    A3_Point(const A3_Point& other);   // 0x424300
    ~A3_Point() {}
    float x, y;
};
class A3_Rect {
public:
    A3_Rect(A3_Point topLeft, HudPoint bottomRight); // 0x4390a0
    int f[5];
};
static float A3_X = 54.0f;             // 0x4e17ec: name column x
static float A3_ARROWX = 762.0f;       // 0x4e17f4: arrow column x
static float A3_BACKY = 610.0f;        // 0x4e17e8: Back row y
static float A3_TOP = 262.0f;          // 0x4e17d8: first row y
static float A3_PITCH = 42.0f;         // 0x4e17dc: row pitch


// ---- from group draft X4 ----
class A4_Device : public RenderDevice {
public:
    int busy;                          // 0x2c
    BYTE unk30[0x5c - 0x30];
    int pending;                       // 0x5c: typed characters waiting
};
// The mouse-warp temporary: inline constructor and empty destructor (see SME_WarpPoint in unk_445e80.cpp).
struct A4_WarpPoint {
    A4_WarpPoint(float px, float py)
    {
        x = px;
        y = py;
    }
    ~A4_WarpPoint() {}
    float x, y;
};
// A view of std::string whose assign(const char*) is expanded by hand: the VC6 inline budget runs out part way through
// Run, so the two sites differ (AssignB calls _Eos 0x402960, AssignC stores the length and terminator inline).
struct A4_Str {
    BYTE alloc[4];
    char* ptr;
    unsigned int len;
    unsigned int res;
    bool Grow(unsigned int n, bool trim);  // 0x402980
    void Eos(unsigned int n);              // 0x402960
    void Erase(unsigned int pos, unsigned int n); // 0x408de0
    void AppendFill(unsigned int n, char c);      // 0x408d10
    void Resize(unsigned int n)
    {
        n <= len ? Erase(n, std::string::npos) : AppendFill(n - len, 0);
    }
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    void AssignC(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            len = n;
            ptr[n] = 0;
        }
    }
};
#define A4_STR(s) ((A4_Str*)&(s))
// Constants the original reads from .rdata as separate objects.
static const float A4_ICONS = 64.0f;   // icon cells (the grid is 4 x 16)
static const float A4_ONE = 1.0f;
// The player profile global DAT_00501300: the six default names are 30-byte slots at +0x38 (0x501338), the six icon
// indices follow at +0xec (0x5013ec) and the row count is the dword at +0x34. Run writes names and icons back through
// this one array (separate externs let VC6 turn the icon copy into a rep movs).

// FUNCTION: RALLYTROPHY 0x43d770
PlayerNameMenu::PlayerNameMenu()
{
    m_selected = 0;
    m_state = 0;
    for (int i = 0; i < 6; i++) {
        name[i] = DAT_00501338[i];
        positions[i].x = (A1_NameX + A1_NameXAdd) * DAT_0056aea8;
        positions[i].y = ((float)i * A1_RowStep + A1_RowY - A1_Up) * DAT_0056aeac;
        labels[i].text = FUN_00405380(0x94);
        char number[8];
        _itoa(i + 1, number, 10);
        labels[i].text += " ";
        labels[i].text += number;
        labels[i].text += ":";
        labels[i].x = A1_LabelX * DAT_0056aea8;
        labels[i].y = ((float)i * A1_RowStep + A1_RowY) * DAT_0056aeac;
        icon[i] = DAT_005013ec[i];
    }
}

// FUNCTION: RALLYTROPHY 0x43db20
// SYMBOL: ??_GPlayerNameMenu@@UAEPAXI@Z
typedef void Generated_43db20();

// FUNCTION: RALLYTROPHY 0x43db40
// SYMBOL: ??0SpriteCell@@QAE@XZ
// Implicit constructor of SpriteCell: the vector constructor of its two HudPoints.
typedef void Generated_43db40();

// FUNCTION: RALLYTROPHY 0x43db60
// SYMBOL: ??1SpriteCell@@QAE@XZ
// Implicit destructor of SpriteCell.
typedef void Generated_43db60();

// FUNCTION: RALLYTROPHY 0x43db80
PlayerNameMenu::~PlayerNameMenu()
{
    Release();
}

// NONMATCHING: RALLYTROPHY 0x43dc70
// 414 vs 416 bytes; every instruction of the loop and the tail is present. The original keeps the int->float temps of
// col, row and row + 1 in the dead parameter slot and the pushed `ecx` slot (frame 0x10), mine allocates a third
// temp for row + 1 (`sub esp,8`); the cell loop reloads this->iconTexture each pass only via cells[i] indexing.
// original: FUN_0043dc70 (vtable slot 1)
void PlayerNameMenu::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    m_game = game;
    iconTexture = *(void**)((BYTE*)game + 0x1390f8);
    m_state = 1;
    int col = 0;
    int row = 0;
    for (int i = 0; i < 64; i++) {
        cells[i].texture = iconTexture;
        cells[i].width = 64.0f;
        cells[i].height = 32.0f;
        cells[i].uv[0].x = col * 0.25f;
        col++;
        cells[i].uv[0].y = row * 0.0625f;
        cells[i].uv[1].x = col * 0.25f;
        cells[i].uv[1].y = (row + 1) * 0.0625f;
        if (col == 4) {
            col = 0;
            row++;
        }
    }
    plate = new Sprite();
    plate->texture = new Texture("data/menu/linename.tga", 0x841, 0);
    plate->texture->Upload(DAT_0056b0ac);
    plate->unk9c = 2;
    plate->unk98 = 0xf;
    plate->unk18 = 5;
    plate->unk60[0] = 1.0f;
    plate->unk60[1] = 1.0f;
    plate->unk60[2] = 1.0f;
    plate->unk60[3] = 1.0f;
    plate->unk1c = 6;
    plate->unk40[0] = 1.0f;
    plate->unk40[1] = 1.0f;
    plate->unk40[2] = 1.0f;
    plate->unk40[3] = 1.0f;
    plate->unk24 = 2;
}

// FUNCTION: RALLYTROPHY 0x43de10
// original: FUN_0043de10 (vtable slot 2)
void PlayerNameMenu::Release()
{
    if (m_state) {
        Sprite* sprite = plate;
        m_state = 0;
        m_game = 0;
        delete sprite->texture;
        delete sprite;
    }
}

// FUNCTION: RALLYTROPHY 0x43de50
// original: FUN_0043de50
int PlayerNameMenu::Open(MenuGame* game, NetRace* net_)
{
    net = net_;
    Setup(game);
    for (int i = 0; i < 6; i++) {
        name[i] = DAT_00501338[i];
    }
    if (DAT_00501458) {
        DAT_00501334 = 1;
    }
    SetupLabels();
    if (m_selected < 6 && m_selected >= rowCount) {
        m_selected = 0;
    }
    return Run();
}

// ---- module end: guard-bit initialiser of the STL header statics and its atexit registration ----

// NONMATCHING: RALLYTROPHY 0x43df70
// 1050 vs 1046 instructions; everything but three spots is identical: (1) the `m_selected` skip loop is a rotated loop
// in mine, an un-rotated top-tested one in the original (no source form found); (2) the title Print loads the device
// pointer one instruction earlier; (3) VC6 creates the draw loop's three induction pointers (name, icon, label) in the
// order label, icon, name here (original name, icon, label), which shifts eax/ecx and the i register of that loop.
// original: FUN_0043df70
int PlayerNameMenu::Run()
{
    HudPoint mouse;
    int done = 0;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    int picked = 0;
    DAT_0056b0bc->ClearKeys();
    TextRenderer* text = m_game->text;
    TextRenderer* nameText = *(TextRenderer**)((BYTE*)m_game + 0x1390bc);
    nameText->scaleX = DAT_0056aea8 * 1.5f;
    int first = 1;
    int typing = 0;
    nameText->scaleY = DAT_0056aeac * 1.5f;
    BuildRects();
    m_selected = 0;
    for (int k = 0; k < 6; k++) {
        editing[k] = 1;
    }
    ((A4_Device*)DAT_0056b0ac)->busy = 1;
    DAT_0056b0ac->vf09();
    int wait = 0;
    while (FUN_004b69f0(wait) && !done && !picked) {
        int hit;
        m_game->Update();
        if (net) {
            net->ServiceNetwork();
        }
        mouse = *(HudPoint*)&DAT_0057ba00;
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            hit = HitTest(mouse, 0, 7);
            if (hit != -1 && !typing) {
                m_selected = hit;
            }
        }
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        int horiz = 0;
        int vert = 0;
        if (DAT_0057ba28[VK_ESCAPE]) {
            if (!typing) {
                done = 1;
            } else {
                name[m_selected] = scratch;
                typing = 0;
            }
        } else if (DAT_0057ba28[VK_RETURN]) {
            if (m_selected == 7) {
                done = 1;
                continue;
            }
            if (m_selected == 6) {
                m_game->Play(1);
                picked = 1;
                continue;
            }
            if (!typing) {
                typing = 1;
                scratch = name[m_selected];
            } else {
                typing = 0;
                if (CheckCheatName(name[m_selected].c_str()) || name[m_selected] == "") {
                    name[m_selected] = scratch;
                }
            }
            DAT_0056b0ac->vf09();
            if (editing[m_selected]) {
                editing[m_selected] = 0;
                A4_STR(name[m_selected])->AssignB("");
            }
        } else if (DAT_0057ba28[VK_LEFT]) {
            horiz = -1;
        } else if (DAT_0057ba28[VK_RIGHT]) {
            horiz = 1;
        } else if (DAT_0057ba28[VK_DOWN]) {
            vert = 1;
            typing = 0;
        } else if (DAT_0057ba28[VK_UP]) {
            vert = -1;
            typing = 0;
        } else if (m_selected < rowCount && typing && ((A4_Device*)DAT_0056b0ac)->pending > 0) {
            unsigned char c = DAT_0056b0ac->ReadChar();
            if (c == 8) {
                int n = (int)name[m_selected].length() - 1;
                if (n >= 0) {
                    A4_STR(name[m_selected])->Resize(n);
                }
            } else if ((c >= 0x21 && c <= 0x7e) || (c >= 0xc0 && c <= 0xff) || c == 0x20) {
                if (name[m_selected].length() == 0x10) {
                    A4_STR(name[m_selected])->Resize(0xf);
                }
                A4_Str* t = A4_STR(name[m_selected]); t->AppendFill(1, c);
            }
        }
        hit = HitTest(mouse, 0, 7);
        if (hit != -1 && !typing) {
            if (m_selected != hit) {
                m_game->Play(0);
            }
            m_selected = hit;
        }
        m_selected += vert;
        for (;;) {
            if (m_selected <= rowCount - 1 || m_selected >= 6) {
                break;
            }
            m_selected += vert;
        }
        if (m_selected < 0) {
            m_selected = 7;
        } else if (m_selected > 7) {
            m_selected = 0;
        }
        if (vert != 0) {
            m_game->Play(0);
            float cx = m_items[m_selected]->GetTopLeft().x +
                       (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f;
            float cy = m_items[m_selected]->GetTopLeft().y +
                       (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f;
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&A4_WarpPoint(cx * sx, cy * sy));
        }
        if (DAT_0057ba20) {
            if (m_items[7]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                done = 1;
                continue;
            }
            if (m_items[6]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                picked = 1;
                continue;
            }
            int k = 8;
            for (int i = 0; i < rowCount; i++) {
                if (m_items[k]->Contains(mouse)) {
                    horiz--;
                }
                k++;
                if (m_items[k]->Contains(mouse)) {
                    horiz++;
                }
                k++;
            }
            if (horiz == 0 && !typing) {
                hit = HitTest(mouse, 0, rowCount - 1);
                if (hit != -1 && 762.0f * DAT_0056aea8 > mouse.x) {
                    typing = 1;
                    if (editing[m_selected]) {
                        editing[m_selected] = 0;
                        A4_STR(name[m_selected])->AssignC("");
                    }
                }
            }
        }
        if (horiz != 0) {
            switch (m_selected) {
            case 0:
                AdjustValue(&icon[0], horiz, 0, (int)(A4_ICONS - A4_ONE));
                break;
            case 1:
                AdjustValue(&icon[1], horiz, 0, (int)(A4_ICONS - A4_ONE));
                break;
            case 2:
                AdjustValue(&icon[2], horiz, 0, (int)(A4_ICONS - A4_ONE));
                break;
            case 3:
                AdjustValue(&icon[3], horiz, 0, (int)(A4_ICONS - A4_ONE));
                break;
            case 4:
                AdjustValue(&icon[4], horiz, 0, (int)(A4_ICONS - A4_ONE));
                break;
            case 5:
                AdjustValue(&icon[5], horiz, 0, (int)(A4_ICONS - A4_ONE));
                break;
            }
        }
        MenuGame* game = m_game;
        game->UpdateVideo();
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            game->DrawVideo(0);
            text->scaleX = DAT_0056aea8 * 2.0f;
            text->color = DAT_00508bfc;
            text->scaleY = DAT_0056aeac * 2.0f;
            text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x39), 0, 1.0f, 1.0f);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            DrawPlate(433.0f, 242.0f);
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, rowCount + 1, 1, 1, 1, -1.0f);
            for (int i = 0; i < 8; i++) {
                if (i < 6 && i >= rowCount) {
                    continue;
                }
                int selected = (i == m_selected);
                Color col;
                col.value = selected ? DAT_00508c0c : DAT_00508c04;
                float ly = labels[i].y;
                float lx = labels[i].x;
                text->color = col.value;
                text->Print(DAT_0056b0ac, lx, ly, labels[i].text.c_str(), 0, 1.0f, 1.0f);
                if (i < rowCount) {
                    nameText->scaleX = DAT_0056aea8 * 1.5f;
                    ly = positions[i].y;
                    lx = positions[i].x;
                    nameText->color = col.value;
                    float s = DAT_0056aea8 * 490.0f / nameText->GetWidth(name[i].c_str(), 0);
                    if (s > 1.5f) {
                        s = 1.5f;
                    }
                    nameText->scaleX = s * DAT_0056aea8;
                    nameText->scaleY = DAT_0056aeac * 1.5f;
                    nameText->Print(DAT_0056b0ac, lx, ly, name[i].c_str(), 0, 1.0f, 1.0f);
                    nameText->scaleX = DAT_0056aea8 * 1.5f;
                    nameText->scaleY = DAT_0056aeac * 1.5f;
                    if (i == m_selected && typing) {
                        int elapsed = GetTickCount() - blinkTick;
                        if (elapsed >= 80 && !blinkOn) {
                            blinkTick = GetTickCount();
                            blinkOn = 1;
                        } else if (elapsed >= 120 && blinkOn) {
                            blinkTick = GetTickCount();
                            blinkOn = 0;
                        }
                        if (blinkOn) {
                            nameText->scaleX = DAT_0056aea8 * 1.5f;
                            float s2 = DAT_0056aea8 * 490.0f / nameText->GetWidth(name[i].c_str(), 0);
                            if (s2 > 1.5f) {
                                s2 = 1.5f;
                            }
                            nameText->scaleX = s2 * DAT_0056aea8;
                            lx = nameText->GetWidth(name[i].c_str(), 0) + lx;
                            col.value = 0xffffff;
                            DrawTextCursor(DAT_0056b0ac, lx, ly, col);
                            nameText->scaleX = DAT_0056aea8 * 1.5f;
                        }
                    }
                }
                HudPoint cell;
                cell.x = 762.0f * DAT_0056aea8;
                cell.y = ly - DAT_0056aeac * 4.0f;
                if (i < rowCount) {
                    col.value = selected ? DAT_00508c0c : DAT_00508c04;
                    text->color = col.value;
                    DrawIconCell(text, &cells[icon[i]], cell.x, cell.y, selected, icon[i], 0, (int)A4_ICONS);
                }
            }
            DrawCursor(mouse);
            DAT_0056b0ac->EndScene();
            DAT_0056b0ac->Present();
        }
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    int r = m_selected;
    FUN_00438d00();
    ((A4_Device*)DAT_0056b0ac)->busy = 0;
    for (int i = 0; i < 6; i++) {
        memcpy((char*)DAT_00501300 + 0x38 + i * 30, name[i].c_str(), strlen(name[i].c_str()) + 1);
        DAT_00501300[0x3b + i] = icon[i];
    }
    nameText->SetScale(DAT_0056aea8, DAT_0056aeac);
    if (done || DAT_0057b9f4) {
        r = -1;
        m_game->Play(2);
    }
    return r;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// NONMATCHING: RALLYTROPHY 0x43ee70
// 1415 vs 1415 bytes, instruction-identical; only stack-slot numbers differ (34 bytes): the original shares one slot
// between the loop counter's fild temp and the (i + 1) temp ('fild [T]; mov [T],ebp; fild [T]') and keeps `this` at
// +0x1c / T at +0x18; mine gives (i + 1) its own temp (hoisted store) and swaps `this`/T.
// original: FUN_0043ee70 (builds the hit rectangles: 6 name rows, Continue, Back, 2 arrow rects per row)
void PlayerNameMenu::BuildRects()
{
    float top;
    int i;
    UIRect* rect;
    A3_Point a;
    HudPoint b;
    top = A3_TOP - 10.0f;
    for (i = 0; i < 6; i++) {
        b.x = A3_X + 915.0f;
        a.x = A3_X;
        a.y = i * A3_PITCH + top;
        b.y = (i + 1) * A3_PITCH + top;
        rect = (UIRect*)new A3_Rect(a, b);
        if (i >= rowCount)
            rect->enabled = 0;
        m_items.push_back(rect);
    }
    a.x = A3_X;
    b.x = A3_X + 420.0f;
    a.y = rowCount * A3_PITCH + top;
    b.y = (rowCount + 1) * A3_PITCH + top;
    rect = (UIRect*)new A3_Rect(a, b);
    m_items.push_back(rect);
    b.x = A3_X + 200.0f;
    a.x = A3_X;
    a.y = A3_BACKY - 10.0f;
    b.y = A3_BACKY + 42.0f;
    rect = (UIRect*)new A3_Rect(a, b);
    m_items.push_back(rect);
    for (int j = 0; j < rowCount; j++) {
        b.x = A3_ARROWX + 32.0f;
        a.x = A3_ARROWX;
        a.y = j * A3_PITCH + top;
        b.y = (j + 1) * A3_PITCH + top;
        rect = (UIRect*)new A3_Rect(a, b);
        if (j >= rowCount)
            rect->enabled = 0;
        m_items.push_back(rect);
        a.x = A3_ARROWX + 96.0f;
        b.x = A3_ARROWX + 128.0f;
        a.y = j * A3_PITCH + top;
        b.y = (j + 1) * A3_PITCH + top;
        rect = (UIRect*)new A3_Rect(a, b);
        if (j >= rowCount)
            rect->enabled = 0;
        std::vector<UIRect*>& v = m_items;
        v.insert(v.end(), rect);
    }
}

// FUNCTION: RALLYTROPHY 0x43f400
// original: FUN_0043f400 (thiscall that reads only this->game; the by-value Color parameter gives EH state 0)
void PlayerNameMenu::DrawTextCursor(RenderDevice* device, float x, float y, Color color)
{
    void* material = ((GameView*)m_game)->FUN_00432a00();
    HudPoint uv[4];
    HudPoint pos[4];
    uv[0].x = uv[3].x = 0.25f;
    uv[0].y = uv[1].y = 0.296875f;
    uv[1].x = uv[2].x = 0.2890625f;
    uv[2].y = uv[3].y = 0.375f;
    pos[0].x = x;
    pos[0].y = y;
    pos[2].x = pos[1].x = DAT_0056aea8 * 12.0f + x;
    pos[1].y = y;
    pos[3].y = pos[2].y = DAT_0056aeac * 24.0f + y;
    pos[3].x = x;
    device->vf35((Sprite*)material);
    ((RaceDeviceSlots*)device)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x43f570
void PlayerNameMenu::SetupLabels()
{
    rowCount = DAT_00501334;
    labels[6].text = FUN_00405380(DAT_004f8f1c[0]);
    labels[6].x = DAT_0056aea8 * 54.0f;
    labels[6].y = (rowCount * 42.0f + 262.0f) * DAT_0056aeac;
    labels[7].text = FUN_00405380(DAT_004f8f1c[1]);
    labels[7].x = DAT_0056aea8 * 54.0f;
    labels[7].y = 610.0f * DAT_0056aeac;
    blinkTick = timeGetTime();
}

// FUNCTION: RALLYTROPHY 0x43f740
// Evidence: ev_458b690eef42d1cb1d8756b8afed275ba2a1f4a75d7b04d9eb77dc29442b42b0.
// Construct Color after the arrays; assign it later to preserve EH and store scheduling.
void PlayerNameMenu::DrawPlate(float x, float y)
{
    HudPoint uv[4];
    HudPoint pos[4];
    Color color;
    pos[0].x = pos[3].x = DAT_0056aea8 * x;
    uv[0].x = uv[3].x = 0.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[0].y = pos[1].y = DAT_0056aeac * y;
    color.value = -1;
    pos[1].x = pos[2].x = (x + 512.0f) * DAT_0056aea8;
    pos[2].y = pos[3].y = (y + 256.0f) * DAT_0056aeac;
    DAT_0056b0ac->vf35(plate);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x43f8b0
// original: FUN_0043f8b0 (thiscall that never reads this; the two strcmp loops are the inlined intrinsic)
int PlayerNameMenu::CheckCheatName(const char* name)
{
    if (strcmp(name, "KALJAKOPPA") == 0) {
        DAT_00501498[0xd] = -1;
        DAT_00501498[0xe] = -1;
        DAT_005014c8 = 1;
        DAT_005014c9 = 1;
        DAT_00501498[0] = 5;
        DAT_00501498[1] = 5;
        for (int i = 2; i < 7; i++) {
            DAT_00501498[i] = 6;
            DAT_00501498[i + 5] = 2;
        }
        DAT_00501498[2] = 8;
        return 1;
    }
    if (strcmp(name, "QUALIFY") == 0) {
        DAT_00501480 = DAT_00501480 == 0;
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x43f9b0
// SYMBOL: _$E23
typedef void Generated_43f9b0();

// FUNCTION: RALLYTROPHY 0x43f9d0
// SYMBOL: _$E22
typedef void Generated_43f9d0();
