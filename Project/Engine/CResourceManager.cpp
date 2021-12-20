#include "pch.h"
#include "CResourceManager.h"

CResourceManager::CResourceManager()
	:resource_array_{}
{

}

CResourceManager::~CResourceManager()
{
	for(size_t i =0; i<resource_array_.size(); ++i)
	{
		Safe_Delete_Map(resource_array_[i]);
	}
	Safe_Delete_Map(default_tex_map_);
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

Ptr<CMaterial> CResourceManager::CreateMaterial(const wstring& key, Ptr<CGraphicsShader> shader)
{
	Ptr<CMaterial> material = FindRes<CMaterial>(key);

	if (nullptr != material)
	{
		MessageBox(nullptr, L"마테리얼 이름 중복,", L"마테리얼 생성 실패", MB_OK);
		return material;
	}
	material = new CMaterial;
	wstring relativePath = CreateRelativePath(key, material);
	material->SetShader(shader);
	material->Save(relativePath);

	AddResource(key, material.Get());

	return material;
}
