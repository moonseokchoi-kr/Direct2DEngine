#pragma once
#include "ComponentWidget.h"
#include "ObjectFilterWidget.h"
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
    void SelectEvnet(void* p, wstring& str);
private:
    void Start();
private:
    CScript* target_script_;
    float widget_height_;
    ObjectFilterWidget object_widget_;
};

