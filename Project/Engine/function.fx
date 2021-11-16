#ifndef _FUNCTION_
#define _FUNCTION_
#include "struct.fx"
#include "value.fx"

void GetLightColor(int lightIndex, float3 worldPosition, inout LightColor color)
{
    if(1==light2DArray[lightIndex].lightType)
    {
        float dist = distance(light2DArray[lightIndex].lightPosition.xy, worldPosition.xy);
        float ratio = 0.f;
        ratio = cos(saturate(dist / light2DArray[lightIndex].lightRange) * (PI / 2.f));
        color.lightColor += light2DArray[lightIndex].lightColor.lightColor * ratio;
    }
}

static float gaussian5x5[25] =
{
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.023792, 0.094907, 0.150342, 0.094907, 0.023792,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
};

float4 Sample_CS(in int2 centerPixelPos, in Texture2D tex, int2 resolution)
{
    float4 output = (float4) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int2 pixelPos = centerPixelPos + int2(i - 2, j - 2);
            
            if (pixelPos.x < 0 || resolution.x <= pixelPos.x
                || pixelPos.y < 0 || resolution.y <= pixelPos.y)
            {
                continue;
            }
            
            output += tex[pixelPos] * gaussian5x5[i * 5 + j];
        }
    }
    
    return output;
}

// 0 ~ 1 ���� ����
float Rand(in float key)
{
    // ���� ���� ��ġ, ����    
    float2 uv = float2(key + g_accumulate_time, g_accumulate_time);
    uv.y += sin(uv.x * 2 * 3.141592);
            
    if (uv.x > 0)
        uv.x = frac(uv.x);
    else
        uv.x = 1.f - abs(uv.x);
            
    if (uv.y > 0)
        uv.y = frac(uv.y);
    else
        uv.y = 1.f - abs(uv.y);
        
    int2 pixelPos = (int2) (g_noise_tex_resolution * uv);
    
    return Sample_CS(pixelPos, noise_tex, g_noise_tex_resolution).x;
}


float DistributeRandomFloat(in float key, float min, float max)
{
    return min + (max - min) * Rand(key);
}

float3 DistributeRandomVec3(in float key, float3 min, float3 max)
{
    return min + (max - min) * float3(Rand(key), Rand(key + 0.1f), Rand(key + 0.2f));
}

#endif