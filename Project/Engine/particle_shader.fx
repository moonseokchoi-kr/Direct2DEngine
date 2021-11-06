#ifndef _PARTICLE_
#define _PARTICLE_

#include "value.fx"

StructuredBuffer<Particle> g_particle : register(t13);

struct VERTEX_INPUT
{
    float3 position : POSITION;
    float2 tex : TEXCOORD;
};

struct VERTEX_OUTPUT
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD;
};


VERTEX_OUTPUT vs_main(VERTEX_INPUT input)
{
    VERTEX_OUTPUT output= (VERTEX_OUTPUT) 0.f;
    
    return output;
}


float4 ps_main(VERTEX_OUTPUT output)
{
    float4 outColor = (float4) 0.f;
    
    
    return outColor;
}


#endif