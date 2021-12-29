#include "pch.h"
#include "ScriptWidget.h"
#include "DataInputWidget.h"

#include <Engine/CScript.h>

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
		string strScriptName = WStringToString(target_script_->GetName());
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text(strScriptName.c_str());
		ImGui::PopStyleColor();

		const vector<ScriptParameter>& parameter_vector = target_script_->GetParameter();

		widget_height_ = (float)parameter_vector.size() * 20.f+100.f;

		if(ImGui::BeginTable("##script_parmeter",2,ImGuiTableFlags_Resizable))
		{
			for (const auto& parameter : parameter_vector)
			{
				switch (parameter.param)
				{
				case SCRIPT_PARAM::INT:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter.name).c_str());
					ImGui::TableNextColumn();
					DataInputWidget::DataInputInt(parameter.name, (int*)parameter.data);
					ImGui::TableNextRow();
				}
					break;
				case SCRIPT_PARAM::FLOAT:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter.name).c_str());
					ImGui::TableNextColumn();
					DataInputWidget::DataInputFloat(parameter.name, (float*)parameter.data);
					ImGui::TableNextRow();
				}
					break;
				case SCRIPT_PARAM::VEC2:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter.name).c_str());
					ImGui::TableNextColumn();
					DataInputWidget::DataInputVec2(parameter.name, (Vec2&)parameter.data);
					ImGui::TableNextRow();
				}
					break;
				case SCRIPT_PARAM::VEC4:
				{
					ImGui::TableNextColumn();
					ImGui::Text(WStringToString(parameter.name).c_str());
					ImGui::TableNextColumn();
					DataInputWidget::DataInputVec4(parameter.name, (Vec4&)parameter.data);
					ImGui::TableNextRow();
				}
					break;
				case SCRIPT_PARAM::MATRIX:
					break;
				case SCRIPT_PARAM::TEXTURE:
					break;
				case SCRIPT_PARAM::PREFAB:

					break;
				case SCRIPT_PARAM::END:
					break;
				default:
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
