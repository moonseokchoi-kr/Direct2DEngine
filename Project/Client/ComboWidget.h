#pragma once
#include "Widget.h"

typedef void (Widget::* COMBO_CALLBACK)(DWORD_PTR, DWORD_PTR);

class ComboWidget :
    public Widget
{
public:
    ComboWidget();
    ~ComboWidget();

public:
    void Init() override;
    void Update() override;

public:
    void SetCallback(Widget* instance, COMBO_CALLBACK callback) { instance_ = instance_; callback_ = callback }
    void AddComboData(const string& data) { combo_data_.push_back(data); }
    void Clear();
private:
    vector<string> combo_data_;
    string selected_item_;
    COMBO_CALLBACK callback_;
    Widget* instance_;
    int current_index_;
    bool is_selected;
};


