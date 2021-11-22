#include "pch.h"
#include "TransformWidget.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

TransformWidget::TransformWidget()
	:ComponentWidget("Transform",Vec2(0,150))
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

		// position
		ImGui::Text("Position");
		ImGui::SameLine(80);

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputPosX", &pos.x, 0.001f, 0.f, 0.f, "%.3f");

		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputPosY", &pos.y, 0.001f, 0.f, 0.f, "%.3f");

		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputPosZ", &pos.z, 0.001f, 0.f, 0.f, "%.3f");

		// scale
		ImGui::Text("Scale");
		ImGui::SameLine(80);

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputScaleX", &scale.x, 0.001f, 0.f, 0.f, "%.3f");

		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputScaleY", &scale.y, 0.001f, 0.f, 0.f, "%.3f");

		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputScaleZ", &scale.z, 0.001f, 0.f, 0.f, "%.3f");

		// rot
		ImGui::Text("Rotation");
		ImGui::SameLine(80);

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputRotX", &rot.x, 0.001f, 0.f, 0.f, "%.3f");

		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputRotY", &rot.y, 0.001f, 0.f, 0.f, "%.3f");

		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##InputRotZ", &rot.z, 0.001f, 0.f, 0.f, "%.3f");

		ImGui::Spacing();
		ImGui::Separator();

		tf->SetPosition(pos);
		tf->SetScale(scale);
		tf->SetRotation(rot);
	}
	End();
}
