#include "pch.h"
#include "CMoveScript.h"

CMoveScript::CMoveScript()
	:move_speed_(200.f)
{
}

CMoveScript::~CMoveScript()
{
}

void CMoveScript::Update()
{
	Vec3 pos = GetTransform()->GetPosition();

	if (KEY_HOLD(KEY::UP))
	{
		pos += Vec3(0.f, fDT * move_speed_ * 0.5f, 0.f);
	}
	if (KEY_HOLD(KEY::DOWN))
	{
		pos -= Vec3(0.f, fDT * move_speed_ * 0.5f, 0.f);
		
	}
	if (KEY_HOLD(KEY::LEFT))
	{
		pos -= Vec3(fDT * move_speed_ * 0.5f, 0.f, 0.f);
		
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		pos += Vec3(fDT * move_speed_
			* 0.5f, 0.f, 0.f);
	}
	GetTransform()->SetPosition(pos);
}
