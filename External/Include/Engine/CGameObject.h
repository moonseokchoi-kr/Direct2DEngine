#pragma once
#include "CEntity.h"

class CComponent;
class CMeshRender;
class CTransform;
class CCollider2D;
class CCamera;


class CGameObject :
    public CEntity
{
public:
    CGameObject();
    CGameObject(const CGameObject& origin);
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
    GET_COMPONENT(Collider2D, COMPONENT_TYPE::COLLIDER2D);
    CGameObject* GetParent() { return parent_object_; }


    void AddChild(CGameObject* child);
    const vector<CGameObject*> GetChildObjects() { return child_object_vector_; }

    bool IsDead() { return object_dead_; }
    bool IsDelete() { return object_delete_; }
public:
    CLONE(CGameObject);

private:
    void SetDead(float t) { object_dead_ = true; dead_time = t; }

private:
    array<CComponent*, static_cast<UINT>(COMPONENT_TYPE::END)> component_array_;
    CGameObject* parent_object_;
    vector<CGameObject*> child_object_vector_;
    bool object_dead_;
    bool object_delete_;
    float delay_accumulated_time;
    float dead_time;
    int layer_index_;

    friend class CLayer;
    friend class CEventManager;
};

