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
    
    if(int_3)
    {
        if (animation_array[0].fullSize.x == animation_array[0].size.x * 2.f && animation_array[0].fullSize.y == animation_array[0].size.y * 2.f)
        {
            float2 animationUV = (_in.uv * animation_array[0].fullSize);
            float2 rangeUV = (animation_array[0].fullSize - animation_array[0].size) / 2.f + animation_array[0].offset;
        
            if (rangeUV.x < animationUV.x && animationUV.x < rangeUV.x + animation_array[0].size.x && rangeUV.y < animationUV.y && animationUV.y < rangeUV.y + animation_array[0].size.y)
            {
                animationUV += (animation_array[0].leftTop - animation_array[0].offset);
                return atlas_tex.Sample(mip_sam, animationUV);
            }
        }
        else
        {
            float2 animationUV = (_in.uv * animation_array[0].fullSize);
            float2 sizeUV = (_in.uv * animation_array[0].size);
            float2 regionUV = animation_array[0].leftTop + animation_array[0].offset;
            if (regionUV.x < animationUV.x && animationUV.x < regionUV.x + animation_array[0].size.x
            && regionUV.y < animationUV.y && animationUV.y < regionUV.y + animation_array[0].size.y)
            {
                animationUV += (animation_array[0].leftTop - animation_array[0].offset);
                return atlas_tex.Sample(mip_sam, animationUV);
            }
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