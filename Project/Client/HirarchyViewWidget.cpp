#include "pch.h"
#include "HirarchyViewWidget.h"

#include "WidgetManager.h"
#include "InspectorWidget.h"

#include <Engine/CSceneManager.h>
#include <Engine/CGameObject.h>

HirarchyViewWidget::HirarchyViewWidget()
	:Widget("hirarchy_view")
{
}

HirarchyViewWidget::~HirarchyViewWidget()
{
}

void HirarchyViewWidget::Init()
{

}

void HirarchyViewWidget::Update()
{
	if (ImGui::Begin("Hirarchy view", &is_active_))
	{
		const char* items[] = {"camera","player"};
		static int item_current_idx = 0; // Here we store our selection data as an index.
		if (ImGui::BeginListBox("##listbox 1"))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
					array<wchar_t, 256> szBuffer = { 0, };
					MultiByteToWideChar(CP_ACP, 0, items[item_current_idx], (int)strlen(items[item_current_idx]), szBuffer.data(), (int)szBuffer.size());
					CGameObject* target = CSceneManager::GetInst()->FindObjectByName(szBuffer.data());
					if(nullptr != target)
						dynamic_cast<InspectorWidget*>(WidgetManager::GetInst()->FindWidget("inspector_view"))->SetGameObject(target);
				}
					
			}
			ImGui::EndListBox();
		}

		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}
