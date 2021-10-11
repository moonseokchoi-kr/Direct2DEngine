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

private:
    float player_move_speed_;

};

