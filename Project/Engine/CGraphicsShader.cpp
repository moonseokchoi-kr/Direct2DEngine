#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"

CGraphicsShader::CGraphicsShader()
	:topology_(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

HRESULT CGraphicsShader::CreateVertexShader(const wstring& strFilePath, const char* funcName)
{
	if (FAILED(D3DCompileFromFile(
		strFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName,
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		vertex_shader_blob_.GetAddressOf(),
		error_blob_.GetAddressOf()
	)))
	{
		return E_FAIL;
	};

	DEVICE->CreateVertexShader(vertex_shader_blob_->GetBufferPointer(), vertex_shader_blob_->GetBufferSize(), nullptr, vertex_shader_.GetAddressOf());
	int layoutCount = sizeof(g_layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	if (FAILED(DEVICE->CreateInputLayout(g_layout, layoutCount, vertex_shader_blob_->GetBufferPointer(), vertex_shader_blob_->GetBufferSize(), layout_.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGraphicsShader::CreatePixelShader(const wstring& strFilePath, const char* funcName)
{
	if (FAILED(D3DCompileFromFile(
		strFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName,
		"ps_5_0",
		D3DCOMPILE_DEBUG,
		0,
		pixel_shader_blob_.GetAddressOf(),
		error_blob_.GetAddressOf()
	)))
	{
		return E_FAIL;
	}


	DEVICE->CreatePixelShader(pixel_shader_blob_->GetBufferPointer(), pixel_shader_blob_->GetBufferSize(), nullptr, pixel_shader_.GetAddressOf());

	return S_OK;
}

void CGraphicsShader::UpdateData()
{
	CONTEXT->VSSetShader(vertex_shader_.Get(), nullptr, 0);
	CONTEXT->HSSetShader(hull_shader_.Get(), nullptr, 0);
	CONTEXT->DSSetShader(domain_shader_.Get(), nullptr, 0);
	CONTEXT->GSSetShader(geometry_shader_.Get(), nullptr, 0);
	CONTEXT->PSSetShader(pixel_shader_.Get(), nullptr, 0);

	CONTEXT->IASetInputLayout(layout_.Get());
	CONTEXT->IASetPrimitiveTopology(topology_);

}
