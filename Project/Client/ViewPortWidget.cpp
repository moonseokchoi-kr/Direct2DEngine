#include "pch.h"
#include "ViewPortWidget.h"
#include "WidgetManager.h"

#include <Engine/CTexture.h>
ViewPortWidget::ViewPortWidget()
	:Widget("view_port")
{
}

ViewPortWidget::~ViewPortWidget()
{
}

void ViewPortWidget::Init()
{
}

void ViewPortWidget::Update()
{
	if (ImGui::Begin("View port"))
	{
		ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
		if (nullptr != WidgetManager::GetInst()->GetRenderTarget())
		{
			ImTextureID textureID = WidgetManager::GetInst()->GetRenderTarget()->GetShaderResourceView();
			ImGui::Image(textureID, ImVec2(1280, 720));
		}

		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}
