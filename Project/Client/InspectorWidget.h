#pragma once
#include "Widget.h"

#include <Engine/CResource.h>

class CGameObject;
class ComponentWidget;
class ResourceWidget;
class ScriptWidget;


class InspectorWidget :
    public Widget
{
public:
    InspectorWidget();
    virtual ~InspectorWidget();
public:
    // CWidget을(를) 통해 상속됨
    virtual void Update() override;
public:
    void SetGameObject(CGameObject* target) { target_object_ = target; target_resource_ = nullptr; }
    void SetTargetResource(Ptr<CResource> target) { target_object_ = nullptr; target_resource_ = target; }
private:
    void ShowObjectInfo();
    void ShowResourceInfo();
    void ShowSciprtInfo();
private:
    CGameObject* target_object_;
    Ptr<CResource> target_resource_;
    array<ComponentWidget*, static_cast<UINT>(COMPONENT_TYPE::END)> component_widget_array_;
    array<ResourceWidget*, (UINT)(RESOURCE_TYPE::END)> resource_widget_array_;
    vector<ScriptWidget*> script_widget_vector_;
};

