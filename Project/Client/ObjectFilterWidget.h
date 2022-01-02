#pragma once
#include "Widget.h"
#include "ComboFilterWidget.h"


class ObjectFilterWidget :
    public Widget
{
public:
    ObjectFilterWidget();
    ~ObjectFilterWidget();
public:
    void Update() override;
public:
    void SetTargetObjectName(const wstring& str) { target_object_name_ = WStringToString(str); }
    wstring GetSelectedName() { return StringToWString(filter_widget_.GetSelectItem()); }
private:
    ComboFilterWidget filter_widget_;
    string target_object_name_;
};

