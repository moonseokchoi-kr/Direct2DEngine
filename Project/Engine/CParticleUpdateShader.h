#pragma once
#include "CComputeShader.h"

class CParticleUpdateShader :
    public CComputeShader
{
public:
    CParticleUpdateShader();
    ~CParticleUpdateShader();

public:
    void Excute() override;
protected:
    void UpdateData() override;
    void Clear() override;
public:
    void SetParticleBuffer(CStructuredBuffer* buffer) { particle_buffer_ = buffer; }
    void SetParticleSharedDataBuffer(CStructuredBuffer* buffer) { particle_shared_data_buffer_ = buffer; }
    void SetObjectPosition(Vec3 worldPosition) { parameter_.vec4_arr[0] = worldPosition; }
    void SetSpawnRange(Vec3 range) { parameter_.vec4_arr[1] = range; }
    void SetMinScale(Vec3 scale) { parameter_.vec4_arr[2] = scale; }
    void SetMaxScale(Vec3 scale) { parameter_.vec4_arr[3] = scale; }
    void SetStartColor(Vec4 color) { parameter_.vec2_arr[0] = Vec2(color.x, color.y); parameter_.vec2_arr[1] = Vec2(color.z, color.w); }
    void SetEndColor(Vec4 color) { parameter_.vec2_arr[2] = Vec2(color.x, color.y); parameter_.vec2_arr[3] = Vec2(color.z, color.w); }
    void SetMinLife(float minLife) { parameter_.float_arr[2] = minLife; }
    void SetMaxLife(float maxLife) { parameter_.float_arr[3] = maxLife; }
    void SetMinSpeed(float minSpeed) { parameter_.float_arr[0] = minSpeed; }
    void SetMaxSpeed(float maxSpeed) { parameter_.float_arr[1] = maxSpeed; }
private:

    CStructuredBuffer* particle_buffer_;
    CStructuredBuffer* particle_shared_data_buffer_;
};

