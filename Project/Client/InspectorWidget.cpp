#include "pch.h"
#include "InspectorWidget.h"
#include "TransformWidget.h"
#include "MeshRenderWidget.h"
#include "AnimationWidget.h"
#include "Collider2DWidget.h"
#include "CameraWidget.h"
#include "TileMapWidget.h"

#include "TextureWidget.h"
#include "MaterialWidget.h"

#include <Engine/CGameObject.h>

InspectorWidget::InspectorWidget()
	:Widget("inspector_view")
	, target_object_(nullptr)
	, component_widget_array_{}
{
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::TRANSFORM)] = new TransformWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::MESHRENDER)] = new MeshRenderWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::ANIMATOR2D)] = new AnimationWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::COLLIDER2D)] = new Collider2DWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::CAMERA)] = new CameraWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::TILEMAP)] = new TileMapWidget;

	resource_widget_array_[static_cast<UINT>(RESOURCE_TYPE::TEXTURE)] = new TextureWidget;
	resource_widget_array_[static_cast<UINT>(RESOURCE_TYPE::MATERIAL)] = new MaterialWidget;
}
InspectorWidget::~InspectorWidget()
{
	Safe_Delete_Array(component_widget_array_);
}

void InspectorWidget::Update()
{
	ImGui::Begin(GetName().c_str(), &is_active_);

	if (nullptr != target_object_ || nullptr != target_resource_)
	{
		if (nullptr != target_object_)
			ShowObjectInfo();
		else if (nullptr != target_resource_)
			ShowResourceInfo();
	}


	UpdateChildren();

	ImGui::End();
}

void InspectorWidget::ShowObjectInfo()
{
	for (size_t i=0; i<component_widget_array_.size(); ++i)
	{
		if(nullptr == component_widget_array_[i] || nullptr == target_object_->GetComponent(static_cast<COMPONENT_TYPE>(i)))
			continue;
		component_widget_array_[i]->SetTarget(target_object_);
		component_widget_array_[i]->Update();
	}
}

void InspectorWidget::ShowResourceInfo()
{
	RESOURCE_TYPE type = target_resource_->GetResourceType();
	if (nullptr == resource_widget_array_[(UINT)type])
		return;

	resource_widget_array_[(UINT)type]->SetResource(target_resource_.Get());
	resource_widget_array_[(UINT)type]->Update();
}
