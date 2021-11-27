#include "pch.h"
#include "AnimationTool.h"
#include "imgui_internal.h"

#include <Engine/CResourceManager.h>
#include <Engine/CKeyManager.h>
#include <Engine/CAnimator2D.h>

AnimationTool::AnimationTool()
	:Widget("animation_tool")
	,is_open_(false)
	,menu_selected_(false)
	,label_("AnimationTool")
	,zoom_(1.0f)
	,target_animator_(nullptr)
{
}

AnimationTool::~AnimationTool()
{
}

void AnimationTool::Init()
{
	atlas_ = nullptr;
	window_flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
}

void AnimationTool::Update()
{
	ImGui::SetNextWindowSize(ImVec2(1200, 600), ImGuiCond_FirstUseEver);
	if(ImGui::Begin(label_.c_str(), &is_open_ , window_flags_))
	{
		
		ShowMenuBar();
		ShowAnimationEditWidget();
		ShowTextureView();
	}
	ImGui::End();
	
}

void AnimationTool::ShowMenuBar()
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

void AnimationTool::ShowAnimationEditWidget()
{
	// Child 1: no border, enable horizontal scrollbar

	ImGui::Text("Animation View");
	ImGui::SameLine(500);
	ImGui::Text("Atlas View");

	{
		// 		static ImVector<ImVec2> points;
		// 		static ImVec2 scrolling(0.0f, 0.0f);
		static bool opt_enable_grid = true;
		static bool opt_enable_context_menu = true;
		static bool adding_line = false;

		if (ImGui::BeginChild("##animation_view", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false))
		{
			// 			ImGui::Checkbox("Enable grid", &opt_enable_grid);
			// 			ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
			// 			ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

						// Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
						// Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
						// To use a child window instead we could use, e.g:
						//      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
						//      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
						//      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_NoMove);
						//      ImGui::PopStyleColor();
						//      ImGui::PopStyleVar();
						//      [...]
						//      ImGui::EndChild();

						// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
			ImVec2 canvas_center = ImVec2(canvas_p0.x + canvas_sz.x / 2.f, canvas_p0.y + canvas_sz.y / 2.f);
			// Draw border and background color
			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			// This will catch our interactions
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held

			// Context menu (under default mouse threshold)
// 			ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
// 			if (opt_enable_context_menu && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
// 				ImGui::OpenPopupOnItemClick("context");
// 			if (ImGui::BeginPopup("context"))
// 			{
// 				if (adding_line)
// 					points.resize(points.size() - 2);
// 				adding_line = false;
// 				if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
// 				if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
// 				ImGui::EndPopup();
// 			}

			// Draw grid + all lines in the canvas
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);
			if (opt_enable_grid)
			{
				const float GRID_STEP = 64.0f;
				for (float x = fmodf(0, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
				for (float y = fmodf(0, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));

			}
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + canvas_sz.y / 2.f), ImVec2(canvas_p1.x, canvas_p0.y + canvas_sz.y / 2.f), IM_COL32(255, 0, 0, 255));
			draw_list->AddLine(ImVec2(canvas_p1.x - canvas_sz.x / 2.f, canvas_p0.y), ImVec2(canvas_p1.x - canvas_sz.x / 2.f, canvas_p1.y), IM_COL32(0, 0, 255, 255));
			if (nullptr != atlas_)
			{
				ImVec2 uv0 = ImVec2((left_top_.x) / (float)atlas_->GetWidth()*zoom_, (left_top_.y) / (float)atlas_->GetHeight()* zoom_);
				ImVec2 uv1 = ImVec2((left_top_.x + region_size_.x) / (float)atlas_->GetWidth()* zoom_, (left_top_.y + region_size_.y) / (float)atlas_->GetHeight()* zoom_);
				draw_list->AddImage(atlas_->GetShaderResourceView(), ImVec2(canvas_center.x - region_size_.x / 2.f, canvas_center.y - region_size_.y / 2.f), ImVec2(canvas_center.x + region_size_.x / 2.f, canvas_center.y + region_size_.y / 2.f), uv0, uv1);
			}
			draw_list->PopClipRect();
			ImGui::EndChild();
		}
	}

	ImGui::SameLine();
	// Child 2: rounded border
	ImGui::BeginChild("##atlas_viewer", ImVec2(0, 512), false);
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
				float region_x = current_mouse_pos_.x- canvas_p0.x;
				float region_y = current_mouse_pos_.y - canvas_p0.y;
				float zoom = 4.0f;
				ImGui::Image(atlas_->GetShaderResourceView(), ImVec2((float)atlas_->GetWidth() * zoom_, (float)atlas_->GetHeight() * zoom_), uv_min, uv_max);
				ImGui::SetItemUsingMouseWheel();
				if (ImGui::IsItemHovered())
				{
					float wheel = io.MouseWheel;
					static bool draw_rect = false;
					window_flags_ |= ImGuiWindowFlags_NoMove;
					if (KEY_HOLD(KEY::CTRL))
					{
						zoom_ += wheel * 0.1;
					}

					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						region_size_ = Vec2(0, 0);
						current_mouse_pos_ = Vec2(io.MousePos.x, io.MousePos.y);

					}
					if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
					{
						region_size_ += Vec2(io.MouseDelta.x, io.MouseDelta.y);
						draw_rect = true;
					}
					else
					{
						left_top_ = Vec2(region_x, region_y);
						//draw_rect = false;
					}



					if (region_x < 0.0f) { region_x = 0.0f; }
					else if (region_x > (float)atlas_->GetWidth() - region_size_.x) { region_x = (float)atlas_->GetWidth() - region_size_.x; }
					if (region_y < 0.0f) { region_y = 0.0f; }
					else if (region_y > (float)atlas_->GetHeight() - region_size_.y) { region_y = (float)atlas_->GetHeight() - region_size_.y; }

					if (draw_rect)
						draw_list->AddRect(current_mouse_pos_, ImVec2(current_mouse_pos_.x + region_size_.x, current_mouse_pos_.y + region_size_.y), IM_COL32(51, 218, 32, 255));
					ImGui::BeginTooltip();


					ImGui::Text("LeftTop: (%.2f, %.2f)", region_x, region_y);
					ImGui::Text("RightBottom: (%.2f, %.2f)", region_x + region_size_.x, region_y + region_size_.y);
					ImVec2 uv0 = ImVec2((region_x) / (float)atlas_->GetWidth(), (region_y) / (float)atlas_->GetHeight());
					ImVec2 uv1 = ImVec2((region_x + region_size_.x) / (float)atlas_->GetWidth(), (region_y + region_size_.y) / (float)atlas_->GetHeight());
					//ImGui::Image(atlas_->GetShaderResourceView(), ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
					ImGui::EndTooltip();
				}
				else
				{
					window_flags_ ^= ImGuiWindowFlags_NoMove;
				}
			}
		}
		else
		{

		}
		
	}
	ImGui::EndChild();
}

void AnimationTool::ShowTextureView()
{
}
