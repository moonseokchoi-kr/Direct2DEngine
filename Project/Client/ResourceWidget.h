#pragma once
#include "Widget.h"
#include <Engine/CResource.h>


class ResourceWidget :
    public Widget
{
public:
    ResourceWidget(const string& widgetName, Vec2 uiSize);
    ~ResourceWidget();

public:
    void Start();
    void End();

public:
    virtual void SetResource(Ptr<CResource> resource) = 0;

private:
    Vec2 size_;
};

