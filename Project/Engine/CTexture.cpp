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
	if (pipline_stage_ & PS_COMPUTE)
	{
		CONTEXT->CSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());
	}
}

void CTexture::UpdateDataRW(UINT registerNumber)
{
	ID3D11UnorderedAccessView* uav = unordered_access_view_.Get();
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(registerNumber, 1, &uav, &i);
}

HRESULT CTexture::Create(UINT width, UINT height, UINT flags, DXGI_FORMAT format)
{
	texture_desc_.Width = width;
	texture_desc_.Height = height;
	
	texture_desc_.MipLevels = 1;
	texture_desc_.ArraySize = 1;


	texture_desc_.SampleDesc.Count = 1;
	texture_desc_.SampleDesc.Quality = 0;

	texture_desc_.Usage = D3D11_USAGE_DEFAULT;
	texture_desc_.CPUAccessFlags = 0;
	
	texture_desc_.BindFlags = flags;
	texture_desc_.Format = format;

	HR(DEVICE->CreateTexture2D(&texture_desc_, nullptr, texure_2D_.GetAddressOf()));

	if (D3D11_BIND_DEPTH_STENCIL & texture_desc_.BindFlags)
	{
		HR(DEVICE->CreateDepthStencilView(texure_2D_.Get(), nullptr, depth_stencil_texture_.GetAddressOf()));
	}
	else
	{
		if (D3D11_BIND_SHADER_RESOURCE & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateShaderResourceView(texure_2D_.Get(), nullptr, shader_resource_view_.GetAddressOf()));
		}
		if (D3D11_BIND_RENDER_TARGET & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateRenderTargetView(texure_2D_.Get(), nullptr, render_target_texture_.GetAddressOf()));
		}
		if (D3D11_BIND_UNORDERED_ACCESS & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateUnorderedAccessView(texure_2D_.Get(), nullptr, unordered_access_view_.GetAddressOf()));
		}
	}

	return S_OK;
}

HRESULT CTexture::Create(ComPtr<ID3D11Texture2D> texture2D)
{
	texure_2D_ = texture2D;
	texure_2D_->GetDesc(&texture_desc_);

	if (D3D11_BIND_DEPTH_STENCIL & texture_desc_.BindFlags)
	{
		HR(DEVICE->CreateDepthStencilView(texure_2D_.Get(), nullptr, depth_stencil_texture_.GetAddressOf()));
	}
	else
	{
		if (D3D11_BIND_SHADER_RESOURCE & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateShaderResourceView(texure_2D_.Get(), nullptr, shader_resource_view_.GetAddressOf()));
		}
		if (D3D11_BIND_RENDER_TARGET & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateRenderTargetView(texure_2D_.Get(), nullptr, render_target_texture_.GetAddressOf()));
		}
		if (D3D11_BIND_UNORDERED_ACCESS & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateUnorderedAccessView(texure_2D_.Get(), nullptr, unordered_access_view_.GetAddressOf()));
		}
	}
	return S_OK;
}

HRESULT CTexture::Resize(UINT width, UINT height)
{
	texture_desc_.Width = width;
	texture_desc_.Height = height;

	HR(DEVICE->CreateTexture2D(&texture_desc_, nullptr, texure_2D_.GetAddressOf()));

	if (D3D11_BIND_DEPTH_STENCIL & texture_desc_.BindFlags)
	{
		HR(DEVICE->CreateDepthStencilView(texure_2D_.Get(), nullptr, depth_stencil_texture_.GetAddressOf()));
	}
	else
	{
		if (D3D11_BIND_SHADER_RESOURCE & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateShaderResourceView(texure_2D_.Get(), nullptr, shader_resource_view_.GetAddressOf()));
		}
		if (D3D11_BIND_RENDER_TARGET & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateRenderTargetView(texure_2D_.Get(), nullptr, render_target_texture_.GetAddressOf()));
		}
		if (D3D11_BIND_UNORDERED_ACCESS & texture_desc_.BindFlags)
		{
			HR(DEVICE->CreateUnorderedAccessView(texure_2D_.Get(), nullptr, unordered_access_view_.GetAddressOf()));
		}
	}

	return S_OK;
}



void CTexture::Clear(UINT registerNumber)
{
	ID3D11ShaderResourceView* srv = nullptr;
	CONTEXT->VSSetShaderResources(registerNumber, 1, &srv);
	CONTEXT->HSSetShaderResources(registerNumber, 1, &srv);
	CONTEXT->DSSetShaderResources(registerNumber, 1, &srv);
	CONTEXT->GSSetShaderResources(registerNumber, 1, &srv);
	CONTEXT->PSSetShaderResources(registerNumber, 1, &srv);
}

void CTexture::ClearRW(UINT registerNumber)
{
	ID3D11UnorderedAccessView* uav = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(registerNumber, 1, &uav, &i);
}


