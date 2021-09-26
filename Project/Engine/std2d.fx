#ifndef _STD2D
#define _STD2D

struct VTX_IN
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VTX_OUT
{
    float4 pos : SV_Position;
};

VTX_OUT VS_std(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.pos = float4(_in.pos, 1.f);
    
    return output;
}

float4 PS_std(VTX_OUT _in):SV_Target
{
    float4 outputColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    return outputColor;
}

#endif