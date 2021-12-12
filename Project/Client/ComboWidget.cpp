#include "pch.h"
#include "ComboWidget.h"

ComboWidget::ComboWidget()
	:Widget("##combo")
	,combo_data_{}
	,current_index_(0)
	,is_selected(false)
{
}

ComboWidget::~ComboWidget()
{
}

void ComboWidget::Init()
{
}

void ComboWidget::Update()
{
	if (ImGui::BeginCombo("##shader_combo", combo_data_[current_index_].c_str()))
	{
		for (UINT i = 0; i < (UINT)combo_data_.size(); ++i)
		{
			is_selected = (current_index_ == i);
			if (ImGui::Selectable(combo_data_[i].c_str(), is_selected))
			{
				current_index_ = i;
				
			}
			if(is_selected)
				ImGui::SetItemDefaultFocus();
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				selected_item_ = combo_data_[current_index_];
				(instance_->*callback_)((DWORD_PTR)this, 0);
				Clear();
			}
		}
		ImGui::EndCombo();
		Clear();
	}
}

void ComboWidget::Clear()
{
	combo_data_.clear();
}
