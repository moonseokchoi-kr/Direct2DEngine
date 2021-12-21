#pragma once
#include <Engine/CScript.h>
class CMonsterHpBarScript :
    public CScript
{
public:
    CMonsterHpBarScript();
    ~CMonsterHpBarScript();
public:
    void Update() override;

    CLONE(CMonsterHpBarScript);

};

