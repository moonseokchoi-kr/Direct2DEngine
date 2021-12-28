#include "pch.h"
#include "CanvasPanelWidget.h"
#include "CResourceManager.h"

CanvasPanelWidget::CanvasPanelWidget()
{
	mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"ui_material");
}

CanvasPanelWidget::~CanvasPanelWidget()
{
}

void CanvasPanelWidget::Update()
{
}

void CanvasPanelWidget::Render()
{
	__super::Render();
}
