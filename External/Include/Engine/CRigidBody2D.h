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
    void SetBodyType(BODY_TYPE type) { type_ = type;  }
private:
    void InitRigidBody();
    b2BodyType RigidBodyTypeToBox2BodyType(BODY_TYPE type);
private:
    b2World* test_world_;
    BODY_TYPE type_ ;
    bool fixed_rotation_; 
    void* runtime_body_;   
};

