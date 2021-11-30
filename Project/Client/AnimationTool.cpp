#include "pch.h"
#include "AnimationTool.h"
#include "imgui_internal.h"
#include "WidgetManager.h"
#include "AtlasTextureTool.h"
#include "AnimationOffsetTool.h"

#include <Engine/CResourceManager.h>
#include <Engine/CKeyManager.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
//atlas가 없으면 에디터 버튼 제외 비활성화
//atlas가 생기면 그때 새 애니메이션 구성
//save를 누르면 애니메이터에 저장
AnimationTool::AnimationTool()
	:Widget("animation_tool")
	, is_open_(false)
	, menu_selected_(false)
	, play_(false)
	, label_("AnimationTool")
	, zoom_(1.0f)
	, target_animator_(nullptr)
	, offset_tool_(nullptr)
	, atlas_tool_(nullptr)
	, window_flags_(0)
	, current_index_(0)
{
}

AnimationTool::~AnimationTool()
{
}

void AnimationTool::Init()
{
	atlas_tool_ = dynamic_cast<AtlasTextureTool*>(WidgetManager::GetInst()->FindWidget("atlas_editor"));
	offset_tool_ = dynamic_cast<AnimationOffsetTool*>(WidgetManager::GetInst()->FindWidget("animation_offset_tool"));
	window_flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
}

void AnimationTool::Update()
{
	ImGui::SetNextWindowSize(ImVec2(1400, 600), ImGuiCond_FirstUseEver);

	if (nullptr != atlas_tool_->GetAtlas() && nullptr == animation_)
	{
		animation_ = new CAnimation2D;
		animation_->SetName(L"animation");
		animation_->CreateFrame(atlas_tool_->GetAtlas(), 0, 0, 0, 0, 1, 0.1f);
		current_frame_ = animation_->GetCurrentFrameData();
		atlas_tool_->SetBackBoardSize(animation_->GetBackBoard());
	}
	if (ImGui::Begin(label_.c_str(), &is_active_, window_flags_))
	{
		ShowMenuBar();
		ShowAnimationEditWidget();
		ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.6f + 10);
		ShowAnimationDetailSettingPanel();
		ShowPlayButton();
		ImGui::End();
	}
	else
	{
		Deactivate();
		ImGui::End();
	}

	if (nullptr != animation_ && ImGui::IsWindowFocused())
		animation_->SetCurrentFrameData(current_frame_);
	if (nullptr != animation_ && play_)
	{
		acc_time_ += fDT;

		if (current_frame_.duration < acc_time_)
		{
			++current_index_;
			animation_->SetCurrentFrame(current_index_);

			if (animation_->GetMaxFrameCount() <= (UINT)current_index_)
			{
				current_index_ = 0;
				play_ = animation_->IsRepeat();
				acc_time_ = 0.f;
				animation_->SetCurrentFrame(current_index_);
				return;
			}
			current_frame_ = animation_->GetCurrentFrameData();
			current_frame_.animation_data.left_top += (current_frame_.animation_data.full_size - current_frame_.animation_data.size) / 2.f;
			acc_time_ -= current_frame_.duration;
		}
	}
}

void AnimationTool::ShowMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "CTRL+S"))
			{
				target_animator_->AddAnimation(animation_);
			}

			if (ImGui::MenuItem("Load", "CTRL+O"))
			{
				int a = 0;
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
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.6f + 10);
	ImGui::Text("Animation Inspector");
	{
		// 		static ImVector<ImVec2> points;
		// 		static ImVec2 scrolling(0.0f, 0.0f);
		static bool opt_enable_grid = true;
		static bool opt_enable_context_menu = true;
		static bool adding_line = false;
		Ptr<CTexture> atlas = atlas_tool_->GetAtlas();
		float y = nullptr != animation_ ? animation_->GetBackBoard().y + 100.f : 360;
		if (ImGui::BeginChild("##animation_view", ImVec2(ImGui::GetContentRegionAvail().x * 0.6f, y), false))
		{
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
			static ImVec2 frame_center = ImVec2();
			if (atlas_tool_->IsRelease())
			{
				current_frame_.animation_data.left_top = atlas_tool_->GetSelectedLeftTop();
				current_frame_.animation_data.size = atlas_tool_->GetRegionSize();
				frame_center = ImVec2(current_frame_.animation_data.size.x / 2.f, current_frame_.animation_data.size.y / 2.f);
				current_frame_.animation_data.left_top = Vec2(current_frame_.animation_data.left_top.x / (float)atlas->GetWidth(), current_frame_.animation_data.left_top.y / (float)atlas->GetHeight());
				current_frame_.animation_data.size = Vec2(current_frame_.animation_data.size.x / (float)atlas->GetWidth(), current_frame_.animation_data.size.y / (float)atlas->GetHeight());
				
				animation_->SetCurrentFrameData(current_frame_);
			}
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
			
			if (nullptr != atlas)
			{
				float atlas_width = (float)atlas->GetWidth();
				float atlas_height = (float)atlas->GetHeight();
				ImVec2 fullRectLT = ImVec2(canvas_center.x - (current_frame_.animation_data.full_size.x * atlas_width) / 2.f,
					canvas_center.y - (current_frame_.animation_data.full_size.y * atlas_width) / 2.f);
				draw_list->AddRect(fullRectLT,
					ImVec2(fullRectLT.x + (current_frame_.animation_data.full_size.x * atlas_width),
						fullRectLT.y + (current_frame_.animation_data.full_size.y * atlas_height)),
					IM_COL32(32, 238, 52, 255));
				ImVec2 uv0 = ImVec2(current_frame_.animation_data.left_top.x, current_frame_.animation_data.left_top.y);
				ImVec2 uv1 = ImVec2(current_frame_.animation_data.left_top.x + current_frame_.animation_data.size.x, current_frame_.animation_data.left_top.y + current_frame_.animation_data.size.y);
				
				draw_list->AddImage(atlas->GetShaderResourceView(),
					ImVec2(canvas_center.x - (current_frame_.animation_data.size.x * atlas_width) / 2.f + (current_frame_.animation_data.offset.x * atlas_width),
						canvas_center.y - (current_frame_.animation_data.size.y * atlas_height) / 2.f + (current_frame_.animation_data.offset.y * atlas_height)),
					ImVec2(canvas_center.x + (current_frame_.animation_data.size.x * atlas_width) / 2.f + (current_frame_.animation_data.offset.x * atlas_width),
						canvas_center.y + (current_frame_.animation_data.size.y * atlas_height) / 2.f + (current_frame_.animation_data.offset.y * atlas_height)), uv0, uv1);
				
			}
			//중점용 이전 프레임 하단 라인
			draw_list->AddCircle(canvas_center, 2.f, IM_COL32(32, 238, 52, 255));
			draw_list->PopClipRect();
			ImGui::EndChild();
		}
	}

}

void AnimationTool::ShowAnimationDetailSettingPanel()
{
	ImGui::BeginChild("##animation_setting_panel", ImVec2(0, 260), true, 0);
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Animation");
		ImGui::Spacing();


		if (ImGui::BeginTable("##inspector", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
		{
			ImGui::TableNextColumn();
			ImGui::Text("Atlas");
			ImGui::TableNextColumn();
			if (ImGui::Button("open editor") && !atlas_tool_->IsOpen())
			{
				atlas_tool_->Activate();
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			if (nullptr == animation_)
			{
				ImGui::EndTable();
				ImGui::EndChild();
				return;
			}
			array<char, 256> szbuffer = { 0, };
			WideCharToMultiByte(CP_ACP, 0, animation_->GetName().c_str(), (int)animation_->GetName().size(), szbuffer.data(), (int)szbuffer.size(), nullptr, nullptr);
			ImGui::Text("Animation Name");
			ImGui::TableNextColumn();
			if (ImGui::InputTextWithHint("##animation name", "input animation name", szbuffer.data(), (int)szbuffer.size()))
			{
				array<wchar_t, 256> animName = { 0, };
				MultiByteToWideChar(CP_ACP, 0, szbuffer.data(), (int)szbuffer.size(), animName.data(), (int)animName.size());
				animation_->SetName(animName.data());
			}

			// 			if (ImGui::Button("Clear"))
			// 			{
			// 				animation_->Clear();
			// 			}

			if (nullptr == atlas_tool_->GetAtlas())
			{
				ImGui::EndTable();
				ImGui::EndChild();
				return;
			}


			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("Current Frame");
			ImGui::TableNextColumn();
			if (ImGui::SliderInt("##current_frame", &current_index_, 0, animation_->GetMaxFrameCount() - 1))
			{
				animation_->SetCurrentFrame(current_index_);
				atlas_tool_->Clear();
				current_frame_ = animation_->GetCurrentFrameData();
				current_frame_.animation_data.left_top += (current_frame_.animation_data.full_size - current_frame_.animation_data.size) / 2.f;
			}

			if (ImGui::Button("Add New Frame"))
			{
				animation_->SetCurrentFrameData(current_frame_);
				atlas_tool_->Clear();
				animation_->CreateFrame(atlas_tool_->GetAtlas(), 0, 0, 0, 0, 1, 0.1f);
				current_index_ = animation_->GetMaxFrameCount() - 1;
				animation_->SetCurrentFrame(current_index_);
				current_frame_ = animation_->GetCurrentFrameData();
				current_frame_.animation_data.left_top += (current_frame_.animation_data.full_size - current_frame_.animation_data.size) / 2.f;

			}
			if (ImGui::Button("Clear Frame"))
			{
				animation_->ClearFrame(current_index_);
				current_index_ = animation_->GetCurrentFrameIndex();
				animation_->SetCurrentFrame(current_index_);
				current_frame_ = animation_->GetCurrentFrameData();
				current_frame_.animation_data.left_top += (current_frame_.animation_data.full_size - current_frame_.animation_data.size) / 2.f;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("OffSet");
			Vec2 offset = Vec2(current_frame_.animation_data.offset.x * atlas_tool_->GetAtlas()->GetWidth(), current_frame_.animation_data.offset.y * atlas_tool_->GetAtlas()->GetHeight());
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::DragFloat("##offset_x", &offset.x, 0.01f))
			{
				current_frame_.animation_data.offset.x = offset.x / atlas_tool_->GetAtlas()->GetWidth();
				animation_->SetCurrentFrameData(current_frame_);
			}
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::DragFloat("##offset_y", &offset.y, 0.01f))
			{
				current_frame_.animation_data.offset.y = offset.y / atlas_tool_->GetAtlas()->GetHeight();
				animation_->SetCurrentFrameData(current_frame_);
			}
			if (ImGui::Button("open offset editor"))
			{
				offset_tool_->SetAnimation(animation_);
				offset_tool_->Activate();
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("duration");
			ImGui::TableNextColumn();
			if (ImGui::InputFloat("##duration", &current_frame_.duration))
			{
				animation_->SetCurrentFrameData(current_frame_);
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("Animation Setting");

			ImGui::TableNextColumn();

			bool check_repeat = animation_->IsRepeat();
			if (ImGui::Checkbox("repeat", &check_repeat))
			{
				animation_->SetRepeat(check_repeat);
			}
			ImGui::SameLine();

			bool check_playOnStart = animation_->IsPlayOnStart();
			if (ImGui::Checkbox("play on start", &check_playOnStart))
			{
				animation_->SetPlayOnStart(check_playOnStart);
			}

			ImGui::EndTable();
		}
	}
	ImGui::EndChild();
}

void AnimationTool::ShowPlayButton()
{
	if (ImGui::Button("Play") && !play_)
	{
		play_ = true;
		current_index_ = 0;
		animation_->SetCurrentFrame(0);
		current_frame_ = animation_->GetCurrentFrameData();
		current_frame_.animation_data.left_top += (current_frame_.animation_data.full_size - current_frame_.animation_data.size) / 2.f;
		current_frame_.animation_data.offset.x = current_frame_.animation_data.offset.x * (float)atlas_tool_->GetAtlas()->GetWidth();
		current_frame_.animation_data.offset.y = current_frame_.animation_data.offset.y * (float)atlas_tool_->GetAtlas()->GetHeight();
	};
	if (ImGui::Button("Stop"))
	{
		play_ = false;
		current_index_ = 0;
		animation_->SetCurrentFrame(0);
	};
}


