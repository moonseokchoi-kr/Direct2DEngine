#include "pch.h"
#include "TextureWidget.h"

TextureWidget::TextureWidget()
	:ResourceWidget("Texture Detail",Vec2(0,0))
{
}

TextureWidget::~TextureWidget()
{
}

void TextureWidget::Update()
{
	Start();
	if(ImGui::CollapsingHeader(GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(0.4f,0.4f,0.4f,1.0f));
		ImGui::Text("File Name");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::Text(WStringToString(target_texture_->GetName()).c_str());
		ImGui::Spacing();
		ImGui::Text("%d x %d", (int)target_texture_->GetWidth(), (int)target_texture_->GetHeight());
		ImGui::Spacing();
		ImVec2 canvas_lt = ImGui::GetCursorScreenPos();
		ImVec2 canvas_size = ImGui::GetContentRegionAvail();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(canvas_lt, ImVec2(canvas_lt.x + canvas_size.x*0.8f,canvas_lt.y+canvas_size.y*0.4f), IM_COL32(50, 50, 50, 255));
		ImGui::Image(target_texture_->GetShaderResourceView(), ImVec2(canvas_size.x * 0.8f, canvas_size.y * 0.4f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.4f,0.4f,0.4f,1.0f));
	}
	End();
}
