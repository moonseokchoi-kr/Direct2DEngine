#ifndef _SHADER_VALUE_
#define _SHADER_VALUE_
#include "struct.fx"
#define PI 3.14159265f
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
    int int_0;
    int int_1;
    int int_2;
    int int_3;
    
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

cbuffer LIGHT2D : register(b3)
{
    LightInfo light2DArray[50];
    uint light2DCount;
    uint3 padding;
}

Texture2D tex_0 : register(t0);
Texture2D tex_1 : register(t1);
Texture2D tex_2 : register(t2);
Texture2D tex_3 : register(t3);
Texture2D tex_4 : register(t4);
Texture2D tex_5 : register(t5);
Texture2D tex_6 : register(t6);
Texture2D tex_7 : register(t7);


Texture2DArray texarr_0 : register(t8);
Texture2DArray texarr_1 : register(t9);

TextureCube texcube_0 : register(t10);
TextureCube texcube_1 : register(t11);

Texture2D altras_tex : register(t12);

SamplerState ati_sam : register(s0);
SamplerState mip_sam : register(s1);

#endif