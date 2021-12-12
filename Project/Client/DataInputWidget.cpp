#include "pch.h"
#include "DataInputWidget.h"

#include "imgui.h"
#include "WidgetManager.h"

bool DataInputWidget::DataInputInt(wstring name, int* data)
{
    string usage = WStringToString(name);
    ImGui::Text(usage.c_str());
    ImGui::SameLine();

    usage = "##" + usage;

    if (ImGui::InputInt(usage.c_str(), data))
    {
        return true;
    }
    return false;
}

bool DataInputWidget::DataInputFloat(wstring name, float* data)
{
	string usage = WStringToString(name);
	ImGui::Text(usage.c_str());
	ImGui::SameLine();

	usage = "##" + usage;

	if (ImGui::InputFloat(usage.c_str(), data))
	{
		return true;
	}
    return false;
}

bool DataInputWidget::DataInputVec2(wstring name, Vec2& data)
{
	string usage = WStringToString(name);
	ImGui::Text(usage.c_str());
	ImGui::SameLine();

	usage = "##" + usage;

	if (ImGui::InputFloat2(usage.c_str(), data))
	{
		return true;
	}
    return false;
}

bool DataInputWidget::DataInputVec4(wstring name, Vec4& data)
{
	string usage = WStringToString(name);
	ImGui::Text(usage.c_str());
	ImGui::SameLine();

	usage = "##" + usage;

	if (ImGui::InputFloat4(usage.c_str(), data))
	{
		return true;
	}
	return false;
}
