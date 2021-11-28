#include "pch.h"
#include "CPlayerScript.h"
#include "CBulletScript.h"
#include "CAnimation2D.h"


CPlayerScript::CPlayerScript()
	:player_move_speed_(400)
	,accumulated_time_(0)
	,player_bullet_attack_speed_(0.1f)
	,preMoveDir(0)
	,moveDir(0)
{
	player_bullet_prefab_ = CResourceManager::GetInst()->FindRes<CPrefab>(L"player_bullet_prefab");
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Update()
{
	preMoveDir = moveDir;
	accumulated_time_ += fDT;
	Vec3 pos = GetTransform()->GetPosition();
	Vec3 view_scale = GetTransform()->GetScale();
	Vec3 rot = GetTransform()->GetRotation();
	if (KEY_HOLD(KEY::UP))
	{
		pos += Vec3(0.f, fDT * player_move_speed_ * 0.5f, 0.f);
		moveDir = 0;
	}
	if (KEY_HOLD(KEY::DOWN))
	{
		pos -= Vec3(0.f, fDT * player_move_speed_ * 0.5f, 0.f);
		moveDir = 0;
	}
	if (KEY_HOLD(KEY::LEFT))
	{
		pos -= Vec3(fDT * player_move_speed_ * 0.5f, 0.f, 0.f);
		moveDir = -1;
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		pos += Vec3(fDT * player_move_speed_
			* 0.5f, 0.f, 0.f);
		moveDir = 1;
	}
	if (KEY_AWAY(KEY::RIGHT) || KEY_AWAY(KEY::LEFT))
	{
		moveDir = 0;
	}

	if (KEY_HOLD(KEY::Q))
	{
		rot.z += 100.f*fDT;
	}
	if (KEY_HOLD(KEY::E))
	{
		rot.z -= 100.f*fDT;
	}
	if (KEY_HOLD(KEY::SPACE))
	{
		if (player_bullet_attack_speed_ <= accumulated_time_)
		{
			CreateBullet();
			accumulated_time_ = 0.f;
		}
		
	}
	GetTransform()->SetPosition(pos);
	GetTransform()->SetScale(view_scale);
	GetTransform()->SetRotation(rot);
	UpdateAnimation();
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
	if (moveDir == preMoveDir)
	{
	
		if (moveDir == -1)
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
		if (moveDir == 0)
			GetAnimator2D()->Play(L"FLY", 0, true);
		if (moveDir == -1)
			GetAnimator2D()->Play(L"FLY_LEFT", 0, true);
	}
}
