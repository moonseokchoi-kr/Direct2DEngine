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
private:
    CScene* current_scene_;
};

