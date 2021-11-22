#pragma once
#include "Widget.h"

#include <Engine/CResource.h>

class CGameObject;
class ComponentWidget;

class InspectorWidget :
    public Widget
{
public:
    InspectorWidget();
    ~InspectorWidget();
public:
    // CWidget을(를) 통해 상속됨
    virtual void Update() override;
public:
    void SetGameObject(CGameObject* target) { target_object_ = target; }

private:
    void ShowObjectInfo();
private:
    CGameObject* target_object_;
    Ptr<CResource> target_resource_;
    array<ComponentWidget*, static_cast<UINT>(COMPONENT_TYPE::END)> component_widget_array_;
};

