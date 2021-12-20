#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

SAVE_GAME_OBJECT CPrefab::g_save_fucntion_ = nullptr;
LOAD_GAME_OBJECT CPrefab::g_load_function_ = nullptr;

CPrefab::CPrefab()
    :
    CResource(RESOURCE_TYPE::PREFAB)
    ,prototype_object_(nullptr)

{
}

CPrefab::CPrefab(CGameObject* gameObject)
    :
    CResource(RESOURCE_TYPE::PREFAB)
    ,prototype_object_(gameObject)
{
}

CPrefab::~CPrefab()
{
    SafeDelete(prototype_object_);
}

CGameObject* CPrefab::Instantiate()
{
    return prototype_object_->Clone();
}

HRESULT CPrefab::Save(const wstring& relativePath)
{
	wstring contentPath = CPathManager::GetInst()->GetContentPath();
	contentPath += relativePath;

	SetRelativePath(relativePath);

	FILE* file = nullptr;
	_wfopen_s(&file, contentPath.c_str(), L"wb");

	if (nullptr == file)
		return E_FAIL;

	// Key, 경로
	SaveWStringToFile(GetKey(), file);
	SaveWStringToFile(GetRelativePath(), file);

	assert(g_save_fucntion_);
	g_save_fucntion_(prototype_object_, file);

	fclose(file);

	return S_OK;
}

HRESULT CPrefab::Load(const wstring& filePath)
{
	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");

	if (nullptr == file)
		return E_FAIL;

	// Key, 경로
	wstring key, relativePath;
	LoadWStringFromFile(key, file);
	LoadWStringFromFile(relativePath, file);

	SetKey(key);
	SetRelativePath(relativePath);

	assert(g_load_function_);
	prototype_object_ = g_load_function_(file);

	fclose(file);

	return S_OK;
}
