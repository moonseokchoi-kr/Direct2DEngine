#ifndef _PARTICLE_
#define _PARTICLE_

#include "value.fx"
#include "function.fx"
#define PARTICLE_TEX tex_0

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

struct GEOMETRY_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};


[maxvertexcount(6)]
void gs_main(point VERTEX_OUTPUT input[1], inout TriangleStream<GEOMETRY_OUTPUT> outputStream)
{
    if (0 == g_particle[input[0].instanceID].isActive)
    {
        return;
    }
    
    // 출력시킬 4개의 정점 ( 1개 -> 4개로 분할 )
    GEOMETRY_OUTPUT output[4] = { (GEOMETRY_OUTPUT) 0.f, (GEOMETRY_OUTPUT) 0.f, (GEOMETRY_OUTPUT) 0.f, (GEOMETRY_OUTPUT) 0.f };
    
    float3 worldPosition = g_particle[input[0].instanceID].worldPosition;
    float3 viewScale = g_particle[input[0].instanceID].viewScale;
    
    float3 viewPos = mul(float4(worldPosition, 1.f), viewMatrix).xyz;
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
    output[0].position = float4(viewPos.x - viewScale.x / 2.f, viewPos.y + viewScale.y / 2.f, viewPos.z, 1.f);
    output[0].position = mul(output[0].position, projectionMatrix);
    output[0].tex = float2(0.f, 0.f);
    output[0].color.rgb = g_particle[input[0].instanceID].color;
    
    output[1].position = float4(viewPos.x + viewScale.x / 2.f, viewPos.y + viewScale.y / 2.f, viewPos.z, 1.f);
    output[1].position = mul(output[1].position, projectionMatrix);
    output[1].tex = float2(1.f, 0.f);
    output[1].color.rgb = g_particle[input[0].instanceID].color;
    
    
    output[2].position = float4(viewPos.x + viewScale.x / 2.f, viewPos.y - viewScale.y / 2.f, viewPos.z, 1.f);
    output[2].position = mul(output[2].position, projectionMatrix);
    output[2].tex = float2(1.f, 1.f);
    output[2].color.rgb = g_particle[input[0].instanceID].color;
    
    output[3].position = float4(viewPos.x - viewScale.x / 2.f, viewPos.y - viewScale.y / 2.f, viewPos.z, 1.f);
    output[3].position = mul(output[3].position, projectionMatrix);
    output[3].tex = float2(0.f, 1.f);
    output[3].color.rgb = g_particle[input[0].instanceID].color;
    
        
    outputStream.Append(output[0]);
    outputStream.Append(output[2]);
    outputStream.Append(output[3]);
    outputStream.RestartStrip();
    
    outputStream.Append(output[2]);
    outputStream.Append(output[0]);
    outputStream.Append(output[1]);
    outputStream.RestartStrip();
}



float4 ps_main(GEOMETRY_OUTPUT input):SV_Target
{
    float4 outColor = (float4) 0.f;
    
    float alpha = PARTICLE_TEX.Sample(ati_sam, input.tex).a;
    outColor = float4(input.color.rgb, alpha);
    
    return outColor;
}


#endif