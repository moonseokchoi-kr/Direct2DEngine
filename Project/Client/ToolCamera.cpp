#include "pch.h"
#include "ToolCamera.h"

#include <Engine/CTransform.h>
#include <Engine/CKeyManager.h>
#include <Engine/CTimeManager.h>
ToolCamera::ToolCamera()
	:move_time_(0.15f)
	,accumulated_time_(0.f)
{
}

ToolCamera::~ToolCamera()
{
}

void ToolCamera::FinalUpdate()
{
	Move();

	CalViewMatrix();
	CalProjectionMatrix();
}

void ToolCamera::Move()
{
	Vec3 position = GetTransform()->GetPosition();
	if (KEY_HOLD(KEY::W))
	{
		Vec3 front = GetTransform()->GetWorldDirection(DIRECTION_TYPE::FRONT);
		position += fDT * front * 500.f;
	}
	if (KEY_HOLD(KEY::A))
	{
		Vec3 right = GetTransform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);
		position += fDT * -right * 500.f;
	}
	if (KEY_HOLD(KEY::S))
	{
		Vec3 front = GetTransform()->GetWorldDirection(DIRECTION_TYPE::FRONT);
		position += fDT * -front * 500.f;
	}
	if (KEY_HOLD(KEY::D))
	{
		Vec3 right = GetTransform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);
		position += fDT * right * 500.f;
	}
	if (KEY_HOLD(KEY::RBTN))
	{
		accumulated_time_ += fDT;
		if (move_time_ <= accumulated_time_)
		{

			GetTransform()->SetPosition(position);
			Vec3 rot = GetTransform()->GetRotation();
			Vec2 dir = CKeyManager::GetInst()->GetMouseDir();

			rot.x += dir.y * fDT * 5.f;
			rot.y += dir.x * fDT * 5.f;

			GetTransform()->SetRotation(rot);
		}

	}
}
