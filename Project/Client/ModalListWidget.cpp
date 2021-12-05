#include "pch.h"
#include "ModalListWidget.h"
#include <Engine/CKeyManager.h>

ModalListWidget::ModalListWidget()
	:Widget("modal_list_widget")
	, callbakc_func_(nullptr)
	, instance_(nullptr)
	, current_index_(0)
	, is_popup_open_(false)
	, edit_menu_(false)
{
}

ModalListWidget::~ModalListWidget()
{
}

void ModalListWidget::Init()
{
}

void ModalListWidget::Update()
{
	
	if (is_popup_open_)
	{
		ImGui::OpenPopup(caption_.c_str(), ImGuiPopupFlags_MouseButtonLeft);
		is_popup_open_ = true;
	}

	if (ImGui::BeginPopup(caption_.c_str(), ImGuiWindowFlags_Popup))
	{
		if (edit_menu_)
		{
			ShowEditMenu();
			ImGui::Spacing();
			ImGui::Separator();
		}

		ShowSelectedBox();
		ImGui::EndPopup();
	}
	else
	{
		Clear();
	}
}

void ModalListWidget::Clear()
{
	ImGui::CloseCurrentPopup();
	Deactivate();
	is_popup_open_ = false;
	item_name_vector_.clear();
}

void ModalListWidget::ShowEditMenu()
{
	
	ImGui::Text("Current Asset");
	ImGui::Spacing();
	ImGui::Separator();
	if (ImGui::MenuItem("Edit")) {}
	if (ImGui::MenuItem("Copy")) {}
	if (ImGui::MenuItem("Delete")) {}
	
}

void ModalListWidget::ShowSelectedBox()
{

	ImGui::Indent(-2);

	if (ImGui::BeginListBox("##resource_list_box", ImVec2(230, 7 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (size_t i = 0; i < item_name_vector_.size(); ++i)
		{
			const bool is_selected = (current_index_ == i);
			if (ImGui::Selectable(item_name_vector_[i].c_str(), is_selected))
			{
				current_index_ = (int)i;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsKeyDown(VK_RETURN))
			{
				selected_item_ = item_name_vector_[i];
				(instance_->*callbakc_func_)((DWORD_PTR)this, 0);
				Clear();
			}
		}
		ImGui::EndListBox();
	}
	ImGui::Unindent();

	ImGui::Indent();
	ImGui::Unindent();
}
