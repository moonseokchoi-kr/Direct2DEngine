#include "pch.h"
#include "TileMapTool.h"

#include "AtlasTextureTool.h"
#include "WidgetManager.h"


#include <Engine/CTileMap.h>



TileMapTool::TileMapTool()
	:Widget("tile_map_tool")
	,value_change_(false)
	,current_tile_index_(0)
{
}

TileMapTool::~TileMapTool()
{
}

void TileMapTool::Init()
{
	atlas_texture_tool_ =dynamic_cast<AtlasTextureTool*>( WidgetManager::GetInst()->FindWidget("atlas_editor"));
}

void TileMapTool::Update()
{
	if (ImGui::Begin("Canvas", &is_active_))
	{
		if (nullptr != tile_map_ && !value_change_)
		{
			tile_index_vector_ = tile_map_->GetTileIndexVector();
			value_change_ = true;
		}
			
		ShowDetail();
		ShowTileCanvas();
		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}

void TileMapTool::ShowTileCanvas()
{
	static ImVector<ImVec2> points;
	static bool opt_enable_grid = true;
	static bool opt_enable_context_menu = true;
	static bool adding_line = false;
	Vec2 tileSize = atlas_texture_tool_->GetRegionSize();
	Ptr<CTexture> atlas= atlas_texture_tool_->GetAtlas();

	ImGui::Checkbox("Enable grid", &opt_enable_grid);
	ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
	ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");
	ImGui::Text("Mouse Pos:(%.f,%.f),Index: %d", current_mouse_pos_.x, current_mouse_pos_.y, current_tile_index_);
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
	canvas_lt_ = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);      // ImDrawList API uses screen coordinates!
	canvas_size_ = Vec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);   // Resize canvas to what's available
	if (canvas_size_.x < 50.0f) canvas_size_.x = 50.0f;
	if (canvas_size_.y < 50.0f) canvas_size_.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_lt_.x + canvas_size_.x, canvas_lt_.y + canvas_size_.y);
	
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_lt_, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_lt_, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_size_, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_lt_.x + scrolling_.x, canvas_lt_.y + scrolling_.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	// Add first and second point
	if (is_hovered)
	{
		MouseEvent();
	}

	// Pan (we use a zero mouse threshold when there's no context menu)
	// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling_.x += io.MouseDelta.x;
		scrolling_.y += io.MouseDelta.y;
	}

	// Context menu (under default mouse threshold)
	ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	if (opt_enable_context_menu && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
		ImGui::OpenPopupOnItemClick("context");
	if (ImGui::BeginPopup("context"))
	{
		if (adding_line)
			points.resize(points.size() - 2);
		adding_line = false;
		if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
		if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
		ImGui::EndPopup();
	}

	if (nullptr != atlas)
	{
		float width = (float)atlas->GetWidth();
		float height = (float)atlas->GetHeight();
		Vec2 size_uv = Vec2(tileSize.x / width, tileSize.y / height);
		for (UINT i = 0; i <(UINT)tile_index_vector_.size(); ++i)
		{
			int index = tile_index_vector_[i].index;
			int x = (int)(1 / size_uv.x);
			int col = (int)(index % x);
			int row = (int)(index / x);
			Vec2 atlas_lt_uv = Vec2(tileSize.x * col / width, tileSize.y * row / height);
			col = i % (int)tile_map_size_.x;
			row = i / (int)tile_map_size_.x;
			draw_list->AddImage(atlas->GetShaderResourceView(), Vec2(canvas_lt_.x+scrolling_.x + tileSize.x * col, canvas_lt_.y+ scrolling_.y + tileSize.y * row),
				Vec2(canvas_lt_.x+scrolling_.x + tileSize.x * (col + 1), canvas_lt_.y+scrolling_.y + tileSize.y * (row + 1)), atlas_lt_uv, atlas_lt_uv + size_uv);
			if (!tile_index_vector_[i].moveable)
			{
				draw_list->AddRect(Vec2(canvas_lt_.x + scrolling_.x + tileSize.x * col, canvas_lt_.y + scrolling_.y + tileSize.y * row),
					Vec2(canvas_lt_.x + scrolling_.x + tileSize.x * (col + 1), canvas_lt_.y + scrolling_.y + tileSize.y * (row + 1)), IM_COL32(255, 0, 0, 255));
			}
		}
	}
	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_lt_, canvas_p1, true);

	if (opt_enable_grid)
	{
	
		for (float x = fmodf(scrolling_.x, tileSize.x); x <= tileSize.x*tile_map_size_.x; x += tileSize.x)
			draw_list->AddLine(ImVec2(canvas_lt_.x + x, canvas_lt_.y), ImVec2(canvas_lt_.x + x, canvas_lt_.y+tileSize.y * tile_map_size_.y), IM_COL32(52, 218, 30, 255));
		for (float y = fmodf(scrolling_.y, tileSize.y); y <= tileSize.y*tile_map_size_.y; y += tileSize.y)
			draw_list->AddLine(ImVec2(canvas_lt_.x, canvas_lt_.y+y), ImVec2(canvas_lt_.x+ tileSize.x * tile_map_size_.x, canvas_lt_.y+y), IM_COL32(52, 218, 30, 255));
	}

	draw_list->PopClipRect();
}

void TileMapTool::ShowDetail()
{
	ImGui::AlignTextToFramePadding();

	ImGui::Text("Texture");
	ImGui::SameLine(100);

	if (ImGui::Button("open editor"))
	{
		atlas_texture_tool_->SetMode(TOOL_TYPE::TILEMAP);
		atlas_texture_tool_->SetAtlas(tile_map_->GetAtlas());
		atlas_texture_tool_->Activate();
	}
	
	Vec2 tileSize = atlas_texture_tool_->GetRegionSize();
	tile_map_size_ = Vec2(tile_map_->GetTileMapColumnCount(), tile_map_->GetTileMapRowCount());
	if (ImGui::BeginTable("##detail", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextColumn();
		ImGui::Text("Tile Size");
		ImGui::TableNextColumn();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		ImGui::InputFloat("##tilesize_x", &tileSize.x, 0, 0, "%0.f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		ImGui::InputFloat("##tilesize_y", &tileSize.y, 0, 0, "%0.f", ImGuiInputTextFlags_ReadOnly);

		tile_map_->SetTileSize(tileSize);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		ImGui::Text("Tile Map Size");

		ImGui::TableNextColumn();

		ImGui::Text("Column");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		if (ImGui::InputFloat("##tilemapsize_x", &tile_map_size_.x, 0, 0, "%0.f"))
		{
			size_change_ = true;
		}
		ImGui::SameLine();
		ImGui::Text("Row");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		if (ImGui::InputFloat("##tilemapsize_y", &tile_map_size_.y, 0, 0, "%0.f"))
		{
			size_change_ = true;
		}

		if (size_change_)
		{
			tile_map_->SetTileMapSize((int)tile_map_size_.x, (int)tile_map_size_.y);
			tile_index_vector_.resize((size_t)(tile_map_size_.x * tile_map_size_.y));
			size_change_ = false;
		}
		
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		
		ImGui::Text("Move Setting Mode");

		ImGui::TableNextColumn();

		ImGui::Checkbox("##move_setting", &move_setting_);
		
		ImGui::EndTable();
	}
	if (ImGui::Button("Save"))
	{
		tile_map_->SetTileIndexVector(tile_index_vector_);
		tile_map_->SetAtlasTexture(atlas_texture_tool_->GetAtlas());
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear"))
	{
		tile_index_vector_.clear();
	}


}

void TileMapTool::CalTileIndex()
{
	Vec2 tileSize = atlas_texture_tool_->GetRegionSize();
	Vec2 mousePos = current_mouse_pos_;
	for (UINT i = 0; i < tile_map_size_.x; ++i)
		for (UINT j = 0; j < tile_map_size_.y; ++j)
		{
			if (tileSize.x * i < current_mouse_pos_.x && current_mouse_pos_.x < tileSize.x * (i + 1) &&
				+tileSize.y * j < current_mouse_pos_.y && current_mouse_pos_.y < tileSize.y * (j + 1))
			{
				current_tile_index_ = j * (int)tile_map_size_.x + i;
			}
		}
}

void TileMapTool::MouseEvent()
{
	ImGuiIO& io = ImGui::GetIO();
	current_mouse_pos_ = Vec2(io.MousePos.x - canvas_lt_.x-scrolling_.x, io.MousePos.y - canvas_lt_.y- scrolling_.y);
	CalTileIndex();
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		if (!move_setting_)
			tile_index_vector_[current_tile_index_].index = atlas_texture_tool_->GetCurrentIndex();
	}

	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		if (!move_setting_)
			tile_index_vector_[current_tile_index_].index = atlas_texture_tool_->GetCurrentIndex();
		else
			tile_index_vector_[current_tile_index_].moveable = tile_index_vector_[current_tile_index_].moveable ? false : true;
	}
}
