#pragma once
#include "CComponent.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransform.h"



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
    void CreateObject(CGameObject* object, Vec3 position,int layerIndex);
    void DeleteObject(CGameObject* object, float t =0.0f);

private:
    int script_type_;
};

