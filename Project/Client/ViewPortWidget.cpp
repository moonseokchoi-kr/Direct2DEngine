#include "pch.h"
#include "ViewPortWidget.h"
#include "WidgetManager.h"
#include "ToolCamera.h"
#include "InspectorWidget.h"
#include "ResourceViewWidget.h"

#include <Engine/CTexture.h>
#include <Engine/CRenderManager.h>
#include <Engine/CSceneManager.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CDevice.h>
#include <Engine/CTransform.h>
#include <Engine/CGameObject.h>
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
	if (ImGui::Begin("Tool View"))
	{
		if (ImGui::BeginChild("SettingCamera", ImVec2(ImGui::GetContentRegionAvail().x, 40), true))
		{
			ImGui::Checkbox("View Othro", &is_othro_);
			ImGui::EndChild();
		}
		if (ImGui::BeginChild("View", ImVec2(ImGui::GetContentRegionAvail()), false))
		{
			ImVec2 viewPortLt = ImGui::GetCursorScreenPos();
			ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
			ImVec2 center = ImVec2(viewPortLt.x + viewPortSize.x / 2.f, viewPortLt.y + viewPortSize.y / 2.f);
			Vec2 resolution = CDevice::GetInst()->GetResolution();
			Vec2 screenRatio = Vec2(viewPortSize.x / resolution.x, viewPortSize.y / viewPortSize.x);
			static ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImTextureID textureID = CRenderManager::GetInst()->GetViewPortTexture()->GetShaderResourceView();
			draw_list->AddImage(textureID, ImVec2(center.x - resolution.x / 2.f, center.y - resolution.y / 2.f), ImVec2(center.x + resolution.x / 2.f, center.y + resolution.y / 2.f));
			//begin tooltip
			if (ImGui::IsWindowHovered())
			{
				ImGui::BeginTooltip();
				ImVec2 mousePos = ImGui::GetMousePos();
				ToolCamera* camera = (ToolCamera*)CRenderManager::GetInst()->GetToolCamera();
				Vec3 moveDelta = camera->GetMoveDelta();
				current_mouse_pos_ = Vec2(mousePos.x - viewPortLt.x - viewPortSize.x / 2.f + (moveDelta.x * screenRatio.x), -(mousePos.y - viewPortLt.y - viewPortSize.y / 2.f + (moveDelta.y * screenRatio.y)));
				ImGui::Text("MousePos: (%.2f, %.2f)", current_mouse_pos_.x, current_mouse_pos_.y);
				ImGui::EndTooltip();
				if (is_othro_)
				{
					camera->SetProjectionType(PROJECTION_TYPE::ORTHO);
					if (nullptr == target_object_)
						ComputeTagetObject(screenRatio);
					MouseEvent();
				}
				else
				{
					camera->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);
					target_object_ = nullptr;
				}


			}
			ImGui::EndChild();
		}
		


		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
	if (ImGui::Begin("Game View"))
	{
		ImVec2 viewPortLt = ImGui::GetCursorScreenPos();
		ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
		ImVec2 center = ImVec2(viewPortLt.x + viewPortSize.x / 2.f, viewPortLt.y + viewPortSize.y / 2.f);
		Vec2 resolution = CDevice::GetInst()->GetResolution();
		static ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImTextureID textureID = CRenderManager::GetInst()->GetGameViewTexture()->GetShaderResourceView();
		draw_list->AddImage(textureID, ImVec2(center.x - resolution.x / 2.f, center.y - resolution.y / 2.f), ImVec2(center.x + resolution.x / 2.f, center.y + resolution.y / 2.f));
		

		
		ImGui::End();

	}
	else
	{
		ImGui::End();
	}

	if (SCENE_MODE::STOP == CSceneManager::GetInst()->GetSceneMode() && !check_window_focus)
	{
		ImGui::SetWindowFocus("Tool View");
		check_window_focus = true;
	}
	if (SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode() && check_window_focus)
	{
		ImGui::SetWindowFocus("Game View");
		check_window_focus = false;
	}
}


void ViewPortWidget::ComputeMousePos()
{
	
}

void ViewPortWidget::ComputeTagetObject(Vec2 screenRatio)
{
	CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* layer = currentScene->GetLayer(i);
		const vector<CGameObject*>& objectVector = layer->GetParentObjects();
		for (const auto& object : objectVector)
		{
			Vec3 pos = object->Transform()->GetPosition();
			Vec3 scale = object->Transform()->GetScale();
			Vec3 lt = Vec3(pos.x +((pos.x?abs(pos.x)/-pos.x:-1) * scale.x / 2.f), pos.y + ((pos.y?abs(pos.y)/ -pos.y:-1) * scale.y / 2.f), pos.z);
			if (lt.x <= current_mouse_pos_.x && current_mouse_pos_.x <= lt.x + scale.x 
				&& lt.y <= current_mouse_pos_.y && current_mouse_pos_.y <= lt.y + scale.y)
			{
				target_object_ = object;
			}
		}
	}
}

void ViewPortWidget::MouseEvent()
{
	if (nullptr != target_object_)
	{
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			InspectorWidget* inspector = dynamic_cast<InspectorWidget*>(WidgetManager::GetInst()->FindWidget("inspector_view"));
			inspector->SetGameObject(target_object_);

			ResourceViewWidget* resourceView = dynamic_cast<ResourceViewWidget*>(WidgetManager::GetInst()->FindWidget("resource_view"));
			resourceView->ReleaseSelectNode();
		}
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			Vec3 pos = target_object_->Transform()->GetPosition();
			pos=Vec3(current_mouse_pos_.x,current_mouse_pos_.y,pos.z);
			target_object_->Transform()->SetPosition(pos);
		}
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			target_object_ = nullptr;
		}
	}
}
		
