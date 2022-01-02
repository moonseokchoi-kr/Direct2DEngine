#pragma once
#include <Engine/CScript.h>

class CGameObject;

class CCameraFollowScript :
    public CScript
{
public:
    CCameraFollowScript();
    ~CCameraFollowScript();
public:
    void Start() override;
    void Update() override;
public:
    void SaveToScene(FILE* file) override;
    void LoadFromScene(FILE* file) override;
public:
    CLONE(CCameraFollowScript);
private:
    wstring target_name_;
    CGameObject* target_;
};

