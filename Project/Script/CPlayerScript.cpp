#include "pch.h"
#include "CPlayerScript.h"
#include "CBulletScript.h"
#include <Engine/CAnimation2D.h>
#include <Engine/CRigidBody2D.h>


CPlayerScript::CPlayerScript()
	:CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, player_walk_speed_(10.f)
	, player_run_speed_(30.f)
	, accumulated_time_(0)
	, player_attack_speed_(0.1f)
	, current_state_(PLAYER_STATE::IDLE)

{
	player_bullet_prefab_ = CResourceManager::GetInst()->FindRes<CPrefab>(L"player_bullet_prefab");
	SetName(L"PlayerScript");
	ADD_PARAMETER(player_walk_speed_);
	ADD_PARAMETER(player_attack_speed_);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Update()
{
	CRigidBody2D* rigidBody = GetRigidBody2D();
	MovePlayer();
	if (KEY_HOLD(KEY::SPACE))
	{

	}
	UpdateAnimation();
	prev_move_dir_ = move_dir_;
	previouse_state_ = current_state_;
}

void CPlayerScript::OnCollisionEnter(CGameObject* otherObject)
{
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
	
	if (velocity.Length() != 0 && prev_move_dir_ != move_dir_)
	{
		if (PLAYER_STATE::MOVE == current_state_)
		{
			if (!is_run_)
			{
				if (move_dir_.y > 0)
				{
					if (move_dir_.x > 0)
					{
						GetAnimator2D()->Play(L"player_walk_up_diag", 0, true);
					}
					else if (move_dir_.x < 0)
					{
						GetAnimator2D()->Play(L"player_walk_up_diag_left", 0, true);
					}
					else
					{
						GetAnimator2D()->Play(L"player_walk_up", 0, true);
					}
				}
				else if (move_dir_.y < 0)
				{
					if (move_dir_.x > 0)
					{
						GetAnimator2D()->Play(L"player_walk_down_diag", 0, true);
					}
					else if (move_dir_.x < 0)
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
					if (move_dir_.x > 0)
					{
						GetAnimator2D()->Play(L"player_walk_down_diag", 0, true);
					}
					else if (move_dir_.x < 0)
					{
						GetAnimator2D()->Play(L"player_walk_down_diag_left", 0, true);
					}
				}
			}
			else
			{
				if (move_dir_.y > 0)
				{
					if (move_dir_.x > 0)
					{
						GetAnimator2D()->Play(L"player_run_up_diag", 0, true);
					}
					else if (move_dir_.x < 0)
					{
						GetAnimator2D()->Play(L"player_run_up_diag_left", 0, true);
					}
					else
					{
						GetAnimator2D()->Play(L"player_run_up", 0, true);
					}
				}
				else if (move_dir_.y < 0)
				{
					if (move_dir_.x > 0)
					{
						GetAnimator2D()->Play(L"player_run_down_diag", 0, true);
					}
					else if (move_dir_.x < 0)
					{
						GetAnimator2D()->Play(L"player_run_down_diag_left", 0, true);
					}
					else
					{
						GetAnimator2D()->Play(L"player_run_down", 0, true);
					}
				}
				else
				{
					if (move_dir_.x > 0)
					{
						GetAnimator2D()->Play(L"player_run_down_diag", 0, true);
					}
					else if (move_dir_.x < 0)
					{
						GetAnimator2D()->Play(L"player_run_down_diag_left", 0, true);
					}
				}
			}

		}
	}

	if (PLAYER_STATE::IDLE == current_state_ && current_state_ != previouse_state_)
	{

		if (face_dir_.y > 0)
		{
			if (face_dir_.x > 0)
			{
				GetAnimator2D()->Play(L"player_idle_up_diag", 0, true);
			}
			else if (face_dir_.x < 0)
			{
				GetAnimator2D()->Play(L"player_idle_up_diag_left", 0, true);
			}
			else
			{
				GetAnimator2D()->Play(L"player_idle_up", 0, true);
			}
		}
		else if (face_dir_.y < 0)
		{
			if (face_dir_.x > 0)
			{
				GetAnimator2D()->Play(L"player_idle_down_diag", 0, true);
			}
			else if (face_dir_.x < 0)
			{
				GetAnimator2D()->Play(L"player_idle_down_diag_left", 0, true);
			}
			else
			{
				GetAnimator2D()->Play(L"player_idle_down", 0, true);
			}
		}
		else
		{
			if (face_dir_.x > 0)
			{
				GetAnimator2D()->Play(L"player_idle_down_diag", 0, true);
			}
			else if (face_dir_.x <= 0)
			{
				GetAnimator2D()->Play(L"player_idle_down_diag_left", 0, true);
			}

		}
	}
}

void CPlayerScript::MovePlayer()
{
	CRigidBody2D* rigidBody = GetRigidBody2D();
	Vec2 velocity = rigidBody->GetVelocity();
	float speed = 0;
	if (PLAYER_STATE::ATTACK == current_state_ || PLAYER_STATE::DAMEGED == current_state_)
		return;
	if (KEY_HOLD(KEY::LSHIFT))
	{
		is_run_ = true;
		if (!is_run_)
		{
			player_walk_speed_ *= 1.2f;

		}
	}
	else
	{
		if (is_run_)
		{
			player_walk_speed_ *= 0.8f;
			is_run_ = false;
		}
	}
	if (KEY_TAP(KEY::W))
	{
		move_dir_.y = 1;
		rigidBody->SetVelocity(Vec2(velocity.x, player_walk_speed_));
		current_state_ = PLAYER_STATE::MOVE;
	}
	if (KEY_TAP(KEY::S))
	{
		move_dir_.y = -1;
		rigidBody->SetVelocity(Vec2(velocity.x, -player_walk_speed_));
		current_state_ = PLAYER_STATE::MOVE;
	}
	if (KEY_TAP(KEY::A))
	{
		move_dir_.x = -1;
		rigidBody->SetVelocity(Vec2(-player_walk_speed_, velocity.y));
		current_state_ = PLAYER_STATE::MOVE;
	}
	if (KEY_TAP(KEY::D))
	{
		move_dir_.x = 1;
		rigidBody->SetVelocity(Vec2(player_walk_speed_, velocity.y));
		current_state_ = PLAYER_STATE::MOVE;
	}

	if (KEY_HOLD(KEY::W))
	{
		rigidBody->ApplyImpulse(Vec2(0.f, player_walk_speed_));
		current_state_ = PLAYER_STATE::MOVE;
	}
	if (KEY_HOLD(KEY::S))
	{
		rigidBody->ApplyImpulse(Vec2(0.f, -player_walk_speed_));
		current_state_ = PLAYER_STATE::MOVE;
	}
	if (KEY_HOLD(KEY::A))
	{
		rigidBody->ApplyImpulse(Vec2(-player_walk_speed_, 0.f));
		current_state_ = PLAYER_STATE::MOVE;
	}
	if (KEY_HOLD(KEY::D))
	{
		rigidBody->ApplyImpulse(Vec2(player_walk_speed_, 0.f));
		current_state_ = PLAYER_STATE::MOVE;
	}
	if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S))
	{
		move_dir_.y = 0;
	}
	if (KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
	{
		move_dir_.x = 0;
	}
	if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
	{
		accumulated_time_ += fDT;
		face_dir_ = rigidBody->GetMoveDir();
		if (accumulated_time_ >= 1.f)
		{
			current_state_ = PLAYER_STATE::IDLE;
			rigidBody->SetVelocity(Vec2());
			accumulated_time_ = 0.f;
		}
		else if(velocity.Length() != 0)
			rigidBody->ApplyImpulse(-face_dir_ * 10.f);
	}
}

void CPlayerScript::SaveToScene(FILE* file)
{
	__super::SaveToScene(file);
	fwrite(&player_walk_speed_, sizeof(float), 1, file);
	SaveResReference(player_bullet_prefab_, file);
}

void CPlayerScript::LoadFromScene(FILE* file)
{
	__super::LoadFromScene(file);
	fread(&player_walk_speed_, sizeof(float), 1, file);
	LoadResReference(player_bullet_prefab_, file);
}
