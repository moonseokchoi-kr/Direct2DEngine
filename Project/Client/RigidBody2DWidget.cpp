#include "pch.h"
#include "RigidBody2DWidget.h"
#include "DataInputWidget.h"

#include <Engine/CRigidBody2D.h>

RigidBody2DWidget::RigidBody2DWidget()
	:ComponentWidget("RigidBody2D",Vec2(0,140))
{
}

RigidBody2DWidget::~RigidBody2DWidget()
{
}

void RigidBody2DWidget::Update()
{
	Start();
	if (ImGui::CollapsingHeader(GetName().c_str(),ImGuiTreeNodeFlags_DefaultOpen))
	{
		CRigidBody2D* rigidBody = GetTarget()->RigidBody2D();
		bool active = rigidBody->IsActive();
		float mass = rigidBody->GetMass();
		float max_velocity_ = rigidBody->GetMaxVelocity();
		float friction = rigidBody->GetFriction();
		if (ImGui::BeginTable("##rigidbody_table",2, ImGuiTableFlags_Resizable))
		{
			ImGui::TableNextColumn();
			ImGui::Text("Active");
			ImGui::TableNextColumn();

			if (ImGui::Checkbox("##active", &active))
			{
				rigidBody->SetActive(active);
			}

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("mass");
			ImGui::TableNextColumn();

			if (DataInputWidget::DataInputFloat(L"mass", &mass))
			{
				rigidBody->SetMass(mass);
			}

			ImGui::TableNextRow();


			ImGui::TableNextColumn();
			ImGui::Text("Max Velocity");
			ImGui::TableNextColumn();

			if (DataInputWidget::DataInputFloat(L"Max Velocity", &max_velocity_))
			{
				rigidBody->SetMaxVelocity(max_velocity_);
			}

			ImGui::TableNextRow();


			ImGui::TableNextColumn();
			ImGui::Text("Friction");
			ImGui::TableNextColumn();

			if (DataInputWidget::DataInputFloat(L"Friction", &friction))
			{
				rigidBody->SetFriction(friction);
			}

			ImGui::EndTable();
			
		}
	}
	End();
}
