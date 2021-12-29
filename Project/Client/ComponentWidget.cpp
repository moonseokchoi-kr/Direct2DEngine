#include "pch.h"
#include "ComponentWidget.h"

#include <Engine/CGameObject.h>

ComponentWidget::ComponentWidget(const string& name, Vec2 size)
	:Widget(name)
	,target_object_(nullptr)
	,widget_size_(size)
{
}

ComponentWidget::~ComponentWidget()
{
}

void ComponentWidget::Start()
{
	ImGui::BeginChild(("##"+GetName()).c_str(), widget_size_);
}

void ComponentWidget::End()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::EndChild();
}
