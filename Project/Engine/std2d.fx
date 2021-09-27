#ifndef _STD2D
#define _STD2D

cbuffer OFFSET : register(b0)
{
    float4 Offset;
};


struct VTX_IN
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VTX_OUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VTX_OUT VS_std(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    float3 pos = _in.pos + Offset.xyz;
    
    output.pos = float4(pos, 1.f);
    output.color = _in.color;
    return output;
}

float4 PS_std(VTX_OUT _in):SV_Target
{
    float4 outputColor = _in.color;
    
    return outputColor;
}

#endif