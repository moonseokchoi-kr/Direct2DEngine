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
	float cameraMoveSpeed = 1000.f;
	Vec3 position = GetTransform()->GetPosition();
	Vec3 delta;
	if (KEY_HOLD(KEY::Q))
	{
		Vec3 front = GetTransform()->GetWorldDirection(DIRECTION_TYPE::FRONT);
		position += fDT * front * cameraMoveSpeed;
	}
	if (KEY_HOLD(KEY::A))
	{
		Vec3 right = GetTransform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);
		delta = fDT * -right * cameraMoveSpeed;
		position += delta;
	}
	if (KEY_HOLD(KEY::E))
	{
		Vec3 front = GetTransform()->GetWorldDirection(DIRECTION_TYPE::FRONT);
		position += fDT * -front * cameraMoveSpeed;
	}
	if (KEY_HOLD(KEY::D))
	{
		Vec3 right = GetTransform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);
		delta = fDT * right * cameraMoveSpeed;
		position += delta;
	}
	if (KEY_HOLD(KEY::W))
	{
		Vec3 up = GetTransform()->GetWorldDirection(DIRECTION_TYPE::UP);
		delta = fDT * up * cameraMoveSpeed;
		position += delta;
	}
	if (KEY_HOLD(KEY::S))
	{
		Vec3 up = GetTransform()->GetWorldDirection(DIRECTION_TYPE::UP);
		delta = fDT * -up * cameraMoveSpeed;
		position += delta;
	}
	if (KEY_HOLD(KEY::RBTN) && GetProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
	{
		accumulated_time_ += fDT;
		if (move_time_ <= accumulated_time_)
		{

			GetTransform()->SetPosition(position);
			Vec3 rot = GetTransform()->GetRotation();
			Vec2 dir = CKeyManager::GetInst()->GetMouseDir();
			camera_move_delta_ += delta;
			rot.x += dir.y * fDT * 50.f;
			rot.y += dir.x * fDT * 50.f;

			GetTransform()->SetRotation(rot);
		}

	}
}
