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
    void CalOffset(AnimationFrame& data);
    int SelectItemWithMouse(vector<AnimationFrame> anims);
private:
    CAnimation2D* animation_;
    ImGuiWindowFlags window_flags_;


    Vec2 origin_image_pos_;

    float zoom_;
};



