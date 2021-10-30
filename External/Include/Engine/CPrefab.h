#pragma once
#include "CResource.h"

class CGameObject;

class CPrefab :
    public CResource
{
public:
    ~CPrefab();

public:
    void UpdateData() override {};
    CGameObject* Instantiate();
    
private:
	CPrefab();
	CPrefab(CGameObject* gameObject);
private:
    CGameObject* prototype_object_;

    friend class CResourceManager;

};

