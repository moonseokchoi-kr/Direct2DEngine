#ifndef _COLLIDER2D_SHADER_
#define _COLLIDER2D_SHADER_

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
    float4 outColor = (float4) 0.f;
    
    if (int_0)
    {
        outColor = float4(0.9f, 0.2f, 0.2f, 1.f);
    }
    else
    {
        outColor = float4(0.2f, 0.9f, 0.2f, 1.f);
    }
    
    return outColor;
}

#endif