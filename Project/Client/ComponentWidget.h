#pragma once
#include "Widget.h"

class CGameObject;

class ComponentWidget :
    public Widget
{
public:
    ComponentWidget(const string& name, Vec2 size);
    virtual ~ComponentWidget();

public:
    void SetTarget(CGameObject* target)
    {
        target_object_ = target;
    }

    CGameObject* GetTarget() { return target_object_; }

protected:
    void Start();
    void End();

private:
    CGameObject* target_object_;
    Vec2    widget_size_;
};

