#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
	, local_direction_{ Vec3::Right,Vec3::Up,Vec3::Front }
	, world_direction_{}
	, local_scale_(Vec3(1.f, 1.f, 1.f))
	, cal_child_transform_(false)
{
	 
}

CTransform::~CTransform()
{
}

void CTransform::FinalUpdate()
{
	Matrix translateMat = XMMatrixTranslation(local_position_.x, local_position_.y, local_position_.z);

	Matrix scaleMat = XMMatrixScaling(local_scale_.x, local_scale_.y, local_scale_.z);

	Matrix rotationXMat = XMMatrixRotationX(XMConvertToRadians(local_rotation_.x));
	Matrix rotationYMat = XMMatrixRotationY(XMConvertToRadians(local_rotation_.y));
	Matrix rotationZMat = XMMatrixRotationZ(XMConvertToRadians(local_rotation_.z));
	Matrix rotationMat = rotationXMat * rotationYMat * rotationZMat;

	world_matrix_ = scaleMat * rotationMat * translateMat;

	for (UINT i = 0; i < (UINT)DIRECTION_TYPE::END; ++i)
	{
		local_direction_[i] = XMVector3TransformNormal(axis_array_[i], rotationMat);
		world_direction_[i] = local_direction_[i].Normalize();
	}

	CGameObject* parentObj = GetOwner()->GetParent();
	if (parentObj)
	{
		const Matrix& parentWorldMatrix = parentObj->Transform()->GetWorldMatrix();
		world_matrix_ *= parentWorldMatrix;

		for (UINT i = 0; i < (UINT)DIRECTION_TYPE::END; ++i)
		{
			world_direction_[i] = XMVector3TransformNormal(axis_array_[i], world_matrix_);
			world_direction_[i].Normalize();
		}

	}

}

void CTransform::UpdateData()
{
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM);
	g_transform.world_matrix = world_matrix_;
	g_transform.world_view_matrix = g_transform.world_matrix * g_transform.view_matrix;
	g_transform.world_view_projection_matrix = g_transform.world_view_matrix * g_transform.projection_matrix;


	cb->SetData(&g_transform, sizeof(Transform));
	cb->SetPipelineStage(PS_NONPIXEL);
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
