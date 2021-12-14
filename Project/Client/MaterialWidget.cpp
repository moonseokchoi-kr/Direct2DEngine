#include "pch.h"
#include "MaterialWidget.h"
#include "ComboWidget.h"
#include "WidgetManager.h"
#include "DataInputWidget.h"

#include <Engine/CGraphicsShader.h>
#include <Engine/CResourceManager.h>
#include <Engine/CResourceManager.h>


MaterialWidget::MaterialWidget()
	:ResourceWidget("Material Detail",Vec2(0,0))
{
}

MaterialWidget::~MaterialWidget()
{
}

void MaterialWidget::Update()
{
	Start();
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,50,50,255));
		ImGui::Text("Material");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::Text(WStringToString(target_resource_->GetName()).c_str());
		ShowMaterialDetail();


	}
	End();
}

void MaterialWidget::ShowMaterialDetail()
{
	if (ImGui::BeginTable("##MaterialDetail",2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextColumn();
		ImGui::Text("Shader");
		
		ImGui::TableNextColumn();
		//shader data

		const unordered_map<wstring, CResource*>& shader_map = CResourceManager::GetInst()->GetResource<CGraphicsShader>();
		//combo widget√ﬂ∞°
		ComboWidget* widget = dynamic_cast<ComboWidget*>(WidgetManager::GetInst()->FindWidget("combo"));
		widget->SetCallback(this, (COMBO_CALLBACK)&MaterialWidget::ChangeShader);
		widget->SetComboLabel("##shader_combo");
		int count = 0;
		for (const auto& pair : shader_map)
		{
			widget->AddComboData(WStringToString(pair.first));
			if (nullptr != target_resource_ && pair.first == target_resource_->GetName())
				widget->SetCurrentIndex(count);
			++count;
		}
		widget->Update();

		ImGui::EndTable();
	}
	ImGui::Spacing();
	ImGui::Separator();

	ShowOutputShaderParam();
}

void MaterialWidget::ShowOutputShaderParam()
{
	Ptr<CGraphicsShader> shader = target_resource_->GetShader();
	if (nullptr == shader)
		return;
	const vector<ShaderParameter>& param_vector = shader->GetShaderParam();

	for (const auto& param : param_vector)
	{
		switch (param.type)
		{
		case SHADER_PARAM::INT_0:
		case SHADER_PARAM::INT_1:
		case SHADER_PARAM::INT_2:
		case SHADER_PARAM::INT_3:
		{
			int data = 0;
			target_resource_->GetData(param.type, &data);
			if (DataInputWidget::DataInputInt(param.usage, &data))
			{
				target_resource_->SetData(param.type, &data);
			}
		}
			break;
		case SHADER_PARAM::FLOAT_0:
		case SHADER_PARAM::FLOAT_1:
		case SHADER_PARAM::FLOAT_2:
		case SHADER_PARAM::FLOAT_3:
		{
			float data = 0;
			target_resource_->GetData(param.type, &data);
			if (DataInputWidget::DataInputFloat(param.usage, &data))
			{
				target_resource_->SetData(param.type, &data);
			}
		}
			break;
		case SHADER_PARAM::VEC2_0:
		case SHADER_PARAM::VEC2_1:
		case SHADER_PARAM::VEC2_2:
		case SHADER_PARAM::VEC2_3:
		{
			Vec2 data;
			target_resource_->GetData(param.type, &data);
			if (DataInputWidget::DataInputVec2(param.usage, data))
			{
				target_resource_->SetData(param.type, &data);
			}
		}
			break;
		case SHADER_PARAM::VEC4_0:
		case SHADER_PARAM::VEC4_1:
		case SHADER_PARAM::VEC4_2:
		case SHADER_PARAM::VEC4_3:
		{
			Vec4 data;
			target_resource_->GetData(param.type, &data);
			if (DataInputWidget::DataInputVec4(param.usage, data))
			{
				target_resource_->SetData(param.type, &data);
			}
		}
			break;
		case SHADER_PARAM::MAT_0:
		case SHADER_PARAM::MAT_1:
		case SHADER_PARAM::MAT_2:
		case SHADER_PARAM::MAT_3:
			break;
		case SHADER_PARAM::TEX_0:
		case SHADER_PARAM::TEX_1:
		case SHADER_PARAM::TEX_2:
		case SHADER_PARAM::TEX_3:
		{
			Ptr<CTexture> data;
			target_resource_->GetData(param.type, &data);
			DataInputWidget::DataInputTexture(param.usage, data, (DWORD_PTR)param.type, (COMBO_CALLBACK)&MaterialWidget::ChangeTexture, this);

		}
			break;
		case SHADER_PARAM::TEX_ARR_0:
		case SHADER_PARAM::TEX_ARR_1:
		case SHADER_PARAM::TEX_CUBE_0:
		case SHADER_PARAM::TEX_CUBE_1:

		default:
			break;
		}
	}
}

void MaterialWidget::ChangeShader(DWORD_PTR instance, DWORD_PTR paramType)
{
	ComboWidget* widget = reinterpret_cast<ComboWidget*>(instance);
	string selectName = widget->GetSelectedItem();
	target_resource_->SetShader(CResourceManager::GetInst()->FindRes<CGraphicsShader>(StringToWString(selectName)));
}

void MaterialWidget::ChangeTexture(DWORD_PTR instance, DWORD_PTR paramType)
{
	ComboWidget* widget = reinterpret_cast<ComboWidget*>(instance);
	wstring selectName = StringToWString(widget->GetSelectedItem());
	Ptr<CTexture> tex = CResourceManager::GetInst()->FindRes<CTexture>(selectName);
	target_resource_->SetData((SHADER_PARAM)paramType, tex.Get());
}
