#include "pch.h"
#include "Collider2DWidget.h"
#include "ModalListWidget.h"
#include "WidgetManager.h"


#include <Engine/CCollider2D.h>
#include <Engine/CMesh.h>
#include <Engine/CResourceManager.h>
Collider2DWidget::Collider2DWidget()
	:ComponentWidget("Collider2D",Vec2(0,120))
{
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

			array<char, 255> str = { 0, };
			WideCharToMultiByte(CP_ACP, 0, mesh->GetName().c_str(), -1, str.data(), (int)mesh->GetName().size(), nullptr, nullptr);
			ImGui::PushID(0);
			if (ImGui::Button(str.data(), ImVec2(200, 0)))
			{
				ModalListWidget* listWidget = dynamic_cast<ModalListWidget*>(WidgetManager::GetInst()->FindWidget("modal_list"));

				array<char, 255> szBuffer = { 0, };
				const unordered_map<wstring, CResource*>& meshMap = CResourceManager::GetInst()->GetResource<CMesh>();
				int count = 0;
				for (const auto& pair : meshMap)
				{
					if (!listWidget->isOpen())
					{
						WideCharToMultiByte(CP_ACP, 0, pair.first.c_str(), -1, szBuffer.data(), (int)pair.first.size(), nullptr, nullptr);
						listWidget->AddItem(szBuffer.data());
						if (nullptr != mesh && mesh->GetName() == pair.first)
							listWidget->SetCurrentIndex(count);
						szBuffer.fill(0);
						++count;
					}
				}

				listWidget->SetCaption("Select Mesh");
				listWidget->SetCallbackFunc(this, nullptr);
				listWidget->Activate();
			}
			ImGui::PopID();

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
