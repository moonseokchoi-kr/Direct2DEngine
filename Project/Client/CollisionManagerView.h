#pragma once
#include "Widget.h"
class CollisionManagerView :
    public Widget
{
public:
    CollisionManagerView();
    ~CollisionManagerView();
public:
    void Init() override;
    void Update() override;
};

