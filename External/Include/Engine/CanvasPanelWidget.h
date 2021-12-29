#pragma once
#include "GameWidget.h"
class CanvasPanelWidget :
    public GameWidget
{
public:
    CanvasPanelWidget();
    ~CanvasPanelWidget();
public:
    void Update() override;
    void Render() override;

public:
    CLONE(CanvasPanelWidget);

public:
    void SaveToScene(FILE* file);
    void LoadFromScene(FILE* file);
};

