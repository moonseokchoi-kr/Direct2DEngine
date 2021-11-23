#include "pch.h"
#include "TransformWidget.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

TransformWidget::TransformWidget()
	:ComponentWidget("Transform",Vec2(0,100))
{
}

TransformWidget::~TransformWidget()
{
}

void TransformWidget::Update()
{
	Start();
	{
		CTransform* tf = GetTarget()->Transform();
		Vec3 pos = tf->GetPosition();
		Vec3 scale = tf->GetScale();
		Vec3 rot = tf->GetRotation();

		ImGui::AlignTextToFramePadding();

		if (ImGui::BeginTable("transform_spilt", 2, ImGuiTableFlags_Resizable|ImGuiTableFlags_NoPadInnerX|ImGuiTableFlags_RowBg,ImVec2(0,0),0.f))
		{
			ImGui::TableNextColumn();
			//Position
			ImGui::Text("Position");
			ImGui::TableNextColumn();
			ImGui::DragFloat3("##InputPos", pos, 0.001f, 0.0f, 0.0f, "%.3f");
			
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Scale
			
			ImGui::Text("Scale");
			ImGui::TableNextColumn();
			ImGui::DragFloat3("##InputScale", scale, 0.001f, 0.0f, 0.0f, "%.3f");


			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Rotation
			ImGui::Text("Rotation");
			ImGui::TableNextColumn();
			ImGui::DragFloat3("##InputRot", rot, 0.001f, 0.0f, 0.0f, "%.3f");
			ImGui::EndTable();
		}


		tf->SetPosition(pos);
		tf->SetScale(scale);
		tf->SetRotation(rot);
	}
	End();
}
