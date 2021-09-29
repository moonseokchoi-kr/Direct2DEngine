#include "pch.h"
#include "CShader.h"


#include "CDevice.h"

CShader::CShader()
	:m_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CShader::~CShader()
{
}

HRESULT CShader::CreateVtxShader(const wstring& _strFilePath, const char* _funcName)
{
	if (FAILED(D3DCompileFromFile(
		_strFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_funcName,
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		m_vsBlob.GetAddressOf(),
		m_errBlob.GetAddressOf()
	)))
	{
		return E_FAIL;
	};

	DEVICE->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_vtxShader.GetAddressOf());
	int layoutCount = sizeof(g_layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	
	if (FAILED(DEVICE->CreateInputLayout(g_layout, layoutCount, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_layout.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CShader::CreatePxlShader(const wstring& _strFilePath, const char* _funcName)
{
	if (FAILED(D3DCompileFromFile(
		_strFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_funcName,
		"ps_5_0",
		D3DCOMPILE_DEBUG,
		0,
		m_psBlob.GetAddressOf(),
		m_errBlob.GetAddressOf()
	)))
	{
		return E_FAIL;
	}


	DEVICE->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_pxlShader.GetAddressOf());

	return S_OK;
}

void CShader::UpdateData()
{
	CONTEXT->VSSetShader(m_vtxShader.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_hullShader.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_domainShader.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_geoShader.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_pxlShader.Get(), nullptr, 0);

	CONTEXT->IASetInputLayout(m_layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_topology);

}
