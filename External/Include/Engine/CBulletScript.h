#pragma once
#include "CScript.h"
class CBulletScript :
    public CScript
{
public:
    CBulletScript();
    ~CBulletScript();

public:
	void Update() override;
public:
    void OnCollisionEnter(CCollider2D* otherCollider);
public:
    void SetMoveDirection(Vec3 direction) { move_direction_ = direction; }
    void SetMoveSpeed(float speed) { move_speed_ = speed; }
    void SetMoveSpeedRate(float speed) { move_speed_rate_ = speed; }
    void SetRotationAngle(float degree) { rotation_angle = degree; }

private:
    void MoveBulluet();
public:
    CLONE(CBulletScript);
protected:
	Vec3 move_direction_;
    float rotation_angle;
	float move_speed_;
    float move_speed_rate_;
};

