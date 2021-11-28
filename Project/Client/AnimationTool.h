#pragma once
#include "Widget.h"
#include <Engine/CTexture.h>

struct AnimationCreateData
{
    Vec2 left_top;
    Vec2 region_size;
    Vec2 offset;
    float duration;
};

class CAnimator2D;
class CAnimation2D;

class AtlasTextureTool;
class AnimationTool :
    public Widget
{
public:
    AnimationTool();
    ~AnimationTool();
public:
    void Init() override;
    void Update() override;
    
public:
    virtual void Activate() 
    {
        Widget::Activate();
        is_open_ = true;
    }
    void SetTargetAnimator(CAnimator2D* animator) { target_animator_ = animator; }
private:
    void ShowMenuBar();
    void ShowAnimationEditWidget();
    void ShowAnimationDetailSettingPanel();
    void ShowPlayButton();
private:
    bool is_open_;
    bool play_;
    bool menu_selected_;
    int current_index_;
    float zoom_;
    float acc_time_;
    
    string label_;
    ImGuiWindowFlags window_flags_;

    Vec2 left_top_;
    Vec2 region_size_;
    Vec2 current_mouse_pos_;
    Vec2 offset_;
    float duration_;
  
    CAnimator2D* target_animator_;
    CAnimation2D* animation_;
    AnimationFrame current_frame;
    AtlasTextureTool* atlas_tool_;
};

