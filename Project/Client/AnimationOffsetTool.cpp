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
	if (ImGui::Begin("OffSetTool", &is_active_, window_flags_) && nullptr != animation_)
	{
		ShowAnimationOffsetCanvasView();
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
		static ImVec2 scrolling(0.0f, 0.0f);
		static bool adding_line = false;
		ImGuiIO& io = ImGui::GetIO();
		CTexture* atlas = animation_->GetAtlas();
		float atlas_width = (float)animation_->GetAtlas()->GetWidth();
		float atlas_height = (float)animation_->GetAtlas()->GetHeight();



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
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					
				}
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
				{
					scrolling.x += io.MouseDelta.x;
					scrolling.y += io.MouseDelta.y;
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
				for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
				for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));

			}
			if (nullptr != animation_)
			{
				ImVec2 full_size = animation_->GetBackBoard();
				draw_list->AddRectFilled(ImVec2((canvas_center.x - full_size.x / 2.f*zoom_), (canvas_center.y - full_size.y / 2.f* zoom_)), ImVec2((canvas_center.x + full_size.x / 2.f* zoom_), (canvas_center.y + full_size.y / 2.f* zoom_)), IM_COL32(255, 30, 32, 180));
				//0번 일때는 이전 프레임 X
				//그 다음 부터는 이전 프레임 같이 플레이팅
// 				ImVec2 uv0 = ImVec2(current_frame_.animation_data.left_top.x, current_frame_.animation_data.left_top.y);
// 				ImVec2 uv1 = ImVec2(current_frame_.animation_data.left_top.x + current_frame_.animation_data.size.x, current_frame_.animation_data.left_top.y + current_frame_.animation_data.size.y);
// 
// 				draw_list->AddImage(atlas->GetShaderResourceView(),
// 					ImVec2(canvas_center.x - (current_frame_.animation_data.size.x * atlas_width) / 2.f + (current_frame_.animation_data.offset.x * atlas_width),
// 						canvas_center.y - (current_frame_.animation_data.size.y * atlas_height) / 2.f + (current_frame_.animation_data.offset.y * atlas_height)),
// 					ImVec2(canvas_center.x + (current_frame_.animation_data.size.x * atlas_width) / 2.f + (current_frame_.animation_data.offset.x * atlas_width),
// 						canvas_center.y + (current_frame_.animation_data.size.y * atlas_height) / 2.f + (current_frame_.animation_data.offset.y * atlas_height)), uv0, uv1);

			}
			//중점용 이전 프레임 하단 라인
			draw_list->AddCircleFilled(canvas_center, 3.f, IM_COL32(32, 238, 52, 255));
			draw_list->PopClipRect();
			ImGui::EndChild();
		}
	}
}

void AnimationOffsetTool::ShowDetailView()
{
}

void AnimationOffsetTool::CalOffset(AnimationFrame& data)
{
	float width = (float)animation_->GetAtlas()->GetWidth();
	float height = (float)animation_->GetAtlas()->GetHeight();
	Vec2 full_size = animation_->GetBackBoard();
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	Vec2 canvas_center = Vec2(canvas_p0.x + canvas_sz.x / 2.f, canvas_p0.y + canvas_sz.y / 2.f);

	Vec2 fullRectLt = Vec2(canvas_center.x - full_size.x / 2.f, canvas_center.y - full_size.y / 2.f);
	Vec2 fullRectRb = fullRectLt + full_size;
	Vec2 leftTop = Vec2(data.animation_data.left_top.x * width, data.animation_data.left_top.y * height);
	Vec2 size = Vec2(data.animation_data.size.x * width, data.animation_data.size.y * height);
	Vec2 frameBottonCenter = Vec2(leftTop.x + size.x / 2.f, leftTop.x + size.y);

	if (fullRectLt.x < leftTop.x && leftTop.x + size.x < fullRectRb.x
		&& fullRectLt.y < leftTop.y && leftTop.y + size.y < fullRectRb.y)
	{
		Vec2 offset = canvas_center - frameBottonCenter;
		data.animation_data.offset = Vec2(offset.x / width, offset.y / height);
	}
	else
	{
		data.animation_data.offset = Vec2();
	}
}

int AnimationOffsetTool::SelectItemWithMouse(vector<AnimationFrame> anims)
{
	//현재 프레임의 좌상단과 마우스를 비교하여 선택 후 이동가능하도록 설정

	return 0;
}
