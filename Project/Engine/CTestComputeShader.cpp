#include "pch.h"
#include "CTestComputeShader.h"

CTestComputeShader::CTestComputeShader()
	:CComputeShader(1024,1,1)
{
	test_buffer_ = new CStructuredBuffer;
	test_buffer_->Create(sizeof(Vec4), 1, STRUCTURE_BUFFER_TYPE::READ_WRITE, nullptr,false);
}

CTestComputeShader::~CTestComputeShader()
{
	SafeDelete(test_buffer_);
}

void CTestComputeShader::Excute()
{
	Dispatch(1, 1024, 1);
}

void CTestComputeShader::UpdateData()
{
	if (nullptr != target_texture_)
		target_texture_->UpdateDataRW(0);
	if (parameter_.int_arr[0])
	{
		test_buffer_->UpdataDataCS(13);
	}
	else
	{
		test_buffer_->UpdateDataRW_CS(1);
	}
}

void CTestComputeShader::Clear()
{
	target_texture_->ClearRW(0);
	target_texture_ = nullptr;
	
	test_buffer_->Clear();
}
