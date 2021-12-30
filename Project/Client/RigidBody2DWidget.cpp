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
		bool fixedRotation = rigidBody->IsFixedRotation();
		BODY_TYPE type = rigidBody->GetBodyType();
		if (ImGui::BeginTable("##rigidbody_table",2, ImGuiTableFlags_Resizable))
		{
			ImGui::TableNextColumn();
			ImGui::Text("Fixed Rotation");
			ImGui::TableNextColumn();

			if (ImGui::Checkbox("##fixed_rotation", &fixedRotation))
			{
				rigidBody->SetFixedRotation(fixedRotation);
			}

			ImGui::TableNextColumn();
			ImGui::Text("RigidBody Type");
			ImGui::TableNextColumn();

			const auto& bodyMap = magic_enum::enum_entries<BODY_TYPE>();
			int count = 0;
			rigid_combo_.SetCallback(this, (COMBO_CALLBACK)&RigidBody2DWidget::ChangeType);
			for (const auto& pair : bodyMap)
			{
				rigid_combo_.AddComboData(string(pair.second));
				if (pair.first == type)
					rigid_combo_.SetCurrentIndex(count);
				++count; 
			}
			rigid_combo_.Update();

			ImGui::EndTable();
			
		}
	}
	End();
}

void RigidBody2DWidget::ChangeType(DWORD_PTR instance, DWORD_PTR data)
{
	ComboWidget* widget = (ComboWidget*)instance;
	string str = widget->GetSelectedItem();
	auto type = magic_enum::enum_cast<BODY_TYPE>(str);
	CRigidBody2D* rigidBody = GetTarget()->RigidBody2D();
	rigidBody->SetBodyType(type.value());
}
