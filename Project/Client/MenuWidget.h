#pragma once
#include "Widget.h"
class MenuWidget :
    public Widget
{
public:
    MenuWidget();
    ~MenuWidget();
public:
    virtual void Update();
private:
    void ShowFileMenu();
    void ShowSceneMenu();

private:
    bool is_select_;
    bool is_play_;
};

