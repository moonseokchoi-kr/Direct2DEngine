#pragma once
#include "Widget.h"

typedef void(Widget::* MODAL_LIST_CALLBACK)(DWORD_PTR, DWORD_PTR);

class ModalListWidget :
    public Widget
{
public:
	ModalListWidget();
	~ModalListWidget();
public:
    virtual void Init();
    virtual void Update();

public:
    void SetCaption(const string& _caption) { caption_ = _caption; }
    const string& GetCaption() { return caption_; }
    const string& GetSelectedName() { return selected_item_; }

    void Activate() override 
    {
        Widget::Activate();
        is_popup_open_ = false;
    }

    void AddItem(const char* name) 
    {
        item_name_vector_.push_back(name);
    }

    void SetCallbackFunc(Widget* instance, MODAL_LIST_CALLBACK callBack)
    {
        instance_ = instance;
        callbakc_func_ = callBack;
    }

    bool isOpen() { return is_popup_open_; }

    void SetCurrentIndex(int index) { current_index_ = index; }

private:
    void Clear();
    void ShowEditMenu();
    void ShowSelectedBox();
private:
    string caption_;
    string selected_item_;
    vector<string> item_name_vector_;

    MODAL_LIST_CALLBACK  callbakc_func_;
    
    Widget* instance_;

    int current_index_;
    bool is_popup_open_;
};

