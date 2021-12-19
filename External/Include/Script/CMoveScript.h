#pragma once
#include <Engine/CScript.h>
class CMoveScript :
    public CScript
{
public:
    CMoveScript();
    ~CMoveScript();
public:
    void Update() override;

public:
    CLONE(CMoveScript);

private:
    float move_speed_;
};

