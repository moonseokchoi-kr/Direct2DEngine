#pragma once
#include "CComponent.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CPrefab.h"


class CScript :
    public CComponent
{
public:
    CScript();
    ~CScript();
public:
    void FinalUpdate() final {};
public:
	virtual void OnCollisionEnter(CCollider2D* otherCollider);
	virtual void OnCollision(CCollider2D* otherCollider);
	virtual void OnCollisionExit(CCollider2D* otherCollider);
private:
    void UpdateData()  override {};

protected:
    void CreateObject(CGameObject* object, int layerIndex);
    void DeleteObject(CGameObject* object, float t =0.0f);

    void Instantiate(Ptr<CPrefab> prefab, int layerIndex = 0);
    void Instantiate(Ptr<CPrefab> prefab, Vec3 position, Vec3 rotation = Vec3(0.f, 0.f, 0.f), int layerIndex = 0);



private:
    int script_type_;
};

