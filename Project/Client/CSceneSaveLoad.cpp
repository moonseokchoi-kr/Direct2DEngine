#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine/CSceneManager.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CComponent.h>
#include <Engine/CGameObject.h>
#include <Engine/CScript.h>

#include <Script/CScriptManager.h>

#include "commdlg.h"

void CSceneSaveLoad::Init()
{
    CPrefab::g_save_fucntion_ = &CSceneSaveLoad::SaveGameObject;
    CPrefab::g_load_function_ = &CSceneSaveLoad::LoadGameObject;
}

void CSceneSaveLoad::SaveScene()
{
	OPENFILENAME ofn = {};
    wstring contentPath = CPathManager::GetInst()->GetContentPath();
    
	//file name
	wchar_t szFile[256] = {};
	wstring strTileFolder = contentPath+L"scene\\";
	strTileFolder += L"tile";
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0scene\0*.scene\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetSaveFileName(&ofn))
    {
        SaveScene(szFile);
    }
}

CScene* CSceneSaveLoad::LoadScene()
{
    CScene* scene = nullptr;
    wstring contentPath = CPathManager::GetInst()->GetContentPath();
    OPENFILENAME ofn = {};
    //file name
    wchar_t szFile[256] = {};
    wstring strTileFolder = contentPath+L"scene\\";
    strTileFolder += L"tile";
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0scene\0*.scene\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = strTileFolder.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn))
    {
        scene = LoadScene(szFile);
    }
    return scene;
}
	

void CSceneSaveLoad::SaveScene(const wstring& filePath)
{
 
    CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();
	wstring relativePath = CPathManager::GetInst()->GetRelativePath(filePath.c_str());
    currentScene->SetScenePath(relativePath);
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
	wstring relativePath = CPathManager::GetInst()->GetRelativePath(filePath.c_str());
	newScene->SetScenePath(relativePath);
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
    SaveScript(obj, file);
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
    LoadScript(obj, file);
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

void CSceneSaveLoad::SaveScript(CGameObject* obj, FILE* file)
{
    const vector<CScript*>& script_vector = obj->GetScripts();
    
    size_t scriptCount = script_vector.size();
    fwrite(&scriptCount, sizeof(size_t), 1, file);

    for (const auto& script : script_vector)
    {
        wstring scriptName = CScriptManager::GetScriptName(script);
        SaveWStringToFile(scriptName, file);

        script->SaveToScene(file);
    }
}

void CSceneSaveLoad::LoadScript(CGameObject* obj, FILE* file)
{
\

	size_t scriptCount = 0;
	fread(&scriptCount, sizeof(size_t), 1, file);
    wstring name;
	for (size_t i=0; i<scriptCount; ++i)
	{
        LoadWStringFromFile(name, file);
		CScript* script = CScriptManager::GetScript(name);
        script->LoadFromScene(file);

        obj->AddComponent(script);
	}
}
