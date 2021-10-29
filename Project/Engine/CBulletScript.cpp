#include "pch.h"
#include "CBulletScript.h"
#include "CDevice.h"
CBulletScript::CBulletScript()
	:move_direction_(Vec3(0,1.f,0.f))
	,move_speed_(500.f)
	,move_speed_rate_(0)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::Update()
{
	MoveBulluet();
	move_speed_ += move_speed_rate_;
	Vec3 position = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();
	if (abs(position.x) >= CDevice::GetInst()->GetResolution().x + (scale.x / 2.f) || abs(position.y) >= CDevice::GetInst()->GetResolution().y + (scale.y / 2.f))
	{
		DeleteObject(GetOwner());
	}
}

void CBulletScript::OnCollisionEnter(CCollider2D* otherCollider)
{
	CGameObject* object = otherCollider->GetOwner();
  	if (L"player" == object->GetName() || L"monster" == object->GetName())
	{
		DeleteObject(GetOwner());
	}
}
	


void CBulletScript::MoveBulluet()
{
	Vec3 position = GetTransform()->GetPosition();
	Vec3 rotation = GetTransform()->GetRotation();
	position += move_direction_ * move_speed_*fDT;
	rotation.z += rotation_angle;
	GetTransform()->SetPosition(position);
	GetTransform()->SetRotation(rotation);
}
