#include "pch.h"
#include "CRigidBody2D.h"

#include "CTimeManager.h"

#include "CGameObject.h"
#include "CTransform.h"

#include "CPhysicsManager.h"


CRigidBody2D::CRigidBody2D()
	:CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	,type_(BODY_TYPE::DYNAMIC)
	,fixed_rotation_(false)
	,runtime_body_(nullptr)
{
}

CRigidBody2D::CRigidBody2D(const CRigidBody2D& origin)
	:CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	, type_(origin.type_)
	, fixed_rotation_(origin.fixed_rotation_)
	, runtime_body_(nullptr)
{
	
}


CRigidBody2D::~CRigidBody2D()
{
	CPhysicsManager::GetInst()->GetPhysicsWorld()->DestroyBody((b2Body*)runtime_body_);
	SafeDelete(runtime_body_);
}

void CRigidBody2D::Start()
{
	
}

void CRigidBody2D::LateUpdate()
{

}
void CRigidBody2D::FinalUpdate()
{
	if (nullptr == runtime_body_)
	{
		InitRigidBody();
		return;

	}

	Vec3 pos = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();
	Vec3 rotation = GetTransform()->GetRotation();

	b2Body* body = (b2Body*)runtime_body_;

	const auto& position = body->GetPosition();

	pos = Vec3(position.x, position.y, pos.z);
	rotation.z = body->GetAngle();

	GetTransform()->SetPosition(pos);
	GetTransform()->SetRotation(rotation);
}
void CRigidBody2D::InitRigidBody()
{
	Vec3 pos = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();
	Vec3 rotation = GetTransform()->GetRotation();

	b2BodyDef bodyDef;
	bodyDef.type = RigidBodyTypeToBox2BodyType(type_);
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.angle = rotation.z;
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(GetOwner());

	b2Body* body = CPhysicsManager::GetInst()->GetPhysicsWorld()->CreateBody(&bodyDef);
	body->SetFixedRotation(fixed_rotation_);
	runtime_body_ = body;
}

b2BodyType CRigidBody2D::RigidBodyTypeToBox2BodyType(BODY_TYPE type)
{
	switch (type)
	{
	case BODY_TYPE::STATIC:
		return b2BodyType::b2_staticBody;
	case BODY_TYPE::DYNAMIC:
		return b2BodyType::b2_dynamicBody;
	case BODY_TYPE::KINEMATIC:
		return b2BodyType::b2_kinematicBody;
	default:
		break;
	}
	return b2BodyType::b2_staticBody;
}
