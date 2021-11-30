#pragma once
#include <Engine/CCamera.h>
class ToolCamera :
    public CCamera
{
public:
    ToolCamera();
    ~ToolCamera();
public:
    void FinalUpdate() override;
};

