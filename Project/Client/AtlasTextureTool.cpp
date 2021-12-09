#include "pch.h"
#include "AtlasTextureTool.h"
#include "imgui_internal.h"
#include "ModalListWidget.h"
#include "WidgetManager.h"

#include <Engine/CKeyManager.h>
#include <Engine/CResourceManager.h>
AtlasTextureTool::AtlasTextureTool()
	:Widget("atlas_editor")
	, zoom_(1.0f)
	, window_flags_(0)
	, release_mouse_(false)
	, mode_(TOOL_TYPE::TILEMAP)
{
}

AtlasTextureTool::~AtlasTextureTool()
{
}

void AtlasTextureTool::Init()
{
	atlas_ = nullptr;
	window_flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
	region_size_ = Vec2(64, 64);
	back_board_size_ = Vec2(100, 100);


}

void AtlasTextureTool::Update()
{
	// Child 2: rounded border
	ImGui::SetNextWindowSize(ImVec2(0, 512));
	if (ImGui::Begin("Atlas Editor", &is_active_, window_flags_))
	{
		ShowMenuBar();

		if (nullptr != atlas_)
		{
			ImGui::Text("Texture Size");
			ImGui::SameLine(100);
			ImGui::Text("%d x %d", atlas_->GetWidth(), atlas_->GetHeight());
		}

		if (mode_ == TOOL_TYPE::ANIMATOR)
		{
			ShowAnimatorMenu();
		}
		else
		{
			ShowTileMapMenu();
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

void AtlasTextureTool::ChangeTexture(DWORD_PTR instance, DWORD_PTR textureName)
{
	ModalListWidget* widget = (ModalListWidget*)instance;
	string strName = widget->GetSelectedName();
	wchar_t szName[256] = {};
	MultiByteToWideChar(CP_ACP, 0, strName.c_str(), (int)strName.size(), szName, 256);
	atlas_ = CResourceManager::GetInst()->FindRes<CTexture>(szName);
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
				ModalListWidget* listWidget = dynamic_cast<ModalListWidget*>(WidgetManager::GetInst()->FindWidget("modal_list_widget"));

				array<char, 255> szBuffer = { 0, };
				const unordered_map<wstring, CResource*>& textureMap = CResourceManager::GetInst()->GetResource<CTexture>();
				int count = 0;
				for (const auto& pair : textureMap)
				{
					if (!listWidget->isOpen())
					{
						WideCharToMultiByte(CP_ACP, 0, pair.first.c_str(), -1, szBuffer.data(), (int)pair.first.size(), nullptr, nullptr);
						listWidget->AddItem(szBuffer.data());
						if (nullptr != atlas_ && atlas_->GetName() == pair.first)
							listWidget->SetCurrentIndex(count);
						szBuffer.fill(0);
						++count;
					}
				}

				listWidget->SetCaption("Select Texture");
				listWidget->SetCallbackFunc(this, (MODAL_LIST_CALLBACK)&AtlasTextureTool::ChangeTexture);
				listWidget->Activate();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void AtlasTextureTool::ShowAnimatorMenu()
{
	if (ImGui::BeginTable("##rectSize", 2, ImGuiTableFlags_Resizable))
	{


		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		ImGui::Text("Rect Size");
		ImGui::TableNextColumn();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		ImGui::InputFloat("##rect_x", &region_size_.x);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		ImGui::InputFloat("##rect_y", &region_size_.y);



		ImGui::EndTable();
	}
}

void AtlasTextureTool::ShowTileMapMenu()
{
	ImGui::Text("Tile Size");
	ImGui::SameLine(120);
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::InputFloat("##rect_x", &region_size_.x);
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::InputFloat("##rect_y", &region_size_.y);
}

void AtlasTextureTool::ShowAtlasVeiw()
{
	ImGuiIO& io = ImGui::GetIO();
	canvas_lt_ = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
	canvas_size_ = Vec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	if (canvas_size_.x < 50.0f) canvas_size_.x = 50.0f;
	if (canvas_size_.y < 50.0f) canvas_size_.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_lt_.x + canvas_size_.x, canvas_lt_.y + canvas_size_.y);
	ImVec2 canvas_center = ImVec2(canvas_lt_.x + canvas_size_.x / 2.f, canvas_lt_.y + canvas_size_.y / 2.f);
	// Draw border and background color
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_lt_, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_lt_, canvas_p1, IM_COL32(255, 255, 255, 255));
	ImGui::InvisibleButton("canvas", canvas_size_, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	static bool opt_enable_grid = true;

	if (nullptr != atlas_)
	{
		{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
			float region_x = current_mouse_pos_.x - canvas_lt_.x-region_size_.x/2.f;
			float region_y = current_mouse_pos_.y - canvas_lt_.y-region_size_.y/2.f;
			float width = (float)atlas_->GetWidth() * zoom_;
			float height = (float)atlas_->GetHeight() * zoom_;

			ImGui::SetItemUsingMouseWheel();
			if (ImGui::IsItemHovered())
			{
				if (mode_ == TOOL_TYPE::ANIMATOR)
					AnimationMouseMode(region_x,region_y);
				else
					TileMapMouseMode();


				ImGui::BeginTooltip();

				if (region_x < 0.0f) { region_x = 0.0f; }
				else if (region_x > (float)atlas_->GetWidth() - region_size_.x) { region_x = (float)atlas_->GetWidth() - region_size_.x; }
				if (region_y < 0.0f) { region_y = 0.0f; }
				else if (region_y > (float)atlas_->GetHeight() - region_size_.y) { region_y = (float)atlas_->GetHeight() - region_size_.y; }
				ImGui::Text("LeftTop: (%.2f, %.2f)", region_x, region_y);
				ImGui::Text("RightBottom: (%.2f, %.2f)", region_x + region_size_.x, region_y + region_size_.y);
				ImGui::Text("index: %d",current_tile_index_);
				ImGui::EndTooltip();
			}
			else
			{
				release_mouse_ = false;
				window_flags_ ^= ImGuiWindowFlags_NoMove;
			}


			draw_list->PushClipRect(canvas_lt_, canvas_p1, true);
			{
				draw_list->AddImage(atlas_->GetShaderResourceView(), ImVec2(canvas_lt_.x + scrolling_.x, canvas_lt_.y + scrolling_.y),
					ImVec2(canvas_lt_.x + scrolling_.x + width, canvas_lt_.y + scrolling_.y + height), uv_min, uv_max);
				if (mode_ == TOOL_TYPE::ANIMATOR)
					draw_list->AddRect(ImVec2(current_mouse_pos_.x - region_size_.x / 2.f * zoom_, current_mouse_pos_.y - region_size_.y / 2.f * zoom_), ImVec2(current_mouse_pos_.x + region_size_.x / 2.f * zoom_, current_mouse_pos_.y + region_size_.y / 2.f * zoom_), IM_COL32(51, 218, 32, 255));
			}
			if (mode_ == TOOL_TYPE::TILEMAP)
			{
				if (opt_enable_grid)
				{
					for (float x = fmodf(scrolling_.x, region_size_.x); x <= atlas_->GetWidth(); x += region_size_.x)
						draw_list->AddLine(ImVec2(canvas_lt_.x + x, canvas_lt_.y), ImVec2(canvas_lt_.x + x, canvas_lt_.y + atlas_->GetHeight()), IM_COL32(51, 218, 32, 255));
					for (float y = fmodf(scrolling_.y, region_size_.y); y <= atlas_->GetHeight(); y += region_size_.y)
						draw_list->AddLine(ImVec2(canvas_lt_.x, canvas_lt_.y + y), ImVec2(canvas_lt_.x + atlas_->GetWidth(), canvas_lt_.y + y), IM_COL32(51, 218, 32, 255));

				}
				int x = atlas_->GetWidth() / (UINT)region_size_.x;

				int col = current_tile_index_ % x;
				int row = current_tile_index_ / x;
				draw_list->AddRectFilled(ImVec2(canvas_lt_.x+scrolling_.x + region_size_.x * col, canvas_lt_.y+ scrolling_.y + region_size_.y * row),
					ImVec2(canvas_lt_.x+scrolling_.x + region_size_.x * (col+1), canvas_lt_.y+ scrolling_.y + region_size_.y * (row+1)), IM_COL32(255, 0, 0, 40));
			}

			draw_list->PopClipRect();
			



		}
	}
	else
	{

	}
}

void AtlasTextureTool::CalculateIndex()
{
	UINT tileCountX = atlas_->GetWidth() / (UINT)region_size_.x;
	UINT tileCountY = atlas_->GetHeight() / (UINT)region_size_.y;
	Vec2 mousePos = current_mouse_pos_ - canvas_lt_ - scrolling_;
	for (UINT i = 0; i < tileCountX; ++i)
		for(UINT j=0; j< tileCountY; ++j)
	{
		if (region_size_.x*i< mousePos.x&& mousePos.x<region_size_.x*(i+1) &&
			+region_size_.y*j< mousePos.y&& mousePos.y<region_size_.y*(j+1))
		{
			current_tile_index_ = j * tileCountX + i;
		}
	}

}

void AtlasTextureTool::AnimationMouseMode(float region_x, float region_y)
{
	ImGuiIO& io = ImGui::GetIO();
	float wheel = io.MouseWheel;
	window_flags_ |= ImGuiWindowFlags_NoMove;
	if (KEY_HOLD(KEY::CTRL))
	{
		zoom_ += wheel * 0.1f;
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		current_mouse_pos_ = Vec2(io.MousePos.x - region_size_.x / 2.f, io.MousePos.y - region_size_.x / 2.f);
		release_mouse_ = false;

	}
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		current_mouse_pos_ += Vec2(io.MouseDelta.x, io.MouseDelta.y);
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		selected_left_top_ = Vec2(region_x, region_y) / zoom_;
		//current_mouse_pos_ = Vec2(-1, -1);
		release_mouse_ = true;
	}
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		scrolling_ += Vec2(io.MouseDelta.x, io.MouseDelta.y);
	}
}

void AtlasTextureTool::TileMapMouseMode()
{
	ImGuiIO& io = ImGui::GetIO();
	current_mouse_pos_ = Vec2(io.MousePos.x, io.MousePos.y);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		CalculateIndex();
	}
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{

	}
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		scrolling_ += Vec2(io.MouseDelta.x, io.MouseDelta.y);
	}
}

