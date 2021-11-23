#include "pch.h"
#include "MeshRenderWidget.h"

MeshRenderWidget::MeshRenderWidget()
	:ComponentWidget("MeshRender", Vec2(0,0))
{
}

MeshRenderWidget::~MeshRenderWidget()
{
}

void MeshRenderWidget::Update()
{
	Start();
	{
		if (ImGui::BeginTable("material_spilt", 2, ImGuiTableFlags_Resizable| ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_RowBg, ImVec2(0, 0), 0.f))
		{
			ImGui::TableNextColumn();
			//Position
			ImGui::Text("Mesh");
			ImGui::TableNextColumn();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Scale

			ImGui::Text("Material");
			ImGui::TableNextColumn();


			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::EndTable();
		}
	}
	End();
}
