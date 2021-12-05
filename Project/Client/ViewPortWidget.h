#pragma once
#include "Widget.h"
class ViewPortWidget :
    public Widget
{
public:
    ViewPortWidget();
    ~ViewPortWidget();
public:
    void Init() override;
    void Update() override;
};

