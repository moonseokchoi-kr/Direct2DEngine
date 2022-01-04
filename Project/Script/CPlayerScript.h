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
    void SaveToScene(FILE* file) override;
    void LoadFromScene(FILE* file) override;
public:
    CLONE(CPlayerScript);
private:
    Vec2 face_dir_;
    Vec2 prev_move_dir_;
    float player_move_speed_;
    float player_attack_speed_;
    int is_hit_;
    float hit_accumulated_time_;
    float accumulated_time_;
    Ptr<CPrefab> player_bullet_prefab_;
};

