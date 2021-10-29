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
    void OnCollisionEnter(CGameObject* otherObject) override;
    void OnCollisionExit(CGameObject* otherObjectr) override;
public:
    void CreateBullet();
public:
    CLONE(CPlayerScript);
private:
    float player_move_speed_;
    float player_bullet_attack_speed_;
    int is_hit_;
    float hit_accumulated_time_;
    float accumulated_time_;
    Ptr<CPrefab> player_bullet_prefab_;
};

