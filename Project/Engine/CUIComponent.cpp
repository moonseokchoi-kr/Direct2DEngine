#include "pch.h"
#include "CUIComponent.h"

#include "CKeyManager.h"

#include "CTransform.h"
#include "CanvasPanelWidget.h"

CUIComponent::CUIComponent()
	:CComponent(COMPONENT_TYPE::UI)
	,is_hovered(false)
	,is_mouse_l_down_(false)
	,is_mouse_l_release_(false)
	,is_select_(false)
	,target_widget_(nullptr)
	,canvas_widget_(nullptr)
{
	canvas_widget_ = new CanvasPanelWidget;
}

CUIComponent::~CUIComponent()
{
	SafeDelete(canvas_widget_);
}

void CUIComponent::Update()
{
	canvas_widget_->Update();
}

void CUIComponent::LateUpdate()
{
	CheckMouseHovered();
	if(is_hovered)
		canvas_widget_->LateUpdate();
	ComputeTargetWidget();
}

void CUIComponent::FinalUpdate()
{
	if (GetOwner())
		canvas_widget_->SetOwner(GetOwner());
	canvas_widget_->FinalUpdate();
}

void CUIComponent::UpdateData()
{
	canvas_widget_->UpdateData();
}

void CUIComponent::Render()
{
	canvas_widget_->Render();
}

void CUIComponent::OnMouseHovered()
{

}

void CUIComponent::OnMouseLeftButtonDown()
{
}

void CUIComponent::OnMouseLeftButtonUp()
{

}

GameWidget* CUIComponent::AddWidget(UI_TYPE type)
{
	GameWidget* widget = nullptr;
	switch (type)
	{
	case UI_TYPE::LIST:
	{

	}
		break;
	case UI_TYPE::GRID:
	{

	}
		break;
	case UI_TYPE::END:
	default:
		break;
	}
	canvas_widget_->AddChild(widget);
	return widget;
}

void CUIComponent::CheckMouseHovered()
{
	Vec2 mousePos = MOUSE_POS;
	Vec3 pos = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();
	Vec3 lt = Vec3(pos.x - scale.x / 2.f, pos.y - scale.y / 2.f, pos.x);

	if (lt.x <= mousePos.x && mousePos.x <= lt.x + scale.x && lt.y <= mousePos.y && mousePos.y <= lt.y + scale.y)
	{
		is_hovered = true;
	}
	else
	{
		is_hovered = false;
	}
}

void CUIComponent::ComputeTargetWidget()
{
	static list<GameWidget*> queue;
	static vector<GameWidget*> noneTargetUIs;
	
	queue.clear();
	noneTargetUIs.clear();

	GameWidget* targetWidget = nullptr;
	bool lButtonAway = KEY_AWAY(KEY::LBTN);

	queue.push_back(canvas_widget_);

	while (!queue.empty())
	{
		GameWidget* ui = queue.front();
		queue.pop_front();

		if (ui->IsHovered())
		{
			if (nullptr != targetWidget)
			{
				noneTargetUIs.push_back(targetWidget);

			}
			targetWidget = ui;
		}
		else
		{
			noneTargetUIs.push_back(ui);
		}
		const vector<GameWidget*> children = canvas_widget_->GetChildWidgets();
		for (const auto& child : children)
		{
			queue.push_back(child);
		}
	}

	if (lButtonAway)
	{
		for (const auto& widget : noneTargetUIs)
		{
			widget->is_mouse_l_down_ = false;
		}
	}

	target_widget_ = targetWidget;
}

void CUIComponent::SaveToScene(FILE* file)
{
	__super::SaveToScene(file);
	canvas_widget_->SaveToScene(file);
}

void CUIComponent::LoadFromScene(FILE* file)
{
	canvas_widget_ = new CanvasPanelWidget;
	canvas_widget_->LoadFromScene(file);
}
