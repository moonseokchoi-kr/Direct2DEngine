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

private:
    CStructuredBuffer* particle_buffer_;
    CStructuredBuffer* particle_shared_data_buffer_;
};

