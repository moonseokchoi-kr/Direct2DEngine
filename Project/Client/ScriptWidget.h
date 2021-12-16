#pragma once
#include "ComponentWidget.h"

class CScript;

class ScriptWidget :
    public ComponentWidget
{
public:
    ScriptWidget();
    ~ScriptWidget();
public:
    void Update() override;

public:
    void SetScript(CScript* script) { target_script_ = script; }

private:
    void Start();
private:
    CScript* target_script_;
    float widget_height_;
};

