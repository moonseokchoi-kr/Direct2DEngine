#pragma once
#include "CEntity.h"
#include "CGameObject.h"


class CComponent :
    public CEntity
{
public:
    CComponent(COMPONENT_TYPE m_type);
    ~CComponent();

public:
    virtual void Start(){}
    virtual void Update() {}
    virtual void LateUpdate() {}
    virtual void FinalUpdate() = 0;
    virtual void Render() {}
    virtual void UpdateData() {};
public:
    COMPONENT_TYPE GetType() { return type_; }
    CGameObject* GetOwner() { return owner_; }


    CTransform* GetTransform() { return owner_->Transform(); }
    CMeshRender* GetMeshRender() { return owner_->MeshRender(); }
    CCamera* GetCamera() { return owner_->Camera(); }
    CCollider2D* GetCollider2D(){ return owner_->Collider2D(); }
    CAnimator2D* GetAnimator2D() { return owner_->Animator2D(); }

public:
    virtual void SaveToScene(FILE* file);
    virtual void LoadFromScene(FILE* file);
public:
    virtual CComponent* Clone() = 0;

private:
    COMPONENT_TYPE type_;
    CGameObject* owner_;

    friend class CGameObject;
};

