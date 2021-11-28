#pragma once
#include "ComponentWidget.h"
class CameraWidget :
    public ComponentWidget
{
public:
    CameraWidget();
    ~CameraWidget();
public:
    void Init() override;
    void Update() override;

private:
    int current_index_;
};

