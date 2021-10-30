#pragma once
#include "singleton.h"

class CScene;

class CSceneManager :
    public CSingleton<CSceneManager>
{
    SINGLE(CSceneManager);
public:
	void Init();
    void Progress();

public:
    CScene* GetCurrentScene() { return current_scene_; }
private:
    CScene* current_scene_;
};

