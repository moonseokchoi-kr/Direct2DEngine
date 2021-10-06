#pragma once
#include "singleton.h"

class CScene;

class CSceneManager :
    public CSingleton<CSceneManager>
{
    SINGLE(CSceneManager);
public:
    HRESULT Init();
    HRESULT Progress();
private:
    CScene* current_scene_;
};

