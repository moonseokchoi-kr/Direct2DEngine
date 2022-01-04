#include "pch.h"
#include "CollisionManagerView.h"
#include <Engine/CCollisionManager.h>

CollisionManagerView::CollisionManagerView()
	:Widget("collision_manager_view")
{
}

CollisionManagerView::~CollisionManagerView()
{
}

void CollisionManagerView::Init()
{
}

void CollisionManagerView::Update()
{
	ImGui::SetNextWindowContentSize(ImVec2(1000, 0));
	if (ImGui::Begin(GetName().c_str(), &is_active_, ImGuiWindowFlags_HorizontalScrollbar))
	{

		if (ImGui::CollapsingHeader("Check Collision Layer"))
		{
			//layer check box 구성
			//역계단 형태
			ImGui::Indent(20);
			if (ImGui::BeginTable("##collision_table", 33, ImGuiTableFlags_SizingFixedSame))
			{
				ImGui::TableNextRow();
				for (int i = 0; i < MAX_LAYER; ++i)
				{
					ImGui::TableSetColumnIndex(i+1);
					ImGui::Text(std::to_string(i).c_str());
				}

				int count = 0;
				string label = "";
				for (int row = 0; row < MAX_LAYER; ++row)
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(std::to_string(row).c_str());
				
					int column = 0;
					for (UINT col = row; col < MAX_LAYER; ++col)
					{
						++count;
						bool b = CCollisionManager::GetInst()->GetCollisionMask(col) & (1 << row);
						label = std::to_string(count);
						label = "##" + label;
						ImGui::TableSetColumnIndex(col+1);
						if (ImGui::Checkbox(label.c_str(), &b))
						{
							CCollisionManager::GetInst()->CheckLayer(row, col);
						}
						++column;
					}
					ImGui::Spacing();
				}
				ImGui::EndTable();
			}

			ImGui::Unindent(20);
			ImGui::Spacing();

		}
		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}
