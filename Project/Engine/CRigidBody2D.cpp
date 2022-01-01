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
	,max_velocity_(300.f,300.f)
{
}

CRigidBody2D::CRigidBody2D(const CRigidBody2D& origin)
	:CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	, type_(origin.type_)
	, fixed_rotation_(origin.fixed_rotation_)
	, runtime_body_(nullptr)
	, max_velocity_(300.f, 300.f)
{
	
}


CRigidBody2D::~CRigidBody2D()
{
	DestroyBody();
}

void CRigidBody2D::Start()
{
	b2Body* body = (b2Body*)runtime_body_;
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
Vec2 CRigidBody2D::GetVelocity()
{
	b2Body* body = CheckBody();

	return Vec2(body->GetLinearVelocity().x,body->GetLinearVelocity().y);
}
void CRigidBody2D::DestroyBody()
{
	CPhysicsManager::GetInst()->GetPhysicsWorld()->DestroyBody((b2Body*)runtime_body_);
	runtime_body_ = nullptr;
}
void CRigidBody2D::SetVelocity(Vec2 velocity)
{
	b2Body* body = CheckBody();
	if (velocity > max_velocity_)
		velocity = max_velocity_;
	body->SetLinearVelocity({ velocity.x,velocity.y });
}
void CRigidBody2D::SetAngluarVelocity(float velocity)
{
	b2Body* body = CheckBody();
	body->SetAngularVelocity(velocity);
}
void CRigidBody2D::ApplyImpulse(Vec2 impulse, bool wake)
{
	b2Body* body = CheckBody();

	body->ApplyLinearImpulseToCenter({ impulse.x,impulse.y },wake);
	Vec2 velocity = Vec2(body->GetLinearVelocity().x,body->GetLinearVelocity().y);
	SetVelocity(velocity);
}
void CRigidBody2D::ApplyAngularImpulse(float impulse, bool wake)
{
	b2Body* body = CheckBody();
	body->ApplyAngularImpulse(impulse, wake);
}
void CRigidBody2D::AddForce(Vec2 force, bool wake)
{
	b2Body* body = CheckBody();
	body->ApplyForceToCenter({ force.x,force.y }, wake);
	Vec2 velocity = Vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y);
	SetVelocity(velocity);
}
Vec2 CRigidBody2D::GetMoveDir()
{
	b2Body* body = CheckBody();
	float angle = body->GetAngle();
	return Vec2(sinf(angle), cosf(angle));
}
void CRigidBody2D::SetMoveDir(Vec2 dir)
{
	b2Body* body = CheckBody();
	//90도가 0이기때문에
	float angle = atan2f(dir.y, dir.x) * 180 / PI -90.f; 

	b2Vec2 pos = body->GetPosition();
	
	body->SetTransform(pos, angle);
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

void CRigidBody2D::SaveToScene(FILE* file)
{
	__super::SaveToScene(file);

	UINT type = (UINT)type_;

	fwrite(&type, sizeof(UINT), 1, file);
	fwrite(&fixed_rotation_, sizeof(bool), 1, file);
	fwrite(&max_velocity_, sizeof(Vec2), 1, file);
}

void CRigidBody2D::LoadFromScene(FILE* file)
{
	__super::LoadFromScene(file);

	UINT type =0;

	fread(&type, sizeof(UINT), 1, file);
	type_ = (BODY_TYPE)type;
	fread(&fixed_rotation_, sizeof(bool), 1, file);
	fread(&max_velocity_, sizeof(Vec2), 1, file);
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

b2Body* CRigidBody2D::CheckBody()
{
	if (nullptr == runtime_body_)
		InitRigidBody();
	return (b2Body*)runtime_body_;
}
