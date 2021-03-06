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
    
    output.pos = mul(float4(_in.pos, 1.f), worldViewProjectionMatrix);
    output.color = _in.color;
    output.uv = _in.uv;
    
    return output;
}

float4 ps_main(VS_OUTPUT _in) : SV_Target
{
    float4 outputColor = (float4)0.f;
    
    if(using_animation)
    {
        if(using_filp_horizon)
            _in.uv.x = 1 - _in.uv.x;
        if(using_filp_verticle)
            _in.uv.y = 1 - _in.uv.y;
        float2 animationUV = (_in.uv * animation_array.fullSize);
        float2 sizeUV = (_in.uv * animation_array.size);
        float2 rangeUV = (animation_array.fullSize - animation_array.size) / 2.f + animation_array.offset;
        if (rangeUV.x < animationUV.x && animationUV.x < rangeUV.x + animation_array.size.x && rangeUV.y < animationUV.y && animationUV.y < rangeUV.y + animation_array.size.y)
        {
            animationUV += (animation_array.leftTop - animation_array.offset);
            return atlas_tex.Sample(mip_sam, animationUV);
        }
        
        clip(-1);
    }
    else
    {
        outputColor = tex_0.Sample(mip_sam, _in.uv);
    }
 
    
    return outputColor;
}



#endif