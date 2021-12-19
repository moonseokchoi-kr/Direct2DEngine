#include "pch.h"
#include "Collider2DWidget.h"
#include "ModalListWidget.h"
#include "WidgetManager.h"
#include "MeshRenderWidget.h"

#include <Engine/CCollider2D.h>
#include <Engine/CMesh.h>
#include <Engine/CResourceManager.h>

Collider2DWidget::Collider2DWidget()
	:ComponentWidget("Collider2D",Vec2(0,120))
{
	meshWidget.SetName("##mesh");
}

Collider2DWidget::~Collider2DWidget()
{
}

void Collider2DWidget::Init()
{
}

void Collider2DWidget::Update()
{
	Start();
	{
		CCollider2D* col = GetTarget()->Collider2D();
		Vec3 offsetPos = col->GetOffsetPosition();
		Vec3 offsetScale = col->GetOffsetScale();
		CMesh* mesh = col->GetColliderMesh();
		
		ImGui::AlignTextToFramePadding();
		//ImVec2 widgetSize = ImGui::GetItemRectSize();
		if (ImGui::BeginTable("transform_spilt", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
		{
			ImGui::TableNextColumn();
			ImGui::Text("Mesh");
			ImGui::TableNextColumn();

			const unordered_map<wstring, CResource*>& meshMap = CResourceManager::GetInst()->GetResource<CMesh>();
			int count = 0;
			meshWidget.SetCallback(this, (MODAL_LIST_CALLBACK)&Collider2DWidget::ChangeMesh);
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

			//Position
			ImGui::Text("Offset Position");
			ImGui::TableNextColumn();
			//ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - ImGui::GetItemRectSize().x*2.f);
			//ImGui::PushItemWidth(-100);
			offsetPos = CreateControlPanel("##offset_position", offsetPos);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// Scale
			ImGui::Indent();
			ImGui::Text("Offset Scale");
			ImGui::Unindent();
			ImGui::TableNextColumn();
			offsetScale = CreateControlPanel("##offset_scale", offsetScale);


			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::EndTable();
		}

		col->SetOffsetPosition(Vec2(offsetPos.x, offsetPos.y));
		col->SetOffsetScale(Vec2(offsetScale.x, offsetScale.y));
	}
	End();
}

Vec3 Collider2DWidget::CreateControlPanel(const string& label, Vec3 value)
{
	string lb = label;

	ImGui::Indent(1.f);
	ImGui::Text("X");
	ImGui::SameLine(0);
	ImGui::PushItemWidth(80);
	ImGui::DragFloat((lb + "x").c_str(), &value.x, 0.001f, 0.f, 0.f, "%.3f");
	ImGui::PopItemWidth();
	ImGui::Unindent();

	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine(0);
	ImGui::PushItemWidth(80);
	ImGui::DragFloat((lb + "y").c_str(), &value.y, 0.001f, 0.f, 0.f, "%.3f");
	ImGui::PopItemWidth();

	return value;

}


void Collider2DWidget::ChangeMesh(DWORD_PTR instance, DWORD_PTR meshName)
{
	ModalListWidget* widget = (ModalListWidget*)instance;
	string strName = widget->GetSelectedName();
	wchar_t szName[256] = {};
	MultiByteToWideChar(CP_ACP, 0, strName.c_str(), (int)strName.size(), szName, 256);
	CCollider2D* collider = GetTarget()->Collider2D();
	collider->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(szName));
}