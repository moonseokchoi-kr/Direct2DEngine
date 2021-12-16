#include "pch.h"
#include "ScriptWidget.h"
#include "DataInputWidget.h"

#include <Engine/CScript.h>

ScriptWidget::ScriptWidget()
	:ComponentWidget("ScriptWidget",Vec2(0,100))
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
	{
		const vector<ScriptParameter>& parameter_vector = target_script_->GetParameter();

		widget_height_ = (float)parameter_vector.size() * 20.f;

		for (const auto& parameter : parameter_vector)
		{
			switch (parameter.param)
			{
			case SCRIPT_PARAM::INT:
				DataInputWidget::DataInputInt(parameter.name, (int*)parameter.data);
				break;
			case SCRIPT_PARAM::FLOAT:
				DataInputWidget::DataInputFloat(parameter.name, (float*)parameter.data);
				break;
			case SCRIPT_PARAM::VEC2:
				DataInputWidget::DataInputVec2(parameter.name, (Vec2&)parameter.data);
				break;
			case SCRIPT_PARAM::VEC4:
				DataInputWidget::DataInputVec4(parameter.name, (Vec4&)parameter.data);
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
	}
	End();
}

void ScriptWidget::Start()
{
	string strScriptName = WStringToString(target_script_->GetName());


	ImGui::BeginChild(strScriptName.c_str(), ImVec2(0, widget_height_));

	widget_height_ = 0.f;

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,255,0,255));
	ImGui::Text(strScriptName.c_str());
	ImGui::PopStyleColor();

	widget_height_ += ImGui::GetItemRectSize().y;
}
