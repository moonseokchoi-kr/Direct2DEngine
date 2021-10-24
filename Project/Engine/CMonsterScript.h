#pragma once
#include "CScript.h"

class CBulletScript;
class CMonsterScript :
    public CScript
{

public:
    CMonsterScript();
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
private:

	float hp;

	float bullet_start_angle_;
	array<float, 6> bullet_rotate_angle_;
	array<float, 6> bullet_speed_rate_;
	array<float, 6> bullet_angle_;
	array<float, 6> bullet_angle_rate_;
	float attack_time_;
	float attack_during_time_;
	float interval_;
	float accumulated_time_;

	float shoot_count_;
	float current_shoot_count;

	bool init_;

	ENEMY_BULLET_TYPE bullet_type_;
};

