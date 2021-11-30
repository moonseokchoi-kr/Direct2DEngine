#include "pch.h"
#include "AtlasTextureTool.h"
#include "imgui_internal.h"

#include <Engine/CKeyManager.h>
#include <Engine/CResourceManager.h>
AtlasTextureTool::AtlasTextureTool()
	:Widget("atlas_editor")
	, is_open_(false)
	, zoom_(1.0f)
	, window_flags_(0)
{
}

AtlasTextureTool::~AtlasTextureTool()
{
}

void AtlasTextureTool::Init()
{
	atlas_ = nullptr;
	window_flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
	region_size_ = Vec2(32, 46);
}

void AtlasTextureTool::Update()
{
	// Child 2: rounded border
	ImGui::SetNextWindowSize(ImVec2(0, 512));
	if(ImGui::Begin("Atlas Editor", &is_active_, window_flags_))
	{
		ShowMenuBar();
		if (ImGui::BeginTable("##rectSize", 2, ImGuiTableFlags_Resizable))
		{
			ImGui::TableNextColumn();
			ImGui::Text("Rect Size");
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::InputFloat("##rect_x", &region_size_.x);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::InputFloat("##rect_y", &region_size_.y);
			ImGui::EndTable();
		}
		ShowAtlasVeiw();
		ImGui::End();

	}
	else
	{
		Deactivate();
		ImGui::End();
	}
	
}

void AtlasTextureTool::Clear()
{
	selected_left_top_ = Vec2();
	release_mouse_ = false;
}

void AtlasTextureTool::ShowMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "CTRL+S"))
			{
				int a = 0;
			}

			if (ImGui::MenuItem("Load", "CTRL+O"))
			{
				int a = 0;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Texture"))
		{
			if (ImGui::MenuItem("Load Texture"))
			{
				atlas_ = CResourceManager::GetInst()->LoadRes<CTexture>(L"atlas", L"texture\\anim_texture\\sakuya_player.png");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void AtlasTextureTool::ShowAtlasVeiw()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	ImVec2 canvas_center = ImVec2(canvas_p0.x + canvas_sz.x / 2.f, canvas_p0.y + canvas_sz.y / 2.f);
	// Draw border and background color
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	//draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	if (nullptr != atlas_)
	{
		{
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
			float region_x = current_mouse_pos_.x - canvas_p0.x;
			float region_y = current_mouse_pos_.y - canvas_p0.y;
			float zoom = 4.0f;
			ImGui::Image(atlas_->GetShaderResourceView(), ImVec2((float)atlas_->GetWidth() * zoom_, (float)atlas_->GetHeight() * zoom_), uv_min, uv_max);
			ImGui::SetItemUsingMouseWheel();
			if (ImGui::IsItemHovered())
			{
				float wheel = io.MouseWheel;
				window_flags_ |= ImGuiWindowFlags_NoMove;
				if (KEY_HOLD(KEY::CTRL))
				{
					zoom_ += wheel * 0.1f;
				}

				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					current_mouse_pos_ = Vec2(io.MousePos.x-region_size_.x/2.f, io.MousePos.y- region_size_.x / 2.f);
					release_mouse_ = false;

				}
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
				{
					current_mouse_pos_ += Vec2(io.MouseDelta.x, io.MouseDelta.y);
				}
				if(ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					selected_left_top_ = Vec2(region_x, region_y);
					//current_mouse_pos_ = Vec2(-1, -1);
					release_mouse_ = true;
				}
			
				ImGui::BeginTooltip();

				if (region_x < 0.0f) { region_x = 0.0f; }
				else if (region_x > (float)atlas_->GetWidth() - region_size_.x) { region_x = (float)atlas_->GetWidth() - region_size_.x; }
				if (region_y < 0.0f) { region_y = 0.0f; }
				else if (region_y > (float)atlas_->GetHeight() - region_size_.y) { region_y = (float)atlas_->GetHeight() - region_size_.y; }
				ImGui::Text("LeftTop: (%.2f, %.2f)", region_x, region_y);
				ImGui::Text("RightBottom: (%.2f, %.2f)", region_x + region_size_.x, region_y + region_size_.y);
				ImGui::EndTooltip();
			}
			else
			{
				release_mouse_ = false;
				window_flags_ ^= ImGuiWindowFlags_NoMove;
			}
			
			draw_list->AddRect(current_mouse_pos_, ImVec2(current_mouse_pos_.x + region_size_.x, current_mouse_pos_.y + region_size_.y), IM_COL32(51, 218, 32, 255));
		}
	}
	else
	{

	}
}
