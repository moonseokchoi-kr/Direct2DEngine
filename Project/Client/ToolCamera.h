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
public:
    Vec3 GetMoveDelta() { return camera_move_delta_; }
private:
    void Move();

private:
    float move_time_;
    float accumulated_time_;
    
    Vec3 camera_move_delta_;
};

