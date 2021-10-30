#ifndef _STD2D
#define _STD2D

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
    float4 worldPos = mul(float4(_in.pos, 1.f), worldMatrix);
    float4 viewPos = mul(worldPos, viewMatrix);
    float4 projPos = mul(viewPos, projectionMatrix);
    
    output.pos = projPos;
    output.color = _in.color;
    output.uv = _in.uv;
    return output;
}

float4 ps_main(VS_OUTPUT _in) : SV_Target
{
    float4 outputColor = _in.color;
    
    outputColor = tex_0.Sample(ati_sam, _in.uv);
    
    if (int_0)
    {
        outputColor.r *= 2.f;
    }
   
    
    return outputColor;
}



#endif