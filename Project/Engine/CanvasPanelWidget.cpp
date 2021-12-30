#include "pch.h"
#include "CanvasPanelWidget.h"
#include "CResourceManager.h"
#include "CGameObject.h"
#include "CUIComponent.h"

CanvasPanelWidget::CanvasPanelWidget()
	:GameWidget(UI_TYPE::CANVAS)
{
	mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"ui_material");
	SetOffSet(Vec3(0.f, -0.3f, 0.f));
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

void CanvasPanelWidget::SaveToScene(FILE* file)
{
	GameWidget::SaveToScene(file);

	size_t childCount = child_widget_vector_.size();
	fwrite(&childCount, sizeof(size_t), 1, file);
	
	for (const auto& widget : child_widget_vector_)
	{
		UINT type = (UINT)widget->GetType();
		fwrite(&type, sizeof(UINT), 1, file);
		widget->SaveToScene(file);
	}
}

void CanvasPanelWidget::LoadFromScene(FILE* file)
{
	GameWidget::LoadFromScene(file);
	size_t childCount = 0;
	fread(&childCount, sizeof(size_t), 1, file);

	for (int i = 0; i < (int)childCount; ++i)
	{
		UINT type;
		fread(&type, sizeof(UINT), 1, file);
		GameWidget* widget = GetOwner()->UIComponent()->AddWidget((UI_TYPE)type);
		widget->LoadFromScene(file);
	}
}
