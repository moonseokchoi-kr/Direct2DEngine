#pragma once
#include "Widget.h"

#include <Engine/CResource.h>

class CGameObject;

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
};

