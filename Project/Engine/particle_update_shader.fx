#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "function.fx"

// ======================
// Particle Update Shader
#define MAX_COUNT int_0

#define MIN_SPEED           float_0
#define MAX_SPEED           float_1

#define MIN_LIFE            float_2
#define MAX_LIFE            float_3

#define PARTICLE_OBJECT_POS vec4_0
#define SPAWN_RANGE         vec4_1

#define MIN_SCALE         vec4_2
#define MAX_SCALE           vec4_3

#define START_COLOR         float4(vec2_0,vec2_1)
#define END_COLOR           float4(vec2_2,vec2_3)
// 문제 1. 병렬처리 (O)
// 문제 2. 랜덤    (X)
// ======================

RWStructuredBuffer<Particle> g_particle : register(u0);
RWStructuredBuffer<ParticleShared> g_shared : register(u1);

static float fSpeed = 100.f;

[numthreads(32, 1, 1)]
void cs_main(int3 id : SV_DispatchThreadID)
{
    if (MAX_COUNT <= id.x)
        return;
        
    
    // 비활성화 파티클 생성시키기
    if (0 == g_particle[id.x].isActive && 0 != g_shared[0].activableCount)
    {
        int origin = g_shared[0].activableCount;
        int exchanged = 0;
        
        while (0 < origin)
        {
            int value = origin - 1;
            //InterlockedExchange(g_shared[0].activableCount, value, exchanged);            
            InterlockedCompareExchange(g_shared[0].activableCount, origin, value, exchanged);
            
            // 성공
            if (exchanged == origin)
            {
                g_particle[id.x].isActive = 1;
                break;
            }
            
            origin = value;
        }
        
        
        // 이번에 생성된 파티클의 경우
        if (1 == g_particle[id.x].isActive)
        {
            // 랜덤            
            // id 를 활용해서, 스레드간에 겹치지 않는 픽셀 참조 위치를 만들어 낸다.
            
            float key = (float) id.x / 100.f;
            float3 random = float3(Rand(key), Rand(key + 0.1f), Rand(key + 0.2f));
            float3 offsetPos = float3(random.x, random.y, random.z);
            offsetPos *= SPAWN_RANGE.xyz;
            
            float3 objectWorld = PARTICLE_OBJECT_POS;
            offsetPos.xy = (offsetPos.xy - g_resolution.xy / 2.f);
            objectWorld += offsetPos;
            
            g_particle[id.x].worldPosition = objectWorld;
            g_particle[id.x].viewScale = float4(DistributeRandomVec3(key,MIN_SCALE.xyz,MAX_SCALE.xyz), g_particle[id.x].viewScale.z);
            g_particle[id.x].direction = float3(DistributeRandomFloat(key, -5, 5), DistributeRandomFloat(key, -5, -1), 0.f);
            g_particle[id.x].maxLifeTime = MIN_LIFE + (MAX_LIFE - MIN_LIFE) * random.z;
            g_particle[id.x].color = START_COLOR + (START_COLOR - END_COLOR) * random.z;
        }
    }
    // 활성화 되어있는 파티클 업데이트
    else
    {
        g_particle[id.x].currnetLifeTime += g_dt;
        if (g_particle[id.x].maxLifeTime < g_particle[id.x].currnetLifeTime)
        {
            g_particle[id.x].isActive = 0;
            g_particle[id.x].currnetLifeTime = 0.f;
        }
        
        float fRatio = g_particle[id.x].currnetLifeTime / g_particle[id.x].maxLifeTime;
        
        float fSpeed = DistributeRandomFloat(id.x / 100.f, MIN_SPEED, MAX_SPEED);
        g_particle[id.x].direction = float3(DistributeRandomFloat(id.x/100.f, -5, 5), DistributeRandomFloat(id.x/100.f, -5, -1), 0.f);
          
        g_particle[id.x].worldPosition += g_dt * fSpeed* g_particle[id.x].direction;

        g_particle[id.x].color = START_COLOR.rgb + (END_COLOR.rgb - START_COLOR.rgb) * fRatio;
    }
}



    //if (0 == g_particle[id.x].isActive)
    //    return;
    
    //float fTheta = (PI / (MAX_COUNT - 1)) * ((MAX_COUNT - 1) - id.x);    
    //float2 vDir = float2(cos(fTheta), sin(fTheta));    
    //g_particle[_id.x].vWorldPos += float3(vDir, 0.f) * fSpeed * g_dt;

#endif