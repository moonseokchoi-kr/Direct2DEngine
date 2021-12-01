#pragma once
#include <Engine/CCamera.h>
class ToolCamera :
    public CCamera
{
public:
    ToolCamera();
    ~ToolCamera();
public:
    void FinalUpdate();

private:
    void Move();

private:
    float move_time_;
    float accumulated_time_;
};

