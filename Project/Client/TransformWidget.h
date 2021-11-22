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
};

