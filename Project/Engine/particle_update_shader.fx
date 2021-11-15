#ifndef _PARTICLE_UPDATE_
#define _PARTICLE_UPDATE_


#include "value.fx"

RWStructuredBuffer<Particle> g_particle : register(u0);

[numthreads(32,1,1)]
void cs_main(int3 _id: SV_DispatchThreadID)
{
    
}
#endif