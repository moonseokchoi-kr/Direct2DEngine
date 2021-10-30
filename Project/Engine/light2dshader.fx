#ifndef _LIGHT2D_SHADER_
#define _LIGHT2D_SHADER_

#include "value.fx"
#include "function.fx"

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};


VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
    output.worldPosition = mul(float4(input.position, 1.f), worldMatrix);
    output.position = mul(float4(input.position, 1.f), worldViewProjectionMatrix);
    output.uv = input.uv;
   
    return output;

}


float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float4 outColor = (float4) 0.f;
    LightColor color = (LightColor) 0.f;
    
    for (uint i = 0; i < light2DCount; ++i)
    {
        GetLightColor(i, input.worldPosition, color);
    }
    
    float4 objectColor = tex_0.Sample(ati_sam, input.uv);
    
    outColor.rgb = objectColor.rgb * color.lightColor.rgb;
    outColor.a = objectColor.a;
    
    return outColor;
}

#endif