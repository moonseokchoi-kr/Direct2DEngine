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
		MessageBox(nullptr, L"�ؽ�ó �̸� �ߺ�", L"�ؽ�ó ���� ����", MB_OK);
		return texture;
	}
	texture = new CTexture;
	if (FAILED(texture->Create(width, height, flags, format)))
	{
		MessageBox(nullptr, L"�ؽ��� ���� ����", L"�ؽ��� ���� ����", MB_OK);
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
		MessageBox(nullptr, L"�ؽ�ó �̸� �ߺ�", L"�ؽ�ó ���� ����", MB_OK);
		return texture;
	}
	texture = new CTexture;
	if (FAILED(texture->Create(texture2D)))
	{
		MessageBox(nullptr, L"�ؽ��� ���� ����", L"�ؽ��� ���� ����", MB_OK);
		assert(nullptr);
	}

	AddResource<CTexture>(key, texture.Get());

	return texture;
}
