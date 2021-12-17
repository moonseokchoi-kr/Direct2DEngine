#pragma once
#include "CComponent.h"
class CRigidBody :
    public CComponent
{
public:
    CRigidBody();
    ~CRigidBody();

public:
    void FinalUpdate() override;

public:
    void AddImpluse(const Vec3& force);

};

