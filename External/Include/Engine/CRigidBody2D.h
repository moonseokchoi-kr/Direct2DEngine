#pragma once
#include "CComponent.h"



class CRigidBody2D :
    public CComponent
{
public:
    CRigidBody2D();
    ~CRigidBody2D();

public:
    void FinalUpdate() override;


public:
    void ApplyImpulse(const Vec2& impulse);
    void ApplyImpulse(const Vec2& impulse, const Vec2& contactVector);
    void SetStatic();
    bool IsStatic();
	void SetVelocity(Vec2 vlc)
	{
		if (vlc.Length() > max_velocity_)
		{
			velocity_ = Vec2(max_velocity_ / sqrtf(2), max_velocity_ / sqrtf(2));

		}
		velocity_ = vlc;
	}
	void SetFriction(float flc)
	{
		static_friction_ = flc;
	}
	void SetMaxVelocity(float maxVelocity)
	{
		max_velocity_ = maxVelocity;
	}
    float GetRestitution() { return restitution_; }
    float GetMass() { return mass_; }
    void SetMass(float mass) { mass_ = mass; }
	void SetAdditionalAccel(const Vec2& ac) { additional_accel_ = ac; }
    void ResolveCollide(CGameObject* other);

public:
    Vec2 GetForce() { return force_; }
    Vec2 GetVelocity() { return velocity_; }
    Vec2 GetAccel() { return accel_; }
public:
	CLONE(CRigidBody2D);
public:
    void SaveToScene(FILE* file);
    void LoadFromScene(FILE* file);
private:
    void Move();
    void CalCulateMass();
private:
    Vec2 force_;
    Vec2 accel_;
    Vec2 additional_accel_;
    Vec2 velocity_;
    Vec2 angular_velocity_;
    
    float torque_; //토크 회전힘


    float mass_;
    float inverse_mass_;

    float intertia_;
    float inverse_intertia_;

    float max_velocity_;

    float static_friction_;

    float density_;//밀도
    float restitution_;//

    bool is_active_;

    float linear_damping_;
    float angular_damping_;
};

