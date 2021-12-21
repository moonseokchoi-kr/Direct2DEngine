#include "pch.h"
#include "CMonsterHpBarScript.h"
#include "CMonsterScript.h"
CMonsterHpBarScript::CMonsterHpBarScript()
	:CScript((int)SCRIPT_TYPE::MONSTERHPBARSCRIPT)
{
}

CMonsterHpBarScript::~CMonsterHpBarScript()
{
}

void CMonsterHpBarScript::Update()
{
	Vec3 view_scale = GetTransform()->GetScale();
	view_scale.x = CMonsterScript::GetHp() * 10.f;
	GetTransform()->SetScale(view_scale);
}
