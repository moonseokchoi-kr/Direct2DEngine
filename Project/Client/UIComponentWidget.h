#pragma once
#include "ComponentWidget.h"
#include "TreeWidget.h"

class UIComponentWidget :
    public ComponentWidget
{
public:
    UIComponentWidget();
    ~UIComponentWidget();
public:
    void Update() override;
private:
    TreeWidget ui_tree_;

};

