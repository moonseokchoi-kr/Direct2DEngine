#include "pch.h"
#include "ResourceWidget.h"

ResourceWidget::ResourceWidget(const string& widgetName, Vec2 uiSize)
	:Widget(widgetName)
	,size_(uiSize)
{
}

ResourceWidget::~ResourceWidget()
{
}

void ResourceWidget::Start()
{
	
	ImGui::BeginChild(GetName().c_str(), size_);

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 191, 124, 255));
	ImGui::Text(GetName().c_str());
	ImGui::PopStyleColor();
	ImGui::Spacing();
	
		
}

void ResourceWidget::End()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::EndChild();
}

