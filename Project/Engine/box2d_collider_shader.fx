#ifndef __COLLIDER_DEBUG
#define __COLLIDER_DEBUG

#include "value.fx"
#include "struct.fx"

struct VERTEX_INPUT
{
    float3 postion : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VERTEX_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VERTEX_OUTPUT vs_main(VERTEX_INPUT input)
{
    VERTEX_OUTPUT output = (VERTEX_OUTPUT) 0.f;
    output.position = mul(float4(input.postion, 1.f),projectionMatrix);
    output.color = input.color;
    output.uv = input.uv;
    
    return output;
}

float4 ps_main(VERTEX_OUTPUT input) : SV_Target
{
    float4 outputColor = (float4) 0.f;
    outputColor = float4(0.4f, 0.9f, 0.2f, 1.0f);
    
    return outputColor;
}



#endif