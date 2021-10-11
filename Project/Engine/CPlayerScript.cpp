#include "pch.h"
#include "CPlayerScript.h"



CPlayerScript::CPlayerScript()
	:player_move_speed_(400)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Update()
{
	Vec3 pos = GetTransform()->GetPos();
	Vec3 scale = GetTransform()->GetScale();

	if (KEY_HOLD(KEY::UP))
	{
		pos += Vec3(0.f, fDT * player_move_speed_ * 0.5f, 0.f);
	}
	if (KEY_HOLD(KEY::DOWN))
	{
		pos -= Vec3(0.f, fDT * player_move_speed_ * 0.5f, 0.f);
	}
	if (KEY_HOLD(KEY::LEFT))
	{
		pos -= Vec3(fDT * player_move_speed_ * 0.5f, 0.f, 0.f);
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		pos += Vec3(fDT * player_move_speed_
			* 0.5f, 0.f, 0.f);
	}

	if (KEY_HOLD(KEY::LBTN))
	{
		scale -= Vec3(1.f, 1.f, 1.f) * 300*fDT;
	}
	if (KEY_HOLD(KEY::RBTN))
	{
		scale += Vec3(1.f, 1.f, 1.f) * 300*fDT;
	}

	GetTransform()->SetPos(pos);
	GetTransform()->SetScale(scale);
}
