#pragma once
#include "CEntity.h"
class CGameObject;

class CLayer :
    public CEntity
{
public:
    CLayer();
    ~CLayer();
public:
    void Update();
    void LateUpdate();
    void FinalUpdate();
    void Render();

    virtual void UpdateData() {};

public:
    void AddGameObject(CGameObject* object);
private:
    vector<CGameObject*> objects_;
};

