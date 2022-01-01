#include "pch.h"
#include "CameraWidget.h"
#include "imgui_internal.h"

#include <Engine/CCamera.h>
CameraWidget::CameraWidget()
	:ComponentWidget("Camera",Vec2(0,240))
{
}

CameraWidget::~CameraWidget()
{
}

void CameraWidget::Init()
{

}

void CameraWidget::Update()
{
	CCamera* camera = GetTarget()->Camera();
	PROJECTION_TYPE proj = camera->GetProjectionType();
	current_index_ = (UINT)proj;
	static array<const char*, (UINT)PROJECTION_TYPE::END> proj_array = { "Perspective","Orthogonal"};
	const char* preview_value = proj_array[current_index_];
	float farZ = camera->GetFarZ();
	float aov = camera->GetAovY();
	float scale = camera->GetScale();
	bool disabled = current_index_ == 0;
	UINT layerCheck = camera->GetCheckLayer();

	ImGui::AlignTextToFramePadding();
	//ImVec2 widgetSize = ImGui::GetItemRectSize();
	Start();
	if(ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginTable("transform_spilt", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
		{
			ImGui::TableNextColumn();
			ImGui::Text("Mesh");
			ImGui::TableNextColumn();

			if (ImGui::BeginCombo("##perspective_combo", preview_value))
			{
				for (int i = 0; i < proj_array.size(); ++i)
				{
					const bool is_selected = (current_index_ == i);
					if (ImGui::Selectable(proj_array[i], is_selected))
					{
						current_index_ = i;
						camera->SetProjectionType((PROJECTION_TYPE)current_index_);
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			//FarZ

			ImGui::Text("Far Z");
			ImGui::TableNextColumn();

			if (!disabled)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			if (ImGui::InputFloat("##input_far_z", &farZ))
			{
				if (farZ > 0.f)
					camera->SetFarZ(farZ);
			}

			if (!disabled)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}


			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// AOV

			ImGui::Text("Angle of View");
			ImGui::TableNextColumn();


			if (!disabled)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			if (ImGui::InputFloat("##input_aov", &aov))
			{
				if (aov > 0.f)
					camera->SetAovY(aov);
			}

			if (!disabled)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}



			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("Scale");
			ImGui::TableNextColumn();

			if (disabled)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			if (ImGui::InputFloat("##input_scale", &scale))
			{
				if (scale > 0.f)
					camera->SetScale(scale);
			}

			if (disabled)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Check Layer");
			ImGui::TableNextColumn();
			for (UINT i = 0; i < MAX_LAYER; ++i)
			{
				string name = std::to_string(i);
				ImGui::Text(name.c_str());
				ImGui::SameLine();
				string label = "##" + name;
				bool check = layerCheck & (1 << i);
				if (ImGui::Checkbox(label.c_str(), &check))
				{
					if (check)
					{
						camera->CheckLayer(i);
					}
					else
					{
						camera->UnCheckLayer(i);
					}
				}
				if(i%6<5)
					ImGui::SameLine();
			}


			ImGui::EndTable();
		}
	}
	End();

}
