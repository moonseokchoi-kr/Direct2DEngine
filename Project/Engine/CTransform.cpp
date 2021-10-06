#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
{
}

CTransform::~CTransform()
{
}

void CTransform::Update()
{
	if (KEY_HOLD(KEY::UP))
	{
		pos_ += Vec3(0.f,fDT * 0.5f,0.f);
	}
	if (KEY_HOLD(KEY::DOWN))
	{
		pos_ -= Vec3(0.f, fDT * 0.5f, 0.f);
	}
	if (KEY_HOLD(KEY::LEFT))
	{
		pos_ -= Vec3(fDT * 0.5f,0.f, 0.f);
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		pos_ += Vec3(fDT * 0.5f, 0.f, 0.f);
	}

	Matrix translateMat = XMMatrixTranslation(pos_.x, pos_.y, pos_.z);

	Matrix scaleMat = XMMatrixScaling(scale_.x, scale_.y, scale_.z);

	Matrix rotationXMat = XMMatrixRotationX(rotation_.x);
	Matrix rotationYMat = XMMatrixRotationY(rotation_.y);
	Matrix rotationZMat = XMMatrixRotationZ(rotation_.z);

	Matrix rotationMat = rotationXMat * rotationYMat * rotationZMat;

	world_matrix_ = scaleMat * rotationMat * translateMat;
	
}

void CTransform::UpdateData()
{

	g_transform.world_matrix = world_matrix_;
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	cb->SetData(&g_transform, sizeof(Transform));
	cb->SetPipelineState(PS_VERTEX);
	cb->UpdateData();
}
