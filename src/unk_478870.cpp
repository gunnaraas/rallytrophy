// Particle pool ParticlePool (0x478870-0x478bdf): the global free list
// (DAT_0056ae90) of 4096-particle blocks the effect emitters draw from.
// Map: analysis/range-478870.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"

// FUNCTION: RALLYTROPHY 0x478870
ParticlePool::ParticlePool()
{
    head = 0;
    Grow();
}

// FUNCTION: RALLYTROPHY 0x4788c0
ParticlePool::~ParticlePool()
{
    for (int i = 0; i < blocks.size(); i++) {
        delete[] blocks[i];
    }
}

// FUNCTION: RALLYTROPHY 0x478960
void ParticlePool::Grow()
{
    Particle* block = new Particle[4096];
    blocks.push_back(block);
    ThreadBlock(block);
}

// FUNCTION: RALLYTROPHY 0x478b80
void ParticlePool::ThreadBlock(Particle* p)
{
    Particle* block = p;
    for (int i = 0; i < 4095; i++) {
        p->next = p + 1;
        p++;
    }
    p->next = head;
    head = block;
}

// FUNCTION: RALLYTROPHY 0x478bb0
// SYMBOL: _$E23
typedef void Generated_478bb0();

// FUNCTION: RALLYTROPHY 0x478bd0
// SYMBOL: _$E22
typedef void Generated_478bd0();
