#include "pch.h"
#include "MeshRenderWidget.h"
#include "WidgetManager.h"
#include "ModalListWidget.h"
#include <Engine/CMeshRender.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>
#include <Engine/CResourceManager.h>
MeshRenderWidget::MeshRenderWidget()
	:ComponentWidget("MeshRender", Vec2(0.f,100.f))
{

}

MeshRenderWidget::~MeshRenderWidget()
{
}

void MeshRenderWidget::Update()
{
	CMeshRender* meshRender = GetTarget()->MeshRender();
	Ptr<CMesh> mesh = meshRender->GetMesh();
	Ptr<CMaterial> material = meshRender->GetCurrentMaterial();
	meshWidget.SetName("##mesh select");
	materialWidget.SetName("##material select");

	Start();
	{
		if (ImGui::BeginTable("material_spilt", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg, ImVec2(0, 0), 0.f))
		{
			ImGui::TableNextColumn();
			//Position
			ImGui::Text("Mesh");
			ImGui::TableNextColumn();
			
			const unordered_map<wstring, CResource*>& meshMap = CResourceManager::GetInst()->GetResource<CMesh>();
			int count = 0;
			meshWidget.SetCallback(this, (COMBO_CALLBACK)&MeshRenderWidget::ChangeMesh);
			for (const auto& pair : meshMap)
			{
				meshWidget.AddComboData(WStringToString(pair.first));
				if (nullptr != mesh && pair.first == mesh->GetKey())
					meshWidget.SetCurrentIndex(count);
				++count;
			}
			meshWidget.Update();
		

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Scale

			ImGui::Text("Material");
			ImGui::TableNextColumn();

			const unordered_map<wstring, CResource*>& materialMap = CResourceManager::GetInst()->GetResource<CMaterial>();
			count = 0;
			materialWidget.SetCallback(this, (COMBO_CALLBACK)&MeshRenderWidget::ChangeMaterial);
			for (const auto& pair : materialMap)
			{
				materialWidget.AddComboData(WStringToString(pair.first));
				if (nullptr != material && pair.first == material->GetKey())
					materialWidget.SetCurrentIndex(count);
				++count;
			}

			materialWidget.Update();


			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::EndTable();
		}
	}
	End();
}

void MeshRenderWidget::ChangeMesh(DWORD_PTR instance, DWORD_PTR meshName)
{
	ComboWidget* widget = (ComboWidget*)instance;
	string strName = widget->GetSelectedItem();
	wchar_t szName[256] = {};
	MultiByteToWideChar(CP_ACP, 0, strName.c_str(), (int)strName.size(), szName, 256);
	CMeshRender* meshRender = GetTarget()->MeshRender();
	meshRender->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(szName));
}

void MeshRenderWidget::ChangeMaterial(DWORD_PTR instance, DWORD_PTR materialName)
{
	ComboWidget* widget = (ComboWidget*)instance;
	string strName = widget->GetSelectedItem();
	wchar_t szName[256] = {};
	MultiByteToWideChar(CP_ACP, 0, strName.c_str(), (int)strName.size(), szName, 256);
	CMeshRender* meshRender = GetTarget()->MeshRender();
	meshRender->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(szName));
}
