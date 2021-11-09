#ifndef _PARTICLE_
#define _PARTICLE_

#include "value.fx"

StructuredBuffer<Particle> g_particle : register(t13);

struct VERTEX_INPUT
{
    float3 position : POSITION;
    float2 tex : TEXCOORD;
    
    uint instanceID : SV_InstanceID;
};

struct VERTEX_OUTPUT
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD;
};


VERTEX_OUTPUT vs_main(VERTEX_INPUT input)
{
    VERTEX_OUTPUT output= (VERTEX_OUTPUT) 0.f;
    
    float3 pos = g_particle[input.instanceID].worldPosition;
    float3 scale = g_particle[input.instanceID].viewScale;
    
    matrix worldMat =
    {
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, scale.z, 0,
        pos.x, pos.y, pos.z, 1
    };
    
    float4 worldPosition = mul(float4(input.position, 1.f), worldMat);
    float4 viewPosition = mul(worldPosition, viewMatrix);
    output.position = mul(viewPosition,projectionMatrix);
    output.tex = input.tex;
    
    
    return output;
}


float4 ps_main(VERTEX_OUTPUT output):SV_Target
{
    float4 outColor = (float4) 0.f;
    
    outColor = float4(1.0f, 0.f, 0.f, 1.0f);
    
    return outColor;
}


#endif