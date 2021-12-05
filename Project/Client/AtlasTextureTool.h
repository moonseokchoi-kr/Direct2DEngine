#pragma once
#include "Widget.h"
#include <Engine/CTexture.h>

enum class TOOL_TYPE
{
    ANIMATOR,
    TILEMAP,
};

struct TileInfo
{
    int col;
    int row;
    int index;
};

class AnimationTool;
class AtlasTextureTool :
    public Widget
{
public:
    AtlasTextureTool();
    ~AtlasTextureTool();
public:
    void Init() override;
    void Update() override;

public:
    void Activate() override
    {
		Widget::Activate();
    }
    Ptr<CTexture> GetAtlas() { return atlas_; }
    void SetAtlas(Ptr<CTexture> atlas) { atlas_ = atlas; }
    Vec2 GetSelectedLeftTop() { return selected_left_top_; }
    Vec2 GetRegionSize() { return region_size_; }

    Vec2 GetBackBoardSize() { return back_board_size_; }
    void SetBackBoardSize(Vec2 backBorad) { back_board_size_ = backBorad; }
    bool IsRelease() { return release_mouse_; }
    void Clear();

    void SetMode(TOOL_TYPE mode) { mode_ = mode; }
    int GetCurrentIndex() { return current_tile_index_; }
private:
    void ChangeTexture(DWORD_PTR instance, DWORD_PTR textureName);
    void ShowMenuBar();
    void ShowAnimatorMenu();
    void ShowTileMapMenu();
    void ShowAtlasVeiw();

    void CalculateIndex();
    void AnimationMouseMode(float region_x, float region_y);
    void TileMapMouseMode();
private:
    bool release_mouse_;
    ImGuiWindowFlags window_flags_;
    
    Ptr<CTexture> atlas_;
    Vec2 current_mouse_pos_;
    Vec2 selected_left_top_;
    Vec2 region_size_;
    Vec2 back_board_size_;
    Vec2 scrolling_;

	Vec2 canvas_lt_;
	Vec2 canvas_size_;


    TOOL_TYPE mode_;
    float zoom_;
    UINT current_tile_index_;

};

