#include "pch.h"
#include "CParticleUpdateShader.h"

CParticleUpdateShader::CParticleUpdateShader()
	:CComputeShader(32,1,1)
	,particle_buffer_(nullptr)
{
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

void CParticleUpdateShader::Excute()
{
}

void CParticleUpdateShader::UpdateData()
{
	particle_buffer_->UpdateDataRW_CS(0);
}

void CParticleUpdateShader::Clear()
{
	particle_buffer_->Clear();
	particle_buffer_ = nullptr;
}
