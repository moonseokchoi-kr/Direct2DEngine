#pragma once
#include "ResourceWidget.h"
#include <Engine/CTexture.h>

class TextureWidget :
    public ResourceWidget
{
public:
    TextureWidget();
    ~TextureWidget();
public:
    void Update() override;
public:
    void SetResource(Ptr<CResource> texture) override
    {
        target_texture_ = dynamic_cast<CTexture*>(texture.Get());
    }
private:
    Ptr<CTexture> target_texture_;
};

