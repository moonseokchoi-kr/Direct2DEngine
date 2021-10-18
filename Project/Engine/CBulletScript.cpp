#include "pch.h"
#include "CBulletScript.h"

CBulletScript::CBulletScript()
	:bullet_speead_(200.f)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::Update()
{
	Vec3 position = GetTransform()->GetPos();
	
	position.y += fDT * bullet_speead_;

	GetTransform()->SetPos(position);

	DeleteObject(GetOwner(), 2.f);
}
