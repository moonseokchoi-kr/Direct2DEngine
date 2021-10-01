#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"
CTexture::CTexture()
	:m_desc{}
{
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Load(const wstring& _strFilePath)
{
	wchar_t ext[50] = L"";
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, 50);

	HRESULT hr = S_OK;

	if (!wcscmp(L".DDS", ext) || !wcscmp(L".dds", ext))
	{
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_image);
	}
	else if (!wcscmp(L".TGA", ext) || !wcscmp(L".tga", ext))
	{
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_image);
	}
	else 
	{
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_image);
	}

	if (FAILED(hr))
		return E_FAIL;

	CreateShaderResourceView(DEVICE, m_image.GetImages(), m_image.GetImageCount(), m_image.GetMetadata(), m_SRV.GetAddressOf());

	m_SRV->GetResource((ID3D11Resource**)m_tex2D.GetAddressOf());
	m_tex2D->GetDesc(&m_desc);

	return S_OK;
}

void CTexture::UpdateData()
{
	if (m_piplineStage & PS_VERTEX)
	{
		CONTEXT->VSSetShaderResources(m_registerNum, 1, m_SRV.GetAddressOf());
	}
	if (m_piplineStage & PS_HULL)
	{
		CONTEXT->HSSetShaderResources(m_registerNum, 1, m_SRV.GetAddressOf());
	}
	if (m_piplineStage & PS_DOMAIN)
	{
		CONTEXT->DSSetShaderResources(m_registerNum, 1, m_SRV.GetAddressOf());
	}
	if (m_piplineStage & PS_GEOMETRY)
	{
		CONTEXT->GSSetShaderResources(m_registerNum, 1, m_SRV.GetAddressOf());
	}
	if (m_piplineStage & PS_PIXEL)
	{
		CONTEXT->PSSetShaderResources(m_registerNum, 1, m_SRV.GetAddressOf());
	}
}
