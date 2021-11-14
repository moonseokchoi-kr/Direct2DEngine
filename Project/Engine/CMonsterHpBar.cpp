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
	Vec3 view_scale = GetTransform()->GetScale();
	view_scale.x = CMonsterScript::GetHp() * 10.f;
	GetTransform()->SetScale(view_scale);
}
