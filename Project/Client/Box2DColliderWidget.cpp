#include "pch.h"
#include "Box2DColliderWidget.h"
#include "DataInputWidget.h"


#include <Engine/CBox2DCollider.h>

Box2DColliderWidget::Box2DColliderWidget()
	:ComponentWidget("Box2D Collision",Vec2(0,140))
{
}

Box2DColliderWidget::~Box2DColliderWidget()
{
}

void Box2DColliderWidget::Update()
{
	if(ImGui::CollapsingHeader(GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		CBox2DCollider* box = GetTarget()->Box2DCollider();
		Vec2 offset = box->GetOffset();
		Vec2 offsetSize = box->GetOffsetSize();
		float density = box->GetDenisty();
		float friction = box->GetFriction();
		float restitution = box->GetRestitution();
		float restitutionThreshold = box->GetRestitutionThreshold();


		if (ImGui::BeginTable("##collision_data_table", 2, ImGuiTableFlags_Resizable))
		{
			ImGui::TableNextColumn();
			ImGui::Text("Offset");
			ImGui::TableNextColumn();
			if (DataInputWidget::DataDragInputVec2(L"##offset", offset))
			{
				box->SetOffset(offset);
			}

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Offset Size");
			ImGui::TableNextColumn();
			if (DataInputWidget::DataDragInputVec2(L"##offset_size", offsetSize, true))
			{
				if (0 == offsetSize.x)
				{
					offsetSize.x = 0.1f;
				}
				if (0 == offsetSize.y)
				{
					offsetSize.y = 0.1f;
				}
				box->SetOffsetSize(offsetSize);
			}


			ImGui::TableNextRow();


			ImGui::TableNextColumn();
			ImGui::Text("Density");
			ImGui::TableNextColumn();
			if (DataInputWidget::DataDragInputFloat(L"##density", &density,true))
			{
				box->SetDenisty(density);
			}


			ImGui::TableNextRow();


			ImGui::TableNextColumn();
			ImGui::Text("Friction");
			ImGui::TableNextColumn();
			if (DataInputWidget::DataDragInputFloat(L"##Friction", &friction, true))
			{
				box->SetDenisty(friction);
			}

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Restitution");
			ImGui::TableNextColumn();
			if (DataInputWidget::DataDragInputFloat(L"##Restitution", &restitution))
			{
				box->SetRestitution(restitution);
			}
			
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Restitution Threshold");
			ImGui::TableNextColumn();
			if (DataInputWidget::DataDragInputFloat(L"##Restitution Threshold", &restitutionThreshold,true))
			{
				box->SetRestitutionThreshold(restitutionThreshold);
			}

			
			ImGui::EndTable();
		}

	}
}
