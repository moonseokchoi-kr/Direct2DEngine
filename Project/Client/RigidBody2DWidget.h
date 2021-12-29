#pragma once
#include "ComponentWidget.h"
class RigidBody2DWidget :
    public ComponentWidget
{
public:
    RigidBody2DWidget();
    ~RigidBody2DWidget();
public:
    void Update() override;
    
};

