#include "pch.h"
#include "CBox2DCollider.h"

#include "CTransform.h"
#include "CRigidBody2D.h"
#include "CSceneManager.h"
#include "CScene.h"

CBox2DCollider::CBox2DCollider()
	:CComponent(COMPONENT_TYPE::BOX2D_COLLIDER)
	,runtime_fixture_(nullptr)
	,denisty_(1.0f)
	,friction_(0.5f)
	,restitution_(0.0f)
	,restitution_threshold_(0.5f)
	,offset_(Vec2(0.f,0.f))
	,offset_size_(Vec2(1.f,1.f))
{
}

CBox2DCollider::CBox2DCollider(const CBox2DCollider& origin)
	: CComponent(COMPONENT_TYPE::BOX2D_COLLIDER)
	,runtime_fixture_(nullptr)
	, offset_(origin.offset_)
	, offset_size_(offset_size_)
	, denisty_(origin.denisty_)
	, friction_(origin.friction_)
	, restitution_(origin.restitution_)
	, restitution_threshold_(origin.restitution_threshold_)
{
}

CBox2DCollider::~CBox2DCollider()
{
}

void CBox2DCollider::Start()
{
	InitCollider();
}

void CBox2DCollider::FinalUpdate()
{	
	if (nullptr == GetRigidBody2D() || nullptr == GetRigidBody2D()->GetRuntimeBody())
		return;
	GetRigidBody2D()->GetRuntimeBody()->DestroyFixture((b2Fixture*)runtime_fixture_);
	InitCollider();
}

void CBox2DCollider::LateUpdate()
{
}

void CBox2DCollider::Render()
{
}

void CBox2DCollider::InitCollider()
{
	if (nullptr == GetRigidBody2D() || nullptr == GetRigidBody2D()->GetRuntimeBody())
		return;
	Vec3 pos = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(scale.x * offset_size_.x, scale.y * offset_size_.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = denisty_;
	fixtureDef.friction = friction_;
	fixtureDef.restitution = restitution_;
	fixtureDef.restitutionThreshold = restitution_threshold_;
	
	runtime_fixture_ = GetRigidBody2D()->GetRuntimeBody()->CreateFixture(&fixtureDef);

}
