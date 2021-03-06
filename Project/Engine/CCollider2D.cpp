#include "pch.h"
#include "CCollider2D.h"

#include "CTransform.h"
#include "CDevice.h"
#include "CConstBuffer.h"

#include "CRigidBody2D.h"
#include "CResourceManager.h"
#include "CScript.h"
CCollider2D::CCollider2D()
	:CComponent(COMPONENT_TYPE::COLLIDER2D)
	,offset_position_(Vec3(0.f,0.f,0.f))
	,offset_scale_(Vec3(1.f,1.f,1.f))
	,collision_count_(0)
{
	collider_mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	collider_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"collider2DMaterial_none");
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::UpdateData()
{
	CConstBuffer* constantBuffer = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM);
	g_transform.world_matrix = collider_world_matrix_;
	g_transform.world_view_matrix = g_transform.world_matrix * g_transform.view_matrix;
	g_transform.world_view_projection_matrix = g_transform.world_view_matrix * g_transform.projection_matrix;

	constantBuffer->SetData(&g_transform, sizeof(Transform));
	constantBuffer->SetPipelineStage(PIPELINE_STAGE::PS_VERTEX);
	constantBuffer->UpdateData();

}

void CCollider2D::FinalUpdate()
{
	Vec3 worldScale = GetTransform()->GetWorldScale();

	Matrix transformMatrix = XMMatrixTranslation(offset_position_.x / worldScale.x, offset_position_.y / worldScale.y, offset_position_.z / worldScale.z);
	Matrix scaleMatrix = XMMatrixScaling(offset_scale_.x, offset_scale_.y, offset_scale_.z);

	collider_world_matrix_ = scaleMatrix * transformMatrix * GetTransform()->GetWorldMatrix();
}

void CCollider2D::Render()
{
	UpdateData();

	collider_mesh_->UpdateData();
	collider_material_->UpdateData();

	collider_mesh_->Render();
}

void CCollider2D::OnCollisionEnter(CGameObject* otherObject)
{
	++collision_count_;
	collider_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"collider2DMaterial_collision");
	const vector<CScript*>& scripts = GetOwner()->GetScripts();
	for (const auto& script : scripts)
	{
		script->OnCollisionEnter(otherObject);
	}
}
	
void CCollider2D::OnCollision(CGameObject* otherObject)
{
	const vector<CScript*>& scripts = GetOwner()->GetScripts();
	for (const auto& script : scripts)
	{
		script->OnCollision(otherObject);
	}
}

void CCollider2D::OnCollisionExit(CGameObject* otherObject)
{
	--collision_count_;
	if (0 == collision_count_)
	{
		collider_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"collider2DMaterial_none");
	}
	const vector<CScript*>& scripts = GetOwner()->GetScripts();
	for (const auto& script : scripts)
	{
		script->OnCollisionExit(otherObject);
	}
	
}

void CCollider2D::SaveToScene(FILE* file)
{
	CComponent::SaveToScene(file);
	fwrite(&offset_position_, sizeof(Vec3),1, file);
	fwrite(&offset_scale_, sizeof(Vec3), 1,file);
}

void CCollider2D::LoadFromScene(FILE* file)
{
	CComponent::LoadFromScene(file);
	fread(&offset_position_, sizeof(Vec3), 1, file);
	fread(&offset_scale_, sizeof(Vec3), 1, file);
}


