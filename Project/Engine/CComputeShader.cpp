#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
CComputeShader::CComputeShader()
    :group_thread_x_(-1)
    ,group_thread_y_(-1)
    ,group_thread_z_(-1)
{
}

CComputeShader::CComputeShader(UINT groupX, UINT groupY, UINT groupZ)
	:group_thread_x_(groupX)
	,group_thread_y_(groupY)
	,group_thread_z_(groupZ)
{
}

CComputeShader::~CComputeShader()
{
}

HRESULT CComputeShader::CreateComputeShader(const wstring& _strFilePath, const char* _funcName)
{
    HR(D3DCompileFromFile(_strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _funcName, "cs_5_0",
        D3DCOMPILE_DEBUG, 0, compute_shader_blob_.GetAddressOf(), error_blob_.GetAddressOf()));
    HR(DEVICE->CreateComputeShader(compute_shader_blob_->GetBufferPointer(), compute_shader_blob_->GetBufferSize(), nullptr, compute_shader_.GetAddressOf()));

    return S_OK;
}

void CComputeShader::Dispatch(UINT groupX, UINT groupY, UINT groupZ)
{
    UpdateData();

    static CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
    cb->SetData(&parameter_, sizeof(MaterialParameter));
    cb->UpdateDataComputeShdaer();

    CONTEXT->CSSetShader(compute_shader_.Get(), nullptr, 0);
    CONTEXT->Dispatch(groupX, groupY, groupZ);

    Clear();
}
