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
    void AddComponent(CComponent* _comp);
    CComponent* GetComponent(COMPONENT_TYPE _type) { return m_components[ENUM_TO_NUMBER(_type)]; }

    GET_COMPONENT(Transform, COMPONENT_TYPE::TRANSFORM);
    GET_COMPONENT(MeshRender, COMPONENT_TYPE::MESHRENDER);

private:
    array<CComponent*, ENUM_TO_NUMBER(COMPONENT_TYPE::END)> m_components;

};

