#include "pch.h"
#include "CPlayerScript.h"
#include "CBulletScript.h"



CPlayerScript::CPlayerScript()
	:player_move_speed_(400)
	,accumulated_time_(0)
	,player_bullet_attack_speed_(0.03f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Update()
{
	accumulated_time_ += fDT;
	Vec3 pos = GetTransform()->GetPos();
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

	if (KEY_HOLD(KEY::LBTN))
	{
		scale -= Vec3(1.f, 1.f, 1.f) * 300*fDT;
	}
	if (KEY_HOLD(KEY::RBTN))
	{
		scale += Vec3(1.f, 1.f, 1.f) * 300*fDT;
	}

	if (KEY_HOLD(KEY::Q))
	{
		rot.z += XM_2PI * fDT;
	}
	if (KEY_HOLD(KEY::E))
	{
		rot.z -= XM_2PI * fDT;
	}
	if (KEY_HOLD(KEY::SPACE))
	{
		if (player_bullet_attack_speed_ <= accumulated_time_)
		{
			CreateBullet();
		}
		accumulated_time_ = 0.f;
	}
	
	GetTransform()->SetPos(pos);
	GetTransform()->SetScale(scale);
	GetTransform()->SetRotation(rot);
}

void CPlayerScript::CreateBullet()
{
	Vec3 pos = GetTransform()->GetPos();
	Vec3 scale = GetTransform()->GetScale();
	Vec3 rot = GetTransform()->GetRotation();

	CGameObject* bullet = new CGameObject;
	bullet->SetName(L"Bullet");
	bullet->AddComponent(new CTransform);
	bullet->AddComponent(new CMeshRender);
	bullet->AddComponent(new CBulletScript);

	Vec3 bulletPosition = pos;
	bulletPosition.y += scale.y / 2.f;

	bullet->Transform()->SetPos(bulletPosition);
	bullet->Transform()->SetScale(Vec3(50.f, 50.f, 1.f));
	
	bullet->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	bullet->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"std2DMaterial"));

	CreateObject(bullet, 0);

	
}
