#pragma once
#include "ResourceWidget.h"

#include <Engine/CMaterial.h>

class MaterialWidget :
    public ResourceWidget
{
public:
    MaterialWidget();
    ~MaterialWidget();

public:
    void Update() override;

public:
    void SetResource(Ptr<CResource> resource) override
    {
        target_resource_ = dynamic_cast<CMaterial*>(resource.Get());
    }
    
private:
    void ShowMaterialDetail();
    void ChangeShader(DWORD_PTR, DWORD_PTR);
    
private:
    Ptr<CMaterial> target_resource_;
    int current_shader_;
};

