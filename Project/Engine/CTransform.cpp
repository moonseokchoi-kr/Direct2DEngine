#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
{
	local_scale_ = Vec3(1.f, 1.f, 1.f);
}

CTransform::~CTransform()
{
}



void CTransform::FinalUpdate()
{
	Matrix translateMat = XMMatrixTranslation(local_position_.x, local_position_.y, local_position_.z);

	Matrix scaleMat = XMMatrixScaling(local_scale_.x, local_scale_.y, local_scale_.z);

	Matrix rotationXMat = XMMatrixRotationX(local_rotation_.x);
	Matrix rotationYMat = XMMatrixRotationY(local_rotation_.y);
	Matrix rotationZMat = XMMatrixRotationZ(local_rotation_.z);
	Matrix rotationMat = rotationXMat * rotationYMat * rotationZMat;

	world_matrix_ = scaleMat * rotationMat * translateMat;

	if (GetOwner()->GetParent())
	{
		world_matrix_ *= GetOwner()->GetParent()->Transform()->GetWorldMatrix();
	}

}

void CTransform::UpdateData()
{
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.world_matrix = world_matrix_;
	g_transform.world_view_matrix = g_transform.world_matrix * g_transform.view_matrix;
	g_transform.world_view_projection_matrix = g_transform.world_view_matrix * g_transform.projection_matrix;


	cb->SetData(&g_transform, sizeof(Transform));
	cb->SetPipelineStage(PS_VERTEX);
	cb->UpdateData();
}

Vec3 CTransform::GetWorldScale()
{
	CGameObject* parent = GetOwner()->GetParent();

	Vec3 worldScale = local_scale_;

	while (nullptr != parent)
	{
		worldScale *= parent->Transform()->GetScale();
		parent = parent->GetParent();
	}

	return worldScale;
}
