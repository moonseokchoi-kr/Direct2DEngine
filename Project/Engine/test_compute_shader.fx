#ifndef _TEST_
#define _TEST_

#include "value.fx"

RWTexture2D<float4> g_rwTex : register(u0);
RWStructuredBuffer<float4> g_buffer_rw : register(u1);


StructuredBuffer<float4> g_buffer : register(t13);

[numthreads(1024,1,1)]
void cs_main(int3 threadID:SV_DispatchThreadID)
{
    if(int_0)
    {
        g_rwTex[threadID.xy] = g_buffer[0];
    }
    else
    {
        g_buffer_rw[0] = float4(1.f, 1.f, 1.f, 1.f);
    }
   
}


#endif