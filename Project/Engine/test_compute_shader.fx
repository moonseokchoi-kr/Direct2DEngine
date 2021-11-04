#ifndef _TEST_
#define _TEST_



RWTexture2D<float4> g_rwTex : register(u0);

[numthreads(1024,1,1)]
void cs_main(int3 threadID:SV_DispatchThreadID)
{
    g_rwTex[threadID.xy] = float4(1.f, 0.f, 0.f, 1.f);
}


#endif