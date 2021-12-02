#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

#define TILE_MAP_COLUMN int_0
#define TILE_MAP_ROW int_1

#define TILE_COL int_2
#define TILE_ROW int_3

#define ATLAS_TEX tex_0
#define ATLAS_RESOLUTION vec2_0
#define ATLAS_TILE_SIZE vec2_1

StructuredBuffer<int> idx_buffer : register(t20);

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
    VERTEX_OUTPUT output = (VERTEX_OUTPUT) 0.f;
    
    output.position = mul(float4(input.position, 1.f), worldViewProjectionMatrix);

    output.tex = input.tex;
    
    return output;

}

float4 ps_main(VERTEX_OUTPUT input):SV_Target
{
    float4 outColor = (float4) 0.f;
    
    float2 uv = input.tex * float2(TILE_MAP_COLUMN, TILE_MAP_ROW);
    
    int2 bufferIdx = floor(uv);
    
    
    int idx = idx_buffer[TILE_MAP_COLUMN * bufferIdx.y + bufferIdx.x];
    int width = 1 / ATLAS_TILE_SIZE.x;
    
    int row = idx / width;
    int column = idx % width;
    
    float2 tileUV = input.tex * float2(TILE_MAP_COLUMN, TILE_MAP_ROW);
    
    tileUV = frac(tileUV);
    
    tileUV *= ATLAS_TILE_SIZE;

    tileUV = ATLAS_TILE_SIZE * float2(column, row) + tileUV;

    
    outColor = ATLAS_TEX.Sample(mip_sam, tileUV);
    
    return outColor;
}


#endif