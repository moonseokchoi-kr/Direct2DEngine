#pragma once
#include <Engine/CScript.h>
class CMonsterHpBar :
    public CScript
{
public:
    CMonsterHpBar();
    ~CMonsterHpBar();
public:
    void Update() override;

    CLONE(CMonsterHpBar);

};

