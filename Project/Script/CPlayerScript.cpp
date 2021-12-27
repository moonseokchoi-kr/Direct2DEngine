#include "pch.h"
#include "CPlayerScript.h"
#include "CBulletScript.h"
#include <Engine/CAnimation2D.h>
#include <Engine/CRigidBody2D.h>


CPlayerScript::CPlayerScript()
	:CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	,player_move_speed_(400)
	,accumulated_time_(0)
	,player_bullet_attack_speed_(0.1f)

{
	player_bullet_prefab_ = CResourceManager::GetInst()->FindRes<CPrefab>(L"player_bullet_prefab");
	SetName(L"PlayerScript");
	AddParam({ L"player_move_speed_",SCRIPT_PARAM::FLOAT,&player_move_speed_ });
	AddParam({ L"player_bullet_attack_speed_",SCRIPT_PARAM::FLOAT,&player_bullet_attack_speed_ });
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Update()
{
	accumulated_time_ += fDT;
	CRigidBody2D* rigidBody = GetRigidBody2D();
	Vec3 rotation = GetTransform()->GetRotation();
	if (KEY_HOLD(KEY::UP))
	{
		rigidBody->SetVelocity(Vec2(0.f, player_move_speed_));
	}
	if (KEY_HOLD(KEY::DOWN))
	{
		rigidBody->SetVelocity(Vec2(0.f, -player_move_speed_));
		
	}
	if (KEY_HOLD(KEY::LEFT))
	{
		rigidBody->SetVelocity(Vec2(-player_move_speed_,0.f));

	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		rigidBody->SetVelocity(Vec2(player_move_speed_, 0.f));

	}
	move_dir_ = GetTransform()->GetMoveDir();
	if (KEY_AWAY(KEY::UP) || KEY_AWAY(KEY::DOWN) || KEY_AWAY(KEY::RIGHT) || KEY_AWAY(KEY::LEFT))
	{
		rigidBody->SetVelocity(move_dir_ *50.f);
	}
	if (KEY_HOLD(KEY::Q))
	{
		rotation.z += 100.f * fDT;
	}
	if (KEY_HOLD(KEY::E))
	{
		rotation.z -= 100.f * fDT;
	}
	if (KEY_HOLD(KEY::SPACE))
	{
		if (player_bullet_attack_speed_ <= accumulated_time_)
		{
			CreateBullet();
			accumulated_time_ = 0.f;
		}
		
	}

	GetTransform()->SetRotation(rotation);
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
	if(0==GetCollider2D()->GetCollisionCount())
		GetMeshRender()->GetSharedMaterial();
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
	
		if (move_dir_ < Vec2(0,0))
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
	SaveResReference(player_bullet_prefab_,file);
}

void CPlayerScript::LoadFromScene(FILE* file)
{
	__super::LoadFromScene(file);
	LoadResReference(player_bullet_prefab_, file);
}
