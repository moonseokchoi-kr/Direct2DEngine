#pragma once
#include "CMesh.h"
#include "CTexture.h"
#include "CComputeShader.h"
#include "CPrefab.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CGraphicsShader.h"
#include "CPathManager.h"

class CResource;
class CGameObject;


class CResourceManager :
    public CSingleton<CResourceManager>
{
    SINGLE(CResourceManager);

public:
    void Init();

public:
    template <typename T>
	void AddResource(const wstring& key, T* res);

	template <typename T>
	Ptr<T> FindRes(const wstring& key);
	template<typename T>
	Ptr<T> LoadRes(const wstring& key, const wstring& strRelativePath);

	template<typename T>
	const unordered_map<wstring, CResource*> GetResource();
	
	Ptr<CGraphicsShader> LoadGraphicShader(const wstring& key, const wstring& strPath, BLEND_TYPE blendType = BLEND_TYPE::DEFAULT, DEPTH_STENCIL_TYPE depthType = DEPTH_STENCIL_TYPE::LESS, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	




public:
	void AddPrefab(const wstring& stringKey, CGameObject* prototype);

	Ptr<CTexture> CreateTexture(const wstring& key, UINT width, UINT height, UINT flags, DXGI_FORMAT format);
	Ptr<CTexture> CreateTexture(const wstring& key, ComPtr<ID3D11Texture2D> texture2D);
private:
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultTexture();
	void CreateDefaultMaterial();
	void CreateDefaultComputeShader();
private:
    array<unordered_map<wstring, CResource*>,static_cast<UINT>(RES_TYPE::END)> resource_array_;
	unordered_map<wstring, CTexture*> default_tex_map_;

};




template<typename  T> 
inline RES_TYPE GetResourceType()
{
	const type_info& info = typeid(T);

	RES_TYPE type = RES_TYPE::END;

	if (info.hash_code() == typeid(CMesh).hash_code())
	{
		type = RES_TYPE::MESH;
	}
	if (info.hash_code() == typeid(CGraphicsShader).hash_code())
	{
		type = RES_TYPE::GRAPHIC_SHADER;
	}
	if (info.hash_code() == typeid(CComputeShader).hash_code())
	{
		type = RES_TYPE::PREFAB;
	}
	if (info.hash_code() == typeid(CTexture).hash_code())
	{
		type = RES_TYPE::TEXTURE;
	}
	if (info.hash_code() == typeid(CMaterial).hash_code())
	{
		type = RES_TYPE::MATERIAL;
	}
	if (info.hash_code() == typeid(CPrefab).hash_code())
	{
		type = RES_TYPE::PREFAB;
	}
	
	return type;
}


template<typename T>
inline const unordered_map<wstring, CResource*> CResourceManager::GetResource()
{
	RES_TYPE type = GetResourceType<T>();
	return resource_array_[(UINT)type];
};

template <typename T>
inline void CResourceManager::AddResource(const wstring& key, T* res)
{
	RES_TYPE type = GetResourceType<T>();
	res->SetName(key);
	resource_array_[static_cast<UINT>(type)].insert(make_pair(key,res));
}

template <typename T>
inline Ptr<T> CResourceManager::FindRes(const wstring& key)
{
	RES_TYPE type = GetResourceType<T>();
	unordered_map<wstring, CResource*>::iterator iterator = resource_array_[static_cast<UINT>(type)].find(key);

	if(iterator == resource_array_[static_cast<UINT>(type)].end())
	{
		return nullptr;
	}
	return (T*)iterator->second;
}

template<typename T>
inline Ptr<T> CResourceManager::LoadRes(const wstring& key, const wstring& strRelativePath)
{
	RES_TYPE type = GetResourceType<T>();

	unordered_map<wstring, CResource*>::iterator iter = resource_array_[static_cast<UINT>(type)].find(key);

	if (iter != resource_array_[static_cast<UINT>(type)].end())
		return (T*)iter->second;

	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += strRelativePath;

	CResource* res = new T;

	if (FAILED(res->Load(strPath)))
	{
		MessageBox(nullptr, L"리소스 로딩 실패", L"리소스 로딩 에러", MB_OK);
		assert(nullptr);
	}
	
	AddResource<T>(key, (T*)res);

	return Ptr<T>((T*)res);
}


