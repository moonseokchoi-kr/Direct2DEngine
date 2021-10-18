#pragma once
#include "CScript.h"
class CBulletScript :
    public CScript
{
public:
    CBulletScript();
    ~CBulletScript();

public:
    void Update() override;

public:
    CLONE(CBulletScript);
public:
    float bullet_speead_;
};

