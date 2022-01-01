#include "pch.h"
#include "CPlayerScript.h"
#include "CBulletScript.h"
#include <Engine/CAnimation2D.h>
#include <Engine/CRigidBody2D.h>


CPlayerScript::CPlayerScript()
	:CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, player_move_speed_(400)
	, accumulated_time_(0)
	, player_bullet_attack_speed_(0.1f)

{
	player_bullet_prefab_ = CResourceManager::GetInst()->FindRes<CPrefab>(L"player_bullet_prefab");
	SetName(L"PlayerScript");
	ADD_PARAMETER(player_move_speed_);
	ADD_PARAMETER(player_bullet_attack_speed_);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Update()
{
	accumulated_time_ += fDT;
	CRigidBody2D* rigidBody = GetRigidBody2D();
	Vec2 velocity = rigidBody->GetVelocity();
	if (KEY_TAP(KEY::UP))
	{
		rigidBody->SetMoveDir(Vec2(0.f, 1.f));
		rigidBody->SetVelocity(Vec2(velocity.x, player_move_speed_));
	}
	if (KEY_TAP(KEY::DOWN))
	{
		rigidBody->SetMoveDir(Vec2(0.f, -1.f));
		rigidBody->SetVelocity(Vec2(velocity.x, -player_move_speed_));

	}
	if (KEY_TAP(KEY::LEFT))
	{
		rigidBody->SetMoveDir(Vec2(-1.f, 0.f));
		rigidBody->SetVelocity(Vec2(-player_move_speed_, velocity.y));

	}
	if (KEY_TAP(KEY::RIGHT))
	{
		rigidBody->SetMoveDir(Vec2(1.f, 0.f));
		rigidBody->SetVelocity(Vec2(player_move_speed_, velocity.y));
	}
	if (KEY_HOLD(KEY::UP))
	{

		rigidBody->ApplyImpulse(Vec2(0.f, player_move_speed_));
	}
	if (KEY_HOLD(KEY::DOWN))
	{

		rigidBody->ApplyImpulse(Vec2(0.f, -player_move_speed_));

	}
	if (KEY_HOLD(KEY::LEFT))
	{

		rigidBody->ApplyImpulse(Vec2(-player_move_speed_, 0.f));

	}
	if (KEY_HOLD(KEY::RIGHT))
	{

		rigidBody->ApplyImpulse(Vec2(player_move_speed_, 0.f));
	}
	move_dir_ = rigidBody->GetMoveDir();
	if (KEY_NONE(KEY::UP) && KEY_NONE(KEY::DOWN) && KEY_NONE(KEY::RIGHT) && KEY_NONE(KEY::LEFT))
	{
		if (velocity.Length() != 0)
			rigidBody->SetVelocity(move_dir_ * 10.f);
		else
			rigidBody->SetVelocity(Vec2());
	}
	if (KEY_HOLD(KEY::Q))
	{
		rigidBody->SetAngluarVelocity(400.f);
	}
	if (KEY_HOLD(KEY::E))
	{
		rigidBody->SetAngluarVelocity(-400.f);
	}
	if (KEY_NONE(KEY::Q) && KEY_NONE(KEY::E))
	{

		rigidBody->SetAngluarVelocity(0);

	}
	if (KEY_HOLD(KEY::SPACE))
	{
		if (player_bullet_attack_speed_ <= accumulated_time_)
		{
			CreateBullet();
			accumulated_time_ = 0.f;
		}

	}
	UpdateAnimation();
	prev_move_dir_ = move_dir_;
}

void CPlayerScript::OnCollisionEnter(CGameObject* otherObject)
{
	if (L"monster_bullet" == otherObject->GetName())
	{
		//OutputDebugString(L"총돌했습니다!\n");
		is_hit_ = 1;
		GetMeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &is_hit_);
	}
}

void CPlayerScript::OnCollisionExit(CGameObject* otherObject)
{
}

void CPlayerScript::CreateBullet()
{
	Vec3 position = GetTransform()->GetPosition();
	Vec3 view_scale = GetTransform()->GetScale();

	position.y += view_scale.y / 2.f;

	Instantiate(player_bullet_prefab_, position, 2);
}

void CPlayerScript::UpdateAnimation()
{
	if (move_dir_ == prev_move_dir_)
	{

		if (move_dir_ < Vec2(0, 0))
		{
			if (GetAnimator2D()->GetCurrentAnimation()->GetCurrentFrameIndex() < 4)
			{
				return;
			}
			else
			{
				GetAnimator2D()->Play(L"FLY_LEFT", 4, true);
			}
		}

	}
	else
	{
		if (move_dir_ == Vec2(0, 0))
			GetAnimator2D()->Play(L"FLY", 0, true);
		if (move_dir_ < Vec2(0, 0))
			GetAnimator2D()->Play(L"FLY_LEFT", 0, true);
	}
}

void CPlayerScript::SaveToScene(FILE* file)
{
	__super::SaveToScene(file);
	SaveResReference(player_bullet_prefab_, file);
}

void CPlayerScript::LoadFromScene(FILE* file)
{
	__super::LoadFromScene(file);
	LoadResReference(player_bullet_prefab_, file);
}
