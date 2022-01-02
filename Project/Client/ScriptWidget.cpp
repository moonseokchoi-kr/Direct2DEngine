#include "pch.h"
#include "ScriptWidget.h"
#include "DataInputWidget.h"

#include <Engine/CScript.h>
#include <Script/CScriptManager.h>

ScriptWidget::ScriptWidget()
	:ComponentWidget("ScriptWidget",Vec2(0,140))
	,target_script_(nullptr)
	,widget_height_(0)
{
}

ScriptWidget::~ScriptWidget()
{
}

void ScriptWidget::Update()
{

	Start();
	if (ImGui::CollapsingHeader(GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		string strScriptName = WStringToString(CScriptManager::GetScriptName(target_script_));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text(strScriptName.c_str());
		ImGui::PopStyleColor();

		const vector<ScriptParameter>& parameter_vector = target_script_->GetParameter();

		widget_height_ = (float)parameter_vector.size() * 20.f+100.f;
		ScriptParameter param = {};
		if(ImGui::BeginTable("##script_parmeter",2,ImGuiTableFlags_Resizable))
		{
			for (int i=0; i<(UINT)parameter_vector.size(); ++i)
			{
				switch (parameter_vector[i].param)
				{
				case SCRIPT_PARAM::INT:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter_vector[i].name).c_str());
					ImGui::TableNextColumn();
					DataInputWidget::DataInputInt(parameter_vector[i].name, (int*)parameter_vector[i].data);
					ImGui::TableNextRow();
				}
				break;
				case SCRIPT_PARAM::FLOAT:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter_vector[i].name).c_str());
					ImGui::TableNextColumn();
					DataInputWidget::DataInputFloat(parameter_vector[i].name, (float*)parameter_vector[i].data);
					ImGui::TableNextRow();
				}
				break;
				case SCRIPT_PARAM::VEC2:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter_vector[i].name).c_str());
					ImGui::TableNextColumn();
					DataInputWidget::DataInputVec2(parameter_vector[i].name, (Vec2&)parameter_vector[i].data);
					ImGui::TableNextRow();
				}
				break;
				case SCRIPT_PARAM::VEC4:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter_vector[i].name).c_str());
					ImGui::TableNextColumn();
					DataInputWidget::DataInputVec4(parameter_vector[i].name, (Vec4&)parameter_vector[i].data);
					ImGui::TableNextRow();
				}
				break;
				case SCRIPT_PARAM::TEXTURE:
					break;
				case SCRIPT_PARAM::PREFAB:
					break;
				case SCRIPT_PARAM::STRING:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter_vector[i].name).c_str());
					ImGui::TableNextColumn();
					wstring& data = *static_cast<wstring*>(parameter_vector[i].data);
					object_widget_.SetTargetObjectName(data);
					object_widget_.Update();
					if(!object_widget_.GetSelectedName().empty())
						data = object_widget_.GetSelectedName();
					param = parameter_vector[i];
					param.data = &data;

					target_script_->SetParameter(i, param);

					ImGui::TableNextRow();
				}
					break;
				}
			}
			ImGui::EndTable();
		}
	}
	End();
}

void ScriptWidget::Start()
{
	string strScriptName = WStringToString(target_script_->GetName());

	const vector<ScriptParameter>& parameter_vector = target_script_->GetParameter();

	widget_height_ = (float)parameter_vector.size() * 20.f + 100.f;

	ImGui::BeginChild(strScriptName.c_str(), ImVec2(0, widget_height_));

}
