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

float4 Sample_CS(in int2 _CenterPixelPos, in Texture2D _tex, int2 _resolution)
{
    float4 fOut = (float4) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int2 iPixelPos = _CenterPixelPos + int2(i - 2, j - 2);
            if (iPixelPos.x < 0 || _resolution.x <= iPixelPos.x
                || iPixelPos.y < 0 || _resolution.y <= iPixelPos.y)
            {
                continue;
            }
            
            fOut += _tex[iPixelPos] * gaussian5x5[i * 5 + j];
        }
    }
    
    return fOut;
}


// 0 ~ 1 사이 랜덤
float Rand(in float _fKey)
{
    // 랜덤 생성 위치, 방향    
    float2 vUV = float2(_fKey + g_accumulate_time, g_accumulate_time);
    vUV.y += sin(vUV.x * 2 * 3.141592);
            
    if (vUV.x > 0)
        vUV.x = frac(vUV.x);
    else
        vUV.x = 1.f - abs(vUV.x);
            
    if (vUV.y > 0)
        vUV.y = frac(vUV.y);
    else
        vUV.y = 1.f - abs(vUV.y);
            
    vUV = vUV * g_noise_tex_resolution;
    
    return Sample_CS(vUV, noise_tex, g_resolution);
}

#endif