#pragma once
#include "ComponentWidget.h"
class MeshRenderWidget :
    public ComponentWidget
{
public:
    MeshRenderWidget();
    ~MeshRenderWidget();

public:
    void Update() override;
    
};

