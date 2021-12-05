#pragma once
#include "ComponentWidget.h"
class TileMapWidget :
    public ComponentWidget
{
public:
    TileMapWidget();
    ~TileMapWidget();
public:
    void Update() override;
};

