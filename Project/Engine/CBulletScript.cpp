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
	Vec3 position = GetTransform()->GetPosition();
	
	position.y += fDT * bullet_speead_;

	GetTransform()->SetPosition(position);

	DeleteObject(GetOwner(), 2.f);
}
