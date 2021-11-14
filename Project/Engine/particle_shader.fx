#ifndef _PARTICLE_
#define _PARTICLE_

#include "value.fx"

StructuredBuffer<Particle> g_particle : register(t14);

struct VERTEX_INPUT
{
    float3 position : POSITION;
    
    uint instanceID : SV_InstanceID;
};

struct VERTEX_OUTPUT
{
    float3 position : POSITION;
    uint instanceID : FOG;
};


VERTEX_OUTPUT vs_main(VERTEX_INPUT input)
{
    VERTEX_OUTPUT output= (VERTEX_OUTPUT) 0.f;

    output.position = input.position;
    output.instanceID = input.instanceID;
    
    
    return output;
}

struct GS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

[maxvertexcount(6)]
void gs_main(point VERTEX_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    if(0==g_particle[input[0].instanceID].isActive)
    {
        return;
    }
    
    GS_OUTPUT output[4] = { (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f };
    float3 worldPosition = g_particle[input[0].instanceID].worldPosition;
    float3 scale = g_particle[input[0].instanceID].viewScale;
    float3 viewPos = mul(float4(worldPosition, 1.f), viewMatrix).xyz;
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
    output[0].position = float4(viewPos.x - scale.x / 2.f, viewPos.y + scale.y / 2.f, viewPos.z, 1.f);
    output[0].position = mul(output[0].position, projectionMatrix);
    output[0].uv = float2(0.f, 0.f);
    
    output[1].position = float4(viewPos.x + scale.x / 2.f, viewPos.y + scale.y / 2.f, viewPos.z, 1.f);
    output[1].position = mul(output[1].position, projectionMatrix);
    output[1].uv = float2(1.f, 0.f);
    
    output[2].position = float4(viewPos.x + scale.x / 2.f, viewPos.y - scale.y / 2.f, viewPos.z, 1.f);
    output[2].position = mul(output[2].position, projectionMatrix);
    output[2].uv = float2(1.f, 1.f);
    
    output[3].position = float4(viewPos.x - scale.x / 2.f, viewPos.y - scale.y / 2.f, viewPos.z, 1.f);
    output[3].position = mul(output[3].position, projectionMatrix);
    output[3].uv = float2(0.f, 1.f);
    
    outputStream.Append(output[0]);
    outputStream.Append(output[2]);
    outputStream.Append(output[3]);
    outputStream.RestartStrip();
    
    outputStream.Append(output[2]);
    outputStream.Append(output[0]);
    outputStream.Append(output[1]);
    outputStream.RestartStrip();
}

float4 ps_main(GS_OUTPUT output):SV_Target
{
    float4 outColor = (float4) 0.f;
    
    outColor = float4(1.0f, 0.f, 0.f, 1.0f);
    
    return outColor;
}


#endif