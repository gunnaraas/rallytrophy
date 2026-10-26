// Race session (in-race game loop object, 0x491fb0-0x4952af): the small
// functions. Types are in include/unk_481390.h.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern ConfigKey DAT_004ffab0[];       // data/settings/video.ini keys
void FUN_00457b10();                   // loads the camera presets (data/settings/camera.txt, src/unk_4563e0.cpp)
extern int DAT_00508d94, DAT_00508d98, DAT_00508d9c;
extern int DAT_00508db4, DAT_00508db8, DAT_00508dbc;

// FUNCTION: RALLYTROPHY 0x491fb0
RaceSession::RaceSession()
{
    m_unk34 = 0;
    FUN_00457b10();
    m_countdown = 0;
    m_fps = 25;
    m_unk2c = 1;
    m_unk28 = 0;
    m_unk20 = 0;
    m_followed = 0;
    Config config;
    config.Define(DAT_004ffab0, 1);
    config.Load("data/settings/video.ini", true, false, 0);
    m_fps = config.GetInt("VideoOutFps");
    config.Close();
}

// FUNCTION: RALLYTROPHY 0x492060
void RaceSession::Shutdown()
{
    delete m_unk28;
    m_unk28 = 0;
    DestroyCountdown();
}

// FUNCTION: RALLYTROPHY 0x492090
void RaceSession::CreateCountdown()
{
    if (!m_countdown) {
        m_countdown = new Sprite("CountDown");
        m_countdown->texture = new Texture("Data/Global/Overlay/CountDown.tga", 0x840, 1);
        m_countdown->texture->Upload(DAT_0056b0ac);
        m_countdown->unk9c = 2;
        m_countdown->unk98 = 15;
        m_countdown->unk18 = 5;
        m_countdown->unk1c = 6;
        m_countdown->unk60[0] = 1.0f;
        m_countdown->unk60[1] = 1.0f;
        m_countdown->unk60[2] = 1.0f;
        m_countdown->unk60[3] = 1.0f;
        m_countdown->unk20 = 2;
        m_countdown->unk40[0] = 1.0f;
        m_countdown->unk40[1] = 1.0f;
        m_countdown->unk40[2] = 1.0f;
        m_countdown->unk40[3] = 1.0f;
        m_countdown->unk24 = 4;
    }
}

// FUNCTION: RALLYTROPHY 0x492190
void RaceSession::DestroyCountdown()
{
    if (m_countdown) {
        delete m_countdown->texture;
        delete m_countdown;
        m_countdown = 0;
    }
}

struct StageProgress {                 // DAT_0056b0c8 + 0x3680, 0x5e8 bytes each
    int best;
    BYTE unk4[0x5e8 - 4];
};

// FUNCTION: RALLYTROPHY 0x494c80
void RaceSession::RecordStageProgress()
{
    if (!DAT_00501454) {
        int stage = m_mode->unk28;
        int progress = m_mode->unk2c;
        if (DAT_00501458) {
            if (stage > DAT_00501498[1]) {
                DAT_00501498[1] = stage;
            }
            int delta = progress - ((StageProgress*)((char*)DAT_0056b0c8 + 0x3680))[stage].best;
            if (delta > DAT_00501498[stage + 6]) {
                DAT_00501498[stage + 6] = delta;
            }
        } else {
            if (stage > DAT_00501498[0]) {
                DAT_00501498[0] = stage;
            }
            if (progress > DAT_00501498[stage + 1]) {
                DAT_00501498[stage + 1] = progress;
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x494f00
void RaceSession::UpdateViewport(int keep)
{
    RaceMode* mode = m_mode;
    int count = mode->m_players.size();
    if (count) {
        if (keep) {
            Player* local = mode->m_localPlayer;
            for (int i = 0; i < (int)mode->m_players.size(); i++) {
                if (mode->m_players[i] == local) {
                    m_followed = i;
                    break;
                }
            }
        }
        if (m_followed >= count) {
            m_followed = 0;
        } else if (m_followed < 0) {
            m_followed = count - 1;
        }
        Player* player = mode->m_players[m_followed];
        Car* car = player->car;
        ViewManager* view = m_view;
        view->local = player->local != 0;
        DAT_00508db4 = car->unkff4;
        DAT_00508db8 = car->unkff8;
        DAT_00508dbc = car->unkffc;
        DAT_00508d94 = car->unk1000;
        DAT_00508d98 = car->unk1004;
        DAT_00508d9c = car->unk1008;
        view->FUN_00458150(car->unk100c);
        view->FUN_00456660(car, m_stage);
        view->FUN_004566e0();
    }
}

// FUNCTION: RALLYTROPHY 0x495000
void RaceSession::PickBestPlayer()
{
    RaceMode* mode = m_mode;
    int count = mode->m_players.size();
    if (count) {
        int best = 1000;
        for (int i = 0; i < (int)mode->m_players.size(); i++) {
            Player* player = mode->m_players[i];
            if (!player->local && player->active && !player->unk30 && player->unk1a18 < best) {
                best = player->unk1a18;
                m_followed = i;
            }
        }
        UpdateViewport(0);
    }
}

// NONMATCHING: RALLYTROPHY 0x495080
// Same size (549); only the placement of `push edi` in the prologue differs (6 bytes). `int active =
// mode->unk40` already fixes the ecx/eax choice; the push order resisted ~25 source variants.
void RaceSession::DrawCountdown(RaceMode* mode)
{
    int active = mode->unk40;
    if (active) {
        int time = mode->m_time;
        if (time >= 1000) {
            DestroyCountdown();
            return;
        }
        CreateCountdown();
        int ms = time + 3000;
        if (ms >= 0 && ms < 4000) {
            int fraction = ms % 1000;
            int second = ms / 1000;
            Vec2 position[4];
            Vec2 uv[4];
            float u = 0.0f;
            float v = 0.0f;
            if (second & 1) {
                u = 0.5f;
            }
            if (second & 2) {
                v = 0.5f;
            }
            uv[0].y = uv[1].y = v;
            uv[0].x = uv[3].x = u;
            uv[1].x = uv[2].x = u + 0.5f;
            uv[2].y = uv[3].y = v + 0.5f;
            float t = fraction * 0.001f;
            float g = t * 320.0f;
            float h = t * 320.0f;
            position[0].x = position[3].x = 480.0f - g;
            position[0].y = position[1].y = 352.0f - h;
            position[1].x = position[2].x = g + 544.0f;
            position[2].y = position[3].y = h + 416.0f;
            for (int i = 0; i < 4; i++) {
                position[i].x *= DAT_0056aea8;
                position[i].y *= DAT_0056aeac;
            }
            m_countdown->unk40[3] = t;
            Color color;
            color.value = 0xffffffff;
            float scaled = t * 255.5f;
            int alpha;
            __asm {
                fld scaled
                fistp alpha
            }
            color.a = 255 - (BYTE)alpha;
            ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(m_countdown);
            ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
        }
    }
}

// The 4x4 identity matrix the session passes to the car placement (0x45cc30);
// built inline, with an empty destructor (EH state 0 of the loop).
struct IdentityMatrix {
    IdentityMatrix()
    {
        m[3] = m[2] = m[1] = 0.0f;
        m[7] = m[6] = m[4] = 0.0f;
        m[11] = m[9] = m[8] = 0.0f;
        m[14] = m[13] = m[12] = 0.0f;
        m[15] = m[10] = m[5] = m[0] = 1.0f;
    }
    ~IdentityMatrix() {}
    float m[16];
};

// A virtual object seen only through its slot 1 (the view manager and the draw
// context have a vtable that the module-local views above hide in unk0).
struct VirtualObject {
    virtual void Slot0();
    virtual void Slot1(void* argument);
};

// The folded empty function 0x48e660 called without an object, and the empty
// debug overlay at DAT_0056aeb0.
void FUN_0048e660_plain();
class DebugOverlay {
public:
    void FUN_0048e660();               // 0x48e660 (empty)
    void FUN_004031e0(TextRenderer* text, int a, int b); // 0x4031e0 (empty, ret 0xc)
};
extern DebugOverlay DAT_0056aeb0;

// Sets the viewport clear/fog colour from a view (inlined at both draw passes).
inline void SetViewColor(RaceDeviceSlots* device, float x, float y, float z)
{
    device->SetClear(z, 1.0f, x, y, 1);
}

#define DEVICE ((RaceDeviceSlots*)DAT_0056b0ac)

// NONMATCHING: RALLYTROPHY 0x493650
// Logic complete (all 11 menu states, replay commands, record/award flags, screenshots), ~87% of instructions
// identical after masking registers/stack slots; built 5608 vs 5672 bytes (the orig has two ladder copies for
// states 1/9 sharing tails differently), local slot order and awards/ebx allocation differ. No jump-table bytes
// are counted in the 5605 listed size (tables at 0x494c38 / 0x494c4c follow the code).
int RaceSession::FUN_00493650(int demo)
{
    int quit = 0;
    int result = 1;
    int wait = 0;
    m_view->target = 0;
    m_view->local = 1;
    Car* car = m_car;
    DAT_00508db4 = car->unkff4;
    DAT_00508db8 = car->unkff8;
    DAT_00508dbc = car->unkffc;
    DAT_00508d94 = car->unk1000;
    DAT_00508d98 = car->unk1004;
    DAT_00508d9c = car->unk1008;
    m_view->FUN_00458150(car->unk100c);
    IdentityMatrix world;
    car->FUN_0048e660();
    car->FUN_0045cc30(Vec3(0.0f, 5.0f, 3.0f), (struct IdentityMatrix*)&world);

    m_unk18 = 0;
    m_followed = 0;
    m_unk1b = 0;
    int view;
    if (demo) {
        view = -1;
    } else {
        view = DAT_004ffab8[DAT_00501328];
    }
    UpdateViewport(1);
    m_view->FUN_00456780(view);
    m_view->FUN_004566e0();
    DEVICE->SetRenderState(1, 2);
    SetViewColor(DEVICE, m_view->nearPlane, m_view->farPlane, m_view->fov);
    DEVICE->Slot41(5, 0.04f, 0.49f);
    DEVICE->SetRenderState(5, 1);
    DEVICE->Slot42(4);
    DEVICE->Slot23(1.0f);

    float ring[8];
    memset(ring, 0, sizeof(ring));
    int ringIndex = 0;
    DAT_0056b0bc->ClearKeys();
    Player* local = m_mode->m_localPlayer;
    m_unk34->unk0 = local->unk24;
    DWORD accum = 0;
    int timer = 0xdac;
    int phase = 0;
    int state;
    int first;
    int startPending;
    if (demo) {
        state = 2;
        first = 1;
        startPending = 1;
        DAT_0050146c = 1;
        m_mode->unk23c = -1;
    } else {
        state = 7;
        first = 1;
        m_unk18 = 0;
        startPending = 1;
        DAT_0050146c = 0;
    }
    Timer clock;
    clock.Reset();
    if (DAT_00501454 && !DAT_00501470) {
        m_unk18 = 0;
    }
    m_mode->FUN_0048e720();
    m_stage->FUN_00416010();
    int speech = 0;
    quit = 0;
    m_mode->Slot4(demo);
    m_view->FUN_00456780(view);
    m_unk34->FUN_00458940();
    int awards;
    int soundTimer = 0;
    int resumed = 1;
    int lastSpeed = -1;
    view = -1;
    while (FUN_004b69f0(wait)) {
        {
            if (quit || phase == 2) {
                break;
            }
            if (resumed) {
                resumed = 0;
                DAT_0056b0bc->ClearKeys();
                DAT_0057ba20 = 0;
                DAT_0057ba24 = 0;
            }
            if (startPending) {
                m_mode->Reset(demo);
                startPending = 0;
            }
            DAT_0056b0c8->FUN_00432c00();
            if (!DAT_0056b0ac->m_active) {
                DAT_0056b0b0->SetVolume(0);
                m_unk18 = 1;
                m_unk34->FUN_004588d0();
                DAT_0056b0b0->Update();
                if (state == 0) {
                    state = 1;
                    first = 1;
                }
                if (!DAT_00501454 || DAT_00501470) {
                    wait = (int)DAT_0056b0ac;
                    continue;
                }
            }
            if (wait) {
                clock.Update();
                accum = 0;
                wait = 0;
                resumed = 1;
                continue;
            }
            if (view != -1 && demo) {
                switch (view) {
                case 0:
                    m_mode->Slot11(0, 0);
                    m_mode->Reset(demo);
                    m_view->FUN_004566e0();
                case 1:
                    DAT_0050146c = 1;
                    break;
                case 2:
                    DAT_0050146c = 2;
                    break;
                case 3:
                    DAT_0050146c = 3;
                    break;
                case 4:
                    DAT_0050146c = 4;
                    break;
                }
                view = -1;
            }
            if (DAT_0057ba28[VK_ESCAPE] && !state) {
                state = 1;
                first = 1;
                m_unk18 = 1;
                DAT_0056b0b0->SetVolume(0);
                m_unk34->FUN_004588d0();
            }
            if (!demo) {
                if (!speech && m_mode->unk40) {
                    speech = 1;
                    DAT_0056b0b0->QueueSpeech(0x20, 0, 0);
                }
                int v = m_mode->unk7ac;
                if (lastSpeed != v) {
                    lastSpeed = v;
                    if (v == 1) {
                        DAT_0056b0b0->QueueSpeech(0x42, 1, 0);
                    }
                }
            }
            if (!m_mode->IsJoined()) {
                phase = 1;
            }
            clock.Update();
            DWORD elapsed = clock.m_delta;
            if (m_unk20) {
                elapsed = 1000 / m_fps;
            }
            int speed = DAT_0050146c;
            DWORD total = accum + elapsed;
            DWORD chunk = speed != 3 ? 4 : 16;
            if (total < chunk) {
                accum = total;
                continue;
            }
            accum = total % chunk;
            total -= accum;
            int dt = total;
            if (speed == 3) {
                total >>= 2;
                dt = total;
            } else if (speed == 4) {
                total <<= 2;
                dt = total;
            }
            DAT_0056b0d0 += dt;
            float dtSec = (float)total * 0.001f;
            m_stage->FUN_004160c0(dt);
            if (demo) {
                if (m_mode->m_time + dt >= m_car->unk1bef4) {
                    if (m_unk30) {
                        phase = 2;
                    } else {
                        m_mode->Slot11(0, 0);
                        m_mode->Reset(demo);
                        m_view->FUN_004566e0();
                    }
                }
            }
            DEVICE->SetRenderState(0xd, 0);
            DEVICE->Slot24(2);
            if (state != 0xb) {
                HandleCameraKeys(m_view, demo);
            }
            if (!demo) {
                CarInput* input = m_unk34;
                if (input->cameraButton && !DAT_00565504) {
                    if (++DAT_00501328 > 5) {
                        DAT_00501328 = 0;
                    }
                    m_view->FUN_00456780(DAT_004ffab8[DAT_00501328]);
                }
                if (input->resetButton && m_mode->unk40 && m_mode->m_time >= 1000) {
                    CarBody* body = car->body;
                    if (body->velocity[0] * body->velocity[0] + body->velocity[1] * body->velocity[1] + body->velocity[2] * body->velocity[2] <= 1.0f) {
                        m_mode->Slot13(local);
                    }
                }
            }
            ring[ringIndex++] = (float)elapsed * 0.001f;
            ringIndex %= 8;
            if (!m_unk18) {
                CarInput* input = m_unk34;
                input->FUN_00458940();
                if (!demo) {
                    RaceMode* mode = m_mode;
                    m_unk34->FUN_00458b30(car, dt, mode->unk4c);
                    if (mode->unk54) {
                        car->unk1338 = 0;
                    }
                } else {
                    input->FUN_00458ad0();
                }
            }
            if (!m_unk18 || DAT_00501454) {
                if (DAT_0050146c != 2) {
                    m_mode->Update(dt);
                }
            }
            ViewManager* v = m_view;
            ((VirtualObject*)v)->Slot1(0);
            v->FUN_004b78f0();
            SetViewColor(DEVICE, v->nearPlane, v->farPlane, v->fov);
            DEVICE->SetMatrix(v->m0, 1);
            DrawContext* ctx = m_unk0c;
            ctx->ClearViews();
            FUN_0048e660_plain();
            Stage* stage = m_stage;
            DAT_00581de4 = ctx;
            stage->FUN_00411170(v, ctx);
            ctx->unk188 = stage->unk1c4;
            ctx->unk190 = stage->unk1cc;
            ctx->unk18c = stage->unk1c8;
            ctx->unk174 = stage->unk1a8;
            ctx->unk17c = stage->unk1b0;
            ctx->unk178 = stage->unk1ac;
            ctx->unk180 = stage->unk1b4;
            ctx->unk184 = DAT_00501434 ? stage->unk1a4 : 0;
            ctx->views.Add(v);
            RaceMode* mode = m_mode;
            mode->DrawSetup((int)ctx, (ViewPoint*)v);
            ctx->Slot1(v);
            ctx->FUN_004b7c80(DEVICE, v);
            car->FUN_00464c10(v);
            mode->DrawCars((int)ctx, (int)v, dt);
            if (DEVICE->BeginScene()) {
                DEVICE->SetMatrix(v->m0, 1);
                SetViewColor(DEVICE, v->nearPlane, v->farPlane, v->fov);
                m_stage->FUN_00411350(DEVICE, v);
                mode->FUN_0048e7b0(dt);
                mode->DrawShadows((int)ctx, (int)v);
                ctx->FUN_004b8160(DEVICE, v);
                mode->DrawLights((int)ctx, (int)v);
                mode->FUN_00488880((ViewPoint*)v);
                DEVICE->Slot44(1);
                DEVICE->Slot32(3);
                m_stage->FUN_00411460(DEVICE, v, ctx, dtSec);
                DEVICE->SetRenderState(5, 1);
                DEVICE->Slot42(4);
                ctx->FUN_004b82f0(DEVICE, v);
                DEVICE->Slot32(3);
                DEVICE->SetRenderState(5, 0);
                DEVICE->Slot42(8);
                DEVICE->SetRenderState(0xd, 0);
                m_stage->FUN_00411490(DEVICE, v, ctx, dtSec);
                DEVICE->Slot32(3);
                if (!demo && !m_unk1b) {
                    car->FUN_00464ef0(dtSec);
                    car->FUN_0046a200();
                    car->FUN_0046a850();
                    if (car->unk1f138) {
                        mode->FUN_00486f80();
                    }
                    mode->FUN_00487080();
                }
                DEVICE->Slot32(3);
                DEVICE->Slot38(0);
                float sum = 0.0f;
                for (int i = 0; i < 8; i++) {
                    sum += ring[i];
                }
                float fps = 8.0f / sum;
                if (!demo) {
                    DrawCountdown(mode);
                }
                int fpsInt;
                __asm {
                    fld fps
                    fistp fpsInt
                }
                TextRenderer* text = DAT_0056b0c8->text;
                text->color = 0xffffffff;
                if (!m_unk30 && !demo && !m_unk1b) {
                    mode->Draw();
                    mode->Slot16();
                    mode->DrawOverlay();
                }
                DAT_0056aeb0.FUN_0048e660();
                DAT_0056aeb0.FUN_004031e0(text, 0, 0);
                DAT_0056aeb0.FUN_0048e660();
                DEVICE->Slot4();
                if (state) {
                    switch (state) {
                    case 7:
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        if (m_mode->FUN_004890e0(first) == 1) {
                            m_unk34->FUN_00458940();
                            state = 0;
                            m_unk18 = 0;
                        }
                        first = 0;
                        break;
                    case 2: {
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        RaceMode* m = m_mode;
                        m_unk18 = 0;
                        int r = m->FUN_0048b620(first);
                        int command = m->unk23c;
                        m->unk23c = -1;
                        if (command != -1) {
                            view = command;
                        }
                        if (r == 1) {
                            if (DAT_00501470) {
                                m_unk34->FUN_00458940();
                                m_unk18 = 0;
                                state = 0;
                                quit = 1;
                                result = 1;
                                break;
                            }
                            state = 9;
                            if (!DAT_00501454 && !DAT_00501464 && DAT_00565500 && DAT_005654fc) {
                                int bestRank = 1000;
                                first = 0;
                                awards = 0;
                                view = 1000;
                                if (DAT_00501458) {
                                    Player* p = m->m_localPlayers[0];
                                    if (p->unk1a5c < view) {
                                        view = p->unk1a5c;
                                    }
                                    if (p->unk1a60 < bestRank) {
                                        bestRank = p->unk1a60;
                                    }
                                    if (view > 3 && DAT_00501480) {
                                        DAT_00565508 = 1;
                                        state = 10;
                                        awards = 0;
                                    } else {
                                        if (m_unk1a) {
                                            first = DAT_0056b0c8->FUN_00452380(bestRank, p);
                                            if (bestRank == 1) {
                                                awards = 1;
                                            } else {
                                                switch (DAT_0050132c) {
                                                case 1:
                                                    if (bestRank <= 2) {
                                                        awards = 2;
                                                    }
                                                    break;
                                                case 2:
                                                    if (bestRank <= 3) {
                                                        awards = 4;
                                                    }
                                                    break;
                                                }
                                            }
                                        } else {
                                            awards = 0x10;
                                            if (view == 1) {
                                                awards = 0x11;
                                            } else if (view <= 3) {
                                                awards = 0x14;
                                            }
                                        }
                                        if (first & 1) {
                                            awards |= 0x20;
                                        }
                                        if (first & 2) {
                                            awards |= 0x40;
                                        }
                                        if (awards) {
                                            state = 8;
                                        }
                                    }
                                } else {
                                    for (int i = 0; i < (int)m->m_localPlayers.size(); i++) {
                                        Player* p = m->m_localPlayers[i];
                                        if (p->unk1a5c < view) {
                                            view = p->unk1a5c;
                                        }
                                        if (p->unk1a60 < bestRank) {
                                            bestRank = p->unk1a60;
                                        }
                                        if (m_unk1a) {
                                            first |= DAT_0056b0c8->FUN_00452470(p->unk1a60, p);
                                        }
                                    }
                                    if (first & 1) {
                                        awards = 0x20;
                                    }
                                    if (first & 2) {
                                        awards |= 0x40;
                                    }
                                    if (m_unk1a) {
                                        if (bestRank == 1) {
                                            awards |= 1;
                                        } else {
                                            switch (DAT_0050132c) {
                                            case 1:
                                                if (bestRank <= 3) {
                                                    awards |= 4;
                                                }
                                                break;
                                            case 2:
                                                if (bestRank <= 6) {
                                                    awards |= 8;
                                                }
                                                break;
                                            }
                                        }
                                    } else {
                                        awards |= 0x10;
                                        if (view == 1) {
                                            awards |= 1;
                                        } else if (view <= 6) {
                                            awards |= 8;
                                        }
                                    }
                                    if (view > 6 && DAT_00501480) {
                                        DAT_00565508 = 2;
                                        state = 10;
                                        awards = 0;
                                    } else if (awards) {
                                        state = 8;
                                    }
                                }
                            }
                            first = 1;
                            view = -1;
                        } else if (r == -1) {
                            view = -1;
                            if (!DAT_00501470) {
                                state = 0xb;
                                first = 1;
                            } else {
                                first = 0;
                            }
                        } else {
                            first = 0;
                        }
                        break;
                    }
                    case 11:
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        m_unk18 = 0;
                        if (m_mode->NameMenu(first) == 1) {
                            state = 2;
                            first = 1;
                        } else {
                            first = 0;
                        }
                        break;
                    case 3:
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        m_unk18 = 0;
                        if (m_mode->FUN_004899a0(first) == 1) {
                            if (DAT_00501464 == 3) {
                                quit = 1;
                                result = 1;
                                state = 0;
                            } else {
                                state = 4;
                                first = 1;
                            }
                        } else {
                            first = 0;
                        }
                        break;
                    case 4:
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        m_unk18 = 0;
                        if (m_mode->FUN_0048a070(first) == 1) {
                            if (DAT_00565500) {
                                state = 5;
                                first = 1;
                            } else {
                                quit = 1;
                                result = 1;
                                state = 0;
                                first = 0;
                            }
                        } else {
                            first = 0;
                        }
                        break;
                    case 5:
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        m_unk18 = 0;
                        if (m_mode->FUN_0048a920(first) == 1) {
                            if (DAT_005654fc) {
                                state = 6;
                                first = 1;
                            } else {
                                quit = 1;
                                result = 1;
                                state = 0;
                                first = 0;
                            }
                        } else {
                            first = 0;
                        }
                        break;
                    case 6:
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        m_unk18 = 0;
                        if (m_mode->FUN_0048b080(first) == 1) {
                            quit = 1;
                            result = 1;
                            state = 0;
                        } else {
                            first = 0;
                        }
                        break;
                    case 1: {
                        if (demo) {
                            m_unk18 = 0;
                        }
                        int r = m_mode->FUN_00489300(first);
                        if (r == -2) {
                            quit = 1;
                            result = -2;
                            DAT_0056b0b0->SetVolume(0x7f);
                            first = 0;
                        } else if (r == -1) {
                            quit = 1;
                            result = -1;
                            DAT_0056b0b0->SetVolume(0x7f);
                            first = 0;
                        } else if (r == 1) {
                            m_unk34->FUN_00458940();
                            m_unk18 = 0;
                            state = 0;
                            if (demo) {
                                quit = 1;
                                result = 1;
                                DAT_0056b0b0->SetVolume(0x7f);
                                first = 0;
                            } else {
                                DAT_0056b0b0->SetVolume(0x7f);
                                first = 0;
                            }
                        } else if (r == 0) {
                            first = 0;
                        } else {
                            DAT_0056b0b0->SetVolume(0x7f);
                            first = 0;
                        }
                        break;
                    }
                    case 9: {
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        m_unk18 = 0;
                        int r = m_mode->PauseMenu(first, DAT_00565500);
                        if (r == -2) {
                            quit = 1;
                            result = -2;
                            DAT_0056b0b0->SetVolume(0x7f);
                            first = 0;
                        } else if (r == -1) {
                            quit = 1;
                            result = -1;
                            DAT_0056b0b0->SetVolume(0x7f);
                            first = 0;
                        } else if (r == 1) {
                            m_unk34->FUN_00458940();
                            m_unk18 = 0;
                            state = 0;
                            if (demo) {
                                quit = 1;
                                result = 1;
                            }
                            DAT_0056b0b0->SetVolume(0x7f);
                            first = 0;
                        } else if (r == 0) {
                            first = 0;
                        } else {
                            DAT_0056b0b0->SetVolume(0x7f);
                            first = 0;
                        }
                        break;
                    }
                    case 8:
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        m_unk18 = 0;
                        if (m_mode->FUN_0048c7e0(first, (BYTE)awards) == 1) {
                            state = 9;
                            first = 1;
                        } else {
                            first = 0;
                        }
                        break;
                    case 10: {
                        if (DAT_0056b0ac->m_active) {
                            DAT_0056b0b0->SetVolume(0x7f);
                        }
                        RaceMode* m = m_mode;
                        m_unk18 = 0;
                        if (m->FUN_0048cca0(first, DAT_00565508) == 1) {
                            state = 9;
                            first = 1;
                            m->unk54 = 1;
                        } else {
                            first = 0;
                        }
                        break;
                    }
                    }
                }
                DEVICE->EndScene();
                if (DAT_0057ba28[VK_F12]) {
                    SessionObject* frame = DEVICE->GrabFrame();
                    if (frame) {
                        char name[0x100];
                        sprintf(name, "ScreenShots/rally_%i.tga", DAT_004ffaac);
                        SessionObject* image = new SessionObject(frame, 0);
                        if (image->Save(name)) {
                            DAT_004ffaac++;
                        }
                        delete image;
                        DEVICE->ReleaseFrame();
                    }
                }
                DEVICE->Present();
            }
            if (DAT_0056b0b0) {
                soundTimer -= dt;
                if (soundTimer <= 0) {
                    soundTimer = 64 - (-soundTimer) % 64;
                    ViewManager* listener = m_view;
                    DAT_0056b0b0->SetListener(*(Matrix*)listener->m0);
                    DAT_0056b0b0->SetListenerPosition(*(Vector3*)listener->pos);
                    DAT_0056b0b0->Update();
                }
            }
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            if (!demo) {
                if (phase == 0) {
                    if (local->unk30) {
                        timer = 0xdac;
                        RecordStageProgress();
                        m_mode->Slot6();
                        int speechId;
                        int splitTime = local->unk1884[10];
                        phase = 1;
                        if ((rand() & 0xff) < 0x5c) {
                            speechId = 0x21;
                        } else {
                            float limit = m_stage->unk24d8[0] * 1000.0f;
                            int limitInt;
                            __asm {
                                fld limit
                                fistp limitInt
                            }
                            if (splitTime < limitInt) {
                                speechId = rand() % 3 + 0x2c;
                            } else {
                                float limit2 = m_stage->unk24d8[1] * 1000.0f;
                                int limit2Int;
                                __asm {
                                    fld limit2
                                    fistp limit2Int
                                }
                                if (splitTime < limit2Int) {
                                    speechId = rand() % 6 + 0x26;
                                } else {
                                    speechId = rand() % 4 + 0x22;
                                }
                            }
                        }
                        if (DAT_0050147c) {
                            DAT_0056b0b0->QueueSpeech(speechId, 1, 0);
                        }
                    }
                    if (local->car->unk1c907) {
                        timer = 0x1388;
                        phase = 1;
                        m_mode->unk54 = 1;
                    }
                } else if (phase == 1) {
                    timer -= dt;
                    if (timer < 0) {
                        if (!m_unk1b) {
                            RaceMode* m = m_mode;
                            m->SendState(3);
                            m_unk1b = 1;
                            if (m->unk54) {
                                state = 10;
                                DAT_00565508 = 0;
                            } else {
                                m->SubmitRecords(1);
                                state = (DAT_00501458 != 1) + 3;
                            }
                            first = 1;
                            if (!m->Slot7()) {
                                PickBestPlayer();
                                m_view->FUN_00456780(-1);
                                m_view->FUN_004566e0();
                            }
                        } else if (DAT_00501454) {
                            m_mode->m_localPlayer->FUN_00491110(1, m_mode->m_time);
                        }
                        RaceMode* m = m_mode;
                        timer = -1;
                        if (m->Slot7() || m->unk54) {
                            m->FUN_00486200();
                        }
                    }
                }
            }
        }
    }
    m_unk34->FUN_004588d0();
    m_mode->FUN_0048e770();
    m_stage->FUN_00416050();
    if (DAT_0056b0b0) {
        DAT_0056b0b0->StopSpeech();
        DAT_0056b0b0->Update();
    }
    return result;
}

extern BYTE DAT_0057ba58[];            // camera key states (0x57ba58-0x57ba61)
extern int DAT_00501328;               // selected camera
extern BYTE DAT_00501470;              // demo/attract flag

// FUNCTION: RALLYTROPHY 0x494d00
void RaceSession::HandleCameraKeys(ViewManager* view, int flag)
{
    int allowed = (!flag && !m_unk1b) ? 0 : 1;
    if (DAT_0057ba58[1]) {
        int select = 8;
        if (!view->local) {
            select = 7;
        }
        m_view->FUN_00456780(select);
        DAT_00501328 = 0;
    } else if (DAT_0057ba58[2]) {
        m_view->FUN_00456780(6);
        DAT_00501328 = 1;
    } else if (DAT_0057ba58[3]) {
        m_view->FUN_00456780(7);
        DAT_00501328 = 2;
    } else if (DAT_0057ba58[4]) {
        m_view->FUN_00456780(0);
        DAT_00501328 = 3;
    } else if (DAT_0057ba58[5]) {
        m_view->FUN_00456780(1);
        DAT_00501328 = 4;
    } else if (DAT_0057ba58[6]) {
        m_view->FUN_00456780(2);
        DAT_00501328 = 5;
    } else if (DAT_0057ba58[7] && allowed) {
        m_view->FUN_00456780(3);
    } else if (DAT_0057ba58[8] && allowed) {
        m_view->FUN_00456780(4);
    } else if (DAT_0057ba58[9] && allowed) {
        m_view->FUN_00456780(5);
    } else if (DAT_0057ba58[0] && allowed) {
        m_view->FUN_00456780(-1);
    }
    if (m_unk1b || (flag && (DAT_00501454 || DAT_00501458)) || DAT_00501470) {
        int step = m_unk34->step;
        if (step) {
            m_followed += step;
            int count = m_mode->m_players.size();
            if (m_followed >= count) {
                m_followed = 0;
            } else if (m_followed < 0) {
                m_followed = count - 1;
            }
            if (!m_mode->m_players[m_followed]->car) {
                for (int i = 0; i < count; i++) {
                    if (step > 0) {
                        if (++m_followed >= count) {
                            m_followed = 0;
                        }
                    } else if (--m_followed < 0) {
                        m_followed = count - 1;
                    }
                    if (m_mode->m_players[m_followed]->car) {
                        break;
                    }
                }
            }
            UpdateViewport(0);
        }
    }
}

// A std::string whose append(s, n) the original calls out of line (0x403ba0) at every site.
class TitleString : public std::string {
public:
    TitleString() {}
    void AppendN(const char* s, size_t n);
    TitleString& operator+=(const char* s)
    {
        AppendN(s, strlen(s));
        return *this;
    }
};
#pragma auto_inline(off)
void TitleString::AppendN(const char* s, size_t n)
{
    append(s, n);
}
#pragma auto_inline(on)

inline void CameraName::Assign(const char* text)
{
    unsigned int n = strlen(text);
    if (Grow(n, true)) {
        memcpy(ptr, text, n);
        Eos(n);
    }
}


// A std::string (VC6 layout) whose _Tidy, _Grow and _Eos the original calls out of line
// (0x402700, 0x402980, 0x402960) while assign/append are expanded inline.
class ReplayName {
public:
    ReplayName(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~ReplayName() { Tidy(true); }
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    void Append(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            std::_Xlen();
        }
        unsigned int total = len + n;
        if (n > 0 && Grow(total, false)) {
            memcpy(ptr + len, s, n);
            Eos(total);
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

extern int DAT_00565508, DAT_005654fc, DAT_00565500;
extern float DAT_005012f8;
void FUN_004701b0(void* a);            // cdecl
class ObjB { public: void FUN_004042c0(); };
class ObjC { public: void FUN_0044e650(); };
class ObjA {
public:
    void FUN_004ae1f0(RenderDevice* device);
};

// NONMATCHING: RALLYTROPHY 0x492e40
// 2034 vs 2050 bytes; same instruction stream and calls, ~290 bytes differ in register choice and in the
// stack slot of `done` (the original reuses the dead `mode` argument slot); `pad`/`block` mimic the frame.
// Variant of Run for the alternative Game entry (0x434300): loads the stage, plays the first local player.
int RaceSession::RunAttract(RaceMode* mode)
{
    int pad[8];
    int block[4];
    DAT_00501470 = 1;
    m_unk30 = 0;
    DAT_00565508 = 0;
    m_unk19 = 0;
    m_unk1a = 0;
    LoadingLabel* label = new LoadingLabel;
    TitleString text;
    TitleString title;
    char number[8];
    int rally = mode->unk28;
    title += FUN_00405380(rally + 0xb4);
    title += " - ";
    if (!DAT_00501458) {
        title += FUN_00405380(0x1a);
        title += " ";
        _itoa(mode->unk2c, number, 10);
        title += number;
    } else {
        title += FUN_00405380(0x2a);
        title += " ";
        title += FUN_00405380(0x1b);
        title += " ";
        _itoa(mode->unk2c - ((StageProgress*)((char*)DAT_0056b0c8 + 0x3680))[rally].best, number, 10);
        title += number;
    }
    title += "\n";
    text = title;
    text += FUN_00405380(0x136);
    label->size = 0;
    label->font = DAT_0056b0c8->text;
    label->SetText(text.c_str());
    label->SetValue(rally);
    label->x = 0.0f;
    label->y = 0.5f;
    label->SetProgress(0.0f);
    DAT_0056b0c8->FUN_00432b30(3, 0);
    ViewManager* view = new ViewManager;
    m_view = view;
    DrawContext* helper = new DrawContext;
    m_unk0c = helper;
    view->name.Assign("Car camera");
    view->nearPlane = 0.2f;
    float cameraDistance = DAT_005012f8;
    view->farPlane = cameraDistance;
    view->fov = 1.3508849f;
    mode->m_camera = (RaceCamera*)view;
    mode->unk38 = (int)helper;
    mode->CreateStage((int)label, DAT_00501458);
    Stage* stage = mode->m_stage;
    m_stage = stage;
    FUN_004701b0((char*)stage + 0x1c4);
    {
    LoadTask** tasks = (LoadTask**)block;
    tasks[0] = (LoadTask*)((char*)stage + 0xd4);
    tasks[1] = (LoadTask*)((char*)DAT_0056b0c8 + 0x1393e4);
    tasks[2] = (LoadTask*)((char*)DAT_0056b0c8 + 0x139404);
    int total = 0;
    for (int i = 0; i < 3; i++) {
        total += tasks[i]->Count();
    }
    text = title;
    static_cast<std::string&>(text) += FUN_00405380(0x138);
    label->size = 50;
    label->SetText(text.c_str());
    label->y = label->x = 0.5f;
    int done = 1;
    for (int t = 0; t < 3; t++) {
        LoadTask* task = tasks[t];
        for (int i = 0; i < task->Count(); i++) {
            float all = (float)total;
            task->Step(DAT_0056b0ac);
            label->SetProgress((float)done / all);
            done++;
        }
    }
    }
    delete label;
    CreateCountdown();
    mode->FUN_00486650((int)DAT_0056b0ac);
    ((ObjA*)((char*)DAT_0056b0c8 + 0x139424))->FUN_004ae1f0(DAT_0056b0ac);
    if (DAT_0056b0b0) {
        for (int i = 0x20; i < 0x44; i++) {
            DAT_0056b0b0->LoadSpeech(i);
        }
    }
    m_mode = mode;
    DAT_005654fc = 0;
    DAT_00565500 = 0;
    m_unk34 = new CarInput(DAT_0056b0cc, &pad[4]);
    m_unk34->FUN_004588d0();
    int step = mode->unk28 + 8;
    if (!DAT_00501458) {
        step = mode->unk28 + 3;
    }
    Player* player = mode->m_localPlayers[0];
    player->active = 1;
    mode->m_localPlayer = player;
    m_car = player->car;
    DAT_00565500 = 0;
    mode->Slot10(0, 0);
    DAT_0056b0c8->FUN_00432b30(step, 1);
    FUN_00493650(1);
    DAT_0056b0c8->FUN_00432b00();
    if (DAT_0056b0b0) {
        for (int i = 0x20; i < 0x44; i++) {
            DAT_0056b0b0->ReleaseSpeech(i);
        }
    }
    {
        RectArg rect(0, 0, 0, 0);
        ((RaceDeviceSlots*)DAT_0056b0ac)->SetViewport(rect);
        ((RaceDeviceSlots*)DAT_0056b0ac)->Slot24(1);
        ((RaceDeviceSlots*)DAT_0056b0ac)->Present();
    }
    delete m_unk34;
    m_unk34 = 0;
    mode->DeleteStage();
    m_stage = 0;
    DAT_0056b0bc->ClearKeys();
    delete m_view;
    delete m_unk0c;
    DAT_00501470 = 0;
    return 1;
}

// NONMATCHING: RALLYTROPHY 0x4921d0
// 3125 vs 3171 bytes; same structure and calls as 0x492e40 plus the demomode and round loops. Differences:
// register allocation, stack layout (A/B/C slots) and the shared zero register of the loop counters.
// demo != 0: replays data/replay/demomode<N>.rep; else one pass per local player. Returns the last loop result.
int RaceSession::Run(RaceMode* mode, int demo, BYTE a, BYTE b)
{
    int pad[8];
    int result = 1;
    m_unk30 = demo;
    DAT_00565508 = 0;
    m_unk19 = a;
    m_unk1a = b;
    LoadingLabel* label = new LoadingLabel;
    TitleString text;
    TitleString title;
    char number[8];
    int rally = mode->unk28;
    title += FUN_00405380(rally + 0xb4);
    title += " - ";
    if (!DAT_00501458) {
        title += FUN_00405380(0x1a);
        title += " ";
        _itoa(mode->unk2c, number, 10);
        title += number;
    } else {
        title += FUN_00405380(0x2a);
        title += " ";
        title += FUN_00405380(0x1b);
        title += " ";
        _itoa(mode->unk2c - ((StageProgress*)((char*)DAT_0056b0c8 + 0x3680))[rally].best, number, 10);
        title += number;
    }
    title += "\n";
    text = title;
    text += FUN_00405380(0x136);
    label->size = 0;
    label->font = DAT_0056b0c8->text;
    label->SetText(text.c_str());
    label->SetValue(rally);
    label->x = 0.0f;
    label->y = 0.5f;
    label->SetProgress(0.0f);
    DAT_0056b0c8->FUN_00432b30(3, 0);
    ViewManager* view = new ViewManager;
    m_view = view;
    DrawContext* helper = new DrawContext;
    m_unk0c = helper;
    view->name.Assign("Car camera");
    view->nearPlane = 0.2f;
    view->farPlane = DAT_005012f8;
    view->fov = 1.3508849f;
    mode->m_camera = (RaceCamera*)view;
    mode->unk38 = (int)helper;
    mode->CreateStage((int)label, DAT_00501458);
    Stage* stage = mode->m_stage;
    m_stage = stage;
    FUN_004701b0((char*)stage + 0x1c4);
    {
    LoadTask* tasks[3];
    tasks[0] = (LoadTask*)((char*)stage + 0xd4);
    tasks[1] = (LoadTask*)((char*)DAT_0056b0c8 + 0x1393e4);
    tasks[2] = (LoadTask*)((char*)DAT_0056b0c8 + 0x139404);
    int total = 0;
    for (int i = 0; i < 3; i++) {
        total += tasks[i]->Count();
    }
    text = title;
    text += FUN_00405380(0x138);
    label->size = 50;
    label->SetText(text.c_str());
    label->x = label->y = 0.5f;
    int done = 1;
    for (int t = 0; t < 3; t++) {
        LoadTask* task = tasks[t];
        for (int i = 0; i < task->Count(); i++) {
            float all = (float)total;
            task->Step(DAT_0056b0ac);
            label->SetProgress((float)done / all);
            done++;
        }
    }
    }
    delete label;
    CreateCountdown();
    mode->FUN_00486650((int)DAT_0056b0ac);
    ((ObjA*)((char*)DAT_0056b0c8 + 0x139424))->FUN_004ae1f0(DAT_0056b0ac);
    if (DAT_0056b0b0) {
        for (int i = 0x20; i < 0x44; i++) {
            DAT_0056b0b0->LoadSpeech(i);
        }
    }
    m_mode = mode;
    DAT_005654fc = 0;
    DAT_00565500 = 0;
    m_unk34 = new CarInput(DAT_0056b0cc, &pad[4]);
    m_unk34->FUN_004588d0();
    if (m_unk30) {
        Player* player = mode->m_localPlayers[0];
        player->active = 1;
        mode->m_localPlayer = player;
        Car* car = player->car;
        m_car = car;
        DAT_0056b0c8->FUN_00432b30(1, 1);
        ReplayName name;
        char digits[16];
        _itoa(m_unk30, digits, 10);
        name.Assign("data/replay/demomode");
        name.Append(digits);
        name.Append(".rep");
        car->FUN_00465530(name.c_str());
        FUN_00493650(1);
    } else {
        int step = mode->unk28 + 8;
    if (!DAT_00501458) {
        step = mode->unk28 + 3;
    }
        if (DAT_00501464 >= 2) {
            int again = 1;
            do {
                if (DAT_00501464 != 3) {
                    mode->Slot8();
                }
                for (int i = 0; i < (int)mode->m_localPlayers.size(); i++) {
                    Player* p = mode->m_localPlayers[i];
                    p->active = 0;
                    p->FUN_00491370();
                }
                for (int idx = 0; idx < (int)mode->m_localPlayers.size();) {
                    int next = idx + 1;
                    if (next == (int)mode->m_localPlayers.size() && DAT_00501464 == 2) {
                        DAT_00565500 = 1;
                    } else {
                        DAT_00565500 = 0;
                    }
                    Player* p = mode->m_localPlayers[idx];
                    Car* car = p->car;
                    p->active = 1;
                    mode->m_localPlayer = p;
                    m_car = car;
                    car->FUN_0045a850();
                    mode->Slot10(0, 0);
                    DAT_0056b0c8->FUN_00432b30(step, 1);
                    int r = FUN_00493650(0);
                    mode->SendState(3);
                    again = 0;
                    if (r > 0) {
                        mode->Slot10(1, 0);
                        mode->SubmitRecords(0);
                        m_car->FUN_0045a850();
                        DAT_0056b0c8->FUN_00432b30(1, 1);
                        r = FUN_00493650(1);
                        p->active = 0;
                    }
                    if (r == -1) {
                        again = 1;
                        break;
                    }
                    if (r == -2) {
                        again = 0;
                        goto multiDone;
                    }
                    idx = next;
                }
            } while (again);
        multiDone:
            result = -2;
        } else {
            mode->Slot8();
            for (int i = 0; i < (int)mode->m_localPlayers.size(); i++) {
                Player* p = mode->m_localPlayers[i];
                p->active = 0;
                p->FUN_00491370();
            }
            for (int idx = 0; idx < (int)mode->m_localPlayers.size();) {
                int next = idx + 1;
                if (next == (int)mode->m_localPlayers.size()) {
                    DAT_005654fc = mode->unk248;
                    DAT_00565500 = 1;
                } else {
                    DAT_005654fc = 0;
                    DAT_00565500 = 0;
                }
                Player* p = mode->m_localPlayers[idx];
                m_car = p->car;
                p->active = 1;
                mode->m_localPlayer = p;
                mode->Slot10(0, 0);
                DAT_0056b0c8->FUN_00432b30(step, 1);
                int r = FUN_00493650(0);
                mode->SendState(3);
                if (r > 0) {
                    mode->Slot10(1, 0);
                    if (DAT_00501472) {
                        (*(ObjB**)((char*)DAT_0056b0c8 + 0x2dd0))->FUN_004042c0();
                        (*(ObjC**)((char*)DAT_0056b0c8 + 0x2dcc))->FUN_0044e650();
                    }
                    mode->SubmitRecords(0);
                    mode->SubmitRally();
                    m_car->FUN_0048e660();
                    mode->Slot11(0, 0);
                    DAT_0056b0c8->FUN_00432b30(1, 1);
                    r = FUN_00493650(1);
                    p->active = 0;
                }
                result = r;
                if (r <= 0) {
                    break;
                }
                idx = next;
            }
        }
    }
    if (DAT_0056b0b0) {
        for (int i = 0x20; i < 0x44; i++) {
            DAT_0056b0b0->ReleaseSpeech(i);
        }
    }
    {
        RectArg rect(0, 0, 0, 0);
        ((RaceDeviceSlots*)DAT_0056b0ac)->SetViewport(rect);
        ((RaceDeviceSlots*)DAT_0056b0ac)->Slot24(1);
        ((RaceDeviceSlots*)DAT_0056b0ac)->Present();
    }
    delete m_unk34;
    m_unk34 = 0;
    mode->DeleteStage();
    m_stage = 0;
    DAT_0056b0bc->ClearKeys();
    delete m_view;
    delete m_unk0c;
    return result;
}

// Module end (0x4952b0-0x4952db): the guard-bit initialiser of the header
// statics and its atexit registration; the compiler emits both for every file
// that includes the STL headers.

// FUNCTION: RALLYTROPHY 0x4952b0
// SYMBOL: _$E23
typedef void Generated_4952b0();

// FUNCTION: RALLYTROPHY 0x4952d0
// SYMBOL: _$E22
typedef void Generated_4952d0();
