#include "pch.h"
#include "CCamera.h"

#include "CKeyManager.h"
#include "CTransform.h"

#include "CTimeManager.h"

CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
{
}

CCamera::~CCamera()
{
}

void CCamera::Update()
{
	Vec3 pos = GetTransform()->GetPos();
	if(KEY_HOLD(KEY::W))
	{
		pos.y -= 0.5f * fDT;
	}
	if (KEY_HOLD(KEY::A))
	{
		pos.x -= 0.5f * fDT;
	}
	if (KEY_HOLD(KEY::S))
	{
		pos.y += 0.5f * fDT;
	}
	if (KEY_HOLD(KEY::D))
	{
		pos.x += 0.5f * fDT;
	}
	GetTransform()->SetPos(pos);

}

void CCamera::LateUpdate()
{
	
}

void CCamera::FinalUpdate()
{
	const Vec3 pos = GetTransform()->GetPos();
	view_matrix_ = XMMatrixTranslation(pos.x, pos.y, pos.z);
	view_matrix_ = view_matrix_.Invert();

	g_transform.view_matrix = view_matrix_;
}
