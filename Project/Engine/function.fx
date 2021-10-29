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


#endif