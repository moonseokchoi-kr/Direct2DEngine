#include "pch.h"
#include "CBox2DCollider.h"

#include "CTransform.h"
#include "CRigidBody2D.h"
#include "CSceneManager.h"
#include "CCollisionManager.h"
#include "CScript.h"
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
}

void CBox2DCollider::LateUpdate()
{
}


void CBox2DCollider::FinalUpdate()
{	
	if (nullptr == GetRigidBody2D() || nullptr == GetRigidBody2D()->GetRuntimeBody())
		return;
	if (nullptr == runtime_fixture_)
	{
		InitCollider();
		return;
	}
	SetFixture();
}

void CBox2DCollider::UpdateData()
{

}


void CBox2DCollider::Render()
{
}

void CBox2DCollider::OnCollisionEnter(CGameObject* otherObject)
{
	++collision_count_;
	const vector<CScript*>& scripts = GetOwner()->GetScripts();
	for (const auto& script : scripts)
	{
		script->OnCollisionEnter(otherObject);
	}
}

void CBox2DCollider::OnCollision(CGameObject* otherObject)
{
	const vector<CScript*>& scripts = GetOwner()->GetScripts();
	for (const auto& script : scripts)
	{
		script->OnCollision(otherObject);
	}
}

void CBox2DCollider::OnCollisionExit(CGameObject* otherObject)
{
	--collision_count_;
	const vector<CScript*>& scripts = GetOwner()->GetScripts();
	for (const auto& script : scripts)
	{
		script->OnCollisionExit(otherObject);
	}
}

void CBox2DCollider::SaveToScene(FILE* file)
{
	__super::SaveToScene(file);

	fwrite(&offset_, sizeof(Vec2), 1, file);
	fwrite(&offset_size_, sizeof(Vec2), 1, file);
	fwrite(&denisty_, sizeof(float), 1, file);
	fwrite(&friction_, sizeof(float), 1, file);
	fwrite(&restitution_, sizeof(float), 1, file);
	fwrite(&restitution_threshold_, sizeof(float), 1, file);
	fwrite(&is_trigger_, sizeof(bool), 1, file);
}

void CBox2DCollider::LoadFromScene(FILE* file)
{
	__super::LoadFromScene(file);

	fread(&offset_, sizeof(Vec2), 1, file);
	fread(&offset_size_, sizeof(Vec2), 1, file);
	fread(&denisty_, sizeof(float), 1, file);
	fread(&friction_, sizeof(float), 1, file);
	fread(&restitution_, sizeof(float), 1, file);
	fread(&restitution_threshold_, sizeof(float), 1, file);
	fread(&is_trigger_, sizeof(bool), 1, file);
}

void CBox2DCollider::InitCollider()
{
	if (nullptr == GetRigidBody2D() || nullptr == GetRigidBody2D()->GetRuntimeBody())
		return;
	Vec3 scale = GetTransform()->GetScale();
	Vec2 rectScale = Vec2(scale.x * 0.5f, scale.y * 0.5f);
	UINT categoryBit = (1 << GetOwner()->GetLayerIndex());
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(rectScale.x * offset_size_.x, rectScale.y * offset_size_.y);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = denisty_;
	fixtureDef.friction = friction_;
	fixtureDef.restitution = restitution_;
	fixtureDef.restitutionThreshold = restitution_threshold_;
	fixtureDef.filter.categoryBits = categoryBit;
	fixtureDef.filter.maskBits = CCollisionManager::GetInst()->GetCollisionMask(GetOwner()->GetLayerIndex());
	fixtureDef.isSensor = is_trigger_;

	runtime_fixture_ = GetRigidBody2D()->GetRuntimeBody()->CreateFixture(&fixtureDef);
}

void CBox2DCollider::SetFixture()
{
	Vec3 scale = GetTransform()->GetScale();
	b2Fixture* fixture = (b2Fixture*)runtime_fixture_;
	b2Filter filter;
	filter.categoryBits = (1 << GetOwner()->GetLayerIndex());
	filter.maskBits = CCollisionManager::GetInst()->GetCollisionMask(GetOwner()->GetLayerIndex());
	fixture->SetDensity(denisty_);
	fixture->SetFriction(friction_);
	fixture->SetRestitution(restitution_);
	fixture->SetRestitutionThreshold(restitution_threshold_);
	fixture->SetFilterData(filter);

	b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
	Vec2 rectScale = Vec2(scale.x * 0.5f, scale.y * 0.5f);
	shape->SetAsBox(rectScale.x * offset_size_.x, rectScale.y * offset_size_.y);
}
