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
    
    // ��Ȱ��ȭ ��ƼŬ ������Ű��
    if (0 == g_particle[id.x].isActive && 0 != g_particle_shared[0].activableCount)
    {
        int iOrigin = g_particle_shared[0].activableCount;
        int iExchanged = 0;
        
        while (0 < iOrigin)
        {
            int iValue = iOrigin - 1;
            //InterlockedExchange(g_particle_shared[0].activableCount, iValue, iExchanged);            
            InterlockedCompareExchange(g_particle_shared[0].activableCount, iOrigin, iValue, iExchanged);
            
            // ����
            if (iExchanged == iOrigin)
            {
                g_particle[id.x].isActive = 1;
                break;
            }
            
            iOrigin = iValue;
        }
        
        
        // �̹��� ������ ��ƼŬ�� ���
        if (1 == g_particle[id.x].isActive)
        {
            // ����            
            // id �� Ȱ���ؼ�, �����尣�� ��ġ�� �ʴ� �ȼ� ���� ��ġ�� ����� ����.
            
            // Sample_CS(, g_noise, g_vResolution_noise);
            g_particle[id.x].color = noise_tex.SampleLevel(ati_sam, float2(0.f, 0.f), 0);
        }
    }
    
    // Ȱ��ȭ �Ǿ��ִ� ��ƼŬ ������Ʈ
    else
    {
        
    }
}
#endif