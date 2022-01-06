#pragma once
#include <Engine/CScript.h>

enum class PLAYER_STATE
{
    INTRO,
    IDLE,
    MOVE,
    ATTACK,
    DAMEGED,
    DEATH,
    END,
};

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
    void MovePlayer();
public:
    void SaveToScene(FILE* file) override;
    void LoadFromScene(FILE* file) override;
public:
    CLONE(CPlayerScript);
private:
    Vec2 face_dir_;
    Vec2 move_dir_;
    Vec2 prev_move_dir_;
    float player_walk_speed_;
    float player_run_speed_;
    float player_attack_speed_;
    bool is_run_;
    PLAYER_STATE current_state_;
    PLAYER_STATE previouse_state_;

    int is_hit_;
    float hit_accumulated_time_;
    float accumulated_time_;
    Ptr<CPrefab> player_bullet_prefab_;
};

