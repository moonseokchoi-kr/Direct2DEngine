#pragma once
#include "ComponentWidget.h"
class TransformWidget :
    public ComponentWidget
{
public:
    TransformWidget();
    ~TransformWidget();
public:
    virtual void Update();

private:
    Vec3 CreateControlPannel(const string& lable, Vec3 value);
};

