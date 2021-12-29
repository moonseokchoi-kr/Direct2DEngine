#include "pch.h"
#include "UIComponentWidget.h"

UIComponentWidget::UIComponentWidget()
	:ComponentWidget("UI", Vec2(0, 100))
{
}

UIComponentWidget::~UIComponentWidget()
{
}

void UIComponentWidget::Update()
{
	Start();
	if (ImGui::CollapsingHeader(GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
	End();
}
