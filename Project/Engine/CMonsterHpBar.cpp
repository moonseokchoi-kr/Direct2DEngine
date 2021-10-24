#include "pch.h"
#include "CMonsterHpBar.h"
#include "CMonsterScript.h"
CMonsterHpBar::CMonsterHpBar()
{
}

CMonsterHpBar::~CMonsterHpBar()
{
}

void CMonsterHpBar::Update()
{
	Vec3 scale = GetTransform()->GetScale();
	scale.x = CMonsterScript::GetHp() * 10.f;
	GetTransform()->SetScale(scale);
}
