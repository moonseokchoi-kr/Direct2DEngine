#include "pch.h"
#include "InspectorWidget.h"

#include <Engine/CGameObject.h>

InspectorWidget::InspectorWidget()
	:Widget("inspector_view")
	, target_object_(nullptr)
{
}

InspectorWidget::~InspectorWidget()
{
}

void InspectorWidget::Update()
{
	ImGui::Begin(GetName().c_str(), &is_active_);

	ShowObjectInfo();

	UpdateChildren();

	ImGui::End();
}

void InspectorWidget::ShowObjectInfo()
{
}
