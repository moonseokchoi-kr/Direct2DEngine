#include "pch.h"
#include "DataInputWidget.h"

#include "imgui.h"
#include "WidgetManager.h"
#include "ComboWidget.h"

bool DataInputWidget::DataInputInt(wstring name, int* data)
{
	string usage = WStringToString(name);

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

	usage = "##" + usage;

	if (ImGui::InputFloat2(usage.c_str(), data))
	{
		return true;
	}
	return false;
}

bool DataInputWidget::DataInputVec3(wstring name, Vec3& data)
{
	string usage = WStringToString(name);

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

	usage = "##" + usage;

	if (ImGui::InputFloat4(usage.c_str(), data))
	{
		return true;
	}
	return false;
}

#include <Engine/CResourceManager.h>

void DataInputWidget::DataInputTexture(wstring name, Ptr<CTexture> texture, DWORD_PTR second, COMBO_CALLBACK callback, Widget* inst)
{
	string usage = WStringToString(name);
	ImGui::Text(usage.c_str());
	usage = "##" + usage;

	//Combo
	const unordered_map<wstring, CResource*>& shader_map = CResourceManager::GetInst()->GetResource<CTexture>();
	//combo widget√ﬂ∞°
	ComboWidget* widget = dynamic_cast<ComboWidget*>(WidgetManager::GetInst()->FindWidget("##sub_combo"));
	widget->SetCallback(inst, (COMBO_CALLBACK)callback);
	int count = 0;
	for (const auto& pair : shader_map)
	{
		widget->AddComboData(WStringToString(pair.first));
		if (nullptr != texture && pair.first == texture->GetName())
			widget->SetCurrentIndex(count);
		++count;
	}
	widget->Update();

	if (nullptr == texture)
	{
		ImGui::Image(nullptr, ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.4f, 0.4f, 0.4f, 1.f));
	}
	else
	{
		ImGui::Image(texture->GetShaderResourceView(), ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.4f, 0.4f, 0.4f, 1.f));
	}
	widget->SetSecondData(second);

}

bool DataInputWidget::DataDragInputInt(wstring name, int* data, bool is_normal)
{
	string usage = WStringToString(name);

	usage = "##" + usage;

	if (ImGui::DragInt(usage.c_str(), data))
	{
		return true;
	}
	return false;
}

bool DataInputWidget::DataDragInputFloat(wstring name, float* data, bool is_normal)
{
	string usage = WStringToString(name);

	usage = "##" + usage;
	if (is_normal)
	{
		if (ImGui::DragFloat(usage.c_str(), data, 0.01f, 0.0f, 1.0f))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::DragFloat(usage.c_str(), data))
		{
			return true;
		}
	}

	return false;
}

bool DataInputWidget::DataDragInputVec2(wstring name, Vec2& data , bool is_normal)
{
	string usage = WStringToString(name);

	usage = "##" + usage;


	if (is_normal)
	{
		if (ImGui::DragFloat2(usage.c_str(), data, 0.01f, 0.0f, 1.0f))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::DragFloat2(usage.c_str(), data))
		{
			return true;
		}
	}

	return false;
}

bool DataInputWidget::DataDragInputVec3(wstring name, Vec3& data, bool is_normal)
{
	string usage = WStringToString(name);

	usage = "##" + usage;
	if (is_normal)
	{
		if (ImGui::DragFloat3(usage.c_str(), data, 0.01f, 0.0f, 1.0f))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::DragFloat3(usage.c_str(), data))
		{
			return true;
		}
	}

	return false;
}

bool DataInputWidget::DataDragInputVec4(wstring name, Vec4& data, bool is_normal)
{
	string usage = WStringToString(name);

	usage = "##" + usage;
	if (is_normal)
	{
		if (ImGui::DragFloat4(usage.c_str(), data, 0.01f, 0.0f, 1.0f))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::DragFloat4(usage.c_str(), data))
		{
			return true;
		}
	}

	return false;
}
