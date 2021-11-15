#ifndef _PARTICLE_UPDATE_
#define _PARTICLE_UPDATE_


#include "value.fx"

#define MAX_COUNT int_0

RWStructuredBuffer<Particle> g_particle : register(u0);
RWStructuredBuffer<ParticleShared> g_particle_shared : register(u1);
[numthreads(32,1,1)]
void cs_main(int3 id: SV_DispatchThreadID)
{
    if(MAX_COUNT <= id.x)
        return;
    
    // 비활성화 파티클 생성시키기
    if (0 == g_particle[id.x].isActive && 0 != g_particle_shared[0].activableCount)
    {
        int iOrigin = g_particle_shared[0].activableCount;
        int iExchanged = 0;
        
        while (0 < iOrigin)
        {
            int iValue = iOrigin - 1;
            //InterlockedExchange(g_particle_shared[0].activableCount, iValue, iExchanged);            
            InterlockedCompareExchange(g_particle_shared[0].activableCount, iOrigin, iValue, iExchanged);
            
            // 성공
            if (iExchanged == iOrigin)
            {
                g_particle[id.x].isActive = 1;
                break;
            }
            
            iOrigin = iValue;
        }
        
        
        // 이번에 생성된 파티클의 경우
        if (1 == g_particle[id.x].isActive)
        {
            // 랜덤            
            // id 를 활용해서, 스레드간에 겹치지 않는 픽셀 참조 위치를 만들어 낸다.
            
            // Sample_CS(, g_noise, g_vResolution_noise);
            g_particle[id.x].color = noise_tex.SampleLevel(ati_sam, float2(0.f, 0.f), 0);
        }
    }
    
    // 활성화 되어있는 파티클 업데이트
    else
    {
        
    }
}
#endif