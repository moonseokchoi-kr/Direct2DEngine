#include "pch.h"
#include "TransformWidget.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

TransformWidget::TransformWidget()
	:ComponentWidget("Transform",Vec2(0,120))
{
}

TransformWidget::~TransformWidget()
{
}

void TransformWidget::Update()
{
	Start();
	if(ImGui::CollapsingHeader(GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		CTransform* tf = GetTarget()->Transform();
		Vec3 pos = tf->GetPosition();
		Vec3 scale = tf->GetScale();
		Vec3 rot = tf->GetRotation();

		ImGui::AlignTextToFramePadding();
		//ImVec2 widgetSize = ImGui::GetItemRectSize();
		if (ImGui::BeginTable("transform_spilt", 2,ImGuiTableFlags_Resizable|ImGuiTableFlags_RowBg))
		{
			ImGui::TableNextColumn();
			//Position
			ImGui::Text("Position");
			ImGui::TableNextColumn();
			//ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - ImGui::GetItemRectSize().x*2.f);
			//ImGui::PushItemWidth(-100);
			pos = CreateControlPannel("##input_pos", pos);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Scale
			ImGui::Indent();
			ImGui::Text("Scale");
			ImGui::Unindent();
			ImGui::TableNextColumn();
			scale = CreateControlPannel("##input_scale", scale);


			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Rotation
			ImGui::Indent(40);
			ImGui::Text("Rotation");
			ImGui::Unindent();
			ImGui::TableNextColumn();
			rot = CreateControlPannel("##input_rotation", rot);
			
			ImGui::EndTable();
		}


		tf->SetPosition(pos);
		tf->SetScale(scale);
		tf->SetRotation(rot);
	}
	End();
}

Vec3 TransformWidget::CreateControlPannel(const string& label, Vec3 value)
{	
	string lb = label;

	ImGui::Indent(1.f);
	ImGui::Text("X");
	ImGui::SameLine(0);
	ImGui::PushItemWidth(80);
	ImGui::DragFloat((lb+"x").c_str(), &value.x, 0.001f, 0.f, 0.f, "%.3f");
	ImGui::PopItemWidth();
	ImGui::Unindent();

	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine(0);
	ImGui::PushItemWidth(80);
	ImGui::DragFloat((lb + "y").c_str(), &value.y, 0.001f, 0.f, 0.f, "%.3f");
	ImGui::PopItemWidth();
	
	ImGui::SameLine();
	ImGui::Text("Z");
	ImGui::SameLine(0);
	ImGui::PushItemWidth(80);
	ImGui::DragFloat((lb + "z").c_str(), &value.z, 0.001f, 0.f, 0.f, "%.3f");
	ImGui::PopItemWidth();

	return value;
	
}
