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
    void SetParticleSharedBuffer(CStructuredBuffer* buffer) { particle_shared_buffer_ = buffer; }
	void SetObjectPos(Vec3 _vWorldPos)
	{
		parameter_.vec4_arr[0] = _vWorldPos;
	}

	void SetSpawnRange(Vec3 _vRange)
	{
		parameter_.vec4_arr[1] = _vRange;
	}

	void SetStartScale(Vec3 _vScale)
	{
		parameter_.vec4_arr[2] = _vScale;
	}

	void SetEndScale(Vec3 _vScale)
	{
		parameter_.vec4_arr[3] = _vScale;
	}
private:
    CStructuredBuffer* particle_buffer_;
    CStructuredBuffer* particle_shared_buffer_;
};

