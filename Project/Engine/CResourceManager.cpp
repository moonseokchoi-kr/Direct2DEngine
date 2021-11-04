#include "pch.h"
#include "CResourceManager.h"

CResourceManager::CResourceManager()
{

}

CResourceManager::~CResourceManager()
{
	for(size_t i =0; i<resource_array_.size(); ++i)
	{
		Safe_Delete_Map(resource_array_[i]);
	}
}


Ptr<CTexture> CResourceManager::CreateTexture(const wstring& key, UINT width, UINT height, UINT flags, DXGI_FORMAT format)
{
	Ptr<CTexture> texture = FindRes<CTexture>(key);

	if (nullptr != texture)
	{
		MessageBox(nullptr, L"텍스처 이름 중복", L"텍스처 생성 실패", MB_OK);
		return texture;
	}
	texture = new CTexture;
	if (FAILED(texture->Create(width, height, flags, format)))
	{
		MessageBox(nullptr, L"텍스쳐 생성 실패", L"텍스쳐 생성 에러", MB_OK);
		assert(nullptr);
	}
	
	AddResource<CTexture>(key, texture.Get());

	return texture;
}

Ptr<CTexture> CResourceManager::CreateTexture(const wstring& key, ComPtr<ID3D11Texture2D> texture2D)
{
	Ptr<CTexture> texture = FindRes<CTexture>(key);

	if (nullptr != texture)
	{
		MessageBox(nullptr, L"텍스처 이름 중복", L"텍스처 생성 실패", MB_OK);
		return texture;
	}
	texture = new CTexture;
	if (FAILED(texture->Create(texture2D)))
	{
		MessageBox(nullptr, L"텍스쳐 생성 실패", L"텍스쳐 생성 에러", MB_OK);
		assert(nullptr);
	}

	AddResource<CTexture>(key, texture.Get());

	return texture;
}
