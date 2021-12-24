#pragma once
#include "Widget.h"
#include <Engine/CTexture.h>


enum class ANIMATION_TOOL_TYPE
{
    CREATE,
    EDIT,
};

class CAnimator2D;
class CAnimation2D;

class AnimationOffsetTool;
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
    void SetCurrentAnimation(CAnimation2D* animation) { animation_ = animation; }
    void SetToolType(ANIMATION_TOOL_TYPE type) { type_ = type; }
private:
    void ShowMenuBar();
    void ShowAnimationEditWidget();
    void ShowAnimationDetailSettingPanel();
    void ShowPlayButton();
private:
    float CalMaxSizeY();
    void RenderAllFrame(ImDrawList* draw_list, Ptr<CTexture> atlas, ImVec2 canvas_center, float atlas_width, float atlas_height);
    void RenderFrame(ImDrawList* draw_list, Ptr<CTexture> atlas, ImVec2 canvas_center, float atlas_width, float atlas_height,AnimationFrame& data);
    AnimationFrame GetAnimationFrameData(UINT index);
private:

	CAnimator2D* target_animator_;
	CAnimation2D* animation_;
	AnimationFrame current_frame_;

	AtlasTextureTool* atlas_tool_;
	AnimationOffsetTool* offset_tool_;

	Vec2 left_top_;
	Vec2 region_size_;
	Vec2 current_mouse_pos_;
	Vec2 offset_;


    bool is_open_;
    bool play_;
    bool render_all_;
    bool is_update_;
    int current_index_;
    int previous_index_;
    float zoom_;
    float acc_time_;
    
    string label_;
    ImGuiWindowFlags window_flags_;

    ANIMATION_TOOL_TYPE type_;
    float duration_;

};

