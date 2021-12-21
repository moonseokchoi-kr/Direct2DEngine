#pragma once
#include "Widget.h"
#include "TreeWidget.h"

class CGameObject;

class HierarchyViewWidget :
    public Widget
{
public:
    HierarchyViewWidget();
    ~HierarchyViewWidget();
public:
    void Init() override;
    void Update() override;
    
public:
	void Renew();
	void KeyCheck();
    void ReleaseSelectNode() { object_tree_.ReleaseSelectNode(); }
private:
    void AddGameObject(Node* destItem, CGameObject* object);
    void ClickGameObject(DWORD_PTR, DWORD_PTR);
    void DropGameObject(DWORD_PTR, DWORD_PTR);
    void ShowRightPopUp();
private:
    TreeWidget object_tree_;
    
    bool is_popup_open_;
};

