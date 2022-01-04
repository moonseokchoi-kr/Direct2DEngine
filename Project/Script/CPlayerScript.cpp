#include "pch.h"
#include "CPlayerScript.h"
#include "CBulletScript.h"
#include <Engine/CAnimation2D.h>
#include <Engine/CRigidBody2D.h>


CPlayerScript::CPlayerScript()
	:CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, player_move_speed_(100.f)
	, accumulated_time_(0)
	, player_attack_speed_(0.1f)

{
	player_bullet_prefab_ = CResourceManager::GetInst()->FindRes<CPrefab>(L"player_bullet_prefab");
	SetName(L"PlayerScript");
	ADD_PARAMETER(player_move_speed_);
	ADD_PARAMETER(player_attack_speed_);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Update()
{
	accumulated_time_ += fDT;
	CRigidBody2D* rigidBody = GetRigidBody2D();
	Vec2 velocity = rigidBody->GetVelocity();
	if (KEY_TAP(KEY::W))
	{
		face_dir_.y = 1;
		rigidBody->SetVelocity(Vec2(velocity.x, player_move_speed_));
	}
	if (KEY_TAP(KEY::S))
	{
		face_dir_.y = -1;
		rigidBody->SetVelocity(Vec2(velocity.x, -player_move_speed_));

	}
	if (KEY_TAP(KEY::A))
	{
		face_dir_.x = -1;
		rigidBody->SetVelocity(Vec2(-player_move_speed_, velocity.y));
	}
	if (KEY_TAP(KEY::D))
	{
		face_dir_.x = 1;
		rigidBody->SetVelocity(Vec2(player_move_speed_, velocity.y));
	}
	if (KEY_HOLD(KEY::W))
	{

		rigidBody->ApplyImpulse(Vec2(0.f, player_move_speed_));
	}
	if (KEY_HOLD(KEY::S))
	{

		rigidBody->ApplyImpulse(Vec2(0.f, -player_move_speed_));

	}
	if (KEY_HOLD(KEY::A))
	{

		rigidBody->ApplyImpulse(Vec2(-player_move_speed_, 0.f));

	}
	if (KEY_HOLD(KEY::D))
	{

		rigidBody->ApplyImpulse(Vec2(player_move_speed_, 0.f));
	}
	
	if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S))
	{
		face_dir_.y = 0;
	}
	if (KEY_NONE(KEY::A) && KEY_NONE(KEY::D))
	{
		face_dir_.x = 0;
	}
	if (KEY_HOLD(KEY::W))
	{

		rigidBody->ApplyImpulse(Vec2(0.f, player_move_speed_));
	}

	if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
	{
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
		if (player_attack_speed_ <= accumulated_time_)
		{
			CreateBullet();
			accumulated_time_ = 0.f;
		}

	}
	UpdateAnimation();
	prev_move_dir_ = face_dir_;
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
	CRigidBody2D* rigidBody = GetRigidBody2D();
	Vec2 velocity = rigidBody->GetVelocity();
	if (velocity.Length() != 0 && prev_move_dir_ != face_dir_)
	{
		if (face_dir_.y>0)
		{
			if (face_dir_.x>0)
			{
				GetAnimator2D()->Play(L"player_walk_up_diag", 0, true);
			}
			else if(face_dir_.x<0)
			{
				GetAnimator2D()->Play(L"player_walk_up_diag_left", 0, true);
			}
			else
			{
				GetAnimator2D()->Play(L"player_walk_up", 0, true);
			}
		}
		else if (face_dir_.y < 0)
		{
			if (face_dir_.x>0)
			{
				GetAnimator2D()->Play(L"player_walk_down_diag", 0, true);
			}
			else if (face_dir_.x < 0)
			{
				GetAnimator2D()->Play(L"player_walk_down_diag_left", 0, true);
			}
			else
			{
				GetAnimator2D()->Play(L"player_walk_down", 0, true);
			}
		}
		else
		{
			if (face_dir_.x>0)
			{
				GetAnimator2D()->Play(L"player_walk_down_diag", 0, true);
			}
			else if (face_dir_.x< 0)
			{
				GetAnimator2D()->Play(L"player_walk_down_diag_left", 0, true);
			}
			else
			{
				//GetAnimator2D()->Play(L"player_walk_up", 0, false);
			}
		}

	}
	if (face_dir_.y == 0 && face_dir_.x != 0)
	{
		if(GetAnimator2D()->GetCurrentAnimation()->GetCurrentFrameIndex() == 5)
			GetAnimator2D()->GetCurrentAnimation()->SetCurrentFrame(1);
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
