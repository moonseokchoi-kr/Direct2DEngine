#pragma once
#include "Widget.h"
#include <Engine/CTexture.h>

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
		is_open_ = true;
    }
    Ptr<CTexture> GetAtlas() { return atlas_; }
    Vec2 GetSelectedLeftTop() { return selected_left_top_; }
    Vec2 GetRegionSize() { return region_size_; }

    Vec2 GetBackBoardSize() { return back_board_size_; }
    void SetBackBoardSize(Vec2 backBorad) { back_board_size_ = backBorad; }
    bool IsRelease() { return release_mouse_; }
    bool IsOpen() { return is_open_; }
    void Clear();
private:
    void ShowMenuBar();
    void ShowAtlasVeiw();
private:
    bool is_open_;
    bool release_mouse_;
    ImGuiWindowFlags window_flags_;
    
    Ptr<CTexture> atlas_;
    Vec2 current_mouse_pos_;
    Vec2 selected_left_top_;
    Vec2 region_size_;
    Vec2 back_board_size_;

    float zoom_;

};

