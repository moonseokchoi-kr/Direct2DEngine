#include "pch.h"
#include "FilterWidget.h"


FilterWidget::FilterWidget()
	:Widget("##filter")
	,is_popup_open_(false)
	,current_index_(0)
	,select_event_(nullptr)
{
}

FilterWidget::~FilterWidget()
{
}

void FilterWidget::Init()
{
}

void FilterWidget::Update()
{
	ShowSelectableList();
}

void FilterWidget::ShowSelectableList()
{
	static ImGuiTextFilter filter;
	ImGui::Indent(-2);
	filter.Draw();
	if (ImGui::BeginListBox("##list", ImVec2(230, 7 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (size_t i = 0; i < filter_string_vector_.size(); ++i)
		{
			if (filter.PassFilter(filter_string_vector_[i].c_str()))
			{
				const bool is_selected = (current_index_ == i);
				if (ImGui::Selectable(filter_string_vector_[i].c_str(), is_selected))
				{
					current_index_ = (int)i;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsKeyDown(VK_RETURN))
				{
					selected_item_ = filter_string_vector_[i];
					(instance_->*select_event_)(selected_item_);
					Clear();
				}
			}

		}
		ImGui::EndListBox();
	}
	ImGui::Unindent();

	ImGui::Indent();
	ImGui::Unindent();
}

void FilterWidget::Clear()
{
	filter_string_vector_.clear();
}
