#ifndef _UI
#define _UI

#include "value.fx"
#include "struct.fx"

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VS_OUTPUT vs_main(VS_INPUT _in)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
    
    output.pos = mul(float4(_in.pos, 1.f), worldViewProjectionMatrix);
    output.color = _in.color;
    output.uv = _in.uv;
    
    return output;
}

float4 ps_main(VS_OUTPUT _in) : SV_Target
{
    float4 outputColor = (float4) 0.f;
    
    outputColor = float4(1.f, 0.f, 0.f, 1.f);
 
    
    return outputColor;
}



#endif