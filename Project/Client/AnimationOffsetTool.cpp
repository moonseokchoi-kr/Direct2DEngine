#include "pch.h"
#include "AnimationOffsetTool.h"

#include <Engine/CKeyManager.h>
#include <Engine/CAnimation2D.h>

AnimationOffsetTool::AnimationOffsetTool()
	:Widget("animation_offset_tool")
	,animation_(nullptr)
	,zoom_(1.f)
	,window_flags_(0)
{
}

AnimationOffsetTool::~AnimationOffsetTool()
{
}

void AnimationOffsetTool::Init()
{
}

void AnimationOffsetTool::Update()
{
	if (ImGui::Begin("OffSetTool", &is_active_, window_flags_) && nullptr != animation_ )
	{
		if (animation_->GetMaxFrameCount() != drag_pos_vector_.size())
		{
			drag_pos_vector_.resize(animation_->GetMaxFrameCount());
			current_index_ = 0;
			SetAnimationData();
		}
		ShowAnimationOffsetCanvasView();
		ImGui::SameLine();
		ShowDetailView();
		ImGui::End();
		
			
	}
	else
	{
		Deactivate();
		ImGui::End();
	}
}

void AnimationOffsetTool::ShowAnimationOffsetCanvasView()
{
	ImGui::Text("AnimationOffset View");
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.6f + 10);
	ImGui::Text("Offset Inspector");
	{
		// 		static ImVector<ImVec2> points;
		// 		static ImVec2 scrolling(0.0f, 0.0f);
		static bool opt_enable_grid = true;
		static bool opt_enable_context_menu = true;
		static bool adding_line = false;
		ImGuiIO& io = ImGui::GetIO();
		CTexture* atlas = animation_->GetAtlas();
		float atlas_width = (float)animation_->GetAtlas()->GetWidth();
		float atlas_height = (float)animation_->GetAtlas()->GetHeight();

		select_item_ = SelectItemWithMouse(io.MousePos);

		if (ImGui::BeginChild("##animation_view", ImVec2(ImGui::GetContentRegionAvail().x * 0.6f, 260), false))
		{
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
			ImVec2 canvas_center = ImVec2(canvas_p0.x + canvas_sz.x / 2.f, canvas_p0.y + canvas_sz.y / 2.f);
			

			// Draw border and background color
		
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			// This will catch our interactions
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held
			const ImVec2 origin(canvas_p0.x +20, canvas_p0.y + 30); // Lock scrolled origin
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

			if (is_hovered)
			{
				window_flags_ |= ImGuiWindowFlags_NoMove;
				if (KEY_HOLD(KEY::CTRL))
				{
					float wheel = io.MouseWheel;
					zoom_ += wheel * 0.1f;
					
				}
				if (select_item_)
				{
					if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
					{
						drag_pos_vector_[current_index_].x += io.MouseDelta.x;
						drag_pos_vector_[current_index_].y += io.MouseDelta.y;
					}
					if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
					{
						current_frame_.animation_data.offset = Vec2(offset_.x / atlas_width, offset_.x / atlas_height);
						animation_->SetCurrentFrameData(current_frame_);
					}
				}


			}
			else
			{
				window_flags_ ^= ImGuiWindowFlags_NoMove;
			}
			//Context menu (under default mouse threshold)
			ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
			if (opt_enable_context_menu && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
				ImGui::OpenPopupOnItemClick("context");

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
			if (nullptr != animation_)
			{
				Vec2 full_size = animation_->GetBackBoard();
				Vec2 fullRectLt = Vec2(canvas_center.x - full_size.x / 2.f * zoom_, canvas_center.y - full_size.y / 2.f * zoom_);
				Vec2 fullRectRb = Vec2(canvas_center.x + full_size.x / 2.f * zoom_, canvas_center.y + full_size.y / 2.f * zoom_);
				draw_list->AddRectFilled(fullRectLt, fullRectRb, IM_COL32(255, 30, 32, 180));
				//0번 일때는 이전 프레임 X
				//그 다음 부터는 이전 프레임 같이 플레이팅
				ImVec2 uv0 = ImVec2(previous_frame_.animation_data.left_top.x, previous_frame_.animation_data.left_top.y);
				ImVec2 uv1 = ImVec2(previous_frame_.animation_data.left_top.x + previous_frame_.animation_data.size.x, previous_frame_.animation_data.left_top.y + previous_frame_.animation_data.size.y);
				if(current_index_)
				{
					draw_list->AddImage(atlas->GetShaderResourceView(),
						ImVec2(canvas_p0.x + drag_pos_vector_[current_index_ - 1].x + (previous_frame_.animation_data.offset.x * atlas_width),
							canvas_p0.y + drag_pos_vector_[current_index_ - 1].y + (previous_frame_.animation_data.offset.y * atlas_height)),
						ImVec2(canvas_p0.x + drag_pos_vector_[current_index_ - 1].x + (previous_frame_.animation_data.size.x * atlas_width * zoom_) + (previous_frame_.animation_data.offset.x * atlas_width),
							canvas_p0.y + drag_pos_vector_[current_index_ - 1].y + (previous_frame_.animation_data.size.y * atlas_height * zoom_) + (previous_frame_.animation_data.offset.y * atlas_height)), uv0, uv1);
				}

				uv0 = ImVec2(current_frame_.animation_data.left_top.x, current_frame_.animation_data.left_top.y);
				uv1 = ImVec2(current_frame_.animation_data.left_top.x + current_frame_.animation_data.size.x, current_frame_.animation_data.left_top.y + current_frame_.animation_data.size.y);

				draw_list->AddImage(atlas->GetShaderResourceView(),
					ImVec2(canvas_p0.x +drag_pos_vector_[current_index_].x+ (current_frame_.animation_data.offset.x * atlas_width),
						canvas_p0.y+ drag_pos_vector_[current_index_].y + (current_frame_.animation_data.offset.y * atlas_height)),
					ImVec2(canvas_p0.x + drag_pos_vector_[current_index_].x + (current_frame_.animation_data.size.x * atlas_width*zoom_) + (current_frame_.animation_data.offset.x * atlas_width),
						canvas_p0.y + drag_pos_vector_[current_index_].y + (current_frame_.animation_data.size.y * atlas_height*zoom_) + (current_frame_.animation_data.offset.y * atlas_height)), uv0, uv1);
				Vec2 frameBottonCenter = Vec2(canvas_p0.x + drag_pos_vector_[current_index_].x + (current_frame_.animation_data.size.x * atlas_height * zoom_) / 2.f + (current_frame_.animation_data.offset.x * atlas_height), canvas_p0.y + drag_pos_vector_[current_index_].y + (current_frame_.animation_data.size.y * atlas_height * zoom_) + (current_frame_.animation_data.offset.y * atlas_height));
				

				if (select_item_ && drag_pos_vector_[current_index_].x !=0 && drag_pos_vector_[current_index_].y!=0)
					CalOffset(frameBottonCenter,Vec2(canvas_center.x,canvas_center.y),Vec2(canvas_p0.x,canvas_p0.y));
				//중점용 이전 프레임 하단 라인
				draw_list->AddCircleFilled(canvas_center, 3.f, IM_COL32(32, 238, 52, 255));
				draw_list->AddCircleFilled(frameBottonCenter, 2.f, IM_COL32(0, 32, 255, 255));
			}

			draw_list->PopClipRect();

			ImGui::EndChild();
		}
	}
}

void AnimationOffsetTool::ShowDetailView()
{
	float atlas_width = (float)animation_->GetAtlas()->GetWidth();
	float atlas_height = (float)animation_->GetAtlas()->GetHeight();
	ImGui::BeginChild("##animation_setting_panel", ImVec2(0, 260), true, 0);
	{
		ImGui::AlignTextToFramePadding();

		if (ImGui::BeginTable("##inspector", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
		{
			ImGui::TableNextColumn();

			ImGui::Text("Current Frame");
			ImGui::TableNextColumn();
			if (ImGui::SliderInt("##current_frame", &current_index_, 0, animation_->GetMaxFrameCount() - 1))
			{
				animation_->SetCurrentFrame(current_index_);
				SetAnimationData();
				
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("OffSet");
			ImGui::TableNextColumn();

			if (ImGui::DragFloat("##offset_x", &offset_.x, 0.001f, 0, 0))
			{
				current_frame_.animation_data.offset.x = offset_.x / atlas_width;
				animation_->SetCurrentFrameData(current_frame_);
			}

			if(ImGui::DragFloat("##offset_y", &offset_.y, 0.0001f, 0, 0))
			{
				current_frame_.animation_data.offset.y = offset_.y/atlas_height;
				animation_->SetCurrentFrameData(current_frame_);
			}
			

			ImGui::EndTable();
		}
	}
	ImGui::EndChild();
}

void AnimationOffsetTool::CalOffset(Vec2 frameBottomCenter, Vec2 canvas_center, Vec2 canvas_lt)
{
	float width = (float)animation_->GetAtlas()->GetWidth();
	float height = (float)animation_->GetAtlas()->GetHeight();
	Vec2 full_size = animation_->GetBackBoard();

	Vec2 fullRectLt = Vec2(canvas_center.x - full_size.x / 2.f*zoom_, canvas_center.y - full_size.y / 2.f*zoom_);
	Vec2 fullRectRb = Vec2(canvas_center.x + full_size.x / 2.f * zoom_, canvas_center.y + full_size.y / 2.f * zoom_);
	Vec2 offset = Vec2(current_frame_.animation_data.offset.x * width, current_frame_.animation_data.offset.y * height);
	Vec2 size = Vec2(current_frame_.animation_data.size.x * width * zoom_, current_frame_.animation_data.size.y * height * zoom_);
	Vec2 leftTop = Vec2(canvas_lt.x+drag_pos_vector_[current_index_].x+offset.x , canvas_lt.y + drag_pos_vector_[current_index_].y + offset.y);
	Vec2 center = Vec2(leftTop.x + size.x / 2.f, leftTop.y + size.y / 2.f);

	if (fullRectLt.x< center.x && center.x< fullRectRb.x
		&& fullRectLt.y < center.y && center.y < fullRectRb.y)
	{
		offset_ = frameBottomCenter - canvas_center;
	}
	else
	{
		current_frame_.animation_data.offset = Vec2();
	}
}

void AnimationOffsetTool::SetAnimationData()
{
	float width = (float)animation_->GetAtlas()->GetWidth();
	float height = (float)animation_->GetAtlas()->GetHeight();
	if (animation_->GetCurrentFrameIndex() == 0)
	{
		previous_frame_ = AnimationFrame{};
		current_index_ = animation_->GetCurrentFrameIndex();
		current_frame_ = animation_->GetCurrentFrameData();
		current_frame_.animation_data.left_top += (current_frame_.animation_data.full_size - current_frame_.animation_data.size) / 2.f;
		
	}
	else
	{
		current_index_ = animation_->GetCurrentFrameIndex();
		current_frame_ = animation_->GetCurrentFrameData();
		current_frame_.animation_data.left_top += (current_frame_.animation_data.full_size - current_frame_.animation_data.size) / 2.f;
		animation_->SetCurrentFrame(current_index_- 1);
		previous_frame_ = animation_->GetCurrentFrameData();
		previous_frame_.animation_data.left_top += (previous_frame_.animation_data.full_size - previous_frame_.animation_data.size) / 2.f;
	}
	offset_ = Vec2(current_frame_.animation_data.offset.x * width, current_frame_.animation_data.offset.y * height);
}

bool AnimationOffsetTool::SelectItemWithMouse(ImVec2 mouse_pos)
{
	float width = (float)animation_->GetAtlas()->GetWidth();
	float height = (float)animation_->GetAtlas()->GetHeight();
	Vec2 full_size = animation_->GetBackBoard();
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	Vec2 canvas_center = Vec2(canvas_p0.x + canvas_sz.x / 2.f, canvas_p0.y + canvas_sz.y / 2.f);

	Vec2 fullRectLt = Vec2(canvas_center.x - full_size.x / 2.f * zoom_, canvas_center.y - full_size.y / 2.f * zoom_);
	Vec2 fullRectRb = fullRectLt + full_size;
	Vec2 offset = Vec2(current_frame_.animation_data.offset.x * width, current_frame_.animation_data.offset.y * height);
	Vec2 leftTop = Vec2(canvas_p0.x + drag_pos_vector_[current_index_].x+offset.x, canvas_p0.y + drag_pos_vector_[current_index_].y + offset.y);
	Vec2 size = Vec2(current_frame_.animation_data.size.x * width * zoom_, current_frame_.animation_data.size.y * height * zoom_);

	
	if (leftTop.x < mouse_pos.x && mouse_pos.x < leftTop.x + size.x
		&& leftTop.y < mouse_pos.y && mouse_pos.y < leftTop.y + size.y)
	{
		return true;
	}


	return false;
}

