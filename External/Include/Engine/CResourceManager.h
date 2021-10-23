#pragma once
#include "CMesh.h"
#include "CTexture.h"

class CResource;
class CMesh;
class CGraphicsShader;
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
	Ptr<T> FindRes(const wstring& key);


private:
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultTexture();
	void CreateDefaultMaterial();
private:
    array<unordered_map<wstring, CResource*>,static_cast<UINT>(RES_TYPE::END)> resrouce_array_;

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
	if (info.hash_code() == typeid(CGraphicsShader).hash_code())
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
	resrouce_array_[static_cast<UINT>(type)].insert(make_pair(key,res));
}

template <typename T>
inline Ptr<T> CResourceManager::FindRes(const wstring& key)
{
	RES_TYPE type = GetResourceType<T>();
	unordered_map<wstring, CResource*>::iterator iterator = resrouce_array_[static_cast<UINT>(type)].find(key);

	if(iterator == resrouce_array_[static_cast<UINT>(type)].end())
	{
		return nullptr;
	}
	return (T*)iterator->second;
}


