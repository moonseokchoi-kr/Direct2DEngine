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
#include "ScriptWidget.h"

#include <Engine/CGameObject.h>

InspectorWidget::InspectorWidget()
	:Widget("inspector_view")
	, target_object_(nullptr)
	, component_widget_array_{}
	, script_widget_vector_{}
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
	Safe_Delete_Vec(script_widget_vector_);
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
	ShowSciprtInfo();
}

void InspectorWidget::ShowResourceInfo()
{
	RESOURCE_TYPE type = target_resource_->GetResourceType();
	if (nullptr == resource_widget_array_[(UINT)type])
		return;

	resource_widget_array_[(UINT)type]->SetResource(target_resource_.Get());
	resource_widget_array_[(UINT)type]->Update();
}

void InspectorWidget::ShowSciprtInfo()
{
	script_widget_vector_.clear();
	const vector<CScript*> script_vector = target_object_->GetScripts();
	
	for (const auto& script : script_vector)
	{
		ScriptWidget* widget = new ScriptWidget;
		widget->SetTarget(target_object_);
		widget->SetScript(script);
		script_widget_vector_.push_back(widget);
	}

	for (const auto& widget : script_widget_vector_)
	{
		widget->Update();
	}
}
