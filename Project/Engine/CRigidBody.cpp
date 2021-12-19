#include "pch.h"
#include "CRigidBody.h"

CRigidBody::CRigidBody()
	:CComponent(COMPONENT_TYPE::RIGIDBODY2D)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::FinalUpdate()
{
}

void CRigidBody::AddImpluse(const Vec3& force)
{
}