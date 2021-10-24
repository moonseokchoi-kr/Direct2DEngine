#include "pch.h"
#include "CMonsterBulletScript.h"
#include "CDevice.h"

CMonsterBulletScript::CMonsterBulletScript()
{
}

CMonsterBulletScript::~CMonsterBulletScript()
{
}

void CMonsterBulletScript::Update()
{
}

void CMonsterBulletScript::MoveBulluet()
{
	Vec3 position = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();
	Vec2 resolution = CDevice::GetInst()->GetResolution();
	float radius = angle * XM_2PI;
	position.x += bullet_speed * cosf(radius) * fDT;
	position.y += bullet_speed * sinf(radius) * fDT;
	angle += angle_rate;
	bullet_speed += bullet_speed_rate;

	if (abs(position.x) >= resolution.x + scale.x || abs(position.y) >= resolution.y + scale.y)
	{
		DeleteObject(GetOwner());
	}

}
