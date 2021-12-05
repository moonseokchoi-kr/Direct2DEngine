#include "pch.h"
#include "ViewPortWidget.h"
#include "WidgetManager.h"

#include <Engine/CTexture.h>
#include <Engine/CRenderManager.h>
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

		ImTextureID textureID = CRenderManager::GetInst()->GetViewPortTexture()->GetShaderResourceView();
		ImGui::Image(textureID, ImVec2(1600, 900));

		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}
