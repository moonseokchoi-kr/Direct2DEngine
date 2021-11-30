#pragma once
#include "singleton.h"

class CScene;
class CGameObject;

class CSceneManager :
    public CSingleton<CSceneManager>
{
    SINGLE(CSceneManager);
public:
	void Init();
    void Progress();

public:
    CScene* GetCurrentScene() { return current_scene_; }
    SCENE_MODE GetSceneMode() { return scene_mode_; }
    void InitTestMap();
    CGameObject* FindObjectByName(const wstring& name);
    void FindObjectByName(const wstring& name, vector<CGameObject*>& object_vector);

    void SetSceneMode(SCENE_MODE mode);
private:
    CScene* current_scene_;
    SCENE_MODE scene_mode_;
};

