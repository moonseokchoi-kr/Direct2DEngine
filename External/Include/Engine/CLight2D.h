#pragma once
#include "CComponent.h"
class CLight2D :
    public CComponent
{
public:
    CLight2D();
    ~CLight2D();

public:
    void SetLightType(LIGHT_TYPE type){ light_info_.light_type= type;}
    void SetLightDir(Vec3 direction) { light_info_.light_direction = direction; }
    void SetLightRange(float range) { light_info_.light_range = range; }
    void SetLightAngle(float angle) { light_info_.light_angle = angle; }
    void SetLightColor(Vec4 color) { light_info_.light_color.light_color = color; }
    void SetLightSpecular(Vec4 specular) { light_info_.light_color.light_specular = specular; }
    void SetLightAmbient(Vec4 ambient) { light_info_.light_color.light_ambient = ambient; }

    const LightInfo& GetLightInfo() { return light_info_; }

public:
    void FinalUpdate() override;
    void UpdateData() override;

    CLONE(CLight2D);
private:
    LightInfo light_info_;

    int light_index_;
};

