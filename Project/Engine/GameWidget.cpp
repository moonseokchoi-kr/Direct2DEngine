#include "pch.h"
#include "GameWidget.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CKeyManager.h"

GameWidget::GameWidget()
	:parent_widget_(nullptr)
	,owner_(nullptr)
	,is_hovered_(false)
	,is_mouse_l_down_(false)
	,is_mouse_l_release_(false)
	,is_selected_(false)
	,offset_(Vec3())
	,offset_scale_(Vec3(1.f,1.f,1.f))
{
}

GameWidget::GameWidget(const GameWidget& origin)
	:parent_widget_(nullptr)
	, owner_(nullptr)
	, is_hovered_(false)
	, is_mouse_l_down_(false)
	, is_mouse_l_release_(false)
	, is_selected_(false)
{
	mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"ui_mateiral");

	for (const auto& child : origin.child_widget_vector_)
	{
		child_widget_vector_.push_back(child);
	}
}

GameWidget::~GameWidget()
{
	Safe_Delete_Vec(child_widget_vector_);
}

void GameWidget::Update()
{
	for (const auto& child : child_widget_vector_)
	{
		child->Update();
	}
}

void GameWidget::Render()
{
	UpdateData();

	material_->UpdateData();
	mesh_->UpdateData();
	
	mesh_->Render();

	for (const auto& child : child_widget_vector_)
	{
		child->Render();
	}
}

void GameWidget::LateUpdate()
{
	CheckMouseHovered();
	for (const auto& child : child_widget_vector_)
	{
		child->LateUpdate();
	}
}

void GameWidget::FinalUpdate()
{
	Vec3 worldScale = owner_->Transform()->GetWorldScale();

	Matrix transformMatrix = XMMatrixTranslation(offset_.x / worldScale.x, offset_.y / worldScale.y, offset_.z / worldScale.z);
	Matrix scaleMatrix = XMMatrixScaling(offset_scale_.x, offset_scale_.y, offset_scale_.z);

	widget_world_matrix_ = scaleMatrix * transformMatrix * owner_->Transform()->GetWorldMatrix();

	for (const auto& child : child_widget_vector_)
	{
		child->FinalUpdate();
	}
}

void GameWidget::UpdateData()
{
	CConstBuffer* constantBuffer = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM);
	g_transform.world_matrix = widget_world_matrix_;
	g_transform.world_view_matrix = g_transform.world_matrix * g_transform.view_matrix;
	g_transform.world_view_projection_matrix = g_transform.world_view_matrix * g_transform.projection_matrix;

	constantBuffer->SetData(&g_transform, sizeof(Transform));
	constantBuffer->SetPipelineStage(PIPELINE_STAGE::PS_VERTEX);
	constantBuffer->UpdateData();

	for (const auto& child : child_widget_vector_)
	{
		child->UpdateData();
	}
}

void GameWidget::CheckMouseHovered()
{
	Vec3 ownerPos = owner_->Transform()->GetPosition();
	Vec3 finalPos = ownerPos + offset_;
	Vec3 ownerScale = owner_->Transform()->GetScale();
	Vec3 finalScale = ownerScale * offset_scale_;
	Vec3 lt = Vec3(finalPos.x-finalScale.x/2.f, finalPos.y-finalScale.y/2.f, finalPos.z);
	Vec2 mousePos = MOUSE_POS;
	if (lt.x <= mousePos.x && mousePos.x <= lt.x + finalScale.x && lt.y<= mousePos.y && mousePos.y <=lt.y+finalScale.y)
	{
		is_hovered_ = true;
	}
	else
	{
		is_hovered_ = false;
	}
}
