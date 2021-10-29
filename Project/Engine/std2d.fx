#ifndef _STD2D
#define _STD2D

cbuffer TRANSFORM : register(b0)
{
    row_major float4x4 worldMatrix;
    row_major float4x4 viewMatrix;
    row_major float4x4 projectionMatrix;
    
    row_major float4x4 worldViewMatrix;
    row_major float4x4 worldViewProjectionMatrix;
};

cbuffer MATERIAL : register(b1)
{
    int  int_0;
    int  int_1;
    int  int_2;
    int  int_3;
    
    float float_0;
    float float_1;
    float float_2;
    float float_3;
    
    float2 vec2_0;
    float2 vec2_1;
    float2 vec2_2;
    float2 vec2_3;
    
    float4 vec4_0;
    float4 vec4_1;
    float4 vec4_2;
    float4 vec4_3;
    
    row_major matrix mat_0;
    row_major matrix mat_1;
    row_major matrix mat_2;
    row_major matrix mat_3;
}

Texture2D tex_0 : register(t0);
Texture2D tex_1 : register(t1);
Texture2D tex_2 : register(t2);
Texture2D tex_3 : register(t3);

Texture2DArray texarr_0 : register(t4);
Texture2DArray texarr_1 : register(t5);

TextureCube texcube_0 : register(t6);
TextureCube texcube_1 : register(t7);

SamplerState ati_sam : register(s0);
SamplerState mip_sam : register(s1);


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