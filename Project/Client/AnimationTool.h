#pragma once
#include "Widget.h"
#include <Engine/CTexture.h>

class CAnimator2D;
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
private:
    void ShowMenuBar();
    void ShowAnimationEditWidget();
    void ShowTextureView();
private:
    bool is_open_;
    bool menu_selected_;
    int window_falgs_;
    float zoom_;
    string label_;
    ImGuiWindowFlags window_flags_;

    Vec2 left_top_;
    Vec2 region_size_;
    Vec2 current_mouse_pos_;
  
    Ptr<CTexture> atlas_;
    CAnimator2D* target_animator_;
    AnimationFrame temp_animation_;
};

