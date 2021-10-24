#pragma once
#include "CScript.h"




class CBulletScript;
class CMonsterScript :
    public CScript
{

public:
    CMonsterScript();
	CMonsterScript(float attack_time, float interval);
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
public:
	static float GetHp() { return hp; };
private:
	static float hp;
	float monster_move_time;
	bool is_right_;
	bool is_move_;
	float bullet_start_angle_;
	array<float, 6> bullet_rotate_angle_;
	array<float, 6> bullet_speed_rate_;
	array<float, 6> bullet_angle_;
	array<float, 6> bullet_angle_rate_;
	float attack_time_;
	float attack_during_time_;
	float interval_;
	float accumulated_time_;
	float move_accumulated_time_;

	float shoot_count_;
	float current_shoot_count;

	bool init_;

	ENEMY_BULLET_TYPE bullet_type_;
};

