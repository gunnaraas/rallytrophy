// Stage::~Stage (0x40ebc0): releases all owned sub-objects. Layout: include/stage.h.
#include "stage.h"

// FUNCTION: RALLYTROPHY 0x40ebc0
// The body deletes the owned objects in the original's order; the member destruction that follows is generated
// from the layout in stage.h (states 41..0, StageScene's destructor inlined with states 42-51).
Stage::~Stage()
{
    delete[] cells19c;
    unsigned int i;
    for (i = 0; i < objs28.Size(); i++) {
        SVirtual* e = objs28.first[i];
        if (e)
            delete e;
    }
    for (i = 0; i < objs38.Size(); i++) {
        SVirtual* e = objs38.first[i];
        if (e)
            delete e;
    }
    for (i = 0; i < objs134.size(); i++) {
        SVirtual* e = objs134[i];
        if (e)
            delete e;
    }
    for (i = 0; i < objs144.size(); i++) {
        SVirtual* e = objs144[i];
        if (e)
            delete e;
    }
    for (i = 0; i < attach154.size(); i++) {
        delete attach154[i];
    }
    for (i = 0; i < owners88.Size(); i++) {
        delete owners88.first[i];
    }
    delete owned13a4;
    if (obj18)
        delete obj18;
    delete sky;
    if (obj6b0)
        delete obj6b0;
    while (list98.head)
        list98.RemoveNodeDelete(list98.head);
    while (listac.head)
        listac.RemoveNodeDelete(listac.head);
    while (listc0.head)
        listc0.RemoveNodeDelete(listc0.head);
    delete sprite9e0;
    delete sprite9dc;
    delete[] arra08;
    delete aiPath;
    if (sound15f8) {
        Sample* sample = sound15f8->sample;
        DAT_0056b0b0->ReleaseSample(sample);
    }
    delete sound15f8;
    for (int k = 0; k < soundCount1600; k++) {
        DAT_0056b0b0->ReleaseSample(sounds15fc[k].sample);
    }
    delete[] sounds15fc;
    delete[] signs24c8;
    delete[] sounds24d4;
}
