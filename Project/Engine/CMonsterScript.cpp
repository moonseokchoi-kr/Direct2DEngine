#include "pch.h"
#include "CMonsterScript.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CGameObject.h"
float CMonsterScript::hp = 100.0f;

CMonsterScript::CMonsterScript()
	:bullet_start_angle_(0)
	, bullet_angle_{ }
	, bullet_rotate_angle_{ }
	, bullet_angle_rate_{ }
	, bullet_type_(ENEMY_BULLET_TYPE::N_WAY)
	, shoot_count_(11)
	, attack_time_(5.f)
	,accumulated_time_(0.5f)
	, interval_(0.0f)
	, init_(false)
	, is_move_(false)
	, monster_state_(MONSTER_STATE::IDLE)
	,monster_prev_state_(MONSTER_STATE::IDLE)
	, monster_state_time_(accumulated_time_)
	,bullet_speed_rate_{}
{
	SetRandomPattern();
}

CMonsterScript::CMonsterScript(float attack_time)
	:bullet_start_angle_(0)
	, bullet_angle_{ }
	, bullet_rotate_angle_{ }
	, bullet_angle_rate_{ }
	, bullet_type_(ENEMY_BULLET_TYPE::MULTIPLE_SPIRAL)
	, shoot_count_(11)
	, attack_time_(attack_time)
	, interval_(attack_time)
	, is_move_(false)
	, init_(false)
	, accumulated_time_(0.0f)
	, monster_state_(MONSTER_STATE::IDLE)
	, monster_prev_state_(MONSTER_STATE::IDLE)
	, monster_state_time_(0.5f)
	, bullet_speed_rate_{}
{
	SetRandomPattern();
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::Update()
{
	Vec3 position = GetTransform()->GetPosition();
	if (hp <= 0)
	{
		DeleteObject(GetOwner());
		return;
	}
	UpdateState();
	UpdateAnimation();
}

void CMonsterScript::OnCollisionEnter(CGameObject* otherObject)
{
	if (L"player_bullet" == otherObject->GetName())
	{
		//OutputDebugString(L"총돌했습니다!\n");
		hp -= 1;
	}
}

void CMonsterScript::OnCollisionExit(CGameObject* otherObject)
{

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
	script->SetMoveSpeed(bullet_speed_);
	script->SetRotationAngle(bullet_rotate_angle_[index]);
	return script;
}

void CMonsterScript::SetRandomPattern()
{
	random_device rd;
	mt19937 gen(rd());
	std::uniform_int_distribution<> dist (0, static_cast<UINT>(ENEMY_BULLET_TYPE::END));

	bullet_type_ = static_cast<ENEMY_BULLET_TYPE>(dist(gen));

	spiral_time_ = 0.1f;
	spiral_accumulated_time_ = 0.0f;
	bullet_speed_ = 60000.f;
	switch (bullet_type_)
	{
	case ENEMY_BULLET_TYPE::DIRECTIONAL:
	{
		spiral_time_ = 0.4f;
		spiral_accumulated_time_ = 0.0f;
		bullet_angle_[0] = CalculateTargetDegree();
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
		shoot_count_ = 4;
		bullet_speed_ = 120000.f;
		for (int i = 0; i < shoot_count_; ++i)
		{
			bullet_angle_[i] = (360 / shoot_count_)*i;
			//bullet_speed_rate_[i] = 10 + 10 * i;
		}
		bullet_angle_rate_.fill(0.5);
	
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
		shoot_count_ = 5;
		for (int i = 0; i < shoot_count_; ++i)
		{
			bullet_angle_[i] = (360.f / shoot_count_) * i;
		}

		bullet_angle_rate_.fill(10);

		bullet_rotate_angle_.fill (20);
		bullet_rotate_angle_[0]=0;
		bullet_rotate_angle_[2]=0;
		bullet_speed_rate_.fill(300);
		bullet_speed_rate_[0] = 0;
		bullet_speed_rate_[2] = 0;
		spiral_time_ = 0.1f;
		spiral_accumulated_time_ = 0.0f;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::WASHER_SPIRAL:
	{
		for (int i = 0; i < bullet_angle_.size(); ++i)
		{
			bullet_angle_[i] = (360.f / bullet_angle_.size()) * i;
		}
		attack_time_ = attack_time_ * 2.f;
		bullet_angle_rate_[0] =1;
		bullet_angle_rate_[1] =5;
		bullet_angle_rate_[2] =1;
		bullet_angle_rate_[3] =5;
		bullet_angle_rate_[4] =1;
		bullet_angle_rate_[5] =5;

		bullet_rotate_angle_.fill(20);
		bullet_rotate_angle_[0] = 5;
		bullet_rotate_angle_[2] = 5;
		bullet_rotate_angle_[4] = 5;


		bullet_speed_rate_.fill(300);
		bullet_speed_rate_[0] = 100;
		bullet_speed_rate_[2] = 200;
		bullet_speed_rate_[4] = 300;
		shoot_count_ = 6;
		init_ = true;
	}
		break;
	case ENEMY_BULLET_TYPE::N_WAY:
	{

		spiral_time_ = 0.4f;
		spiral_accumulated_time_ = 0.0f;
		bullet_angle_[0] = 180.f;
		shoot_count_ = 10;
		bullet_angle_rate_[0] = 180.f / shoot_count_;
		bullet_speed_rate_[0] = -10.f;
		init_ = true;
	}
	break;
	case ENEMY_BULLET_TYPE::CIRCLE:
	{
		bullet_angle_[0] = 90;
		bullet_angle_[1] = 0;
		bullet_angle_rate_[0] = 5;
		bullet_angle_rate_[1] = 10;
		shoot_count_ = 5;
		init_ = true;
	}
		break;
	default:
		break;
	}
}

void CMonsterScript::UpdateState()
{
	accumulated_time_ += fDT;
	if (attack_time_ >= interval_ && MONSTER_STATE::ATTACK == monster_state_)
		return;
	if (monster_state_time_ >= accumulated_time_)
		return;
	if (MONSTER_STATE::IDLE == monster_prev_state_)
	{
		monster_prev_state_ = monster_state_;
		monster_state_ = MONSTER_STATE::ATTACK;
	}
	if (MONSTER_STATE::MOVE == monster_prev_state_)
	{
		monster_prev_state_ = monster_state_;
		monster_state_ = MONSTER_STATE::IDLE;
	}
	if (MONSTER_STATE::ATTACK == monster_prev_state_)
	{
		random_device rd;
		mt19937 gen(rd());
		std::uniform_int_distribution<> dist(-4, 4);
		int i = dist(gen);
		move_dirction = Vec3(30.f*i,30.f*i,0.0f);
		monster_prev_state_ = monster_state_;
		monster_state_ = MONSTER_STATE::MOVE;
	}
	accumulated_time_ = 0.0f;;
}

void CMonsterScript::UpdateAnimation()
{
	
	Vec3 position = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();
	if (MONSTER_STATE::MOVE == monster_state_)
	{
		position += move_dirction*fDT;
		if (abs(position.x) >= 400.f - scale.x / 2.f )
		{
			position.x = 400.f-scale.x/2.f;
		}
		if (abs(position.y) >= 450.f - scale.x / 2.f)
		{
			position.y = 450.f - scale.x / 2.f;
		}
		GetTransform()->SetPosition(position);
	}
	if (MONSTER_STATE::ATTACK == monster_state_)
	{
		
		if (!init_)
			SetRandomPattern();
		if (attack_time_ >= interval_)
		{
			Fire();
			interval_ += fDT;
		}
		else
		{
			interval_ = 0.f;
			init_ = false;
		}
	}
}

float CMonsterScript::CalculateTargetDegree()
{
	CGameObject* target = CSceneManager::GetInst()->GetCurrentScene()->GetObjectWithName(L"player");
	if (nullptr == target->Transform() || nullptr == GetOwner())
		return -90;
	Vec3 direction = (target->Transform()->GetPosition() - GetTransform()->GetPosition()).Normalize();
	float radian = asinf(direction.y);
	float degree = (radian * 180.f) / XM_PI;
	if (target->Transform()->GetPosition().x < 0)
		degree -= 90;
	return degree;
}


#include "CPathManager.h"
void CMonsterScript::CreateBullet(int index = 0)
{
	CGameObject* bullet = new CGameObject;
	CBulletScript* script = nullptr;
	bullet->SetName(L"monster_bullet");
	bullet->AddComponent(new CTransform);
	bullet->AddComponent(new CMeshRender);
	bullet->AddComponent(new CCollider2D);
	bullet->AddComponent(CreateBulletMoveDirection(index));
	Vec3 position = GetTransform()->GetPosition();
	position.y -= 25.f;
	bullet->Transform()->SetPosition(position);
	bullet->Transform()->SetScale(Vec3(25.f, 25.f, 1.f));
	bullet->Transform()->SetRotation(Vec3(0.f, 0.f, 0.f));

	bullet->Collider2D()->SetOffsetScale(Vec2(0.6f, 0.6f));

	bullet->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"CircleMesh"));

	int bullet_type = (int)bullet_type_;

	if (bullet_type % 2)
	{
		bullet->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"monster_bulluet_red_material"));
	}
	else
	{
		bullet->MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"monster_bulluet_blue_material"));
	}
	CreateObject(bullet, 3);
}

void CMonsterScript::Directional()
{

}

void CMonsterScript::Spiral()
{

}

void CMonsterScript::Circle()
{
}

void CMonsterScript::Fire()
{
	switch (bullet_type_)
	{
	case ENEMY_BULLET_TYPE::BI_DIRECTIONAL:
	case ENEMY_BULLET_TYPE::BENT_SPIRAL:
	case ENEMY_BULLET_TYPE::COMBINE_SPIRAL:
	{	

		spiral_accumulated_time_ += fDT;
		if (spiral_time_ <= spiral_accumulated_time_)
		{
			for (int i = 0; i < shoot_count_; ++i)
			{
				CreateBullet(i);
			}
			spiral_accumulated_time_ = 0.f;
		}

	}
		break;
	case ENEMY_BULLET_TYPE::WASHER_SPIRAL:
	{
		spiral_accumulated_time_ += fDT;
		if (spiral_time_ <= spiral_accumulated_time_)
		{
			for (int i = 0; i < shoot_count_; ++i)
			{
				CreateBullet(i);
			}
			spiral_accumulated_time_ = 0.f;
		}
	}
	break;
	case ENEMY_BULLET_TYPE::DIRECTIONAL:
	case ENEMY_BULLET_TYPE::SPIRAL:
	{
		spiral_accumulated_time_ += fDT;
		if (spiral_time_ <= spiral_accumulated_time_)
		{
			CreateBullet();
			spiral_accumulated_time_ = 0.f;
		}
	}
	break;
	case ENEMY_BULLET_TYPE::MULTIPLE_SPIRAL:
	{
		spiral_accumulated_time_ += fDT;
		if (spiral_time_ <= spiral_accumulated_time_)
		{
			for (int i = 0; i < shoot_count_; ++i)
			{
				CreateBullet(i);
			}
			spiral_accumulated_time_ = 0.f;
		}
	}
	break;
	case ENEMY_BULLET_TYPE::N_WAY:
	{
		spiral_accumulated_time_ += fDT;
		if (spiral_time_ <= spiral_accumulated_time_)
		{
			for (int i = 0; i < shoot_count_; ++i)
			{
				CreateBullet();
			}
			spiral_accumulated_time_ = 0.f;
			bullet_angle_[0] = 180;

		}
	}
	break;
	case ENEMY_BULLET_TYPE::CIRCLE:
	{
		spiral_accumulated_time_ += fDT;
		if (spiral_time_ <= spiral_accumulated_time_)
		{
			for (int i = 0; i < shoot_count_; ++i)
			{
				CreateBullet();
				CreateBullet(1);
			}
			spiral_accumulated_time_ = 0.f;
		}
	}
	break;
	default:
		break;
	}
}
