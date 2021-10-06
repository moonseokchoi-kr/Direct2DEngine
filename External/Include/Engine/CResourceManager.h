#pragma once
#include "singleton.h"

class CResource;
class CMesh;
class CShader;
class CTexture;
class CResourceManager :
    public CSingleton<CResourceManager>
{
    SINGLE(CResourceManager);

public:
    void Init();
private:
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultTexture();
	void CreateDefaultMaterial();
public:
    template <typename T>
	void AddRes(const wstring& key, T* res);
	template <typename T>
	T* FindRes(const wstring& key);
private:
	template<typename T>
	RES_TYPE FindResType(const wstring& key);
private:
    array<unordered_map<wstring, CResource*>,static_cast<UINT>(RES_TYPE::END)> resources_;

};

template<typename T>
inline void CResourceManager::AddRes(const wstring& key, T* res)
{
	RES_TYPE _type = FindResType<T>(key);

	resources_[static_cast<UINT>(_type)].insert(make_pair(key, res));
}

template<typename T>
inline T* CResourceManager::FindRes(const wstring& key)
{
	RES_TYPE _type = FindResType<T>(key);

	unordered_map<wstring, CResource*>::iterator iter = resources_[static_cast<UINT>(_type)].find(key);

	if (resources_[static_cast<UINT>(_type)].end() == iter)
	{
		return nullptr;
	}

	return static_cast<T*>(iter->second);
}

template<typename T>
inline RES_TYPE CResourceManager::FindResType(const wstring& key)
{
	const type_info& t = typeid(T);
	RES_TYPE type = RES_TYPE::END;
	if (t.hash_code() == typeid(CMesh).hash_code())
	{
		type = RES_TYPE::MESH;
	}
	if (t.hash_code() == typeid(CShader).hash_code())
	{
		type = RES_TYPE::SHADER;
	}
	if (t.hash_code() == typeid(CTexture).hash_code())
	{
		type = RES_TYPE::TEXTURE;
	}

	return type;
}
