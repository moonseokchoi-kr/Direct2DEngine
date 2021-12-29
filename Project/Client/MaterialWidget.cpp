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
	if(ImGui::CollapsingHeader(GetName().c_str(),ImGuiTreeNodeFlags_DefaultOpen))
	{
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
			if (nullptr != target_resource_ && pair.first == target_resource_->GetShader()->GetName())
				widget->SetCurrentIndex(count);
			++count;
		}
		widget->Update();

		//Drop item
		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload("resource_view_tree"))
			{
				DWORD_PTR data = *((DWORD_PTR*)ImGui::GetDragDropPayload()->Data);
				CResource* resource = reinterpret_cast<CResource*>(data);
				if (resource->GetResourceType() == RESOURCE_TYPE::GRAPHIC_SHADER)
				{
					target_resource_->SetShader(dynamic_cast<CGraphicsShader*>(resource));
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::EndTable();
	}
	ImGui::Spacing();
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Shader Parameter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ShowOutputShaderParam();
	}
	
}

void MaterialWidget::ShowOutputShaderParam()
{
	Ptr<CGraphicsShader> shader = target_resource_->GetShader();
	if (nullptr == shader)
		return;
	const vector<ShaderParameter>& param_vector = shader->GetShaderParam();
	if (ImGui::BeginTable("##shader_table", 2, ImGuiTableFlags_Resizable))
	{
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
				ImGui::TableNextColumn();
				ImGui::Text(WStringToString(param.usage).c_str());
				ImGui::TableNextColumn();
				if (DataInputWidget::DataInputInt(param.usage, &data))
				{
					target_resource_->SetData(param.type, &data);
				}
				ImGui::TableNextRow();
			}
			break;
			case SHADER_PARAM::FLOAT_0:
			case SHADER_PARAM::FLOAT_1:
			case SHADER_PARAM::FLOAT_2:
			case SHADER_PARAM::FLOAT_3:
			{
				float data = 0;
				target_resource_->GetData(param.type, &data);
				ImGui::TableNextColumn();
				ImGui::Text(WStringToString(param.usage).c_str());
				ImGui::TableNextColumn();
				if (DataInputWidget::DataInputFloat(param.usage, &data))
				{
					target_resource_->SetData(param.type, &data);
				}
				ImGui::TableNextRow();
			}
			break;
			case SHADER_PARAM::VEC2_0:
			case SHADER_PARAM::VEC2_1:
			case SHADER_PARAM::VEC2_2:
			case SHADER_PARAM::VEC2_3:
			{
				Vec2 data;
				target_resource_->GetData(param.type, &data);
				ImGui::TableNextColumn();
				ImGui::Text(WStringToString(param.usage).c_str());
				ImGui::TableNextColumn();
				if (DataInputWidget::DataInputVec2(param.usage, data))
				{
					target_resource_->SetData(param.type, &data);
				}
				ImGui::TableNextRow();
			}
			break;
			case SHADER_PARAM::VEC4_0:
			case SHADER_PARAM::VEC4_1:
			case SHADER_PARAM::VEC4_2:
			case SHADER_PARAM::VEC4_3:
			{
				Vec4 data;
				target_resource_->GetData(param.type, &data);
				ImGui::TableNextColumn();
				ImGui::Text(WStringToString(param.usage).c_str());
				ImGui::TableNextColumn();
				if (DataInputWidget::DataInputVec4(param.usage, data))
				{
					target_resource_->SetData(param.type, &data);
				}
				ImGui::TableNextRow();
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
				ImGui::TableNextColumn();
				ImGui::Text(WStringToString(param.usage).c_str());
				ImGui::TableNextColumn();
				DataInputWidget::DataInputTexture(param.usage, data, (DWORD_PTR)param.type, (COMBO_CALLBACK)&MaterialWidget::ChangeTexture, this);
				ImGui::TableNextRow();
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
		ImGui::EndTable();
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
