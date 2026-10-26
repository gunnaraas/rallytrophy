// CarSnapshot (0x46bb90-0x46c28f, stub 0x46c290). Map: analysis/range-46a8a0.md section 3.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46bb90.h"
#include <stddef.h>

// FUNCTION: RALLYTROPHY 0x46bb90
CarSnapshot::CarSnapshot()
{
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    Clear();
}

// FUNCTION: RALLYTROPHY 0x46bbb0
CarSnapshot::~CarSnapshot()
{
    Clear();
}

// FUNCTION: RALLYTROPHY 0x46bbc0
void CarSnapshot::Clear()
{
    int i;
    setup[0] = 2;
    setup[1] = 2;
    setup[2] = 2;
    setup[3] = 2;
    setup[4] = 2;
    damage[0] = 0;
    damage[1] = 0;
    damage[2] = 0;
    for (i = 0; i < 4; i++) {
        values[i] = 0;
        angleB[i] = 0;
        angleA[i] = 0;
    }
    int* block = (int*)&unk44;
    for (i = 0; i < 8; i++)
        block[i] = 0;
    lamps[0] = 0;
    lamps[1] = 0;
    lamps[2] = 0;
    lamps[3] = 0;
    flagA = 0;
    flagB = 0;
    for (i = 0; i < 3; i++) {
        lightsA[i] = 0;
        lightsB[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        lights2A[i] = 0;
        lights2B[i] = 0;
    }
    size[0] = 0;
    size[1] = 0;
    size[2] = 0;
    operator delete(buffer[0]);
    operator delete(buffer[1]);
    operator delete(buffer[2]);
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
}

// FUNCTION: RALLYTROPHY 0x46bc90
CarSnapshot* Car::FUN_0046bc90(int slot)
{
    CarSnapView* c = (CarSnapView*)this;
    CarSnapshot* s = &c->snapshot[slot];
    int i;
    s->Clear();
    int a = setup.value[0], b = setup.value[1], d = setup.value[2], e = setup.value[3], f = setup.value[4];
    s->setup[0] = a;
    s->setup[1] = b;
    s->setup[2] = d;
    s->setup[3] = e;
    s->setup[4] = f;
    s->damage[0] = c->damage[0];
    s->damage[1] = c->damage[1];
    s->damage[2] = c->damage[2];
    for (i = 0; i < 4; i++) {
        s->values[i] = c->values[i];
        SnapWheel* w = &c->wheel[i];
        s->angleB[i] = w->GetAngleB();
        s->angleA[i] = w->GetAngleA();
    }
    s->unk44 = c->GetUnk1c758();
    s->lamps[0] = c->lamp[0].state;
    s->lamps[1] = c->lamp[1].state;
    s->lamps[2] = c->lamp[2].state;
    s->lamps[3] = c->lamp[3].state;
    s->flagA = c->flagA != 0;
    s->flagB = c->flagB != 0;
    for (i = 0; i < 3; i++) {
        if (c->light[i].sprite)
            s->lightsA[i] = c->light[i].state;
        if (c->light[i + 3].sprite)
            s->lightsB[i] = c->light[i + 3].state;
    }
    for (i = 0; i < 2; i++) {
        if (c->light2[i].sprite)
            s->lights2A[i] = c->light2[i].state;
        if (c->light2[i + 2].sprite)
            s->lights2B[i] = c->light2[i + 2].state;
    }
    SnapMesh* mesh = c->body->meshes.At(0);
    if (mesh)
        mesh->Save((void**)&s->buffer[0], &s->size[0]);
    if (c->partA) {
        mesh = c->partA->meshes.At(0);
        if (mesh)
            mesh->Save((void**)&s->buffer[1], &s->size[1]);
    }
    if (c->partB) {
        mesh = c->partB->meshes.At(0);
        if (mesh)
            mesh->Save((void**)&s->buffer[2], &s->size[2]);
    }
    return s;
}

// FUNCTION: RALLYTROPHY 0x46be50
void Car::FUN_0046be50(int slot)
{
    CarSnapView* c = (CarSnapView*)this;
    int i;
    FUN_00465e90(c->snapshot[slot].setup[0]);
    CarSnapshot* s = &c->snapshot[slot];
    FUN_00466500(s->setup[1]);
    FUN_00466520(s->setup[2]);
    FUN_00466590(s->setup[3]);
    FUN_00466610(s->setup[4]);
    c->damage[0] = s->damage[0];
    c->damage[1] = s->damage[1];
    c->damage[2] = s->damage[2];
    for (i = 0; i < 4; i++) {
        c->values[i] = s->values[i];
        SnapWheel* w = &c->wheel[i];
        w->angleB = s->GetAngleB(i);
        w->angleA = s->GetAngleA(i);
    }
    c->unk1c758 = s->unk44;
    BYTE l0 = s->lamps[0], l1 = s->lamps[1], l2 = s->lamps[2], l3 = s->lamps[3];
    c->lamp[3].state = l3;
    c->lamp[0].state = l0;
    c->lamp[1].state = l1;
    c->lamp[2].state = l2;
    if (c->lamp[0].target) {
        if (l0)
            *c->lamp[0].target = c->lamp[0].on;
        else
            *c->lamp[0].target = c->lamp[0].off;
    }
    if (c->marker)
    {
        if (l0)
            c->marker->mode = 0x11;
        else
            c->marker->mode = 0;
    }
    if (c->lamp[1].target) {
        if (l1)
            *c->lamp[1].target = c->lamp[1].on;
        else
            *c->lamp[1].target = c->lamp[1].off;
    }
    if (c->lamp[2].target) {
        if (l2)
            *c->lamp[2].target = c->lamp[2].on;
        else
            *c->lamp[2].target = c->lamp[2].off;
    }
    if (c->lamp[3].target) {
        if (l3)
            *c->lamp[3].target = c->lamp[3].on;
        else
            *c->lamp[3].target = c->lamp[3].off;
    }
    c->flagA = s->flagA;
    c->flagB = s->flagB;
    for (i = 0; i < 3; i++) {
        if (c->light[i].sprite) {
            BYTE st = s->lightsA[i];
            c->light[i].state = st;
            if (c->light[i].target)
                if (st)
                    *c->light[i].target = c->light[i].on;
                else
                    *c->light[i].target = c->light[i].off;
            if (st)
                c->light[i].sprite->Show();
            else
                c->light[i].sprite->SetParent(c->body);
            if (c->glow[i + 2]) {
                if (st)
                    c->glow[i + 2]->Show();
                else
                    c->glow[i + 2]->SetParent(c->body);
            }
        }
        if (c->light[i + 3].sprite) {
            BYTE st = s->lightsB[i];
            c->light[i + 3].state = st;
            if (c->light[i + 3].target)
                if (st)
                    *c->light[i + 3].target = c->light[i + 3].on;
                else
                    *c->light[i + 3].target = c->light[i + 3].off;
            if (st)
                c->light[i + 3].sprite->Show();
            else
                c->light[i + 3].sprite->SetParent(c->body);
            if (c->glow[i + 5]) {
                if (st)
                    c->glow[i + 5]->Show();
                else
                    c->glow[i + 5]->SetParent(c->body);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (c->light2[i].sprite) {
            BYTE st = s->lights2A[i];
            c->light2[i].state = st;
            if (c->light2[i].target)
                if (st)
                    *c->light2[i].target = c->light2[i].on;
                else
                    *c->light2[i].target = c->light2[i].off;
            if (st)
                c->light2[i].sprite->Show();
            else
                c->light2[i].sprite->SetParent(c->body);
            SnapEffect* g = i == 0 ? c->glow[0] : c->glow[1];
            if (g) {
                if (st)
                    g->Show();
                else
                    g->SetParent(c->body);
            }
        }
        if (c->light2[i + 2].sprite) {
            BYTE st = s->lights2B[i];
            c->light2[i + 2].state = st;
            if (c->light2[i + 2].target) {
                if (st) {
                    *c->light2[i + 2].target = c->light2[i + 2].on;
                    c->light2[i + 2].sprite->Show();
                } else {
                    *c->light2[i + 2].target = c->light2[i + 2].off;
                    c->light2[i + 2].sprite->SetParent(c->body);
                }
            }
        }
    }
    SnapMesh* mesh = c->body->meshes.At(0);
    if (mesh)
        mesh->Restore(s->buffer[0], s->size[0]);
    if (c->partA) {
        mesh = c->partA->meshes.At(0);
        if (mesh)
            mesh->Restore(s->buffer[1], s->size[1]);
    }
    if (c->partB) {
        mesh = c->partB->meshes.At(0);
        if (mesh)
            mesh->Restore(s->buffer[2], s->size[2]);
    }
    c->ApplyWheel(0);
    c->ApplyWheel(1);
    c->ApplyWheel(2);
    c->ApplyWheel(3);
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x46c290
// SYMBOL: _$E23
typedef void Generated_46c290();

// FUNCTION: RALLYTROPHY 0x46c2b0
// SYMBOL: _$E22
typedef void Generated_46c2b0();
