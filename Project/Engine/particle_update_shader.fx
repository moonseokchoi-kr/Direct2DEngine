#ifndef _PARTICLE_UPDATE_
#define _PARTICLE_UPDATE_


#include "value.fx"
#include "function.fx"

#define MAX_COUNT int_0
#define PARTICLE_OBJECT_POS vec4_0
#define SPAWN_RANGE         vec4_1
#define START_SCALE         vec4_2
#define END_SCALE           vec4_3

RWStructuredBuffer<Particle> g_particle : register(u0);
RWStructuredBuffer<ParticleShared> g_particle_shared : register(u1);

static float speed = 100.f;

[numthreads(32,1,1)]
void cs_main(int3 id: SV_DispatchThreadID)
{

    
}
#endif