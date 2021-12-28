#pragma once
#include "Widget.h"

class CGameObject;

class ViewPortWidget :
    public Widget
{
public:
    ViewPortWidget();
    ~ViewPortWidget();
public:
    void Init() override;
    void Update() override;
private:
    void ComputeMousePos();
    void ComputeTagetObject(Vec2 screenRatio);
    void MouseEvent();
private:
    bool check_window_focus;
    bool is_othro_;
    Vec2 current_mouse_pos_;
    CGameObject*  target_object_;
};

