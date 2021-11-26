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

private:
    void ChangeMesh(DWORD_PTR instance, DWORD_PTR meshName);
    void ChangeMaterial(DWORD_PTR instance, DWORD_PTR materialName);
private:

};

