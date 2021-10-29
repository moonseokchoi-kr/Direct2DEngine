#ifndef _MONSET_HP_BAR_SHADER_
#define _MONSET_HP_BAR_SHADER_

#include "value.fx"


struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
};


VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
    output.position = mul(float4(input.position, 1.f), worldViewProjectionMatrix);
    
    return output;
}

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float4 outcolor = (float4) 0.f;
    outcolor = float4(186.f/255.f,187.f/255.f,216.f/255.f,1.f);
    return outcolor;
}
#endif