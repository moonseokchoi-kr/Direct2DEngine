#pragma once
#include <Engine/CScript.h>
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
    void UpdateAnimation();
public:
    CLONE(CPlayerScript);
private:
    int moveDir;
    int preMoveDir;
    float player_move_speed_;
    float player_bullet_attack_speed_;
    int is_hit_;
    float hit_accumulated_time_;
    float accumulated_time_;
    Ptr<CPrefab> player_bullet_prefab_;
};

