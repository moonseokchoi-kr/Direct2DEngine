#pragma  once


template<typename T, int N>

void Safe_Delete_Array(array<T, N> arr)
{
	for (int i = 0; i < arr.size(); ++i)
	{
		if(nullptr != arr[i])
			delete arr[i];
	}
}

template<typename T1, typename T2>

void Safe_Delete_Map(unordered_map<T1,T2> map)
{
	typename unordered_map<T1, T2>::iterator iter = map.begin();

	for (; iter != map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}

	map.clear();
}


template<typename T>
void Safe_Delete_Vec(vector<T> vec)
{
	for (auto& v : vec)
	{
		if (nullptr != v)
		{
			delete v;
		}
	}
	vec.clear();
}


void SaveWStringToFile(const wstring& str, FILE* file);
void LoadWStringFromFile(wstring& str, FILE* file);

#include "CResource.h"
template<typename T>
void SaveResReference(Ptr<T> res, FILE* file)
{
	int i = !!res.Get();
	fwrite(&i, sizeof(int), 1, file);

	if (i)
	{
		SaveWStringToFile(res->GetKey(), file);
		SaveWStringToFile(res->GetRelativePath(), file);
	}
}

#include "CResourceManager.h"
template<typename T>
void LoadResReference(Ptr<T>& res, FILE* file)
{
	int i = 0;
	fread(&i, sizeof(int), 1, file);

	if (i)
	{
		wstring strkey, strRelativePath;

		LoadWStringFromFile(strkey, file);
		LoadWStringFromFile(strRelativePath, file);

		res = CResourceManager::GetInst()->LoadRes<T>(strkey, strRelativePath);
	}

}

