#pragma once
#include "CScript.h"


enum class MONSTER_STATE
{
	IDLE,
	MOVE,
	ATTACK,
};

class CBulletScript;
class CMonsterScript :
    public CScript
{

public:
    CMonsterScript();
	CMonsterScript(float attack_time);
    ~CMonsterScript();
public:
    void Update() override;
	void OnCollisionEnter(CCollider2D* otherCollider);
public:
    CLONE(CMonsterScript);
private:
    void CreateBullet(int index);
    void Fire();
    CBulletScript* CreateBulletMoveDirection(int index);
    void SetRandomPattern();
	void UpdateState();
	void UpdateAnimation();
public:
	static float GetHp() { return hp; };
private:
	static float hp;

	
	bool is_right_;
	bool is_move_;
	float bullet_start_angle_;
	array<float, 6> bullet_rotate_angle_;
	array<float, 6> bullet_speed_rate_;
	array<float, 6> bullet_angle_;
	array<float, 6> bullet_angle_rate_;
	
	float attack_time_;
	float interval_;

	float monster_state_time_;
	float accumulated_time_;

	float spiral_time_;
	float spiral_accumulated_time_;

	float shoot_count_;
	float current_shoot_count;

	bool init_;
	Vec3 move_dirction;
	Vec2 destination_position_;
	MONSTER_STATE monster_state_;
	MONSTER_STATE monster_prev_state_;
	ENEMY_BULLET_TYPE bullet_type_;
};

