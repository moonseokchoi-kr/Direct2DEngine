#include "pch.h"
#include "CTestComputeShader.h"

CTestComputeShader::CTestComputeShader()
	:CComputeShader(1024,1,1)
{
}

CTestComputeShader::~CTestComputeShader()
{
}

void CTestComputeShader::Excute()
{
	Dispatch(1, 1024, 1);
}

void CTestComputeShader::UpdateData()
{
	target_texture_->UpdateDataRW(0);
}

void CTestComputeShader::Clear()
{
	target_texture_->ClearRW(0);
	target_texture_ = nullptr;

	
}
