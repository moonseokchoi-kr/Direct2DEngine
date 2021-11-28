#pragma once
#include "Widget.h"
class HirarchyViewWidget :
    public Widget
{
public:
    HirarchyViewWidget();
    ~HirarchyViewWidget();
public:
    void Init() override;
    void Update() override;

};

