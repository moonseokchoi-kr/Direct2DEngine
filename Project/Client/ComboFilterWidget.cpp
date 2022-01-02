#include "pch.h"
#include "ComboFilterWidget.h"

ComboFilterWidget::ComboFilterWidget()
	:Widget("##combo_filter")
{
}

ComboFilterWidget::~ComboFilterWidget()
{
}

void ComboFilterWidget::Init()
{
}

void ComboFilterWidget::Update()
{
	static ImGuiTextFilter filter;
	filter.Draw();
	if (ImGui::BeginCombo("##combo",filter_string_vector_[current_index_].c_str()))
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
					if(nullptr != instance_)
						(instance_->*select_event_)(selected_item_);
					Clear();
				}
			}

		}
		ImGui::EndCombo();
	}
}

void ComboFilterWidget::Clear()
{
	filter_string_vector_.clear();
}
