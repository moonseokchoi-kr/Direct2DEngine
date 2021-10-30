#ifndef _SHADER_STRUCT_
#define _SHADER_STRUCT_

struct LightColor
{
    float4 lightColor;
    float4 specular;
    float4 ambient;
};

struct LightInfo
{
    LightColor lightColor;
    
    int lightType;
    float3 lightDirection;
    float3 lightPosition;
    
    float lightRange;
    float lightAngle;
    float3 padding;
};


#endif