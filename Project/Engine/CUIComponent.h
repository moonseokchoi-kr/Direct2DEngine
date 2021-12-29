#pragma once
#include "CComponent.h"

class CanvasPanelWidget;
class GameWidget;

class CUIComponent :
    public CComponent
{
public:
    CUIComponent();
    ~CUIComponent();
  
public:
    void Update() override;
    void LateUpdate() override;
    void FinalUpdate() override;
    void UpdateData() override;
    void Render();
public:
    virtual void OnMouseHovered();
    virtual void OnMouseLeftButtonDown();
    virtual void OnMouseLeftButtonUp();
public:
    GameWidget* AddWidget(UI_TYPE type);
public:
    CanvasPanelWidget* GetWidget() { return canvas_widget_; }
private:
    void CheckMouseHovered();
    void ComputeTargetWidget();
public:
    CLONE(CUIComponent);

public:
    void SaveToScene(FILE* file);
    void LoadFromScene(FILE* file);
private:
    bool check_use_mouse_;
    bool is_hovered;
    bool is_mouse_l_down_;
    bool is_mouse_l_release_;
    bool is_select_;

    CanvasPanelWidget* canvas_widget_;
    GameWidget* target_widget_;
};

