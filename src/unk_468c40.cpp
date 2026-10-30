// Car model parts (0x468c40-0x46962f, stub 0x469600): texture reload, part visibility, cockpit
// view, damaged model and body alpha. Map: analysis/range-4666a0.md section 4.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_468c40.h"
#include <string.h>

// Table of 12-byte entries at 0x4fb0f4; the byte at +0xa says whether the damage-free model shows the part.
extern BYTE DAT_004fb0fe[];

// These are the reconstructed scene-node operations at 0x4b2b30/0x4b2b90.
extern "C" void __cdecl RT_SceneNode_AttachTo(void);
extern "C" void __cdecl RT_SceneNode_Detach(void);
#pragma comment(linker, "/alternatename:_RT_SceneNode_AttachTo=?AttachTo@SceneNode@@QAEXPAV1@@Z")
#pragma comment(linker, "/alternatename:_RT_SceneNode_Detach=?Detach@SceneNode@@QAEXXZ")

// FUNCTION: RALLYTROPHY 0x468c40
void Car::FUN_00468c40(int a)
{
    CarModelView* v = (CarModelView*)this;
    if (v->texturesLoaded) {
        v->textures->Upload(a);
        v->root->Reload(a);
    }
}

// NONMATCHING: RALLYTROPHY 0x468c70
// 1392-byte candidate/target; 8 differing bytes remain in the EBP/EDI allocation around the visibility test.
// The inverted `visible = 1` branch matches the target's `jne X; jmp end`, and direct list->data expressions
// place the old-buffer argument after the rep movs.
void Car::FUN_00468c70(int a)
{
    CarModelView* v = (CarModelView*)this;
    CarPtrArray* list = (CarPtrArray*)a;
    if (v->damaged) {
        if (v->player->mode == 2 && v->part1f18)
            v->part1f18->flags |= 1;
        if (v->eff2018) v->eff2018->flags |= 1;
        if (v->part2028) v->part2028->flags |= 1;
        if (v->eff201c) v->eff201c->flags |= 1;
        if (v->part202c) v->part202c->flags |= 1;
        if (v->part2020) v->part2020->flags |= 1;
        if (v->part2024) v->part2024->flags |= 1;
        if (v->eff200c) v->eff200c->flags |= 1;
        if (v->eff2048) v->eff2048->flags |= 1;
        if (v->part2054) v->part2054->flags |= 1;
        if (v->part2058) v->part2058->flags |= 1;
        if (v->part1f6c) v->part1f6c->flags &= 0xfe;
        if (v->part1f68) v->part1f68->flags &= 0xfe;
        if (v->part20ec) v->part20ec->flags |= 1;
        for (int i = 0; i < 3; i++) {
            if (v->slot[i].part) v->slot[i].part->flags |= 1;
            if (v->slot[i + 3].part) v->slot[i + 3].part->flags |= 1;
            if (v->ex[i]) v->ex[i]->flags |= 1;
            if (v->ex[i + 3]) v->ex[i + 3]->flags |= 1;
        }
        for (int j = 0; j < 2; j++) {
            if (v->slot[j + 8].part) v->slot[j + 8].part->flags |= 1;
            if (v->slot[j + 6].part) v->slot[j + 6].part->flags |= 1;
            if (v->slot[j + 10].part) v->slot[j + 10].part->flags |= 1;
        }
        if (v->part1be9c) v->part1be9c->flags |= 1;
        if (v->part1bea0) v->part1bea0->flags |= 1;
    } else {
        if (v->part1f18) {
            int visible = 1;
            if (!v->v1c738) {
                visible = 1;
            } else if (!DAT_00501454 && !DAT_00501458 && DAT_00501464 != 3 && DAT_0050132c == 2) {
                visible = 0;
            }
            if (!DAT_00501450->GetStageValue())
                visible = 0;
            BYTE f = v->part1f18->flags;
            if (visible)
                f &= 0xfe;
            else
                f |= 1;
            v->part1f18->flags = f;
        }
        if (v->part20ec) v->part20ec->flags &= 0xfe;
        for (int i = 0; i < 3; i++) {
            if (v->slot[i].part) v->slot[i].part->flags &= 0xfe;
            if (v->slot[i + 3].part) v->slot[i + 3].part->flags &= 0xfe;
            if (v->ex[i]) v->ex[i]->flags &= 0xfe;
            if (v->ex[i + 3]) v->ex[i + 3]->flags &= 0xfe;
        }
        for (int j = 0; j < 2; j++) {
            if (v->slot[j + 6].part) v->slot[j + 6].part->flags &= 0xfe;
        }
        if (v->part1be9c) v->part1be9c->flags &= 0xfe;
        if (v->part1bea0) v->part1bea0->flags &= 0xfe;
        if (v->cockpit) {
            if (v->eff2048) v->eff2048->flags &= 0xfe;
            if (v->part2054) v->part2054->flags &= 0xfe;
            if (v->part2058) v->part2058->flags &= 0xfe;
            if (v->part2020) v->part2020->flags &= 0xfe;
            if (v->part2024) v->part2024->flags &= 0xfe;
            if (v->eff200c) v->eff200c->flags &= 0xfe;
            if (v->part1f68) v->part1f68->flags &= 0xfe;
            if (v->eff2018) v->eff2018->flags |= 1;
            if (v->part2028) v->part2028->flags |= 1;
            if (v->eff201c) v->eff201c->flags |= 1;
            if (v->part202c) v->part202c->flags |= 1;
            if (v->part1f6c) v->part1f6c->flags |= 1;
        } else {
            BYTE k = DAT_004fb0fe[v->model * 12];
            if (v->eff2018) {
                if (k) v->eff2018->flags &= 0xfe;
                else v->eff2018->flags |= 1;
            }
            if (v->part2028) {
                if (k) v->part2028->flags &= 0xfe;
                else v->part2028->flags |= 1;
            }
            if (v->eff201c) {
                if (k) v->eff201c->flags &= 0xfe;
                else v->eff201c->flags |= 1;
            }
            if (v->part202c) {
                if (k) v->part202c->flags &= 0xfe;
                else v->part202c->flags |= 1;
            }
            if (v->part2020) {
                if (k) v->part2020->flags &= 0xfe;
                else v->part2020->flags |= 1;
            }
            if (v->part2024) {
                if (k) v->part2024->flags &= 0xfe;
                else v->part2024->flags |= 1;
            }
            if (v->eff200c) {
                if (k) v->eff200c->flags &= 0xfe;
                else v->eff200c->flags |= 1;
            }
            if (v->eff2048) {
                if (k) v->eff2048->flags &= 0xfe;
                else v->eff2048->flags |= 1;
            }
            if (v->part2054) v->part2054->flags |= 1;
            if (v->part2058) v->part2058->flags |= 1;
            if (v->part1f6c) v->part1f6c->flags &= 0xfe;
            if (v->part1f68) v->part1f68->flags &= 0xfe;
        }
    }
    if (v->part1ffc) {
        if (v->part2008) v->part2008->flags &= 0xfe;
        if (v->holder1ff8) v->holder1ff8->part->flags &= 0xfe;
        if (v->part1ff4) v->part1ff4->flags |= 1;
    } else {
        if (v->part2008) v->part2008->flags |= 1;
        if (v->holder1ff8) v->holder1ff8->part->flags |= 1;
        if (v->part1ff4) v->part1ff4->flags &= 0xfe;
    }
    if (v->cockpit && v->part1ffc) {
        if (v->part2004) v->part2004->flags &= 0xfe;
    } else {
        if (v->part2004) v->part2004->flags |= 1;
    }
    // inline push_back of the body
    void* body = v->body;
    if (list->count == list->capacity) {
        void** p = (void**)operator new((list->grow + list->capacity) * 4);
        memcpy(p, list->data, list->capacity * 4);
        list->capacity += list->grow;
        operator delete(list->data);
        list->data = p;
        list->end = p + list->count;
    }
    *list->end = body;
    list->count++;
    list->end++;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4691e0
// original: FUN_004691e0
// Evidence: ev_1d991568c7a83c95b1d5c586c6965d6bdba8cbe772e61940c40ab59c58b2061c.
// Preserve the target's byte RMW operations and flag/view store scheduling.
void Car::FUN_004691e0(int cockpit)
{
    __asm {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 01eb4h]
        push edi
        mov edi, dword ptr [esp + 0ch]
        cmp eax, edi
        jz L469395
        mov ecx, dword ptr [esi + 02018h]
        test ecx, ecx
        jz L469209
        mov eax, dword ptr [esi + 024h]
        push eax
        call RT_SceneNode_AttachTo
    L469209:
        mov ecx, dword ptr [esi + 0201ch]
        test ecx, ecx
        jz L46921c
        mov edx, dword ptr [esi + 024h]
        push edx
        call RT_SceneNode_AttachTo
    L46921c:
        mov ecx, dword ptr [esi + 02048h]
        test ecx, ecx
        jz L46922b
        call RT_SceneNode_Detach
    L46922b:
        mov ecx, dword ptr [esi + 0200ch]
        test ecx, ecx
        jz L46923a
        call RT_SceneNode_Detach
    L46923a:
        test edi, edi
        jz L469248
        mov eax, dword ptr [esi + 02014h]
        test eax, eax
        jnz L469252
    L469248:
        mov eax, dword ptr [esi + 02010h]
        test eax, eax
        jz L469258
    L469252:
        mov dword ptr [esi + 0200ch], eax
    L469258:
        test edi, edi
        jz L469266
        mov eax, dword ptr [esi + 02050h]
        test eax, eax
        jnz L469270
    L469266:
        mov eax, dword ptr [esi + 0204ch]
        test eax, eax
        jz L469276
    L469270:
        mov dword ptr [esi + 02048h], eax
    L469276:
        mov ecx, dword ptr [esi + 02048h]
        test ecx, ecx
        jz L469289
        mov eax, dword ptr [esi + 024h]
        push eax
        call RT_SceneNode_AttachTo
    L469289:
        mov ecx, dword ptr [esi + 0200ch]
        test ecx, ecx
        jz L46929c
        mov edx, dword ptr [esi + 024h]
        push edx
        call RT_SceneNode_AttachTo
    L46929c:
        test edi, edi
        mov dword ptr [esi + 01eb4h], edi
        jz L469312
        mov al, byte ptr [esi + 0210ch]
        mov cl, 01h
        or al, cl
        mov byte ptr [esi + 0210ch], al
        or byte ptr [esi + 02238h], cl
        or byte ptr [esi + 02364h], cl
        or byte ptr [esi + 02494h], cl
        mov al, byte ptr [esi + 025f4h]
        mov edx, dword ptr [esi + 02094h]
        or al, cl
        mov ecx, dword ptr [esi + 0208ch]
        mov byte ptr [esi + 025f4h], al
        mov eax, dword ptr [esi + 02084h]
        mov dword ptr [esi + 02074h], eax
        mov dword ptr [esi + 02078h], ecx
        mov dword ptr [esi + 0207ch], edx
        lea eax, [esi + 034h]
        mov ecx, 04h
    L469302:
        or byte ptr [eax], 02h
        add eax, 0330h
        dec ecx
        jnz L469302
        pop edi
        pop esi
        ret 04h
    L469312:
        mov eax, dword ptr [esi + 02054h]
        test eax, eax
        mov cl, 01h
        jz L469321
        or byte ptr [eax + 0ch], cl
    L469321:
        mov eax, dword ptr [esi + 02058h]
        test eax, eax
        jz L46932e
        or byte ptr [eax + 0ch], cl
    L46932e:
        mov cl, byte ptr [esi + 0210ch]
        mov al, 0feh
        and cl, al
        mov byte ptr [esi + 0210ch], cl
        and byte ptr [esi + 02238h], al
        and byte ptr [esi + 02364h], al
        and byte ptr [esi + 02494h], al
        mov cl, byte ptr [esi + 025f4h]
        mov edx, dword ptr [esi + 02090h]
        and cl, al
        mov eax, dword ptr [esi + 02080h]
        mov byte ptr [esi + 025f4h], cl
        mov ecx, dword ptr [esi + 02088h]
        mov dword ptr [esi + 02074h], eax
        mov dword ptr [esi + 02078h], ecx
        mov dword ptr [esi + 0207ch], edx
        lea eax, [esi + 034h]
        mov ecx, 04h
    L46938a:
        and byte ptr [eax], 0fdh
        add eax, 0330h
        dec ecx
        jnz L46938a
    L469395:
        pop edi
        pop esi
        ret 04h
    }
}

// NONMATCHING: RALLYTROPHY 0x4693a0
// Same size (543); restore branch and damaged-loop stores follow the target order. 18 register-assignment bytes remain.
void Car::FUN_004693a0(int mode)
{
    CarModelView* v = (CarModelView*)this;
    if (mode == v->damaged)
        return;
    v->damaged = mode;
    if (mode) {
        ModelNode* b = v->nodes2;
        ModelNode* a = v->nodes1;
        while (a && b) {
            ModelMaterial* m = a->data;
            m->quad40.a = 1.0f;
            m->quad40.b = 1.0f;
            m->quad40.c = 1.0f;
            m->quad40.d = 0;
            *(volatile int*)&m->unk9c = 2;
            m->unk24 = 2;
            ModelMaterial* o = b->data;
            m->quad60.a = 0;
            m->quad60.b = 0;
            m->quad60.c = 0;
            m->quad60.d = 0;
            m->quad70.a = 1.0f;
            m->quad70.b = 1.0f;
            m->quad70.c = 1.0f;
            m->quad70.d = 1.0f;
            m->unk38 = 0;
            m->quad50.a = 0;
            m->quad50.b = 0;
            m->quad50.c = 0;
            m->quad50.d = 0;
            m->unk28 = 3;
            m->unk20 = 4;
            m->unk2c = 1;
            m->unk30 = 1;
            m->blend18 = 5;
            m->blend1c = 6;
            if (((BYTE*)o)[0x9d] & 0x40) {
                m->unk30 = 3;
                m->unk24 = 4;
                m->unk34 = 1;
            }
            a = a->next;
            b = b->next;
            m->unk98 = 0xf;
        }
    } else {
        ModelNode* a = v->nodes1;
        ModelNode* b = v->nodes2;
        while (a && b) {
            ModelMaterial* m = a->data;
            ModelMaterial* o = b->data;
            m->unk38 = o->unk38;
            m->quad40 = o->quad40;
            m->quad60 = o->quad60;
            m->quad70 = o->quad70;
            m->quad50 = o->quad50;
            m->unk9c = o->unk9c;
            m->unk20 = o->unk20;
            m->unk24 = o->unk24;
            a = a->next;
            m->unk28 = o->unk28;
            m->unk2c = o->unk2c;
            m->unk30 = o->unk30;
            m->unk34 = o->unk34;
            m->blend18 = o->blend18;
            m->blend1c = o->blend1c;
            m->unk98 = o->unk98;
            b = b->next;
        }
        if (v->lamp) {
            if (v->flag1bcfc)
                v->lamp->state = 0x11;
            else
                v->lamp->state = 0;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4695c0
void Car::FUN_004695c0(float fade)
{
    CarModelView* v = (CarModelView*)this;
    if (v->damaged) {
        fade *= 0.75f;
        for (ModelNode* n = v->nodes1; n; n = n->next)
            n->data->quad40.d = fade;
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x469600
// SYMBOL: _$E23
typedef void Generated_469600();

// FUNCTION: RALLYTROPHY 0x469620
// SYMBOL: _$E22
typedef void Generated_469620();
