#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"

CGraphicsShader::CGraphicsShader()
	:topology_(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	,rasterizer_type_(RASTERIZER_TYPE::CULL_BACK)
	,blend_type_(BLEND_TYPE::DEFAULT)
	,depth_stencil_type_(DEPTH_STENCIL_TYPE::LESS)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

HRESULT CGraphicsShader::CreateVertexShader(const wstring& strFilePath, const char* funcName)
{
	HR(D3DCompileFromFile(
		strFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName,
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		vertex_shader_blob_.GetAddressOf(),
		error_blob_.GetAddressOf()
	));
	

	DEVICE->CreateVertexShader(vertex_shader_blob_->GetBufferPointer(), vertex_shader_blob_->GetBufferSize(), nullptr, vertex_shader_.GetAddressOf());
	int layoutCount = sizeof(g_layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	HR(DEVICE->CreateInputLayout(g_layout, layoutCount, vertex_shader_blob_->GetBufferPointer(), vertex_shader_blob_->GetBufferSize(), layout_.GetAddressOf()));

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

	ComPtr<ID3D11RasterizerState> rasterizer = CDevice::GetInst()->GetRasterizerState(rasterizer_type_);
	CONTEXT->RSSetState(rasterizer.Get());

	ComPtr<ID3D11DepthStencilState> depthStencil = CDevice::GetInst()->GetDepthStencilState(depth_stencil_type_);
	CONTEXT->OMSetDepthStencilState(depthStencil.Get(), 0);

	ComPtr<ID3D11BlendState> blend = CDevice::GetInst()->GetBlendState(blend_type_);
	CONTEXT->OMSetBlendState(blend.Get(), nullptr, 0xffffffff);


}
