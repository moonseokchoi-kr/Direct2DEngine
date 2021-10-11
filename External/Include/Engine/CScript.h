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

private:
    int script_type_;
};

