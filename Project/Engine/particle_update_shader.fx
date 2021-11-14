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
    if(MAX_COUNT <= id.x)
        return;
    //비활성화된 파티클 활성화 
    if(0==g_particle[id.x].isActive && 0 != g_particle_shared[0].activableCount)
    {
        int origin = g_particle_shared[0].activableCount;
        int exchanged = 0;
        
        while(0<origin)
        {
            int value = origin - 1;
            InterlockedCompareExchange(g_particle_shared[0].activableCount, origin, value, exchanged);
            
            if(exchanged == origin)
            {
                g_particle[id.x].isActive = 1;
                break;
            }
            origin = value;
        }
        //생성될 파티클 랜덤화
        if (1 == g_particle[id.x].isActive)
        {
            float key = (float) id.x / 100.f;
            float3 random = float3(Rand(key), Rand(key + 0.1f), Rand(key + 0.2f));
            
            float3 offsetPosition = float3(random.x, random.y, random.z);
            
            offsetPosition += SPAWN_RANGE.xyz;
            
            float3 objWorld = PARTICLE_OBJECT_POS;
            offsetPosition = (offsetPosition - SPAWN_RANGE.xyz / 2.f);
            objWorld += offsetPosition;
            
            g_particle[id.x].worldPosition = objWorld;
            g_particle[id.x].viewScale = START_SCALE;
            g_particle[id.x].direction = -normalize(offsetPosition);
            g_particle[id.x].maxLifeTime = 2.f;

        }
        
    }
    //활성화 되어있는 파티클 업데이트
    else
    {
        g_particle[id.x].currnetLifeTime += g_dt;
        
        if(g_particle[id.x].maxLifeTime < g_particle[id.x].currnetLifeTime)
        {
            g_particle[id.x].isActive = 0;
            g_particle[id.x].currnetLifeTime = 0;
        }
        
        float ratio = g_particle[id.x].currnetLifeTime / g_particle[id.x].maxLifeTime;
        g_particle[id.x].viewScale = (END_SCALE - START_SCALE) * ratio;
        g_particle[id.x].worldPosition += g_dt * speed * g_particle[id.x].direction;
        
    }
    
}
#endif