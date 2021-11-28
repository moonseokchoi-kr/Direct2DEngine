#include "pch.h"
#include "Collider2DWidget.h"

#include <Engine/CCollider2D.h>

Collider2DWidget::Collider2DWidget()
	:ComponentWidget("collider_widget",Vec2(0,100))
{
}

Collider2DWidget::~Collider2DWidget()
{
}

void Collider2DWidget::Init()
{
}

void Collider2DWidget::Update()
{
	Start();
	{
		CCollider2D* col = GetTarget()->Collider2D();
		Vec2 offsetPos = GetTarget()->Get
		

		ImGui::AlignTextToFramePadding();
		//ImVec2 widgetSize = ImGui::GetItemRectSize();
		if (ImGui::BeginTable("transform_spilt", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
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
