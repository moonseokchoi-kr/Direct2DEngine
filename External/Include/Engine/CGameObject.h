#pragma once
#include "CEntity.h"
class CComponent;
class CMeshRender;
class CTransform;

class CGameObject :
    public CEntity
{
public:
    CGameObject();
    virtual ~CGameObject();

public:
    virtual void Update();
    virtual void Render();

    virtual void UpdateData() {}

public:
    void AddComponent(CComponent* comp);
    CComponent* GetComponent(COMPONENT_TYPE type) { return components_[static_cast<UINT>(type)]; }

    GET_COMPONENT(Transform, COMPONENT_TYPE::TRANSFORM);
    GET_COMPONENT(MeshRender, COMPONENT_TYPE::MESHRENDER);

private:
    array<CComponent*, static_cast<UINT>(COMPONENT_TYPE::END)> components_;

};

