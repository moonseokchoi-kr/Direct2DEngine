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
	void AddRes(const wstring& _key, T* _res);
	template <typename T>
	T* FindRes(const wstring& _key);
private:
	template<typename T>
	RES_TYPE FindResType(const wstring& key);
private:
    array<unordered_map<wstring, CResource*>,(UINT)RES_TYPE::END> m_resources;

};

template<typename T>
inline void CResourceManager::AddRes(const wstring& _key, T* _res)
{
	RES_TYPE _type = FindResType<T>(_key);

	m_resources[(UINT)_type].insert(make_pair(_key, _res));
}

template<typename T>
inline T* CResourceManager::FindRes(const wstring& _key)
{
	RES_TYPE _type = FindResType<T>(_key);

	unordered_map<wstring, CResource*>::iterator iter = m_resources[(UINT)_type].find(_key);

	if (m_resources[(UINT)_type].end() == iter)
	{
		return nullptr;
	}

	return (T*)iter->second;
}

template<typename T>
inline RES_TYPE CResourceManager::FindResType(const wstring& key)
{
	const type_info& t = typeid(T);
	RES_TYPE _type = RES_TYPE::END;
	if (t.hash_code() == typeid(CMesh).hash_code())
	{
		_type = RES_TYPE::MESH;
	}
	if (t.hash_code() == typeid(CShader).hash_code())
	{
		_type = RES_TYPE::SHADER;
	}
	if (t.hash_code() == typeid(CTexture).hash_code())
	{
		_type = RES_TYPE::TEXTURE;
	}

	return _type;
}
