#pragma once
#include "CScript.h"
class CPlayerScript :
    public CScript
{
public:
    CPlayerScript();
    ~CPlayerScript();

public:
    void Update() override;
public:
    CLONE(CPlayerScript);
private:
    float player_move_speed_;

};

