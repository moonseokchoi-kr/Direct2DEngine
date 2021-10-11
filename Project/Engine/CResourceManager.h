#pragma once
#include "CMesh.h"

class CResource;
class CMesh;
class CShader;
class CTexture;
class CMaterial;

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
	T* FindRes(const wstring& key);


private:
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultTexture();
	void CreateDefaultMaterial();
private:
    array<unordered_map<wstring, CResource*>,static_cast<UINT>(RES_TYPE::END)> resources_;

};

template<typename  T>
RES_TYPE GetResourceType()
{
	const type_info& info = typeid(T);

	RES_TYPE type = RES_TYPE::END;

	if (info.hash_code() == typeid(CMesh).hash_code())
	{
		type = RES_TYPE::MESH;
	}
	if (info.hash_code() == typeid(CShader).hash_code())
	{
		type = RES_TYPE::SHADER;
	}
	if (info.hash_code() == typeid(CTexture).hash_code())
	{
		type = RES_TYPE::TEXTURE;
	}
	if (info.hash_code() == typeid(CMaterial).hash_code())
	{
		type = RES_TYPE::MATERIAL;
	}
	return type;
}


template <typename T>
inline void CResourceManager::AddResource(const wstring& key, T* res)
{
	RES_TYPE type = GetResourceType<T>();
	resources_[static_cast<UINT>(type)].insert(make_pair(key,res));
}

template <typename T>
inline T* CResourceManager::FindRes(const wstring& key)
{
	RES_TYPE type = GetResourceType<T>();
	unordered_map<wstring, CResource*>::iterator iterator = resources_[static_cast<UINT>(type)].find(key);

	if(iterator == resources_[static_cast<UINT>(type)].end())
	{
		return nullptr;
	}
	return (T*)iterator->second;
}


