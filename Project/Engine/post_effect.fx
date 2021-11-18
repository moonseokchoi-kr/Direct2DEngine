#ifndef _POSTEFFECT
#define _POSTEFFECT

#include "value.fx"
#include "function.fx"

#define NOISE tex_1
#define RAIDAL tex_2

static float strength = 10.0f;
static float speed = 100.10f;

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};

float3 permute(float3 x)
{
    return fmod(((x * 34.0) + 1.0) * x, 289.0f);
}

float snoise(float2 v)
{
    const float4 C = float4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
    float2 i = floor(v + dot(v, C.yy));
    float2 x0 = v - i + dot(i, C.xx);
    float2 i1;
    i1 = (x0.x > x0.y) ? float2(1.0, 0.0) : float2(0.0, 1.0);
    float4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = fmod(i, 289.0);
    float3 p = permute(permute(i.y + float3(0.0, i1.y, 1.0))
  + i.x + float3(0.0, i1.x, 1.0));
    float3 m = max(0.5 - float3(dot(x0, x0), dot(x12.xy, x12.xy),
    dot(x12.zw, x12.zw)), 0.0);
    m = m * m;
    m = m * m;
    float3 x = 2.0 * frac(p * C.www) - 1.0;
    float3 h = abs(x) - 0.5;
    float3 ox = floor(x + 0.5);
    float3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);
    float3 g;
    g.x = a0.x * x0.x + h.x * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}
float snoise_octaves(float2 uv, int octaves, float alpha, float beta, float2 gamma, float delta)
{
    float2 pos = uv;
    float t = 1.0;
    float s = 1.0;
    float2 q = gamma;
    float r = 0.0;
    for (int i = 0; i < octaves; i++)
    {
        r += s * snoise(pos + q);
        pos += t * uv;
        t *= beta;
        s *= alpha;
        q *= delta;
    }
    return r;
}



VTX_OUT vs_main(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), worldViewProjectionMatrix);
    output.vUV = _in.vUV;
   
    
    return output;
}

float4 ps_main(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    float2 vScreenUV = _in.vPos.xy / g_resolution;
    
    float dx = 0.0133 * snoise_octaves(vScreenUV * 2.0 + g_accumulate_time * float2(0.00323, 0.00345), 9, 0.85, -3.0, g_accumulate_time * float2(-0.0323, -0.345), 1.203);
    float dy = 0.0123 * snoise_octaves(vScreenUV * 2.0 + 3.0 + g_accumulate_time * float2(-0.00323, 0.00345), 9, 0.85, -3.0, g_accumulate_time * float2(-0.0323, -0.345), 1.203);
    
    float raidal = RAIDAL.SampleLevel(ati_sam, _in.vUV, 0);
    
    vScreenUV.x += dx * raidal;
    vScreenUV.y += dy * raidal;
   
    
    vOutColor = tex_0.Sample(ati_sam, vScreenUV);
    vOutColor.r * 3;
    
    return vOutColor;
}

    
   

#endif