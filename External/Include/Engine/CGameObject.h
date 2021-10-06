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
    CComponent* GetComponent(COMPONENT_TYPE type) { return components_[static_cast<UINT>(type)]; }

    GET_COMPONENT(Transform, COMPONENT_TYPE::TRANSFORM);
    GET_COMPONENT(MeshRender, COMPONENT_TYPE::MESHRENDER);
    GET_COMPONENT(Camera, COMPONENT_TYPE::CAMERA);

private:
    array<CComponent*, static_cast<UINT>(COMPONENT_TYPE::END)> components_;

};

