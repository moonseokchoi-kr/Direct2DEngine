#include "pch.h"
#include "CompoenetFilterPopupWidget.h"

#include <Script/CScriptManager.h>
#include <Engine/CGameObject.h>

CompoenetFilterPopupWidget::CompoenetFilterPopupWidget()
	:Widget("##component_filter")
	,target_object_(nullptr)
{
}

CompoenetFilterPopupWidget::~CompoenetFilterPopupWidget()
{
}

void CompoenetFilterPopupWidget::Init()
{

}

void CompoenetFilterPopupWidget::Update()
{
	if (ImGui::BeginPopup(GetName().c_str(), ImGuiWindowFlags_Popup))
	{
		const auto& compoenetNames = magic_enum::enum_names<COMPONENT_TYPE>();
		for (int i=0; i<compoenetNames.size()-2; ++i)
		{
			if(nullptr == target_object_->GetComponent((COMPONENT_TYPE)i))
				filter_widget_.AddFilterString(string(compoenetNames[i]));
		}
		vector<wstring> vec;
		CScriptManager::GetScriptInfo(vec);
		for (const auto& name : vec)
		{
			filter_widget_.AddFilterString(WStringToString(name));
		}
		filter_widget_.SetSelectEvent(this,(SELECT_EVENT)&CompoenetFilterPopupWidget::AddComponent);

		filter_widget_.Update();

		ImGui::EndPopup();

		filter_widget_.Clear();
	}
	else
	{
		Clear();
	}
}

void CompoenetFilterPopupWidget::AddComponent(string selectItem)
{
	if (selectItem.find("Script") != std::string::npos)
	{
		target_object_->AddComponent((CComponent*)(CScriptManager::GetScript(StringToWString(selectItem))));
	}
	else
	{
		auto type = magic_enum::enum_cast<COMPONENT_TYPE>(selectItem);
		target_object_->AddComponent(type.value());
	}
	Clear();
}

void CompoenetFilterPopupWidget::Clear()
{
	ImGui::CloseCurrentPopup();
	filter_widget_.Clear();
}
