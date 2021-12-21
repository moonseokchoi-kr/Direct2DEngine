#pragma once
#include "Widget.h"

typedef void (Widget::* SELECT_EVENT)(string selectName);


class FilterWidget :
    public Widget
{
public:
    FilterWidget();
    ~FilterWidget();

public:
    void Init() override;
    void Update() override;

public:
    void AddFilterString(const string& str) { filter_string_vector_.push_back(str); }
    void SetSelectEvent(Widget* instance, SELECT_EVENT evnet) { instance_ = instance; select_event_ = evnet; }
    void Clear();
private:
    void ShowSelectableList();
 
private:
    vector<string> filter_string_vector_;
    string selected_item_;
    Widget* instance_;
    SELECT_EVENT select_event_;
	int current_index_;
    bool is_popup_open_;
};

