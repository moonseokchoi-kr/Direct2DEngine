#include "pch.h"
#include "MeshRenderWidget.h"

#include <Engine/CMeshRender.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>
MeshRenderWidget::MeshRenderWidget()
	:ComponentWidget("MeshRender", Vec2(0.f,150.f))
{
}

MeshRenderWidget::~MeshRenderWidget()
{
}

void MeshRenderWidget::Update()
{
	CMeshRender* meshRender = GetTarget()->MeshRender();
	Ptr<CMesh> mesh = meshRender->GetMesh();
	Ptr<CMaterial> material = meshRender->GetMaterial();

	Start();
	{
		if (ImGui::BeginTable("material_spilt", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg, ImVec2(0, 0), 0.f))
		{
			ImGui::TableNextColumn();
			//Position
			ImGui::Text("Mesh");
			ImGui::TableNextColumn();
			
			char str[255] = { 0, };
			WideCharToMultiByte(CP_ACP, 0, mesh->GetName().c_str(), -1, str, (int)mesh->GetName().size(), nullptr, nullptr);
			ImGui::PushID(0);
			ImGui::PushItemWidth(100);
			ImGui::Button(str);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Scale

			ImGui::Text("Material");
			ImGui::TableNextColumn();

			WideCharToMultiByte(CP_ACP, 0, material->GetName().c_str(), -1, str, (int)material->GetName().size(), nullptr, nullptr);
			ImGui::PushID(1);
			ImGui::PushItemWidth(100);
			ImGui::Button(str);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::EndTable();
		}
	}
	End();
}
