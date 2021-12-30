#pragma once
#include "ComponentWidget.h"
class Box2DColliderWidget :
    public ComponentWidget
{
public:
    Box2DColliderWidget();
    ~Box2DColliderWidget();
public:
    void Update() override;

};

