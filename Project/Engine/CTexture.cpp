#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"
CTexture::CTexture()
	:texture_desc_{}
{
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Load(const wstring& strFilePath)
{
	wchar_t ext[50] = L"";
	_wsplitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, 50);

	HRESULT hr = S_OK;

	if (!wcscmp(L".DDS", ext) || !wcscmp(L".dds", ext))
	{
		hr = LoadFromDDSFile(strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, image_);
	}
	else if (!wcscmp(L".TGA", ext) || !wcscmp(L".tga", ext))
	{
		hr = LoadFromTGAFile(strFilePath.c_str(), nullptr, image_);
	}
	else 
	{
		hr = LoadFromWICFile(strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, image_);
	}

	if (FAILED(hr))
		return E_FAIL;

	CreateShaderResourceView(DEVICE, image_.GetImages(), image_.GetImageCount(), image_.GetMetadata(), shader_resource_view_.GetAddressOf());

	shader_resource_view_->GetResource((ID3D11Resource**)texure_2D_.GetAddressOf());
	texure_2D_->GetDesc(&texture_desc_);

	return S_OK;
}

void CTexture::UpdateData()
{
	if (pipline_stage_ & PS_VERTEX)
	{
		CONTEXT->VSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());
	}
	if (pipline_stage_ & PS_HULL)
	{
		CONTEXT->HSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());
	}
	if (pipline_stage_ & PS_DOMAIN)
	{
		CONTEXT->DSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());
	}
	if (pipline_stage_ & PS_GEOMETRY)
	{
		CONTEXT->GSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());
	}
	if (pipline_stage_ & PS_PIXEL)
	{
		CONTEXT->PSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());
	}
}
