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
	UINT groupX = (particle_buffer_->GetElementCount() / GetGroupPerThreadX()) + 1;
	Dispatch(groupX, 1, 1);
}

void CParticleUpdateShader::UpdateData()
{
	parameter_.int_arr[0] = particle_buffer_->GetElementCount();

	particle_buffer_->UpdateDataRW_CS(0);
	particle_shared_buffer_->UpdateDataRW_CS(1);
}

void CParticleUpdateShader::Clear()
{
	particle_buffer_->Clear();
	particle_buffer_ = nullptr;
}
