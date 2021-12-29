#pragma once
#include "ComponentWidget.h"
#include "ComboWidget.h"
#include "TreeWidget.h"

class ParticleWidget :
    public ComponentWidget
{
public:
    ParticleWidget();
    ~ParticleWidget();
public:
    void Update() override;

private:
	void ChangeMesh(DWORD_PTR instance, DWORD_PTR meshName);
	void ChangeMaterial(DWORD_PTR instance, DWORD_PTR materialName);
    void ChangeTexture(DWORD_PTR instance, DWORD_PTR textureName);
private:
    ComboWidget material_combo_;
    ComboWidget mesh_combo_;
    ComboWidget texture_combo_;
    TreeWidget data_tree_;
};

