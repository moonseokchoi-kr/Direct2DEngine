#include "pch.h"
#include "MaterialWidget.h"


#include <Engine/CGraphicsShader.h>
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
		ImGui::SameLine();
		ImGui::Text(WStringToString(target_resource_->GetName()).c_str());
		


	}
	End();
}

void MaterialWidget::ShowMaterialDetail()
{
	if (ImGui::BeginTable("##MaterialDetail",2))
	{
		ImGui::TableNextColumn();
		ImGui::Text("Shader");
		
		ImGui::TableNextColumn();
		//shader data

		const unordered_map<wstring, CResource*>& shader_map = CResourceManager::GetInst()->GetResource<CGraphicsShader>();
		//combo widget√ﬂ∞°
		ImGui::EndTable();
	}
}

void MaterialWidget::ChangeShader(DWORD_PTR, DWORD_PTR)
{
}
