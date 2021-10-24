#pragma once
#include "CBulletScript.h"
class CMonsterShooterScript :
    public CBulletScript
{
public:
    CMonsterShooterScript(ENEMY_BULLET_TYPE bulletType,float shotAngle, float shotAngleRate, float shotSpeed, int shotCount, float bulletSpeedRate=0, float bulletAngleRate=0,float shotAngleOffset=0);
    ~CMonsterShooterScript();

public:
    void Update() override;

private:
    //
	//	DIRECTIONAL,
	//  BI_DIRECTIONAL,
	//	SPIRAL,
	//	MULTIPLE_SPIRAL,
	//	INTERVAL_MUTIPLE_SPIRAL,
	//	BENT_SPIRAL,
	//	COMINE_SPIRAL,
	//	WASHER_SPIRAL,
	//	N_WAY,
    //
    void DirectionalShoot();
    void Spiral();
    void MultipleSpiral();
    void BentSpiral();
    void WasherSpiral();

private:
    float shot_angle_;
    float shot_angle_offset_;
    float shot_angle_rate_;
    float shot_speed_;
    int shot_count_;

    float bullet_speed_rate_;
    float bullet_angle_rate_;

	float acculamted_time_;
	float interval_;

    ENEMY_BULLET_TYPE bullet_type_;
};

