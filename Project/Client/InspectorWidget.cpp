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
#include "ParticleWidget.h"
#include "RigidBody2DWidget.h"
#include "UIComponentWidget.h"
#include "Box2DColliderWidget.h"

#include <Engine/CGameObject.h>
#include <Engine/CKeyManager.h>
#include <Script/CScriptManager.h>


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
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::BOX2D_COLLIDER)] = new Box2DColliderWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::CAMERA)] = new CameraWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::TILEMAP)] = new TileMapWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::PARTICLESYSTEM)] = new ParticleWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::RIGIDBODY2D)] = new RigidBody2DWidget;
	component_widget_array_[static_cast<UINT>(COMPONENT_TYPE::UI)] = new UIComponentWidget;

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
		{
			ShowObjectInfo();

		}
			
		else if (nullptr != target_resource_)
			ShowResourceInfo();
	}


	UpdateChildren();

	ImGui::End();
}



void InspectorWidget::ShowObjectInfo()
{
	string name = WStringToString(target_object_->GetName());
	if (ImGui::InputText("##targetName", name.data(), (int)name.size()))
	{
		if (KEY_TAP(KEY::ENTER))
			target_object_->SetName(StringToWString(name));
	}

	for (size_t i=0; i<component_widget_array_.size(); ++i)
	{
		if(nullptr == component_widget_array_[i] || nullptr == target_object_->GetComponent(static_cast<COMPONENT_TYPE>(i)))
			continue;
		component_widget_array_[i]->SetTarget(target_object_);
		component_widget_array_[i]->Update();
	}


	ShowSciprtInfo();

	ShowAddComponentButton();

}

void InspectorWidget::ShowAddComponentButton()
{
	ImGui::Spacing();

	ImGui::Indent(80);
	ImGui::Unindent();

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(180,248,201,255));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(161,231,229,255));
	if (ImGui::Button("Add Component", ImVec2(200, 0)))
	{
		ImGui::OpenPopup(component_filter_widget_.GetName().c_str());
		component_filter_widget_.SetInstance(target_object_);
	}
	component_filter_widget_.Update();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopID();
	ImGui::Indent(80);
	ImGui::Unindent();
}

void InspectorWidget::ShowResourceInfo()
{
	string name = WStringToString(target_resource_->GetKey());
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50, 255, 210, 255));
	ImGui::Text(name.c_str());
	ImGui::PopStyleColor();

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
