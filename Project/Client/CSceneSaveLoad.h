#pragma once

#include <Engine/CScene.h>

class CSceneSaveLoad
{
public:
	static void Init();
	static void SaveScene();
	static CScene* LoadScene();
	static void SaveScene(const wstring& filePath);
	static CScene* LoadScene(const wstring& filePath);

private:
	static void SaveGameObject(CGameObject* obj, FILE* file);
	static CGameObject* LoadGameObject(FILE* file);

	static void SaveScript(CGameObject* obj, FILE* file);
	static void LoadScript(CGameObject* obj, FILE* file);
 
};

