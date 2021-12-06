#include "pch.h"
#include "ViewPortWidget.h"
#include "WidgetManager.h"

#include <Engine/CTexture.h>
#include <Engine/CRenderManager.h>
#include <Engine/CDevice.h>
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
		ImVec2 viewPortLt = ImGui::GetCursorScreenPos();
		ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
		ImVec2 center = ImVec2(viewPortLt.x + viewPortSize.x / 2.f, viewPortLt.y + viewPortSize.y / 2.f);
		Vec2 resolution = CDevice::GetInst()->GetResolution();
		static ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImTextureID textureID = CDevice::GetInst()->GetRenderTarget()->GetRenderTargetView();
		draw_list->AddImage(textureID, ImVec2(center.x - resolution.x/2.f, center.y - resolution.y/2.f), ImVec2(center.x + resolution.x/2.f, center.y + resolution.y/2.f));

		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}
