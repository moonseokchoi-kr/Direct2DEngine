#pragma once
#include "Widget.h"
#include "TreeWidget.h"

class CGameObject;

class HirarchyViewWidget :
    public Widget
{
public:
    HirarchyViewWidget();
    ~HirarchyViewWidget();
public:
    void Init() override;
    void Update() override;
    void Renew();

private:
    void AddGameObject(Node* destItem, CGameObject* object);
    void ClickGameObject(DWORD_PTR, DWORD_PTR);
private:
    TreeWidget object_tree_;
};

