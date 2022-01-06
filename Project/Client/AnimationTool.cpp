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
#include <Engine/CSceneManager.h>
#include <limits>
//atlas가 없으면 에디터 버튼 제외 비활성화
//atlas가 생기면 그때 새 애니메이션 구성
//save를 누르면 애니메이터에 저장
//backboard size 지정하도록 생성
AnimationTool::AnimationTool()
	:Widget("animation_tool")
	, is_open_(false)
	, render_all_(false)
	, play_(false)
	, label_("AnimationTool")
	, zoom_(1.0f)
	, target_animator_(nullptr)
	, offset_tool_(nullptr)
	, atlas_tool_(nullptr)
	, is_update_(false)
	, window_flags_(0)
	, current_index_(0)
	, duration_(0.1f)
	,type_(ANIMATION_TOOL_TYPE::CREATE)
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
	if (CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		return; 
	if (nullptr != animation_&&ANIMATION_TOOL_TYPE::EDIT==type_&&!is_update_)
	{
		previous_index_ = 0;
		atlas_tool_->SetAtlas(animation_->GetAtlas());
		current_frame_ = animation_->GetCurrentFrameData();
		current_index_ = 0;
		duration_ = current_frame_.duration;
		atlas_tool_->SetMode(TOOL_TYPE::ANIMATOR);
		atlas_tool_->SetSelectedLeftTop(current_frame_.animation_data.left_top* Vec2(atlas_tool_->GetAtlas()->GetWidth(),atlas_tool_->GetAtlas()->GetHeight()));
		atlas_tool_->SetRegionSize(current_frame_.animation_data.size * Vec2(atlas_tool_->GetAtlas()->GetWidth(), atlas_tool_->GetAtlas()->GetHeight()));
		atlas_tool_->Activate();
		is_update_ = true;
	}
	if (ANIMATION_TOOL_TYPE::CREATE == type_)
	{
		if (nullptr != atlas_tool_->GetAtlas())
		{
			current_index_ = 0;
			previous_index_ = 0;
			animation_ = new CAnimation2D;
			animation_->SetName(L"animation");
			animation_->CreateFrame(atlas_tool_->GetAtlas(), 0, 0, 0, 0, 1, 0.1f);
			current_frame_ = animation_->GetCurrentFrameData();
			type_ = ANIMATION_TOOL_TYPE::EDIT;
			is_update_ = true;
		}
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
	if(!is_active_)
	{
		//Deactivate();
		atlas_tool_->Deactivate();
		animation_ = nullptr;
		is_update_ = false;
	}

	if (nullptr != animation_ )
	{
		if (ImGui::IsItemHovered())
		{
			animation_->SetCurrentFrameData(current_frame_);
		}
	}
		

	if (nullptr != animation_ && play_)
	{
		acc_time_ += fDT;

		if (current_frame_.duration < acc_time_)
		{
			++current_index_;
			if (animation_->GetMaxFrameCount() <= (UINT)current_index_)
			{
				current_index_ = 0;
				play_ = animation_->IsRepeat();
				acc_time_ = 0.f;
				animation_->SetCurrentFrame(current_index_);
				return;
			}
			current_frame_ = GetAnimationFrameData(current_index_);
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
		static bool opt_enable_show_collider = false;
		static bool adding_line = false;
		Ptr<CTexture> atlas = atlas_tool_->GetAtlas();
		float y = nullptr != animation_ && animation_->GetBackBoard().y>200 ? animation_->GetBackBoard().y + 100.f : 360;
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
			if (is_hovered)
			{
				if (KEY_HOLD(KEY::CTRL))
				{
					float wheel = io.MouseWheel;
					zoom_ += wheel * 0.1f;
				}
			}
			if (atlas_tool_->IsRelease())
			{
				current_frame_.animation_data.left_top = atlas_tool_->GetSelectedLeftTop();
				current_frame_.animation_data.size = atlas_tool_->GetRegionSize();
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
			
		
			if (nullptr != atlas)
			{
				float atlas_width = (float)atlas->GetWidth() * zoom_;
				float atlas_height = (float)atlas->GetHeight() * zoom_;
				float y = canvas_center.y + 10.f;
				draw_list->AddLine(ImVec2(canvas_p0.x, y), ImVec2(canvas_p1.x, y), IM_COL32(255, 0, 0, 255));
				draw_list->AddLine(ImVec2(canvas_p1.x - canvas_sz.x / 2.f, canvas_p0.y), ImVec2(canvas_p1.x - canvas_sz.x / 2.f, canvas_p1.y), IM_COL32(0, 0, 255, 255));
				ImVec2 fullRectLT = ImVec2(canvas_center.x - (current_frame_.animation_data.full_size.x * atlas_width) / 2.f,
					canvas_center.y - (current_frame_.animation_data.full_size.y * atlas_width) / 2.f);
				draw_list->AddRect(fullRectLT,
					ImVec2(fullRectLT.x + (current_frame_.animation_data.full_size.x * atlas_width),
						fullRectLT.y + (current_frame_.animation_data.full_size.y * atlas_height)),
					IM_COL32(32, 238, 52, 255));

				if (render_all_ && !play_)
					RenderAllFrame(draw_list, atlas, canvas_center, atlas_width, atlas_height);
				else
				{
					RenderFrame(draw_list, atlas, canvas_center, atlas_width, atlas_height, current_frame_);
				}

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
	ImGui::BeginChild("##animation_setting_panel", ImVec2(0, 360), true, 0);
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Animation");
		ImGui::Spacing();


		if (ImGui::BeginTable("##inspector", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
		{
			ImGui::TableNextColumn();
			ImGui::Text("Atlas");
			ImGui::TableNextColumn();
			if (ImGui::Button("open editor") && !atlas_tool_->IsActive())
			{
				atlas_tool_->SetMode(TOOL_TYPE::ANIMATOR);
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

			ImGui::Text("Zoom");
			ImGui::TableNextColumn();

			ImGui::SetNextItemWidth(50);
			if (ImGui::InputFloat("##zoom", &zoom_))
			{
				if (0 == zoom_)
				{
					zoom_ = 1.f;
				}
			}
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
	

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

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			
			Vec2 backBoardSize = animation_->GetBackBoard();
			ImGui::Text("Back Board Size");
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::InputFloat("##back_board_x", &backBoardSize.x))
			{
				animation_->SetBackBoard(backBoardSize);
			}

			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::InputFloat("##back_board_y", &backBoardSize.y))
			{
				animation_->SetBackBoard(backBoardSize);
			}


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
			int slilde = current_index_;
			if (ImGui::SliderInt("##current_frame", &slilde, 0, animation_->GetMaxFrameCount() - 1))
			{
				previous_index_ = current_index_;
				current_index_ = slilde;
				atlas_tool_->Clear();
				current_frame_ = GetAnimationFrameData(current_index_);
			}

			if (ImGui::Button("Add New Frame"))
			{
				previous_index_ = current_index_;
				animation_->SetCurrentFrameData(current_frame_);
				atlas_tool_->Clear();
				animation_->CreateFrame(atlas_tool_->GetAtlas(), 0, 0, 0, 0, 1, 0.1f);
				current_index_ = animation_->GetMaxFrameCount() - 1;
				current_frame_ = GetAnimationFrameData(current_index_);

			}
			if (ImGui::Button("Clear Frame"))
			{
				if(current_index_>0)
				{
					animation_->ClearFrame(current_index_);
					current_index_ = animation_->GetCurrentFrameIndex();
					current_frame_ = GetAnimationFrameData(current_index_);
				}
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();


			//Animation Offset
			ImGui::Text("OffSet");
			Vec2 offset = Vec2(current_frame_.animation_data.offset.x* atlas_tool_->GetAtlas()->GetWidth() * zoom_, current_frame_.animation_data.offset.y* atlas_tool_->GetAtlas()->GetWidth() * zoom_);
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::DragFloat("##offset_x", &offset.x, 0.01f))
			{
				current_frame_.animation_data.offset.x = offset.x / atlas_tool_->GetAtlas()->GetWidth()*zoom_;
				animation_->SetCurrentFrameData(current_frame_);
			}
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::DragFloat("##offset_y", &offset.y, 0.01f))
			{
				current_frame_.animation_data.offset.y = offset.y / atlas_tool_->GetAtlas()->GetHeight()*zoom_;
				animation_->SetCurrentFrameData(current_frame_);
			}
	
			//Collision Offset
			

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("duration");
			ImGui::TableNextColumn();
			if (ImGui::InputFloat("##duration", &duration_))
			{
				current_frame_.duration = duration_;
				animation_->SetCurrentFrameData(current_frame_);
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("Animation Setting");

			ImGui::TableNextColumn();
			
			ImGui::Text("Render All Frame");
			ImGui::SameLine();
			ImGui::Checkbox("##Render All Frame", &render_all_);
			
			bool check_repeat = animation_->IsRepeat();
			ImGui::Text("Repeat");
			ImGui::SameLine();
			if (ImGui::Checkbox("##repeat", &check_repeat))
			{
				animation_->SetRepeat(check_repeat);
			}

			ImGui::Text("Play On Start");
			ImGui::SameLine();

			bool check_playOnStart = animation_->IsPlayOnStart();
			if (ImGui::Checkbox("##play on start", &check_playOnStart))
			{
				animation_->SetPlayOnStart(check_playOnStart);
			}

			ImGui::Text("Flip Horizon");
			ImGui::SameLine();
			bool horizon = animation_->IsFlipHorizon();
			if (ImGui::Checkbox("##horizon", &horizon))
			{
				animation_->SetFlipHorizon(horizon);
			}

			ImGui::Text("Flip Vertical");
			ImGui::SameLine();

			bool vertical = animation_->IsFlipVertical();
			if (ImGui::Checkbox("##vertical", &vertical))
			{
				animation_->SetFlipVertical(vertical);
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

	};
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		play_ = false;
		current_index_ = 0;
		animation_->SetCurrentFrame(0);
	};
}



float AnimationTool::CalMaxSizeY()
{
	float minY = std::numeric_limits<float>::infinity();
	AnimationFrame data = {};
	for (UINT i = 0; i < animation_->GetMaxFrameCount(); ++i)
	{
		animation_->SetCurrentFrame(i);
		data = animation_->GetCurrentFrameData();
		if (data.animation_data.size.y < minY)
			minY = data.animation_data.size.y;
	}
	animation_->SetCurrentFrame(current_index_);
	return minY;
}

void AnimationTool::RenderAllFrame(ImDrawList* draw_list, Ptr<CTexture> atlas, ImVec2 canvas_center, float atlas_width, float atlas_height)
{
	AnimationFrame data = {};
	
	data = GetAnimationFrameData(previous_index_);
	RenderFrame(draw_list, atlas, canvas_center, atlas_width, atlas_height, data);
	draw_list->AddRectFilled(ImVec2(canvas_center.x - (data.animation_data.size.x * atlas_width) / 2.f + (data.animation_data.offset.x * atlas_width),
		canvas_center.y - (data.animation_data.size.y * atlas_height) / 2.f + (data.animation_data.offset.y * atlas_height)),
		ImVec2(canvas_center.x + (data.animation_data.size.x * atlas_width) / 2.f + (data.animation_data.offset.x * atlas_width),
			canvas_center.y + (data.animation_data.size.y * atlas_height) / 2.f + (data.animation_data.offset.y * atlas_height)), IM_COL32(249, 59, 97, 255 * 0.1));

	data = GetAnimationFrameData(current_index_);
	draw_list->AddRectFilled(ImVec2(canvas_center.x - (data.animation_data.size.x * atlas_width) / 2.f + (data.animation_data.offset.x * atlas_width),
		canvas_center.y - (data.animation_data.size.y * atlas_height) / 2.f + (data.animation_data.offset.y * atlas_height)),
		ImVec2(canvas_center.x + (data.animation_data.size.x * atlas_width) / 2.f + (data.animation_data.offset.x * atlas_width),
			canvas_center.y + (data.animation_data.size.y * atlas_height) / 2.f + (data.animation_data.offset.y * atlas_height)), IM_COL32(83, 255, 61, 255*0.1));
	RenderFrame(draw_list, atlas, canvas_center, atlas_width, atlas_height, data);
}

void AnimationTool::RenderFrame(ImDrawList* draw_list, Ptr<CTexture> atlas, ImVec2 canvas_center, float atlas_width, float atlas_height, AnimationFrame& data)
{
	ImVec2 uv0 = ImVec2(data.animation_data.left_top.x, data.animation_data.left_top.y);
	ImVec2 uv1 = ImVec2(data.animation_data.left_top.x + data.animation_data.size.x, data.animation_data.left_top.y + data.animation_data.size.y);

	if (animation_->IsFlipHorizon())
	{
		uv0.x = 1 - uv0.x;
		uv1.x = 1 - uv1.x;
	}
	if (animation_->IsFlipVertical())
	{
		uv0.y = 1 - uv0.y;
		uv1.y = 1 - uv1.y;
	}

	draw_list->AddImage(atlas->GetShaderResourceView(),
		ImVec2(canvas_center.x - (data.animation_data.size.x * atlas_width) / 2.f + (data.animation_data.offset.x* atlas_width),
			canvas_center.y - (data.animation_data.size.y * atlas_height) / 2.f + (data.animation_data.offset.y* atlas_height)),
		ImVec2(canvas_center.x + (data.animation_data.size.x * atlas_width) / 2.f + (data.animation_data.offset.x* atlas_width),
			canvas_center.y + (data.animation_data.size.y * atlas_height) / 2.f + (data.animation_data.offset.y* atlas_height)), uv0, uv1);
}

AnimationFrame AnimationTool::GetAnimationFrameData(UINT index)
{
	AnimationFrame data = {};
	AnimationFrame res = {};
	animation_->SetCurrentFrame(index);
	data = animation_->GetCurrentFrameData();
	res = data;
	res.animation_data.left_top += (data.animation_data.full_size - current_frame_.animation_data.size) / 2.f;

	return res;
}


