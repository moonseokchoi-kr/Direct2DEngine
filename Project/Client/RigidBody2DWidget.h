#pragma once
#include "ComponentWidget.h"
#include "ComboWidget.h"

class RigidBody2DWidget :
    public ComponentWidget
{
public:
    RigidBody2DWidget();
    ~RigidBody2DWidget();
public:
    void Update() override;
    
private:
    void ChangeType(DWORD_PTR instance, DWORD_PTR data);
private:
    ComboWidget rigid_combo_;
};

