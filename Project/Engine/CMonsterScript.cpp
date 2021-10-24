#include "pch.h"
#include "CMonsterScript.h"
#include "CCollider2D.h"

CMonsterScript::CMonsterScript()
	:bullet_start_angle_(0)
	, bullet_angle_{ }
	, bullet_rotate_angle_{ }
	, bullet_angle_rate_{ }
	, bullet_type_(ENEMY_BULLET_TYPE::COMBINE_SPIRAL)
	, shoot_count_(11)
	, attack_time_(0.3f)
	, attack_during_time_(0.2f)
	, interval_(0.1f)
	, init_(false)
	, hp(100.f)
	,bullet_speed_rate_{}
{
	SetRandomPattern();
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::Update()
{
	if (hp <= 0)
	{
		DeleteObject(GetOwner());
		return;
	}
	accumulated_time_ += fDT;

	if (attack_time_ <= accumulated_time_)
	{
		Fire();
		if (!init_)
			SetRandomPattern();
		
		accumulated_time_ = 0.0f;
	}
}

void CMonsterScript::OnCollisionEnter(CCollider2D* otherCollider)
{
	CGameObject* object = otherCollider->GetOwner();
	if (L"player_bullet" == object->GetName())
	{
		//OutputDebugString(L"총돌했습니다!\n");
		hp -= 1;
	}
}


#include "CBulletScript.h"

CBulletScript* CMonsterScript::CreateBulletMoveDirection(int index = 0)
{
	CBulletScript* script = new CBulletScript;
	Vec3 position = GetTransform()->GetPosition();

	Vec3 bulletMoveVector = Vec3(position.x + cosf((bullet_angle_[index] * XM_PI) / 180.f), position.y + sinf((bullet_angle_[index] * XM_PI) / 180.f), 0.f);
	Vec3 bulletDirection = (bulletMoveVector - position).Normalize();

	bullet_angle_[index] += bullet_angle_rate_[index];

	script->SetMoveDirection(bulletDirection);
	script->SetMoveSpeedRate(bullet_speed_rate_[index]);
	script->SetMoveSpeed(50000.f * 2);
	script->SetRotationAngle(bullet_rotate_angle_[index]);
	return script;
}

void CMonsterScript::SetRandomPattern()
{
	random_device rd;
	mt19937 gen(rd());
	std::uniform_int_distribution<> dist (0, static_cast<UINT>(ENEMY_BULLET_TYPE::END));

	bullet_type_ = static_cast<ENEMY_BULLET_TYPE>(dist(gen));

	switch (bullet_type_)
	{
	case ENEMY_BULLET_TYPE::DIRECTIONAL:
	{
		bullet_start_angle_ = -90;
		bullet_angle_[0] = bullet_start_angle_;
		bullet_angle_rate_[0] = 0;
		shoot_count_ = 1;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::BI_DIRECTIONAL:
	{
		bullet_angle_[0] = 180;
		bullet_angle_[1] = 0;
		bullet_angle_rate_[0] = 10;
		bullet_angle_rate_[1] = 10;
		shoot_count_ = 2;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::SPIRAL:
	{
		bullet_start_angle_ = 0;
		bullet_angle_rate_[0] = 10;
		shoot_count_ = 1;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::MULTIPLE_SPIRAL:
	{
		bullet_start_angle_ = 180;
		shoot_count_ = 10;
		bullet_angle_rate_[0] = (360 - bullet_start_angle_) / shoot_count_;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::BENT_SPIRAL:
	{
		bullet_angle_[0] = 180;
		bullet_angle_[1] = 0;
		bullet_angle_rate_[0] = 10;
		bullet_angle_rate_[1] = 10;
		bullet_rotate_angle_[0] = 20;
		bullet_rotate_angle_[1] = 20;
		bullet_speed_rate_[0] = 200.f;
		bullet_speed_rate_[1] = 200.f;
		shoot_count_ = 2;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::COMBINE_SPIRAL:
	{
		for (int i = 0; i < bullet_angle_.size(); ++i)
		{
			bullet_angle_[i] = (360.f / bullet_angle_.size()) * i;
		}

		bullet_angle_rate_.fill(10);

		bullet_rotate_angle_.fill (20);
		bullet_rotate_angle_[0]=0;
		bullet_rotate_angle_[2]=0;
		bullet_speed_rate_.fill(300);
		bullet_speed_rate_[0] = 0;
		bullet_speed_rate_[2] = 0;
		shoot_count_ = 4;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::WASHER_SPIRAL:
	{
		for (int i = 0; i < bullet_angle_.size(); ++i)
		{
			bullet_angle_[i] = (360.f / bullet_angle_.size()) * i;
		}

		bullet_angle_rate_.fill(10);

		bullet_rotate_angle_.fill(20);
		bullet_rotate_angle_[0] = 0;
		bullet_rotate_angle_[2] = 0;
		bullet_rotate_angle_[4] = 0;


		bullet_speed_rate_.fill(300);
		bullet_speed_rate_[0] = 0;
		bullet_speed_rate_[2] = 0;
		bullet_speed_rate_[4] = 0;
		shoot_count_ = 6;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::N_WAY:
	{
		bullet_start_angle_ = 180;
		bullet_angle_[0] = bullet_start_angle_;
		shoot_count_ = 10;
		bullet_angle_rate_[0] = (360 - bullet_start_angle_) / shoot_count_;
	}
		break;
	default:
		break;
	}
}


#include "CPathManager.h"
void CMonsterScript::CreateBullet(int index = 0)
{
	CGameObject* bullet = new CGameObject;
	CBulletScript* script = nullptr;
	bullet->SetName(L"Bullet");
	bullet->AddComponent(new CTransform);
	bullet->AddComponent(new CMeshRender);
	bullet->AddComponent(new CCollider2D);
	bullet->AddComponent(CreateBulletMoveDirection(index));
	Vec3 position = GetTransform()->GetPosition();
	position.y -= 25.f;
	bullet->Transform()->SetPosition(position);
	bullet->Transform()->SetScale(Vec3(25.f, 25.f, 1.f));
	bullet->Transform()->SetRotation(Vec3(0.f, 0.f, 0.f));

	bullet->Collider2D()->SetOffsetScale(Vec2(0.8f, 0.8f));

	bullet->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));

	Ptr<CTexture> bulletTex = new CTexture;
	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"texture\\player_bullet.png";
	bulletTex->Load(strPath);
	CResourceManager::GetInst()->AddResource(L"player_bullet", bulletTex.Get());


	CMaterial* bulletMaterial = new CMaterial;
	bulletMaterial->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(L"std2DShader"));
	bulletMaterial->SetData(SHADER_PARAM::TEX_0, bulletTex.Get());
	CResourceManager::GetInst()->AddResource(L"bulletMaterial", bulletMaterial);
	bullet->MeshRender()->SetMaterial(bulletMaterial);

	CreateObject(bullet, 3);
}

void CMonsterScript::Fire()
{
	switch (bullet_type_)
	{
	case ENEMY_BULLET_TYPE::DIRECTIONAL:
	case ENEMY_BULLET_TYPE::SPIRAL:
	case ENEMY_BULLET_TYPE::BI_DIRECTIONAL:
	case ENEMY_BULLET_TYPE::BENT_SPIRAL:
	case ENEMY_BULLET_TYPE::COMBINE_SPIRAL:
	case ENEMY_BULLET_TYPE::WASHER_SPIRAL:
	{
		if (attack_during_time_ >= interval_)
		{
			for (int i = 0; i < shoot_count_; ++i)
			{
				CreateBullet(i);
			}
			attack_during_time_ -= fDT;
		}
		else
		{
			init_ = false;
			attack_during_time_ = interval_ * 2;
		}
	}
		break;
	case ENEMY_BULLET_TYPE::MULTIPLE_SPIRAL:
	{
		if (attack_during_time_ >= interval_)
		{
			for (int i = 0; i < shoot_count_; ++i)
			{
				CreateBullet();
			}
			attack_during_time_ -= fDT;
		}
		else
		{
			init_ = false;
			attack_during_time_ = interval_ * 2;
		}
	}

	case ENEMY_BULLET_TYPE::N_WAY:
	{
		for (int i = 0; i < shoot_count_; i++)
		{
			CreateBullet();
		}
		init_ = false;
	}
		break;
	default:
		break;
	}
}
