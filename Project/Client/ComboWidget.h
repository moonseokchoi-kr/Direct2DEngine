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
    void SetCallback(Widget* instance, COMBO_CALLBACK callback) { instance_ = instance; callback_ = callback; }
    void SetComboLabel(const string& label) { combo_label_ = label; }
    void SetSecondData(DWORD_PTR data) { second_data_ = data; }
    void SetCurrentIndex(int index) { current_index_ = index; }
    void AddComboData(const string& data) { combo_data_.push_back(data); }
    string GetSelectedItem() { return selected_item_; }
    void Clear();
private:
    vector<string> combo_data_;
    DWORD_PTR second_data_;

    string selected_item_;
    string combo_label_;
    COMBO_CALLBACK callback_;
    Widget* instance_;
    int current_index_;
    bool is_selected;
};


