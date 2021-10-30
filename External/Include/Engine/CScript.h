#pragma once
#include "CComponent.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CCollider2D.h"
#include "CPrefab.h"

class CCollider2D;

class CScript :
    public CComponent
{
public:
    CScript();
    ~CScript();
public:
    void FinalUpdate() final {};

private:
    void UpdateData()  override {};

protected:
    void CreateObject(CGameObject* object, int layerIndex);
    void DeleteObject(CGameObject* object, float t =0.0f);

    void Instantiate(Ptr<CPrefab> prefab, int layerIndex = 0);
    void Instantiate(Ptr<CPrefab> prefab, Vec3 position, int layerIndex = 0, Vec3 rotation = Vec3(0.f, 0.f, 0.f) );
protected:
	virtual void OnCollisionEnter(CGameObject* otherObject);
	virtual void OnCollision(CGameObject* otherObject);
	virtual void OnCollisionExit(CGameObject* otherObject);


private:
    int script_type_;

    friend class CCollider2D;
};

