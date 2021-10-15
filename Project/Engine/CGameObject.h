#pragma once
#include "CEntity.h"

class CComponent;
class CMeshRender;
class CTransform;
class CCamera;


class CGameObject :
    public CEntity
{
public:
    CGameObject();
    virtual ~CGameObject();

public:
    void Update();
    void LateUpdate();
    void FinalUpdate();
    void Render();

    virtual void UpdateData() {}

public:
    void AddComponent(CComponent* comp);
    CComponent* GetComponent(COMPONENT_TYPE type) { return component_array_[static_cast<UINT>(type)]; }

    GET_COMPONENT(Transform, COMPONENT_TYPE::TRANSFORM);
    GET_COMPONENT(MeshRender, COMPONENT_TYPE::MESHRENDER);
    GET_COMPONENT(Camera, COMPONENT_TYPE::CAMERA);

    CGameObject* GetParent() { return parent_object_; }


    void AddChild(CGameObject* child);
    const vector<CGameObject*> GetChildObjects() { return child_object_vector_; }

private:
    array<CComponent*, static_cast<UINT>(COMPONENT_TYPE::END)> component_array_;
    CGameObject* parent_object_;
    vector<CGameObject*> child_object_vector_;

    int layer_index_;

    friend class CLayer;
};

