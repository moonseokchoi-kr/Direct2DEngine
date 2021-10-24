#pragma once
#include "CScript.h"
class CMonsterBulletScript :
    public CScript
{
public:
    CMonsterBulletScript();
    ~CMonsterBulletScript();

public:
    void Update() override;
private:
    void MoveBulluet();

private:
    float angle;
    float bullet_speed;
    float bullet_speed_rate;
    float angle_rate;

};

