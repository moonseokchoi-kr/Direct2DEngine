#pragma once
#include "CComponent.h"

enum class BODY_TYPE { STATIC = 0, DYNAMIC, KINEMATIC };

class b2World;
class CRigidBody2D :
    public CComponent
{
public:
    CRigidBody2D();
    CRigidBody2D(const CRigidBody2D& origin);
    ~CRigidBody2D();

public:
    void Start() override;
    void LateUpdate() override;
    void FinalUpdate() override;
    CLONE(CRigidBody2D);
public:
    b2Body* GetRuntimeBody() { return static_cast<b2Body*>(runtime_body_); }
    bool IsFixedRotation() { return fixed_rotation_; }
    void SetFixedRotation(bool b) { fixed_rotation_ = b; }

    BODY_TYPE GetBodyType() { return type_; }
    Vec2 GetMaxVelocity() { return max_velocity_; }
    Vec2 GetVelocity();
    void SetBodyType(BODY_TYPE type) { type_ = type;  }
    void SetMaxVelocity(Vec2 velocity) { max_velocity_ = velocity; }

    void DestroyBody();
    void SetVelocity(Vec2 velocity);
    void SetAngluarVelocity(float velocity);
    void ApplyImpulse(Vec2 impulse, bool wake=true);
    void ApplyAngularImpulse(float impulse, bool wake=true);
    void AddForce(Vec2 force, bool wake=true);

    Vec2 GetMoveDir();
    void SetMoveDir(Vec2 dir);

    void InitRigidBody();
private:
    b2BodyType RigidBodyTypeToBox2BodyType(BODY_TYPE type);
    b2Body* CheckBody();
private:
    b2World* test_world_;

    void* runtime_body_; 

    BODY_TYPE type_;
    bool fixed_rotation_;
    Vec2 max_velocity_;
};

