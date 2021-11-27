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
	Ptr<CMaterial> material = meshRender->GetMaterial();

	Start();
	{
		if (ImGui::BeginTable("material_spilt", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg, ImVec2(0, 0), 0.f))
		{
			ImGui::TableNextColumn();
			//Position
			ImGui::Text("Mesh");
			ImGui::TableNextColumn();
			
			array<char, 255> str = { 0, };
			WideCharToMultiByte(CP_ACP, 0, mesh->GetName().c_str(), -1, str.data(), (int)mesh->GetName().size(), nullptr, nullptr);
			ImGui::PushID(0);
			if (ImGui::Button(str.data(), ImVec2(200, 0)))
			{
				ModalListWidget* listWidget = dynamic_cast<ModalListWidget*>(WidgetManager::GetInst()->FindWidget("modal_list_widget"));

				array<char, 255> szBuffer = { 0, };
				const unordered_map<wstring, CResource*>& meshMap = CResourceManager::GetInst()->GetResource<CMesh>();
				int count = 0;
				for (const auto& pair : meshMap)
				{
					if (!listWidget->isOpen())
					{
						WideCharToMultiByte(CP_ACP, 0, pair.first.c_str(), -1, szBuffer.data(), (int)pair.first.size(), nullptr, nullptr);
						listWidget->AddItem(szBuffer.data());
						if(nullptr != mesh && mesh->GetName() == pair.first)
							listWidget->SetCurrentIndex(count);
						szBuffer.fill(0);
						++count;
					}
				}

				listWidget->SetCaption("Select Mesh");
				listWidget->SetCallbackFunc(this, (MODAL_LIST_CALLBACK)&MeshRenderWidget::ChangeMesh);
				listWidget->Activate();
			}
			ImGui::PopID();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Scale

			ImGui::Text("Material");
			ImGui::TableNextColumn();

			WideCharToMultiByte(CP_ACP, 0, material->GetName().c_str(), -1, str.data(), (int)material->GetName().size(), nullptr, nullptr);
			ImGui::PushID(1);
			if (ImGui::Button(str.data(), ImVec2(200, 0)))
			{
				ModalListWidget* listWidget = dynamic_cast<ModalListWidget*>(WidgetManager::GetInst()->FindWidget("modal_list_widget"));

				array<char, 255> szBuffer = { 0, };
				const unordered_map<wstring, CResource*>& meshMap = CResourceManager::GetInst()->GetResource<CMaterial>();
				int count = 0;
				for (const auto& pair : meshMap)
				{
					if (!listWidget->isOpen())
					{
						WideCharToMultiByte(CP_ACP, 0, pair.first.c_str(), -1, szBuffer.data(), (int)pair.first.size(), nullptr, nullptr);
						listWidget->AddItem(szBuffer.data());
						if (GetTarget()->MeshRender()->GetMaterial()->GetName() == pair.first)
							listWidget->SetCurrentIndex(count);
						szBuffer.fill(0);
						++count;
					}
				}

				listWidget->SetCaption("Select Material");
				listWidget->SetCallbackFunc(this, (MODAL_LIST_CALLBACK)&MeshRenderWidget::ChangeMaterial);

				listWidget->Activate();
			}
			ImGui::PopID();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::EndTable();
		}
	}
	End();
}

void MeshRenderWidget::ChangeMesh(DWORD_PTR instance, DWORD_PTR meshName)
{
	ModalListWidget* widget = (ModalListWidget*)instance;
	string strName = widget->GetSelectedName();
	wchar_t szName[256] = {};
	MultiByteToWideChar(CP_ACP, 0, strName.c_str(), strName.size(), szName, 256);
	CMeshRender* meshRender = GetTarget()->MeshRender();
	meshRender->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(szName));
}

void MeshRenderWidget::ChangeMaterial(DWORD_PTR instance, DWORD_PTR materialName)
{
	ModalListWidget* widget = (ModalListWidget*)instance;
	string strName = widget->GetSelectedName();
	wchar_t szName[256] = {};
	MultiByteToWideChar(CP_ACP, 0, strName.c_str(), strName.size(), szName, 256);
	CMeshRender* meshRender = GetTarget()->MeshRender();
	meshRender->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(szName));
}
