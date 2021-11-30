#pragma once
#include "Widget.h"

struct OffsetData
{
    int index;
    AnimationFrame data;
};
class CAnimation2D;

class AnimationOffsetTool :
    public Widget
{
public:
    AnimationOffsetTool();
    ~AnimationOffsetTool();
public:
    void Init() override;
    void Update() override;

public:
    void SetAnimation(CAnimation2D* anim) { animation_ = anim; }
private:
    void ShowAnimationOffsetCanvasView();
    void ShowDetailView();
    void CalOffset(Vec2 frameBufferCenter, Vec2 canvasCenter, Vec2 canvas_lt);
    void SetAnimationData();
    bool SelectItemWithMouse(ImVec2 mouse_pos);
private:
    CAnimation2D* animation_;
    ImGuiWindowFlags window_flags_;

    AnimationFrame previous_frame_;
    AnimationFrame current_frame_;

    Vec2 origin_image_pos_;
    Vec2 offset_;
    vector<Vec2> drag_pos_vector_;

    float zoom_;
    int current_index_;
    bool select_item_;
};



