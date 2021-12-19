#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine/CSceneManager.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CComponent.h>
#include <Engine/CGameObject.h>


void CSceneSaveLoad::SaveScene(const wstring& filePath)
{
    CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();
    FILE* file = nullptr;
    _wfopen_s(&file, filePath.c_str(), L"wb");
    assert(file);

    currentScene->SaveToScene(file);

    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        CLayer* layer = currentScene->GetLayer(i);
        layer->SaveToScene(file);

        const vector<CGameObject*>& parent_vector = layer->GetParentObjects();
        size_t objectCount = parent_vector.size();
        fwrite(&objectCount, sizeof(size_t), 1, file);

        for (const auto& object : parent_vector)
        {
            SaveGameObject(object, file);
        }
    }

    fclose(file);
}

CScene* CSceneSaveLoad::LoadScene(const wstring& filePath)
{
    CScene* newScene = new CScene;
	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");
	assert(file);

	newScene->LoadFromScene(file);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* layer = newScene->GetLayer(i);
		layer->LoadFromScene(file);

        size_t objectCount = 0;
        fread(&objectCount, sizeof(size_t), 1, file);
        for (size_t j = 0; j < objectCount; ++j)
        {
            CGameObject* object = LoadGameObject(file);
            layer->AddGameObject(object, false);
        }
		
	}

    fclose(file);

    return newScene;
}

void CSceneSaveLoad::SaveGameObject(CGameObject* obj, FILE* file)
{
    //save object name
    obj->SaveToScene(file);

    //save component
    int componentCount = obj->GetComponentCount();
    fwrite(&componentCount, sizeof(int), 1, file);

    for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
    {
        CComponent* component = obj->GetComponent((COMPONENT_TYPE)i);
        if (nullptr != component)
        {
            component->SaveToScene(file);
        }
    }

    //script

    //child object 
    const vector<CGameObject*>& child_vector = obj->GetChildObjects();
    size_t childCount = child_vector.size();
    fwrite(&childCount, sizeof(size_t), 1, file);

    for (size_t i = 0; i < childCount; ++i)
    {
        SaveGameObject(child_vector[i], file);
    }
}

CGameObject* CSceneSaveLoad::LoadGameObject(FILE* file)
{
    CGameObject* obj = new CGameObject;

    //Load object name
	obj->LoadFromScene(file);

	//Load component
	int componentCount = 0;
	fread(&componentCount, sizeof(int), 1, file);

	for (int i = 0; i < (int)componentCount; ++i)
	{
        UINT type = 0;
        fread(&type, sizeof(UINT), 1, file);

	    obj->AddComponent((COMPONENT_TYPE)type);

        CComponent* component = obj->GetComponent((COMPONENT_TYPE)type);
        component->LoadFromScene(file);
	}

    //Script

    //child object

	size_t childCount = 0;
	fread(&childCount, sizeof(size_t), 1, file);

	for (size_t i = 0; i < childCount; ++i)
	{
		CGameObject* child = LoadGameObject(file);
        obj->AddChild(child);
	}

    return obj;
}
