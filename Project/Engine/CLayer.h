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
    void AddGameObject(CGameObject* object, bool bMove);
    void RegisterObject(CGameObject* object) { layer_object_vector_.push_back(object); }

    const vector<CGameObject*> GetLayerObjects() { return layer_object_vector_; }
    const vector < CGameObject*> GetParentObjects() { return parent_object_vector_; }

    CLayer* Clone() { return nullptr; }
private:
    vector<CGameObject*> parent_object_vector_;
    vector<CGameObject*> layer_object_vector_;
    int layer_index_;

    friend class CScene;
};

