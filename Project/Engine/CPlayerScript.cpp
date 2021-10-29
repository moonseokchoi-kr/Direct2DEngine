#include "pch.h"
#include "CPlayerScript.h"
#include "CBulletScript.h"



CPlayerScript::CPlayerScript()
	:player_move_speed_(400)
	,accumulated_time_(0)
	,player_bullet_attack_speed_(0.1f)
{
	player_bullet_prefab_ = CResourceManager::GetInst()->FindRes<CPrefab>(L"player_bullet_prefab");
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Update()
{
	accumulated_time_ += fDT;
	Vec3 pos = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();
	Vec3 rot = GetTransform()->GetRotation();
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
	GetTransform()->SetScale(scale);
	GetTransform()->SetRotation(rot);
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
	Vec3 scale = GetTransform()->GetScale();

	position.y += scale.y / 2.f;

	Instantiate(player_bullet_prefab_, position, 2);

	
}
